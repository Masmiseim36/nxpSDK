/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "ais_authentication_task.h"

#include "sln_RT10xx_RGB_LED_driver.h"
#include "ux_attention_system.h"

static TaskHandle_t s_appInitTask = NULL;
static char *g_refresh_token      = NULL;

/** @brief Creates a http string request for obtaining tokens from amazon tokens endpoint
 *
 *  @param https_buf[out]           Address of a pre-allocated buffer where the resulting http request will be stored
 *  @param https_buf_len[in]        Size of the pre-allocated buffer
 *  @return                         length of the request on success, -1 otherwise
 */
static int create_tokens_http_request(char *https_buf, int https_buf_len)
{
    cJSON *json          = NULL;
    cJSON *grant_type    = NULL;
    cJSON *code          = NULL;
    cJSON *redirect_uri  = NULL;
    cJSON *client_id     = NULL;
    cJSON *code_verifier = NULL;
    char *string         = NULL;

    lwa_auth_data_t *auth_data = get_lwa_auth_data();
    if (auth_data == NULL)
    {
        configPRINTF(("Fail to get auth_data\r\n"));
        goto exit;
    }

    char *verifier = get_generated_code_verifier();
    if (verifier == NULL)
    {
        configPRINTF(("Fail to get verifier\r\n"));
        goto exit;
    }

    /* create content json */
    json = cJSON_CreateObject();
    if (json == NULL)
    {
        goto exit;
    }

    grant_type = cJSON_CreateString("authorization_code");
    if (grant_type == NULL)
    {
        goto exit;
    }
    cJSON_AddItemToObject(json, "grant_type", grant_type);

    code = cJSON_CreateString(auth_data->auth_code);
    if (code == NULL)
    {
        goto exit;
    }
    cJSON_AddItemToObject(json, "code", code);

    redirect_uri = cJSON_CreateString(auth_data->redirect_uri);
    if (redirect_uri == NULL)
    {
        goto exit;
    }
    cJSON_AddItemToObject(json, "redirect_uri", redirect_uri);

    client_id = cJSON_CreateString(auth_data->client_id);
    if (client_id == NULL)
    {
        goto exit;
    }
    cJSON_AddItemToObject(json, "client_id", client_id);

    code_verifier = cJSON_CreateString(verifier);
    if (code_verifier == NULL)
    {
        goto exit;
    }
    cJSON_AddItemToObject(json, "code_verifier", code_verifier);

    string = cJSON_PrintUnformatted(json);
    if (string == NULL)
    {
        goto exit;
    }

    /* we built the json content, let's create the HTTP request */
    int len = snprintf((char *)https_buf, https_buf_len,
                       "POST /auth/o2/token HTTP/1.1\r\n"
                       "Host: api.amazon.com\r\n"
                       "Content-Type: application/json\r\n"
                       "Content-Length: %d\r\n\r\n"
                       "%s",
                       strlen(string), string);

    /* cleanup */
    cJSON_Delete(json);
    cJSON_free(string);

    return len;

exit:
    cJSON_Delete(json);
    cJSON_free(string);
    return -1;
}

/** @brief Parses the http reply received after the tokens request and copies the refresh_token from it
 *
 *  @param https_buf[in]            Address of the buffer where the http reply is stored
 *  @param refresh_token[out]        Address of a pointer which will be set to the address of a new allocated buffer
 *                                  where the refresh_token will be copied
 *  @return                         0 on success, -1 otherwise
 *
 *  @note                           allocates memory, caller responsible for freeing it!
 */
static int get_token_from_https_reply(char *https_buf, char **refresh_token)
{
    const cJSON *json_item = NULL;

    char *json_buf_start         = strstr(https_buf, "{\"access_token\":\"");
    cJSON *parsed_tokens_message = cJSON_Parse(json_buf_start);

    if (parsed_tokens_message == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            configPRINTF(("Error at cJSON_Parse: %s\r\n", error_ptr));
        }
        return -1;
    }

    json_item = cJSON_GetObjectItemCaseSensitive(parsed_tokens_message, "refresh_token");
    if (cJSON_IsString(json_item) && json_item->valuestring != NULL)
    {
        uint16_t refresh_token_len = strlen(json_item->valuestring);
        char *refresh_token_temp   = pvPortCalloc(refresh_token_len + 1, 1);
        assert(refresh_token_temp);
        memcpy(refresh_token_temp, json_item->valuestring, refresh_token_len);
        *refresh_token = refresh_token_temp;
    }
    else
    {
        configPRINTF(("Did not find access_token element in amazon reply\r\n"));
        return -1;
    }

    return 0;
}

void ais_authentication_set_app_init_task_handle(TaskHandle_t *handle)
{
    if (NULL != handle)
    {
        s_appInitTask = *handle;
    }
}

char *refresh_token_get(void)
{
    return g_refresh_token;
}

void refresh_token_free(void)
{
    vPortFree(g_refresh_token);
}

void ais_authentication_task(void *arg)
{
    BaseType_t xReturned;
    TaskHandle_t own_task_handle      = xTaskGetCurrentTaskHandle();
    char *https_buf                   = NULL;
    int ret                           = 0;
    udp_server_args_t udp_server_args = {NETWORK_MODE_STATION, NULL};

    /* Set LED Blue */
    ux_attention_set_state(uxDiscovery);

    udp_server_set_ais_auth_task_handle(&own_task_handle);
    xReturned = xTaskCreate(udp_server_task, "UDP_Server_Task", DEFAULT_THREAD_STACKSIZE, &udp_server_args,
                            DEFAULT_THREAD_PRIO, NULL);
    assert(xReturned == pdPASS);

    /* suspend the task here, wait for the udp server task to wake it */
    vTaskSuspend(NULL);

    /* got awaken, lwa auth data obtained, let's use it */

    /* Set LED Yellow */
    // RGB_LED_SetColor(LED_COLOR_YELLOW);
    ux_attention_set_state(uxAuthentication);

    /* initialize https client */
    ret = https_client_tls_init();
    if (ret != SUCCESS)
    {
        configPRINTF(("Fail to initialize HTTPS client\r\n"));
        goto exit;
    }

    /* create the http request */
    https_buf = pvPortMalloc(HTTPS_REQUEST_BUFFER_LEN);
    if (https_buf == NULL)
    {
        configPRINTF(("Fail to allocate https_buf\r\n"));
        goto exit;
    }

    memset(https_buf, 0, HTTPS_REQUEST_BUFFER_LEN);
    ret = create_tokens_http_request(https_buf, HTTPS_REQUEST_BUFFER_LEN);
    if (ret < 0)
    {
        configPRINTF(("Fail to create the HTTP request\r\n"));
        goto exit;
    }

    /* send the request, read the reply */
    write_request(https_buf, ret);
    vTaskDelay(portTICK_PERIOD_MS * 1000);

    memset(https_buf, 0, HTTPS_REQUEST_BUFFER_LEN);
    ret = read_request(https_buf, HTTPS_REQUEST_BUFFER_LEN);
    if (ret < 0)
    {
        goto exit;
    }

    /* un-initialize https client */
    https_client_tls_release();

    ret = get_token_from_https_reply(https_buf, &g_refresh_token);
    if (ret < 0)
    {
        goto exit;
    }
    /* token can now be obtained using access_token_get calls */

    /* awake the appInit task */
    vTaskResume(s_appInitTask);

exit:
    /* cleanup */
    vPortFree(https_buf);

    /* good bye, cruel world! */
    vTaskDelete(NULL);
}
