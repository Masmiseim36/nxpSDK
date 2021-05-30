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

#include <stdio.h>
#include <string.h>
#include "slcd_engine.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_slcd.h"
#include "fsl_rnga.h"

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


#define offLow32Pin(n) ((uint32_t)1 << (n))       /* Pin offset for the low 32 pins. */
#define offHigh32Pin(n) ((uint32_t)1 << (n - 32)) /* Pin offset for the high 32 pins. */
#define SLCD_INVALIDINPUT (-1)                    /* Invalid input. */
#define SLCD_OK (0)                               /* Execute success. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief SLCD time delay.
 * @param count The timedelay counter number.
 */
static void SLCD_TimeDelay(uint32_t count);

/*!
 * @brief SLCD set lcd pins.
 *
 * @param type lcd setting type @ref "lcd_set_type_t".
 * @param lcd_pin lcd pin.
 * @param pin_val pin setting value.
 * @param on The display on/off flag.
 */
static void SLCD_SetLCDPin(lcd_set_type_t type, uint32_t lcd_pin, uint8_t pin_val, int32_t on);

/*!
 * @brief SLCD Application Initialization.
 */
static void SLCD_APP_Init(void);

/*!
 * @brief SLCD Clear Screen.
 */
static void SLCD_Clear_Screen(void);

/*!
 * @brief SLCD basic test.
 * @param slcd_engine The SLCD engine structure pointer.
 */
static void SLCD_Basic_Test(tSLCD_Engine *slcd_engine);

/*!
 * @brief SLCD show victory.
 * @param slcd_engine The SLCD engine structure pointer.
 */
static void SLCD_Show_Victory(tSLCD_Engine *slcd_engine);

/*!
 * @brief SLCD show game start.
 * @param slcd_engine The SLCD engine structure pointer.
 */
static void SLCD_Show_Game_Start(tSLCD_Engine *slcd_engine);

/*!
 * @brief SLCD show number.
 * @param slcd_engine The SLCD engine structure pointer.
 * @param number The number to be shown.
 * @return The execution result.
 */
static int32_t SLCD_Show_Number(tSLCD_Engine *slcd_engine, int32_t number);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* LCD segment pin and slcd gpio pin number map. */
static uint8_t slcd_lcd_gpio_seg_pin[SLCD_PIN_NUM] = {38, 36, 34, 32, 31, 29, 25, 23, 43, 37,
                                                      35, 33, 50, 30, 45, 24, 26, 28, 44, 59};

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

static void SLCD_SetLCDPin(lcd_set_type_t type, uint32_t lcd_pin, uint8_t pin_val, int32_t on)
{
    assert(lcd_pin > 0);

    uint8_t gpio_pin = 0;
    uint8_t bit_val = 0;
    uint8_t i = 0;

    /* lcd _pin starts from 1. */
    gpio_pin = slcd_lcd_gpio_seg_pin[lcd_pin - 1];

    if (type == SLCD_Set_Num)
    {
        SLCD_SetFrontPlaneSegments(LCD, gpio_pin, (on ? pin_val : 0));
    }
    else
    {
        for (i = 0; i < 8; ++i)
        {
            bit_val = (uint8_t)(pin_val >> i) & 0x1U;
            if (bit_val)
            {
                SLCD_SetFrontPlaneOnePhase(LCD, gpio_pin, (slcd_phase_index_t)i, on);
            }
        }
    }
}

