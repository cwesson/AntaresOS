/**
 * @file interrupt/dt.c
 * Functions for setting up and using the descriptor tables.
 * @author Conlan Wesson
 */

#include "dt.h"

#include <kernel/ioport.h>
#include <kernel/msr.h>
#include <stdint.h>
#include "isr.h"

#define PIC_INIT_COMMAND      0x11    //!< Command to initialize the PIC.
#define PIC_MASTER_IRQ_OFFSET IRQ0    //!< Offset to remap master PIC IRQs.
#define PIC_SLAVE_IRQ_OFFSET  PIC_SLAVE_IRQ_START    //!< Offset to remap slave PIC IRQs.
#define PIC_8086_MODE         0x01    //!< Use 8086/88 PIC mode.

#define GDT_NUM_ENTRIES 5    //!< Number of entries in the GDT.
#define IDT_NUM_ENTRIES 256  //!< Number of entries in the IDT.

#define APIC_BASE_MSR 0x1B     //!< APIC MSR number.
#define APIC_ENABLE   0x800    //!< APIC enable bit.

#define GDT_ACCESS_PR    0x01    //!< GDT present flag.
#define GDT_ACCESS_PRIV0 0x00    //!< GDT ring 0 flag.
#define GDT_ACCESS_PRIV1 0x01    //!< GDT ring 1 flag.
#define GDT_ACCESS_PRIV2 0x02    //!< GDT ring 2 flag.
#define GDT_ACCESS_PRIV3 0x03    //!< GDT ring 3 flag.
#define GDT_ACCESS_EXEC  0x01    //!< GDT executable flag.
#define GDT_ACCESS_NOEXEC  0x00    //!< GDT non-executable flag.
#define GDT_ACCESS_DC_UP   0x00    //!< Data segment grows up.
#define GDT_ACCESS_DC_DOWN 0x01    //!< Data segment grows down.
#define GDT_ACCESS_DC_CONFORM 0x01    //!< Code segment can be executed from lower privilege.
#define GDT_ACCESS_DC_BLOCK   0x00    //!< Code segment can only be executed from same privilege.
#define GDT_ACCESS_RW    0x01    //!< GDT read/write flag.
#define GDT_ACCESS_RO    0x00    //!< GDT read only flag.
#define GDT_ACCESS_AC    0x01    //!< GDT accessed flag.
#define GDT_FLAG_GR_1B 0x00    //!< GDT granularity flag.
#define GDT_FLAG_GR_4K 0x01    //!< GDT granularity flag.
#define GDT_FLAG_SZ_16 0x00    //!< GDT Size flag.
#define GDT_FLAG_SZ_32 0x01    //!< GDT Size flag.

#define IDT_FLAG_PR     0x01    //!< IDT present flag.
#define IDT_FLAG_PRIV0  0x00    //!< IDT ring 0 flag.
#define IDT_FLAG_PRIV1  0x01    //!< IDT ring 1 flag.
#define IDT_FLAG_PRIV2  0x02    //!< IDT ring 2 flag.
#define IDT_FLAG_PRIV3  0x03    //!< IDT ring 3 flag.
#define IDT_FLAG_SS     0x01    //!< IDT storage segment flag.
#define IDT_FLAG_TASK   0x05    //!< IDT task gate flag.
#define IDT_FLAG_INT16  0x06    //!< IDT 16bit interrupt gate flag.
#define IDT_FLAG_TRAP16 0x07    //!< IDT 16bit trap gate flag.
#define IDT_FLAG_INT32  0x0E    //!< IDT 32bit interrupt flag.
#define IDT_FLAG_TRAP32 0x0F    //!< IDT 32bit trap gate flag.
#define IDT_SEL_PRIV0  0x00    //!< IDT selector ring 0 flag.
#define IDT_SEL_PRIV1  0x01    //!< IDT selector ring 1 flag.
#define IDT_SEL_PRIV2  0x02    //!< IDT selector ring 2 flag.
#define IDT_SEL_PRIV3  0x03    //!< IDT selector ring 3 flag.
#define IDT_SEL_TI_GDT 0x00    //!< IDT selector GDT flag.
#define IDT_SEL_TI_LDT 0x04    //!< IDT selector LDT flag.

/**
 * Struct for Global Descriptor Table entries.
 */
struct gdt_entry{
	uint16_t limit_lo;      //!< The lower 16 bits of the limit.
	uint16_t base_lo;       //!< The lower 16 bits of the base.
	uint8_t  base_mid;      //!< The next 8 bits of the base.
	//! Access flags, determine what ring this segment can be used in.
	struct {
		uint8_t  accessed:1;    //!< Accessed bit.
		uint8_t  rw      :1;    //!< Read/write bit.
		uint8_t  dc      :1;    //!< Direction/conforming bit.
		uint8_t  exec    :1;    //!< Executable bit.
		uint8_t  __resv1 :1;    //!< Reserved.  Must be set to one.
		uint8_t  privl   :2;    //!< Privilege bits.
		uint8_t  present :1;    //!< Present bit.
	};
	//! Additional flags and limit bits.
	struct {
		uint8_t  limit_hi:4;    //!< The upper 4 bits of the limit.
		uint8_t  __resv2 :2;    //!< Reserved. Must be set to zero.
		uint8_t  size    :1;    //!< Size flag.
		uint8_t  gran    :1;    //!< Granularity flag.
	};
	uint8_t  base_hi;       //!< The last 8 bits of the base.
} __attribute__((packed));

