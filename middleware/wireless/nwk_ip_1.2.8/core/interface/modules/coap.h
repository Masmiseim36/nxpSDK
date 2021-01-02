/*! *********************************************************************************
 * \defgroup COAP  CoAP Interface
 * @{
 ***********************************************************************************/
/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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

/*! The maximum memory size that can be allocated to keep payload data */
#define COAP_MAX_MEMORY_SIZE           (1200U)

/*! The maximum length of URI-path options */
#define COAP_MAX_URI_PATH_OPT_SIZE     (254U)

/*! Maximum length of a block option */
#define COAP_MAX_BLOCK_VALUE_SIZE      (3U)


/*! Maximum length of token as defined in RFC 7252 */
#define COAP_MAX_TOKEN_LEN             (8U)

/*! CoAP Option Names */
#define COAP_IF_MATCH_OPTION           (1U)
#define COAP_URI_HOST_OPTION           (3U)
#define COAP_IF_NONE_MATCH_OPTION      (5U)
#define COAP_OBSERVE_OPTION            (6U)
#define COAP_URI_PORT_OPTION           (7U)
#define COAP_LOCATION_PATH_OPTION      (8U)
#define COAP_URI_PATH_OPTION           (11U)
#define COAP_CONTENT_FORMAT_OPTION     (12U)
#define COAP_MAX_AGE_OPTION            (14U)
#define COAP_URI_QUERY_OPTION          (15U)
#define COAP_ACCEPT_OPTION             (17U)
#define COAP_LOCATION_QUERY_OPTION     (20U)
#define COAP_BLOCK2_OPTION             (23U)
#define COAP_BLOCK1_OPTION             (27U)
#define COAP_PROXY_URI_OPTION          (35U)
#define COAP_PROXY_SCHEME_OPTION       (39U)

#define COAP_TIMER_INTERVAL             500   /* milliseconds */
#define COAP_BLOCK_TIMER_INTERVAL_MS   3000   /* milliseconds */

/*! CoAP Ports */
#define COAP_DEFAULT_PORT              5683
#define COAP_DEFAULT_SECURED_PORT      5684

#define COAP_DEFAULT_LEISURE           (5U)   /* seconds */

/*! Number of CoAP instances allowed for one URI-path */
#define COAP_INSTANCES_URI_PATH        (4U)

#define COAP_INVALID_SESSION_ID         (0xFF)

#define COAP_INVALID_INSTANCE           (0xFF)

/*! interop usage, replace with final IANA allocated value once allocated */
#define COAP_CONTENT_TYPE_AUDIT_NONCE   (60)
#define COAP_CONTENT_TYPE_CSRATTRS      (285)
#define COAP_CONTENT_TYPE_PKCS10        (286)

/*! Set number of retransmissions for a CON message */
#define COAP_SetMaxRetransmitCount(pSession, maxRetransmitCount)  ((pSession)->coapMaxRetransmit = (maxRetransmitCount))
#define COAP_SetInitialAckTimeoutMs(pSession, initialAckTimeoutMs)((pSession)->coapAckTimeoutMs = (initialAckTimeoutMs))

/*! Do not close CoAP session */
#define COAP_KeepSessionOpen(pSession) pSession->autoClose = FALSE

/*! RFC 7252, Section 8.2: The server SHOULD pick a random point of time within the chosen leisure
period to send back the unicast response to the multicast request */
extern uint8_t gCoapLeisure;
#define COAP_SetLeisureForResponseToMulticast(seconds) (gCoapLeisure = seconds)

/*! Allow Block-wise transfer */
#define COAP_AllowBlockWiseTransfer(pSession) ((pSession)->allowBlockWise = TRUE)
/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef struct coapSession_tag coapSession_t;

typedef enum
{
    gCoapFinishedTransfer_c,
    gCoapFinishedBlockTransfer_c,
    gCoapAskForNextBlock_c,
    gCoapSendNextBlock_c,
    gCoapSendLastBlock_c,
    gCoapMemAllocErr_c,
    gCoapSendReset_c,
    gCoapSendNotFound_c,
    gCoapSendBadOption_c
} coapParseOptionsStatus_t;