static void SLCD_APP_Init(void)
{
    slcd_config_t config;
    slcd_clock_config_t clkConfig =
    {
#if defined FRDM_KL46Z || defined FRDM_KL43Z
        kSLCD_AlternateClk1,
#else
        kSLCD_DefaultClk,
#endif
#if defined FRDM_KL43Z
        kSLCD_AltClkDivFactor256,
#else
        kSLCD_AltClkDivFactor1,
#endif
        kSLCD_ClkPrescaler01,
#if FSL_FEATURE_SLCD_HAS_FAST_FRAME_RATE
        ,
        false
#endif
    };

    /* Get Default configuration. */
    /*
     * config.displayMode = kSLCD_NormalMode;
     * config.powerSupply = kSLCD_InternalVll3UseChargePump;
     * config.voltageTrim = kSLCD_RegulatedVolatgeTrim00;
     * config.lowPowerBehavior = kSLCD_EnabledInWaitStop;
     * config.frameFreqIntEnable = false;
     * config.faultConfig = NULL;
     */
    SLCD_GetDefaultConfig(&config);

    /* Verify and Complete the configuration structure. */
    config.clkConfig = &clkConfig;
    config.loadAdjust = kSLCD_HighLoadOrSlowestClkSrc;
#if defined FRDM_KL46Z
    config.dutyCycle = kSLCD_1Div4DutyCycle;
    config.slcdLowPinEnabled = 0x000e0d80U;  /* LCD_P19/P18/P17/P11/P10/P8/P7. */
    config.slcdHighPinEnabled = 0x00300160U; /* LCD_P53/P52/P40/P38/P37. */
    config.backPlaneLowPin = 0x000c0000U;    /* LCD_P19/P18 --> b19/b18. */
    config.backPlaneHighPin = 0x00100100U;   /* LCD_P52/P40 --> b20/b8. */
#elif defined TWR_KM34Z75M
    config.dutyCycle = kSLCD_1Div8DutyCycle;
    /* LCD_P31/P30/P29/P28/P26/P25/P24/P23/P22/P20/P19/P14/P13. */
    config.slcdLowPinEnabled =
        (offLow32Pin(14) | offLow32Pin(20) | offLow32Pin(22) | offLow32Pin(13) | offLow32Pin(19) | offLow32Pin(28) |
         offLow32Pin(26) | offLow32Pin(24) | offLow32Pin(31) | offLow32Pin(29) | offLow32Pin(25) | offLow32Pin(23) |
         offLow32Pin(30));
    /* LCD_P59/P58/P57/P56/P50/P45/P44/P43/P38/P37/P36/P35/P34/P33/P32. */
    config.slcdHighPinEnabled =
        (offHigh32Pin(56) | offHigh32Pin(58) | offHigh32Pin(57) | offHigh32Pin(59) | offHigh32Pin(44) |
         offHigh32Pin(45) | offHigh32Pin(38) | offHigh32Pin(36) | offHigh32Pin(34) | offHigh32Pin(32) |
         offHigh32Pin(43) | offHigh32Pin(37) | offHigh32Pin(35) | offHigh32Pin(33) | offHigh32Pin(50));
    /* LCD_P22/20/19/14/13 --> b22/b20/b19/b14/b13 = 1. */
    config.backPlaneLowPin = (offLow32Pin(14) | offLow32Pin(20) | offLow32Pin(22) | offLow32Pin(13) | offLow32Pin(19));
    /* LCD_P58/57/56 --> b26/b25/b24 = 1. */
    config.backPlaneHighPin = (offHigh32Pin(56) | offHigh32Pin(58) | offHigh32Pin(57));
#elif defined FRDM_KL43Z
    config.dutyCycle = kSLCD_1Div4DutyCycle;
    config.slcdLowPinEnabled = 0x0d10c000U;  /* LCD_P27/26/24/20 -> b27/26/24/20 = 1. */
    config.slcdHighPinEnabled = 0x18001d00U; /* LCD_P44/43/42/40 -> b12/11/10/8 = 1. */
    config.backPlaneLowPin = 0x0000c000U;    /* LCD_P15/P14 -> b15/b14 = 1. */
    config.backPlaneHighPin = 0x18000000U;   /* LCD_P60/P59 -> b28/27 = 1. */
#endif
    SLCD_Init(LCD, &config);
}

static void SLCD_Clear_Screen(void)
{
    /* Disables all front plane pins on all eight phases Phase A ~ Phase H.
    P59/P50/P45/P44/P43/P38/P37/P36/P35/P34/P33/P32/P31/P30/P29/P28/P26/P25/P24/P23 */
    SLCD_SetFrontPlaneSegments(LCD, 23, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 24, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 25, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 26, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 28, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 29, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 30, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 31, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 32, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 33, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 34, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 35, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 36, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 37, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 38, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 43, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 44, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 45, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 50, kSLCD_NoPhaseActivate);
    SLCD_SetFrontPlaneSegments(LCD, 59, kSLCD_NoPhaseActivate);
}

