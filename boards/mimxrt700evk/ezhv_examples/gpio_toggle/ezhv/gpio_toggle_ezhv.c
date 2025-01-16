/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdarg.h>
#include "fsl_sema42.h"
#include "app.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef PRINTF
#undef PRINTF
#define PRINTF EZHV_PRINTF
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void EZHV_PRINTF(const char *ptr, ...)
{
    va_list ap;
    SEMA42_Lock(DEMO_SEMA42_INSTANCE, DEMO_SEMA42_PRINTF_GATE, DEMO_SEMA42_CORE_ID_EZHV);
    va_start(ap, ptr);
    DbgConsole_Vprintf(ptr, ap);
    va_end(ap);
    SEMA42_Unlock(DEMO_SEMA42_INSTANCE, DEMO_SEMA42_PRINTF_GATE);
}

/*!
 * @brief Main function
 */
int main(void)
{
    ezhv_gpio_pin_config_t config = {
        .pinDirection = kEZHV_GPIO_DigitalOutput,
        .outputLogic  = kEZHV_GPIO_Low,
    };

    BOARD_InitHardware();

    /* SEMA42 init */
    SEMA42_Init(DEMO_SEMA42_INSTANCE);

    PRINTF("Gpio toggle demo - message from ezhv core side.\r\n");
    PRINTF("Press any key to start the program.\r\n");
    GETCHAR();

    EZHV_InitGpioPin(kEZHV_GPIO_Pin4, &config);

    while (1)
    {
        EZHV_PinToggle(kEZHV_GPIO_Pin4);

        SDK_DelayAtLeastUs(1000 * 1000, CLOCK_GetMediaMainClkFreq());

        XZMSG_IndIntChan(1U);
    }
}
