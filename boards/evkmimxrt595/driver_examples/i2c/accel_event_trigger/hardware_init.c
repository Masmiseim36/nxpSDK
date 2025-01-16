/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_power.h"
#include "fsl_gpio.h"
#include "fsl_pint.h"
#include "fsl_inputmux.h"
/*${header:end}*/

/*${function:start}*/
static void EXAMPLE_PinIntHandler(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    APP_ACCEL_IntHandler();
}

void EXAMPLE_InitWakeupPin(void)
{
    gpio_pin_config_t gpioPinConfigStruct;

    /* Set SW pin as GPIO input. */
    gpioPinConfigStruct.pinDirection = kGPIO_DigitalInput;
    GPIO_PinInit(EXAMPLE_SENSOR_INT_GPIO, EXAMPLE_SENSOR_INT_PORT, EXAMPLE_SENSOR_INT_PIN, &gpioPinConfigStruct);

    /* Configure the Input Mux block and connect the trigger source to PinInt channle. */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt0, EXAMPLE_SENSOR_INT_INPUTMUX_SEL); /* Using channel 0. */
    INPUTMUX_Deinit(INPUTMUX); /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */

    /* Configure the interrupt for SW pin. */
    PINT_Init(PINT);
    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableFallEdge, EXAMPLE_PinIntHandler);
    PINT_EnableCallback(PINT); /* Enable callbacks for PINT */

    EnableDeepSleepIRQ(EXAMPLE_INT_IRQn);
}

void EXAMPLE_EnterDeepSleep(void)
{
    /* Enter deep sleep mode by using power API. */
    POWER_EnterDeepSleep(EXAMPLE_EXCLUDE_FROM_DEEPSLEEP);
}

void BOARD_InitHardware(void)
{
    /* Use 48 MHz clock for the FLEXCOMM4 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM4);

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
