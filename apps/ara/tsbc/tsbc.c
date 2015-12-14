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

#include "tsbc.h"
#include <string.h>
#include <errno.h>
#include "mem.h"


/* #define DEBUG_TSBC */
#ifdef DEBUG_TSBC
#define dprintf(fmt, ...) printf("%s: " fmt, __func__, ##__VA_ARGS__)
#else
#define dprintf(fmt, ...)
#endif


static const struct tsbc_peripheral_info tsb_pinfo[] = {
    { TSB_PER_SCM,      "scm"},
    { TSB_PER_PMU,      "pmu"},
    { TSB_PER_I2C,      "i2c"},
    { TSB_PER_TMR,      "mr"},
    { TSB_PER_GPIO,     "gpio"},
    { TSB_PER_PWM,      "pwm"},
    { TSB_PER_UART,     "uart"},
    { TSB_PER_MBX,      "mbx"},
    { TSB_PER_I2S,      "i2s"},
    { TSB_PER_CDSI,     "cdsi"},
    { TSB_PER_DMA,      "dma"},
    { TSB_PER_SPI,      "spi"},
    { TSB_PER_SDIO,     "sdio"},
    { TSB_PER_UNIPRO,   "unipro"},
    { TSB_PER_HSIC,     "hsic"},
};


static const struct tsbc_op_info tsb_ops[] = {
    { TSBC_OP_DUMP,     "dump"},
    { TSBC_OP_READ,     "read"},
    { TSBC_OP_WRITE,    "write"},
    { TSBC_OP_SET,      "set"},
    { TSBC_OP_CLEAR,    "clear"},
    { TSBC_OP_SHOW,     "show"},
};


static tsbc_inv_optr tsbc_pinvops[TSB_PER_COUNT];


void tsbc_init(void)
{
    /* Placeholder */
    dprintf("init done.\n");
}


int tsbc_register(int per, tsbc_inv_optr inv_op)
{
    if (per >= TSB_PER_COUNT) {
        eprintf("per (%d) >= TSB_PER_COUNT! (%d)\n", per, TSB_PER_COUNT);
        return -EINVAL;
    }

    tsbc_pinvops[per] = inv_op;
    dprintf("%s registered.\n", tsbc_get_per_name(per));
    return 0;
}


int tsbc_get_per(char *name)
{
    uint8_t i;

    for (i = 0; i < TSB_PER_COUNT; i++) {
        if (strcasecmp(name, tsb_pinfo[i].name) == 0) {
            dprintf("name=\"%s\" => per=%d\n", name, tsb_pinfo[i].per);
            return tsb_pinfo[i].per;
        }
    }

    dprintf("\"%s\" name not found!\n", name);
    return -EINVAL;
}

const char *tsbc_get_per_name(int per)
{
    if (per >= TSB_PER_COUNT) {
        eprintf("per (%d) >= TSB_PER_COUNT (%d)\n", per, TSB_PER_COUNT);
        return NULL;
    }
    dprintf("per: %d => name: %s\n", per, tsb_pinfo[per].name);
    return tsb_pinfo[per].name;
}

int tsbc_get_op(char *name)
{
    uint8_t i;

    for (i = 0; i < TSBC_OP_COUNT; i++) {
        if (strcasecmp(name, tsb_ops[i].name) == 0) {
            dprintf("name=\"%s\" => op=%d\n", name, tsb_ops[i].op);
            return tsb_ops[i].op;
        }
    }

    dprintf("\"%s\" name not found!\n", name);
    return -EINVAL;
}

const char *tsbc_get_op_name(int op)
{
    if (op >= TSBC_OP_COUNT) {
        eprintf("op (%d) >= TSBC_OP_COUNT (%d)\n", op, TSBC_OP_COUNT);
        return NULL;
    }
    dprintf("op: %d => name: %s\n", op, tsb_ops[op].name);
    return tsb_ops[op].name;
}

int tsbc_invoke_op(int per, int op, int op_argc, char **op_argv)
{
    #ifdef DEBUG_TSBC
    int i;

    dprintf("per: %d (%s) op: %d (%s)\n", per, tsbc_get_per_name(per),
            op, tsbc_get_op_name(op));
    for (i = 0; i < op_argc; i++) {
        dprintf("op_argv=%s\n", op_argv[i]);
    }
    #endif

    return tsbc_pinvops[per](op, op_argc, op_argv);
}


int tsbc_s2hex(char *s, unsigned int *hex)
{
    if (!s) {
        return -EINVAL;
    }

    if (strncasecmp(s, "0x", 2)) {
        return -EINVAL;
    }

    if (sscanf(s + 2 * sizeof(char), "%x", hex) != 1) {
        return -EINVAL;
    }

    dprintf("%s => %x\n", s, *hex);
    return 0;
}


