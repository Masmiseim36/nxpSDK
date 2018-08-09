/*
 * The Clear BSD License
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#include "fsl_common.h"
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
#define APP_DATA_BUS 16

/* Display. */
#define LCD_DISP_GPIO GPIO1
#define LCD_DISP_GPIO_PIN 2
/* Back light. */
#define LCD_BL_GPIO GPIO2
#define LCD_BL_GPIO_PIN 31

/* Frame buffer data alignment, for better performance, the LCDIF frame buffer should be 64B align. */
#define FRAME_BUFFER_ALIGN 64


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_EnableLcdInterrupt(void);
/* How many frames to show before swithing to the other picture. */
#define APP_FRAME_PER_PIC 60

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile uint32_t s_curFrame = 0; /* Frame counting. */

AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_frameBuffer[APP_IMG_HEIGHT][APP_IMG_WIDTH], FRAME_BUFFER_ALIGN);

uint32_t lutData[2][ELCDIF_LUT_ENTRY_NUM];

/*******************************************************************************
 * Code
 ******************************************************************************/
extern void APP_LCDIF_IRQHandler(void);

void LCDIF_IRQHandler(void)
{
    APP_LCDIF_IRQHandler();
}

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

    GPIO_PinWrite(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 0);

    while (i--)
    {
    }

    GPIO_PinWrite(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 1);

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
    CLOCK_SetMux(kCLOCK_LcdifPreMux, 2);

    CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 4);

    CLOCK_SetDiv(kCLOCK_LcdifDiv, 1);
}


void APP_LCDIF_IRQHandler(void)
{
    uint32_t intStatus;

    intStatus = ELCDIF_GetInterruptStatus(APP_ELCDIF);

    ELCDIF_ClearInterruptStatus(APP_ELCDIF, intStatus);

    if (intStatus & kELCDIF_CurFrameDone)
    {
        s_curFrame++;
    }
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
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
        .bufferAddr = (uint32_t)s_frameBuffer,
        .pixelFormat = kELCDIF_PixelFormatRAW8,
        .dataBus = kELCDIF_DataBus8Bit,
    };

    ELCDIF_RgbModeInit(APP_ELCDIF, &config);

    /* Load the LUT data. */
    ELCDIF_UpdateLut(APP_ELCDIF, kELCDIF_Lut0, 0, lutData[0], ELCDIF_LUT_ENTRY_NUM);
    ELCDIF_UpdateLut(APP_ELCDIF, kELCDIF_Lut1, 0, lutData[1], ELCDIF_LUT_ENTRY_NUM);

    ELCDIF_EnableLut(APP_ELCDIF, true);
}

uint32_t APP_MakeLutData(uint8_t r, uint8_t g, uint8_t b)
{
#if (APP_DATA_BUS == 16)
    /* 16 bit data bus. */
    return (((r >> 3)) << 11) | (((g >> 2)) << 5) | (((b >> 3)) << 0);
#else
    /* 24-bit data bus. */
    return (r << 16) | (g << 8) | (b << 0);
#endif
}

/*
 * Fill the LUT data.
 *
 * The first LUT memory is red, the second LUT memory is blue.
 */
void APP_FillLutData(void)
{
    uint32_t i;

    for (i = 0; i < ELCDIF_LUT_ENTRY_NUM; i++)
    {
        lutData[0][i] = APP_MakeLutData(i, 0, 0);
    }

    for (i = 0; i < ELCDIF_LUT_ENTRY_NUM; i++)
    {
        lutData[1][i] = APP_MakeLutData(0, 0, i);
    }
}

/*
 * Fill the frame buffer.
 *
 * The color degree changes in horizontal direction.
 */
void APP_FillFrameBuffer(void)
{
    uint32_t i, j;

    for (i = 0; i < APP_IMG_HEIGHT; i++)
    {
        for (j = 0; j < APP_IMG_WIDTH; j++)
        {
            s_frameBuffer[i][j] = j;
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t lutIndex = 0;
    uint32_t frameToWait = 0;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_InitSemcPins();
    BOARD_BootClockRUN();
    BOARD_InitLcdifPixelClock();
    BOARD_InitDebugConsole();
    BOARD_InitLcd();

    PRINTF("LCDIF LUT example start...\r\n");

    /* Clear the frame buffer. */
    memset(s_frameBuffer, 0, sizeof(s_frameBuffer));

    APP_FillLutData();
    APP_FillFrameBuffer();

    APP_ELCDIF_Init();

    BOARD_EnableLcdInterrupt();

    ELCDIF_EnableInterrupts(APP_ELCDIF, kELCDIF_CurFrameDoneInterruptEnable);
    ELCDIF_RgbModeStart(APP_ELCDIF);

    while (1)
    {
        frameToWait += APP_FRAME_PER_PIC;

        while (frameToWait != s_curFrame)
        {
        }

        lutIndex ^= 1U;

        /* The LSB of frame buffer determines which LUT memory to use. */
        ELCDIF_SetNextBufferAddr(APP_ELCDIF, (uint32_t)s_frameBuffer | lutIndex);
    }
}
