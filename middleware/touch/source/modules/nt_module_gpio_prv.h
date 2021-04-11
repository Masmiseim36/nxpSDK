/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_MODULE_GPIO_PRV_H_
#define _NT_MODULE_GPIO_PRV_H_

/**
 * \defgroup gpio_private GPIO module
 * \ingroup modules_private
 *
 * The GPIO module describes the hardware configuration and control of the elementary functionality
 * of the method that is using standard GPIO pins of the MCU.
 *
 * The GPIO method is designed for all general processors that have a GPIO module.
 *
 * \{
 */

#include "../source/electrodes/nt_electrodes_prv.h"
#include "nt_modules.h"

#include "nt_types.h"
#include "nt_electrodes.h"
#include "../source/filters/nt_filters_prv.h"

/** GPIO module's RAM. This structure contains
 */
struct nt_module_gpio_data
{
    uint32_t pen; /*!< PEN - enablement of all modules electrodes */
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/** \} */ /* end of gpio_private group */

#endif /* _NT_MODULE_GPIO_PRV_H_*/
