/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_SAFETY_H_
#define _NT_SAFETY_H_

/**
 * \defgroup safety Safety
 * \ingroup ntapi
 * Safety represents the check layer for the FMEA tests in the NXP Touch system;
 * it is the layer that is tightly coupled with the hardware module available on
 * the NXP MCU device and the GPIO pins.
 * \{
 */

#include "nt_types.h"
#include "nt_filters.h"
#include "nt_electrodes.h"
#include "../source/safety/nt_safety_crc.h"

#define NT_MODULE_SAFETY_NAME  "nt_module_safety_interface"

/* Forward declaration */
struct nt_module;
struct nt_electrode;

struct nt_module_safety_interface;



/** GPIO interface structure for the Safety tests */
struct nt_module_safety_gpio_params {
    const struct nt_module_gpio_user_interface *user_interface;  
    uint32_t delay_interval;            /*!< Delay interval to to apply the physical changes on the GPIO pin and equalize charge. */
};


/**
 * \defgroup gsafety General API
 * \ingroup safety
 * General Function definition of the safety.
 *
 * \{
 */

/**
 * The TSI module interface structure.
 */
extern const struct nt_module_safety_interface nt_safety_interface;   /*!< Can't be NULL. */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Check the safety interface. 
 * \return the gpio user interface status
 *
 * Check the status safety gpio user interface and check if the GPIO pins and the timer function are properly defined.    
  \code
    // Complete the safety test of all electrodes from the module where my_electrode is defined.  
    int32_t testResult;
    testResult = nt_module_safety_gpio_check_data(nt_electrode_get_module_data(&my_electrode));
  \endcode
 */ 
int32_t nt_module_safety_gpio_check_data(const struct nt_module_data *module);

/**
 * \brief Short to ground (GND) test.
 * \return The test result status (success or failure).
 *
 * This function tests the possible electrode short to the GND. Firstly, the function
 * switches the electrode pin to the GPIO input mode with the pull-up enabled. The pin value
 * is read from the port. The "logic One" value is expected for a good, undamaged electrode and 
 * the success result. If "logic Zero" is detected, the failure status is returned.
 * When the mutual-cap mode is defined for the electrode, both of the associated (Rx and Tx) pin  
 * shorts are tested.
  \code
    // Test if "my_electrode" is shorted to the GND
    int32_t testResult;
    testResult = nt_module_safety_check_electrode_short_gnd(&my_electrode);
  \endcode
 */
int32_t nt_module_safety_check_electrode_short_gnd_set(const struct nt_electrode *elec);

/**
 * \brief Short to ground (GND) test.
 * \return The test result status (success or failure).
 *
 * This function tests the possible electrode short to the GND. Firstly, the function
 * switches the electrode pin to the GPIO input mode with the pull-up enabled. The pin value
 * is read from the port. The "logic One" value is expected for a good, undamaged electrode and 
 * the success result. If "logic Zero" is detected, the failure status is returned.
 * When the mutual-cap mode is defined for the electrode, both of the associated (Rx and Tx) pin  
 * shorts are tested.
  \code
    // Test if "my_electrode" is shorted to the GND
    int32_t testResult;
    testResult = nt_module_safety_check_electrode_short_gnd(&my_electrode);
  \endcode
 */
int32_t nt_module_safety_check_electrode_short_gnd_read(const struct nt_electrode *elec);
/**
 * \brief Short to the power supply (VDD) test.
 * \return The test result status (success or failure).
 *
 * This function tests the possible electrode short to the VDD. Firstly, the function
 * switches the electrode pin to the GPIO input mode with pull-down enabled. The pin value
 * is read from the port. The logic zero value is expected for a good, undamaged electrode and 
 * the success result. If the logic one is detected, the failure status is returned.
 * When the mutual-cap mode is defined for the electrode, both of the associated (Rx and Tx) pin  
 * shorts are tested.
  \code
    // Test if "my_electrode" is shorted to VDD
    int32_t testResult;
    testResult = nt_module_safety_check_electrode_short_vdd(&my_electrode);
  \endcode
 */
int32_t nt_module_safety_check_electrode_short_vdd(const struct nt_electrode *elec);

