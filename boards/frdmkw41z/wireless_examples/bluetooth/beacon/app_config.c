/*! *********************************************************************************
* \addtogroup App Config
* @{
********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* This file contains configuration data for the application and stack
*
* SPDX-License-Identifier: BSD-3-Clause
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
