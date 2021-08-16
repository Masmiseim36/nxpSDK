/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_CONFIG_H_
#define _SAFETY_CONFIG_H_

#include "MK22F51212.h"
#include "iec60730b.h"
#include "iec60730b_core.h"
#include "project_setup_frdmk22f.h"
#include "safety_cm4_cm7_kinetis.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* This macro enables infinity while loop in SafetyErrorHandling() function */
#define SAFETY_ERROR_ACTION 1

/* TEST SWITCHES - for debugging it is better to turn the flash test and watchdog OFF */
#define ADC_TEST_ENABLED    1
#define CLOCK_TEST_ENABLED  1
#define DIO_TEST_ENABLED    1
#define FLASH_TEST_ENABLED  1
#define RAM_TEST_ENABLED    1
#define PC_TEST_ENABLED     1
#define WATCHDOG_ENABLED    1
#define FMSTR_SERIAL_ENABLE 1

/* CLock Test */
#define LPTMR_USED                LPTMR0
#define SYSTICK_RELOAD_VALUE      234374
#define ISR_FREQUENCY             320 /* Hz */
#define CLOCK_TEST_TOLERANCE      20  /* % */
#define LPTMR_CLOCK_FREQUENCY     32768
#define REF_TIMER_CLOCK_FREQUENCY LPTMR_CLOCK_FREQUENCY

/*WDOG test */
#define WDOG_REF_TIMER_BASE   LPTMR0
#define USED_WDOG             WDOG
#define RESET_DETECT_REGISTER &(RCM->SRS0)
#define RESET_DETECT_MASK     0x20
#define CLEAR_FLAG            0
#define REFRESH_INDEX         FS_KINETIS_WDOG
#define REG_WIDE              FS_WDOG_SRS_WIDE_8b

#define WD_RUN_TEST_CONDITION_SRS0 RCM_SRS0_PIN_MASK | RCM_SRS0_POR_MASK | RCM_SRS0_WAKEUP_MASK
#define WD_RUN_TEST_CONDITION_SRS1 RCM_SRS1_SW_MASK
#define WD_CHECK_TEST_CONDITION    RCM_SRS0_WDOG_MASK

#define Watchdog_refresh    \
    WDOG->REFRESH = 0xA602; \
    WDOG->REFRESH = 0xB480

#define WD_REF_TIMER_CLOCK_FREQUENCY REF_TIMER_CLOCK_FREQUENCY
#define ENDLESS_LOOP_ENABLE          1 /* set 1 or 0 */
#define WATCHDOG_RESETS_LIMIT        1000
#define WATCHDOG_REFRESH_RATIO       1
#define WATCHDOG_TIMEOUT_VALUE       100U /* set the appropriate watchdog timeout value */
#define LPO_FREQUENCY                1000 /* frequency of watchdog timer clock */
#define WATCHDOG_CLOCK               LPO_FREQUENCY
#define WD_TEST_TOLERANCE            20 /* % */

#define USE_CRC16                 0
#define FLASH_TEST_BLOCK_SIZE     0x10
#define FLASH_TEST_CONDITION_SEED 0x0000

#if defined(__GNUC__) || defined(__ARMCC_VERSION)
/*! @note The following flash test settings must be in consistence with
          "User AFTER BUILD = srec_cat!*/
/* The CRC16 of safety-related FALSH memory. */
#define FS_CFG_FLASH_TST_CRC (0xFFFFU)
#endif

#define RAM_TEST_BLOCK_SIZE 0x4 /* size of block for runtime testing */

#if defined(__IAR_SYSTEMS_ICC__) || (defined(__GNUC__) && (__ARMCC_VERSION >= 6010050)) /* KEIL */
#define RAM_TEST_BACKUP_SIZE 0x20 /* must fit with the setup from linker configuration file */

#define STACK_TEST_BLOCK_SIZE 0x10 /* must fit with the setup from linker configuration file */
#endif

#define STACK_TEST_PATTERN 0x77777777

#define TESTED_ADC ADC0 /*which ADC is use for AIO test*/

#define ADC_RESOLUTION          12
#define ADC_MAX                 ((1 << (ADC_RESOLUTION)) - 1)
#define ADC_REFERENCE           5.00
#define ADC_BANDGAP_LEVEL       1.5 /* depends on power supply configuration */
#define ADC_BANDGAP_LEVEL_RAW   (((ADC_BANDGAP_LEVEL) * (ADC_MAX)) / (ADC_REFERENCE))
#define ADC_DEVIATION_PERCENT   20
#define ADC_MIN_LIMIT(val)      (((val) * (100 - ADC_DEVIATION_PERCENT)) / 100)
#define ADC_MAX_LIMIT(val)      (((val) * (100 + ADC_DEVIATION_PERCENT)) / 100)
#define FS_CFG_AIO_CHANNELS_CNT 3
#define FS_CFG_AIO_CHANNELS_LIMITS_INIT                                                                     \
    {                                                                                                       \
        {(uint32_t)ADC_MIN_LIMIT(0), (uint32_t)ADC_MAX_LIMIT(10)},                                          \
            {(uint32_t)ADC_MIN_LIMIT(ADC_MAX), (uint32_t)ADC_MAX_LIMIT(ADC_MAX)},                           \
        {                                                                                                   \
            (uint32_t) ADC_MIN_LIMIT(ADC_BANDGAP_LEVEL_RAW), (uint32_t)ADC_MAX_LIMIT(ADC_BANDGAP_LEVEL_RAW) \
        }                                                                                                   \
    }
#define FS_CFG_AIO_CHANNELS_INIT \
    {                            \
        30, 29, 27               \
    } /* ADC Channels for V_refh, V_refl, bandgap */
#define FS_CFG_AIO_SAMPLES_INIT \
    {                           \
        2                       \
    } /* ADC SAMPLES number for test  KV46 */

#define ADC_COMMAND_BUFFER 2 /* must be from 1 to 15, HAS NO EFFECT HERE */

/*Program Counter TEST*/
#define PC_TEST_PATTERN 0x20000000 /* test address for Program counter test (in RAM region) */

/*GPIO macros*/
#define PIN_DIRECTION_IN  0
#define PIN_DIRECTION_OUT 1

#define PIN_MUX_GPIO 0x1U
#define PIN_MUX_UART 0x2U

#define LOGICAL_ONE  1
#define LOGICAL_ZERO 0

/*Dio port settings*/
#define DIO_EXPECTED_VALUE 0

#define DIO_WAIT_CYCLE 100

#define DIO_BACKUP_ENABLE  1
#define DIO_BACKUP_DISABLE 0
#define DIO_BACKUP         DIO_BACKUP_ENABLE

#define DIO_SHORT_TO_GND_TEST 1
#define DIO_SHORT_TO_VDD_TEST 0

/*IS USE PCC or SIM*/
#define USE_SIM 1 /*1 = PCC, 0 = SIM*/

/* UART macros */
#define UART_BAUD_RATE 9600

/* unused */
#define CLOCK_GPT1_FREQ 1
#define CLOCK_GPT2_FREQ 1
#define CLOCK_GPT1_SRC  1
#define CLOCK_GPT1_CMP  1
#define CLOCK_GPT1_DIV  1
#define CLOCK_GPT2_SRC  1
#define CLOCK_GPT2_CMP  1
#define CLOCK_GPT2_DIV  1

#endif /* _SAFETY_CONFIG_H_ */