/**
 * \brief Short one electrode with the other one test.
 * \return The test result status (success or failure).
 *
 * This function tests a possible electrode short to another electrode. Firstly, 
 * the function changes all electrode pins (except for the tested electrode) to the GPIO mode 
 * with the logic zero output. The tested electrode is set as a GPIO input with pull-up enabled.
 * The level of the tested electrode is then read from the port. The "logic One" value is expected 
 * for an undamaged electrode and the success result. If "logic Zero" is detected, 
 * the failure status is returned.
 * When the mutual-cap mode is defined for an electrode, both of the associated (Rx and Tx) pins  
 * are tested for shorts.
  \code
    // Test if "my_electrode" is shorted to another electrode
    int32_t testResult;
    testResult = nt_module_safety_check_electrode_short_adj(&my_electrode);
  \endcode
 */
int32_t nt_module_safety_check_electrode_short_adj(const struct nt_electrode *elec);

/**
 * \brief Electrode baseline too low level test
 * \return The test result status (success or failure).
 *
 * This function reads the actual baseline level of the tested electrode and compares it 
 * to the stored baseline level. If the actual baseline value is approximately 12.5% lower than the 
 * stored baseline, the function returns the failure status. If the actual baseline
 * level is higher than 87.5% of the stored baseline, the function returns the 
 * success status. 
  \code
    // Test the baseline low level for the already defined my_electrode 
    int32_t testResult;
    testResult = nt_module_safety_check_baseline_low(&my_electrode);
  \endcode
 */
int32_t nt_module_safety_check_baseline_low(const struct nt_electrode *elec);

/**
 * \brief Electrode baseline too high level test
 * \return The test result status (success or failure).
 *
 * This function reads the actual baseline level of the tested electrode and compares it 
 * to the stored baseline level. If the actual baseline value is 25% higher than the 
 * stored baseline, the function returns the failure status. If the actual baseline
 * level is lower than 125% of the stored baseline, the function returns the 
 * success status. If all tests passed, the function returns the success status. If not, the 
 * failure status is returned. 
  \code
    // Test the baseline high level for the already defined my_electrode 
    int32_t testResult;
    testResult = nt_module_safety_check_baseline_high(&my_electrode);
  \endcode
 */
int32_t nt_module_safety_check_baseline_high(const struct nt_electrode *elec);

/**
 * \brief Execute all safety tests.
 * \return The test result status (success or failure).
 *
 * The function is a high level safety function that checks all electrodes for shorts to 
 * the GND, VDD, or other electrodes and it executes the baseline too low and high level tests. 
 * To make all tests, the electrodes must be switched to the GPIO mode using the 
 * _nt_module_safety_switch_electrodes_gpio function. After the test, the electrodes are 
 * switched back using the _nt_module_safety_switch_electrodes_tsi function.  
  \code
    // Complete the safety test of all electrodes from the module where my_electrode is defined.  
    int32_t testResult;
    testResult = nt_module_safety_check_electrodes (nt_electrode_get_module_data(&my_electrode));
  \endcode
 */
int32_t nt_module_safety_check_electrodes(struct nt_module_data *module);

/**
 * \brief Function change electrode pin mode from TSI to GPIO.
 * \return The test result status (success or failure).
 *
 * The function read the gpio and pin information from gpio_input member of the electrode structure and 
 * and change this pin to GPIO mode.    
  \code
    // get module data
    struct nt_module_data *module = _nt_module_get_data(_nt_system_get_module((uint32_t)&nt_module_tsi_interface, instance));
    NT_ASSERT(module != NULL);
    // switch electrode 0 to GPIO
    struct nt_electrode const* electrode = module->rom->electrodes[0];
    int32_t Result = _nt_module_safety_switch_electrode_gpio(electrode);
  \endcode
 */
int32_t _nt_module_safety_switch_electrode_gpio(const struct nt_electrode *elec);

/**
 * \brief Function change electrode pin mode from GPIO to TSI.
 * \return The test result status (success or failure).
 *
 * Function change electrode pin mode from GPIO to TSI.   
  \code
    // get module data
    struct nt_module_data *module = _nt_module_get_data(_nt_system_get_module((uint32_t)&nt_module_tsi_interface, instance));
    NT_ASSERT(module != NULL);
    // switch electrode 0 to GPIO
    struct nt_electrode const* electrode = module->rom->electrodes[0];
    int32_t Result = _nt_module_safety_switch_electrode_tsi(electrode);
  \endcode
 */
int32_t _nt_module_safety_switch_electrode_tsi(const struct nt_electrode *elec);

#ifdef __cplusplus
}
#endif

/** \} end of gsafety_api group */
/** \} end of safety group */

#endif