/*! Return status of a CoAP session */
typedef enum
{
    gCoapSuccess_c,  /*!< CoAP transaction succeeded*/
    gCoapFailure_c,  /*!< Retransmission timer expired and no reply was received */
    gCoapClose_c,    /*!< The CoAP instance has been closed */
    gCoapDuplicate_c,/*!< A message with same message ID was received in the latest gCoapMaxMsgIds messages */
    gCoapRequestNextBlock_c /* The CoAP transfer is not yet finished, and the application needs to request the next block */
}coapSessionStatus_t;

/*! Security at MAC layer for CoAP messages. By default, all messages use gCoapMacSecMode1Level5_c*/
typedef enum coapMacSecFlags_tag
{
    gCoapMacSecMode0Level5_c  = BSDS_SET_TX_SEC_FLAGS(0, 5),
    gCoapMacSecMode1Level5_c  = BSDS_SET_TX_SEC_FLAGS(1, 5),
    gCoapMacSecUnsecured_c    = BSDS_SET_TX_SEC_FLAGS(0, 0)
}coapMacSecFlags_t; /*!< MAC security flags */

/*! CoAP session type - not set, RX or TX*/
typedef enum coapSessionType_tag
{
    gCoapSessionTypeNotSet_c  = 0x00, /*!< CoAP Session type not set */
    gCoapSessionTypeRx_c,             /*!< CoAP Session used for RX */
    gCoapSessionTypeTx_c              /*!< CoAP Session used for TX */
}coapSessionType_t;

/*! URI-path structure of a CoAP message */
typedef struct coapUriPath_tag
{
    uint8_t length;     /*!< sizeof URI-path. e.g. for  "/thread/client", uriPathLen = 14*/
    uint8_t* pUriPath;  /*!< pointer to URI-path. URI-path example: "/thread/client" */
}coapUriPath_t;

/*! CoAP callback function prototype for receiving a CoAP message */
typedef void (* coapCallback_t)(coapSessionStatus_t sessionStatus, uint8_t* pData, coapSession_t* pSession,
                                    uint32_t dataLen);
/*! This structure keeps the parameters of a CoAP instance */
typedef struct coapInstance_tag
{
    void* pTransport;                  /*!< sockFd or DTLS peer/context(for servers) */
    list_t sessionList;                /*!< list of ongoing CoAP sessions */
    ipIfUniqueId_t ipIfId;             /*!< IP interface unique ID */
    bool_t usedEntry;                  /*!< TRUE - if entry is populated, FALSE - entry is free */
#if DTLS_ENABLED
    void (*event)(void *pCtx, uint8_t level, uint8_t code); /*!< Callback
                                                                function for DTLS session status */
#endif
    void *pSecContext;                 /*!< DTLS context */
}coapInstance_t;

/*! The URI-path, callback and instance id tuple for associating an incoming message with its
callback */
typedef struct coapCallbackStruct_tag
{
    coapUriPath_t* pUriPathStruct;  /*!< pointer to URI-path and its length */
    coapCallback_t pCallback;       /*!< pointer to callback function */
    uint8_t coapInstanceId[COAP_INSTANCES_URI_PATH]; /*!< CoAP instance ID array */
    uint8_t* pUserContext;         /*!< pointer to callback function */
}coapCallbackStruct_t;

/*! The token, callback, instance id tuple needed for associating a non-piggybacked message with
its callback */
typedef struct coapTokenCbStruct_tag
{
    uint8_t aToken[COAP_MAX_TOKEN_LEN]; /*!< Token of variable length */
    coapCallback_t pCallback;           /*!< pointer to callback function */
    uint8_t coapInstanceId;             /*!< CoAP instance ID */
    uint8_t tokenLen;                   /*!< The length of the token */
    uint8_t* pUserContext;              /*!< pointer to callback function */
}coapTokenCbStruct_t;

