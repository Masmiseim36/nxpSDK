/*
 * Copyright 2017 NXP
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

#include "fsl_tsc.h"
#include "fsl_adc_5hc.h"
#include "touch.h"
#include "demo_config.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _touch_state
{
    bool isPressDown; /*!< Is press down or not. */
    uint16_t x, y;    /*!< Touch point coordinates, if the touch screen is calibrated,
                           these are the coordinates after calibration, if not
                           calibrated, these are the coordinates got from ADC directly. */
} touch_state_t;

typedef struct _touch_handle
{
    SemaphoreHandle_t mutex;
    touch_state_t state;
    touch_callback_t callback;
    void *callbackData;
} touch_handle_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
static touch_handle_t s_touchHandle;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void TOUCH_Exec(TimerHandle_t timer);

static void TOUCH_InitHardware(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
void TOUCH_Init(touch_callback_t callback, void *userData)
{
    TimerHandle_t touchTimer;

    memset(&s_touchHandle, 0, sizeof(s_touchHandle));

    TOUCH_InitHardware();

    s_touchHandle.mutex = xSemaphoreCreateRecursiveMutex();

    configASSERT(NULL != s_touchHandle.mutex);

    /* Create timer to run TOUCH_Exec periodically. */
    touchTimer = xTimerCreate("Touch Timer", 5, pdTRUE, 0, TOUCH_Exec);

    configASSERT(NULL != touchTimer);

    s_touchHandle.callbackData = userData;
    s_touchHandle.callback = callback;

    xTimerStart(touchTimer, portMAX_DELAY);

    TSC_SoftwareReset(TOUCH_TSC);
    TSC_StartSenseDetection(TOUCH_TSC);
}

static void TOUCH_Exec(TimerHandle_t timer)
{
    touch_event_t event;
    touch_state_t state;
    bool sendEvent = false;
    bool pressValid = false;
    uint32_t stateMachine;
    uint32_t intStatus;

    stateMachine = TSC_GetStatusFlags(TOUCH_TSC) & kTSC_StateMachineFlag;
    intStatus = TSC_GetInterruptStatusFlags(TOUCH_TSC);

    if (((0x01 << 20) == stateMachine) || ((0x02 << 20) == stateMachine))
    {
        state.isPressDown = false;
    }
    else if ((0x00 << 20) == stateMachine)
    {
        TSC_ClearInterruptStatusFlags(TOUCH_TSC, intStatus);
        state.isPressDown = true;

        if (kTSC_ValidSignalFlag & intStatus)
        {
            pressValid = true;
            state.x = TSC_GetMeasureValue(TOUCH_TSC, kTSC_XCoordinateValueSelection);
            state.y = TSC_GetMeasureValue(TOUCH_TSC, kTSC_YCoordinateValueSelection);
        }

        TSC_StartSenseDetection(TOUCH_TSC);
    }
    else
    {
        return;
    }

    xSemaphoreTakeRecursive(s_touchHandle.mutex, portMAX_DELAY);

    /*
     *    Old state     |     New State     |     Action
     *      Down        |       Down        | Check touch points to send event
     *      Down        |        Up         | Send "UP" event
     *       Up         |       Down        | Send "DOWN" event
     */

    if (state.isPressDown)
    {
        if (pressValid)
        {
            if (s_touchHandle.state.isPressDown)
            {
                if ((state.x != s_touchHandle.state.x) || (state.y != s_touchHandle.state.y))
                {
                    s_touchHandle.state.x = state.x;
                    s_touchHandle.state.y = state.y;
                    sendEvent = true;
                    event = kTOUCH_Contact;
                }
            }
            else
            {
                s_touchHandle.state.x = state.x;
                s_touchHandle.state.y = state.y;
                sendEvent = true;
                event = kTOUCH_Down;
            }

            s_touchHandle.state.isPressDown = true;
        }
    }
    else
    {
        if (s_touchHandle.state.isPressDown)
        {
            sendEvent = true;
            event = kTOUCH_Up;
            s_touchHandle.state.isPressDown = false;
        }
    }

    xSemaphoreGiveRecursive(s_touchHandle.mutex);

    if (sendEvent && (NULL != s_touchHandle.callback))
    {
        s_touchHandle.callback(event, s_touchHandle.state.x, s_touchHandle.state.y, s_touchHandle.callbackData);
    }
}

static void TOUCH_InitHardware(void)
{
    status_t status;

    tsc_config_t tscConfig;
    adc_5hc_config_t adcConfig;
    adc_5hc_channel_config_t adcChannelConfig;

    /* Initialize the ADC module. */
    ADC_5HC_GetDefaultConfig(&adcConfig);
    ADC_5HC_Init(TOUCH_ADC, &adcConfig);
    ADC_5HC_EnableHardwareTrigger(TOUCH_ADC, true);

    /* Before TSC starts work, software driver configure ADC_HCx.
     * For four-wire resistive screen, x-coordinate measure triggers to ADC_HC3
     * and y-coordinate measure triggers to ADC_HC1. So we need configure ADC_HC1
     * and ADC_HC3 to make sure TOUCH_TSC could work normally.
     */
    adcChannelConfig.channelNumber = 1U; /* Channel1 is ynlr port. */
    adcChannelConfig.enableInterruptOnConversionCompleted = false;
    ADC_5HC_SetChannelConfig(TOUCH_ADC, 3U, &adcChannelConfig);
    adcChannelConfig.channelNumber = 3U; /* Channel3 is xnur port. */
    ADC_5HC_SetChannelConfig(TOUCH_ADC, 1U, &adcChannelConfig);

    /* Do auto hardware calibration. */
    status = ADC_5HC_DoAutoCalibration(TOUCH_ADC);

    configASSERT(kStatus_Success == status);

    /*
     * config->enableAutoMeausre = false;
     * config->measureDelayTime = 0xFFFFU;
     * config->prechargeTime = 0xFFFFU;
     * config->detectionMode = kTSC_4WireDetectionMode;
     */
    TSC_GetDefaultConfig(&tscConfig);
    tscConfig.enableAutoMeasure = true;

    TSC_Init(TOUCH_TSC, &tscConfig);
    TSC_EnableInterruptSignals(TOUCH_TSC, kTSC_MeasureSignalEnable | kTSC_ValidSignalEnable);
}
