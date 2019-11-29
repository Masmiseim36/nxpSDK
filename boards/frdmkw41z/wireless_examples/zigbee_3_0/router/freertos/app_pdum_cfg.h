/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _PDUM_GEN_H
#define _PDUM_GEN_H

/*!
\file       pdum_gen.h
\brief      PDU Manager Configuration
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "pdum_apl.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/* APDUs */
#define apduZDP &pdum_apduZDP
#define apduZCL &pdum_apduZCL

/*! Defines pdum instances by block size and number of blocks.*/   
#ifndef PdumsDetails_c
#define PdumsDetails_c \
         _pdum_handler_name_ (pdum_apduZDP)  _pdum_block_size_ (100) _pdum_queue_size_ (3)  _eol_ \
         _pdum_handler_name_ (pdum_apduZCL)  _pdum_block_size_ (100) _pdum_queue_size_ (10)  _eol_
#endif
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/* APDUs */
extern  struct pdum_tsAPdu_tag pdum_apduZDP;
extern  struct pdum_tsAPdu_tag pdum_apduZCL;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void PDUM_vInit(void);

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif
