/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "ux_attention_system.h"
#include "FreeRTOSConfig.h"
#include "sln_RT10xx_RGB_LED_driver.h"
#include "sln_amplifier.h"
#include "FreeRTOSConfig.h"
#include "ais_alerts.h"
#include "audio_processing_task.h"

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_pit.h"

#include "clock_config.h"
#include "pin_mux.h"

#include "queue.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PIT_LED_HANDLER PIT_IRQHandler
#define PIT_IRQ_ID      PIT_IRQn

#if configMAX_SYSCALL_INTERRUPT_PRIORITY > 0
#define PIT_IRQ_PRIORITY (configMAX_SYSCALL_INTERRUPT_PRIORITY - 1)
#else
#error "configMAX_SYSCALL_INTERRUPT_PRIORITY must be greater than 0"
#endif

/* Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_OscClk)

/*! @brief uvoice UX Attention Task settings */
#define UX_ATT_TASK_NAME     "ux_attention_task"
#define UX_ATT_TASK_STACK    256U
#define UX_ATT_TASK_PRIORITY (configTIMER_TASK_PRIORITY - 2)

static TimerHandle_t s_speakingAlertTimerHandle;
#define UX_ALERT_SPEAKING_INTERVAL_MSEC 10000

static rgb_led_brightness_t currentBrightness = LED_BRIGHT_HIGH;

volatile bool toggle;
volatile rgbLedColor_t pit_led_color1 = LED_COLOR_CYAN;
volatile rgbLedColor_t pit_led_color2 = LED_COLOR_BLUE;
uint16_t toggleCount                  = 0;
bool toggleCountEnabled               = 0;
bool resumePrevState                  = 0;

QueueHandle_t g_uxQueue;

#define RBG_PATTERN_MAX_COUNT 8U

uint32_t pit_led_index                                      = 0;
uint32_t pit_led_count                                      = 0;
volatile rgbLedColor_t pit_led_color[RBG_PATTERN_MAX_COUNT] = {0};

void PIT_LED_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);

    if (pit_led_index >= pit_led_count)
    {
        pit_led_index = 0;
    }

    RGB_LED_SetBrightnessColor(currentBrightness, pit_led_color[pit_led_index]);

    if ((pit_led_index & 0x1))
    {
        toggleCount--;
    }

    if ((toggleCountEnabled == 1) && (toggleCount == 0))
    {
        PIT_StopTimer(PIT, kPIT_Chnl_0);
        if (resumePrevState)
        {
            ux_attention_resume_state(AIS_APP_GetAppData()->state);
            resumePrevState = false;
        }
        else
        {
            RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_OFF);
        }
    }

    pit_led_index++;
}

/*  Current State Machine isn't very good..it will handle the Main State fine but the auxiliary state handling needs
    more work.

    MAIN_STATE = IDLE|LISTENING|THINKING|SPEAKING
    Main State can only be one of these at a time.
    Auxiliary States can all be independent but I need to determine precedence for LED?
    Aux States: MIC_STATE = ON|OFF
                TIMER_STATE = ON|OFF
                ALARM_STATE = ON|OFF
                NOTIFICATION_STATE = ON|OFF
                REMINDER_STATE = ON|OFF

    TODO: Improve how to handle the impact of the Auxiliary States with the Main and with each other.
*/

static ux_attention_states_t prevState = uxIdle;

void ux_speaking_alert_timer_cb(TimerHandle_t xTimer)
{
    if (uxTimerShort == prevState)
    {
        ux_attention_set_state(uxTimerShort);
    }
}

