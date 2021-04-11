/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_KEYDETECTOR_SAFA_PRV_H_
#define _NT_KEYDETECTOR_SAFA_PRV_H_

/**
 * \defgroup safa_prv Safa Detector
 * \ingroup detectors_private
 *
 * Safa key detector is a method for recognition of touch or release states. It can be used for each type of control.
 *
 * If the measured sample is reported as a valid sample, the module calculates the delta value from the actual signal
 * value and the baseline value. The delta value is compared to the threshold value computed from the expected signal
 * and baseline values. Based on the result, it determines the electrode state, which can be released, touched, changing
 * from released to touched, and changing from touched to released. The method is using moving average filters to
 * determine the baseline and expected signal values with a different depth of the filter, depending on the state of the
 * electrode. The deadband filters in the horizontal and vertical directions are also implemented.
 * \{
 */

#include "../source/filters/nt_filters_prv.h"
#include "../source/keydetectors/nt_keydetector_prv.h"
#include "nt_types.h"
#include "nt_keydetector_safa.h"

/**
 * Safa RAM structure. This structure is used for internal
 * algorithms to store data, while evaluating Safa.
 * Contains the data of result calculation and auxiliary variables.
 *
 * This structure only manages and uses internal methods.
 */
struct nt_keydetector_safa_data
{
    enum nt_filter_state filter_state;                 /**< Input filter state. */
    struct nt_filter_moving_average_data base_avrg;    /**< Baseline moving average filter data. */
    struct nt_filter_moving_average_data noise_avrg;   /**< Noise signal moving average filter data. */
    struct nt_filter_moving_average_data f_noise_avrg; /**< Fast Noise signal moving average filter data. */
    struct nt_filter_moving_average_data
        predicted_signal_avrg;                         /**< Predicted signal line moving average filter data. */
    struct nt_filter_moving_average base_avrg_init;    /**< Baseline moving average filter settings. */
    struct nt_filter_moving_average noise_avrg_init;   /**< Noise moving average filter settings. */
    struct nt_filter_moving_average f_noise_avrg_init; /**< Fast Noise moving average filter settings. */
    struct nt_filter_moving_average predicted_signal_avrg_init; /**< Predicted signal moving average filter settings. */
    uint32_t noise;                                             /**< Noise value. */
    uint32_t f_noise;                                           /**< Fast Noise value. */
    uint32_t predicted_signal;                                  /**< Predicted signal value. */
    int32_t entry_event_cnt;                                    /**< Event counter value. */
    int32_t deadband_cnt;                                       /**< Deadband event counter. */
    int32_t recovery_cnt;                                       /**< Recovery counter. */
    uint32_t deadband_h;                                        /**< Deadband high watermark */
    uint32_t deadband_l;                                        /**< Deadband low watermark */
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/** \} */ /* end of safa private group */

#endif /* _NT_KEYDETECTOR_SAFA_PRV_H_*/
