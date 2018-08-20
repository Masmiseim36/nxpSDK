/*! *********************************************************************************
 * \defgroup COAP  CoAP Interface
 * @{
 ***********************************************************************************/
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

#ifndef _COAP_H
#define _COAP_H

/*!
\file       coap.h
\brief      This is a header file for the CoAP module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "ip.h"
#include "sockets.h"
#include "session.h"
#include "dtls.h"

/*! Macro used to disable or enable Coap when compiling the Thread library */
#ifndef COAP_ENABLED
    #define COAP_ENABLED                    1
#endif

#if COAP_ENABLED
/*==================================================================================================
Public macros
==================================================================================================*/

/*--------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------
  INFORMATIVE MACROS THAT SHOULD NOT BE CHANGED BY THE USER
  (USED BY THREAD LIBRARY AT COMPILE TIME)
----------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------*/
/*! The maximum length of URI-path options */
#define COAP_MAX_URI_PATH_OPT_SIZE     (254U)

/*! Maximum length of one option.
   URI-path options are not limited by this value if they are added with COAP_SetUriPath() function */
#define COAP_MAX_OPTION_VALUE_SIZE     (20U)

/*! Maximum length of token as defined in RFC 7252 */
#define COAP_MAX_TOKEN_LEN             (8U)

/*! CoAP Option Names */
#define COAP_OBSERVE_OPTION            (6U)
#define COAP_URI_PATH_OPTION           (11U)
#define COAP_CONTENT_FORMAT_OPTION     (12U)
#define COAP_URI_QUERY_OPTION          (15U)
#define COAP_ACCEPT_OPTION             (17U)

#define COAP_TIMER_INTERVAL             200   /* milliseconds */

/*! CoAP Ports */
#define COAP_DEFAULT_PORT              5683
#define COAP_DEFAULT_SECURED_PORT      5684


/*! Number of CoAP instances allowed for one URI-path */
#define COAP_INSTANCES_URI_PATH        (4U)

/*! Set number of retransmissions for a CON message */
#define COAP_SetMaxRetransmitCount(coapInstanceId, maxRetransmitCount)  gCoapInstances[coapInstanceId].coapMaxRetransmit = maxRetransmitCount
/*==================================================================================================
Public type definitions
==================================================================================================*/
struct coapSession_tag;

/*! Return status of a CoAP session */
typedef enum
{
    gCoapSuccess_c,  /*!< CoAP transaction succeeded*/
    gCoapFailure_c,  /*!< Retransmission timer expired and no reply was received */
    gCoapDuplicate_c /*!< A message with same message ID was received in the latest gCoapMaxMsgIds messages */
}coapSessionStatus_t;

/*! Security at MAC layer for CoAP messages. By default, all messages use gCoapMacSecMode1Level5_c*/
typedef enum coapMacSecFlags_tag
{
    gCoapMacSecMode0Level5_c  = BSDS_SET_TX_SEC_FLAGS(0, 5),
    gCoapMacSecMode1Level5_c  = BSDS_SET_TX_SEC_FLAGS(1, 5),
    gCoapMacSecUnsecured_c    = BSDS_SET_TX_SEC_FLAGS(0, 0)
}coapMacSecFlags_t; /*!< MAC security flags */

/*! URI-path structure of a CoAP message */
typedef struct coapUriPath_tag
{
    uint8_t length;     /*!< sizeof URI-path. e.g. for  "/thread/client", uriPathLen = 14*/
    uint8_t* pUriPath;  /*!< pointer to URI-path. URI-path example: "/thread/client" */
}coapUriPath_t;

/*! CoAP callback function prototype for receiving a CoAP message */
typedef void (* coapCallback_t)(coapSessionStatus_t sessionStatus, void* pData, struct coapSession_tag* pSession,
                                    uint32_t dataLen);
/*! This structure keeps the parameters of a CoAP instance */
typedef struct coapInstance_tag
{
    void* pTransport;                  /*!< sockFd or DTLS peer/context(for servers) */
    list_t sessionList;                /*!< list of ongoing CoAP sessions */
    uint16_t port;                     /*!< UDP port number */
    uint16_t coapAckTimeoutMs;         /*!< the minimum spacing before another retransmission in
                                            milliseconds */
    ipIfUniqueId_t ipIfId;             /*!< IP interface unique ID */
    uint8_t coapMaxRetransmit;         /*!< number of retransmissions */
    bool_t usedEntry;                  /*!< TRUE - if entry is populated, FALSE - entry is free */
#if DTLS_ENABLED
    dtlsContext_t * pContext;          /*!< DTLS context */
    void (*event)(dtlsPeerPtr_t pPeer, dtlsAlertLevel_t level, dtlsAlertCode_t code); /*!< Callback
                                                                function for DTLS session status */
#endif
}coapInstance_t;

