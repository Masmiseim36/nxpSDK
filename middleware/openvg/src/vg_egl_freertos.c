/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include "vg_context.h"
#include "EGL/eglplatform.h"
#include "fsl_fbdev.h"
#include "display_support.h"

#define APP_BUFFER_COUNT 2
#if !DEMO_BUFFER_FIXED_ADDRESS
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_frameBuffer[APP_BUFFER_COUNT][DEMO_BUFFER_HEIGHT][DEMO_BUFFER_WIDTH][DEMO_BUFFER_BYTE_PER_PIXEL],
    FRAME_BUFFER_ALIGN);

#define DEMO_BUFFER0_ADDR (uint32_t) s_frameBuffer[0]

#if APP_BUFFER_COUNT > 1
#define DEMO_BUFFER1_ADDR (uint32_t) s_frameBuffer[1]
#endif

#if APP_BUFFER_COUNT > 2
#define DEMO_BUFFER2_ADDR (uint32_t) s_frameBuffer[2]
#endif

#endif

static const uint32_t s_frameBufferAddress[APP_BUFFER_COUNT] = {DEMO_BUFFER0_ADDR,
#if APP_BUFFER_COUNT > 1
                                                                DEMO_BUFFER1_ADDR,
#endif
#if APP_BUFFER_COUNT > 2
                                                                DEMO_BUFFER2_ADDR
#endif
};

struct fr_display
{
    fbdev_t g_fbdev;
    fbdev_fb_info_t g_fbInfo;
    int refCount;
};

#define FREERTOS_EGL_BACKBUFFERS 2
struct fr_window
{
    struct fr_display *display;
    vg_lite_buffer_t buffers[FREERTOS_EGL_BACKBUFFERS];
    int width;
    int height;
    int bufferCount;
};

struct fr_pixmap
{
    vg_lite_buffer_t *imgbuf;
    /* Pointer to memory bits. */
    void *bits;
    /* Bits per pixel. */
    int bpp;
    /* Size and format. */
    int width, height;
    int stride;
    VGImageFormat format;
};

typedef struct
{
    struct fr_window *window;
    struct fr_display *display;
} OSWindowContext;

struct eglFbPlatform
{
    int (*GetDisplay)(NativeDisplayType *display);

    int (*GetDisplayByIndex)(int displayIndex, NativeDisplayType *display);

    int (*GetDisplayInfo)(NativeDisplayType display, int *width, int *height,
                          size_t *physical, int *stride, int *bitsPerPixel);

    int (*DestroyDisplay)(NativeDisplayType Display);

    int (*CreateWindow)(NativeDisplayType Display, int X, int Y, int Width,
                        int Height, NativeWindowType *Window);

    int (*GetWindowInfo)(NativeDisplayType Display, NativeWindowType Window,
                         int *X, int *Y, int *Width, int *Height,
                         int *BitsPerPixel, unsigned int *Offset);

    int (*DestroyWindow)(NativeDisplayType Display, NativeWindowType Window);

    int (*CreatePixmap)(NativeDisplayType Display, int Width, int Height,
                        int BitsPerPixel, NativePixmapType *Pixmap);

    int (*GetPixmapInfo)(NativeDisplayType Display, NativePixmapType Pixmap,
                         int *Width, int *Height, int *BitsPerPixel,
                         int *Stride, void **Bits);

    int (*DestroyPixmap)(NativeDisplayType Display, NativePixmapType Pixmap);

};

static struct eglFbPlatform *fbBackend = NULL;
static struct fr_display *os_display = NULL;
static SemaphoreHandle_t mutex = NULL;
static int mutex_locked = 0;

static vg_lite_buffer_format_t video_format_to_vglite(video_pixel_format_t format)
{
    vg_lite_buffer_format_t fmt;
    switch (format)
    {
        case kVIDEO_PixelFormatRGB565:
            fmt = VG_LITE_BGR565;
            break;

        case kVIDEO_PixelFormatBGR565:
            fmt = VG_LITE_RGB565;
            break;

        case kVIDEO_PixelFormatXRGB8888:
            fmt = VG_LITE_BGRX8888;
            break;

        default:
            fmt = VG_LITE_RGB565;
            break;
    }

    return fmt;
}

