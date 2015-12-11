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
 * @file    apps/ara/tsbc/tsbc_scm_regs.h
 * @brief   TSBCONF System Controller Module (SCM) Registers Definition
 * @author  Patrick Titiano
 */

#ifndef __TSBC_SCM_REGS_H__
#define __TSBC_SCM_REGS_H__

#include "tsbc.h"

#ifdef CONFIG_TSB_CHIP_REV_ES2
#ifdef CONFIG_ARCH_CHIP_APBRIDGE

#define TSBC_SCM_REGS_COUNT             40

#define TSB_SCM_SOFTRESET0              0x40000000
#define TSB_SCM_SOFTRESET1              0x40000004
#define TSB_SCM_SOFTRESET2              0x40000008
#define TSB_SCM_SOFTRESETRELEASE0       0x40000100
#define TSB_SCM_SOFTRESETRELEASE1       0x40000104
#define TSB_SCM_SOFTRESETRELEASE2       0x40000108
#define TSB_SCM_CLOCKGATING0            0x40000200
#define TSB_SCM_CLOCKGATING1            0x40000204
#define TSB_SCM_CLOCKGATING2            0x40000208
#define TSB_SCM_CLOCKENABLE0            0x40000300
#define TSB_SCM_CLOCKENABLE1            0x40000304
#define TSB_SCM_CLOCKENABLE2            0x40000308
#define TSB_SCM_BOOTSELECTOR            0x40000400
#define TSB_SCM_I2S_CLOCK_SELECTOR      0x40000440
#define TSB_SCM_SPI_CLOCK_SELECTOR      0x40000480
#define TSB_SCM_SYSTEM_CONF             0x400004C0
#define TSB_SCM_ECCERROR                0x400004C4
#define TSB_SCM_USBOTG_HSIC_CONTROL     0x40000500
#define TSB_SCM_VID                     0x40000700
#define TSB_SCM_PID                     0x40000704
#define TSB_SCM_REVISION0               0x40000710
#define TSB_SCM_REVISION1               0x40000714
#define TSB_SCM_PINSHARE                0x40000800
#define TSB_SCM_CHIPID0                 0x40000880
#define TSB_SCM_CHIPID1                 0x40000884
#define TSB_SCM_CHIPID2                 0x40000888
#define TSB_SCM_PLLA_INIT               0x40000900
#define TSB_SCM_PLLA_DIVIDE_MODE        0x40000904
#define TSB_SCM_PLLA_CLK_FACTOR         0x40000908
#define TSB_SCM_PLLA_CLKFREQ_MODE       0x4000090C
#define TSB_SCM_PLLA_SSCG_MODE          0x40000910
#define TSB_SCM_PLLA_SSCG_PARAM         0x40000914
#define TSB_SCM_IO_DRIVE_STRENGHT0      0x40000A00
#define TSB_SCM_IO_DRIVE_STRENGHT1      0x40000A04
#define TSB_SCM_IO_PULL_UPDOWN0         0x40000A10
#define TSB_SCM_IO_PULL_UPDOWN1         0x40000A14
#define TSB_SCM_IO_PULL_UPDOWN_ENABLE0  0x40000A20
#define TSB_SCM_IO_PULL_UPDOWN_ENABLE1  0x40000A24
#define TSB_SCM_IO_PULL_UP_ENABLE0      0x40000A30
#define TSB_SCM_IO_PULL_UP_ENABLE1      0x40000A34

