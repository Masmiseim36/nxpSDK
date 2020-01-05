/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include <stdint.h>
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
/* Application includes */
#include <stdio.h>
#include "board.h"
#include "fsl_debug_console.h"

#include "iperf_api.h"
#define IPERF_DEFAULT_DURATION 10

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

/* System clock name. */
#define EXAMPLE_CLOCK_NAME kCLOCK_CoreSysClk

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/

#ifdef IPERF3_ENET

/* IP address configuration. */
#define configIP_ADDR0 192
#define configIP_ADDR1 168
#define configIP_ADDR2 0
#define configIP_ADDR3 102

/* Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 255
#define configNET_MASK3 0

/* Gateway address configuration. */
#define configGW_ADDR0 192
#define configGW_ADDR1 168
#define configGW_ADDR2 0
#define configGW_ADDR3 100

/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }

#endif

// 0 is the highest priority and priority 15 is the lowest priority
const int TASK_MAIN_PRIO       = configMAX_PRIORITIES - 3;
const int TASK_MAIN_STACK_SIZE = 800;

// portSTACK_TYPE *task_main_stack = NULL;
TaskHandle_t task_main_task_handler;
TaskHandle_t recv_task_handler;

// Hardwired SSID, passphrase, auth and cipher algo of AP to connect to
// Change this to fit your AP
#define AP_SSID "nxp"
#define AP_PASSPHRASE "NXP0123456789"

#define CERT_HEADER_LEN sizeof(CERT_HEADER_T)

typedef struct
{
    char id[4];
    uint32_t length;
    uint8_t data[1]; // NOTE: previously 0
} CERT_HEADER_T;

extern int numIrqs;
extern int initTime;

char input;
char *buffer_iperf_data = NULL;
char *json_buf;
TimerHandle_t iperf_timer = NULL;
void *tmp_addr            = NULL;
char json_results[OUTPUT_STR_MAX];

char json_format[OUTPUT_STR_MAX] =
    "{"
    "\"cpu_util_total\":0,"
    "\"cpu_util_user\":0,"
    "\"cpu_util_system\":0,"
    "\"sender_has_retransmits\":-1,"
    "\"congestion_used\":\"N/A\","
    "\"streams\": [{\"id\":1,"
    "\"start_time\": 0,"
    "\"bytes\": %d,"
    "\"retransmits\": 0,"
    "\"jitter\": 0,"
    "\"errors\": 0,"
    "\"packets\": %d,"
    "\"end_time\": 10}]}";

void separator()
{
    PRINTF("\r\n===========================================================\r\n");
}

struct iperf_ctx iperf_test = {
    //.server_ip = {192, 168, 2, 115},
    .server_ip = {0},
};

void iperf_init(struct iperf_ctx *ctx)
{
    iperf_hw_init(ctx);
}

void menu(struct iperf_ctx *ctx)
{
    char mode_switch;
    int choosing_mode = 1;
    PRINTF(
        "Menu:\r\nPress \'s\' to start client Tx mode\r\n"
        "Press \'r\' to start client Rx mode\r\n"
        "Press \'S\' to start client Tx UDP mode\r\n"
        "Press \'R\' to start client Rx UDP mode\r\n");
    while (choosing_mode)
    {
        mode_switch = getchar();
        switch (mode_switch)
        {
            case 's':
                choosing_mode = 0;
                ctx->mode     = IPERF_CLIENT_TX;
                PRINTF("\n\rTx mode!\r\n");
                break;

            case 'r':
                choosing_mode = 0;
                ctx->mode     = IPERF_CLIENT_RX;
                PRINTF("\n\rRx mode!\r\n");
                break;

            case 'S':
                choosing_mode = 0;
                ctx->mode     = IPERF_CLIENT_TX_UDP;
                PRINTF("\n\rTx UDP mode!\r\n");
                break;

            case 'R':
                choosing_mode = 0;
                ctx->mode     = IPERF_CLIENT_RX_UDP;
                PRINTF("\n\rRx UDP mode!\r\n");
                break;

            default:
                PRINTF("\'%c\' is not valid mode\r\n", mode_switch);
                break;
        }
    }
}
uint32_t iperf_bswap32(uint32_t source)
{
    uint32_t tmp = 0;
    tmp |= ((source & 0xFF) << 24);
    tmp |= ((source & 0xFF00) << 8);
    tmp |= ((source & 0xFF0000) >> 8);
    tmp |= ((source & 0xFF000000) >> 24);
    return tmp;
}

