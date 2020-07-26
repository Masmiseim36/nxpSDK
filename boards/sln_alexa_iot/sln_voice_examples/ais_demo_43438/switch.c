/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <switch.h>

/*******************************************************************************
 * Code
 ******************************************************************************/
EventGroupHandle_t g_button_pressed;
extern void button_callback(int button_nr, int state);
static uint32_t current_state[SW_COUNT];
static uint32_t last_state[SW_COUNT] = {15, 15};
static uint32_t last_debounce[SW_COUNT];
/*!
 * @brief Interrupt service fuction of switch.
 */
void GPIO_IRQHandler(void)
{
    OsaTimeval timeval = {0};
    uint32_t intPin    = 0x00;

    /* Get interrupt flag for the GPIO */
    intPin = GPIO_PortGetInterruptFlags(SW1_GPIO);

    /* Check for the interrupt pin on the GPIO */
    if ((intPin >> SW1_GPIO_PIN) & 0x01)
    {
        current_state[0] = GPIO_PinRead(SW1_GPIO, SW1_GPIO_PIN);

        if (current_state[0] != last_state[0])
        {
            osa_time_get(&timeval);

            if (timeval.tv_usec - last_debounce[0] > DEBOUNCE_TIME_US)
            {
                button_callback(1, current_state[0]);
            }

            last_debounce[0] = timeval.tv_usec;
        }

        /* clear the interrupt status */
        GPIO_PortClearInterruptFlags(SW1_GPIO, 1U << SW1_GPIO_PIN);

        last_state[0] = current_state[0];
    }

    if ((intPin >> SW2_GPIO_PIN) & 0x01)
    {
        current_state[1] = GPIO_PinRead(SW2_GPIO, SW2_GPIO_PIN);

        if (current_state[1] != last_state[1])
        {
            osa_time_get(&timeval);

            if (timeval.tv_usec - last_debounce[1] > DEBOUNCE_TIME_US)
            {
                button_callback(2, current_state[1]);
            }

            last_debounce[1] = timeval.tv_usec;
        }

        /* clear the interrupt status */
        GPIO_PortClearInterruptFlags(SW2_GPIO, 1U << SW2_GPIO_PIN);

        last_state[1] = current_state[1];
    }

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void switchInit(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
        kGPIO_IntRisingOrFallingEdge,
    };

    g_button_pressed = xEventGroupCreate();
    last_state[0]    = RELEASED;
    last_state[1]    = RELEASED;

    /* Init input switch GPIO. */
    NVIC_SetPriority(BOARD_USER_BUTTON_IRQ, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);
    EnableIRQ(BOARD_USER_BUTTON_IRQ);
    GPIO_PinInit(SW1_GPIO, SW1_GPIO_PIN, &sw_config);
    GPIO_PortClearInterruptFlags(SW1_GPIO, 1U << SW1_GPIO_PIN);

    GPIO_PinInit(SW2_GPIO, SW2_GPIO_PIN, &sw_config);
    GPIO_PortClearInterruptFlags(SW2_GPIO, 1U << SW2_GPIO_PIN);

    /* Enable GPIO pin interrupt */
    GPIO_PortEnableInterrupts(SW1_GPIO, 1U << SW1_GPIO_PIN);
    GPIO_PortEnableInterrupts(SW2_GPIO, 1U << SW2_GPIO_PIN);
}