static void SLCD_Basic_Test(tSLCD_Engine *slcd_engine)
{
    uint32_t pinNum = 0;
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t allPhaseOn = kSLCD_PhaseAActivate | kSLCD_PhaseBActivate | kSLCD_PhaseCActivate | kSLCD_PhaseDActivate |
                          kSLCD_PhaseEActivate | kSLCD_PhaseFActivate | kSLCD_PhaseGActivate | kSLCD_PhaseHActivate;

    for (pinNum = 0; pinNum < FSL_FEATURE_SLCD_HAS_PIN_NUM; pinNum++)
    {
        SLCD_SetFrontPlaneSegments(LCD, pinNum, allPhaseOn);
    }

    SLCD_SetBackPlanePhase(LCD, 14, kSLCD_PhaseHActivate);
    SLCD_SetBackPlanePhase(LCD, 20, kSLCD_PhaseGActivate);
    SLCD_SetBackPlanePhase(LCD, 22, kSLCD_PhaseFActivate);
    SLCD_SetBackPlanePhase(LCD, 56, kSLCD_PhaseEActivate);
    SLCD_SetBackPlanePhase(LCD, 58, kSLCD_PhaseDActivate);
    SLCD_SetBackPlanePhase(LCD, 13, kSLCD_PhaseCActivate);
    SLCD_SetBackPlanePhase(LCD, 19, kSLCD_PhaseBActivate);
    SLCD_SetBackPlanePhase(LCD, 57, kSLCD_PhaseAActivate);

    PRINTF("---------- Start basic SLCD test -------------\r\n");

    SLCD_StartDisplay(LCD);

    SLCD_StartBlinkMode(LCD, kSLCD_AltDisplayBlink, kSLCD_BlinkRate03);

    SLCD_TimeDelay(0x7FFFFFU);

    SLCD_StopBlinkMode(LCD);

    SLCD_Clear_Screen();

    for (i = 0; i < NUM_POSEND; ++i)
    {
        for (j = 0; j < 10; ++j)
        {
            SLCD_Engine_Show_Num(slcd_engine, j, i, 1);
            SLCD_TimeDelay(0x1FFFFFU);
            SLCD_Engine_Show_Num(slcd_engine, j, i, 0);
        }
    }

    for (i = 0; i < ICON_END; ++i)
    {
        SLCD_Engine_Show_Icon(slcd_engine, i, 1);
        SLCD_TimeDelay(0x1FFFFFU);
        SLCD_Engine_Show_Icon(slcd_engine, i, 0);
    }

    SLCD_TimeDelay(0xFFFFFU);
}

static void SLCD_Show_Victory(tSLCD_Engine *slcd_engine)
{
    SLCD_Engine_Show_Icon(slcd_engine, ICON_S3, 1);
    SLCD_Engine_Show_Icon(slcd_engine, ICON_S6, 1);
    SLCD_Engine_Show_Icon(slcd_engine, ICON_S7, 1);
    SLCD_Engine_Show_Icon(slcd_engine, ICON_S9, 1);
    SLCD_Engine_Show_Icon(slcd_engine, ICON_S11, 1);
    SLCD_Engine_Show_Icon(slcd_engine, ICON_S12, 1);
    SLCD_Engine_Show_Icon(slcd_engine, ICON_S13, 1);
}

static void SLCD_Show_Game_Start(tSLCD_Engine *slcd_engine)
{
    SLCD_Engine_Show_Num(slcd_engine, 1, NUM_POS6, 0);
    SLCD_Engine_Show_Num(slcd_engine, 1, NUM_POS6, 1);
    SLCD_TimeDelay(0x3FFFFFU);
    SLCD_Engine_Show_Num(slcd_engine, 2, NUM_POS6, 0);
    SLCD_Engine_Show_Num(slcd_engine, 2, NUM_POS6, 1);
    SLCD_TimeDelay(0x3FFFFFU);
    SLCD_Engine_Show_Num(slcd_engine, 3, NUM_POS6, 0);
    SLCD_Engine_Show_Num(slcd_engine, 3, NUM_POS6, 1);
    SLCD_TimeDelay(0x3FFFFFU);
    SLCD_Engine_Show_Num(slcd_engine, 3, NUM_POS6, 0);
    /* Show Let */
    /* Clear number */
    /* Show G at num pos 8 */
    SLCD_Engine_Show_Num(slcd_engine, 0, NUM_POS5, 0);
    SLCD_Engine_Show_Icon(slcd_engine, ICON_5A, 1);
    SLCD_Engine_Show_Icon(slcd_engine, ICON_5C, 1);
    SLCD_Engine_Show_Icon(slcd_engine, ICON_5D, 1);
    SLCD_Engine_Show_Icon(slcd_engine, ICON_5E, 1);
    SLCD_Engine_Show_Icon(slcd_engine, ICON_5F, 1);
    /* Show O at num pos 9. */
    SLCD_Engine_Show_Num(slcd_engine, 0, NUM_POS6, 0);
    SLCD_Engine_Show_Num(slcd_engine, 0, NUM_POS6, 1);
    SLCD_TimeDelay(0xFFFFFU);
}

