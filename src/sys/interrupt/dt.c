/**
 * @file interrupt/dt.c
 * Functions for setting up and using the descriptor tables.
 * @author Conlan Wesson
 */

#include "dt.h"

#include <kernel/ioport.h>
#include <kernel/msr.h>
#include <stdbool.h>
#include <stdint.h>
#include "isr.h"

#define PIC_INIT_COMMAND      0x11    //!< Command to initialize the PIC.
#define PIC_MASTER_IRQ_OFFSET IRQ0    //!< Offset to remap master PIC IRQs.
#define PIC_SLAVE_IRQ_OFFSET  PIC_SLAVE_IRQ_START    //!< Offset to remap slave PIC IRQs.
#define PIC_8086_MODE         0x01    //!< Use 8086/88 PIC mode.

#define GDT_NUM_ENTRIES   5    //!< Number of entries in the GDT.
#define IDT_NUM_ENTRIES 256    //!< Number of entries in the IDT.

#define APIC_BASE_MSR 0x1B     //!< APIC MSR number.
#define APIC_ENABLE   0x800    //!< APIC enable bit.

typedef enum {
	GDT_PRIV0 = 0x0,    //!< GDT ring 0 flag.
	GDT_PRIV1 = 0x1,    //!< GDT ring 1 flag.
	GDT_PRIV2 = 0x2,    //!< GDT ring 2 flag.
	GDT_PRIV3 = 0x3     //!< GDT ring 3 flag.
} gdt_priv;
typedef enum {
	GDT_EXEC   = 0x1,    //!< GDT executable flag.
	GDT_NOEXEC = 0x0     //!< GDT non-executable flag.
} gdt_exec;
typedef enum {
	GDT_DC_UP      = 0x0,    //!< Data segment grows up.
	GDT_DC_DOWN    = 0x1,    //!< Data segment grows down.
	GDT_DC_CONFORM = 0x1,    //!< Code segment can be executed from lower privilege.
	GDT_DC_BLOCK   = 0x0     //!< Code segment can only be executed from same privilege.
} gdt_dc;
typedef enum {
	GDT_RW = 0x1,    //!< GDT read/write flag.
	GDT_RO = 0x0     //!< GDT read only flag.
} gdt_rw;
typedef enum {
	GDT_GR_1B = 0x00,    //!< GDT granularity flag.
	GDT_GR_4K = 0x01     //!< GDT granularity flag.
} gdt_gran;
typedef enum {
	GDT_SZ_16 = 0x0,    //!< GDT Size flag.
	GDT_SZ_32 = 0x1     //!< GDT Size flag.
} gdt_size;

typedef enum {
	IDT_PRIV0 = 0x0,    //!< IDT ring 0 flag.
	IDT_PRIV1 = 0x1,    //!< IDT ring 1 flag.
	IDT_PRIV2 = 0x2,    //!< IDT ring 2 flag.
	IDT_PRIV3 = 0x3     //!< IDT ring 3 flag.
} idt_priv;
typedef enum {
	IDT_TASK   = 0x5,    //!< IDT task gate flag.
	IDT_INT16  = 0x6,    //!< IDT 16bit interrupt gate flag.
	IDT_TRAP16 = 0x7,    //!< IDT 16bit trap gate flag.
	IDT_INT32  = 0xE,    //!< IDT 32bit interrupt flag.
	IDT_TRAP32 = 0xF     //!< IDT 32bit trap gate flag.
} idt_type;
typedef enum {
	IDT_TI_GDT = 0x0,    //!< IDT selector GDT flag.
	IDT_TI_LDT = 0x1     //!< IDT selector LDT flag.
} idt_ti;

/**
 * Struct for Global Descriptor Table entries.
 */