static const struct tsbc_reg_info tsb_scm_regs[] = {
    { TSB_SCM_SOFTRESET0,               "SoftReset0"},
    { TSB_SCM_SOFTRESET1,               "SoftReset1"},
    { TSB_SCM_SOFTRESET2,               "SoftReset2"},
    { TSB_SCM_SOFTRESETRELEASE0,        "SoftResetRelease0"},
    { TSB_SCM_SOFTRESETRELEASE1,        "SoftResetRelease1"},
    { TSB_SCM_SOFTRESETRELEASE2,        "SoftResetRelease2"},
    { TSB_SCM_CLOCKGATING0,             "ClockGating0"},
    { TSB_SCM_CLOCKGATING1,             "ClockGating1"},
    { TSB_SCM_CLOCKGATING2,             "ClockGating2"},
    { TSB_SCM_CLOCKENABLE0,             "ClockEnable0"},
    { TSB_SCM_CLOCKENABLE1,             "ClockEnable1"},
    { TSB_SCM_CLOCKENABLE2,             "ClockEnable2"},
    { TSB_SCM_BOOTSELECTOR,             "BootSelector"},
    { TSB_SCM_I2S_CLOCK_SELECTOR,       "I2S_Clock_Selector"},
    { TSB_SCM_SPI_CLOCK_SELECTOR,       "SPI_Clock_Selector"},
    { TSB_SCM_SYSTEM_CONF,              "System_Conf"},
    { TSB_SCM_ECCERROR,                 "EccError"},
    { TSB_SCM_USBOTG_HSIC_CONTROL,      "USBOTG_HSIC_control"},
    { TSB_SCM_VID,                      "VID"},
    { TSB_SCM_PID,                      "PID"},
    { TSB_SCM_REVISION0,                "Revision0"},
    { TSB_SCM_REVISION1,                "Revision1"},
    { TSB_SCM_PINSHARE,                 "PINSHARE"},
    { TSB_SCM_CHIPID0,                  "ChipID0"},
    { TSB_SCM_CHIPID1,                  "ChipID1"},
    { TSB_SCM_CHIPID2,                  "ChipID2"},
    { TSB_SCM_PLLA_INIT,                "PLLA_INIT"},
    { TSB_SCM_PLLA_DIVIDE_MODE,         "PLLA_Divide_MODE"},
    { TSB_SCM_PLLA_CLK_FACTOR,          "PLLA_CLK_FACTOR"},
    { TSB_SCM_PLLA_CLKFREQ_MODE,        "PLLA_CLKFREQ_MODE"},
    { TSB_SCM_PLLA_SSCG_MODE,           "PLLA_SSCG_MODE"},
    { TSB_SCM_PLLA_SSCG_PARAM,          "PLLA_SSCG_PARAM"},
    { TSB_SCM_IO_DRIVE_STRENGHT0,       "IO_DRIVE_STRENGHT0"},
    { TSB_SCM_IO_DRIVE_STRENGHT1,       "IO_DRIVE_STRENGHT1"},
    { TSB_SCM_IO_PULL_UPDOWN0,          "IO_Pull_UpDown0"},
    { TSB_SCM_IO_PULL_UPDOWN1,          "IO_Pull_UpDown1"},
    { TSB_SCM_IO_PULL_UPDOWN_ENABLE0,   "IO_Pull_UpDown_Enable0"},
    { TSB_SCM_IO_PULL_UPDOWN_ENABLE1,   "IO_Pull_UpDown_Enable1"},
    { TSB_SCM_IO_PULL_UP_ENABLE0,       "IO_Pull_Up_Enable0"},
    { TSB_SCM_IO_PULL_UP_ENABLE1,       "IO_Pull_Up_Enable1"},
};

#elif defined(CONFIG_ARCH_CHIP_GPBRIDGE)

#define TSBC_SCM_REGS_COUNT             59

