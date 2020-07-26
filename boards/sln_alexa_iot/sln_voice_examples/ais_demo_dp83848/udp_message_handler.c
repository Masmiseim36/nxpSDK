/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "udp_message_handler.h"

/* Device specific includes */
#include "device_utils.h"

/* Application Wifi includes*/
#include "wifi_credentials.h"

#include "sln_RT10xx_RGB_LED_driver.h"

static companion_message_t g_expected_message = HELLO_REQ;
static uint8_t g_crypto_initialized = 0;
static uint8_t g_auth_code_received = 0;
static TimerHandle_t g_auth_code_timer = NULL;
static struct netconn *retry_conn = NULL;
static struct netbuf retry_buf_send;

__attribute__((section(".ocram_non_cacheable_data")))
char retry_payload[MAX_MESSAGE_LEN] = {0};

#if USE_WIFI_CONNECTION
static uint8_t apmode_parse_message(void *message, uint16_t buffer_len, companion_message_t *message_type);
static uint8_t apmode_response_create(void *message, uint16_t buffer_len);
#endif
static uint8_t parse_message(void *message, uint16_t buffer_len, companion_message_t *message_type);
static uint8_t hello_response_create(void *message, uint16_t buffer_len);
static uint8_t confirmation_response_create(void *message, uint16_t buffer_len);
static uint8_t query_send_reply(companion_message_t message, companion_message_t *next_expected_message);
static uint8_t generate_code_challenge();
static struct netbuf *pbuf_send_create(struct netbuf *buf_recv, char *payload);
static void vAuthCodeTimerCallback(TimerHandle_t xTimer);

/* data needed for lwa authentication */

static lwa_auth_data_t g_lwa_auth_data = {0};

__attribute__((section(".ocram_non_cacheable_data")))
static char g_code_challenge_buffer[65] = {0};
__attribute__((section(".ocram_non_cacheable_data")))
static char g_code_verifier_buffer[44] = {0};

companion_message_t message_handle(struct netconn *conn, struct netbuf *buf_recv)
{
    err_t net_err = ERR_OK;
    struct netbuf *buf_send = NULL;
    char payload[MAX_MESSAGE_LEN] = {0};
    companion_message_t message_type = INVALID_MSG, next_message_type = INVALID_MSG;

    if (netbuf_copy(buf_recv, payload, MAX_MESSAGE_LEN) != buf_recv->p->tot_len)
    {
        configPRINTF(("netbuf_copy failed\n"));
        goto clean;
    }

    payload[buf_recv->p->tot_len] = '\0';

    configPRINTF(("Received message from: %u.%u.%u.%u on port %d -> %s\r\n", ((u8_t *)&buf_recv->addr.addr)[0],
                  ((u8_t *)&buf_recv->addr.addr)[1], ((u8_t *)&buf_recv->addr.addr)[2],
                  ((u8_t *)&buf_recv->addr.addr)[3], buf_recv->port, payload));

    if (parse_message(payload, MAX_MESSAGE_LEN, &message_type))
    {
        /* invalid message, parsing failed or reply not needed */
        configPRINTF(("Parsing the message failed\r\n"));
        goto clean;
    }

    if (query_send_reply(message_type, &next_message_type))
    {
        /* do not reply */
        configPRINTF(("Will not reply to the message\r\n"));
        goto clean;
    }

    /* valid message received, reply successfully created, let's send it */
    buf_send = pbuf_send_create(buf_recv, payload);
    if (!buf_send)
    {
        goto clean;
    }

    configPRINTF(("Replying with %s\r\n", payload));
    net_err = netconn_send(conn, buf_send);
    if (net_err != ERR_OK)
    {
        configPRINTF(("netconn_send failed: %d\n", (int)net_err));
        goto clean;
    }
    else
    {
        /* update expected message */
        g_expected_message = next_message_type;

        /* start a timer for checking if we received an
         * auth code message from the companion app */
        if (g_expected_message == AUTH_CODE)
        {
            retry_conn = conn;
            netbuf_set_fromaddr(&retry_buf_send, netbuf_fromaddr(buf_recv));
            retry_buf_send.port = netbuf_fromport(buf_recv);
            memcpy(retry_payload, payload, strlen(payload) + 1);

            /* timer not created? create it and start it */
            if (!g_auth_code_timer)
            {
                g_auth_code_timer = xTimerCreate("AuthCodeTimer",       /* name */
                                                 pdMS_TO_TICKS(2000),   /* period */
                                                 pdTRUE,                /* self reload */
                                                 (void *)0,             /* id */
                                                 vAuthCodeTimerCallback /* function to call when expiring */
                );

                if (!g_auth_code_timer)
                {
                    configPRINTF(("xTimerCreate failed\n"));
                }

                if (xTimerStart(g_auth_code_timer, 0) != pdPASS)
                {
                    configPRINTF(("xTimerStart failed\n"));
                }
            }
            else
            {
                /* reset it */
                xTimerStop(g_auth_code_timer, 0);
                vTimerSetTimerID(g_auth_code_timer, (void *)0);
                if (xTimerReset(g_auth_code_timer, 0) != pdPASS)
                {
                    configPRINTF(("xTimerReset failed\n"));
                }
            }
        }
    }

clean:
    netbuf_delete(buf_send);
    return message_type;
}

