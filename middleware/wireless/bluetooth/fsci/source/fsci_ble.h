/*! *********************************************************************************
 * \defgroup FSCI_BLE
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is a header file for FSCI BLE management.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef FSCI_BLE_H
#define FSCI_BLE_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "EmbeddedTypes.h"
#include "MemManager.h"
#include "fsci_ble_interface.h"
#include "fsci_ble_types.h"

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

#ifndef gFsciBleTest_d
    #define gFsciBleTest_d                          0U
#endif

#ifndef gFsciBleBBox_d
    #define gFsciBleBBox_d                          0U
#endif

#ifndef gFsciBleHost_d
    #define gFsciBleHost_d                          0U
#endif


#if ((gFsciBleTest_d + gFsciBleBBox_d + gFsciBleHost_d) > 1U)
    #error "Select maximum one of gFsciBleTest_d, gFsciBleBBox_d and gFsciBleHost_d"
#endif


/*! Mask that indicates for which BLE layers is FSCI enabled
    BIT0 - if set, FSCI for HCI is enabled
    BIT1 - if set, FSCI for L2CAP is enabled
    BIT2 - if set, FSCI for L2CAP CB is enabled
    BIT3 - if set, FSCI for SM is enabled
    BIT4 - if set, FSCI for ATT is enabled
    BIT5 - if set, FSCI for GATT is enabled
    BIT6 - if set, FSCI for GATT Database (application) is enabled
    BIT7 - if set, FSCI for GATT Database (ATT) is enabled
    BIT8 - if set, FSCI for GAP is enabled
*/

#ifndef gFsciBleEnabledLayersMask_d
    #if gFsciBleTest_d
        #define gFsciBleEnabledLayersMask_d             0x9B        
    #elif gFsciBleBBox_d || gFsciBleHost_d
        #define gFsciBleEnabledLayersMask_d             0x0164
    #else
        #define gFsciBleEnabledLayersMask_d             0
    #endif
#endif

/*! Macro which indicates if FSCI for L2CAP CB is enabled or not */
#if ((gFsciBleEnabledLayersMask_d & 0x0001) != 0U)
    #define gFsciBleHciLayerEnabled_d               1
#else
    #define gFsciBleHciLayerEnabled_d               0
#endif

/*! Macro which indicates if FSCI for L2CAP is enabled or not */
#if ((gFsciBleEnabledLayersMask_d & 0x0002) != 0U)
    #define gFsciBleL2capLayerEnabled_d             1
#else
    #define gFsciBleL2capLayerEnabled_d             0
#endif

/*! Macro which indicates if FSCI for L2CAP CB is enabled or not */
#if ((gFsciBleEnabledLayersMask_d & 0x0004) != 0U)
    #define gFsciBleL2capCbLayerEnabled_d           1
#else
    #define gFsciBleL2capCbLayerEnabled_d           0
#endif

/*! Macro which indicates if FSCI for SM is enabled or not */
#if ((gFsciBleEnabledLayersMask_d & 0x0008) != 0U)
    #define gFsciBleSmLayerEnabled_d                1
#else
    #define gFsciBleSmLayerEnabled_d                0
#endif

/*! Macro which indicates if FSCI for ATT is enabled or not */
#if ((gFsciBleEnabledLayersMask_d & 0x0010) != 0U)
    #define gFsciBleAttLayerEnabled_d               1
#else
    #define gFsciBleAttLayerEnabled_d               0
#endif

/*! Macro which indicates if FSCI for GATT is enabled or not */
#if ((gFsciBleEnabledLayersMask_d & 0x0020) != 0U)
    #define gFsciBleGattLayerEnabled_d              1
#else
    #define gFsciBleGattLayerEnabled_d              0
#endif

/*! Macro which indicates if FSCI for GATT Database (application) is enabled or not */
#if ((gFsciBleEnabledLayersMask_d & 0x0040) != 0U)
    #define gFsciBleGattDbAppLayerEnabled_d         1
#else
    #define gFsciBleGattDbAppLayerEnabled_d         0
#endif

/*! Macro which indicates if FSCI for GATT Database (ATT) is enabled or not */
#if ((gFsciBleEnabledLayersMask_d & 0x0080) != 0U)
    #define gFsciBleGattDbAttLayerEnabled_d         1
#else
    #define gFsciBleGattDbAttLayerEnabled_d         0
#endif

/*! Macro which indicates if FSCI for GAP is enabled or not */
#if ((gFsciBleEnabledLayersMask_d & 0x0100) != 0U)
    #define gFsciBleGapLayerEnabled_d               1
#else
    #define gFsciBleGapLayerEnabled_d               0
#endif
  

#define fsciBleRegisterOpGroup(opGroup, pfHandler, fsciInterface)                FSCI_RegisterOpGroup(opGroup, gFsciMonitorMode_c, pfHandler, NULL, fsciInterface)
#define fsciBleTransmitFormatedPacket(pClientPacket, fsciBleInterfaceIdentifier) FSCI_transmitFormatedPacket((void*)pClientPacket, fsciBleInterfaceIdentifier)
#define fsciBleError(errorCode, fsciInterface)                                   FSCI_Error((uint8_t)errorCode, fsciInterface)


/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/*! FSCI interface on which the monitored commands, events or statuses 
should be printed */
extern uint32_t fsciBleInterfaceId;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*! *********************************************************************************
* \brief  Creates the FSCI packet which contains the status of the last executed BLE 
*         command and sends it over UART.
*
* \param[in]    opCodeGroup     FSCI operation group of the command's layer. 
* \param[in]    opCode          FSCI status operation code.
* \param[in]    result          Status of the executed command.
*
********************************************************************************** */
void fsciBleStatusMonitor
(
    opGroup_t   opCodeGroup, 
    uint8_t     opCode, 
    bleResult_t result
 );

/*! *********************************************************************************
* \brief  Allocates a FSCI packet.
*
* \param[in]    opCodeGroup     FSCI operation group.
* \param[in]    opCode          FSCI operation code.
* \param[in]    dataSize        Size of the payload.
*
* \return The allocated FSCI packet
*
********************************************************************************** */
clientPacketStructured_t* fsciBleAllocFsciPacket
(
    opGroup_t   opCodeGroup,
    uint8_t     opCode, 
    uint16_t    dataSize
);

/*! *********************************************************************************
* \brief  Creates a FSCI packet without payload and sends it over UART.
*
* \param[in]    opCodeGroup     FSCI operation group.
* \param[in]    opCode          FSCI operation code.
*
********************************************************************************** */
void fsciBleNoParamCmdOrEvtMonitor
(
    opGroup_t   opCodeGroup, 
    uint8_t     opCode
);

#ifdef __cplusplus
}
#endif 

#endif /* FSCI_BLE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
