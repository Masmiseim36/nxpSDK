/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_ROTARY_PRV_H_
#define _NT_ROTARY_PRV_H_

/**
 * \defgroup rotary_private Rotary Control
 * \ingroup controls_private
 *
 * Rotary enables the detection of jog-dial-like finger movement using discrete
 * electrodes; it is represented by the nt_control_rotary_control structure.
 *
 * The Rotary Control uses a set of discrete electrodes to enable the calculation of finger
 * position within a circular area. The position algorithm localizes the touched
 * electrode and its sibling electrodes to estimate the finger position. The Rotary consisting
 * of N electrodes enables the rotary position to be calculated in 2N steps.
 *
 * The Rotary Control provides Position, Direction, and Displacement values. It
 * is able to generate event callbacks when finger Movement, Initial-touch, or Release is detected.
 *
 * The image below shows a typical Rotary electrode placement.
 * \image html rotary.png "Rotary Electrodes"
 * \image latex rotary.png "Rotary Electrodes"
 *
 * \{
 */

/**
 * Rotary flags.
 */
enum nt_control_rotary_flags
{
    NT_ROTARY_INVALID_POSITION_FLAG = 1 << NT_FLAGS_SPECIFIC_SHIFT(0), /*!< Rotary invalid position flag. */
    NT_ROTARY_DIRECTION_FLAG        = 1 << NT_FLAGS_SPECIFIC_SHIFT(1), /*!< Rotary direction flag. */
    NT_ROTARY_MOVEMENT_FLAG         = 1 << NT_FLAGS_SPECIFIC_SHIFT(2), /*!< Rotary movement flag.*/
    NT_ROTARY_TOUCH_FLAG            = 1 << NT_FLAGS_SPECIFIC_SHIFT(3), /*!< Rotary touch flag. */
};

/**
 *  Rotary RAM structure used to store volatile parameters of the control.
 *
 *  You must allocate this structure and put a pointer into the nt_control_rotary_control
 *  structure when it is being registered in the system.
 */
struct nt_control_rotary_data
{
    nt_control_rotary_callback callback; /*!< Callback handler. */
    uint8_t position;                    /*!< Position. */
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/** \} */ /* end of rotary_private group */

#endif /* _NT_ROTARY_PRV_H_ */
