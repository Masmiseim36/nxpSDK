/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_CFG_H_
#define _SAFETY_CFG_H_


MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

#include "fsl_device_registers.h"

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* This macro enables infinity while loop in FS_fsErrorHandling() function.  */
#define FS_CFG_SAFETY_ERROR_ACTION      (1U)

/*                             TEST SWITCHES                                  */
/*! @note It is recommended to disable flash test during debugging as the SW
          breakpoint corrupt the CRC calculation. */
/*! @note It is recommended to disable clock test during debugging as LPTMR
          counter is still running. */
#define FS_CFG_ENABLE_TEST_CLOCK        (1U)
#define FS_CFG_ENABLE_TEST_FLASH        (0U)
#define FS_CFG_ENABLE_TEST_PC           (1U)
#define FS_CFG_ENABLE_TEST_ISR          (1U)
#define FS_CFG_ENABLE_TEST_FLOW         (1U)
#define FS_CFG_ENABLE_WATCHDOG          (1U)

/*                              WATCHDOG TEST                                 */
/* The appropriate watchdog counter timeout value. */
#define FS_CFG_WDOG_TOUT_VALUE          (30UL)
/* The watchdog timeout reset delay in us. */
#define FS_CFG_WDOG_TOUT_RST_DLY        (0UL)
/* Tolerated deviance from expected backup counter value in %. */
#define FS_CFG_WDOG_TEST_TOLERANCE      (20UL)
/* Enable/disable internal endless loop upon watchdog test error detection. */
#define FS_CFG_WDOG_ENDLESS_LOOP_EN     (1U)
/* Limit of number of watchdog resets to evaluate the error. */
#define FS_CFG_WDOG_RESETS_LIMIT        (10U)
/* Fast control loop watchdog refresh ratio (number of fast control loops per
   one watchdog refresh). */
/*! @note Number of uninterruptible safety tests affect the resolution. */
#define FS_CFG_WDOG_REFRESH_RATIO       (50U)

/* WDOG refresh macro. */
#if (FS_CFG_ENABLE_WATCHDOG == 1U)
    #define FS_WDOG_REFRESH()           WDOG->REFRESH = 0xA602U;\
                                        WDOG->REFRESH = 0xB480U
#else
    #define FS_WDOG_REFRESH()           __asm("nop")
#endif

/*                               CLOCK TEST                                   */
/* Tolerated deviance to expected backup counter value in %. */
#define FS_CFG_CLOCK_TST_TOLERANCE      (20U)


/*                                FLASH TEST                                  */
/* Size of tested flash block. */
#define FS_CFG_FLASH_TST_BLOCK_SIZE     (0x20U)

#if defined(__GNUC__) || defined(__ARMCC_VERSION)
    /*! @note The following flash test settings must be in consistence with
              "User AFTER BUILD = srec_cat!*/
    /* The CRC16 of safety-related FALSH memory. */
    #define FS_CFG_FLASH_TST_CRC        (0xFFFFUL)
#endif

/*                               RAM TEST                                     */
/* Size of back-up RAM for safety RAM testing. */
#if defined(__ARMCC_VERSION)
    /*! @note Must match the setup from linker configuration file! */
    #define FS_CFG_RAM_BACKUP_SIZE      (0x20U)
#endif
/* Size of block for run-time safety RAM testing. */
#define FS_CFG_RAM_TEST_BLOCK_SIZE      (0x04U)


/*                              STACK TEST                                    */
/* Test pattern to be written below and above tested stack area. */
#define FS_CFG_STACK_TEST_PATTERN       (0xA5A5AA55UL)
/* The stack test region block size. */
#if defined(__ARMCC_VERSION)
    /*! @note Must match the setup from linker configuration file! */
    #define FS_CFG_STACK_BLOCK_SIZE     (0x10U)
#endif


/*                          PROGRAM COUNTER TEST                              */
/* Test RAM address for program counter (PC) test. */
#define FS_CFG_PC_TEST_PATTERN          (0x1FFFFBEEU)


/*                        INTERRUPT HANDLING TEST                             */
/* The percentage tolerance of fast-to-slow ISR execution rate. */
#define FS_CFG_ISR_FL_TO_SL_RATE_TOLERANCE  (20U)

/* The maximal number of slow-control loop ISR executions per one background
   execution. */
#define FS_CFG_ISR_SL_PER_BL_EXECS_MAX      (10U)


#endif /* _SAFETY_CFG_H */
