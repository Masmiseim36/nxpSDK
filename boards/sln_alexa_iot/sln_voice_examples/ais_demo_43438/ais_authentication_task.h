/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/** @file ais_authentication_task.h
 *  @brief This file handles the authentication with the AIS service
 */

#ifndef AIS_AUTHENTICATION_TASK_H_
#define AIS_AUTHENTICATION_TASK_H_

#include "network_connection.h"

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"

#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwipopts.h"

#include "httpsclient.h"
#include "udp_message_handler.h"
#include "udp_server.h"

#include "flexspi_hyper_flash_ops.h"
#include "sln_flash.h"

#define HTTPS_REQUEST_BUFFER_LEN 2048

/**
 * @brief Returns the access token buffer start
 *
 * @return                       char *
 */
char *refresh_token_get(void);

/**
 * @brief Frees the memory occupied by the access token
 *
 * @return                       Void
 */
void refresh_token_free(void);

/**
 * @brief Setter function for app_init_task_handle, needed for waking that task.
 *
 * @param handle[in]              Pointer to task handle.
 * @return                        Void.
 */
void ais_authentication_set_app_init_task_handle(TaskHandle_t *handle);

/**
 * @brief The task is responsible for on-boarding and should be typically executed only once
 *        in the lifetime of a device. It handles multiple operations, like communicating with
 *        the companion app through UDP messages, making https requests for getting the amazon
 *        access token and sending status messages to the companion app while all of these happen.
 *        Some of these operations are performed from other tasks which have this task as a parent.
 *
 * @param arg[in]                 Pointer to an argument, in accordance with TaskFunction_t
 * @return                        Void.
 */
void ais_authentication_task(void *arg);

#endif /* AIS_AUTHENTICATION_TASK_H_ */
