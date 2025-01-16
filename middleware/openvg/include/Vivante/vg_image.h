/****************************************************************************
*
*    Copyright 2022 Vivante Corporation, Santa Clara, California.
*    Copyright 2024 NXP
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

#ifndef __VG_IMAGE_H__
#define __VG_IMAGE_H__

#include "openvg.h"
#include "vg_math.h"

typedef enum
{
    lRGBA = 0,
    sRGBA = 1,
    lRGBA_PRE = 2,
    sRGBA_PRE = 3,
    lLA = 4,
    sLA = 5,
    lLA_PRE = 6,
    sLA_PRE = 7
} InternalFormat;

typedef enum
{
    NONLINEAR = (1 << 0),
    PREMULTIPLIED = (1 << 1),
    LUMINANCE = (1 << 2)
} FormatBits;

typedef enum
{
    START_SUBPATH = (1 << 0),
    END_SUBPATH = (1 << 1),
    START_SEGMENT = (1 << 2),
    END_SEGMENT = (1 << 3),
    CLOSE_SUBPATH = (1 << 4),
    IMPLICIT_CLOSE_SUBPATH = (1 << 5)
} VertexFlags;

typedef enum
{
    GLYPH_UNINITIALIZED = 0,
    GLYPH_PATH = 1,
    GLYPH_IMAGE = 2
} GlyphState;

typedef struct {
    VGint                   x;
    VGint                   y;
    VGint                   width;
    VGint                   height;
} VGRectangle;

typedef struct {
    VGint                   redBits;
    VGint                   redShift;
    VGint                   greenBits;
    VGint                   greenShift;
    VGint                   blueBits;
    VGint                   blueShift;
    VGint                   alphaBits;
    VGint                   alphaShift;
    VGint                   luminanceBits;
    VGint                   luminanceShift;
    VGImageFormat           format;
    InternalFormat          internalFormat;
    VGint                   bitsPerPixel;
} ColorDescriptor;

typedef enum _ObjectType
{
    OBJECTTYPE_PATH = 0,
    OBJECTTYPE_IMAGE,
    OBJECTTYPE_MASK,
    OBJECTTYPE_FONT,
    OBJECTTYPE_PAINT,
    OBJECTTYPE_COUNT
} ObjectType;

typedef struct _Object_list* Objectptr;

typedef struct _Object_list
{
    Objectptr prev;
    Objectptr next;
    ObjectType type;
    VGint name;
} Object_list;

typedef struct ImageRec {
    Object_list             object;
    ColorDescriptor         m_desc;
    VGint                   m_width;
    VGint                   m_height;
    VGbitfield              m_allowedQuality;
    VGint                   m_inUse;
    VGint                   m_stride;
    VGint                   m_referenceCount;
    VGint                   m_storageOffsetX;
    VGint                   m_storageOffsetY;
    VGboolean               m_ownsData;
    VGubyte*                m_data;
    struct ImageRec*        m_parent;
    void*                   m_vglbuf;
    VGboolean               m_mipmapsValid;
    //Image*                  m_mipmaps;
} Image;

typedef struct
{
    Object_list             object;
    struct
    {
        int                 x;
        int                 miny;
        int                 maxy;
        int                 direction;        //1 start, -1 end
    } ScissorEdge;

    int                     m_width;
    int                     m_height;
    int                     m_numSamples;
    int                     m_referenceCount;
    Image* m_image;
} Surface;

typedef struct
{
    int                     m_referenceCount;
    Surface* m_color;
    Surface* m_mask;
} Drawable;

typedef struct {
    VGfloat                 r;
    VGfloat                 g;
    VGfloat                 b;
    VGfloat                 a;
    InternalFormat          m_format;
} Color;

typedef struct {
    VGfloat                 offset;
    Color                   color;
} GradientStop;

typedef struct {
    Object_list             object;
    VGPaintType             m_paintType;
    Color                   m_paintColor;
    Color                   m_inputPaintColor;
    VGColorRampSpreadMode   m_colorRampSpreadMode;
    GradientStop            m_colorRampStops[VIV_MAX_COLOR_RAMP_STOPS];
    GradientStop            m_inputColorRampStops[VIV_MAX_COLOR_RAMP_STOPS];
    VGint                   m_colorRampStopsCount;
    VGint                   m_inputcolorRampStopsCount;
    VGboolean               m_colorRampPremultiplied;
    Vector2                 m_inputLinearGradientPoint0;
    Vector2                 m_inputLinearGradientPoint1;
    Vector2                 m_inputRadialGradientCenter;
    Vector2                 m_inputRadialGradientFocalPoint;
    VGfloat                 m_inputRadialGradientRadius;
    Vector2                 m_linearGradientPoint0;
    Vector2                 m_linearGradientPoint1;
    Vector2                 m_radialGradientCenter;
    Vector2                 m_radialGradientFocalPoint;
    VGfloat                 m_radialGradientRadius;
    VGTilingMode            m_patternTilingMode;
    Image*                  m_pattern;
    VGint                   m_referenceCount;
    vg_lite_buffer_t        grad_image;
} Paint;

typedef struct {
    //  Array<Edge>         m_edges;
    //  Array<ScissorEdge>  m_scissorEdges;
    //  bool                m_scissor;
    //  Sample              m_samples[RI_MAX_SAMPLES];
    VGint                   m_numSamples;
    VGint                   m_numFSAASamples;
    VGfloat                 m_sumWeights;
    VGfloat                 m_sampleRadius;

    struct 
    {
        VGfloat   x;
        VGfloat   y;
        VGfloat   weight;
    }m_samples[VIV_MAX_SAMPLES];

    Vector2                 m_edgeMin;
    Vector2                 m_edgeMax;
    VGint                   m_covMinx;
    VGint                   m_covMiny;
    VGint                   m_covMaxx;
    VGint                   m_covMaxy;
    VGint                   m_vpx;
    VGint                   m_vpy;
    VGint                   m_vpwidth;
    VGint                   m_vpheight;
    VGFillRule              m_fillRule;
    VGuint*                 m_covBuffer;

    struct 
    {
        VGint                   x;
        VGint                   miny;
        VGint                   maxy;
        VGint                   direction;  
    }ScissorEdge;

} Rasterizer;

typedef struct {
    Drawable*               m_drawable;
    VGboolean               m_masking;
    Image*                  m_image;
    const Paint*            m_paint;
    Paint                   m_defaultPaint;
    VGBlendMode             m_blendMode;
    VGImageMode             m_imageMode;
    VGImageQuality          m_imageQuality;
    Color                   m_tileFillColor;
    VGboolean               m_colorTransform;
    VGfloat                 m_colorTransformValues[8];
    Matrix3x3               m_surfaceToPaintMatrix;
    Matrix3x3               m_surfaceToImageMatrix;
} PixelPipe;

typedef struct {

    /* Embedded object */
    Object_list               object;
    /* Object states. */
    VGint                   m_format;
    VGPathDatatype          m_datatype;
    VGfloat                 m_scale;
    VGfloat                 m_bias;
    VGbitfield              m_capabilities;
    VGint                   m_referenceCount;
    VGubyte*                m_segments;
    VGint                   m_numSegments;
    VGubyte**               m_data;
    VGint                   m_numCoordinates;

    void*                   m_vglPath;
    VGboolean               m_pathChanged;
    VGboolean               m_pathFont;
} Path;

