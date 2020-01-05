  /*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef NT_KEYDETECTOR_USAFA_H
#define NT_KEYDETECTOR_USAFA_H

 /**
 * \defgroup usafa Usafa Detector
 * \ingroup detectors
 *
 * The usafa key detector is a method for recognizing the touch or release states. It can be used for each type of control.
 *
 * If the measured sample is reported as a valid sample, the module calculates the delta value from the actual signal and
 * baseline values. The delta value is compared to the threshold value computed from the expected signal and baseline values.
 * Based on the result, it determines the electrode state, which can be released, touched, changing from
 * released to touch, and changing from touched to release. This method is using moving average filters to determine the baseline and the expected signal values 
 * with a different depth of the filter, depending on the state of the electrode. The dead band filters in the horizontal and vertical directions are also implemented.
 * \{
 */

#include "nt_types.h"

/**
 * The default Usafa settings of the key detector are:
 * - signal_filter (2 by default) 
 * - base_avrg (9 by default, which means 512 samples) 
 * - non_activity_avrg (NT_FILTER_MOVING_AVERAGE_MAX_ORDER by default, which means 65K samples) 
 * - entry_event_cnt (8 by default) 
 * - signal_to_noise_ratio (16 by default)
 * - deadband (10 by default)
 * - min_noise_limit (20 by default)
 * - dc_track_enabled (enabled by default), to avoid negative signal drop below baseline
 * - dc_track_cnt (100 by default), debouncing period
 * Here is an example definition of the default values for usafa ASC:
  \code
    const struct nt_keydetector_usafa nt_keydetector_usafa_default = 
    {
        .signal_filter = 2,
        .base_avrg = {.n2_order = 9},
        .non_activity_avrg =  {.n2_order = NT_FILTER_MOVING_AVERAGE_MAX_ORDER},
        .entry_event_cnt = 8,
        .signal_to_noise_ratio = 16,
        .deadband = 10,
        .min_noise_limit = 20,
        .dc_track_enabled = 1,
        .dc_track_cnt = 100,
    };
  \endcode
 */
extern const struct nt_keydetector_usafa nt_keydetector_usafa_default;

/**
 * The main structure representing the Usafa key detector.
 * An instance of this data type represents the Usafa key detector. It consists of the used filters' parameters.
 * 
 * You're responsible for the initialization of all members before registering the usafa in
 * the module. This structure can be allocated in ROM.
 */
struct nt_keydetector_usafa {
    struct nt_filter_iir                    signal_filter;  		/**< Coefficient of the input IIR signal filter, used to suppress high-frequency noise. */
    struct nt_filter_moving_average         base_avrg;      		/**< Settings of the moving average filter for the baseline in the release state of an electrode. */
    struct nt_filter_moving_average         non_activity_avrg;  	/**< Settings of the moving average filter for the signals in the inactivity state of an electrode. (for example baseline in a touch state). */
    uint32_t                                entry_event_cnt;    	/**< Sample count for the touch event. This means that this count of samples must meet the touch condition to trigger a real touch event. */
    uint32_t                                deadband_cnt;       	/**< Sample count for the deadband filter. This field specifies the number of samples that cannot proceed to the next event. 
                                                                         For Example: after the touch event, a release event with "deadband_cnt" samples can follow. */
    uint32_t                                signal_to_noise_ratio;  /**< Signal-to-noise ratio – it is used to count the minimum size of the signal that is ignored. */
    uint32_t                                min_noise_limit;    	/**< Minimum noise value. */
    uint32_t                                dc_track_enabled;       /**< Enable or disable DC track feature to avoid negative signal drop below the baseline - used to debounce the short glitches. 
                                                                         If enabled and the signal is under baseline longer then defined in dc_track_cnt value the usafa keydetector will be reset.*/
    uint32_t                                dc_track_cnt;           /**< Define minimum number of cycles when signal must fall down below tha baseline to reset the usafa keydetector if DC track functionality is enabled. */
};

/**
 * USAFA key detector interface structure.
 */
extern const struct nt_keydetector_interface nt_keydetector_usafa_interface;

#ifdef __cplusplus
extern "C" {
#endif

/** \} */ /* end of safa group */

#ifdef __cplusplus
}
#endif

#endif
