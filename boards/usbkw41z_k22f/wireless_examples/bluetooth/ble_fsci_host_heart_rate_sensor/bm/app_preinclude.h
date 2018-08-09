/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the app configuration file which is pre included.
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