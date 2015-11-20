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
 *
 * @brief   Wake Detect Module (WDM) Driver
 * @author  Patrick Titiano <ptitiano@baylibre.com>
 */

#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <nuttx/lib.h>
#include <nuttx/util.h>
#include <nuttx/kmalloc.h>
#include <nuttx/wqueue.h>
#include <nuttx/device.h>
#include <nuttx/device_wdm.h>
#include <nuttx/gpio.h>
#include <debug.h>


/**
 * @brief private WDM device information
 */
struct wdm_info {
    /** Driver model representation of the device */
    struct device   *dev;

    /** WDM driver info */
    uint32_t powerdown_gpio;
    uint32_t wake_frame_gpio;
    uint32_t wake_mod_gpio;
};

/* WDM Pins */
#define WDM_WAKE_FRAME_GPIO         25 /* GPIO25 - GPBx_WAKE_OUT */
#define WDM_WAKE_MOD_GPIO           23 /* GPIO23 - GPBx_WAKE_IN */
#define WDM_POWERDOWN_GPIO          26 /* GPIO26 */

#define WDM_WAKE_FRAME_HIGH_TIME    2000000 /* us */

/**
 * @brief   WDM power down function.
 * @param   dev: The pointer to the WDM device structure.
 * @return  0 for success, -errno for failures.
 */
static int wdm_power_down(struct device *dev)
{
    struct wdm_info *info = NULL;

    /* check input parameter */
    if (!dev) {
        return -EINVAL;
    }

    info = device_get_private(dev);
    gpio_set_value(info->powerdown_gpio, 0);
    lldbg("WDM POWERDOWM pin set to 0.\n");

    return 0;
}

/**
 * @brief   WDM wake frame function.
 * @param   dev: The pointer to the WDM device structure.
 * @return  0 for success, -errno for failures.
 */
static int wdm_wake_frame(struct device *dev)
{
    struct wdm_info *info = NULL;

    /* check input parameter */
    if (!dev) {
        return -EINVAL;
    }

    info = device_get_private(dev);

    gpio_set_value(info->wake_frame_gpio, 1);
    lldbg("WDM WAKE_FRAME pin set to 1.\n");
    usleep(WDM_WAKE_FRAME_HIGH_TIME); /* to be further adjusted */
    gpio_set_value(info->wake_frame_gpio, 0);
    lldbg("WDM WAKE_FRAME pin set to 0.\n");

    return 0;
}

/**
 * @brief   WDM function to retrieve the level of the WAKE_MOD pin.
 * @param   dev: The pointer to the WDM device structure.
 * @return  >= 0: pin level, -errno for failures.
 */
static int wdm_get_wake_mod(struct device *dev)
{
    struct wdm_info *info = NULL;
    uint8_t wake_mod;

    /* check input parameter */
    if (!dev) {
        return -EINVAL;
    }

    info = device_get_private(dev);

    wake_mod = gpio_get_value(info->wake_mod_gpio);
    lldbg("WDM WAKE_MOD pin = %u.\n", wake_mod);

    return (int) wake_mod;
}

/**
 * @brief   Open WDM device
 *
 * This function is called when the caller is preparing to use this device
 * driver. This function should be called after probe() function and need to
 * check whether the driver already open or not. If driver was opened, it needs
 * to return an error code to the caller to notify the driver was opened.
 *
 * @param   dev: pointer to structure of device data
 * @return  0 on success, negative errno on error
 */
static int wdm_dev_open(struct device *dev)
{
    struct wdm_info *info = NULL;

    /* check input parameter */
    if (!dev || !device_get_private(dev)) {
        return -EINVAL;
    }

    info = device_get_private(dev);

    /* Configure GPIOs */
    gpio_activate(info->powerdown_gpio);
    gpio_activate(info->wake_frame_gpio);
    gpio_activate(info->wake_mod_gpio);
    gpio_direction_out(info->powerdown_gpio, 0);
    gpio_direction_out(info->wake_frame_gpio, 0);
    gpio_direction_in(info->wake_mod_gpio);

    return 0;
}

/**
 * @brief   Close WDM device
 *
 * This function is called when the caller no longer using this driver. It
 * should release or close all resources that allocated by the open() function.
 * This function should be called after the open() function.
 *
 * @param   dev: pointer to structure of device data
 */
static void wdm_dev_close(struct device *dev)
{
    struct wdm_info *info = NULL;

    /* check input parameter */
    if (!dev || !device_get_private(dev)) {
        return;
    }

    info = device_get_private(dev);

    /* Release GPIO */
    gpio_set_value(info->wake_frame_gpio, 0);
    gpio_deactivate(info->powerdown_gpio);
    gpio_deactivate(info->wake_frame_gpio);
    gpio_deactivate(info->wake_mod_gpio);
}

/**
* @brief    The device probe function.
*
* This function is called by the system to register this driver when the
* system boots up. This function allocates memory for saving driver internal
* information data.
*
* @param    dev: The pointer to the WDM device structure.
* @return   0 for success, -errno for failure.
*/
static int wdm_dev_probe(struct device *dev)
{
    struct wdm_info *info;

    info = zalloc(sizeof(*info));
    if (!info)
        return -ENOMEM;

    info->powerdown_gpio = WDM_POWERDOWN_GPIO;
    info->wake_frame_gpio = WDM_WAKE_FRAME_GPIO;
    info->wake_mod_gpio = WDM_WAKE_MOD_GPIO;
    device_set_private(dev, info);

    return 0;
}

/**
 * @brief   The device remove function.
 *
 * This function is called by the system to unregister this driver. It
 * must be called after probe() and open(). It frees the internal information
 * memory space.
 *
 * @param   dev: The pointer to the WDM device structure.
 * @return  None.
 */
static void wdm_dev_remove(struct device *dev)
{
    struct wdm_info *info = NULL;

    /* check input parameter */
    if (!dev || !device_get_private(dev)) {
        return;
    }

    info = device_get_private(dev);

    free(info);
    device_set_private(dev, NULL);
}

static struct device_wdm_type_ops wdm_type_ops = {
    .wake_frame         = wdm_wake_frame,
    .power_down         = wdm_power_down,
    .get_wake_mod       = wdm_get_wake_mod,
};

static struct device_driver_ops wdm_driver_ops = {
    .probe              = wdm_dev_probe,
    .remove             = wdm_dev_remove,
    .open               = wdm_dev_open,
    .close              = wdm_dev_close,
    .type_ops           = &wdm_type_ops,
};

struct device_driver wdm_driver = {
    .type       = DEVICE_TYPE_WDM_DEVICE,
    .name       = "ara_bridge_wdm",
    .desc       = "ARA BRIDGE WDM Driver",
    .ops        = &wdm_driver_ops,
};
