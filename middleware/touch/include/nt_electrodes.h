/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_ELECTRODES_H_
#define _NT_ELECTRODES_H_


/**
 * \defgroup electrodes Electrodes
 * \ingroup ntapi
 *  
 * Electrodes are data objects that are used by data-acquisition algorithms to store
 * the per-electrode data, as well as the resulting signal and touch / timestamp information.
 *
 * Each Electrode provides at minimum
 * the processed and normalized signal value, the baseline value, and touch / timestamp
 * buffer containing the time of last few touch and release events. All such common
 * information are contained in the nt_electrode structure type. Also, the electrode 
 * contains information about the key detector used to detect touches for this physical 
 * electrode (this is a mandatory field in the electrode definition). This has the advantage 
 * that each electrode has its own setting of key detector, independent on the module used. 
 * It contains information about hardware pin, immediate touch status, and time 
 * stamps of the last few touch or release events.
 *
 * \{
 */
#include "nt_types.h"
#include "nt_filters.h"
#include "nt_keydetectors.h"

   
#ifndef NT_ELECTRODE_STATUS_HISTORY_COUNT
  /** Definition of the electrode history status depth. Default setting is 4.
   */
  #define NT_ELECTRODE_STATUS_HISTORY_COUNT  4
#else
  #if NT_ELECTRODE_STATUS_HISTORY_COUNT < 4
    #error The depth of electrode history buffer (NT_ELECTRODE_STATUS_HISTORY_COUNT) can be set less than 4.
  #endif
#endif

#ifndef NT_ELECTRODE_SHIELD_THRESHOLD
  /** Definition of the lowest signal value on the shield electrode used for offset compensation */
  #define NT_ELECTRODE_SHIELD_THRESHOLD  10
#endif

#ifndef NT_ELECTRODE_SHIELD_GAIN
  /** Definition of the lowest signal value on the shield electrode used for offset compensation */
  #define NT_ELECTRODE_SHIELD_GAIN  1
#endif

#ifndef NT_ELECTRODE_SHIELD_SENS
  /** Definition of the max signal value on the shield electrode used for offset compensation */
  #define NT_ELECTRODE_SHIELD_SENS  1000
#endif

#ifndef NT_ELECTRODE_SHIELD_ATTN
  /** Definition of the max signal value on the shield electrode used for offset compensation */
  #define NT_ELECTRODE_SHIELD_ATTN  1
#endif
/* forward declarations */
struct nt_filter;
union nt_keydetector_params;

/** Electrode states. */
enum nt_electrode_state {
    NT_ELECTRODE_STATE_INIT    =  0, /*!< Initial state; Not enough data for the touch-detection algorithm yet. */
    NT_ELECTRODE_STATE_RELEASE =  1, /*!< Release state; A signal is near to the baseline. */
    NT_ELECTRODE_STATE_TOUCH   =  2, /*!< Touch state; the selected algorithm has decided that a finger is present. */
};

/**
 *  Electrode status structure holding one entry in the touch-timestamp buffer.
 *  An array of this structure type is a part of each Electrode, and contains last few
 *  touch or release events detected on the electrode.
 */
struct nt_electrode_status {
    uint32_t time_stamp; /*!< Time stamp. */
    uint8_t  state;      /*!< Electrode's state. */
};

struct gpio_inputs
{
  uint32_t gpio1;
  uint32_t gpio2;
};

/**
 *  The main structure representing the Electrode instance.
 *  There are all the parameters needed to define the behaviour of the NXP Touch
 *  electrode, including its key detector, hardware pins, multiplier / divider to normalize
 *  the signal, and the optional shielding electrode.
 *
 *  This structure can be allocated in ROM.
 */
