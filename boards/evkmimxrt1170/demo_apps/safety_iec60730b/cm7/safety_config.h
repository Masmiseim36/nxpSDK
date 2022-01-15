/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_CONFIG_H_
#define _SAFETY_CONFIG_H_

#include "MIMXRT1176_cm7.h"
#include "iec60730b.h"
#include "iec60730b_core.h"
#include "project_setup_evkmimxrt1170.h"
#include "safety_cm7_imxrt.h"

#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
//#include "fsl_rtwdog.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* This macro enables infinity while loop in SafetyErrorHandling() function */
#define SAFETY_ERROR_ACTION 0

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
#define GPT_SRC_16M 5U /* Value from ref. manual */

// Due to bud in RM/header file, RT has GPT on 16MHZ instead of 24
#define GPT_SRC_24M GPT_SRC_16M

#define CLOCK_GPT1_SRC GPT_SRC_32K
#define CLOCK_GPT1_DIV 1.F
#define CLOCK_GPT1_CMP 0xFFFFFFFFU
#define CLOCK_GPT2_SRC GPT_SRC_16M
#define CLOCK_GPT2_DIV 16.F
#define CLOCK_GPT2_CMP 0xFFFFFFFFU

#define CLOCK_GPT1_FREQ ((CLOCK_GPT1_SRC == GPT_SRC_32K ? 32768.F : 16000000.F) / CLOCK_GPT1_DIV)
#define CLOCK_GPT2_FREQ ((CLOCK_GPT2_SRC == GPT_SRC_32K ? 32768.F : 16000000.F) / CLOCK_GPT2_DIV)

#define SYSTICK_FREQ 996000000 /* 996MHz */
#define SYSTICK_RELOAD_VALUE \
    996000 /* 996MHz/996kHz = 1ms period */  // TODO: BUG in SDK/RM, core_clock.c says that Systick runs at 10kHz, but
                                             // this value is 996 MHz
#define SYSTICK_RELOAD_VALUE_MAX 0x00FFFFFFU // TODO:unused

#define ISR_FREQUENCY (SYSTICK_FREQ / SYSTICK_RELOAD_VALUE)

#define CLOCK_TEST_TOLERANCE 20U /* % */

#define GPIO_AD_19_GPIO     GPIO9
#define GPIO_AD_19_GPIO_PIN (18U)
/*********  Clock END *********/

/********* Watchdog *********/
#define WDOG_REF_TIMER_BASE GPT1
#if defined(MECC1) || defined(MECC2)
#define RESET_DETECT_REGISTER &(SRC->GPR[10])
#else
#define RESET_DETECT_REGISTER &(SRC->SRSR)
#endif
#define RESET_DETECT_MASK SRC_SRSR_WDOG3_RST_B_M7_MASK
#define REFRESH_INDEX     FS_IMXRT1170
#define CLEAR_FLAG        1
#define REG_WIDE          FS_WDOG_SRS_WIDE_32b

#define USED_WDOG        RTWDOG3
#define Watchdog_refresh USED_WDOG->CNT = RTWDOG_REFRESH_KEY

#define ENDLESS_LOOP_ENABLE          0 /* Set 1 or 0 */
#define WATCHDOG_RESETS_LIMIT        1000
#define WD_TEST_TOLERANCE            20          /* % */
#define WATCHDOG_CLOCK               32768U      /* 32.768 kHz */
#define WD_REF_TIMER_CLOCK_FREQUENCY 1000000U    /* 1 MHz*/
#define WATCHDOG_TIMEOUT_VALUE       164U        /* 2.5ms limit with 32.768kHz */
#define WATCHDOG_REFRESH_RATIO       2           /* refresh wdog every 2nd ISR */
#define WD_GPT1_SRC                  GPT_SRC_16M /* Clock source */
#define WD_GPT1_DIV                  16U         /* 16MHz/div = 1MHz */
#define WD_GPT1_CMP                  0xFFFFFFFFU

#define WD_RUN_TEST_CONDITION   (0xFFFFFFFFU ^ (SRC_SRSR_WDOG3_RST_B_M7_MASK | SRC_SRSR_WDOG3_RST_B_M4_MASK))
#define WD_CHECK_TEST_CONDITION (SRC_SRSR_WDOG3_RST_B_M7_MASK | SRC_SRSR_WDOG3_RST_B_M4_MASK)
/********* Watchdog END *********/

#define PC_TEST_PATTERN 0x0000FBFE /* Test address for Program counter test */

/********* Flashtest *********/
/* Hyper flash size */
#define BOARD_FLASH_SIZE (0x800000U)

#define FLASH_TEST_BLOCK_SIZE     0x4
#define FLASH_TEST_CONDITION_SEED 0xFFFFFFFF
#define FLASH_TEST_START_ADDRESS  0x30002430
#define FLASH_TEST_END_ADDRESS    0x300FFFFF

#if defined(__GNUC__) || defined(__ARMCC_VERSION)
/*! @note The following flash test settings must be in consistence with
          "User AFTER BUILD = srec_cat!*/
/* The CRC16 of safety-related FLASH memory. */
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
#define UART_BAUD_RATE          115200
#define SERIAL_CLOCK            80e6
/********* UART END *********/

/********* ADC *********/
#define TESTED_ADC              LPADC1
#define ADC_USED_FIFO_NUM       0
#define ADC_RESOLUTION          12
#define ADC_MAX                 ((1 << (ADC_RESOLUTION)) - 1)
#define ADC_REFERENCE           1.8
#define ADC_BANDGAP_LEVEL       1.5 /* depends on power supply configuration */
#define ADC_BANDGAP_LEVEL_RAW   (((ADC_BANDGAP_LEVEL) * (ADC_MAX)) / (ADC_REFERENCE))
#define ADC_DEVIATION_PERCENT   25
#define ADC_MIN_LIMIT(val)      (uint16_t)(((val) * (100 - ADC_DEVIATION_PERCENT)) / 100)
#define ADC_MAX_LIMIT(val)      (uint16_t)(((val) * (100 + ADC_DEVIATION_PERCENT)) / 100)
#define FS_CFG_AIO_CHANNELS_CNT 3
      
#define ADC_COMMAND_BUFFER       1
#define TRIGGER_EVENT            0  /* write to the SWTRIG register  select between 0 - 3, SWTRIG[SWT0] is associated with TCTRL0 */       
      
      
#define FS_CFG_AIO_CHANNELS_LIMITS_INIT                                                                     \
    {                                                                                                       \
        {(uint32_t)ADC_MIN_LIMIT(0), (uint32_t)ADC_MAX_LIMIT(60)},                                          \
            {(uint32_t)ADC_MIN_LIMIT(ADC_MAX), (uint32_t)ADC_MAX_LIMIT(ADC_MAX)},                           \
        {                                                                                                   \
            (uint32_t) ADC_MIN_LIMIT(ADC_BANDGAP_LEVEL_RAW), (uint32_t)ADC_MAX_LIMIT(ADC_BANDGAP_LEVEL_RAW) \
        }                                                                                                   \
    }

#define FS_CFG_AIO_CHANNELS_INIT \
    {                            \
        0x01U, 0x03U, 0x02U      \
    } /* ADC Channels for V_refl, V_refh, bandgap */

#define FS_CFG_AIO_CHANNELS_SIDE_INIT {0, 0, 0}  /* sides associated with input channels,  0 = A, 1 = B */    

/********* ADC END *********/

/* Define i.MX device (to differentiate from other devices in the vector table etc.) */
#define _IMX_ 1U

/* NULL definition */
#ifndef NULL
#define NULL 0
#endif

#endif /* _SAFETY_CONFIG_H_ */
