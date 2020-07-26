/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef LWIP_UDPSERVER_H
#define LWIP_UDPSERVER_H

#include "network_connection.h"

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "event_groups.h"
#include "task.h"

#include "udp_message_handler.h"

typedef struct udp_server_args
{
    network_mode_t network_mode;
    EventGroupHandle_t ap_close_event;
} udp_server_args_t;

/**
 * @brief Setter function for ais_auth_task_handle, needed for waking that task.
 *
 * @param handle[in]              Pointer to task handle.
 * @return                        Void.
 */
void udp_server_set_ais_auth_task_handle(TaskHandle_t *handle);

/**
 * @brief The task is responsible for communicating with the companion app through UDP messages.
 *        The current task ends after receiving the authentication code message from the companion app.
 *        Prior to its ending, this task creates the companion_app_status_task task, which will
 *        send status messages to the companion app.
 *
 * @param arg[in]                 Pointer to an argument, in accordance with TaskFunction_t
 * @return                        Void.
 */
void udp_server_task(void *arg);

/**
 * @brief The task is responsible for sending UDP status messages to the companion app.
 *        It ends after sending a connection complete / connection error message.
 *
 * @param arg[in]                 Pointer to an argument, in accordance with TaskFunction_t
 * @return                        Void.
 */
void companion_app_status_task(void *arg);

/**
 * @brief Function called from the app_init_task to inform the companion_app_status_task about status progress.
 *        The communication between tasks is being made through event bits.
 *
 * @param status[in]              Latest status update.
 * @return                        Void.
 */
void companion_app_status_set(app_status_event_t status);

#endif /* LWIP_UDPSERVER_H */
