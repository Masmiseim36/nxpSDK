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
 * @brief     IEC60730 Class B Safety Routines RAM tests for Cortex CM4, CM7 cores
 *
 ***********************************************************************************************/

#ifndef _IEC60730B_CM4_CM7_RAM_
#define _IEC60730B_CM4_CM7_RAM_

/******************************************************************************
* Macros
******************************************************************************/

#define FS_RAM_PASS    (0x0000)
/*! @brief Test failed.*/
#define FS_RAM_FAIL    (FS_RAM_FAIL_CODE + 0x01)

/******************************************************************************
* Global functions
******************************************************************************/
#ifndef __ASM__

typedef FS_RESULT (*tFcn)(uint32_t, uint32_t);

/*******************************************************************************
*
* brief  FS_CM4_CM7_RAM_AfterReset
*
* param  startAddress, endAddress, blockSize, backupAddress, p_March_type
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                        FS_RAM_FAIL  - Test failed
*
* remarks Function performs March test of the RAM , after reset.
*
*******************************************************************************/
FS_RESULT FS_CM4_CM7_RAM_AfterReset(uint32_t startAddress, uint32_t endAddress, uint32_t blockSize, uint32_t backupAddress, tFcn pMarchType);

/*******************************************************************************
*
* brief  FS_CM4_CM7_RAM_Runtime
*
* param  startAddress, endAddress, *pActualAddress, blockSize, backupAddress, p_March_type
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                        FS_RAM_FAIL  - Test failed
*
* remarks Function performs March test of the RAM , during runtime.
*
*******************************************************************************/
FS_RESULT FS_CM4_CM7_RAM_Runtime(uint32_t startAddress, uint32_t endAddress, uint32_t *pActualAddress, uint32_t blockSize, uint32_t backupAddress, tFcn pMarchType);

/*******************************************************************************
*
* brief  FS_CM4_CM7_RAM_CopyToBackup
*
* param  startAddress, blockSize, backupAddress
*
* return void
*
* remarks Function copy block of memory to dedicated backup area.
*
*******************************************************************************/
void FS_CM4_CM7_RAM_CopyToBackup(uint32_t startAddress, uint32_t blockSize, uint32_t backupAddress);

/*******************************************************************************
*
* brief  FS_CM4_CM7_RAM_CopyFromBackup
*
* param  startAddress, blockSize, backupAddress
*
* return void
*
* remarks Function copy block of memory from backup area to dedicated place.
*
*******************************************************************************/
void FS_CM4_CM7_RAM_CopyFromBackup(uint32_t startAddress, uint32_t blockSize, uint32_t backupAddress);

/*******************************************************************************
*
* brief  FS_CM4_CM7_RAM_SegmentMarchC
*
* param  startAddress, blockSize
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                        FS_RAM_FAIL  - Test failed
*
* remarks Function performs March C test.
*
*******************************************************************************/
FS_RESULT FS_CM4_CM7_RAM_SegmentMarchC(uint32_t startAddress, uint32_t blockSize);

/*******************************************************************************
*
* brief  FS_CM4_CM7_RAM_SegmentMarchX
*
* param  startAddress, blockSize
*
* return Result of test: FS_CPU_PASS  - Test successfuly done
*                        FS_RAM_FAIL  - Test failed
*
* remarks Function performs March X test.
*
*******************************************************************************/
FS_RESULT FS_CM4_CM7_RAM_SegmentMarchX(uint32_t startAddress, uint32_t blockSize);

#endif /* __ASM__ */

#endif /* _IEC60730B_CM4_CM7_RAM_ */