#if USE_WIFI_CONNECTION
companion_message_t apmode_message_handle(struct netconn *conn, struct netbuf *buf_recv)
{
    err_t net_err = ERR_OK;
    struct netbuf *buf_send = NULL;
    char payload[MAX_MESSAGE_LEN] = {0};
    companion_message_t message_type = INVALID_MSG;

    if (netbuf_copy(buf_recv, payload, MAX_MESSAGE_LEN) != buf_recv->p->tot_len)
    {
        configPRINTF(("netbuf_copy failed\n"));
        goto clean;
    }

    payload[buf_recv->p->tot_len] = '\0';

    configPRINTF(("AP Received message from: %u.%u.%u.%u on port %d -> %s\r\n", ((u8_t *)&buf_recv->addr.addr)[0],
                  ((u8_t *)&buf_recv->addr.addr)[1], ((u8_t *)&buf_recv->addr.addr)[2],
                  ((u8_t *)&buf_recv->addr.addr)[3], buf_recv->port, payload));

    if (apmode_parse_message(payload, MAX_MESSAGE_LEN, &message_type))
    {
        /* invalid message, parsing failed or reply not needed */
        configPRINTF(("AP Parsing the message failed\r\n"));
        goto clean;
    }

    /* valid message received, reply successfully created, let's send it */
    buf_send = pbuf_send_create(buf_recv, payload);
    if (!buf_send)
    {
        goto clean;
    }

    configPRINTF(("AP Replying with %s\r\n", payload));
    net_err = netconn_send(conn, buf_send);
    if (net_err != ERR_OK)
    {
        configPRINTF(("netconn_send failed: %d\n", (int)net_err));
        goto clean;
    }

clean:
    netbuf_delete(buf_send);
    return message_type;
}
#endif

/**
 * @brief Function which allocates a netbuf structure and populates it with a payload and
 *        destination port and address
 *
 * @param buf_recv[in]    Used only for obtaining the destination port and address
 * @param payload[in]     The payload which should be incorporated into the resulting netbuf
 * @return                Pointer to the resulting netbuf if successfully created, otherwise NULL
 * @note                  Allocates memory, caller responsible for freeing it
 */
static struct netbuf *pbuf_send_create(struct netbuf *buf_recv, char *payload)
{
    err_t net_err = ERR_OK;
    struct netbuf *buf_send = netbuf_new();

    if (!buf_send)
    {
        configPRINTF(("netbuf_new failed\n"));
        goto clean;
    }

    buf_send->p = pbuf_alloc(PBUF_TRANSPORT, strlen(payload), PBUF_RAM);
    if (!buf_send->p)
    {
        configPRINTF(("pbuf_alloc failed\n"));
        goto clean;
    }

