/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_ASLIDER_PRV_H_
#define _NT_ASLIDER_PRV_H_

/**
 * \defgroup aslider_private Analog Slider Control
 * \ingroup controls_private
 *
 * Analog Slider enables the detection of linear finger movement using two or more electrodes;
 * it is represented by the nt_control_aslider structure.
 *
 * An Analog Slider Control uses two or more specially-shaped electrodes to enable
 * the calculation of finger position within a linear area. The position algorithm uses
 * a ratio of electrode signals to estimate the finger position with required precision.
 *
 * The Analog Slider works similarly to the "standard" Slider, but requires less
 * electrodes, while achieving a higher resolution of the calculated position. For
 * example, a two-electrode analog slider can provide finger position detection in the
 * range of 0-127. The shape of the electrodes needs to be designed specifically to achieve
 * stable signal with a linear dependence on finger movement.
 *
 * The Analog Slider Control provides Position, Direction, and Displacement values. It
 * is able to generate event callbacks when finger Movement, Initial-touch, or Release
 * is detected.
 *
 * The image below shows a typical two-electrode Analog Slider electrode placement.
 * \image html aslider.png "Analog Slider Electrodes"
 * \image latex aslider.png "Analog Slider Electrodes"
 *
 * \{
 */

/**
 * Value that is used to mark an invalid position of analog slider.
 */
#define NT_ASLIDER_INVALID_POSITION_VALUE 65535U

/**
 * Analogue Slider flags.
 */
enum nt_control_aslider_flags
{
    NT_ASLIDER_INVALID_POSITION_FLAG = 1 << NT_FLAGS_SPECIFIC_SHIFT(0), /*!< Analog Slider invalid position flag. */
    NT_ASLIDER_DIRECTION_FLAG        = 1 << NT_FLAGS_SPECIFIC_SHIFT(1), /*!< Analog Slider direction flag. */
    NT_ASLIDER_MOVEMENT_FLAG         = 1 << NT_FLAGS_SPECIFIC_SHIFT(2), /*!< Analog Slider movement flag.*/
    NT_ASLIDER_TOUCH_FLAG            = 1 << NT_FLAGS_SPECIFIC_SHIFT(3), /*!< Analog Slider touch flag. */
};

/**
 *  Analog Slider RAM structure used to store volatile parameters of the control.
 *
 *  You need to allocate this structure and put a pointer into the nt_control_aslider
 *  structure when it is being registered in the system.
 */
struct nt_control_aslider_data
{
    nt_control_aslider_callback callback; /*!< Analog Slider callback handler. */
    uint8_t position;                     /*!< Position. */
};

/**
 *  Analog Slider help structure to handle temporary values
 */
struct nt_control_aslider_temp_data
{
    uint32_t active_el_ix; /*!< Index of electrode of first active electrode. */
    uint32_t first_delta;  /*!< Value of first delta (signal - baseline). */
    uint32_t range;        /*!< tmp. value of slider range */
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/** \} end of aslider_private group */

#endif /* _NT_ASLIDER_PRV_H_ */
