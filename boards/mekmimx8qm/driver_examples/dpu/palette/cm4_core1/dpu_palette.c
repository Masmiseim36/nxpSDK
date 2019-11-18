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

/* Landscape or portrait. */
#if (APP_PANEL_WIDTH < APP_PANEL_HEIGHT)
#define APP_PORTRAIT 1
#else
#define APP_PORTRAIT 0
#endif

#define APP_MAKE_COLOR(red, green, blue, alpha) \
    ((((uint32_t)(alpha)) << 24U) | (((uint32_t)(red)) << 16U) | (((uint32_t)(green)) << 8U) | ((uint32_t)(blue)))

#if APP_PORTRAIT
#define APP_STREAM_WIDTH (APP_PANEL_WIDTH)
#define APP_STREAM_HEIGHT (APP_PANEL_HEIGHT / 2)
#else
#define APP_STREAM_WIDTH (APP_PANEL_WIDTH / 2)
#define APP_STREAM_HEIGHT (APP_PANEL_HEIGHT)
#endif

#define APP_SUBLAYER_WIDTH (APP_STREAM_WIDTH)
#define APP_SUBLAYER_HEIGHT (APP_STREAM_HEIGHT / 8)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/



/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Frame buffer for fetch layer, all sublayers share one frame buffer, but use different color palette. */
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_fetchLayerFrameBuffer[APP_SUBLAYER_HEIGHT][APP_SUBLAYER_WIDTH], 32);
/* Frame buffer for fetch decode. */
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_fetchDecodeFrameBuffer[APP_STREAM_HEIGHT][APP_STREAM_WIDTH], 32);

/* Palette use ARGB8888 format. */
uint32_t s_fetchLayerPalette[DPU_PALETTE_ENTRY_NUM];
uint32_t s_fetchDecodePalette[DPU_PALETTE_ENTRY_NUM];

volatile bool s_isSafetyStreamShadowPending  = false;
volatile bool s_isContentStreamShadowPending = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void APP_InitFrameBuffer(void)
{
    uint32_t i, j;

    for (i = 0; i < APP_SUBLAYER_HEIGHT; i++)
    {
        for (j = 0; j < APP_SUBLAYER_WIDTH; j++)
        {
            s_fetchLayerFrameBuffer[i][j] = j;
        }
    }

    for (i = 0; i < APP_STREAM_HEIGHT; i++)
    {
        for (j = 0; j < APP_STREAM_WIDTH; j++)
        {
            s_fetchDecodeFrameBuffer[i][j] = j;
        }
    }
}

void APP_InitPalette(void)
{
    uint32_t i;

    /*
     * Fetch layer
     *
     * In this project, all sublayers use different palettes.
     */
    for (i = 0; i < ARRAY_SIZE(s_fetchLayerPalette) / 8; i++)
    {
        s_fetchLayerPalette[i] = APP_MAKE_COLOR(0, 0, 0xFF - i, 0xFF);
    }
    for (; i < ARRAY_SIZE(s_fetchLayerPalette) * 2 / 8; i++)
    {
        s_fetchLayerPalette[i] = APP_MAKE_COLOR(0, 0xFF - i, 0, 0xFF);
    }
    for (; i < ARRAY_SIZE(s_fetchLayerPalette) * 3 / 8; i++)
    {
        s_fetchLayerPalette[i] = APP_MAKE_COLOR(0xFF - i, 0, 0, 0xFF);
    }
    for (; i < ARRAY_SIZE(s_fetchLayerPalette) * 4 / 8; i++)
    {
        s_fetchLayerPalette[i] = APP_MAKE_COLOR(0, 0xFF - i, 0xFF - i, 0xFF);
    }
    for (; i < ARRAY_SIZE(s_fetchLayerPalette) * 5 / 8; i++)
    {
        s_fetchLayerPalette[i] = APP_MAKE_COLOR(0xFF - i, 0, 0xFF - i, 0xFF);
    }
    for (; i < ARRAY_SIZE(s_fetchLayerPalette) * 6 / 8; i++)
    {
        s_fetchLayerPalette[i] = APP_MAKE_COLOR(0xFF - i, 0xFF - i, 0, 0xFF);
    }
    for (; i < ARRAY_SIZE(s_fetchLayerPalette) * 7 / 8; i++)
    {
        s_fetchLayerPalette[i] = APP_MAKE_COLOR(0xFF - i, 0xFF - i, 0xFF - i, 0xFF);
    }
    for (; i < ARRAY_SIZE(s_fetchLayerPalette) * 8 / 8; i++)
    {
        s_fetchLayerPalette[i] = APP_MAKE_COLOR(i, i, i, 0xFF);
    }

    for (i = 0; i < ARRAY_SIZE(s_fetchDecodePalette); i++)
    {
        /* Fetch decode palette is set to blue color. */
        s_fetchDecodePalette[i] = APP_MAKE_COLOR(0, 0, 0xFF - i, 0xFF);
    }
}

