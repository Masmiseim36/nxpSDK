/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
PUBLIC void APP_vPrintfNullImpl(char * fmt, ...);
extern void ResetMCU(void);
bool bGetInstallCode(uint8_t* pInstallCode);
void APP_vSetLedState(bool_t bDeviceOnTheNwk);

uint64_t hex_str_to_uint(char * str);
uint64_t dec_str_to_uint(char * str);
int16_t hex_str_to_array(char * str, uint8_t * pOut, uint16_t max_len);
char *pccHandlePlaceHolder(char *pcFormat, va_list *pap, char * pOut, int* n);
/*================================================================================================*/


#endif /* _APP_ZB_UTILS_H */