/*! The URI-path, callback and instance id tuple for associating an incoming message with its
callback */
typedef struct coapCallbackStruct_tag
{
    const coapUriPath_t* pUriPathStruct;  /*!< pointer to URI-path and its length */
    coapCallback_t pCallback;             /*!< pointer to callback function */
    uint8_t coapInstanceId[COAP_INSTANCES_URI_PATH]; /*!< CoAP instance ID array */
}coapCallbackStruct_t;

/*! The token, callback, instance id tuple needed for associating a non-piggybacked message with
its callback */
typedef struct coapTokenCbStruct_tag
{
    uint8_t aToken[COAP_MAX_TOKEN_LEN]; /*!< Token of variable length */
    coapCallback_t pCallback;           /*!< pointer to callback function */
    uint8_t coapInstanceId;             /*!< CoAP instance ID */
    uint8_t tokenLen;                   /*!< The length of the token */
}coapTokenCbStruct_t;

/*! CoAP option structure */
typedef struct coapOptionDetails_tag
{
    uint8_t optName;                    /*!< The ID of the CoAP option */
    uint8_t optValueLen;                /*!< Length in bytes of the option value */
    uint8_t optValue[COAP_MAX_OPTION_VALUE_SIZE]; /*!< Option value */
}coapOptionDetails_t;

/*! This enum is meant to compress the most used message type and code combinations in one constant */
typedef enum
{
    gCoapMsgTypeConPost_c              = 1, /*!< CON POST message */
    gCoapMsgTypeNonPost_c              = 2, /*!< NON POST message */
    gCoapMsgTypeAckSuccessChanged_c    = 3, /*!< ACK Success Changed message */
    gCoapMsgTypeAckSuccessContent_c    = 4, /*!< ACK Success Content message */
    gCoapMsgTypeConGet_c               = 5, /*!< CON GET message */
    gCoapMsgTypeNonGet_c               = 6, /*!< NON GET message */
    gCoapMsgTypeEmptyAck_c             = 7, /*!< ACK Empty message */
    gCoapMsgTypeUseSessionValues_c     = 8  /*!< Use the (msgType, code) values set in the session */
}coapMsgTypesAndCodes_t;

/*! CoAP message types */
typedef enum
{
    gCoapConfirmable_c          = 0,
    gCoapNonConfirmable_c       = 1,
    gCoapAcknowledgement_c      = 2,
    gCoapReset_c                = 3
}coapMessageTypes_t;

/*! CoAP Method and Response Codes */
typedef enum
{
    /* Method Codes --- Used in Request messages */
    gCoapGET_c                     = 001,
    gCoapPOST_c                    = 002,
    gCoapPUT_c                     = 003,
    gCoapDELETE_c                  = 004,

    /* Response Codes --- Used in Response messages */
    gEmpty_c                       = 000,
    gCreated_c                     = 201,
    gDeleted_c                     = 202,
    gValid_c                       = 203,
    gChanged_c                     = 204,
    gContent_c                     = 205,
    gBadRequest_c                  = 400,
    gUnauthorized_c                = 401,
    gBadOption_c                   = 402,
    gForbidden_c                   = 403,
    gNotFound_c                    = 404,
    gMethodNotAllowed_c            = 405,
    gNotAcceptable_c               = 406,
    gPreconditionFailed_c          = 412,
    gRequestEntityTooLarge_c       = 413,
    gUnsupportedContentFormat_c    = 415,
    gInternalServerError_c         = 500,
    gNotImplemented_c              = 501,
    gBadGateway_c                  = 502,
    gServiceUnavailable_c          = 503,
    gGatewayTimeout_c              = 504,
    gProxyingNotSupported_c        = 505
}coapReqRespCodes_t;

typedef  coapOptionDetails_t coapOption_t;

