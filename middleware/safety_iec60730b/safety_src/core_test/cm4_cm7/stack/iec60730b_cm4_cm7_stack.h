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
* file      IEC60730B_B_CM4_CM7_Stack.h
*
* author    B48632
*
* version   3.0
*
* date      Nov-01-2018
*
* brief     IEC60730 Class B Safety Routines Stack overflow test for Cortex CM4, CM7 cores
*
*******************************************************************************/
#ifndef _IEC60730_B_STACK_H_
#define _IEC60730_B_STACK_H_


/*******************************************************************************
* Macros
*******************************************************************************/
#define IEC60730B_ST_STACK_PASS    (0x0000)
#define IEC60730B_ST_STACK_FAIL    (IEC60730B_ST_STACK_FAIL_CODE + 0x01)  // | -> +


/*******************************************************************************
* Global functions
*******************************************************************************/
#ifndef __ASM__


/*******************************************************************************
*
* brief    IEC60730B_CM4_CM7_Stack_Init
*
* param    stack_test_pattern, first_address, second_address, block_size
*
* return   void
*
* remarks  Function performs initialization of Stack test. block_size must be multiple of 4.
* block_size = 0x4 - the minimal size. Corresponds to one 32-bit memory space
*
*******************************************************************************/
void IEC60730B_CM4_CM7_Stack_Init(unsigned long stack_test_pattern, unsigned long first_address, unsigned long second_address, unsigned long block_size);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_Stack_Test
*
* param  stack_test_pattern, first_address, second_address, block_size
*
* return Result of test: IEC60730B_ST_STACK_PASS  - Test successfuly done
*                        IEC60730B_ST_STACK_FAIL  - Test failed
*
* remarks Function performs Stack overflow test. block_size must be multiple of 4.
* block_size = 0x4 - the minimal size. Corresponds to one 32-bit memory space
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_Stack_Test(unsigned long stack_test_pattern, unsigned long first_address, unsigned long second_address, unsigned long block_size);

#endif /* __ASM__ */
#endif  /* _IEC60730_B_CM4_CM7_STACK_H_ */
