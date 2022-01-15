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
 * @version   4.2
 *
 * @brief     IEC60730 Class B Safety Routines common header file
 *
 ***********************************************************************************************/

#ifndef IEC60730B_H_
#define IEC60730B_H_

/******************************************************************************
 * Types
 ******************************************************************************/
#include "iec60730b_types.h"

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "asm_mac_common.h"

/* Include type definitions and safety routines */
#include "iec60730b_aio.h"
#include "iec60730b_clock.h"
#include "iec60730b_dio.h"
#include "iec60730b_dio_ext.h"
#include "iec60730b_tsi.h"
#include "iec60730b_wdog.h"
#include "iec60730b_invariable_memory.h"

#endif /* IEC60730B_H_ */
