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

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_pxp.h"
#include "fsl_elcdif.h"

#include "pin_mux.h"
#include "fsl_gpio.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_ELCDIF LCDIF
#define APP_PXP PXP

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

/*
 * Frame buffer data alignment.
 * The PXP input buffer, output buffer, and LCDIF frame buffer address 64B align.
 */
#define FRAME_BUFFER_ALIGN 64

/* PS input buffer is square. */
#define APP_PS_WIDTH (APP_IMG_WIDTH / 2U)
#define APP_PS_HEIGHT (APP_IMG_HEIGHT / 2U)
#define APP_AS_WIDTH (APP_IMG_WIDTH / 2U)
#define APP_AS_HEIGHT (APP_IMG_HEIGHT / 2U)

#define APP_PS_ULC_X ((APP_IMG_WIDTH / 2) - (APP_PS_SIZE / 2))
#define APP_PS_ULC_Y ((APP_IMG_HEIGHT / 2) - (APP_PS_SIZE / 2))
#define APP_PS_LRC_X ((APP_IMG_WIDTH / 2) + (APP_PS_SIZE / 2) - 1U)
#define APP_PS_LRC_Y ((APP_IMG_HEIGHT / 2) + (APP_PS_SIZE / 2) - 1U)

#define APP_BPP 4U /* Use 24-bit RGB888 format. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_InitInputBuffer(void);
static void APP_InitLcdif(void);
static void APP_InitPxp(void);
static void APP_Blend(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_BufferLcd[2][APP_IMG_HEIGHT][APP_IMG_WIDTH], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_psBufferPxp[APP_PS_HEIGHT][APP_PS_WIDTH], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_asBufferPxp[APP_AS_HEIGHT][APP_AS_WIDTH], FRAME_BUFFER_ALIGN);

/* PXP Output buffer config. */
static pxp_output_buffer_config_t outputBufferConfig;

/*******************************************************************************
 * Code
 ******************************************************************************/

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


int main(void)
{
    BOARD_InitPins();
    BOARD_LCDIF_ConfigurePins();
    BOARD_BootClockRUN();
    BOARD_InitLcdifPixelClock();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    BOARD_InitLcd();

    PRINTF("\r\nPXP Blend example start...\r\n");

    APP_InitInputBuffer();
    APP_InitPxp();
    APP_InitLcdif();
    APP_Blend();

    while (1)
    {
    }
}

static void APP_InitLcdif(void)
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
        .bufferAddr = (uint32_t)s_BufferLcd[0],
        .pixelFormat = kELCDIF_PixelFormatRGB888,
        .dataBus = kELCDIF_DataBus24Bit,
    };

    ELCDIF_RgbModeInit(APP_ELCDIF, &config);

    ELCDIF_RgbModeStart(APP_ELCDIF);
}

static void APP_InitPxp(void)
{
    PXP_Init(APP_PXP);

    /* PS configure. */
    const pxp_ps_buffer_config_t psBufferConfig = {
        .pixelFormat = kPXP_PsPixelFormatRGB888,
        .swapByte = false,
        .bufferAddr = (uint32_t)s_psBufferPxp,
        .bufferAddrU = 0U,
        .bufferAddrV = 0U,
        .pitchBytes = APP_PS_WIDTH * APP_BPP,
    };

    PXP_SetProcessSurfaceBackGroundColor(APP_PXP, 0U);

    PXP_SetProcessSurfaceBufferConfig(APP_PXP, &psBufferConfig);

    /* AS config. */
    const pxp_as_buffer_config_t asBufferConfig = {
        .pixelFormat = kPXP_AsPixelFormatRGB888,
        .bufferAddr = (uint32_t)s_asBufferPxp,
        .pitchBytes = APP_AS_WIDTH * APP_BPP,
    };

    const pxp_as_blend_config_t asBlendConfig = {.alpha = 0U,          /* Don't care. */
                                                 .invertAlpha = false, /* Don't care. */
                                                 .alphaMode = kPXP_AlphaRop,
                                                 .ropMode = kPXP_RopMergeAs};

    PXP_SetAlphaSurfaceBufferConfig(APP_PXP, &asBufferConfig);
    PXP_SetAlphaSurfaceBlendConfig(APP_PXP, &asBlendConfig);

    /* Output config. */
    outputBufferConfig.pixelFormat = kPXP_OutputPixelFormatRGB888;
    outputBufferConfig.interlacedMode = kPXP_OutputProgressive;
    outputBufferConfig.buffer0Addr = (uint32_t)s_BufferLcd[0];
    outputBufferConfig.buffer1Addr = 0U;
    outputBufferConfig.pitchBytes = APP_IMG_WIDTH * APP_BPP;
    outputBufferConfig.width = APP_IMG_WIDTH;
    outputBufferConfig.height = APP_IMG_HEIGHT;

    PXP_SetOutputBufferConfig(APP_PXP, &outputBufferConfig);

    /* Disable CSC1, it is enabled by default. */
    PXP_EnableCsc1(APP_PXP, false);
}

