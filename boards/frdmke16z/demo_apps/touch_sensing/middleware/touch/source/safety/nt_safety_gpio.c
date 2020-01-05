 /*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
#include "fsl_device_registers.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#if defined(FSL_FEATURE_TSI_VERSION)
#include "fsl_ftm.h"
#endif
#include "nt.h"
#include "fsl_clock.h"

#define USED_FTM        0
#define BOARD_FTM_BASEADDR FTM0

FGPIO_Type * g_gpioBaseAddr[] = FGPIO_BASE_PTRS;
PORT_Type * g_portBaseAddr[] = PORT_BASE_PTRS;
FTM_Type * g_ftmBaseAddr[] = FTM_BASE_PTRS;

#define FTM_BASE_ADDR (g_ftmBaseAddr[USED_FTM])

/* Set pin direction */
void set_pin_dir(FGPIO_Type *base, uint32_t pin, const gpio_pin_config_t *config);

/* Configure pin as gpio output */
void set_pin_ouput(uint32_t port, uint32_t pin)
{
  
    gpio_pin_config_t pin_cfg = {
        kGPIO_DigitalOutput, 0,
    };
 
  set_pin_dir(g_gpioBaseAddr[port], pin, &pin_cfg);
}

/* Configure pin as gpio input. */
void set_pin_input(uint32_t port, uint32_t pin)
{
    gpio_pin_config_t pin_cfg = {
        kGPIO_DigitalInput, 0,
    };
    
  set_pin_dir(g_gpioBaseAddr[port], pin, &pin_cfg);
}

/* Configure pin value as logic 0. */
void set_pin_low(uint32_t port, uint32_t pin)
{
  FGPIO_PinWrite(g_gpioBaseAddr[port], pin, 0U);
}

/* Configure pin value as logic 1. */
void set_pin_high(uint32_t port, uint32_t pin)
{
  FGPIO_PinWrite(g_gpioBaseAddr[port], pin, 1U);
}

/* Reads the pin value. */
uint32_t get_pin_value(uint32_t port, uint32_t pin)
{ 
  return FGPIO_PinRead(g_gpioBaseAddr[port], pin);
}

/* Configure pin as gpio output and set output to logic 1.  */
void set_pin_default_state(uint32_t port, uint32_t pin)
{      
  set_pin_ouput(port, pin);
  set_pin_high(port, pin);
}

/* Initialize the FlexTimer.*/
void init_timer(void)
{
  ftm_config_t ftmInfo;
  
  FTM_GetDefaultConfig(&ftmInfo);
  FTM_Init(FTM_BASE_ADDR, &ftmInfo);
  
  /* Set the timer to be in free-running mode */
  BOARD_FTM_BASEADDR->MOD = 0xFFFF;
  BOARD_FTM_BASEADDR->CNT = 0U;
}

/* Start the FlexTimer. */
void start_timer(void)
{  
  FTM_StartTimer(FTM_BASE_ADDR, kFTM_SystemClock);
  FTM_ClearStatusFlags(FTM_BASE_ADDR, kFTM_TimeOverflowFlag);
}

/* Stop the FlexTimer.*/
void stop_timer(void)
{
  FTM_StopTimer(FTM_BASE_ADDR);
}

/* Reset the FlexTimer. */
void timer_reset_counter(void)
{  
  BOARD_FTM_BASEADDR->CNT = 0U;
}

/* Reads FlexTimer counted value. */
uint32_t timer_get_counter(void)
{
  uint32_t counter_val;
  counter_val = BOARD_FTM_BASEADDR->CNT;
  return counter_val;
}

/* Reads FlexTimer overflow.*/
uint32_t timer_get_overrun(void)
{
    /* check and return TOF flag */   
    if(FTM_GetStatusFlags(FTM_BASE_ADDR) & kFTM_TimeOverflowFlag)
    {
        return 1U;
    }
    else
    {
        return 0U;
    }   
}

/* Reads FlexTimer overrun. */
void set_pin_pull_up(uint32_t port, uint32_t pin)
{
  port_pin_config_t pcr_cfg;
  
  pcr_cfg.pullSelect = kPORT_PullUp; /* pull-up */
  pcr_cfg.mux = kPORT_MuxAsGpio;
  pcr_cfg.passiveFilterEnable = kPORT_PassiveFilterDisable;
  pcr_cfg.driveStrength = kPORT_LowDriveStrength;
   
  PORT_SetPinConfig(g_portBaseAddr[port], pin, &pcr_cfg);  
}

