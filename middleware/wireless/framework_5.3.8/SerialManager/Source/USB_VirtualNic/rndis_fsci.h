/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