#define TSB_SCM_SOFTRESET0              0x40000000
#define TSB_SCM_SOFTRESET1              0x40000004
#define TSB_SCM_SOFTRESETRELEASE0       0x40000100
#define TSB_SCM_SOFTRESETRELEASE1       0x40000104
#define TSB_SCM_CLOCKGATING0            0x40000200
#define TSB_SCM_CLOCKGATING1            0x40000204
#define TSB_SCM_CLOCKENABLE0            0x40000300
#define TSB_SCM_CLOCKENABLE1            0x40000304
#define TSB_SCM_BOOTSELECTOR            0x40000400
#define TSB_SCM_I2S_CLOCK_SELECTOR      0x40000440
#define TSB_SCM_SPI_CLOCK_SELECTOR      0x40000480
#define TSB_SCM_SYSTEM_CONF             0x400004C0
#define TSB_SCM_ECCERROR                0x400004C4
#define TSB_SCM_USBOTG_HSIC_CONTROL     0x40000500
#define TSB_SCM_UHSSD_CAPINIT0          0x40000600
#define TSB_SCM_UHSSD_CAPAINIT1         0x40000604
#define TSB_SCM_UHSSD_PRESET0           0x40000608
#define TSB_SCM_UHSSD_PRESET1           0x4000060C
#define TSB_SCM_UHSSD_PRESET2           0x40000610
#define TSB_SCM_UHSSD_PRESET3           0x40000614
#define TSB_SCM_UHSSD_MAXCURNT          0x40000618
#define TSB_SCM_UHSSD_INDLY             0x40000620
#define TSB_SCM_UHSSD_OUTDLY            0x40000624
#define TSB_SCM_UHSSD_DLLCTRL           0x40000628
#define TSB_SCM_UHSSD_IO0CTRL           0x40000640
#define TSB_SCM_UHSSD_IO1CTRL           0x40000644
#define TSB_SCM_UHSSD_IO2CTRL           0x40000648
#define TSB_SCM_UHSSD_IO3CTRL           0x4000064C
#define TSB_SCM_UHSSD_IO4CTRL           0x40000650
#define TSB_SCM_UHSSD_IO5CTRL           0x40000654
#define TSB_SCM_UHSSD_IO6CTRL           0x40000658
#define TSB_SCM_UHSSD_IO7CTRL           0x4000065C
#define TSB_SCM_UHSSD_IOENACTRL         0x40000660
#define TSB_SCM_UHSSD_DMADEBUG          0x40000674
#define TSB_SCM_UHSSD_CMDDEBUG          0x40000678
#define TSB_SCM_UHSSD_TXDEBUG           0x4000067C
#define TSB_SCM_UHSSD_RXDEBUG           0x40000680
#define TSB_SCM_VID                     0x40000700
#define TSB_SCM_PID                     0x40000704
#define TSB_SCM_REVISION0               0x40000710
#define TSB_SCM_REVISION1               0x40000714
#define TSB_SCM_PINSHARE                0x40000800
#define TSB_SCM_CHIPID0                 0x40000880
#define TSB_SCM_CHIPID1                 0x40000884
#define TSB_SCM_CHIPID2                 0x40000888
#define TSB_SCM_PLLA_INIT               0x40000900
#define TSB_SCM_PLLA_DIVIDE_MODE        0x40000904
#define TSB_SCM_PLLA_CLK_FACTOR         0x40000908
#define TSB_SCM_PLLA_CLKFREQ_MODE       0x4000090C
#define TSB_SCM_PLLA_SSCG_MODE          0x40000910
#define TSB_SCM_PLLA_SSCG_PARAM         0x40000914
#define TSB_SCM_IO_DRIVE_STRENGHT0      0x40000A00
#define TSB_SCM_IO_DRIVE_STRENGHT1      0x40000A04
#define TSB_SCM_IO_PULL_UPDOWN0         0x40000A10
#define TSB_SCM_IO_PULL_UPDOWN1         0x40000A14
#define TSB_SCM_IO_PULL_UPDOWN_ENABLE0  0x40000A20
#define TSB_SCM_IO_PULL_UPDOWN_ENABLE1  0x40000A24
#define TSB_SCM_IO_PULL_UP_ENABLE0      0x40000A30
#define TSB_SCM_IO_PULL_UP_ENABLE1      0x40000A34

