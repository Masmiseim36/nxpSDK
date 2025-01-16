/****************************************************************************
*
*    Copyright 2022 Vivante Corporation, Santa Clara, California.
*    All Rights Reserved.
*
*    Permission is hereby granted, free of charge, to any person obtaining
*    a copy of this software and associated documentation files (the
*    'Software'), to deal in the Software without restriction, including
*    without limitation the rights to use, copy, modify, merge, publish,
*    distribute, sub license, and/or sell copies of the Software, and to
*    permit persons to whom the Software is furnished to do so, subject
*    to the following conditions:
*
*    The above copyright notice and this permission notice (including the
*    next paragraph) shall be included in all copies or substantial
*    portions of the Software.
*
*    THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
*    IN NO EVENT SHALL VIVANTE AND/OR ITS SUPPLIERS BE LIABLE FOR ANY
*    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
*    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
*    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/

/*------------------------------------------------------------------------
 * \brief   Based on Simple implementation of EGL 1.3
 * \note    caveats:
            - always renders into the backbuffer and blits it to window (no single buffered rendering)
            - no native Windows or Mac OS X pixmap support
            - no power management events
            - no support for swap interval
 * \todo    what happens in egl functions when eglTerminate has been called but the context and surface are still in use?
 * \todo    OSDeinitMutex should be called in case getEGL fails.
 *//*-------------------------------------------------------------------*/

#include "vg_context.h"

void* OSGetCurrentThreadID(void);
void OSAcquireMutex(void);
void OSReleaseMutex(void);
void OSDeinitMutex(void);

EGLDisplay OSGetDisplay(EGLNativeDisplayType display_id);
void OSDestroyDisplay(void* display);
void* OSCreateWindowContext(EGLNativeWindowType window);
void OSDestroyWindowContext(void* context);
VGboolean OSIsWindow(const void* context);
void OSGetWindowSize(const void* context, int* width, int* height);
void OSBlitToWindow(void* context, const Drawable* drawable);
VGuint OSGetPixmapInfo(EGLNativePixmapType pixmap, VGuint* width, VGuint* height, VGImageFormat* format, VGuint* bitsPerPixel, VGuint* stride, VGubyte** bits);

#define VIV_EGL_NUMTHREAD 64
#define VIV_EGL_NUMCURTHREAD 16
#define VIV_EGL_NUMDISPLAY 4

typedef struct
{
    VGEGLThread*        m_threads;            //threads that have called EGL
    VGEGLThread*        m_currentThreads;    //threads that have a bound context
    VGEGLDisplay*       m_displays;
    VGint               m_referenceCount;
} VGEGL;

static VGEGL g_egl = { 0 };    //never use this directly

static VGEGL* getEGL()
{
    g_egl.m_referenceCount++;
    return &g_egl;
}

void destroyVgEglDisplay(VGEGLDisplay* display);

static void releaseEGL()
{
    if (!g_egl.m_referenceCount)
    {
        //RI_DELETE(g_egl); Clean up g_egl lists.
        VGEGLDisplay* dpy;

        dpy = g_egl.m_displays;    
        while (dpy)
        {
            g_egl.m_displays = dpy->next;
            destroyVgEglDisplay(dpy);
            dpy = g_egl.m_displays;
        }
    }
}

static void destroyVgEglContext(VGEGLContext* vgeglctx)
{
    VG_ASSERT(vgeglctx->m_referenceCount == 0);
    destroyVgContext(vgeglctx->m_vgContext);
    free(vgeglctx);
}

static VG_INLINE void setEGLConfig(VGEGLConfig *vgEglConfig, int r, int g, int b, int a, int l, int bpp, int samples, int maskBits, int ID)
{
    vgEglConfig->m_desc.redBits = r;
    vgEglConfig->m_desc.greenBits = g;
    vgEglConfig->m_desc.blueBits = b;
    vgEglConfig->m_desc.alphaBits = a;
    vgEglConfig->m_desc.luminanceBits = l;
    vgEglConfig->m_desc.alphaShift = 0;
    vgEglConfig->m_desc.luminanceShift = 0;
    vgEglConfig->m_desc.blueShift = b ? a : 0;
    vgEglConfig->m_desc.greenShift = g ? a + b : 0;
    vgEglConfig->m_desc.redShift = r ? a + b + g : 0;
    vgEglConfig->m_desc.format = (VGImageFormat)-1;
    vgEglConfig->m_desc.internalFormat = l ? sLA : sRGBA;
    vgEglConfig->m_desc.bitsPerPixel = bpp;
    VG_ASSERT(isValidDescriptor(&vgEglConfig->m_desc));
    vgEglConfig->m_samples = samples;
    vgEglConfig->m_maskBits = maskBits;
    vgEglConfig->m_configID = ID;
    vgEglConfig->m_config = (EGLConfig)((size_t)ID);
    vgEglConfig->m_depth = 0;
    if (r == 8 && g == 8 && b == 8 && a == 8){
        vgEglConfig->m_desc.format = VG_sRGBA_8888;
    }
    else if (r == 8 && g == 8 && b == 8 && a == 0) {
        vgEglConfig->m_desc.format = VG_sRGBX_8888;
    }
    else if (r == 5 && g == 5 && b == 5 && a == 1) {
        vgEglConfig->m_desc.format = VG_sRGBA_5551;
    }
    else if (r == 5 && g == 6 && b == 5 && a == 0) {
        vgEglConfig->m_desc.format = VG_sRGB_565;
    }
    else if (r == 4 && g == 4 && b == 4 && a == 4) {
        vgEglConfig->m_desc.format = VG_sRGBA_4444;
    }

    if (!l)
        vgEglConfig->m_bufferType = EGL_RGB_BUFFER;
    else
        vgEglConfig->m_bufferType = EGL_LUMINANCE_BUFFER;

    vgEglConfig->m_renderType = EGL_OPENVG_BIT;
    vgEglConfig->m_maskAlpha = 8;
}

static VG_INLINE ColorDescriptor configToDescriptor(VGEGLConfig* vgEglConfig, VGboolean sRGB, VGboolean premultiplied)
{
    ColorDescriptor desc = vgEglConfig->m_desc;
    VGuint f = vgEglConfig->m_desc.luminanceBits ? LUMINANCE : 0;
    f |= sRGB ? NONLINEAR : 0;
    f |= premultiplied ? PREMULTIPLIED : 0;
    desc.internalFormat = (InternalFormat)f;
    if (desc.format == VG_sRGBA_8888) {
        if (premultiplied)
            desc.format++;
        if (!sRGB)
            desc.format += 7;   
    }
    else if (desc.format == VG_sRGBX_8888) {
        if (premultiplied) {
            if (!sRGB)
                desc.format = OPENVG_lRGBX_8888_PRE;
            else
                desc.format = OPENVG_sRGBX_8888_PRE;
        }
        else if (!sRGB)
            desc.format = OPENVG_lRGBX_8888;
    }
    else if (desc.format == VG_sRGBA_5551) {
        if (premultiplied) {
            if (!sRGB)
                desc.format = OPENVG_lRGBA_5551_PRE;
            else
                desc.format = OPENVG_sRGBA_5551_PRE;
        }
        else {
            if (!sRGB) {
                desc.format = OPENVG_lRGBA_5551;
            }
        }
    }
    else if (desc.format == VG_sRGBA_4444) {
        if (premultiplied) {
            if (!sRGB)
                desc.format = OPENVG_lRGBA_4444_PRE;
            else
                desc.format = OPENVG_sRGBA_4444_PRE;
        }
        else {
            if (!sRGB) {
                desc.format = OPENVG_lRGBA_4444;
            }
        }
    }
    else if (desc.format == VG_sRGB_565) {
        if (premultiplied) {
            if (!sRGB)
                desc.format = OPENVG_lRGB_565_PRE;
            else
                desc.format = OPENVG_sRGB_565_PRE;
        }
        else {
            if (!sRGB)
                desc.format = OPENVG_lRGB_565;
        }
    }
    return desc;
}

VGEGLSurface* createVgEglSurface(void* OSWindowContext, const EGLConfig config, Drawable* drawable[], VGuint drawableCount, VGboolean largestPbuffer, VGint renderBuffer)
{
    VG_ASSERT(renderBuffer == EGL_BACK_BUFFER);   //only back buffer rendering is supported

    VGEGLSurface* surf = (VGEGLSurface*)malloc(sizeof(VGEGLSurface));
    VGuint count = 0;
    if (surf)
    {
        memset(surf, 0, sizeof(VGEGLSurface));

        surf->m_OSWindowContext = OSWindowContext;
        surf->m_config = config;

        for (count = 0; count < drawableCount; count++)
        {
            surf->m_drawable[count] = drawable[count];
            surf->m_drawable[count]->m_referenceCount++;
        }

        surf->m_drawableCount = drawableCount;
        surf->m_drawableIndex = 0;
        surf->m_largestPbuffer = largestPbuffer;
        surf->m_renderBuffer = renderBuffer;
    }
    else
    {
        // Out of memory
        return NULL;
    }

    return surf;
}

void destroyVgEglSurface(VGEGLSurface* surf)
{
    VG_ASSERT(surf->m_referenceCount == 0);
    OSDestroyWindowContext(surf->m_OSWindowContext);
    for (VGuint i = 0; i < surf->m_drawableCount; i++)
    {
        if (surf->m_drawable[i])
        {
            surf->m_drawable[i]->m_referenceCount--;
            VG_ASSERT(surf->m_drawable[i]->m_referenceCount >= 0);
            if (!surf->m_drawable[i]->m_referenceCount)
                destroyDrawable(surf->m_drawable[i]);
        }
    }
    free(surf);
}

static VG_INLINE VGEGLConfig* getDisplayConfig(VGEGLDisplay* dpy, int i)
{ 
    VG_ASSERT(i >= 0 && i < VIV_EGL_NUMCONFIGS);
    return &dpy->m_configs[i];
}

static VG_INLINE VGEGLConfig* findDisplayConfig(VGEGLDisplay* dpy, const EGLConfig config)
{
    for (int i = 0; i < VIV_EGL_NUMCONFIGS; i++)
    {
        if (dpy->m_configs[i].m_config == config)
            return &dpy->m_configs[i];
    }
    VG_ASSERT(0);
    return &dpy->m_configs[0];
}

