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
* @file      IEC60730B_B_CM4_CM7_ram.h
*
* @author    B48632
*
* @version   3.0
*
* @date      Nov-01-2018
*
* @brief     IEC60730 Class B Safety Routines RAM tests for Cortex CM4, CM7 cores
*
************************************************************************************************

***********************************************************************************************/
#ifndef _IEC60730_B_CM4_CM7_RAM_H_
#define _IEC60730_B_CM4_CM7_RAM_H_



/******************************************************************************
* Macros
******************************************************************************/


/******************************************************************************
* Constants
******************************************************************************/
#define IEC60730B_ST_RAM_PASS    (0x0000)
/*! @brief Test failed.*/
#define IEC60730B_ST_RAM_FAIL    (IEC60730B_ST_RAM_FAIL_CODE + 0x01)  // | -> +

/******************************************************************************
* Global functions
******************************************************************************/
#ifndef __ASM__

typedef IEC60730B_RESULT (*tFcn)(unsigned long, unsigned long);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_RAM_AfterResetTest
*
* param  first_address, last_address, block_size, backup_address, p_March_type
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                        IEC60730B_ST_RAM_FAIL  - Test failed
*
* remarks Function performs March test of the RAM , after reset.
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_RAM_AfterResetTest(unsigned long first_address, unsigned long last_address, unsigned long block_size, unsigned long backup_address, tFcn pMarch_type);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_RAM_RuntimeTest
*
* param  first_address, last_address, *pActual_address, block_size, backup_address, p_March_type
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                        IEC60730B_ST_RAM_FAIL  - Test failed
*
* remarks Function performs March test of the RAM , during runtime.
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_RAM_RuntimeTest(unsigned long first_address, unsigned long last_address, unsigned long *pActual_address, unsigned long block_size, unsigned long backup_address, tFcn pMarch_type);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_RAM_CopyToBackup
*
* param  start_address, block_size, backup_address
*
* return void
*
* remarks Function copy block of memory to dedicated backup area.
*
*******************************************************************************/
void IEC60730B_CM4_CM7_RAM_CopyToBackup(unsigned long start_address, unsigned long block_size, unsigned long backup_address);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_RAM_CopyFromBackup
*
* param  start_address, block_size, backup_address
*
* return void
*
* remarks Function copy block of memory from backup area to dedicated place.
*
*******************************************************************************/
void IEC60730B_CM4_CM7_RAM_CopyFromBackup(unsigned long start_address, unsigned long block_size, unsigned long backup_address);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_RAM_SegmentMarchC
*
* param  start_address, block_size
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                        IEC60730B_ST_RAM_FAIL  - Test failed
*
* remarks Function performs March C test.
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_RAM_SegmentMarchC(unsigned long start_address, unsigned long block_size);

/*******************************************************************************
*
* brief  IEC60730B_CM4_CM7_RAM_SegmentMarchX
*
* param  start_address, block_size
*
* return Result of test: IEC60730B_ST_CPU_PASS  - Test successfuly done
*                        IEC60730B_ST_RAM_FAIL  - Test failed
*
* remarks Function performs March X test.
*
*******************************************************************************/
IEC60730B_RESULT IEC60730B_CM4_CM7_RAM_SegmentMarchX(unsigned long start_address, unsigned long block_size);


#endif /* __ASM__ */
 #endif /* _IEC60730_B_CM4_CM7_RAM_H_ */