int iperf_send_state(struct iperf_ctx *ctx, uint8_t state)
{
    uint32_t transmitted = 0;
    ctx->ctrl_buf[0]     = state;
    transmitted          = iperf_send(ctx->ctrl_sock, ctx->ctrl_buf, 1, 0);
    assert(1 == transmitted);
    return 1 == transmitted ? 0 : -1;
}

static void iperf_set_state(struct iperf_ctx *ctx, uint8_t state)
{
    ctx->state = state;
}

int iperf_send_and_set_state(struct iperf_ctx *ctx, uint8_t state)
{
    if (0 == iperf_send_state(ctx, state))
    {
        iperf_set_state(ctx, state);
        return 0;
    }
    return -1;
}

int iperf_recv_state(struct iperf_ctx *ctx, uint8_t *state)
{
    int received = 0;
    received     = iperf_recv_noblock(ctx->ctrl_sock, ctx->recv_buf, 1, 0);
    if (received == 1)
    {
        *state = ctx->recv_buf[0];
        return 0;
    }
    return -1;
}

int iperf_recv_and_set_state(struct iperf_ctx *ctx)
{
    uint8_t state = 0;
    if (0 == iperf_recv_state(ctx, &state))
    {
        ctx->state = state;
        if (ctx->state == EXCHANGE_RESULTS)
            return 0;
    }
    return -1;
}

int turn                  = 0;
int juck_bytes            = 0;
int juck_counter          = 0;
int junk                  = 0;
TickType_t start_ticks    = 0;
TickType_t expected_ticks = 0;
TickType_t current_ticks  = 0;

void disposal_of_unprofitable_data_tcp(int dataSocket, struct iperf_ctx *ctx)
{
    juck_bytes = juck_counter = junk = 0;
    start_ticks                      = xTaskGetTickCount();
    expected_ticks                   = start_ticks + 1000 / portTICK_PERIOD_MS;
    do
    {
        junk          = iperf_recv_noblock(dataSocket, ctx->recv_buf, IPERF_BUFFER_MAX, 0);
        juck_counter  = junk > 0 ? 0 : juck_counter + 1;
        current_ticks = xTaskGetTickCount();
    } while (expected_ticks > current_ticks);
}

void disposal_of_unprofitable_data_udp(int dataSocket, struct iperf_ctx *ctx)
{
    juck_bytes = juck_counter = junk = 0;
    start_ticks                      = xTaskGetTickCount();
    expected_ticks                   = start_ticks + 3000 / portTICK_PERIOD_MS;
    do
    {
        junk          = iperf_recv_from(dataSocket, ctx->recv_buf, IPERF_BUFFER_MAX, 0);
        juck_counter  = junk > 0 ? 0 : juck_counter + 1;
        current_ticks = xTaskGetTickCount();

    } while (expected_ticks > current_ticks);
}

uint8_t tmp_state           = 0;
volatile uint8_t tmp_statev = 0;
int g_ignore_start          = 0;

void iperfc_timer_start(struct iperf_ctx *ctx)
{
    BaseType_t result = 0;
    result     = xTimerStart(iperf_timer, 0);
    assert(pdPASS == result);
}

int iperfc_create_streams(struct iperf_ctx *ctx)
{
    int result = 0;
    PRINTF("Creating streams\r\n");
    if (ctx->mode == IPERF_CLIENT_TX || ctx->mode == IPERF_CLIENT_TX_UDP)
    {
        for (int i = 0; i < IPERF_BUFFER_MAX; i++)
        {
            ctx->send_buf[i] = 0;
        }
    }
    ctx->data_sock = iperf_socket(ctx->socket_type);
    if (ctx->socket_type == TCP)
    {
        result = iperf_connect(ctx->data_sock, ctx->ctrl_addr, ctx->ctrl_addr_len);
        assert(0 == result);
        strcpy(ctx->ctrl_buf, MAGIC_COOKIE);
        result = iperf_send(ctx->data_sock, ctx->ctrl_buf, 37, 0);
        assert(37 == result);
        ctx->state = IPERF_NOP;
    }
    else
    {
        /* Send "hello" to server - tells server to start countin */
        strcpy(ctx->send_buf, "hello");
        result = iperf_send_to(ctx->data_sock, ctx->send_buf, IPERF_BUFFER_MAX, 0, ctx->ctrl_addr, ctx->ctrl_addr_len);
        if (ctx->mode != IPERF_CLIENT_TX_UDP)
        {
            /* Wait for "hello" from server */
            do
            {
                result = iperf_recv_from(ctx->data_sock, ctx->recv_buf, IPERF_BUFFER_MAX, 0);
            } while (result != IPERF_BUFFER_MAX);
            /* Force running state, because server sends data immediately */

            ctx->skip_state_start = 1;
            iperfc_timer_start(ctx);
            ctx->state = TEST_RUNNING;
        }
        else
        {
            ctx->state = IPERF_NOP;
        }
    }
    return 0;
}

