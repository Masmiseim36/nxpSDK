/*! *********************************************************************************
 * \defgroup Beacon
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file 
*
* This file is the interface file for the Beacon application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef BLE_GENFSK_ADVERTISING_H
#define BLE_GENFSK_ADVERTISING_H

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/
#define TEST_GENFSK_ADV

#define gGenFSK_AdvAddress_c            0x09, 0x09, 0x09, 0x09, 0x09, 0x09

#define gGenFskDefaultH0FieldSize_c     (8U)
#ifdef TEST_GENFSK_ADV
#define gGenFskDefaultLengthFieldSize_c (8U)
#define gGenFskDefaultH1FieldSize_c     (0U)
#else
#define gGenFskDefaultLengthFieldSize_c (6U)
#define gGenFskDefaultH1FieldSize_c     (2U)
#endif
#define gGenFskDefaultHeaderSizeBytes_c ((gGenFskDefaultH0FieldSize_c + \
                                         gGenFskDefaultLengthFieldSize_c + \
                                             gGenFskDefaultH1FieldSize_c) >> 3U)

#define gGenFskDefaultH0Value_c        (0x0000U)
#define gGenFskDefaultH0Mask_c          0U

#define gGenFskDefaultH1Value_c        (0x0000U)
#ifdef TEST_GENFSK_ADV
#define gGenFskDefaultH1Mask_c         0//((1U << gGenFskDefaultH1FieldSize_c) - 1U)
#else
#define gGenFskDefaultH1Mask_c         ((1U << gGenFskDefaultH1FieldSize_c) - 1U)
#endif

#define gGenFskDefaultSyncAddress_c  (0x8E89BED6U)
#define gGenFskDefaultSyncAddrSize_c (0x03U) /*bytes = size + 1*/

#define gGenFskDefaultCrcSize_c      (0x3U)
#define gGenFskDefaultCrcSeed_c       0x00555555
#define gGenFskDefaultCrccPoly_c      0x0000065B

#define gGenFskDefaultTxPowerLevel_c (0x08U)

#ifdef TEST_GENFSK_ADV
#define gGenFskDefaultChannel_c      (42U)
#else
#define gGenFskDefaultChannel_c      (40U)
#endif

#define gGenFskApp_TxInterval_c      (1000U) /* [ms] */
#define gGenFskApp_GuardTime_c       (4U) /* [ms] */

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
/*! *********************************************************************************
* \brief        Initializat GFSK layer
*
********************************************************************************** */
void GfskApp_Init(uint8_t appSerMgrIf);
/*! *********************************************************************************
* \brief        Start GFSK App timer for periodic TX
*
********************************************************************************** */
void GfskApp_StartTx(void);
/*! *********************************************************************************
* \brief        Stop GFSK App timer
*
********************************************************************************** */
void GfskApp_StopTx(void);
/*! *********************************************************************************
* \brief        Start GFSK App timer for periodic RX
*
********************************************************************************** */
void GfskApp_StartRx(void);
/*! *********************************************************************************
* \brief        Stop GFSK App timer
*
********************************************************************************** */
void GfskApp_StopRx(void);

#ifdef __cplusplus
}
#endif 


#endif /* BLE_GENFSK_ADVERTISING_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
