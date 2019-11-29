/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_OBSERVE_DEMO_H
#define _APP_OBSERVE_DEMO_H
/*!=================================================================================================
\file       app_observe_demo.h
\brief      This is a header file for the app_observe_demo module.

\copyright  (c) Copyright 2016, Freescale, Inc.  All rights reserved.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/


/*==================================================================================================
Public macros
==================================================================================================*/


/*==================================================================================================
Public type definitions
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn    void APP_ObserveStartServer()
\brief This is an application function that starts a CoAP server for a set of resources.

\param [in]       msgQueue                  task message queue
\param [in]       interfaceId               IP interface id

\return       
***************************************************************************************************/
void APP_ObserveStartServer
(   
    taskMsgQueue_t * msgQueue,
    ipIfUniqueId_t ipIfId
);

/*!*************************************************************************************************
\fn    void APP_ObserveStartClient()
\brief This is an application function that starts a CoAP observe client for a resource.

\param [in]       msgQueue                  task message queue
\param [in]       interfaceId               IP interface id
\param [in]       pServerIpAddr             pointer to server's IP address
\param [in]       pResource                 pointer to CoAP resource the client wants to observe

\return       
***************************************************************************************************/
void APP_ObserveStartClient
(   
    taskMsgQueue_t * msgQueue,
    ipIfUniqueId_t ipIfId,
    ipAddr_t * pServerIpAddr,
    coapUriPath_t * pResource
);

/*!*************************************************************************************************
\fn    void APP_ObserveStopClient()
\brief This is an application function that stops a CoAP observe client.

\param [in]       msgQueue                  task message queue
\param [in]       interfaceId               IP interface id
\param [in]       pServerIpAddr             pointer to server's IP address
\param [in]       pResource                 pointer to CoAP resource the client wants to observe

\return       
***************************************************************************************************/
void APP_ObserveStopClient
(   
    ipAddr_t * pServerIpAddr,
    coapUriPath_t * pResource
);


#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _APP_OBSERVE_DEMO_H */