int iperfc_test_start(struct iperf_ctx *ctx)
{
    if (ctx->skip_state_start)
        return 0;

    PRINTF("Starting test\r\n");
    iperfc_timer_start(ctx);

    if (ctx->mode == IPERF_CLIENT_RX || ctx->mode == IPERF_CLIENT_RX_UDP)
    {
        ctx->state = TEST_RUNNING;
    }
    else
    {
        ctx->state = IPERF_NOP;
    }
    return 0;
}


char *json_req[] = {
    "{\"tcp\":1, \"time\": 10}",
    "{\"tcp\":1, \"reverse\":1, \"time\": 10}",
    "{\"udp\":\"true\", \"len\": 1024, \"time\": 10}",
    "{\"udp\":\"true\", \"reverse\":1, \"len\": 1024, ""\"time\": 10, \"fqrate\": 512}",
};


int iperf_exchange_params(struct iperf_ctx *ctx)
{
    int result = 0;
    int32_t exchange_len = 0;
    PRINTF("Exchanging parameters\r\n");
    switch (ctx->mode)
    {
        case IPERF_CLIENT_TX:
            json_buf         = json_req[0];
            ctx->socket_type = TCP;
            break;
        case IPERF_CLIENT_RX:
            json_buf         = json_req[1];
            ctx->socket_type = TCP;
            break;
        case IPERF_CLIENT_TX_UDP:
            json_buf         = json_req[2];
            ctx->socket_type = UDP;
            break;
        case IPERF_CLIENT_RX_UDP:
            json_buf         = json_req[3];
            ctx->socket_type = UDP;
            break;
    }
    strncpy(&ctx->ctrl_buf[4], json_buf, strlen(json_buf));
    *((uint32_t *)&ctx->ctrl_buf[0]) = iperf_bswap32(strlen(json_buf));
    exchange_len                     = strlen(json_buf) + 4;
    result                           = iperf_send(ctx->ctrl_sock, ctx->ctrl_buf, exchange_len, 0);
    assert(result == exchange_len);
    ctx->state                       = IPERF_NOP;
    return 0;
}

void iperf_running_test(struct iperf_ctx *ctx)
{
    int result = 0;
    if (ctx->mode == IPERF_CLIENT_TX || ctx->mode == IPERF_CLIENT_TX_UDP)
    {
        ctx->send_counter++;
        if (ctx->socket_type == TCP)
        {
            result = iperf_send(ctx->data_sock, ctx->send_buf, IPERF_BUFFER_MAX, 0);
            assert(result == IPERF_BUFFER_MAX);
        }
        else
        {
			/* Prepare header */
			/* seconds */
			*((uint32_t*)&ctx->send_buf[0]) = 0;
			/* usec */
			*((uint32_t*)&ctx->send_buf[4]) = 0;
			/* pcount */
            *((uint32_t*)&ctx->send_buf[8]) = iperf_bswap32((uint32_t)(turn + 1));

            // Iperf server counts UDP packets using it`s data...
            for (int i = 12; i < IPERF_BUFFER_MAX; i++)
            {
                ctx->send_buf[i] = turn;
            }

            turn++;
            result =
                iperf_send_to(ctx->data_sock, ctx->send_buf, IPERF_BUFFER_MAX, 0, ctx->ctrl_addr, ctx->ctrl_addr_len);
        }
        if (result > 0)
        {
            ctx->real_send_buff_counter += result;
        }
    }
    else if (ctx->mode == IPERF_CLIENT_RX || ctx->mode == IPERF_CLIENT_RX_UDP)
    {
        if (ctx->socket_type == TCP)
        {
            result = iperf_recv_blocked(ctx->data_sock, ctx->recv_buf, IPERF_BUFFER_MAX, 0);
        }
        else
        {
            result = iperf_recv_from(ctx->data_sock, ctx->recv_buf, IPERF_BUFFER_MAX, 0);
        }
        if (result > 0)
        {
            ctx->real_recv_buff_counter += result;
        }
    }
}

