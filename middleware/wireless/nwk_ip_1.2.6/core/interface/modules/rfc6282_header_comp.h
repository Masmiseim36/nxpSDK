/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
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

#ifndef _RFC_6282_HEADER_COMP_H
#define _RFC_6282_HEADER_COMP_H
/*!=================================================================================================
\file       rfc6282_header_comp.h
\brief      This is a header file for the RFC 6282 module. It contains the implementation for 
            6LowPan Header Compression Protocol (6LowPanHC).
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "sixlowpan.h"

/*==================================================================================================
Public macros
==================================================================================================*/


/*==================================================================================================
Public type definitions
==================================================================================================*/

#define LOWPAN_IPHC_HEADER_SIZE     3U
#define LOWPAN_FRAG1_HEADER_SIZE    4U
#define LOWPAN_FRAGN_HEADER_SIZE    5U
#define LOWPAN_MIN_MSDU_SIZE        64U
#define SAM_FIELD_SHIFT             4U

typedef enum
{
    gHeadCompSucces_c = 0U,
    gHeadCompFailed_c,    
    gHeadCompSizeExceeded_c,
    gHeadCompContinueComp_c,
    gHeadCompContinueDecomp_c,
    gHeadCompBadContext_c,
}headCompResultType_t;

typedef struct decompInfo_tag
{
    uuint64_t       srcIID;
    uuint64_t       dstIID;
    nwkBuffer_t *   pDecompBuff;
    uint8_t *       pDecompPos;
    uint8_t         nextHeader;
} decompInfo_t;

/* address compression options structure */
typedef struct addrCompResult_tag
{
  uint8_t   addrMode;
  uint8_t   addrCmpr;
  uint8_t   cid;
}addrCompOpt_t;


/*==================================================================================================
Public global variables declarations
==================================================================================================*/


/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     bool_t RFC6282_CompressPacket(macAbsMcpsDataInd_t * pMcpsDataInd, uint8_t ** ppCrtPos)
\brief  Public interface function for the RFC6282 module. This function is used to compress the packet.

\param  [in]   pSlwpPktInfo    Pointer to the transmission info structure.

\return        bool_t          TRUE if packet has been successfully compressed, FALSE otherwise.
***************************************************************************************************/
bool_t RFC6282_CompressPacket(slwpPktInfo_t * pSlwpPktInfo);

/*!*************************************************************************************************
\fn     bool_t RFC6282_ProcessIPHC(macAbsMcpsDataInd_t * pMcpsDataInd, uint8_t ** ppCrtPos)
\brief  Public interface function for the RFC6282 module. This function processes IP header compressed.

\param  [in]  pMcpsDataInd   The MCPS Data Indication containing the received packet.
\param  [in]  ppCrtPos       Pointer to IPHC current position

\retval       bool_t         Status of the function
***************************************************************************************************/
bool_t RFC6282_ProcessIPHC(macAbsMcpsDataInd_t * pMcpsDataInd, uint8_t ** ppCrtPos);

/*!*************************************************************************************************
\fn     bool_t RFC6282_ProcessSingleFragIPHC(macAbsMcpsDataInd_t * pMcpsDataInd, uint8_t ** ppCrtPos)
\brief  Public interface function for the RFC6282 module. This function processes single fragment
        IP header compressed.

\param  [in]  pMcpsDataInd   The MCPS Data Indication containing the received packet.
\param  [in]  ppCrtPos       Pointer to IPHC current position

\retval       bool_t         Status of the function
***************************************************************************************************/
bool_t RFC6282_ProcessSingleFragIPHC(macAbsMcpsDataInd_t * pMcpsDataInd, uint8_t ** ppCrtPos);

/*!*************************************************************************************************
\fn     uint32_t RFC6282_GetDecompressedSize(macAbsMcpsDataInd_t * pMcpsDataInd)
\brief  Public interface function for the RFC6282 module. This function returns the size of the
        decompressed packet.

\param  [in]  pMcpsDataInd   The MCPS Data Indication containing the received packet.

\retval       uint32_t       Size of the packet.
***************************************************************************************************/
uint32_t RFC6282_GetDecompressedSize(macAbsMcpsDataInd_t * pMcpsDataInd);

/*!*************************************************************************************************
\fn     headCompResultType_t RFC6282_DecompressPacket(macAbsMcpsDataInd_t * pMcpsDataInd,
                                                      uint8_t ** ppCrtPos, decompInfo_t * pDecompInfo)
\brief  Public interface function for the RFC6282 module. This function decompresses the received
        MAC packet.

\param  [in]      pMcpsDataInd            The MCPS Data Indication containing the received packet.
\param  [in]      ppCrtPos                Pointer to IPHC current position
\param  [out]      pDecompInfo             Pointer to the decompressed information

\retval       headCompResultType_t    Result of the function
***************************************************************************************************/
headCompResultType_t RFC6282_DecompressPacket(macAbsMcpsDataInd_t * pMcpsDataInd, uint8_t ** ppCrtPos,
                                              decompInfo_t * pDecompInfo);

