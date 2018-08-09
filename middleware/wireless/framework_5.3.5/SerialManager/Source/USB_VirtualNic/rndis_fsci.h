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
#ifndef _RNDIS_FSCI_H_
#define _RNDIS_FSCI_H_ 1

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#define gFSCI_RndisReqOpcodeGroup_c      0x81
#define gFSCI_RndisCnfOpcodeGroup_c      0x82

#ifndef gFsciHost_Rndis_c
#define gFsciHost_Rndis_c 0
#endif

#ifndef gFsciHost_WaitForStatusTimeout_c
#define gFsciHost_WaitForStatusTimeout_c  500 /* milliseconds */
#endif
/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
enum { /* RNDIS Req OG = 0x81 */
  /* Request Op Codes */
  mFsciRndisInitReqOpcode_c                     = 0x00,
  mFsciRndisSendReqOpcode_c                     = 0x01,
  mFsciRndisSetDestMacAddrReqOpcode_c           = 0x02,
  mFsciRndisSetRndisMacAddrReqOpcode_c          = 0x03,
  mFsciRndisEnableIpv6ReqOpcode_c               = 0x04,
  mFsciRndisEnableIpv4ReqOpcode_c               = 0x05,
  mFsciRndisRegisterMulticastMacAddrReqOpCode_c = 0x06,
};

enum { /* RNDIS Cnf OG = 0x82 */
  /* Confirm Op Codes */
  mFsciRndisInitCnfOpcode_c                     = 0x00,
  mFsciRndisSendCnfOpcode_c                     = 0x01,
  mFsciRndisSetDestMacAddrCnfOpcode_c           = 0x02,
  mFsciRndisSetRndisMacAddrCnfOpcode_c          = 0x03,
  mFsciRndisEnableIpv6CnfOpcode_c               = 0x04,
  mFsciRndisEnableIpv4CnfOpcode_c               = 0x05,
  mFsciRndisRegisterMulticastMacAddrCnfOpCode_c = 0x06,
  /* Indication Op Codes */
  mFsciRndisDataIndOpcode_c                     = 0x07,
};

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
void fsciRegisterRndis		( uint32_t fsciInterfaceId );
void fsciRegisterRndisToHost( uint32_t fsciInterfaceId );

#endif /* _RNDIS_FSCI_H_ */
