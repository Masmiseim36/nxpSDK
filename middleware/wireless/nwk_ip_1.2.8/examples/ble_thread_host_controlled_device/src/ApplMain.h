/*!
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file AppMain.h
* This is the header file for the main application.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APPL_MAIN_H_
#define _APPL_MAIN_H_

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "l2ca_cb_interface.h"

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

/*! Enable/disable use NV flash procedures for operations triggered by the host stack */
#ifndef gAppUseNvm_d
#define gAppUseNvm_d    (FALSE)
#endif

/*! Number of maximum connections supported at application level. Do not modify this
    directly. Redefine it in app_preinclude.h if the application supports multiple
    connections */
#ifndef gAppMaxConnections_c
#define gAppMaxConnections_c     1
#endif

/*
 * These values should be modified by the application as necessary.
 * They are used by the idle task initialization code from ApplMain.c.
 */
/*! Idle Task Stack Size */
#ifndef gAppIdleTaskStackSize_c
#define gAppIdleTaskStackSize_c (400)
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
* \brief  Application wrapper function for Gap_StartScanning.
*
* \param[in] pScanningParameters The scanning parameters; may be NULL.
* \param[in] scanningCallback The scanning callback.
* \param[in] enableFilterDuplicates Enable duplicate advertising reports filtering
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
    bool_t                      enableFilterDuplicates
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

void App_GenericCallback(gapGenericEvent_t* pGenericEvent);
#endif /* _APPL_MAIN_H_ */