/*! A CoAP session keeps all the information necessary for a CoAP message exchange */
typedef struct coapSession_tag
{
    ipAddr_t remoteAddr;                /*!< The destination IP address */
    ipAddr_t sourceAddr;                /*!< The source IP address */

    coapOption_t* pTxOptionList;        /*!< Options to be included in an outgoing CoAP msg */
    coapOption_t* pRxOptionList;        /*!< Options received in the incoming CoAP msg */

    coapCallback_t pCallback;           /*!< Pointer to callback function */

    uint8_t aToken[COAP_MAX_TOKEN_LEN]; /*!< Token of variable length */
    coapUriPath_t* pUriPath;            /*!< Pointer to URI-Path structure */

    uint32_t macTxFlags;                /*!< Sets the security level and the key ID mode for MAC layer
                                            encryption. This value is by default set to key ID mode
                                            1 and encryption level 5 and should not be modified */
    uint16_t messageID;                 /*!< Message ID is a random number generated by CoAP module*/
    uint16_t remotePort;                /*!< Source port*/

    ipIfUniqueId_t ipIfId;              /*!< IP interface id */
    uint8_t coapInstId;                 /*!< CoAP instance ID */
    uint8_t tokenLen;                   /*!< The length of the token */
    bool_t isDtlsSecured;               /*!< A flag indicating if the message uses a secure
                                            connection (DTLS) or not */
    bool_t autoClose;                   /*!< Set this flag to FALSE if the coap session should not
                                             be automatically closed when receiving the ACK (on the
                                             requester) or sending the ACK (on the server)*/

    bool_t bIsSubscribed;               /*!< On client side keeps record if the server successfully
                                             registered the client as observer and pass this parameter to application.
                                             On server side, application checks this parameter to see if the client asked for subscription
                                             and depending on the server availability responds positive or not.*/
    uint8_t observeOption;             /*!< Value of the observe option. LSB 24 bits keep the
                                             sequence id */

    coapMessageTypes_t msgType;         /*!< CoAP message types are: CON, NON, ACK, RESET*/
    coapReqRespCodes_t code;            /*!< Depending on the message type (Request or Response) */

    uint8_t lastHopLQI;                 /*!< LQI of last hop, if the message is multi hop */
    uint8_t hopLimit;                   /*!< Hop limt to use when sending a COAP packet */
    uint8_t ipQos;                      /*!< Ip packet Quality of service -> DSCP field */
}coapSession_t;

/*! Parameters needed for creating a secured CoAP instance over DTLS */
typedef struct coapStartSecParams_tag
{
    sockaddrStorage_t* pServerAddr;     /*!< DTLS Server's IP address */
    sockaddrStorage_t* pLocalAddr;      /*!< My local IP address */
    uint32_t retransmitTimeUnits;       /*!< Number of time units to retransmit a packet */
#if DTLS_ENABLED
    void (*event)(dtlsPeerPtr_t pPeer, dtlsAlertLevel_t level, dtlsAlertCode_t code); /*!< callback to signal an alert event */
    void (*getJpakePasswd)(dtlsPeerPtr_t pPeer, uint32_t *pSecretLen, uint8_t **ppSecret); /*!< callback to get to passphrase for the peer */
    int32_t  (*fpRelayTx)(int32_t sockfd, void *pMsg, uint32_t msgLen, uint32_t flags,
        sockaddrStorage_t *to, socklen_t toLen); /*!<Function used to set a TX function for a peer. This will be used to send dtls records,
        instead of UDP.*/
    bool_t (*fpCheckClient)(ipAddr_t *pIpAddr); /*!< server function used to check a client */
#endif
    uint8_t maxRetransmitCnt;           /*!< Number of message retransmissions */
}coapStartSecParams_t;

/*! Parameters needed for creating an unsecured CoAP instance over UDP */
typedef struct coapStartUnsecParams_tag
{
    uint16_t port;          /*!< UDP port */
    uint8_t addrFamily;     /*!< address family (AF_INET and AF_INET6 supported)*/
}coapStartUnsecParams_t;

/*! The callback, URI-path tuple for associating an incoming message with its callback function */
typedef struct coapRegCbParams_tag
{
    coapCallback_t pCallback;           /*!< pointer to the callback function */
    coapUriPath_t* pUriPathStruct;       /*!< pointer to URI-path and its length */
}coapRegCbParams_t;

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
\fn     void COAP_Init(taskMsgQueue_t *pTaskMsgQueue)
\brief  This function initializes the CoAP module.