/**
 * Struct for Global Descriptor Table pointer.
 */
struct gdt_ptr{
	uint16_t size;   //!< Size of the GDT in bytes.
	uint32_t base;   //!< The address of the first gdt_entry struct.
} __attribute__((packed));

/**
 * Struct for Interrupt Descriptor Table entries.
 */
struct idt_entry{
	uint16_t base_lo;  //!< The lower 16 bits of the address to jump to when this interrupt fires.
	uint16_t sel;      //!< Kernel segment selector.
	uint8_t  __resv;   //!< This must always be zero.
	//! Flags.
	struct {
		uint8_t  type   :4;    //!< Gate type bits.
		uint8_t  ss     :1;    //!< Storage segment.  Must be zero.
		uint8_t  privl  :2;    //!< Privilege bits.
		uint8_t  present:1;    //!< Present flag.
	};
	uint16_t base_hi;  //!< The upper 16 bits of the address to jump to.
} __attribute__((packed));

/**
 * Struct for Interrupt Descriptor Table pointer.
 */
struct idt_ptr{
	uint16_t size;    //!< Size of the IDT in bytes.
	uint32_t base;    //!< The address of the first element in our idt_entry_t array.
} __attribute__((packed));

extern void gdt_flush(struct gdt_ptr*);  
extern void idt_flush(struct idt_ptr*);

// ASM based ISR handlers
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr128();  // System call interrupt

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

//! Array of the GDT entries.
static struct gdt_entry gdt_entries[GDT_NUM_ENTRIES];
//! Pointer to the GDT table.
static struct gdt_ptr gdtp;

//! Array of the IDT entries.
static struct idt_entry idt_entries[IDT_NUM_ENTRIES];
//! Pointer to the IDT table.
static struct idt_ptr idtp;

/**
 * Creates a GDT entry.
 * @param num The GDT entry number.
 * @param base The starting address of the new segment.
 * @param limit The highest address of the new segment.
 * @param rw The read/write flag.
 * @param dc The direction/conforming flag.
 * @param exec The executable flag.
 * @param privl The privilege flag.
 * @param present The present flag.
 * @param size The size flag.
 * @param gran The granularity flag.
 */
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t rw, uint8_t dc, uint8_t exec, uint8_t privl, uint8_t present, uint8_t size, uint8_t gran){
	gdt_entries[num].base_lo = (base & 0xFFFF);
	gdt_entries[num].base_mid = (base >> 16) & 0xFF;
	gdt_entries[num].base_hi = (base >> 24) & 0xFF;

	gdt_entries[num].limit_lo = (limit & 0xFFFF);
	gdt_entries[num].limit_hi = (limit >> 16) & 0x0F;

	gdt_entries[num].__resv2 = 0;    // lower 2 bits must be 0.
	gdt_entries[num].size = size;
	gdt_entries[num].gran = gran;
	gdt_entries[num].accessed = 0;
	gdt_entries[num].rw = rw;
	gdt_entries[num].dc = dc;
	gdt_entries[num].exec = exec;
	gdt_entries[num].__resv1 = 1;    // must be set to 1.
	gdt_entries[num].privl = privl;
	gdt_entries[num].present = present;
}

/**
 * Initilizes the GDT.
 */
static void gdt_init(){
	gdtp.size = (sizeof(struct gdt_entry) * GDT_NUM_ENTRIES) - 1;
	gdtp.base = (uint32_t)&gdt_entries;

	gdt_set_gate(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);    // Null segment
	gdt_set_gate(1, 0, 0xFFFFFFFF, GDT_ACCESS_RW, GDT_ACCESS_DC_BLOCK, GDT_ACCESS_EXEC,   GDT_ACCESS_PRIV0, GDT_ACCESS_PR, GDT_FLAG_SZ_32, GDT_FLAG_GR_4K); // Code segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, GDT_ACCESS_RW, GDT_ACCESS_DC_DOWN,  GDT_ACCESS_NOEXEC, GDT_ACCESS_PRIV0, GDT_ACCESS_PR, GDT_FLAG_SZ_32, GDT_FLAG_GR_4K); // Data segment
	gdt_set_gate(3, 0, 0xFFFFFFFF, GDT_ACCESS_RW, GDT_ACCESS_DC_BLOCK, GDT_ACCESS_EXEC,   GDT_ACCESS_PRIV3, GDT_ACCESS_PR, GDT_FLAG_SZ_32, GDT_FLAG_GR_4K); // User mode code segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, GDT_ACCESS_RW, GDT_ACCESS_DC_DOWN,  GDT_ACCESS_NOEXEC, GDT_ACCESS_PRIV3, GDT_ACCESS_PR, GDT_FLAG_SZ_32, GDT_FLAG_GR_4K); // User mode data segment

	gdt_flush(&gdtp);
}

