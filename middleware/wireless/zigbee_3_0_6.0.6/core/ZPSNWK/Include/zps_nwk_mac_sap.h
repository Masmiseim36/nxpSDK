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
