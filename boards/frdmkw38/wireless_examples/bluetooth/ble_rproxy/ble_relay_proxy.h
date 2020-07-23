/*! *********************************************************************************
 * \defgroup Bluetooth Relay Proxy
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the Bluetooth Relay Proxy
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef BLE_RELAY_PROXY_H
#define BLE_RELAY_PROXY_H

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/
/*! Selection for the proxied device is made based on the specified service */
#ifndef gAppProxySelectService_d
#define gAppProxySelectService_d  gBleSig_HeartRateService_d
#endif

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
extern gapConnectionParameters_t        gPrefferedConnectionParams;
/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void BleApp_Init(void);
void BleApp_Start(void);

#ifdef __cplusplus
}
#endif 


#endif /* BLE_RELAY_PROXY_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
