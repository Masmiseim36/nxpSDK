/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
     
#ifndef BDB_NF_INCLUDED
#define BDB_NF_INCLUDED

/*!
\file       bdb_nf.h
\brief      BDB NetworkFormation API
*/

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "bdb_api.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum
{
    E_NF_IDLE,
    E_NF_WAIT_FORM_CENTRALIZED,
    E_NF_WAIT_DISCOVERY,
    E_NF_WAIT_FORM_DISTRIBUTED
}teNF_State;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC BDB_teStatus BDB_eNfStartNwkFormation(void);
PUBLIC void BDB_vNfStateMachine(BDB_tsZpsAfEvent *psZpsAfEvent);
PUBLIC void BDB_vNfFormCentralizedNwk(void);
PUBLIC uint16 BDB_vNfGetPanID(void);
PUBLIC void BDB_vNfSetPanID(uint16 panId);

PUBLIC uint16 BDB_vNfGetNwkAddress(void);
PUBLIC void BDB_vNfSetNwkAddress(uint16 nwkAddr);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern PUBLIC teNF_State eNF_State;


#if defined __cplusplus
}
#endif

#endif  /* BDB_NF_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/






