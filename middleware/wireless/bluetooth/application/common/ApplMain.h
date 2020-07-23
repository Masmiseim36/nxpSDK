/*! *********************************************************************************
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
*
* \file
*
* This is the header file for the main application.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef APPL_MAIN_H
#define APPL_MAIN_H

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "Keyboard.h"
#include "l2ca_cb_interface.h"
#include "ble_config.h"

/* Bluetooth Low Energy */
#include "gatt_interface.h"
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "ble_init.h"
#include "ble_config.h"
#include "l2ca_cb_interface.h"
#include "ble_constants.h"

#if !defined(gUseHciTransportDownward_d) || (!gUseHciTransportDownward_d)
#include "controller_interface.h"
#endif /* gUseHciTransportDownward_d */

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef void* appCallbackParam_t;
typedef void (*appCallbackHandler_t)(appCallbackParam_t param);

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */

/*! Enable/disable use NV flash procedures for operations triggered by the host stack
    Do not modify directly. Redefine it in the app_preinclude.h file*/
#ifndef gAppUseNvm_d
#define gAppUseNvm_d    (FALSE)
#endif

/*
 * These values should be modified by the application as necessary.
 * They are used by the idle task initialization code from ApplMain.c.
 */
/*! Idle Task Stack Size */
#ifndef gAppIdleTaskStackSize_c
#define gAppIdleTaskStackSize_c (500)
#endif

/*! Idle Task OS Abstraction Priority */
#ifndef gAppIdleTaskPriority_c
#define gAppIdleTaskPriority_c  (8)
#endif

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
#if !defined(gHybridApp_d) || (!gHybridApp_d)
/*! *********************************************************************************
* \brief  This is the first task created by the OS. This task will initialize
*         the system
*
* \param[in]  param
*
********************************************************************************** */
void main_task(uint32_t param);
#endif

extern void BleApp_Init(void);
extern void BleApp_HandleKeys(key_event_t events);

#if defined(gUseHciTransportUpward_d) && (gUseHciTransportUpward_d)
#define BleApp_GenericCallback(param)
#else
extern void BleApp_GenericCallback(gapGenericEvent_t *pGenericEvent);
#endif

/*! *********************************************************************************
* \brief  Application wrapper function for Gap_Connect.
*
* \param[in] pParameters           Callback used by the application to receive advertising events.
*                                  Can be NULL.
* \param[in] connCallback Callback used to receive connection events.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function should be used by the application if the callback should
*          be executed in the context of the Application Task.
*
********************************************************************************** */
bleResult_t App_Connect(
    gapConnectionRequestParameters_t*   pParameters,
    gapConnectionCallback_t             connCallback
);

/*! *********************************************************************************
* \brief  Application wrapper function for Gap_StartAdvertising.
*
* \param[in] advertisingCallback   Callback used by the application to receive advertising events.
*                                  Can be NULL.
* \param[in] connectionCallback    Callback used by the application to receive connection events.
*                                  Can be NULL.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function should be used by the application if the callback should
*          be executed in the context of the Application Task.
*
********************************************************************************** */
bleResult_t App_StartAdvertising(
    gapAdvertisingCallback_t    advertisingCallback,
    gapConnectionCallback_t     connectionCallback
);

/*! *********************************************************************************
* \brief  Application wrapper function for Gap_StartExtAdvertising.
*
* \param[in] advertisingCallback   Callback used by the application to receive advertising events.
*                                  Can be NULL.
* \param[in] connectionCallback    Callback used by the application to receive connection events.
*                                  Can be NULL.
* \param[in] handle                The ID of the advertising set
* \param[in] duration              The duration of the advertising
* \param[in] maxExtAdvEvents       The maximum number of advertising events
*
* \return  gBleSuccess_c or error.
*
* \remarks This function should be used by the application if the callback should
*          be executed in the context of the Application Task.
*
********************************************************************************** */
bleResult_t App_StartExtAdvertising(
    gapAdvertisingCallback_t    advertisingCallback,
    gapConnectionCallback_t     connectionCallback,
    uint8_t                     handle,
    uint16_t                    duration,
    uint8_t                     maxExtAdvEvents
);


