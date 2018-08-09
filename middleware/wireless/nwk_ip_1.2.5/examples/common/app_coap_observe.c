/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
 

/*!=================================================================================================
\file       app_coap_observe.c
\brief      This is a public source file for the app_coap_observe module.

\copyright  (c) Copyright 2016, Freescale, Inc.  All rights reserved.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "FunctionLib.h"

#include "app_coap_observe.h"
#include "thread_utils.h"

/*==================================================================================================
Private macros
==================================================================================================*/


/*==================================================================================================
Private type definitions
==================================================================================================*/


/*==================================================================================================
Private prototypes
==================================================================================================*/


/*==================================================================================================
Private global variables declarations
==================================================================================================*/
#if COAP_OBSERVE_CLIENT

/* Standard value for CoAP Observe option send by client */
const uint8_t gCoapObserveDeregister = 1;
#endif

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
#if COAP_OBSERVE_SERVER
/* table of all registered clients */
coapObserverInfo_t mCoapObservers[COAP_MAX_OBSERVERS]; 

/* CoAP Instance number for Observe Server */
uint8_t mCoapServerInstanceId = THR_ALL_FFs8;
#endif

#if COAP_OBSERVE_CLIENT
/* CoAP Instance number for Observe Client */
uint8_t mCoapClientInstanceId = THR_ALL_FFs8;
#endif
/*==================================================================================================
Public functions
==================================================================================================*/
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
uint8_t COAP_Server_InitObserve
(
    ipIfUniqueId_t ipIfId,
    coapStartUnsecParams_t *pCoapStartUnsecParams,
    coapRegCbParams_t* pCallbacksStruct,
    uint32_t nbOfCallbacks
)
{   
    
    mCoapServerInstanceId = COAP_CreateInstance(NULL, pCoapStartUnsecParams, ipIfId, pCallbacksStruct, 
                                            nbOfCallbacks);

    for(uint32_t iCount = 0; iCount < COAP_MAX_OBSERVERS; iCount++)
    {
        FLib_MemSet(&mCoapObservers[iCount], 0, sizeof(coapObserverInfo_t));
    }

    return mCoapServerInstanceId;
}
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
uint8_t COAP_Client_InitObserve
(
    ipIfUniqueId_t ipIfId,
    coapStartUnsecParams_t *pCoapStartUnsecParams,
    coapRegCbParams_t* pCallbacksStruct,
    uint32_t nbOfCallbacks
)
{
    mCoapClientInstanceId = COAP_CreateInstance(NULL, pCoapStartUnsecParams, ipIfId, pCallbacksStruct, 
                                            nbOfCallbacks);
    return mCoapClientInstanceId;
}
#endif

#if COAP_OBSERVE_SERVER
/*!*************************************************************************************************
\fn    void COAP_Server_NotifyObservers()
\brief This function checks the list of subscribers and sends a notification to clients 
       registered for that resource.

\param [in]       pResource       pointer to string resource and string length
\param [in]       coapMsgType     CON or NON -> make sure 1 CON is sent at least in 24 hours 
\param [in]       pValue          pointer to payload value
\param [in]       valueLen        length of data

\return       
***************************************************************************************************/
void COAP_Server_NotifyObservers 
(
    coapUriPath_t* pResource,
    coapMessageTypes_t coapMsgType,
    uint8_t sequenceId,
    uint8_t* pValue,
    uint8_t valueLen
)
{
    coapSession_t* pSession;

    for (uint32_t iCount = 0; iCount < COAP_MAX_OBSERVERS; iCount++)
    {   
        if (!IP_IsAddrEqual(&mCoapObservers[iCount].clientAddr, &in6addr_any)
            && (FLib_MemCmp(mCoapObservers[iCount].pResource->pUriPath, pResource->pUriPath, 
            mCoapObservers[iCount].pResource->length)))
        {
            pSession = NULL;
            pSession = COAP_OpenSession(mCoapServerInstanceId);
            if (NULL != pSession)
            {
                FLib_MemCpy(&pSession->remoteAddr, &mCoapObservers[iCount].clientAddr, sizeof(ipAddr_t));

                if ((THR_IsIpAddrBasedOnMeshPrefix(gInvalidInstanceId_c, &pSession->remoteAddr))
                && (!NWKU_IsIPAddrBasedOnShort(&pSession->remoteAddr)))
                {                 
                    /* use as ip source address the ML-EID address */
                    (void)THR_GetIP6Addr(0, gMLEIDAddr_c, &pSession->sourceAddr, NULL);                
                }                
                /*use the same token as in the client's request */
                pSession->tokenLen = mCoapObservers[iCount].tokenLen;
                FLib_MemCpy(pSession->aToken, mCoapObservers[iCount].token, mCoapObservers[iCount].tokenLen);
                pSession->msgType = coapMsgType;
                pSession->code = gContent_c;
                pSession->remotePort = mCoapObservers[iCount].remotePort;
                /* increment sequence id */
                COAP_AddOptionToList(pSession, COAP_OBSERVE_OPTION, (uint8_t*)&sequenceId, sizeof(uint8_t));
                COAP_SendMsg(pSession, pValue, valueLen);
            }
        }
    }
}