static const struct tsbc_reg_info tsb_scm_regs[] = {
    { TSB_SCM_SOFTRESET0,               "SoftReset0"},
    { TSB_SCM_SOFTRESET1,               "SoftReset1"},
    { TSB_SCM_SOFTRESETRELEASE0,        "SoftResetRelease0"},
    { TSB_SCM_SOFTRESETRELEASE1,        "SoftResetRelease1"},
    { TSB_SCM_CLOCKGATING0,             "ClockGating0"},
    { TSB_SCM_CLOCKGATING1,             "ClockGating1"},
    { TSB_SCM_CLOCKENABLE0,             "ClockEnable0"},
    { TSB_SCM_CLOCKENABLE1,             "ClockEnable1"},
    { TSB_SCM_BOOTSELECTOR,             "BootSelector"},
    { TSB_SCM_I2S_CLOCK_SELECTOR,       "I2S_Clock_Selector"},
    { TSB_SCM_SPI_CLOCK_SELECTOR,       "SPI_Clock_Selector"},
    { TSB_SCM_SYSTEM_CONF,              "System_Conf"},
    { TSB_SCM_ECCERROR,                 "EccError"},
    { TSB_SCM_USBOTG_HSIC_CONTROL,      "USBOTG_HSIC_control"},
    { TSB_SCM_UHSSD_CAPINIT0,           "UHSSD_CAPINIT0"},
    { TSB_SCM_UHSSD_CAPAINIT1,          "UHSSD_CAPAINIT1"},
    { TSB_SCM_UHSSD_PRESET0,            "UHSSD_PRESET0"},
    { TSB_SCM_UHSSD_PRESET1,            "UHSSD_PRESET1"},
    { TSB_SCM_UHSSD_PRESET2,            "UHSSD_PRESET2"},
    { TSB_SCM_UHSSD_PRESET3,            "UHSSD_PRESET3"},
    { TSB_SCM_UHSSD_MAXCURNT,           "UHSSD_MAXCURNT"},
    { TSB_SCM_UHSSD_INDLY,              "UHSSD_INDLY"},
    { TSB_SCM_UHSSD_OUTDLY,             "UHSSD_OUTDLY"},
    { TSB_SCM_UHSSD_DLLCTRL,            "UHSSD_DLLCTRL"},
    { TSB_SCM_UHSSD_IO0CTRL,            "UHSSD_IO0CTRL"},
    { TSB_SCM_UHSSD_IO1CTRL,            "UHSSD_IO1CTRL"},
    { TSB_SCM_UHSSD_IO2CTRL,            "UHSSD_IO2CTRL"},
    { TSB_SCM_UHSSD_IO3CTRL,            "UHSSD_IO3CTRL"},
    { TSB_SCM_UHSSD_IO4CTRL,            "UHSSD_IO4CTRL"},
    { TSB_SCM_UHSSD_IO5CTRL,            "UHSSD_IO5CTRL"},
    { TSB_SCM_UHSSD_IO6CTRL,            "UHSSD_IO6CTRL"},
    { TSB_SCM_UHSSD_IO7CTRL,            "UHSSD_IO7CTRL"},
    { TSB_SCM_UHSSD_IOENACTRL,          "UHSSD_IOENACTRL"},
    { TSB_SCM_UHSSD_DMADEBUG,           "UHSSD_DMADEBUG"},
    { TSB_SCM_UHSSD_CMDDEBUG,           "UHSSD_CMDDEBUG"},
    { TSB_SCM_UHSSD_TXDEBUG,            "UHSSD_TXDEBUG"},
    { TSB_SCM_UHSSD_RXDEBUG,            "UHSSD_RXDEBUG"},
    { TSB_SCM_VID,                      "VID"},
    { TSB_SCM_PID,                      "PID"},
    { TSB_SCM_REVISION0,                "Revision0"},
    { TSB_SCM_REVISION1,                "Revision1"},
    { TSB_SCM_PINSHARE,                 "PINSHARE"},
    { TSB_SCM_CHIPID0,                  "ChipID0"},
    { TSB_SCM_CHIPID1,                  "ChipID1"},
    { TSB_SCM_CHIPID2,                  "ChipID2"},
    { TSB_SCM_PLLA_INIT,                "PLLA_INIT"},
    { TSB_SCM_PLLA_DIVIDE_MODE,         "PLLA_Divide_MODE"},
    { TSB_SCM_PLLA_CLK_FACTOR,          "PLLA_CLK_FACTOR"},
    { TSB_SCM_PLLA_CLKFREQ_MODE,        "PLLA_CLKFREQ_MODE"},
    { TSB_SCM_PLLA_SSCG_MODE,           "PLLA_SSCG_MODE"},
    { TSB_SCM_PLLA_SSCG_PARAM,          "PLLA_SSCG_PARAM"},
    { TSB_SCM_IO_DRIVE_STRENGHT0,       "IO_DRIVE_STRENGHT0"},
    { TSB_SCM_IO_DRIVE_STRENGHT1,       "IO_DRIVE_STRENGHT1"},
    { TSB_SCM_IO_PULL_UPDOWN0,          "IO_Pull_UpDown0"},
    { TSB_SCM_IO_PULL_UPDOWN1,          "IO_Pull_UpDown1"},
    { TSB_SCM_IO_PULL_UPDOWN_ENABLE0,   "IO_Pull_UpDown_Enable0"},
    { TSB_SCM_IO_PULL_UPDOWN_ENABLE1,   "IO_Pull_UpDown_Enable1"},
    { TSB_SCM_IO_PULL_UP_ENABLE0,       "IO_Pull_Up_Enable0"},
    { TSB_SCM_IO_PULL_UP_ENABLE1,       "IO_Pull_Up_Enable1"},
};

