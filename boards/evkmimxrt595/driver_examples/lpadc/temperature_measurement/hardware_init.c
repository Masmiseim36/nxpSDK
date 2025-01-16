/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_lpadc.h"
#include "app.h"
/*${header:end}*/

extern const uint32_t g_LpadcFullRange;

/*
 * Porting Guild.
 *
 * To port LPADC Temperature measurement driver example to other platforms, please recode
 * DEMO_MeasureTemperature function based on the actual approximate transfer function of
 * the temperature sensor.
 *
 * If the actual calculation equation is similar to the following equation, please take
 * the DEMO_MeasureTemperature function in this file as a reference.
 *          Temp = 25 - [(Vtemp - Vtemp25) / m]
 * In the above equation:
 *          Vtemp      -- The voltage of the temperature sensor channel at the ambient temperature.
 *          Vtemp25    -- The voltage of the temperature sensor channel at 25 Celsius.
 *          m          -- Temperature sensor slope.
 */
/*${function:start}*/
/*!
 * brief Measure Temperature sensor.
 */
float DEMO_MeasureTemperature(ADC_Type *base, uint32_t commandId, uint32_t index)
{
    lpadc_conv_result_t convResultStruct;
    float Vtemp              = 0.0f;
    uint32_t convResultShift = 0U;
    float parameterSlope     = DEMO_LPADC_TEMPERATURE_SLOPE;
    float parameterIntercept = DEMO_LPADC_TEMPERATURE_INTERCEPT;
    float temperature        = -273.15f; /* Absolute zero degree as the incorrect return value. */

    if (true == LPADC_GetConvResult(base, &convResultStruct, (uint8_t)index))
    {
        Vtemp       = (((convResultStruct.convValue >> convResultShift) *
                  ((DEMO_LPADC_VREEFP - DEMO_LPADC_VREFN) * 1000.0f / g_LpadcFullRange)));
        temperature = (Vtemp - parameterIntercept) / parameterSlope;
    }

    return temperature;
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    SYSCTL0->PDRUNCFG0_CLR = SYSCTL0_PDRUNCFG0_ADC_PD_MASK;
    SYSCTL0->PDRUNCFG0_CLR = SYSCTL0_PDRUNCFG0_ADC_LP_MASK;
    /* Power up the ADC temperature sensor via SYSCTL. */
    SYSCTL0->PDRUNCFG0_CLR = SYSCTL0_PDRUNCFG0_ADC_TEMPSNS_PD_MASK;

    RESET_PeripheralReset(kADC0_RST_SHIFT_RSTn);
    CLOCK_AttachClk(kFRO_DIV4_to_ADC_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivAdcClk, 1);
}
/*${function:end}*/