void iperf_exchange_results(struct iperf_ctx *ctx)
{
    int result        = 0;
    int tmp_len       = 0;
    int bytes_to_recv = 0;
    if (ctx->data_sock != -1)
    {
        iperf_socket_close(ctx->data_sock);
        ctx->data_sock = -1;
    }

    PRINTF("Exchanging results\r\n");
    if (ctx->mode == IPERF_CLIENT_TX || ctx->mode == IPERF_CLIENT_TX_UDP)
    {
        snprintf(json_results, OUTPUT_STR_MAX, json_format, ctx->real_send_buff_counter,
                 ctx->real_send_buff_counter / IPERF_BUFFER_MAX);
    }
    else
    {
        snprintf(json_results, OUTPUT_STR_MAX, json_format, ctx->real_send_buff_counter,
                 ctx->real_send_buff_counter / IPERF_BUFFER_MAX);
    }

    json_buf = json_results;
    tmp_len  = strlen(json_buf);

    assert(tmp_len + 1 + 4 < IPERF_BUFFER_MAX);
    strncpy(&ctx->ctrl_buf[4], json_buf, tmp_len);

    *((uint32_t *)&ctx->ctrl_buf[0]) = iperf_bswap32(strlen(json_buf));
    result                           = iperf_send(ctx->ctrl_sock, ctx->ctrl_buf, tmp_len + 4, 0);

    result = iperf_recv_blocked(ctx->ctrl_sock, ctx->recv_buf, 4, 0);
    assert(result == 4);

    bytes_to_recv = *(uint32_t *)(ctx->recv_buf);
    bytes_to_recv = iperf_bswap32(bytes_to_recv);
    result        = iperf_recv_blocked(ctx->ctrl_sock, ctx->recv_buf, bytes_to_recv, 0);
    assert(result == bytes_to_recv);
    separator();
    PRINTF("Server Results\r\n");
    for (int i = 0; i < result; i++)
    {
        PRINTF("%c", ctx->recv_buf[i]);
        if (ctx->recv_buf[i] == ',')
        {
            PRINTF("\r\n");
        }
    }
    ctx->state = IPERF_NOP;
}

void iperf_end_test(struct iperf_ctx *ctx)
{
    PRINTF("Ending test\r\n");
    iperf_send_state(ctx, TEST_END);
    if (ctx->mode == IPERF_CLIENT_RX || ctx->mode == IPERF_CLIENT_RX_UDP)
    {
        if (ctx->socket_type == TCP)
        {
            disposal_of_unprofitable_data_tcp(ctx->data_sock, ctx);
        }
        else
        {
            vTaskDelay((const TickType_t)10000 / portTICK_PERIOD_MS);
            disposal_of_unprofitable_data_udp(ctx->data_sock, ctx);
        }
    }
    ctx->state = IPERF_NOP;
}

void print_results(struct iperf_ctx *ctx)
{
    separator();
    if (ctx->mode == IPERF_CLIENT_TX || ctx->mode == IPERF_CLIENT_TX_UDP)
    {
        PRINTF(
            "IPERF finished, supposed to send %d KB (%d KiB)(%d Bytes)!\r\n"
            "Transmited %d KB (%d KiB)(%d Bytes).\r\n",
            ((ctx->send_counter * IPERF_BUFFER_MAX) / 1000), ((ctx->send_counter * IPERF_BUFFER_MAX) / 1024),
            ctx->send_counter * IPERF_BUFFER_MAX, (ctx->real_send_buff_counter / 1000),
            (ctx->real_send_buff_counter / 1024), ctx->real_send_buff_counter);
    }
    else
    {
        PRINTF("IPERF finished, Recieved %d KB (%d KiB)!\r\n", (ctx->real_recv_buff_counter / 1000),
               (ctx->real_recv_buff_counter / 1024));
    }
#if 0
    if (ctx->mode == IPERF_CLIENT_TX_UDP)
    {
        PRINTF(
            "If iperf shows OUT-OF-ORDER, check wireshark to make sure that all"
            " packets are in order.\r\n");
    }
#endif
    separator();
}

