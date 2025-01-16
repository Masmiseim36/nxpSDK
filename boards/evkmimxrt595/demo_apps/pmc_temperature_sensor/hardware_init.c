/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_iap.h"
#include "fsl_lpadc.h"
#include "app.h"
/*${header:end}*/

extern volatile bool g_LpadcConversionCompletedFlag;
extern volatile uint16_t g_LpadcConvValue;

/*${variable:start}*/
uint32_t tsensorCal = 0;
/*${variable:end}*/

/*${function:start}*/
status_t DEMO_TempsenorInit(void)
{
    status_t status = kStatus_Success;
    /* Config to use PMC temperature sensor. */
    SYSCTL0->TEMPSENSORCTL |= SYSCTL0_TEMPSENSORCTL_TSSRC_MASK;

    POWER_DisablePD(kPDRUNCFG_PD_PMC_TEMPSNS);
    POWER_ApplyPD();

    /* Another 40us for the temperature sensor to sattle. */
    SDK_DelayAtLeastUs(40U, SystemCoreClock);

    status = IAP_OtpInit(SystemCoreClock);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Read TSENS_CAL calibration constant value from OTP Fuse. */
    status = IAP_OtpFuseRead(TSENS_CAL_OTP_FUSE_INDEX, &tsensorCal);
    return status;
}

float DEMO_GetTempsenorValue(void)
{
    /* TSENSOR[TSENSM] value set. */
    uint8_t tsensmSel[15]     = {0U, 1U, 3U, 2U, 6U, 7U, 5U, 4U, 5U, 7U, 6U, 2U, 3U, 1U, 0U};
    uint16_t tsensorValue[15] = {0U};
    float t, cm_vref, cm_ctat, cm_temp;
    int8_t calibration = 0;
    uint8_t i;

    for (i = 0; i < 15U; i++)
    {
        PMC->TSENSOR = PMC_TSENSOR_TSENSM(tsensmSel[i]);

        /* Do ADC convert */
        LPADC_DoSoftwareTrigger(DEMO_LPADC_BASE, 1U); /* 1U is trigger0 mask. */
        while (!g_LpadcConversionCompletedFlag)
        {
        }
        tsensorValue[i]                = g_LpadcConvValue;
        g_LpadcConversionCompletedFlag = false;
    }

    /* Calculate temperature. */
    /* CM_CTAT = (2*C1_001 - C1_011 + 2*C2_001 - C2_011 + 2*C1_101 - C1_111 + 2*C2_101 - C2_111)/4 */
    cm_ctat = (float)(2 * tsensorValue[1] - tsensorValue[2] + 2 * tsensorValue[13] - tsensorValue[12] +
                      2 * tsensorValue[6] - tsensorValue[5] + 2 * tsensorValue[8] - tsensorValue[9]) /
              4.0F;
    /* CM_TEMP = (2*C1_000 - C1_010 + 2*C2_000 - C2_010 + 4*CM_100 - C1_110 - C2_110)/4 */
    cm_temp = (float)(2 * tsensorValue[0] - tsensorValue[3] + 2 * tsensorValue[14] - tsensorValue[11] +
                      4 * tsensorValue[7] - tsensorValue[4] - tsensorValue[10]) /
              4.0F;

    calibration = (int8_t)(tsensorCal & 0xFF);
    cm_vref     = cm_ctat + (953.36F + calibration) * cm_temp / 2048;

    t = 370.98F * (cm_temp / cm_vref) - 273.15F;
    return t;
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    POWER_DisablePD(kPDRUNCFG_PD_ADC);
    POWER_DisablePD(kPDRUNCFG_LP_ADC);

    RESET_PeripheralReset(kADC0_RST_SHIFT_RSTn);
    CLOCK_AttachClk(kFRO_DIV4_to_ADC_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivAdcClk, 1);
}
/*${function:end}*/
