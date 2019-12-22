// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>

#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/shared_util_options.h"

#include "iothub_client_ll.h"
#include "iothub_message.h"
#include "iothubtransportamqp.h"
#include "certs_azure.h"

#include "board.h"
#include "ksdk_mbedtls.h"

#include "FreeRTOS.h"
#include "task.h"

#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/netifapi.h"
#include "lwip/prot/dhcp.h"
#include "netif/ethernet.h"
#include "enet_ethernetif.h"
#include "sntp.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
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

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

/* System clock name. */
#define EXAMPLE_CLOCK_NAME kCLOCK_CoreSysClk

#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif /* EXAMPLE_NETIF_INIT_FN */

#define AZURE_TASK_STACKSIZE (configMINIMAL_STACK_SIZE + 6000)
#define AZURE_TASK_PRIORITY 6

/*! @brief Stack size of the thread which prints DHCP info. */
#define PRINT_THREAD_STACKSIZE 512

/*! @brief Priority of the thread which prints DHCP info. */
#define PRINT_THREAD_PRIO DEFAULT_THREAD_PRIO

#define MESSAGE_COUNT 5
#define DOWORK_LOOP_NUM 3

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void print_dhcp_state(void *arg);
extern int SNTP_Init(void);

/*!
 * @brief Parse incoming command and execute
 */
int process_message(const char *buffer);

/*!
 * @brief Turn on selected LED
 */
void led_on(uint8_t whichLed);

/*!
 * @brief Turn off selected LED
 */
void led_off(uint8_t whichLed);

/*!
 * @brief Initialize GPIO pins
 */
static void init_pins(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*String containing Hostname, Device Id & Device Key in the format:                         */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>"                */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessSignature=<device_sas_token>"    */
/*Primary key */
static const char* connectionString = "[device connection string]";

static bool g_continueRunning;
typedef enum {RED, GREEN, BLUE} led_enum;

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {.enableClkOutput = true, .enableClkOutput25M = false, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);
}

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}


int process_message(const char *buffer)
{
    /* LED ON command */
    if(strstr(buffer, "RED LED ON") != NULL)
    {
        led_on(RED);
    }
    else if(strstr(buffer, "GREEN LED ON") != NULL)
    {
        led_on(GREEN);
    }
    else if(strstr(buffer, "BLUE LED ON") != NULL)
    {
        led_on(BLUE);
    }
    /* LED OFF command */
    else if(strstr(buffer, "RED LED OFF") != NULL)
    {
        led_off(RED);
    }
    else if(strstr(buffer, "GREEN LED OFF") != NULL)
    {
        led_off(GREEN);
    }
    else if(strstr(buffer, "BLUE LED OFF") != NULL)
    {
        led_off(BLUE);
    }
    else
    {
        printf("Recieved message: %s\n", buffer);
    }

    return 0;
}

/*!
 * @brief Turn on selected LED
 */
void led_on(uint8_t whichLed)
{
    switch (whichLed)
    {
        /* Red */
        case RED:
#if defined(LED_RED_ON)
            LED_RED_ON();
#elif defined(LED1_ON)
            LED1_ON();
#endif
            break;

        /* Green */
        case GREEN:
#if defined(LED_GREEN_ON)
            LED_GREEN_ON();
#elif defined(LED2_ON)
            LED2_ON();
#endif
            break;

        /* Blue */
        case BLUE:
#if defined(LED_BLUE_ON)
            LED_BLUE_ON();
#elif defined(LED3_ON)
            LED3_ON();
#endif
            break;
    }
}


/*!
 * @brief Turn off selected LED
 */
void led_off(uint8_t whichLed)
{
    switch (whichLed)
    {
        /* Red */
        case RED:
#if defined(LED_RED_OFF)
            LED_RED_OFF();
#elif defined(LED1_OFF)
            LED1_OFF();
#endif
            break;

        /* Green */
        case GREEN:
#if defined(LED_GREEN_OFF)
            LED_GREEN_OFF();
#elif defined(LED2_OFF)
            LED2_OFF();
#endif
            break;

        /* Blue */
        case BLUE:
#if defined(LED_BLUE_OFF)
            LED_BLUE_OFF();
#elif defined(LED3_OFF)
            LED3_OFF();
#endif
            break;
    }
}

/*!
 * @brief Initialize GPIO pins
 */
