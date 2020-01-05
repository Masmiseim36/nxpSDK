/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////

// SDK Included Files
#include "board.h"
#include "fsl_debug_console.h"
#include "qcom_api.h"
#include "wlan_qcom.h"

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

// 0 is the highest priority and priority 15 is the lowest priority
const int TASK_MAIN_PRIO       = configMAX_PRIORITIES - 3;
const int TASK_MAIN_STACK_SIZE = 800;

portSTACK_TYPE *task_main_stack = NULL;
TaskHandle_t task_main_task_handler;

// Hardwired SSID, passphrase, auth and cipher algo of AP to connect to
// Change this to fit your AP
#define AP_SSID "nxp"
#define AP_PASSPHRASE "NXP0123456789"

QCOM_SSID g_ssid             = {.ssid = (AP_SSID)};
QCOM_PASSPHRASE g_passphrase = {.passphrase = (AP_PASSPHRASE)};

WLAN_AUTH_MODE g_auth    = WLAN_AUTH_WPA2_PSK;
WLAN_CRYPT_TYPE g_cipher = WLAN_CRYPT_AES_CRYPT;

// ============================================================================
// Menu Handling
// ============================================================================

extern int numIrqs;
extern int initTime;

static void printSeparator(void)
{
    PRINTF("========================================\r\n");
}

static void menuConnectToAP(void)
{
    apConnect(&g_ssid, &g_passphrase, g_auth, g_cipher);
}

static void menuDisconnect(void)
{
    apDisconnect();
}

static void menuPingSomeHost(void)
{
    pingHost("nxp.com");
}

static void menuPingGateway(void)
{
    pingHost(NULL);
}

static void menuUdpServerStart(void)
{
    udpBind(9000);
}

enum STATE
{
    STATE_IDLE,
    STATE_CONNECTING,
    STATE_CONNECTED,
};
int state = STATE_IDLE;

static void menuAllStateMachine(void)
{
    if (state == STATE_IDLE)
        return;

    if (state == STATE_CONNECTING && isConnected())
    {
        state = STATE_CONNECTED;
        getDhcp();
        PRINTF("Connected to AP and IP address assigned\r\n");

        pingHost(NULL);

        menuUdpServerStart();

        state = STATE_IDLE;
    }
}

//#define WWW_SERVER "cr.yp.to"
#define WWW_SERVER "nxp.com"
static void menuGetHttp(void)
{
    httpGet(WWW_SERVER, 2000);
}

static void menuGetHttpGateway(void)
{
    httpGet(NULL, 2000);
}

#if 0
static void menuTraceQcomApi(void)
{
    int was = traceQcomApi(-1);
    PRINTF("QCOM API trace is now %s\r\n", (was ? "DISABLED" : "ENABLED"));
}

static void menuAll(void)
{
    menuConnectToAP();
    state = STATE_CONNECTING;
}
#endif

static void menuSeparator(void)
{
    printSeparator();
    PRINTF("numIrqs = %d, initTime = %d\r\n", numIrqs, initTime);

#if (INCLUDE_uxTaskGetStackHighWaterMark == 1)
    PRINTF("mainTask  watermark=%4d\r\n", uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t));
    PRINTF("drvTask   watermark=%4d\r\n", uxTaskGetStackHighWaterMark(atheros_wifi_task_id) * sizeof(StackType_t));
#endif
    printHeapFree(NULL);

    extern TaskHandle_t atheros_wifi_task_id;
    extern TaskHandle_t task_main_task_handler;
    uint16_t wifiTaskPrio = uxTaskPriorityGet(atheros_wifi_task_id);
    uint16_t mainTaskPrio = uxTaskPriorityGet(task_main_task_handler);
    PRINTF("wifiPrio=%d, mainPrio=%d\r\n", wifiTaskPrio, mainTaskPrio);
}

static void menuExtraHelp(void)
{
    PRINTF("If you are on a Linux work station with a wireless network card, it is easy to create\r\n");
    PRINTF("a WIFI Access Point using the create_ap utility on GitHub:\r\n");
    PRINTF("  git clone https://github.com/oblique/create_ap\r\n");
    PRINTF("  cd create_ap\r\n");
    PRINTF("  make install\r\n");
    PRINTF("Now create the AP, replacing 'wlp2s0' with the name of your wireless interface name\r\n");
    PRINTF("and replacing 'eno1' with the name of your Ethernet interface name\r\n");
    PRINTF("  sudo create_ap wlp2s0 eno1 nxp NXP0123456789\r\n");
    PRINTF("\r\n");
    PRINTF("Now you can connect by pressing 'c' key from the console menu\r\n");
    PRINTF("NOTE that the SSID and passphrase of the AP is hardwired in the example app source\r\n");
    PRINTF("so you need to change the defaults and recompile if you don't have the\r\n");
    PRINTF("possibility to make a WIFI AP using the create_ap utility\r\n");
}

static void menuPrint(void);

