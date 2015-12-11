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
 * @file    apps/ara/tsbc/tsbc_app.c
 * @brief   TSB Unipro Bridge Configuration (TSBCONF) Application
 * @author  Patrick Titiano
 */

#include <nuttx/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include "tsbc.h"
#include "mem.h"
#include "tsbc_scm.h"
#include "tsbc_pmu.h"

/* #define DEBUG_TSBCAPP */
#ifdef DEBUG_TSBCAPP
#define dprintf(format, ...) printf("%s: " format, __func__, ##__VA_ARGS__)
#else
#define dprintf(format, ...)
#endif

static int _per;
static int _op;
static char **_op_argv;
static uint8_t _op_argc;

/**
 * @brief           Display application help message
 */
static void tsbc_usage(void)
{
    printf("Usage: tsbc <operation> [ip] <args>\n");
    printf("       tsbc <operation> <args>\n");
    printf("\nSupported operations:\n");
    printf("   read     Read IP register / memory content\n");
    printf("   write    Write into IP register / memory address\n");
    printf("   set      Set bit(s) of any IP register / memory address\n");
    printf("   clear    Clear bit(s) of any IP register / memory address\n");
    printf("   dump     Dump IP registers / memory address range\n");
    printf("   show     Display configuration/status information\n");
    printf("\nSupported IPs:\n");
    printf("   SCM      System Controller\n");
    printf("   PMU      Power Management Unit\n\n");
}

/**
 * @brief           Retrieve command-line user options.
 * @return          0 on success, -EINVAL otherwise
 * @param[in]       argc: user arguments count
 * @param[in]       argv: user arguments
 */
static int tsbc_get_user_options(int argc, char **argv)
{
    /*
     * As Nuttx apps are not really apps, (it is allocated statically),
     * make sure all static variables are initialized each time
     * the application is launched.
     */
    _op = -EINVAL;
    _per = -EINVAL;
    _op_argv = NULL;
    _op_argc = 0;

    /*
     * argv[1] = operation
     * argv[2] = peripheral
     * argv[3...] = peripheral op1...
     */
    #ifdef DEBUG_TSBCAPP
    int i;
    dprintf("argc=%d\n", argc);
    for (i = 0; i < argc; i++) {
        dprintf("argv[%d]=%s\n", i, argv[i]);
    }

    #endif
    if (argc < 3) {
        dprintf("argc < 3!\n");
        return -EINVAL;
    }

    _op = tsbc_get_op(argv[1]);
    _per = tsbc_get_per(argv[2]);
    if (_per < 0) {
        _op_argv = argv + 2;
        _op_argc = argc - 2;
    } else {
        _op_argv = argv + 3;
        _op_argc = argc - 3;
    }
    dprintf("per=%d op=%d\n", _per, _op);

    return 0;
}

static int tsbc_read_mem(int op_argc, char **op_argv)
{
    uint32_t addr;

    if ((op_argc != 1) || (!op_argv)) {
        goto tsbc_read_usage;
    }

    if (!op_argv[0]) {
        goto tsbc_read_usage;
    }
    /* Retrieve address from user arguments. */
    if (tsbc_s2hex(op_argv[0], &addr)) {
        goto tsbc_read_usage;
    }

    printf("%08X\n", mem_read32(addr));
    return 0;

tsbc_read_usage:
    printf("Invalid argument!\n");
    printf("Usage: tsbc read <address>\n");
    printf("       <address>: 32-bit aligned hex address, prefixed with '0x'.\n\n");
    return -EINVAL;
}


static int tsbc_write_mem(int op_argc, char **op_argv)
{
    uint32_t addr, val;

    if ((op_argc != 2) || (!op_argv)) {
        goto tsbc_write_usage;
    }

    if ((!op_argv[0]) || (!op_argv[1])) {
        goto tsbc_write_usage;
    }

    /* Retrieve address and value from user arguments. */
    if (tsbc_s2hex(op_argv[0], &addr)) {
        goto tsbc_write_usage;
    }
    if (tsbc_s2hex(op_argv[1], &val)) {
        goto tsbc_write_usage;
    }

    dprintf("writing 0x%08X @ 0x%08X\n", val, addr);
    return mem_write32(addr, val);

tsbc_write_usage:
    printf("Invalid argument!\n");
    printf("Usage: tsbc write <address> <value>\n");
    printf("       <address>: 32-bit aligned hex address, prefixed with '0x'.\n");
    printf("       <value>: 32-bit value, prefixed with '0x'.\n\n");
    return -EINVAL;
}

static int tsbc_setclear_mem(int op_argc, char **op_argv, unsigned char set)
{
    uint32_t addr, mask = 0;

    if ((op_argc < 2) || (!op_argv)) {
        goto tsbc_setclear_mem_usage;
    }

    if (!op_argv[0]) {
        goto tsbc_setclear_mem_usage;
    }

    /* Retrieve address from user arguments. */
    if (tsbc_s2hex(op_argv[0], &addr)) {
        goto tsbc_setclear_mem_usage;
    }

    /* Retrieve bit mask from user arguments. */
    if (tsbc_get_bitmask(op_argc - 1, op_argv + 1, &mask)) {
        goto tsbc_setclear_mem_usage;
    }

    if (set) {
        return tsbc_setclear_bits(addr, mask, set);
    } else {
        return tsbc_setclear_bits(addr, mask, set);
    }

tsbc_setclear_mem_usage:
    printf("Invalid argument!\n");
    if (set) {
        printf("Usage: tsbc set <address> <bit0> [bit1..bitN]\n");
    } else {
        printf("Usage: tsbc clear <address> <bit0> [bit1..bitN]\n");
    }
    printf("       <address>: 32-bit aligned hex address, prefixed with '0x'.\n");
    if (set) {
        printf("       <bit0>, [bit1..bitN]: bit position(s) to set (0 <= bit < 32).\n\n");
    } else {
        printf("       <bit0>, [bit1..bitN]: bit position(s) to clear (0 <= bit < 32).\n\n");
    }
    return -EINVAL;
}


static int tsbc_set_mem(int op_argc, char **op_argv)
{
    return tsbc_setclear_mem(op_argc, op_argv, 1);
}

static int tsbc_clear_mem(int op_argc, char **op_argv)
{
    return tsbc_setclear_mem(op_argc, op_argv, 0);
}


static tsbc_optr tsbc_ops[TSBC_OP_COUNT] = {
    tsbc_dump_mem,
    tsbc_read_mem,
    tsbc_write_mem,
    tsbc_set_mem,
    tsbc_clear_mem,
    NULL,
};


/**
 * @brief           Application main entry point.
 * @return          0 on success, standard error codes otherwise
 * @param[in]       argc: user arguments count
 * @param[in]       argv: user arguments
 */
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tsbc_main(int argc, char *argv[])
#endif
{
    int ret = 0;

    /* Retrieve user options */
    ret = tsbc_get_user_options(argc, argv);
    if ((ret) || (_op < 0)) {
        tsbc_usage();
        return -EINVAL;
    }

    tsbc_init();

    if (tsbc_scm_register()) {
        fprintf(stderr, "tsbc: failed to register SCM ops!\n\n");
    }
    if (tsbc_pmu_register()) {
        fprintf(stderr, "tsbc: failed to register PMU ops!\n\n");
    }

    if (_per >= 0) {
        ret = tsbc_invoke_op(_per, _op, _op_argc, _op_argv);
    } else {
        if (tsbc_ops[_op]) {
            ret = tsbc_ops[_op](_op_argc, _op_argv);
        } else {
            printf("Operation not available!\n");
            ret = -EOPNOTSUPP;
        }
    }

    return ret;
}
