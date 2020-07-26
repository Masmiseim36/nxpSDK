/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef UDP_MESSAGE_HANDLER_H_
#define UDP_MESSAGE_HANDLER_H_

#include "network_connection.h"

#include "aws_clientcredential.h"
#include "mbedtls/base64.h"
#include "cJSON.h"
#include "fsl_debug_console.h"
#include "fsl_trng.h"
#include "ksdk_mbedtls.h"
#include "lwip/api.h"
#include "mbedtls/sha256.h"
#include "sha256_alt.h"
#include "stdint.h"
#include "timers.h"

#define clientcredentialIOT_PRODUCT_NAME "uVoice_0v9"

#define MAX_MESSAGE_LEN 512

#define STATUS_REGISTERING_REQUEST_STRING "registering request"
#define STATUS_REGISTERING_RESPONSE_STRING "registering response"
#define STATUS_MQTT_SUBSCRIBED_STRING "mqtt subscribed"
#define STATUS_CONNECTION_COMPLETE_STRING "connection complete"
#define STATUS_CONNECTION_ERROR_STRING "error in connection"

typedef struct lwa_auth_data
{
    char *auth_code;
    char *client_id;
    char *redirect_uri;
} lwa_auth_data_t;

typedef enum companion_message
{
    INVALID_MSG = -1, /* invalid message */
    HELLO_REQ   = 0,  /* hello broadcast */
    CONFIRMATION_REQ, /* confirmation request */
    AUTH_CODE,        /* auth code, client ID, URI message */
    STATUS_MSG,       /* status messages sent to the companion app */
    AP_MODE,          /* hello ap mode */
    WIFI_CRED,        /* wifi credentials (SSID and password) */
} companion_message_t;

typedef enum app_status_event
{
    REGISTERING_REQ = (1 << 0U),
    REGISTERING_RSP = (1 << 1U),
    MQTT_SUBSCRIBED = (1 << 2U),
    CONN_COMPLETE   = (1 << 3U),
    CONN_ERROR      = (1 << 4U),
} app_status_event_t;

/**
 * @brief Function called when a message is received in the UDP server
 *
 * @param conn[in]        Pointer towards the structure which describes the connection
 * @param buf_recv[in]    Pointer towards the structure which describes the received package
 * @return                companion_message_t associated to the received message
 */
companion_message_t message_handle(struct netconn *conn, struct netbuf *buf_recv);

/**
 * @brief Function called when in AP mode and a message is received in the UDP server
 *
 * @param conn[in]        Pointer towards the structure which describes the connection
 * @param buf_recv[in]    Pointer towards the structure which describes the received package
 * @return                companion_message_t associated to the received message
 */
companion_message_t apmode_message_handle(struct netconn *conn, struct netbuf *buf_recv);

/**
 * @brief Returns the pointer of the global lwa auth data
 *
 * @return                lwa_auth_data_t *
 */
lwa_auth_data_t *get_lwa_auth_data();

/**
 * @brief Returns the pointer of the latest generated code challenge
 *
 * @return                char *
 */
char *get_generated_code_challenge();

/**
 * @brief Returns the pointer of the latest generated code verifier
 *
 * @return                char *
 */
char *get_generated_code_verifier();

/**
 * @brief Creates a JSON status message in the form expected by the companion app
 * @param buffer[out]       Pointer towards a pre-buffer where the resulting string will be written
 * @param buffer_len[in]    Length of the pre-allocated buffer
 * @param status[in]        app_status_event_t type describing the status
 * @return                  0 on success, 1 otherwise
 */
uint8_t status_message_create(void *buffer, uint16_t buffer_len, app_status_event_t status);

#endif /* UDP_MESSAGE_HANDLER_H_ */