/* Configures pin to have pull down resistor. */
void set_pin_pull_down(uint32_t port, uint32_t pin)
{
  port_pin_config_t pcr_cfg;
  
  pcr_cfg.pullSelect = kPORT_PullDown; /* pull-down */
  pcr_cfg.mux = kPORT_MuxAsGpio;
  pcr_cfg.passiveFilterEnable = kPORT_PassiveFilterDisable;
  pcr_cfg.driveStrength = kPORT_LowDriveStrength;
   
  PORT_SetPinConfig(g_portBaseAddr[port], pin, &pcr_cfg);
}

/* Disable pin pull resistor. */
void dis_pin_pull(uint32_t port, uint32_t pin)
{
  port_pin_config_t pcr_cfg;
  
  pcr_cfg.pullSelect = kPORT_PullDisable; /* pull-disable */
  pcr_cfg.mux = kPORT_MuxAsGpio;
  pcr_cfg.passiveFilterEnable = kPORT_PassiveFilterDisable;
  pcr_cfg.driveStrength = kPORT_LowDriveStrength;
   
  PORT_SetPinConfig(g_portBaseAddr[port], pin, &pcr_cfg);
}

/* Configure pin as TSI peripheral pin. */ 
void set_pin_tsi_mode(uint32_t port, uint32_t pin)
{
  port_pin_config_t pcr_cfg;
  
  pcr_cfg.pullSelect = kPORT_PullDisable; 
  pcr_cfg.mux = kPORT_PinDisabledOrAnalog; /* Analog = TSI */
  pcr_cfg.passiveFilterEnable = kPORT_PassiveFilterDisable;
  pcr_cfg.driveStrength = kPORT_LowDriveStrength;
   
  PORT_SetPinConfig(g_portBaseAddr[port], pin, &pcr_cfg);
}

/* Set GPIO mode */
void set_pin_gpio_mode(uint32_t port, uint32_t pin)
{
  port_pin_config_t pcr_cfg;
  
  pcr_cfg.pullSelect = kPORT_PullDisable; 
  pcr_cfg.mux = kPORT_MuxAsGpio; /* GPIO */
  pcr_cfg.passiveFilterEnable = kPORT_PassiveFilterDisable;
  pcr_cfg.driveStrength = kPORT_LowDriveStrength;
   
  PORT_SetPinConfig(g_portBaseAddr[port], pin, &pcr_cfg);
}

/* GPIO interface structure */
const struct nt_module_gpio_user_interface gpio_interface = {
  .set_pin_output = set_pin_ouput,
  .set_pin_input = set_pin_input,
  .set_pin_low = set_pin_low,
  .set_pin_high =  set_pin_high,
  .get_pin_value = get_pin_value,
  .set_pin_default_state = set_pin_default_state,
  .init_timer = init_timer,
  .start_timer = start_timer,
  .stop_timer = stop_timer,
  .timer_reset_counter = timer_reset_counter,
  .timer_get_counter = timer_get_counter,
  .timer_get_overrun = timer_get_overrun,
  .set_pin_pull_up = set_pin_pull_up,
  .set_pin_pull_down = set_pin_pull_down,
  .set_pin_tsi_mode = set_pin_tsi_mode,
  .set_pin_gpio_mode = set_pin_gpio_mode, 
};

/* Configure pin as gpio output. */
void configure_gpio_touch_sensing_pins(uint32_t instance)
{
  port_pin_config_t pcr_cfg;
  
  pcr_cfg.pullSelect = kPORT_PullDisable;
  pcr_cfg.mux = kPORT_MuxAsGpio;
  pcr_cfg.passiveFilterEnable = kPORT_PassiveFilterDisable;
  pcr_cfg.driveStrength = kPORT_HighDriveStrength;
    
  switch(instance) {
    case 0:                      /* configuration 0 */
      /* Affects PORTA_PCR4 register */
      PORT_SetPinConfig(PORTA,4u, &pcr_cfg);
      /* Affects PORTB_PCR3 register */
      PORT_SetPinConfig(PORTB,3u, &pcr_cfg);
      /* Affects PORTB_PCR2 register */
      PORT_SetPinConfig(PORTB,2u, &pcr_cfg);
      /* Affects PORTB_PCR16 register */
      PORT_SetPinConfig(PORTB,16u, &pcr_cfg);
      break;   
    default:
      break;
  }
}

/* Set pin direction */
void set_pin_dir(FGPIO_Type *base, uint32_t pin, const gpio_pin_config_t *config)
{
    if (config->pinDirection == kGPIO_DigitalInput)
    {
        base->PDDR &= ~(1U << pin);
    }
    else
    {
        base->PDDR |= (1U << pin);
    }
}