\param  [in]   pTaskMsgQueue     Pointer to message task queue.
***************************************************************************************************/
void COAP_Init(taskMsgQueue_t *pTaskMsgQueue);

/*!*************************************************************************************************
\fn     uint8_t COAP_CreateInstance(coapStartSecParams_t* pCoapStartSecParams,
                                    coapStartUnsecParams_t *pCoapStartUnsecParams, ipIfUniqueId_t ipIfId,
                                    coapRegCbParams_t* pCallbacksStruct, uint32_t nbOfCallbacks)
\brief  This function opens a secure or unsecured Coap instance.

\param  [in]   pCoapStartSecParams   Pointer to initialization structure for a secure transmission
                                     over DTLS.
\param  [in]   pCoapStartUnsecParams Pointer to initialization structure for an unsecured transmission
                                    over sockets.
\param  [in]   ipIfUniqueId          ip interface ID.
\param  [in]   pCallbacksStruct      Pointer to callbacks registered for that instance.
\param  [in]   nbOfCallbacks         Number of registered callbacks.

\return        uint8_t               CoAP instance id.
***************************************************************************************************/
uint8_t COAP_CreateInstance(coapStartSecParams_t* pCoapStartSecParams,
                            coapStartUnsecParams_t *pCoapStartUnsecParams, ipIfUniqueId_t ipIfId,
                            coapRegCbParams_t* pCallbacksStruct, uint32_t nbOfCallbacks);

/*!*************************************************************************************************
\fn     bool_t COAP_CloseInstance(uint8_t coapInstanceId)
\brief  This function closes a CoAP instance. Make sure that no other module uses the same
        instance.

\param  [in]   coapInstanceId     CoAP instance

\return        bool_t            TRUE - if the closing succeeded
                                 FALSE - otherwise
***************************************************************************************************/
bool_t COAP_CloseInstance(uint8_t coapInstanceId);

/*!*************************************************************************************************
\fn     coapSession_t* COAP_OpenSession(uint8_t coapInstanceId)
\brief  This function opens a CoAP session for a specific instance. A session is identified by the
        message ID of the message.

\param  [in]   coapInstanceId    CoAP instance Id.

\return        coapSession_t*    Pointer to CoAP session.
***************************************************************************************************/
coapSession_t* COAP_OpenSession(uint8_t coapInstanceId);

/*!*************************************************************************************************
\fn     void COAP_CloseSession(coapSession_t* pSession)
\brief  This function deletes a CoAP session when completed. This function must be called when a
        response message was received (in the case of the client/initiator), or when a response
        message is sent (in the case of the server)

\param  [in]   pSession  Pointer to CoAP session to be deleted.
***************************************************************************************************/
void COAP_CloseSession(coapSession_t* pSession);

/*!*************************************************************************************************
\fn     void COAP_AddOptionToList(coapSession_t* pSession, uint8_t optName,
                                  uint8_t* optValue,uint8_t optValueLen)
\brief  This function adds the options named by application to a list.

\param  [in]  pSession       Pointer to CoAP session.
\param  [in]  optName        The name of the uri-path.
\param  [in]  optValue       The value of the option.
\param  [in]  optValueLen    The length of the option value.
***************************************************************************************************/
void COAP_AddOptionToList(coapSession_t* pSession, uint8_t optName, uint8_t* optValue,uint8_t optValueLen);

/*!*************************************************************************************************
\fn     void COAP_SetUriPath(coapSession_t *pSess, coapUriPath_t *pUriPath)
\brief  This function adds the URI-paths to the option list.

\param  [in]     pSession        Pointer to CoAP session.
\param  [in]     pUriPath        Pointer to URI-path.
***************************************************************************************************/
void COAP_SetUriPath(coapSession_t *pSess, coapUriPath_t *pUriPath);

/*!*************************************************************************************************
\fn     nwkStatus_t COAP_Send(coapSession_t*  pSession, coapMsgTypesAndCodes_t coapMsgType,
                              void* pData, uint32_t payloadLen)
\brief  This function builds and transmits a CoAP message. This function shall be used when sending
        one of the predefined messages. For a custom build message please use COAP_SendMsg()
        function.

\param  [in]     pSession        Pointer to CoAP session.
\param  [in]     coapMsgType     CoAP message type. Is one from the list coapMsgTypesAndCodes_t.
\param  [in]     pData           Pointer to data payload.
\param  [in]     payloadLen      Payload length.

\return         nwkStatus_t     Status of the operation.
***************************************************************************************************/
nwkStatus_t COAP_Send(coapSession_t*  pSession, coapMsgTypesAndCodes_t coapMsgType,
                      void* pData, uint32_t payloadLen);