int tsbc_find_addr(const struct tsbc_reg_info regs[],
                   unsigned int count, const char *name,
                   unsigned int *addr)
{
    unsigned int i;

    if ((!regs) || (!count) || (!name) || (!addr)) {
        return -EINVAL;
    }

    for (i = 0; i < count; i++) {
        if (strcasecmp(name, regs[i].name) == 0) {
            *addr = regs[i].addr;
            dprintf("name=%s => addr=0x%08X\n", name, *addr);
            return 0;
        }
    }

    return -EINVAL;
}

int tsbc_dump_regs(const char *ip, const struct tsbc_reg_info regs[],
                   unsigned int count)
{
    unsigned int i, tmp, llen, nlen, nlenmax = 0;
    char *dash_line, *line;

    /* Get longest name length */
    nlenmax = strlen(ip) + strlen(" Register Name");
    for (i = 0; i < count; i++) {
        nlen = strlen(regs[i].name);
        if (nlen > nlenmax) {
            nlenmax = nlen;
        }
    }

    /* Compute line length */
    llen = nlenmax + 10 + 10 + 4 + 6;
    dash_line = malloc(sizeof(char) * (llen + 1));
    line  = malloc(sizeof(char) * (llen + 1));

    /* Create dash line */
    dash_line[0] = '|';
    memset(dash_line + sizeof(char), '-', llen - 2);
    dash_line[llen - 1] = '|';
    dash_line[llen] = '\0';

    /* Print table */
    printf("%s\n", dash_line);
    /* Print table header */
    sprintf(line, "| %s Register Name", ip);
    if (strlen(line) != nlenmax) {
        for (tmp = strlen(line) - 2; tmp < nlenmax; tmp++) {
            strcat(line, " ");
        }
    }
    strcat(line, " | Address    | Content    |\n");
    printf("%s", line);
    printf("%s\n", dash_line);
    /* Dump Registers content */
    for (i = 0; i < count; i++) {
        sprintf(line, "| %s", regs[i].name);
        if (strlen(regs[i].name) != nlenmax) {
            for (tmp = strlen(regs[i].name); tmp < nlenmax; tmp++) {
                strcat(line, " ");
            }
        }
        sprintf(line + strlen(line),
                " | 0x%08X | 0x%08X |\n", regs[i].addr,
                mem_read32(regs[i].addr));
        printf("%s", line);
    }
    printf("%s\n\n", dash_line);

    free(dash_line);
    free(line);
    return 0;
}


int tsbc_dump_mem(int op_argc, char **op_argv)
{
    uint32_t addr_start, addr_end, i, count;

    if ((!op_argv) || (op_argc != 2)) {
        goto tsbc_dump_mem_usage;
    }

    if ((!op_argv[0]) || (!op_argv[1])) {
        goto tsbc_dump_mem_usage;
    }

    /* Retrieve start and end addresses from user arguments. */
    if (tsbc_s2hex(op_argv[0], &addr_start)) {
        goto tsbc_dump_mem_usage;
    }
    if (tsbc_s2hex(op_argv[1], &addr_end)) {
        goto tsbc_dump_mem_usage;
    }
    if (addr_end < addr_start + 4) {
        goto tsbc_dump_mem_usage;
    }
    if ((addr_start % 4) || (addr_end % 4)) {
        goto tsbc_dump_mem_usage;
    }
    count = 1 + ((addr_end - addr_start) / 4);
    for (i = 0; i < count; i++) {
        printf("%08X: %08X\n", addr_start + (4 * i),
               mem_read32(addr_start + (4 * i)));
    }
    return 0;

tsbc_dump_mem_usage:
    printf("Invalid argument!\n");
    printf("Usage: tsbc dump <start> <end>\n");
    printf("       <start>, <end>: 32-bit aligned hex address, prefixed with '0x'.\n\n");
    return -EINVAL;
}


int tsbc_read_reg(const char *ip,
                  const struct tsbc_reg_info regs[], unsigned int count,
                  int op_argc, char **op_argv)
{
    uint32_t addr;

    if ((!ip) || (!regs) || (!count) ||
        (op_argc != 1) || (!op_argv)) {
        goto tsbc_read_reg_usage;
    }

    if (!op_argv[0]) {
        goto tsbc_read_reg_usage;
    }

    /* Retrieve address from register name */
    if (tsbc_find_addr(regs, count, op_argv[0], &addr)) {
        goto tsbc_read_reg_usage;
    }

    /* Read register content */
    printf("%08X\n", mem_read32(addr));
    return 0;

tsbc_read_reg_usage:
    printf("Invalid argument!\n");
    printf("Usage: tsbc read %s <register>\n", ip);
    printf("       <register>: %s register name, as found in documentation.\n\n",
           ip);
    return -EINVAL;
}


