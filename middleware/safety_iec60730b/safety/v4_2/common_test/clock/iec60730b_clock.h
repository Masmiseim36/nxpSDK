/*
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2015-2021 NXP.
 *
 * License: LA_OPT_NXP_Software_License
 *
 * This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms. If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.
 *
 * @version 4.2
 *
 * @brief Clock test common routines - IEC60730 Class B.
 */

#ifndef IEC60730B_CLOCK_H_
#define IEC60730B_CLOCK_H_

#include "iec60730b_types.h"

/*
 * List of devices and supported functions can be found in library user guide,
 * section Core self test library - source code version.
 */

/******************************************************************************
 * Macros
 ******************************************************************************/
/* Return values */
#define FS_FAIL_CLK     (FS_CLK_CODE + 0x1U)
#define FS_CLK_PROGRESS (FS_CLK_CODE + 0x2U)

/* Assembler skips following section */
#ifndef __ASM__

/*******************************************************************************
 *
 * @brief    Function checks if the testContext value is within the defined limits.
 *
 * @param    testContext - captured value of timer.
 * @param    limitLow    - low limit.
 * @param    limitHigh   - high limit.
 *
 * @return   FS_PASS         - testContext fit into limits.
 *           FS_FAIL_CLK     - testContext value doesn't fit into limits.
 *           FS_CLK_PROGRESS - reference counter value not read yet.
 *
 *
 ******************************************************************************/
FS_RESULT FS_CLK_Check(uint32_t testContext, uint32_t limitLow, uint32_t limitHigh);

/*******************************************************************************
 *
 * @brief    Function initializes the clock test.
 *
 * @param    *pTestContext - pointer to the variable which holds
 *                           captured timer value.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_CLK_Init(uint32_t *pTestContext);

/*******************************************************************************
 *
 * @brief    Function records the LPTMR counter value and start the counter again.
 *
 * @param    *pSafetyTmr   - timer module  address.
 * @param    *pTestContext - pointer to the variable which holds
 *                           captured timer value.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_CLK_LPTMR(fs_lptmr_t *pSafetyTmr, uint32_t *pTestContext);

/*******************************************************************************
 *
 * @brief    Function records the RTC counter value and start the counter again.
 *
 * @param    *pSafetyTmr   - timer module address.
 * @param    *pTestContext - pointer to the variable which holds
 *                           captured timer value.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_CLK_RTC(fs_rtc_t *pSafetyTmr, uint32_t *pTestContext);

/*******************************************************************************
 *
 * @brief    Function records the GPT counter value and start the counter again.
 *
 * @param    *pSafetyTmr   - timer module address.
 * @param    *pTestContext - pointer to the variable which holds
 *                           captured timer value.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_CLK_GPT(fs_gpt_t *pSafetyTmr, uint32_t *pTestContext);

/*******************************************************************************
 *
 * @brief    Function records the WKT counter value and start the counter again.
 *
 * @param    *pSafetyTmr   - timer module address.
 * @param    *pTestContext - pointer to the variable which holds
 *                           captured timer value.
 * @param    startValue    - start value for decreasing of WKT counter.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_CLK_WKT_LPC(fs_wkt_t *pSafetyTmr, uint32_t *pTestContext, uint32_t startValue);

/*******************************************************************************
 *
 * @brief    Function records the CTIMER counter value and start the counter again.
 *
 * @param    *pSafetyTmr   - timer module address.
 * @param    *pTestContext - pointer to the variable which holds
 *                           captured timer value.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_CLK_CTIMER_LPC(fs_ctimer_t *pSafetyTmr, uint32_t *pTestContext);

#endif /* __ASM__ */
#endif /* IEC60730B_CLOCK_H_ */
