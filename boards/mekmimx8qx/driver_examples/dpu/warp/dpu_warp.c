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

#include "fsl_common.h"
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

#if APP_PORTRAIT
#define APP_FETCH_FRAME_WIDTH (APP_PANEL_WIDTH)
#define APP_FETCH_FRAME_HEIGHT (APP_PANEL_HEIGHT / 2)
#else
#define APP_FETCH_FRAME_WIDTH (APP_PANEL_WIDTH / 2)
#define APP_FETCH_FRAME_HEIGHT (APP_PANEL_HEIGHT)
#endif

/* Warp coordinate plane uses 32 bpp: 2 x s12.4 (signed fix-point) */
#define APP_WARP_COOR_BPP 32

/*******************************************************************************
 * Prototypes
 ******************************************************************************/



/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Frame buffer before warp */
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_frameBuffer[APP_FETCH_FRAME_HEIGHT][APP_FETCH_FRAME_WIDTH], 32);
/* Frame buffer for coordinate plane. */
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_coordinateFrameBuffer[APP_FETCH_FRAME_HEIGHT][APP_FETCH_FRAME_WIDTH], 32);

volatile bool s_isSafetyStreamShadowPending  = false;
volatile bool s_isContentStreamShadowPending = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t APP_MakeCoordinatate(float x, float y)
{
    /* 32 bpp: 2 x s12.4 (signed fix-point) */
    return (((int32_t)(x * (float)(1 << 4))) << 16) | (((int32_t)(y * (float)(1 << 4))) << 0);
}

