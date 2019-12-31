/*! *********************************************************************************
 * \defgroup Bluetooth Shell Application
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the Bluetooth Shell Application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef APP_H
#define APP_H

#include "gap_interface.h"
/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/

/* Enable Advertising Extension shell commands */
#ifndef BLE_SHELL_AE_SUPPORT
#define BLE_SHELL_AE_SUPPORT    0U
#endif

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef struct thrConfig_tag
{
  uint16_t buffCnt;
  uint8_t buffSz;
  bool_t bTestInProgress;
} thrConfig_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
extern gapAdvertisingData_t             gAppAdvertisingData;
extern gapScanResponseData_t            gAppScanRspData;
extern gapAdvertisingParameters_t       gAdvParams;
extern gapScanningParameters_t          gAppScanParams;
extern gapConnectionRequestParameters_t gConnReqParams;
extern gapPairingParameters_t           gPairingParameters;
extern gapExtAdvertisingParameters_t    gExtAdvParams;
extern gapScanResponseData_t            gAppExtScanRspData;
extern gapAdvertisingData_t             gAppExtAdvertisingData;
extern gapPeriodicAdvParameters_t       gPeriodicAdvParams;
extern gapAdvertisingData_t             gAppPeriodicAdvData;

/* Peer Device ID */
extern gapSmpKeys_t                     gSmpKeys;
extern uint8_t                          gActiveConnections;

extern thrConfig_t  gThroughputConfig[];
extern bool_t       gUseShellThrGenericCb;

/** Bit manipulation macros */
#define SET_NEW_CONN(bit)           ((gActiveConnections) |=  (1U << (bit)))

#define CLEAR_CONN(bit)             ((gActiveConnections)  &= ~(1U << (bit)))

#define IS_CONNECTED(bit)           ((1U << (bit)) & (gActiveConnections))

/* Timeout to consider a throughput test finished.
   This is multiplied with the number of active connections */
#ifndef SHELL_THRPUT_TIMEOUT_MS
#define SHELL_THRPUT_TIMEOUT_MS     (3U*((uint32_t)gGapDefaultMaxConnectionInterval_d + (uint32_t)gGapDefaultMaxConnectionInterval_d/4U))
#endif
/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*! *********************************************************************************
* \brief    Initializes application specific functionality before the BLE stack init.
*
********************************************************************************** */
void BleApp_Init(void);

/*! *********************************************************************************
 * \brief        Parses a string input interpreting its content as a hex number.
 *
 * \param[in]    pInput         Pointer to string
 *
 * \return       uint8_t        Returns the equivalent of the string as an uint8_t number
 ********************************************************************************** */
uint8_t BleApp_ParseHexValue(char* pInput);

/*!*************************************************************************************************
\fn     int32_t BleApp_atoi(char *str)
\brief  Converts a string into an integer.

\param  [in]    pStr       Pointer to string

\return                    Integer converted from string.
***************************************************************************************************/
int32_t BleApp_atoi(char *pStr);

/*!*************************************************************************************************
\fn     uint32_t BleApp_AsciiToHex(char *pString, uint32_t strLen)
\brief  Converts a string into hex.

\param  [in]    pString     pointer to string
\param  [in]    strLen      string length

\return uint32_t value in hex
***************************************************************************************************/
uint32_t BleApp_AsciiToHex(char *pString, uint32_t strLen);

#ifdef __cplusplus
}
#endif


#endif /* APP_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
