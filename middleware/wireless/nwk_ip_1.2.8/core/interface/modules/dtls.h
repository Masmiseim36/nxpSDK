/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

 /*! *********************************************************************************
 * \defgroup DTLS Interface
 * @{
 ***********************************************************************************/

#ifndef _DTLS_H
#define _DTLS_H

/*!
\file       DTLS.h
\brief      his is a header file for the DTLS module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

#if DTLS_ENABLED
#include "sockets.h"
#include "EmbeddedTypes.h"
#include "TimersManager.h"
#include "GenericList.h"
#include "network_utils.h"
#include "ip_if_management.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*-----------------------------------------------------------------------------
-------------------------------------------------------------------------------
  INFORMATIVE MACROS THAT SHOULD NOT BE CHANGED BY THE USER
  (USED BY THREAD LIBRARY AT COMPILE TIME)
-------------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
/*! The maximum number of peers per DTLS session */
#define DTLS_MAX_PEERS             (5U)
/*! The length of the session ID  */
#define DTLS_SESSION_ID_LENGTH     (32U)
/*! The cookie length */
#define DTLS_COOKIE_LENGTH         (8U)
/*! The retransmission unit in milliseconds ( 1 unit has 100ms)  */
#define DTLS_RETRANSMIT_UNIT       (100U) /* ms*/

/*==================================================================================================
Public type definitions
==================================================================================================*/
/*! DTLS Peer Identifier */
typedef void* dtlsPeerPtr_t;

typedef struct dtls_InitParams_t{
    uint32_t ipIfId;
    uint8_t maxRetransmitCnt;           /*!< Number of message retransmissions. */
    uint32_t retransmitTimeUnits;       /*!< Number of time units to retransmit a packet */
} dtlsInitParams_t;

/*! Known cipher suites.*/
typedef enum
{
    DTLS_ECJPAKE_AES_128_CCM_8 = 0xC0FF,            /*!< ECJPAKE cipher */
} dtlsCipher_t;

typedef enum
{
    TLS_COMPRESSION_NULL = 0x00                     /*!< NULL compression */
} dtlsCompression_t;

/*! DTLS Alert levels.*/
typedef enum
{
    DTLS_ALERT_LEVEL_OK         = 0,
    DTLS_ALERT_LEVEL_WARNING    = 1,
    DTLS_ALERT_LEVEL_FATAL      = 2
} dtlsAlertLevel_t;

/*! DTLS Alert status*/
typedef enum
{
    DTLS_ALERT_CLOSE_NOTIFY = 0,                    /*!< close_notify */
    DTLS_ALERT_UNEXPECTED_MESSAGE = 10,                /*!< unexpected_message */
    DTLS_ALERT_BAD_RECORD_MAC = 20,                    /*!< bad_record_mac */
    DTLS_ALERT_RECORD_OVERFLOW = 22,                /*!< record_overflow */
    DTLS_ALERT_HANDSHAKE_FAILURE = 40,                /*!< handshake_failure */
    DTLS_ALERT_ILLEGAL_PARAMETER = 47,                /*!< illegal_parameter */
    DTLS_ALERT_UNKNOWN_CA = 48,                        /*!< unknown_ca */
    DTLS_ALERT_ACCESS_DENIED = 49,                    /*!< access_denied */
    DTLS_ALERT_DECODE_ERROR = 50,                    /*!< decode_error */
    DTLS_ALERT_DECRYPT_ERROR = 51,                    /*!< decrypt_error */
    DTLS_ALERT_PROTOCOL_VERSION = 70,                /*!< protocol_version */
    DTLS_ALERT_INSUFFICIENT_SECURITY = 71,            /*!< insufficient_security */
    DTLS_ALERT_INTERNAL_ERROR = 80,                    /*!< internal_error */
    DTLS_ALERT_USER_CANCELED = 90,                    /*!< user_canceled */
    DTLS_ALERT_NO_RENEGOTIATION = 100,                /*!< no_renegotiation */
    DTLS_ALERT_UNSUPPORTED_EXTENSION = 110,            /*!< unsupported_extension */
    /*! Internal events sent to application  */
    DTLS_ALERT_CONNECTED  = 0xB0U,                  /*!< Signal when peers were successfully
                                                    connected */
} dtlsAlertCode_t;

typedef enum dtlsErr_tag
{
    gDtlsErrOK_c = 0x00U,
    gDtlsErrInvalidDestination_c,
    gDtlsErrNoMemory_c,
    gDtlsErrGenericError_c = THR_ALL_FFs8
} dtlsErr_t;

