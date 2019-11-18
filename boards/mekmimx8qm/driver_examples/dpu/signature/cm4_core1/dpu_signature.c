/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_dpu.h"
#include "dpu_example.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 *******************************************************************************/

#define APP_MAKE_COLOR(red, green, blue, alpha) \
    ((((uint32_t)(alpha)) << 24U) | (((uint32_t)(red)) << 16U) | (((uint32_t)(green)) << 8U) | ((uint32_t)(blue)))

#define APP_COLOR_BLUE APP_MAKE_COLOR(0, 0, 0xFF, 0xFF)
#define APP_COLOR_GREEN APP_MAKE_COLOR(0, 0xFF, 0, 0xFF)
#define APP_COLOR_RED APP_MAKE_COLOR(0xFF, 0, 0, 0xFF)
#define APP_COLOR_YELLOW APP_MAKE_COLOR(0xFF, 0xFF, 0, 0xFF)
#define APP_COLOR_CYAN APP_MAKE_COLOR(0, 0xFF, 0xFF, 0xFF)
#define APP_COLOR_MAGENTA APP_MAKE_COLOR(0xFF, 0, 0xFF, 0xFF)
#define APP_COLOR_BLACK APP_MAKE_COLOR(0, 0, 0, 0xFF)
#define APP_COLOR_WHITE APP_MAKE_COLOR(0xFF, 0xFF, 0xFF, 0xFF)
#define APP_COLOR_SILVER APP_MAKE_COLOR(0xC0, 0xC0, 0xC0, 0xFF)
#define APP_COLOR_GRAY APP_MAKE_COLOR(0x80, 0x80, 0x80, 0xFF)

/* Signature validate window 0. */
#define APP_SIG_WIN0_UPPER_LEFT_X 0
#define APP_SIG_WIN0_UPPER_LEFT_Y 0
#define APP_SIG_WIN0_LOWER_RIGHT_X (APP_PANEL_WIDTH / 4 - 1)
#define APP_SIG_WIN0_LOWER_RIGHT_Y (APP_PANEL_HEIGHT / 4 - 1)
#define APP_SIG_WIN0_WIDTH (APP_SIG_WIN0_LOWER_RIGHT_X - APP_SIG_WIN0_UPPER_LEFT_X + 1)
#define APP_SIG_WIN0_HEIGHT (APP_SIG_WIN0_LOWER_RIGHT_Y - APP_SIG_WIN0_UPPER_LEFT_Y + 1)
#define APP_SIG_WIN0_COLOR APP_COLOR_BLUE

/* Signature validate window 1. */
#define APP_SIG_WIN1_UPPER_LEFT_X (APP_PANEL_WIDTH * 3 / 4)
#define APP_SIG_WIN1_UPPER_LEFT_Y (APP_PANEL_HEIGHT * 3 / 4)
#define APP_SIG_WIN1_LOWER_RIGHT_X (APP_PANEL_WIDTH - 1)
#define APP_SIG_WIN1_LOWER_RIGHT_Y (APP_PANEL_HEIGHT - 1)
#define APP_SIG_WIN1_WIDTH (APP_SIG_WIN1_LOWER_RIGHT_X - APP_SIG_WIN1_UPPER_LEFT_X + 1)
#define APP_SIG_WIN1_HEIGHT (APP_SIG_WIN1_LOWER_RIGHT_Y - APP_SIG_WIN1_UPPER_LEFT_Y + 1)
#define APP_SIG_WIN1_COLOR APP_COLOR_YELLOW

/* Floating overlay window. */
#define APP_FLOAT_WIN_WIDTH (APP_PANEL_WIDTH / 2)
#define APP_FLOAT_WIN_HEIGHT (APP_PANEL_HEIGHT / 2)
#define APP_FLOAT_WIN_INIT_OFFSET_X (APP_PANEL_WIDTH / 4)
#define APP_FLOAT_WIN_INIT_OFFSET_Y (APP_PANEL_HEIGHT / 4)
#define APP_FLOAT_WIN_COLOR APP_COLOR_RED

/*******************************************************************************
 * Prototypes
 ******************************************************************************/