void APP_TriggerSafetyStreamShadowLoad(void)
{
    s_isSafetyStreamShadowPending = true;
    DPU_TriggerPipelineShadowLoad(APP_DPU, APP_SAFETY_STREAM_PIPELINE);
}

void APP_TriggerContentStreamShadowLoad(void)
{
    s_isContentStreamShadowPending = true;
    DPU_TriggerPipelineShadowLoad(APP_DPU, APP_CONTENT_STREAM_PIPELINE);
}

void APP_DPU_Palette(void)
{
    dpu_display_config_t displayConfig;
    dpu_display_timing_config_t displayTimingConfig;
    dpu_const_frame_config_t cfConfig;
    dpu_layer_blend_config_t lbConfig;
    dpu_src_buffer_config_t sbConfig;
    dpu_fetch_unit_config_t fetchConfig;

    DPU_Init(APP_DPU);

    DPU_PreparePathConfig(APP_DPU);

    /*
     * Step 1: Configure the safety stream
     *
     * The safety stream uses fetch decoder.
     */

    /* Pipeline. */
    DPU_InitPipeline(APP_DPU, APP_SAFETY_STREAM_PIPELINE);

    /* ExtDst. */
    DPU_InitExtDst(APP_DPU, APP_SAFETY_STREAM_EXTDST, DPU_MAKE_SRC_REG1(kDPU_UnitSrcLayerBlend1));

    /* Layer blend. */
    DPU_InitLayerBlend(APP_DPU, kDPU_LayerBlend1, DPU_MAKE_SRC_REG2(kDPU_UnitSrcConstFrame4, kDPU_UnitSrcFetchDecode1));
    DPU_LayerBlendGetDefaultConfig(&lbConfig);
    DPU_SetLayerBlendConfig(APP_DPU, kDPU_LayerBlend1, &lbConfig);
    DPU_EnableLayerBlend(APP_DPU, kDPU_LayerBlend1, true);

    /* Const frame. */
    cfConfig.frameWidth  = APP_STREAM_WIDTH;
    cfConfig.frameHeight = APP_STREAM_HEIGHT;
    cfConfig.constColor  = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);
    DPU_InitConstFrame(APP_DPU, kDPU_ConstFrame4);
    DPU_SetConstFrameConfig(APP_DPU, kDPU_ConstFrame4, &cfConfig);

    /*
     * Fetch decode unit.
     */
    DPU_FetchUnitGetDefaultConfig(&fetchConfig);
    fetchConfig.frameHeight = APP_STREAM_HEIGHT;
    fetchConfig.frameWidth  = APP_STREAM_WIDTH;
    DPU_InitFetchUnit(APP_DPU, kDPU_FetchDecode1, &fetchConfig);

    DPU_SrcBufferGetDefaultConfig(&sbConfig);

    /* strideBytes and bitsPerPixel set to the actually value in frame buffer. */
    sbConfig.strideBytes  = 1 * APP_STREAM_WIDTH;
    sbConfig.bitsPerPixel = 8;
    /* pixel format set to align with the palette pixel format. */
    sbConfig.pixelFormat  = kDPU_PixelFormatARGB8888;
    sbConfig.bufferHeight = APP_STREAM_HEIGHT;
    sbConfig.bufferWidth  = APP_STREAM_WIDTH;
    sbConfig.constColor   = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);
    sbConfig.baseAddr     = (uint32_t)s_fetchDecodeFrameBuffer;

    DPU_SetFetchUnitSrcBufferConfig(APP_DPU, kDPU_FetchDecode1, 0, &sbConfig);
    DPU_EnableFetchUnitSrcBuffer(APP_DPU, kDPU_FetchDecode1, 0, true);

    DPU_SetColorPaletteIndexWidth(APP_DPU, kDPU_FetchDecode1, 8);
    DPU_UpdateColorPalette(APP_DPU, kDPU_FetchDecode1, 0, s_fetchDecodePalette, ARRAY_SIZE(s_fetchDecodePalette));
    DPU_EnableColorPalette(APP_DPU, kDPU_FetchDecode1, 0, true);

    /* Configuration complete, trigger the shadow load. */
    APP_TriggerSafetyStreamShadowLoad();
    DPU_EnableInterrupts(APP_DPU, 0, APP_SAFETY_STREAM_INT_GROUP0);
    DPU_EnableInterrupts(APP_DPU, 1, APP_SAFETY_STREAM_INT_GROUP1);

    /*
     * Step 2: Configure the content stream
     *
     * The content stream uses the fetch layer, all sublayers are enabled.
     */

    /* Pipeline. */
    DPU_InitPipeline(APP_DPU, APP_CONTENT_STREAM_PIPELINE);

    /* ExtDst. */
    DPU_InitExtDst(APP_DPU, APP_CONTENT_STREAM_EXTDST, DPU_MAKE_SRC_REG1(kDPU_UnitSrcLayerBlend0));

    /* Layer blend. */
    DPU_InitLayerBlend(APP_DPU, kDPU_LayerBlend0, DPU_MAKE_SRC_REG2(kDPU_UnitSrcConstFrame0, kDPU_UnitSrcFetchLayer0));
    DPU_LayerBlendGetDefaultConfig(&lbConfig);
    DPU_SetLayerBlendConfig(APP_DPU, kDPU_LayerBlend0, &lbConfig);
    DPU_EnableLayerBlend(APP_DPU, kDPU_LayerBlend0, true);

    /* Const frame. */
    cfConfig.frameWidth  = APP_STREAM_WIDTH;
    cfConfig.frameHeight = APP_STREAM_HEIGHT;
    cfConfig.constColor  = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);
    DPU_InitConstFrame(APP_DPU, kDPU_ConstFrame0);
    DPU_SetConstFrameConfig(APP_DPU, kDPU_ConstFrame0, &cfConfig);

    /*
     * Fetch layer unit.
     */

    /* Fetch unit source buffer configure. */
    DPU_FetchUnitGetDefaultConfig(&fetchConfig);
    fetchConfig.frameHeight = APP_STREAM_HEIGHT;
    fetchConfig.frameWidth  = APP_STREAM_WIDTH;
    DPU_InitFetchUnit(APP_DPU, kDPU_FetchLayer0, &fetchConfig);

    DPU_SrcBufferGetDefaultConfig(&sbConfig);

    /* strideBytes and bitsPerPixel set to the actually value in frame buffer. */
    sbConfig.strideBytes  = 1 * APP_SUBLAYER_WIDTH;
    sbConfig.bitsPerPixel = 8;
    /* pixel format set to align with the palette pixel format. */
    sbConfig.pixelFormat  = kDPU_PixelFormatARGB8888;
    sbConfig.bufferHeight = APP_SUBLAYER_HEIGHT;
    sbConfig.bufferWidth  = APP_SUBLAYER_WIDTH;
    sbConfig.constColor   = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);
    sbConfig.baseAddr     = (uint32_t)s_fetchLayerFrameBuffer;

    DPU_SetColorPaletteIndexWidth(APP_DPU, kDPU_FetchLayer0, 5);
    DPU_UpdateColorPalette(APP_DPU, kDPU_FetchLayer0, 0, s_fetchLayerPalette, ARRAY_SIZE(s_fetchLayerPalette));

    for (uint32_t i = 0; i < 8; i++)
    {
        DPU_SetFetchUnitSrcBufferConfig(APP_DPU, kDPU_FetchLayer0, i, &sbConfig);
        DPU_SetFetchUnitOffset(APP_DPU, kDPU_FetchLayer0, i, 0, i * APP_SUBLAYER_HEIGHT);
        DPU_EnableFetchUnitSrcBuffer(APP_DPU, kDPU_FetchLayer0, i, true);
        DPU_EnableColorPalette(APP_DPU, kDPU_FetchLayer0, i, true);
    }

    /* Configuration complete, trigger the shadow load. */
    APP_TriggerContentStreamShadowLoad();
    DPU_EnableInterrupts(APP_DPU, 0, APP_CONTENT_STREAM_INT_GROUP0);
    DPU_EnableInterrupts(APP_DPU, 1, APP_CONTENT_STREAM_INT_GROUP1);

    /* Step 3: Configure the display stream. */
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

    displayConfig.displayMode = kDPU_DisplayPrimOnTop;

