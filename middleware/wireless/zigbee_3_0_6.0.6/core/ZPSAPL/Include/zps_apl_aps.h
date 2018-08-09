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

#ifndef ZPS_APL_APS_H_
#define ZPS_APL_APS_H_

/*!
\file       zps_apl_aps.h
\brief      Application Support Sub-Layer public interface
*/


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define ZPS_E_SUCCESS               (0UL)

/* broadcast network addresses */
#define ZPS_E_BROADCAST_ALL_IEEE    (0xffffffffffffffffULL)
#define ZPS_E_BROADCAST_ALL         (0xffff)
#define ZPS_E_BROADCAST_RX_ON       (0xfffd)
#define ZPS_E_BROADCAST_ZC_ZR       (0xfffc)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum {
    /* [I SP001349_sfr 6] */
    ZPS_APL_APS_E_ASDU_TOO_LONG = 0xa0,
    ZPS_APL_APS_E_DEFRAG_DEFERRED,
    ZPS_APL_APS_E_DEFRAG_UNSUPPORTED,
    ZPS_APL_APS_E_ILLEGAL_REQUEST,
    ZPS_APL_APS_E_INVALID_BINDING,
    ZPS_APL_APS_E_INVALID_GROUP,
    ZPS_APL_APS_E_INVALID_PARAMETER,
    ZPS_APL_APS_E_NO_ACK,
    ZPS_APL_APS_E_NO_BOUND_DEVICE,
    ZPS_APL_APS_E_NO_SHORT_ADDRESS,
    ZPS_APL_APS_E_NOT_SUPPORTED,
    ZPS_APL_APS_E_SECURED_LINK_KEY,
    ZPS_APL_APS_E_SECURED_NWK_KEY,
    ZPS_APL_APS_E_SECURITY_FAIL,
    ZPS_APL_APS_E_TABLE_FULL,
    ZPS_APL_APS_E_UNSECURED,
    ZPS_APL_APS_E_UNSUPPORTED_ATTRIBUTE = 0xb0
} ZPS_teApsStatus;

typedef enum
{
    ZPS_E_ADDR_MODE_BOUND        = 0,
    ZPS_E_ADDR_MODE_GROUP        = 1,
    ZPS_E_ADDR_MODE_SHORT        = 2,
    ZPS_E_ADDR_MODE_IEEE         = 3
} ZPS_teAplApsdeAddressMode;

/* address type for short and long addresses */
typedef union {
    uint16 u16Addr;
    uint64 u64Addr;
} ZPS_tuAddress;

typedef struct
{
  enum {
      ZPS_E_AM_INTERPAN_GROUP = 0x01,
      ZPS_E_AM_INTERPAN_SHORT,
      ZPS_E_AM_INTERPAN_IEEE
  }eMode;
  uint16        u16PanId;
  ZPS_tuAddress uAddress;
} ZPS_tsInterPanAddress;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Inlined Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /* ZPS_APL_APS_H_ */
