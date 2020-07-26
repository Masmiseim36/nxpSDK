/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_NAND_DISK_H_
#define _FSL_NAND_DISK_H_

#include <stdint.h>
#include "ff.h"
#include "diskio.h"
#include "map.h"

/*!
 * @addtogroup nand Disk
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*************************************************************************************************
 * API
 ************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name nand Disk Function
 * @{
 */

/*!
 * @brief Initializes nand disk.
 *
 * @param pdrv Physical drive number.
 * @retval STA_NOINIT Failed.
 * @retval RES_OK Success.
 */
DSTATUS nand_disk_initialize(BYTE pdrv);

/*!
 * Gets nand disk status
 *
 * @param pdrv Physical drive number.
 * @retval STA_NOINIT Failed.
 * @retval RES_OK Success.
 */
DSTATUS nand_disk_status(BYTE pdrv);

/*!
 * @brief Reads nand disk.
 *
 * @param pdrv Physical drive number.
 * @param buff The data buffer pointer to store read content.
 * @param sector The start sector number to be read.
 * @param count The sector count to be read.
 * @retval RES_PARERR Failed.
 * @retval RES_OK Success.
 */
DRESULT nand_disk_read(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count);

/*!
 * @brief Writes nand disk.
 *
 * @param pdrv Physical drive number.
 * @param buff The data buffer pointer to store write content.
 * @param sector The start sector number to be written.
 * @param count The sector count to be written.
 * @retval RES_PARERR Failed.
 * @retval RES_OK Success.
 */
DRESULT nand_disk_write(BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count);

/*!
 * @brief nand disk IO operation.
 *
 * @param pdrv Physical drive number.
 * @param cmd The command to be set.
 * @param buff The buffer to store command result.
 * @retval RES_PARERR Failed.
 * @retval RES_OK Success.
 */
DRESULT nand_disk_ioctl(BYTE pdrv, BYTE cmd, void* buff);

/* @} */
#if defined(__cplusplus)
}
#endif

#endif /* _FSL_NAND_DISK_H_ */
