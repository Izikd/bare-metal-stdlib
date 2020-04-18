// SPDX-License-Identifier: MIT

#include "arm_uart.h"
#include "arm_psci.h"

#include "stdio.h"

void board_init(void)
{
	arm_uart_init(ARM_UART_BASE, 9600);
}
