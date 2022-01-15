/**
 * @file sm_demo_utils.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par LICENSE
 *
 * Copyright 2016 NXP
 * SPDX-License-Identifier: Apache-2.0
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

#if SSS_HAVE_APPLET_A71CH
#include <HLSETypes.h>
#endif

/*******************************************************************************
 * includes
 ******************************************************************************/
#if defined (LPC_ENET) || defined (LPC_WIFI)
#include "jsmn.h"
#endif
#if defined (LPC_WIFI)
#   include "iot_wifi.h"
#endif

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
#if SSS_HAVE_APPLET_A71CH
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

#if SSS_HAVE_APPLET_A71CH
extern int SetGetFlag_GPstorage(U32 *p_val, GpStorageMode_t mode, HLSE_OBJECT_HANDLE handle);

extern int GetHandle_GPstorage(HLSE_OBJECT_INDEX index);
#endif

#if defined (LPC_ENET) || defined (LPC_WIFI)
extern int8_t jsoneq(const char *json, jsmntok_t *tok, const char *s);
#endif
#ifdef __cplusplus
}
#endif

#if defined(LPC_WIFI)

WIFIReturnCode_t network_wifi_init(void);

WIFIReturnCode_t network_wifi_connect_ap();

#endif //LPC_WIFI

#endif /*_sm_demo_utils_H_*/
