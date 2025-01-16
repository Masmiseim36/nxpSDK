/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdarg.h>
#include "fsl_sema42.h"
#include "pin_mux.h"
#include "app.h"
#include "board.h"
#include "ezhv_support.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_ezhv.h"
#include "fsl_gpio.h"
#include "fsl_inputmux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef PRINTF
#undef PRINTF
#define PRINTF CM33_PRINTF
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void CM33_PRINTF(const char *ptr, ...)
{
    va_list ap;
    SEMA42_Lock(DEMO_SEMA42_INSTANCE, DEMO_SEMA42_PRINTF_GATE, DEMO_SEMA42_CORE_ID_CM33);
    va_start(ap, ptr);
    DbgConsole_Vprintf(ptr, ap);
    va_end(ap);
    SEMA42_Unlock(DEMO_SEMA42_INSTANCE, DEMO_SEMA42_PRINTF_GATE);
}

int main(void)
{
    gpio_pin_config_t inputConfig = {
        kGPIO_DigitalInput,
        0U,
    };

    gpio_pin_config_t outputConfig = {
        kGPIO_DigitalOutput,
        0U,
    };

    BOARD_InitHardware();

    SEMA42_Init(DEMO_SEMA42_INSTANCE);
    SEMA42_ResetAllGates(DEMO_SEMA42_INSTANCE);

    PRINTF("\r\nBLC detect demo - message from primary core side.\r\n");

    GPIO_PinInit(DEMO_GPIO_INSTANCE, DEMO_INPUT_PIN_NUMBER, &inputConfig);
    GPIO_PinInit(DEMO_GPIO_INSTANCE, DEMO_OUTPUT_PIN_NUMBER, &outputConfig);

    GPIO_SetPinInterruptChannel(DEMO_GPIO_INSTANCE, DEMO_INPUT_PIN_NUMBER, kGPIO_InterruptOutput0);
    GPIO_SetPinInterruptConfig(DEMO_GPIO_INSTANCE, DEMO_INPUT_PIN_NUMBER, kGPIO_ActiveHighTriggerOutputEnable);

    INPUTMUX_Init(DEMO_INPUTMUX_INSTANCE);
    INPUTMUX_AttachSignal(DEMO_INPUTMUX_INSTANCE, 0U, kINPUTMUX_GpioInt0Trig0ToEzhv);
    INPUTMUX_Deinit(DEMO_INPUTMUX_INSTANCE);

    BOARD_EZHV_Init();

    while (!EZHV_GetEzhvWaitStatusFlag())
        ;

    PRINTF("Press any key to trigger output.\r\n");

    GETCHAR();
    GPIO_PinWrite(DEMO_GPIO_INSTANCE, DEMO_OUTPUT_PIN_NUMBER, 1U);
    
    PRINTF("Output signal triggered.\r\n");

    while(1)
    {
    }
}
