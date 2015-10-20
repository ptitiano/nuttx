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
 * @brief   Wake Detect Module (WDM) Device
 * @author  Patrick Titiano <ptitiano@baylibre.com>
 */

#ifndef _DEVICE_WDM_H_
#define _DEVICE_WDM_H_

#include <nuttx/util.h>
#include <nuttx/device.h>

#define DEVICE_TYPE_WDM_DEVICE       "WDM"

/**
 * WDM device driver operations
 */
struct device_wdm_type_ops {
    /** WDM wake_frame() function pointer */
    int (*wake_frame)(struct device *dev);

    /** WDM power_down() function pointer */
    int (*power_down)(struct device *dev);

    /** WDM get_wake_mod() function pointer */
    int (*get_wake_mod)(struct device *dev);
};

/**
 * @brief       WDM wake frame function.
 * @param[in]   dev: pointer to structure of device data.
 * @return      0 on success, negative errno on error.
 */
static inline int device_wdm_wake_frame(struct device *dev)
{
    DEVICE_DRIVER_ASSERT_OPS(dev);

    if (!device_is_open(dev))
        return -ENODEV;

    if (DEVICE_DRIVER_GET_OPS(dev, wdm)->wake_frame)
        return DEVICE_DRIVER_GET_OPS(dev, wdm)->wake_frame(dev);

    return -ENOSYS;
}

/**
 * @brief       WDM power down function.
 * @param[in]   dev: pointer to structure of device data.
 * @return      0 on success, negative errno on error.
 */
static inline int device_wdm_power_down(struct device *dev)
{
    DEVICE_DRIVER_ASSERT_OPS(dev);

    if (!device_is_open(dev))
        return -ENODEV;

    if (DEVICE_DRIVER_GET_OPS(dev, wdm)->power_down)
        return DEVICE_DRIVER_GET_OPS(dev, wdm)->power_down(dev);

    return -ENOSYS;
}

/**
 * @brief       WDM function to retrieve the level of the WAKE_MOD pin.
 * @param[in]   dev: pointer to structure of device data.
 * @return      >= 0: pin level, negative errno on error.
 */
static inline int device_wdm_get_wake_mod(struct device *dev)
{
    DEVICE_DRIVER_ASSERT_OPS(dev);

    if (!device_is_open(dev))
        return -ENODEV;

    if (DEVICE_DRIVER_GET_OPS(dev, wdm)->get_wake_mod)
        return DEVICE_DRIVER_GET_OPS(dev, wdm)->get_wake_mod(dev);

    return -ENOSYS;
}

#endif /* _DEVICE_WDM_H_ */
