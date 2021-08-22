/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_AROTARY_H_
#define _NT_AROTARY_H_

/**
 * \defgroup arotary Analog Rotary Control
 * \ingroup controls
 *
 * Analog Rotary enables the detection of jog-dial-like finger movement using three or more electrodes;
 * it is represented by the nt_control structure.
 *
 * The Analog Rotary Control uses three or more specially-shaped electrodes to enable
 * the calculation of finger position within a circular area. The position algorithm uses the
 * ratio of sibling electrode signals to estimate the finger position with the required precision.
 *
 * The Analog Rotary works similarly to the "standard" Rotary, but requires less
 * electrodes, while achieving a higher resolution of the calculated position. For
 * example, a four-electrode Analog Rotary can provide the finger position detection in the
 * range of 0-64. The shape of the electrodes needs to be designed specifically to achieve
 * a stable signal with a linear dependence on finger movement.
 *
 * The Analog Rotary Control provides Position, Direction, and Displacement values. It
 * is able to generate event callbacks when finger Movement, Initial-touch, or Release is detected.
 *
 * The image below shows a typical four-electrode Analog Rotary electrode placement.
 * \image html arotary.png "Analog Rotary Electrodes"
 * \image latex arotary.png "Analog Rotary Electrodes"
 *
 * \{
 */

#include "nt_controls.h"
#include "nt_types.h"

#define NT_CONTROL_AROTARY_NAME "nt_control_arotary_interface"

/** Analog Rotary event types. */
enum nt_control_arotary_event
{
    NT_AROTARY_MOVEMENT      = 0, /*!< Finger movement event. */
    NT_AROTARY_ALL_RELEASE   = 1, /*!< Release event. */
    NT_AROTARY_INITIAL_TOUCH = 2, /*!< Initial-touch event. */
};

/**
 * Analog Rotary event callback function pointer type.
 */
typedef void (*nt_control_arotary_callback)(const struct nt_control *control,
                                            enum nt_control_arotary_event event,
                                            uint32_t position);

/**
 *  The main structure representing the Analog Rotary Control.
 *
 *  An instance of this data type represents the Analog Rotary Control. You are
 *  responsible to initialize all the members before registering the control in
 *  the system. This structure can be allocated in ROM.
 *
 */
struct nt_control_arotary
{
    uint8_t range; /*!< Range. */
};

/** An interface structure, which contains pointers to the entry points of the Analog
    Rotary algorithms. A pointer to this structure must be assigned to any
    instance of the nt_control to define the control behaviour. */
extern const struct nt_control_interface nt_control_arotary_interface; /*!< Can't be NULL. */

/**
 * \defgroup arotary_api Analog Rotary Control API
 * \ingroup arotary

 * These functions can be used to set or get the Analog Rotary control properties.

 * A common example definition of the Analog Rotary control for all source code examples is as follows:
 * \code
 *  const struct nt_electrode  * const control_0_electrodes[] = {&electrode_0, &electrode_1,
 *    &electrode_2, &electrode_3, NULL};

 *  const struct nt_control_arotary my_arotary_params =
 *  {
 *   .range = 255,
 *  };

 *  const struct nt_control my_arotary_control =
 *  {
 *    .interface = &nt_control_arotary_control_interface,
 *    .electrodes = control_0_electrodes,
 *    .control_params.arotary = &my_arotary_params,
 *  };

 * \endcode
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Registers the Analog Rotary events handler function.
 * \param control  Pointer to the control.
 * \param callback Address of function to be invoked.
 * \return none
 *
 * Register the specified callback function as the Analog Rotary events handler.
 * If the callback parameter is NULL, the callback is disabled.
 * Example:
 * \code
 *
 *  static void my_arotary_cb(const struct nt_control *control,
 *                            enum nt_control_arotary_event event,
 *                            uint32_t position)
 *  {
 *    (void)control;
 *    char* event_names[] =
 *     {
 *      "NT_AROTARY_MOVEMENT",
 *      "NT_AROTARY_ALL_RELEASE",
 *      "NT_AROTARY_INITIAL_TOUCH",
 *      };
 *    printf("New analog rotary control event %s on position: %d.", event_names[event], position);
 *  }
 *
 *  nt_control_arotary_register_callback(&my_arotary_control, my_arotary_cb);
 * \endcode
 */
void nt_control_arotary_register_callback(const struct nt_control *control, nt_control_arotary_callback callback);

/**
 * \brief Get the Analog Rotary 'Position' value.
 * \param control Pointer to the control.
 * \return Position. The returned value is in the range of zero to the maximum
 *         value configured in the nt_control structure.
 *
 * This function retrieves the actual finger position value.
 * Example:
 * \code
 * uint32_t position;
 * position = nt_control_arotary_get_position(&my_arotary_control);
 * printf("Position of Analog Rotary control is: %d.", position);
 * \endcode
 */
uint32_t nt_control_arotary_get_position(const struct nt_control *control);

/**
 * \brief Get 'Touched' state.
 * \param control Pointer to the control.
 * \return Non-zero value, if the control is currently touched.
 * Example:
 * \code
 * uint32_t touched;
 * touched = nt_control_arotary_is_touched(&my_arotary_control);
 * if(touched)
 *      printf("The Analog Rotary control is currently touched.");
 * else
 *      printf("The Analog Rotary control is currently not touched.);
 * \endcode
 */
uint32_t nt_control_arotary_is_touched(const struct nt_control *control);

/**
 * \brief Get 'Movement' flag.
 * \param control Pointer to the control.
 * \return Non-zero value, if the control currently detects finger movement.
 * Example:
 * \code
 * uint32_t movement;
 * movement = nt_control_arotary_movement_detected(&my_arotary_control);
 * if(movement)
 *      printf("The Analog Rotary control is currently moving.");
 * else
 *      printf("The Analog Rotary control is currently not moving.);
 * \endcode
 */
uint32_t nt_control_arotary_movement_detected(const struct nt_control *control);

/**
 * \brief Get 'Direction' flag.
 * \param control Pointer to the control.
 * \return
 *   - Non-zero value, if a movement towards higher values is detected.
 *   - Returns zero, if a movement towards zero is detected.
 * Example:
 * \code
 * uint32_t direction;
 * direction = nt_control_arotary_get_direction(&my_arotary_control);
 * if(direction)
 *      printf("The Analog Rotary direction is left.");
 * else
 *      printf("The Analog Rotary direction is right.");
 * \endcode
 */
uint32_t nt_control_arotary_get_direction(const struct nt_control *control);

/**
 * \brief Returns invalid position flag.
 * \param control Pointer to the control.
 * \return Non-zero value, if an invalid touch is detected.
 *
 * This flag is set, if the algorithm detects two or more fingers touching the
 * electrodes, which are not adjacent to each other.
 * Example:
 * \code
 * uint32_t invalid_position;
 * invalid_position = nt_control_arotary_get_invalid_position(&my_arotary_control);
 * if(invalid_position)
 *      printf("The Analog Rotary control has an invalid position (two fingers touch ?).");
 * else
 *      printf("The Analog Rotary control has a valid position.");
 * \endcode
 */
uint32_t nt_control_arotary_get_invalid_position(const struct nt_control *control);

/** \} end of arotary_api group */
/** \} end of arotary group */

#ifdef __cplusplus
}
#endif

#endif
