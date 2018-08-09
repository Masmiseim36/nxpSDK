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
#define smpEdiv                 0x1F99
#define mcEncryptionKeySize_c   16
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/* Default Advertising Parameters. Values can be changed at runtime 
    to align with profile requirements */
gapAdvertisingParameters_t gAdvParams = {
    /* minInterval */         gGapAdvertisingIntervalDefault_c, 
    /* maxInterval */         gGapAdvertisingIntervalDefault_c, 
    /* advertisingType */     gAdvConnectableUndirected_c, 
    /* addressType */         gBleAddrTypePublic_c, 
    /* directedAddressType */ gBleAddrTypePublic_c, 
    /* directedAddress */     {0, 0, 0, 0, 0, 0}, 
    /* channelMap */          (gapAdvertisingChannelMapFlags_t) (gGapAdvertisingChannelMapDefault_c), 
    /* filterPolicy */        gProcessAll_c 
};

/* Scanning and Advertising Data */
static gapAdStructure_t advScanStruct[10];

gapAdvertisingData_t gAppAdvertisingData = 
{
    0,
    (void *)advScanStruct
};

gapScanResponseData_t gAppScanRspData = 
{
    0,
    NULL
};

gapScanningParameters_t gScanParams =
{
    /* type */              gScanTypePassive_c,
    /* interval */          gGapScanIntervalDefault_d,
    /* window */            gGapScanWindowDefault_d,
    /* ownAddressType */    gBleAddrTypePublic_c,
    /* filterPolicy */      gScanAll_c 
};

/* Default Connection Request Parameters */
gapConnectionRequestParameters_t gConnReqParams =
{
    .scanInterval = gGapScanIntervalDefault_d,
    .scanWindow = gGapScanWindowDefault_d,
    .filterPolicy = gUseDeviceAddress_c,
    .ownAddressType = gBleAddrTypePublic_c,
    .connIntervalMin = gGapDefaultMaxConnectionInterval_d,
    .connIntervalMax = gGapDefaultMaxConnectionInterval_d,
    .connLatency = gGapConnLatencyMin_d,
    .supervisionTimeout = gGapConnSuperTimeoutMax_d,
    .connEventLengthMin = gGapConnEventLengthMin_d,
    .connEventLengthMax = gGapConnEventLengthMax_d,
};

gapPairingParameters_t gPairingParameters = {
    .withBonding = TRUE,
    .securityModeAndLevel = gSecurityMode_1_Level_3_c,
    .maxEncryptionKeySize = mcEncryptionKeySize_c,
    .localIoCapabilities = gIoKeyboardDisplay_c,
    .oobAvailable = FALSE,
    .centralKeys = (gapSmpKeyFlags_t)(gLtk_c | gIrk_c),
    .peripheralKeys = (gapSmpKeyFlags_t)(gLtk_c | gIrk_c),
    .leSecureConnectionSupported = FALSE,
    .useKeypressNotifications = FALSE,
};


/* LTK */
static uint8_t smpLtk[mcEncryptionKeySize_c] =
    {0xD6, 0x93, 0xE8, 0xA4, 0x23, 0x55, 0x48, 0x99,
     0x1D, 0x77, 0x61, 0xE6, 0x63, 0x2B, 0x10, 0x8E};

/* RAND*/
static uint8_t smpRand[gcSmpMaxRandSize_c] =
    {0x26, 0x1E, 0xF6, 0x09, 0x97, 0x2E, 0xAD, 0x7E};

/* IRK */
static uint8_t smpIrk[gcSmpIrkSize_c] =
    {0x0A, 0x2D, 0xF4, 0x65, 0xE3, 0xBD, 0x7B, 0x49,
     0x1E, 0xB4, 0xC0, 0x95, 0x95, 0x13, 0x46, 0x73};

/* CSRK */
static uint8_t smpCsrk[gcSmpCsrkSize_c] =
    {0x90, 0xD5, 0x06, 0x95, 0x92, 0xED, 0x91, 0xD7,
     0xA8, 0x9E, 0x2C, 0xDC, 0x4A, 0x93, 0x5B, 0xF9};

gapSmpKeys_t gSmpKeys = {
    .cLtkSize = mcEncryptionKeySize_c,
    .aLtk = (void *)smpLtk,
    .aIrk = (void *)smpIrk,
    .aCsrk = (void *)smpCsrk,
    .aRand = (void *)smpRand,
    .cRandSize = gcSmpMaxRandSize_c,
    .ediv = smpEdiv,
};

/* Device Security Requirements */
static const gapSecurityRequirements_t        masterSecurity = gGapDefaultSecurityRequirements_d;

gapDeviceSecurityRequirements_t deviceSecurityRequirements = {
    .pMasterSecurityRequirements    = (void*)&masterSecurity,
    .cNumServices                   = 0,
    .aServiceSecurityRequirements   = NULL
};