/*! *********************************************************************************
* \brief  Application wrapper function for Gap_StartScanning.
*
* \param[in] pScanningParameters The scanning parameters; may be NULL.
* \param[in] scanningCallback The scanning callback.
* \param[in] enableFilterDuplicates Enable duplicate advertising reports filtering
* \param[in] duration               Scan duration
* \param[in] period                 Scan period
*
* \return  gBleSuccess_c or error.
*
* \remarks This function should be used by the application if the callback should
*          be executed in the context of the Application Task.
*
********************************************************************************** */
bleResult_t App_StartScanning(
    gapScanningParameters_t*    pScanningParameters,
    gapScanningCallback_t       scanningCallback,
    gapFilterDuplicates_t       enableFilterDuplicates,
    uint16_t                    duration,
    uint16_t                    period
);

/*! *********************************************************************************
* \brief  Application wrapper function for GattClient_RegisterNotificationCallback.
*
* \param[in] callback   Application defined callback to be triggered by this module.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function should be used by the application if the callback should
*          be executed in the context of the Application Task.
*
********************************************************************************** */
bleResult_t App_RegisterGattClientNotificationCallback(
    gattClientNotificationCallback_t  callback
);

/*! *********************************************************************************
* \brief  Application wrapper function for GattClient_RegisterIndicationCallback.
*
* \param[in] callback   Application defined callback to be triggered by this module.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function should be used by the application if the callback should
*          be executed in the context of the Application Task.
*
********************************************************************************** */
bleResult_t App_RegisterGattClientIndicationCallback(
    gattClientIndicationCallback_t  callback
);

/*! *********************************************************************************
* \brief  Application wrapper function for GattServer_RegisterCallback.
*
* \param[in] callback Application-defined callback to be triggered by this module.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function should be used by the application if the callback should
*          be executed in the context of the Application Task.
*
********************************************************************************** */
bleResult_t App_RegisterGattServerCallback(
    gattServerCallback_t  serverCallback
);

/*! *********************************************************************************
* \brief  Application wrapper function for App_RegisterGattClientProcedureCallback.
*
* \param[in] callback Application-defined callback to be triggered by this module.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function should be used by the application if the callback should
*          be executed in the context of the Application Task.
*
********************************************************************************** */
bleResult_t App_RegisterGattClientProcedureCallback(
    gattClientProcedureCallback_t  callback
);

/*! *********************************************************************************
* \brief  Application wrapper function for L2ca_RegisterLeCbCallbacks.
*
* \param[in] callback Application-defined callback to be triggered by this module.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function should be used by the application if the callback should
*          be executed in the context of the Application Task.
*
********************************************************************************** */
bleResult_t App_RegisterLeCbCallbacks
(
    l2caLeCbDataCallback_t      pCallback,
    l2caLeCbControlCallback_t   pCtrlCallback
);

/*! *********************************************************************************
* \brief  Posts an application event containing a callback handler and parameter.
*
* \param[in] handler Handler function, to be executed when the event is processed.
* \param[in] param   Parameter for the handler function.
*
* \return  gBleSuccess_c or error.
*
* \remarks This function should be used by the application if a callback must
*          be executed in the context of the Application Task.
*
********************************************************************************** */
bleResult_t App_PostCallbackMessage
(
    appCallbackHandler_t   handler,
    appCallbackParam_t     param
);

void App_NvmErase(uint8_t mEntryIdx);
void App_NvmWrite(
    uint8_t  mEntryIdx,
    void*    pBondHeader,
    void*    pBondDataDynamic,
    void*    pBondDataStatic,
    void*    pBondDataDeviceInfo,
    void*    pBondDataDescriptor,
    uint8_t  mDescriptorIndex
);
void App_NvmRead(
    uint8_t  mEntryIdx,
    void*    pBondHeader,
    void*    pBondDataDynamic,
    void*    pBondDataStatic,
    void*    pBondDataDeviceInfo,
    void*    pBondDataDescriptor,
    uint8_t  mDescriptorIndex
);

#endif /* APPL_MAIN_H */
