/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_CONTROLS_H_
#define _NT_CONTROLS_H_

/**
 * \defgroup controls Controls
 * \ingroup ntapi   
 *
 * Controls represent the highest level of abstraction in the finger touch evaluation;
 *
 * Based on the signal and status information coming from the Electrode layer, the controls
 * calculate finger actions like movement, keyboard touch, hold, and so on.
 * \{
 */

#include "nt_types.h"
#include "nt_electrodes.h"

/* forward declarations */
struct nt_electrode;
struct nt_control;
struct nt_control_interface;

/**
 * \defgroup gcontrols General API
 * \ingroup controls
 * General Function definition of controls.
 *
 * \{
 */

/** Container, which covers all possible variants of the control parameters. When 
  * defining the control setup structure, initialize only one member of this union. 
  * Use the member that corresponds to the control type.
 */
union nt_control_params
{
  const struct nt_control_arotary     * arotary;        /*!< Pointer to the Analog Rotary control parameters structure */
  const struct nt_control_aslider     * aslider;        /*!< Pointer to the Analog Slider control parameters structure */
  const struct nt_control_keypad      * keypad;         /*!< Pointer to the Keypad control parameters structure*/
  const struct nt_control_proxi       * proxi;          /*!< Pointer to the Proxi control parameters structure */
  const struct nt_control_matrix      * matrix;         /*!< Pointer to the Matrix control parameters structure */
  const void                          * general;        /*!< Pointer for general use */
};

/**
 *  The main structure representing the control instance; this structure is used for all
 *  control implementations. The type of the control is specified by the "interface" member, 
 *  which defines the control behaviour. 
 *  Note that the "control_params" must correspond to the control type.
 *
 *  This structure can be allocated in ROM.
 */
struct nt_control  {
    const struct nt_control_interface   *interface;             /*!< An instance of the control interface. Cannot be NULL.*/
    const struct nt_electrode           * const * electrodes;   /*!< List of electrodes. Cannot be NULL. */
    const union nt_control_params       control_params;         /*!< An instance of the control params. Cannot be NULL.*/
};

/**
 * \defgroup gcontrols_api API Functions
 * \ingroup gcontrols
 * General API functions of the controls.
 *
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif
/**
 * \brief Enable control
 * \param control Pointer to the control instance.
 * \return none
 *
 * Enables the control operation by setting the NT_CONTROL_ENABLE_FLAG.
 * This is an example of enabling the control in the NT library:
 * \code
 *  // The NT control my_nt_control_keypad is enabled
 *  nt_control_enable(&my_nt_control_keypad);
 * \endcode 
 */
void nt_control_enable(const struct nt_control *control);

/**
 * \brief Disable control
 * \param control Pointer to the control instance.
 * \return none
 *
 * Disables the control operation by clearing the NT_CONTROL_ENABLE_FLAG.
 * This is an example of disabling the control in the NT library:
 * \code
 *  // The NT control my_nt_control_keypad is disabled
 *  nt_control_disable(&my_nt_control_keypad);
 * \endcode
 */
void nt_control_disable(const struct nt_control *control);

/**
 * \brief Get touched electrode.
 * \param control Pointer to the control.
 * \param index   Index of the first electrode to be probed. Use 0 during the first
 *                call. Use the last-returned index+1 to get the next touched electrode.
 * \return Index of the touched electrode, or NT_FAILURE when no electrode is touched.
 *
 * Use this function to determine, which control electrodes are currently touched.
 * This is an example of getting the touched electrodes of control in the NT library:
 * \code
 *  int32_t last_touched_electrode = 0;
 *  uint32_t electrode_count = nt_control_count_electrodes(&my_control);
 *  last_touched_electrode = nt_control_get_touch_button(&my_control, last_touched_electrode);
 *  while(last_touched_electrode != NT_FAILURE)
 *  {
 *    printf("The electrode %d in my control is touched", last_touched_electrode);
 *    last_touched_electrode = nt_control_get_touch_button(&my_control, last_touched_electrode);
 *  }
 * \endcode
 */
int32_t nt_control_get_touch_button(const struct nt_control *control,
                                    uint32_t index);

/**
 * \brief Get the state of all control electrodes.
 * \param control Pointer to the control data.
 * \return This function returns a bit-mask value, where each bit represents one
 *         control electrode. Logic 1 in the returned value represents a touched
 *         electrode.
 * \code
 *  uint64_t touched_electrode = 0;
 *  touched_electrode = nt_control_get_electrodes_state(&my_control);
*  printf("The electrode state is following: 0x%X in my control.", touched_electrode);
 * \endcode
 */
uint64_t nt_control_get_electrodes_state(struct nt_control *control);
      
/**
 * \internal
 * \brief Calculate the number of electrodes.
 * \param control Pointer to the control.
 * \return The number of electrodes in the control.
 * This is an example of getting the electrodes count of control in the NT library:
 * \code
 *  int32_t last_touched_electrode = 0;
 *  uint32_t electrode_count = nt_control_count_electrodes(&my_control);
 *  last_touched_electrode = nt_control_get_touch_button(&my_control, last_touched_electrode);
 *  while(last_touched_electrode != NT_FAILURE)
 *  {
 *    printf("The electrode %d in my control is touched", last_touched_electrode);
 *    last_touched_electrode = nt_control_get_touch_button(&my_control, last_touched_electrode);
 *  }
 * \endcode
 */
uint32_t nt_control_count_electrodes(const struct nt_control *control);

/**
 * \brief Return the electrode by index.
 * \param control Pointer to the control.
 * \param index
 * \return Pointer to the electrode instance retrieved from control's electrode list.
 * This is an example of getting the electrode pointer of control by index in the NT library:
 * \code
 *  // Get the pointer of electrode on index 2 for my_control
 *  nt_electrode *my_electrode = nt_control_get_electrode(&my_control, 2);
 * \endcode*/
const struct nt_electrode *nt_control_get_electrode(const struct nt_control *control,
                                                                  uint32_t index);

#ifdef __cplusplus
}
#endif

/** \} end of gcontrols_api group */
/** \} end of gcontrols group */
/** \} end of controls group */

#endif
