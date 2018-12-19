/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_device_registers.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(__GNUC__)
#define HAL_GPIO_CLZ(x) __builtin_clz(x)
#elif defined(__ICCARM__)
#define HAL_GPIO_CLZ(x) __CLZ(x)
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define HAL_GPIO_CLZ(x) __builtin_clz(x)
#else
#error The tool-chain is not supported.
#endif
#define HAL_GPIO_BSR(x) (31 - HAL_GPIO_CLZ(x))

typedef struct _hal_gpio_state
{
    struct _hal_gpio_state *next;
    hal_gpio_callback_t callback;
    void *callbackParam;
    hal_gpio_pin_config_t pin;
    hal_gpio_interrupt_trigger_t trigger;
} hal_gpio_state_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static hal_gpio_state_t *s_GpioHead;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void HAL_GpioInterruptHandle(uint8_t port)
{
    GPIO_Type *gpioList[] = GPIO_BASE_PTRS;
    hal_gpio_state_t *head = s_GpioHead;
    uint32_t pinInterruptSetFlag;
    uint8_t pin;

    while (head)
    {
        if (head->pin.port == port)
        {
            break;
        }
        head = head->next;
    }

    if (NULL == head)
    {
        return;
    }

    head = s_GpioHead;

    pinInterruptSetFlag = GPIO_PortGetInterruptFlags(gpioList[port]);
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(gpioList[port], pinInterruptSetFlag);

    if (pinInterruptSetFlag)
    {
        pin = HAL_GPIO_BSR(pinInterruptSetFlag);
        while (head)
        {
            if ((pin == head->pin.pin) && (port == head->pin.port))
            {
                if (NULL != head->callback)
                {
                    head->callback(head->callbackParam);
                }
                pinInterruptSetFlag &= ~(1U << pin);
                if (!pinInterruptSetFlag)
                {
                    break;
                }
                pin = HAL_GPIO_BSR(pinInterruptSetFlag);
            }
            head = head->next;
        }
    }
}

static hal_gpio_status_t HAL_GpioConflictSearch(hal_gpio_state_t *head, uint8_t port, uint8_t pin)
{
    while (head)
    {
        if ((head->pin.port == port) && (head->pin.pin == pin))
        {
            return kStatus_HAL_GpioPinConflict;
        }
        head = head->next;
    }
    return kStatus_HAL_GpioSuccess;
}

static hal_gpio_status_t HAL_GpioAddItem(hal_gpio_state_t **head, hal_gpio_state_t *node)
{
    hal_gpio_state_t *p = *head;
    hal_gpio_state_t *q = *head;
    uint32_t regPrimask;

    regPrimask = DisableGlobalIRQ();

    if (NULL == p)
    {
        *head = node;
    }
    else
    {
        while (p)
        {
            if (p == node)
            {
                EnableGlobalIRQ(regPrimask);
                return kStatus_HAL_GpioPinConflict;
            }
            q = p;
            p = p->next;
        }

        q->next = node;
    }
    node->next = NULL;
    EnableGlobalIRQ(regPrimask);
    return kStatus_HAL_GpioSuccess;
}

static hal_gpio_status_t HAL_GpioRemoveItem(hal_gpio_state_t **head, hal_gpio_state_t *node)
{
    hal_gpio_state_t *p = *head;
    hal_gpio_state_t *q = NULL;
    uint32_t regPrimask;

    regPrimask = DisableGlobalIRQ();
    while (p)
    {
        if (p == node)
        {
            if (NULL == q)
            {
                *head = p->next;
            }
            else
            {
                q->next = p->next;
            }
            break;
        }
        else
        {
            q = p;
            p = p->next;
        }
    }
    EnableGlobalIRQ(regPrimask);
    return kStatus_HAL_GpioSuccess;
}

