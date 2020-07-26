/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/**
 * @file common_connection_handler.h
 * @brief This file contains the common interface to all communication interface handlers
 */

#ifndef COMMON_CONNECTION_HANDLER_H_
#define COMMON_CONNECTION_HANDLER_H_

#include "assert.h"
#include "stdbool.h"
#include "stdint.h"

#include "comms_message_handler_cfg.h"
#if COMMS_MESSAGE_HANDLER_FWUPDATE_METHOD_OTA_TCP
#include "tcp_connection_handler.h"
#endif
#if COMMS_MESSAGE_HANDLER_FWUPDATE_METHOD_OTW_UART
#include "uart_connection_handler.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Enumeration of all status messages */
typedef enum _sln_common_connection_message_status
{
    kCommon_Success = 0,
    kCommon_Failed,
    kCommon_ToManyBytes,
    kCommon_NoDataRead,
    kCommon_ConnectionLost,
} sln_common_connection_message_status;

typedef enum _sln_common_interface_type
{
    kCommonInterfaceTcp = 0,
    kCommonInterfaceUart
} sln_common_interface_type;

/* Union of all supported connection contexts used to differentiate between connected clients */
typedef union sln_common_connection_context
{
#if COMMS_MESSAGE_HANDLER_FWUPDATE_METHOD_OTA_TCP
    sln_tcp_connection_context sTcpContext;
#endif
#if COMMS_MESSAGE_HANDLER_FWUPDATE_METHOD_OTW_UART
    sln_uart_connection_context sUartContext;
#endif

} sln_common_connection_context;

/* Structure specifically for the recv callback function */
typedef struct _sln_common_connection_recv_context
{
    sln_common_connection_context connContext;
    uint8_t *data;
    uint32_t packet_size;
} sln_common_connection_recv_context;

/* Structure specifically for the write function */
typedef struct _sln_common_connection_write_context
{
    sln_common_connection_context connContext;
    uint32_t len;
    uint8_t *data;
} sln_common_connection_write_context;

/* Callback defintion to where the connection data received calls */
typedef sln_common_connection_message_status (*recv_cb_fn)(sln_common_connection_recv_context *rxContext);

/* Function defintion to define the connection write function common to all interfaces */
typedef sln_common_connection_message_status (*write_fn)(sln_common_connection_write_context *txContext);

typedef union _sln_common_interface_context
{
#if COMMS_MESSAGE_HANDLER_FWUPDATE_METHOD_OTA_TCP
    sln_tcp_interface_context sTcpContext;
#endif
#if COMMS_MESSAGE_HANDLER_FWUPDATE_METHOD_OTW_UART
    sln_uart_interface_context sUartContext;
#endif

} sln_common_interface_context;

/* This is a small structure member. This may be added to in the future
 * so it's best to start as a structure
 */
typedef struct _sln_common_connection_desc
{
    recv_cb_fn recv_cb;
    write_fn write;
    sln_common_interface_type connType;
    sln_common_interface_context context;

} sln_common_connection_desc;

/**
 * @brief The initializaton interface for the common connection
 *
 * @param sln_common_connection_desc: Descriptor detailing the specific comms interface descriptor
 *
 * @return Success or fail
 *
 */
sln_common_connection_message_status SLN_CommonConnectionHandlerInit(sln_common_connection_desc *desc);

#endif /* COMMON_CONNECTION_HANDLER_H_ */
