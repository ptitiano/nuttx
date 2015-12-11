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
 * @file    arch/arm/src/tsb/tsb_pmu_es3.h
 * @brief   TSB PMU (Power Management Unit) ES3.0 Register Definitions
 * @author  Patrick Titiano
 */

#ifndef __TSB_PMU_ES3_H__
#define __TSB_PMU_ES3_H__

#include "chip.h"

#define TSB_PMU_ISO_EN              (PMU_BASE + 0x00000000)
#define TSB_PMU_ISO_FOR_IO_EN       (PMU_BASE + 0x00000004)
#define TSB_PMU_RETFFSAVE           (PMU_BASE + 0x00000008)
#define TSB_PMU_RETFFRSTR           (PMU_BASE + 0x0000000C)
#define TSB_PMU_RETSRAMRET          (PMU_BASE + 0x00000010)
#define TSB_PMU_RETSRAMCENCONT      (PMU_BASE + 0x00000014)
#define TSB_PMU_RETSRAMCLKCONT      (PMU_BASE + 0x00000018)
#define TSB_PMU_HB8CLK_EN           (PMU_BASE + 0x0000001C)
#define TSB_PMU_SYSCLKOFFN          (PMU_BASE + 0x00000020)
#define TSB_PMU_WAKEUPSRC           (PMU_BASE + 0x00000024)
#define TSB_PMU_BOOTRET_O           (PMU_BASE + 0x0000002C)
#define TSB_PMU_PWROFFCNT           (PMU_BASE + 0x00000030)
#define TSB_PMU_PWRONCNT            (PMU_BASE + 0x00000034)


#endif /* __TSB_PMU_ES3_H__ */
