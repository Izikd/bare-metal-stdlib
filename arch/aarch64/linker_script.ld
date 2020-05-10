/* SPDX-License-Identifier: MIT */

ENTRY(_start)

SECTIONS
{
	. = 0x40010000;
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }

	. = 0x4FE00000;

	/* Stack - 2MB */
	stack_base = .;
	. += 0x200000;
	stack_top = .;

	/* Heap - 64MB */
	heap_base = .;
	. += 0x4000000;
	heap_top = .;
}