static int32_t SLCD_Show_Number(tSLCD_Engine *slcd_engine, int32_t number)
{
    if ((number < 0) || (number > 9999))
    {
        return SLCD_INVALIDINPUT;
    }

    uint32_t thousand_val = 0;
    uint32_t hundred_val = 0;
    uint32_t decimal_val = 0;
    uint32_t single_val = 0;

    SLCD_Engine_Show_Num(slcd_engine, 8, NUM_POS3, 0);
    SLCD_Engine_Show_Num(slcd_engine, 8, NUM_POS4, 0);
    SLCD_Engine_Show_Num(slcd_engine, 8, NUM_POS5, 0);
    SLCD_Engine_Show_Num(slcd_engine, 8, NUM_POS6, 0);

    thousand_val = number / 1000;
    hundred_val = (number % 1000) / 100;
    decimal_val = (number % 100) / 10;
    single_val = (number % 10);

    if (number >= 1000)
    {
        SLCD_Engine_Show_Num(slcd_engine, thousand_val, NUM_POS3, 0);
        SLCD_Engine_Show_Num(slcd_engine, thousand_val, NUM_POS3, 1);
    }

    if (number >= 100)
    {
        SLCD_Engine_Show_Num(slcd_engine, hundred_val, NUM_POS4, 0);
        SLCD_Engine_Show_Num(slcd_engine, hundred_val, NUM_POS4, 1);
    }

    if (number >= 10)
    {
        SLCD_Engine_Show_Num(slcd_engine, decimal_val, NUM_POS5, 0);
        SLCD_Engine_Show_Num(slcd_engine, decimal_val, NUM_POS5, 1);
    }

    SLCD_Engine_Show_Num(slcd_engine, single_val, NUM_POS6, 0);
    SLCD_Engine_Show_Num(slcd_engine, single_val, NUM_POS6, 1);

    return SLCD_OK;
}

static void SLCD_Guess_Num(tSLCD_Engine *slcd_engine)
{
    int32_t input_num = 0;
    int32_t target_num = 0;

    while (1)
    {
        int32_t play_again = 0;
        uint32_t randout = 0;

        PRINTF("-------------- SLCD Guess Num Game --------------\r\n");
        PRINTF("The number input and final number will be shown on SLCD.\r\n");
        PRINTF("Please check SLCD for these numbers.\r\n");
        PRINTF("Let's play:\r\n");

        RNGA_GetRandomData(RNG, &randout, sizeof(uint32_t));

        target_num = randout % 10000;

        SLCD_Clear_Screen();

        SLCD_Show_Game_Start(slcd_engine);

        while (1)
        {
            uint8_t input_num_size = 4;
            char ch_in = 0;
            PRINTF("Please guess the number I want(0 - 9999), Press 'enter' to end: ");
            input_num = 0;
            /* Check for enter key */
            while (1)
            {
                if (('\r' == ch_in) && (input_num_size < 4))
                {
                    break;
                }

                ch_in = GETCHAR();
                if ((ch_in >= '0') && (ch_in <= '9') && (input_num_size > 0))
                {
                    PUTCHAR(ch_in);
                    input_num = (input_num * 10) + (ch_in - '0');
                    input_num_size--;
                }
            }
            PRINTF("\r\n");

            if (SLCD_INVALIDINPUT == SLCD_Show_Number(slcd_engine, input_num))
            {
                PRINTF("Input number out of range! Should be in the range of 0-9999.\r\n");
            }
            else
            {
                if (input_num == target_num)
                {
                    ch_in = 0;
                    bool loop = true;

                    SLCD_Show_Victory(slcd_engine);
                    PRINTF("Great, %d, you have GOT it!\r\n", input_num);
                    while (loop)
                    {
                        PRINTF("Play again? Input 'Y' or 'N':\r\n");
                        ch_in = GETCHAR();
                        PUTCHAR(ch_in);
                        PRINTF("\r\n");

                        if ((ch_in == 'Y') || (ch_in == 'y'))
                        {
                            play_again = 1;
                            loop = false;
                        }
                        else if ((ch_in == 'N') || (ch_in == 'n'))
                        {
                            play_again = 0;
                            loop = false;
                        }
                        else
                        {
                            PRINTF("Wrong input!Please input again.\r\n");
                            continue;
                        }
                    }
                    break;
                }
                else
                {
                    PRINTF("The input number %d is %s than what I want. Guess again!\r\n", input_num,
                           (input_num > target_num) ? "bigger" : "smaller");
                }
            }
        }

        if (play_again == 0)
        {
            SLCD_Clear_Screen();
            PRINTF("Bye bye!\r\n");
            break;
        }
    }
}

/*!
 * @brief main function
 */
int main(void)
{
    tSLCD_Engine slcd_engine;

    /* Init hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* RNGA clock bug fix. */
    CLOCK_EnableClock(kCLOCK_Rnga0);
    CLOCK_DisableClock(kCLOCK_Rnga0);
    CLOCK_EnableClock(kCLOCK_Rnga0);

    /* RNGA Initialization. */
    RNGA_Init(RNG);

    /* SLCD Initialization. */
    SLCD_APP_Init();

    memset(&slcd_engine, 0, sizeof(tSLCD_Engine));
    SLCD_Engine_Init(&slcd_engine, SLCD_SetLCDPin);

    SLCD_Basic_Test(&slcd_engine);

    SLCD_Guess_Num(&slcd_engine);

    while (1)
    {
    }
}
