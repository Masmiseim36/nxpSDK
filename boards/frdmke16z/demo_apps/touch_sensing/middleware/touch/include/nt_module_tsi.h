/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_MODULE_TSI_H_
#define _NT_MODULE_TSI_H_

/**
 * \defgroup tsi TSI module
 * \ingroup modules
 *
 * The TSI module describes the hardware configuration and control of the elementary functionality
 * of the TSI peripheral; it covers all versions of the TSI peripheral by a generic
 * low-level driver API.
 *
 * The TSI Basic module is designed for processors that have the hardware TSI module
 * version 1, 2, or 4 (for example Kinetis L).
 *
 * The module also handles the NOISE mode supported by the TSI v4 (Kinetis L).
 * \{
 */

#include "nt_modules.h"

#include "nt_types.h"
#include "nt_electrodes.h"

#include "../source/drivers/tsi/nt_drv_tsi_driver.h"

#define NT_MODULE_TSI_NAME  "nt_module_tsi_interface"

/* ASM implemented macro                                                      */
#if defined(__CC_ARM)
  #define CUSTOM_DELAY() {__nop();}
#else
  #define CUSTOM_DELAY() {asm("NOP");}
#endif     
     
/**
 * The structure represents the Noise detection of the TSI v4 module.
 * An instance of this data type represents the Noise detection of the TSI v4 module. It contains the parameters
 * of Noise filters automatic sensitive calibration.
 *
 * You must initialize all the members before registering the noise in
 * the module. This structure can be allocated in ROM.
 */
struct nt_module_tsi_noise
{
  struct nt_filter_iir noise_filter;       /*!< Noise filter parameters */
  uint8_t              update_rate;        /*!< Period of the noise checking on the measured electrodes */
  uint8_t              noise_mode_timeout; /*!< Time of switching back to capacitive mode since last noise event */
};

/**
 * The main structure representing the Noise detection of the TSI v4 module.
 * An instance of this data type represents the Noise detection of the TSI v4 module. It contains the parameters
 * of the Noise filters automatic sensitive calibration.
 *
 * You must initialize all the members before registering the Noise in
 * the module. This structure can be allocated in ROM.
 */
struct nt_module_tsi_params
{
  struct nt_module_tsi_noise    noise;
  const  tsi_config_t         *config;  /*!< A pointer to the HW configuration. Can't be NULL. */
};

/**
 * The TSI module interface structure.
 */
extern const struct nt_module_interface nt_module_tsi_interface;   /*!< Can't be NULL. */

#if (NT_SAFETY_SUPPORT == 1)
/**
* \brief Automatically update TSI HW configuration structure used by release library from pointed structure.
*
*  The function copies all TSI HW configuration structure parameters from pointed structure to TSI HW configuration 
*  structure used by Release sensing. 
*
  \code
    // define nt_module structure, typically in nt_setup.c file
    struct nt_module tsi_module

    // call the nt_module_TSI_update function
    nt_module_TSI_update (&tsi_module);

  \endcode
* \param module         The pointer to the TSI module structure to load TSI HW configuration parameters
*/
void nt_module_TSI_update (struct nt_module const* module);     

/**
* \brief The function make virtual release on all electrodes assigned to TSI module by adding pull-up/down resistor.
*
* This function simulate the release events on all electrodes assigned to the TSI module. The release event is simulated by 
* changing the pin mode from the GPIO mode back to the TSI mode without the pull-up/down resistor. The function requires to assign the gpio_input
* into the nt_electrode structure, one gpio for the self-capacitance based electrode or the array of two gpios 
* for the mutual-capacitance based electrode.
*
  \code
    int32_t returned_status;
    returned_status = nt_module_TSI_ReleaseAll(0);
    if (returned_status != NT_SUCCESS)
    {
      // virtual release was not successful for all electrodes
    }

  \endcode
* \param instance       The TSI module instance.
* \return
*   - NT_SUCCESS if TSI register has been updated
*   - NT_FAILURE if TSI register has not been updated
*/
int32_t nt_module_TSI_ReleaseAll (uint32_t instance);

/**
* \brief The function make virtual the release on the specified electrodes assigned in the second function parameter by switching back to TSI mode.
*
* This function simulate the release events on the specified electrodes defined as the bits in the second function parameters. The release event  
* is simulated by changing the pin mode from GPIO mode back to TSI mode without pull-up/down resistor. The function requires to assign the gpio_input
* into the nt_electrode structure, one gpio for the self-capacitance based electrode or the array of two gpios 
* for the mutual-capacitance based electrode.
*
  \code
    int32_t returned_status;
    uint64_t enabledElectrodes = 0x7;
    returned_status = nt_module_TSI_Release(0, enabledElectrodes);
    if (returned_status != NT_SUCCESS)
    {
      // virtual release was not successful for the specified electrodes
    }

  \endcode
* \param instance       The TSI module instance.
* \return
*   - NT_SUCCESS if the specified electrodes have been switched back to the TSI mode
*   - NT_FAILURE if the specified electrodes have not been switched back to the TSI mode
*/
int32_t nt_module_TSI_Release (uint32_t instance, uint64_t enabledElectrode);

