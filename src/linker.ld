ENTRY (loader)

SECTIONS{
	. = 0x00100000;

	.__mbHeader : {
		*(.__mbHeader)
	}

	.text : {
		*(.text)
		*(.rodata*)
	}

	.data ALIGN (0x1000) : {
		*(.data)
	}

	.bss : {
		sbss = .;
		*(COMMON)
		*(.bss)
		ebss = .;
		kend = .;
	}
}
