/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/**
 * @file comms_message_handler_fwupdate_server.h
 * @brief This file contains the FwUpdate server handler
 */

#ifndef COMMS_MESSAGE_HANDLER_FWUPDATE_SERVER_H_
#define COMMS_MESSAGE_HANDLER_FWUPDATE_SERVER_H_

#include "comms_message_handler.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef enum _sln_comms_message_fwupdate_server_type
{
    kCommsFwUpdateBlock = 0,
    kCommsFwUpdateStop,
    kCommsFwUpdateStartSelfCheck,
    kCommsFwUpdateActivateImage
} sln_comms_message_fwupdate_server_type;

/**
 * @brief Process the FwUpdate server JSON message
 *
 * @param clientInstance: The client instance data (received data and connection context)
 * @param json: Incoming payload to be processed
 *
 * @return          sln_comms_message_status
 */
sln_comms_message_status processFwUpdateServerReq(void *clientInstance, cJSON *json);

/**
 * @brief Start the FwUpdate process
 *
 * @param img_type: The type of the firmware image
 *
 * @return          sln_comms_message_status
 */
sln_comms_message_status handleServerFwUpdateStartReq(uint32_t img_type);

#endif /* COMMS_MESSAGE_HANDLER_FWUPDATE_SERVER_H_ */