int GetDisplayByIndex(int display_id, NativeDisplayType *display_type)
{
    /* We support only one display instance. Once initialized, return a pointer
     * to the global display instance and increase its reference count.
     */
    if (os_display)
    {
        *display_type = os_display;
        os_display->refCount++;
        return 0;
    }

    os_display = (struct fr_display*)malloc(sizeof(struct fr_display));
    if (os_display == NULL)
        return -1;

    memset(os_display, 0, sizeof(struct fr_display));
    os_display->refCount++;

    BOARD_PrepareDisplayController();
    FBDEV_Open(&os_display->g_fbdev, &g_dc, 0);
    *display_type = os_display;

    return 0;
}

int GetDisplay(NativeDisplayType *display)
{
    return GetDisplayByIndex(0, display);
}

int GetDisplayInfo(NativeDisplayType display, int *width, int *height,
                         size_t *physical, int *stride, int *bitsPerPixel)
{
    struct fr_display *disp = (struct fr_display*)display;
    fbdev_fb_info_t *g_fbInfo = &(disp->g_fbInfo);

    if (width)
        *width = g_fbInfo->bufInfo.width;

    if (height)
        *height = g_fbInfo->bufInfo.height;

    if (physical)
        *physical = 0;

    if (stride)
        *stride = g_fbInfo->bufInfo.strideBytes;

    if (bitsPerPixel)
        *bitsPerPixel = VIDEO_GetPixelSizeBits(g_fbInfo->bufInfo.pixelFormat);

    return 0;
}

/* All calls to GetDisplay/GetDisplayByIndex return pointers to the same
 * struct fr_display instance (os_display). This can result to multiple
 * pointers indicating to the same memory region, and an increased reference
 * counter. Avoid double frees by tearing down and freeing the display only
 * once the reference counter reaches zero.
 */
int DestroyDisplay(NativeDisplayType display)
{
    struct fr_display *disp = (struct fr_display*)display;

    if (!disp)
        return 0;

    disp->refCount--;

    if (disp->refCount <= 0)
    {
        if (disp->g_fbdev.dc)
        {
            FBDEV_Disable(&disp->g_fbdev);
            FBDEV_Close(&disp->g_fbdev);
        }

        if (disp == os_display)
            os_display = NULL;

        free(disp);
    }

    return 0;
}

int CreateWindow(NativeDisplayType display, int x, int y, int width,
                 int height, NativeWindowType *window)
{
    struct fr_display *disp = (struct fr_display*)display;
    fbdev_fb_info_t *g_fbInfo = &(disp->g_fbInfo);
    fbdev_t *g_fbdev = &(disp->g_fbdev);
    struct fr_window *win = NULL;
    vg_lite_buffer_t *vg_buffer;
    status_t status;
    void *buffer;

    win = (struct fr_window*)malloc(sizeof(struct fr_window));
    if (win == NULL)
        return -1;

    memset(win, 0, sizeof(struct fr_window));

    win->bufferCount = FREERTOS_EGL_BACKBUFFERS;
    win->display = disp;
    win->width = width;
    win->height = height;

    FBDEV_GetFrameBufferInfo(g_fbdev, g_fbInfo);

    g_fbInfo->bufInfo.startX = x;
    g_fbInfo->bufInfo.startY = y;
    g_fbInfo->bufInfo.width = win->width;
    g_fbInfo->bufInfo.height = win->height;
    g_fbInfo->bufInfo.pixelFormat = DEMO_BUFFER_PIXEL_FORMAT;
    g_fbInfo->bufInfo.strideBytes = win->width * DEMO_BUFFER_BYTE_PER_PIXEL;
    g_fbInfo->bufferCount = win->bufferCount;

    for (uint8_t i = 0; i < win->bufferCount; i++)
    {
        vg_buffer            = &(win->buffers[i]);
        g_fbInfo->buffers[i] = (void *)s_frameBufferAddress[i];
        vg_buffer->memory    = g_fbInfo->buffers[i];
        vg_buffer->address   = s_frameBufferAddress[i];
        vg_buffer->width     = g_fbInfo->bufInfo.width;
        vg_buffer->height    = g_fbInfo->bufInfo.height;
        vg_buffer->stride    = g_fbInfo->bufInfo.strideBytes;
        vg_buffer->format    = video_format_to_vglite(DEMO_BUFFER_PIXEL_FORMAT);
    }

    status = FBDEV_SetFrameBufferInfo(g_fbdev, g_fbInfo);
    if (status != kStatus_Success)
    {
        while (1)
            ;
    }

    buffer = FBDEV_GetFrameBuffer(g_fbdev, 0);

    assert(buffer != NULL);

    memset(buffer, 0, g_fbInfo->bufInfo.height * g_fbInfo->bufInfo.strideBytes);

    FBDEV_SetFrameBuffer(g_fbdev, buffer, 0);

    FBDEV_Enable(g_fbdev);

    *window = (NativeWindowType)win;
    return 0;
}