/*! The DTLS needed callbacks */
typedef struct dtlsCallbacks_tag
{
    /*! callback to receive data */
    void (*rcvd)(dtlsPeerPtr_t pPeer, uint8_t * pData, uint32_t len);
    /*! callback to signal an alert event */
    void (*event)(void *pPeer, uint8_t level, uint8_t code);
    /*! callback to get to passphrase for the peer */
    void (*getJpakePasswd)(dtlsPeerPtr_t pPeer, uint32_t *pSecretLen, uint8_t **ppSecret);
    /*! server function used to check a client */
    bool_t (*pfCheckClient)(ipAddr_t *pIpAddr);
} dtlsCallbacks_t;

typedef int32_t(*fpTx_t)(int32_t sockfd, uint8_t *pMsg, uint32_t msgLen, uint32_t flags,
            sockaddrStorage_t const *to, socklen_t toLen);

/*<! Keep track of all information for DTLS module. */
typedef struct dtlsContext_tag
{
    int32_t fd;
    fpTx_t fpSend;  /*!< Pointer to the function used to send the DTLS payload */
    tmrTimerID_t timerId;
    dtlsInitParams_t initParams;
    list_t peersList;
    list_t retransQueue;
    dtlsCallbacks_t cb;
    uint8_t aCookieSecret[16];
} dtlsContext_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/
/*!*************************************************************************************************
\fn     void DTLS_Init(taskMsgQueue_t *pDtlsTaskMsgQueue)
\brief  This function initializes the DTLS module

\param  [in]  pDtlsTaskMsgQueue  Pointer to DTLS task message queue
***************************************************************************************************/
void DTLS_Init(taskMsgQueue_t *pDtlsTaskMsgQueue);

/*!*************************************************************************************************
\fn     dtlsContext_t *DTLS_NewContext(dtlsInitParams_t* pDtlsInitParams, dtlsCallbacks_t *pCb,
        sockaddrStorage_t *pAddrStorage)
\brief  This function allocates a new context for DTLS. This context is used to store the DTLS
        configuration parameters and function callbacks. It is also opening a UDP socket using the
        provided IPv6 address information.

\param  [in]  pDtlsInitParams  Pointer to DTLS initialization parameters
\param  [in]  pCb              Pointer to DTLS callback to be registered
\param  [in]  pAddrStorage     Pointer to local device addressing information

\return       dtlsContext_t *  Return a pointer to the allocated DTLS context
***************************************************************************************************/
dtlsContext_t *DTLS_NewContext(dtlsInitParams_t* pDtlsInitParams, dtlsCallbacks_t *pCb,
                               sockaddrStorage_t *pAddrStorage);

/*!*************************************************************************************************
\fn     void DTLS_FreeContext(dtlsContext_t *pCtx)
\brief  This function is used to free an allocated DTLS context.

\param  [in]  pCtx  Pointer to allocated DTLS context
***************************************************************************************************/
void DTLS_FreeContext(dtlsContext_t *pCtx);

/*!*************************************************************************************************
\fn     dtlsPeerPtr_t DTLS_Connect(dtlsContext_t *pCtx, sockaddrStorage_t * pAddr)
\brief  This function is used to establish a DTLS secure session with a destination device.
        The secured session is successfully established if a DTLS_ALERT_CONNECTED event
        is received in the register "event callback".

\param  [in]  pCtx              Pointer to DTLS context
\param  [in]  pAddr             Destination addressing information

\return       dtlsPeerPtr_t     Peer ID
***************************************************************************************************/
dtlsPeerPtr_t DTLS_Connect(dtlsContext_t *pCtx, sockaddrStorage_t * pAddr);

/*!*************************************************************************************************
\fn     dtlsErr_t DTLS_Send(dtlsPeerPtr_t peer, uint8_t *pData, uint32_t len)
\brief  This function sends application data using the secured DTL session specified by a known
        peer.

\param  [in]  peer      DTLS peer
\param  [in]  pData     Pointer to data
\param  [in]  len       Data length

\return       dtlsErr_t DTLS status
***************************************************************************************************/
dtlsErr_t DTLS_Send(dtlsPeerPtr_t peer, uint8_t *pData, uint32_t len);

