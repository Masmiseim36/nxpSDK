/*! *********************************************************************************
 * \defgroup Wireless UART Application
 * @{
 ********************************************************************************** */
/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
 * \file app.h
 * This file is the interface file for the Wireless UART application
*/

#ifndef _APP_H_
#define _APP_H_

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/

/*! Enable/disable use of bonding capability */
#ifndef gAppUseBonding_d
#define gAppUseBonding_d   0
#endif

/*! Enable/disable use of pairing procedure */
#ifndef gAppUsePairing_d
#define gAppUsePairing_d   0
#endif

/*! Enable/disable use of privacy */
#ifndef gAppUsePrivacy_d
#define gAppUsePrivacy_d   0
#endif

#if (gAppUseBonding_d) && (!gAppUsePairing_d)
  #error "Enable pairing to make use of bonding"
#endif

/*! Maximum number of active connections */
#ifndef gAppMaxConnections_c
#define gAppMaxConnections_c  1
#endif

#if (gAppMaxConnections_c > 1)
    #error "The application does not support more than 1 connection"
#endif

#define gPasskeyValue_c                999999


/* Profile Parameters */
#define gScanningTime_c         3   /* 3 s*/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void BleApp_Init(void);
void BleApp_Start (gapRole_t mGapStartRole);
void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent);

#ifdef __cplusplus
}
#endif 


#endif /* _APP_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
