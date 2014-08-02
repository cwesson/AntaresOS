;;
; @file interrupt/isrs.s
; Common ISR stubs.
; @author Conlan Wesson
;;

GLOBAL gdt_flush
GLOBAL idt_flush
EXTERN isr_handler
EXTERN irq_handler

;;
; Macro for ISRs with no error code.
; @param %1 The interrupt code.
;;
%macro ISR_NOERRCODE 1
[GLOBAL isr%1]
isr%1:
	cli
	push  byte 0
	push  word %1            ; Push a word to surpress "signed byte value exceeds bounds" warning
	jmp   isr_common_stub
%endmacro

;;
; Macro for ISRs with an error code.
; @param %1 The interrupt code.
;;
%macro ISR_ERRCODE 1
[GLOBAL isr%1]
isr%1:
	cli
	push  byte %1
	jmp   isr_common_stub
%endmacro

;;
; Macro for IRQs.
; @param %1 The IRQ number.
; @param %2 The interrupt code.
;;
%macro IRQ 2
[GLOBAL irq%1]
irq%1:
	cli
	push  byte 0
	push  byte %2
	jmp   irq_common_stub
%endmacro

;;
; Flushes the Global Descriptor Table
;;
gdt_flush:
	mov   eax, [esp+4]    ; Get the pointer to the GDT, passed as a parameter.
	lgdt  [eax]           ; Load the new GDT pointer
	mov   ax, 0x10        ; 0x10 is the offset in the GDT to our data segment
	mov   ds, ax          ; Load all data segment selectors
	mov   es, ax
	mov   fs, ax
	mov   gs, ax
	mov   ss, ax
	ret

;;
; Flushes the Interrupt Descriptor Table
;;
idt_flush:
	mov   eax, [esp+4]    ; Get the pointer to the IDT, passed as a parameter.
	lidt  [eax]           ; Load the IDT pointer.
	ret

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
ISR_NOERRCODE 128

IRQ 0,  32
IRQ 1,  33
IRQ 2,  34
IRQ 3,  35
IRQ 4,  36
IRQ 5,  37
IRQ 6,  38
IRQ 7,  39
IRQ 8,  40
IRQ 9,  41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

;;
; Common handler for all ISRs.
;;
isr_common_stub:
	pusha             ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax.
	
	mov   ax, ds      ; Lower 16-bits of eax = ds.
	push  eax         ; Save the data segment descriptor.
	
	mov   ax, 0x10    ; Load the kernel data segment descriptor.
	mov   ds, ax
	mov   es, ax
	mov   fs, ax
	mov   gs, ax
	
	call  isr_handler
	
	pop   eax       ; Reload the original data segment descriptor.
	mov   ds, ax
	mov   es, ax
	mov   fs, ax
	mov   gs, ax
	
	popa            ; Pops edi,esi,ebp,esp,ebx,edx,ecx,eax.
	add   esp, 8    ; Cleans up the pushed error code and pushed ISR number.
	sti
	iret            ; Return from the ISR.

;;
; Common handler for all IRQs.
;;
irq_common_stub:
	pusha             ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax.
	
	mov   ax, ds      ; Lower 16-bits of eax = ds.
	push  eax         ; Save the data segment descriptor.
	
	mov   ax, 0x10    ; Load the kernel data segment descriptor.
	mov   ds, ax
	mov   es, ax
	mov   fs, ax
	mov   gs, ax
	
	call  irq_handler
	
	pop   ebx       ; Reload the original data segment descriptor.
	mov   ds, bx
	mov   es, bx
	mov   fs, bx
	mov   gs, bx
	
	popa            ; Pops edi,esi,ebp,esp,ebx,edx,ecx,eax.
	add   esp, 8    ; Cleans up the pushed error code and pushed ISR number.
	sti
	iret            ; Return from the ISR.

