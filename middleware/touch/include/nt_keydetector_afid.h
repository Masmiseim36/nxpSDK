/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_KEYDETECTOR_AFID_H_
#define _NT_KEYDETECTOR_AFID_H_

/**
 * \defgroup afid Advanced Filtering and Integrating Detection
 * \ingroup detectors
 * The AFID (Advanced Filtering and Integrating Detection) key detector is based on using two IIR filters
 * with different depths (one short / fast, the other long / slow) and on integrating the difference between the
 * two filtered signals. The algorithm uses two thresholds: the touch threshold and the release threshold. The touch
 * threshold is defined in the sensitivity register. The release threshold has a twice lower level than the touch
 * threshold. If the integrated signal is higher than the touch threshold, or lower than the release threshold, then the
 * integrated signal is reset. The touch state is reported for the electrode when the first touch reset is detected. The
 * release state is reported when as many release resets are detected as the touch resets were detected during the
 * previous touch state.
 * \{
 */
#include "nt_types.h"

/**
 * AFID Automatic Sensitive Calibration default ASC settings:
 * - touch_threshold_fall_rate (default 255)
 * - noise_resets_minimum (default 128)
 * - resets_for_touch (default 6)
 * This default values for AFID ASC definition for example:
  \code
  const struct nt_keydetector_afid keydec =
  {
      .signal_filter = {
          .cutoff = 8
      },
      .baseline_filter = {
          .cutoff = 4
      },
      .reset_rate = 1000,
      .asc = NT_KEYDETECTOR_AFID_ASC_DEFAULT,
  };
  \endcode
 */
#define NT_KEYDETECTOR_AFID_ASC_DEFAULT                                                        \
    {                                                                                          \
        .touch_threshold_fall_rate = 1000, .noise_resets_minimum = 128, .resets_for_touch = 10 \
    }

/**
 * AFID Automatic Sensitive Calibration structure; This structure is used to define the parameters of
 * evaluating the AFID process flow.
 * You can manage your own setup of parameters, or use the default setting in the \ref NT_KEYDETECTOR_AFID_ASC_DEFAULT.
 * This structure must be filled in.
 */
struct nt_keydetector_afid_asc
{
    int16_t touch_threshold_fall_rate; /*!< Rate of how often the touch threshold can fall. The value represent the time
                                          period after each the touch threshold will fall down if touch threshold is
                                          higher then destination threshold */
    uint32_t noise_resets_minimum; /*!< Noise Resets Minimum. If touch threshold is lower then noise reset minimum (low
                                      sensitivity) the touch threshold will be limited to noise level minimum value. */
    int16_t resets_for_touch;      /*!< Number of resets required for touch. The low number causes quicker touch event
                                      recognition, high number causes longer integration process. */
};

/**
 * The main structure representing the AFID key detector.
 * An instance of this data type represents the AFID key detector. Consisting of parameters
 * of filters, the AFID automatic sensitive calibration, and update rate.
 *
 * You're responsible to initialize all the members before registering the AFID in
 * the module. This structure can be allocated in ROM.
 */
struct nt_keydetector_afid
{
    struct nt_filter_iir
        signal_filter; /*!< Coefficient of the input IIR signal filter, used to suppress high frequency noise. */
    struct nt_filter_fbutt fast_signal_filter; /*!< Signal butterworth signal (fast).*/
    struct nt_filter_fbutt slow_signal_filter; /*!< Baseline butterworth signal (slow).*/
    struct nt_filter_moving_average
        base_avrg; /*!< Settings of the moving average filter for the baseline in release state of electrode. */
    struct nt_keydetector_afid_asc asc; /*!< Automatic Sensitive Calibration structure for the AFID detector. */
    uint16_t reset_rate; /*!< Reset rate of the AFID. Cannot be 0. It can clear integration value to avoid the long-term
                            temperature drift */
};

/**
 * AFID key detector interface structure.
 */
extern const struct nt_keydetector_interface nt_keydetector_afid_interface;

#ifdef __cplusplus
extern "C" {
#endif

/** \} */ /* end of the afid group */

#ifdef __cplusplus
}
#endif

#endif
