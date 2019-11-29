/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_ZB_UTILS_H
#define _APP_ZB_UTILS_H

/*!
\file       app_zb_utils.h
\brief      This is a header file for the applications zigbee utilities.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include <jendefs.h>
#include "EmbeddedTypes.h"


/*==================================================================================================
Public macros
==================================================================================================*/
#define MAX_PARAM_LEN 20

/* IEEE address. If set to all FFs a unique address will be generated. */
#ifndef gMacAddress_c
#define gMacAddress_c 0xFFFFFFFFFFFFFFFF
#endif

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

extern uint8     APP_SerialId;


/*==================================================================================================
Public function prototypes
==================================================================================================*/

PUBLIC void APP_vSetMacAddr(void);
PUBLIC void APP_vPrintf(char * fmt, ...);
PUBLIC void APP_vPrintfNullImpl(bool_t stream, ...);
extern void ResetMCU(void);
bool bGetInstallCode(uint8_t* pInstallCode);
void APP_vSetLedState(bool_t bDeviceOnTheNwk);

uint64_t hex_str_to_uint(char * str);
uint64_t dec_str_to_uint(char * str);
int16_t hex_str_to_array(char * str, uint8_t * pOut, uint16_t max_len);
char *pccHandlePlaceHolder(char *pcFormat, va_list *pap, char * pOut, int* n);
/*================================================================================================*/


#endif /* _APP_ZB_UTILS_H */
