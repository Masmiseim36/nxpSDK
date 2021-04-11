/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_MATRIX_PRV_H_
#define _NT_MATRIX_PRV_H_

/**
 * \defgroup matrix_private Matrix Control
 * \ingroup controls_private
 *
 * The matrix implements a similar functionality to the keypad but the matrix has the electrode surfaces placed
 * closer to each other. Moreover, the matrix control allows processing the position tracking and gesture recognition.
 * Usually, the matrix control is used for data processing from the touchpad hardware.
 *
 * Applications may use the electrode API to determine the touch or release states of individual
 * electrodes (digital approach) but the matrix control processes the electrodes as a two-dimensional array.
 * The images below show a simple electrode array where the self-cap sensing is used.
 * \image html matrix.png "Matrix Self-cap Sensing Electrodes"
 * \image latex matrix.png "Matrix Self-cap Sensing Electrodes "
 * To recognize the position on the shown layout means to activate two electrodes (horizontal and
 * vertical). When this happens, the x and y coordinates of the touch position are known and can be tracked.
 * When the finger is moving on the surface, the actual x and y coordinates are changing
 * in time and the position changes are stored into the buffer. At the end of the movement, the release
 * position is stored as well and the gesture recognition algorithm can be executed.
 *
 * The same processing is used for the mutual-cap-sensing-based matrix where the position is known
 * after touching one electrode only because the mutual-cap-sensing electrode consists of the Tx and Rx
 * electrodes which determine the position on the surface by a mutual Tx and Rx electrode hardware interconnection.
 * The position detection is only one difference between the self-cap and mutual-cap matrices. Other matrix control
 * processing algorithms remain the same. The mutual-cap-sensing-based matrix is enabled by defining the value of "true"
 * to the sensing_mode matrix constant, similarly like the matrix horizontal and vertical sizes.
 * \image html matrix_mutual.png "Matrix Mutual-cap Sensing Electrodes"
 * \image latex matrix_mutual.png "Matrix Mutual-cap Sensing Electrodes"
 *
 * The position recognition can be extended by a simple trick for both matrix types. The touch on the two nearest
 * electrodes can be considered as virtual electrodes between the touched electrodes. In this way, the horizontal and
 * vertical resolution can be extended by adding gaps between the electrodes. The new resolution is twice the original
 * resolution minus one (extended_resolution = origin_resolution + electrode_gaps = 2 * origin_resolution - 1).
 * If two nearest electrodes are activated at the same time, the algorithm determines that a virtual electrode
 * is activated between the real electrodes.
 *
 * The matrix control processes the position regardless of the finger size. A large finger activates
 * more than one horizontal electrode and one vertical electrode for the self-cap mode. The algorithm ignores the other
 * nearest touched electrodes to centralize the recognized position. The feature ensures a most precise
 * position on the surface for next processing. The position can be determined from one, two, or three nearest
 * electrodes. If more nearest electrodes are enabled, the position is recognized as two active figures. This rule is
 * valid for horizontal and vertical electrodes.
 *
 * The matrix control algorithm recognizes the position of not only one finger but two fingers as well.
 * The finger position or two finger positions are stored for the touch, release, and movement (actual position) events.
 * Also, the movement tracking and gestures can process two fingers.
 *
 * The gesture feature is the most important part of the matrix control. The gestures can be divides to two groups:
 * control gestures and unlock gestures. The control gestures are usually used to control the device, change the mode,
 * configure a feature, or make some user action. The unlock gestures stored by the user are typically used to unlock
 * the screen, wake up the device, or recognize the movement more precisely. The matrix control enables to recognize
 * these control gestures:
 *  - swipe up
 *  - swipe down
 *  - swipe left
 *  - swipe right
 *  - cover
 *  - double tap
 *  - zoom in
 *  - zoom out
 *
 * The conditions to recognize the swipe gestures:
 *  - buffered gesture must be longer than three recognized positions horizontally or vertically (for one finger)
 *  - recognized positions horizontally or vertically and the movement in dominant direction must cover twice the amount
 *    recognized positions than in insignificant direction
 *
 * The double tap gesture enables to define the maximum time interval between taps. To recognize the cover gesture, each
 * touchpad electrode must be touched. The conditions to recognize the zoom in/out gestures:
 *  - buffered gesture must be longer than three recognized positions horizontally or vertically (for both fingers)
 *  - square of the position distance change must be higher(lower) then 15(-15) to increment zoom in(out) counter
 *  - the number of zoom in(out) counters must be higher then 1 and the ration between zoom in and zoom out counters
 *    must be double at least
 *
 * You can store an arbitrary number of unlock gestures, limited by the free memory. The unlock gestures have higher
 * priority than the control gestures. If both the unlock and control gestures are recognized, the control gesture is
 * ignored. The unlock gesture means a continuous curve drawn by one finger or two continuous curves drawn by two
 * fingers. The gesture can be stored from the drawn pattern into the RAM memory by changing the selected gesture
 * variable (0 means no gesture to store; the number specifies which gesture number is stored). For permanent usage of
 * the drawn gestures, the stored gestures must be copied or rewritten as matrix gesture parameters to be stored into
 * the flash memory.
 *
 * After the release event, the actual drawn gesture is buffered and compared to the stored unlock gestures (if any).
 * The dissimilarity is a value that provides information about how much the actual gesture is similar to the stored
 * ones. In case of a calculated dissimilarity is lower the dissimilarity_max parameter defined in matrix constants, a
 * specified gesture is recognized. A dissimilarity value of 0 means that the gestures are the same.
 *
 * When drawing the gesture, the finger (or fingers) position changes are stored to a buffer. The maximum position
 * resolution is 16 values (hexadecimal value) for each dimension. If the matrix is too small, the resolution can be
 * extended by creating virtual electrodes between the real ones. This feature is enabled by defining the value of
 * "true" to the increase_resolution matrix constant. The conditions to recognize or store the unlock gestures:
 *  - buffered gesture must be longer than four recognized positions horizontally or vertically (for one or two fingers)
 *  - buffered gesture must be longer than four position changes to have enough recorded points for
 * cooperation,
 *  - buffered gesture must be shorter than sixteen position changes to not exceed the buffer size
 *
 * The movement on the matrix surface is buffered in such way that each position change is stored into 64-bit x and y
 * buffers in the right to left order as a hexadecimal value. Therefore, the same gestures drawn by a different speed
 * are not the same because the buffer size is different. Considered this when a gesture is not recognized. The
 * gesture-comparison algorithm is based on the summation of the square point to point distances.
 *
 * The matrix control is represented by the nt_control_matrix structure.
 *
 * \{
 */

