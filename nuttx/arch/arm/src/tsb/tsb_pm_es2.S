/**
 * Copyright (c) 2015 Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * @file    arch/arm/src/tsb/tsb_pm_es2.S
 * @brief   TSB ES2.0 Standby and Resume Procedures
 * @author  Patrick Titiano
 */

#include "chip.h"
#include "tsb_pmu.h"


#define TSB_SYSCTL_CLOCKGATING1        (SYSCTL_BASE + 0x0204)
#define TSB_SYSCTL_CLOCKENABLE1        (SYSCTL_BASE + 0x0304)


.syntax unified
.thumb
.section .asmpmsec, "ax"
.file "tsb_pm_es2.S"
.globl up_standby, asmpmsec_start, asmpmsec_sz


/**
 * @brief Put the device into Standby Mode.
 */
.align 4
asmpmsec_start:

.thumb_func
.type up_standby, function
up_standby:
    /* Save registers */
    push {r0-r12,lr}

    /* Save stack */
    ldr r1, =stackptrcpy
    str sp, [r1]

    /*
     * After exiting from retention mode, Cortex-M3 jumps to the address which
     * is specified by the value of byte offset 0x4 of the workram
     * (address 0x10000004) and normal mode starts.
     */
    /* Save reset handler */
    ldr r1, resetvector
    ldr r1, [r1]
    ldr r2, =resethdlrcpy
    str r1, [r2]
    /* Store resume address at workram address 0x10000004 */
    ldr r1, resetvector
    ldr r2, resume_addr
    str r2, [r1]

    /* Standby Shutoff sequence, from ES3 specs rev100 section 4.1.4.4 */
    /* (0) Clear BOOTRET flag */
    ldr r1, pmu_bootret_o
    ldr r2, pmu_bootret_o_val
    str r2, [r1]

    /* (1) After all data transition completes, CM3 starts sequence below. */
    /* (2) CM3 configures [PwrOffCnt] if needed. */
    /* (3) CM3 sets [RetSramCENCont].RetSramCENCont to 1. */
    ldr r1, pmu_ret_sram_cen_cont
    ldr r2, pmu_ret_sram_cen_cont_val
    str r2, [r1]

    /* (4) Wait 5 CPU clock cycles or more. */
    nop
    nop
    nop
    nop
    nop
    nop

    /* (5) Clear [RetSramClkCont].RetSramClkCont to 0. */
    ldr r1, pmu_ret_sram_clk_cont
    ldr r2, pmu_ret_sram_clk_cont_val
    str r2, [r1]

    /* (6) Wait 5 CPU clock cycles or mode. */
    nop
    nop
    nop
    nop
    nop
    nop

    /* (7) CM3 sets [RetSramRET].RetSramRET to 1. */
    ldr r1, pmu_ret_sram_ret
    ldr r2, pmu_ret_sram_ret_val
    str r2, [r1]

    /* (8) Enter UniPro into Hibern8 after step7. */
    /* (9) CM3 waits until [UNIPRO_CLK_EN].UNIPRO_SYSCLKOFF_N becomes 0. */
    /* (10) CM3 sets [ClockGating1].CG_unipro_SysClk to 1. */
    /*
     * FIXME:
     * Unipro hibernation process skipped. Gating Unipro clocks directly.
     * Done earlier in tsb_pmstandby().
     */

    /*
     * (11) Clear [HB8CLK_EN].HB8CLK_EN to 0.
     *
     * Not found in ES2.0 register map.
     */

    /* (12) CM3 waits 1.5usec or more. */
    ldr r3, delay1_5us_val
loop1_5us:
    cbz r3, loop1_5us_done
    sub r3, r3, #1
    b loop1_5us

    /* (13) CM3 sets [RetFFSave].RetFFSave to 1. */
loop1_5us_done:
    /* Skipped. Marked as "Not used in ES2" in documentation. */

    /* (14) CM3 waits 0.1us or more. 1 CPU clock cycle = 10.4ns (@96MHz) */
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    /*
     * (15) Clear [RetFFSave].RetFFSave to 0.
     *
     * Skipped. Marked as "Not used in ES2" in documentation.
     */

    /* (16) CM3 sets [Iso_for_io_en].Iso_for_io_en to 1. */
    ldr r1, pmu_iso_for_io_en
    ldr r2, pmu_iso_for_io_en_val
    str r2, [r1]

    /* (17) CM3 sets [Iso_en].Iso_en to 1. */
    ldr r1, pmu_iso_en
    ldr r2, pmu_iso_en_val
    str r2, [r1]

    /* (18) After step17, GPBridge (PMU) asserts PWROFF_N (=0). */

    /*
     *  (19) After step18, external power supply must be turned off within
     *  (50ns x [PwrOffCnt].PwrOffCnt) externally.
     *
     * loop until the power is off
     */
    b .

resume_point:
    /* 6. Unlock latch state of IO cells via Iso_for_io_en register */
    ldr r1, pmu_iso_for_io_en
    ldr r0, pmu_iso_for_io_en_val
    str r0, [r1]
    mov r2, #7

    /* 7. CLR BOOTRET = 0 */
    ldr r1, pmu_bootret_o
    ldr r0, pmu_bootret_o_val
    str r0, [r1]
    mov r2, #8

    /* Restore Stack */
    ldr sp, stackptrcpy

    /* Restore Reset Handler */
    ldr r1, resetvector
    ldr r2, resethdlrcpy
    str r2, [r1]

    /* Restore registers */
    pop {r0-r12, lr}
    bx lr

.align 4

stackptrcpy: /* Copy of the stack pointer, to be restored after standby boot */
    .word 0xDEADBEEF

delay1_5us_val: /* 1 loop = 3 CPU cycles, 1 CPU clock cycle = 10.4ns (@96MHz) */
    .word 50

pmu_bootret_o:
    .word TSB_PMU_BOOTRET_O
pmu_bootret_o_val:
    .word 0x00000001

pmu_ret_sram_cen_cont:
    .word TSB_PMU_RETSRAMCENCONT
pmu_ret_sram_cen_cont_val:
    .word 0x00000001

pmu_ret_sram_clk_cont:
    .word TSB_PMU_RETSRAMCLKCONT
pmu_ret_sram_clk_cont_val:
    .word 0x00000000

pmu_ret_sram_ret:
    .word TSB_PMU_RETSRAMRET
pmu_ret_sram_ret_val:
    .word 0x00000001

pmu_iso_for_io_en:
    .word TSB_PMU_ISO_FOR_IO_EN
pmu_iso_for_io_en_val:
    .word 0x00000001

pmu_iso_en:
    .word TSB_PMU_ISO_EN
pmu_iso_en_val:
    .word 0x00000001

resetvector:
    .word 0x10000004
resethdlrcpy: /* Copy of the reset handler, to be restored after standby boot */
    .word 0xDEADBEEF

resume_addr: /* Address in workram where code shall resume after standby boot */
    .word resume_point

asmpmsec_sz:
    .word . - asmpmsec_start

/*
.section .pmresumesec, "w"
.align 4
    .word 0x2BE3ABBA
    .word resume_point
*/
