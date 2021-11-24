/*
############################################################################
#             Copyright (C), NXP Semiconductors
#                       (C), NXP B.V. of Eindhoven
#
# All rights are reserved. Reproduction in whole or in part is prohibited
# without the written consent of the copyright owner.
# NXP reserves the right to make changes without notice at any time.
# NXP makes no warranty, expressed, implied or statutory, including but
# not limited to any implied warranty of merchantability or fitness for
# any particular purpose, or that the use will not infringe any third
# party patent, copyright or trademark. NXP must not be liable for any
# loss or damage arising from its use.
############################################################################
*/

#include "board_define.h"

#include <FreeRTOS.h>
#include <stdlib.h>
#include <task.h>
#include <timers.h>

#include "board.h"
#include "fsl_gpio.h"
#include "sln_push_buttons_driver.h"

#define SW1_TIMER_DEBOUNCE (2)
#define SW2_TIMER_DEBOUNCE (3)
#define SW3_TIMER_DEBOUNCE (4)
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static TimerHandle_t s_SW1DebouncingTimer    = NULL;
static TimerHandle_t s_SW2DebouncingTimer    = NULL;
static TimerHandle_t s_SW3DebouncingTimer    = NULL;
static int s_SW1TimerDebounceID              = SW1_TIMER_DEBOUNCE;
static int s_SW2TimerDebounceID              = SW2_TIMER_DEBOUNCE;
static int s_SW3TimerDebounceID              = SW3_TIMER_DEBOUNCE;

static uint8_t s_msdButtonPressed = false;
static uint8_t s_otwButtonPressed = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void TimerCallback(TimerHandle_t xTimer);

status_t PUSH_BUTTONS_Init()
{
    status_t error = 0;
    //LOGD("++input_dev_push_buttons_init \r\n");

    gpio_pin_config_t input_dev_push_buttons_pin_config;

    memset(&input_dev_push_buttons_pin_config, 0, sizeof(input_dev_push_buttons_pin_config));

    input_dev_push_buttons_pin_config.direction     = kGPIO_DigitalInput;
    input_dev_push_buttons_pin_config.interruptMode = kGPIO_IntFallingEdge;

    GPIO_PinInit(BOARD_BUTTON_SW1_GPIO, BOARD_BUTTON_SW1_PIN, &input_dev_push_buttons_pin_config);
    GPIO_PinInit(BOARD_BUTTON_SW2_GPIO, BOARD_BUTTON_SW2_PIN, &input_dev_push_buttons_pin_config);
    GPIO_PinInit(BOARD_BUTTON_SW3_GPIO, BOARD_BUTTON_SW3_PIN, &input_dev_push_buttons_pin_config);

    s_SW1DebouncingTimer =
        xTimerCreate("SW1_DebounceTimer", pdMS_TO_TICKS(400), pdFALSE, (void *)&s_SW1TimerDebounceID, TimerCallback);
    if (s_SW1DebouncingTimer == NULL)
    {
        // LOGE("[ERROR] Create timer for SW1 push button failed.");
        error = 1;
    }

    s_SW2DebouncingTimer =
        xTimerCreate("SW2_DebounceTimer", pdMS_TO_TICKS(400), pdFALSE, (void *)&s_SW2TimerDebounceID, TimerCallback);
    if (s_SW2DebouncingTimer == NULL)
    {
        // LOGE("[ERROR] Create timer for SW2 push button failed.");
        error = 1;
    }

    s_SW3DebouncingTimer =
        xTimerCreate("SW3_DebounceTimer", pdMS_TO_TICKS(400), pdFALSE, (void *)&s_SW3TimerDebounceID, TimerCallback);
    if (s_SW3DebouncingTimer == NULL)
    {
        // //LOGE("[ERROR] Create timer for SW3 push button failed.");
        error = 1;
    }
    GPIO_PortClearInterruptFlags(BOARD_BUTTON_SW1_GPIO, (1 << BOARD_BUTTON_SW1_PIN));
    GPIO_PortClearInterruptFlags(BOARD_BUTTON_SW2_GPIO, (1 << BOARD_BUTTON_SW2_PIN));
    GPIO_PortClearInterruptFlags(BOARD_BUTTON_SW3_GPIO, (1 << BOARD_BUTTON_SW3_PIN));

    /* Enable Interrupts */
    // NVIC_SetPriority(INPUT_DEV_PUSH_BUTTONS_IRQ, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);
    // EnableIRQ(INPUT_DEV_PUSH_BUTTONS_IRQ);
    // GPIO_PortEnableInterrupts(BOARD_BUTTON_SW1_GPIO, (1 << BOARD_BUTTON_SW1_PIN));
    // GPIO_PortEnableInterrupts(BOARD_BUTTON_SW2_GPIO, (1 << BOARD_BUTTON_SW2_PIN));
    // GPIO_PortEnableInterrupts(BOARD_BUTTON_SW3_GPIO, (1 << BOARD_BUTTON_SW3_PIN));

    //LOGD("--input_dev_push_buttons_init\r\n");
    return error;
}

