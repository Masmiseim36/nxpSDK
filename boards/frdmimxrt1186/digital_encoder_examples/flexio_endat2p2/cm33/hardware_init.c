/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
/*${header:end}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_BootClockRUN();
    BOARD_InitBootPins();
    BOARD_InitPins();

    clock_root_config_t rootCfg = {0};
    rootCfg.mux = BOARD_FLEXIO_CLOCK_ROOT_MUX;
    rootCfg.div = 3;     /* Set Flexio clk to 96Mhz */
    CLOCK_SetRootClock(BOARD_FLEXIO_CLOCK_ROOT, &rootCfg);
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