/*! CoAP option structure */
typedef struct coapOptionDetails_tag
{
    uint8_t optName;                    /*!< The ID of the CoAP option */
    uint8_t optValueLen;                /*!< Length in bytes of the option value */
    uint8_t optValue[];                 /*!< Option value */
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
    gContinue_c                    = 231,
    gBadRequest_c                  = 400,
    gUnauthorized_c                = 401,
    gBadOption_c                   = 402,
    gForbidden_c                   = 403,
    gNotFound_c                    = 404,
    gMethodNotAllowed_c            = 405,
    gNotAcceptable_c               = 406,
    gRequestEntityIncomplete       = 408,
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
struct coapSession_tag
{
    sockaddrStorage_t remoteAddrStorage;/*!< The remote IP address storage */
    uint16_t contentFormat;             /*!< Content format used by the exchange */
    uint16_t acceptOption;              /*!< Accept option value if present in request */

    ipAddr_t localAddr;                 /*!< The source IP address */
    uint8_t aToken[COAP_MAX_TOKEN_LEN]; /*!< Token of variable length */
    coapCallback_t pCallback;           /*!< Pointer to callback function */
    coapUriPath_t* pUriPath;            /*!< Pointer to URI-Path structure */

    list_t pTxOptionList;               /*!< Options to be included in an outgoing CoAP msg */
    list_t pRxOptionList;               /*!< Options received in the incoming CoAP msg */
    uint8_t *pData;                     /*!< The full application payload - necessary for block transfers */
    uint32_t pLen;                      /*!< The length of the payload */

    uint32_t pBufferSz;                 /*!< The size of the pData buffer if allocated by the application */
    uint8_t* pUserContext;              /*!< Reference to a data structure used by pCallback */
    uint16_t messageID;                 /*!< Message ID is a random number generated by CoAP module*/
    uint8_t sessionType;                /*!< CoAP session type set at RX or TX, used as resource mutex for session */
    uint8_t reserved;
    uint32_t coapAckTimeoutMs;          /*!< The minimum spacing before another retransmission in
                                             milliseconds */
    uint32_t blockNum;                  /*!< Block number being currently transferred */
    bool_t usesBlock1;                  /*!< TRUE - if current session uses Block1 transfer */
    bool_t usesBlock2;                  /*!< TRUE - if current session uses Block2 transfer */
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
    uint8_t observeOption;              /*!< Value of the observe option. LSB 24 bits keep the
                                             sequence id */
    coapMessageTypes_t msgType;         /*!< CoAP message types are: CON, NON, ACK, RESET*/
    coapReqRespCodes_t code;            /*!< Depending on the message type (Request or Response) */
    uint8_t lastHopLQI;                 /*!< LQI of last hop, if the message is multi hop */
    uint8_t hopLimit;                   /*!< Hop limt to use when sending a COAP packet */
    uint8_t ipQos;                      /*!< Ip packet Quality of service -> DSCP field */
    uint8_t coapMaxRetransmit;          /*!< number of retransmissions */
    bool_t  allowBlockWise;             /*!< TRUE - if CoAP payload is larger than COAP_BLOCK_SIZE fragment it in blocks, FALSE - otherwise */
};

/*! Parameters needed for creating a secured CoAP instance over DTLS */
typedef struct coapStartSecParams_tag
{
    sockaddrStorage_t* pServerAddr;     /*!< DTLS Server's IP address */
    sockaddrStorage_t* pLocalAddr;      /*!< My local IP address */
    uint32_t retransmitTimeUnits;       /*!< Number of time units to retransmit a packet */
    uint8_t maxRetransmitCnt;           /*!< Number of message retransmissions */
#if DTLS_ENABLED
    void (*event)
        (void *pCtx,
         uint8_t level,
         uint8_t code);                 /*!< Callback to signal an alert event */
    void (*getJpakePasswd)
        (void *pPeer,
         uint32_t *pSecretLen,
         uint8_t **ppSecret);           /*!< Callback to get to passphrase for the peer (client and
                                             server side)
                                             TBD: use pfPskCb also for JPAKE */
    bool_t (*pfCheckClient)
        (ipAddr_t *pIpAddr);            /*!< Server function used to check a client */
#endif /* DTLS_ENABLED */
}coapStartSecParams_t;

/*! The callback, URI-path tuple for associating an incoming message with its callback function */
typedef struct coapRegCbParams_tag
{
    coapCallback_t pCallback;           /*!< pointer to the callback function */
    coapUriPath_t* pUriPathStruct;       /*!< pointer to URI-path and its length */
}coapRegCbParams_t;

/*! The fields of a CoAP block: NUM, MORE, SZX */
typedef struct coapBlock_tag
{
    uint32_t num;   /*!< the number of the block - can be up to 20 bits long */
    uint8_t more;       /*!< the more bit - 1 if more blocks follow, 0 if last block */
    uint8_t szx;        /*!< the size of the block - between 2 and 6 - actual size is 2**(szx+4) */
}coapBlock_t;

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
                                    sockaddrStorage_t *pCoapStartUnsecParams, ipIfUniqueId_t ipIfId,
                                    coapRegCbParams_t* pCallbacksStruct, uint32_t nbOfCallbacks)