typedef struct
{
    char key;
    void (*func)(void);
    const char *text;
} menu_item_t;
menu_item_t menuItems[] = {
    //    {'a', menuAll, "Connect, DHCP, ping gateway, start UDP echo server"},
    {'s', apScan, "AP Scan"},
    {'c', menuConnectToAP, "AP Connect (SSID='" AP_SSID "', pass='" AP_PASSPHRASE "')"},
    {'D', menuDisconnect, "AP Disconnect"},
    {'d', getDhcp, "Get DHCP address"},
    {'g', menuGetHttp, "HTTP GET " WWW_SERVER},
    {'w', menuGetHttpGateway, "HTTP GET from gateway"},
    {'p', menuPingGateway, "Ping gateway"},
    {'P', menuPingSomeHost, "Ping nxp.com"},
    //    {'u', menuUdpServerStart, "Start UDP echo server"},
    {'i', printIpConfig, "Print IP configuration"},
    {'R', resolveManyHosts, "Resolve some hosts"},
    //    {'v', menuTraceQcomApi, "Toggle tracing of QCOM API"},
    {'h', menuPrint, "Help (print this menu)"},
    {'H', menuExtraHelp, "Print extended help"},
    {0, NULL, NULL},
};

static void menuAction(int ch)
{
    if (ch == '\r' || ch == ' ')
    {
        menuSeparator();
        return;
    }

    for (int i = 0; menuItems[i].func != NULL; i++)
    {
        if (menuItems[i].key == ch)
        {
            PRINTF("Key '%c': %s\r\n", ch, menuItems[i].text);
            menuItems[i].func();
            return;
        }
    }
    PRINTF("ERROR: No action bound to '%c'\r\n", ch);
}

static void menuPrint(void)
{
    PRINTF("============ %s ============\r\n", isConnected() ? "CONNECTED" : "NOT connected");

    for (int i = 0; menuItems[i].text != NULL; i++)
    {
        if (menuItems[i].key)
            PRINTF("  %c  %s\r\n", menuItems[i].key, menuItems[i].text);
        else
            PRINTF("  %d  %s\r\n", i, menuItems[i].text);
    }
}

static int pollChar()
{
    //    if (!UART_HAL_GetStatusFlag(BOARD_DEBUG_UART_BASEADDR, kUartRxDataRegFull))
    //        return -1;
    //
    //    uint8_t ch;
    //    UART_HAL_Getchar(BOARD_DEBUG_UART_BASEADDR, &ch);
    //    // Flush the UART to avoid the "OpenSDA UART infinite RX FIFO" bug:
    //    // If user presses keys very quickly then after a while, the UART enters
    //    // a mode where the same sequence of characters are repeatedly returned
    //    // by UART_HAL_Getchar(). Even a hard reset does not fix it.
    //    // Only way to recover is by unplugging OpenSDA USB.
    //    UART_HAL_FlushRxFifo(BOARD_DEBUG_UART_BASEADDR);

    int tmp;
    // TODO: verify the message above. Which board has this issue ?
    tmp = GETCHAR();
    return tmp;
}

// ============================================================================
// Main
// ============================================================================

static void print_version(void)
{
    ATH_VERSION_STR verstr;

    int res = qcom_get_versionstr(&verstr);
    if (A_OK == res)
    {
        PRINTF("Host version:      %s\r\n", verstr.host_ver);
        PRINTF("Target version:    %s\r\n", verstr.target_ver);
        PRINTF("Firmware version:  %s\r\n", verstr.wlan_ver);
        PRINTF("Interface version: %s\r\n", verstr.abi_ver);
    }
    else
    {
        PRINTF("ERROR: Failed to get QCA400X version information\r\n");
    }
}

void task_main(void *param)
{
    int32_t result = 0;
    (void)result;

    /* Initialize WIFI shield */
    result = WIFISHIELD_Init();
    assert(A_OK == result);

    /* Initialize the WIFI driver (thus starting the driver task) */
    result = wlan_driver_start();
    assert(A_OK == result);

    print_version();

    UBaseType_t numTasks = uxTaskGetNumberOfTasks();
    PRINTF("number of FreeRTOS tasks = %d\r\n", numTasks);
    DbgConsole_Flush();
    PRINTF("Entering main loop\r\n");

    printSeparator();
    menuExtraHelp();

    menuPrint();

    while (1)
    {
        int ch = pollChar();
        if (ch != -1)
        {
            menuAction(ch);
        }

        menuAllStateMachine();

        int received = udpPollAndRecv(50);
        if (received >= 1)
        {
            udpSendEcho();
        }
        else
        {
            vTaskDelay(MSEC_TO_TICK(50));
        }
    }
}

int main(void)
{
    BaseType_t result = 0;
    (void)result;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    result =
        xTaskCreate(task_main, "main", TASK_MAIN_STACK_SIZE, task_main_stack, TASK_MAIN_PRIO, &task_main_task_handler);
    assert(pdPASS == result);

    vTaskStartScheduler();
    for (;;)
        ;
}
