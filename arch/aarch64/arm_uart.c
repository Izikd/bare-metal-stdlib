// SPDX-License-Identifier: MIT
/*
 * ARM UART PL011
 *
 * Reference:
 * http://infocenter.arm.com/help/topic/com.arm.doc.ddi0183g/DDI0183G_uart_pl011_r1p5_trm.pdf
 */

#include "arm_uart.h"

static volatile uint32_t *arm_uart_reg(uintptr_t base_addr, uintptr_t reg_addr)
{
	return (volatile uint32_t *)(base_addr + reg_addr);
}

void arm_uart_init(uintptr_t base_addr, unsigned int baud)
{
	uint32_t val;

	/* Disable */
	val = *arm_uart_reg(base_addr, ARM_UART_LCR_H);
	val &= ~ARM_UART_LCR_H_FEN;
	*arm_uart_reg(base_addr, ARM_UART_LCR_H) = val;

	val = *arm_uart_reg(base_addr, ARM_UART_CR);
	val &= ~ARM_UART_CR_UARTEN;
	*arm_uart_reg(base_addr, ARM_UART_CR) = val;

	/* Configure */
	/*
	 * FIXME: Configure baud properly
	 * QEMU doesn't care about baud rate register, so it will work there.
	 */
	val = baud;
	*arm_uart_reg(base_addr, ARM_UART_IBRD) = val;

	val = *arm_uart_reg(base_addr, ARM_UART_CR);
	val |= ARM_UART_CR_TXE;
	val |= ARM_UART_CR_RXE;
	val |= ARM_UART_CR_RTSEN;
	val |= ARM_UART_CR_CTSEN;
	*arm_uart_reg(base_addr, ARM_UART_CR) = val;

	/* Enable */
	val = *arm_uart_reg(base_addr, ARM_UART_LCR_H);
	val &= ~ARM_UART_LCR_H_FEN;
	*arm_uart_reg(base_addr, ARM_UART_LCR_H) = val;

	val = *arm_uart_reg(base_addr, ARM_UART_CR);
	val &= ~ARM_UART_CR_UARTEN;
	*arm_uart_reg(base_addr, ARM_UART_CR) = val;
}

void arm_uart_putc(uintptr_t base_addr, char c)
{
	while (*arm_uart_reg(base_addr, ARM_UART_FR) & ARM_UART_FR_TXFF)
		; /* yield */

	*arm_uart_reg(base_addr, ARM_UART_DR) = c;
}

void arm_uart_nputs(uintptr_t base_addr, const char *s, size_t len)
{
	while (*s && len) {
		arm_uart_putc(base_addr, *s);

		s++;
		len--;
	}
}

void arm_uart_puts(uintptr_t base_addr, const char *s)
{
	while (*s) {
		arm_uart_putc(base_addr, *s);

		s++;
	}
}

char arm_uart_getc(uintptr_t base_addr)
{
	while (*arm_uart_reg(base_addr, ARM_UART_FR) & ARM_UART_FR_RXFE)
		; /* yield */

	return (char)(*arm_uart_reg(base_addr, ARM_UART_DR) & ARM_UART_DR_DATA);
}
