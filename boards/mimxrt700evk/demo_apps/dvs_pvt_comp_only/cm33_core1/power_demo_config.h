/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef POWER_MODE_CONFIG_H_
#define POWER_MODE_CONFIG_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*-----------------Demo configuration ----------------------*/
/* Define if PVT runs on CPU only (0) or both on CPU & DSP(1). */
#define DEMO_PVT_ON_CPU_DSP 1U

/* How often for PVT task to check if VDDCORE can go lower. */
#define PVT_TASK_WAIT_MS 10000

/* Amount of time it takes the PMIC OUT to stabilize after changing it by 1 step.*/
#define PMIC_SETTLING_TIME_MS 5U

/* Amount of time to wait in between Coremark or DSP workload runs.
 * Allows idle task to activate low power mode (deep sleep).
 */
#define WORKLOAD_DELAY_MS 5000U

#define DEMO_PRINTF_INTERRUPT_STATUS 0U /* Print in interrupt. */

#define DEMO_MAINCLK_FREQ DEMO_MAINCLK_FREQ_SP1
/*----------------------------------------------------------*/

/* Valid Frequencies: 110000000U, 192000000U, 325000000U */
#define DEMO_MAINCLK_FREQ_SP0 110000000U
#define DEMO_MAINCLK_FREQ_SP1 192000000U
#define DEMO_MAINCLK_FREQ_SP2 325000000U

/* Define voltage limits */
#if (DEMO_MAINCLK_FREQ == DEMO_MAINCLK_FREQ_SP2)
#define MAX_VDDCORE 1100000U /*1V1*/
#elif (DEMO_MAINCLK_FREQ == DEMO_MAINCLK_FREQ_SP1)
#define MAX_VDDCORE 900000U  /*0V9*/
#elif (DEMO_MAINCLK_FREQ == DEMO_MAINCLK_FREQ_SP0)
#define MAX_VDDCORE 800000U  /*0V9*/
#else
#error "Unsupported frequency point for PVTS."
#endif
#define MIN_VDDCORE 700000U           /*0V7*/

#define DEMO_PMIC_ADJUST_STEP (6250U) /* in uV. */

/* Don't change this marco. Only PMIC supply is supported. */
#define DEMO_POWER_SUPPLY_PMIC   2U
#define DEMO_POWER_SUPPLY_OPTION DEMO_POWER_SUPPLY_PMIC

/* Channel transmit and receive register */
#define APP_MU_REG kMU_MsgReg0
/* Define the event for entering DPD and FDPD. */
#define DEMO_EVENT_ENTER_DPD  3U
#define DEMO_EVENT_ENTER_FDPD 4U

#define BOOT_FLAG                     0x1U      /* Flag indicates Core1 Boot Up*/
#define DEMO_SENSE_M33_CPU_CLOCK_FREQ 32000000U /* CPU1 clock frequency. */
#define DEMO_SENSE_MIN_VDD1_VOLT      750000U   /* Minimum CPU1 deep sleep voltage. */ 

#define DEMO_POWER_CPU1_PRINT_ENABLE 1U         /* Enable CPU1 log print or not, disable the log can save power. */
#define DEMO_POWER_ENABLE_DEBUG      1U /* Enable debug or not, disable the debug function/clock can save power. */

/* Some typical delay value for TT samples.
 * !!!NOTE!!!
 * This is only used to allow runing the demo on some early typical samples which are not burned with delay values in fuse
 * and can't be used in production. The function may not work for some samples and temparautre. 
 */
#if (DEMO_MAINCLK_FREQ == DEMO_MAINCLK_FREQ_SP2)
#define DEMO_TYPICAL_DELAY (16U | (16U << 8U))
#elif (DEMO_MAINCLK_FREQ == DEMO_MAINCLK_FREQ_SP1)
#define DEMO_TYPICAL_DELAY (17U | (17U << 8U))
#elif (DEMO_MAINCLK_FREQ == DEMO_MAINCLK_FREQ_SP0)
#define DEMO_TYPICAL_DELAY (18U | (18U << 8U))
#endif
/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif /* POWER_MODE_CONFIG_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