/*!*************************************************************************************************
\fn     headCompResultType_t RFC6282_DecompressIpHeader(macAbsMcpsDataInd_t * pMcpsDataInd,
                                                       uint8_t ** ppCrtPos, decompInfo_t * pDecompInfo);
\brief  Public interface function for the RFC6282 module. This function decompresses the received
        IP header in the MAC packet.

\param  [in]  pMcpsDataInd            The MCPS Data Indication containing the received packet.
\param  [in]  ppCrtPos                Pointer to IPHC current position
\param  [out]  pDecompInfo             Pointer to the decompressed information

\retval       headCompResultType_t    Result of the function
***************************************************************************************************/
headCompResultType_t RFC6282_DecompressIpHeader(macAbsMcpsDataInd_t * pMcpsDataInd, uint8_t ** ppCrtPos,
                                                decompInfo_t * pDecompInfo);

/*!*************************************************************************************************
\fn uint32_t RFC6282_DecompressExtentionHeader( macAbsMcpsDataInd_t * pMcpsDataInd,
                            uint8_t ** ppCrtPos, decompInfo_t * pDecompInfo )
\brief  This function decompresses the extension Header.

\param [in]        pMcpsDataInd    The MCPS Data Indication containing the received packet.
\param [in]        ppCrtPos        Pointer to IPHC current position
\param [out]    pDecompInfo        Pointer to the decompressed information

\return       headCompResultType_t    Result of the function
***************************************************************************************************/
headCompResultType_t RFC6282_DecompressExtentionHeader(macAbsMcpsDataInd_t * pMcpsDataInd,
                                                       uint8_t ** ppCrtPos, decompInfo_t * pDecompInfo);

/*!*************************************************************************************************
\fn     headCompResultType_t RFC6282_DecompressUdpHeader(macAbsMcpsDataInd_t * pMcpsDataInd,
                                                         uint8_t ** ppCrtPos, decompInfo_t * pDecompInfo)
\brief  Public interface function for the RFC6282 module. This function decompresses the UDP header
        in the received MAC packet.

\param  [in]  pMcpsDataInd            The MCPS Data Indication containing the received packet.
\param  [in]  ppCrtPos                Pointer to IPHC current position
\param  [out]  pDecompInfo             Pointer to the decompressed information

\return       headCompResultType_t    Result of the function
***************************************************************************************************/
headCompResultType_t RFC6282_DecompressUdpHeader(macAbsMcpsDataInd_t * pMcpsDataInd, uint8_t ** ppCrtPos,
                                                decompInfo_t * pDecompInfo);

/*!*************************************************************************************************
\fn     headCompResultType_t RFC6282_DecompressUnicastAddress(macAbsMcpsDataInd_t * pMcpsDataInd,
                                                              uint8_t ** ppCrtPos, uuint64_t * pIID,
                                                              uint8_t * pIpAddr, addrCompOpt_t * pAddrCompOpt);
\brief  Public interface function for the RFC6282 module. This function decompresses the unicast address
        in the received MAC packet.

\param  [in]      pMcpsDataInd            The MCPS Data Indication containing the received packet.
\param  [in]      ppCrtPos                Pointer to IPHC current position
\param  [in]      pIID                    Pointer to IID information
\param  [out]      pIpAddr                    Pointer to the unicast address of the packet
\param  [in]      pAddrCompOpt            Pointer to the address compression options

\return       headCompResultType_t    Result of the function
***************************************************************************************************/
headCompResultType_t RFC6282_DecompressUnicastAddress(macAbsMcpsDataInd_t * pMcpsDataInd, uint8_t ** ppCrtPos,
                                                      uuint64_t * pIID, uint8_t * pIpAddr,
                                                      addrCompOpt_t * pAddrCompOpt);

/*!*************************************************************************************************
\fn     headCompResultType_t RFC6282_DecompressMulticastAddress(macAbsMcpsDataInd_t * pMcpsDataInd,
                                                                uint8_t ** ppCrtPos, uuint64_t * pIID,
                                                                uint8_t * pIpAddr, addrCompOpt_t * pAddrCompOpt);
\brief  Public interface function for the RFC6282 module. This function decompresses the multicast address
        in the received MAC packet.

\param  [in]   pMcpsDataInd            The MCPS Data Indication containing the received packet.
\param  [in]   ppCrtPos                Pointer to IPHC current position
\param  [in]   pIID                    Pointer to IID information
\param  [out]  pIpAddr                 Pointer to the multicast address of the packet
\param  [in]   pAddrCompOpt            Pointer to the address compression options

\return        headCompResultType_t    Result of the function
***************************************************************************************************/
headCompResultType_t RFC6282_DecompressMulticastAddress(macAbsMcpsDataInd_t * pMcpsDataInd,
                                                        uint8_t ** ppCrtPos, uuint64_t * pIID,
                                                        uint8_t * pIpAddr, addrCompOpt_t * pAddrCompOpt);

/*!*************************************************************************************************
\fn     void RFC6282_ComputeLenghtAndUdpChecksum(macAbsMcpsDataInd_t * pMcpsDataInd)
\brief  Recalculates IP length and UDP checksum.

\param [in]   pMcpsDataInd      pointer to the mcps data indication

\retval       none
***************************************************************************************************/
void RFC6282_ComputeLengthAndUdpChecksum(macAbsMcpsDataInd_t * pMcpsDataInd);

/*!*************************************************************************************************
\fn     bool RFC6282_IsIPHC(uint8_t* pInData)
\brief  Checks if data is a IPHC

\param [in]   pInData       pointer to input data
       
\retval       bool          TRUE or FALSE (operation status)
***************************************************************************************************/
bool_t RFC6282_IsIPHC(uint8_t* pInData);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /*_RFC_6282_HEADER_COMP_H */
