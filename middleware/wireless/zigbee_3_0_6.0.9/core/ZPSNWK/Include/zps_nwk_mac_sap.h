/*
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* SPDX-License-Identifier: BSD-3-Clause
*/

/**
 * @defgroup g_zps_nwk ZPS Network Layer infrastructure
 */

#ifndef _zps_nwk_mac_sap_h_
#define _zps_nwk_mac_sap_h_

/*!
\file       zps_nwk_mac_sap.h
\brief      ZPS NWK Public exported API
*/

     
#ifdef __cplusplus
extern "C" {
#endif

/***********************/
/**** INCLUDE FILES ****/
/***********************/

#include "jendefs.h"
#include "mac_sap.h"
#include "mac_vs_sap.h"

/************************/
/**** MACROS/DEFINES ****/
/************************/

/**************************/
/**** TYPE DEFINITIONS ****/
/**************************/

/****************************/
/**** EXPORTED VARIABLES ****/
/****************************/

/****************************/
/**** EXPORTED FUNCTIONS ****/
/****************************/

PUBLIC void
ZPS_vNwkHandleMlmeDcfmInd(void *pvNwk,
        MAC_DcfmIndHdr_s *psMlmeDcfmInd);

PUBLIC void
ZPS_vNwkHandleMcpsVsDcfmInd(void *pvNwk,
                            MAC_tsMcpsVsDcfmInd *psMcpsDcfmInd);

#ifdef __cplusplus
};
#endif

#endif /* _zps_nwk_mac_sap_h_ */

/* End of file $Id: zps_nwk_mac_sap.h 77413 2016-02-26 15:34:22Z nxp29772 $ *******************************************/