void ux_attention_task(void *pvParameters)
{
    bool speakingState            = false;
    prevState                     = uxIdle;
    ux_attention_states_t uxState = uxNull;

    while (1)
    {
        if (g_uxQueue != 0)
        {
            xQueueReceive(g_uxQueue, &uxState, (TickType_t)500);
        }

        if (uxHighBrightness == uxState)
        {
            currentBrightness = LED_BRIGHT_HIGH;
            uxState           = prevState;
        }

        if (uxMediumBrightness == uxState)
        {
            currentBrightness = LED_BRIGHT_MEDIUM;
            uxState           = prevState;
        }

        if (uxLowBrightness == uxState)
        {
            currentBrightness = LED_BRIGHT_LOW;
            uxState           = prevState;
        }

        // idle
        if (uxIdle == uxState)
        {
            PIT_StopTimer(PIT, kPIT_Chnl_0);

            rgbLedColor_t muteColour = ((audio_processing_get_mic_mute()) ? LED_COLOR_RED : LED_COLOR_OFF);

            RGB_LED_SetBrightnessColor(currentBrightness, muteColour);
            toggleCountEnabled = 0;
            speakingState      = false;
            prevState          = uxIdle;
        }

        /*LISTENING START = WakeWordTriggered */
        if (uxListeningStart == uxState)
        {
            toggleCountEnabled = 0;
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_BLUE);

            prevState = uxListeningStart;
        }

        // LISTENING ACTIVE
        if (uxListeningActive == uxState)
        {
            vTaskDelay(0xF);
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_CYAN);
            toggleCountEnabled = 0;

            prevState = uxListeningActive;
        }

        // LISTENING END
        if (uxListeningEnd == uxState)
        {
            // We should stop the leds only if the prevState was uxListeningActive otherwise we interfere with other
            // uxStates
            if (uxListeningActive == prevState)
            {
                PIT_StopTimer(PIT, kPIT_Chnl_0);
                toggleCountEnabled = 0;
                RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_OFF);
            }

            prevState = uxListeningEnd;
        }

        // THINKING : alternate blue and cyan every 200ms
        if (uxThinking == uxState)
        {
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCountEnabled = 0;
            pit_led_color[0]   = LED_COLOR_CYAN;
            pit_led_color[1]   = LED_COLOR_BLUE;

            // 200ms timer
            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(200000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxThinking;
        }

        // Need to think more clearly what happens when thinking Ends.For now assume moving to speaking state
        // or listening state or idle state is the end of thinking state.

        // SPEAKING: alternate blue and cyan every 500ms
        if (uxSpeaking == uxState)
        {
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCountEnabled = 0;
            pit_led_color[0]   = LED_COLOR_CYAN;
            pit_led_color[1]   = LED_COLOR_BLUE;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(500000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);
            speakingState = true;

            prevState = uxSpeaking;
        }

        // END SPEAKING:
        if (uxSpeakingEnd == uxState)
        {
            PIT_StopTimer(PIT, kPIT_Chnl_0);

            rgbLedColor_t muteColour = ((audio_processing_get_mic_mute()) ? LED_COLOR_RED : LED_COLOR_OFF);

            RGB_LED_SetBrightnessColor(currentBrightness, muteColour);
            toggleCountEnabled = 0;

            speakingState = false;
            // If timer/alarm/reminder flag then play long versions of timer,alarm, reminder

            prevState = uxIdle;
        }

        // MIC ON to MIC OFF
        if (uxMicOntoOff == uxState)
        {
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCount        = 1;
            toggleCountEnabled = 1;
            resumePrevState    = true;
            pit_led_color[0]   = LED_COLOR_RED;
            pit_led_color[1]   = LED_COLOR_RED;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(250000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxMicOntoOff;
        }

        // MIC OFF TO MIC ON
        if (uxMicOfftoOn == uxState)
        {
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCount        = 1;
            toggleCountEnabled = 1;
            resumePrevState    = true;
            pit_led_color[0]   = LED_COLOR_OFF;
            pit_led_color[1]   = LED_COLOR_OFF;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(250000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxMicOfftoOn;
        }

        if (speakingState)
        {
            /* If uvoice is speaking, play the shortened versions of the alerts */
            /* TODO: speakingState not currently enabled because not sure how to interrupt speaking audio */

            if ((uxTimerShort == uxState) || (uxTimer == uxState))
            {
                // TODO: Play toggling cyan/blue led and audio prompt every 10 seconds
                PIT_StopTimer(PIT, kPIT_Chnl_0);
                pit_led_index      = 0;
                pit_led_count      = 8;
                toggleCount        = 16;
                toggleCountEnabled = 1;

                rgbLedColor_t muteColour = ((audio_processing_get_mic_mute()) ? LED_COLOR_RED : LED_COLOR_OFF);

                pit_led_color[0] = LED_COLOR_BLUE;
                pit_led_color[1] = muteColour;
                pit_led_color[2] = LED_COLOR_BLUE;
                pit_led_color[3] = muteColour;
                pit_led_color[4] = LED_COLOR_CYAN;
                pit_led_color[5] = muteColour;
                pit_led_color[6] = LED_COLOR_CYAN;
                pit_led_color[7] = muteColour;

                PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(250000U, PIT_SOURCE_CLOCK));
                PIT_StartTimer(PIT, kPIT_Chnl_0);

                if (xTimerReset(s_speakingAlertTimerHandle, 0) != pdPASS)
                {
                    configPRINTF(("xTimerReset failed\r\n"));
                }

                prevState = uxTimerShort;
            }

            if ((uxAlarmShort == uxState) || (uxAlarm == uxState))
            {
                // TODO: Play toggling cyan/blue led and audio prompt every 10 seconds
                PIT_StopTimer(PIT, kPIT_Chnl_0);
                pit_led_index      = 0;
                pit_led_count      = 8;
                toggleCount        = 16;
                toggleCountEnabled = 1;

                rgbLedColor_t muteColour = ((audio_processing_get_mic_mute()) ? LED_COLOR_RED : LED_COLOR_OFF);

                pit_led_color[0] = LED_COLOR_BLUE;
                pit_led_color[1] = muteColour;
                pit_led_color[2] = LED_COLOR_BLUE;
                pit_led_color[3] = muteColour;
                pit_led_color[4] = LED_COLOR_CYAN;
                pit_led_color[5] = muteColour;
                pit_led_color[6] = LED_COLOR_CYAN;
                pit_led_color[7] = muteColour;

                PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(250000U, PIT_SOURCE_CLOCK));
                PIT_StartTimer(PIT, kPIT_Chnl_0);

                if (xTimerReset(s_speakingAlertTimerHandle, 0) != pdPASS)
                {
                    configPRINTF(("xTimerReset failed\r\n"));
                }

                prevState = uxAlarmShort;
            }

            if ((uxReminderShort == uxState) || (uxReminder == uxState))
            {
                // TODO: Play toggling cyan/blue led and audio prompt every 10 seconds
                PIT_StopTimer(PIT, kPIT_Chnl_0);
                pit_led_index      = 0;
                pit_led_count      = 8;
                toggleCount        = 16;
                toggleCountEnabled = 1;

                rgbLedColor_t muteColour = ((audio_processing_get_mic_mute()) ? LED_COLOR_RED : LED_COLOR_OFF);

                pit_led_color[0] = LED_COLOR_BLUE;
                pit_led_color[1] = muteColour;
                pit_led_color[2] = LED_COLOR_BLUE;
                pit_led_color[3] = muteColour;
                pit_led_color[4] = LED_COLOR_CYAN;
                pit_led_color[5] = muteColour;
                pit_led_color[6] = LED_COLOR_CYAN;
                pit_led_color[7] = muteColour;

                PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(250000U, PIT_SOURCE_CLOCK));
                PIT_StartTimer(PIT, kPIT_Chnl_0);

                if (xTimerReset(s_speakingAlertTimerHandle, 0) != pdPASS)
                {
                    configPRINTF(("xTimerReset failed\r\n"));
                }

                prevState = uxReminderShort;
            }
        }
        else
        {
            if (uxTimer == uxState)
            {
                // loop between cyan and blue every 300ms
                PIT_StopTimer(PIT, kPIT_Chnl_0);
                pit_led_index      = 0;
                pit_led_count      = 8;
                toggleCountEnabled = 0;

                rgbLedColor_t muteColour = ((audio_processing_get_mic_mute()) ? LED_COLOR_RED : LED_COLOR_OFF);

                pit_led_color[0] = LED_COLOR_BLUE;
                pit_led_color[1] = muteColour;
                pit_led_color[2] = LED_COLOR_BLUE;
                pit_led_color[3] = muteColour;
                pit_led_color[4] = LED_COLOR_CYAN;
                pit_led_color[5] = muteColour;
                pit_led_color[6] = LED_COLOR_CYAN;
                pit_led_color[7] = muteColour;

                PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(250000U, PIT_SOURCE_CLOCK));
                PIT_StartTimer(PIT, kPIT_Chnl_0);

                // SLN_AMP_Write((uint8_t *)med_system_alerts_melodic_01_mono_wav,
                // MED_SYSTEM_ALERTS_MELODIC_01_MONO_WAV_LEN);

                prevState = uxTimer;
            }

            if (uxAlarm == uxState)
            {
                // loop between cyan and blue every 300ms
                PIT_StopTimer(PIT, kPIT_Chnl_0);
                pit_led_index      = 0;
                pit_led_count      = 8;
                toggleCountEnabled = 0;

                rgbLedColor_t muteColour = ((audio_processing_get_mic_mute()) ? LED_COLOR_RED : LED_COLOR_OFF);

                pit_led_color[0] = LED_COLOR_BLUE;
                pit_led_color[1] = muteColour;
                pit_led_color[2] = LED_COLOR_BLUE;
                pit_led_color[3] = muteColour;
                pit_led_color[4] = LED_COLOR_CYAN;
                pit_led_color[5] = muteColour;
                pit_led_color[6] = LED_COLOR_CYAN;
                pit_led_color[7] = muteColour;

                PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(250000U, PIT_SOURCE_CLOCK));
                PIT_StartTimer(PIT, kPIT_Chnl_0);

                prevState = uxAlarm;
            }

            if (uxReminder == uxState)
            {
                // loop 2 blue 2 cyan every 250ms
                PIT_StopTimer(PIT, kPIT_Chnl_0);
                pit_led_index      = 0;
                pit_led_count      = 8;
                toggleCountEnabled = 0;

                rgbLedColor_t muteColour = ((audio_processing_get_mic_mute()) ? LED_COLOR_RED : LED_COLOR_OFF);

                pit_led_color[0] = LED_COLOR_BLUE;
                pit_led_color[1] = muteColour;
                pit_led_color[2] = LED_COLOR_BLUE;
                pit_led_color[3] = muteColour;
                pit_led_color[4] = LED_COLOR_CYAN;
                pit_led_color[5] = muteColour;
                pit_led_color[6] = LED_COLOR_CYAN;
                pit_led_color[7] = muteColour;

                PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(250000U, PIT_SOURCE_CLOCK));
                PIT_StartTimer(PIT, kPIT_Chnl_0);

                prevState = uxReminder;
            }
        }

        if (uxTimerEnd == uxState)
        {
            toggleCountEnabled = 0;
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_OFF);

            // SLN_AMP_AbortWrite();

            prevState = uxIdle;
        }

        if (uxAlarmEnd == uxState)
        {
            toggleCountEnabled = 0;
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_OFF);

            prevState = uxIdle;
        }

        if (uxReminderEnd == uxState)
        {
            toggleCountEnabled = 0;
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_OFF);

            prevState = uxIdle;
        }

        if (uxNotificationIncoming == uxState)
        {
            // blink yellow led once with 250ms period
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCountEnabled = 0;
            pit_led_color[0]   = LED_COLOR_YELLOW;
            pit_led_color[1]   = LED_COLOR_OFF;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(500000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxNotificationIncoming;
        }

        if (uxNotificationQueued == uxState)
        {
            // loop yellow led on and off every 2 seconds
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 6;
            toggleCountEnabled = 0;

            rgbLedColor_t muteColour = ((audio_processing_get_mic_mute()) ? LED_COLOR_RED : LED_COLOR_YELLOW);

            pit_led_color[0] = LED_COLOR_YELLOW;
            pit_led_color[1] = LED_COLOR_OFF;
            pit_led_color[2] = LED_COLOR_OFF;
            pit_led_color[3] = muteColour;
            pit_led_color[4] = LED_COLOR_OFF;
            pit_led_color[5] = LED_COLOR_OFF;

            /* When we have a notification queued we no longer hit idle state so we have to mark the end of speaking
             * state*/
            speakingState = false;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(1000000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxNotificationQueued;
        }

        if (uxNotificationCleared == uxState)
        {
            // stop loop of yellow on and off
            toggleCountEnabled = 0;
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_OFF);

            prevState = uxIdle;
        }

        if (uxDoNotDisturb == uxState)
        {
            // Blink purple led
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_PURPLE);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCount        = 1;
            toggleCountEnabled = 1;
            resumePrevState    = true;
            pit_led_color[0]   = LED_COLOR_PURPLE;
            pit_led_color[1]   = LED_COLOR_OFF;

            /* When we are in do not disturb state we no longer hit idle state so we have to mark the end of speaking
             * state*/
            speakingState = false;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(500000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            AIS_APP_GetAppData()->state = AIS_STATE_IDLE;

            prevState = uxIdle;
        }

        if (uxReconnecting == uxState)
        {
            // loop green led on and off every 0.5 seconds
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCountEnabled = 0;
            pit_led_color[0]   = LED_COLOR_GREEN;
            pit_led_color[1]   = LED_COLOR_OFF;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(500000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxReconnecting;
        }

        if (uxConnected == uxState)
        {
            // loop green led on and off every 0.25 seconds
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCountEnabled = 0;
            pit_led_color[0]   = LED_COLOR_GREEN;
            pit_led_color[1]   = LED_COLOR_OFF;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(250000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxConnected;
        }

        if (uxBootUp == uxState)
        {
            // alternate blue and cyan every 1 second
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCountEnabled = 0;
            pit_led_color[0]   = LED_COLOR_BLUE;
            pit_led_color[1]   = LED_COLOR_CYAN;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(1000000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxBootUp;
        }

        if (uxWiFiSetup == uxState)
        {
            // loop yellow led on and off every 0.5 seconds
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCountEnabled = 0;
            pit_led_color[0]   = LED_COLOR_YELLOW;
            pit_led_color[1]   = LED_COLOR_OFF;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(500000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxWiFiSetup;
        }

        if (uxAccessPointFound == uxState)
        {
            // loop yellow led on and off every 0.25 seconds
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCountEnabled = 0;
            pit_led_color[0]   = LED_COLOR_YELLOW;
            pit_led_color[1]   = LED_COLOR_OFF;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(250000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxAccessPointFound;
        }

        if (uxNoAccessPoint == uxState)
        {
            // loop yellow led and red led every 0.5 seconds
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCountEnabled = 0;
            pit_led_color[0]   = LED_COLOR_RED;
            pit_led_color[1]   = LED_COLOR_YELLOW;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(500000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxNoAccessPoint;
        }

        if (uxInvalidWiFiCred == uxState)
        {
            // loop yellow led and red led every 0.25 seconds
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCountEnabled = 0;
            pit_led_color[0]   = LED_COLOR_RED;
            pit_led_color[1]   = LED_COLOR_YELLOW;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(250000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxInvalidWiFiCred;
        }

        if (uxDiscovery == uxState)
        {
            toggleCountEnabled = 0;
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_BLUE);

            prevState = uxDiscovery;
        }

        if (uxAuthentication == uxState)
        {
            toggleCountEnabled = 0;
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_YELLOW);

            prevState = uxAuthentication;
        }

        if (uxDeviceChange == uxState)
        {
            /*leave the white led on for 2 ticks a total of 1s*/
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCount        = 2;
            toggleCountEnabled = 1;
            resumePrevState    = true;
            pit_led_color[0]   = LED_COLOR_WHITE;
            pit_led_color[1]   = LED_COLOR_WHITE;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(500000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxDeviceChange;
        }

        if (uxDisconnected == uxState)
        {
            // blink red 3 times every 0.5s
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCount        = 3;
            toggleCountEnabled = 1;
            /* We are disconnected. We need to ensure that the device goes back into the previous offline state */
            resumePrevState  = true;
            pit_led_color[0] = LED_COLOR_RED;
            pit_led_color[1] = LED_COLOR_OFF;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(500000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxDisconnected;
        }

        if (uxError == uxState)
        {
            // toggle between red and white every 0.5 second for 6 times
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit_led_index      = 0;
            pit_led_count      = 2;
            toggleCount        = 6;
            toggleCountEnabled = 1;
            pit_led_color[0]   = LED_COLOR_RED;
            pit_led_color[1]   = LED_COLOR_WHITE;

            PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(500000U, PIT_SOURCE_CLOCK));
            PIT_StartTimer(PIT, kPIT_Chnl_0);

            prevState = uxError;
        }

        if (uxApMode == uxState)
        {
            /* set orange led */
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_ORANGE);
        }

        if (uxBleProvision == uxState)
        {
            /* set orange led */
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_ORANGE);
        }

        if (uxBleProvision == uxState)
        {
            /* set orange led */
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_ORANGE);
        }

        uxState = uxNull;
    }
}

void ux_attention_task_Init(TaskHandle_t *handle)
{
    /* Structure of initialize PIT */
    pit_config_t pitConfig;

    /* Enable clock gate for GPIO1 */
    CLOCK_EnableClock(kCLOCK_Gpio1);
    /* Set PERCLK_CLK source to OSC_CLK*/
    CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
    /* Set PERCLK_CLK divider to 1 */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);

    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(1000000U, PIT_SOURCE_CLOCK));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

    /* Enable at the NVIC */
    NVIC_SetPriority(PIT_IRQ_ID, PIT_IRQ_PRIORITY);
    NVIC_EnableIRQ(PIT_IRQ_ID);

    /* Create a timer for triggering short alert sequence every 10 seconds */
    s_speakingAlertTimerHandle = xTimerCreate("UX_ALERT_SPEAKING", UX_ALERT_SPEAKING_INTERVAL_MSEC, pdFALSE, (void *)0,
                                              ux_speaking_alert_timer_cb);

    if (NULL == s_speakingAlertTimerHandle)
    {
        return;
    }

    /* Create a queue capable of containing 5 unsigned long values. */
    g_uxQueue = xQueueCreate(5, sizeof(ux_attention_states_t));

    if (g_uxQueue == NULL)
    {
        configPRINTF(("[UX Queue] Queue Creation Failed\r\n"));
    }
    else
    {
        if (xTaskCreate(ux_attention_task, UX_ATT_TASK_NAME, UX_ATT_TASK_STACK, NULL, UX_ATT_TASK_PRIORITY,
                        (void *)handle) != pdPASS)
        {
            configPRINTF(("[UX TASK] Task Creation Failed\r\n"));
            while (1)
                ;
        }
    }
}

