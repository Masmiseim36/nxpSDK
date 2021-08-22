/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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

#if (FSL_FEATURE_TSI_VERSION == 5)
#include "fsl_ftm.h"
extern FGPIO_Type *g_gpioBaseAddr[];
extern PORT_Type *g_portBaseAddr[];
extern FTM_Type *g_ftmBaseAddr[];
#endif

#define NT_MODULE_SAFETY_NAME "nt_module_safety_interface"

/**
 * \brief Reads FlexTimer overflow.
 * \return 1 if there was FlexTimer overrun, 0 if not
 *
 * Function check and return TOF flag.
  \code
    #include "gpio.h"
    uint32_t testResult;
    testResult = timer_get_overrun();
  \endcode
 */
uint32_t timer_get_overrun(void);

/**
 * \brief Stop the FlexTimer.
 * \return void
 *
   Function Stop the FlexTimer to count.
  \code
    #include "gpio.h"
    stop_timer();
  \endcode
 */
void stop_timer(void);

/**
 * \brief Initialize the FlexTimer.
 * \return void
 *
 * Function Initialize the FlexTimer to be in free-running mode.
  \code
    #include "gpio.h"
    init_timer();
  \endcode
 */
void init_timer(void);

/**
 * \brief Configure pin as gpio output.
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function configures selected pin of the selected port as gpio output.
  \code
    #include "gpio.h"
    uint32_t port = 2;
    uint32_t pin = 5;
    set_pin_ouput(port, pin);
  \endcode
 */
void set_pin_ouput(uint32_t port, uint32_t pin);

/**
 * \brief Configure pin value as logic 0.
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function configures selected pin of the selected port as logic 0.
  \code
    #include "gpio.h"
    uint32_t port = 2;
    uint32_t pin = 5;
    set_pin_low(port, pin);
  \endcode
 */
void set_pin_low(uint32_t port, uint32_t pin);

/**
 * \brief Configure pin value as logic 1.
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function configures selected pin of the selected port as logic 1.
  \code
    #include "gpio.h"
    uint32_t port = 2;
    uint32_t pin = 5;
    set_pin_high(port, pin);
  \endcode
 */
void set_pin_high(uint32_t port, uint32_t pin);

/**
 * \brief Reads the pin value.
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return pin value in uint32_t data type
 *
 * Function reads selected pin value of the selected port.
  \code
    #include "gpio.h"
    uint32_t port = 2;
    uint32_t pin = 5;
    uint32_t testResult;
    testResult = get_pin_value(port, pin);
  \endcode
 */
uint32_t get_pin_value(uint32_t port, uint32_t pin);

/**
 * \brief Configure pin as gpio output and set output to logic 1.
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function configures selected pin of the selected port as gpio output and set output to logic 1.
  \code
    #include "gpio.h"
    uint32_t port = 2;
    uint32_t pin = 5;
    set_pin_default_state(port, pin);
  \endcode
 */
void set_pin_default_state(uint32_t port, uint32_t pin);

/**
 * \brief Start the FlexTimer.
 * \return void
 *
 * Function Start the FlexTimer to count.
  \code
    #include "gpio.h"
    start_timer();
  \endcode
 */
void start_timer(void);

/**
 * \brief Reset the FlexTimer.
 * \return void
 *
 * Function resets the FlexTimer counter.
  \code
    #include "gpio.h"
    timer_reset_counter();
  \endcode
 */
void timer_reset_counter(void);

/**
 * \brief Reads FlexTimer counted value.
 * \return uint32_t FlexTimer counter value
 *
 * Function reads FlexTimer counted value.
  \code
    #include "gpio.h"
    uint32_t testResult;
    testResult = timer_get_counter();
  \endcode
 */
uint32_t timer_get_counter(void);

/**
 * \brief Configures pin to have pull up resistor.
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function configure selected pin to have pull up resistor.
  \code
    #include "gpio.h"
    uint32_t port = 2;
    uint32_t pin = 5;
    set_pin_pull_up(port, pin);
  \endcode
 */
void set_pin_pull_up(uint32_t port, uint32_t pin);