/*!*************************************************************************************************
\fn    bool_t COAP_Server_AddObserver()
\brief This function adds a new client in the list of observers.

\param [in]       pSession      pointer to coap session 
\param [in]       pResource     pointer to string resource and string length 


\return       
***************************************************************************************************/
bool_t COAP_Server_AddObserver
(
    coapSession_t* pSession,
    coapUriPath_t* pResource
)
{   
    bool_t retStatus = FALSE;
    uint32_t freeEntry = THR_ALL_FFs8;
    uint32_t iCount;
    
    for (iCount = 0; iCount < COAP_MAX_OBSERVERS; iCount++)
    {   
        /* check if entry is free */
        if (IP_IsAddrEqual(&mCoapObservers[iCount].clientAddr, &in6addr_any))
        {   
            freeEntry = iCount;
        }
        else /* check if client is already subscribed */
        if ((TRUE == FLib_MemCmp(&mCoapObservers[iCount].clientAddr, &pSession->remoteAddr, sizeof(ipAddr_t)))
            && (TRUE == FLib_MemCmp(mCoapObservers[iCount].pResource->pUriPath, pResource->pUriPath, mCoapObservers[iCount].pResource->length))
            && mCoapObservers[iCount].remotePort == pSession->remotePort)
        {
            freeEntry = iCount;
            break;
        }
    }

    if (freeEntry != THR_ALL_FFs8)
    {   
        FLib_MemCpy(&mCoapObservers[freeEntry].clientAddr, &pSession->remoteAddr, sizeof(ipAddr_t));
        mCoapObservers[freeEntry].tokenLen = pSession->tokenLen;
        FLib_MemCpy(&mCoapObservers[freeEntry].token, pSession->aToken, pSession->tokenLen);
        mCoapObservers[freeEntry].pResource = pResource;
        mCoapObservers[freeEntry].remotePort = pSession->remotePort;

        retStatus = TRUE;
    }
    return retStatus;
}

/*!*************************************************************************************************
\fn    bool_t COAP_Server_RemoveObserver()
\brief This function removes a client from the list of observers.

\param [in]       pSession      pointer to coap session 
\param [in]       pResource     pointer to string resource and string length 


\return       
***************************************************************************************************/
bool_t COAP_Server_RemoveObserver
(
    coapSession_t* pSession,
    coapUriPath_t* pResource
)
{   
    bool_t retStatus = FALSE;
    
    for (uint32_t iCount = 0; iCount < COAP_MAX_OBSERVERS; iCount++)
    {   
        /* find client */
        if (IP_IsAddrEqual(&mCoapObservers[iCount].clientAddr, &pSession->remoteAddr)
            && FLib_MemCmp(mCoapObservers[iCount].pResource->pUriPath, pResource->pUriPath, 
            mCoapObservers[iCount].pResource->length))
        {
            
            FLib_MemSet(&mCoapObservers[iCount], 0, sizeof(coapObserverInfo_t));
            retStatus = TRUE;
        }
    }
    return retStatus;
}
#endif

