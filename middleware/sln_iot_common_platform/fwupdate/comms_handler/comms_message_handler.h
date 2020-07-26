/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/**
 * @file comms_message_handler.h
 * @brief This file contains the FwUpdate handlers (initialization, data processing hook)
 */

#ifndef COMMS_MESSAGE_HANDLER_H_
#define COMMS_MESSAGE_HANDLER_H_

#include "assert.h"
#include "stdbool.h"
#include "stdint.h"

#include "FreeRTOS.h"
#include "comms_message_handler_cfg.h"
#include "common_connection_handler.h"
#include "cJSON.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef enum _sln_comms_message_status
{
    kComms_Success = 0,
    kComms_FailedParsing,
    kComms_CrcMismatch,
    kComms_FailedProcessing,
    kComms_AlreadyInProgress,
    kComms_UnexpectedMessage,
    kComms_UnknownMessage,
    kComms_InvalidParameter,
    kComms_FailedToTransmit,
} sln_comms_message_status;

typedef enum _sln_comms_message_type
{
    kCommsAis = 0,
    kCommsFwUpdate,
} sln_comms_message_type;

/**
 * @brief Initialize the comms message handler
 */
void SLN_COMMS_MESSAGE_Init(sln_common_interface_type interfaceType);

/**
 * @brief Send a string representation of the JSON message
 *
 * @param readContext: Incoming context containing the received data and the connection context
 * @param jsonMessage: JSON object to be converted to string and sent
 *
 * @return sln_comms_message_status
 */
sln_comms_message_status SLN_COMMS_MESSAGE_Send(sln_common_connection_recv_context *readContext, cJSON *jsonMessage);

/**
 * @brief Process the incoming JSON message
 *
 * @param readContext: Incoming context containing the message to be processed and the connection context
 *
 * @return          sln_common_connection_message_status
 */
sln_common_connection_message_status SLN_COMMS_MESSAGE_Process(sln_common_connection_recv_context *readContext);

#endif /* COMMS_MESSAGE_HANDLER_H_ */
