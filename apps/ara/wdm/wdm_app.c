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
 * @file    apps/ara/wdm/wdm_app.c
 * @brief   Wake Detect Module (WDM) Validation Application
 * @author  Patrick Titiano
 */

#include <nuttx/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <nuttx/device_wdm.h>
#include <tsb_pm.h>

#ifdef DEBUG_WDMAPP
#define dbg_verbose printf
#else
#define dbg_verbose(format, ...)
#endif

#define WDM_NOP             0
#define WDM_NOP_S           ""
#define WDM_OP_POWERDOWN    1
#define WDM_OP_POWERDOWN_S  "powerdown"
#define WDM_OP_WAKE_FRAME   2
#define WDM_OP_WAKE_FRAME_S "wake_frame"
#define WDM_OP_WAKE_MOD     3
#define WDM_OP_WAKE_MOD_S   "wake_mod"
#define WDM_OP_STANDBY      4
#define WDM_OP_STANDBY_S    "standby"


static unsigned int wdm_op = WDM_NOP;


/**
 * @brief           Display application help message
 */
static void wdm_main_usage(void)
{
            printf("Usage: wdm [operation]\n");
            printf("         operation: \n");
            printf("            \"%s\": power down the bridge.\n",
                   WDM_OP_POWERDOWN_S);
            printf("            \"%s\": Wakeup up the Frame.\n\n",
                   WDM_OP_WAKE_FRAME_S);
            printf("            \"%s\": Retrieve wake_mod pin level.\n\n",
                   WDM_OP_WAKE_MOD_S);
            printf("            \"%s\": Initiate standby transition.\n\n",
                   WDM_OP_STANDBY_S);
}

/**
 * @brief           Retrieve command-line user options.
 * @return          0 on success, -EINVAL otherwise
 * @param[in]       argc: user arguments count
 * @param[in]       argv: user arguments
 */
static int wdm_main_get_user_options(int argc, char **argv)
{
    int ret = 0;

    /*
     * As Nuttx apps are not really apps, (it is allocated statically),
     * make sure all static variables are initialized each time
     * the application is launched.
     */
    wdm_op = WDM_NOP;

    if (argc != 2) {
        fprintf(stderr, "Invalid argument count! (%u).\n", argc);
        return -EINVAL;
    }

    if (strcmp(argv[1], WDM_OP_POWERDOWN_S) == 0) {
        wdm_op = WDM_OP_POWERDOWN;
        ret = 0;
    } else if (strcmp(argv[1], WDM_OP_WAKE_FRAME_S) == 0) {
        wdm_op = WDM_OP_WAKE_FRAME;
        ret = 0;
    } else if (strcmp(argv[1], WDM_OP_WAKE_MOD_S) == 0) {
        wdm_op = WDM_OP_WAKE_MOD;
        ret = 0;
    } else if (strcmp(argv[1], WDM_OP_STANDBY_S) == 0) {
        wdm_op = WDM_OP_STANDBY;
        ret = 0;
    } else {
        fprintf(stderr, "Invalid argument! (%s).\n", argv[1]);
        ret = -EINVAL;
    }

    return ret;
}

/**
 * @brief           Initiate a STANDBY transition.
 * @return          0 on success, standard error codes otherwise
 */
int wdm_main_go_standby(struct device *dev)
{
    int ret;

    ret = device_wdm_power_down(dev);
    if (ret) {
        fprintf(stderr, "device_wdm_powerdown() failed! (%d).\n", ret);
    }

    printf("Attempting Standby Mode...\n");
    usleep(100000);

    ret = tsb_pmstandby();
    printf("Returned from Standby Mode.\n");

    return ret;
}


/**
 * @brief           Application main entry point.
 * @return          0 on success, standard error codes otherwise
 * @param[in]       argc: user arguments count
 * @param[in]       argv: user arguments
 */
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int wdm_main(int argc, char *argv[])
#endif
{
    struct device *dev = NULL;
    int ret;

    /* Retrieve user options */
    ret = wdm_main_get_user_options(argc, argv);
    if (ret) {
        wdm_main_usage();
        exit(-EINVAL);
    }

    /* Open WDM device */
    dev = device_open(DEVICE_TYPE_WDM_DEVICE, 0);
    if (!dev) {
        printf("Failed to open WDM driver!\n");
        return -ENODEV;
    }

    /* Execute user-selected WDM operation */
    switch(wdm_op) {
    case WDM_OP_POWERDOWN:
        ret = device_wdm_power_down(dev);
        if (ret) {
            fprintf(stderr, "device_wdm_powerdown() failed! (%d).\n", ret);
        }
        break;
    case WDM_OP_WAKE_FRAME:
        ret = device_wdm_wake_frame(dev);
        if (ret) {
            fprintf(stderr, "device_wdm_wake_frame() failed! (%d).\n", ret);
        }
        break;
    case WDM_OP_WAKE_MOD:
        ret = device_wdm_get_wake_mod(dev);
        if (ret < 0) {
            fprintf(stderr, "device_wdm_wake_frame() failed! (%d).\n", ret);
        } else {
            printf("wake_mod level = %d\n", ret);
        }
        break;
    case WDM_OP_STANDBY:
        ret = wdm_main_go_standby(dev);
        if (ret < 0) {
            fprintf(stderr, "device didn't go to standby! (%d).\n", ret);
        }
        break;
    default:
        ret = -EINVAL;
    }

    /* Close WDM device */
    device_close(dev);
    dev = NULL;

    return ret;
}
