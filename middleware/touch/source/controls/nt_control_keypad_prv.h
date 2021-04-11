/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_KEYPAD_PRV_H_
#define _NT_KEYPAD_PRV_H_

/**
 * \defgroup keypad_private Keypad Control
 * \ingroup controls_private
 *
 * Keypad implements the keyboard-like functionality on top of an array of
 * electrodes; it is represented by the nt_control_keypad structure.
 *
 * An application may use the Electrode API to determine the touch or release states
 * of individual electrodes. The Keypad simplifies this task and it extends this
 * simple scenario by introducing a concept of a "key". The "key" is represented
 * by one or more physical electrodes, so the Keypad control enables one
 * electrode to be shared by several keys. Each key is defined by a set of electrodes that
 * all need to be touched in order to report the "key press" event.
 *
 * The Keypad Control provides Key status values and is able to generate the Key Touch,
 * Auto-repeat, and Release events.
 *
 * The images below show simple and grouped keypad electrode layouts.
 * \image html keypad.png "Keypad Electrodes"
 * \image latex keypad.png "Keypad Electrodes"
 *
 * \{
 */

#include "nt_controls.h"
#include "nt_types.h"
/* forward declaration */
struct nt_control_keypad;

/**
 * Keypad flags.
 */
enum nt_control_keypad_flags
{
    NT_KEYPAD_ONLY_ONE_KEY_FLAG = 1 << NT_FLAGS_SPECIFIC_SHIFT(0), /*!< Keypad only one key is valid flag. */
    NT_KEYPAD_MULTI_TOUCH_FLAG = 1 << NT_FLAGS_SPECIFIC_SHIFT(1), /*!< Multifunction keypad - multi touch recognition */
};

/**
 *  The Keypad RAM structure used to store volatile parameters of the control.
 *
 *  You must allocate this structure and put a pointer into the nt_control_keypad
 *  structure when it is being registered in the system.
 */
struct nt_control_keypad_data
{
    uint64_t last_state;                 /*!< Last state of keypad electrodes. */
    uint64_t last_key_state;             /*!< Last state of keypad keys. */
    uint32_t start_autorepeat_rate;      /*!< Start Autorepeat rate. */
    uint32_t repeat_time;                /*!< Time of next autorepeat event. */
    int32_t last_electrode;              /*!< Last touched electrode index. */
    uint16_t autorepeat_rate;            /*!< Autorepeat rate. */
    nt_control_keypad_callback callback; /*!< Keypad callback handler. */
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/** \} end of keypad_private group */

#endif /* _NT_KEYPAD_PRV_H_ */
