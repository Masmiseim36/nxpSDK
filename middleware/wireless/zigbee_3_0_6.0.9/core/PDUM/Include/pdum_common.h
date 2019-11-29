/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
