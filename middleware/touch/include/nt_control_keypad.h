/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_KEYPAD_H_
#define _NT_KEYPAD_H_

/**
 * \defgroup keypad Keypad Control
 * \ingroup controls
 *
 * Keypad implements the keyboard-like functionality on top of an array of
 * electrodes; it is represented by the nt_control_keypad structure.
 *
 * The application may use the Electrode API to determine the touch or release states
 * of individual electrodes. The Keypad simplifies this task, and extends this
 * simple scenario by introducing a concept of a "key". The "key" is represented
 * by one or more physical electrodes, therefore the Keypad control enables sharing of one
 * electrode by several keys. Each key is defined by a set of electrodes that
 * all must be touched, in order to report the "key press" event.
 * The keypad enables to detect so called multi-touch event when more keys were touched
 * simultaneously. The key combinations consist of two or more "key press" events
 * together can be defined by user for the group or single electrode based keypad.
 *
 * The Keypad Control provides the Key status values and is able to generate Key Touch,
 * Auto-repeat,Multi-touch and Release events.
 *
 * The figures below show simple and grouped Keypad electrode layouts.
 * \image html keypad.png "Keypad Electrodes"
 * \image latex keypad.png "Keypad Electrodes"
 *
 * \{
 */

#include "nt_controls.h"
#include "nt_types.h"

#define NT_CONTROL_KEYPAD_NAME "nt_control_keypad_interface"

/** Keypad event types. */
enum nt_control_keypad_event
{
    NT_KEYPAD_RELEASE     = 0, /*!< Release event */
    NT_KEYPAD_TOUCH       = 1, /*!< Key-touch event */
    NT_KEYPAD_AUTOREPEAT  = 2, /*!< Auto-repeat event */
    NT_KEYPAD_MULTI_TOUCH = 3  /*!< Key-multi touch event */
};

/**
 * Keypad event callback function pointer type.
 */
typedef void (*nt_control_keypad_callback)(const struct nt_control *control,
                                           enum nt_control_keypad_event event,
                                           uint32_t index);

/**
 *  The main structure representing the Keypad Control.
 *
 *  An instance of this data type represents the Keypad Control. You must initialize all the members before registering
 * the control in the system. This structure can be allocated in ROM.
 *
 */
struct nt_control_keypad
{
    uint32_t const *groups;      /*!< Pointer to the group definitions. An array of integers, where bits in
                                      the integer represents electrodes in a group. Enable key groups feature. */
    uint32_t const *multi_touch; /*!< Key combination for multitouch, depending on the key representation by one
                                      or more electrodes. Enable muti-touch feature. */
    uint8_t groups_size;         /*!< Number of groups. */
    uint8_t multi_touch_size;    /*!< Number of multi-touch key combination. */
};

/** An interface structure, which contains pointers to the entry points of the Keypad
 *  algorithms. A pointer to this structure must be assigned to any
 *  instance of the nt_control_keypad, to define the control behavior. */
extern const struct nt_control_interface nt_control_keypad_interface;

/**
 * \defgroup keypad_api Keypad Control API
 * \ingroup keypad
 *
 * These functions can be used to set or get the Keypad control properties.
 *
 * A common example definition of the Keypad control for all source code examples is as follows:
 * \code
 *  const struct nt_electrode  * const control_0_electrodes[] = {&electrode_0, &electrode_1,
 *    &electrode_2, &electrode_3, NULL};
 *
 *  const struct nt_control_keypad keypad_params =
 *  {
 *     .groups = NULL,
 *     .groups_size = 0,
 *  };
 *
 *  const struct nt_control my_keypad_control =
 *  {
 *    .interface = &nt_control_keypad_control_interface,
 *    .electrodes = control_0_electrodes,
 *    .control_params.keypad = keypad_params,
 *  };
 *
 * \endcode
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Enable or disable the functionality that only one key press is valid.
 * \param control   Pointer to the control.
 * \param enable    enable the only one key pressed is valid.
 * \return none
 *
 * Enable or Disable the only one key press is valid at once. The behavior is following:
 * Once the feature is enabled the first touched key is valid and all other are ignored
 * since the active electrode is pressed.
 * Example:
 * \code
 *
 *  nt_control_keypad_only_one_key_valid(&my_keypad_control, 0);
 * \endcode
 */
void nt_control_keypad_only_one_key_valid(const struct nt_control *control, uint32_t enable);

/**
 * \brief Registers the Keypad event handler function.
 * \param control  Pointer to the control.
 * \param callback Address of function to be invoked.
 * \return none
 *
 * Register the specified callback function as the KeyPad event handler.
 * If the callback parameter is NULL, the callback is disabled.
 * Example:
 * \code
 *
 *  static void my_keypad_cb(const struct nt_control *control,
 *                           enum nt_control_keypad_event event,
 *                           uint32_t index)
 *  {
 *    (void)control;
 *     char* event_names[] =
 *     {
 *      "NT_KEYPAD_RELEASE",
 *      "NT_KEYPAD_TOUCH",
 *      "NT_KEYPAD_AUTOREPEAT",
 *      };
 *
 *    printf("New keypad control event %s on key: %d.", event_names[event], index);
 *  }
 *
 *  nt_control_keypad_register_touch_callback(&my_keypad_control, my_keypad_touch_cb);
 * \endcode
 */
void nt_control_keypad_register_callback(const struct nt_control *control, nt_control_keypad_callback callback);

/**
 * \brief Set the auto-repeat rate.
 * \param control Pointer to the Keypad control.
 * \param value Auto-repeat value. Value 0 disables the auto-repeat feature.
 * \param start_value Auto-repeat start value. Value 0 disables the auto-repeat start feature.
 * \return none
 * Example:
 * \code
 *
 *  nt_control_keypad_set_autorepeat_rate(&my_keypad_control, 100, 1000);
 * \endcode
 */
void nt_control_keypad_set_autorepeat_rate(const struct nt_control *control, uint32_t value, uint32_t start_value);

/**
 * \brief Get the auto-repeat rate.
 * \param control Pointer to the Keypad control.
 * \return The auto-repeat value or 0 when this feature is disabled.
 * Example:
 * \code
 *  uint32_t autorepeat_rate;
 *  autorepeat_rate = nt_control_keypad_get_autorepeat_rate(&my_keypad_control);
 *  printf("Auto-repeat rate of my keypad control is set to : %d.", autorepeat_rate);
 * \endcode
 */
uint32_t nt_control_keypad_get_autorepeat_rate(const struct nt_control *control);

/**
 * \brief Get the button touch status.
 * \param control Pointer to the Keypad control.
 * \param index   The button's number (index) in the control.
 * \return 1 if the button is touched,
 *         0 otherwise.
 *
 * Returns the state of the keypad button. In case there are groups defined, the touch
 * state reflects that all electrodes forming one button are touched. Otherwise, a button is
 * in the release state.
 * Example:
 * \code
 * uint32_t touched;
 * touched = nt_control_keypad_is_button_touched(&my_keypad_control, 0);
 * if(touched)
 *      printf("The first key of the Keypad control is currently touched.");
 * else
 *      printf("The first key of the Keypad control is currently not touched.);
 * \endcode
 */
uint32_t nt_control_keypad_is_button_touched(const struct nt_control *control, uint32_t index);

/** \} end of keypad_api group */
/** \} end of keypad group */

#ifdef __cplusplus
}
#endif

#endif