struct nt_electrode {
    const struct nt_electrode                   *shielding_electrode;   /*!< Shielding electrode. */
    uint8_t                                     multiplier;             /*!< Multiplier. */
    uint8_t                                     divider;                /*!< Divider. */
    uint8_t                                     shield_threshold;       /*!< SH Threshold for shield activating */
    uint8_t                                     shield_gain;            /*!< SH Gain for shielding el. signal */
    uint8_t                                     shield_attn;            /*!< SH Attenuate for shielding el. signal */
    uint16_t                                    shield_sens;            /*!< SH Sensitivity for shielding limitation */
    uint16_t                                    baseline_level;         /*!< typical Baseline level value used for safety failure detection */
    uint32_t                                    pin_input;              /*!< Input pin. */
    struct gpio_inputs                          gpio_input;             /*!< corresponding GPIO input pins */
    const struct nt_keydetector_interface       *keydetector_interface; /*!< Pointer to Key Detector interface. */
    const union nt_keydetector_params           keydetector_params;     /*!< Pointer to Key Detector params. */
};

/**
 * \defgroup electrodes_api API Functions
 * \ingroup electrodes
 * General Function definition of the electrodes.
 *
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Enable the electrode. The function is used to enable the electrode; it should be
 *              used after the NT initialization, because the default state after the startup
 *              of the NT is electrode disabled.
 * \param electrode Pointer to the electrode params that identify the electrode.
 * \param touch Default parameter if the electrode was touched or not after the enable process.
 * \return result of operation \ref nt_result.
 * This is an example of using this function in the code:
  \code
    //The electrode that is defined in the setup of NT after the initialization must be enabled.
    if (nt_init(&system_0, nt_memory_pool, sizeof(nt_memory_pool)) < NT_SUCCESS)
    {
      while(1); // add code to handle this error 
    }
    // Enable electrode_0 that is defined in the setup of NT 
    if(nt_electrode_enable(&electrode_0, 0) != NT_SUCCESS)
    {
      printf("Enable electrode_0 failed.");
    }
    
   \endcode
 */
int32_t nt_electrode_enable(const struct nt_electrode *electrode, uint32_t touch);

/**
 * \brief Resume the electrode after the Suspending (temp.disabled), without reseting the keydetector.
 * \param electrode Pointer to the electrode params that identify the electrode.
 * \return result of operation \ref nt_result.
 * This is an example of using this function in the code:
  \code
    // re-enable electrode_0 that is defined in the setup of NT 
    if(nt_electrode_resume(&electrode_0) != NT_SUCCESS)
    {
      printf("Enable electrode_0 failed.");
    }
    
   \endcode
 */
int32_t nt_electrode_resume(const struct nt_electrode *electrode);

/**
 * \brief Suspend (temporary disable) the electrode.
 * \param electrode Pointer to the electrode params that identify the electrode.
 * \return result of operation \ref nt_result.
 * This is an example of using this function in code:
 *\code
 *  // Suspend electrode_0 that is defined in the setup of NT 
 *  if(nt_electrode_suspend(&electrode_0) != NT_SUCCESS)
 *  {
 *    printf("Suspending electrode_0 failed.");
 *  }
 *\endcode
*/
int32_t nt_electrode_suspend(const struct nt_electrode *electrode);

/**
 * \brief Disable the electrode.
 * \param electrode Pointer to the electrode params that identify the electrode.
 * \return result of operation \ref nt_result.
 * This is an example of using this function in code:
 *\code
 *  // Disable electrode_0 that is defined in the setup of NT 
 *  if(nt_electrode_disable(&electrode_0) != NT_SUCCESS)
 *  {
 *    printf("Disable electrode_0 failed.");
 *  }
 *\endcode
*/
int32_t nt_electrode_disable(const struct nt_electrode *electrode);

/**
 * \brief Reset the electrode to the current state.
 * \param electrode Pointer to the electrode params that identify the electrode.
 * \return result of operation \ref nt_result.
 * This is an example of using this function in code:
 *\code
 *  // Reset electrode_0 that is defined in the setup of NT 
 *  if(nt_electrode_reset(&electrode_0) != NT_SUCCESS)
 *  {
 *    printf("reset electrode_0 failed.");
 *  }
 *\endcode
*/
int32_t nt_electrode_reset(const struct nt_electrode *electrode);

