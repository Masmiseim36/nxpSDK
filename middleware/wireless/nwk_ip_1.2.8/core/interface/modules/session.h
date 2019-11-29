/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _SESSION_H
#define _SESSION_H
/*!=================================================================================================
\file       session.h
\brief      This is a header file for the Session module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "network_utils.h"
#include "sockets.h"

/*==================================================================================================
Public macros
==================================================================================================*/

#ifndef SESSION_MODULE_ENABLE
    #define SESSION_MODULE_ENABLE        1
#endif

/* the TCP data indication callback is called by sending a message or directly from the context of
   TCP task*/
#define SESSION_TCP_DATA_IND_SEND_MSG    0


/*==================================================================================================
Public type definitions
==================================================================================================*/

/*!< Session event status codes */
typedef enum
{
    gSessSuccess_c          = 0U,   /*!<  Success */
    gSessSockConnected_c    = 1U,   /*!<  Socket is connected */
    gSessSockConnReset_c    = 1U,   /*!<  Connection reset by peer */
    gSessSockConnClosing_c  = 2U,   /*!<  Connection is being closed by peer */
    gSessError_c            = THR_ALL_FFs32   /*!<  Invalid */
} sessionEvCodes_t;

/*! Structure used to keep information about a received packet */
typedef struct sessionPacket_tag
{
    int32_t sockFd;
    sockaddrStorage_t remAddr; /* from */
    sockaddrStorage_t localAddr; /* to */
    uint32_t dataLen;
    uint8_t *pData;
    recvOptions_t packetOpt;
    sessionEvCodes_t sessStatus;
} sessionPacket_t;

/*! Structure used to keep information about a socket handler */
typedef struct sessEntity_tag
{
    int32_t sockFd;
    taskMsgQueue_t *pMsgQueue;
    nwkMsgHandler pHandler;
    nwkMsgHandler pEventHandler;
} sessEnt_t;

typedef enum
{
    mSessCbData_c,   /*!< The callback registered is of data indication type */
    mSessCbEvent_c,  /*!< The callback registered is of event indication type(connect or accept) */
} sessCbType_t;

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
\fn    void Session_Init()
\brief Public interface function for Session module. This function initializes the Session task.
       This function must be called from the upper layer task directly.
***************************************************************************************************/
void Session_Init(void);

/*!*************************************************************************************************
\fn    nwkStatus_t Session_RegisterCb(int32_t sockFd, nwkMsgHandler pHandler,
       taskMsgQueue_t *pMsgQueue, sessCbType_t cbType)
\brief This function registers a data/event indication callback. This callback wll be called whenever
       new data is received on the socket for a data callback and whenever a new connection related
       event happens in case of an event callback. Connection events can only be registered on a
       STREAM socket. Possible connection events are: connected, connection shutdown, connection
       reset.

\param [in]  sockFd     socket file descriptor
\param [in]  pHandler   pointer to the network message handler, MUST not be NULL
\param [in]  pMsgQueue  pointer to the task message queue, can be NULL and the pHandler will execute
                        on Session task
\param [in]  cbType     callback type, can be data(mSessCbData_c) or event(mSessCbEvent_c)

\retval      gNwkStatusSuccess_c       Registration successfull
\retval      gNwkStatusInvalidParam_c  One or more of the input parameters are invalid
\retval      gNwkStatusMemAllocErr_c   No more space for new callback registration

\details This function does the following:
          - registers a callback function for the corresponding socket file descriptor
***************************************************************************************************/
nwkStatus_t Session_RegisterCb(int32_t sockFd, nwkMsgHandler pHandler,
                                        taskMsgQueue_t *pMsgQueue,sessCbType_t cbType);

/*!*************************************************************************************************
\fn    nwkStatus_t Session_UnRegisterCb(int32_t sockFd)
\brief This function unregisters the callback(s) of a socket from Session layer. If both data and
       event callback are registerd, both wiil be clear from Session layer.

\param [in]  sockFd     socket file descriptor

\retval      gNwkStatusSuccess_c       Deregistration successfull
\retval      gNwkStatusInvalidParam_c  Input parameter is invalid

\details This function does the following:
          - unregisters a socket and its corresponding callbacks from the select list
***************************************************************************************************/
nwkStatus_t Session_UnRegisterCb(int32_t sockFd);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _SESSION_H */
