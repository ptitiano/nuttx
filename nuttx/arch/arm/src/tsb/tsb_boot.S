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

.global bootstrap
.extern _stext_lma, _stext, _text_size
.extern _bootstrap_lma, _bootstrap_vma, _bootstrap_size
.extern __start, tsb_resume_addr
.syntax unified
.thumb

#include "tsb_pmu.h"

.section .bootstrap, "ax"
.align 4
.thumb_func
copy_to_ram:
    ldr r3, [r1], #4
    str r3, [r2], #4
    cbz r0, done_copy_to_ram
    sub r0, r0, #4
    b copy_to_ram
done_copy_to_ram:
    bx lr

.align 4
_copy_to_ram:

.section .bootstrap.loader
.thumb_func
bootstrap:
    ldr r0, =_bootstrap_lma
    ldr r1, =_bootstrap_size
    ldr r2, =_bootstrap_vma

bootstrap_copy:

    cbz r1, done_bootstrap_copy
    ldr r3, [r0], #4
    str r3, [r2], #4
    sub r1, r1, #4
    b bootstrap_copy

done_bootstrap_copy:
    ldr r0, =_text_size
    add r0, r0, #3
    bic r0, r0, #3
    ldr r1, =_stext_lma
    ldr r2, =_stext
    ldr r4, =copy_to_ram
    orr r4, r4, #1
    blx r4

    bl __start

.align 4
pmu_bootret_o:
    .word TSB_PMU_BOOTRET_O
pmu_bootret_o_val:
    .word 0x00000001

pmu_ret_sram_cen_cont:
    .word TSB_PMU_RETSRAMCENCONT
pmu_ret_sram_cen_cont_val:
    .word 0x00000000

pmu_ret_sram_clk_cont:
    .word TSB_PMU_RETSRAMCLKCONT
pmu_ret_sram_clk_cont_val:
    .word 0x00000001

pmu_ret_sram_ret:
    .word TSB_PMU_RETSRAMRET
pmu_ret_sram_ret_val:
    .word 0x00000000

pmu_iso_for_io_en:
    .word TSB_PMU_ISO_FOR_IO_EN
pmu_iso_for_io_en_val:
    .word 0x00000000
