/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_flexio.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_TIME_DELAY_FOR_DUTY_CYCLE_UPDATE 100000U
#define DEMO_FLEXIO_BASEADDR FLEXIO0
#define DEMO_FLEXIO_OUTPUTPIN 2U /* Select FXIO_D2 as PWM output */
#define DEMO_FLEXIO_TIMER_CH 0U  /* Flexio timer0 used */

#define DEMO_FLEXIO_CLOCK_FREQUENCY CLOCK_GetOsc0ErClkFreq()
#define DEMO_FLEXIO_FREQUENCY 48000U
#define FLEXIO_MAX_FREQUENCY (DEMO_FLEXIO_CLOCK_FREQUENCY / 2U)
#define FLEXIO_MIN_FREQUENCY (DEMO_FLEXIO_CLOCK_FREQUENCY / 256U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Configures the timer as a 8-bits PWM mode to generate the PWM waveform
 *
 * @param freq_Hz PWM frequency in hertz, range is [FLEXIO_MIN_FREQUENCY, FLEXIO_MAX_FREQUENCY]
 * @param duty Specified duty in unit of %, with a range of [1, 99]
 */
static void flexio_pwm_init(uint32_t freq_Hz, uint32_t duty);

/*!
 * @brief Enables the timer by setting TIMOD to 8-bits PWM and start generating the PWM
 */
static void flexio_pwm_start(void);

/*******************************************************************************
* Variables
*******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static void flexio_pwm_init(uint32_t freq_Hz, uint32_t duty)
{
    assert((freq_Hz < FLEXIO_MAX_FREQUENCY) && (freq_Hz > FLEXIO_MIN_FREQUENCY));

    uint32_t lowerValue = 0; /* Number of clock cycles in high logic state in one period */
    uint32_t upperValue = 0; /* Number of clock cycles in low logic state in one period */
    uint32_t sum = 0;        /* Number of clock cycles in one period */
    flexio_timer_config_t fxioTimerConfig;

    /* Check parameter */
    if ((duty > 99) || (duty == 0))
    {
        duty = 50;
    }

    /* Configure the timer DEMO_FLEXIO_TIMER_CH for generating PWM */
    fxioTimerConfig.triggerSelect = FLEXIO_TIMER_TRIGGER_SEL_SHIFTnSTAT(0U);
    fxioTimerConfig.triggerSource = kFLEXIO_TimerTriggerSourceInternal;
    fxioTimerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
    fxioTimerConfig.pinConfig = kFLEXIO_PinConfigOutput;
    fxioTimerConfig.pinPolarity = kFLEXIO_PinActiveHigh;
    fxioTimerConfig.pinSelect = DEMO_FLEXIO_OUTPUTPIN; /* Set pwm output */
    fxioTimerConfig.timerMode = kFLEXIO_TimerModeDisabled;
    fxioTimerConfig.timerOutput = kFLEXIO_TimerOutputOneNotAffectedByReset;
    fxioTimerConfig.timerDecrement = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    fxioTimerConfig.timerDisable = kFLEXIO_TimerDisableNever;
    fxioTimerConfig.timerEnable = kFLEXIO_TimerEnabledAlways;
    fxioTimerConfig.timerReset = kFLEXIO_TimerResetNever;
    fxioTimerConfig.timerStart = kFLEXIO_TimerStartBitDisabled;
    fxioTimerConfig.timerStop = kFLEXIO_TimerStopBitDisabled;

    /* Calculate timer lower and upper values of TIMCMP */
    /* Calculate the nearest integer value for sum, using formula round(x) = (2 * floor(x) + 1) / 2 */
    /* sum = DEMO_FLEXIO_CLOCK_FREQUENCY / freq_H */
    sum = (DEMO_FLEXIO_CLOCK_FREQUENCY * 2 / freq_Hz + 1) / 2;
    /* Calculate the nearest integer value for lowerValue, the high period of the pwm output */
    /* lowerValue = sum * duty / 100 */
    lowerValue = (sum * duty / 50 + 1) / 2;
    /* Calculate upper value, the low period of the pwm output */
    upperValue = sum - lowerValue;
    fxioTimerConfig.timerCompare = ((upperValue - 1) << 8U) | (lowerValue - 1);

    FLEXIO_SetTimerConfig(DEMO_FLEXIO_BASEADDR, DEMO_FLEXIO_TIMER_CH, &fxioTimerConfig);
}

static void flexio_pwm_start(void)
{
    /* Set Timer mode to kFLEXIO_TimerModeDual8BitPWM to start timer */
    DEMO_FLEXIO_BASEADDR->TIMCTL[DEMO_FLEXIO_TIMER_CH] |= FLEXIO_TIMCTL_TIMOD(kFLEXIO_TimerModeDual8BitPWM);
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i;
    uint32_t duty = 100;
    flexio_config_t fxioUserConfig;

    /* Init board hardware */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetFlexio0Clock(2U);

    /* Init flexio, use default configure
     * Disable doze and fast access mode
     * Enable in debug mode
     */
    FLEXIO_GetDefaultConfig(&fxioUserConfig);
    FLEXIO_Init(DEMO_FLEXIO_BASEADDR, &fxioUserConfig);

    PRINTF("\r\nFLEXIO_PWM demo start.\r\n");

    while (1)
    {
        flexio_pwm_init(DEMO_FLEXIO_FREQUENCY, --duty);
        flexio_pwm_start();

        for (i = 0; i < DEMO_TIME_DELAY_FOR_DUTY_CYCLE_UPDATE; i++)
        {
            __NOP();
        }

        if (duty == 0)
        {
            duty = 100;
        }
    }
}
