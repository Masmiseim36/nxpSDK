/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_KEYDETECTOR_AFID_PRV_H_
#define _NT_KEYDETECTOR_AFID_PRV_H_

/**
 * \defgroup afid_private Advanced Filtering and Integrating Detection
 * \ingroup detectors_private
 * The AFID (Advanced Filtering and Integrating Detection) key detector is based on using two IIR filters
 * with different depths (one short / fast, the other long / slow) and on integrating the difference between the
 * two filtered signals. The algorithm uses two thresholds; the touch threshold and the release threshold. The touch
 * threshold is defined in the sensitivity register. The release threshold has twice the lower level than the touch
 * threshold. If the integrated signal is higher than the touch threshold, or lower than the release threshold, then the
 * integrated signal is reset. The Touch state is reported for the electrode when the first touch reset is detected. The
 * Release state is reported, when as many release resets as the touch resets were detected during the previous touch
 * state.
 * \{
 */

#include "../source/filters/nt_filters_prv.h"
#include "../source/keydetectors/nt_keydetector_prv.h"
#include "nt_types.h"
#include "nt_keydetector_afid.h"

/**
 * The initial integration value of the AFID.
 */
#define NT_KEYDETECTOR_AFID_INITIAL_INTEGRATOR_VALUE 0

/**
 * The initial reset counter value of the AFID.
 */
#define NT_KEYDETECTOR_AFID_INITIAL_RESET_TOUCH_COUNTER_VALUE 0

/**
 * The reset threshold value of the AFID.
 */
#define NT_KEYDETECTOR_AFID_INITIAL_RESET_RELEASE_COUNTER_VALUE 10

/**
 * AFID Automatic Sensitive Calibration RAM structure; This structure is used for internal
 * algorithms to store the data while evaluating the AFID.
 * Contains data of the calculating result and auxiliary variables.
 *
 * This structure only manages and uses the internal methods.
 */
struct nt_keydetector_afid_asc_data
{
    int32_t max_resets;
};

/**
 * AFID Ram structure; This structure is used for internal
 * algorithms to store the data while evaluating the AFID.
 * Contains the data of the calculating result and auxiliary variables.
 *
 * This structure only manages and uses the internal methods.
 */
struct nt_keydetector_afid_data
{
    struct nt_filter_fbutt_data fast_signal_filter; /*!< Signal fast butterworth filter data storage. */
    struct nt_filter_fbutt_data slow_signal_filter; /*!< Signal slow butterworth filter data storage. */
    struct nt_filter_moving_average_data base_avrg; /*!< Base line moving average filter data. */
    uint16_t slow_signal;                           /*!< Slow signal value .*/
    struct nt_keydetector_afid_asc_data asc; /*!< Storage of ASC (Automatic sensitive calibration) data for AFID*/
    uint8_t filter_state;                    /*!< State of filter. */
    int32_t integration_value;               /*!< Current value of internal integrator. */
    uint32_t touch_threshold;                /*!< Current threshold value for integrator resets. */
    uint32_t touch_reset_counter;            /*!< Count of touch resets. */
    uint32_t release_reset_counter;          /*!< Count of release events resets. */
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/** \} */ /* end of afid_private group */

#endif /* _NT_KEYDETECTOR_AFID_PRV_H_*/
