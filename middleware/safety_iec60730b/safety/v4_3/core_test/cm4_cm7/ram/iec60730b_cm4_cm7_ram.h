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
 * @version 4.1
 *
 * @brief RAM test routines for Cortex-M4/M7 core - IEC60730 Class B.
 */

#ifndef IEC60730B_CM4_CM7_RAM_H_
#define IEC60730B_CM4_CM7_RAM_H_

/*
 * List of devices and supported functions can be found in library user guide,
 * section Core self test library - source code version.
 */

/******************************************************************************
 * Macros
 ******************************************************************************/
/* Return values */
#define FS_FAIL_RAM (FS_RAM_CODE + 0x01)

/* Assembler skips following section */
#ifndef __ASM__

/******************************************************************************
 * Definitions
 ******************************************************************************/
typedef FS_RESULT (*tFcn)(uint32_t startAddress, uint32_t blockSize);

/******************************************************************************
 * Functions
 ******************************************************************************/
/*******************************************************************************
 *
 * @brief    Function performs March test of the RAM after reset.
 *
 * @param    startAddress  - the first adress of tested RAM area.
 * @param    endAddress    - address of the first byte after the tested RAM area.
 * @param    blockSize     - tested block size.
 * @param    backupAddress - address of the backup area.
 * @param    *pMarchType   - address of the March function (MarchX or MarchC).
 *
 * @return   FS_PASS
 *           FS_FAIL_RAM
 *
 ******************************************************************************/
FS_RESULT FS_CM4_CM7_RAM_AfterReset(
    uint32_t startAddress, uint32_t endAddress, uint32_t blockSize, uint32_t backupAddress, tFcn pMarchType);

/*******************************************************************************
 *
 * @brief    Function performs March test of the RAM during runtime.
 *
 * @param    startAddress    - the first adress of tested RAM area.
 * @param    endAddress      - address of the first byte after the tested RAM area.
 * @param    *pActualAddress - adress of variable which holds actual address value.
 * @param    blockSize       - tested block size.
 * @param    backupAddress   - address of the backup area.
 * @param    *pMarchType     - address of the March function (MarchX or MarchC).
 *
 * @return   FS_PASS
 *           FS_FAIL_RAM
 *
 ******************************************************************************/
FS_RESULT FS_CM4_CM7_RAM_Runtime(uint32_t startAddress,
                                 uint32_t endAddress,
                                 uint32_t *pActualAddress,
                                 uint32_t blockSize,
                                 uint32_t backupAddress,
                                 tFcn pMarchType);

/*******************************************************************************
 *
 * @brief    Function copies block of memory to dedicated backup area.
 *
 * @param    startAddress  - the first adress of source.
 * @param    blockSize     - size of block memory.
 * @param    backupAddress - address of the backup area.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_CM4_CM7_RAM_CopyToBackup(uint32_t startAddress, uint32_t blockSize, uint32_t backupAddress);

/*******************************************************************************
 *
 * @brief    Function copies block of memory from backup area to dedicated place.
 *
 * @param    startAddress  - the first adress of destination.
 * @param    blockSize     - size of block memory.
 * @param    backupAddress - address of the backup area.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_CM4_CM7_RAM_CopyFromBackup(uint32_t startAddress, uint32_t blockSize, uint32_t backupAddress);

/*******************************************************************************
 *
 * @brief    Function performs March C test.
 *
 * @param    startAddress - the first adress of tested memory block.
 * @param    blockSize    - size of tested memory block.
 *
 * @return   FS_PASS
 *           FS_FAIL_RAM
 *
 ******************************************************************************/
FS_RESULT FS_CM4_CM7_RAM_SegmentMarchC(uint32_t startAddress, uint32_t blockSize);

/*******************************************************************************
 *
 * @brief    Function performs March X test.
 *
 * @param    startAddress - the first adress of tested memory block.
 * @param    blockSize    - size of tested memory block.
 *
 * @return   FS_PASS
 *           FS_FAIL_RAM
 *
 ******************************************************************************/
FS_RESULT FS_CM4_CM7_RAM_SegmentMarchX(uint32_t startAddress, uint32_t blockSize);

#endif /* __ASM__ */
#endif /* IEC60730B_CM4_CM7_RAM_H_ */
