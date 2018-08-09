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


#ifndef PDUM_APP_H_
#define PDUM_APP_H_

/*!
\file       pdum_apl.h
\brief      PDU manager interface for application Manages APDUs
*/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "pdum_common.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* APDU instance handle */
typedef const struct pdum_tsAPduInstance_tag *PDUM_thAPduInstance;

/* APDU handle */
typedef const struct pdum_tsAPdu_tag *PDUM_thAPdu;

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Public Functions                                     ***/
/****************************************************************************/

PUBLIC uint16 PDUM_u16APduInstanceReadNBO(PDUM_thAPduInstance hAPduInst, uint16 u16Pos, const char *szFormat, void *pvStruct);
PUBLIC uint16 PDUM_u16APduInstanceWriteNBO(PDUM_thAPduInstance hAPduInst, uint16 u16Pos, const char *szFormat, ...);
PUBLIC uint16 PDUM_u16APduInstanceWriteStrNBO(PDUM_thAPduInstance hAPduInst, uint16 u16Pos, const char *szFormat, void *pvStruct);
PUBLIC void * PDUM_pvAPduInstanceGetPayload(PDUM_thAPduInstance hAPduInst);
PUBLIC uint16 PDUM_u16APduInstanceGetPayloadSize(PDUM_thAPduInstance hAPduInst);
PUBLIC PDUM_teStatus PDUM_eAPduInstanceSetPayloadSize(PDUM_thAPduInstance hAPduInst, uint16 u16Size);
PUBLIC PDUM_thAPdu PDUM_thAPduInstanceGetApdu(PDUM_thAPduInstance hAPduInst);

PUBLIC PDUM_thAPduInstance PDUM_hAPduAllocateAPduInstance(PDUM_thAPdu hAPdu);
PUBLIC PDUM_teStatus PDUM_eAPduFreeAPduInstance(PDUM_thAPduInstance hAPduInst);
PUBLIC uint16 PDUM_u16APduGetSize(PDUM_thAPdu hAPdu);

PUBLIC uint8 u8GetMaxApdu(void);
PUBLIC uint8 u8GetApduUse(void);

#ifdef DBG_ENABLE
PUBLIC void PDUM_vDBGPrintAPduInstance(PDUM_thAPduInstance hAPduInst);
#else
#define PDUM_vDBGPrintAPduInstance(apdu)
#endif

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*PDUM_APDU_H_*/
