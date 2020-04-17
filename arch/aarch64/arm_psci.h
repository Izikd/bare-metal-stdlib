/* SPDX-License-Identifier: MIT */
/*
 * ARM PSCI (Power State Coordination Interface)
 *
 * Reference:
 * http://infocenter.arm.com/help/topic/com.arm.doc.den0022d/Power_State_Coordination_Interface_PDD_v1_1_DEN0022D.pdf
 */

#ifndef __ARM_PSCI__
#define __ARM_PSCI__

#ifndef __ASSEMBLY__
#include <inttypes.h>
#endif

#define ARM_PSCI_VERSION	0x84000000
#define ARM_PSCI_SYSTEM_OFF	0x84000008
#define ARM_PSCI_SYSTEM_RESET	0x84000009

#ifndef __ASSEMBLY__

/**
 * PSCI version
 *
 * @return
 *	Bits[31:16] Major Version
 *	Bits[15:0] Minor Version
 */
uint32_t arm_psci_version(void);

/**
 * PSCI shutdown the system
 *
 * @return
 *	Shouldn't return
 */
void arm_psci_system_off(void);
/**
 * PSCI reset the system

 * @return
 *	Shouldn't return
 */
void arm_psci_system_reset(void);

#endif

#endif
