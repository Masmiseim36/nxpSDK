/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
