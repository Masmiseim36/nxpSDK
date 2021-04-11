/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_MODULE_TSI_PRV_H_
#define _NT_MODULE_TSI_PRV_H_

/**
 * \defgroup tsi_private TSI module
 * \ingroup modules_private
 *
 * The TSI module describes the hardware configuration and control of elementary functionality
 * of the TSI peripheral, it covers all versions of the TSI peripheral by a generic
 * low-level driver API.
 *
 * The TSI module is designed for processors that have a hardware TSI module
 * with version 1, 2, or 4 (for example Kinetis L).
 *
 * The module also handles the NOISE mode supported by TSI v4 (Kinetis L).
 * \{
 */
#include "../source/drivers/tsi/nt_drv_tsi_driver.h"
#include "../source/electrodes/nt_electrodes_prv.h"
#include "nt_modules.h"

#include "nt_types.h"
#include "nt_electrodes.h"
#include "../source/filters/nt_filters_prv.h"

/**
 * The TSI module noise mode initial touch threshold value.
 */
#define NT_TSI_NOISE_INITIAL_TOUCH_THRESHOLD 4U

/**
 * The TSI module noise mode touch range value.
 */
#define NT_TSI_NOISE_TOUCH_RANGE 7U

/**
 * Noise data structure; This structure is used for internal
 * algorithms to store data while evaluating the noise.
 * Contains data of calculating the result and auxiliary variables.
 *
 * This structure manages and uses internal methods only.
 */
struct nt_module_tsi_noise_data
{
    enum nt_filter_state filter_state; /*!< Noise filter state. */
    uint8_t noise;                     /*!< Noise current value. */
    uint8_t touch_threshold;           /*!< Noise touch threshold run-time value. */
};

struct nt_module_tsi_data
{
    nt_tsi_state_t tsi_state; /*!< main NT driver data structure with state variables */
    uint32_t noise_timestamp; /*!< Noise mode switch event time-stamp     */
};

/**
 * The TSI module's noise mode flags definition.
 */
enum nt_module_tsi_flags
{
    NT_MODULE_IN_NOISE_MODE_FLAG =
        1 << NT_FLAGS_SPECIFIC_SHIFT(0), /*!< This flag signalizes that the module is currently in the noise mode. */
    NT_MODULE_HAS_NOISE_MODE_FLAG =
        1 << NT_FLAGS_SPECIFIC_SHIFT(
            1), /*!< This flag signalizes that the module can be switched to the noise mode (TSI v4). */
    NT_MODULE_NOISE_MODE_REQ_FLAG =
        1 << NT_FLAGS_SPECIFIC_SHIFT(2), /*!< This flag signalizes that the module wants to switch to the noise mode. */
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/** \} */ /* end of tsi_private group */

#endif /* _NT_MODULE_TSI_PRV_H_ */