#if APP_PORTRAIT
    displayConfig.primAreaStartX = 1;
    displayConfig.primAreaStartY = APP_STREAM_HEIGHT + 1;
#else
    displayConfig.primAreaStartX = APP_STREAM_WIDTH + 1;
    displayConfig.primAreaStartY = 1;
#endif

    displayConfig.secAreaStartX = 1;
    displayConfig.secAreaStartY = 1;

    DPU_SetDisplayConfig(APP_DPU, APP_DPU_DISPLAY_INDEX, &displayConfig);

    DPU_TriggerDisplayShadowLoad(APP_DPU, APP_DPU_DISPLAY_INDEX);

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
}

void DPU_IRQHandler(void)
{
    uint32_t intGroup0Flags, intGroup1Flags;
    intGroup0Flags = DPU_GetInterruptsPendingFlags(APP_DPU, 0);
    intGroup1Flags = DPU_GetInterruptsPendingFlags(APP_DPU, 1);

    if ((intGroup0Flags & APP_SAFETY_STREAM_INT_GROUP0) || (intGroup1Flags & APP_SAFETY_STREAM_INT_GROUP1))
    {
        s_isSafetyStreamShadowPending = false;
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

    PRINTF("DPU palette example start...\r\n");

    APP_InitFrameBuffer();
    APP_InitPalette();

    /* Start the DPU. */
    APP_DPU_Palette();

    while (1)
    {
    }
}
