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


/***********************************************************************************************
*
* @file      IEC60730_B_CM4_CM7_clock.h
*
* @author    B48632
* 
* @version   3.0
*
* @date      Nov-01-2018
* 
* @brief     IEC60730 Class B Safety Routines for clock test
*
************************************************************************************************/

#ifndef _IEC60730_B_CM4_CM7_CLK_H_
#define _IEC60730_B_CM4_CM7_CLK_H_


/******************************************************************************
* Constants
******************************************************************************/

/* brief Definition of clock frequency limit.*/
#define IEC60730B_CLK_FREQ_LIMIT      ((IEC60730B_CFG_CLK_TEST_PERIODE_MSEC) * (IEC60730B_CFG_CLK_SYSTEM_CLK_HZ/1000)) 
/* brief Definition of clock high frequency limit.*/
#define IEC60730B_CLK_FREQ_LIMIT_HI   (((IEC60730B_CLK_FREQ_LIMIT) * (100 + IEC60730B_CFG_CLK_DEVIATION_PERCENT)) / 100)
/* brief Definition of clock low frequency limit.*/
#define IEC60730B_CLK_FREQ_LIMIT_LO   (((IEC60730B_CLK_FREQ_LIMIT) * (100 - IEC60730B_CFG_CLK_DEVIATION_PERCENT)) / 100)

/* Clock test results constants */
/* brief Test succesfully.*/
#define IEC60730B_ST_CLK_PASS         (0x00000000)
/* brief Test failed.*/
#define IEC60730B_ST_CLK_FAIL         (IEC60730B_ST_CLK_FAIL_CODE|0x1)
/* brief Test in progress.*/
#define IEC60730B_ST_CLK_PROGRESS     (IEC60730B_ST_CLK_FAIL_CODE|0x2)

/******************************************************************************
* Global functions
******************************************************************************/
#ifndef __ASM__



/* Structure type to access the low power timer (GPT).  */
typedef struct {
  unsigned long CR;                              /* GPT Control Register, offset: 0x0 */
  unsigned long PR;                              /* GPT Prescaler Register, offset: 0x4 */
  unsigned long SR;                              /* GPT Status Register, offset: 0x8 */
  unsigned long IR;                              /* GPT Interrupt Register, offset: 0xC */
  unsigned long OCR[3];                          /* GPT Output Compare Register 1..GPT Output Compare Register 3, array offset: 0x10, array step: 0x4 */
  unsigned long ICR[2];                          /* GPT Input Capture Register 1..GPT Input Capture Register 2, array offset: 0x1C, array step: 0x4 */
  unsigned long CNT;                             /* GPT Counter Register, offset: 0x24 */
} GPT_Str;

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_CLK_Check
*
* param  testContext, low_limit, high_limit
*
* return IEC60730B_RESULT: - IEC60730B_ST_CLK_PROGRESS
*                          - IEC60730B_ST_CLK_FAIL
*                          - IEC60730B_ST_CLK_PASS
*                     		
* remarks Function checks if the testContext value is within the defined limits. 
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_CLK_Check(unsigned long testContext, unsigned long low_limit, unsigned long high_limit);
    
/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_CLK_SYNC_Init
*
* param  *pTestContext
*
* return void
*                     		
* remarks Function initializes the synchronized clock test.
*
*******************************************************************************/
void IEC60730B_CM4_CM7_CLK_SYNC_Init(unsigned long* pTestContext);


/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_CLK_SYNC_LPTMR_Isr
*
* param  *pSafetyTmr, *pTestContext
*
* return void
*                     		
* remarks. Function records the counter value from LPTMR and start the LPTMR again.
*
*******************************************************************************/
void IEC60730B_CM4_CM7_CLK_SYNC_LPTMR_Isr(unsigned long* pSafetyTmr, unsigned long* pTestContext);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_CLK_SYNC_GPT_Isr
*
* param  *pSafetyTmr, *pTestContext
*
* return void
*                     		
* remarks. Function records the counter value from GPT and start the GPT again.
*
*******************************************************************************/
void IEC60730B_CM4_CM7_CLK_SYNC_GPT_Isr(unsigned long* pSafetyTmr, unsigned long* pTestContext);

#endif /* __ASM__ */
	
#endif /* _IEC60730_B_CM4_CM7_CLK_H_ */