int32_t ux_attention_set_state(ux_attention_states_t uxState)
{
    if (g_uxQueue != 0)
    {
        /* Send an unsigned long.  Wait for 20 ticks for space to become
        available if necessary. */
        if (xQueueSend(g_uxQueue, (void *)&uxState, (TickType_t)20) != pdPASS)
        {
            /* Failed to post the message, even after 20 ticks. */
            configPRINTF(("[UX Queue] Failed to post message even after 20 ticks\r\n"));
            return 1;
        }

        return 0;
    }
    else
        return 1;
}

void ux_attention_resume_state(ais_state_t state)
{
    switch (state)
    {
        case AIS_STATE_IDLE:
            ux_attention_set_state(uxIdle);
            break;

        case AIS_STATE_THINKING:
            ux_attention_set_state(uxThinking);
            break;

        case AIS_STATE_SPEAKING:
            ux_attention_set_state(uxSpeaking);
            break;

        case AIS_STATE_ALERTING:
            ux_attention_set_state(AIS_Alerts_GetUxNotifyBit());
            break;

        case AIS_STATE_DO_NOT_DISTURB:
            ux_attention_set_state(uxDoNotDisturb);
            break;

        case AIS_STATE_NOTIFICATION:
            ux_attention_set_state(uxNotificationQueued);
            break;

        case AIS_STATE_INVALID:
            configPRINTF(("Error: Invalid attention state\r\n"));
            break;
    }
}

void ux_attention_sys_fault(void)
{
    // Stop the timer
    PIT_StopTimer(PIT, kPIT_Chnl_0);

    // Loop through a blinking state to indicate a fault has occurred
    volatile uint32_t delay = 10000000;
    for (uint32_t count = 0; count < 20; count++)
    {
        RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_OFF);
        RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_RED);

        while (delay--)
            ;
        delay = 10000000;

        RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_OFF);
        RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_WHITE);

        while (delay--)
            ;
        delay = 10000000;
    }

    RGB_LED_SetBrightnessColor(currentBrightness, LED_COLOR_OFF);
}

ux_attention_states_t ux_attention_get_state(void)
{
    return prevState;
}
