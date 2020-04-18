/* SPDX-License-Identifier: MIT */

.text

.global _start
_start:
	/* Don't trap SIMD/NEON - allows usage of quad registers (q0, q1, etc.) */
	mov x1, #(0x3 << 20)
	msr CPACR_EL1, x1
	isb

	/* Stack setup */
	ldr x0, =stack_top
	mov sp, x0

	/* Extra pre-main setups that can be done in C code */
	bl board_init

	/* Call main(NULL, 0) - no arguments */
	mov x0, 0
	mov x1, 0
	bl main

	/* Poweroff system using PSCI */
	bl arm_psci_system_off

	/* Shouldn't get here if PSCI works properly */
	b hang

hang:
    wfi
    b hang
