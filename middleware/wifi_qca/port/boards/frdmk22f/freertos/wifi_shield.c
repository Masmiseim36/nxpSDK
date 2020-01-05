/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#include "wifi_common.h"
#include "fsl_gpio.h"
#include "driver_cxt.h"
#include "wifi_spi.h"
#include "wifi_shield.h"


/*!
 * @brief Low level initialization, RTOS does not have to run yet
 */
A_STATUS WIFISHIELD_Init(void)
{
    gpio_pin_config_t config;
    memset(&config, 0, sizeof(config));

    /* initialize pinmux */
#ifdef WIFISHIELD_PINMUX_INIT
    WIFISHIELD_PINMUX_INIT();
#endif

    /* Set up WLAN_PWRON signal */
    config.pinDirection = (gpio_pin_direction_t)WIFISHIELD_WLAN_PWRON_DIRECTION;
    GPIO_PinInit(WIFISHIELD_WLAN_PWRON_GPIO, WIFISHIELD_WLAN_PWRON_PIN, &config);

    /* Set up WLAN_IRQ signal */
    config.pinDirection = (gpio_pin_direction_t)WIFISHIELD_WLAN_IRQ_DIRECTION;
    PORT_SetPinInterruptConfig(WIFISHIELD_WLAN_IRQ_PORT, WIFISHIELD_WLAN_IRQ_PIN, kPORT_InterruptFallingEdge);
    GPIO_PinInit(WIFISHIELD_WLAN_IRQ_GPIO, WIFISHIELD_WLAN_IRQ_PIN, &config);

    /* Enable NVIC interrupt for WLAN_IRQ */
    NVIC_EnableIRQ(WIFISHIELD_WLAN_IRQn);

    /* Set NVIC priority if is required by Freertos */
    NVIC_SetPriority(WIFISHIELD_WLAN_IRQn, WIFISHIELD_WLAN_IRQ_PRIORITY);

    return A_OK;
}

/*!
 * @brief Initialization is maintained by WIFI stack
 */
A_STATUS WIFISHIELD_InitDrivers(void *param)
{
    /* expose capabilities of SPI transfer to upper layer TODO: remove in future */
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(param);

    /* Force WIFI stack to use this SPI settings */
    pDCxt->spi_hcd.PowerUpDelay = 1;
    pDCxt->spi_hcd.SpiHWCapabilitiesFlags = (HW_SPI_FRAME_WIDTH_8 | HW_SPI_NO_DMA | HW_SPI_INT_EDGE_DETECT);

    /* Complex DMAMUX/DMA/SPI config structure */
    WIFIDRVS_SPI_config_t spi_config = {0};

    /* load default settings */
    WIFIDRVS_SPI_GetDefaultConfig(&spi_config);

#if defined(WIFISHIELD_DMA) && defined(WIFISHIELD_DMAMUX)
    /* configure dma_mode */
    spi_config.dma_mode.enabled = true;
    spi_config.dma_mode.dmamux_base = (void*)WIFISHIELD_DMAMUX;
    spi_config.dma_mode.dmamux_rx_req = WIFISHIELD_DMAMUX_RX_REQ;
    spi_config.dma_mode.dma_base = (void*)WIFISHIELD_DMA;
    spi_config.dma_mode.dma_rx_chnl = WIFISHIELD_DMA_RX_CHNL;
    spi_config.dma_mode.dma_im_chnl = WIFISHIELD_DMA_IM_CHNL;
    spi_config.dma_mode.dma_tx_chnl = WIFISHIELD_DMA_TX_CHNL;
    spi_config.dma_mode.dma_irq_prio = WIFISHIELD_DMA_IRQ_PRIORITY;
#endif

#if defined(WIFISHIELD_SPI_IRQ_PRIORITY)
    /* configure irq_mode */
    spi_config.irq_mode.enabled = true;
    spi_config.irq_mode.spi_irq_prio = WIFISHIELD_SPI_IRQ_PRIORITY;
#endif

    /* configure spi */
    spi_config.spi.base = (void*)WIFISHIELD_SPI;
    spi_config.spi.clk_hz = CLOCK_GetFreq(WIFISHIELD_SPI_CLOCKSRC);
    spi_config.spi.baudrate = WIFISHIELD_SPI_BAUDRATE;
    spi_config.spi.irq_threshold = WIFISHIELD_SPI_THRESHOLD;
    spi_config.spi.xfer_cs = WIFISHIELD_SPI_XFER_CS;

    /* load recommended SPI settings */
    WIFIDRVS_SPI_GetSPIConfig(&spi_config.spi.config, WIFISHIELD_SPI_BAUDRATE, WIFISHIELD_SPI_INIT_CS);

    /* initialize driver */
    A_STATUS result = WIFIDRVS_SPI_Init(&spi_config);
    assert(A_OK == result);

    return result;
}

/*!
 * @brief Deinitialization is maintained by WIFI stack
 */
A_STATUS WIFISHIELD_DeinitDrivers(void *param)
{
    // WIFI_Spi_Deinit();
    return A_OK;
}

/*!
 * @brief Power up WiFi shield, RTOS does not have to run yet
 */
A_STATUS WIFISHIELD_PowerUp(uint32_t enable)
{
    if (enable)
    {
        GPIO_PortSet(WIFISHIELD_WLAN_PWRON_GPIO, (1 << (WIFISHIELD_WLAN_PWRON_PIN)));
    }
    else
    {
        GPIO_PortClear(WIFISHIELD_WLAN_PWRON_GPIO, (1 << (WIFISHIELD_WLAN_PWRON_PIN)));
    }
    return A_OK;
}

/*!
 * @brief Fn post DriverTask semaphore, can be called only from WLAN_IRQ ISR
 */
void WIFISHIELD_NotifyDriverTask(void *param)
{
    extern QCA_CONTEXT_STRUCT wifiCtx;
    void HW_InterruptHandler(void *pCxt);

    void *pCxt = wifiCtx.MAC_CONTEXT_PTR;
    if (pCxt)
    {
        HW_InterruptHandler(pCxt);
    }
}

#ifdef WIFISHIELD_WLAN_ISR

/*!
 * @brief GPIO IRQ handler, drop request line, notify driver
 */
void WIFISHIELD_WLAN_ISR(void)
{
    uint32_t pin_flags = GPIO_PortGetInterruptFlags(WIFISHIELD_WLAN_IRQ_GPIO);
    GPIO_PortClearInterruptFlags(WIFISHIELD_WLAN_IRQ_GPIO, pin_flags);

    if (pin_flags & (1 << WIFISHIELD_WLAN_IRQ_PIN))
    {
        WIFISHIELD_NotifyDriverTask(NULL);
    }
}

#endif