void initEGLDisplayConfigs(VGEGLDisplay *dpy)
{
    //sorted by RGB/LUMINANCE (exact), larger total number of color bits (at least), buffer size (at least), config ID (exact)
    //NOTE: 16 bit configs need to be sorted on the fly if the request ignores some channels
    //NOTE: config IDs start from 1
    //               R  B  G  A  L  bpp samples maskBits ID
    setEGLConfig(&dpy->m_configs[0], 8, 8, 8, 8, 0, 32, 1, 8, 1);    //EGL_RGB_BUFFER, buffer size = 32
    setEGLConfig(&dpy->m_configs[1], 8, 8, 8, 0, 0, 32, 1, 8, 2);    //EGL_RGB_BUFFER, buffer size = 24
    setEGLConfig(&dpy->m_configs[2], 5, 5, 5, 1, 0, 16, 1, 4, 3);    //EGL_RGB_BUFFER, buffer size = 16
    setEGLConfig(&dpy->m_configs[3], 5, 6, 5, 0, 0, 16, 1, 4, 4);    //EGL_RGB_BUFFER, buffer size = 16
    setEGLConfig(&dpy->m_configs[4], 4, 4, 4, 4, 0, 16, 1, 4, 5);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[5], 0, 0, 0, 8, 0, 8,  1, 8, 6);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[6], 0, 0, 0, 4, 0, 4,  1, 4, 7);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[7], 0, 0, 0, 1, 0, 1,  1, 1, 8);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[8], 0, 0, 0, 0, 8, 8,  1, 8, 9);    //EGL_LUMINANCE_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[9], 0, 0, 0, 0, 1, 1,  1, 1, 10);    //EGL_LUMINANCE_BUFFER, buffer size = 1

    //setEGLConfig(&dpy->m_configs[10], 8, 8, 8, 8, 0, 32, 4, 1, 11);    //EGL_RGB_BUFFER, buffer size = 32
    //setEGLConfig(&dpy->m_configs[11], 8, 8, 8, 0, 0, 32, 4, 1, 12);    //EGL_RGB_BUFFER, buffer size = 24
    //setEGLConfig(&dpy->m_configs[12], 5, 5, 5, 1, 0, 16, 4, 1, 13);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[13], 5, 6, 5, 0, 0, 16, 4, 1, 14);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[14], 4, 4, 4, 4, 0, 16, 4, 1, 15);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[15], 0, 0, 0, 8, 0, 8,  4, 1, 16);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[16], 0, 0, 0, 4, 0, 4,  4, 1, 17);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[17], 0, 0, 0, 1, 0, 1,  4, 1, 18);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[18], 0, 0, 0, 0, 8, 8,  4, 1, 19);    //EGL_LUMINANCE_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[19], 0, 0, 0, 0, 1, 1,  4, 1, 20);    //EGL_LUMINANCE_BUFFER, buffer size = 1

    //setEGLConfig(&dpy->m_configs[20], 8, 8, 8, 8, 0, 32, 32, 1, 21);    //EGL_RGB_BUFFER, buffer size = 32
    //setEGLConfig(&dpy->m_configs[21], 8, 8, 8, 0, 0, 32, 32, 1, 22);    //EGL_RGB_BUFFER, buffer size = 24
    //setEGLConfig(&dpy->m_configs[22], 5, 5, 5, 1, 0, 16, 32, 1, 23);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[23], 5, 6, 5, 0, 0, 16, 32, 1, 24);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[24], 4, 4, 4, 4, 0, 16, 32, 1, 25);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[25], 0, 0, 0, 8, 0, 8,  32, 1, 26);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[26], 0, 0, 0, 4, 0, 4,  32, 1, 27);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[27], 0, 0, 0, 1, 0, 1,  32, 1, 28);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[28], 0, 0, 0, 0, 8, 8,  32, 1, 29);    //EGL_LUMINANCE_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[29], 0, 0, 0, 0, 1, 1,  32, 1, 30);    //EGL_LUMINANCE_BUFFER, buffer size = 1

    ////configs without mask
    //setEGLConfig(&dpy->m_configs[30], 8, 8, 8, 8, 0, 32, 1, 0, 31);    //EGL_RGB_BUFFER, buffer size = 32
    //setEGLConfig(&dpy->m_configs[31], 8, 8, 8, 0, 0, 32, 1, 0, 32);    //EGL_RGB_BUFFER, buffer size = 24
    //setEGLConfig(&dpy->m_configs[32], 5, 5, 5, 1, 0, 16, 1, 0, 33);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[33], 5, 6, 5, 0, 0, 16, 1, 0, 34);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[34], 4, 4, 4, 4, 0, 16, 1, 0, 35);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[35], 0, 0, 0, 8, 0, 8,  1, 0, 36);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[36], 0, 0, 0, 4, 0, 4,  1, 0, 37);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[37], 0, 0, 0, 1, 0, 1,  1, 0, 38);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[38], 0, 0, 0, 0, 8, 8,  1, 0, 39);    //EGL_LUMINANCE_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[39], 0, 0, 0, 0, 1, 1,  1, 0, 40);    //EGL_LUMINANCE_BUFFER, buffer size = 1

    //setEGLConfig(&dpy->m_configs[40], 8, 8, 8, 8, 0, 32, 4, 0, 41);    //EGL_RGB_BUFFER, buffer size = 32
    //setEGLConfig(&dpy->m_configs[41], 8, 8, 8, 0, 0, 32, 4, 0, 42);    //EGL_RGB_BUFFER, buffer size = 24
    //setEGLConfig(&dpy->m_configs[42], 5, 5, 5, 1, 0, 16, 4, 0, 43);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[43], 5, 6, 5, 0, 0, 16, 4, 0, 44);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[44], 4, 4, 4, 4, 0, 16, 4, 0, 45);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[45], 0, 0, 0, 8, 0, 8,  4, 0, 46);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[46], 0, 0, 0, 4, 0, 4,  4, 0, 47);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[47], 0, 0, 0, 1, 0, 1,  4, 0, 48);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[48], 0, 0, 0, 0, 8, 8,  4, 0, 49);    //EGL_LUMINANCE_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[49], 0, 0, 0, 0, 1, 1,  4, 0, 50);    //EGL_LUMINANCE_BUFFER, buffer size = 1

    //setEGLConfig(&dpy->m_configs[50], 8, 8, 8, 8, 0, 32, 32, 0, 51);    //EGL_RGB_BUFFER, buffer size = 32
    //setEGLConfig(&dpy->m_configs[51], 8, 8, 8, 0, 0, 32, 32, 0, 52);    //EGL_RGB_BUFFER, buffer size = 24
    //setEGLConfig(&dpy->m_configs[52], 5, 5, 5, 1, 0, 16, 32, 0, 53);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[53], 5, 6, 5, 0, 0, 16, 32, 0, 54);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[54], 4, 4, 4, 4, 0, 16, 32, 0, 55);    //EGL_RGB_BUFFER, buffer size = 16
    //setEGLConfig(&dpy->m_configs[55], 0, 0, 0, 8, 0, 8,  32, 0, 56);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[56], 0, 0, 0, 4, 0, 4,  32, 0, 57);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[57], 0, 0, 0, 1, 0, 1,  32, 0, 58);    //EGL_RGB_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[58], 0, 0, 0, 0, 8, 8,  32, 0, 59);    //EGL_LUMINANCE_BUFFER, buffer size = 8
    //setEGLConfig(&dpy->m_configs[59], 0, 0, 0, 0, 1, 1,  32, 0, 60);    //EGL_LUMINANCE_BUFFER, buffer size = 1
    /*
    attrib                default        criteria order   priority
    --------------------------------------------------------------
    EGL_COLOR_BUFFER_TYPE EGL_RGB_BUFFER Exact    None    2 
    EGL_RED_SIZE          0              AtLeast  Special 3 
    EGL_GREEN_SIZE        0              AtLeast  Special 3 
    EGL_BLUE_SIZE         0              AtLeast  Special 3 
    EGL_LUMINANCE_SIZE    0              AtLeast  Special 3 
    EGL_ALPHA_SIZE        0              AtLeast  Special 3 
    EGL_BUFFER_SIZE       0              AtLeast  Smaller 4 
    EGL_CONFIG_ID         EGL_DONT_CARE  Exact    Smaller 11
    */
}

EGLBoolean contextExists(VGEGLDisplay *display, const EGLContext ctx)
{
    VGEGLContext *ctxptr = display->m_contexts;
    while (ctxptr)
    {
        if (ctxptr == ctx) return EGL_TRUE;
        ctxptr = ctxptr->next;
    }

    return EGL_FALSE;
}

EGLBoolean surfaceExists(VGEGLDisplay* display, const EGLSurface surf)
{
    VGEGLSurface* surptr = display->m_surfaces;
    while (surptr)
    {
        if (surptr == surf) return EGL_TRUE;
        surptr = surptr->next;
    }

    return EGL_FALSE;
}

void removeSurface(VGEGLDisplay* display, VGEGLSurface* surf)
{
    VGEGLSurface* surptr = display->m_surfaces;
    VGEGLSurface* preptr = NULL;

    while (surptr)
    {
        if (surptr == surf)
        {
            if (preptr)
                preptr->next = surptr->next;
            else
                display->m_surfaces = surptr->next;
            break;
        }

        preptr = surptr;
        surptr = surptr->next;
    }
}

void removeContext(VGEGLDisplay* display, VGEGLContext* ctx)
{
    VGEGLContext* curptr = display->m_contexts;
    VGEGLContext* preptr = NULL;

    while (curptr)
    {
        if (curptr == ctx)
        {
            if (preptr)
                preptr->next = curptr->next;
            else
                display->m_contexts = curptr->next;
            break;
        }

        preptr = curptr;
        curptr = curptr->next;
    }
}

EGLBoolean configExists(VGEGLDisplay* display, const EGLConfig config)
{
    for (int i = 0; i < VIV_EGL_NUMCONFIGS; i++)
    {
        if (display->m_configs[i].m_config == config)
        return EGL_TRUE;
    }

    return EGL_FALSE;
}