/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_sigWin0FrameBuffer[APP_SIG_WIN0_HEIGHT][APP_SIG_WIN0_WIDTH], 32);
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_sigWin1FrameBuffer[APP_SIG_WIN1_HEIGHT][APP_SIG_WIN1_WIDTH], 32);
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_floatWinFrameBuffer[APP_FLOAT_WIN_HEIGHT][APP_FLOAT_WIN_WIDTH], 32);

volatile bool s_isSigShadowPending           = false;
volatile bool s_isDisplayStreamShadowPending = false;
volatile bool s_isSafetyStreamShadowPending  = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void APP_InitFrameBuffer(uint32_t frameBuffer, uint16_t height, uint16_t width, uint32_t color)
{
    uint32_t i, j;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            *((uint32_t *)frameBuffer + (width * i) + j) = color;
        }
    }
}

bool APP_IsSigShadowPending(void)
{
    return s_isSigShadowPending;
}

bool APP_IsSafetyStreamShadowPending(void)
{
    return s_isSafetyStreamShadowPending;
}

bool APP_IsDisplayStreamShadowPending(void)
{
    return s_isDisplayStreamShadowPending;
}

void APP_TriggerSigShadowLoad(void)
{
    s_isSigShadowPending = true;
    DPU_TriggerSignatureShadowLoad(APP_DPU, APP_DPU_DISPLAY_INDEX);
}

void APP_TriggerSafetyStreamShadowLoad(void)
{
    s_isSafetyStreamShadowPending = true;
    DPU_TriggerPipelineShadowLoad(APP_DPU, APP_SAFETY_STREAM_PIPELINE);
}

void APP_TriggerDisplayStreamShadowLoad(void)
{
    s_isDisplayStreamShadowPending = true;
    DPU_TriggerDisplayShadowLoad(APP_DPU, APP_DPU_DISPLAY_INDEX);
}

