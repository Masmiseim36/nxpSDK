/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_CONFIG_H_
#define _SAFETY_CONFIG_H_
   
#include "MIMX8MN6_cm7.h"   
#include "iec60730b.h" 
#include "iec60730b_core.h"
#include "project_setup_evkmimx8mn.h"
#include "safety_cm4_cm7_imx8m.h"

#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_iomuxc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* This macro enables infinity while loop in SafetyErrorHandling() function */
#define SAFETY_ERROR_ACTION 1

/* TEST SWITCHES - for debugging it is better to turn the flash test and watchdog OFF */
#define CLOCK_TEST_ENABLED 1
#define DIO_TEST_ENABLED   1
#define FLASH_TEST_ENABLED 1
#define RAM_TEST_ENABLED   1 
#define PC_TEST_ENABLED    1
#define WATCHDOG_ENABLED   1
#define FMSTR_SERIAL_ENABLE 1

/********* Clock *********/
#define GPT_SRC_32K 4U /* Value from ref. manual */
#define GPT_SRC_24M 5U /* Value from ref. manual */

#define CLOCK_GPT1_SRC  GPT_SRC_32K
#define CLOCK_GPT1_DIV  1.F
#define CLOCK_GPT1_CMP  0xFFFFFFFFU
   
#define CLOCK_GPT1_FREQ ((CLOCK_GPT1_SRC == GPT_SRC_32K ? 32768.F : 24000000.F)/CLOCK_GPT1_DIV)

#define SYSTICK_FREQ          600000000U /* 600MHz */
#define SYSTICK_RELOAD_VALUE  600000U    /* 600MHz/600kHz = 1ms period */ 
#define SYSTICK_RELOAD_VALUE_MAX 0x00FFFFFFU

#define ISR_FREQUENCY (SYSTICK_FREQ/SYSTICK_RELOAD_VALUE)  
   
#define CLOCK_TEST_TOLERANCE  20U /* % */
/*********  Clock END *********/

/********* Watchdog *********/
#define WDOG_REF_TIMER_BASE   GPT1
#define RESET_DETECT_REGISTER &(SRC->SRSR)
#define RESET_DETECT_MASK     SRC_SRSR_wdog3_rst_b_MASK
#define REFRESH_INDEX         FS_IMX8M
#define CLEAR_FLAG            1
#define REG_WIDE              FS_WDOG_SRS_WIDE_32b

#define USED_WDOG           WDOG3
#define Watchdog_refresh    USED_WDOG->WSR = 0x5555;USED_WDOG->WSR = 0xAAAA

#define ENDLESS_LOOP_ENABLE    1      /* Set 1 or 0 */
#define WATCHDOG_RESETS_LIMIT  1000
#define WD_TEST_TOLERANCE      20     /* % */
#define WATCHDOG_CLOCK               32768U   /* 32.768 kHz */
#define WD_REF_TIMER_CLOCK_FREQUENCY 1000000U /* 1 MHz*/
#define WATCHDOG_TIMEOUT_VALUE 16384U   /* 0.5s limit with 32.768kHz */
#define WATCHDOG_REFRESH_RATIO 450      /* refresh wdog every 450th ISR */
#define WD_GPT1_SRC  GPT_SRC_24M /* Clock source */
#define WD_GPT1_DIV  24U /* 24MHz/div = 1MHz */
#define WD_GPT1_CMP  0xFFFFFFFFU

#define WD_RUN_TEST_CONDITION   (SRC_SRSR_ipp_reset_b_MASK | SRC_SRSR_csu_reset_b_MASK | SRC_SRSR_ipp_user_reset_b_MASK | SRC_SRSR_jtag_sw_rst_MASK)
#define WD_CHECK_TEST_CONDITION  SRC_SRSR_wdog3_rst_b_MASK
/********* Watchdog END *********/

#define PC_TEST_PATTERN 0x2001F5E0 /* Test address for Program counter test */
   
/********* Flashtest *********/
#define FLASH_TEST_BLOCK_SIZE     0x4
#define FLASH_TEST_CONDITION_SEED 0xFFFFFFFF
#define FLASH_TEST_START_ADDRESS  0x08000A30
#define FLASH_TEST_END_ADDRESS    0x08001003

#if defined(__GNUC__) || defined(__ARMCC_VERSION)
	#define CRC_VALUE_ADDR   0x08000A00 /* must fit with the setup from linker configuration file */
#endif
/********* Flashtest END *********/

/********* RAM *********/
#define RAM_TEST_BLOCK_SIZE       0x4        /* size of block for runtime testing */

#if defined(__IAR_SYSTEMS_ICC__) || defined(__ARMCC_VERSION)
    #define RAM_TEST_BACKUP_SIZE      0x20       /* must fit with the setup from linker configuration file */
    #define STACK_TEST_BLOCK_SIZE     0x10       /* must fit with the setup from linker configuration file */
#endif
/********* RAM END *********/

#define STACK_TEST_PATTERN        0x77777777

/********* DIO *********/
#define PIN_DIRECTION_IN  0
#define PIN_DIRECTION_OUT 1
   
#define DIO_EXPECTED_VALUE 0

#define DIO_WAIT_CYCLE 2000U

#define DIO_BACKUP_ENABLE    1
#define DIO_BACKUP_DISABLE   0
#define DIO_BACKUP  DIO_BACKUP_ENABLE 

#define DIO_SHORT_TO_GND_TEST   1
#define DIO_SHORT_TO_VDD_TEST   0

#define LOGICAL_ONE   1
#define LOGICAL_ZERO  0
/********* DIO END *********/
   
/********* UART *********/
#define APPLICATION_SERIAL_BASE LPUART1      
#define UART_BAUD_RATE      9600
#define SERIAL_CLOCK      80e6
/********* UART END *********/

/* Define i.MX device (to differentiate from other devices in the vector table etc.) */
#define _IMX_ 1U

#endif /* _SAFETY_CONFIG_H_ */