#if COAP_OBSERVE_CLIENT
/*!*************************************************************************************************
\fn    void COAP_Client_StartObserving()
\brief This function send a GET request with Observe option included.

\param [in]       pServerIpAddr   Pointer to IPv6 address of the server
\param [in]       pResource       Pointer to CoAP resource
\param [in]       rcvReplyCb      callback function pointer for receiving the reply

\return       
***************************************************************************************************/
void COAP_Client_StartObserving
(
    ipAddr_t* pServerIpAddr,
    coapUriPath_t* pResource,
    coapCallback_t rcvReplyCb
)
{   
    coapSession_t* pSession = NULL;

    if (pResource != NULL)
    {
        pSession = COAP_OpenSession(mCoapClientInstanceId);
        if (NULL != pSession)
        {
            FLib_MemCpy(&pSession->remoteAddr, pServerIpAddr, sizeof(ipAddr_t));

            if ((THR_IsIpAddrBasedOnMeshPrefix(gInvalidInstanceId_c, &pSession->remoteAddr))
                && (!NWKU_IsIPAddrBasedOnShort(&pSession->remoteAddr)))
            {                 
                /* use as ip source address the ML-EID address */
                (void)THR_GetIP6Addr(0, gMLEIDAddr_c, &pSession->sourceAddr, NULL);                
            }
            pSession->msgType = gCoapConfirmable_c;
            pSession->code = gCoapGET_c;
            pSession->pCallback = rcvReplyCb;

            /* Observer Option - Register -- can have 0 length */
            COAP_AddOptionToList(pSession, COAP_OBSERVE_OPTION, NULL, 0);
            COAP_SetUriPath(pSession, pResource);

            /* Save the token and callback for receiving the notifications */
            COAP_RegisterTokenCallback(pSession, rcvReplyCb);
            
            /* Send GET request with no payload */
            COAP_SendMsg(pSession, NULL, 0);
        }
    }
}
/*!*************************************************************************************************
\fn    void COAP_Client_StopObserving()
\brief This function send a GET request with Observe option included.

\param [in]       pServerIpAddr   Pointer to IPv6 address of the server
\param [in]       pResource       Pointer to CoAP resource
\param [in]       rcvReplyCb      callback function pointer for receiving the reply

\return       
***************************************************************************************************/
void COAP_Client_StopObserving
(
    ipAddr_t* pServerIpAddr,
    coapUriPath_t* pResource,
    coapCallback_t rcvReplyCb
)
{   
    coapSession_t* pSession = NULL;

    if (pResource != NULL)
    {
        pSession = COAP_OpenSession(mCoapClientInstanceId);
        if (NULL != pSession)
        {
            FLib_MemCpy(&pSession->remoteAddr, pServerIpAddr, sizeof(ipAddr_t));

            if ((THR_IsIpAddrBasedOnMeshPrefix(gInvalidInstanceId_c, &pSession->remoteAddr))
                && (!NWKU_IsIPAddrBasedOnShort(&pSession->remoteAddr)))
            {                 
                /* use as ip source address the ML-EID address */
                (void)THR_GetIP6Addr(0, gMLEIDAddr_c, &pSession->sourceAddr, NULL);                
            }
            
            pSession->msgType = gCoapConfirmable_c;
            pSession->code = gCoapGET_c;
            pSession->pCallback = rcvReplyCb;

            COAP_SetUriPath(pSession, pResource);

            COAP_UnregisterTokenCallback(pSession, rcvReplyCb);
            
            /* Send GET request with no payload */
            COAP_SendMsg(pSession, NULL, 0);
        }
    }
}

#endif
/*==================================================================================================
Private functions
==================================================================================================*/
      

/* Delimiters */
/*================================================================================================*/


/*==================================================================================================
Private debug functions
==================================================================================================*/