struct gdt_entry{
	uint16_t limit_lo;      //!< The lower 16 bits of the limit.
	uint16_t base_lo;       //!< The lower 16 bits of the base.
	uint8_t  base_mid;      //!< The next 8 bits of the base.
	//! Access flags, determine what ring this segment can be used in.
	//struct {
		volatile const bool accessed:1;    //!< Accessed bit.
		uint8_t rw      :1;    //!< Read/write bit.
		uint8_t dc      :1;    //!< Direction/conforming bit.
		uint8_t exec    :1;    //!< Executable bit.
		const uint8_t __resv1 :1;    //!< Reserved.  Must be set to one.
		uint8_t privl   :2;    //!< Privilege bits.
		bool present    :1;    //!< Present bit.
	//};
	//! Additional flags and limit bits.
	//struct {
		uint8_t limit_hi:4;    //!< The upper 4 bits of the limit.
		const uint8_t __resv2 :2;    //!< Reserved. Must be set to zero.
		uint8_t size    :1;    //!< Size flag.
		uint8_t gran    :1;    //!< Granularity flag.
	//};
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
	//! Kernel segment selector.
	struct {
		uint16_t rpl   :2;    //!< Requested privilege level.
		uint16_t ti    :1;    //!< Table index.  GDT=0, LDT=1.
		uint16_t index:13;    //!< Index into GDT/LDT.
	};
	uint8_t  __resv;   //!< This must always be zero.
	//! Flags.
	struct {
		uint8_t type  :4;    //!< Gate type bits.
		uint8_t ss    :1;    //!< Storage segment.  Must be zero.
		uint8_t privl :2;    //!< Privilege bits.
		bool present  :1;    //!< Present flag.
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
static struct gdt_entry gdt_entries[] = {
	// Null segment
	{
		.base_lo = (0 & 0xFFFF),
		.base_mid = (0 >> 16) & 0xFF,
		.base_hi = (0 >> 24) & 0xFF,

		.limit_lo = (0 & 0xFFFF),
		.limit_hi = (0 >> 16) & 0x0F,

		.__resv2 = 0,    // lower 2 bits must be 0.
		.size = 0,
		.gran = 0,
		.accessed = 0,
		.rw = 0,
		.dc = 0,
		.exec = 0,
		.__resv1 = 1,    // must be set to 1.
		.privl = 0,
		.present = false
	},
	
	// Code segment
	{
		.base_lo = (0 & 0xFFFF),
		.base_mid = (0 >> 16) & 0xFF,
		.base_hi = (0 >> 24) & 0xFF,

		.limit_lo = (0xFFFFFFFF & 0xFFFF),
		.limit_hi = (0xFFFFFFFF >> 16) & 0x0F,

		.__resv2 = 0,    // lower 2 bits must be 0.
		.size = GDT_SZ_32,
		.gran = GDT_GR_4K,
		.accessed = 0,
		.rw = GDT_RW,
		.dc = GDT_DC_BLOCK,
		.exec = GDT_EXEC,
		.__resv1 = 1,    // must be set to 1.
		.privl = GDT_PRIV0,
		.present = true
	},
	
	// Data segment
	{
		.base_lo = (0 & 0xFFFF),
		.base_mid = (0 >> 16) & 0xFF,
		.base_hi = (0 >> 24) & 0xFF,

		.limit_lo = (0xFFFFFFFF & 0xFFFF),
		.limit_hi = (0xFFFFFFFF >> 16) & 0x0F,

		.__resv2 = 0,    // lower 2 bits must be 0.
		.size = GDT_SZ_32,
		.gran = GDT_GR_4K,
		.accessed = 0,
		.rw = GDT_RW,
		.dc = GDT_DC_DOWN,
		.exec = GDT_NOEXEC,
		.__resv1 = 1,    // must be set to 1.
		.privl = GDT_PRIV0,
		.present = true
	},
	
	// User mode code segment
	{
		.base_lo = (0 & 0xFFFF),
		.base_mid = (0 >> 16) & 0xFF,
		.base_hi = (0 >> 24) & 0xFF,

		.limit_lo = (0xFFFFFFFF & 0xFFFF),
		.limit_hi = (0xFFFFFFFF >> 16) & 0x0F,

		.__resv2 = 0,    // lower 2 bits must be 0.
		.size = GDT_SZ_32,
		.gran = GDT_GR_4K,
		.accessed = 0,
		.rw = GDT_RW,
		.dc = GDT_DC_BLOCK,
		.exec = GDT_EXEC,
		.__resv1 = 1,    // must be set to 1.
		.privl = GDT_PRIV3,
		.present = true
	},
	
