/**
 * @file tst_utils_kinetis.h
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

#ifndef _TST_UTILS_KINETIS_H_
#define _TST_UTILS_KINETIS_H_


#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * includes
 ******************************************************************************/
#include "jsmn.h"
#include "board.h"
#include "sm_types.h"
#include <HLSETypes.h>

/*******************************************************************************
 * DEFINITONS
 ******************************************************************************/

#if defined(CPU_LPC54018JET180) || defined(IMX_RT)
#define LED_RED_INIT(output)    //LED1_INIT(output)
#define LED_RED_ON()            //LED1_ON()
#define LED_RED_OFF()           //LED1_OFF()
#define LED_RED_TOGGLE()        //LED1_TOGGLE()

#define LED_GREEN_INIT(output)  //LED2_INIT(output)
#define LED_GREEN_ON()          //LED2_ON()
#define LED_GREEN_OFF()         //LED2_OFF()
#define LED_GREEN_TOGGLE()      //LED2_TOGGLE()

#define LED_BLUE_INIT(output)   //LED3_INIT(output)
#define LED_BLUE_ON()           //LED3_ON()
#define LED_BLUE_OFF()          //LED3_OFF()
#define LED_BLUE_TOGGLE()       //LED3_TOGGLE()
#endif
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
extern HLSE_OBJECT_HANDLE Gpstorage_handle;

/*******************************************************************
* GLOBAL FUNCTION DECLARATIONS
*******************************************************************/
extern void BOARD_InitNetwork(const unsigned char buffer[18]);

extern int SetGetFlag_GPstorage(U32 *p_val, GpStorageMode_t mode, HLSE_OBJECT_HANDLE handle);

extern int GetHandle_GPstorage(HLSE_OBJECT_INDEX index);

extern int8_t jsoneq(const char *json, jsmntok_t *tok, const char *s);
#ifdef __cplusplus
}
#endif
#endif /*_TST_UTILS_KINETIS_H_*/
