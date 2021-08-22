/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_MATRIX_H_
#define _NT_MATRIX_H_

/**
 * \defgroup matrix Matrix Control
 * \ingroup controls
 *
 * Matrix enables the one or two finger position and gestures detection on the touchpad surface.
 *
 *
 * \{
 */

#include "nt_controls.h"
#include "nt_types.h"

#define NT_CONTROL_MATRIX_NAME "nt_control_matrix_interface"

/** Matrix event types. */
enum nt_control_matrix_event
{
    NT_MATRIX_RELEASE             = 1, /*!< Release event */
    NT_MATRIX_TOUCH               = 2, /*!< Touch event */
    NT_MATRIX_MOVEMENT            = 3, /*!< Movement event */
    NT_MATRIX_RELEASE_SET_GESTURE = 4, /*!< New unlock gesture is stored */
    NT_MATRIX_RELEASE_GET_GESTURE = 5, /*!< Stored unlock gesture is recognized */
    NT_MATRIX_GESTURE_TOO_LONG    = 6, /*!< Unlock gesture is too long */
    NT_MATRIX_GET_CONTROL_GESTURE = 7, /*!< Recognized control gesture */
};

/** Matrix control gesture event types. */
enum nt_control_matrix_control_gestures
{
    NT_MATRIX_NO_GESTURE  = 0, /*!< No control gesture recognized */
    NT_MATRIX_DOUBLE_TAP  = 1, /*!< Double tap control gesture */
    NT_MATRIX_COVER       = 2, /*!< Covered control gesture */
    NT_MATRIX_SWIPE_UP    = 3, /*!< Swipe up control gesture */
    NT_MATRIX_SWIPE_DOWN  = 4, /*!< Swipe down control gesture */
    NT_MATRIX_SWIPE_LEFT  = 5, /*!< Swipe left control gesture */
    NT_MATRIX_SWIPE_RIGHT = 6, /*!< Swipe right control gesture */
    NT_MATRIX_ZOOM_IN     = 7, /*!< Zoom in control gesture */
    NT_MATRIX_ZOOM_OUT    = 8, /*!< Zoom out control gesture */
};

/**
 *  The Matrix RAM structure used to store x and y axis data as group.
 */
typedef struct
{
    uint8_t x; /*!< x axis */
    uint8_t y; /*!< y axis */
} nt_coordinates8_xy;

/**
 *  The Matrix RAM structure used to store x and y axis data of few points.
 */
typedef nt_coordinates8_xy nt_points8[2];

/**
 * Matrix event callback function pointer type.
 */
typedef void (*nt_control_matrix_callback)(const struct nt_control *control,
                                           enum nt_control_matrix_event event,
                                           nt_coordinates8_xy *position,
                                           uint32_t index);

/**
 *  The Matrix RAM structure used to store x and y axis data as group.
 */
typedef struct
{
    uint64_t x; /*!< x axis */
    uint64_t y; /*!< y axis */
} nt_coordinates64_xy;

/**
 *  The Matrix RAM structure used to store x and y axis data of few points.
 */
typedef nt_coordinates64_xy nt_points64[2]; /* Maximum two fingers */

/**
 *  The main structure representing the Matrix Control.
 *
 *  An instance of this data type represents the Matrix Control. You must initialize all the members before
 *  registering the control in the system. This structure can be allocated in ROM.
 *
 */
struct nt_control_matrix
{
    uint32_t double_tap_interval;     /*!< Max. time interval between taps for double tap. */
    uint16_t dissimilarity_max;       /*!< Maximum difference for unlock gesture recognition */
    uint8_t touchpad_size_horizontal; /*!< Number of touchpad segments on x axis (horizontal segments). First
                                           touchpad_size_horizontal number of electrodes assigned to the matrix control
                                           are horizontal electrodes. */
    uint8_t touchpad_size_vertical;   /*!< Number of touchpad segments on y axis (vertical segments). */
    uint8_t sensing_mode;             /*!< False value means touchpad is based on self-capacitance sensing mode,
                                           true value means touchpad is based on mutual-capacitance sensing mode. */
    uint8_t increase_resolution;      /*!< False value means default resolution, true value means that two the nearest
                                           touched electrodes create addition position value, it extends resolution
                                           according: new_resolution = 2 * default_resolution - 1. */
    nt_points64 *gesture;             /*!< Stored matrix gesture array. The gesture means the time change of fingers
                                           x and y coordinates written in hexadecimal format from right to left.
                                           For example, x coordinate gesture 0x4321 means that firstly x position 1
                                           was touched, then x position 2, 3 and 4. The gesture array must be finished
                                           by last zero member {{0,0},{0,0}}},  */
};

