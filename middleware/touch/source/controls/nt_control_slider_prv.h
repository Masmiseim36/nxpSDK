/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_SLIDER_PRV_H_
#define _NT_SLIDER_PRV_H_

/**
 * \defgroup slider_private Slider Control
 * \ingroup controls_private
 *
 * Slider control enables the detection of a linear finger movement using discrete electrodes;
 * it is represented by the nt_control structure.
 *
 * The Slider Control uses a set of discrete electrodes to enable the calculation of finger
 * position within a linear area. The position algorithm localizes the touched
 * electrode and its sibling electrodes to estimate the finger position. The Slider consisting
 * of N electrodes enables the position to be calculated in 2N-1 steps.
 *
 * The Slider Control provides Position, Direction, and Displacement values. It
 * is able to generate event callbacks when finger Movement, Initial-touch, or Release
 * is detected.
 *
 * The image below shows a typical Slider electrode placement.
 * \image html slider.png "Slider Electrodes"
 * \image latex slider.png "Slider Electrodes"
 *
 * \{
 */

/**
 *Slider flags.
 */
enum nt_control_slider_flags
{
    NT_SLIDER_INVALID_POSITION_FLAG = 1 << NT_FLAGS_SPECIFIC_SHIFT(0), /*!< Slider invalid position flag. */
    NT_SLIDER_DIRECTION_FLAG        = 1 << NT_FLAGS_SPECIFIC_SHIFT(1), /*!< Slider direction flag. */
    NT_SLIDER_MOVEMENT_FLAG         = 1 << NT_FLAGS_SPECIFIC_SHIFT(2), /*!< Slider movement flag.*/
    NT_SLIDER_TOUCH_FLAG            = 1 << NT_FLAGS_SPECIFIC_SHIFT(3), /*!< Slider touch flag. */
};

/**
 *  Slider RAM structure used to store volatile parameters of the control.
 *
 *  You must allocate this structure and put a pointer into the nt_control_slider_control
 *  structure when it is being registered in the system.
 */
struct nt_control_slider_data
{
    nt_control_slider_callback callback; /*!< Slider Callback handler. */
    uint8_t position;                    /*!< Position. */
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/** \} end of slider_private group */

#endif /* _NT_SLIDER_PRV_H_ */
