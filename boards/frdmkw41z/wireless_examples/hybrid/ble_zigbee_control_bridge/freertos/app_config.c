/*! *********************************************************************************
* \addtogroup App Config
* @{
********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This file contains configuration data for the application and stack
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

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "gap_interface.h"
#include "ble_constants.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define mAdvCompanyId   0xFF, 0x01
#define mBeaconId       0xBC

/* This value will be set to a random value on BLE App init */
#define mUuid           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
/* Advertising Parameters */
const gapAdvertisingParameters_t gAppAdvParams = {
    /* minInterval */         800 /* 500 ms */, \
    /* maxInterval */         1600 /* 1 s */, \
    /* advertisingType */     gAdvNonConnectable_c, \
    /* addressType */         gBleAddrTypePublic_c, \
    /* directedAddressType */ gBleAddrTypePublic_c, \
    /* directedAddress */     {0, 0, 0, 0, 0, 0}, \
    /* channelMap */          (gapAdvertisingChannelMapFlags_t) (gAdvChanMapFlag37_c | gAdvChanMapFlag38_c | gAdvChanMapFlag39_c), \
    /* filterPolicy */        gProcessAll_c \
};

/* Advertising Data */
static const uint8_t adData0[1] =  { (gapAdTypeFlags_t)(gLeGeneralDiscoverableMode_c | gBrEdrNotSupported_c) };
static uint8_t adData1[26] = { 
                       /* Company Identifier*/ 
                       mAdvCompanyId,    
                       /* Beacon Identifier */ 
                       mBeaconId,
                       /* UUID */
                       mUuid,                               
                       /* A */
                       0x00, 0x00,
                       /* B */
                       0x00, 0x00,
                       /* C */
                       0x00, 0x00,
                       /* RSSI at 1m */
                       0x1E};

static const gapAdStructure_t advScanStruct[] = {
  {
    .length = NumberOfElements(adData0) + 1,
    .adType = gAdFlags_c,
    .aData = (void *)adData0
  },  
  {
    .length = NumberOfElements(adData1) + 1,
    .adType = gAdManufacturerSpecificData_c,
    .aData = (void *)adData1
  }
};

gapAdvertisingData_t gAppAdvertisingData = 
{
    NumberOfElements(advScanStruct),
    (void *)advScanStruct
};
