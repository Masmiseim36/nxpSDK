/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
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

#include "fsl_elcdif.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_ELCDIF LCDIF

#define APP_IMG_HEIGHT 272
#define APP_IMG_WIDTH 480
#define APP_HSW 41
#define APP_HFP 4
#define APP_HBP 8
#define APP_VSW 10
#define APP_VFP 4
#define APP_VBP 2
#define APP_POL_FLAGS \
    (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)

/* Display. */
#define LCD_DISP_GPIO GPIO5
#define LCD_DISP_GPIO_PIN 9
/* Back light. */
#define LCD_BL_GPIO GPIO1
#define LCD_BL_GPIO_PIN 8

/* Frame buffer data alignment, for better performance, the LCDIF frame buffer should be 64B align. */
#define FRAME_BUFFER_ALIGN 64


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_psBuffer[APP_IMG_HEIGHT][APP_IMG_WIDTH], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_asBuffer[APP_IMG_HEIGHT][APP_IMG_WIDTH], FRAME_BUFFER_ALIGN);

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Enable interrupt. */
void BOARD_EnableLcdInterrupt(void)
{
    EnableIRQ(LCDIF_IRQn);
}

/* Initialize the LCD_DISP. */
void BOARD_InitLcd(void)
{
    volatile uint32_t i = 0x100U;

    gpio_pin_config_t config = {
        kGPIO_DigitalOutput, 0,
    };

    /* Reset the LCD. */
    GPIO_PinInit(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, &config);

    GPIO_WritePinOutput(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 0);

    while (i--)
    {
    }

    GPIO_WritePinOutput(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 1);

    /* Backlight. */
    config.outputLogic = 1;
    GPIO_PinInit(LCD_BL_GPIO, LCD_BL_GPIO_PIN, &config);
}

void BOARD_InitLcdifPixelClock(void)
{
    /*
     * The desired output frame rate is 60Hz. So the pixel clock frequency is:
     * (480 + 41 + 4 + 18) * (272 + 10 + 4 + 2) * 60 = 9.2M.
     * Here set the LCDIF pixel clock to 9.3M.
     */

    /*
     * Initialize the Video PLL.
     * Video PLL output clock is OSC24M * (loopDivider + (denominator / numerator)) / postDivider = 93MHz.
     */
    clock_video_pll_config_t config = {
        .loopDivider = 31, .postDivider = 8, .numerator = 0, .denominator = 0,
    };

    CLOCK_InitVideoPll(&config);

    /*
     * 000 derive clock from PLL2
     * 001 derive clock from PLL3 PFD3
     * 010 derive clock from PLL5
     * 011 derive clock from PLL2 PFD0
     * 100 derive clock from PLL2 PFD1
     * 101 derive clock from PLL3 PFD1
     */
    CLOCK_SetMux(kCLOCK_Lcdif1PreMux, 2);

    CLOCK_SetDiv(kCLOCK_Lcdif1PreDiv, 4);

    CLOCK_SetDiv(kCLOCK_Lcdif1Div, 1);

    /*
     * 000 derive clock from divided pre-muxed lcdif1 clock
     * 001 derive clock from ipp_di0_clk
     * 010 derive clock from ipp_di1_clk
     * 011 derive clock from ldb_di0_clk
     * 100 derive clock from ldb_di1_clk
     */
    CLOCK_SetMux(kCLOCK_Lcdif1Mux, 0);
}



void APP_FillPsBuffer(uint32_t buffer[][APP_IMG_WIDTH])
{
    /* PS buffer is a blue rectangle in the center. */

    uint32_t i, j;

    /* Background color. */
    memset(buffer, 0, sizeof(uint32_t) * APP_IMG_WIDTH * APP_IMG_HEIGHT);

    /* Foreground color. */
    for (i = APP_IMG_HEIGHT / 4; i < APP_IMG_HEIGHT * 3 / 4; i++)
    {
        for (j = APP_IMG_WIDTH / 4; j < APP_IMG_WIDTH * 3 / 4; j++)
        {
            buffer[i][j] = 0x000000FFU;
        }
    }
}

void APP_ELCDIF_Init(void)
{
    const elcdif_rgb_mode_config_t config = {
        .panelWidth = APP_IMG_WIDTH,
        .panelHeight = APP_IMG_HEIGHT,
        .hsw = APP_HSW,
        .hfp = APP_HFP,
        .hbp = APP_HBP,
        .vsw = APP_VSW,
        .vfp = APP_VFP,
        .vbp = APP_VBP,
        .polarityFlags = APP_POL_FLAGS,
        .bufferAddr = (uint32_t)s_psBuffer,
        .pixelFormat = kELCDIF_PixelFormatRGB888,
        .dataBus = kELCDIF_DataBus24Bit,
    };

    ELCDIF_RgbModeInit(APP_ELCDIF, &config);
}

void APP_FillAsBuffer(uint32_t buffer[][APP_IMG_WIDTH])
{
    /* AS buffer is full screen red. */

    uint32_t i, j;

    /* Foreground color. */
    for (i = 0; i < APP_IMG_HEIGHT; i++)
    {
        for (j = 0; j < APP_IMG_WIDTH; j++)
        {
            buffer[i][j] = 0x00FF0000U;
        }
    }
}

void APP_ELCDIF_AlphaRun(void)
{
    uint32_t alpha = 0U;
    int32_t alphaInc = 1U;

    const elcdif_as_buffer_config_t asBufferConfig = {
        .bufferAddr = (uint32_t)s_asBuffer, .pixelFormat = kELCDIF_AsPixelFormatRGB888,
    };

    elcdif_as_blend_config_t asBlendConfig = {
        .alpha = alpha,
        .invertAlpha = false,
        .alphaMode = kELCDIF_AlphaOverride,
        .ropMode = kELCDIF_RopMaskAs, /* Don't care. */
    };

    ELCDIF_SetAlphaSurfaceBufferConfig(APP_ELCDIF, &asBufferConfig);
    ELCDIF_SetAlphaSurfaceBlendConfig(APP_ELCDIF, &asBlendConfig);
    ELCDIF_EnableAlphaSurface(APP_ELCDIF, true);

    ELCDIF_EnableInterrupts(APP_ELCDIF, kELCDIF_CurFrameDoneInterruptEnable);
    ELCDIF_RgbModeStart(APP_ELCDIF);

    while (1)
    {
        alpha += alphaInc;

        if (0xFF == alpha)
        {
            alphaInc = -1;
        }
        else if (0 == alpha)
        {
            alphaInc = 1;
        }

        asBlendConfig.alpha = alpha;

        /* Wait for previous frame complete. */
        while (!(kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(APP_ELCDIF)))
        {
        }
        ELCDIF_ClearInterruptStatus(APP_ELCDIF, kELCDIF_CurFrameDone);

        ELCDIF_SetAlphaSurfaceBlendConfig(APP_ELCDIF, &asBlendConfig);
    }
}

void setUp(void)
{
}

void tearDown(void)
{
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitPins();
    BOARD_LCDIF_ConfigurePins();
    BOARD_BootClockRUN();
    BOARD_InitLcdifPixelClock();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    BOARD_InitLcd();

    PRINTF("LCDIF alpha example started ...\r\n");

    APP_FillPsBuffer(s_psBuffer);

    APP_FillAsBuffer(s_asBuffer);

    APP_ELCDIF_Init();

    APP_ELCDIF_AlphaRun();

    while(1)
    {
    }
}
