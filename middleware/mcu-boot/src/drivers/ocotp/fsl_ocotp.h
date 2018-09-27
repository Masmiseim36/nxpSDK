/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __FSL_OCOTP_H__
#define __FSL_OCOTP_H__

#include "fsl_common.h"
#include "bootloader_common.h"

/**********************************************************************************************************************
 * Definitions
 *********************************************************************************************************************/

/*! @brief Status for FLEXSPI NAND driver */
enum _serial_nand_status
{
    kStatus_OCOTP_ReadFailure = MAKE_STATUS(kStatusGroup_OCOTP, 0),    /*! < OCOTP Read Failure */
    kStatus_OCOTP_ProgramFailure = MAKE_STATUS(kStatusGroup_OCOTP, 1), /*! < OCOTP Program Failure */
    kStatus_OCOTP_ReloadFailure = MAKE_STATUS(kStatusGroup_OCOTP, 2),  /*! < OCOTP Reload Shadows Failure */
    kStatus_OCOTP_WaitTimeout = MAKE_STATUS(kStatusGroup_OCOTP, 3),    /*! < OCOTP Access Timeout */
};

/**********************************************************************************************************************
 * API
 *********************************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

//!@brief Initialize OCOTP
status_t ocotp_init(OCOTP_Type *base);

//!@brief Program OCOTP
status_t ocotp_program_once(OCOTP_Type *base, uint32_t index, uint32_t *src, uint32_t lengthInBytes);

//!@brief Read OCOTP
status_t ocotp_read_once(OCOTP_Type *base, uint32_t index, uint32_t *dst, uint32_t lengthInBytes);

uint32_t get_ocotp_clock(void);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __FSL_OCOTP_H__
