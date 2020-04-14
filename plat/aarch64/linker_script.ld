/* SPDX-License-Identifier: MIT */

SECTIONS
{
	. = 0x40010000;
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}
