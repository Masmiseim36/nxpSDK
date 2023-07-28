/* -------------------------------------------------------------------------- */
/*                           Copyright 2022 NXP                               */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <stdint.h>
#include "fwk_platform_coex.h"
#include "fsl_common.h"
#include "wifi-decl.h"
#include "sduart_nw61x.h"
#include "firmware_dnld.h"
#include "sdio.h"

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */
#ifndef PLATFORM_CONFIG_DEFAULT_RESET_DELAY_MS
// Default delay after R̅E̅S̅E̅T̅ assertion, in miliseconds.
#define PLATFORM_CONFIG_DEFAULT_RESET_DELAY_MS 200
#endif

#ifndef RESET_PIN_PORT
#define RESET_PIN_PORT 3
#endif

#ifndef RESET_PIN_NUM
#define RESET_PIN_NUM 9
#endif
/* -------------------------------------------------------------------------- */
/*                                Private types                               */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static GPIO_HANDLE_DEFINE(otGpioResetHandle);

static const hal_gpio_pin_config_t resetPinConfig = {
    .direction = kHAL_GpioDirectionOut,
    .level     = 0U,
    .port      = RESET_PIN_PORT,
    .pin       = RESET_PIN_NUM,
};

static bool isOtControllerUp = false;

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

static int PLATFORM_ResetController(void);

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

int PLATFORM_InitControllers(uint8_t controllersMask)
{
    (void)controllersMask;

    hal_gpio_status_t status;
    int               ret = 0;

    /*  initialize controller if not done */
    if (isOtControllerUp == false)
    {
        do
        {
            /* Init the reset output gpio */
            status = HAL_GpioInit((hal_gpio_handle_t)otGpioResetHandle, (hal_gpio_pin_config_t *)&resetPinConfig);
            if (status != kStatus_HAL_GpioSuccess)
            {
                ret = -1;
                break;
            }
            (void)status;

            /* Reset RCP chip. */
            if (PLATFORM_ResetController() != 0)
            {
                ret = -2;
                break;
            }

            /* Download firmware */
            ret = sdio_init();
            assert(WM_SUCCESS == ret);
            ret = sdio_ioport_init();
            assert(WM_SUCCESS == ret);
            ret = firmware_download(wlan_fw_bin, wlan_fw_bin_len);
            assert(WM_SUCCESS == ret);

            /* Waiting for the RCP chip starts up */
            OSA_TimeDelay(PLATFORM_CONFIG_DEFAULT_RESET_DELAY_MS);

            isOtControllerUp = true;
        } while (0);
    }

    return ret;
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */
static int PLATFORM_ResetController(void)
{
    hal_gpio_status_t status;
    int               ret = 0;

    do
    {
        // Set Reset pin to low level.
        status = HAL_GpioSetOutput((hal_gpio_handle_t)otGpioResetHandle, 0);
        if (status != kStatus_HAL_GpioSuccess)
        {
            ret = -1;
            break;
        }

        OSA_TimeDelay(PLATFORM_CONFIG_DEFAULT_RESET_DELAY_MS);

        // Set Reset pin to high level.
        status = HAL_GpioSetOutput((hal_gpio_handle_t)otGpioResetHandle, 1);
        if (status != kStatus_HAL_GpioSuccess)
        {
            ret = -2;
            break;
        }
    } while (0);

    return ret;
}
