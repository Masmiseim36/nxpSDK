/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "app.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"
#include "fsl_jpegdec.h"
#include "jpeg.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_ezhv.h"
#include "ezhv_support.h"
#include "display_support.h"

#include "vglite_support.h"
#include "vglite_window.h"
#include "vg_lite_kernel.h"
#include "vg_lite.h"

#include "ezhv_para.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
JPEG_DECODER_Type g_jpegdec = {
    .core    = JPEGDEC,
    .wrapper = JPGDECWRP,
};

#define APP_JPEGDEC            (&g_jpegdec)
#define APP_JPEGDEC_IRQHandler JPEGDEC_IRQHandler
#define APP_JPEGDEC_IRQn       JPEGDEC_IRQn

/* GPU_EN should be 1 if FORMAT_TYPE is 8 */
#if (FORMAT_TYPE == 8)
#define APP_JPG_DEC_OUTBUF_NUM   1U
#define APP_FB_FORMAT            kVIDEO_PixelFormatYUYV
#define APP_FB_BPP               1U
#define APP_JPG_PATCH            3U
#define APP_FMT                  kFMT_yuv444_linear2yv24
#elif (FORMAT_TYPE == 7)
#define APP_JPG_DEC_OUTBUF_NUM   1U
#define APP_FB_FORMAT            kVIDEO_PixelFormatRGB888
#define APP_FB_BPP               3U
#define APP_JPG_PATCH            3U
#define APP_FMT                  kFMT_yuv444_linear2rgb888
#elif (FORMAT_TYPE == 6)
#define APP_JPG_DEC_OUTBUF_NUM   1U
#define APP_FB_FORMAT            kVIDEO_PixelFormatYUYV
#define APP_FB_BPP               2U
#define APP_JPG_PATCH            3U
#define APP_FMT                  kFMT_yuv444_linear2tiledyuyv
#elif (FORMAT_TYPE == 5) /* not support */
#elif (FORMAT_TYPE == 4) /* not support */
#elif (FORMAT_TYPE == 3) /* not support */
#elif (FORMAT_TYPE == 2)
#define APP_JPG_DEC_OUTBUF_NUM   1U
#define APP_FB_FORMAT            kVIDEO_PixelFormatYUYV
#define APP_FB_BPP               2U
#define APP_JPG_PATCH            2U
#define APP_FMT                  kFMT_yuyv_linear2tiled
#elif (FORMAT_TYPE == 1)
#define APP_JPG_DEC_OUTBUF_NUM   2U
#define APP_FB_FORMAT            kVIDEO_PixelFormatNV12
#define APP_FB_BPP               1U
#define APP_JPG_PATCH            1U
#define APP_FMT                  kFMT_nv12_linear2tiled
#else
#define APP_JPG_DEC_OUTBUF_NUM   2U
#define APP_FB_FORMAT            kVIDEO_PixelFormatRGB565
#define APP_FB_BPP               2U
#define APP_JPG_PATCH            1U
#define APP_FMT                  kFMT_nv12_linear2rgb565
#endif

/*! @brief  resolution */
#define DEMO_IMG_WIDHT 640U
#define DEMO_IMG_HEIHT 640U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if (GPU_EN == 1)
static void vglite_task(void *pvParam);
#else
static void ezhv_task(void *pvParam);
static void DEMO_BufferSwitchOffCallback(void *param, void *switchOffBuffer);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if (APP_JPG_DEC_OUTBUF_NUM == 1)
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_decodedAddr[DEMO_IMG_WIDHT * DEMO_IMG_HEIHT * 3], 1024);
#endif
#if (APP_JPG_DEC_OUTBUF_NUM == 2)
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_decodedAddr[DEMO_IMG_WIDHT * DEMO_IMG_HEIHT], 1024);
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_decodedAddr1[DEMO_IMG_WIDHT * DEMO_IMG_HEIHT], 1024);
#endif
#if (FORMAT_TYPE == 8)
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_decodeAddrUV[DEMO_IMG_WIDHT * DEMO_IMG_HEIHT*2], 1024);
#endif

AT_NONCACHEABLE_SECTION(static jpegdec_descpt_t s_decoderDespt);

static volatile bool s_newFrameShown = false;

static vg_lite_display_t display;
static vg_lite_window_t window;

