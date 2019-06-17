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

#ifndef __EVKMIMXRT1060_DCD_SDRAM_INIT__
#define __EVKMIMXRT1060_DCD_SDRAM_INIT__

#include <stdint.h>

/*! @name Driver version */
/*@{*/
/*! @brief XIP_BOARD driver version 2.0.0. */
#define FSL_XIP_BOARD_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*@}*/

/*************************************
 *  DCD Data
 *************************************/
#define DCD_TAG_HEADER (0xD2)
#define DCD_TAG_HEADER_SHIFT (24)
#define DCD_VERSION (0x40)
#define DCD_ARRAY_SIZE 1

#endif /* __EVKMIMXRT1060_DCD_SDRAM_INIT__ */