/*!*************************************************************************************************
\fn     nwkStatus_t COAP_SendMsg(coapSession_t*  pSession,void* pData,uint32_t payloadLen)
\brief  This function builds and transmits a CoAP message.

\param  [in]     pSession        Pointer to CoAP session.
\param  [in]     pData           Pointer to data payload.
\param  [in]     payloadLen      Payload length.

\return         nwkStatus_t     Status of the operation.
***************************************************************************************************/
nwkStatus_t COAP_SendMsg(coapSession_t*  pSession,void* pData,uint32_t payloadLen);


/*!*************************************************************************************************
\fn     nwkStatus_t COAP_RegisterResourceCallback(uint8_t coapInstanceId, coapRegCbParams_t* pCallbacksStruct,
                                                  uint32_t nbOfCallbacks)
\brief  This function registers a callback for a given uri-path name.

\param  [in]  coapInstanceId  CoAP instance.
\param  [in]  pCallbacksStruct      Pointer to callback functions array.
\param  [in]  nbOfCallbacks  Number of callbacks.

\return       nwkStatus_t - Success if registering succeeded
                          - Fail if table is full
***************************************************************************************************/
nwkStatus_t COAP_RegisterResourceCallback(uint8_t coapInstanceId, coapRegCbParams_t* pCallbacksStruct,
                                          uint32_t nbOfCallbacks);

/*!*************************************************************************************************
\fn     void COAP_RegisterTokenCallback(coapSession_t* pSession, coapCallback_t pCallback)
\brief  This function registers a callback for a given token, for non-piggybacked responses. The
        client calls this function when it expects another message with the same token.

\param  [in]  pSession       Pointer to CoAP session.
\param  [in]  pCallback      Pointer to callback function.
***************************************************************************************************/
void COAP_RegisterTokenCallback(coapSession_t* pSession, coapCallback_t pCallback);

/*!*************************************************************************************************
\fn     bool_t COAP_UnregisterTokenCallback(coapSession_t* pSession, coapCallback_t pCallback)
\brief  This function unregisters a callback for a given token.

\param  [in]  pSession       Pointer to CoAP session.
\param  [in]  pCallback      Pointer to callback function.

\return       bool_t         TRUE - if the unregister succeeded
                             FALSE - otherwise
***************************************************************************************************/
bool_t COAP_UnregisterTokenCallback(coapSession_t* pSession, coapCallback_t pCallback);

/*!*************************************************************************************************
\fn     bool_t COAP_UnregisterResourceCallback(uint8_t coapInstanceId, coapRegCbParams_t* pCallbacksStruct,
                                               uint32_t nbOfCallbacks)
\brief  This function unregisters a callback for a given uri-path name.

\param  [in]  coapInstanceId CoAP instance.
\param  [in]  pCallback      Pointer to callback functions array.
\param  [in]  nbOfCallbacks  Number of callbacks.

\return       bool_t         TRUE - if the unregister succeeded
                             FALSE - otherwise
***************************************************************************************************/
bool_t COAP_UnregisterResourceCallback(uint8_t coapInstanceId,
                                       coapRegCbParams_t* pCallbacksStruct, uint32_t nbOfCallbacks);
/*!*************************************************************************************************
\fn     void COAP_CloseAnySession(void)
\brief  This function close any sessions for  CoAP module.

***************************************************************************************************/
void COAP_CloseAnySession(void);

#if DTLS_ENABLED
/*!*************************************************************************************************
\fn     void* COAP_GetContextByInstanceId(uint8_t coapInstanceId)
\brief  This function returns the pointer to DTLS context from a CoAP instance.

\param  [in]  coapInstanceId          CoAP instance ID

\return       void*                   pointer to DTLS context
***************************************************************************************************/
void* COAP_GetContextByInstanceId(uint8_t coapInstanceId);
#endif

#endif /* COAP_ENABLED */
#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _COAP_H */

/*!
** @}
*/
