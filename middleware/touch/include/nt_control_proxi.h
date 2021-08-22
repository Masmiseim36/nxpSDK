/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_PROXI_H_
#define _NT_PROXI_H_

/**
 * \defgroup proxi Proxi Control
 * \ingroup controls
 *
 * Proxi implements object presence detection in the near field (approaching finger or hand),
 * it is represented by the nt_control_proxi structure.
 *
 * The Proxi Control provides the proximity Key status values and is able to generate Proxi Touch,
 * Movement, and Release events.
 *
 * The figures below show simple and grouped Proxi electrode layouts.
 * \image html proxi.png "Proxi Electrodes"
 * \image latex proxi.png "Proxi Electrodes"
 *
 * \{
 */

#include "nt_controls.h"
#include "nt_types.h"

#define NT_CONTROL_PROXI_NAME "nt_control_proxi_interface"

/** Proxi event types. */
enum nt_control_proxi_event
{
    NT_PROXI_MOVEMENT    = 0, /*!< Release event */
    NT_PROXI_RELEASE     = 1, /*!< Key-touch event */
    NT_PROXI_TOUCH       = 2, /*!< Proximity movement event */
    NT_PROXI_SET_GESTURE = 3, /*!< Gesture recognized event event */
    NT_PROXI_GET_GESTURE = 4, /*!< Gesture stored event event */
};

/**
 * Proxi event callback function pointer type.
 */
typedef void (*nt_control_proxi_callback)(const struct nt_control *control,
                                          enum nt_control_proxi_event event,
                                          uint32_t index_or_gesture,
                                          uint32_t proximity);

/**
 *  The main structure representing the Proxi Control.
 *
 *  An instance of this data type represents the Proxi Control. You must initialize all the members before registering
 *  the control in the system. This structure can be allocated in ROM.
 *
 */
struct nt_control_proxi
{
    uint32_t *gesture;     /*!< Stored proxi gesture array. The gesture means the time change of activated
                                proxi electrodes written in hexadecimal format from right to left. For example,
                                gesture 0x321 means that firstly electrode 1 was touched, then electrode 2 and 3.*/
    uint16_t scale;        /*!< Max signal delta level for max. prox (100%) value */
    uint8_t range;         /*!< Proxi scale (i.e. 0-100% or 0-255) value. */
    uint8_t insensitivity; /*!< Insensitivity for the callbacks invokes when the position is changed within selected
                              range. */
    uint8_t threshold;     /*!< Proxi Touch/Release threshold value from within selected range. */
};

/** An interface structure, which contains pointers to the entry points of the Proxi
 *  algorithms. A pointer to this structure must be assigned to any
 *  instance of the nt_control_proxi, to define the control behaviour. */
extern const struct nt_control_interface nt_control_proxi_interface;
/**
 * \defgroup proxi_api Proxi Control API
 * \ingroup proxi
 *
 * These functions can be used to set or get the Proxi control properties.
 *
 * A common example definition of the Proxi control for all source code examples is as follows:
 * \code
 * #define NT_PROXI_ELECTRODES 4
 * #define NT_PROXI_GESTURES 3
 *
 * const struct nt_electrode  * const control_0_electrodes[] = {&electrode_0, &electrode_1,
 *   NULL};
 *
 * const struct nt_control_proxi proxi_params =
 * {
 *   .range = 5000,
 *   .scale = 100,
 *   .threshold = 3,
 *   .insensitivity = 2,
 *   .gesture = (uint32_t []){0x0123, 0x3210, 0},
 * };
 *
 * const struct nt_control proxi_0 =
 * {
 *   .interface = &nt_control_proxi_interface,
 *   .electrodes = control_0_electrodes,
 *   .control_params.proxi = &proxi_params,
 * };
 *
 * \endcode
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Registers the Proxi event handler function.
 * \param control  Pointer to the control.
 * \param callback Address of function to be invoked.
 * \return none
 *
 * Register the specified callback function as the KeyPad event handler.
 * If the callback parameter is NULL, the callback is disabled.
 * Example:
 * \code
 *
 *  static void my_proxi_cb(const struct nt_control *control,
 *                           enum nt_control_proxi_event event,
 *                           uint32_t index_or_gesture,
 *                           uint32_t proximity)
 *  {
 *    (void)control;
 *     char* event_names[] =
 *     {
 *      "NT_PROXI_MOVEMENT",
 *      "NT_PROXI_RELEASE",
 *      "NT_PROXI_TOUCH",
 *      "NT_PROXI_GESTURE_RECOGNIZED",
 *      };
 *
 *    printf("New proxi control event %s on key: %d.", event_names[event], index);
 *  }
 *
 *  nt_control_proxi_register_touch_callback(&my_proxi_control, my_proxi_touch_cb);
 * \endcode
 */
void nt_control_proxi_register_callback(const struct nt_control *control, nt_control_proxi_callback callback);

/**
 * \brief Enable proxi control to store unlock gesture.
 * \param control Pointer to the Proxi control.
 * \param gesture Gesture number to store.
 * \return result of operation \ref nt_result.
 *
 * In case that gesture parameter is lower or equal to the defined value in the NT_PROXI_UNLOCK_GESTURES macro,
 * the function set the select_gesture variable (select_gesture) to positive value corresponding gesture number to store
 * after drawing and return NT_SUCCESS value. The function return the NT_FAILURE value if the input gesture number is
 * the gesture higher then the NT_PROXI_UNLOCK_GESTURES macro.
 *
 * Example:
 * \code
 * uint8_t gesture = 1;
 * nt_control_proxi_set_gesture(&my_proxi_control, gesture);
 * \endcode
 */
int32_t nt_control_proxi_set_gesture(const struct nt_control *control, uint8_t gesture);

/** \} end of proxi_api group */
/** \} end of proxi group */

#ifdef __cplusplus
}
#endif

#endif
