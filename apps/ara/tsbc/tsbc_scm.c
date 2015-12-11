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
 * @file    apps/ara/tsbc/tsbc_scm.c
 * @brief   TSBCONF System Controller Module (SCM) Library
 * @author  Patrick Titiano
 */

#include <nuttx/arch.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "tsbc_scm_regs.h"
#include "tsbc_scm.h"
#include "tsb_scm.h"

static const char *productnames[] = {
	"Unknown",
	"APBridge",
	"GPBridge",
	"SWitch",
};


const char *tsbc_scm_get_product_name(void)
{
	return productnames[tsb_get_product_id()];
}

const char *tsbc_scm_get_product_rev(void)
{
	return NULL;
}

static int tsbc_scm_dump(int op_argc, char **op_argv)
{
    return tsbc_dump_regs("SCM", tsb_scm_regs, TSBC_SCM_REGS_COUNT);
}

static int tsbc_scm_read(int op_argc, char **op_argv)
{
    return tsbc_read_reg("scm", tsb_scm_regs, TSBC_SCM_REGS_COUNT,
                         op_argc, op_argv);
}

static int tsbc_scm_write(int op_argc, char **op_argv)
{
    return tsbc_write_reg("scm", tsb_scm_regs, TSBC_SCM_REGS_COUNT,
                          op_argc, op_argv);
}

static int tsbc_scm_set(int op_argc, char **op_argv)
{
    return tsbc_setclear_reg("scm",
                             tsb_scm_regs, TSBC_SCM_REGS_COUNT,
                             op_argc, op_argv, 1);
}

static int tsbc_scm_clear(int op_argc, char **op_argv)
{
    return tsbc_setclear_reg("scm",
                             tsb_scm_regs, TSBC_SCM_REGS_COUNT,
                             op_argc, op_argv, 0);
}


static int tsbc_scm_show(int op_argc, char **op_argv)
{
    if ((op_argc != 1) || (!op_argv)) {
        goto tsbc_scm_show_usage;
    }
    if (!op_argv[0]) {
        goto tsbc_scm_show_usage;
    }
    if (!strcasecmp(op_argv[0], "clk")) {
        tsb_clk_dump();
        return 0;
    } else {
        goto tsbc_scm_show_usage;
    }

tsbc_scm_show_usage:
    printf("Invalid argument!\n");
    printf("Usage: tsbc show scm clk\n\n");
    return -EINVAL;
}


static tsbc_optr tsbc_scm_ops[TSBC_OP_COUNT] = {
    tsbc_scm_dump,
    tsbc_scm_read,
    tsbc_scm_write,
    tsbc_scm_set,
    tsbc_scm_clear,
    tsbc_scm_show,
};

int tsbc_scm_register(void)
{
    return tsbc_register(TSB_PER_SCM, tsbc_scm_invoke_op);
}

int tsbc_scm_invoke_op(int op, int op_argc, char **op_argv)
{
    if (op >= TSBC_OP_COUNT) {
        eprintf("op (%d) >= TSBC_OP_COUNT (%d)\n", op, TSBC_OP_COUNT);
        return -EINVAL;
    }

    if (tsbc_scm_ops[op]) {
        return tsbc_scm_ops[op](op_argc, op_argv);
    } else {
        printf("Operation not available!\n");
        return -EOPNOTSUPP;
    }
}