int tsbc_write_reg(const char *ip,
                   const struct tsbc_reg_info regs[], unsigned int count,
                   int op_argc, char **op_argv)
{
    uint32_t addr, val;
    int ret;

    if ((!ip) || (!regs) || (!count) ||
        (op_argc != 2) || (!op_argv)) {
        goto tsbc_write_reg_usage;
    }
    if ((!op_argv[0]) || (!op_argv[1])) {
        goto tsbc_write_reg_usage;
    }

    /* Retrieve address from register name */
    if (tsbc_find_addr(regs, count, op_argv[0], &addr)) {
        goto tsbc_write_reg_usage;
    }
    /* Retrieve value to write into register */
    if (tsbc_s2hex(op_argv[1], &val)) {
        goto tsbc_write_reg_usage;
    }

    /* Write register content */
    dprintf("writing 0x%08X @ 0x%08X\n", val, addr);
    ret = mem_write32(addr, val);
    if (ret) {
        fprintf(stderr, "memory write operation failed!\n");
        return -EIO;
    } else {
        /* Read register content */
        printf("%08X\n", mem_read32(addr));
        return 0;
    }

tsbc_write_reg_usage:
    printf("Invalid argument!\n");
    printf("Usage: tsbc write %s <register> <value>\n", ip);
    printf("       <register>: %s register name, as found in documentation.\n",
           ip);
    printf("       <value>: 32-bit value, prefixed with '0x'.\n\n");
    return -EINVAL;
}


int tsbc_get_bitmask(int op_argc, char **op_argv, uint32_t *mask)
{
    unsigned int i, bit;

    if ((op_argc <= 0) || (!op_argv)) {
        return -EINVAL;
    }
    /* Retrieve bit mask from user arguments. */
    *mask = 0;
    dprintf("%u\n", op_argc);
    for (i = 0; i < op_argc; i++) {
        if (!op_argv[i]) {
            return -EINVAL;
        }
        if (sscanf(op_argv[i], "%u", &bit) != 1) {
            return -EINVAL;
        }
        if (bit >= 32) {
            return -EINVAL;
        }
        *mask |= 1 << bit;
    }

    dprintf("mask=0x%08X\n", *mask);
    return 0;
}

int tsbc_setclear_bits(uint32_t addr, uint32_t mask, unsigned char set)
{
    uint32_t val;
    int ret;

    /* Read register content */
    val = mem_read32(addr);
    dprintf("read 0x%08X @ 0x%08X\n", val, addr);

    /* Set/Clear selected bit(s) */
    if (set) {
        val |= mask;
    } else {
        val &= ~mask;
    }
    dprintf("mask=0x%08X writing 0x%08X @ 0x%08X\n", mask, val, addr);

    ret = mem_write32(addr, val);
    if (ret) {
        fprintf(stderr, "memory write operation failed!\n");
        return -EIO;
    } else {
        /* Read register content */
        printf("%08X\n", mem_read32(addr));
        return 0;
    }
}


int tsbc_setclear_reg(const char *ip,
                      const struct tsbc_reg_info regs[], unsigned int count,
                      int op_argc, char **op_argv, unsigned char set)
{
    uint32_t addr, mask = 0;

    if ((!ip) || (!regs) || (!count) ||
        (op_argc < 2) || (!op_argv)) {
        goto tsbc_setclear_reg_usage;
    }

    if (!op_argv[0]) {
        goto tsbc_setclear_reg_usage;
    }

    /* Retrieve address from user arguments. */
    if (tsbc_find_addr(regs, count, op_argv[0], &addr)) {
        goto tsbc_setclear_reg_usage;
    }

    /* Retrieve bit mask from user arguments. */
    if (tsbc_get_bitmask(op_argc - 1, op_argv + 1, &mask)) {
        goto tsbc_setclear_reg_usage;
    }

    return tsbc_setclear_bits(addr, mask, set);

tsbc_setclear_reg_usage:
    printf("Invalid argument!\n");
    if (set) {
        printf("Usage: tsbc set %s <register> <bit0> [bit1..bitN]\n", ip);
    } else {
        printf("Usage: tsbc clear %s <register> <bit0> [bit1..bitN]\n", ip);
    }
    printf("       <register>: %s register name, as found in documentation.\n",
           ip);
    if (set) {
        printf("       <bit0>, [bit1..bitN]: bit position(s) to set (0 <= bit < 32).\n\n");
    } else {
        printf("       <bit0>, [bit1..bitN]: bit position(s) to clear (0 <= bit < 32).\n\n");
    }
    return -EINVAL;
}