void destroyVgEglDisplay(VGEGLDisplay* display)
{
    VGEGLContext* ctxptr = display->m_contexts;
    VGEGLSurface* surptr = display->m_surfaces;

    //mark everything for deletion, but don't delete the current context and surface
    while (ctxptr)
    {
        ctxptr->m_referenceCount--; VG_ASSERT(ctxptr->m_referenceCount >= 0);
        if (!ctxptr->m_referenceCount)
        {
            display->m_contexts = ctxptr->next;
            destroyVgEglContext(ctxptr);
            ctxptr = display->m_contexts;
        }
        else
        {
            display->m_contexts = ctxptr->next;
            // Just remove the current context from the list (makes further references to the current contexts invalid)
            ctxptr = display->m_contexts;
        }
    }

    while (surptr)
    {
        surptr->m_referenceCount--; VG_ASSERT(surptr->m_referenceCount >= 0);
        if (!surptr->m_referenceCount)
        {
            display->m_surfaces = surptr->next;
            destroyVgEglSurface(surptr);
            surptr = display->m_surfaces;
        }
        else
        {
            display->m_surfaces = surptr->next;
            // Just remove the current surfaces from the list (makes further references to the current surfaces invalid)
            surptr = display->m_surfaces;
        }
    }
}

/*-------------------------------------------------------------------*//*!
* \brief     Given a display ID, return the corresponding object, or NULL
*            if the ID hasn't been initialized.
* \param    
* \return   
* \note      if egl has been initialized for this display, the display ID can
*            be found from egl->m_displays
*//*-------------------------------------------------------------------*/

void addEglDisplay(VGEGLDisplay* display)
{
    VGEGL *egl = getEGL();
    VGEGLDisplay *dpy = egl->m_displays;

    while (dpy)
    {
        if (display == dpy) break;
        dpy = dpy->next;
    }

    if (!dpy)
    {
        display->next = egl->m_displays;
        egl->m_displays = display;
    }
}

/*-------------------------------------------------------------------*//*!
* \brief     Given a display ID, return the corresponding object, or NULL
*            if the ID hasn't been initialized.
* \param    
* \return   
* \note      if egl has been initialized for this display, the display ID can
*            be found from egl->m_displays
*//*-------------------------------------------------------------------*/

VGEGLDisplay* getEglDisplay(EGLDisplay display)
{
    VGEGL* egl = getEGL();
    VGEGLDisplay *dpy = egl->m_displays;
    while (dpy)
    {
        if (display == dpy) return dpy;
        dpy = dpy->next;
    }

    return NULL; //error: the display hasn't been eglInitialized
}

EGLDisplay findEglDisplay(EGLContext ctx)
{
    VGEGL* egl = getEGL();
    VGEGLDisplay* dpy = egl->m_displays;
    while (dpy)
    {
        if (contextExists(dpy, ctx))
            return dpy->m_id;
        dpy = dpy->next;
    }

    return EGL_NO_DISPLAY;
}

void removeEglDisplay(EGLDisplay display)
{
    VGEGL* egl = getEGL();
    VGEGLDisplay* dpyptr = egl->m_displays;
    VGEGLDisplay* preptr = NULL;

    while (dpyptr)
    {
        if (display == dpyptr)
        {
            if (preptr)
                preptr = dpyptr->next;
            else
                egl->m_displays = dpyptr->next;
            break;
        }
        preptr = dpyptr;
        dpyptr = dpyptr->next;
    }

    OSDestroyDisplay(display);
}

VGEGLThread* getEglThread()
{
    VGEGL* egl = getEGL();
    VGEGLThread* thrptr = egl->m_threads;
    void* currentThreadID = OSGetCurrentThreadID();
    VGEGLThread* newThread = NULL;

    while (thrptr)
    {
        if (currentThreadID == thrptr->m_threadID)
            return thrptr;
        thrptr = thrptr->next;
    }

    //EGL doesn't have a struct for the thread yet, add it to EGL's m_threads list
    newThread = (VGEGLThread*)malloc(sizeof(VGEGLThread));
    if (newThread)
    {
        memset(newThread, 0, sizeof(VGEGLThread));
        newThread->m_threadID = currentThreadID;
        newThread->m_boundAPI = EGL_OPENVG_API;

        newThread->next = egl->m_threads;
        egl->m_threads = newThread;

        return newThread;
    }
    else
    {
        return NULL;
    }
}

void destroyEglThreads()
{
    VGEGL* egl = getEGL();
    VGEGLThread* thrptr = egl->m_threads;

    while (thrptr)
    {
        egl->m_threads = thrptr->next;
        free(thrptr);
        thrptr = egl->m_threads;
    }
}

VGEGLThread* getEglCurrentThread()
{
    VGEGL* egl = getEGL();
    VGEGLThread* thrptr = egl->m_currentThreads;
    void* currentThreadID = OSGetCurrentThreadID();

    while (thrptr)
    {
        if (currentThreadID == thrptr->m_threadID) return thrptr;
        thrptr = thrptr->next;
    }

    // Thread is not current
    return NULL;
}

void addEglCurrentThread(VGEGLThread * thread)
{
    VGEGL* egl = getEGL();
    VGEGLThread* thrptr = egl->m_currentThreads;

    while (thrptr)
    {
        if (thread == thrptr) return;
        thrptr = thrptr->next;
    }

    thread->next = egl->m_currentThreads;
    egl->m_currentThreads = thread;
}

void removeEglCurrentThread(VGEGLThread* thread)
{
    VGEGL* egl = getEGL();
    VGEGLThread* thrptr = egl->m_currentThreads;
    VGEGLThread* prethr = NULL;

    while (thrptr)
    {
        if (thread == thrptr)
        {
            if (prethr)
                prethr = thrptr->next;
            else
                egl->m_currentThreads = thrptr->next;
            break;
        }
        prethr = thrptr;
        thrptr = thrptr->next;
    }
}

VGboolean isDrawableInUse(Drawable* draw)
{
    if (draw->m_color->m_image->m_inUse)
        return VG_TRUE;

    if (draw->m_mask && draw->m_mask->m_image->m_inUse)
        return VG_TRUE;

    return VG_FALSE;
}

VGboolean isImageInUse(Drawable* draw, Image* image)
{
    if (draw->m_color->m_image == image)
        return VG_TRUE;

    if (draw->m_mask && draw->m_mask->m_image == image)
        return VG_TRUE;

    return VG_FALSE;
}

/*-------------------------------------------------------------------*//*!
* \brief    Check if the image is current in any of the displays
* \param
* \return
* \note     This function is always called from a mutexed API function
*//*-------------------------------------------------------------------*/

VGboolean isEglVgInUse(void* image)
{
    VGEGL* egl = getEGL();
    VGEGLThread* thrptr = egl->m_currentThreads;

    while (thrptr)
    {
        VGEGLSurface* s = thrptr->m_surface;
        for (VGuint i = 0; i < s->m_drawableCount; i++)
        {
            if (s && s->m_drawable[i] && isImageInUse(s->m_drawable[i], (Image*)image))
            {
                return VG_TRUE;
            }
        }

        thrptr = thrptr->next;
    }

    return VG_FALSE;
}

VGboolean isPixmapInUse(EGLNativePixmapType pixmap )
{
    VGEGL* egl = getEGL();
    VGEGLSurface * surface = egl->m_displays->m_surfaces;
    unsigned int width, height, stride, bitsPerPixel;
    VGubyte * bits;
    VGImageFormat format = {0};

    OSGetPixmapInfo(pixmap, &width, &height, &format, &bitsPerPixel, &stride, &bits);
    for (;surface != NULL;)
    {
        Image* img = surface->m_drawable[0]->m_color->m_image;
        if (img->m_data == bits   || img->m_height == height      ||
            img->m_width == width || img->m_desc.format == format ||
            img->m_stride == stride
            )
        {
           return VG_TRUE;
        }

        if (surface == surface->next)
        {
            break;
        }
        else
        {
            surface = surface->next;
        }
    }

    return VG_FALSE;

}

VGboolean isEglSurfaceBind(VGEGLSurface * Surface)
{
    VGEGL * egl = getEGL();
    VGEGLThread *vthread = egl->m_currentThreads;

    for (;vthread;)
    {
        if (vthread->m_surface == Surface)
            return VG_TRUE;
        if (vthread->next)
            vthread = vthread->next;
        else
            break;
    }

    return VG_FALSE;
}

VGboolean isEglContextBind(VGEGLContext *Context)
{
    VGEGL * egl = getEGL();
    VGEGLThread *vthread = egl->m_currentThreads;

    for (;vthread;)
    {
        if (vthread->m_context == Context)
            return VG_TRUE;
        if (vthread->next)
            vthread = vthread->next;
        else
            break;
    }

    return VG_FALSE;

}


#define EGL_GET_DISPLAY(DISPLAY, RETVAL) \
    OSAcquireMutex(); \
    VGEGL* egl = getEGL(); \
    if (!egl) \
    { \
        OSReleaseMutex(); \
        return RETVAL; \
    } \
    VGEGLDisplay* display = getEglDisplay(DISPLAY);

#define EGL_GET_EGL(RETVAL) \
    OSAcquireMutex(); \
    VGEGL* egl = getEGL(); \
    if (!egl) \
    { \
        OSReleaseMutex(); \
        return RETVAL; \
    } \

#define EGL_IF_ERROR(COND, ERRORCODE, RETVAL) \
    if (COND) { eglSetError(egl, ERRORCODE); OSReleaseMutex(); return RETVAL; } \

#define EGL_RETURN(ERRORCODE, RETVAL) \
    { \
        eglSetError(egl, ERRORCODE); \
        OSReleaseMutex(); \
        return RETVAL; \
    }


static void eglSetError(VGEGL* egl, EGLint error)
{
    VGEGLThread* thread = getEglThread();

    // egl error handling model differs from OpenVG. The latest error is stored instead of the oldest one.
    if (thread)
        thread->m_error = error;
}

/*-------------------------------------------------------------------*//*!
* \brief    Returns the OpenVG context current to the calling thread.
* \param    
* \return    
* \note        This function is always called from a mutexed API function
*//*-------------------------------------------------------------------*/