#if (CUSTOM_VGLITE_MEMORY_CONFIG != 1)
#error "Application must be compiled with CUSTOM_VGLITE_MEMORY_CONFIG=1"
#else
#define VGLITE_COMMAND_BUFFER_SZ (128 * 1024)
/* On RT595S */
#if defined(CPU_MIMXRT595SFFOC_cm33) || defined(CPU_MIMXRT798SGFOA_cm33_core0)
#define VGLITE_HEAP_SZ 0x100000 /* 1 MB */
/* On RT1170 */
#elif defined(CPU_MIMXRT1176DVMAA_cm7) || defined(CPU_MIMXRT1166DVM6A_cm7)
#define VGLITE_HEAP_SZ 8912896 /* 8.5 MB */
#else
#error "Unsupported CPU !"
#endif
#if (720 * 1280 == (DEMO_PANEL_WIDTH) * (DEMO_PANEL_HEIGHT))
#define TW 720
#if defined(CPU_MIMXRT595SFFOC_cm33) || defined(CPU_MIMXRT798SGFOA_cm33_core0)
/* Tessellation window = 720 x 640 */
#define TH 640
/* On RT1170 */
#elif defined(CPU_MIMXRT1176DVMAA_cm7) || defined(CPU_MIMXRT1166DVM6A_cm7)
/* Tessellation window = 720 x 1280 */
#define TH 1280
#else
#error "Unsupported CPU !"
#endif
/* Panel RM67162. Supported only by platform RT595S. */
#elif (400 * 400 == (DEMO_PANEL_WIDTH) * (DEMO_PANEL_HEIGHT))
/* Tessellation window = 400 x 400 */
#define TW 400
#define TH 400
#else
/* Tessellation window = 256 x 256 */
#define TW 256
#define TH 256
#endif
/* Allocate the heap and set the command buffer(s) size */
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t vglite_heap[VGLITE_HEAP_SZ], 64);
AT_NONCACHEABLE_SECTION(vg_lite_error_t vglite_error);
void *vglite_heap_base        = &vglite_heap;
uint32_t vglite_heap_size     = VGLITE_HEAP_SZ;
#endif

#if (GPU_EN == 1)
static vg_lite_matrix_t s_matrix;
static vg_lite_buffer_t s_blit_source;

#else
static dc_fb_info_t g_fbInfo;
static uint32_t s_FbBuf[2] = {DEMO_BUFFER0_ADDR, DEMO_BUFFER1_ADDR};
#endif

static volatile uint32_t s_ezhvIrqIdx = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/
void EZHV_Callback(void *param)
{
    s_ezhvIrqIdx = 1;
}

void ezhv_convertion(void)
{
    EZHV_Para_t *pstPara = (EZHV_Para_t *)EZHV_GetParaAddr();
    ezhv_para_linear2tile_t *pstFmt =
              (ezhv_para_linear2tile_t*)&(pstPara->paraAddr);
    pstPara->apiIdx = kEZHV_API_linear2tile;
    pstFmt->jpgPatch = APP_JPG_PATCH;
    pstFmt->fb_bpp = APP_FB_BPP;
    pstFmt->fmt_conv = APP_FMT;
    pstFmt->width = s_decoderDespt.config.width;
    pstFmt->height = s_decoderDespt.config.height;

#if (GPU_EN == 1)
    /* update source buffers */
    pstFmt->srcBuff0 = s_decoderDespt.config.outBufAddr0;
#if (0 == FORMAT_TYPE) || (1 == FORMAT_TYPE)
    pstFmt->srcBuff1 = s_decoderDespt.config.outBufAddr1;
#endif
    /* update destination buffers */
#if (8 == FORMAT_TYPE)
    pstFmt->dstBuff0 = s_decoderDespt.config.outBufAddr0;
    pstFmt->dstBuff1 = (uint32_t)&s_decodeAddrUV[0];
    pstFmt->dstBuff2 = (uint32_t)&s_decodeAddrUV[s_decoderDespt.config.width \
                     * s_decoderDespt.config.height];
#endif
#else
    pstFmt->srcBuff0 = s_decoderDespt.config.outBufAddr0;
    pstFmt->dstBuff0 = (uint32_t)s_FbBuf[0];
#if (0 == FORMAT_TYPE)
    pstFmt->srcBuff1 = s_decoderDespt.config.outBufAddr1;
#elif (1 == FORMAT_TYPE)
    pstFmt->srcBuff1 = s_decoderDespt.config.outBufAddr1;
    pstFmt->dstBuff1 = (uint32_t)s_FbBuf[0] +
            pstFmt->height * pstFmt->width * APP_FB_BPP;
#endif
#endif

    /* wake-up ezhv */
    EZHV_WakeUpEzhv(kEZHV_ARM2EZHV_MSI);
    while(!s_ezhvIrqIdx)
    {;}
}