/**
* \brief The function make virtual release on one specified electrode by switching back to the TSI mode.
*
* This function simulate the release events on one electrode assigned as function input electrode pointer. The release event is simulated by 
* changing the pin mode from TSI channel to GPIO mode with pull-up/down resistor. For self-capacitance based electrode 
* the pull-up resistor is used to simulate release event and for mutual-capacitance based the pull-down resistor applied on 
* receiver pin is used to simulate release event. Considering the fact that all receiver pins are routed together the 
* capacity change will be applied to all electrodes with connected rx pins. The function requires to assign the gpio_input
* into the nt_electrode structure, one gpio for the self-capacitance based electrode or the array of two gpios 
* for the mutual-capacitance based electrode.
*
  \code
    int32_t returned_status;
    returned_status = nt_module_TSI_ReleaseOneElect(&El_0);
    if (returned_status != NT_SUCCESS)
    {
      // virtual release was not successful for one specified electrode
    }

  \endcode
* \param instance       The TSI module instance.
* \return
*   - NT_SUCCESS if specified electrode has been switched back to the TSI mode
*   - NT_FAILURE if specified electrode has not been switched back to the TSI mode
*/
int32_t nt_module_TSI_ReleaseOneElect (struct nt_electrode* electrode);

/**
* \brief The function make the virtual release on one specified electrode assigned to the TSI module by switching back to the TSI mode.
*
* This function simulate the release events on all electrodes assigned to the TSI modul. The release event is simulated by 
* changing back the pin mode from the GPIO mode with the pull-up/down resistor to the TSI channel. The function requires to assign the gpio_input
* into the nt_electrode structure, one gpio for the self-capacitance based electrode or the array of two gpios 
* for the mutual-capacitance based electrode.
*
  \code
    int32_t returned_status;
    returned_status = nt_module_TSI_TouchAll(0);
    if (returned_status != NT_SUCCESS)
    {
      // virtual touch was not successful for all electrodes
    }

  \endcode
* \param instance       The TSI module instance.
* \return
*   - NT_SUCCESS if all electrodes have been switched to GPIO mode with pull-up/down
*   - NT_FAILURE if all electrodes have not been switched to GPIO mode with pull-up/down
*/
int32_t nt_module_TSI_TouchAll (uint32_t instance);

/**
* \brief The function make the virtual touch on the selected electrodes assigned in the second function parameter by adding pull-up/down resistor.
*
* This function simulate the touch events on the selected electrodes defined as the bits in the second function parameter. The touch event 
* is simulated by changing the pin mode from TSI channel to GPIO mode with pull-up/down resistor. For self-capacitance based electrode 
* the pull-up resistor is used to simulate touch event and for mutual-capacitance based the pull-down resistor applied on 
* receiver pin is used to simulate touch event. Considering the fact that all receiver pins are routed together the 
* capacity change will be applied to all electrodes with connected rx pins.The function requires to assign the gpio_input
* into the nt_electrode structure, one gpio for the self-capacitance based electrode or the array of two gpios 
* for the mutual-capacitance based electrode.
*
  \code
    int32_t returned_status;
    uint64_t enabledElectrodes = 0x7;
    returned_status = nt_module_TSI_Touch(0, enabledElectrodes);
    if (returned_status != NT_SUCCESS)
    {
      // virtual touch was not successful for the specified electrodes
    }

  \endcode
* \param instance       The TSI module instance.
* \return
*   - NT_SUCCESS if the specified electrodes have been switched to GPIO mode with pull-up/down
*   - NT_FAILURE if the specified electrodes have not been switched to GPIO mode with pull-up/down
*/
int32_t nt_module_TSI_Touch (uint32_t instance, uint64_t enabledElectrode);

/**
* \brief The function make virtual touch on one specified electrode by adding the pull-up/down resistor.
*
* This function simulate the touch events on one electrode assigned as the function input electrode pointer. The touch event is simulated by 
* changing the pin mode from the TSI channel to the GPIO mode with pull-up/down resistor. For the self-capacitance based electrode 
* the pull-up resistor is used to simulate the touch event and for mutual-capacitance based the pull-down resistor applied on 
* receiver pin is used to simulate the touch event. Considering the fact that all receiver pins are routed together the 
* capacity change will be applied to all electrodes with connected rx pins. The function requires to assign the gpio_input
* into the nt_electrode structure, one gpio for the self-capacitance based electrode or the array of two gpios 
* for the mutual-capacitance based electrode.
*
  \code
    int32_t returned_status;
    returned_status = nt_module_TSI_TouchOneElect(&El_0);
    if (returned_status != NT_SUCCESS)
    {
      // virtual touch was not successful for one specified electrode
    }

  \endcode
* \param instance       The TSI module instance.
* \return
*   - NT_SUCCESS if one specified electrode has been switched to GPIO mode with pull-up/down
*   - NT_FAILURE if one specified electrodes has not been switched to GPIO mode with pull-up/down
*/
int32_t nt_module_TSI_TouchOneElect (struct nt_electrode* electrode);
#endif /* NT_SAFETY_SUPPORT */

void delay(void);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/** \} */ /* end of the TSI group */

#endif