#include "nt_controls.h"
#include "nt_types.h"
#include "nt_control_matrix.h"
#include "nt_setup.h"

/* configure maximum unlock gestures number to 1 in case that NT_MATRIX_UNLOCK_GESTURES is not defined */
#ifndef NT_MATRIX_UNLOCK_GESTURES
#define NT_MATRIX_UNLOCK_GESTURES 1
#endif

/* forward declaration */
struct nt_control_matrix;
struct nt_coordinate_xy;

/**
 * Matrix Matrix control flags.
 */
enum nt_control_matrix_flags
{
    NT_MATRIX_RELEASE_FLAG             = 1 << NT_FLAGS_SPECIFIC_SHIFT(0), /*!< Release event */
    NT_MATRIX_TOUCH_FLAG               = 1 << NT_FLAGS_SPECIFIC_SHIFT(1), /*!< Key-touch event */
    NT_MATRIX_MOVEMENT_FLAG            = 1 << NT_FLAGS_SPECIFIC_SHIFT(2), /*!< Movement event */
    NT_MATRIX_SET_UNLOCK_GESTURE_FLAG  = 1 << NT_FLAGS_SPECIFIC_SHIFT(3), /*!< New gesture is stored */
    NT_MATRIX_GET_UNLOCK_GESTURE_FLAG  = 1 << NT_FLAGS_SPECIFIC_SHIFT(4), /*!< Stored gesture is recognized */
    NT_MATRIX_GET_CONTROL_GESTURE_FLAG = 1 << NT_FLAGS_SPECIFIC_SHIFT(5), /*!< Control gesture is recognized */
};

/**
 *  The matrix RAM structure is used to store volatile parameters of the control.
 *
 *  Allocate this structure and put a pointer into the nt_control_matrix
 *  structure when it is being registered in the system.
 */
struct nt_control_matrix_data
{
    nt_points64 buffer;            /*!< Buffer to store movement on x and y axis. */
    uint64_t last_electode_states; /*!< Last electrodes states */
    uint64_t touchpad_mask;        /*!< Internal mask defining the touch-pad electrodes used (horizontal and vertical
                                      electrodes must be next to each other) */
    uint32_t double_tap_next_time; /*!< Time instant for the second tap to make a valid double tap */
    uint16_t touchpad_mask_horizontal; /*!< Touch-pad mask for the horizontal coordinate */
    uint16_t
        gesture_diff[NT_MATRIX_UNLOCK_GESTURES]; /*!< Calculated differences between the stored and buffered gestures */
    uint16_t two_points_gesture_flag;            /*!< Indicates two-point gesture */
    nt_points8 touch_position;                   /*!< Touch-pad touch position on the x and y axes */
    nt_points8 actual_position;                  /*!< Touch-pad touch position on the x and y axes */
    nt_points8 release_position;                 /*!< Touch-pad release position on the x and y axes */
    uint8_t buffer_position;                     /*!< Store the position into the buffer */
    uint8_t select_gesture;                      /*!< Number code of the gesture to be stored */
    uint8_t recognized_gesture;                  /*!< Number code of the recognized gesture */
    uint8_t get_position_add_val[6]; /*!< Constants for the get_position function defined due to the increase_resolution
                                        constant */
    uint8_t gesture_number;          /*!< Number of stored gesture */
    nt_control_matrix_callback callback; /*!< Matrix callback handler. */
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/** \} end of matrix_private group */

#endif /* _NT_MATRIX_PRV_H_ */
