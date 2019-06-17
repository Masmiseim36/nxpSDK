/*
 * Copyright  2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
#define LCD_DISP_GPIO GPIO1
#define LCD_DISP_GPIO_PIN 2
/* Back light. */
#define LCD_BL_GPIO GPIO2
#define LCD_BL_GPIO_PIN 31

#define APP_LCDIF_DATA_BUS kELCDIF_DataBus16Bit

/*
 * Frame buffer data alignment.
 * The PXP input buffer, output buffer, and LCDIF frame buffer address 64B align.
 */
#define FRAME_BUFFER_ALIGN 64

/* PS input buffer is square. */
#if APP_IMG_WIDTH > APP_IMG_HEIGHT
#define APP_PS_SIZE APP_IMG_HEIGHT
#else
#define APP_PS_SIZE APP_IMG_WIDTH
#endif

#define APP_PS_ULC_X ((APP_IMG_WIDTH / 2) - (APP_PS_SIZE / 2))
#define APP_PS_ULC_Y ((APP_IMG_HEIGHT / 2) - (APP_PS_SIZE / 2))
#define APP_PS_LRC_X ((APP_IMG_WIDTH / 2) + (APP_PS_SIZE / 2) - 1U)
#define APP_PS_LRC_Y ((APP_IMG_HEIGHT / 2) + (APP_PS_SIZE / 2) - 1U)

#define APP_BPP 4U           /* Use 24-bit RGB888 format. */
#define APP_FRAME_PER_PIC 60 /* Keep every picture 80 frames. */

#ifndef APP_LCDIF_DATA_BUS
#define APP_LCDIF_DATA_BUS kELCDIF_DataBus24Bit
#endif

typedef struct _pxp_command
{
    uint32_t CTRL;
    uint32_t STAT;
    uint32_t OUT_CTRL;
    uint32_t OUT_BUF;
    uint32_t OUT_BUF2;
    uint32_t OUT_PITCH;
    uint32_t OUT_LRC;
    uint32_t OUT_PS_ULC;
    uint32_t OUT_PS_LRC;
    uint32_t OUT_AS_ULC;
    uint32_t OUT_AS_LRC;
    uint32_t PS_CTRL;
    uint32_t PS_BUF;
    uint32_t PS_UBUF;
    uint32_t PS_VBUF;
    uint32_t PS_PITCH;
    uint32_t PS_BACHGROUND;
    uint32_t PS_SCALE;
    uint32_t PS_OFFSET;
    uint32_t PS_CLRKEYLOW;
    uint32_t PS_CLRKEYHIGH;
    uint32_t AS_CTRL;
    uint32_t AS_BUF;
    uint32_t AS_PITCH;
    uint32_t AS_CLRKEYLOW;
    uint32_t AS_CLRKEYHIGH;
    uint32_t CSC1_COEF0;
    uint32_t CSC1_COEF1;
    uint32_t CSC1_COEF2;
#if !(defined(FSL_FEATURE_PXP_HAS_NO_CSC2) && FSL_FEATURE_PXP_HAS_NO_CSC2)
    uint32_t CSC2_CTRL;
    uint32_t CSC2_COEF0;
    uint32_t CSC2_COEF1;
    uint32_t CSC2_COEF2;
    uint32_t CSC2_COEF3;
    uint32_t CSC2_COEF4;
    uint32_t CSC2_COEF5;
#else
    uint32_t CSC2_RESERVE[7];
#endif
#if !(defined(FSL_FEATURE_PXP_HAS_NO_LUT) && FSL_FEATURE_PXP_HAS_NO_LUT)
    uint32_t LUT_CTRL;
    uint32_t LUT_ADDR;
    uint32_t LUT_DATA;
    uint32_t LUT_EXTMEM;
    uint32_t CFA;
#else
    uint32_t LUT_RESERVE[5];
#endif
    uint32_t HIST_CTRL;
    uint32_t HIST2_PARAM;
    uint32_t HIST4_PARAM;
    uint32_t HIST8_PARAM0;
    uint32_t HIST8_PARAM1;
    uint32_t HIST16_PARAM0;
    uint32_t HIST16_PARAM1;
    uint32_t HIST16_PARAM2;
    uint32_t HIST16_PARAM3;
    uint32_t POWER;
    uint32_t NEXT;
} pxp_command_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_InitInputBuffer(void);
static void APP_InitLcdif(void);
static void APP_InitPxp(void);
static void APP_InitPxpCommand(void);
static void APP_RotateCmdQueue(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_psBufferLcd[2][APP_IMG_HEIGHT][APP_IMG_WIDTH], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_psBufferPxp[APP_PS_SIZE][APP_PS_SIZE], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static pxp_command_t pxpCommand, 4); /* 32-bit align. */

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialize the LCD_DISP. */
void BOARD_InitLcd(void)
{
    gpio_pin_config_t config = {
        kGPIO_DigitalOutput,
        0,
    };

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
        .loopDivider = 31,
        .postDivider = 8,
        .numerator   = 0,
        .denominator = 0,
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


int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitLcdifPixelClock();
    BOARD_InitDebugConsole();
    BOARD_InitLcd();

    PRINTF("\r\nPXP command queue example start...\r\n");

    APP_InitInputBuffer();
    APP_InitPxpCommand();
    APP_InitPxp();
    APP_InitLcdif();

    APP_RotateCmdQueue();

    while (1)
    {
    }
}

