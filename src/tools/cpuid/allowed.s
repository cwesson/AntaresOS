;;
; @file tools/cpuid/allowed.s
; Checks if CPUID is supported.
; @author Conlan Wesson
;;

GLOBAL cpuid_allowed

;;
; Checks if CPUID is supported.
; @return 1 if supported, 0 otherwise
;;
cpuid_allowed:
	pushfd
	pop     eax
	mov     ecx, eax
	xor     eax, 0x200000    ; Mask CPUID allowed bit.
	push    eax
	popfd
	pushfd
	pop     eax
	xor     eax, ecx    ; Mask changed bits.
	shr     eax, 21     ; Shift bit 21 to bit 0.
	and     eax, 1      ; Mask all other bits.
	ret
