/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
 
#ifndef _APP_COAP_OBSERVE_H
#define _APP_COAP_OBSERVE_H
/*!=================================================================================================
\file       app_coap_observe.h
\brief      This is a header file for the app_coap_observe module.

\copyright  (c) Copyright 2016, Freescale, Inc.  All rights reserved.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "network_utils.h"
#include "coap.h"
/*==================================================================================================
Public macros
==================================================================================================*/
#ifndef COAP_OBSERVE_SERVER
    /* Enables CoAP Observe Server functionality */
    #define COAP_OBSERVE_SERVER         0
#endif

#ifndef COAP_OBSERVE_CLIENT
    /* Enables CoAP Observe Client functionality */
    #define COAP_OBSERVE_CLIENT         0
#endif

#ifndef COAP_MAX_OBSERVERS
    /* Maximum number of CoAP Clients supported by a server */
    #define COAP_MAX_OBSERVERS          5
#endif
/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef struct coapObserverInfo_tag
{
    ipAddr_t clientAddr;                /*!< observer's IPv6 address */
    uint8_t tokenLen;                   /*!< the actual length of the token */
    uint8_t token[COAP_MAX_TOKEN_LEN];  /*!< token used for notifications */
    coapUriPath_t* pResource;           /*!< pointer to string resource and string length */
    uint16_t remotePort;                /*!< for Copper compatibility */ 
} coapObserverInfo_t;


/*==================================================================================================
Public global variables declarations
==================================================================================================*/
#if COAP_OBSERVE_SERVER
    extern coapObserverInfo_t mCoapObservers[];
#endif

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#if COAP_OBSERVE_CLIENT
/*!*************************************************************************************************
\fn    uint8_t COAP_Client_InitObserve()
\brief This function initializes a client for a CoAP resource.

\param [in]       ipIfId                    IP interface id
\param [in]       pCoapStartUnsecParams     pointer to coap instance init parameters (port and 
                                            address family)
\param [in]       pCallbacksStruct          pointer to the callbacks for server's resources
\param [in]       nbOfCallbacks             number of callbacks 

\return       
***************************************************************************************************/
uint8_t COAP_Client_InitObserve(ipIfUniqueId_t ipIfId, sockaddrStorage_t *pCoapStartUnsecParams,
                                    coapRegCbParams_t* pCallbacksStruct, uint32_t nbOfCallbacks);

/*!*************************************************************************************************
\fn    void COAP_Client_SendGetObserve()
\brief This function send a GET request with Observe option included.

\param [in]       pServerIpAddr   Pointer to IPv6 address of the server
\param [in]       pResource       Pointer to CoAP resource
\param [in]       rcvReplyCb      callback function pointer for receiving the reply

\return       
***************************************************************************************************/
void COAP_Client_StartObserving(ipAddr_t* pServerIpAddr, coapUriPath_t* pResource,
                                                coapCallback_t rcvReplyCb);
/*!*************************************************************************************************
\fn    void COAP_Client_SendGetObserve()
\brief This function send a GET request with Observe option included.

\param [in]       pServerIpAddr   Pointer to IPv6 address of the server
\param [in]       pResource       Pointer to CoAP resource
\param [in]       rcvReplyCb      callback function pointer for receiving the reply

\return       
***************************************************************************************************/
void COAP_Client_StopObserving(ipAddr_t* pServerIpAddr, coapUriPath_t* pResource,
                                                coapCallback_t rcvReplyCb);

#endif

#if COAP_OBSERVE_SERVER
/*!*************************************************************************************************
\fn    uint8_t COAP_Server_InitObserve()
\brief This function initializes a server for a CoAP resource.

\param [in]       ipIfId                    IP interface id
\param [in]       pCoapStartUnsecParams     pointer to coap instance init parameters (port and 
                                            address family)
\param [in]       pCallbacksStruct          pointer to the callbacks for server's resources
\param [in]       nbOfCallbacks             number of callbacks 

\return       
***************************************************************************************************/
uint8_t COAP_Server_InitObserve(ipIfUniqueId_t ipIfId,sockaddrStorage_t *pCoapStartUnsecParams,
    coapRegCbParams_t* pCallbacksStruct, uint32_t nbOfCallbacks);

/*!*************************************************************************************************
\fn    void COAP_NotifyObservers(type1_t parmName1, type2_t parmName2)
\brief This function checks the list of subscribers and sends a notification to clients 
       registered for that resource.

\param [in]       pResource       pointer to string resource and string length
\param [in]       coapMsgType     CON or NON -> make sure 1 CON is sent at least in 24 hours 
\param [in]       pValue          pointer to payload value
\param [in]       valueLen        length of data

\return       
***************************************************************************************************/
void COAP_Server_NotifyObservers(coapUriPath_t* pResource, coapMessageTypes_t coapMsgType,
                                    uint8_t sequenceId, uint8_t* pValue, uint8_t valueLen);

/*!*************************************************************************************************
\fn    bool_t COAP_Server_AddObserver()
\brief This function adds a new client in the list of observers.

\param [in]       pSession      pointer to coap session 
\param [in]       pResource     pointer to string resource and string length 


\return       
***************************************************************************************************/
bool_t COAP_Server_AddObserver(coapSession_t* pSession, coapUriPath_t* pResource);

/*!*************************************************************************************************
\fn    bool_t COAP_Server_RemoveObserver()
\brief This function removes a client from the list of observers.

\param [in]       pSession      pointer to coap session 
\param [in]       pResource     pointer to string resource and string length 


\return       
***************************************************************************************************/
bool_t COAP_Server_RemoveObserver(coapSession_t* pSession, coapUriPath_t* pResource);

#endif

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _APP_COAP_OBSERVE_H */