void APP_DPU_Signature(void)
{
    uint32_t redCRC, greenCRC, blueCRC;
    dpu_display_timing_config_t displayTimingConfig;
    dpu_signature_config_t sigConfig;
    dpu_signature_window_config_t sigWinConfig;
    dpu_const_frame_config_t cfConfig;
    dpu_layer_blend_config_t lbConfig;
    dpu_fetch_unit_config_t fetchConfig;
    dpu_src_buffer_config_t sbConfig;
    dpu_display_config_t displayConfig;

    /* For the floating window. */
    int16_t offsetIncX = 1;
    int16_t offsetIncY = 1;
    uint16_t offsetX   = APP_FLOAT_WIN_INIT_OFFSET_X;
    uint16_t offsetY   = APP_FLOAT_WIN_INIT_OFFSET_Y;

    DPU_Init(APP_DPU);

    DPU_PreparePathConfig(APP_DPU);

    /* Step 1: Configure the safety stream */

    /* Pipeline. */
    DPU_InitPipeline(APP_DPU, APP_SAFETY_STREAM_PIPELINE);

    /* ExtDst. */
    DPU_InitExtDst(APP_DPU, APP_SAFETY_STREAM_EXTDST, DPU_MAKE_SRC_REG1(kDPU_UnitSrcLayerBlend2));

    /* Layer blend. */
    DPU_InitLayerBlend(APP_DPU, kDPU_LayerBlend2, DPU_MAKE_SRC_REG2(kDPU_UnitSrcConstFrame4, kDPU_UnitSrcFetchLayer0));
    DPU_LayerBlendGetDefaultConfig(&lbConfig);
    DPU_SetLayerBlendConfig(APP_DPU, kDPU_LayerBlend2, &lbConfig);
    DPU_EnableLayerBlend(APP_DPU, kDPU_LayerBlend2, true);

    /* Const frame. */
    cfConfig.frameWidth  = APP_PANEL_WIDTH;
    cfConfig.frameHeight = APP_PANEL_HEIGHT;
    cfConfig.constColor  = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);
    DPU_InitConstFrame(APP_DPU, kDPU_ConstFrame4);
    DPU_SetConstFrameConfig(APP_DPU, kDPU_ConstFrame4, &cfConfig);

    /*
     * Fetch unit.
     * FetchLayer0 is used here, the sublayer 0 ~ 2 are used.
     * sublayer 0: Show rectangle in the signature validate window 0.
     * sublayer 1: Show rectangle in the signature validate window 1.
     * sublayer 2: Show float rectangle moving in the whole screen.
     */
    DPU_FetchUnitGetDefaultConfig(&fetchConfig);
    fetchConfig.frameHeight = APP_PANEL_HEIGHT;
    fetchConfig.frameWidth  = APP_PANEL_WIDTH;
    DPU_InitFetchUnit(APP_DPU, kDPU_FetchLayer0, &fetchConfig);

    /* Fetch unit source buffer configure. */
    DPU_SrcBufferGetDefaultConfig(&sbConfig);

    sbConfig.bitsPerPixel = 32;
    sbConfig.pixelFormat  = kDPU_PixelFormatARGB8888;
    sbConfig.constColor   = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);

    /* Sublayer 0. */
    sbConfig.strideBytes  = 4 * APP_SIG_WIN0_WIDTH;
    sbConfig.bufferHeight = APP_SIG_WIN0_HEIGHT;
    sbConfig.bufferWidth  = APP_SIG_WIN0_WIDTH;
    sbConfig.baseAddr     = (uint32_t)s_sigWin0FrameBuffer;
    DPU_SetFetchUnitSrcBufferConfig(APP_DPU, kDPU_FetchLayer0, 0, &sbConfig);
    DPU_SetFetchUnitOffset(APP_DPU, kDPU_FetchLayer0, 0, APP_SIG_WIN0_UPPER_LEFT_X, APP_SIG_WIN0_UPPER_LEFT_Y);
    DPU_EnableFetchUnitSrcBuffer(APP_DPU, kDPU_FetchLayer0, 0, true);

    /* Sublayer 1. */
    sbConfig.strideBytes  = 4 * APP_SIG_WIN1_WIDTH;
    sbConfig.bufferHeight = APP_SIG_WIN1_HEIGHT;
    sbConfig.bufferWidth  = APP_SIG_WIN1_WIDTH;
    sbConfig.baseAddr     = (uint32_t)s_sigWin1FrameBuffer;
    DPU_SetFetchUnitSrcBufferConfig(APP_DPU, kDPU_FetchLayer0, 1, &sbConfig);
    DPU_SetFetchUnitOffset(APP_DPU, kDPU_FetchLayer0, 1, APP_SIG_WIN1_UPPER_LEFT_X, APP_SIG_WIN1_UPPER_LEFT_Y);
    DPU_EnableFetchUnitSrcBuffer(APP_DPU, kDPU_FetchLayer0, 1, true);

    /* Sublayer 2. */
    sbConfig.strideBytes  = 4 * APP_FLOAT_WIN_WIDTH;
    sbConfig.bufferHeight = APP_FLOAT_WIN_HEIGHT;
    sbConfig.bufferWidth  = APP_FLOAT_WIN_WIDTH;
    sbConfig.baseAddr     = (uint32_t)s_floatWinFrameBuffer;
    DPU_SetFetchUnitSrcBufferConfig(APP_DPU, kDPU_FetchLayer0, 2, &sbConfig);
    DPU_SetFetchUnitOffset(APP_DPU, kDPU_FetchLayer0, 2, APP_FLOAT_WIN_INIT_OFFSET_X, APP_FLOAT_WIN_INIT_OFFSET_Y);
    DPU_EnableFetchUnitSrcBuffer(APP_DPU, kDPU_FetchLayer0, 2, true);

    /* Configuration complete, trigger the shadow load. */
    APP_TriggerSafetyStreamShadowLoad();
    DPU_EnableInterrupts(APP_DPU, 0, APP_SAFETY_STREAM_INT_GROUP0);
    DPU_EnableInterrupts(APP_DPU, 1, APP_SAFETY_STREAM_INT_GROUP1);

    /* Step 2: Configure the display stream. */
    DPU_DisplayTimingGetDefaultConfig(&displayTimingConfig);

    displayTimingConfig.flags  = APP_DPU_TIMING_FLAGS;
    displayTimingConfig.width  = APP_PANEL_WIDTH;
    displayTimingConfig.hsw    = APP_HSW;
    displayTimingConfig.hfp    = APP_HFP;
    displayTimingConfig.hbp    = APP_HBP;
    displayTimingConfig.height = APP_PANEL_HEIGHT;
    displayTimingConfig.vsw    = APP_VSW;
    displayTimingConfig.vfp    = APP_VFP;
    displayTimingConfig.vbp    = APP_VBP;

    DPU_InitDisplayTiming(APP_DPU, APP_DPU_DISPLAY_INDEX, &displayTimingConfig);

    DPU_DisplayGetDefaultConfig(&displayConfig);

    /* Only show the safety stream in normal mode. */
    displayConfig.displayMode = kDPU_DisplayOnlyPrim;
    /* Show the test pattern in panic mode. */
    displayConfig.displayModeInPanic = kDPU_DisplayTest;

    displayConfig.primAreaStartX = 1;
    displayConfig.primAreaStartY = 1;

    DPU_SetDisplayConfig(APP_DPU, APP_DPU_DISPLAY_INDEX, &displayConfig);

    APP_TriggerDisplayStreamShadowLoad();
    DPU_EnableInterrupts(APP_DPU, 0, APP_DISPLAY_STREAM_INT_GROUP0);
    DPU_EnableInterrupts(APP_DPU, 1, APP_DISPLAY_STREAM_INT_GROUP1);

    /*
     * Initialize workflow:
     * 1. Init DPU
     * 2. Configure pixel link.
     * 3. Init and start display interface.
     * 4. Start DPU
     */
    APP_InitPixelLink();

    APP_StartDisplayInterface();

    DPU_StartDisplay(APP_DPU, APP_DPU_DISPLAY_INDEX);

    /* Wait for the safety stream and display stream shadow load finished. */
    while (APP_IsDisplayStreamShadowPending())
    {
    }

    while (APP_IsSafetyStreamShadowPending())
    {
    }

    /* Step 3: Initialize the signature unit. */
    DPU_SignatureGetDefaultConfig(&sigConfig);
    DPU_InitSignature(APP_DPU, APP_DPU_DISPLAY_INDEX, &sigConfig);

    DPU_SignatureWindowGetDefaultConfig(&sigWinConfig);

    /* Signature validate window 0. */
    sigWinConfig.controlFlags = kDPU_SignatureWindowEnableLocalPanic;
    sigWinConfig.upperLeftX   = APP_SIG_WIN0_UPPER_LEFT_X;
    sigWinConfig.upperLeftY   = APP_SIG_WIN0_UPPER_LEFT_Y;
    sigWinConfig.lowerRightX  = APP_SIG_WIN0_LOWER_RIGHT_X;
    sigWinConfig.lowerRightY  = APP_SIG_WIN0_LOWER_RIGHT_Y;

    DPU_SetSignatureWindowConfig(APP_DPU, APP_DPU_DISPLAY_INDEX, 0, &sigWinConfig);
    DPU_EnableSignatureWindowCompute(APP_DPU, APP_DPU_DISPLAY_INDEX, 0, true);

    /* Signature validate window 1. */
    sigWinConfig.controlFlags = kDPU_SignatureWindowEnableLocalPanic;
    sigWinConfig.upperLeftX   = APP_SIG_WIN1_UPPER_LEFT_X;
    sigWinConfig.upperLeftY   = APP_SIG_WIN1_UPPER_LEFT_Y;
    sigWinConfig.lowerRightX  = APP_SIG_WIN1_LOWER_RIGHT_X;
    sigWinConfig.lowerRightY  = APP_SIG_WIN1_LOWER_RIGHT_Y;

    DPU_SetSignatureWindowConfig(APP_DPU, APP_DPU_DISPLAY_INDEX, 1, &sigWinConfig);
    DPU_EnableSignatureWindowCompute(APP_DPU, APP_DPU_DISPLAY_INDEX, 1, true);

    APP_TriggerSigShadowLoad();

    DPU_EnableInterrupts(APP_DPU, 0, APP_SIG_SHD_LD_INT_GROUP0);
    DPU_EnableInterrupts(APP_DPU, 1, APP_SIG_SHD_LD_INT_GROUP1);

    /* Wait for signature shadow load finished */
    while (APP_IsSigShadowPending())
    {
    }

    /* Wait for signature measurement finish. */
    while (!(kDPU_SignatureValid & DPU_GetSignatureStatus(APP_DPU, APP_DPU_DISPLAY_INDEX)))
    {
    }

    /* Get current measured signature value and set them as the reference signature value. */
    DPU_GetSignatureWindowCrc(APP_DPU, APP_DPU_DISPLAY_INDEX, 0, &redCRC, &greenCRC, &blueCRC);
    DPU_SetSignatureWindowRefCrc(APP_DPU, APP_DPU_DISPLAY_INDEX, 0, redCRC, greenCRC, blueCRC);

    DPU_GetSignatureWindowCrc(APP_DPU, APP_DPU_DISPLAY_INDEX, 1, &redCRC, &greenCRC, &blueCRC);
    DPU_SetSignatureWindowRefCrc(APP_DPU, APP_DPU_DISPLAY_INDEX, 1, redCRC, greenCRC, blueCRC);

    /* Enable the evaluation window check. */
    DPU_EnableSignatureWindowCheck(APP_DPU, APP_DPU_DISPLAY_INDEX, 0, true);
    DPU_EnableSignatureWindowCheck(APP_DPU, APP_DPU_DISPLAY_INDEX, 1, true);

    APP_TriggerSigShadowLoad();
    /* Wait for signature shadow load finished */
    while (APP_IsSigShadowPending())
    {
    }

    /* Moving the float window. */
    for (;;)
    {
        if (0U == offsetX)
        {
            offsetIncX = 1;
        }
        else if (APP_PANEL_WIDTH - APP_FLOAT_WIN_WIDTH - 1 <= offsetX)
        {
            offsetX    = APP_PANEL_WIDTH - APP_FLOAT_WIN_WIDTH - 1;
            offsetIncX = -1;
        }

        if (0U == offsetY)
        {
            offsetIncY = 1;
        }
        else if (APP_PANEL_HEIGHT - APP_FLOAT_WIN_HEIGHT - 1 <= offsetY)
        {
            offsetY    = APP_PANEL_HEIGHT - APP_FLOAT_WIN_HEIGHT - 1;
            offsetIncY = -1;
        }

        /* Update the format: offset. */
        offsetX += offsetIncX;
        offsetY += offsetIncY;

        while (APP_IsSafetyStreamShadowPending())
        {
        }

        DPU_SetFetchUnitOffset(APP_DPU, kDPU_FetchLayer0, 2, offsetX, offsetY);

        APP_TriggerSafetyStreamShadowLoad();
    }
}

