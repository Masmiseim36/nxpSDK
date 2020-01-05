 /*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _NT_SAFETY_GPIO_H_
#define _NT_SAFETY_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup safetyGPIO SafetyGPIO
 * \ingroup ntapi
 * SafetyGPIO represents the functions to support GPIO or timer periferal configuration. 
 *
 * \{
 */    

/**
 * \brief Configure pin as gpio output. 
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function configures selected pin of the selected port as gpio output. 
  \code
    #include "gpio.h"         
    // Set pin 5 of the port 2 (PTC) as gpio output  
    uint32_t port = 2;
    uint32_t pin = 5;    
    set_pin_ouput(port, pin);
  \endcode
 */  
void set_pin_ouput(uint32_t port, uint32_t pin);

/**
 * \brief Configure pin as gpio input. 
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function configures selected pin of the selected port as gpio input. 
  \code
    #include "gpio.h"         
    // Set pin 5 of the port 2 (PTC) as gpio input  
    uint32_t port = 2;
    uint32_t pin = 5;    
    set_pin_input(port, pin);
  \endcode
 */
void set_pin_input(uint32_t port, uint32_t pin);

/**
 * \brief Configure pin value as logic 0. 
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function configures selected pin of the selected port as logic 0. 
  \code
    #include "gpio.h"         
    // Set pin 5 of the port 2 (PTC) as logic 0  
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
    // Set pin 5 of the port 2 (PTC) as logic 1 
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
    // Set pin 5 of the port 2 (PTC) as gpio output  
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
    // Set pin 5 of the port 2 (PTC) as gpio output  
    uint32_t port = 2;
    uint32_t pin = 5;    
    set_pin_default_state(port, pin);
  \endcode
 */
void set_pin_default_state(uint32_t port, uint32_t pin);

/**
 * \brief Initialize the FlexTimer. 
 * \return void
 *
 * Function Initialize the FlexTimer to be in free-running mode.
  \code
    #include "gpio.h"         
    // Initialize the FlexTimer.      
    init_timer();
  \endcode
 */
void init_timer(void);

/**
 * \brief Start the FlexTimer. 
 * \return void
 *
 * Function Start the FlexTimer to count.
  \code
    #include "gpio.h"         
    // Start the FlexTimer.      
    start_timer();
  \endcode
 */
void start_timer(void);

/**
 * \brief Stop the FlexTimer. 
 * \return void
 *
   Function Stop the FlexTimer to count.
  \code
    #include "gpio.h"         
    // Stop the FlexTimer.      
    stop_timer();
  \endcode
 */
void stop_timer(void);

/**
 * \brief Reset the FlexTimer. 
 * \return void
 *
 * Function resets the FlexTimer counter.
  \code
    #include "gpio.h"         
    // Reset the FlexTimer.      
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
    // Reads FlexTimer counted value.  
    uint32_t testResult;
    testResult = timer_get_counter();
  \endcode
 */
uint32_t timer_get_counter(void);

/**
 * \brief Reads FlexTimer overflow. 
 * \return 1 if there was FlexTimer overrun, 0 if not 
 *
 * Function check and return TOF flag. 
  \code
    #include "gpio.h"         
    // Get FlexTimer overrun state.  
    uint32_t testResult;
    testResult = timer_get_overrun();
  \endcode
 */
uint32_t timer_get_overrun(void);

/**
 * \brief Configures pin to have pull up resistor. 
 * \param port GPIO port where pin is located.
 * \param pin GPIO pin.
 * \return void
 *
 * Function configure selected pin to have pull up resistor. 
  \code
    #include "gpio.h"         
    // Set pin 5 of the port 2 (PTC) to have pull up resistor. 
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
    // Set pin 5 of the port 2 (PTC) to have pull down resistor. 
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
    // Disable pin 5 of the port 2 (PTC) pull resistor. 
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
    // Set pin 5 of the port 2 (PTC) as gpio output  
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
    // Set pin 5 of the port 2 (PTC) as gpio output  
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
    // Set pin 5 of the port 2 (PTC) as gpio output  
    uint32_t port = 2;
    uint32_t pin = 5;    
    testResult = set_pin_ouput(port, pin);
  \endcode
 */
void configure_gpio_touch_sensing_pins(uint32_t instance);

#ifdef __cplusplus
}
#endif

/** \} */ /* end of nt_safetygpio private  group */
#endif  /* _NT_SAFETY_GPIO_H_ */
