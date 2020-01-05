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
/* The Flextimer instance/channel used for board */
#define BOARD_TPM_BASEADDR TPM0
#define BOARD_FIRST_TPM_CHANNEL 2U
#define BOARD_SECOND_TPM_CHANNEL 4U

/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint8_t getCharValue = 0U;
volatile uint8_t updatedDutycycle = 10U;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    tpm_config_t tpmInfo;
    tpm_chnl_pwm_signal_param_t tpmParam[2];

    /* Configure tpm params with frequency 24kHZ */
    tpmParam[0].chnlNumber = (tpm_chnl_t)BOARD_FIRST_TPM_CHANNEL;
    tpmParam[0].level = kTPM_LowTrue;
    tpmParam[0].dutyCyclePercent = updatedDutycycle;

    tpmParam[1].chnlNumber = (tpm_chnl_t)BOARD_SECOND_TPM_CHANNEL;
    tpmParam[1].level = kTPM_LowTrue;
    tpmParam[1].dutyCyclePercent = updatedDutycycle;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Select the clock source for the TPM counter as kCLOCK_PllFllSelClk */
    CLOCK_SetTpmClock(1U);

    /* Print a note to terminal */
    PRINTF("\r\nTPM example to output PWM on 2 channels\r\n");
    PRINTF("\r\nIf an LED is connected to the TPM pin, you will see a change in LED brightness if you enter different values");
    PRINTF("\r\nIf no LED is connected to the TPM pin, then probe the signal using an oscilloscope");

    /*
     * tpmInfo.prescale = kTPM_Prescale_Divide_1;
     * tpmInfo.useGlobalTimeBase = false;
     * tpmInfo.enableDoze = false;
     * tpmInfo.enableDebugMode = false;
     * tpmInfo.enableReloadOnTrigger = false;
     * tpmInfo.enableStopOnOverflow = false;
     * tpmInfo.enableStartOnTrigger = false;
     * tpmInfo.enablePauseOnTrigger = false;
     * tpmInfo.triggerSelect = kTPM_Trigger_Select_0;
     * tpmInfo.triggerSource = kTPM_TriggerSource_External;
     */
    TPM_GetDefaultConfig(&tpmInfo);
    /* Initialize TPM module */
    TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);

    TPM_SetupPwm(BOARD_TPM_BASEADDR, tpmParam, 2U, kTPM_EdgeAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
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

        /* Start PWM mode with updated duty cycle */
        TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_FIRST_TPM_CHANNEL, kTPM_EdgeAlignedPwm,
                               updatedDutycycle);
        TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_SECOND_TPM_CHANNEL, kTPM_EdgeAlignedPwm,
                               updatedDutycycle);

        PRINTF("The duty cycle was successfully updated!\r\n");
    }
}