/** An interface structure, which contains pointers to the entry points of the Matrix
 *  algorithms. A pointer to this structure must be assigned to any
 *  instance of the nt_control_matrix, to define the control behaviour. */
extern const struct nt_control_interface nt_control_matrix_interface;

/**
 * \defgroup matrix_api Matrix Control API
 * \ingroup matrix
 *
 * These functions can be used to set or get the Matrix control properties.
 *
 * A common example definition of the Matrix control for all source code examples is as follows:
 * \code
 * #define NT_MATRIX_UNLOCK_GESTURES  4
 *
 *  const struct nt_electrode  * const control_0_electrodes[] = {&electrode_0, &electrode_1,&electrode_2, &electrode_3,
 *    &electrode_4,&electrode_5,&electrode_6,&electrode_7,&electrode_8,&electrode_9,&electrode_10,&electrode_11, NULL};
 *
 *  const struct nt_control_matrix matrix_params =
 *  {
 *     .double_tap_interval     = 500,
 *     .dissimilarity_max       = 5,
 *     .touchpad_size_horizontal= 6,
 *     .touchpad_size_vertical  = 6,
 *     .sensing_mode            = false,
 *     .increase_resolution     = true,
 *     .gesture = (nt_points64 []){{{0x0BA9977555533211,0x0BBA987765443221},{0x0000000000000000,0x0000000000000000}},
 *                                {{0x0000001111223578,0x000000567889ABAA},{0x0000000000000000,0x0000000000000000}},
 *                                {{0x00000BAAABBBBAA9,0x00000BBA98765422},{0x0000000000000000,0x0000000000000000}},
 *                                {{0,0},{0,0}}},
 *  };
 *
 *  const struct nt_control my_matrix_control =
 *  {
 *    .interface = &nt_control_matrix_control_interface,
 *    .electrodes = control_0_electrodes,
 *    .control_params.matrix = matrix_params,
 *  };
 *
 * \endcode
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Registers the Matrix event handler function.
 * \param control  Pointer to the control.
 * \param callback Address of function to be invoked.
 * \return none
 *
 * Register the specified callback function as the KeyPad event handler.
 * If the callback parameter is NULL, the callback is disabled.
 * Example:
 * \code
 *
 *  static void my_matrix_cb(const struct nt_control *control,
 *                           enum nt_control_matrix_event event,
 *                           nt_points8 *position,
 *                           uint32_t index)
 *  {
 *    (void)control;
 *     char* event_names[] =
 *     {
 *      "NT_MATRIX_RELEASE            ",
 *      "NT_MATRIX_TOUCH              ",
 *      "NT_MATRIX_MOVEMENT           ",
 *      "NT_MATRIX_RELEASE_SET_GESTURE",
 *      "NT_MATRIX_RELEASE_GET_GESTURE",
 *      "NT_MATRIX_GESTURE_TOO_LONG   ",
 *      "NT_MATRIX_GET_CONTROL_GESTURE",
 *      };
 *
 *    printf("New matrix control event %s on key: %d.", event_names[event], index);
 *  }
 *
 *  nt_control_matrix_register_touch_callback(&my_matrix_control, my_matrix_touch_cb);
 * \endcode
 */
void nt_control_matrix_register_callback(const struct nt_control *control, nt_control_matrix_callback callback);

/**
 * \brief Enable matrix control to store unlock gesture.
 * \param control Pointer to the Matrix control.
 * \param gesture Gesture number to store.
 * \return result of operation \ref nt_result.
 *
 * In case that gesture parameter is lower or equal to the defined value in the NT_MATRIX_UNLOCK_GESTURES macro,
 * the function set the select_gesture variable (select_gesture) to positive value corresponding gesture number to store
 * after drawing and return NT_SUCCESS value. The function return the NT_FAILURE value if the input gesture number is
 * the gesture higher then the NT_MATRIX_UNLOCK_GESTURES macro.
 *
 * Example:
 * \code
 * uint8_t gesture = 1;
 * nt_control_matrix_set_gesture(&my_matrix_control, gesture);
 * \endcode
 */
int32_t nt_control_matrix_set_gesture(const struct nt_control *control, uint8_t gesture);

/** \} */ /* end of matrix_api group */
/** \} */ /* end of matrix group*/
#endif