void APP_InitFrameBuffer(void)
{
    uint32_t i, j;
    uint32_t leftLine, leftPixel;
    uint8_t lineStartColorIdx = 0;
    uint8_t colorIdx;
    uint32_t color;
    uint32_t chessBoardColors[] = {APP_COLOR_WHITE, APP_COLOR_BLACK};
    float x, y;

#define APP_CHESS_BOARD_GRID (APP_FETCH_FRAME_WIDTH / 8)

    /* Draw chess board. */
    j = 0;
    for (i = 0; i < APP_FETCH_FRAME_HEIGHT;)
    {
        colorIdx = lineStartColorIdx;

        leftLine = APP_FETCH_FRAME_HEIGHT - j;
        leftLine = (APP_CHESS_BOARD_GRID > leftLine) ? leftLine : APP_CHESS_BOARD_GRID;

        while (leftLine--)
        {
            for (j = 0; j < APP_FETCH_FRAME_WIDTH;)
            {
                leftPixel = APP_FETCH_FRAME_WIDTH - j;
                leftPixel = (APP_CHESS_BOARD_GRID > leftPixel) ? leftPixel : APP_CHESS_BOARD_GRID;

                color = chessBoardColors[colorIdx];

                while (leftPixel--)
                {
                    s_frameBuffer[i][j++] = color;
                }

                colorIdx ^= 1;
            }

            i++;
        }
        lineStartColorIdx ^= 1;
    }

    /*
     * Initialize the warping coordinate.
     *
     * In this example, the relationship between pixel position in input frame (x, y)
     * and pixel position in output frame (u, v) is:
     *
     * x = (u * u) / frame_width
     * y = (v * v) / frame_height
     *
     */
    for (i = 0; i < APP_FETCH_FRAME_HEIGHT; i++)
    {
        y = (float)(i * i) / (float)APP_FETCH_FRAME_HEIGHT;

        /*
         * Because biliear filter is used in warp, so for the edge, should
         * take care of the pixel (black) out of input frame buffer.
         */
        if (y < 1)
        {
            y = 1;
        }
        else if (y > APP_FETCH_FRAME_HEIGHT - 2)
        {
            y = APP_FETCH_FRAME_HEIGHT - 2;
        }

        for (j = 0; j < APP_FETCH_FRAME_WIDTH; j++)
        {
            x = (float)(j * j) / (float)APP_FETCH_FRAME_WIDTH;

            if (x < 1)
            {
                x = 1;
            }
            else if (x > APP_FETCH_FRAME_WIDTH - 2)
            {
                x = APP_FETCH_FRAME_WIDTH - 2;
            }

            s_coordinateFrameBuffer[i][j] = APP_MakeCoordinatate(x, y);
        }
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

void APP_DPU_ArbitaryWarping(void)
{
    dpu_display_config_t displayConfig;
    dpu_display_timing_config_t displayTimingConfig;
    dpu_const_frame_config_t cfConfig;
    dpu_layer_blend_config_t lbConfig;
    dpu_warp_config_t warpConfig;
    dpu_src_buffer_config_t sbConfig;
    dpu_coordinates_config_t coordinateConfig;
    dpu_fetch_unit_config_t fetchConfig;

    DPU_Init(APP_DPU);

    DPU_PreparePathConfig(APP_DPU);

    /*
     * Step 1: Configure the safety stream
     *
     * The safety stream shows the frame before warping.
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
    cfConfig.frameWidth  = APP_FETCH_FRAME_WIDTH;
    cfConfig.frameHeight = APP_FETCH_FRAME_HEIGHT;
    cfConfig.constColor  = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);
    DPU_InitConstFrame(APP_DPU, kDPU_ConstFrame4);
    DPU_SetConstFrameConfig(APP_DPU, kDPU_ConstFrame4, &cfConfig);

    /*
     * Fetch decode unit.
     */
    DPU_FetchUnitGetDefaultConfig(&fetchConfig);
    fetchConfig.frameHeight = APP_FETCH_FRAME_HEIGHT;
    fetchConfig.frameWidth  = APP_FETCH_FRAME_WIDTH;
    DPU_InitFetchUnit(APP_DPU, kDPU_FetchDecode1, &fetchConfig);

    DPU_SrcBufferGetDefaultConfig(&sbConfig);

    sbConfig.strideBytes  = 4 * APP_FETCH_FRAME_WIDTH;
    sbConfig.bitsPerPixel = 32;
    sbConfig.pixelFormat  = kDPU_PixelFormatARGB8888;
    sbConfig.bufferHeight = APP_FETCH_FRAME_HEIGHT;
    sbConfig.bufferWidth  = APP_FETCH_FRAME_WIDTH;
    sbConfig.constColor   = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);
    sbConfig.baseAddr     = (uint32_t)s_frameBuffer;

    DPU_SetFetchUnitSrcBufferConfig(APP_DPU, kDPU_FetchDecode1, 0, &sbConfig);
    DPU_EnableFetchUnitSrcBuffer(APP_DPU, kDPU_FetchDecode1, 0, true);

    /* Configuration complete, trigger the shadow load. */
    APP_TriggerSafetyStreamShadowLoad();
    DPU_EnableInterrupts(APP_DPU, 0, APP_SAFETY_STREAM_INT_GROUP0);
    DPU_EnableInterrupts(APP_DPU, 1, APP_SAFETY_STREAM_INT_GROUP1);

    /*
     * Step 2: Configure the content stream
     *
     * The content stream shows the frame after warping.
     */

    /* Pipeline. */
    DPU_InitPipeline(APP_DPU, APP_CONTENT_STREAM_PIPELINE);

    /* ExtDst. */
    DPU_InitExtDst(APP_DPU, APP_CONTENT_STREAM_EXTDST, DPU_MAKE_SRC_REG1(kDPU_UnitSrcLayerBlend0));

    /* Layer blend. */
    DPU_InitLayerBlend(APP_DPU, kDPU_LayerBlend0, DPU_MAKE_SRC_REG2(kDPU_UnitSrcConstFrame0, kDPU_UnitSrcFetchWarp2));
    DPU_LayerBlendGetDefaultConfig(&lbConfig);
    DPU_SetLayerBlendConfig(APP_DPU, kDPU_LayerBlend0, &lbConfig);
    DPU_EnableLayerBlend(APP_DPU, kDPU_LayerBlend0, true);

    /* Const frame. */
    cfConfig.frameWidth  = APP_FETCH_FRAME_WIDTH;
    cfConfig.frameHeight = APP_FETCH_FRAME_HEIGHT;
    cfConfig.constColor  = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);
    DPU_InitConstFrame(APP_DPU, kDPU_ConstFrame0);
    DPU_SetConstFrameConfig(APP_DPU, kDPU_ConstFrame0, &cfConfig);

    /*
     * Fetch warp unit.
     */
    DPU_FetcUnitGetDefaultWarpConfig(&warpConfig);
    warpConfig.srcReg           = DPU_MAKE_SRC_REG1(kDPU_UnitSrcFetchEco2);
    warpConfig.frameHeight      = APP_FETCH_FRAME_HEIGHT;
    warpConfig.frameWidth       = APP_FETCH_FRAME_WIDTH;
    warpConfig.warpBitsPerPixel = APP_WARP_COOR_BPP;
    DPU_InitFetchUnitWarp(APP_DPU, kDPU_FetchWarp2, &warpConfig);

    /* Fetch unit source buffer configure. */
    DPU_SrcBufferGetDefaultConfig(&sbConfig);

    sbConfig.strideBytes  = 4 * APP_FETCH_FRAME_WIDTH;
    sbConfig.bitsPerPixel = 32;
    sbConfig.pixelFormat  = kDPU_PixelFormatARGB8888;
    sbConfig.bufferHeight = APP_FETCH_FRAME_HEIGHT;
    sbConfig.bufferWidth  = APP_FETCH_FRAME_WIDTH;
    sbConfig.constColor   = DPU_MAKE_CONST_COLOR(0, 0, 0, 0);
    sbConfig.baseAddr     = (uint32_t)s_frameBuffer;

    DPU_SetFetchUnitSrcBufferConfig(APP_DPU, kDPU_FetchWarp2, 0, &sbConfig);
    DPU_SetFetchUnitOffset(APP_DPU, kDPU_FetchWarp2, 0, 0, 0);
    DPU_EnableFetchUnitSrcBuffer(APP_DPU, kDPU_FetchWarp2, 0, true);

    /* Warp coordinate configure. */
    DPU_CorrdinatesGetDefaultConfig(&coordinateConfig);
    coordinateConfig.bitsPerPixel = APP_WARP_COOR_BPP;
    coordinateConfig.strideBytes  = (APP_WARP_COOR_BPP / 8) * APP_FETCH_FRAME_WIDTH;
    coordinateConfig.baseAddr     = (uint32_t)s_coordinateFrameBuffer;
    coordinateConfig.frameHeight  = APP_FETCH_FRAME_HEIGHT;
    coordinateConfig.frameWidth   = APP_FETCH_FRAME_WIDTH;

    DPU_InitWarpCoordinates(APP_DPU, kDPU_FetchEco2, &coordinateConfig);

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
    displayConfig.primAreaStartY = APP_FETCH_FRAME_HEIGHT + 1;
#else
    displayConfig.primAreaStartX = APP_FETCH_FRAME_WIDTH + 1;
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
    sc_ipc_t ipc;

    ipc = BOARD_InitRpc();
    BOARD_InitPins(ipc);
    BOARD_BootClockRUN();
    BOARD_PrepareDisplay();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    PRINTF("DPU Warp Example:\r\n");

    APP_InitFrameBuffer();

    APP_DPU_ArbitaryWarping();

    while (1)
    {
    }
}