\brief  This function opens a secure or unsecured Coap instance.

\param  [in]   pCoapStartSecParams   Pointer to initialization structure for a secure transmission
                                     over DTLS.
\param  [in]   pCoapStartUnsecParams Pointer to initialization structure for an unsecured transmission
                                    over sockets.
\param  [in]   pCallbacksStruct      Pointer to callbacks registered for that instance.
\param  [in]   nbOfCallbacks         Number of registered callbacks.

\return        uint8_t               CoAP instance id.
***************************************************************************************************/
uint8_t COAP_CreateInstance(coapStartSecParams_t* pCoapStartSecParams,
                            sockaddrStorage_t* pCoapStartUnsecParams,
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
\fn     nwkStatus_t COAP_AddOptionToList(coapSession_t* pSession, uint8_t optName,
                                  uint8_t* optValue,uint8_t optValueLen)
\brief  This function adds the options named by application to a list.

\param  [in]  pSession       Pointer to CoAP session.
\param  [in]  optName        The name of the uri-path.
\param  [in]  optValue       The value of the option.
\param  [in]  optValueLen    The length of the option value.

\return         nwkStatus_t     Status of the operation.
***************************************************************************************************/
nwkStatus_t COAP_AddOptionToList(coapSession_t* pSession, uint8_t optName, uint8_t* optValue,uint8_t optValueLen);

/*!*************************************************************************************************
\fn     void COAP_SetUriPath(coapSession_t *pSess, coapUriPath_t *pUriPath)
\brief  This function adds the URI-paths to the option list.

\param  [in]     pSession        Pointer to CoAP session.
\param  [in]     pUriPath        Pointer to URI-path.
***************************************************************************************************/
void COAP_SetUriPath(coapSession_t *pSess, coapUriPath_t *pUriPath);

/*!*************************************************************************************************
\fn     void COAP_SetCallback(coapSession_t *pSession, coapCallback_t pCallback)
\brief  This function sets the callback for CoAP message.

\param [in]     pSession        Pointer to CoAP session.
\param [in]     pCallback       Callback function.

\return      none
***************************************************************************************************/
void COAP_SetCallback(coapSession_t *pSession, coapCallback_t pCallback);

/*!*************************************************************************************************
\fn     nwkStatus_t COAP_Send(coapSession_t*  pSession, coapMsgTypesAndCodes_t coapMsgType,
                              uint8_t* pData, uint32_t payloadLen)
\brief  This function builds and transmits a CoAP message.

\param  [in]     pSession        Pointer to CoAP session.
\param  [in]     coapMsgType     CoAP message type. Is one from the list coapMsgTypesAndCodes_t.
\param  [in]     pData           Pointer to data payload.
\param  [in]     payloadLen      Payload length.

\return         nwkStatus_t     Status of the operation.
***************************************************************************************************/
nwkStatus_t COAP_Send(coapSession_t*  pSession, coapMsgTypesAndCodes_t coapMsgType,
                      uint8_t* pData, uint32_t payloadLen);

/*!*************************************************************************************************
\fn     nwkStatus_t COAP_SendBlock(coapSession_t*  pSession, uint8_t* pNextBlock,
                                        uint32_t dataLen, bool_t bIsLastBlock)
\brief  This function sends a CoAP message using blockwise transfer.
        Application handles the transmission of each block.

\param [in]     pSession        Pointer to CoAP session.
\param [in]     pNextBlock      Pointer to the payload of the block.
\param [in]     dataLen         Payload length. It must be less than or at most equal to COAP_BLOCK_SIZE.
                                Otherwise it will be truncated to COAP_BLOCK_SIZE.
\param [in]     bIsLastBlock    TRUE - if this is the last block of the transfer, FALSE - otherwise

\return         nwkStatus_t     Status
***************************************************************************************************/
nwkStatus_t COAP_SendBlock(coapSession_t* pSession, uint8_t* pNextBlock, uint32_t dataLen,
                                        bool_t bIsLastBlock);

/*!*************************************************************************************************
\fn     nwkStatus_t COAP_RequestNextBlock(coapSession_t*  pSession)
\brief  This function sends a CoAP message, requesting the next block, in a block-wise transfer.
        Application handles the transmission of each block.

\param [in]     pSession        Pointer to CoAP session.

\return         nwkStatus_t     Status
***************************************************************************************************/
nwkStatus_t COAP_RequestNextBlock(coapSession_t* pSession);

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
\fn     nwkStatus_t COAP_RegisterTokenCallback(coapSession_t* pSession, coapCallback_t pCallback)
\brief  This function registers a callback for a given token, for non-piggybacked responses. The
        client calls this function when it expects another message with the same token.

\param  [in]  pSession       Pointer to CoAP session.
\param  [in]  pCallback      Pointer to callback function.

\return       nwkStatus_t - Success if registering succeeded
                          - Fail if table is full
***************************************************************************************************/
nwkStatus_t COAP_RegisterTokenCallback(coapSession_t* pSession, coapCallback_t pCallback);

/*!*************************************************************************************************
\fn     bool_t COAP_UnregisterTokenCallback(coapSession_t* pSession, coapCallback_t pCallback)
\brief  This function unregisters a callback for a given token.

\param [in]  coapInstId       CoAP instance ID.
\param [in]  tokenLen         Length of token in bytes.
\param [in]  pToken           Pointer to token array.
\param [in]  pCallback        Pointer to callback function..

\return       bool_t         TRUE - if the unregister succeeded
                             FALSE - otherwise
***************************************************************************************************/
bool_t COAP_UnregisterTokenCallback(uint8_t coapInstId, uint8_t tokenLen,uint8_t* pToken,
                                                    coapCallback_t pCallback);

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

/*!*************************************************************************************************
\fn     nwkStatus_t COAP_CancelRetransmissions(coapSession_t *pSession)
\brief  This function cancels all the following retransmissions and closes the CoAP session.

\param [in]  pSession       Pointer to CoAP session.

\return         nwkStatus_t     Status
***************************************************************************************************/
nwkStatus_t COAP_CancelRetransmissions(coapSession_t *pSession);

/*!*************************************************************************************************
\fn     uint8_t COAP_GetSessionId(coapSession_t *pSession)
\brief  This function returns an unique identifier of the CoAP session.

\param [in]  pSession       Pointer to CoAP session.

\return      uint8_t        Id of the session
***************************************************************************************************/
uint8_t COAP_GetSessionId(coapSession_t *pSession);
/*!*************************************************************************************************
\fn     coapSession_t* COAP_GetSessionById(coapSession_t *pSession)
\brief  This function returns an unique identifier of the CoAP session.

\param [in]  sessionId     Id of the session

\return      pSession      Pointer to CoAP session.
***************************************************************************************************/
coapSession_t* COAP_GetSessionById(uint8_t sessionId);

/*!*************************************************************************************************
\fn     bool_t COAP_CmpUriPaths(const coapUriPath_t *uriPath1, const coapUriPath_t *uriPath2);
\brief  This function compare two COAP URI Paths

\param [in]  uriPath1      Pointer to a COAP Uri Path
\param [in]  uriPath2      Pointer to a COAP Uri Path

\return      bool_t        TRUE if the Uri Paths are Equal
***************************************************************************************************/
bool_t COAP_CmpUriPaths(const coapUriPath_t *uriPath1, const coapUriPath_t *uriPath2);

/*!*************************************************************************************************
\fn     ipIfUniqueId_t COAP_GetIpIfIdByInstId(uint8_t coapInstId)
\brief  This function returns the IP interface ID of a given COAP instance ID.

\param [in]   coapInstId          COAP instance ID

\return       ipIfUniqueId_t      IP interface ID
***************************************************************************************************/
ipIfUniqueId_t COAP_GetIpIfIdByInstId(uint8_t coapInstId);

/*!*************************************************************************************************
\fn     bool_t COAP_IsInstanceSecured(uint8_t coapInstanceId)
\brief  This function tells if a CoAP instance is secured or not.

\param  [in]  coapInstanceId          CoAP instance ID
\return       bool_t                  TRUE - if instance is secured, FALSE - otherwise
***************************************************************************************************/
bool_t COAP_IsInstanceSecured(uint8_t coapInstanceId);

#if DTLS_ENABLED
/*!*************************************************************************************************
\fn     void* COAP_GetContextByInstanceId(uint8_t coapInstanceId)
\brief  This function returns the pointer to DTLS context from a CoAP instance.

\param  [in]  coapInstanceId          CoAP instance ID

\return       void*                   pointer to DTLS context
***************************************************************************************************/
void* COAP_GetContextByInstanceId(uint8_t coapInstanceId);

/*!*************************************************************************************************
\private
\fn    uint8_t COAP_GetClientInstanceByPeer(bool_t secured, sockaddrStorage_t *pPeerSocket)
\brief  This function returns the COAP instanceID for a given peer

\param [in]   secured          Search for a secured Instance
\param [in]   pPeerSocket      Peer description

\return       coapInstance_t*     pointer to CoAP instance
***************************************************************************************************/
uint8_t COAP_GetClientInstanceByPeer(bool_t secured, sockaddrStorage_t *pPeerSocket);

#endif

#endif /* COAP_ENABLED */
#ifdef __cplusplus
}
#endif

