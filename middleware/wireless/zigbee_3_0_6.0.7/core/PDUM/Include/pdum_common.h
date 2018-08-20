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

#ifndef PDUM_COMMON_H_
#define PDUM_COMMON_H_

/*!
\file       pdum_common.h
\brief      PDU manager common definitions Manages NPDUs
*/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

/* ensure pdum_common.h is only included from pdum_nwk or pdum_app headers */
#if !(defined(PDUM_NWK_H_) || defined(PDUM_APP_H_))
#error "pdum_common.h should not be directly included. Include either pdum_nwk.h or pdum_app.h"
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define PDUM_INVALID_HANDLE     NULL

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* Status return codes */
typedef enum {
    PDUM_E_OK,
    PDUM_E_INVALID_HANDLE,
    PDUM_E_BAD_DIRECTION,
    PDUM_E_BAD_PARAM,
    PDUM_E_NPDUS_EXHAUSTED,
    PDUM_E_NPDU_TOO_BIG,
    PDUM_E_NPDU_ALREADY_FREE,
    PDUM_E_APDU_INSTANCE_ALREADY_FREE,
    PDUM_E_APDU_INSTANCE_TOO_BIG,
    PDUM_E_INTERNAL_ERROR
} PDUM_teStatus;

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC uint16 PDUM_u16SizeNBO(const char *szFormat);

#endif /*PDUMAN_H_*/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