#endif
#elif defined(CONFIG_TSB_CHIP_REV_ES3)
#ifdef CONFIG_ARCH_CHIP_APBRIDGE

#define TSBC_SCM_REGS_COUNT             39

#define TSB_SCM_SOFTRESET0              0x40000000
#define TSB_SCM_SOFTRESET1              0x40000004
#define TSB_SCM_SOFTRESET2              0x40000008
#define TSB_SCM_SOFTRESETRELEASE0       0x40000100
#define TSB_SCM_SOFTRESETRELEASE1       0x40000104
#define TSB_SCM_SOFTRESETRELEASE2       0x40000108
#define TSB_SCM_CLOCKGATING0            0x40000200
#define TSB_SCM_CLOCKGATING1            0x40000204
#define TSB_SCM_CLOCKGATING2            0x40000208
#define TSB_SCM_CLOCKGATING3            0x4000020C
#define TSB_SCM_CLOCKENABLE0            0x40000300
#define TSB_SCM_CLOCKENABLE1            0x40000304
#define TSB_SCM_CLOCKENABLE2            0x40000308
#define TSB_SCM_CLOCKENABLE3            0x4000030C
#define TSB_SCM_BOOTSELECTOR            0x40000400
#define TSB_SCM_SYSTEM_CLOCK_DIVIDER0   0x40000410
#define TSB_SCM_SYSTEM_CLOCK_DIVIDER1   0x40000414
#define TSB_SCM_I2S_CLOCK_SELECTOR      0x40000440
#define TSB_SCM_SYSTEM_CONF             0x400004C0
#define TSB_SCM_ECCERROR                0x400004C4
#define TSB_SCM_USBOTG_HSIC_CONTROL     0x40000500
#define TSB_SCM_VID                     0x40000700
#define TSB_SCM_PID                     0x40000704
#define TSB_SCM_PINSHARE                0x40000800
#define TSB_SCM_PLLA_INIT               0x40000900
#define TSB_SCM_PLLA_DIVIDE_MODE        0x40000904
#define TSB_SCM_PLLA_CLK_FACTOR         0x40000908
#define TSB_SCM_PLLA_CLKFREQ_MODE       0x4000090C
#define TSB_SCM_PLLA_SSCG_MODE          0x40000910
#define TSB_SCM_PLLA_SSCG_PARAM         0x40000914
#define TSB_SCM_IO_DRIVE_STRENGTH0      0x40000A00
#define TSB_SCM_IO_DRIVE_STRENGTH1      0x40000A04
#define TSB_SCM_IO_DRIVE_STRENGTH2      0x40000A08
#define TSB_SCM_IO_PULL_UPDOWN0         0x40000A20
#define TSB_SCM_IO_PULL_UPDOWN1         0x40000A24
#define TSB_SCM_IO_PULL_UPDOWN_ENABLE0  0x40000A30
#define TSB_SCM_IO_PULL_UPDOWN_ENABLE1  0x40000A34
#define TSB_SCM_UNIPRO_CLK_EN           0x40000E10
#define TSB_SCM_UNIPRO_CLK_FLAG         0x40000E20

