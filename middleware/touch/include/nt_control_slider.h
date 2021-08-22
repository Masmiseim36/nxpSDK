/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_SLIDER_H_
#define _NT_SLIDER_H_

/**
 * \defgroup slider Slider control
 * \ingroup controls
 *
 * Slider control enables the detection of linear finger movement using discrete electrodes;
 * it is represented by the nt_control structure.
 *
 * The Slider control uses a set of discrete electrodes to enable calculation of the finger
 * position within a linear area. The position algorithm localizes the touched
 * electrode and its sibling electrodes to estimate finger position. A Slider consisting
 * of N electrodes enables the position to be calculated in 2N-1 steps.
 *
 * The Slider control provides Position, Direction, and Displacement values. It
 * is able to generate event callbacks when finger Movement, Initial-touch, or Release
 * is detected.
 *
 * The image below shows a typical Slider electrode placement.
 * \image html slider.png "Slider Electrodes"
 * \image latex slider.png "Slider Electrodes"
 *
 * \{
 */

#include "nt_controls.h"
#include "nt_types.h"

#define NT_CONTROL_SLIDER_NAME "nt_control_slider_interface"

/** Slider event types. */
enum nt_control_slider_event
{
    NT_SLIDER_MOVEMENT      = 0, /*!< Finger movement event. */
    NT_SLIDER_ALL_RELEASE   = 1, /*!< Release event. */
    NT_SLIDER_INITIAL_TOUCH = 2, /*!< Initial-touch event. */
};

/* forward declarations */

/**
 * Slider event callback function pointer type.
 */
typedef void (*nt_control_slider_callback)(const struct nt_control *control,
                                           enum nt_control_slider_event event,
                                           uint32_t position);

/** An interface structure, which contains pointers to the entry points of
 *  Slider algorithms. A pointer to this structure must be assigned to any
 *  instace of nt_control_slider_control to define the control behaviour. */
extern const struct nt_control_interface nt_control_slider_interface;

/**
 * \defgroup slider_api Slider Control API
 * \ingroup slider
 *
 * These functions can be used to set or get the Slider control properties.
 *
 * A common example definition of the Slider control for all source code examples is as follows:
 * \code
 *  const struct nt_electrode  * const control_0_electrodes[] = {&electrode_0, &electrode_1,
 *    NULL};
 *
 *  const struct nt_control my_slider_control =
 *  {
 *    .interface = &nt_control_slider_interface,
 *    .electrodes = control_0_electrodes,
 *  };
 *
 * \endcode
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Registers the events handler function.
 * \param control  Pointer to the control.
 * \param callback Address of function to be invoked.
 * \return none
 *
 * Register the specified callback function as the Slider events handler.
 * Example:
 * \code
 *
 *  static void my_slider_cb(const struct nt_control *control,
 *                            enum nt_control_aslider_event event,
 *                            uint32_t position)
 *  {
 *    (void)control;
 *    char* event_names[] =
 *     {
 *      "NT_SLIDER_MOVEMENT",
 *      "NT_SLIDER_ALL_RELEASE",
 *      "NT_SLIDER_INITIAL_TOUCH",
 *      };
 *    printf("New slider control event %s on position: %d.", event_names[event], position);
 *  }
 *
 *  nt_control_slider_register_callback(&my_slider_control, my_slider_cb);
 * \endcode
 */
void nt_control_slider_register_callback(const struct nt_control *control, nt_control_slider_callback callback);
/**
 * \brief Get the Slider 'Position' value.
 * \param control Pointer to the control.
 * \return Position. The returned value is in the range of zero to the maximum
 *         value configured in the nt_control structure.
 *
 * This function retrieves the actual finger position value.
 * Example:
 * \code
 * uint32_t position;
 * position = nt_control_slider_get_position(&my_slider_control);
 * printf("Position of Slider control is: %d.", position);
 * \endcode
 */
uint32_t nt_control_slider_get_position(const struct nt_control *control);

/**
 * \brief Get 'Touched' state.
 * \param control Pointer to the control.
 * \return Non-zero value, when the control is currently touched.
 * Example:
 * \code
 * uint32_t touched;
 * touched = nt_control_slider_is_touched(&my_slider_control);
 * if(touched)
 *      printf("The Slider control is currently touched.");
 * else
 *      printf("The Slider control is currently not touched.");
 * \endcode
 */
uint32_t nt_control_slider_is_touched(const struct nt_control *control);

/**
 * \brief Get 'Movement' flag.
 * \param control Pointer to the control.
 * \return Non-zero value, when the control detects finger movement.
 * Example:
 * \code
 * uint32_t movement;
 * movement = nt_control_slider_movement_detected(&my_slider_control);
 * if(movement)
 *      printf("The Slider control is currently moving.");
 * else
 *      printf("The Slider control is currently not moving.");
 * \endcode
 */
uint32_t nt_control_slider_movement_detected(const struct nt_control *control);

/**
 * \brief Get 'Direction' flag.
 * \param control Pointer to the control.
 * \return Non-zero value, when a movement is detected in a direction towards higher
 *         values. Returns zero, when a movement towards zero is detected.
 * Example:
 * \code
 * uint32_t direction;
 * direction = nt_control_slider_get_direction(&my_slider_control);
 * if(direction)
 *      printf("The Slider direction is left.");
 * else
 *      printf("The Slider direction is right.");
 * \endcode
 */
uint32_t nt_control_slider_get_direction(const struct nt_control *control);

/**
 * \brief Get 'Invalid' flag.
 * \param control Pointer to the control.
 * \return Non-zero value, when an invalid position was detected,
 *         otherwise zero.
 * Example:
 * \code
 * uint32_t invalid_position;
 * invalid_position = nt_control_slider_get_invalid_position(&my_slider_control);
 * if(invalid_position)
 *      printf("The Slider control has an invalid position (two fingers touch ?).");
 * else
 *      printf("The Slider control has a valid position.");
 * \endcode
 */
uint32_t nt_control_slider_get_invalid_position(const struct nt_control *control);

/** \} end of slider_api group */
/** \} end of slider group */

#ifdef __cplusplus
}
#endif

#endif