    net_err = netbuf_take(buf_send, payload, strlen(payload));
    if (net_err != ERR_OK)
    {
        configPRINTF(("netbuf_take failed: %d\n", (int)net_err));
        goto clean;
    }

    netbuf_set_fromaddr(buf_send, netbuf_fromaddr(buf_recv));
    /* companion app listens on port 8888 */
    buf_send->port = 8888;

    return buf_send;

clean:
    netbuf_delete(buf_send);
    return NULL;
}

/**
 * @brief Function called when the registered timer's timeout is triggered.
 *        Used only for re-trying to send confirmation responses towards the companion app.
 *        If an auth message is received in the mean time, the timer is stopped.
 *
 * @param xTimer[in]    Pointer to the timer structure
 * @return              Void
 * @note                The timer Id is used as a retry counter
 */
static void vAuthCodeTimerCallback(TimerHandle_t xTimer)
{
    uint32_t ulCount = (uint32_t)pvTimerGetTimerID(xTimer);
    struct netbuf *buf_send = NULL;

    /* resend the message for up to 5 times */
    if (g_auth_code_received || ulCount == 5)
    {
        xTimerStop(xTimer, 0);
        g_auth_code_received = 0;
    }
    else
    {
        ulCount++;
        vTimerSetTimerID(xTimer, (void *)ulCount);
        buf_send = pbuf_send_create(&retry_buf_send, retry_payload);
        if (buf_send)
        {
            netconn_send(retry_conn, buf_send);
            netbuf_delete(buf_send);
        }
    }
}

/**
 * @brief Function which tries to parse a message received by the UDP server
 *
 * @param message[in/out]   Pointer towards the buffer containing the received string
 *                          The reply JSON will be written in the same buffer
 * @param buffer_len[in]    Length of the buffer
 * @param message_type[out] Pointer for storing the message type
 * @return                  0 when succeeded, 1 otherwise
 * @note                    JSON messages are expected
 */