static const struct tsbc_reg_info tsb_scm_regs[] = {
    { TSB_SCM_SOFTRESET0,               "SoftReset0"},
    { TSB_SCM_SOFTRESET1,               "SoftReset1"},
    { TSB_SCM_SOFTRESET2,               "SoftReset2"},
    { TSB_SCM_SOFTRESETRELEASE0,        "SoftResetRelease0"},
    { TSB_SCM_SOFTRESETRELEASE1,        "SoftResetRelease1"},
    { TSB_SCM_SOFTRESETRELEASE2,        "SoftResetRelease2"},
    { TSB_SCM_CLOCKGATING0,             "ClockGating0"},
    { TSB_SCM_CLOCKGATING1,             "ClockGating1"},
    { TSB_SCM_CLOCKGATING2,             "ClockGating2"},
    { TSB_SCM_CLOCKGATING3,             "ClockGating3"},
    { TSB_SCM_CLOCKENABLE0,             "ClockEnable0"},
    { TSB_SCM_CLOCKENABLE1,             "ClockEnable1"},
    { TSB_SCM_CLOCKENABLE2,             "ClockEnable2"},
    { TSB_SCM_CLOCKENABLE3,             "ClockEnable3"},
    { TSB_SCM_BOOTSELECTOR,             "BootSelector"},
    { TSB_SCM_SYSTEM_CLOCK_DIVIDER0,    "System_Clock_Divider0"},
    { TSB_SCM_SYSTEM_CLOCK_DIVIDER1,    "System_Clock_Divider1"},
    { TSB_SCM_I2S_CLOCK_SELECTOR,       "I2S_Clock_Selector"},
    { TSB_SCM_SYSTEM_CONF,              "System_Conf"},
    { TSB_SCM_ECCERROR,                 "EccError"},
    { TSB_SCM_USBOTG_HSIC_CONTROL,      "USBOTG_HSIC_control"},
    { TSB_SCM_VID,                      "VID"},
    { TSB_SCM_PID,                      "PID"},
    { TSB_SCM_PINSHARE,                 "PINSHARE"},
    { TSB_SCM_PLLA_INIT,                "PLLA_INIT"},
    { TSB_SCM_PLLA_DIVIDE_MODE,         "PLLA_Divide_MODE"},
    { TSB_SCM_PLLA_CLK_FACTOR,          "PLLA_CLK_FACTOR"},
    { TSB_SCM_PLLA_CLKFREQ_MODE,        "PLLA_CLKFREQ_MODE"},
    { TSB_SCM_PLLA_SSCG_MODE,           "PLLA_SSCG_MODE"},
    { TSB_SCM_PLLA_SSCG_PARAM,          "PLLA_SSCG_PARAM"},
    { TSB_SCM_IO_DRIVE_STRENGTH0,       "IO_DRIVE_STRENGTH0"},
    { TSB_SCM_IO_DRIVE_STRENGTH1,       "IO_DRIVE_STRENGTH1"},
    { TSB_SCM_IO_DRIVE_STRENGTH2,       "IO_DRIVE_STRENGTH2"},
    { TSB_SCM_IO_PULL_UPDOWN0,          "IO_Pull_UpDown0"},
    { TSB_SCM_IO_PULL_UPDOWN1,          "IO_Pull_UpDown1"},
    { TSB_SCM_IO_PULL_UPDOWN_ENABLE0,   "IO_Pull_UpDown_Enable0"},
    { TSB_SCM_IO_PULL_UPDOWN_ENABLE1,   "IO_Pull_UpDown_Enable1"},
    { TSB_SCM_UNIPRO_CLK_EN,            "UNIPRO_CLK_EN"},
    { TSB_SCM_UNIPRO_CLK_FLAG,          "UNIPRO_CLK_FLAG"},
};

#elif defined(CONFIG_ARCH_CHIP_GPBRIDGE)

#define TSBC_SCM_REGS_COUNT             55