void PORTA_IRQHandler(void)
{
    HAL_GpioInterruptHandle(0);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void PORTB_IRQHandler(void)
{
    HAL_GpioInterruptHandle(1);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void PORTC_IRQHandler(void)
{
    HAL_GpioInterruptHandle(2);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void PORTB_PORTC_IRQHandler(void)
{
    HAL_GpioInterruptHandle(1);
    HAL_GpioInterruptHandle(2);
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void PORTB_PORTC_PORTD_PORTE_IRQHandler(void)
{
    HAL_GpioInterruptHandle(1);
    HAL_GpioInterruptHandle(2);
    HAL_GpioInterruptHandle(3);
    HAL_GpioInterruptHandle(4);
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void PORTC_PORTD_IRQHandler(void)
{
    HAL_GpioInterruptHandle(2);
    HAL_GpioInterruptHandle(3);
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void PORTD_IRQHandler(void)
{
    HAL_GpioInterruptHandle(3);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void PORTE_IRQHandler(void)
{
    HAL_GpioInterruptHandle(4);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void PORTF_IRQHandler(void)
{
    HAL_GpioInterruptHandle(5);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

hal_gpio_status_t HAL_GpioInit(hal_gpio_handle_t gpioHandle, hal_gpio_pin_config_t *pinConfig)
{
    hal_gpio_state_t *gpioState;
    GPIO_Type *gpioList[] = GPIO_BASE_PTRS;
    hal_gpio_status_t status;
    gpio_pin_config_t gpioPinconfig = {
        kGPIO_DigitalInput, 0,
    };

    assert(gpioHandle);
    assert(pinConfig);

    if (HAL_GPIO_HANDLE_SIZE < sizeof(hal_gpio_state_t))
    {
        return kStatus_HAL_GpioError;
    }

    gpioState = (hal_gpio_state_t *)gpioHandle;

    if ((NULL != s_GpioHead) &&
        (kStatus_HAL_GpioSuccess != HAL_GpioConflictSearch(s_GpioHead, pinConfig->port, pinConfig->pin)))
    {
        return kStatus_HAL_GpioPinConflict;
    }

    status = HAL_GpioAddItem(&s_GpioHead, gpioState);
    if (kStatus_HAL_GpioSuccess != status)
    {
        return status;
    }

    gpioState->pin.pin = pinConfig->pin;
    gpioState->pin.port = pinConfig->port;
    gpioState->pin.direction = pinConfig->direction;
    if (kHAL_GpioDirectionOut == pinConfig->direction)
    {
        gpioPinconfig.pinDirection = kGPIO_DigitalOutput;
    }
    else
    {
        gpioPinconfig.pinDirection = kGPIO_DigitalInput;
    }
    GPIO_PinInit(gpioList[pinConfig->port], pinConfig->pin, &gpioPinconfig);

    return kStatus_HAL_GpioSuccess;
}

hal_gpio_status_t HAL_GpioDeinit(hal_gpio_handle_t gpioHandle)
{
    hal_gpio_state_t *gpioState;

    assert(gpioHandle);

    gpioState = (hal_gpio_state_t *)gpioHandle;
    if (kHAL_GpioDirectionIn == gpioState->pin.direction)
    {
        HAL_GpioSetTriggerMode(gpioHandle, kHAL_GpioInterruptDisable);
    }
    HAL_GpioRemoveItem(&s_GpioHead, gpioState);
    return kStatus_HAL_GpioSuccess;
}

hal_gpio_status_t HAL_GpioGetInput(hal_gpio_handle_t gpioHandle, uint8_t *pinState)
{
    hal_gpio_state_t *gpioState;
    GPIO_Type *gpioList[] = GPIO_BASE_PTRS;

    assert(gpioHandle);
    assert(pinState);

    gpioState = (hal_gpio_state_t *)gpioHandle;

    *pinState = (GPIO_PinRead(gpioList[gpioState->pin.port], gpioState->pin.pin)) ? 1 : 0;
    return kStatus_HAL_GpioSuccess;
}

hal_gpio_status_t HAL_GpioSetOutput(hal_gpio_handle_t gpioHandle, uint8_t pinState)
{
    hal_gpio_state_t *gpioState;
    GPIO_Type *gpioList[] = GPIO_BASE_PTRS;

    assert(gpioHandle);

    gpioState = (hal_gpio_state_t *)gpioHandle;

    GPIO_PinWrite(gpioList[gpioState->pin.port], gpioState->pin.pin, (pinState) ? 1 : 0);
    return kStatus_HAL_GpioSuccess;
}

hal_gpio_status_t HAL_GpioInstallCallback(hal_gpio_handle_t gpioHandle,
                                          hal_gpio_callback_t callback,
                                          void *callbackParam)
{
    hal_gpio_state_t *gpioState;

    assert(gpioHandle);

    gpioState = (hal_gpio_state_t *)gpioHandle;

    gpioState->callbackParam = callbackParam;
    gpioState->callback = callback;

    return kStatus_HAL_GpioSuccess;
}

hal_gpio_status_t HAL_GpioGetTriggerMode(hal_gpio_handle_t gpioHandle, hal_gpio_interrupt_trigger_t *gpioTrigger)
{
    hal_gpio_state_t *gpioState;

    assert(gpioHandle);

    gpioState = (hal_gpio_state_t *)gpioHandle;

    if (kHAL_GpioDirectionOut == gpioState->pin.direction)
    {
        return kStatus_HAL_GpioError;
    }

    *gpioTrigger = gpioState->trigger;
    return kStatus_HAL_GpioSuccess;
}

hal_gpio_status_t HAL_GpioSetTriggerMode(hal_gpio_handle_t gpioHandle, hal_gpio_interrupt_trigger_t gpioTrigger)
{
    PORT_Type *portList[] = PORT_BASE_PTRS;
    IRQn_Type portIrq[] = PORT_IRQS;
    hal_gpio_state_t *gpioState;
    port_interrupt_t pinInt;

    assert(gpioHandle);

    gpioState = (hal_gpio_state_t *)gpioHandle;

    if (kHAL_GpioDirectionOut == gpioState->pin.direction)
    {
        return kStatus_HAL_GpioError;
    }

    switch (gpioTrigger)
    {
        case kHAL_GpioInterruptLogicZero:
            pinInt = kPORT_InterruptLogicZero;
            break;
        case kHAL_GpioInterruptLogicOne:
            pinInt = kPORT_InterruptLogicOne;
            break;
        case kHAL_GpioInterruptRisingEdge:
            pinInt = kPORT_InterruptRisingEdge;
            break;
        case kHAL_GpioInterruptFallingEdge:
            pinInt = kPORT_InterruptFallingEdge;
            break;
        case kHAL_GpioInterruptEitherEdge:
            pinInt = kPORT_InterruptEitherEdge;
            break;
        default:
            pinInt = kPORT_InterruptOrDMADisabled;
            break;
    }

    gpioState->trigger = gpioTrigger;

    /* initialize port interrupt */
    PORT_SetPinInterruptConfig(portList[gpioState->pin.port], gpioState->pin.pin, pinInt);
    NVIC_EnableIRQ(portIrq[gpioState->pin.port]);

    return kStatus_HAL_GpioSuccess;
}

hal_gpio_status_t HAL_GpioWakeUpSetting(hal_gpio_handle_t gpioHandle, uint8_t enable)
{
    hal_gpio_state_t *gpioState;

    assert(gpioHandle);

    gpioState = (hal_gpio_state_t *)gpioHandle;

    if (kHAL_GpioDirectionOut == gpioState->pin.direction)
    {
        return kStatus_HAL_GpioError;
    }
    /* The wakeup feature of GPIO cannot be disabled. */
    if (!enable)
    {
        return kStatus_HAL_GpioError;
    }
    return kStatus_HAL_GpioSuccess;
}

hal_gpio_status_t HAL_GpioEnterLowpower(hal_gpio_handle_t gpioHandle)
{
    assert(gpioHandle);

    return kStatus_HAL_GpioSuccess;
}

hal_gpio_status_t HAL_GpioExitLowpower(hal_gpio_handle_t gpioHandle)
{
    assert(gpioHandle);

    return kStatus_HAL_GpioSuccess;
}
