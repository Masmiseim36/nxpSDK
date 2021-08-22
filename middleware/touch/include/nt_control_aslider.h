/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_ASLIDER_H_
#define _NT_ASLIDER_H_

/**
 * \defgroup aslider Analog Slider Control
 * \ingroup controls
 *
 * Analog Slider enables detection of linear finger movement using two or more electrodes;
 * it is represented by the nt_control_aslider structure.
 *
 * The Analog Slider Control uses two or more specially-shaped electrodes to enable
 * the calculation of finger position within a linear area. The position algorithm uses
 * ratio of electrode signals to estimate finger position with required precision.
 *
 * The Analog Slider works similarly to the "standard" Slider, but requires less
 * electrodes, while achieving a higher resolution of the calculated position. For
 * example, a two-electrode analog slider can provide finger position detection in the
 * range of 0-127. The shape of the electrodes needs to be designed specifically to achieve
 * a stable signal with a linear dependence on finger movement.
 *
 * The Analog Slider Control provides Position, Direction, and Displacement values. It
 * is able to generate event callbacks when finger Movement, Initial-touch, or Release
 * is detected.
 *
 * The figure below shows a typical two-electrode Analog Slider electrode placement.
 * \image html aslider.png "Analog Slider Electrodes"
 * \image latex aslider.png "Analog Slider Electrodes"
 *
 * \{
 */

#include "nt_controls.h"
#include "nt_types.h"

#define NT_CONTROL_ASLIDER_NAME "nt_control_aslider_interface"

/** Analog Slider event types. */
enum nt_control_aslider_event
{
    NT_ASLIDER_MOVEMENT      = 0, /*!< Finger movement event */
    NT_ASLIDER_ALL_RELEASE   = 1, /*!< Release event */
    NT_ASLIDER_INITIAL_TOUCH = 2, /*!< Initial-touch event */
};

/**
 * Analog Slider event callback function pointer type.
 */
typedef void (*nt_control_aslider_callback)(const struct nt_control *control,
                                            enum nt_control_aslider_event event,
                                            uint32_t position);

/**
 *  The main structure representing the Analog Slider Control.
 *
 *  An instance of this data type represents the Analog Slider Control. You are
 *  responsible to initialize all the members before registering the control in
 *  the system. This structure can be allocated in ROM.
 */
struct nt_control_aslider
{
    uint8_t range;         /*!< Maximum range for the ram->position value. */
    uint8_t insensitivity; /*!< Insensitivity for the callbacks invokes when the position is changed. */
};

/** An interface structure, which contains pointers to the entry points of the Analog
 *  Slider algorithms. A pointer to this structure must be assigned to any
 *  instance of the nt_control_aslider to define the control behaviour. */
extern const struct nt_control_interface nt_control_aslider_interface;

/**
 * \defgroup aslider_api Analog Slider Control API
 * \ingroup aslider
 *
 * These functions can be used to set or get the Analog Slider control properties.
 *
 * Common example definition of the Analog Slider control for all source code examples is as follows:
 * \code
 *  const struct nt_electrode  * const control_0_electrodes[] = {&electrode_0, &electrode_1,
 *    NULL};
 *
 *  const struct nt_control_aslider my_aslider_params =
 *  {
 *   .range = 100,
 *  };
 *
 *  const struct nt_control my_aslider_control =
 *  {
 *    .interface = &nt_control_aslider_control_interface,
 *    .electrodes = control_0_electrodes,
 *    .control_params.aslider = &my_aslider_params,
 *  };
 *
 * \endcode
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Registers the Analog Slider events handler function.
 * \param control  Pointer to the control.
 * \param callback Address of the function to be invoked.
 * \return none
 *
 * Register the specified callback function as the Analog Slider events handler.
 * If the callback parameter is NULL, the callback is disabled.
 * Example:
 * \code
 *
 *  static void my_aslider_cb(const struct nt_control *control,
 *                            enum nt_control_aslider_event event,
 *                            uint32_t position)
 *  {
 *    (void)control;
 *    char* event_names[] =
 *     {
 *      "NT_ASLIDER_MOVEMENT",
 *      "NT_ASLIDER_ALL_RELEASE",
 *      "NT_ASLIDER_INITIAL_TOUCH",
 *      };
      printf("New analog slider control event %s on position: %d.", event_names[event], position);
    }

    nt_control_aslider_register_callback(&my_aslider_control, my_aslider_cb);
   \endcode
 */
void nt_control_aslider_register_callback(const struct nt_control *control, nt_control_aslider_callback callback);

/**
 * \brief Get the Analog Slider 'Position' value.
 * \param control Pointer to the control.
 * \return Position. The returned value is in the range of zero to maximum
 *         value configured in the nt_control_aslider structure.
 *
 * This function retrieves the actual finger position value.
 * Example:
 * \code
 * uint32_t position;
 * position = nt_control_aslider_get_position(&my_aslider_control);
 * printf("Position of analog slider control is: %d.", position);
 * \endcode
 */
uint32_t nt_control_aslider_get_position(const struct nt_control *control);

/**
 * \brief Get 'Touched' state.
 * \param control Pointer to the control.
 * \return Non-zero value, when the control is currently touched.
 * Example:
 * \code
 * uint32_t touched;
 * touched = nt_control_aslider_is_touched(&my_aslider_control);
 * if(touched)
 *      printf("The Analog Slider control is currently touched.");
 * else
 *      printf("The Analog Slider control is currently not touched.);
 * \endcode
 */
uint32_t nt_control_aslider_is_touched(const struct nt_control *control);

/**
 * \brief Get 'Movement' flag.
 * \param control Pointer to the control.
 * \return Non-zero value, if the control currently detects finger movement.
 * Example:
 * \code
 * uint32_t movement;
 * movement = nt_control_aslider_movement_detected(&my_aslider_control);
 * if(movement)
 *      printf("The Analog Slider control is currently moving.");
 * else
 *      printf("The Analog Slider control is currently not moving.);
 * \endcode
 */
uint32_t nt_control_aslider_movement_detected(const struct nt_control *control);

/**
 * \brief Get 'Direction' flag.
 * \param control Pointer to the control.
 * \return Non-zero value, when a movement towards higher
 *         values is detected. Returns zero when a movement towards zero is detected.
 * Example:
 * \code
 * uint32_t direction;
 * direction = nt_control_aslider_get_direction(&my_aslider_control);
 * if(direction)
 *      printf("The Analog Slider direction is left.");
 * else
 *      printf("The Analog Slider direction is right.");
 * \endcode
 */
uint32_t nt_control_aslider_get_direction(const struct nt_control *control);

/**
 * \brief Returns invalid position flag.
 * \param control Pointer to the control.
 * \return Non-zero value, when an invalid touch is detected.
 *
 * This function works only in the Analog Slider controls, consisting of at least
 * three electrodes. This flag is set when the algorithm detects two or more fingers
 * touching the electrodes that are not adjacent to each other.
 * Example:
 * \code
 * uint32_t invalid_position;
 * invalid_position = nt_control_aslider_get_invalid_position(&my_aslider_control);
 * if(invalid_position)
 *      printf("The Analog Slider control has an invalid position (two fingers touch ?).");
 * else
 *      printf("The Analog Slider control has a valid position.");
 * \endcode
 */
uint32_t nt_control_aslider_get_invalid_position(const struct nt_control *control);

#ifdef __cplusplus
}
#endif

/** \} end of aslider_api group */
/** \} end of aslider group */

#endif
