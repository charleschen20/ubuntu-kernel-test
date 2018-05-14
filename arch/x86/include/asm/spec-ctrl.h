/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SPECCTRL_H_
#define _ASM_X86_SPECCTRL_H_

#include <asm/nospec-branch.h>

/*
 * On VMENTER we must preserve whatever view of the SPEC_CTRL MSR
 * the guest has, while on VMEXIT we restore the host view. This
 * would be easier if SPEC_CTRL were architecturally maskable or
 * shadowable for guests but this is not (currently) the case.
 * Takes the guest view of SPEC_CTRL MSR as a parameter.
 */
extern void x86_spec_ctrl_set_guest(u64);
extern void x86_spec_ctrl_restore_host(u64);

/*
 * On VMEXIT we must ensure that no RSB predictions learned in the guest
 * can be followed in the host, by overwriting the RSB completely. Both
 * retpoline and IBRS mitigations for Spectre v2 need this; only on future
 * CPUs with IBRS_ATT *might* it be avoided.
 */
static inline void vmexit_fill_RSB(void)
{
#ifdef CONFIG_RETPOLINE
	alternative_input("",
			  "call __fill_rsb",
			  X86_FEATURE_RETPOLINE,
			  ASM_NO_INPUT_CLOBBER(_ASM_BX, "memory"));
#endif
}

/* AMD specific Speculative Store Bypass MSR data */
extern u64 x86_amd_ls_cfg_base;
extern u64 x86_amd_ls_cfg_rds_mask;

#endif