void decode_jpeg(void)
{
    jpegdec_config_t config;
    uint32_t status;

    memset(&s_decoderDespt, 0U, sizeof(jpegdec_descpt_t));

    /* Step 1: Init JPEG decoder module. */
    JPEGDEC_GetDefaultConfig(&config);

    config.slots = kJPEGDEC_Slot0; /* Enable only one slot. */

    JPEGDEC_Init(APP_JPEGDEC, &config);

    /* Step 2: Set source buffer, buffer size. */
    JPEGDEC_SetJpegBuffer(&s_decoderDespt.config,
            (uint8_t *)jpegImg, jpegImgLen);

#if (APP_JPG_DEC_OUTBUF_NUM==1)
    /* Step 3: Set buffer of generated image for JPEG decoder. */
    JPEGDEC_SetOutputBuffer(&s_decoderDespt.config,
            (uint8_t *)s_decodedAddr, NULL);
#else
    /* Step 3: Set buffer of generated image for JPEG decoder. */
    JPEGDEC_SetOutputBuffer(&s_decoderDespt.config,
            (uint8_t *)s_decodedAddr, (uint8_t *)s_decodedAddr1);
#endif

    /* Step 4: Parse header. */
    JPEGDEC_ParseHeader(&s_decoderDespt.config);

    /* Step 5: Set output pitch, auto start decode when descriptor is loaded. */
    JPEGDEC_SetDecodeOption(&s_decoderDespt.config, 
            s_decoderDespt.config.width * APP_JPG_PATCH, false, true);

    /* Step 6: Set slot descriptor. */
    JPEGDEC_SetSlotNextDescpt(APP_JPEGDEC, 0U, &s_decoderDespt);

    /* Step 7: Enable the descriptor to start the decoding. */
    JPEGDEC_EnableSlotNextDescpt(APP_JPEGDEC, 0U);

    /* Step 8: Wait for decoding complete. */
    status = JPEGDEC_GetStatusFlags(APP_JPEGDEC, 0U);

    while ((status & (kJPEGDEC_DecodeCompleteFlag | kJPEGDEC_ErrorFlags)) == 0U)
    {
        status = JPEGDEC_GetStatusFlags(APP_JPEGDEC, 0U);
    }

    if ((status & kJPEGDEC_DecodeCompleteFlag) == 0U)
    {
        JPEGDEC_ClearStatusFlags(APP_JPEGDEC, 0U, status);
        PRINTF("Error occured during JPEG decoding\r\n");
        assert(false);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    BOARD_Init16bitsPsRam(XSPI2);

    /* Power on JPEG decoder. */
    POWER_DisablePD(kPDRUNCFG_PPD_JPEGDEC);
    POWER_DisablePD(kPDRUNCFG_APD_JPEGDEC);
    POWER_ApplyPD();

    /* EZH-V initization */
    BOARD_EZHV_Init();
    EZHV_EnableEzhv2ArmIntChan(kEZHV_EzhvToArmIntChan0);
    EnableIRQ(EZHV_IRQn);
    EZHV_SetCallback(EZHV_Callback, 0, NULL);

    PRINTF("JPEG decode and EZHV linear2tile conversion demo start.\r\n");

#if (GPU_EN == 1)
    if (xTaskCreate(vglite_task, "vglite_task",
            configMINIMAL_STACK_SIZE + 2000, NULL, 1, NULL) != pdPASS)
#else
    if (xTaskCreate(ezhv_task, "ezhv_task",
            configMINIMAL_STACK_SIZE + 2000, NULL, 1, NULL) != pdPASS)
#endif
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}

#if (GPU_EN == 1)
static void vglite_blit_resources(void)
{
    s_blit_source.tiled	         = VG_LITE_LINEAR;
    s_blit_source.compress_mode  = VG_LITE_DEC_DISABLE;
}

static void gpu_blit_source_update(void)
{
    s_blit_source.width	 = s_decoderDespt.config.width;
    s_blit_source.height = s_decoderDespt.config.height;
    s_blit_source.memory     = (uint32_t *)s_decodedAddr; 
    s_blit_source.address    = (vg_lite_uint32_t)s_blit_source.memory;
    s_blit_source.width      = VG_LITE_ALIGN(s_blit_source.width, 8);
    s_blit_source.height     = VG_LITE_ALIGN(s_blit_source.height, 8);
    s_blit_source.image_mode = VG_LITE_NORMAL_IMAGE_MODE;
    s_blit_source.transparency_mode = VG_LITE_IMAGE_OPAQUE;

#if (FORMAT_TYPE == 0) || (FORMAT_TYPE == 1)
    s_blit_source.format  = VG_LITE_NV12;
    s_blit_source.stride  =
                VG_LITE_ALIGN(s_blit_source.width * APP_JPG_PATCH, 64);
    s_blit_source.yuv.uv_stride =
                VG_LITE_ALIGN(s_blit_source.width * APP_JPG_PATCH, 64);
    s_blit_source.yuv.uv_height = s_blit_source.height / 2;
    s_blit_source.yuv.uv_memory = (uint32_t *) s_decodedAddr1;
    s_blit_source.yuv.uv_planar =
                (vg_lite_uint32_t)s_decoderDespt.config.outBufAddr1;
#elif (FORMAT_TYPE == 2)
    s_blit_source.format = VG_LITE_YUYV;
    s_blit_source.stride =
                VG_LITE_ALIGN(s_blit_source.width * APP_JPG_PATCH, 64);
#elif (FORMAT_TYPE == 8)
    s_blit_source.format = VG_LITE_YV24;
    s_blit_source.stride  = VG_LITE_ALIGN(s_blit_source.width, 64);
    s_blit_source.yuv.uv_stride = VG_LITE_ALIGN(s_blit_source.width, 64);
    s_blit_source.yuv.v_stride  = s_blit_source.yuv.uv_stride;
    s_blit_source.yuv.uv_height = VG_LITE_ALIGN(s_blit_source.height, 8);
    s_blit_source.yuv.v_height  = s_blit_source.yuv.uv_height;
    s_blit_source.yuv.uv_planar = (vg_lite_uint32_t)&s_decodeAddrUV[0];
    s_blit_source.yuv.uv_memory = (vg_lite_pointer)&s_decodeAddrUV[0];
    s_blit_source.yuv.v_planar  = (vg_lite_uint32_t)&s_decodeAddrUV[ \
                s_blit_source.yuv.uv_height * s_blit_source.yuv.uv_stride];
    s_blit_source.yuv.v_memory  = (vg_lite_pointer)s_blit_source.yuv.v_planar;
#else
    /* not support */
#endif
}

static void gpu_blit(vg_lite_buffer_t *rt, vg_lite_buffer_t * source)
{
    vg_lite_buffer_t *blitBuffer = source;
    int8_t error = 0;
    
    if (rt == NULL)
    {
        PRINTF("vg_lite_get_renderTarget error\r\n");
        while (1)
            ;
    }
    
    /* clear buffer */
    error = vg_lite_clear(rt, NULL, 0x0);
    if (error) 
    {
      PRINTF("Error****%d:%s\n", __LINE__, __func__);
      while(1);
    }
    //vg_lite_finish();

    error = vg_lite_blit(rt, blitBuffer, &s_matrix,
                VG_LITE_BLEND_NONE, 0, VG_LITE_FILTER_POINT);
    if (error) 
    {
      PRINTF("Error****%d:%s\n", __LINE__, __func__);
      return;
    }

    vg_lite_finish();

    return;
}

static void cleanup(void)
{
    vg_lite_close();
}

static vg_lite_error_t init_vg_lite(void)
{
    vg_lite_error_t error = VG_LITE_SUCCESS;

    error = VGLITE_CreateDisplay(&display);
    if (error)
    {
        PRINTF("VGLITE_CreateDisplay() returned error %d\r\n", error);
        return error;
    }

    // Initialize the window.
    error = VGLITE_CreateWindow(&display, &window);
    if (error)
    {
        PRINTF("VGLITE_CreateWindow() returned error %d\r\n", error);
        return error;
    }

    // Set GPU command buffer size for this drawing task.
    error = vg_lite_set_command_buffer_size(VGLITE_COMMAND_BUFFER_SZ);
    if (error)
    {
        PRINTF("vg_lite_set_command_buffer_size() returned error %d\n", error);
        cleanup();
        return error;
    }
    
    // Initialize the draw.
    error = vg_lite_init(TW, TH);
    if (error)
    {
        PRINTF("vg_lite_init() returned error %d\r\n", error);
        cleanup();
        return error;
    }

    vg_lite_identity(&s_matrix);

    vg_lite_scale(1.0f, 1.0f, &s_matrix);

    return error;
}

static void redraw()
{    
    /* update blit_source */
    gpu_blit_source_update();

    vg_lite_buffer_t *rt = VGLITE_GetRenderTarget(&window);
    if (rt == NULL)
    {
        PRINTF("vg_lite_get_renderTarget error\r\n");
        while (1)
            ;
    }

    gpu_blit(rt, &s_blit_source);

    VGLITE_SwapBuffers(&window);
}

static uint32_t getTime()
{
    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
}

static void vglite_task(void *pvParam)
{
    status_t status;
    vg_lite_error_t error;

    status = BOARD_PrepareVGLiteController();
    if (status != kStatus_Success)
    {
        PRINTF("Prepare VGlite contolor error\r\n");
        while (1)
            ;
    }

    /* re-set gpu root clock */
    CLOCK_AttachClk(kMAIN_PLL_PFD0_to_VGPU);
    CLOCK_SetClkDiv(kCLOCK_DivVgpuClk, 2);

    vglite_blit_resources();

    error = init_vg_lite();
    if (error)
    {
        PRINTF("init_vg_lite() returned error %d\r\n", error);
        while (1)
            ;
    }

    /* decode jpeg file */
    decode_jpeg();

#if (FORMAT_TYPE == 8)
    /* ezhv conversion */
    ezhv_convertion(); 
#endif

    redraw();

    while(1)
    {
        vTaskDelay(portMAX_DELAY);
    }
}

#else
static void DEMO_BufferSwitchOffCallback(void *param, void *switchOffBuffer)
{
    s_newFrameShown = true;
}

static void DEMO_InitDisplay(void)
{
    status_t status;

    BOARD_PrepareDisplayController();

#if (FORMAT_TYPE==1)
    memset((void *)s_FbBuf[0], 0, DEMO_PANEL_HEIGHT * DEMO_PANEL_WIDTH * APP_FB_BPP);
    memset((void *)s_FbBuf[1], 0, DEMO_PANEL_HEIGHT * DEMO_PANEL_WIDTH * APP_FB_BPP);
#else
    memset((void *)s_FbBuf[0], 0, DEMO_PANEL_HEIGHT * DEMO_PANEL_WIDTH * APP_FB_BPP);
#endif

    status = g_dc.ops->init(&g_dc);
    if (kStatus_Success != status)
    {
        PRINTF("Display initialization failed\r\n");
        assert(false);
    }

    g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &g_fbInfo);
    g_dc.ops->setCallback(&g_dc, 0, DEMO_BufferSwitchOffCallback, NULL);
    g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)s_FbBuf[0]);
}

