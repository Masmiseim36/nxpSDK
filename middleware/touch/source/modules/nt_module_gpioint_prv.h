/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_MODULE_GPIOINT_PRV_H_
#define _NT_MODULE_GPIOINT_PRV_H_

/**
 * \defgroup gpioint_private GPIO interrupt module
 * \ingroup modules_private
 *
 * The GPIO interrupt module describes the hardware configuration and control of the elementary functionality
 * of the method that is using standard GPIO pins of the MCU with the GPIO and timer interrupts.
 *
 * The GPIO interrupt method is designed for all general processors that have a GPIO module with interrupt capability.
 *
 * \{
 */

#include "../source/electrodes/nt_electrodes_prv.h"
#include "nt_modules.h"

#include "nt_types.h"
#include "nt_electrodes.h"
#include "../source/filters/nt_filters_prv.h"

/** GPIO interrupt module's RAM. This structure contains
 */
struct nt_module_gpioint_data
{
    uint32_t pen;         /*!< PEN - enablement of all modules' electrodes */
    uint8_t measured_pin; /*!< The currently measured pin */
    uint8_t scan_status;  /*!< Module's scanning status - see enum nt_gpio_scan_states */
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/** \} */ /* end of gpioint_private group */

#endif /* _NT_MODULE_GPIOINT_PRV_H_*/
