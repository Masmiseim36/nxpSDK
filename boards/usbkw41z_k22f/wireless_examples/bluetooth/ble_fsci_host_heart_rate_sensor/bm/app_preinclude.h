/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* This file is the app configuration file which is pre included.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef __APP_PREINCLUDE_H__
#define __APP_PREINCLUDE_H__

/*! *********************************************************************************
 * KSDK configuration 
 ********************************************************************************** */
#define CPU_MK22FN512VMP12              1
#define USB_STACK_BM                    1

/*! *********************************************************************************
 * Connectivity Framework configuration 
 ********************************************************************************** */
#define gSerialMgrUseUart_c             1
#define gSerialMgrUseSPI_c              0
#define gSerialMgrUseIIC_c              0
   
/* Defines Rx Buffer Size for Serial Manager */
#define gSerialMgrRxBufSize_c           320

#define gFsciHostSupport_c              1
#define gFsciIncluded_c                 1
#define gFsciMaxInterfaces_c            1
#define gFsciLenHas2Bytes_c             1
#define gFsciTxAck_c                    0 
#define gFsciRxAck_c                    0
#define gFsciRxTimeout_c                1
#define mFsciRxTimeoutUsePolling_c      1
#define gFsciRxAckTimeoutUseTmr_c       0
#define gFsciMaxPayloadLen_c            600
#define gKBD_KeysCount_c                1
#define gLEDSupported_d                 1
#define gLEDsOnTargetBoardCnt_c         1
#define gEepromType_d                   gEepromDevice_None_c

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define PoolsDetails_c \
         _block_size_  32  _number_of_blocks_    6 _eol_  \
         _block_size_  64  _number_of_blocks_    3 _eol_  \
         _block_size_ 128  _number_of_blocks_   10 _eol_  \
         _block_size_ 640  _number_of_blocks_   10 _eol_

/*! *********************************************************************************
 * 	App Configuration
 ********************************************************************************** */
/*! Enable/disable use of bonding capability */
#define gAppUseBonding_d   0

/*! Enable/disable use of pairing procedure */
#define gAppUsePairing_d   0

/*! Enable/disable use of privacy */
#define gAppUsePrivacy_d   0

#define gPasskeyValue_c                999999

#if (gAppUseBonding_d) && (!gAppUsePairing_d)
  #error "Enable pairing to make use of bonding"
#endif
             
#define gGattDbDynamic_d                1
#define gUseHciTransportDownward_d      1

#if gSerialMgrUseUart_c
    #define gAppFSCIHostInterfaceBaud_d     gUARTBaudRate115200_c
    #define gAppFSCIHostInterfaceType_d     gSerialMgrUart_c
    #define gAppFSCIHostInterfaceId_d       1
#elif gSerialMgrUseSPI_c
    #define gAppFSCIHostInterfaceBaud_d     gSPI_BaudRate_1000000_c
    #define gAppFSCIHostInterfaceType_d     gSerialMgrSPIMaster_c
    #define gAppFSCIHostInterfaceId_d       0
#elif gSerialMgrUseIIC_c
    #define gAppFSCIHostInterfaceBaud_d     gIIC_BaudRate_100000_c
    #define gAppFSCIHostInterfaceType_d     gSerialMgrIICMaster_c
    #define gAppFSCIHostInterfaceId_d       1
#endif   
   
#endif /* __APP_PREINCLUDE_H__ */