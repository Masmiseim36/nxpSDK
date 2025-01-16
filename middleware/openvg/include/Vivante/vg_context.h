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

#ifndef __VG_CONTEXT_H__
#define __VG_CONTEXT_H__

#define OPENVG_DLL_EXPORTS
#ifndef EGLAPI
#  if defined(_WIN32) && !defined(__SCITECH_SNAP__)
#    define EGLAPI    __declspec(dllexport)
#  else
#    define EGLAPI
#  endif
#endif
#ifndef EGLAPIENTRY
#    define EGLAPIENTRY
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "egl.h"
#include "openvg.h"
#include "vg_lite.h"

#include "vg_defs.h"
#include "vg_math.h"
#include "vg_image.h"

extern void  OSAcquireMutex(void);
extern void  OSReleaseMutex(void);

#define VIV_EGL_NUMCONFIGS  10

#define VG_NO_RETVAL

//this must be the first line in an API function
#define VG_GET_CONTEXT(RETVAL) \
    OSAcquireMutex(); \
    VGContext* context = (VGContext*)getCurrentEglVgContext(); \
    if (!context) \
    { \
        OSReleaseMutex(); \
        return RETVAL;\
    }

typedef struct VGImageEntryRec
{
    Objectptr                      m_array[VIV_OBJECTS_HASH];
    VGint                          count;
} VGImageEntry;

typedef struct
{
    VGImageEntry*                   m_resources;
    VGint                           m_referenceCount;
    VGuint                          lohandle;
    VGuint                          hihandle;
} VGImageManager;

typedef struct VGMaskLayerEntryRec
{
    Objectptr                       m_array[VIV_OBJECTS_HASH];
    VGint                           count;
} VGMaskLayerEntry;

typedef struct
{
    VGMaskLayerEntry*               m_resources;
    VGint                           m_referenceCount;
    VGuint                          lohandle;
    VGuint                          hihandle;
} VGMaskLayerManager;

typedef struct VGPathEntryRec
{
    Objectptr                       m_array[VIV_OBJECTS_HASH];
    VGint                           count;
} VGPathEntry;

typedef struct
{
    VGPathEntry*                    m_resources;
    VGint                           m_referenceCount;
    VGuint                          lohandle;
    VGuint                          hihandle;
} VGPathManager;

typedef struct VGPaintEntryRec
{
    Paint*                          resource;
    struct VGPaintEntryRec*         next;
} VGPaintEntry;

typedef struct
{
    VGPaintEntry*                   m_resources;
    VGint                           m_referenceCount;
} VGPaintManager;

typedef struct VGFontEntryRec
{
    Objectptr                       m_array[VIV_OBJECTS_HASH];
    VGint                           count;
} VGFontEntry;

typedef struct
{
    VGFontEntry*                    m_resources;
    VGint                           m_referenceCount;
    VGuint                          lohandle;
    VGuint                          hihandle;
} VGFontManager;

typedef struct __VGcontextRec
{
    // Mode settings
    VGMatrixMode                    m_matrixMode;
    VGFillRule                      m_fillRule;
    VGImageQuality                  m_imageQuality;
    VGRenderingQuality              m_renderingQuality;
    VGBlendMode                     m_blendMode;
    VGImageMode                     m_imageMode;

    // Scissor rectangles
    VGRectangle                     m_scissor[VIV_MAX_SCISSOR_RECTS];
    VGint                           m_scissorCount;

    // Stroke parameters
    VGfloat                         m_strokeLineWidth;
    VGfloat                         m_inputStrokeLineWidth;
    VGCapStyle                      m_strokeCapStyle;
    VGJoinStyle                     m_strokeJoinStyle;
    VGfloat                         m_strokeMiterLimit;
    VGfloat                         m_inputStrokeMiterLimit;

    VGfloat                         m_strokeDashPattern[VIV_MAX_DASH_COUNT];
    VGfloat                         m_inputStrokeDashPattern[VIV_MAX_DASH_COUNT];
    VGint                           m_strokeDashPatternCount;

    VGfloat                         m_strokeDashPhase;
    VGfloat                         m_inputStrokeDashPhase;
    VGboolean                       m_strokeDashPhaseReset;

    // Edge fill color for vgConvolve and pattern paint
    Color                           m_tileFillColor;
    Color                           m_inputTileFillColor;

    // Color for vgClear
    Color                           m_clearColor;
    Color                           m_inputClearColor;

    Vector2                         m_glyphOrigin;
    Vector2                         m_inputGlyphOrigin;

    VGboolean                       m_masking;
    VGboolean                       m_scissoring;
    VGboolean                       m_image;

    VGPixelLayout                   m_pixelLayout;

    VGboolean                       m_filterFormatLinear;
    VGboolean                       m_filterFormatPremultiplied;
    VGbitfield                      m_filterChannelMask;

    // Matrices
    Matrix3x3                       m_pathUserToSurface;
    Matrix3x3                       m_imageUserToSurface;
    Matrix3x3                       m_glyphUserToSurface;
    Matrix3x3                       m_fillPaintToUser;
    Matrix3x3                       m_strokePaintToUser;

    VGPaint                         m_fillPaint;
    VGPaint                         m_strokePaint;

    VGboolean                       m_colorTransform;
    VGfloat                         m_colorTransformValues[8];
    VGfloat                         m_inputColorTransformValues[8];

    VGErrorCode                     m_error;

    VGImageManager*                 m_imageManager;
    VGPathManager*                  m_pathManager;
    VGPaintManager*                 m_paintManager;
    VGFontManager*                  m_fontManager;
    VGMaskLayerManager*             m_maskLayerManager;
    VGPaintType                     m_paintType;
    Color                           m_paintColor;
    Image*                          m_pattern;
    Drawable*                       m_eglDrawable;

} VGContext;