#define TSB_SCM_SOFTRESET0              0x40000000
#define TSB_SCM_SOFTRESET1              0x40000004
#define TSB_SCM_SOFTRESETRELEASE0       0x40000100
#define TSB_SCM_SOFTRESETRELEASE1       0x40000104
#define TSB_SCM_CLOCKGATING0            0x40000200
#define TSB_SCM_CLOCKGATING1            0x40000204
#define TSB_SCM_CLOCKGATING3            0x4000020C
#define TSB_SCM_CLOCKENABLE0            0x40000300
#define TSB_SCM_CLOCKENABLE1            0x40000304
#define TSB_SCM_CLOCKENABLE3            0x4000030C
#define TSB_SCM_BOOTSELECTOR            0x40000400
#define TSB_SCM_SYSTEM_CLOCK_DIVIDER0   0x40000410
#define TSB_SCM_SYSTEM_CLOCK_DIVIDER1   0x40000414
#define TSB_SCM_I2S_CLOCK_SELECTOR      0x40000440
#define TSB_SCM_SYSTEM_CONF             0x400004C0
#define TSB_SCM_ECCERROR                0x400004C4
#define TSB_SCM_USBOTG_HSIC_CONTROL     0x40000500
#define TSB_SCM_UHSSD_CAPINIT0          0x40000600
#define TSB_SCM_UHSSD_CAPAINIT1         0x40000604
#define TSB_SCM_UHSSD_PRESET0           0x40000608
#define TSB_SCM_UHSSD_PRESET1           0x4000060C
#define TSB_SCM_UHSSD_PRESET2           0x40000610
#define TSB_SCM_UHSSD_PRESET3           0x40000614
#define TSB_SCM_UHSSD_MAXCURNT          0x40000618
#define TSB_SCM_UHSSD_IO0CTRL           0x40000640
#define TSB_SCM_UHSSD_IO1CTRL           0x40000644
#define TSB_SCM_UHSSD_IO2CTRL           0x40000648
#define TSB_SCM_UHSSD_IO3CTRL           0x4000064C
#define TSB_SCM_UHSSD_IO4CTRL           0x40000650
#define TSB_SCM_UHSSD_IO5CTRL           0x40000654
#define TSB_SCM_UHSSD_IO6CTRL           0x40000658
#define TSB_SCM_UHSSD_IO7CTRL           0x4000065C
#define TSB_SCM_UHSSD_IOENACTRL         0x40000660
#define TSB_SCM_VID                     0x40000700
#define TSB_SCM_PID                     0x40000704
#define TSB_SCM_PINSHARE                0x40000800
#define TSB_SCM_PLLA_INIT               0x40000900
#define TSB_SCM_PLLA_DIVIDE_MODE        0x40000904
#define TSB_SCM_PLLA_CLK_FACTOR         0x40000908
#define TSB_SCM_PLLA_CLKFREQ_MODE       0x4000090C
#define TSB_SCM_PLLA_SSCG_MODE          0x40000910
#define TSB_SCM_PLLA_SSCG_PARAM         0x40000914
#define TSB_SCM_IO_DRIVE_STRENGTH0      0x40000A00
#define TSB_SCM_IO_DRIVE_STRENGTH1      0x40000A04
#define TSB_SCM_IO_DRIVE_STRENGTH2      0x40000A08
#define TSB_SCM_IO_DRIVE_STRENGTH3      0x40000A0C
#define TSB_SCM_IO_DRIVE_STRENGTH4      0x40000A10
#define TSB_SCM_IO_PULL_UPDOWN0         0x40000A20
#define TSB_SCM_IO_PULL_UPDOWN1         0x40000A24
#define TSB_SCM_IO_PULL_UPDOWN_ENABLE0  0x40000A30
#define TSB_SCM_IO_PULL_UPDOWN_ENABLE1  0x40000A34
#define TSB_SCM_IO_PULL_UP_ENABLE0      0x40000A40
#define TSB_SCM_IO_PULL_UP_ENABLE1      0x40000A44
#define TSB_SCM_UNIPRO_CLK_EN           0x40000E10
#define TSB_SCM_UNIPRO_CLK_FLAG         0x40000E20};