void DPU_IRQHandler(void)
{
    uint32_t intGroup0Flags, intGroup1Flags;
    intGroup0Flags = DPU_GetInterruptsPendingFlags(APP_DPU, 0);
    intGroup1Flags = DPU_GetInterruptsPendingFlags(APP_DPU, 1);

    if ((intGroup0Flags & APP_SIG_SHD_LD_INT_GROUP0) || (intGroup1Flags & APP_SIG_SHD_LD_INT_GROUP1))
    {
        s_isSigShadowPending = false;
    }

    if ((intGroup0Flags & APP_SAFETY_STREAM_INT_GROUP0) || (intGroup1Flags & APP_SAFETY_STREAM_INT_GROUP1))
    {
        s_isSafetyStreamShadowPending = false;
    }

    if ((intGroup0Flags & APP_DISPLAY_STREAM_INT_GROUP0) || (intGroup1Flags & APP_DISPLAY_STREAM_INT_GROUP1))
    {
        s_isDisplayStreamShadowPending = false;
    }

    DPU_ClearInterruptsPendingFlags(APP_DPU, 0, intGroup0Flags);
    DPU_ClearInterruptsPendingFlags(APP_DPU, 1, intGroup1Flags);
    __DSB();
}

/*!
 * @brief Main function
 */
int main(void)
{
    sc_ipc_t ipc = BOARD_InitRpc();

    BOARD_InitPins(ipc);
    BOARD_BootClockRUN();
    BOARD_PrepareDisplay();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    PRINTF("DPU Signature Example:\r\n");

    APP_InitFrameBuffer((uint32_t)s_sigWin0FrameBuffer, APP_SIG_WIN0_HEIGHT, APP_SIG_WIN0_WIDTH, APP_SIG_WIN0_COLOR);
    APP_InitFrameBuffer((uint32_t)s_sigWin1FrameBuffer, APP_SIG_WIN1_HEIGHT, APP_SIG_WIN1_WIDTH, APP_SIG_WIN1_COLOR);
    APP_InitFrameBuffer((uint32_t)s_floatWinFrameBuffer, APP_FLOAT_WIN_HEIGHT, APP_FLOAT_WIN_WIDTH,
                        APP_FLOAT_WIN_COLOR);

    APP_DPU_Signature();

    while (1)
    {
    }
}