typedef struct
{
    //EGL RED SIZE bits of Red in the color buffer
    //EGL GREEN SIZE bits of Green in the color buffer
    //EGL BLUE SIZE bits of Blue in the color buffer
    //EGL ALPHA SIZE bits of Alpha in the color buffer
    //EGL LUMINANCE SIZE bits of Luminance in the color buffer
    ColorDescriptor                 m_desc;
    int                             m_samples;
    int                             m_maskBits;
    EGLint                          m_configID;            //EGL CONFIG ID unique EGLConfig identifier
    EGLConfig                       m_config;
    //EGL BUFFER SIZE depth of the color buffer (sum of channel bits)
    EGLint                          m_depth;
    //EGL ALPHA MASK SIZE number alpha mask bits (always 8)
    EGLint                          m_maskAlpha;
    //EGL BIND TO TEXTURE RGB boolean True if bindable to RGB textures. (always EGL_FALSE)
    EGLint                          m_texRGB;
    //EGL BIND TO TEXTURE RGBA boolean True if bindable to RGBA textures. (always EGL_FALSE)
    EGLint                          m_texRGBA;
    //EGL COLOR BUFFER TYPE enum color buffer type (EGL_RGB_BUFFER, EGL_LUMINANCE_BUFFER)
    EGLint                          m_bufferType;
    //EGL CONFIG CAVEAT enum any caveats for the configuration (always EGL_NONE)
    EGLint                          m_caveats;
    //EGL DEPTH SIZE integer bits of Z in the depth buffer (always 0)
    EGLint                          m_depthSize;
    //EGL LEVEL integer frame buffer level (always 0)
    EGLint                          m_Leve;
    //EGL MAX PBUFFER WIDTH integer maximum width of pbuffer (always INT_MAX)
    EGLint                          m_widthPbuffer;
    //EGL MAX PBUFFER HEIGHT integer maximum height of pbuffer (always INT_MAX)
    EGLint                          m_heightPbuffer;
    //EGL MAX PBUFFER PIXELS integer maximum size of pbuffer (always INT_MAX)
    EGLint                          m_pixelPbuffer;
    //EGL MAX SWAP INTERVAL integer maximum swap interval (always 1)
    EGLint                          m_maxSwapInterval;
    //EGL MIN SWAP INTERVAL integer minimum swap interval (always 1)
    EGLint                          m_minSwapInterval;
    //EGL NATIVE RENDERABLE boolean EGL TRUE if native rendering APIs can render to surface (always EGL_FALSE)
    EGLBoolean                      m_nativeRenderable;
    //EGL NATIVE VISUAL ID integer handle of corresponding native visual (always 0)
    EGLint                          m_naviveVisualID;
    //EGL NATIVE VISUAL TYPE integer native visual type of the associated visual (always EGL_NONE)
    EGLint                          m_nativeVisualType;
    //EGL RENDERABLE TYPE bitmask which client rendering APIs are supported. (always EGL_OPENVG_BIT)
    EGLint                          m_renderType;
    //EGL SAMPLE BUFFERS integer number of multisample buffers (always 0)
    EGLint                          m_samplesBuffers;
    //EGL SAMPLES integer number of samples per pixel (always 0)
    EGLint                          m_samplesPerPixel;
    //EGL STENCIL SIZE integer bits of Stencil in the stencil buffer (always 0)
    EGLint                          m_stencilSize;
    //EGL SURFACE TYPE bitmask which types of EGL surfaces are supported. (always EGL WINDOW BIT | EGL PIXMAP BIT | EGL PBUFFER BIT)
    EGLint                          m_surfaceType;
    //EGL TRANSPARENT TYPE enum type of transparency supported (always EGL_NONE)
    //EGL TRANSPARENT RED VALUE integer transparent red value (undefined)
    //EGL TRANSPARENT GREEN VALUE integer transparent green value (undefined)
    //EGL TRANSPARENT BLUE VALUE integer transparent blue value (undefined)
} VGEGLConfig;

