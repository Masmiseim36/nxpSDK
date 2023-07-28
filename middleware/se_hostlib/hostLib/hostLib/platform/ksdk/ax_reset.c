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
#include "se_board_config.h"

/*
 * Where applicable, Configure the PINs on the Host
 *
 */
void axReset_HostConfigure()
{
// TODO: Add config for QN9090
#ifndef QN9090DK6
#if defined(NORDIC_MCU)
//    nrf_drv_gpiote_out_config_t reset_pin_cfg;
//    nrf_drv_gpiote_out_init(SE05X_ENA_HOST_PIN, &reset_pin_cfg);
#else // other supported platforms
    se05x_host_configure();
#endif //NORDIC_MCU

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
#if defined(NORDIC_MCU)
//    nrf_gpio_pin_write(SE05X_ENA_HOST_PIN, !SE_RESET_LOGIC);
#else // other supported platforms
    se05x_host_powerdown();
#endif // NORDIC_MCU
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
#if defined(NORDIC_MCU)
//    nrf_gpio_pin_write(SE05X_ENA_HOST_PIN, SE_RESET_LOGIC);
#else // other supported platforms
    se05x_host_powerup();
#endif // NORDIC_MCU
#endif // QN9090DK6
    return;
}

void axReset_HostUnconfigure()
{
    /* Nothing to be done */
    return;
}