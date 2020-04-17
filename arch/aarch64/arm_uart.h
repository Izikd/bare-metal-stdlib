/* SPDX-License-Identifier: MIT */
/*
 * ARM UART PL011
 *
 * Reference:
 * http://infocenter.arm.com/help/topic/com.arm.doc.ddi0183g/DDI0183G_uart_pl011_r1p5_trm.pdf
 */

#ifndef __ARM_UART__
#define __ARM_UART__

#ifndef __ASSEMBLY__
#include <stddef.h>
#include <inttypes.h>
#endif

/* UART base address on QEMU virt machine */
#define ARM_UART_BASE		0x09000000

/* Data Register */
#define ARM_UART_DR		0x0
#define ARM_UART_DR_DATA	0xFF

/* Flag Register */
#define ARM_UART_FR		0x18
#define ARM_UART_FR_BUSY	(1 << 3)
#define ARM_UART_FR_RXFE	(1 << 4)
#define ARM_UART_FR_TXFF	(1 << 5)
#define ARM_UART_FR_RXFF	(1 << 6)
#define ARM_UART_FR_TXFE	(1 << 7)

/* Integer Baud Rate Register */
#define ARM_UART_IBRD		0x24

/* Line Control Register */
#define ARM_UART_LCR_H		0x2c
#define ARM_UART_LCR_H_FEN	(1 << 4)

/* Control Register */
#define ARM_UART_CR		0x30
#define ARM_UART_CR_UARTEN	(1 << 0)
#define ARM_UART_CR_SIREN	(1 << 1)
#define ARM_UART_CR_SIRLP	(1 << 2)
#define ARM_UART_CR_LBE		(1 << 7)
#define ARM_UART_CR_TXE		(1 << 8)
#define ARM_UART_CR_RXE		(1 << 9)
#define ARM_UART_CR_DTR		(1 << 10)
#define ARM_UART_CR_RTS		(1 << 11)
#define ARM_UART_CR_OUT1	(1 << 12)
#define ARM_UART_CR_OUT2	(1 << 13)
#define ARM_UART_CR_RTSEN	(1 << 14)
#define ARM_UART_CR_CTSEN	(1 << 15)

#ifndef __ASSEMBLY__

/**
 * UART initialization
 *
 * Will enable TX and RX using FIFO mode
 *
 * NOTICE: Tested on QEMU virt machine which is very permissive and
 *	   might not require all actual needed configuration as real HW unit.
 *
 * @param base_addr
 *	UART base address
 * @param baud
 *	UART baud rate (i.e. 9600)
 *
 * @return
 *	None
 */
void arm_uart_init(uintptr_t base_addr, unsigned int baud);

/**
 * UART put char
 *
 * @param base_addr
 *	UART base address
 * @param c
 *	Character to print
 *
 * @return
 *	None
 */
void arm_uart_putc(uintptr_t base_addr, char c);

/**
 * UART put string up to provided size
 *
 * @param base_addr
 *	UART base address
 * @param s
 *	String to print
 * @param len
 *	Maximal string length to print
 *
 * @return
 *	None
 */
void arm_uart_nputs(uintptr_t base_addr, const char *s, size_t len);

/**
 * UART put string (not safe)
 *
 * @param base_addr
 *	UART base address
 * @param s
 *	String to print
 *
 * @return
 *	None
 */
void arm_uart_puts(uintptr_t base_addr, const char *s);

/**
 * UART wait and get char
 *
 * @param base_addr
 *	UART base address
 *
 * @return
 *	Character from user
 */
char arm_uart_getc(uintptr_t base_addr);

#endif

#endif
