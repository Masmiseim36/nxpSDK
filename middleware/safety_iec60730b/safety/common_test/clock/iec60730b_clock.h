/************************************************************************************************
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2015-2019 NXP.
 *
 * License: LA_OPT_NXP_Software_License
 *
 * This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms.  If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.
 *
 * 
 * @version   4.0
 * 
 * @brief     IEC60730 Class B Safety Routines for Clock test
 *
 ***********************************************************************************************/

#ifndef _IEC60730B_CLOCK_
#define _IEC60730B_CLOCK_

/******************************************************************************
* Global variables
******************************************************************************/
/* Clock test result constants */
#define FS_CLK_PASS      (0x00000000)
#define FS_CLK_FAIL      (FS_CLK_FAIL_CODE|0x1)
#define FS_CLK_PROGRESS  (FS_CLK_FAIL_CODE|0x2)

/******************************************************************************
* Global functions
******************************************************************************/
#ifndef __ASM__
    
/*******************************************************************************
*
* brief  FS_CLK_Check
*
* param  testContext, low_limit, high_limit
*
* return FS_RESULT: - FS_CLK_PROGRESS
*                   - FS_CLK_FAIL
*                   - FS_CLK_PASS
*                     		
* remarks Function checks if the testContext value is within the defined limits. 
*
*******************************************************************************/
FS_RESULT FS_CLK_Check(uint32_t testContext, uint32_t limitLow, uint32_t limitHigh);

/*******************************************************************************
*
* brief  FS_CLK_Init
*
* param  *pTestContext
*
* return void
*                     		
* remarks Function initializes the clock test.
*
*******************************************************************************/
void FS_CLK_Init(uint32_t *pTestContext);

/*******************************************************************************
*
* brief  FS_CLK_LPTMR
*
* param  *pSafetyTmr, *pTestContext
*
* return void
*                     		
* remarks. Function records the counter value from LPTMR and start the LPTMR again.
*
*******************************************************************************/
void FS_CLK_LPTMR(uint32_t *pSafetyTmr, uint32_t *pTestContext);

/*******************************************************************************
*
* brief  FS_CLK_RTC
*
* param  *pSafetyTmr, *pTestContext
*
* return void
*                     		
* remarks. Function records the counter value from RTC and start the RTC again.
*
*******************************************************************************/
void FS_CLK_RTC(uint32_t *pSafetyTmr, uint32_t *pTestContext);

/*******************************************************************************
*
* brief  FS_CLK_GPT
*
* param  *pSafetyTmr, *pTestContext
*
* return void
*                     		
* remarks. Function records the counter value from GPT and start the GPT again.
*
*******************************************************************************/
void FS_CLK_GPT(uint32_t *pSafetyTmr, uint32_t *pTestContext);

/*******************************************************************************
*
* brief  FS_CLK_WKT_LPC
*
* param  *pSafetyTmr, *pTestContext, startValue
*
* return void
*                     		
* remarks. Function records the counter value from WKT and start the WKT again.
*
*******************************************************************************/
void FS_CLK_WKT_LPC(uint32_t *pSafetyTmr, uint32_t *pTestContext, uint32_t startValue);

/*******************************************************************************
*
* brief  FS_CLK_CTIMER_LPC
*
* param  *pSafetyTmr, *pTestContext
*
* return void
*                     		
* remarks. Function records the counter value from CTIMER and start the CTIMER again.
*
*******************************************************************************/
void FS_CLK_CTIMER_LPC(uint32_t *pSafetyTmr, uint32_t *pTestContext);

#endif /* __ASM__ */

#endif /* _IEC60730B_CLOCK_ */
