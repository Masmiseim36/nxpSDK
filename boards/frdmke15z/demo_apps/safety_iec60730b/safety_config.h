/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_CONFIG_H_
#define _SAFETY_CONFIG_H_

#include "freemaster_cfg.h"
#include "MKE15Z7.h"
#include "iec60730b.h" 
#include "iec60730b_core.h"
#include "safety_cm0_kinetis.h"
#include "project_setup_frdmke15z.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NULL 0 

/* This macro enables infinity while loop in SafetyErrorHandling() function */
#define SAFETY_ERROR_ACTION 1

/* TEST SWITCHES - for debugging it is better to turn the flash test and watchdog OFF */
#define ADC_TEST_ENABLED   1
#define CLOCK_TEST_ENABLED 1
#define DIO_TEST_ENABLED   1
#define FLASH_TEST_ENABLED 1
#define RAM_TEST_ENABLED   1 
#define PC_TEST_ENABLED    1
#define WATCHDOG_ENABLED   0
#define TSI_TEST_ENABLED   1
#define FMSTR_SERIAL_ENABLE 1


/*Macros for different reference timer setings*/
#define CLOCK_TEST 1
#define WDOG_TEST  2

/* Clock test*/
#define LPTMR_USED                LPTMR0
#define SYSTICK_RELOAD_VALUE          10000
#define ISR_FREQUENCY                 7200       /* Hz */
#define CLOCK_TEST_TOLERANCE          20         /* % */
#define REF_TIMER_CLOCK_FREQUENCY     4e06


/*WDOG test */
#define WD_REF_TIMER_CLOCK_FREQUENCY  REF_TIMER_CLOCK_FREQUENCY 
#define WDOG_REF_TIMER_BASE LPTMR0
#define USED_WDOG WDOG
#define RESET_DETECT_REGISTER &(RCM->SRS)
#define RESET_DETECT_MASK 0x20
#define REFRESH_INDEX FS_WDOG32
#define CLEAR_FLAG 0
#define REG_WIDE FS_WDOG_SRS_WIDE_32b

#define WD_RUN_TEST_CONDITION_SRS0  RCM_SRS_PIN_MASK | RCM_SRS_POR_MASK | RCM_SRS_SW_MASK
#define WD_RUN_TEST_CONDITION_SRS1  0 
#define WD_CHECK_TEST_CONDITION     RCM_SRS_WDOG_MASK   

#define Watchdog_refresh              WDOG->CNT = 0xB480A602;

#define ENDLESS_LOOP_ENABLE           1          /* set 1 or 0 */
#define WATCHDOG_RESETS_LIMIT         1000
#define WATCHDOG_REFRESH_RATIO        1
#define WATCHDOG_TIMEOUT_VALUE        1600  /* set the appropriate watchdog timeout value */
#define WD_TEST_TOLERANCE             30         /* % */ 
#define LPO_FREQUENCY                 128000  /* frequency of watchdog timer clock */
#define WATCHDOG_CLOCK                LPO_FREQUENCY    


/* GPIO macros */
#define PIN_DIRECTION_IN  0
#define PIN_DIRECTION_OUT 1
 
#define PIN_MUX_GPIO 1
#define PIN_MUX_UART 3

/*Define of use PCC or SIM*/
#define USE_PCC 1

#define LOGICAL_ONE   1
#define LOGICAL_ZERO  0

/*Dio port settings*/
#define DIO_EXPECTED_VALUE 0


#define DIO_WAIT_CYCLE 75

#define DIO_BACKUP_ENABLE    1
#define DIO_BACKUP_DISABLE   0
#define DIO_BACKUP  DIO_BACKUP_ENABLE 

#define DIO_SHORT_TO_GND_TEST   1
#define DIO_SHORT_TO_VDD_TEST   0
 
/* Program Counter TEST */
#define PC_TEST_PATTERN    0x20000000/* test address for Program counter test (in RAM region) */

/* UART macros */
#define UART_BAUD_RATE      9600

/*FLASH TEST MACROS*/
#define HW_FLASH_TEST 1 /*Use HW flash TEST*/

#define FLASH_TEST_BLOCK_SIZE         0x20//0x100

#define FLASH_TEST_CONDITION_SEED 0x0000

#if defined(__GNUC__) || defined(__ARMCC_VERSION)
    /*! @note The following flash test settings must be in consistence with
              "User AFTER BUILD = srec_cat!*/
    /* The CRC16 of safety-related FALSH memory. */
    #define FS_CFG_FLASH_TST_CRC        (0xFFFFU)
#endif



#define RAM_TEST_BLOCK_SIZE           0x4   /* size of block for runtime testing */