static uint8_t parse_message(void *message, uint16_t buffer_len, companion_message_t *message_type)
{
    cJSON *parsed_message = NULL;
    uint8_t status = 1;
    const cJSON *json_item = NULL;
    const cJSON *json_item_mode = NULL;
    const cJSON *json_item_client_id = NULL;
    const cJSON *json_item_auth_code = NULL;
    const cJSON *json_item_redirect_uri = NULL;

    parsed_message = cJSON_Parse(message);
    if (parsed_message == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            configPRINTF(("Error at cJSON_Parse: %s\n", error_ptr));
        }
        goto end;
    }

    /* TODO For production, the Hello request messages will be encrypted,
     *      so it'll be needed to decrypt them here */

    /* Check if a state message was received */
    json_item = cJSON_GetObjectItemCaseSensitive(parsed_message, "State");
    if (cJSON_IsString(json_item) && (json_item->valuestring != NULL))
    {
        /* Expected Json: {"State":"Hello","Mode":"Broadcast"} */
        if (!strcmp(json_item->valuestring, "Hello"))
        {
            json_item_mode = cJSON_GetObjectItemCaseSensitive(parsed_message, "Mode");
            if (cJSON_IsString(json_item_mode) && (json_item_mode->valuestring != NULL) &&
                (!strcmp(json_item_mode->valuestring, "Broadcast")))
            {
                *message_type = HELLO_REQ;
            }
        }
        /* Expected Json {"State":"ConfirmationRequest"} */
        else if (!strcmp(json_item->valuestring, "ConfirmationRequest"))
        {
            *message_type = CONFIRMATION_REQ;
        }

        /* create the reply payload only if a reply is needed */
        if (!query_send_reply(*message_type, NULL))
        {
            status = *message_type == HELLO_REQ ? hello_response_create(message, buffer_len) :
                                                  confirmation_response_create(message, buffer_len);
        }

        goto end;
    }

    RGB_LED_SetColor(LED_COLOR_PURPLE);
    /* Check if a message containing the auth code was received */
    json_item_auth_code = cJSON_GetObjectItemCaseSensitive(parsed_message, "AuthCode");
    json_item_client_id = cJSON_GetObjectItemCaseSensitive(parsed_message, "ClientID");
    json_item_redirect_uri = cJSON_GetObjectItemCaseSensitive(parsed_message, "RedirectURI");

    if (!json_item_auth_code || !json_item_client_id || !json_item_redirect_uri || !json_item_auth_code->valuestring ||
        !json_item_client_id->valuestring || !json_item_redirect_uri->valuestring ||
        cJSON_GetArraySize(parsed_message) != 3)
    {
        goto end;
    }

    *message_type = AUTH_CODE;

    /* get auth code */

    if (g_lwa_auth_data.auth_code)
    {
        vPortFree(g_lwa_auth_data.auth_code);
    }

    uint16_t stringlen = strlen(json_item_auth_code->valuestring);
    g_lwa_auth_data.auth_code = pvPortMalloc(stringlen + 1);

    if (!g_lwa_auth_data.auth_code)
    {
        goto end;
    }
    else
    {
        memcpy(g_lwa_auth_data.auth_code, json_item_auth_code->valuestring, stringlen);
        g_lwa_auth_data.auth_code[stringlen] = '\0';
    }

    /* get client id */

    if (g_lwa_auth_data.client_id)
    {
        vPortFree(g_lwa_auth_data.client_id);
    }

    stringlen = strlen(json_item_client_id->valuestring);
    g_lwa_auth_data.client_id = pvPortMalloc(stringlen + 1);

    if (!g_lwa_auth_data.client_id)
    {
        goto end;
    }
    else
    {
        memcpy(g_lwa_auth_data.client_id, json_item_client_id->valuestring, stringlen);
        g_lwa_auth_data.client_id[stringlen] = '\0';
    }

    /* get redirect uri */

    if (g_lwa_auth_data.redirect_uri)
    {
        vPortFree(g_lwa_auth_data.redirect_uri);
    }

    stringlen = strlen(json_item_redirect_uri->valuestring);
    g_lwa_auth_data.redirect_uri = pvPortMalloc(stringlen + 1);

    if (!g_lwa_auth_data.redirect_uri)
    {
        goto end;
    }
    else
    {
        memcpy(g_lwa_auth_data.redirect_uri, json_item_redirect_uri->valuestring, stringlen);
        g_lwa_auth_data.redirect_uri[stringlen] = '\0';
    }

    g_auth_code_received = 1;
    status = 0;

end:
    cJSON_Delete(parsed_message);
    if (*message_type == AUTH_CODE && status == 1)
    {
        vPortFree(g_lwa_auth_data.auth_code);
        vPortFree(g_lwa_auth_data.client_id);
        vPortFree(g_lwa_auth_data.redirect_uri);
    }
    return status;
}

#if USE_WIFI_CONNECTION
/**
 * @brief Function which tries to parse a message received by the UDP server in AP mode
 *
 * @param message[in/out]   Pointer towards the buffer containing the received string
 *                          The reply JSON will be written in the same buffer
 * @param buffer_len[in]    Length of the buffer
 * @param message_type[out] Pointer for storing the message type
 * @return                  0 when succeeded, 1 otherwise
 * @note                    JSON messages are expected
 */