/**
 * Creates an IDT entry.
 * @param num Interrupt number.
 * @param base Pointer to the corresponding ISR.
 * @param sel IDT selector flags.
 * @param index IDT entry index.
 * @param type IDT gate type.
 * @param privl The privilege flag.
 * @param present The present flag.
 */
static void idt_set_gate(uint8_t num, uint32_t base, uint8_t sel, uint16_t index, uint8_t type, uint8_t privl, uint8_t present){
	idt_entries[num].base_lo = base & 0xFFFF;
	idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

	idt_entries[num].sel = sel | (index << 3);
	idt_entries[num].__resv = 0;
	idt_entries[num].type = type;
	idt_entries[num].ss = 0;
	idt_entries[num].privl = privl;
	idt_entries[num].present = present;
}

/**
 * Initializes the IDT.
 */
static void idt_init(){
	// Put the APIC into legacy mode.
	uint32_t msrhi, msrlo;
	rdmsr(APIC_BASE_MSR, &msrhi, &msrlo);
	wrmsr(APIC_BASE_MSR, msrhi, msrlo & ~APIC_ENABLE);
	
	idtp.size = (sizeof(struct idt_entry) * IDT_NUM_ENTRIES) - 1;
	idtp.base = (uint32_t)&idt_entries;
	
	uint8_t master = inb(PIC_MASTER_DATA);
	uint8_t slave = inb(PIC_SLAVE_DATA);
	// Set PIC Commands
	outb(PIC_MASTER_CMD, PIC_INIT_COMMAND);
	outb(PIC_SLAVE_CMD, PIC_INIT_COMMAND);
	// Define Offset Vectors
	outb(PIC_MASTER_DATA, PIC_MASTER_IRQ_OFFSET);
	outb(PIC_SLAVE_DATA, PIC_SLAVE_IRQ_OFFSET);
	// Continue Initialize
	outb(PIC_MASTER_DATA, 0x04);
	outb(PIC_SLAVE_DATA, 0x02);
	outb(PIC_MASTER_DATA, PIC_8086_MODE);
	outb(PIC_SLAVE_DATA, PIC_8086_MODE);
	outb(PIC_MASTER_DATA, master);
	outb(PIC_SLAVE_DATA, slave);
	
	uint8_t sel = IDT_SEL_PRIV0 | IDT_SEL_TI_GDT;
	uint16_t index = 1;

	// Register exception handlers
	idt_set_gate(0, (uint32_t)isr0 , sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(1, (uint32_t)isr1 , sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(2, (uint32_t)isr2 , sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(3, (uint32_t)isr3 , sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(4, (uint32_t)isr4 , sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(5, (uint32_t)isr5 , sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(6, (uint32_t)isr6 , sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(7, (uint32_t)isr7 , sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(8, (uint32_t)isr8 , sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(9, (uint32_t)isr9 , sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(10, (uint32_t)isr10, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(11, (uint32_t)isr11, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(12, (uint32_t)isr12, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(13, (uint32_t)isr13, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(14, (uint32_t)isr14, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(15, (uint32_t)isr15, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(16, (uint32_t)isr16, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(17, (uint32_t)isr17, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(18, (uint32_t)isr18, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(19, (uint32_t)isr19, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(20, (uint32_t)isr20, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(21, (uint32_t)isr21, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(22, (uint32_t)isr22, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(23, (uint32_t)isr23, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(24, (uint32_t)isr24, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(25, (uint32_t)isr25, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(26, (uint32_t)isr26, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(27, (uint32_t)isr27, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(28, (uint32_t)isr28, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(29, (uint32_t)isr29, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(30, (uint32_t)isr30, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(31, (uint32_t)isr31, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	// Register hardware IRQs
	idt_set_gate(IRQ0, (uint32_t)irq0, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ1, (uint32_t)irq1, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ2, (uint32_t)irq2, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ3, (uint32_t)irq3, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ4, (uint32_t)irq4, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ5, (uint32_t)irq5, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ6, (uint32_t)irq6, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ7, (uint32_t)irq7, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ8, (uint32_t)irq8, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ9, (uint32_t)irq9, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ10, (uint32_t)irq10, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ11, (uint32_t)irq11, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ12, (uint32_t)irq12, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ13, (uint32_t)irq13, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ14, (uint32_t)irq14, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	idt_set_gate(IRQ15, (uint32_t)irq15, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);
	// Register system call interrupt
	idt_set_gate(ISR_SYSCALL, (uint32_t)isr128, sel, index, IDT_FLAG_INT32, IDT_FLAG_PRIV0, IDT_FLAG_PR);

	idt_flush(&idtp);
}

/**
 * Initializes the descriptor tables.
 */
void descriptor_tables_init(){
	gdt_init();
	idt_init();
}
