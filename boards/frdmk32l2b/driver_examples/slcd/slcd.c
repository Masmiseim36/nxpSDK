/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "fsl_common.h"
#include "fsl_slcd.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief SLCD time delay.
 * @param count The timedelay counter number.
 */
static void SLCD_TimeDelay(uint32_t count);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void SLCD_TimeDelay(uint32_t count)
{
    while (count--)
    {
        __NOP();
    }
}

int main(void)
{
    uint8_t waveForm = 0;
    slcd_config_t config;
    slcd_clock_config_t clkConfig = {
#if defined FRDM_KL46Z || defined FRDM_KL43Z || defined FRDM_K32L2B
        kSLCD_AlternateClk1,
#else
        kSLCD_DefaultClk,
#endif
#if defined FRDM_KL43Z || defined FRDM_K32L2B
        kSLCD_AltClkDivFactor256,
#else
        kSLCD_AltClkDivFactor1,
#endif
        kSLCD_ClkPrescaler01
#if FSL_FEATURE_SLCD_HAS_FAST_FRAME_RATE
        ,
        false
#endif
    };

    /* Hardware initialize. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Enable the MCGIRCLK */
    MCG->C1 |= MCG_C1_IRCLKEN_MASK;

    PRINTF("\r\nSLCD Example Starts.\r\n");

    /* SLCD get default configure. */
    /*
     * config.displayMode = kSLCD_NormalMode;
     * config.powerSupply = kSLCD_InternalVll3UseChargePump;
     * config.voltageTrim = kSLCD_RegulatedVolatgeTrim08;
     * config.lowPowerBehavior = kSLCD_EnabledInWaitStop;
     * config.frameFreqIntEnable = false;
     * config.faultConfig = NULL;
     */
    SLCD_GetDefaultConfig(&config);

    /* Verify and Complete the configuration structure. */
    config.clkConfig  = &clkConfig;
    config.loadAdjust = kSLCD_HighLoadOrSlowestClkSrc;
#if defined FRDM_KL46Z
    config.dutyCycle          = kSLCD_1Div4DutyCycle;
    config.slcdLowPinEnabled  = 0x000e0d80U; /* LCD_P19/P18/P17/P11/P10/P8/P7. */
    config.slcdHighPinEnabled = 0x00300160U; /* LCD_P53/P52/P40/P38/P37. */
    config.backPlaneLowPin    = 0x000c0000U; /* LCD_P19/P18 --> b19/b18. */
    config.backPlaneHighPin   = 0x00100100U; /* LCD_P52/P40 --> b20/b8. */
#elif defined TWR_KM34Z75M
    config.dutyCycle = kSLCD_1Div8DutyCycle;
    config.slcdLowPinEnabled = 0xF7D86000U;  /* LCD_P31/P30/P29/P28/P26/P25/P24/P23/P22/P20/P19/P14/P13. */
    config.slcdHighPinEnabled = 0x0F04387FU; /* LCD_P59/P58/P57/P56/P50/P45/P44/P43/P38/P37/P36/P35/P34/P33/P32. */
    config.backPlaneLowPin = 0x00586000U;    /* LCD_P22/20/19/14/13 --> b22/b20/b19/b14/b13 = 1. */
    config.backPlaneHighPin = 0x07000000U;   /* LCD_P58/57/56 --> b26/b25/b24 = 1. */
#elif defined FRDM_KL43Z || defined FRDM_K32L2B
    config.dutyCycle          = kSLCD_1Div4DutyCycle;
    config.slcdLowPinEnabled  = 0x0d10c000U; /* LCD_P27/26/24/20 -> b27/26/24/20 = 1. */
    config.slcdHighPinEnabled = 0x18001d00U; /* LCD_P44/43/42/40 -> b12/11/10/8 = 1. */
    config.backPlaneLowPin    = 0x0000c000U; /* LCD_P15/P14 -> b15/b14 = 1. */
    config.backPlaneHighPin   = 0x18000000U; /* LCD_P60/P59 -> b28/27 = 1. */
#endif

    config.faultConfig = NULL;
    /* SLCD Initialize. */
    SLCD_Init(LCD, &config);

/* Set SLCD front plane phase to show: all segments on . */
#if defined FRDM_KL46Z
    waveForm = (kSLCD_PhaseAActivate | kSLCD_PhaseBActivate | kSLCD_PhaseCActivate | kSLCD_PhaseDActivate);
    /* Set SLCD back plane phase. */
    SLCD_SetBackPlanePhase(LCD, 40, kSLCD_PhaseAActivate); /* SLCD COM1 --- LCD_P40. */
    SLCD_SetBackPlanePhase(LCD, 52, kSLCD_PhaseBActivate); /* SLCD COM2 --- LCD_P52. */
    SLCD_SetBackPlanePhase(LCD, 19, kSLCD_PhaseCActivate); /* SLCD COM3 --- LCD_P19. */
    SLCD_SetBackPlanePhase(LCD, 18, kSLCD_PhaseDActivate); /* SLCD COM4 --- LCD_P18. */
    /* Set SLCD front plane phase to show. */
    SLCD_SetFrontPlaneSegments(LCD, 37, waveForm); /* SLCD P05 --- LCD_P37. */
    SLCD_SetFrontPlaneSegments(LCD, 17, waveForm); /* SLCD P06 --- LCD_P17. */
    SLCD_SetFrontPlaneSegments(LCD, 7, waveForm);  /* SLCD P07 --- LCD_P7. */
    SLCD_SetFrontPlaneSegments(LCD, 8, waveForm);  /* SLCD P08 --- LCD_P8. */
    SLCD_SetFrontPlaneSegments(LCD, 53, waveForm); /* SLCD P09 --- LCD_P53. */
    SLCD_SetFrontPlaneSegments(LCD, 38, waveForm); /* SLCD P10 --- LCD_P38. */
    SLCD_SetFrontPlaneSegments(LCD, 10, waveForm); /* SLCD P11 --- LCD_P10. */
    SLCD_SetFrontPlaneSegments(LCD, 11, waveForm); /* SLCD P12 --- LCD_P11. */
#elif defined FRDM_KL43Z || defined FRDM_K32L2B
    waveForm = (kSLCD_PhaseAActivate | kSLCD_PhaseBActivate | kSLCD_PhaseCActivate | kSLCD_PhaseDActivate);
    /* Set SLCD back plane phase. */
    SLCD_SetBackPlanePhase(LCD, 59, kSLCD_PhaseAActivate); /* SLCD COM1 --- LCD_P59. */
    SLCD_SetBackPlanePhase(LCD, 60, kSLCD_PhaseBActivate); /* SLCD COM2 --- LCD_P60. */
    SLCD_SetBackPlanePhase(LCD, 14, kSLCD_PhaseCActivate); /* SLCD COM3 --- LCD_P14. */
    SLCD_SetBackPlanePhase(LCD, 15, kSLCD_PhaseDActivate); /* SLCD COM4 --- LCD_P15. */
    /* Set SLCD front plane phase to show. */
    SLCD_SetFrontPlaneSegments(LCD, 20, waveForm); /* SLCD P05 --- LCD_P20. */
    SLCD_SetFrontPlaneSegments(LCD, 24, waveForm); /* SLCD P06 --- LCD_P24. */
    SLCD_SetFrontPlaneSegments(LCD, 26, waveForm); /* SLCD P07 --- LCD_P26. */
    SLCD_SetFrontPlaneSegments(LCD, 27, waveForm); /* SLCD P08 --- LCD_P27. */
    SLCD_SetFrontPlaneSegments(LCD, 40, waveForm); /* SLCD P09 --- LCD_P40. */
    SLCD_SetFrontPlaneSegments(LCD, 42, waveForm); /* SLCD P10 --- LCD_P42. */
    SLCD_SetFrontPlaneSegments(LCD, 43, waveForm); /* SLCD P11 --- LCD_P43. */
    SLCD_SetFrontPlaneSegments(LCD, 44, waveForm); /* SLCD P12 --- LCD_P44. */
#elif defined TWR_KM34Z75M
    waveForm = (kSLCD_PhaseAActivate | kSLCD_PhaseBActivate | kSLCD_PhaseCActivate | kSLCD_PhaseDActivate |
                kSLCD_PhaseEActivate | kSLCD_PhaseFActivate | kSLCD_PhaseGActivate | kSLCD_PhaseHActivate);
    /* Set SLCD back plane phase. */
    SLCD_SetBackPlanePhase(LCD, 57, kSLCD_PhaseAActivate); /* SLCD_COM1 --- LCD_P57. */
    SLCD_SetBackPlanePhase(LCD, 19, kSLCD_PhaseBActivate); /* SLCD_COM2 --- LCD_P19. */
    SLCD_SetBackPlanePhase(LCD, 13, kSLCD_PhaseCActivate); /* SLCD_COM3 --- LCD_P13. */
    SLCD_SetBackPlanePhase(LCD, 58, kSLCD_PhaseDActivate); /* SLCD_COM4 --- LCD_P58. */
    SLCD_SetBackPlanePhase(LCD, 56, kSLCD_PhaseEActivate); /* SLCD_COM5 --- LCD_P56. */
    SLCD_SetBackPlanePhase(LCD, 22, kSLCD_PhaseFActivate); /* SLCD_COM6 --- LCD_P22. */
    SLCD_SetBackPlanePhase(LCD, 20, kSLCD_PhaseGActivate); /* SLCD_COM7 --- LCD_P20. */
    SLCD_SetBackPlanePhase(LCD, 14, kSLCD_PhaseHActivate); /* SLCD_COM8 --- LCD_P14. */
    /* Set SLCD front plane phase to show. */
    SLCD_SetFrontPlaneSegments(LCD, 38, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 36, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 34, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 32, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 31, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 29, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 25, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 23, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 43, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 37, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 35, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 33, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 50, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 30, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 45, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 24, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 26, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 28, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 44, waveForm);
    SLCD_SetFrontPlaneSegments(LCD, 59, waveForm);
#endif
    /* Starts SLCD display. */
    SLCD_StartDisplay(LCD);
    PRINTF("\r\nSLCD Displays All Segments.\r\n");
    SLCD_TimeDelay(0xFFFFFFU);

    PRINTF("\r\nSLCD Starts Blink Mode.\r\n");

    /* Blink mode Display. */
    SLCD_StartBlinkMode(LCD, kSLCD_BlankDisplayBlink, kSLCD_BlinkRate01);
    SLCD_TimeDelay(0xFFFFFFU);

    PRINTF("\r\nSLCD Stops Blink Mode.\r\n");
    /* Stops SLCD blink display mode. */
    SLCD_StopBlinkMode(LCD);
    SLCD_TimeDelay(0xFFFFFFU);

    PRINTF("\r\nSLCD Stops Display.\r\n");
    /* Stops SLCD display. */
    SLCD_StopDisplay(LCD);

    PRINTF("\r\nSLCD Example Ends.\r\n");

    while (1)
    {
    }
}