static void APP_InitLcdif(void)
{
    const elcdif_rgb_mode_config_t config = {
        .panelWidth    = APP_IMG_WIDTH,
        .panelHeight   = APP_IMG_HEIGHT,
        .hsw           = APP_HSW,
        .hfp           = APP_HFP,
        .hbp           = APP_HBP,
        .vsw           = APP_VSW,
        .vfp           = APP_VFP,
        .vbp           = APP_VBP,
        .polarityFlags = APP_POL_FLAGS,
        .bufferAddr    = (uint32_t)s_psBufferLcd[0],
        .pixelFormat   = kELCDIF_PixelFormatXRGB8888,
        .dataBus       = APP_LCDIF_DATA_BUS,
    };

    ELCDIF_RgbModeInit(APP_ELCDIF, &config);

    ELCDIF_RgbModeStart(APP_ELCDIF);
}

static void APP_InitPxp(void)
{
    PXP_Init(APP_PXP);
}

static void APP_RotateCmdQueue(void)
{
    uint8_t i, j;
    uint8_t curLcdBufferIdx = 1U;

    static const pxp_rotate_degree_t degrees[] = {
        kPXP_Rotate0,
        kPXP_Rotate90,
        kPXP_Rotate180,
        kPXP_Rotate270,
    };

    for (;;)
    {
        for (i = 0; i < ARRAY_SIZE(degrees); i++)
        {
            /* Prepare next buffer for LCD. */
            pxpCommand.CTRL    = (pxpCommand.CTRL & ~PXP_CTRL_ROTATE_MASK) | PXP_CTRL_ROTATE(degrees[i]);
            pxpCommand.OUT_BUF = (uint32_t)s_psBufferLcd[curLcdBufferIdx];
            __DSB();

            while (PXP_IsNextCommandPending(APP_PXP))
            {
            }
            PXP_SetNextCommand(APP_PXP, &pxpCommand);

            /* Wait for process complete. */
            while (!(kPXP_CompleteFlag & PXP_GetStatusFlags(APP_PXP)))
            {
            }

            PXP_ClearStatusFlags(APP_PXP, kPXP_CompleteFlag);

            /* Now new frame is ready, pass it to LCDIF. */
            ELCDIF_SetNextBufferAddr(APP_ELCDIF, (uint32_t)s_psBufferLcd[curLcdBufferIdx]);

            /* Show the active frame buffer for a while. */
            for (j = 0; j < APP_FRAME_PER_PIC; j++)
            {
                ELCDIF_ClearInterruptStatus(APP_ELCDIF, kELCDIF_CurFrameDone);
                while (!(kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(APP_ELCDIF)))
                {
                }
            }

            /* Switch to the other LCD buffer. */
            curLcdBufferIdx ^= 1U;
        }
    }
}

