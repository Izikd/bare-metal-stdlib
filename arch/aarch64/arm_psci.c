// SPDX-License-Identifier: MIT
/*
 * ARM PSCI
 *
 * Reference:
 * http://infocenter.arm.com/help/topic/com.arm.doc.den0022d/Power_State_Coordination_Interface_PDD_v1_1_DEN0022D.pdf
 */

#include "arm_psci.h"

static uint32_t arm_psci_call(uint32_t func, uint64_t p1, uint64_t p2)
{
	uint32_t res;

	asm volatile(
		"mov x0, %1\n"
		"mov x1, %2\n"
		"mov x2, %3\n"
		"hvc 0\n"
		"mov %0, x0\n"
		: "=r" (res)
		: "r" (func), "r" (p1), "r" (p2)
	);

	return res;
}

uint32_t arm_psci_version(void)
{
	return arm_psci_call(ARM_PSCI_VERSION, 0, 0);
}

void arm_psci_system_off(void)
{
	arm_psci_call(ARM_PSCI_SYSTEM_OFF, 0, 0);
}

void arm_psci_system_reset(void)
{
	arm_psci_call(ARM_PSCI_SYSTEM_RESET, 0, 0);
}