static uint8_t apmode_parse_message(void *message, uint16_t buffer_len, companion_message_t *message_type)
{
    cJSON *parsed_message = NULL;
    uint8_t status = 1;
    const cJSON *json_item1 = NULL;
    const cJSON *json_item2 = NULL;

    parsed_message = cJSON_Parse(message);
    if (parsed_message == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            configPRINTF(("Error at cJSON_Parse: %s\n", error_ptr));
        }
        goto exit;
    }

    /* TODO For production, the Hello request messages will be encrypted,
     *      so it'll be needed to decrypt them here */

    /* Check if AP_MODE or WIFI_CRED message was received */
    if (cJSON_GetArraySize(parsed_message) == 2)
    {
        /* Expected Json: {"State":"Hello","Mode":"Broadcast"} */
        json_item1 = cJSON_GetObjectItemCaseSensitive(parsed_message, "State");
        json_item2 = cJSON_GetObjectItemCaseSensitive(parsed_message, "Mode");
        if (cJSON_IsString(json_item1) && (json_item1->valuestring != NULL) &&
            (!strcmp(json_item1->valuestring, "Hello")) && cJSON_IsString(json_item2) &&
            (json_item2->valuestring != NULL) && (!strcmp(json_item2->valuestring, "AP Mode")))
        {
            *message_type = AP_MODE;
            status = apmode_response_create(message, buffer_len);

            goto exit;
        }

        /* Expected Json {"SSID":"<ssid>","Password":"<password>"} */
        json_item1 = cJSON_GetObjectItemCaseSensitive(parsed_message, "SSID");
        json_item2 = cJSON_GetObjectItemCaseSensitive(parsed_message, "Password");
        if (cJSON_IsString(json_item1) && (json_item1->valuestring != NULL) && cJSON_IsString(json_item2) &&
            (json_item2->valuestring != NULL))
        {
            wifi_cred_t wifi_cred = {0};
            status_t credsave_status = 1;

            *message_type = WIFI_CRED;
            status = status_message_create(message, buffer_len, CONN_COMPLETE);

            configPRINTF(("Wifi credentials received: SSID '%s', Password '%s'\r\n", json_item1->valuestring,
                          json_item2->valuestring));

            wifi_cred.ssid.length = strlen(json_item1->valuestring);
            wifi_cred.password.length = strlen(json_item2->valuestring);

            if (wifi_cred.ssid.length <= sizeof(wifi_cred.ssid.value))
            {
                memcpy(wifi_cred.ssid.value, json_item1->valuestring, wifi_cred.ssid.length);
            }
            else
            {
                configPRINTF(("Cannot get wifi SSID\r\n"));
                goto exit;
            }

            if (wifi_cred.password.length <= sizeof(wifi_cred.password.value))
            {
                memcpy(wifi_cred.password.value, json_item2->valuestring, wifi_cred.password.length);
            }
            else
            {
                configPRINTF(("Cannot get wifi Password\r\n"));
                goto exit;
            }

            credsave_status = wifi_credentials_flash_set(&wifi_cred);
            if (!credsave_status)
            {
                configPRINTF(("Wifi credentials saved in flash\r\n"));
            }
            else
            {
                configPRINTF(("Failed to write wifi credentials in flash, error code %d\r\n", credsave_status));
            }
        }
    }

exit:
    cJSON_Delete(parsed_message);
    return status;
}

#endif
/**
 * @brief Creates a hello response in the form of a JSON string: {“DeviceName”: “<Serial Number>”}
 *
 * @param buffer[in/out]   Address of the buffer where the resulting string should be stored
 * @param buffer_len[in]   Length of the buffer
 * @return                 0 when succeeded, 1 otherwise
 */