typedef struct
{
    //Glyph() { m_state = GLYPH_UNINITIALIZED; m_path = m_image = VG_INVALID_HANDLE; m_isHinted = false; m_origin.set(0.0f, 0.0f); m_escapement.set(0.0f, 0.0f); }
    VGuint                  m_index;
    GlyphState              m_state;
    VGPath                  m_path;
    VGImage                 m_image;
    VGboolean               m_isHinted;
    Vector2                 m_origin;
    Vector2                 m_escapement;
} Glyph;

typedef struct {
    Object_list             object;
    Glyph*                  m_glyphs;
    VGint                   m_glyphArraySize;
    VGint                   m_glyphSize;
    VGint                   m_referenceCount;
    VGint                   m_glyphsAllocSize;
} Font;

typedef struct Image_kernel_allocate
{
    /* Number of bytes to allocate. */
    VGuint bytes;

    /* Flag to indicate whether the allocated memory is contiguous or not. */
    VGint contiguous;

    /* Memory handle. */
    void * memory_handle;

    /* Allocated memory. */
    void * memory;

    /* GPU address of allocated memory. */
    VGuint memory_gpu;
}
Image_kernel_allocate_t;

#define VG_ALIGN(number, alignment)    \
        (((number) + ((alignment) - 1)) & ~((alignment) - 1))

