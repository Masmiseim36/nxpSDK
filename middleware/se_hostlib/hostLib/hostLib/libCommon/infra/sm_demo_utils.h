/**
 * @file sm_demo_utils.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par LICENSE
 * Copyright 2016 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * This file provides the interface to utility functions used by the example programs, not
 * the actual Host Library.
 * @par HISTORY
 * 1.0   20-mar-2018 : Initial version
 *
 */

#ifndef _sm_demo_utils_H_
#define _sm_demo_utils_H_


#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if SSS_HAVE_A71CH
#include <HLSETypes.h>
#endif

/*******************************************************************************
 * includes
 ******************************************************************************/
#include "jsmn.h"

/*******************************************************************************
 * DEFINITONS
 ******************************************************************************/

/*******************************************************************************
 * TYPES
 ******************************************************************************/

typedef enum GpStorageMode
{
    SET,
    GET
} GpStorageMode_t;

/*******************************************************************
* GLOBAL VARIABLES
*******************************************************************/
#if SSS_HAVE_A71CH
extern HLSE_OBJECT_HANDLE Gpstorage_handle;
#endif

/*******************************************************************
* GLOBAL FUNCTION DECLARATIONS
*******************************************************************/

/* Init network and provide a 18 bit buffer to create a
 * unique-enough MAC and avoid MAC Clash.
 *
 * The 18 byte buffer is extracted from the A7x IC.
 */
extern void BOARD_InitNetwork_MAC(const unsigned char buffer[18]);

#if SSS_HAVE_A71CH
extern int SetGetFlag_GPstorage(U32 *p_val, GpStorageMode_t mode, HLSE_OBJECT_HANDLE handle);

extern int GetHandle_GPstorage(HLSE_OBJECT_INDEX index);
#endif

extern int8_t jsoneq(const char *json, jsmntok_t *tok, const char *s);
#ifdef __cplusplus
}
#endif
#endif /*_sm_demo_utils_H_*/
