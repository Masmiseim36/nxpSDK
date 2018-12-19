/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "gpio.h"
#include "timer_manager.h"

/*
 * The OSA_USED macro can only be defined when the OSA component is used.
 * If the source code of the OSA component does not exist, the OSA_USED cannot be defined.
 * OR, If OSA component is not added into project event the OSA source code exists, the OSA_USED
 * also cannot be defined.
 * The source code path of the OSA component is <MCUXpresso_SDK>/components/osa.
 *
 */
#if defined(OSA_USED)
#include "fsl_os_abstraction.h"
#if (defined(BUTTON_USE_COMMON_TASK) && (BUTTON_USE_COMMON_TASK > 0U))
#include "common_task.h"
#endif
#endif

#include "button.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BUTTON_ENTER_CRITICAL() uint32_t regPrimask = DisableGlobalIRQ();
#define BUTTON_EXIT_CRITICAL() EnableGlobalIRQ(regPrimask);

typedef struct _button
{
    uint64_t pushPeriodCount;
    uint64_t pushPeriodCountLast;
    button_callback_t callback;
    void *callbackParam;
    uint8_t port;
    uint8_t pin;
    uint8_t pinStateDefault;
    volatile uint8_t pressed;
    volatile uint8_t pending;
} button_t;

typedef struct _button_state
{
    uint64_t periodCount;
    uint8_t timerHandleBuffer[TIMER_HANDLE_SIZE];
    timer_handle_t timerHandle;
    uint8_t gpioHandleBuffer[HAL_GPIO_HANDLE_SIZE];
    hal_gpio_handle_t gpioHandle;
#if defined(OSA_USED)

#if (defined(BUTTON_USE_COMMON_TASK) && (BUTTON_USE_COMMON_TASK > 0U))
    common_task_message_t commonTaskMsg;
#else
    uint8_t eventHandle[OSA_EVENT_HANDLE_SIZE];
    uint8_t taskHandle[OSA_TASK_HANDLE_SIZE];
#endif

#endif
    button_t button;
    button_callback_message_t msg;
    uint8_t newMessage;
} button_state_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void BUTTON_Task(void *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern const uint8_t gUseRtos_c;

#if (defined(BUTTON_USE_COMMON_TASK) && (BUTTON_USE_COMMON_TASK > 0U))

#else
/*
 * \brief Defines the button task's stack
 */
OSA_TASK_DEFINE(BUTTON_Task, BUTTON_TASK_PRIORITY, 1, BUTTON_TASK_STACK_SIZE, false);
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

static void BUTTON_NotificationUpdate(button_state_t *buttonState, button_event_t event)
{
    buttonState->button.pending = 0;
    buttonState->newMessage = 1;
    buttonState->msg.event = event;
#if defined(OSA_USED)

#if (defined(BUTTON_USE_COMMON_TASK) && (BUTTON_USE_COMMON_TASK > 0U))
    buttonState->commonTaskMsg.callback = BUTTON_Task;
    buttonState->commonTaskMsg.callbackParam = buttonState;
    COMMON_TASK_post_message(&buttonState->commonTaskMsg);
#else
    (void)OSA_EventSet((osa_event_handle_t)buttonState->eventHandle, BUTTON_EVENT_BUTTON);
#endif

#else
    BUTTON_Task(buttonState);
#endif
}

static void BUTTON_Event(void *param)
{
    button_state_t *buttonState = (button_state_t *)param;
    uint8_t pinState = 0U;

    HAL_GpioGetInput(buttonState->gpioHandle, &pinState);

    {
        pinState = (pinState) ? 1U : 0U;
        if (!buttonState->button.pressed)
        {
            if (buttonState->button.pinStateDefault != pinState)
            {
                buttonState->button.pressed = 1;
                buttonState->button.pushPeriodCount = buttonState->periodCount;
            }
        }
        else
        {
            if (buttonState->button.pinStateDefault == pinState)
            {
                buttonState->button.pressed = 0;

                if ((BUTTON_BOUBLE_CLICK_THRESHOLD + buttonState->button.pushPeriodCountLast) >=
                    buttonState->button.pushPeriodCount)
                {
                    if ((buttonState->periodCount - buttonState->button.pushPeriodCount) < BUTTON_SHORT_PRESS_THRESHOLD)
                    {
                        BUTTON_NotificationUpdate(buttonState, kBUTTON_EventDoubleClick);
                    }
                    else
                    {
                        BUTTON_NotificationUpdate(buttonState, kBUTTON_EventError);
                    }
                }
                else
                {
                    if ((buttonState->periodCount - buttonState->button.pushPeriodCount) < BUTTON_SHORT_PRESS_THRESHOLD)
                    {
                        buttonState->button.pushPeriodCountLast = buttonState->periodCount;
                        buttonState->button.pending = 1;
                    }
                    else if ((buttonState->periodCount - buttonState->button.pushPeriodCount) <
                             BUTTON_LONG_PRESS_THRESHOLD)
                    {
                        BUTTON_NotificationUpdate(buttonState, kBUTTON_EventShortPress);
                    }
                    else
                    {
                        BUTTON_NotificationUpdate(buttonState, kBUTTON_EventLongPress);
                    }
                }
            }
        }
    }
}

static void BUTTON_Task(void *param)
{
    button_state_t *buttonState = (button_state_t *)param;
    if (NULL != buttonState)
    {
#if defined(OSA_USED)

#if (defined(BUTTON_USE_COMMON_TASK) && (BUTTON_USE_COMMON_TASK > 0U))
#else
        osa_event_flags_t ev = 0;

        do
        {
            if (KOSA_StatusSuccess == OSA_EventWait((osa_event_handle_t)buttonState->eventHandle, osaEventFlagsAll_c,
                                                    false, osaWaitForever_c, &ev))
            {
#endif

#endif
        if (buttonState->newMessage)
        {
            buttonState->button.callback(buttonState, &buttonState->msg, buttonState->button.callbackParam);
            buttonState->newMessage = 0;
        }
#if defined(OSA_USED)

#if (defined(BUTTON_USE_COMMON_TASK) && (BUTTON_USE_COMMON_TASK > 0U))
#else
            }
        } while (gUseRtos_c);
#endif

#endif
    }
}

static void BUTTON_TimerEvent(void *param)
{
    button_state_t *buttonState = (button_state_t *)param;
    if (NULL != buttonState)
    {
        buttonState->periodCount += BUTTON_TIMER_INTERVAL;

        /*
         * The code block is used to indentify the button event is one click or double click.
         * If the flag pending is set and the button is not pressed, check the user activity is timeout or not.
         * If is times out, notify the upper layer it is kBUTTON_EventOneClick.
         * Otherwise, check the status next time.
         */
        if ((buttonState->button.pending) && (!buttonState->button.pressed))
        {
            /* If the flag pending and pressed are set, enter the critical section. */
            BUTTON_ENTER_CRITICAL();
            /* Check the flag pending and pressed again to make sure the flag is not changed. */
            if ((buttonState->button.pending) && (!buttonState->button.pressed))
            {
                if ((BUTTON_BOUBLE_CLICK_THRESHOLD + buttonState->button.pushPeriodCountLast) <
                    buttonState->periodCount)
                {
                    BUTTON_NotificationUpdate(buttonState, kBUTTON_EventOneClick);
                    buttonState->button.pushPeriodCountLast = 0;
                }
            }
            BUTTON_EXIT_CRITICAL();
        }
    }
}

button_status_t BUTTON_Init(button_handle_t buttonHandle, button_config_t *buttonConfig)
{
    hal_gpio_pin_config_t controlPin;
    button_state_t *buttonState;

    assert(buttonHandle);
    assert(buttonConfig);

    if (BUTTON_HANDLE_SIZE < sizeof(button_state_t))
    {
        return kStatus_BUTTON_Error;
    }

    buttonState = (button_state_t *)buttonHandle;

    for (int i = 0; i < sizeof(button_state_t); i++)
    {
        *(((uint8_t *)buttonState) + i) = 0x00;
    }

#if defined(OSA_USED)

#if (defined(BUTTON_USE_COMMON_TASK) && (BUTTON_USE_COMMON_TASK > 0U))
    COMMON_TASK_init();
#else
    if (KOSA_StatusSuccess != OSA_EventCreate((osa_event_handle_t)buttonState->eventHandle, true))
    {
        return kStatus_BUTTON_Error;
    }

    if (KOSA_StatusSuccess !=
        OSA_TaskCreate((osa_task_handle_t)buttonState->taskHandle, OSA_TASK(BUTTON_Task), buttonState))
    {
        return kStatus_BUTTON_Error;
    }
#endif

#endif

    buttonState->timerHandle = (timer_handle_t)&buttonState->timerHandleBuffer[0];
    if (kStatus_TimerSuccess != TM_Open(buttonState->timerHandle))
    {
        return kStatus_BUTTON_LackSource;
    }

    controlPin.port = buttonConfig->gpio.port;
    controlPin.pin = buttonConfig->gpio.pin;
    controlPin.direction = kHAL_GpioDirectionIn;
    buttonState->gpioHandle = (hal_gpio_handle_t)&buttonState->gpioHandleBuffer[0];
    if (kStatus_HAL_GpioSuccess != HAL_GpioInit(buttonState->gpioHandle, &controlPin))
    {
        return kStatus_BUTTON_Error;
    }

    buttonState->button.port = buttonConfig->gpio.port;
    buttonState->button.pin = buttonConfig->gpio.pin;
    buttonState->button.pinStateDefault = (buttonConfig->gpio.pinStateDefault) ? 1U : 0U;
    buttonState->button.pushPeriodCount = 0U;
    buttonState->button.pressed = 0U;
    buttonState->button.pending = 0U;
    buttonState->button.pushPeriodCountLast = 0U;

    if (kStatus_HAL_GpioSuccess != HAL_GpioSetTriggerMode(buttonState->gpioHandle, kHAL_GpioInterruptEitherEdge))
    {
        return kStatus_BUTTON_Error;
    }
    return kStatus_BUTTON_Success;
}

button_status_t BUTTON_InstallCallback(button_handle_t buttonHandle, button_callback_t callback, void *callbackParam)
{
    button_state_t *buttonState;

    assert(buttonHandle);

    buttonState = (button_state_t *)buttonHandle;

    buttonState->button.callback = callback;
    buttonState->button.callbackParam = callbackParam;

    if (kStatus_HAL_GpioSuccess != HAL_GpioInstallCallback(buttonState->gpioHandle, BUTTON_Event, buttonState))
    {
        return kStatus_BUTTON_Error;
    }

    if (kStatus_TimerSuccess != TM_InstallCallback(buttonState->timerHandle, BUTTON_TimerEvent, buttonState))
    {
        return kStatus_BUTTON_Error;
    }

    if (kStatus_TimerSuccess != TM_Start(buttonState->timerHandle, kTimerModeIntervalTimer, BUTTON_TIMER_INTERVAL))
    {
        return kStatus_BUTTON_Error;
    }

    return kStatus_BUTTON_Success;
}

button_status_t BUTTON_Deinit(button_handle_t buttonHandle)
{
    button_state_t *buttonState;

    assert(buttonHandle);

    buttonState = (button_state_t *)buttonHandle;
#if defined(OSA_USED)

#if (defined(BUTTON_USE_COMMON_TASK) && (BUTTON_USE_COMMON_TASK > 0U))

#else
    OSA_TaskDestroy((osa_task_handle_t)buttonState->taskHandle);
    OSA_EventDestroy((osa_event_handle_t)buttonState->eventHandle);
#endif

#endif
    TM_Close(buttonState->timerHandle);

    HAL_GpioDeinit(buttonState->gpioHandle);

    buttonState->gpioHandle = NULL;
    buttonState->newMessage = 0;

    return kStatus_BUTTON_Success;
}

button_status_t BUTTON_WakeUpSetting(button_handle_t buttonHandle, uint8_t enable)
{
    button_state_t *buttonState;
    hal_gpio_status_t status;

    assert(buttonHandle);

    buttonState = (button_state_t *)buttonHandle;

    status = HAL_GpioWakeUpSetting(buttonState->gpioHandle, enable);

    if (kStatus_HAL_GpioSuccess == status)
    {
        return kStatus_BUTTON_Success;
    }
    return kStatus_BUTTON_Error;
}

button_status_t BUTTON_EnterLowpower(button_handle_t buttonHandle)
{
    button_state_t *buttonState;
    hal_gpio_status_t status;

    assert(buttonHandle);

    buttonState = (button_state_t *)buttonHandle;

    status = HAL_GpioEnterLowpower(buttonState->gpioHandle);

    if (kStatus_HAL_GpioSuccess == status)
    {
        return kStatus_BUTTON_Success;
    }
    return kStatus_BUTTON_Error;
}

button_status_t BUTTON_ExitLowpower(button_handle_t buttonHandle)
{
    button_state_t *buttonState;
    hal_gpio_status_t status;

    assert(buttonHandle);

    buttonState = (button_state_t *)buttonHandle;

    status = HAL_GpioExitLowpower(buttonState->gpioHandle);

    if (kStatus_HAL_GpioSuccess == status)
    {
        return kStatus_BUTTON_Success;
    }
    return kStatus_BUTTON_Error;
}
