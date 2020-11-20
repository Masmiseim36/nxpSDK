/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_CONFIG_H_
#define _SAFETY_CONFIG_H_

#include "LPC55S69_cm33_core0.h"
#include "iec60730b.h"
#include "project_setup_lpcxpresso55s69.h"
#include "safety_cm33_lpc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* This macro enables infinity while loop in SafetyErrorHandling() function */
#define SAFETY_ERROR_ACTION  1

/* TEST SWITCHES - for debugging it is better to turn the flash test and watchdog OFF */
#define ADC_TEST_ENABLED     0
#define CLOCK_TEST_ENABLED   0
#define DIO_TEST_ENABLED     1
#define FLASH_TEST_ENABLED   0
#define PC_TEST_ENABLED      1
#define WATCHDOG_ENABLED     0
   
/* CLOCK test */
#define USE_WKT 0 /*USE CTIMER = 0 USE WKT = 1 */
#define CLOCK_ERROR_HANDLING 1
#define REF_TIMER_USED                CTIMER0
#define REF_TIMER_CLOCK_FREQUENCY     1e06
#define SYSTICK_RELOAD_VALUE          96000
#define ISR_FREQUENCY                 1000 /* Hz */
#define CLOCK_TEST_TOLERANCE          20   /* % */

/********* Watchdog *********/
#define WDOG_REF_TIMER_BASE   CTIMER0
#define RESET_DETECT_REGISTER &(PMC->AOREG1)
#define RESET_DETECT_MASK     0x10U
#define REG_WIDE              FS_WDOG_SRS_WIDE_32b

#define Watchdog_refresh    WWDT->FEED = 0xAA;WWDT->FEED = 0x55
#define USED_WDOG           WWDT

#define ENDLESS_LOOP_ENABLE           1          /* set 1 or 0 */
#define WATCHDOG_RESETS_LIMIT         1000
#define WATCHDOG_REFRESH_RATIO        1
#define WATCHDOG_TIMEOUT_VALUE        500  /* 2ms refresh period (500 / 250kHz) */
#define WD_REF_TIMER_CLOCK_FREQUENCY  96000000 
#define WATCHDOG_CLOCK                250000    
#define WD_TEST_TOLERANCE             40         /* % */

#define WD_RUN_TEST_CONDITION        (PMC_AOREG1_POR_MASK | PMC_AOREG1_PADRESET_MASK | PMC_AOREG1_BODRESET_MASK | PMC_AOREG1_SYSTEMRESET_MASK | PMC_AOREG1_DPDRESET_WAKEUPIO_MASK | PMC_AOREG1_DPDRESET_RTC_MASK | PMC_AOREG1_DPDRESET_OSTIMER_MASK)
#define WD_CHECK_TEST_CONDITION      PMC_AOREG1_WDTRESET_MASK
/********* Watchdog END *********/


/* GPIO macros */
#define PIN_DIRECTION_IN  0
#define PIN_DIRECTION_OUT 1

#define PIN_PULL_DISABLE  0
#define PIN_PULL_DOWN     1
#define PIN_PULL_UP       2

#define LOGICAL_ONE   1
#define LOGICAL_ZERO  0

/* Dio port settings */
#define DIO_EXPECTED_VALUE 0


#define DIO_WAIT_CYCLE 75

#define DIO_BACKUP_ENABLE    1
#define DIO_BACKUP_DISABLE   0
#define DIO_BACKUP  DIO_BACKUP_ENABLE 

#define DIO_SHORT_TO_GND_TEST   1
#define DIO_SHORT_TO_VDD_TEST   0
 
/* Program Counter TEST */
#define PC_TEST_PATTERN    0x20001000/* test address for Program counter test (in RAM region) */
                           
/* UART macros */
#define USE_UART 1 /* 0 = Use LPUART */

#define APPLICATION_SERIAL_BASE USART0      
#define SERIAL_BAUD_RATE        115200U
#define SERIAL_CLOCK            12e6
#define UART_TX_PIN           49 // P1_17
#define UART_RX_PIN           48 // P1_16

/* FLASH TEST MACROS */
#define HW_FLASH_TEST         1 /* Use HW = 1 SW = 0  flash TEST*/
#define FLASH_TEST_BLOCK_SIZE 0x20
#define CRC_BASE              CRC_ENGINE_BASE

#if defined(__IAR_SYSTEMS_ICC__) /* IAR */
    #define FLASH_TEST_CONDITION_SEED 0x0000 /* 0xFFFFFFFF CRC32, 0x0000 CRC16 */ 
#endif

#if defined(__GNUC__) || (defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050)) /* MCUXpresso + KEIL */
    #define FLASH_TEST_CONDITION_SEED 0x1D0F
    /* This must be in consistence with setting in "User AFTER BUILD = srec_cat*/
    #define FLASH_TEST_START_ADDRESS  0x100
    #define FLASH_TEST_END_ADDRESS    0x300
    #define CRC_VALUE_ADDR            0xFFF0
#endif


#define RAM_TEST_BLOCK_SIZE           0x4   /* size of block for runtime testing */

#if defined(__IAR_SYSTEMS_ICC__) || (defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050)) /* IAR + KEIL */
    #define RAM_TEST_BACKUP_SIZE     0x20   /* must fit with the setup from linker configuration file */
    #define STACK_TEST_BLOCK_SIZE    0x10       /* must fit with the setup from linker configuration file */
#endif

#define STACK_TEST_PATTERN            0x77777777

#define TESTED_ADC ADC0 /*which ADC is use for AIO test*/
#define ADC_USED_FIFO_NUM 0
   
#define ADC_RESOLUTION  16
#define ADC_MAX  ((1<<(ADC_RESOLUTION))-1)
#define ADC_REFERENCE  3.3
#define ADC_EXTERNAL_PIN_LEVEL  1.65   /* depends on power supply configuration */
#define ADC_EXTERNAL_PIN_LEVEL_RAW  (((ADC_EXTERNAL_PIN_LEVEL)*(ADC_MAX))/(ADC_REFERENCE))
#define ADC_1V_LEVEL  1.0   /* depends on power supply configuration */
#define ADC_1V_LEVEL_RAW  (((ADC_1V_LEVEL)*(ADC_MAX))/(ADC_REFERENCE))
#define ADC_DEVIATION_PERCENT 20
#define ADC_MIN_LIMIT(val)  (uint32_t)(((val) * (100 - ADC_DEVIATION_PERCENT)) / 100)
#define ADC_MAX_LIMIT(val)  (uint32_t)(((val) * (100 + ADC_DEVIATION_PERCENT)) / 100)
#define FS_CFG_AIO_CHANNELS_CNT    3

#define FS_CFG_AIO_CHANNELS_LIMITS_INIT \
{\
    {ADC_MIN_LIMIT(ADC_MAX), ADC_MAX_LIMIT(ADC_MAX)}, \
    {ADC_MIN_LIMIT(ADC_1V_LEVEL_RAW), ADC_MAX_LIMIT(ADC_1V_LEVEL_RAW)},\
    {ADC_MIN_LIMIT(ADC_EXTERNAL_PIN_LEVEL_RAW), ADC_MAX_LIMIT(ADC_EXTERNAL_PIN_LEVEL_RAW)}\
}

#define FS_CFG_AIO_CHANNELS_INIT { 0, 1, 2 }  /* ADC trigger ID for respective channel (set in Adc0Setup()) */

/* NULL definition */
#ifndef NULL
    #define NULL 0
#endif

#endif /* _SAFETY_CONFIG_H_ */

