/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "httpsclient.h"
#include "pin_mux.h"
#include "board.h"
#include "lwip/netifapi.h"
#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "ksdk_mbedtls.h"
#include "mflash_drv.h"
#include "mcuboot_app_support.h"
#include "fsl_debug_console.h"
#include "ota_config.h"

#include "fsl_enet.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

int initNetwork(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitModuleClock(void)
{
    const clock_sys_pll1_config_t sysPll1Config = {
        .pllDiv2En = true,
    };
    CLOCK_InitSysPll1(&sysPll1Config);

#if BOARD_NETWORK_USE_100M_ENET_PORT
    clock_root_config_t rootCfg = {.mux = 4, .div = 10}; /* Generate 50M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet1, &rootCfg);
#else
    clock_root_config_t rootCfg = {.mux = 4, .div = 4}; /* Generate 125M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet2, &rootCfg);
#endif
}

void IOMUXC_SelectENETClock(void)
{
#if BOARD_NETWORK_USE_100M_ENET_PORT
    IOMUXC_GPR->GPR4 |= IOMUXC_GPR_GPR4_ENET_REF_CLK_DIR_MASK; /* 50M ENET_REF_CLOCK output to PHY and ENET module. */
#else
    IOMUXC_GPR->GPR5 |= IOMUXC_GPR_GPR5_ENET1G_RGMII_EN_MASK; /* bit1:iomuxc_gpr_enet_clk_dir
                                                                 bit0:GPR_ENET_TX_CLK_SEL(internal or OSC) */
#endif
}

void BOARD_ENETFlexibleConfigure(enet_config_t *config)
{
#if BOARD_NETWORK_USE_100M_ENET_PORT
    config->miiMode = kENET_RmiiMode;
#else
    config->miiMode = kENET_RgmiiMode;
#endif
}


void *pvPortCalloc(size_t num, size_t size)
{
    void *ptr;
    int allocSize = num * size;

    ptr = pvPortMalloc(allocSize);
    if (ptr != NULL)
    {
        memset(ptr, 0, allocSize);
    }

    return ptr;
}

/*!
 * @brief The main function containing client thread.
 */
static void httpsclient_task(void *arg)
{
    int ret;

    /* Network Initialization */

    ret = initNetwork();
    if (ret)
    {
        PRINTF("FAILED to init network (ret=%d)\n", ret);
        goto task_cleanup;
    }

    /* File Download Over TLS */

    ret = https_client_tls_init();
    if (ret != SUCCESS)
    {
        PRINTF("FAILED to init TLS (ret=%d)\n", ret);
        goto tls_cleanup;
    }

    ret = https_client_ota_download(OTA_IMAGE_PATH);
    if (ret != SUCCESS)
    {
        PRINTF("FAILED to download OTA image (ret=%d)\n", ret);
        goto tls_cleanup;
    }

    /* Mark downloaded image ready for test */

    ret = bl_update_image_state(kSwapType_ReadyForTest);
    if (ret != kStatus_Success)
    {
        PRINTF("FAILED to mark OTA image as ReadyForTest (ret=%d)\n", ret);
        goto tls_cleanup;
    }

    PRINTF("OTA image was downloaded successfully. Now press the reset button to reboot the board.\n");

tls_cleanup:
    https_client_tls_release();
task_cleanup:
    vTaskDelete(NULL);
}

/*!
 * @brief Main function.
 */
int main(void)
{
    uint32_t state;
    int ret;

    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    SCB_DisableDCache();

    IOMUXC_SelectENETClock();

#if BOARD_NETWORK_USE_100M_ENET_PORT
    BOARD_InitEnetPins();
    GPIO_PinInit(GPIO9, 11, &gpio_config);
    GPIO_PinInit(GPIO12, 12, &gpio_config);
    /* Pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO9, 11, 1);
    GPIO_WritePinOutput(GPIO12, 12, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO12, 12, 1);
    SDK_DelayAtLeastUs(6, CLOCK_GetFreq(kCLOCK_CpuClk));
#else
    BOARD_InitEnet1GPins();
    GPIO_PinInit(GPIO11, 14, &gpio_config);
    /* For a complete PHY reset of RTL8211FDI-CG, this pin must be asserted low for at least 10ms. And
     * wait for a further 30ms(for internal circuits settling time) before accessing the PHY register */
    GPIO_WritePinOutput(GPIO11, 14, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO11, 14, 1);
    SDK_DelayAtLeastUs(30000, CLOCK_GetFreq(kCLOCK_CpuClk));

    EnableIRQ(ENET_1G_MAC0_Tx_Rx_1_IRQn);
    EnableIRQ(ENET_1G_MAC0_Tx_Rx_2_IRQn);
#endif
    CRYPTO_InitHardware();

    mflash_drv_init();

    /* Get image state */

    if (bl_get_image_state(&state) != kStatus_Success)
    {
        PRINTF("Failed to read image status, cannot continue!\n");
        goto ota_abort;
    }

    if (state == kSwapType_Testing)
    {
        PRINTF("Current image is marked as TEST image.\n");
        PRINTF("Assuming all is OK and marking the image as PERMANENT.\n");

        ret = bl_update_image_state(kSwapType_Permanent);
        if (ret != kStatus_Success)
        {
            PRINTF("FAILED to mark OTA image as PERMANENT (ret=%d)\n", ret);
            goto ota_abort;
        }

        PRINTF("Press the reset button to reboot the board\n");
        goto ota_abort;
    }

    /* Start the OTA */

    if (xTaskCreate(httpsclient_task, "httpsclient_task", 2048, NULL, configMAX_PRIORITIES - 4 /*3*/, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!\r\n");
        while (1)
            ;
    }

    /* Run RTOS */
    vTaskStartScheduler();

ota_abort:
    /* Should not reach this statement */
    for (;;)
        ;
}
