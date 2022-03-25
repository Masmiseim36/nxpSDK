/*! *********************************************************************************
 * \defgroup Wireless UART Application
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the Wireless UART application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef WIRELESS_UART_H
#define WIRELESS_UART_H

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/

/* Profile Parameters */
#define gScanningTime_c        10   /* 10 s*/

/* Enable/Disable Controller Adv/Scan/Connection Notifications */
#ifndef gUseControllerNotifications_c
#define gUseControllerNotifications_c 0
#endif

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
extern bool_t gAppCodedPhyInUse;

extern gapConnectionRequestParameters_t gConnReqParams;
extern gapScanningParameters_t          gScanParams;
extern gapPairingParameters_t           gPairingParameters;
extern gapAdvertisingData_t             gAppAdvertisingData;
extern gapScanResponseData_t            gAppScanRspData;
extern gapAdvertisingParameters_t       gAdvParams;
extern gapExtAdvertisingParameters_t    gExtAdvParams;

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
* \brief    Initializes application specific functionality
*
********************************************************************************** */
void BleApp_InitSerialInterface(uint8_t appSerMgrIf);

/*! *********************************************************************************
 * \brief    Starts the BLE application.
 *
 * \param[in]    gapRole    GAP Start Role (Central or Peripheral).
 ********************************************************************************** */
void BleApp_Start(gapRole_t gapRole);

#ifdef __cplusplus
}
#endif


#endif /* _APP_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
