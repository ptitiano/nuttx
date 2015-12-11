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
 */

/*
 * @file    apps/ara/tsbc/tsbc_pmu.c
 * @brief   TSBCONF Power Management Unit (PMU) Library
 * @author  Patrick Titiano
 */

#include "tsbc_pmu.h"
#include "tsbc.h"
#include "tsb_pmu.h"
#include <stdio.h>
#include <errno.h>


#define DEBUG_TSBC_PMU
#ifdef DEBUG_TSBC_PMU
#define dprintf(fmt, ...) printf("%s: " fmt, __func__, ##__VA_ARGS__)
#else
#define dprintf(fmt, ...)
#endif


#ifdef CONFIG_TSB_CHIP_REV_ES2
static const struct tsbc_reg_info tsb_pmu_regs[] = {
    { TSB_PMU_ISO_EN,           "ISO_EN"},
    { TSB_PMU_ISO_FOR_IO_EN,    "ISO_FOR_IO_EN"},
    { TSB_PMU_RETFFSAVE,        "RETFFSAVE"},
    { TSB_PMU_RETFFRSTR,        "RETFFRSTR"},
    { TSB_PMU_RETSRAMRET,       "RETSRAMRET"},
    { TSB_PMU_RETSRAMCENCONT,   "RETSRAMCENCONT"},
    { TSB_PMU_RETSRAMCLKCONT,	"RETSRAMCLKCONT"},
    { TSB_PMU_STANDBYMODE,      "STANDBYMODE"},
    { TSB_PMU_BOOTRET_O,        "BOOTRET_O"},
    { TSB_PMU_PWROFFCNT,        "PWROFFCNT"},
    { TSB_PMU_PWRONCNT,         "PWRONCNT"},
};

#define TSBC_PMU_REGS_COUNT	11

#endif

#ifdef CONFIG_TSB_CHIP_REV_ES3
static const struct tsbc_reg_info tsb_pmu_regs[] = {
    { TSB_PMU_ISO_EN,           "ISO_EN"},
    { TSB_PMU_ISO_FOR_IO_EN,	"ISO_FOR_IO_EN"},
    { TSB_PMU_RETFFSAVE,        "RETFFSAVE"},
    { TSB_PMU_RETFFRSTR,        "RETFFRSTR"},
    { TSB_PMU_RETSRAMRET,       "RETSRAMRET"},
    { TSB_PMU_RETSRAMCENCONT,	"RETSRAMCENCONT"},
    { TSB_PMU_RETSRAMCLKCONT,	"RETSRAMCLKCONT"},
    { TSB_PMU_HB8CLK_EN,        "HB8CLK_EN"},
    { TSB_PMU_SYSCLKOFFN,       "SYSCLKOFFN"},
    { TSB_PMU_WAKEUPSRC,        "WAKEUPSRC"},
    { TSB_PMU_BOOTRET_O,        "BOOTRET_O"},
    { TSB_PMU_PWROFFCNT,        "PWROFFCNT"},
    { TSB_PMU_PWRONCNT,         "PWRONCNT"},
};

#define TSBC_PMU_REGS_COUNT	13
#endif

static int tsbc_pmu_dump(int op_argc, char **op_argv)
{
    return tsbc_dump_regs("PMU", tsb_pmu_regs, TSBC_PMU_REGS_COUNT);
}

static int tsbc_pmu_read(int op_argc, char **op_argv)
{
    return tsbc_read_reg("pmu", tsb_pmu_regs, TSBC_PMU_REGS_COUNT,
                         op_argc, op_argv);
}

static int tsbc_pmu_write(int op_argc, char **op_argv)
{
    return tsbc_write_reg("pmu", tsb_pmu_regs, TSBC_PMU_REGS_COUNT,
                         op_argc, op_argv);
}

static int tsbc_pmu_set(int op_argc, char **op_argv)
{
    return tsbc_setclear_reg("pmu",
                             tsb_pmu_regs, TSBC_PMU_REGS_COUNT,
                             op_argc, op_argv, 1);
}

static int tsbc_pmu_clear(int op_argc, char **op_argv)
{
    return tsbc_setclear_reg("pmu",
                             tsb_pmu_regs, TSBC_PMU_REGS_COUNT,
                             op_argc, op_argv, 0);
}


static tsbc_optr tsbc_pmu_ops[TSBC_OP_COUNT] = {
    tsbc_pmu_dump,
    tsbc_pmu_read,
    tsbc_pmu_write,
    tsbc_pmu_set,
    tsbc_pmu_clear,
    NULL,
};

int tsbc_pmu_register(void)
{
    return tsbc_register(TSB_PER_PMU, tsbc_pmu_invoke_op);
}


int tsbc_pmu_invoke_op(int op, int op_argc, char **op_argv)
{
    if (op >= TSBC_OP_COUNT) {
        eprintf("op (%d) >= TSBC_OP_COUNT (%d)\n", op, TSBC_OP_COUNT);
        return -EINVAL;
    }

    if (tsbc_pmu_ops[op]) {
        return tsbc_pmu_ops[op](op_argc, op_argv);
    } else {
        printf("Operation not available!\n");
        return -EOPNOTSUPP;
    }
}