#if defined(__IAR_SYSTEMS_ICC__) || (defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050)) /* KEIL */
    #define RAM_TEST_BACKUP_SIZE     0x20   /* must fit with the setup from linker configuration file */

    #define STACK_TEST_BLOCK_SIZE    0x10       /* must fit with the setup from linker configuration file */
#endif

#define STACK_TEST_PATTERN            0x77777777

#define TESTED_ADC ADC0 /*which ADC is use for AIO test*/

#define ADC_RESOLUTION  12
#define ADC_MAX  ((1<<(ADC_RESOLUTION))-1)
#define ADC_REFERENCE  5.0
#define ADC_BANDGAP_LEVEL  1     /* depends on power supply configuration */
#define ADC_BANDGAP_LEVEL_RAW  (((ADC_BANDGAP_LEVEL)*(ADC_MAX))/(ADC_REFERENCE))
#define ADC_DEVIATION_PERCENT 20
#define ADC_MIN_LIMIT(val)  (uint16_t)(((val) * (100 - ADC_DEVIATION_PERCENT)) / 100)
#define ADC_MAX_LIMIT(val)  (uint16_t)(((val) * (100 + ADC_DEVIATION_PERCENT)) / 100)
#define FS_CFG_AIO_CHANNELS_CNT    3
#define FS_CFG_AIO_CHANNELS_LIMITS_INIT \
{\
  {(uint32_t)ADC_MIN_LIMIT(0), (uint32_t)ADC_MAX_LIMIT(60)}, \
  {(uint32_t)ADC_MIN_LIMIT(ADC_MAX), (uint32_t)ADC_MAX_LIMIT(ADC_MAX)},\
  {(uint32_t)ADC_MIN_LIMIT(ADC_BANDGAP_LEVEL_RAW), (uint32_t)ADC_MAX_LIMIT(ADC_BANDGAP_LEVEL_RAW)}\
}

#define FS_CFG_AIO_CHANNELS_INIT {30, 29, 27}  /* ADC Channels for V_refh, V_refl, bandgap */


#ifdef _MKE15Z7_H_ 
   

#define TSI_CNT_MAX                        (65535U)
#define TSI_DEVIATION_PERCENT              (50) /* Used for TSI typ. signal level comparison */
#define TSI_MIN_DELTA_PERCENT              (50) /* Used for Delta comparison for TSI input stimulated test */
#define TSI_CNT_LOW_LIMIT(val)   (((val) * (100 - TSI_DEVIATION_PERCENT)) / 100)
#define TSI_CNT_HIGH_LIMIT(val)  (((val) * (100 + TSI_DEVIATION_PERCENT)) / 100)
#define TSI_DELTA(val, sval)     (((sval)-(val)))
#define TSI_COUNTER_LIMIT        10000 /* defined by user */
   
/* Used TSI channel  in SAFETY test */
#define TSI0_CH0 0
#define TSI0_CH1 1
#define TSI0_CH2 2 
#define TSI0_CH3 3
#define TSI0_CH5 5
#define TSI0_CH6 6
#define TSI0_CH7 7 
#define TSI0_CH8 8
#define TSI0_CH9 9
#define TSI0_CH10 10

/*Macro for  CLOCK init, this port are used by TSI and clock must be enable */
#define TSI_PORT_USE {PORTB, PORTD, PORTE, 0 } // 0 = NULL it must be there as a termination

/* TSI channel limits for the Normal and Stimulated state (by internal pull device) */
#define TSI_VAL0   (5128U) /* Normal state */
#define TSI_SVAL0  (5632U) /* Stimulated state */

#define TSI_VAL1   (4736U) 
#define TSI_SVAL1  (5184U)

#define TSI_VAL2   (4844U)
#define TSI_SVAL2  (5292U)

#define TSI_VAL3   (4357U) 
#define TSI_SVAL3  (4800U)

#define TSI_VAL4   (4233U) 
#define TSI_SVAL4  (4660U)

#define TSI_VAL5   (4672U) 
#define TSI_SVAL5  (5120U)
/*------------------------- TSI mutual section -------------------------------*/

/* TSI channel limits for the Normal and Stimulated state (by internal pull device) */
#define TSI_M_VAL0   (5472U) /* Normal state */
#define TSI_M_SVAL0  (6553U) /* Stimulated state */

#define TSI_M_VAL1   (5500U)//(3692U) 
#define TSI_M_SVAL1  (5800U)//(3804U)

#define TSI_M_VAL2   (6500U)//(4624U)
#define TSI_M_SVAL2  (6900U)//(5488U)

#define TSI_M_VAL3   (5600U)//(4393U) 
#define TSI_M_SVAL3  (5900U)//(5136U)

#endif


#endif /* _SAFETY_CONFIG_H_ */
