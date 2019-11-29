/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_MONITORING_H             
#define _APP_MONITORING_H

/*!=================================================================================================
\file       app_monitoring.h
\brief      This is the header file for the app event monitoring system
==================================================================================================*/
#if THR_ENABLE_EVENT_MONITORING
   
/*==================================================================================================
Include Files
==================================================================================================*/

/*==================================================================================================
Public macros
==================================================================================================*/


/*==================================================================================================
Public type definitions
==================================================================================================*/
/* event monitor data - byte alignment */
typedef struct eventMonitorData_tag  
{
    uint8_t eventSet[2];            /* Event set */
    uint8_t instanceId;             /* InstanceId */
    uint8_t eventStatus[2];         /* Event Status */
    uint8_t dataSize[2];            /* Event Data Size */
    uint8_t aData[];                /* Event Data: see thrEventData_t */
}eventMonitorData_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/


/*==================================================================================================
Public function prototypes
==================================================================================================*/

/*!*************************************************************************************************
\public
\fn     void APP_EventMonitor_Handler(uint8_t* param)
\brief  This function is used to monitor stack events.

\param  [in]    param   pointer to stack event

\return         void
***************************************************************************************************/
void APP_EventMonitor_Handler(uint8_t* param);
/*!*************************************************************************************************
\fn    bool_t APP_EventMonitorRegister(uint32_t eventSet, pfFunction_t pfFunction, instanceId_t instanceId
                                  pfFunction_t pfFunction)                                                
\brief Registers listening for an event dinamically(at run time). 

\param [in]   eventSet      event set code (multiple events)
\param [in]   instanceId    instance Id of the module which is providing the event. Use gEvmAnyInstanceId_c
                            to be notified for all instance Ids
\param [in]   pfFunction    callback function for event
\param [out]  status        TRUE - Success, FALSE - failed
***************************************************************************************************/
bool_t APP_EventMonitorRegister(uint32_t eventSet, instanceId_t instanceId, pfFunction_t pfFunction);
/*!*************************************************************************************************
\fn    bool_t APP_EventMonitorUnregister(uint32_t eventSet, instanceId_t instanceId)                                                
\brief Unregisters listening for an event dinamically(at run time). 

\param [in]   code          event code (single event) / event set code (multiple events)
\param [in]   instanceId    instance Id of the module which is providing the event. Use gEvmAnyInstanceId_c
                            to be notified for all instance Ids
\param [out]  status        TRUE - Success, FALSE - failed
***************************************************************************************************/
bool_t APP_EventMonitorUnregister(uint32_t eventSet, instanceId_t instanceId);
/*!*************************************************************************************************
\fn    uint16_t APP_InitEventMonitor(instanceId_t instanceId)                                               
\brief  Init event monitoring

\param [in]   instanceId    thread instance Id
***************************************************************************************************/
void APP_InitEventMonitor(instanceId_t instanceId);
#ifdef __cplusplus
extern "C" {
#endif


  
#ifdef __cplusplus
}
#endif

#endif /*  THR_ENABLE_EVENT_MONITORING */
/*================================================================================================*/
#endif  /* _APP_MONITORING_H */
