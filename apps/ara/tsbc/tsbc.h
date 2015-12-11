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
 * @file    apps/ara/tsbc/tsbc.h
 * @brief   TSBCONF Library
 * @author  Patrick Titiano
 */

#ifndef __TSBC_H__
#define __TSBC_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define eprintf(fmt, ...) fprintf(stderr, "%s: " fmt, __func__, ##__VA_ARGS__)

enum tsbc_peripheral_id {
    TSB_PER_SCM = 0,
    TSB_PER_PMU = 1,
    TSB_PER_I2C = 2,
    TSB_PER_TMR = 3,
    TSB_PER_GPIO = 4,
    TSB_PER_PWM = 5,
    TSB_PER_UART = 6,
    TSB_PER_MBX = 7,
    TSB_PER_I2S = 8,
    TSB_PER_CDSI = 9,
    TSB_PER_DMA = 10,
    TSB_PER_SPI = 11,
    TSB_PER_SDIO = 12,
    TSB_PER_UNIPRO = 13,
    TSB_PER_HSIC = 14,
    TSB_PER_COUNT = 15,
};

struct tsbc_peripheral_info {
    int per;
    const char name[32];
};

struct tsbc_reg_info {
    uint32_t    addr;
    const char  name[32];
};

enum tsbc_op {
    TSBC_OP_DUMP = 0,
    TSBC_OP_READ = 1,
    TSBC_OP_WRITE = 2,
    TSBC_OP_SET = 3,
    TSBC_OP_CLEAR = 4,
    TSBC_OP_SHOW = 5,
    TSBC_OP_COUNT = 6,
};

struct tsbc_op_info {
    int op;
    const char name[32];
};

typedef int (*tsbc_optr)(int argc, char **argv);
typedef int (*tsbc_inv_optr)(int op, int argc, char **argv);

void tsbc_init(void);
int tsbc_register(int per, tsbc_inv_optr inv_op);


int tsbc_s2hex(char *s, unsigned int *hex);

int tsbc_get_per(char *name);
const char *tsbc_get_per_name(int per);
int tsbc_get_op(char *name);
const char *tsbc_get_op_name(int op);

int tsbc_get_bitmask(int op_argc, char **op_argv, uint32_t *mask);

int tsbc_dump_mem(int op_argc, char **op_argv);
int tsbc_setclear_bits(uint32_t addr, uint32_t mask, unsigned char set);

int tsbc_invoke_op(int per, int op, int op_argc, char **op_argv);
int tsbc_dump_regs(const char *ip, const struct tsbc_reg_info regs[],
                   unsigned int count);
int tsbc_find_addr(const struct tsbc_reg_info regs[],
                   unsigned int count, const char *name,
                   unsigned int *addr);
int tsbc_read_reg(const char *ip,
                  const struct tsbc_reg_info regs[], unsigned int count,
                  int op_argc, char **op_argv);
int tsbc_write_reg(const char *ip,
                   const struct tsbc_reg_info regs[], unsigned int count,
                   int op_argc, char **op_argv);
int tsbc_setclear_reg(const char *ip,
                      const struct tsbc_reg_info regs[], unsigned int count,
                      int op_argc, char **op_argv, unsigned char set);


#endif
