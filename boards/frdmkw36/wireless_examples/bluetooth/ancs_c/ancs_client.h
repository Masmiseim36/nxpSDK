/*! *********************************************************************************
 * \defgroup ANCS Client
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright 2018 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the ANCS Client application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef ANCS_CLIENT_H
#define ANCS_CLIENT_H

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/

/* Profile Parameters */

#define gFastConnMinAdvInterval_c       32 /* 20 ms */
#define gFastConnMaxAdvInterval_c       48 /* 30 ms */

#define gReducedPowerMinAdvInterval_c   1600 /* 1 s */
#define gReducedPowerMaxAdvInterval_c   4000 /* 2.5 s */

#define gFastConnAdvTime_c              30  /* s */
#define gReducedPowerAdvTime_c          300 /* s */

#if gAppUseBonding_d
#define gFastConnWhiteListAdvTime_c     10 /* s */
#else
#define gFastConnWhiteListAdvTime_c     0
#endif

#ifndef gAppUseTimeService_d
#define gAppUseTimeService_d    0
#endif

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
extern gapAdvertisingData_t             gAppAdvertisingData;
extern gapScanResponseData_t            gAppScanRspData;
extern gapAdvertisingParameters_t       gAdvParams;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void BleApp_Start(void);

#ifdef __cplusplus
}
#endif


#endif /* ANCS_CLIENT_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
