/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "board.h"
#include "pin_mux.h"

#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_SW_GPIO GPIO3
#define EXAMPLE_SW_GPIO_PIN 11U
#define EXAMPLE_SW_IRQ GPIO3_Combined_0_15_IRQn
#define EXAMPLE_SW_NAME "SW601-3"
   
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
static void delay(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Whether the SW is turned on */
volatile bool g_InputSignal = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Interrupt service fuction of switch.
 */
void EXAMPLE_GPIO_IRQHandler(void)
{ 
    /* clear the interrupt status */
    GPIO_ClearPinsInterruptFlags(EXAMPLE_SW_GPIO, 1U << EXAMPLE_SW_GPIO_PIN);
    /* Change state of switch. */
    g_InputSignal = true;
}

/*!
* @brief Main function
*/
int main(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput, 0,
        kGPIO_IntRisingEdge,
    };

    /* hardware initialiize, include IOMUX, Uart debug initialize */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    
    /* Install IRQ Handler */
    SystemInitIrqTable();
    /* Tricky here: As IRQ handler in example doesn't use parameters, just ignore them */
    SystemInstallIrqHandler(GPIO3_Combined_0_15_IRQn, (system_irq_handler_t)EXAMPLE_GPIO_IRQHandler, NULL);
    PRINTF("GPIO Driver example.\r\n");

    /* Init input switch GPIO. */
    EnableIRQ(EXAMPLE_SW_IRQ);
    GPIO_PinInit(EXAMPLE_SW_GPIO, EXAMPLE_SW_GPIO_PIN, &sw_config);

    /* Enable GPIO pin interrupt */  
    GPIO_EnableInterrupts(EXAMPLE_SW_GPIO, 1U << EXAMPLE_SW_GPIO_PIN);

    while(1)
    {
        if(g_InputSignal)
        {
            delay();
            if(1 == GPIO_ReadPinInput(EXAMPLE_SW_GPIO, EXAMPLE_SW_GPIO_PIN))
            {
                PRINTF("%s is turned on.\r\n", EXAMPLE_SW_NAME);
            }
            /* Reset state of switch. */
            g_InputSignal = false; 
        }
    }
}

static void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __NOP(); /* delay */
    }
}