void INPUT_DEV_PUSH_BUTTONS_IRQHandler(void)
{
    uint32_t intPin = 0x00;
    // Get interrupt flag for the GPIO
    intPin = GPIO_PortGetInterruptFlags(BOARD_BUTTON_SW1_GPIO);

    // Check for the specific button that triggered the interrupt
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        BaseType_t HigherPriorityTaskWoken = pdFALSE;

        if ((intPin >> BOARD_BUTTON_SW1_PIN) & 0x01)
        {
            {
                //LOGD("SW1 Pressed.\r\n");
                GPIO_PortDisableInterrupts(BOARD_BUTTON_SW1_GPIO, (1 << BOARD_BUTTON_SW1_PIN));

                // Prevent multiple update types being set at once
                s_otwButtonPressed = false;
                s_msdButtonPressed = true;

                /* clear the interrupt status */
                GPIO_PortClearInterruptFlags(BOARD_BUTTON_SW1_GPIO, (1 << BOARD_BUTTON_SW1_PIN));

                /* Start debounce timer */
                xTimerResetFromISR(s_SW1DebouncingTimer, &HigherPriorityTaskWoken);
                //                portYIELD_FROM_ISR(HigherPriorityTaskWoken);
            }
        }
        else if ((intPin >> BOARD_BUTTON_SW2_PIN) & 0x01)
        {
            {
                //LOGD("SW2 Pressed.\r\n");
                GPIO_PortDisableInterrupts(BOARD_BUTTON_SW2_GPIO, (1 << BOARD_BUTTON_SW2_PIN));

                // Prevent multiple update types being set at once
                s_otwButtonPressed = true;
                s_msdButtonPressed = false;

                /* clear the interrupt status */
                GPIO_PortClearInterruptFlags(BOARD_BUTTON_SW2_GPIO, (1 << BOARD_BUTTON_SW2_PIN));

                /* Start debounce timer */
                xTimerResetFromISR(s_SW2DebouncingTimer, &HigherPriorityTaskWoken);
                //                portYIELD_FROM_ISR(HigherPriorityTaskWoken);
            }
        }
        else if ((intPin >> BOARD_BUTTON_SW3_PIN) & 0x01)
        {
            {
                //LOGD("SW3 Pressed.\r\n");
                GPIO_PortDisableInterrupts(BOARD_BUTTON_SW3_GPIO, (1 << BOARD_BUTTON_SW3_PIN));
                /* clear the interrupt status */
                GPIO_PortClearInterruptFlags(BOARD_BUTTON_SW3_GPIO, (1 << BOARD_BUTTON_SW3_PIN));

                /* Start debounce timer */
                xTimerResetFromISR(s_SW3DebouncingTimer, &HigherPriorityTaskWoken);
                //                portYIELD_FROM_ISR(HigherPriorityTaskWoken);
            }
        }
        else
        {
            // This should never happen
            //LOGE("[ERROR] Unknown button interrupt");
        }
    }
}

static void TimerCallback(TimerHandle_t xTimer)
{
    switch (*(int *)pvTimerGetTimerID(xTimer))
    {
        case SW1_TIMER_DEBOUNCE:
            GPIO_PortClearInterruptFlags(BOARD_BUTTON_SW1_GPIO, 1U << BOARD_BUTTON_SW1_PIN);
            GPIO_PortEnableInterrupts(BOARD_BUTTON_SW1_GPIO, (1U << BOARD_BUTTON_SW1_PIN));
            break;
        case SW2_TIMER_DEBOUNCE:
            GPIO_PortClearInterruptFlags(BOARD_BUTTON_SW2_GPIO, 1U << BOARD_BUTTON_SW2_PIN);
            GPIO_PortEnableInterrupts(BOARD_BUTTON_SW2_GPIO, (1U << BOARD_BUTTON_SW2_PIN));
            break;
        case SW3_TIMER_DEBOUNCE:
            GPIO_PortClearInterruptFlags(BOARD_BUTTON_SW3_GPIO, 1U << BOARD_BUTTON_SW3_PIN);
            GPIO_PortEnableInterrupts(BOARD_BUTTON_SW3_GPIO, (1U << BOARD_BUTTON_SW3_PIN));
            break;
    }
}

bool PUSH_BUTTONS_MSDPressed(){
    return !GPIO_PinRead(BOARD_BUTTON_SW1_GPIO, BOARD_BUTTON_SW1_PIN);
}

bool PUSH_BUTTONS_OTWPressed(){
    return s_otwButtonPressed;
}