static void APP_InitInputBuffer(void)
{
    uint32_t i, j;

    /*
     * The PS input buffer is:
     *
     *  -----------------------------
     *  |             |             |
     *  |             |             |
     *  |   BLUE      |     RED     |
     *  |             |             |
     *  |             |             |
     *  |-------------+-------------|
     *  |                           |
     *  |                           |
     *  |           GREEN           |
     *  |                           |
     *  |                           |
     *  -----------------------------
     */
    for (i = 0; i < (APP_PS_SIZE / 2); i++)
    {
        for (j = 0; j < (APP_PS_SIZE / 2); j++)
        {
            s_psBufferPxp[i][j] = 0xFFU;
        }

        for (; j < APP_PS_SIZE; j++)
        {
            s_psBufferPxp[i][j] = 0xFF0000U;
        }
    }

    for (; i < APP_PS_SIZE; i++)
    {
        for (j = 0; j < APP_PS_SIZE; j++)
        {
            s_psBufferPxp[i][j] = 0xFF00U;
        }
    }

    memset(s_psBufferLcd, 0x0U, sizeof(s_psBufferLcd));
}

static void APP_InitPxpCommand(void)
{
    pxpCommand.CTRL       = PXP_CTRL_ROT_POS(1) | PXP_CTRL_ENABLE_MASK;
    pxpCommand.STAT       = 0x00000000;                                       /* STAT, don't care */
    pxpCommand.OUT_CTRL   = PXP_PS_CTRL_FORMAT(kPXP_OutputPixelFormatRGB888); /* OUT_CTRL */
    pxpCommand.OUT_BUF    = (uint32_t)s_psBufferLcd[0];                       /* OUT_BUF */
    pxpCommand.OUT_BUF2   = 0x00000000;                                       /* OUT_BUF2 */
    pxpCommand.OUT_PITCH  = APP_BPP * APP_IMG_WIDTH;                          /* OUT_PITCH */
    pxpCommand.OUT_LRC    = PXP_OUT_LRC_Y(APP_IMG_HEIGHT - 1U) | PXP_OUT_LRC_X(APP_IMG_WIDTH - 1U); /* OUT_LRC */
    pxpCommand.OUT_PS_ULC = PXP_OUT_PS_ULC_Y(APP_PS_ULC_Y) | PXP_OUT_PS_ULC_X(APP_PS_ULC_X);        /* OUT_PS_ULC */
    pxpCommand.OUT_PS_LRC = PXP_OUT_PS_LRC_Y(APP_PS_LRC_Y) | PXP_OUT_PS_LRC_X(APP_PS_LRC_X);        /* OUT_PS_LRC */
    /* Disable AS. */
    pxpCommand.OUT_AS_ULC    = 0x3FFF3FFF;                 /* OUT_AS_ULC */
    pxpCommand.OUT_AS_LRC    = 0x00000000;                 /* OUT_AS_LRC */
    pxpCommand.PS_CTRL       = 0x00000004;                 /* PS_CTRL */
    pxpCommand.PS_BUF        = (uint32_t)s_psBufferPxp;    /* PS_BUF */
    pxpCommand.PS_UBUF       = 0x00000000;                 /* PS_UBUF */
    pxpCommand.PS_VBUF       = 0x00000000;                 /* PS_VBUF */
    pxpCommand.PS_PITCH      = APP_BPP * APP_PS_SIZE;      /* PS_PITCH */
    pxpCommand.PS_BACHGROUND = 0x00000000;                 /* PS_BACHGROUND */
    pxpCommand.PS_SCALE      = 0x10001000;                 /* PS_SCALE */
    pxpCommand.PS_OFFSET     = 0x00000000;                 /* PS_OFFSET */
    pxpCommand.PS_CLRKEYLOW  = 0x00FFFFFF;                 /* PS_CLRKEYLOW */
    pxpCommand.PS_CLRKEYHIGH = 0x00000000;                 /* PS_CLRKEYHIGH */
    pxpCommand.AS_CTRL       = 0x00000000;                 /* AS_CTRL */
    pxpCommand.AS_BUF        = 0x00000000;                 /* AS_BUF */
    pxpCommand.AS_PITCH      = 0x00000000;                 /* AS_PITCH */
    pxpCommand.AS_CLRKEYLOW  = 0x00FFFFFF;                 /* AS_CLRKEYLOW */
    pxpCommand.AS_CLRKEYHIGH = 0x00000000;                 /* AS_CLRKEYHIGH */
    pxpCommand.CSC1_COEF0    = PXP_CSC1_COEF0_BYPASS_MASK; /* CSC1_COEF0, don't care. */
    pxpCommand.CSC1_COEF1    = 0x00000000;                 /* CSC1_COEF1, don't care. */
    pxpCommand.CSC1_COEF2    = 0x00000000;                 /* CSC1_COEF2, don't care. */
#if !(defined(FSL_FEATURE_PXP_HAS_NO_CSC2) && FSL_FEATURE_PXP_HAS_NO_CSC2)
    pxpCommand.CSC2_CTRL  = PXP_CSC2_CTRL_BYPASS_MASK; /* CSC2_CTRL */
    pxpCommand.CSC2_COEF0 = 0x00000000;                /* CSC2_COEF0, don't care. */
    pxpCommand.CSC2_COEF1 = 0x00000000;                /* CSC2_COEF1, don't care. */
    pxpCommand.CSC2_COEF2 = 0x00000000;                /* CSC2_COEF2, don't care. */
    pxpCommand.CSC2_COEF3 = 0x00000000;                /* CSC2_COEF3, don't care. */
    pxpCommand.CSC2_COEF4 = 0x00000000;                /* CSC2_COEF4, don't care. */
    pxpCommand.CSC2_COEF5 = 0x00000000;                /* CSC2_COEF5, don't care. */
#endif
#if !(defined(FSL_FEATURE_PXP_HAS_NO_LUT) && FSL_FEATURE_PXP_HAS_NO_LUT)
    pxpCommand.LUT_CTRL   = PXP_LUT_CTRL_BYPASS_MASK; /* LUT_CTRL */
    pxpCommand.LUT_ADDR   = 0x00000000;               /* LUT_ADDR */
    pxpCommand.LUT_DATA   = 0x00000000;               /* LUT_DATA */
    pxpCommand.LUT_EXTMEM = 0x00000000;               /* LUT_EXTMEM */
    pxpCommand.CFA        = 0x00000000;               /* CFA */
#endif
    pxpCommand.HIST_CTRL     = 0x00000020; /* HIST_CTRL */
    pxpCommand.HIST2_PARAM   = 0x00000F00; /* HIST2_PARAM */
    pxpCommand.HIST4_PARAM   = 0x0F0A0500; /* HIST4_PARAM */
    pxpCommand.HIST8_PARAM0  = 0x06040200; /* HIST8_PARAM0 */
    pxpCommand.HIST8_PARAM1  = 0x0F0D0B09; /* HIST8_PARAM1 */
    pxpCommand.HIST16_PARAM0 = 0x03020100; /* HIST16_PARAM0 */
    pxpCommand.HIST16_PARAM1 = 0x07060504; /* HIST16_PARAM1 */
    pxpCommand.HIST16_PARAM2 = 0x0B0A0908; /* HIST16_PARAM2 */
    pxpCommand.HIST16_PARAM3 = 0x0F0E0D0C; /* HIST16_PARAM3 */
    pxpCommand.POWER         = 0x00000000; /* POWER */
    pxpCommand.NEXT          = 0x00000000; /* NEXT, don't care */
}
