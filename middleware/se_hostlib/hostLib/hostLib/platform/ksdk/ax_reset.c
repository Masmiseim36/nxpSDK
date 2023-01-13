/* Copyright 2018-2019 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include <board.h>
#include "ax_reset.h"
#ifndef NORDIC_MCU
#include "fsl_gpio.h"
#include "fsl_common.h"
#endif
#include "sm_timer.h"
#include "sm_types.h"
#include "se05x_apis.h"
#include "se_reset_config.h"

#if defined(CPU_MIMXRT1176DVMAA_cm7)
#include "fsl_iomuxc.h"
#endif

/*
 * Where applicable, Configure the PINs on the Host
 *
 */
void axReset_HostConfigure()
{
// TODO: Add config for QN9090
#ifndef QN9090DK6
#if defined(CPU_MIMXRT1062DVL6A)
    gpio_pin_config_t reset_pin_cfg = {kGPIO_DigitalOutput, SE_RESET_LOGIC, kGPIO_NoIntmode};
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
    gpio_pin_config_t reset_pin_cfg = {kGPIO_DigitalOutput, SE_RESET_LOGIC, kGPIO_NoIntmode};
#elif defined(NORDIC_MCU)
//    nrf_drv_gpiote_out_config_t reset_pin_cfg;
#else // K64F && LPC_55x
    gpio_pin_config_t reset_pin_cfg = {kGPIO_DigitalOutput, SE_RESET_LOGIC};
#endif

#if defined(LPC_55x)
    GPIO_PinInit(GPIO, (uint32_t)SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, &reset_pin_cfg);
#elif defined(NORDIC_MCU)
//    nrf_drv_gpiote_out_init(SE05X_ENA_HOST_PIN, &reset_pin_cfg);
#else
    GPIO_PinInit(SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, &reset_pin_cfg);
#if defined(CPU_MIMXRT1176DVMAA_cm7)
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_05_GPIO9_IO04,           /* GPIO_AD_05 is configured as GPIO9_IO04 */
        0U);
#endif // CPU_MIMXRT1176DVMAA_cm7
#endif // LPC_55x
#endif // QN9090DK6
    return;
}

/*
 * Where applicable, PowerCycle the SE
 *
 * Pre-Requistie: @ref axReset_Configure has been called
 */
void axReset_ResetPluseDUT()
{
    axReset_PowerDown();
    sm_usleep(2000);
    axReset_PowerUp();
    return;
}

/*
 * Where applicable, put SE in low power/standby mode
 *
 * Pre-Requistie: @ref axReset_Configure has been called
 */
void axReset_PowerDown()
{
#ifndef QN9090DK6
#if defined(LPC_55x)
    GPIO_PinWrite(GPIO, (uint32_t)SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, !SE_RESET_LOGIC);
#elif defined(NORDIC_MCU)
//    nrf_gpio_pin_write(SE05X_ENA_HOST_PIN, !SE_RESET_LOGIC);
#else
    GPIO_PinWrite(SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, !SE_RESET_LOGIC);
#endif
#endif // QN9090DK6
    return;
}

/*
 * Where applicable, put SE in powered/active mode
 *
 * Pre-Requistie: @ref axReset_Configure has been called
 */
void axReset_PowerUp()
{
#ifndef QN9090DK6
#if defined(LPC_55x)
    GPIO_PinWrite(GPIO, (uint32_t)SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, SE_RESET_LOGIC);
#elif defined(NORDIC_MCU)
//    nrf_gpio_pin_write(SE05X_ENA_HOST_PIN, SE_RESET_LOGIC);
#else
    GPIO_PinWrite(SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, SE_RESET_LOGIC);
#endif
#endif // QN9090DK6
    return;
}

void axReset_HostUnconfigure()
{
    /* Nothing to be done */
    return;
}