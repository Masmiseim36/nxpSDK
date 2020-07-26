/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "udp_server.h"

#include "lwip/opt.h"

#if LWIP_NETCONN

#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/udp.h"

static TaskHandle_t s_aisAuthTask               = NULL;
static EventGroupHandle_t s_AppStatusEventGroup = NULL;
ip_addr_t companion_app_ipaddr                  = {0};

void udp_server_task(void *arg)
{
    struct netconn *conn;
    struct netbuf *buf_recv;
    err_t err;
    companion_message_t message_type = INVALID_MSG;
    udp_server_args_t *server_args   = (udp_server_args_t *)arg;

#if LWIP_IPV6
    conn = netconn_new(NETCONN_UDP_IPV6);
    netconn_bind(conn, IP6_ADDR_ANY, 8888);
#else  /* LWIP_IPV6 */
    conn = netconn_new(NETCONN_UDP);
    netconn_bind(conn, IP_ADDR_ANY, 8888);
#endif /* LWIP_IPV6 */
    LWIP_ERROR("udpecho: invalid conn", (conn != NULL), return;);

    configPRINTF(("Starting UDP server, expecting companion app messages\r\n"));

#if USE_WIFI_CONNECTION
    if ((NULL == server_args) ||
        ((NETWORK_MODE_AP == server_args->network_mode) && (NULL == server_args->ap_close_event)))
#else
    if ((NULL == server_args))
#endif
    {
        configPRINTF(("Wrong task arguments\r\n"));
        goto stop_task;
    }

    while (1)
    {
        err = netconn_recv(conn, &buf_recv);
        if (err == ERR_OK)
        {
            /* limited memory available, do not parse messages bigger than 512 */
            if (buf_recv->p->tot_len <= MAX_MESSAGE_LEN)
            {
                if (NETWORK_MODE_STATION == server_args->network_mode)
                {
                    message_type = message_handle(conn, buf_recv);
                }
#if USE_WIFI_CONNECTION
                else if (NETWORK_MODE_AP == server_args->network_mode)
                {
                    message_type = apmode_message_handle(conn, buf_recv);
                }
#endif
            }

            /* break the loop when received auth code message */
            if ((message_type == AUTH_CODE) && (NETWORK_MODE_STATION == server_args->network_mode))
            {
                /* back-up the ip of the device where companion app runs */
                ip_addr_set(&companion_app_ipaddr, &buf_recv->addr);
                netbuf_delete(buf_recv);

                /* create the companion_app_status_task before
                 * current task deletion */
                xTaskCreate(companion_app_status_task, "Comp_App_Stat", 512, NULL, configMAX_PRIORITIES - 1, NULL);

                break;
            }
#if USE_WIFI_CONNECTION
            /* break the loop when received wifi_cred */
            else if ((message_type == WIFI_CRED) && (NETWORK_MODE_AP == server_args->network_mode))
            {
                configPRINTF(("WiFi credentials were received; Set the ap_close_event bit\r\n"));

                /* Signals to stop the AP Mode */
                xEventGroupSetBits(server_args->ap_close_event, 1);

                netbuf_delete(buf_recv);
                break;
            }
            /* Check if update wifi_cred message received in discover mode */
            else if ((message_type == INVALID_MSG) && (NETWORK_MODE_STATION == server_args->network_mode))
            {
                message_type = apmode_message_handle(conn, buf_recv);
                if (message_type == WIFI_CRED)
                {
                    configPRINTF(("WiFi credentials were updated; Reset the board\r\n"));
                    vTaskDelay(portTICK_PERIOD_MS * 500);
                    NVIC_SystemReset();
                }
            }
#endif
            netbuf_delete(buf_recv);
        }
    }

stop_task:
    configPRINTF(("Stop udp server and delete task\r\n"));

    /* clean resources */
    netconn_delete(conn);

    /* wake ais auth task */
    vTaskResume(s_aisAuthTask);

    /* good bye, cruel world! */
    vTaskDelete(NULL);
}

void companion_app_status_task(void *arg)
{
    err_t err;
    volatile EventBits_t appStatusEvents = 0U;
    char udp_send_payload[50]            = {0};
    struct udp_pcb *pcb                  = udp_new();
    app_status_event_t status;

    LWIP_ERROR("companion_app_status_task: invalid p_udp_pcb", (pcb != NULL), return;);

    err = udp_connect(pcb, &companion_app_ipaddr, 8888);
    if (err != ERR_OK)
    {
        configPRINTF(("udp_connect failed, error %d\r\n", err));
    }

    s_AppStatusEventGroup = xEventGroupCreate();

    while (1)
    {
        /* sleep and wait for new status */
        appStatusEvents = xEventGroupWaitBits(s_AppStatusEventGroup, 0x00FFFFFF, pdTRUE, pdFALSE, portMAX_DELAY);

        /* check why got awaken */
        if (appStatusEvents & REGISTERING_REQ)
        {
            status = REGISTERING_REQ;
        }
        else if (appStatusEvents & REGISTERING_RSP)
        {
            status = REGISTERING_RSP;
        }
        else if (appStatusEvents & MQTT_SUBSCRIBED)
        {
            status = MQTT_SUBSCRIBED;
        }
        else if (appStatusEvents & CONN_COMPLETE)
        {
            status = CONN_COMPLETE;
        }
        else if (appStatusEvents & CONN_ERROR)
        {
            status = CONN_ERROR;
        }
        else
        {
            configPRINTF(("strange status received: %d\r\n", appStatusEvents));
            continue;
        }

        /* create companion app json */
        if (status_message_create(udp_send_payload, sizeof(udp_send_payload), status))
        {
            configPRINTF(("status_message_create failed\r\n"));
            break;
        }

        /* create pbuf to store the payload */
        struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, strlen(udp_send_payload), PBUF_RAM);
        if (!p)
        {
            configPRINTF(("pbuf_alloc failed\r\n"));
            break;
        }
        memcpy(p->payload, udp_send_payload, strlen(udp_send_payload));

        configPRINTF(("Sending status message: %s\r\n", udp_send_payload));

        /* send the package */
        err = udp_send(pcb, p);
        if (err != ERR_OK)
        {
            configPRINTF(("udp_send failed, error %d\r\n", err));
        }

        /* cleaning up is the nice thing to do */
        pbuf_free(p);

        /* nice talking to you */
        if (status == CONN_COMPLETE || status == CONN_ERROR)
        {
            break;
        }
    }

    /* clean event group */
    vEventGroupDelete(s_AppStatusEventGroup);

    /* clean pcb resources */
    udp_remove(pcb);

    /* good bye, cruel world! */
    vTaskDelete(NULL);
}

void udp_server_set_ais_auth_task_handle(TaskHandle_t *handle)
{
    if (NULL != handle)
    {
        s_aisAuthTask = *handle;
    }
}

void companion_app_status_set(app_status_event_t status)
{
    if (s_AppStatusEventGroup)
    {
        xEventGroupSetBits(s_AppStatusEventGroup, status);
    }
}

#endif /* LWIP_NETCONN */
