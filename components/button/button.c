/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "gpio.h"
#include "timer_manager.h"

#include "button.h"
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

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BUTTON_ENTER_CRITICAL() uint32_t regPrimask = DisableGlobalIRQ();
#define BUTTON_EXIT_CRITICAL() EnableGlobalIRQ(regPrimask);

typedef struct _button_state
{
    struct _button_state *next;
    button_callback_t callback;
    void *callbackParam;
    uint8_t gpioHandle[HAL_GPIO_HANDLE_SIZE];
    volatile uint32_t pushPeriodCount;
    volatile uint32_t pushPeriodCountLast;
    struct
    {
        uint16_t port : 3U;
        uint16_t reserved : 1U;
        uint16_t pin : 5U;
        uint16_t pinStateDefault : 1U;
        uint8_t pressed : 1U;
        uint8_t pending : 1U;
        uint16_t msg : 3U;
        uint16_t newMessage : 1U;
    } state;
} button_state_t;

typedef struct _button_list
{
    volatile uint32_t periodCount;
    uint8_t timerHandle[TIMER_HANDLE_SIZE];
#if defined(OSA_USED)

#if (defined(BUTTON_USE_COMMON_TASK) && (BUTTON_USE_COMMON_TASK > 0U))
    common_task_message_t commonTaskMsg;
#else
    uint8_t eventHandle[OSA_EVENT_HANDLE_SIZE];
    uint8_t taskHandle[OSA_TASK_HANDLE_SIZE];
#endif

#endif
    button_state_t *button;
} button_list_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void BUTTON_Task(void *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static button_list_t s_buttonList;

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
    buttonState->state.pending    = 0U;
    buttonState->state.newMessage = 1U;
    buttonState->state.msg        = (uint16_t)event;
#if defined(OSA_USED)

#if (defined(BUTTON_USE_COMMON_TASK) && (BUTTON_USE_COMMON_TASK > 0U))
    s_buttonList.commonTaskMsg.callback      = BUTTON_Task;
    s_buttonList.commonTaskMsg.callbackParam = buttonState;
    COMMON_TASK_post_message(&s_buttonList.commonTaskMsg);
#else
    (void)OSA_EventSet((osa_event_handle_t)s_buttonList.eventHandle, BUTTON_EVENT_BUTTON);
#endif

#else
    BUTTON_Task(&s_buttonList);
#endif
}