static void init_pins(void)
{

  /* Initialize pins for LEDs */
#if defined(LED_GREEN_INIT) && defined(LED_RED_INIT) && defined(LED_BLUE_INIT)
    LED_GREEN_INIT(0);
    LED_RED_INIT(0);
    LED_BLUE_INIT(0);

    LED_RED_OFF();
    LED_GREEN_OFF();
    LED_BLUE_OFF();
#elif defined(BOARD_LED1_GPIO) && defined(BOARD_LED2_GPIO) && defined(BOARD_LED3_GPIO)
    LED1_INIT(0);
    LED2_INIT(0);
    LED3_INIT(0);

    LED1_OFF();
    LED2_OFF();
    LED3_OFF();
#endif /*LED_GREEN_INIT && LED_RED_INIT && LED_BLUE_INIT*/
}

static IOTHUBMESSAGE_DISPOSITION_RESULT ReceiveMessageCallback(IOTHUB_MESSAGE_HANDLE message, void* userContextCallback)
{
    int* counter = (int*)userContextCallback;
    const char* buffer;
    size_t size;
    MAP_HANDLE mapProperties;
    const char* messageId;
    const char* correlationId;
    const char* userDefinedContentType;
    const char* userDefinedContentEncoding;

    // Message properties
    if ((messageId = IoTHubMessage_GetMessageId(message)) == NULL)
    {
        messageId = "<null>";
    }

    if ((correlationId = IoTHubMessage_GetCorrelationId(message)) == NULL)
    {
        correlationId = "<null>";
    }

    if ((userDefinedContentType = IoTHubMessage_GetContentTypeSystemProperty(message)) == NULL)
    {
        userDefinedContentType = "<null>";
    }

    if ((userDefinedContentEncoding = IoTHubMessage_GetContentEncodingSystemProperty(message)) == NULL)
    {
        userDefinedContentEncoding = "<null>";
    }

    // Message content
    if (IoTHubMessage_GetByteArray(message, (const unsigned char**)&buffer, &size) != IOTHUB_MESSAGE_OK)
    {
        (void)printf("unable to retrieve the message data\r\n");
    }
    else
    {
        process_message(buffer);
        (void)printf("Received Message [%d]\r\n Message ID: %s\r\n Correlation ID: %s\r\n Content-Type: %s\r\n Content-Encoding: %s\r\n Data: <<<%.*s>>> & Size=%d\r\n",
            *counter, messageId, correlationId, userDefinedContentType, userDefinedContentEncoding, (int)size, buffer, (int)size);
        // If we receive the work 'quit' then we stop running
        if (size == (strlen("quit") * sizeof(char)) && memcmp(buffer, "quit", size) == 0)
        {
            g_continueRunning = false;
        }
    }

    // Retrieve properties from the message
    mapProperties = IoTHubMessage_Properties(message);
    if (mapProperties != NULL)
    {
        const char*const* keys;
        const char*const* values;
        size_t propertyCount = 0;
        if (Map_GetInternals(mapProperties, &keys, &values, &propertyCount) == MAP_OK)
        {
            if (propertyCount > 0)
            {
                size_t index;

                printf(" Message Properties:\r\n");
                for (index = 0; index < propertyCount; index++)
                {
                    (void)printf("\tKey: %s Value: %s\r\n", keys[index], values[index]);
                }
                (void)printf("\r\n");
            }
        }
    }

    /* Some device specific action code goes here... */
    (*counter)++;
    return IOTHUBMESSAGE_ACCEPTED;
}

void iothub_client_sample_amqp_run(void)
{
    IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;
    int receiveContext = 0;
    g_continueRunning = true;
    srand((unsigned int)time(NULL));

    if (platform_init() != 0)
    {
        (void)printf("Failed to initialize the platform.\r\n");
    }
    else
    {
        if ((iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, AMQP_Protocol)) == NULL)
        {
            (void)printf("ERROR: iotHubClientHandle is NULL!\r\n");
        }
        else
        {
            // Add the certificate information
            if (IoTHubClient_LL_SetOption(iotHubClientHandle, OPTION_TRUSTED_CERT, certificates) != IOTHUB_CLIENT_OK)
            {
                printf("failure to set option \"TrustedCerts\"\r\n");
            }

            /* Setting Message call back, so we can receive Commands. */
            if (IoTHubClient_LL_SetMessageCallback(iotHubClientHandle, ReceiveMessageCallback, &receiveContext) != IOTHUB_CLIENT_OK)
            {
                (void)printf("ERROR: IoTHubClient_LL_SetMessageCallback..........FAILED!\r\n");
            }
            else
            {
                (void)printf("IoTHubClient_LL_SetMessageCallback...successful.\r\n");

                while(g_continueRunning == true)
                {
                    IoTHubClient_LL_DoWork(iotHubClientHandle);
                    ThreadAPI_Sleep(1);
                }
            }
            IoTHubClient_LL_Destroy(iotHubClientHandle);
        }
        platform_deinit();
    }
}