/**
 * \brief Configures pin to have pull down resistor.
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function configure selected pin to have pull down resistor.
  \code
    #include "gpio.h"
    uint32_t port = 2;
    uint32_t pin = 5;
    set_pin_pull_down(port, pin);
  \endcode
 */
void set_pin_pull_down(uint32_t port, uint32_t pin);

/**
 * \brief Disable pin pull resistor.
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function disable selected pin pull resistor.
  \code
    #include "gpio.h"
    uint32_t port = 2;
    uint32_t pin = 5;
    dis_pin_pull(port, pin);
  \endcode
 */
void dis_pin_pull(uint32_t port, uint32_t pin);

/**
 * \brief Configure pin as TSI peripheral pin.
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function configures selected pin of the selected port as TSI peripheral pin.
  \code
    #include "gpio.h"
    uint32_t port = 2;
    uint32_t pin = 5;
    set_pin_tsi_mode(port, pin);
  \endcode
 */
void set_pin_tsi_mode(uint32_t port, uint32_t pin);

/**
 * \brief Configure pin as GPIO pin.
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function configures selected pin of the selected port as GPIO pin.
  \code
    #include "gpio.h"
    uint32_t port = 2;
    uint32_t pin = 5;
    set_pin_gpio_mode(port, pin);
  \endcode
 */
void set_pin_gpio_mode(uint32_t port, uint32_t pin);

/**
 * \brief Configure pin as gpio output.
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function configures selected pin of the selected port as gpio input.
  \code
    #include "gpio.h"
    uint32_t port = 2;
    uint32_t pin = 5;
    testResult = set_pin_ouput(port, pin);
  \endcode
 */
void configure_gpio_touch_sensing_pins(uint32_t instance);

/**
 * \brief Configure pin as gpio input.
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function configures selected pin of the selected port as gpio input.
  \code
    #include "gpio.h"
    uint32_t port = 2;
    uint32_t pin = 5;
    set_pin_input(port, pin);
  \endcode
 */
void set_pin_input(uint32_t port, uint32_t pin);

/* Forward declaration */
struct nt_module;
struct nt_electrode;

struct nt_module_safety_interface;

/** GPIO interface structure for the Safety tests */
struct nt_module_safety_gpio_params
{
    const struct nt_module_gpio_user_interface *user_interface;
    uint32_t
        delay_interval; /*!< Delay interval to to apply the physical changes on the GPIO pin and equalize charge. */
};

/**
 * \defgroup gsafety General API
 * \ingroup safety
 * General Function definition of the safety.
 *
 * \{
 */

/**
 * The Safety interface structure.
 */
extern const struct nt_module_safety_interface nt_safety_interface; /*!< Can't be NULL. */
/**
 * The Safety GPIO interface structure.
 */
extern const struct nt_module_gpio_user_interface gpio_interface; /*!< Can't be NULL. */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Check the safety interface.
 * \return the gpio user interface status
 *
 * Check the status safety gpio user interface and check if the GPIO pins and the timer function are properly defined.
  \code
    int32_t testResult;
    testResult = nt_module_safety_gpio_check_data(nt_electrode_get_module_data(&my_electrode));
  \endcode
 */
int32_t nt_module_safety_gpio_check_data(const struct nt_module_data *module);

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
    int32_t testResult;
    testResult = nt_module_safety_check_electrode_short_vdd_set(&my_electrode);
  \endcode
 */
int32_t nt_module_safety_check_electrode_short_vdd_set(const struct nt_electrode *elec);

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
    struct nt_module_data *module = _nt_module_get_data(_nt_system_get_module((uint32_t)&nt_module_tsi_interface,
 instance)); NT_ASSERT(module != NULL);
    struct nt_electrode const *electrode = module->rom->electrodes[0];
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
    struct nt_module_data *module = _nt_module_get_data(_nt_system_get_module((uint32_t)&nt_module_tsi_interface,
 instance)); NT_ASSERT(module != NULL);
    struct nt_electrode const *electrode = module->rom->electrodes[0];
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
