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
typedef enum MAC_teRadioType_tag				/* Indicates MAC Radio Frequency types */
{
	E_MAC_FREQ_2400=0,
	E_MAC_FREQ_868=1,
	E_MAC_FREQ_915=2,
	E_MAC_FREQ_DEVICE_NOT_FOUND=0xFE,
	E_MAC_FREQ_UNKNOWN_ERROR=0xFF

} MAC_teRadioType;

/****************************/
/**** EXPORTED VARIABLES ****/
/****************************/

extern PUBLIC const uint32 ZPS_g_u32MacVersion;

/****************************/
/**** EXPORTED FUNCTIONS ****/
/****************************/

PUBLIC void
ZPS_vNwkHandleMcpsDcfmInd(void *pvNwk,
                          MAC_DcfmIndHdr_s *psMcpsDcfmInd);
PUBLIC MAC_teRadioType
ZPS_eMacRadioTypeFromChannelMask(uint32 u32ChannelMask);

PUBLIC teMacStatus
ZPS_eMacChannelsToChannelMask(uint8 u8Page, uint8 *pu8ChannelList, uint8 u8Size, uint32 *pu32Mask);

PUBLIC uint8
ZPS_u8MacChannelsFromChannelMask(uint32 u32Mask, uint8  *pu8Size, uint8 *pu8Page, uint8 *pu8ChannelList);


PUBLIC uint8
ZPS_u8MacGetChannelOffset(uint8 u8Page);

PUBLIC void
ZPS_vMacPibSetEBFilteringEnable(bool_t bEBFilteringEnabled);

PUBLIC void
ZPS_vMacPibSetEBR_Payload(const uint8* pu8EBRPayload, uint8 u8EBRPayloadLen);



#ifdef __cplusplus
};
#endif

#endif /* _zps_mac_h_ */