/*!*************************************************************************************************
\fn     dtlsErr_t DTLS_EncryptData(ipAddr_t *pIpAddr, uint8_t *pData, uint32_t len, uint8_t **pOut,
        uint32_t *pOutLen)
\brief  This function encrypts the application data and puts it in a DTLS record.

\param  [in]    pIpAddr     DTLS peer IP address
\param  [in]    pData       pointer to data
\param  [in]    len         data length

\param  [out]   pOut        pointer to encrypted data
\param  [out]   pOutLen     encrypted data length

\return         dtlsErr_t   DTLS status
***************************************************************************************************/
dtlsErr_t DTLS_EncryptData(ipAddr_t *pIpAddr, uint8_t *pData, uint32_t len, uint8_t **pOut,
                           uint32_t *pOutLen);

/*!*************************************************************************************************
\fn     dtlsErr_t DTLS_Close(dtlsPeerPtr_t peer)
\brief  This function sends a Close Notification alert message and puts the DTLS session in
        "Closing" state. This function is not freeing the peer. The peer will be freed if a Close
        Notification will be received from the peer. If no Close Notification will be received
        the application should free the peer using DTLS_DeletePeer();

\param  [in]    peer            DTLS peer

\return         dtlsErr_t       DTLS status
***************************************************************************************************/
dtlsErr_t DTLS_Close(dtlsPeerPtr_t peer);

/*!*************************************************************************************************
\fn     void DTLS_DeletePeer(dtlsPeerPtr_t peer)
\brief  This function will free the given DTLS peer. If the peer is in the "Connected" state, it
        will also send Close Notification alert message before freeing the peer

\param [in] peer    DTLS peer
***************************************************************************************************/
void DTLS_DeletePeer(dtlsPeerPtr_t peer);

/*!*************************************************************************************************
\fn     sockaddrStorage_t *DTLS_GetPeerName(dtlsPeerPtr_t pPeer)
\brief  This function returns the IPv6 address information for a known peer.

\param  [in]  pPeer  The peer

\retval       sockaddrStorage_t*  Pointer to IPv6 address information
***************************************************************************************************/
sockaddrStorage_t *DTLS_GetPeerName(dtlsPeerPtr_t pPeer);

/*!*************************************************************************************************
\fn     dtlsPeerPtr_t DTLS_GetPeer(dtlsContext_t *pCtx, ipAddr_t *pIpAddr)
\brief  This function returns the DTLS peer information for a DTLS context and peer IP address. If
        a DTLS context is not provided, all existing DTLS contexts will be searched for the peer
        address.

\param  [in]    pCtx                DTLS context
\param  [in]    pIpAddr             DTLS peer IPv6 address

\return         dtlsPeerPtr_t       DTLS peer id
***************************************************************************************************/
dtlsPeerPtr_t DTLS_GetPeer(dtlsContext_t *pCtx, ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     dtlsContext_t *DTLS_GetContext(dtlsPeerPtr_t peer, ipAddr_t *pIpAddr)
\brief  This function returns the DTLS context for a given DTLS peer or peer ip address.

\param  [in]  peer      pointer to the peer (can be NULL is if the search is done after pIpAddr)
\param  [in]  pIpAddr   pointer to the peer (can be NULL is if the search is done after peer)

\return dtlsContext_t   DTLS peer context if peer was found, NULL otherwise
***************************************************************************************************/
dtlsContext_t *DTLS_GetContext(dtlsPeerPtr_t peer, ipAddr_t* pIpAddr);

/*!*************************************************************************************************
\fn     uint8_t *DTLS_GetRawKeyBlock(dtlsPeerPtr_t pPeer, uint8_t *pKeyBlockSize)
\brief  This function is used to get a pointer to the DTLS KeyBlock.

\param  [in]    pPeer           pointer to the peer
\param  [out]   pKeyBlockSize   return the KeyBlock Size

\return         uint8_t*        Pointer to the KeyBlock
***************************************************************************************************/
uint8_t *DTLS_GetRawKeyBlock(dtlsPeerPtr_t pPeer, uint8_t* pKeyBlockSize);

/*!*************************************************************************************************
\fn     void DTLS_HandleRecords(dtlsContext_t *pCtx, uint8_t *pBuff, uint16_t buffLen,
        sockaddrStorage_t *pRemAddr, bool_t loopback)
\brief  This function is used to process the data received

\param  [in] pCtx       pointer to the context of the connection which received this message
\param  [in] pBuff      pointer to the received buffer
\param  [in] buffLen    received buffer length
\param  [in] pRemAddr   source addressing information
\param  [in] loopback   flag indicating if the packet originated from this device
***************************************************************************************************/
void DTLS_HandleRecords(dtlsContext_t *pCtx, uint8_t *pBuff, uint16_t buffLen,
                        sockaddrStorage_t *pRemAddr, bool_t loopback);

#endif /*DTLS_ENABLED*/
/*================================================================================================*/
#endif
