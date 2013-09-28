;;
; @file loader.s
; Functions for booting the OS.
; @author Conlan Wesson
;;

GLOBAL loader    ; Entry point from GRUB.
EXTERN kmain     ; Main C function.

; Reserve initial kernel stack space.
STACKSIZE     equ 0x4000        ; 16KiB

; Set up Multiboot header.
MODULEALIGN   equ 0x00000001             ; Align loaded modules on page boundaries.
MEMINFO       equ 0x00000002             ; Provide memory map.
VIDEOMODE     equ 0x00000004             ; Provide video mode table.
ADDRESSFIELDS equ 0x00010000             ; Address fields are valid.
FLAGS         equ MEMINFO | VIDEOMODE    ; Multiboot flags
MAGIC         equ 0x1BADB002             ; Magic number lets bootloader find the header.
CHECKSUM      equ -(MAGIC + FLAGS)       ; Checksum required.

SECTION .__mbHeader
ALIGN 4
MultiBootHeader:
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

;;
; Main bootloader, called by GRUB.
; @param eax The multiboot magic number.
; @param ebx Pointer the the multiboot structure.
;;
SECTION .text
ALIGN 4
loader:
	mov   esp, stack+STACKSIZE    ; Set up the stack.
	push  eax    ; Pass Multiboot magic number.
	push  ebx    ; Pass Multiboot info structure.
	cli
	call  kmain    ; Call kernel.

;;
; Halts or hangs the CPU after kernel returns.
;;
hang:
	hlt
	jmp  hang


SECTION .bss
ALIGN 4
stack:
	resb  STACKSIZE    ; Reserve stack on a doubleword boundary.