void azure_task(void *pvParameter)
{
    SNTP_Init();

    iothub_client_sample_amqp_run();
    while (1)
    {
        vTaskDelay(1000);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);

    GPIO_PinInit(GPIO1, 9, &gpio_config);
    GPIO_PinInit(GPIO1, 10, &gpio_config);
    /* pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO1, 10, 1);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    delay();
    GPIO_WritePinOutput(GPIO1, 9, 1);
    CRYPTO_InitHardware();
    init_pins();

    static struct netif netif;
#if defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0)
    static mem_range_t non_dma_memory[] = NON_DMA_MEMORY_ARRAY;
#endif /* FSL_FEATURE_SOC_LPC_ENET_COUNT */

    /* Initialize network interface */
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;
    ethernetif_config_t enet_config = {
        .phyAddress = EXAMPLE_PHY_ADDRESS, .clockName = EXAMPLE_CLOCK_NAME, .macAddress = configMAC_ADDR,
#if defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0)
        .non_dma_memory = non_dma_memory,
#endif /* FSL_FEATURE_SOC_LPC_ENET_COUNT */
    };

    IP4_ADDR(&netif_ipaddr, 0U, 0U, 0U, 0U);
    IP4_ADDR(&netif_netmask, 0U, 0U, 0U, 0U);
    IP4_ADDR(&netif_gw, 0U, 0U, 0U, 0U);

    tcpip_init(NULL, NULL);

    netifapi_netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, EXAMPLE_NETIF_INIT_FN,
                       tcpip_input);
    netifapi_netif_set_default(&netif);
    netifapi_netif_set_up(&netif);

    netifapi_dhcp_start(&netif);

    if (sys_thread_new("print_dhcp", print_dhcp_state, &netif, PRINT_THREAD_STACKSIZE, PRINT_THREAD_PRIO ) == NULL)
    {
        LWIP_ASSERT("stack_init(): Task creation failed.", 0);
    }

    vTaskStartScheduler();
    return 0;
}

static void print_dhcp_state(void *arg)
{
    struct netif *netif = (struct netif *)arg;
    struct dhcp *dhcp;
    u8_t dhcp_last_state = DHCP_STATE_OFF;

    while (netif_is_up(netif))
    {
        dhcp = netif_dhcp_data(netif);

        if (dhcp == NULL)
        {
            dhcp_last_state = DHCP_STATE_OFF;
        }
        else if (dhcp_last_state != dhcp->state)
        {
            dhcp_last_state = dhcp->state;

            PRINTF(" DHCP state       : ");
            switch (dhcp_last_state)
            {
                case DHCP_STATE_OFF:
                    PRINTF("OFF");
                    break;
                case DHCP_STATE_REQUESTING:
                    PRINTF("REQUESTING");
                    break;
                case DHCP_STATE_INIT:
                    PRINTF("INIT");
                    break;
                case DHCP_STATE_REBOOTING:
                    PRINTF("REBOOTING");
                    break;
                case DHCP_STATE_REBINDING:
                    PRINTF("REBINDING");
                    break;
                case DHCP_STATE_RENEWING:
                    PRINTF("RENEWING");
                    break;
                case DHCP_STATE_SELECTING:
                    PRINTF("SELECTING");
                    break;
                case DHCP_STATE_INFORMING:
                    PRINTF("INFORMING");
                    break;
                case DHCP_STATE_CHECKING:
                    PRINTF("CHECKING");
                    break;
                case DHCP_STATE_BOUND:
                    PRINTF("BOUND");
                    break;
                case DHCP_STATE_BACKING_OFF:
                    PRINTF("BACKING_OFF");
                    break;
                default:
                    PRINTF("%u", dhcp_last_state);
                    assert(0);
                    break;
            }
            PRINTF("\r\n");

            if (dhcp_last_state == DHCP_STATE_BOUND)
            {
                PRINTF("\r\n IPv4 Address     : %s\r\n", ipaddr_ntoa(&netif->ip_addr));
                PRINTF(" IPv4 Subnet mask : %s\r\n", ipaddr_ntoa(&netif->netmask));
                PRINTF(" IPv4 Gateway     : %s\r\n\r\n", ipaddr_ntoa(&netif->gw));
                if (xTaskCreate(&azure_task, "azure_task", AZURE_TASK_STACKSIZE, NULL, AZURE_TASK_PRIORITY, NULL) != pdPASS)
                {
                    PRINTF("Task creation failed!.\r\n");
                    while (1)
                        ;
                }
                vTaskDelete(NULL);
            }
        }
        sys_msleep(20U);
    }
}
