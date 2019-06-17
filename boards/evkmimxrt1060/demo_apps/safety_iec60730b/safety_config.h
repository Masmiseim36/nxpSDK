/*******************************************************************************
*
* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
* 
*
*******************************************************************************/

#ifndef _SAFETY_CONFIG_H_
#define _SAFETY_CONFIG_H_
   
#include "MIMXRT1062.h"   

#include "project_setup_evkmimxrt1060.h"
#include "safety_cm4_cm7_imxrt.h"
#include "iec60730b.h"

#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if(1)
#define SAFETY_ERROR_ACTION
#endif

#if(1)
#define WATCHDOG_ON   /* Should be OFF in debug configuration (RAM) */     
#endif

#if(0)
#define FLASH_TEST_ON /* Should be OFF in debug configuration (RAM) */
#endif

#if(1)
#define PC_TEST_ON    /* Should be OFF in debug configuration (RAM) */
#endif
   
/*********  Clock *********/
#define GPT_SRC_32K 4U /* Value from ref. manual */
#define GPT_SRC_24M 5U /* Value from ref. manual */

#define CLOCK_GPT1_SRC  GPT_SRC_24M
#define CLOCK_GPT1_DIV  12.F
#define CLOCK_GPT1_CMP  0xFFFFFFFFU
#define CLOCK_GPT2_SRC  GPT_SRC_24M
#define CLOCK_GPT2_DIV  24.F
#define CLOCK_GPT2_CMP  0xFFFFFFFFU
   
#define CLOCK_GPT1_FREQ ((CLOCK_GPT1_SRC == GPT_SRC_32K ? 32768.F : 24000000.F)/CLOCK_GPT1_DIV)
#define CLOCK_GPT2_FREQ ((CLOCK_GPT2_SRC == GPT_SRC_32K ? 32768.F : 24000000.F)/CLOCK_GPT2_DIV)

#define SYSTICK_FREQ          600000000U /* 600MHz */
#define SYSTICK_RELOAD_VALUE  600000U    /* 600MHz/600kHz = 1ms period */ 
#define SYSTICK_RELOAD_VALUE_MAX 0x00FFFFFFU

#define ISR_FREQUENCY (SYSTICK_FREQ/SYSTICK_RELOAD_VALUE)  
   
#define CLOCK_TEST_TOLERANCE  20U /* % */
/*********  Clock END *********/

/********* Watchdog *********/
#define Watchdog_refresh    RTWDOG->CNT = 0xB480A602

#define ENDLESS_LOOP_ENABLE    1      /* Set 1 or 0 */
#define WATCHDOG_RESETS_LIMIT  1000
#define WD_TEST_TOLERANCE      20     /* % */
#define WATCHDOG_CLOCK               32768U   /* 32.768 kHz */
#define WD_REF_TIMER_CLOCK_FREQUENCY 1000000U /* 1 MHz*/
#define WATCHDOG_TIMEOUT_VALUE 82U   /* 2.5ms limit with 32.768kHz */
#define WATCHDOG_REFRESH_RATIO 2     /* refresh wdog every 2nd ISR */
#define WD_GPT1_SRC  GPT_SRC_24M /* Clock source */
#define WD_GPT1_DIV  24U /* 24MHz/div = 1MHz */
#define WD_GPT1_CMP  0xFFFFFFFFU

#define WD_RUN_TEST_CONDITION   (SRC_SRSR_IPP_RESET_B_MASK | SRC_SRSR_LOCKUP_SYSRESETREQ_MASK | SRC_SRSR_IPP_USER_RESET_B_MASK | SRC_SRSR_JTAG_SW_RST_MASK)
#define WD_CHECK_TEST_CONDITION SRC_SRSR_WDOG3_RST_B_MASK
/********* Watchdog END *********/

#define PC_TEST_PATTERN 0x81FFDBEC /* Test address for Program counter test (SDRAM region) */
   
/********* Flashtest *********/
#define FLASH_TEST_BLOCK_SIZE     0x10
#define FLASH_TEST_CONDITION_SEED 0xFFFFFFFF
#define FLASH_TEST_START_ADDRESS  0x60002410
#define FLASH_TEST_END_ADDRESS    0x60005000

#if defined(__GNUC__) || defined(__CC_ARM)
	#define CRC_VALUE_ADDR   0x60002400 /* must fit with the setup from linker configuration file */
#endif
/********* Flashtest END *********/

/********* RAM *********/
#define RAM_TEST_BLOCK_SIZE       0x4        /* size of block for runtime testing */

#if defined(__IAR_SYSTEMS_ICC__) || defined(__CC_ARM)
    #define RAM_TEST_BACKUP_SIZE      0x20       /* must fit with the setup from linker configuration file */
    #define STACK_TEST_BLOCK_SIZE     0x10       /* must fit with the setup from linker configuration file */
#endif
/********* RAM END *********/

#define STACK_TEST_PATTERN        0x77777777

/********* DIO *********/
#define PIN_DIRECTION_IN  0
#define PIN_DIRECTION_OUT 1
   
#define DIO_EXPECTED_VALUE 0

#define DIO_WAIT_CYCLE 3500U

#define DIO_BACKUP_ENABLE    1
#define DIO_BACKUP_DISABLE   0
#define DIO_BACKUP  DIO_BACKUP_ENABLE 

#define DIO_SHORT_TO_GND_TEST   1
#define DIO_SHORT_TO_VDD_TEST   0

#define LOGICAL_ONE   1
#define LOGICAL_ZERO  0
/********* DIO END *********/
   
/********* UART *********/
#define UART_TYPE   0
#define LPUART_TYPE 1
#define APPLICATION_UART      LPUART1     /* Address from device header file */
#define APPLICATION_UART_TYPE LPUART_TYPE /* Chosen type */

#define UART_BAUD_RATE      19200
#define UART_CLOCK_INPUT    80e6
/********* UART END *********/

/* Hyper flash size */
#define BOARD_FLASH_SIZE    (0x800000U)

/* Time range in which safety_ram_crc should be tested */
#define RAM_CHECK_TIME_THRESHOLD 100 /* 1ms Systick period * 100 = 100ms threshold */

#endif /* _SAFETY_CONFIG_H_ */
