/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    clock_root_config_t rootCfg = {0};

    /* Board pin, clock, debug console init */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitFLEXIO_TFORMATPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    rootCfg.mux = kCLOCK_FLEXIO1_ClockRoot_MuxOscRc400M;
    rootCfg.div = 5;
    CLOCK_SetRootClock(DEMO_FLEXIO_CLOCK, &rootCfg);
}

void PWM_Trigger_Init(PWM_Type *PWMBase)
{
    uint32_t pwmSourceClockInHz = PWM_SRC_CLK_FREQ / (1 << DEMO_PWM_CLOCK_DEVIDER);
    uint32_t temp = pwmSourceClockInHz / DEMO_PWM_FREQUENCE;
    unsigned short int ui16M1PwmModulo = temp & 0xFFFF;
    unsigned short int ui16EnociderTransactionTime = TRANSACTION_TIME_US * pwmSourceClockInHz / 1000000U;
    /* Full cycle reload */
    PWMBase->SM[0].CTRL |= PWM_CTRL_FULL_MASK;

    PWMBase->SM[0].CTRL |= PWM_CTRL_PRSC(DEMO_PWM_CLOCK_DEVIDER);

    /* Value register initial values, duty cycle 50% */
    PWMBase->SM[0].INIT = (uint16_t)(-(ui16M1PwmModulo / 2));
    PWMBase->SM[0].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));

    PWMBase->SM[0].VAL1 = ((ui16M1PwmModulo / 2) - 1);

    /* Trigger for Encoder synchronization */
    PWMBase->SM[0].VAL5 = -(ui16M1PwmModulo / 2) + 10;

    /* Trigger for interrupt synchronization */
    PWMBase->SM[0].VAL4 = ((ui16M1PwmModulo / 2 - 1) - ui16EnociderTransactionTime );

    /* PWM0 ~ PWM3 module 0 trigger on VAL4 enabled for ADC synchronization */
    PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 4);
    PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 5);

    /* Master reload is generated every one opportunity */
    PWMBase->SM[0].CTRL = (PWMBase->SM[0].CTRL & ~PWM_CTRL_LDFQ_MASK) | PWM_CTRL_LDFQ(ENCODER_ACCESS_FREQ_VS_PWM_FRE0 - 1);

    /* Start PWM trigger*/
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0x1);
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0x1);
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_RUN_MASK) | PWM_MCTRL_RUN(0x1);

    XBAR_Init(kXBAR_DSC1);
    XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Pwm0OutTrig0, kXBAR1_OutputFlexio1TrigIn0);
}
/*${function:end}*/
