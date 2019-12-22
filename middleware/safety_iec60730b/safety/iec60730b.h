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
 * @brief     IEC60730 Class B Safety Routines common header file
 *
 ***********************************************************************************************/

#ifndef _IEC60730B_
#define _IEC60730B_

/******************************************************************************
* Error codes for tests
******************************************************************************/
#define FS_CPU_FAIL_CODE      (0x0100)
#define FS_PC_FAIL_CODE       (0x0200)
#define FS_FLASH_FAIL_CODE    (0x0300)
#define FS_RAM_FAIL_CODE      (0x0400)
#define FS_STACK_FAIL_CODE    (0x0500)
#define FS_CLK_FAIL_CODE      (0x0600)
#define FS_AIO_FAIL_CODE      (0x0700)
#define FS_DIO_FAIL_CODE      (0x0800)
#define FS_TSI_FAIL_CODE      (0x0900)

/******************************************************************************
* Includes
******************************************************************************/
/* Macros for IDEs */
#include "asm_mac_common.h"

/* Include type definitions and safety routines */
#include "iec60730b_core.h"
#include "iec60730b_aio.h"
#include "iec60730b_clock.h"
#include "iec60730b_dio.h"
#include "iec60730b_dio_ext.h"
#include "iec60730b_wdog.h"

#endif /* _IEC60730B_ */