void iperf_switch_state(struct iperf_ctx *ctx)
{
    int xStatus = 0;
    switch (ctx->state)
    {
        case PARAM_EXCHANGE: // 9
            iperf_exchange_params(ctx);
            break;

        case CREATE_STREAMS: // 10 0xA
            iperfc_create_streams(ctx);
            break;

        case TEST_START: // 1
            iperfc_test_start(ctx);
            break;

        case TEST_RUNNING: // 2
            iperf_running_test(ctx);
            break;

        case EXCHANGE_RESULTS: // 13 0xD
            iperf_exchange_results(ctx);
            break;

        case DISPLAY_RESULTS: // 14 0xE
            ctx->ctrl_buf[0] = IPERF_DONE;
            xStatus          = iperf_send(ctx->ctrl_sock, ctx->ctrl_buf, CTRL_IPERF_MSG_LEN, 0);
            assert(xStatus != 0);
            ctx->iperf_done = 1;
            break;

        case TEST_END: // 4
            iperf_end_test(ctx);
            break;

        case IPERF_START: // 15
            PRINTF("IPERF START..\r\n");
            ctx->state = IPERF_NOP;
            break;

        case IPERF_NOP: //-42
            break;

        case SERVER_TERMINATE:
            PRINTF("SERVER TERMINATED!!!\r\n");
            ctx->iperf_done = 2;
            break;

        case ACCESS_DENIED:
            PRINTF("ACCESS_DENIED!!!\r\n");
            ctx->iperf_done = 2;
            break;

        case SERVER_ERROR:
            PRINTF("ACCESS_DENIED!!!\r\n");
            ctx->iperf_done = 2;
            break;

        default:
            PRINTF("Unexpected state, shutting down!!\r\n");
            ctx->ctrl_buf[0] = CLIENT_TERMINATE;
            xStatus          = iperf_send(ctx->ctrl_sock, ctx->ctrl_buf, CTRL_IPERF_MSG_LEN, 0);
            assert(xStatus != -1);
            ctx->iperf_done = 2;
            break;
    }
}

void get_server_ip(struct iperf_ctx *ctx)
{
    uint32_t safe_scanf_ip[4] = {0};
    int result                = 0;
    do
    {
        PRINTF("Enter IP addr of a server in format '192.168.1.2' \n\r");
        result = SCANF("%d..%d..%d..%d", &safe_scanf_ip[0], &safe_scanf_ip[1], &safe_scanf_ip[2], &safe_scanf_ip[3]);
        if (4 != result)
        {
            PRINTF("\n\rYour IP is not valid, please try again \n\r");
        }
    } while (result != 4);
    /* To avoid stack overflow from user input */
    ctx->server_ip.ip[0] = safe_scanf_ip[0];
    ctx->server_ip.ip[1] = safe_scanf_ip[1];
    ctx->server_ip.ip[2] = safe_scanf_ip[2];
    ctx->server_ip.ip[3] = safe_scanf_ip[3];

    PRINTF("\n\rUsing IP %d.%d.%d.%d \r\n", ctx->server_ip.ip[0], ctx->server_ip.ip[1], ctx->server_ip.ip[2],
           ctx->server_ip.ip[3]);
}

void iperf_run(struct iperf_ctx *ctx)
{
    int xStatus                 = -1;
    char                        c = '\0';
    ctx->iperf_done             = 0;
    ctx->send_counter           = 0;
    ctx->real_send_buff_counter = 0;
    ctx->real_recv_buff_counter = 0;

    menu(&iperf_test);

    do
    {
        ctx->socket_type = TCP;
        ctx->ctrl_sock   = iperf_socket(ctx->socket_type);
        PRINTF("Connecting to the server...\r\n");
        xStatus = iperf_connect(ctx->ctrl_sock, ctx->ctrl_addr, ctx->ctrl_addr_len);
        if (xStatus == -1)
        {
            PRINTF("Failed!\r\n");
            iperf_socket_close(ctx->ctrl_sock);
        }
    } while (xStatus != 0);

    /* Cookie */
    PRINTF("Sending cookie!!...\r\n");
    strcpy(ctx->ctrl_buf, MAGIC_COOKIE);
    xStatus    = iperf_send(ctx->ctrl_sock, ctx->ctrl_buf, 37, 0);
    ctx->state = IPERF_START;

    do
    {
        /* Receive context 'state' from server */
        iperf_recv_and_set_state(ctx);
        iperf_switch_state(ctx);

    } while (!ctx->iperf_done);
    if (ctx->iperf_done == 1)
    {
        print_results(ctx);
    }
    else
    {
        PRINTF("Test ended");
    }
    iperf_socket_close(ctx->ctrl_sock);
    memset(ctx->recv_buf, 0, IPERF_BUFFER_MAX);

    do
    {
        PRINTF("\n\rPRESS \"F\" to restart...\r\n");
        c = getchar();
    } while (c != 'F');
}

