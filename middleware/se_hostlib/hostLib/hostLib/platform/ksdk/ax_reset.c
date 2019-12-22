/* Copyright 2018-2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <board.h>
#include "ax_reset.h"

#include "fsl_gpio.h"
#include "sm_timer.h"
#include "sm_types.h"
#include "fsl_common.h"
#include "se05x_apis.h"
#include "se_reset_config.h"

/*
 * Where applicable, Configure the PINs on the Host
 *
 */
void axReset_HostConfigure()
{
#if defined(CPU_MIMXRT1052DVL6B)
    gpio_pin_config_t reset_pin_cfg = {kGPIO_DigitalOutput, 0, SE_RESET_LOGIC};
#else
    gpio_pin_config_t reset_pin_cfg = {kGPIO_DigitalOutput, SE_RESET_LOGIC};
#endif
#if defined(LPC_55x)
    GPIO_PortInit(GPIO, (uint32_t)SE05X_ENA_HOST_PORT);
    GPIO_PinInit(GPIO, (uint32_t)SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, &reset_pin_cfg);
#else
    GPIO_PinInit(SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, &reset_pin_cfg);
#endif
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
#if defined(LPC_55x)
    GPIO_PinWrite(GPIO, (uint32_t)SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, !SE_RESET_LOGIC);
#else
    GPIO_PinWrite(SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, !SE_RESET_LOGIC);
#endif
    return;
}

/*
 * Where applicable, put SE in powered/active mode
 *
 * Pre-Requistie: @ref axReset_Configure has been called
 */
void axReset_PowerUp()
{
#if defined(LPC_55x)
    GPIO_PinWrite(GPIO, (uint32_t)SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, SE_RESET_LOGIC);
#else
    GPIO_PinWrite(SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, SE_RESET_LOGIC);
#endif
    return;
}