int GetWindowInfo(NativeDisplayType display, NativeWindowType window, int *x,
                  int *y, int *width, int *height, int *bitsPerPixel,
                  unsigned int *offset)
{
    struct fr_window *win = (struct fr_window*)window;
    fbdev_fb_info_t g_fbInfo;

    if (win == NULL)
    {
        printf("%s: NULL window\n", __func__);
        return -1;
    }

    if (width)
        *width = win->width;

    if (height)
        *height = win->height;

    if (offset)
        *offset = 0;

    if (!win->display) {
        printf("%s: window isn't associated with an initialized display\n",
               __func__);
        return -1;
    }

    g_fbInfo = win->display->g_fbInfo;

    if (x)
        *x = g_fbInfo.bufInfo.startX;

    if (y)
        *y = g_fbInfo.bufInfo.startY;

    if (bitsPerPixel)
        *bitsPerPixel = VIDEO_GetPixelSizeBits(g_fbInfo.bufInfo.pixelFormat);

    return 0;
}

int DestroyWindow(NativeDisplayType display, NativeWindowType window)
{
    if (window)
        free(window);

    return 0;
}

int CreatePixmap(NativeDisplayType display, int width, int height,
                 int bitsPerPixel, NativePixmapType *pixmap)
{
    vg_lite_buffer_t *imgbuf = NULL;
    struct fr_pixmap *pmap = NULL;
    VGImageFormat format;

    if (bitsPerPixel <= 16)
        format = VG_sRGB_565;
    else if (bitsPerPixel == 24)
        format = VG_sXRGB_8888;
    else
        format = VG_sARGB_8888;

    pmap = (struct fr_pixmap*)malloc(sizeof(struct fr_pixmap));
    if (!pmap)
        return -1;

    memset(pmap, 0, sizeof(struct fr_pixmap));

    imgbuf = (vg_lite_buffer_t*)malloc(sizeof(vg_lite_buffer_t));
    if (!imgbuf)
    {
        free(pmap);
        return -1;
    }

    memset(imgbuf, 0, sizeof(vg_lite_buffer_t));

    imgbuf->width = width;
    imgbuf->height = height;
    imgbuf->format = (vg_lite_buffer_format_t)format;

    if (vg_lite_allocate(imgbuf) != VG_LITE_SUCCESS)
    {
        free(imgbuf);
        free(pmap);
        return -1;
    }

    pmap->bits = imgbuf->memory;
    pmap->stride = imgbuf->stride;
    pmap->bpp = imgbuf->stride / imgbuf->width;
    pmap->imgbuf = imgbuf;

    *pixmap = (NativePixmapType)pmap;
    return 0;
}

static int GetPixmapInfo(NativeDisplayType display, NativePixmapType pixmap,
                         int *width, int *height, int *bitsPerPixel,
                         int *stride, void **bits)
{
    struct fr_pixmap *pmap = (struct fr_pixmap*)pixmap;

    if (pmap == NULL)
    {
        printf("%s: invalid pixmap\n", __func__);
        return -1;
    }

    if (width)
        *width = (int)pmap->imgbuf->width;

    if (height)
        *height = (int)pmap->imgbuf->height;

    if (bitsPerPixel)
        *bitsPerPixel = (int)pmap->bpp;

    if (stride)
        *stride = (int)pmap->stride;

    if (bits)
        *bits = pmap->bits;

    return 0;

}

static int DestroyPixmap(NativeDisplayType display, NativePixmapType pixmap)
{
    struct fr_pixmap *pmap = (struct fr_pixmap*)pixmap;

    if (pmap == NULL)
        return 0;

    if (pmap->imgbuf)
    {
        vg_lite_free(pmap->imgbuf);
        free(pmap->imgbuf);
    }

    free(pmap);
    pmap = NULL;
    pixmap = NULL;

    return 0;
}

