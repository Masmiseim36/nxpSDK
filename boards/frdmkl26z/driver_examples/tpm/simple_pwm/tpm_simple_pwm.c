/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
#include "board.h"
#include "fsl_tpm.h"

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_TPM_BASEADDR TPM0
#define BOARD_TPM_CHANNEL 4U

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl4InterruptEnable
#define TPM_CHANNEL_FLAG kTPM_Chnl4Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM0_IRQn
#define TPM_LED_HANDLER TPM0_IRQHandler

/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
void delay(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool brightnessUp = true; /* Indicate LED is brighter or dimmer */
volatile uint8_t updatedDutycycle = 10U;
volatile uint8_t getCharValue = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    tpm_config_t tpmInfo;
    tpm_chnl_pwm_signal_param_t tpmParam;
    tpm_pwm_level_select_t pwmLevel = kTPM_LowTrue;

    /* Configure tpm params with frequency 24kHZ */
    tpmParam.chnlNumber = (tpm_chnl_t)BOARD_TPM_CHANNEL;
    tpmParam.level = pwmLevel;
    tpmParam.dutyCyclePercent = updatedDutycycle;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Select the clock source for the TPM counter as kCLOCK_PllFllSelClk */
    CLOCK_SetTpmClock(1U);

    /* Print a note to terminal */
    PRINTF("\r\nTPM example to output center-aligned PWM signal\r\n");
    PRINTF("\r\nIf an LED is connected to the TPM pin, you will see a change in LED brightness if you enter different values");
    PRINTF("\r\nIf no LED is connected to the TPM pin, then probe the signal using an oscilloscope");

    TPM_GetDefaultConfig(&tpmInfo);
    /* Initialize TPM module */
    TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);

    TPM_SetupPwm(BOARD_TPM_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, 24000U, TPM_SOURCE_CLOCK);

    TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);

    while (1)
    {
        do
        {
            PRINTF("\r\nPlease enter a value to update the Duty cycle:\r\n");
            PRINTF("Note: The range of value is 0 to 9.\r\n");
            PRINTF("For example: If enter '5', the duty cycle will be set to 50 percent.\r\n");
            PRINTF("Value:");
            getCharValue = GETCHAR() - 0x30U;
            PRINTF("%d", getCharValue);
            PRINTF("\r\n");
        } while (getCharValue > 9U);

        updatedDutycycle = getCharValue * 10U;

        /* Disable channel output before updating the dutycycle */
        TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, 0U);

        /* Update PWM duty cycle */
        TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, kTPM_CenterAlignedPwm,
                               updatedDutycycle);

        /* Start channel output with updated dutycycle */
        TPM_UpdateChnlEdgeLevelSelect(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, pwmLevel);

        PRINTF("The duty cycle was successfully updated!\r\n");
    }
}