static void BUTTON_Event(void *param)
{
    button_state_t *buttonState = (button_state_t *)param;
    uint8_t pinState            = 0U;

    assert(param);

    HAL_GpioGetInput(buttonState->gpioHandle, &pinState);
    pinState = (pinState) ? 1U : 0U;
    if (!buttonState->state.pressed)
    {
        if (buttonState->state.pinStateDefault != pinState)
        {
            buttonState->state.pressed   = 1;
            buttonState->pushPeriodCount = s_buttonList.periodCount;
        }
    }
    else
    {
        if (buttonState->state.pinStateDefault == pinState)
        {
            buttonState->state.pressed = 0;

            if ((BUTTON_DOUBLE_CLICK_THRESHOLD + buttonState->pushPeriodCountLast) >= buttonState->pushPeriodCount)
            {
                if ((s_buttonList.periodCount - buttonState->pushPeriodCount) < BUTTON_SHORT_PRESS_THRESHOLD)
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
                if ((s_buttonList.periodCount - buttonState->pushPeriodCount) < BUTTON_SHORT_PRESS_THRESHOLD)
                {
                    buttonState->pushPeriodCountLast = s_buttonList.periodCount;
                    buttonState->state.pending       = 1;
                }
                else if ((s_buttonList.periodCount - buttonState->pushPeriodCount) < BUTTON_LONG_PRESS_THRESHOLD)
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

static void BUTTON_Task(void *param)
{
#if defined(OSA_USED)

#if (defined(BUTTON_USE_COMMON_TASK) && (BUTTON_USE_COMMON_TASK > 0U))
#else
    osa_event_flags_t ev = 0;

    do
    {
        if (KOSA_StatusSuccess == OSA_EventWait((osa_event_handle_t)s_buttonList.eventHandle, osaEventFlagsAll_c, false,
                                                osaWaitForever_c, &ev))
        {
#endif

#endif
    button_state_t *buttonState = s_buttonList.button;

    BUTTON_ENTER_CRITICAL();
    while (buttonState != NULL)
    {
        if (buttonState->state.newMessage)
        {
            button_callback_message_t msg;
            BUTTON_EXIT_CRITICAL();
            msg.event = (button_event_t)buttonState->state.msg;
            (void)buttonState->callback(buttonState, &msg, buttonState->callbackParam);
            buttonState->state.newMessage = 0;
            regPrimask                    = DisableGlobalIRQ();
        }
        buttonState = buttonState->next;
    }
    BUTTON_EXIT_CRITICAL();
#if defined(OSA_USED)

#if (defined(BUTTON_USE_COMMON_TASK) && (BUTTON_USE_COMMON_TASK > 0U))
#else
        }
    } while (gUseRtos_c);
#endif

#endif
}

static void BUTTON_TimerEvent(void *param)
{
    button_state_t *buttonState;

    s_buttonList.periodCount += BUTTON_TIMER_INTERVAL;

    BUTTON_ENTER_CRITICAL();
    buttonState = s_buttonList.button;
    while (buttonState != NULL)
    {
        /*
         * The code block is used to indentify the button event is one click or double click.
         * If the flag pending is set and the button is not pressed, check the user activity is timeout or not.
         * If is times out, notify the upper layer it is kBUTTON_EventOneClick.
         * Otherwise, check the status next time.
         */
        if ((buttonState->state.pending) && (!buttonState->state.pressed))
        {
            /* If the flag pending and pressed are set, enter the critical section. */
            /* Check the flag pending and pressed again to make sure the flag is not changed. */
            if ((buttonState->state.pending) && (!buttonState->state.pressed))
            {
                if ((BUTTON_DOUBLE_CLICK_THRESHOLD + buttonState->pushPeriodCountLast) < s_buttonList.periodCount)
                {
                    BUTTON_NotificationUpdate(buttonState, kBUTTON_EventOneClick);
                    buttonState->pushPeriodCountLast = 0U;
                }
            }
        }
        buttonState = buttonState->next;
    }
    BUTTON_EXIT_CRITICAL();
}

button_status_t BUTTON_Init(button_handle_t buttonHandle, button_config_t *buttonConfig)
{
    hal_gpio_pin_config_t controlPin;
    button_state_t *buttonState;
    uint32_t regPrimask;
    hal_gpio_status_t gpioStatus;
    timer_status_t timerStatus;

    assert(buttonHandle && buttonConfig);
    assert(BUTTON_HANDLE_SIZE >= sizeof(button_state_t));

    buttonState = (button_state_t *)buttonHandle;

    (void)memset(buttonHandle, 0, sizeof(button_state_t));

    regPrimask = DisableGlobalIRQ();

    if (NULL == s_buttonList.button)
    {
#if defined(OSA_USED)

#if (defined(BUTTON_USE_COMMON_TASK) && (BUTTON_USE_COMMON_TASK > 0U))
        COMMON_TASK_init();
#else
        if (KOSA_StatusSuccess != OSA_EventCreate((osa_event_handle_t)s_buttonList.eventHandle, true))
        {
            return kStatus_BUTTON_Error;
        }

        if (KOSA_StatusSuccess !=
            OSA_TaskCreate((osa_task_handle_t)s_buttonList.taskHandle, OSA_TASK(BUTTON_Task), &s_buttonList))
        {
            return kStatus_BUTTON_Error;
        }
#endif

#endif
        timerStatus = TM_Open((timer_handle_t)s_buttonList.timerHandle);
        assert(kStatus_TimerSuccess == timerStatus);
        (void)timerStatus;
        timerStatus = TM_InstallCallback(s_buttonList.timerHandle, BUTTON_TimerEvent, &s_buttonList);
        assert(kStatus_TimerSuccess == timerStatus);
        (void)timerStatus;
        timerStatus = TM_Start(s_buttonList.timerHandle, kTimerModeIntervalTimer, BUTTON_TIMER_INTERVAL);
        assert(kStatus_TimerSuccess == timerStatus);
        (void)timerStatus;
    }
    else
    {
        buttonState->next = s_buttonList.button;
    }
    s_buttonList.button = buttonState;
    EnableGlobalIRQ(regPrimask);

    controlPin.port      = buttonConfig->gpio.port;
    controlPin.pin       = buttonConfig->gpio.pin;
    controlPin.direction = kHAL_GpioDirectionIn;
    gpioStatus           = HAL_GpioInit(buttonState->gpioHandle, &controlPin);
    assert(kStatus_HAL_GpioSuccess == gpioStatus);
    (void)gpioStatus;

    buttonState->state.port            = buttonConfig->gpio.port;
    buttonState->state.pin             = buttonConfig->gpio.pin;
    buttonState->state.pinStateDefault = (buttonConfig->gpio.pinStateDefault) ? 1U : 0U;

    gpioStatus = HAL_GpioSetTriggerMode(buttonState->gpioHandle, kHAL_GpioInterruptEitherEdge);
    assert(kStatus_HAL_GpioSuccess == gpioStatus);
    (void)gpioStatus;

    return kStatus_BUTTON_Success;
}

button_status_t BUTTON_InstallCallback(button_handle_t buttonHandle, button_callback_t callback, void *callbackParam)
{
    button_state_t *buttonState;
    assert(buttonHandle);

    buttonState = (button_state_t *)buttonHandle;

    buttonState->callback      = callback;
    buttonState->callbackParam = callbackParam;

    (void)HAL_GpioInstallCallback(buttonState->gpioHandle, BUTTON_Event, buttonState);

    return kStatus_BUTTON_Success;
}

button_status_t BUTTON_Deinit(button_handle_t buttonHandle)
{
    button_state_t *buttonState;
    button_state_t *buttonStatePre;
    uint32_t regPrimask;

    assert(buttonHandle);

    buttonState = (button_state_t *)buttonHandle;

    regPrimask     = DisableGlobalIRQ();
    buttonStatePre = s_buttonList.button;
    if (buttonStatePre != buttonState)
    {
        while ((NULL != buttonStatePre) && (buttonStatePre->next != buttonState))
        {
            buttonStatePre = buttonStatePre->next;
        }
        if (NULL != buttonStatePre)
        {
            buttonStatePre->next = buttonState->next;
        }
    }
    else
    {
        s_buttonList.button = buttonState->next;
    }

    if (NULL == s_buttonList.button)
    {
#if defined(OSA_USED)

#if (defined(BUTTON_USE_COMMON_TASK) && (BUTTON_USE_COMMON_TASK > 0U))

#else
        (void)OSA_TaskDestroy((osa_task_handle_t)s_buttonList.taskHandle);
        (void)OSA_EventDestroy((osa_event_handle_t)s_buttonList.eventHandle);
#endif

#endif
        (void)TM_Close(s_buttonList.timerHandle);
    }
    EnableGlobalIRQ(regPrimask);

    (void)HAL_GpioDeinit(buttonState->gpioHandle);
    buttonState->state.newMessage = 0;

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
