/****************************************************************************
 * arch/arm/src/stm32/stm32_pmstandby.c
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/*
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
 *
 * @file    arch/arm/src/tsb/tsb_pmstandby.c
 * @brief   Implement TSB STANDBY mode (Power Management)
 * @author  Patrick Titiano
 */

#include "tsb_pm.h"
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include "chip.h"
#include "nvic.h"
#include "up_arch.h"
#include "tsb_scm.h"
 #include <stdio.h>


int tsb_pmstandby(void)
{
#ifdef CONFIG_PM
#if defined(CONFIG_ARCH_CHIP_GPBRIDGE) && defined(CONFIG_TSB_CHIP_REV_ES2)
    extern uint32_t asmpmsec_start, asmpmsec_sz;
    int (* up_standby_bufram)(void) = (void *) (BUFRAM3_BASE | 0x00000001);
    uint32_t regval;
    irqstate_t flags;

    /* (8) Enter UniPro into Hibern8 after step7. */
    /* [HACK][FIXME] Reset Unipro IP to make sure it is not causing problem */
    tsb_reset(TSB_RST_UNIPROSYS);

    /*
     * (9) CM3 waits until [UNIPRO_CLK_EN].UNIPRO_SYSCLKOFF_N becomes 0.
     *
     * SKIPPED, UNIPRO_CLK_EN register does not exist on ES2.
     */

    /* (10) CM3 sets [ClockGating1].CG_unipro_SysClk to 1. */
    tsb_clk_disable(TSB_CLK_UNIPROSYS);

    /* Disable interrupts (procedure cannot be interrupted/pre-empted)*/
    flags = irqsave();

    /* Relocate vector table (eg from bootrom) */
//    putreg32((uint32_t *) &spiromvector, NVIC_VECTAB);
//    putreg32(0, NVIC_VECTAB);

    /*
     * Standby procedure must be copied to BufRam (BufRam not maintained)
     *
     * FIXME:
     *   Using BUFRAM3_BASE as location for the standby assembly code
     *   because USB is not used on GPB by default.
     *   A BUFRAM memory allocator is under development.
     *   When available in mainline, it shall be used or configured to reserve
     * a dedicated location for the power management code.
     */
    memcpy((void *) BUFRAM3_BASE, &asmpmsec_start, asmpmsec_sz);

    /* Set SLEEPDEEP bit of Cortex System Control Register */
    regval  = getreg32(NVIC_SYSCON);
    regval |= NVIC_SYSCON_SLEEPDEEP;
    putreg32(regval, NVIC_SYSCON);

    /* Execute Standby procedure */
    up_standby_bufram();


    while(1);

    irqrestore(flags);
    return OK;

#endif
    return -ENOSYS;
#endif

    return OK;
}