/*!*************************************************************************************************
\fn     void * COAP_GetTransportByInstId(uint8_t coapInstId)
\brief  This function returns the pointer to socket or peer for a given COAP instance ID.

\param [in]   coapInstId          COAP instance ID

\return       void*     pointer to socket or peer
***************************************************************************************************/
void *COAP_GetTransportByInstId(uint8_t coapInstId);

/*!*************************************************************************************************
\fn    uint32_t COAP_EncodeUintOptValue(uint8_t *pBuf, uint32_t optValue)
\brief  This function takes a CoAP option value represented as uint and converts it to a buffer of
uint8_t values for writing in a packet.

\param [out]   pBuf           Pointer to buffer to be populated.
\param [in]   optValue        CoAP uint option value.

\return       uint32_t       The length of the newly filled buffer.
***************************************************************************************************/
uint8_t COAP_EncodeUintOptValue(uint8_t* pBuf, uint32_t optValue);

/*!*************************************************************************************************
\fn     void COAP_SerializeUriPath(coapUriPath_t* pUriPath, uint8_t *pDelta, uint8_t **currentPos)
\brief  This function serializes an Uri-Path and places the result starting with currentPos

\param [in]    pUriPath     Pointer to the Uri-Path to be serialized
\param [in]    pDelta       Delta compared to the previous COAP Option
\param [in]    currentPos   Current Position in the resulting serialized buffer

\return       void
***************************************************************************************************/
void COAP_SerializeUriPath(coapUriPath_t* pUriPath, uint8_t *pDelta, uint8_t **currentPos);

/*!*************************************************************************************************
\fn    uint32_t COAP_BlockToOptValue(coapBlock_t *pBlock)
\brief  This function takes a pointer to a coapBlock_t structure and returns its representation as a
CoAP uint option value.

\param [in]   pOptDesc         Pointer to coapBlock_t structure.

\return       uint32_t         The corresponding option value.
***************************************************************************************************/
uint32_t COAP_BlockToOptValue(coapBlock_t* pBlock);

/*================================================================================================*/
#endif  /* _COAP_H */

/*!
** @}
*/
