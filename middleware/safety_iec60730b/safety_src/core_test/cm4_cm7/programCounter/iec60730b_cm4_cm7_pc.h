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

/*******************************************************************************
*
* file      IEC60730B_B_CM4_CM7_pc.h
*
* author    B48632
* 
* version   3.0
*
* date      Nov-01-2018
* 
* brief     IEC60730 Class B Safety Routines Program counter core register tests for Cortex CM4, CM7 cores
*
*******************************************************************************/
#ifndef _IEC60730_B_CM4_CM7_PC_H_
#define _IEC60730_B_CM4_CM7_PC_H_

/*******************************************************************************
* Macros
*******************************************************************************/
#define IEC60730B_ST_PC_PASS    (0x0000)
#define IEC60730B_ST_PC_FAIL    (IEC60730B_ST_PC_FAIL_CODE + 0x01)  // | -> +

/******************************************************************************
* Global functions
******************************************************************************/
#ifndef __ASM__
typedef void (*tFcn_pc)(void);  /*  for using the address of function IEC60730B_PC_object as a parameter of IEC60730B_CM7_PC_Test */

/*********************************************************************************************************
* brief        Function make an initialization for PC test on CM4 core-based devices
* param[in]    pattern1         Dedicated address in RAM memory, adequate as a pattern for PC.
* param[in]    pattern2         Dedicated address in RAM memory, adequate as a pattern for PC.
* return      void
**********************************************************************************************************/
void IEC60730B_CM4_PC_Init(unsigned long pattern1, unsigned long pattern2);

/*********************************************************************************************************
* brief        This function executes the PC test for CM4 core-based devices .
* param[in]    pattern1         Dedicated address in RAM memory, adequate as a pattern for PC.
* param[in]    pattern2         Dedicated address in RAM memory, adequate as a pattern for PC.
* param[in]    * PC_flag address of variable / place in memory used as a flag. If it is 0, test was successful, if 1, test went wrong
* return      IEC60730B_ST_PC_PASS
*             IEC60730B_ST_PC_FAIL
**********************************************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_PC_Test(unsigned long pattern1, unsigned long pattern2, unsigned long* PC_flag);

/*********************************************************************************************************
* brief        This function executes the PC test for devices with CM4 and CM7 cores .
* param[in]    pattern1         Address from RAM memory, adequate as a pattern for PC.
* param[in]    pObject_function Address of IEC60730B_PC_object() function
* param[in]    * PC_flag address of variable / place in memory used as a flag. If it is 0, test was successful, if 1, test went wrong
* return      IEC60730B_ST_PC_PASS
*             IEC60730B_ST_PC_FAIL
**********************************************************************************************************/
IEC60730B_RESULT IEC60730B_CM7_PC_Test(unsigned long pattern1, tFcn_pc pObject_function, unsigned long* PC_flag);

/*********************************************************************************************************
* brief        This function is internally used by the IEC60730B_CM7_PC_Test() function.
* param[in]    none
* return       void
**********************************************************************************************************/
void IEC60730B_PC_object(void);

#endif  /* __ASM__ */
#endif  /* IEC60730_B_CM4_CM7_PC_H_ */