	// User mode data segment
	{
		.base_lo = (0 & 0xFFFF),
		.base_mid = (0 >> 16) & 0xFF,
		.base_hi = (0 >> 24) & 0xFF,

		.limit_lo = (0xFFFFFFFF & 0xFFFF),
		.limit_hi = (0xFFFFFFFF >> 16) & 0x0F,

		.__resv2 = 0,    // lower 2 bits must be 0.
		.size = GDT_SZ_32,
		.gran = GDT_GR_4K,
		.accessed = 0,
		.rw = GDT_RW,
		.dc = GDT_DC_DOWN,
		.exec = GDT_NOEXEC,
		.__resv1 = 1,    // must be set to 1.
		.privl = GDT_PRIV3,
		.present = true
	},
};

//! Pointer to the GDT table.
static struct gdt_ptr gdtp = {
	.size = sizeof(gdt_entries) - 1,
	.base = (uint32_t)&gdt_entries
};

//! Array of the IDT entries.
static struct idt_entry idt_entries[IDT_NUM_ENTRIES];

//! Pointer to the IDT table.
static struct idt_ptr idtp = {
	.size = sizeof(idt_entries) - 1,
	.base = (uint32_t)&idt_entries
};

/**
 * Initilizes the GDT.
 */
inline static void gdt_init(){
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
static void idt_set_gate(uint8_t num, void (*handler)(), idt_priv rpl, uint16_t index, idt_type type, idt_priv privl){
	uint32_t base = (uint32_t)handler;
	idt_entries[num].base_lo = base & 0xFFFF;
	idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

	idt_entries[num].rpl = rpl;
	idt_entries[num].ti = IDT_TI_GDT;
	idt_entries[num].index = index;
	idt_entries[num].__resv = 0;    // must be set to 0.
	idt_entries[num].type = type;
	idt_entries[num].ss = 0;    // must be set to 0.
	idt_entries[num].privl = privl;
	idt_entries[num].present = true;
}

/**
 * Initializes the IDT.
 */
static void idt_init(){
	// Put the APIC into legacy mode.
	uint32_t msrhi, msrlo;
	rdmsr(APIC_BASE_MSR, &msrhi, &msrlo);
	wrmsr(APIC_BASE_MSR, msrhi, msrlo & ~APIC_ENABLE);
	
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
	
	uint8_t sel = IDT_PRIV0;
	uint16_t index = 1;

	// Register exception handlers
	idt_set_gate(0, isr0 , sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(1, isr1 , sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(2, isr2 , sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(3, isr3 , sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(4, isr4 , sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(5, isr5 , sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(6, isr6 , sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(7, isr7 , sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(8, isr8 , sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(9, isr9 , sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(10, isr10, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(11, isr11, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(12, isr12, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(13, isr13, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(14, isr14, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(15, isr15, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(16, isr16, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(17, isr17, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(18, isr18, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(19, isr19, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(20, isr20, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(21, isr21, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(22, isr22, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(23, isr23, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(24, isr24, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(25, isr25, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(26, isr26, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(27, isr27, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(28, isr28, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(29, isr29, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(30, isr30, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(31, isr31, sel, index, IDT_INT32, IDT_PRIV0);
	// Register hardware IRQs
	idt_set_gate(IRQ0, irq0, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ1, irq1, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ2, irq2, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ3, irq3, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ4, irq4, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ5, irq5, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ6, irq6, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ7, irq7, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ8, irq8, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ9, irq9, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ10, irq10, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ11, irq11, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ12, irq12, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ13, irq13, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ14, irq14, sel, index, IDT_INT32, IDT_PRIV0);
	idt_set_gate(IRQ15, irq15, sel, index, IDT_INT32, IDT_PRIV0);
	// Register system call interrupt
	idt_set_gate(ISR_SYSCALL, isr128, sel, index, IDT_INT32, IDT_PRIV0);

	idt_flush(&idtp);
}

/**
 * Initializes the descriptor tables.
 */
void descriptor_tables_init(){
	gdt_init();
	idt_init();
}