static const struct tsbc_reg_info tsb_scm_regs[] = {
    { TSB_SCM_SOFTRESET0,               "SoftReset0"},
    { TSB_SCM_SOFTRESET1,               "SoftReset1"},
    { TSB_SCM_SOFTRESETRELEASE0,        "SoftResetRelease0"},
    { TSB_SCM_SOFTRESETRELEASE1,        "SoftResetRelease1"},
    { TSB_SCM_CLOCKGATING0,             "ClockGating0"},
    { TSB_SCM_CLOCKGATING1,             "ClockGating1"},
    { TSB_SCM_CLOCKGATING3,             "ClockGating3"},
    { TSB_SCM_CLOCKENABLE0,             "ClockEnable0"},
    { TSB_SCM_CLOCKENABLE1,             "ClockEnable1"},
    { TSB_SCM_CLOCKENABLE3,             "ClockEnable3"},
    { TSB_SCM_BOOTSELECTOR,             "BootSelector"},
    { TSB_SCM_SYSTEM_CLOCK_DIVIDER0,    "System_Clock_Divider0"},
    { TSB_SCM_SYSTEM_CLOCK_DIVIDER1,    "System_Clock_Divider1"},
    { TSB_SCM_I2S_CLOCK_SELECTOR,       "I2S_Clock_Selector"},
    { TSB_SCM_SYSTEM_CONF,              "System_Conf"},
    { TSB_SCM_ECCERROR,                 "EccError"},
    { TSB_SCM_USBOTG_HSIC_CONTROL,      "USBOTG_HSIC_control"},
    { TSB_SCM_UHSSD_CAPINIT0,           "UHSSD_CAPINIT0"},
    { TSB_SCM_UHSSD_CAPAINIT1,          "UHSSD_CAPAINIT1"},
    { TSB_SCM_UHSSD_PRESET0,            "UHSSD_PRESET0"},
    { TSB_SCM_UHSSD_PRESET1,            "UHSSD_PRESET1"},
    { TSB_SCM_UHSSD_PRESET2,            "UHSSD_PRESET2"},
    { TSB_SCM_UHSSD_PRESET3,            "UHSSD_PRESET3"},
    { TSB_SCM_UHSSD_MAXCURNT,           "UHSSD_MAXCURNT"},
    { TSB_SCM_UHSSD_IO0CTRL,            "UHSSD_IO0CTRL"},
    { TSB_SCM_UHSSD_IO1CTRL,            "UHSSD_IO1CTRL"},
    { TSB_SCM_UHSSD_IO2CTRL,            "UHSSD_IO2CTRL"},
    { TSB_SCM_UHSSD_IO3CTRL,            "UHSSD_IO3CTRL"},
    { TSB_SCM_UHSSD_IO4CTRL,            "UHSSD_IO4CTRL"},
    { TSB_SCM_UHSSD_IO5CTRL,            "UHSSD_IO5CTRL"},
    { TSB_SCM_UHSSD_IO6CTRL,            "UHSSD_IO6CTRL"},
    { TSB_SCM_UHSSD_IO7CTRL,            "UHSSD_IO7CTRL"},
    { TSB_SCM_UHSSD_IOENACTRL,          "UHSSD_IOENACTRL"},
    { TSB_SCM_VID,                      "VID"},
    { TSB_SCM_PID,                      "PID"},
    { TSB_SCM_PINSHARE,                 "PINSHARE"},
    { TSB_SCM_PLLA_INIT,                "PLLA_INIT"},
    { TSB_SCM_PLLA_DIVIDE_MODE,         "PLLA_Divide_MODE"},
    { TSB_SCM_PLLA_CLK_FACTOR,          "PLLA_CLK_FACTOR"},
    { TSB_SCM_PLLA_CLKFREQ_MODE,        "PLLA_CLKFREQ_MODE"},
    { TSB_SCM_PLLA_SSCG_MODE,           "PLLA_SSCG_MODE"},
    { TSB_SCM_PLLA_SSCG_PARAM,          "PLLA_SSCG_PARAM"},
    { TSB_SCM_IO_DRIVE_STRENGTH0,       "IO_DRIVE_STRENGTH0"},
    { TSB_SCM_IO_DRIVE_STRENGTH1,       "IO_DRIVE_STRENGTH1"},
    { TSB_SCM_IO_DRIVE_STRENGTH2,       "IO_DRIVE_STRENGTH2"},
    { TSB_SCM_IO_DRIVE_STRENGTH3,       "IO_DRIVE_STRENGTH3"},
    { TSB_SCM_IO_DRIVE_STRENGTH4,       "IO_DRIVE_STRENGTH4"},
    { TSB_SCM_IO_PULL_UPDOWN0,          "IO_Pull_UpDown0"},
    { TSB_SCM_IO_PULL_UPDOWN1,          "IO_Pull_UpDown1"},
    { TSB_SCM_IO_PULL_UPDOWN_ENABLE0,   "IO_Pull_UpDown_Enable0"},
    { TSB_SCM_IO_PULL_UPDOWN_ENABLE1,   "IO_Pull_UpDown_Enable1"},
    { TSB_SCM_IO_PULL_UP_ENABLE0,       "IO_Pull_Up_Enable0"},
    { TSB_SCM_IO_PULL_UP_ENABLE1,       "IO_Pull_Up_Enable1"},
    { TSB_SCM_UNIPRO_CLK_EN,            "UNIPRO_CLK_EN"},
    { TSB_SCM_UNIPRO_CLK_FLAG,          "UNIPRO_CLK_FLAG"},
};
#endif
#endif

#endif
