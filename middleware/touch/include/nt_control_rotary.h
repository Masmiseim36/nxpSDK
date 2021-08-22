/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_ROTARY_H_
#define _NT_ROTARY_H_

/**
 * \defgroup rotary Rotary Control
 * \ingroup controls
 *
 * The Rotary control enables the detection of jog-dial-like finger movement using discrete
 * electrodes; it is represented by the nt_control_rotary_control structure.
 *
 * The Rotary control uses a set of discrete electrodes to enable the calculation of finger
 * position within a circular area. The position algorithm localizes the touched
 * electrode and its sibling electrodes, to estimate the finger position. A Rotary control consisting
 * of N electrodes enables the rotary position to be calculated in 2N steps.
 *
 * The Rotary control provides Position, Direction, and Displacement values. It
 * is able to generate event callbacks when the finger Movement, Initial-touch, or Release is detected.
 *
 * The figure below shows a typical Rotary electrode placement.
 * \image html rotary.png "Rotary Electrodes"
 * \image latex rotary.png "Rotary Electrodes"
 *
 * \{
 */

#include "nt_controls.h"
#include "nt_types.h"

#define NT_CONTROL_ROTARY_NAME "nt_control_rotary_interface"

/** Rotary event types. */
enum nt_control_rotary_event
{
    NT_ROTARY_MOVEMENT      = 0, /*!< Finger movement event */
    NT_ROTARY_ALL_RELEASE   = 1, /*!< Release event */
    NT_ROTARY_INITIAL_TOUCH = 2, /*!< Initial-touch event */
};

/**
 * Rotary event callback function pointer type.
 */
typedef void (*nt_control_rotary_callback)(const struct nt_control *control,
                                           enum nt_control_rotary_event event,
                                           uint32_t position);

/** The interface structure, which contains pointers to the entry points of the
 *  Rotary algorithms. A pointer to this structure must be assigned to any
 *  instance of nt_control_rotary_control to define the control behaviour. */
extern const struct nt_control_interface nt_control_rotary_interface;

/**
 * \defgroup rotary_api Rotary Control API
 * \ingroup rotary
 *
 * These functions can be used to set or get the Rotary control properties.
 *
 * The common example definition of Rotary control for all source code examples is as follows:
 * \code
 *  const struct nt_electrode  * const control_0_electrodes[] = {&electrode_0, &electrode_1,
 *    &electrode_2, &electrode_3, NULL};
 *
 *  const struct nt_control my_rotary_control =
 *  {
 *    .interface = &nt_control_rotary_interface,
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

 * Register the specified callback function as the Rotary events handler.
 * Example:
 * \code
 *
 *  static void my_rotary_cb(const struct nt_control *control,
 *                           enum nt_control_arotary_event event,
 *                           uint32_t position)
 *  {
 *    (void)control;
 *    char* event_names[] =
 *     {
 *      "NT_ROTARY_MOVEMENT",
 *      "NT_ROTARY_ALL_RELEASE",
 *      "NT_ROTARY_INITIAL_TOUCH",
 *      };
 *    printf("New rotary control event %s on position: %d.", event_names[event], position);
 *  }
 *
 *  nt_control_rotary_register_callback(&my_rotary_control, my_rotary_cb);
 * \endcode
 */
void nt_control_rotary_register_callback(const struct nt_control *control, nt_control_rotary_callback callback);

/**
 * \brief Get the Rotary 'Position' value.
 * \param control Pointer to the control.
 * \return Position. The returned value is in the range of zero to 2N-1, where N is
 *     the number of electrodes assigned to Rotary control.
 *
 * This function retrieves the actual finger position value.
 * Example:
 * \code
 * uint32_t position;
 * position = nt_control_rotary_get_position(&my_rotary_control);
 * printf("Position of Rotary control is: %d.", position);
 * \endcode
 */
uint32_t nt_control_rotary_get_position(const struct nt_control *control);

/**
 * \brief Get 'Touched' state.
 * \param control Pointer to the control.
 * \return Non-zero value, when the control is currently touched.
 * Example:
 * \code
 * uint32_t touched;
 * touched = nt_control_rotary_is_touched(&my_rotary_control);
 * if(touched)
 *      printf("The Rotary control is currently touched.");
 * else
 *      printf("The Rotary control is currently not touched.);
 * \endcode
 */
uint32_t nt_control_rotary_is_touched(const struct nt_control *control);

/**
 * \brief Get 'Movement' flag.
 * \param control Pointer to the control.
 * \return Non-zero value, when the control detects finger movement.
 * Example:
 * \code
 * uint32_t movement;
 * movement = nt_control_rotary_movement_detected(&my_rotary_control);
 * if(movement)
 *      printf("The Rotary control is currently moving.");
 * else
 *      printf("The Rotary control is currently not moving.);
 * \endcode
 */
uint32_t nt_control_rotary_movement_detected(const struct nt_control *control);

/**
 * \brief Get 'Direction' flag.
 * \param control Pointer to the control.
 * \return Non-zero value, when a movement is detected in a direction towards higher
 *         values. Returns zero, when a movement is detected towards zero.
 * Example:
 * \code
 * uint32_t direction;
 * direction = nt_control_rotary_get_direction(&my_rotary_control);
 * if(direction)
 *      printf("The Rotary direction is left.");
 * else
 *      printf("The Rotary direction is right.");
 * \endcode
 */
uint32_t nt_control_rotary_get_direction(const struct nt_control *control);

/**
 * \brief Get 'Invalid' flag.
 * \param control Pointer to the control.
 * \return Non-zero value when an invalid position was detected,
 *         otherwise a zero value.
 * Example:
 * \code
 * uint32_t invalid_position;
 * invalid_position = nt_control_rotary_get_invalid_position(&my_rotary_control);
 * if(invalid_position)
 *      printf("The Rotary control has an invalid position (two fingers touch ?).");
 * else
 *      printf("The Rotary control has a valid position.");
 * \endcode
 */
uint32_t nt_control_rotary_get_invalid_position(const struct nt_control *control);

#ifdef __cplusplus
}
#endif

/** \} */ /* end of rotary_api group */
/** \} */ /* end of rotary group */

#endif