typedef struct VGEGLContextRec
{
    VGContext*                      m_vgContext;
    EGLConfig                       m_config;
    int                             m_referenceCount;
    struct VGEGLContextRec*         next;
} VGEGLContext;

typedef struct VGEGLSurfaceRec
{
    void*                           m_OSWindowContext;
    EGLConfig                       m_config;
    Drawable*                       m_drawable[VG_FRAMEBUFFER_COUNT];
    VGuint                          m_drawableCount;
    VGuint                          m_drawableIndex;
    VGboolean                       m_largestPbuffer;
    VGint                           m_renderBuffer;        //EGL_BACK_BUFFER or EGL_SINGLE_BUFFER
    VGint                           m_referenceCount;
    struct VGEGLSurfaceRec*         next;
} VGEGLSurface;

typedef struct VGEGLDisplayRec
{
    EGLDisplay                      m_id;
    VGEGLContext*                   m_contexts;
    VGEGLSurface*                   m_surfaces;
    VGEGLConfig                     m_configs[VIV_EGL_NUMCONFIGS];
    void*                           nativeDisplay;
    struct VGEGLDisplayRec*         next;
} VGEGLDisplay;


typedef struct VGEGLThreadRec
{
    VGEGLContext*                   m_context;
    VGEGLSurface*                   m_surface;
    EGLint                          m_error;
    void*                           m_threadID;
    EGLint                          m_boundAPI;
    struct VGEGLThreadRec*          next;
} VGEGLThread;

/* EGL&OS functions for use in an OpenVG implementation */

extern Drawable* createDrawable(ColorDescriptor* desc, VGint width, VGint height, VGint numSamples, VGint maskBits);
extern Drawable* createDescDrawable(ColorDescriptor* desc, VGint width, VGint height, VGint stride, VGubyte* data, VGint maskBits);
extern Drawable* createImageDrawable(Image* image, int maskBits);
extern void destroyDrawable(Drawable* draw);
extern void setDefaultDrawable(VGContext* ctx, Drawable* drawable);
extern void destroyVgContext(VGContext* ctx);
extern void* getCurrentEglVgContext(void);
extern ColorDescriptor formatToDescriptor(VGImageFormat format);
extern VGContext* createVgContext(VGContext* shareContext);
extern Paint* createdefaultPaint();

extern VGboolean isAlphaOnly(ColorDescriptor* desc);
extern VGboolean isValidDescriptor(ColorDescriptor* desc);
extern VGboolean isEglVgInUse(void* image);
extern VGboolean isValidImage(VGContext* ctx, VGImage image);
extern VGboolean isValidMaskLayer(VGContext* ctx, VGHandle mask);
extern VGboolean isValidPath(VGContext* ctx, VGPath path);
extern VGboolean isValidPaint(VGContext* ctx, VGPaint paint);
extern VGboolean isValidFont(VGContext* ctx, VGFont font);

extern Image* createBlankImage(ColorDescriptor* desc, VGint width, VGint height, VGbitfield allowedQuality);
extern void addImageResource(VGContext* ctx, Image* image);
extern void removeImageResource(VGContext* ctx, Image* image);
extern void createChildImage(Image* im, Image* parent, int x, int y, int width, int height);
extern VGboolean imageOverlaps(Image* dst, Image* src);

extern void addMaskLayerResource(VGContext* ctx, Surface* layer);
extern void removeMaskLayerResource(VGContext* ctx, Surface* layer);
extern void addPathResource(VGContext* ctx, Path* path);
extern void removePathResource(VGContext* ctx, Path* path);
extern void addPaintResource(VGContext* ctx, Paint* paint);
extern void removePaintResource(VGContext* ctx, Paint* paint);
extern void releasePaint(VGContext* ctx, VGbitfield paintModes);
extern void addFontResource(VGContext* ctx, Font* font);
extern void removeFontResource(VGContext* ctx, Font* font);
extern Glyph* findGlyph(Font* font, unsigned int index);

extern Surface* createSurface(ColorDescriptor* desc, VGint width, VGint height, VGint numSamples);
extern Surface* createDataImgSurface(ColorDescriptor* desc, VGint width, VGint height, VGint stride, VGubyte* data);
extern void destroySurface(Surface* surf);
extern void destroyImage(Image** im, VGContext* context);

void freePathImpl(Path* path);
void freePaintImpl(Paint* paint);
void freeFontImpl(Font* font);

extern void setError(int errcode);

#endif /* __VG_CONTEXT_H__ */