static struct eglFbPlatform frBackend =
{
    GetDisplay,
    GetDisplayByIndex,
    GetDisplayInfo,
    DestroyDisplay,
    CreateWindow,
    GetWindowInfo,
    DestroyWindow,
    CreatePixmap,
    GetPixmapInfo,
    DestroyPixmap
};

static struct eglFbPlatform *getFbDevBackend()
{
    return &frBackend;
}

static void fbGetBackends(struct eglFbPlatform **fbBackend)
{
    if (*fbBackend == NULL)
    {
        *fbBackend = getFbDevBackend();
    }
}

void *fbGetDisplay(void *context)
{
    NativeDisplayType display = NULL;
    fbGetBackends(&fbBackend);
    fbBackend->GetDisplay(&display);
    return display;
}

void *fbGetDisplayByIndex(int DisplayIndex)
{
    NativeDisplayType display = NULL;
    fbGetBackends(&fbBackend);
    fbBackend->GetDisplayByIndex(DisplayIndex, &display);
    return display;
}

void fbGetDisplayGeometry(void *Display, int *Width, int *Height)
{
    fbGetBackends(&fbBackend);
    fbBackend->GetDisplayInfo((void *)Display, Width, Height, NULL, NULL, NULL);
}

void fbGetDisplayInfo(void *Display, int *Width, int *Height,
                      unsigned long *Physical, int *Stride, int *BitsPerPixel)
{
    fbGetBackends(&fbBackend);
    fbBackend->GetDisplayInfo(Display, Width, Height,(size_t*)Physical, Stride,
                              BitsPerPixel);
}

void fbDestroyDisplay(void *Display)
{
    fbGetBackends(&fbBackend);
    fbBackend->DestroyDisplay(Display);
}

void *fbCreateWindow(void *Display, int X, int Y, int Width, int Height)
{
    NativeWindowType window = NULL;
    fbGetBackends(&fbBackend);
    fbBackend->CreateWindow((NativeDisplayType)Display, X, Y, Width, Height,
                             &window);
    return window;
}

void
fbGetWindowGeometry(
    void *window,
    int *x,
    int *y,
    int *width,
    int *height
    )
{
    fbGetBackends(&fbBackend);
    fbBackend->GetWindowInfo(NULL, (void *)window, x, y, width, height, NULL,
                             NULL);
}

void
fbGetWindowInfo(
    void *window,
    int *x,
    int *y,
    int *width,
    int *height,
    int *bitsPerPixel,
    unsigned int *offset
    )
{
    fbGetBackends(&fbBackend);
    fbBackend->GetWindowInfo(NULL, (NativeWindowType)window, x, y, width,
                             height, bitsPerPixel, offset);
}

void fbDestroyWindow(void *Window)
{
    fbGetBackends(&fbBackend);
    fbBackend->DestroyWindow(NULL, (NativeWindowType)Window);
}

void *
fbCreatePixmap(
    void *display,
    int width,
    int height
    )
{
    NativePixmapType pixmap = NULL;
    fbGetBackends(&fbBackend);
    fbBackend->CreatePixmap((NativeDisplayType)display, width, height, 32,
                            &pixmap);
    return pixmap;
}

void *
fbCreatePixmapWithBpp(
    void *display,
    int width,
    int height,
    int bitsPerPixel
    )
{
    NativePixmapType pixmap = NULL;
    fbGetBackends(&fbBackend);
    fbBackend->CreatePixmap((NativeDisplayType)display, width, height,
                            bitsPerPixel, &pixmap);
    return pixmap;
}

void
fbGetPixmapGeometry(
    void *pixmap,
    int *width,
    int *height
    )
{
    fbGetBackends(&fbBackend);
    fbBackend->GetPixmapInfo(NULL, (NativePixmapType)pixmap, width, height,
                             NULL, NULL, NULL);
}

void
fbGetPixmapInfo(
    void *pixmap,
    int *width,
    int *height,
    int *bitsPerPixel,
    int *stride,
    void **bits
    )
{
    fbGetBackends(&fbBackend);
    fbBackend->GetPixmapInfo(NULL, (NativeDisplayType)pixmap, width, height,
                             bitsPerPixel, stride, bits);
}