void* getCurrentEglVgContext(void)
{
    VGEGL* egl = getEGL();

    if (egl)
    {
        VGEGLThread* thread = getEglCurrentThread();
        if (thread)
        {
            VG_ASSERT(thread->m_context && thread->m_surface);
            return thread->m_context->m_vgContext;
        }
    }

    return NULL;    //not initialized or made current
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLint EGLAPIENTRY eglGetError()
{
    OSAcquireMutex();
    EGLint ret = EGL_SUCCESS;
    VGEGL* egl = getEGL();

    if (egl)
    {
        VGEGLThread* thread = getEglThread();
        if (thread)
            ret = thread->m_error;    //initialized, return error code
    }
    else
        ret = EGL_NOT_INITIALIZED;

    OSReleaseMutex();
    return ret;
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLDisplay EGLAPIENTRY eglGetDisplay(EGLNativeDisplayType display_id)
{
    EGL_GET_EGL(EGL_FALSE);
    //Currently, we suppose display_id is a valid display device context handle, althought it can be a NULL value.
    //EGL_IF_ERROR((display_id == NULL), EGL_BAD_DISPLAY, EGL_FALSE);

    //create the current display
    //if a context and a surface are bound by the time of eglTerminate, they remain bound until eglMakeCurrent is called
    VGEGLDisplay* newDisplay = NULL;

    newDisplay = getEglDisplay(display_id);

    if (newDisplay == NULL)
    {
        newDisplay= OSGetDisplay(display_id);

        // init display eglconfigs
        initEGLDisplayConfigs(newDisplay);
        addEglDisplay(newDisplay);
    }

    EGL_RETURN(EGL_SUCCESS, (EGLDisplay)newDisplay);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor)
{
    EGL_GET_DISPLAY(dpy, EGL_FALSE);
    EGL_IF_ERROR(display, EGL_SUCCESS, EGL_TRUE);    //already initialized
  //  VGEGLThread * thread = getEglThread();

    if (major) *major = 1;
    if (minor) *minor = 2;
    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglTerminate(EGLDisplay dpy)
{
    EGL_GET_DISPLAY(dpy, EGL_FALSE);
    EGL_IF_ERROR(!display, EGL_SUCCESS, EGL_TRUE);

    vg_lite_close();

    removeEglDisplay(display);

    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI const char* EGLAPIENTRY eglQueryString(EGLDisplay dpy, EGLint name)
{
    EGL_GET_DISPLAY(dpy, NULL);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, NULL);

    static const char apis[] = "OpenVG";
    static const char extensions[] = "";
    static const char vendor[] = "Vivante";
    static const char version[] = "1.3";

    const char* ret = NULL;
    switch(name)
    {
    case EGL_CLIENT_APIS:
        ret = apis;
        break;

    case EGL_EXTENSIONS:
        ret = extensions;
        break;

    case EGL_VENDOR:
        ret = vendor;
        break;

    case EGL_VERSION:
        ret = version;
        break;

    default:
        EGL_RETURN(EGL_BAD_PARAMETER, NULL);
    }
    EGL_RETURN(EGL_SUCCESS, ret);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglGetConfigs(EGLDisplay dpy, EGLConfig* configs, EGLint config_size, EGLint* num_config)
{
    EGL_GET_DISPLAY(dpy, EGL_FALSE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
    EGL_IF_ERROR(!num_config, EGL_BAD_PARAMETER, EGL_FALSE);

    if (!configs)
    {
        *num_config = VIV_EGL_NUMCONFIGS;
        EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
    }
    *num_config = VG_INT_MIN(config_size, VIV_EGL_NUMCONFIGS);
    for (int i = 0; i < *num_config; i++)
        configs[i] = display->m_configs[i].m_config;

    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

static VGboolean smaller(EGLint c, EGLint filter)
{
    return (filter != EGL_DONT_CARE) && (c < filter);
}

EGLAPI EGLBoolean EGLAPIENTRY eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
    EGL_GET_DISPLAY(dpy, EGL_FALSE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
    EGL_IF_ERROR(!num_config, EGL_BAD_PARAMETER, EGL_FALSE);

    if (!configs)
    {
        *num_config = VIV_EGL_NUMCONFIGS;
        EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
    }
    *num_config = 0;
    if (!config_size)
        EGL_RETURN(EGL_SUCCESS, EGL_TRUE);

    int bufferSize = 0;
    int redSize = 0;
    int greenSize = 0;
    int blueSize = 0;
    int luminanceSize = 0;
    int alphaSize = 0;
    int colorBufferType = EGL_RGB_BUFFER;
    int configID = EGL_DONT_CARE;
    int sampleBuffers = 0;
    int samples = 0;
    if (attrib_list)
    {
        for (int i=0;attrib_list[i] != EGL_NONE;i+=2)
        {
            switch(attrib_list[i])
            {
            case EGL_BUFFER_SIZE:                //depth of the color buffer
                bufferSize = attrib_list[i+1];
                break;
            case EGL_RED_SIZE:                    //bits of Red in the color buffer
                redSize = attrib_list[i+1];
                break;
            case EGL_GREEN_SIZE:                //bits of Green in the color buffer
                greenSize = attrib_list[i+1];
                break;
            case EGL_BLUE_SIZE:                    //bits of Blue in the color buffer
                blueSize = attrib_list[i+1];
                break;
            case EGL_LUMINANCE_SIZE:            //bits of Luminance in the color buffer
                luminanceSize = attrib_list[i+1];
                break;
            case EGL_ALPHA_SIZE:                //bits of Alpha in the color buffer
                alphaSize = attrib_list[i+1];
                break;
            case EGL_ALPHA_MASK_SIZE:            //bits of Alpha in the alpha mask buffer
                if (attrib_list[i+1] > 8)
                    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);    //not supported
                break;
            case EGL_COLOR_BUFFER_TYPE:            //enum color buffer type (EGL_RGB_BUFFER, EGL_LUMINANCE_BUFFER)
                EGL_IF_ERROR(attrib_list[i+1] != EGL_RGB_BUFFER && attrib_list[i+1] != EGL_LUMINANCE_BUFFER && attrib_list[i+1] != EGL_DONT_CARE, EGL_BAD_ATTRIBUTE, EGL_FALSE);
                colorBufferType = attrib_list[i+1];
                break;
            case EGL_CONFIG_ID:                    //unique EGLConfig identifier
                configID = attrib_list[i+1];
                break;

            case EGL_SAMPLE_BUFFERS:            //integer number of multisample buffers
                sampleBuffers = attrib_list[i+1];
                break;
            case EGL_SAMPLES:                    //integer number of samples per pixel
                samples = attrib_list[i+1];
                break;
            case EGL_DEPTH_SIZE:                //integer bits of Z in the depth buffer (ignore for OpenVG)
                break;
            case EGL_BIND_TO_TEXTURE_RGB:        //boolean True if bindable to RGB textures. (always EGL_FALSE)
            case EGL_BIND_TO_TEXTURE_RGBA:        //boolean True if bindable to RGBA textures. (always EGL_FALSE)
            case EGL_LEVEL:                        //integer frame buffer level (always 0)
            case EGL_NATIVE_RENDERABLE:            //boolean EGL TRUE if native rendering APIs can render to surface (always EGL_FALSE)
            case EGL_STENCIL_SIZE:                //integer bits of Stencil in the stencil buffer (always 0)
                if (attrib_list[i+1])
                    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);    //not supported
                break;

            case EGL_CONFIG_CAVEAT:                //enum any caveats for the configuration (always EGL_NONE)
            case EGL_NATIVE_VISUAL_TYPE:        //integer native visual type of the associated visual (always EGL_NONE)
                if (attrib_list[i+1] != EGL_NONE)
                    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);    //not supported
                break;

            case EGL_MAX_SWAP_INTERVAL:            //integer maximum swap interval (always 1)
            case EGL_MIN_SWAP_INTERVAL:            //integer minimum swap interval (always 1)
                if (attrib_list[i+1] != 1)
                    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);    //not supported
                break;

            case EGL_RENDERABLE_TYPE:            //bitmask which client rendering APIs are supported. (always EGL_OPENVG_BIT)
                if (!(attrib_list[i+1] & EGL_OPENVG_BIT))
                    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);    //not supported
                break;

            case EGL_SURFACE_TYPE:                //bitmask which types of EGL surfaces are supported. (always EGL_WINDOW_BIT | EGL_PIXMAP_BIT | EGL_PBUFFER_BIT | EGL_VG_COLORSPACE_LINEAR_BIT | EGL_VG_ALPHA_FORMAT_PRE_BIT)
                break;    //all types are always supported

            case EGL_TRANSPARENT_TYPE:            //enum type of transparency supported (always EGL_NONE)
            case EGL_NATIVE_VISUAL_ID:            //integer handle of corresponding native visual (always 0)
            case EGL_MAX_PBUFFER_WIDTH:            //integer maximum width of pbuffer (always INT_MAX)
            case EGL_MAX_PBUFFER_HEIGHT:        //integer maximum height of pbuffer (always INT_MAX)
            case EGL_MAX_PBUFFER_PIXELS:        //integer maximum size of pbuffer (always INT_MAX)
            case EGL_TRANSPARENT_RED_VALUE:        //integer transparent red value (undefined)
            case EGL_TRANSPARENT_GREEN_VALUE:    //integer transparent green value (undefined)
            case EGL_TRANSPARENT_BLUE_VALUE:    //integer transparent blue value (undefined)
                break;    //ignored

            default:
                EGL_RETURN(EGL_BAD_ATTRIBUTE, EGL_FALSE);    //unknown attribute
            }
        }
    }

    if (configID && configID != EGL_DONT_CARE)
    {    //if CONFIG_ID is defined, ignore the rest of the attribs
        for (int i=0; i<VIV_EGL_NUMCONFIGS; i++)
        {
            if (display->m_configs[i].m_configID == configID)
            {
                *num_config = 1;
                *configs = display->m_configs[i].m_config;
            }
        }
        EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
    }

    //go through all configs, add passed configs to return list
    //TODO take alpha mask size into account
    EGLConfig found[VIV_EGL_NUMCONFIGS];
    int keys[VIV_EGL_NUMCONFIGS];
    int numFound = 0;
    for (int i = 0; i < VIV_EGL_NUMCONFIGS; i++)
    {
        const VGEGLConfig* c = getDisplayConfig(display, i);

        int colorBits = c->m_desc.redBits + c->m_desc.greenBits + c->m_desc.blueBits;
        int luminanceBits = c->m_desc.luminanceBits;
        int configBufferSize;
        if (colorBits)
        {
            VG_ASSERT(!luminanceBits);
            colorBits += c->m_desc.alphaBits;
            configBufferSize = colorBits;
        }
        else if (luminanceBits)
        {
            luminanceBits += c->m_desc.alphaBits;
            configBufferSize = luminanceBits;
        }
        else
        {    //alpha only surface
            colorBits = c->m_desc.alphaBits;
            luminanceBits = c->m_desc.alphaBits;
            configBufferSize = colorBits;
        }

        if (smaller(configBufferSize, bufferSize))
            continue;

        int configSampleBuffers = c->m_samples == 1 ? 0 : 1;
        if (smaller(configSampleBuffers, sampleBuffers))
            continue;
        if (smaller(c->m_samples, samples))
            continue;

        if (smaller(c->m_desc.redBits, redSize)
            || smaller(c->m_desc.greenBits, greenSize)
            || smaller(c->m_desc.blueBits, blueSize) 
            || smaller(c->m_desc.alphaBits, alphaSize) )
            continue;

        if (smaller(c->m_desc.luminanceBits, luminanceSize))
            continue;

        if ((colorBufferType == EGL_RGB_BUFFER && !colorBits) ||
            (colorBufferType == EGL_LUMINANCE_BUFFER && !luminanceBits))
            continue;

        int sortKey = c->m_configID;    //sort from smaller to larger
        int sortBits = 0;
        if (redSize != 0 && redSize != EGL_DONT_CARE)
            sortBits += c->m_desc.redBits;
        if (greenSize != 0 && greenSize != EGL_DONT_CARE)
            sortBits += c->m_desc.greenBits;
        if (blueSize != 0 && blueSize != EGL_DONT_CARE)
            sortBits += c->m_desc.blueBits;
        if (alphaSize != 0 && alphaSize != EGL_DONT_CARE)
            sortBits += c->m_desc.alphaBits;
        if (luminanceSize != 0 && luminanceSize != EGL_DONT_CARE)
            sortBits += c->m_desc.luminanceBits;
        VG_ASSERT(c->m_configID <= VIV_EGL_NUMCONFIGS);    //if there are more configs, increase the shift value
        VG_ASSERT(sortBits <= 32);
        sortKey += (32-sortBits) << 4;    //sort from larger to smaller

        found[numFound] = c->m_config;
        keys[numFound++] = sortKey;
    }
    if (!numFound)
        EGL_RETURN(EGL_SUCCESS, EGL_TRUE);

    //sort return list into increasing order
    for (int e=0; e<numFound-1; e++)
    {
        for (int f=e+1; f<numFound; f++)
        {
            if (keys[e] > keys[f])
            {
                EGLConfig tmp = found[e];
                VGint tmpkey = keys[e];

                found[e] = found[f];
                found[f] = tmp;
                keys[e] = keys[f];
                keys[f] = tmpkey;
            }
        }
    }

    //write configs into return array
    numFound = (numFound < config_size) ? numFound : config_size;
    for (int i=0;i<numFound;i++)
    {
        configs[i] = found[i];
    }
    *num_config = numFound;

    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value)
{
    EGL_GET_DISPLAY(dpy, EGL_FALSE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
    EGL_IF_ERROR(!configExists(display, config), EGL_BAD_CONFIG, EGL_FALSE);
    VGEGLConfig* c = findDisplayConfig(display, config);

    switch(attribute)
    {
    case EGL_BUFFER_SIZE:
        *value = (c->m_desc.redBits + c->m_desc.greenBits + c->m_desc.blueBits + c->m_desc.alphaBits) > (c->m_desc.luminanceBits + c->m_desc.alphaBits) ? 
            (c->m_desc.redBits + c->m_desc.greenBits + c->m_desc.blueBits + c->m_desc.alphaBits) : (c->m_desc.luminanceBits + c->m_desc.alphaBits);
        break;

    case EGL_RED_SIZE:
        *value = c->m_desc.redBits;
        break;

    case EGL_GREEN_SIZE:
        *value = c->m_desc.greenBits;
        break;

    case EGL_BLUE_SIZE:
        *value = c->m_desc.blueBits;
        break;

    case EGL_LUMINANCE_SIZE:
        *value = c->m_desc.luminanceBits;
        break;

    case EGL_ALPHA_SIZE:
        *value = c->m_desc.alphaBits;
        break;

    case EGL_ALPHA_MASK_SIZE:
        *value = c->m_maskBits;
        break;

    case EGL_BIND_TO_TEXTURE_RGB:
    case EGL_BIND_TO_TEXTURE_RGBA:
        *value = EGL_FALSE;
        break;

    case EGL_COLOR_BUFFER_TYPE:
        if (c->m_desc.redBits)
            *value = EGL_RGB_BUFFER;
        else
            *value = EGL_LUMINANCE_BUFFER;
        break;

    case EGL_CONFIG_CAVEAT:
        *value = EGL_NONE;
        break;

    case EGL_CONFIG_ID:
        *value = c->m_configID;
        break;

    case EGL_DEPTH_SIZE:
        *value = 0;
        break;

    case EGL_LEVEL:
        *value = 0;
        break;

    case EGL_MAX_PBUFFER_WIDTH:
    case EGL_MAX_PBUFFER_HEIGHT:
        *value = 16384;            //NOTE arbitrary maximum
        break;
        
    case EGL_MAX_PBUFFER_PIXELS:
        *value = 16384*16384;    //NOTE arbitrary maximum
        break;

    case EGL_MAX_SWAP_INTERVAL:
    case EGL_MIN_SWAP_INTERVAL:
        *value = 1;
        break;

    case EGL_NATIVE_RENDERABLE:
        *value = EGL_FALSE;
        break;

    case EGL_NATIVE_VISUAL_ID:
        *value = 0;
        break;

    case EGL_NATIVE_VISUAL_TYPE:
        *value = EGL_NONE;
        break;

    case EGL_RENDERABLE_TYPE:
        *value = EGL_OPENVG_BIT;
        break;

    case EGL_SAMPLE_BUFFERS:
        *value = c->m_samples > 1 ? 1 : 0;
        break;

    case EGL_SAMPLES:
        *value = c->m_samples > 1 ? c->m_samples : 0;
        break;

    case EGL_STENCIL_SIZE:
        *value = 0;
        break;

    case EGL_SURFACE_TYPE:
        *value = EGL_WINDOW_BIT | EGL_PIXMAP_BIT | EGL_PBUFFER_BIT | EGL_VG_COLORSPACE_LINEAR_BIT | EGL_VG_ALPHA_FORMAT_PRE_BIT;
        break;

    case EGL_TRANSPARENT_TYPE:
        *value = EGL_NONE;
        break;

    case EGL_TRANSPARENT_RED_VALUE:
    case EGL_TRANSPARENT_GREEN_VALUE:
    case EGL_TRANSPARENT_BLUE_VALUE:
        *value = 0;
        break;

    case EGL_CONFORMANT:
        *value = EGL_OPENVG_BIT;  //TODO return proper value
        break;

    default:
        EGL_RETURN(EGL_BAD_ATTRIBUTE, EGL_FALSE);
    }
    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

#define OPENGL_VGLITE_CMDBUF_SIZE (256 << 10)

EGLAPI EGLSurface EGLAPIENTRY eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list)
{
    EGL_GET_DISPLAY(dpy, EGL_NO_SURFACE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_NO_SURFACE);
    EGL_IF_ERROR(!configExists(display, config), EGL_BAD_CONFIG, EGL_NO_SURFACE);

    int renderBuffer = EGL_BACK_BUFFER;
    int colorSpace = EGL_VG_COLORSPACE_sRGB;
    int alphaFormat = EGL_VG_ALPHA_FORMAT_NONPRE;
    if (attrib_list)
    {
        for (int i=0;attrib_list[i] != EGL_NONE;i+=2)
        {
            switch(attrib_list[i])
            {
            case EGL_RENDER_BUFFER:
                renderBuffer = attrib_list[i+1];
                break;

            case EGL_VG_COLORSPACE:
                colorSpace = attrib_list[i+1];
                break;

            case EGL_VG_ALPHA_FORMAT:
                alphaFormat = attrib_list[i+1];
                break;

            default:
                EGL_RETURN(EGL_BAD_ATTRIBUTE, EGL_NO_SURFACE);
            }
        }
    }
    //we ignore the renderBuffer parameter since we can only render to double buffered surfaces

    //TODO If the attributes of win do not correspond to config, then an EGL BAD MATCH error is generated.
    //TODO If there is already an EGLConfig associated with win (as a result of a previous eglCreateWindowSurface call), then an EGL BAD ALLOC error is generated
    VGEGLSurface* s = NULL;

    void* wc = NULL;
    Drawable* d[VG_FRAMEBUFFER_COUNT];
    int count = 0;

    {
        wc = OSCreateWindowContext(win);

        VG_ASSERT(wc);
        //TODO what should happen if window width or height is zero?
        int windowWidth = 0, windowHeight = 0;
        OSGetWindowSize(wc, &windowWidth, &windowHeight);
        VGboolean isWindow = OSIsWindow(wc);
        if (windowWidth <= 0 || windowHeight <= 0 || !isWindow)
        {
            OSDestroyWindowContext(wc);
            EGL_IF_ERROR(!isWindow, EGL_BAD_NATIVE_WINDOW, EGL_NO_SURFACE);
            EGL_IF_ERROR(windowWidth <= 0 || windowHeight <= 0, EGL_BAD_NATIVE_WINDOW, EGL_NO_SURFACE);
        }

        /* VGMark needs at least 256K command buffer size for VGLite */
        vg_lite_set_command_buffer_size(OPENGL_VGLITE_CMDBUF_SIZE);

        /* Initialize VGLite API here */
        if (vg_lite_init(windowWidth, windowHeight) != VG_LITE_SUCCESS)
        {
            printf("OpenVG driver fails to initialize VGLite API!\n");
            OSDestroyWindowContext(wc);
            EGL_RETURN(EGL_NOT_INITIALIZED, EGL_NO_SURFACE);
        }

        //d = RI_NEW(Drawable, (display->getConfig(config).configToDescriptor((colorSpace == EGL_VG_COLORSPACE_LINEAR) ? false : true, (alphaFormat == EGL_VG_ALPHA_FORMAT_PRE) ? true : false), windowWidth, windowHeight, display->getConfig(config).m_samples, display->getConfig(config).m_maskBits));    //throws bad_alloc
        VGEGLConfig* ec = findDisplayConfig(display, config);
        ColorDescriptor desc = configToDescriptor(ec, (colorSpace == EGL_VG_COLORSPACE_LINEAR) ? VG_FALSE : VG_TRUE,
            (alphaFormat == EGL_VG_ALPHA_FORMAT_PRE) ? VG_TRUE : VG_FALSE);

        for (count = 0; count < VG_FRAMEBUFFER_COUNT; count++)
        {
            d[count] = createDrawable(&desc, windowWidth, windowHeight, ec->m_samples, ec->m_maskBits);
        }

        if (d[0])
        {
            //s = RI_NEW(VGEGLSurface,(wc, config, d, false, renderBuffer));    //throws bad_alloc
            s = createVgEglSurface(wc, config, d, count, VG_FALSE, renderBuffer);
            if (s)
            {
                s->m_referenceCount++;

                //display->addSurface(s)
                s->next = display->m_surfaces;
                display->m_surfaces = s;
            }
            else
            {
                OSDestroyWindowContext(wc);
                for (count =0 ; count < VG_FRAMEBUFFER_COUNT; count++)
                {
                    destroyDrawable(d[count]);
                }
                EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_SURFACE);
            }
        }
        else
        {
            OSDestroyWindowContext(wc);
            EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_SURFACE);
        }
    }

    EGL_RETURN(EGL_SUCCESS, (EGLSurface)s);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLSurface EGLAPIENTRY eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list)
{
    EGL_GET_DISPLAY(dpy, EGL_NO_SURFACE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_NO_SURFACE);
    EGL_IF_ERROR(!configExists(display, config), EGL_BAD_CONFIG, EGL_NO_SURFACE);

    int width = 0, height = 0;
    VGboolean largestPbuffer = VG_FALSE;
    int colorSpace = EGL_VG_COLORSPACE_sRGB;
    int alphaFormat = EGL_VG_ALPHA_FORMAT_NONPRE;
    if (attrib_list)
    {
        for (int i=0;attrib_list[i] != EGL_NONE;i+=2)
        {
            switch(attrib_list[i])
            {
            case EGL_WIDTH:
                width = attrib_list[i+1];
                break;

            case EGL_HEIGHT:
                height = attrib_list[i+1];
                break;

            case EGL_LARGEST_PBUFFER:
                largestPbuffer = attrib_list[i+1] ? VG_TRUE : VG_FALSE;
                break;

            case EGL_VG_COLORSPACE:
                colorSpace = attrib_list[i+1];
                break;

            case EGL_VG_ALPHA_FORMAT:
                alphaFormat = attrib_list[i+1];
                break;

            case EGL_TEXTURE_FORMAT:    //config doesn't support OpenGL ES
            case EGL_TEXTURE_TARGET:    //config doesn't support OpenGL ES
            case EGL_MIPMAP_TEXTURE:    //config doesn't support OpenGL ES
            default:
                EGL_RETURN(EGL_BAD_ATTRIBUTE, EGL_NO_SURFACE);
            break;
            }
        }
    }
    EGL_IF_ERROR(width <= 0 || height <= 0, EGL_BAD_ATTRIBUTE, EGL_NO_SURFACE);
    VGEGLSurface* s = NULL;
    Drawable* d = NULL;

    /* VGMark needs at least 256K command buffer size for VGLite */
    vg_lite_set_command_buffer_size(OPENGL_VGLITE_CMDBUF_SIZE);

    /* Initialize VGLite API here */
    if (vg_lite_init(width, height) != VG_LITE_SUCCESS)
    {
        printf("OpenVG driver fails to initialize VGLite API!\n");
        EGL_RETURN(EGL_NOT_INITIALIZED, EGL_NO_SURFACE);
    }

    {
        //d = RI_NEW(Drawable, (display->getConfig(config).configToDescriptor((colorSpace == EGL_VG_COLORSPACE_LINEAR) ? false : true, (alphaFormat == EGL_VG_ALPHA_FORMAT_PRE) ? true : false), width, height, display->getConfig(config).m_samples, display->getConfig(config).m_maskBits));    //throws bad_alloc
        VGEGLConfig* ec = findDisplayConfig(display, config);
        ColorDescriptor desc = configToDescriptor(ec, (colorSpace == EGL_VG_COLORSPACE_LINEAR) ? VG_FALSE : VG_TRUE,
            (alphaFormat == EGL_VG_ALPHA_FORMAT_PRE) ? VG_TRUE : VG_FALSE);
        d = createDrawable(&desc, width, height, ec->m_samples, ec->m_maskBits);
        if (d)
        {
            //s = RI_NEW(VGEGLSurface, (NULL, config, d, largestPbuffer, EGL_BACK_BUFFER));    //throws bad_alloc
            s = createVgEglSurface(NULL, config, &d, 1, largestPbuffer, EGL_BACK_BUFFER);
            if (s)
            {
                s->m_referenceCount++;

                //display->addSurface(s)
                s->next = display->m_surfaces;
                display->m_surfaces = s;
            }
            else
            {
                destroyDrawable(d);
                EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_SURFACE);
            }
        }
        else
        {
            EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_SURFACE);
        }
    }

    EGL_RETURN(EGL_SUCCESS, (EGLSurface)s);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLSurface EGLAPIENTRY eglCreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list)
{
    EGL_GET_DISPLAY(dpy, EGL_NO_SURFACE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_NO_SURFACE);
    EGL_IF_ERROR(buftype != EGL_OPENVG_IMAGE, EGL_BAD_PARAMETER, EGL_NO_SURFACE);
    EGL_IF_ERROR(!buffer, EGL_BAD_PARAMETER, EGL_NO_SURFACE);    //TODO should also check if buffer really is a valid VGImage object (needs VG context for that)
    Image* image = (Image*)buffer;
    EGL_IF_ERROR(image->m_inUse, EGL_BAD_ACCESS, EGL_NO_SURFACE);    //buffer is in use by OpenVG
    EGL_IF_ERROR(!configExists(display, config), EGL_BAD_CONFIG, EGL_NO_SURFACE);
    EGL_IF_ERROR(attrib_list && attrib_list[0] != EGL_NONE, EGL_BAD_ATTRIBUTE, EGL_NO_SURFACE);    //there are no valid attribs for OpenVG
    ColorDescriptor* bc = &((Image*)buffer)->m_desc;
    VGEGLConfig* vc = findDisplayConfig(dpy, config);
    ColorDescriptor* cc = &vc->m_desc;
    EGL_IF_ERROR(bc->redBits != cc->redBits || bc->greenBits != cc->greenBits || bc->blueBits != cc->blueBits ||
                 bc->alphaBits != cc->alphaBits || bc->luminanceBits != cc->luminanceBits, EGL_BAD_MATCH, EGL_NO_SURFACE);

    //TODO If buffer is already bound to another pbuffer, an EGL BAD ACCESS error is generated.
    EGL_IF_ERROR(isEglVgInUse(image), EGL_BAD_ACCESS, EGL_NO_SURFACE);

    Drawable* d = NULL;
    VGEGLSurface* s = NULL;
 
    {
        //d = RI_NEW(Drawable, (image, display->getConfig(config).m_maskBits));
        VGEGLConfig* ec = findDisplayConfig(display, config);
        d = createImageDrawable(image, ec->m_maskBits);
        if (d)
        {
            //s = RI_NEW(VGEGLSurface,(NULL, config, d, false, EGL_BACK_BUFFER));    //throws bad_alloc
            s = createVgEglSurface(NULL, config, &d, 1, VG_FALSE, EGL_BACK_BUFFER);
            if (s)
            {
                s->m_referenceCount++;

                //display->addSurface(s)
                s->next = display->m_surfaces;
                display->m_surfaces = s;
            }
            else
            {
                destroyDrawable(d);
                EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_SURFACE);
            }
        }
        else
        {
            EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_SURFACE);
        }
    }

    EGL_RETURN(EGL_SUCCESS, (EGLSurface)s);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLSurface EGLAPIENTRY eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list)
{
    EGL_GET_DISPLAY(dpy, EGL_NO_SURFACE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_NO_SURFACE);
    EGL_IF_ERROR(!configExists(display, config), EGL_BAD_CONFIG, EGL_NO_SURFACE);
    EGL_IF_ERROR(!pixmap, EGL_BAD_NATIVE_PIXMAP, EGL_NO_SURFACE);
    VGEGLConfig* ec = findDisplayConfig(display, config);
    EGL_IF_ERROR(ec->m_samples != 1, EGL_BAD_MATCH, EGL_NO_SURFACE);

    //TODO If there is already an EGLSurface associated with pixmap (as a result of a previous eglCreatePixmapSurface call), then a EGL BAD ALLOC error is generated.
    EGL_IF_ERROR(isPixmapInUse(pixmap), EGL_BAD_ALLOC, EGL_NO_SURFACE);

    Drawable* d = NULL;
    VGEGLSurface* s = NULL;
    VGuint width, height, stride, bitsPerPixel;
    VGImageFormat format = {0};
    VGubyte *bits;
    OSGetPixmapInfo(pixmap, &width, &height, &format, &bitsPerPixel, &stride, &bits);

    {
        //d = RI_NEW(Drawable, (Color::formatToDescriptor((VGImageFormat)pixmap->format), pixmap->width, pixmap->height, pixmap->stride, (RIuint8*)pixmap->data, display->getConfig(config).m_maskBits));    //throws bad_alloc
        VGEGLConfig* ec = findDisplayConfig(display, config);
        ColorDescriptor desc = formatToDescriptor(format);
        d = createDescDrawable(&desc, width, height, stride, (VGubyte*)bits, ec->m_maskBits);
        if (d)
        {
            //s = RI_NEW(VGEGLSurface,(NULL, config, d, false, EGL_BACK_BUFFER));    //throws bad_alloc
            s = createVgEglSurface(NULL, config, &d, 1, VG_FALSE, EGL_BACK_BUFFER);
            if (s)
            {
                s->m_referenceCount++;

                //display->addSurface(s)
                s->next = display->m_surfaces;
                display->m_surfaces = s;
            }
            else
            {
                destroyDrawable(d);
                EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_SURFACE);
            }
        }
        else
        {
            EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_SURFACE);
        }
    }

    EGL_RETURN(EGL_SUCCESS, (EGLSurface)s);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglDestroySurface(EGLDisplay dpy, EGLSurface surface)
{
    EGL_GET_DISPLAY(dpy, EGL_FALSE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);

    EGL_IF_ERROR(!surfaceExists(display, surface), EGL_BAD_SURFACE, EGL_FALSE);

    removeSurface(display, (VGEGLSurface*)surface);

    ((VGEGLSurface*)surface)->m_referenceCount--; VG_ASSERT(((VGEGLSurface*)surface)->m_referenceCount >= 0);
    if (!((VGEGLSurface*)surface)->m_referenceCount)
        destroyVgEglSurface((VGEGLSurface*)surface);

    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value)
{
    EGL_GET_DISPLAY(dpy, EGL_FALSE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
    EGL_IF_ERROR(!surfaceExists(display, surface), EGL_BAD_SURFACE, EGL_FALSE);

    //do nothing
    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value)
{
    EGL_GET_DISPLAY(dpy, EGL_FALSE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
    EGL_IF_ERROR(!surfaceExists(display, surface), EGL_BAD_SURFACE, EGL_FALSE);
    //TODO give an error if value is NULL?

    VGEGLSurface* s = (VGEGLSurface*)surface;
    switch(attribute)
    {
    case EGL_VG_ALPHA_FORMAT:
        //*value = (s->getDrawable()->getDescriptor().isPremultiplied()) ? EGL_VG_ALPHA_FORMAT_PRE : EGL_VG_ALPHA_FORMAT_NONPRE;
        *value = (s->m_drawable[0]->m_color->m_image->m_desc.internalFormat & PREMULTIPLIED) ? EGL_VG_ALPHA_FORMAT_PRE : EGL_VG_ALPHA_FORMAT_NONPRE;
        break;

    case EGL_VG_COLORSPACE:
        //*value = (s->getDrawable()->getDescriptor().isNonlinear()) ? EGL_VG_COLORSPACE_sRGB : EGL_VG_COLORSPACE_LINEAR;
        *value = (s->m_drawable[0]->m_color->m_image->m_desc.internalFormat & NONLINEAR) ? EGL_VG_COLORSPACE_sRGB : EGL_VG_COLORSPACE_LINEAR;
        break;

    case EGL_CONFIG_ID:
        {
            //*value = display->getConfig(s->m_config).m_configID;
            VGEGLConfig* config = findDisplayConfig(display, s->m_config);
            *value = config->m_configID;
        }
        break;

    case EGL_HEIGHT:
        //*value = s->getDrawable()->getHeight();
        *value = s->m_drawable[0]->m_color->m_height;
        break;

    case EGL_HORIZONTAL_RESOLUTION:
        *value = EGL_UNKNOWN;            //TODO Horizontal dot pitch
        break;

    case EGL_LARGEST_PBUFFER:
        if (!s->m_OSWindowContext)
            *value = s->m_largestPbuffer ? EGL_TRUE : EGL_FALSE;
        break;

    case EGL_MIPMAP_TEXTURE:
        if (!s->m_OSWindowContext)
            *value = EGL_FALSE;
        break;

    case EGL_MIPMAP_LEVEL:
        if (!s->m_OSWindowContext)
            *value = 0;
        break;

    case EGL_PIXEL_ASPECT_RATIO:
        *value = EGL_UNKNOWN;            //TODO Display aspect ratio
        break;

    case EGL_RENDER_BUFFER:
        //*value = s->getRenderBuffer();
        *value = s->m_renderBuffer;
        break;

    case EGL_SWAP_BEHAVIOR:
        *value = EGL_BUFFER_PRESERVED;
        break;

    case EGL_TEXTURE_FORMAT:
        if (!s->m_OSWindowContext)
            *value = EGL_NO_TEXTURE;
        break;

    case EGL_TEXTURE_TARGET:
        if (!s->m_OSWindowContext)
            *value = EGL_NO_TEXTURE;
        break;

    case EGL_VERTICAL_RESOLUTION:
        *value = EGL_UNKNOWN;            //TODO Vertical dot pitch
        break;

    case EGL_WIDTH:
        *value = s->m_drawable[0]->m_color->m_width;
        break;

    default:
        EGL_RETURN(EGL_BAD_ATTRIBUTE, EGL_FALSE);
    }

    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLContext EGLAPIENTRY eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list)
{
    EGL_GET_DISPLAY(dpy, EGL_NO_CONTEXT);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_NO_CONTEXT);
    EGL_IF_ERROR(!configExists(display, config), EGL_BAD_CONFIG, EGL_NO_CONTEXT);

    VGEGLThread* thread = getEglThread();
    if (!thread)
        EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_CONTEXT);

    //creation of OpenGL ES contexts is not allowed in this implementation
    if (thread->m_boundAPI != EGL_OPENVG_API)
        EGL_RETURN(EGL_BAD_MATCH, EGL_NO_CONTEXT);

    VGContext* vgctx = NULL;
    VGEGLContext* eglctx = NULL;

    //vgctx = RI_NEW(VGContext, (share_context ? ((VGEGLContext*)share_context)->getVGContext() : NULL));    //throws bad_alloc
    vgctx = createVgContext(share_context ? ((VGEGLContext*)share_context)->m_vgContext : NULL);
    if (!vgctx)
    {
        // Out of memory
        EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_CONTEXT);
    }

    //c = RI_NEW(VGEGLContext, (vgctx, config));    //throws bad_alloc
    eglctx = (VGEGLContext*)malloc(sizeof(VGEGLContext));
    if (!eglctx)
    {
        // Out of memory
        destroyVgContext(vgctx);
        EGL_RETURN(EGL_BAD_ALLOC, EGL_NO_CONTEXT);
    }
    eglctx->m_vgContext = vgctx;
    eglctx->m_config = config;
    eglctx->m_referenceCount = 1;
    eglctx->next = NULL;

    //display->addContext(c);    //throws bad_alloc
    eglctx->next = display->m_contexts;
    display->m_contexts = eglctx;

    EGL_RETURN(EGL_SUCCESS, (EGLContext)eglctx);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglDestroyContext(EGLDisplay dpy, EGLContext ctx)
{
    EGL_GET_DISPLAY(dpy, EGL_FALSE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
    EGL_IF_ERROR(!contextExists(display, ctx), EGL_BAD_CONTEXT, EGL_FALSE);

    VGEGLContext* context = (VGEGLContext*)ctx;
    removeContext(display, context);
    
    context->m_referenceCount--; VG_ASSERT(context->m_referenceCount >= 0);
    if (!context->m_referenceCount)
        destroyVgEglContext(context);

    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
    EGL_GET_DISPLAY(dpy, EGL_FALSE);
    EGL_IF_ERROR(ctx != EGL_NO_CONTEXT && !contextExists(display, ctx), EGL_BAD_CONTEXT, EGL_FALSE);
    EGL_IF_ERROR(draw != EGL_NO_SURFACE && !surfaceExists(display, draw), EGL_BAD_SURFACE, EGL_FALSE);
    EGL_IF_ERROR(read != EGL_NO_SURFACE && !surfaceExists(display, read), EGL_BAD_SURFACE, EGL_FALSE);
    EGL_IF_ERROR(draw != read, EGL_BAD_MATCH, EGL_FALSE);    //TODO what's the proper error code?
    EGL_IF_ERROR((draw != EGL_NO_SURFACE && ctx == EGL_NO_CONTEXT) || (draw == EGL_NO_SURFACE && ctx != EGL_NO_CONTEXT), EGL_BAD_MATCH, EGL_FALSE);

    VGEGLSurface* s = (VGEGLSurface*)draw;
    VGEGLContext* c = (VGEGLContext*)ctx;

    //Return if context/draw are already current
    VGEGLThread* thread = getEglCurrentThread();
    if (thread && thread->m_context == c && thread->m_surface == s)
    {
        EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
    }

    if (draw != EGL_NO_SURFACE && ctx != EGL_NO_CONTEXT)
    {
        EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);

        //If either draw or read are pbuffers created with eglCreatePbufferFromClientBuffer, and the underlying bound client API buffers
        //are in use by the client API that created them, an EGL BAD ACCESS error is generated.
        for (VGuint i = 0; i < s->m_drawableCount; i++)
        {
            EGL_IF_ERROR(isDrawableInUse(s->m_drawable[i]), EGL_BAD_ACCESS, EGL_FALSE);
        }

        //TODO properly check compatibility of surface and context:
        //-both have RGB or LUMINANCE configs
        //-buffer bit depths match
        //-configs support OpenVG
        EGL_IF_ERROR(!(display->m_configs[(EGLint)((size_t)(c->m_config))].m_renderType | EGL_OPENVG_BIT), EGL_BAD_CONTEXT, EGL_FALSE);
        EGL_IF_ERROR(!(display->m_configs[(EGLint)((size_t)(s->m_config))].m_renderType | EGL_OPENVG_BIT), EGL_BAD_CURRENT_SURFACE, EGL_FALSE);

        //-both have the same display
        EGL_IF_ERROR(s->m_config != c->m_config, EGL_BAD_MATCH, EGL_FALSE);

        //TODO check if context or surfaces are already bound to another thread
        EGL_IF_ERROR(isEglSurfaceBind(s) || isEglContextBind(c), EGL_BAD_CONFIG, EGL_FALSE);

        //If a native window underlying either draw or read is no longer valid, an EGL BAD NATIVE WINDOW error is generated.
        EGL_IF_ERROR(s->m_OSWindowContext && !OSIsWindow(s->m_OSWindowContext), EGL_BAD_NATIVE_WINDOW, EGL_FALSE);

        //TODO If the previous context of the calling display has unflushed commands, and the previous surface is no longer valid, an EGL BAD CURRENT SURFACE error is generated. (can this happen?)
        //TODO If the ancillary buffers for draw and read cannot be allocated, an EGL BAD ALLOC error is generated. (mask buffer?)
    }

    //check if the thread is current
    if (thread)
    {    //thread is current, release the old bindinds and remove the thread from the current thread list
        VGEGLContext* pc = thread->m_context;
        VGEGLSurface* ps = thread->m_surface;
        if (pc)
        {
            vgFlush();
            setDefaultDrawable(pc->m_vgContext, NULL);

            pc->m_referenceCount--; VG_ASSERT(pc->m_referenceCount >= 0);
            if (!pc->m_referenceCount)
                destroyVgEglContext(pc);
        }
        if (ps)
        {
            ps->m_referenceCount--; VG_ASSERT(ps->m_referenceCount >= 0);
            if (!ps->m_referenceCount)
                destroyVgEglSurface(ps);
        }

        removeEglCurrentThread(thread);
    }

    if ( c && s )
    {
        //bind context and surface to the current thread
        VGEGLThread* newThread = getEglThread();
        if (!newThread)
            EGL_RETURN(EGL_BAD_ALLOC, EGL_FALSE);
        
        newThread->m_context = c;
        newThread->m_surface = s;
        if (s->m_drawableIndex == 0)
            s->m_drawableIndex++;

        setDefaultDrawable(c->m_vgContext, s->m_drawable[s->m_drawableIndex - 1]);
        c->m_referenceCount++;
        s->m_referenceCount++;
        addEglCurrentThread(newThread);
    }

    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLContext EGLAPIENTRY eglGetCurrentContext()
{
    EGL_GET_EGL(EGL_NO_CONTEXT);
    EGLContext ret = EGL_NO_CONTEXT;
    VGEGLThread* thread = getEglCurrentThread();

    if (thread && thread->m_boundAPI == EGL_OPENVG_API)
    {
        ret = thread->m_context;
        VG_ASSERT(ret);
    }

    EGL_RETURN(EGL_SUCCESS, ret);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLSurface EGLAPIENTRY eglGetCurrentSurface(EGLint readdraw)
{
    EGL_GET_EGL(EGL_NO_SURFACE);
    EGL_IF_ERROR(readdraw != EGL_READ && readdraw != EGL_DRAW, EGL_BAD_PARAMETER, EGL_NO_SURFACE);
    EGLContext ret = EGL_NO_SURFACE;

    VGEGLThread* thread = getEglCurrentThread();
    if (thread && thread->m_boundAPI == EGL_OPENVG_API)
    {
        ret = thread->m_surface;
        VG_ASSERT(ret);
    }

    EGL_RETURN(EGL_SUCCESS, ret);
}

/*-------------------------------------------------------------------*//*!
* \brief    Returns the current display
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLDisplay EGLAPIENTRY eglGetCurrentDisplay(void)
{
    EGL_GET_EGL(EGL_NO_DISPLAY);

    VGEGLThread* thread = getEglCurrentThread();
    if (!thread || thread->m_boundAPI != EGL_OPENVG_API)
        EGL_RETURN(EGL_SUCCESS, EGL_NO_DISPLAY);

    VGEGLContext* ctx = thread->m_context;
    VG_ASSERT(ctx);
    EGLDisplay ret = findEglDisplay(ctx);

    EGL_RETURN(EGL_SUCCESS, ret);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint* value)
{
    EGL_GET_DISPLAY(dpy, EGL_FALSE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
    EGL_IF_ERROR(!contextExists(display, ctx), EGL_BAD_CONTEXT, EGL_FALSE);
    EGL_IF_ERROR(attribute != EGL_CONFIG_ID && attribute != EGL_CONTEXT_CLIENT_TYPE, EGL_BAD_ATTRIBUTE, EGL_FALSE);
    if (attribute == EGL_CONFIG_ID)
    {
        // *value = display->getConfig(((VGEGLContext*)ctx)->getConfig()).m_configID;
        VGEGLConfig* config = findDisplayConfig(display, ((VGEGLContext*)ctx)->m_config);
        *value = config->m_configID;
    }
    if (attribute == EGL_CONTEXT_CLIENT_TYPE)
        *value = EGL_OPENVG_API;
    // TODO Handling of EGL_RENDER_BUFFER attribute is missing.

    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglBindAPI(EGLenum api)
{
    EGL_GET_EGL(EGL_FALSE);
    EGL_IF_ERROR(api != EGL_OPENVG_API && api != EGL_OPENGL_ES_API, EGL_BAD_PARAMETER, EGL_FALSE);

    VGEGLThread* thread = getEglThread();
    if (thread)
        thread->m_boundAPI = api;

    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLenum EGLAPIENTRY eglQueryAPI(void)
{
    EGL_GET_EGL(EGL_NONE);

    VGEGLThread* thread = getEglThread();
    if (thread)
        EGL_RETURN(EGL_SUCCESS, thread->m_boundAPI);

    EGL_RETURN(EGL_SUCCESS, EGL_NONE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglWaitClient()
{
    EGL_GET_EGL(EGL_FALSE);

    VGEGLThread* thread = getEglCurrentThread();
    if (thread && thread->m_boundAPI == EGL_OPENVG_API)
        vgFinish();

    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    Waits for OpenGL ES
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglWaitGL(void)
{
    return EGL_TRUE;
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        We don't support native rendering
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglWaitNative(EGLint engine)
{
    return EGL_TRUE;
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
    EGL_GET_DISPLAY(dpy, EGL_FALSE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
    EGL_IF_ERROR(!surfaceExists(display, surface), EGL_BAD_SURFACE, EGL_FALSE);

    VGEGLSurface* s = (VGEGLSurface*)surface;

    VGEGLThread* currentThread = getEglCurrentThread();
    EGL_IF_ERROR(!currentThread || currentThread->m_surface!= s, EGL_BAD_SURFACE, EGL_FALSE);
    EGL_IF_ERROR(!OSIsWindow(s->m_OSWindowContext), EGL_BAD_NATIVE_WINDOW, EGL_FALSE);

    vgFlush();

    if (!s->m_OSWindowContext)
    {    //do nothing for other than window surfaces (NOTE: single-buffered window surfaces should return immediately as well)
        EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
    }

    int windowWidth = 0, windowHeight = 0;
    OSGetWindowSize(s->m_OSWindowContext, &windowWidth, &windowHeight);

    Drawable *drawable = s->m_drawable[s->m_drawableIndex - 1];

    if (windowWidth != drawable->m_color->m_width || windowHeight != drawable->m_color->m_height)
    {    //resize the back buffer
        VGEGLContext* c __attribute__((unused)) = currentThread->m_context;
        VG_ASSERT(c);
#if 0 // TODO support resize
        try
        {
            s->getDrawable()->resize(windowWidth, windowHeight);    //throws bad_alloc
        }
        catch(std::bad_alloc)
        {
            setDefaultDrawable(c->m_vgContext, NULL);
            EGL_RETURN(EGL_BAD_ALLOC, EGL_FALSE);
        }
#endif
    }

    OSBlitToWindow(s->m_OSWindowContext, drawable);
    
    /* Update drawable of render target */
    {
        VGEGLContext* ctx = currentThread->m_context;  
        s->m_drawableIndex++;
    if (s->m_drawableIndex > s->m_drawableCount)
        s->m_drawableIndex = 1;

    setDefaultDrawable(ctx->m_vgContext, s->m_drawable[s->m_drawableIndex - 1]);
    }

    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglCopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target)
{
    EGL_GET_DISPLAY(dpy, EGL_FALSE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
    EGL_IF_ERROR(!surfaceExists(display, surface), EGL_BAD_SURFACE, EGL_FALSE);
    EGL_IF_ERROR(!target, EGL_BAD_NATIVE_PIXMAP, EGL_FALSE);
#if 0
    try
    {
        Image output(Color::formatToDescriptor((VGImageFormat)target->format), target->width, target->height, target->stride, (RIuint8*)target->data);
        output.addReference();
        output.blit(((VGEGLSurface*)surface)->getDrawable()->getColorBuffer(), 0, 0, 0, 0, target->width, target->height);    //throws bad_alloc
        output.removeReference();
    }
#endif
    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        We support only swap interval one
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglSwapInterval(EGLDisplay dpy, EGLint interval)
{
    EGL_GET_DISPLAY(dpy, EGL_FALSE);
    EGL_IF_ERROR(!display, EGL_NOT_INITIALIZED, EGL_FALSE);
    EGL_RETURN(EGL_SUCCESS, EGL_TRUE);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI void (*EGLAPIENTRY eglGetProcAddress(const char *procname))()
{
    if (!procname)
        return NULL;
    return NULL;
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

EGLAPI EGLBoolean EGLAPIENTRY eglReleaseThread(void)
{
    EGL_GET_EGL(EGL_FALSE);

    //check if the thread is current
    VGEGLThread* thread = getEglCurrentThread();
    if (thread)
    {    //thread is current, release the old bindings and remove the thread from the current thread list
        VGEGLContext* pc = thread->m_context;
        VGEGLSurface* ps = thread->m_surface;
        if (pc)
        {
            vgFlush();
            setDefaultDrawable(pc->m_vgContext, NULL);

            pc->m_referenceCount--; VG_ASSERT(pc->m_referenceCount >= 0);
            if (!pc->m_referenceCount)
                destroyVgEglContext(pc);
        }
        if (ps)
        {
            ps->m_referenceCount--; VG_ASSERT(ps->m_referenceCount >= 0);
            if (!ps->m_referenceCount)
                destroyVgEglSurface(ps);
        }

        removeEglCurrentThread(thread);
    }

    //destroy EGL's thread struct
    destroyEglThreads();

    //destroy the EGL instance
    releaseEGL();

    OSReleaseMutex();
    OSDeinitMutex();

    return EGL_SUCCESS;
}