static void DEMO_Display(void)
{
    /* Step 10: Configure display layer configuration. */
    g_fbInfo.strideBytes = s_decoderDespt.config.width * APP_FB_BPP;

#if (FORMAT_TYPE==1)
    g_fbInfo.strideBytes_2p = s_decoderDespt.config.width * APP_FB_BPP;
#endif

    g_fbInfo.pixelFormat = APP_FB_FORMAT;
    g_fbInfo.width       = s_decoderDespt.config.width;
    g_fbInfo.height      = s_decoderDespt.config.height;
    g_fbInfo.startX = (DEMO_PANEL_WIDTH - s_decoderDespt.config.width) / 2U;
    g_fbInfo.startY = (DEMO_PANEL_HEIGHT - s_decoderDespt.config.height) / 2U;
    if (kStatus_Success != g_dc.ops->setLayerConfig(&g_dc, 0, &g_fbInfo))
    {
        PRINTF("Error: Could not configure the display controller\r\n");
        assert(false);
    }
    /* Update fb buffer again */
    g_dc.ops->setFrameBuffer(&g_dc, 0, (void *)s_FbBuf[0]);
    /* Step 11: Enable layer and display the decoded image. */
    g_dc.ops->enableLayer(&g_dc, 0);

    if ((g_dc.ops->getProperty(&g_dc) & kDC_FB_ReserveFrameBuffer) == 0)
    {
        while (s_newFrameShown == false)
        {
        }
    }
}

static void ezhv_task(void *pvParam)
{
    /* initialize display resource */
    DEMO_InitDisplay();
    /* decode jpeg file */
    decode_jpeg();
    /* ezhv conversion */
    ezhv_convertion();
    /* display on */
    DEMO_Display();

    while(1)
    {
        vTaskDelay(portMAX_DELAY);
    }
}

#endif