void
fbDestroyPixmap(
    void *Pixmap
    )
{
    fbGetBackends(&fbBackend);
    fbBackend->DestroyPixmap(NULL, (void *)Pixmap);
}

void* OSGetCurrentThreadID(void)
{
    TaskStatus_t task;

    vTaskGetInfo(NULL, &task, pdFALSE, eRunning);
    return (void*) task.xTaskNumber;
}

void OSAcquireMutex(void)
{
    BaseType_t error;

    if (mutex == NULL)
    {
        mutex = xSemaphoreCreateRecursiveMutex();
        if (mutex == NULL)
            return;
        mutex_locked = 0;
    }

    error = xSemaphoreTakeRecursive(mutex, portMAX_DELAY);

    if (error == pdTRUE)
        mutex_locked += 1;
    else
        printf("%s: timeout\n", __func__);
}

void OSReleaseMutex(void)
{
    BaseType_t error;

    if (mutex == NULL)
    {
        return;
    }

    error = xSemaphoreGiveRecursive(mutex);

    if (error == pdTRUE)
        mutex_locked -= 1;
    else
        printf("%s: Trying to free a mutex not locked by itself\n", __func__);

    if (mutex_locked < 0)
        printf("%s: Trying to free an unlocked mutex\n", __func__);
}

void OSDeinitMutex(void)
{
    if (mutex == NULL)
        return;

    vSemaphoreDelete(mutex);
    mutex = NULL;
}

VGEGLDisplay* OSGetDisplay(NativeDisplayType display_id)
{
    struct fr_display *disp;
    VGEGLDisplay *vdisplay;
    int err;

    /* The VGEGLDisplay is freed by the OpenVG EGL layer using 'free'
     * in eglTerminate(). Use the same memory management API here.
     */
    vdisplay = (VGEGLDisplay*)malloc(sizeof(VGEGLDisplay));
    if (vdisplay == NULL)
        return NULL;

    err = GetDisplayByIndex(0, (NativeDisplayType*)&disp);
    if (err)
        goto out;

    memset(vdisplay, 0, sizeof(VGEGLDisplay));
    vdisplay->m_id = (NativeDisplayType)disp;
    vdisplay->nativeDisplay = (NativeDisplayType)disp;

    return vdisplay;

out:
    free(vdisplay);
    return NULL;
}

void OSDestroyDisplay(VGEGLDisplay *vdisplay)
{
    fbDestroyDisplay(vdisplay->nativeDisplay);
    free(vdisplay);
}

void* OSCreateWindowContext(NativeWindowType window)
{
    OSWindowContext *ctx = NULL;
    ctx = (OSWindowContext *)malloc(sizeof(OSWindowContext));
    if (ctx)
    {
        memset(ctx, 0, sizeof(OSWindowContext));
        ctx->window = (struct fr_window *)window;
        ctx->display = ctx->window->display;
    } else {
        printf("Create window context failed!\n");
    }

    return ctx;
}

void OSDestroyWindowContext(void *context)
{
    OSWindowContext *ctx = (OSWindowContext *)context;
    free(ctx);
}

VGboolean OSIsWindow(const void *context)
{
    OSWindowContext *ctx = (OSWindowContext*)context;

    if (ctx->window)
        return EGL_TRUE;
    else
        return EGL_FALSE;
}

void OSGetWindowSize(const void *context, int *width, int *height)
{
    OSWindowContext *ctx = (OSWindowContext*)context;

    if (!ctx || !ctx->window)
        return;

    *width = ctx->window->width;
    *height = ctx->window->height;
}

void OSBlitToWindow(void *context, const Drawable *drawable)
{
    vg_lite_buffer_t *vglbuf = drawable->m_color->m_image->m_vglbuf;
    OSWindowContext *ctx = (OSWindowContext*)context;
    struct fr_window *win = ctx->window;

    vgFinish();
    FBDEV_SetFrameBuffer(&win->display->g_fbdev, vglbuf->memory, 0);
}

VGuint OSGetPixmapInfo(NativePixmapType pixmap, VGuint *width, VGuint *height,
                       VGImageFormat *format, VGuint *bitsPerPixel,
                       VGuint *stride, VGubyte **bits)
{
    fbGetPixmapInfo(pixmap, (int*)width, (int*)height, (int*)bitsPerPixel,
                    (int*)stride, (void**)bits);
    return 0;
}
