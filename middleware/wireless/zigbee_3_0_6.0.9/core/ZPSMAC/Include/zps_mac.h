/*
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _zps_mac_h_
#define _zps_mac_h_

/*!
\file       zps_mac.h
\brief      MAC shim layer to provide PDU manager i/f to NWK layer
*/
     
#ifdef __cplusplus
extern "C" {
#endif

/***********************/
/**** INCLUDE FILES ****/
/***********************/

#include "jendefs.h"
#include "mac_sap.h"

/************************/
/**** MACROS/DEFINES ****/
/************************/

/**************************/
/**** TYPE DEFINITIONS ****/
/**************************/


/****************************/
/**** EXPORTED VARIABLES ****/
/****************************/

extern PUBLIC const uint32 ZPS_g_u32MacVersion;

/****************************/
/**** EXPORTED FUNCTIONS ****/
/****************************/
PUBLIC uint32 zps_u32GetMacVersion(void);
PUBLIC void
ZPS_vNwkHandleMcpsDcfmInd(void *pvNwk,
                          MAC_DcfmIndHdr_s *psMcpsDcfmInd);






#ifdef __cplusplus
};
#endif

#endif /* _zps_mac_h_ */



