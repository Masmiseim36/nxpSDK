/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/**
 * @file comms_message_handler_fwupdate_common.h
 * @brief This file contains the FwUpdate common handlers
 */

#ifndef COMMS_MESSAGE_HANDLER_FWUPDATE_COMMON_H_
#define COMMS_MESSAGE_HANDLER_FWUPDATE_COMMON_H_

#include "comms_message_handler.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define COMMS_FWUPDATE_MAX_SIGNATURE_SIZE 256
#define COMMS_PACKET_CRC_SIZE 4
#define COMMS_PACKET_CRC_OFFSET(packet_length) (packet_length - 4)

/* Use this index to determine the base programming address from the passed image reset vector */
#define COMMS_PACKET_IMG_ADDR_IDX 6

typedef struct _sln_comms_fwupdate_job_desc
{
    uint32_t status;
    uint32_t imageSize;
    uint32_t appBankType;
    uint32_t dataWritten;
    char signature[COMMS_FWUPDATE_MAX_SIGNATURE_SIZE];
    char *jobId;
} sln_comms_fwupdate_job_desc;

typedef enum _sln_comms_message_fwupdate_type
{
    kCommsFwUpdateCommonMsg = 0,
    kCommsFwUpdateServerMsg,
} sln_comms_message_fwupdate_type;

typedef enum _sln_comms_message_fwupdate_common_type
{
    kCommsFwUpdateStart = 0,
    kCommsFwUpdateState,
    kCommsFwUpdateClean,
} sln_comms_message_fwupdate_common_type;

/**
 * @brief Process the incoming FwUpdate JSON message
 *
 * @param clientInstance: The client instance data (received data and connection context)
 * @param json: Incoming payload to be processed
 *
 * @return          sln_comms_message_status
 */
sln_comms_message_status processFwUpdateReq(void *clientInstance, cJSON *json);

/**
 * @brief Get the current FwUpdate job handle
 *
 * @return          sln_comms_fwupdate_job_desc: Handle of the FwUpdate descriptor
 */
sln_comms_fwupdate_job_desc *getCurrentFwUpdateJob(void);

#endif /* COMMS_MESSAGE_HANDLER_FWUPDATE_COMMON_H_ */