/**
 * \brief Get the normalized and processed electrode signal.
 * \param electrode Pointer to the electrode data.
 * \return electrode signal calculated from the last raw value measured.
 *
 * The signal value is calculated from the raw electrode capacitance or other
 * physical signal by applying the filtering and normalization algorithms.
 * This signal is used by the "analog" \ref controls that estimate the finger
 * position based on the signal value, rather than on a simple touch / release status.
 * This is an example of using this function in the code:
 *\code
 *  // Get current signal of my_electrode 
 *  printf("The my_electrode has signal: %d.", nt_electrode_get_signal(&my_electrode));
 *\endcode
 */
uint32_t nt_electrode_get_signal(const struct nt_electrode *electrode);

/**
 * \brief Get the last known electrode status.
 * \param electrode Pointer to the electrode data.
 * \return Current electrode status.
 * \code
 *  // Get the latest status of my_electrode 
 *  char * electrode_state_name[3] = 
 * {
 *   "Initialize",
 *   "Released",
 *   "Touched"
 * };
 *  uint32_t state = nt_electrode_get_last_status(&my_electrode);
 *  printf("The my_electrode last status is: %s.", electrode_state_name[state]);
 * \endcode
 */
int32_t nt_electrode_get_last_status(const struct nt_electrode *electrode);

/**
 * \brief Get the time from the last electrode event.
 * \param electrode Pointer to the electrode data.
 * \return Time from the last electrode event.
 * \code
 *  // Get the time offset from the last change of the electrode status
 *  uint32_t offset = nt_electrode_get_time_offset(&my_electrode);
 *  printf("The my_electrode last status change has been before: %d ms .", offset);
 * \endcode
 */
uint32_t nt_electrode_get_time_offset(const struct nt_electrode *electrode);

/**
 * \brief Get the last known electrode time stamp.
 * \param electrode Pointer to the electrode data.
 * \return Current electrode status.
 * \code
 *  // Get the time stamp of the last change of the electrode status
 *  uint32_t time = nt_electrode_get_last_time_stamp(&my_electrode);
 *  printf("The my_electrode last status change was at: %d ms .", time);
 * \endcode
 */
uint32_t nt_electrode_get_last_time_stamp(const struct nt_electrode *electrode);   

/**
 * \brief Get the raw electrode signal.
 * \param electrode Pointer to the electrode data.
 * \return electrode Signal, as it is measured by the physical module.
 *
 * The raw signal is used internally by the filtering and normalization algorithms
 * to calculate the real electrode signal value, which is good to be compared
 * with the signals coming from other electrodes.
 *\code
 *  // Get the current raw signal of my_electrode 
 *  printf("The my_electrode has raw signal: %d.", nt_electrode_get_raw_signal(&my_electrode));
 *\endcode
 */
uint32_t nt_electrode_get_raw_signal(const struct nt_electrode *electrode);

/**
 * \brief Get electrode data structure pointer.
 * \param electrode Pointer to the electrode user parameter structure.
 * \return The pointer to the data electrode structure that is represented by the handled user parameter structure pointer.
 */
struct nt_electrode_data *nt_electrode_get_data(const struct nt_electrode *electrode);

/**
 * \brief Get electrode enabled status.
 * \param electrode Pointer to the electrode user parameter structure.
 * \return TRUE if the electrode is enabled, FALSE if disabled
 */
uint32_t nt_electrode_is_enabled(const struct nt_electrode *electrode);

/**
 * \brief Get module data structure pointer.
 * \param electrode Pointer to the electrode user parameter structure.
 * \return The pointer to the module data structure that is represented by the handled user parameter structure pointer.
 */
struct nt_module_data *nt_electrode_get_module_data(const struct nt_electrode *electrode);

#ifdef __cplusplus
}
#endif

/** \} end of electrodes_api group */
/** \} end of electrodes group */

#endif