/*-------------------------------------------------------------------*//*!
* \brief    Converts from numBits into a shifted mask
* \param
* \return
* \note
*//*-------------------------------------------------------------------*/

static VG_INLINE VGuint bitsToMask(VGuint bits, VGuint shift)
{
    return ((1 << bits) - 1) << shift;
}

static VG_INLINE VGboolean isValidImageFormat(int f)
{
    if (f < VG_sRGBX_8888 || f > VG_lABGR_8888_PRE)
        return VG_FALSE;
    return VG_TRUE;
}

#endif /* __VG_IMAGE_H__ */

VGuint vgFormatTovgliteFormat(VGImageFormat format);

void setGlyphToPath(VGFont font, VGuint index, VGPath path, VGboolean isHinted, const Vector2* origin, const Vector2* escapement);

void clearGlyph(Glyph* g);

void setGlyphToImage(VGFont font, VGuint index, VGImage image, const Vector2* origin, const Vector2* escapement);

void initImage(Image* image, ColorDescriptor desc, VGuint width, VGuint height, VGbitfield allowedQuality);

void initDrawable(Drawable* drawable, ColorDescriptor desc, VGuint width, VGuint height, VGuint numSamples, VGuint maskBits);

void initSurface(Surface* surface, ColorDescriptor desc, VGuint width, VGuint height, VGuint numSamples);

void maskSurface(Drawable* drawable, const Surface* src, VGMaskOperation operation, int x, int y, int w, int h);

void setupRasterizer(Rasterizer* raster, VGint vpx, VGint vpy, VGint vpwidth, VGint vpheight, VGFillRule fillRule, const PixelPipe* pixelPipe, VGuint* covBuffer); //belong to rasterizer

VGint setupSamplingPattern(Rasterizer* raster, VGRenderingQuality renderingQuality, VGint numFSAASamples); //belong to rasterizer

void setDrawable(PixelPipe* pixelpipe, Drawable* drawable); //belong to pixelpipe

void setMask(PixelPipe* pixelpipe, VGboolean masking); //belong to pixelpipe

void setPaint(PixelPipe* pixelpipe, const Paint* paint); //belong to pixelpipe

void setBlendMode(PixelPipe* pixelpipe, VGBlendMode blendMode); //belong to pixelpipe

void lookup(Image* dst, const Image* src, const VGubyte* redLUT, const VGubyte* greenLUT, const VGubyte* blueLUT, const VGubyte* alphaLUT, VGboolean outputLinear, VGboolean outputPremultiplied, VGboolean filterFormatLinear, VGboolean filterFormatPremultiplied, VGbitfield channelMask);

void lookupSingle(Image* dst, const Image* src, const VGuint* lookupTable, VGImageChannel sourceChannel, VGboolean outputLinear, VGboolean outputPremultiplied, VGboolean filterFormatLinear, VGboolean filterFormatPremultiplied, VGbitfield channelMask);

void colorMatrix(const VGfloat* matrix, Image* src, Image* dst, VGboolean filterFormatLinear, VGboolean filterFormatPremultiplied, VGbitfield channelMask);

void gaussianBlur(VGImage dst, VGImage src, VGfloat stdDeviationX, VGfloat stdDeviationY, VGboolean filterFormatLinear, VGboolean filterFormatPremultiplied, VGTilingMode tilingMode, Color tileFillColor, VGbitfield filterChannelmask);

void convolve(Image* dst, Image* src, int kernelWidth, int kernelHeight, int shiftX, int shiftY, const VGshort* kernel, VGfloat scale, VGfloat bias, VGTilingMode tilingMode, Color* edgeFillColor, VGboolean filterFormatLinear, VGboolean filterFormatPremultiplied, VGbitfield channelMask);

void separableConvolve(Image* dst, Image* src, int kernelWidth, int kernelHeight, int shiftX, int shiftY, const VGshort* kernelX, const VGshort* kernelY, VGfloat scale, VGfloat bias, VGTilingMode tilingMode, Color* edgeFillColor, VGboolean filterFormatLinear, VGboolean filterFormatPremultiplied, VGbitfield channelMask);

int getNumGlyphs(Font* font);

VGboolean descriptorIsPremultiplied(const ColorDescriptor* dec);
