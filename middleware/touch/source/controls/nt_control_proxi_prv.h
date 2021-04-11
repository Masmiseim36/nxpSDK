/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_PROXI_PRV_H_
#define _NT_PROXI_PRV_H_

/**
 * \defgroup proxi_private Proxi Control
 * \ingroup controls_private
 *
 * The proxi control enables the detection of a finger or object presence in the
 * near field of the electrode, so that the approaching finger/hand can be detected
 * without a direct electrode touch. Moreover, the proximity position and
 * direction of the movement can be evaluated for each proximity electrode.
 * It is represented by the nt_control structure.
 *
 * The proxi control uses a single electrode or a set of discrete electrodes to enable the calculation of the finger
 * proximity position within a near field area. If more electrodes are enabled for the proximity control,
 * the proxi detection algorithm processes each electrode separately and localizes the electrode
 * with the highest delta signal to detect the maximum proximity electrode from the group
 * for a single signal requirement to process the whole group. The signals from each electrode
 * are then used for the evaluation of the proximity position, movement, and its direction.
 * The electrode index with the maximum proximity is stored to the buffer at each maximum proximity
 * electrode index change. The history of the maximum proximity changes consists of the same number of steps as
 * the number of the proximity electrodes. The history stored in the buffer can be used for a simple gesture
 * evaluation so that the buffered history is compared to the stored histories (stored gesture) and
 * in case the same gesture is recognized. Using this feature, not only the vertical movement, but
 * also the horizontal movement can be recognized. The gesture can be stored from the captured finger
 * movement by modifying the gesture number variable that stores the last buffer gesture to a gesture array.
 * Another way to store a gesture is to write the gestures directly to the proxi configuration as the array.
 *
 * The proxi control provides the position, direction, and displacement values of all proximity electrodes
 * and the buffered history of the maximum proximity changes in time. It can generate the event callbacks when
 * the finger movement, initial-touch, or release is detected for each electrode and if the gesture is recognized.
 * The image below shows the proxi electrode functionality.
 * \image html proxi.png "Proxi Electrode"
 * \image latex proxi.png "Proxi Electrode"
 *
 * \{
 */

#include "nt_controls.h"
#include "nt_types.h"
#include "nt_setup.h"

/* Make project build-able in case no define defined */
#ifndef NT_PROXI_ELECTRODES
#define NT_PROXI_ELECTRODES 1
#endif
#ifndef NT_PROXI_GESTURES
#define NT_PROXI_GESTURES 1
#endif
/* Make project build-able in case of define to zero */
#if NT_PROXI_ELECTRODES == 0
#undef NT_PROXI_ELECTRODES
#define NT_PROXI_ELECTRODES 1
#endif
#if NT_PROXI_GESTURES == 0
#undef NT_PROXI_GESTURES
#define NT_PROXI_GESTURES 1
#endif
/* Avoid incorrect configuration when gesture are defined without electrodes */
#if NT_PROXI_ELECTRODES < 2 && NT_PROXI_GESTURES > 1
#undef NT_PROXI_GESTURES
#define NT_PROXI_GESTURES 1
#endif

/* forward declaration */
struct nt_control_proxi;
static inline void _nt_control_proxi_set_flag(uint32_t *electrode, uint32_t flags);
static inline void _nt_control_proxi_clear_flag(uint32_t *electrode, uint32_t flags);
static inline uint32_t _nt_control_proxi_get_flag(uint32_t *electrode, uint32_t flags);

/**
 * Proxi proximity flags.
 */
enum nt_control_prox_flags
{
    NT_PROXI_DIRECTION_FLAG = 1 << NT_FLAGS_SPECIFIC_SHIFT(0), /*!< Proxi direction flag */
    NT_PROXI_MOVEMENT_FLAG  = 1 << NT_FLAGS_SPECIFIC_SHIFT(1), /*!< Proxi movement flag */
    NT_PROXI_TOUCH_FLAG     = 1 << NT_FLAGS_SPECIFIC_SHIFT(2), /*!< Proxi touch flag */
    NT_PROXI_RELEASE_FLAG   = 1 << NT_FLAGS_SPECIFIC_SHIFT(3), /*!< Proxi release flag */
};

/**
 *  The proxi RAM structure is used to store the volatile parameters of the control.
 *
 *  Allocate this structure and put a pointer into the nt_control_proxi
 *  structure when it is being registered in the system.
 */
struct nt_control_proxi_data
{
    nt_control_proxi_callback callback;          /*!< Proxi callback handler */
    uint32_t proxi_flags[NT_PROXI_ELECTRODES];   /*!< State flags for each electrode */
    uint16_t proxi_max[NT_PROXI_ELECTRODES];     /*!< Maximum proximity for each electrode */
    uint8_t proxi_curr[NT_PROXI_ELECTRODES];     /*!< Current scaled proximity value for each electrode */
    uint8_t proxi_movement[NT_PROXI_ELECTRODES]; /*!< Proximity movement level for each electrode */
    uint8_t index_k_1;                           /*!< Previous maximum proximity key index */
    uint8_t proximity;                           /*!< Maximum proximity value of the index electrode */
    uint8_t index;                               /*!< Proximity active key index with the maximum proximity */
#if NT_PROXI_GESTURES > 0
    uint32_t buffer; /*!< Buffer to store index key with maximum proximity each step to hex symbol from right to left */
    uint32_t buffer_mask;       /*!< Buffer mask to clean longer buffer history then electrode number */
    uint8_t select_gesture;     /*!< Selected gesture number for storing gesture */
    uint8_t recognized_gesture; /*!< Recognized gesture number to indicate*/
    uint8_t gesture_number;     /*!< Number of stored gesture */
#endif                          /* NT_PROXI_GESTURES > 0 */
};

/**
 * \internal
 * \brief Set the assigned flag for the specific electrodes.
 * \param electrode The electrode number to specify the flags array index.
 * \param flag The flag to be set.
 * \return none
 *
 * This function sets the specified flag in the specific electrode.
 */
static inline void _nt_control_proxi_set_flag(uint32_t *electrode, uint32_t flags)
{
    *electrode |= flags;
}

/**
 * \internal
 * \brief Clears the assigned flag for specific electrodes.
 * \param electrode The electrode number to specify the flags array index.
 * \param flag The flag to be cleared.
 * \return none
 *
 * This function clears the specified flag in the specific electrode.
 */
static inline void _nt_control_proxi_clear_flag(uint32_t *electrode, uint32_t flags)
{
    *electrode &= ~flags;
}

/**
 * \internal
 * \brief Get the assigned flag from the specific electrodes.
 * \param electrode The electrode number to specify the flags array index.
 * \param flag The flag to be set.
 * \return (bool)flag value
 *
 * This function reads the specified flag in the specific electrode.
 */
static inline uint32_t _nt_control_proxi_get_flag(uint32_t *electrode, uint32_t flags)
{
    return (*electrode & flags);
}

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/** \} end of proxi_private group */

#endif /* _NT_PROXI_PRV_H_ */
