/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_CONFIG_H_
#define _SAFETY_CONFIG_H_

#include "MIMXRT1052.h"
#include "iec60730b.h"
#include "iec60730b_core.h"
#include "safety_test_items.h"
#include "project_setup_evkbimxrt1050.h"
#include "safety_cm7_imxrt.h"

#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#ifndef NULL
#ifdef __cplusplus
#define NULL (0)
#else
#define NULL ((void *)0)
#endif
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* This macro enables infinity while loop in SafetyErrorHandling() function */
#define SAFETY_ERROR_ACTION 1

/* TEST SWITCHES - for debugging it is better to turn the flash test and watchdog OFF */
#define ADC_TEST_ENABLED    0
#define CLOCK_TEST_ENABLED  1
#define DIO_TEST_ENABLED    1
#define FLASH_TEST_ENABLED  1
#define RAM_TEST_ENABLED    1
#define PC_TEST_ENABLED     1
#define WATCHDOG_ENABLED    0
#define FMSTR_SERIAL_ENABLE 1

/********* Clock *********/
#define GPT_SRC_32K 4U /* Value from ref. manual */
#define GPT_SRC_24M 5U /* Value from ref. manual */

#define CLOCK_GPT1_SRC GPT_SRC_24M
#define CLOCK_GPT1_DIV 12.F
#define CLOCK_GPT1_CMP 0xFFFFFFFFU
#define CLOCK_GPT2_SRC GPT_SRC_24M
#define CLOCK_GPT2_DIV 24.F
#define CLOCK_GPT2_CMP 0xFFFFFFFFU

#define CLOCK_GPT1_FREQ ((CLOCK_GPT1_SRC == GPT_SRC_32K ? 32768.F : 24000000.F) / CLOCK_GPT1_DIV)
#define CLOCK_GPT2_FREQ ((CLOCK_GPT2_SRC == GPT_SRC_32K ? 32768.F : 24000000.F) / CLOCK_GPT2_DIV)

#define SYSTICK_FREQ             600000000U /* 600MHz */
#define SYSTICK_RELOAD_VALUE     600000U    /* 600MHz/600kHz = 1ms period */
#define SYSTICK_RELOAD_VALUE_MAX 0x00FFFFFFU

#define ISR_FREQUENCY (SYSTICK_FREQ / SYSTICK_RELOAD_VALUE)

#define CLOCK_TEST_TOLERANCE 20U /* % */
/*********  Clock END *********/

/********* Watchdog *********/
#define WDOG_REF_TIMER_BASE   GPT1
#define RESET_DETECT_REGISTER &(SRC->SRSR)
#define RESET_DETECT_MASK     SRC_SRSR_WDOG3_RST_B_MASK
#define REFRESH_INDEX         FS_IMXRT
#define CLEAR_FLAG            1
#define REG_WIDE              FS_WDOG_SRS_WIDE_32b

#define Watchdog_refresh RTWDOG->CNT = RTWDOG_REFRESH_KEY
#define USED_WDOG        RTWDOG

#define ENDLESS_LOOP_ENABLE          1 /* Set 1 or 0 */
#define WATCHDOG_RESETS_LIMIT        1000
#define WD_TEST_TOLERANCE            20          /* % */
#define WATCHDOG_CLOCK               32768U      /* 32.768 kHz */
#define WD_REF_TIMER_CLOCK_FREQUENCY 1000000U    /* 1 MHz*/
#define WATCHDOG_TIMEOUT_VALUE       82U         /* 2.5ms limit with 32.768kHz */
#define WATCHDOG_REFRESH_RATIO       2           /* refresh wdog every 2nd ISR */
#define WD_GPT1_SRC                  GPT_SRC_24M /* Clock source */
#define WD_GPT1_DIV                  24U         /* 24MHz/div = 1MHz */
#define WD_GPT1_CMP                  0xFFFFFFFFU

#define WD_RUN_TEST_CONDITION                                                                        \
    (SRC_SRSR_IPP_RESET_B_MASK | SRC_SRSR_LOCKUP_SYSRESETREQ_MASK | SRC_SRSR_IPP_USER_RESET_B_MASK | \
     SRC_SRSR_JTAG_SW_RST_MASK)
#define WD_CHECK_TEST_CONDITION SRC_SRSR_WDOG3_RST_B_MASK
/********* Watchdog END *********/

#define PC_TEST_PATTERN 0x0000FBFE /* Test address for Program counter test */

/********* Flashtest *********/
/* Hyper flash size */
#define BOARD_FLASH_SIZE (0x4000000U)

#define FLASH_USED_DCP DCP
#define FLASH_DCP_TAG  0U

#define FLASH_TEST_BLOCK_SIZE     0x10
#define FLASH_TEST_CONDITION_SEED 0xFFFFFFFF

#if defined(__GNUC__) || defined(__ARMCC_VERSION)
/*! @note The following flash test settings must be in consistence with
          "User AFTER BUILD = srec_cat!*/
/* The CRC32 of safety-related FLASH memory. */
#define FS_CFG_FLASH_TST_CRC (0xFFFFFFFFUL)
#endif
/********* Flashtest END *********/

/********* RAM *********/
#define RAM_TEST_BLOCK_SIZE 0x4 /* size of block for runtime testing */

#if defined(__IAR_SYSTEMS_ICC__) || (defined(__GNUC__) && (__ARMCC_VERSION >= 6010050)) /* IAR + KEIL */
#define RAM_TEST_BACKUP_SIZE  0x20 /* must fit with the setup from linker configuration file */
#define STACK_TEST_BLOCK_SIZE 0x10 /* must fit with the setup from linker configuration file */
#endif

/* Time range in which safety_ram_crc should be tested */
#define RAM_CRC_CHECK_TIME_THRESHOLD 100 /* 1ms Systick period * 100 = 100ms threshold */
/********* RAM END *********/

#define STACK_TEST_PATTERN 0x77777777

/********* DIO *********/
#define PIN_DIRECTION_IN  0
#define PIN_DIRECTION_OUT 1

#define DIO_EXPECTED_VALUE 0

#define DIO_WAIT_CYCLE 3500U

#define DIO_BACKUP_ENABLE  1
#define DIO_BACKUP_DISABLE 0
#define DIO_BACKUP         DIO_BACKUP_ENABLE

#define DIO_SHORT_TO_GND_TEST 1
#define DIO_SHORT_TO_VDD_TEST 0

#define LOGICAL_ONE  1
#define LOGICAL_ZERO 0
/********* DIO END *********/

/********* UART *********/
#define APPLICATION_SERIAL_BASE LPUART1
#define UART_BAUD_RATE          19200
#define SERIAL_CLOCK            80e6
/********* UART END *********/

/********* ADC *********/
#define TESTED_ADC              ADC1 /* which ADC is use for AIO test */
#define ADC_RESOLUTION          12
#define ADC_REFERENCE           3.3
#define ADC_BANDGAP_LEVEL       1.6 /* depends on power supply configuration */
#define ADC_DEVIATION_PERCENT   25

/********* ADC END *********/

/* Define i.MX device (to differentiate from other devices in the vector table etc.) */
#define _IMX_ 1U

#endif /* _SAFETY_CONFIG_H_ */