#if 0
static uint8_t hello_response_create(void *buffer, uint16_t buffer_len)
{
    uint8_t ret = 1;
    uint16_t json_len = 0;
    cJSON *json = NULL;
    cJSON *voice_name = NULL;
    char *string = NULL;

    json = cJSON_CreateObject();
    if (json == NULL)
    {
        goto end;
    }

    read_serial_number();
    voice_name = cJSON_CreateString(g_serial_number.serialNumber);
    if (voice_name == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(json, "DeviceName", voice_name);

    string = cJSON_PrintUnformatted(json);
    if (string == NULL)
    {
        goto end;
    }

    json_len = strlen(string);
    if (buffer_len <= json_len)
    {
        goto end;
    }

    ret = 0;
    memcpy(buffer, string, json_len);
    ((char *)buffer)[json_len] = '\0';

end:
    cJSON_Delete(json);
    cJSON_free(string);
    return ret;
}
#else
/**
 * @brief Creates a hello response in the form of a JSON string:
 *        {​“DSN”: “<Device Serial Number>”,​ “ProductID”: “<Device Product ID>”,​ “CodeChallenge”: “<Encoded Code
 * Challenge>”, “state":“Hello”​}
 *
 * @param buffer[in/out]   Address of the buffer where the resulting string should be stored
 * @param buffer_len[in]   Length of the buffer
 * @return                 0 when succeeded, 1 otherwise
 */
static uint8_t hello_response_create(void *buffer, uint16_t buffer_len)
{
    uint8_t ret = 1;
    uint16_t json_len = 0;
    cJSON *json = NULL;
    cJSON *dsn = NULL;
    cJSON *productId = NULL;
    cJSON *codeChallenge = NULL;
    cJSON *voice_state = NULL;
    char *string = NULL;
    char *serialNumber = NULL;

    json = cJSON_CreateObject();
    if (json == NULL)
    {
        goto end;
    }

    APP_GetUniqueID(&serialNumber, false);
    dsn = cJSON_CreateString(serialNumber);
    if (dsn == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(json, "DSN", dsn);

    productId = cJSON_CreateString(clientcredentialIOT_PRODUCT_NAME);
    if (productId == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(json, "ProductID", productId);

    if (generate_code_challenge())
    {
        goto end;
    }
    codeChallenge = cJSON_CreateString(g_code_challenge_buffer);
    if (codeChallenge == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(json, "CodeChallenge", codeChallenge);

    voice_state = cJSON_CreateString("Hello");
    if (voice_state == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(json, "state", voice_state);

    string = cJSON_PrintUnformatted(json);
    if (string == NULL)
    {
        goto end;
    }

    json_len = strlen(string);
    if (buffer_len <= json_len)
    {
        goto end;
    }

    ret = 0;
    memcpy(buffer, string, json_len);
    ((char *)buffer)[json_len] = '\0';

end:
    cJSON_Delete(json);
    cJSON_free(string);
    vPortFree(serialNumber);
    return ret;
}
#endif

/**
 * @brief Creates a confirmation response in the form of a JSON string:
 *        {​“DSN”: “<Device Serial Number>”,​ “State”: “ConfirmationReply”​}
 *
 * @param buffer[in/out]   Address of the buffer where the resulting string should be stored
 * @param buffer_len[in]   Length of the buffer
 * @return                 0 when succeeded, 1 otherwise
 */
static uint8_t confirmation_response_create(void *buffer, uint16_t buffer_len)
{
    uint8_t ret = 1;
    uint16_t json_len = 0;
    cJSON *json = NULL;
    cJSON *dsn = NULL;
    cJSON *state = NULL;
    char *string = NULL;
    char *serialNumber = NULL;

    json = cJSON_CreateObject();
    if (json == NULL)
    {
        goto end;
    }

    APP_GetUniqueID(&serialNumber, false);
    dsn = cJSON_CreateString(serialNumber);
    if (dsn == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(json, "DSN", dsn);

    state = cJSON_CreateString("ConfirmationReply");
    if (state == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(json, "State", state);

    string = cJSON_PrintUnformatted(json);
    if (string == NULL)
    {
        goto end;
    }

    json_len = strlen(string);
    if (buffer_len <= json_len)
    {
        goto end;
    }

    ret = 0;
    memcpy(buffer, string, json_len);
    ((char *)buffer)[json_len] = '\0';

end:
    cJSON_Delete(json);
    cJSON_free(string);
    vPortFree(serialNumber);
    return ret;
}

/**
 * @brief Creates an AP Mode response in the form of a JSON string:
 *        {​"metadata": "metadata",​ "DSN": "<Device Serial Number>"​}
 *
 * @param buffer[in/out]   Address of the buffer where the resulting string should be stored
 * @param buffer_len[in]   Length of the buffer
 * @return                 0 when succeeded, 1 otherwise
 */
static uint8_t apmode_response_create(void *buffer, uint16_t buffer_len)
{
    uint8_t ret = 1;
    uint16_t json_len = 0;
    cJSON *json = NULL;
    cJSON *dsn = NULL;
    cJSON *state = NULL;
    char *string = NULL;
    char *serialNumber = NULL;

    json = cJSON_CreateObject();
    if (json == NULL)
    {
        goto end;
    }

    state = cJSON_CreateString("metadata");
    if (state == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(json, "metadata", state);

    APP_GetUniqueID(&serialNumber, false);
    dsn = cJSON_CreateString(serialNumber);
    if (dsn == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(json, "DSN", dsn);

    string = cJSON_PrintUnformatted(json);
    if (string == NULL)
    {
        goto end;
    }

    json_len = strlen(string);
    if (buffer_len <= json_len)
    {
        goto end;
    }

    ret = 0;
    memcpy(buffer, string, json_len);
    ((char *)buffer)[json_len] = '\0';

end:
    cJSON_Delete(json);
    cJSON_free(string);
    vPortFree(serialNumber);
    return ret;
}

uint8_t status_message_create(void *buffer, uint16_t buffer_len, app_status_event_t status)
{
    uint8_t ret = 1;
    uint16_t json_len = 0;
    cJSON *json = NULL;
    cJSON *voice_status = NULL;
    char *string = NULL;
    char *status_string = NULL;

    json = cJSON_CreateObject();
    if (json == NULL)
    {
        goto end;
    }

    switch (status)
    {
        case REGISTERING_REQ:
            status_string = STATUS_REGISTERING_REQUEST_STRING;
            break;
        case REGISTERING_RSP:
            status_string = STATUS_REGISTERING_RESPONSE_STRING;
            break;
        case MQTT_SUBSCRIBED:
            status_string = STATUS_MQTT_SUBSCRIBED_STRING;
            break;
        case CONN_COMPLETE:
            status_string = STATUS_CONNECTION_COMPLETE_STRING;
            break;
        case CONN_ERROR:
            status_string = STATUS_CONNECTION_ERROR_STRING;
            break;
    }

    voice_status = cJSON_CreateString(status_string);
    if (voice_status == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(json, "Status", voice_status);

    string = cJSON_PrintUnformatted(json);
    if (string == NULL)
    {
        goto end;
    }

    json_len = strlen(string);
    if (buffer_len <= json_len)
    {
        goto end;
    }

    ret = 0;
    memcpy(buffer, string, json_len);
    ((char *)buffer)[json_len] = '\0';

end:
    cJSON_Delete(json);
    cJSON_free(string);
    return ret;
}

/**
 * @brief Called from message_handle to check if a reply message is needed
 *        Also has a role in setting the type of the next expected message
 * @param message[in]         The type of the received message
 * @param next_message[out]   Pointer to a variable where the next expected message type is written
 *                            NULL is accepted as well
 * @return                    1 when a reply is not needed, 0 otherwise
 */
static uint8_t query_send_reply(companion_message_t message, companion_message_t *next_message)
{
    uint8_t ret = 1;

    if (message == INVALID_MSG)
        return 1;

    switch (g_expected_message)
    {
        case HELLO_REQ:
            /* Send a reply when expecting hello broadcast only when receiving the hello broadcast.
             * confirmation requests and auth code messages will be ignored */
            if (message == HELLO_REQ)
            {
                ret = 0;
                if (next_message)
                    *next_message = CONFIRMATION_REQ;
            }
            break;
        case CONFIRMATION_REQ:
            /* send a reply when expecting confirmation request if:
             * - hello request received (maybe the companion app restarted or the hello rsp UDP was lost.
             *   A hello response will be sent.)
             * - confirmation request received
             * Auth code messages will be ignored */
            if (message == HELLO_REQ || message == CONFIRMATION_REQ)
            {
                if (next_message)
                    *next_message = message == HELLO_REQ ? CONFIRMATION_REQ : AUTH_CODE;
                ret = 0;
            }
            break;
        case AUTH_CODE:
            /* send a reply when expecting auth code messages if:
             * - hello request received (maybe the companion app restarted. A hello response will be sent.)
             * - confirmation request received (maybe the confirmation reply UDP was lost. A confirmation rsp will be
             * sent.)
             * - auth code message received, which is nice, 'cause we've been exxpecting it */
            if (message == HELLO_REQ)
            {
                if (next_message)
                    *next_message = CONFIRMATION_REQ;
                ret = 0;
            }
            else if (message == CONFIRMATION_REQ)
            {
                if (next_message)
                    *next_message = AUTH_CODE;
                ret = 0;
            }
            else if (message == AUTH_CODE)
            {
                /* Do not send replies after receiving auth code message */
                if (next_message)
                    *next_message = HELLO_REQ;
            }

            /* TODO stop auth timer here if active ? code challenge may be overwritten otherwise, bad bad bad ... */
            break;
        default:
            /* should never happen */
            configPRINTF(("Strange value for expected companion message: %d\n", g_expected_message));
    }

    return ret;
}

/**
 * @brief Generates a code challenge following the rfc7636 instructions
 *        Initializes DCP and TRNG for enhanced entropy
 * @return                 0 when succeeded, 1 otherwise
 */
static uint8_t generate_code_challenge(void)
{
    uint8_t random_data[32] = {0};
    size_t base64_len = 0;
    uint8_t base64_data[64] = {0};
    uint8_t sha256_data[32] = {0};
    char *index = NULL;

    if (!g_crypto_initialized)
    {
        CRYPTO_InitHardware();
        g_crypto_initialized = 1;

        /* get one batch of random data here; I saw that the first call after a reset gets the same data ... */
        TRNG_GetRandomData(((TRNG_Type *)TRNG_BASE), random_data, 32);
    }

    if (TRNG_GetRandomData(((TRNG_Type *)TRNG_BASE), random_data, 32))
    {
        configPRINTF(("TRNG_GetRandomData failed\n"));
        return 1;
    }

    if (mbedtls_base64_encode(base64_data, sizeof(base64_data), &base64_len, random_data, sizeof(random_data)))
    {
        configPRINTF(("mbedtls_base64_encode failed\n"));
        return 1;
    }

    /* for the code below, check https://tools.ietf.org/html/rfc7636 */

    /* get rid of trailing '='s */
    strtok((char *)base64_data, "=");
    base64_len--;

    /* '+' is not accepted in rfc7637, replace it with the next accepted char */
    index = strchr((char *)base64_data, '+');
    while (index)
    {
        *index = '-';
        index = strchr(index + 1, '+');
    }

    /* '/' is not accepted in rfc7637, replace it with the next accepted char */
    index = strchr((char *)base64_data, '/');
    while (index)
    {
        *index = '_';
        index = strchr(index + 1, '/');
    }

    /* what we have obtained so far is the code verifier */
    /* we save it and move further to create the code challenge */
    memcpy(g_code_verifier_buffer, base64_data, strlen((char *)base64_data));

    /* code_challenge = BASE64URL-ENCODE(SHA256(ASCII(code_verifier))) */

    if (mbedtls_sha256_ret(base64_data, base64_len, sha256_data, 0))
    {
        configPRINTF(("mbedtls_sha256_ret failed\n"));
        return 1;
    }

    if (mbedtls_base64_encode(base64_data, sizeof(base64_data), &base64_len, sha256_data, sizeof(sha256_data)))
    {
        configPRINTF(("mbedtls_base64_encode failed\n"));
        return 1;
    }

    /* get rid of trailing '='s */
    strtok((char *)base64_data, "=");
    base64_len--;

    /* '+' is not accepted in rfc7637, replace it with the next accepted char */
    index = strchr((char *)base64_data, '+');
    while (index)
    {
        *index = '-';
        index = strchr(index + 1, '+');
    }

    /* '/' is not accepted in rfc7637, replace it with the next accepted char */
    index = strchr((char *)base64_data, '/');
    while (index)
    {
        *index = '_';
        index = strchr(index + 1, '/');
    }

    /* done, we have a code challenge! */
    memcpy(g_code_challenge_buffer, base64_data, strlen((char *)base64_data));

    return 0;
}

lwa_auth_data_t *get_lwa_auth_data()
{
    return &g_lwa_auth_data;
}

char *get_generated_code_challenge()
{
    return g_code_challenge_buffer;
}

char *get_generated_code_verifier()
{
    return g_code_verifier_buffer;
}