static void APP_Blend(void)
{
    uint8_t curLcdBufferIdx = 1U;
    int8_t psIncX = 1;
    int8_t psIncY = 1;
    int8_t asIncX = -1;
    int8_t asIncY = -1;
    uint16_t psUlcX = 0U;
    uint16_t psUlcY = 0U;
    uint16_t asUlcX = APP_IMG_WIDTH - APP_AS_WIDTH;
    uint16_t asUlcY = APP_IMG_HEIGHT - APP_AS_HEIGHT;
    uint16_t psLrcX, psLrcY, asLrcX, asLrcY;

    psLrcX = psUlcX + APP_PS_WIDTH - 1U;
    psLrcY = psUlcY + APP_PS_HEIGHT - 1U;
    asLrcX = asUlcX + APP_AS_WIDTH - 1U;
    asLrcY = asUlcY + APP_AS_HEIGHT - 1U;

    for (;;)
    {
        /* Prepare next buffer for LCD. */
        PXP_SetProcessSurfacePosition(APP_PXP, psUlcX, psUlcY, psLrcX, psLrcY);
        PXP_SetAlphaSurfacePosition(APP_PXP, asUlcX, asUlcY, asLrcX, asLrcY);

        outputBufferConfig.buffer0Addr = (uint32_t)s_BufferLcd[curLcdBufferIdx];
        PXP_SetOutputBufferConfig(APP_PXP, &outputBufferConfig);

        /* Start PXP. */
        PXP_Start(APP_PXP);

        /* Wait for process complete. */
        while (!(kPXP_CompleteFlag & PXP_GetStatusFlags(APP_PXP)))
        {
        }

        PXP_ClearStatusFlags(APP_PXP, kPXP_CompleteFlag);

        /* Now new frame is ready, pass it to LCDIF. */
        ELCDIF_SetNextBufferAddr(APP_ELCDIF, (uint32_t)s_BufferLcd[curLcdBufferIdx]);

        /*
         * The new frame is not loaded untill current frame display finished. So
         * wait until current frame finished.
         */
        ELCDIF_ClearInterruptStatus(APP_ELCDIF, kELCDIF_CurFrameDone);
        while (!(kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(APP_ELCDIF)))
        {
        }

        /* Switch to the other LCD buffer. */
        curLcdBufferIdx ^= 1U;

        psLrcX += psIncX;
        psLrcY += psIncY;
        asLrcX += asIncX;
        asLrcY += asIncY;
        psUlcX += psIncX;
        psUlcY += psIncY;
        asUlcX += asIncX;
        asUlcY += asIncY;

        if (0 == asUlcX)
        {
            asIncX = 1;
        }
        else if (APP_IMG_WIDTH - 1 == asLrcX)
        {
            asIncX = -1;
        }

        if (0 == asUlcY)
        {
            asIncY = 1;
        }
        else if (APP_IMG_HEIGHT - 1 == asLrcY)
        {
            asIncY = -1;
        }

        if (0 == psUlcX)
        {
            psIncX = 1;
        }
        else if (APP_IMG_WIDTH - 1 == psLrcX)
        {
            psIncX = -1;
        }

        if (0 == psUlcY)
        {
            psIncY = 1;
        }
        else if (APP_IMG_HEIGHT - 1 == psLrcY)
        {
            psIncY = -1;
        }
    }
}

static void APP_InitInputBuffer(void)
{
    uint32_t i, j;

    /* The PS buffer is BLUE rectangle, the AS buffer is RED rectangle. */

    for (i = 0; i < APP_PS_HEIGHT; i++)
    {
        for (j = 0; j < APP_PS_WIDTH; j++)
        {
            s_psBufferPxp[i][j] = 0xFFU;
        }
    }

    for (i = 0; i < APP_PS_HEIGHT; i++)
    {
        for (j = 0; j < APP_PS_WIDTH; j++)
        {
            s_asBufferPxp[i][j] = 0xFF0000U;
        }
    }

    memset(s_BufferLcd, 0x0U, sizeof(s_BufferLcd));
}
