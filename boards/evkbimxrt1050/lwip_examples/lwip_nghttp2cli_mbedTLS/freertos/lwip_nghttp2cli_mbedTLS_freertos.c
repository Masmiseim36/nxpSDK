/*
 * Copyright 2016 Freescale Semiconductor, Inc
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "nghttp2client.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "peripherals.h"
#include "board.h"
#include "ksdk_mbedtls.h"

#include "fsl_debug_console.h"

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "fsl_phy.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitNetwork(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Binary semaphore to block thread until valid IPv4 address is obtained from DHCP */
SemaphoreHandle_t BOARD_sem_ipv4_addr_valid;
/* Memory for lwIP stack needed registration of callback */
NETIF_DECLARE_EXT_CALLBACK(netif_ext_cb_mem);

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {true, false, 1};
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


static void netif_ext_cb(struct netif *cb_netif, netif_nsc_reason_t reason, const netif_ext_callback_args_t *args)
{
    (void)cb_netif;
    (void)args;

    if (0U != (reason & (netif_nsc_reason_t)LWIP_NSC_IPV4_ADDR_VALID))
    {
        (void)xSemaphoreGive(BOARD_sem_ipv4_addr_valid);
    }
}

void tcpip_init_callback(void *arg)
{
    /* register lwIP callback */
    netif_add_ext_callback(&netif_ext_cb_mem, netif_ext_cb);
}

void BOARD_InitNetwork(void)
{
    struct dhcp *dhcp;

    /* create semaphore */
    BOARD_sem_ipv4_addr_valid = xSemaphoreCreateBinary();

    BOARD_InitLwip();

    PRINTF("Getting IP address from DHCP ...\n");

    dhcp = (struct dhcp *)netif_get_client_data(&lwIP_netif0, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

    xSemaphoreTake(BOARD_sem_ipv4_addr_valid, portMAX_DELAY);

    if (dhcp->state == DHCP_STATE_BOUND)
    {
        PRINTF("\r\n IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&lwIP_netif0.ip_addr.addr)[0],
               ((u8_t *)&lwIP_netif0.ip_addr.addr)[1], ((u8_t *)&lwIP_netif0.ip_addr.addr)[2],
               ((u8_t *)&lwIP_netif0.ip_addr.addr)[3]);
    }

    PRINTF("DHCP OK");
}

/*!
 * @brief The main function containing client thread.
 */
static void httpsclient_task(void *arg)
{
    BOARD_InitNetwork();
    nghttp2_client();

#if defined(INCLUDE_uxTaskGetStackHighWaterMark) && INCLUDE_uxTaskGetStackHighWaterMark
    int32_t freeStack = uxTaskGetStackHighWaterMark(NULL);
    PRINTF("Stack high water mark: %d\n", freeStack);
#endif

#if defined(configTOTAL_HEAP_SIZE)
    int32_t freeHeap = xPortGetMinimumEverFreeHeapSize();
    PRINTF("Minimum ever free heap size: %d\n", freeHeap);
#endif

    vTaskDelete(NULL);
}

/*!
 * @brief Main function.
 */
int main(void)
{
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
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

    if (xTaskCreate(httpsclient_task, "httpsclient_task", 2000, NULL, configMAX_PRIORITIES - 4 /*3*/, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    /* Run RTOS */
    vTaskStartScheduler();

    /* Should not reach this statement */
    for (;;)
        ;
}