/**/
#if defined(IPERF3_WIFI)
#include "iot_wifi.h"
WIFIReturnCode_t xWifiStatus;

void con_to_ap(struct iperf_ctx *ctx)
{
    PRINTF("Trying to connect to AP: %s\r\n", AP_SSID);
    WIFINetworkParams_t xNetworkParams = {0};

    xNetworkParams.pcSSID = AP_SSID;
    assert(NULL != xNetworkParams.pcSSID);

    xNetworkParams.ucSSIDLength = strlen(xNetworkParams.pcSSID);
    xNetworkParams.pcPassword   = AP_PASSPHRASE;

    assert(NULL != xNetworkParams.pcPassword);
    xNetworkParams.ucPasswordLength = strlen(xNetworkParams.pcPassword);

    xNetworkParams.xSecurity = eWiFiSecurityWPA2;
    xWifiStatus              = WIFI_ConnectAP(&(xNetworkParams));

    if (xWifiStatus == eWiFiSuccess)
    {
        uint8_t ucIPAddr[4];
        xWifiStatus = WIFI_GetIP(&ucIPAddr[0]);
        if (xWifiStatus == eWiFiSuccess)
        {
            PRINTF("Connected, device IP: %d.%d.%d.%d \r\n", ucIPAddr[0], ucIPAddr[1], ucIPAddr[2], ucIPAddr[3]);
            while (1)
            {
                iperf_run(ctx);
            }
        }
    }
    else
    {
        PRINTF("Failed to connect to AP.\r\n");
        while (1)
            ;
    }
}

void iperf_startup(struct iperf_ctx *ctx)
{
    xWifiStatus = WIFI_On();
    if (xWifiStatus == eWiFiSuccess)
    {
        WIFIDeviceMode_t xDeviceMode = eWiFiModeStation;
        xWifiStatus                  = WIFI_GetMode(&xDeviceMode);
        if (xWifiStatus == eWiFiSuccess || xWifiStatus == eWiFiNotSupported)
        {
            uint8_t ucMacAddressVal[wificonfigMAX_BSSID_LEN];
            WIFI_GetMAC(&ucMacAddressVal[0]);
            PRINTF("Wifi turned on.\r\nMAC: %x:%x:%x:%x:%x:%x \r\n", ucMacAddressVal[0], ucMacAddressVal[1],
                   ucMacAddressVal[2], ucMacAddressVal[3], ucMacAddressVal[4], ucMacAddressVal[5]);
            con_to_ap(ctx);
        }
    }
    else
    {
        PRINTF("Problem with turning wifi on!\r\n");
    }
}
#elif defined(IPERF3_ENET)
extern void enet_init(void);
void iperf_startup(struct iperf_ctx *ctx)
{
    enet_init();
    while (1)
    {
        iperf_run(ctx);
    }
}
#endif

void task_main(void *arg)
{
    struct iperf_ctx *ctx = (struct iperf_ctx *)arg;

    get_server_ip(ctx);

    iperf_init(ctx);
#if defined(IPERF3_WIFI)
    iperf_startup(ctx);
#elif defined(IPERF3_ENET)
    iperf_startup(ctx);
#endif

    while (1)
        ;
}

void callback_time(TimerHandle_t xTimer)
{
    iperf_test.state = TEST_END;
    if (iperf_test.mode != IPERF_CLIENT_RX && iperf_test.mode != IPERF_CLIENT_RX_UDP)
    {
        iperf_socket_close(iperf_test.data_sock);
        iperf_test.data_sock = -1;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BaseType_t result = 0;
    (void)result;
    SYSMPU_Type *base = SYSMPU;
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Disable SYSMPU. */
    base->CESR &= ~SYSMPU_CESR_VLD_MASK;

    iperf_timer = xTimerCreate("iperf_timer", configTICK_RATE_HZ * 12, pdFALSE, (void *)0, callback_time);
    assert(NULL != iperf_timer);
    result = xTaskCreate(task_main, "main", TASK_MAIN_STACK_SIZE, &iperf_test, TASK_MAIN_PRIO, &task_main_task_handler);

    assert(pdPASS == result);
    vTaskStartScheduler();
    for (;;)
        ;
}
