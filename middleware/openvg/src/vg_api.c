/****************************************************************************
*
*    Copyright 2022 Vivante Corporation, Santa Clara, California.
*    Copyright 2024 NXP
*
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

#include "vg_context.h"
#include "vg_image.h"

extern void  OSAcquireMutex(void);
extern void  OSReleaseMutex(void);
void convert(InternalFormat srcFormat, InternalFormat dstFormat, Color* c);

#define VG_IF_ERROR(COND, ERRORCODE, RETVAL) \
    if (COND) { setError(ERRORCODE); OSReleaseMutex(); return RETVAL; }

//all API functions must call this as their last operation (also functions that don't return values)
//NOTE: don't evaluate anything or read state in RETVAL (it'll be executed after the mutex has been released)
#define VG_RETURN(RETVAL) \
    { OSReleaseMutex(); \
    return RETVAL; }

#define VG_FLOAT_TO_UB(x)   ((VGubyte)((x)*255 + 0.5))
#define VG_FLOAT_EPSILON    1e-12f

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define VG_CLAMP(x, min, max) (((x) < (min)) ? (min) : ((x) > (max)) ? (max) : (x))

#define MAX_UINT32 (0xffffffffu)
#define MAX_INT32  (0x7fffffff)
#define MIN_INT32  (-0x7fffffff-1)
static const VGuint _commandSize[] = {
    0,              /* 0: VG_CLOSE_PATH         */
    0,              /* 1: NONE                  */
    2,              /* 2: VG_MOVE_TO_ABS        */
    2,              /* 3: VG_MOVE_TO_REL        */
    2,              /* 4: VG_LINE_TO_ABS        */
    2,              /* 5: VG_LINE_TO_REL        */
    1,              /* 6: VG_HLINE_TO_ABS       */
    1,              /* 7: VG_HLINE_TO_REL       */
    1,              /* 8: VG_VLINE_TO_ABS       */
    1,              /* 9: VG_VLINE_TO_REL       */
    4,              /* 10: VG_QUAD_TO_ABS       */
    4,              /* 11: VG_QUAD_TO_REL       */
    6,              /* 12: VG_CUBIC_TO_ABS      */
    6,              /* 13: VG_CUBIC_TO_REL      */
    2,              /* 14: VG_SQUAD_TO_ABS      */
    2,              /* 15: VG_SQUAD_TO_REL      */
    4,              /* 16: VG_SCUBIC_TO_ABS     */
    4,              /* 17: VG_SCUBIC_TO_REL     */
    5,              /* 18: VG_SCCWARC_TO_ABS    */
    5,              /* 19: VG_SCCWARC_TO_REL    */
    5,              /* 20: VG_SCWARC_TO_ABS     */
    5,              /* 21: VG_SCWARC_TO_REL     */
    5,              /* 22: VG_LCCWARC_TO_ABS    */
    5,              /* 23: VG_LCCWARC_TO_REL    */
    5,              /* 24: VG_LCWARC_TO_ABS     */
    5,              /* 25: VG_LCWARC_TO_REL     */
};

static const VGuint _dataSize[] = {
    1,              /* 0: VG_PATH_DATATYPE_S_8  */
    2,              /* 1: VG_PATH_DATATYPE_S_16 */
    4,              /* 1: VG_PATH_DATATYPE_S_32 */
    4,              /* 1: VG_PATH_DATATYPE_F    */
};

static const uint8_t _vglOpcodes[] = {
    VLC_OP_CLOSE,       /* 0: VG_CLOSE_PATH         */
    VLC_OP_CLOSE,       /* 1: VG_CLOSE_PATH | VG_RELATIVE */
    VLC_OP_MOVE,        /* 2: VG_MOVE_TO_ABS        */
    VLC_OP_MOVE_REL,    /* 3: VG_MOVE_TO_REL        */
    VLC_OP_LINE,        /* 4: VG_LINE_TO_ABS        */
    VLC_OP_LINE_REL,    /* 5: VG_LINE_TO_REL        */
    VLC_OP_HLINE,       /* 6: VG_HLINE_TO_ABS       */
    VLC_OP_HLINE_REL,   /* 7: VG_HLINE_TO_REL       */
    VLC_OP_VLINE,       /* 8: VG_VLINE_TO_ABS       */
    VLC_OP_VLINE_REL,   /* 9: VG_VLINE_TO_REL       */
    VLC_OP_QUAD,        /* 10: VG_QUAD_TO_ABS       */
    VLC_OP_QUAD_REL,    /* 11: VG_QUAD_TO_REL       */
    VLC_OP_CUBIC,       /* 12: VG_CUBIC_TO_ABS      */
    VLC_OP_CUBIC_REL,   /* 13: VG_CUBIC_TO_REL      */
    VLC_OP_SQUAD,       /* 14: VG_SQUAD_TO_ABS      */
    VLC_OP_SQUAD_REL,   /* 15: VG_SQUAD_TO_REL      */
    VLC_OP_SCUBIC,      /* 16: VG_SCUBIC_TO_ABS     */
    VLC_OP_SCUBIC_REL,  /* 17: VG_SCUBIC_TO_REL     */
    VLC_OP_SCWARC,      /* 18: VG_SCCWARC_TO_ABS    */
    VLC_OP_SCWARC_REL,  /* 19: VG_SCCWARC_TO_REL    */
    VLC_OP_SCCWARC,     /* 20: VG_SCWARC_TO_ABS     */
    VLC_OP_SCCWARC_REL, /* 21: VG_SCWARC_TO_REL     */
    VLC_OP_LCWARC,      /* 22: VG_LCCWARC_TO_ABS    */
    VLC_OP_LCWARC_REL,  /* 23: VG_LCCWARC_TO_REL    */
    VLC_OP_LCCWARC,     /* 24: VG_LCWARC_TO_ABS     */
    VLC_OP_LCCWARC_REL, /* 25: VG_LCWARC_TO_REL     */
};
#define CMD_TO_SEGMENT(cmd) (cmd & 0xFE)
#define CMD_IS_ABS(cmd) ((cmd & 0x01) == 0x00) 

static void get_format_bytes(vg_lite_buffer_format_t format,
    uint32_t* mul,
    uint32_t* div,
    uint32_t* bytes_align)
{
    *mul = *div = 1;
    *bytes_align = 4;
    switch (format) {
    case VG_LITE_L8:
    case VG_LITE_A8:
    case VG_LITE_RGBA8888_ETC2_EAC:
        break;

    case VG_LITE_A4:
        *div = 2;
        break;

    case VG_LITE_ABGR1555:
    case VG_LITE_ARGB1555:
    case VG_LITE_BGRA5551:
    case VG_LITE_RGBA5551:
    case VG_LITE_RGBA4444:
    case VG_LITE_BGRA4444:
    case VG_LITE_ABGR4444:
    case VG_LITE_ARGB4444:
    case VG_LITE_RGB565:
    case VG_LITE_BGR565:
    case VG_LITE_YUYV:
    case VG_LITE_YUY2:
    case VG_LITE_YUY2_TILED:
        /* AYUY2 buffer memory = YUY2 + alpha. */
    case VG_LITE_AYUY2:
    case VG_LITE_AYUY2_TILED:
        /* ABGR8565_PLANAR buffer memory = RGB565 + alpha. */
    case VG_LITE_ABGR8565_PLANAR:
    case VG_LITE_ARGB8565_PLANAR:
    case VG_LITE_RGBA5658_PLANAR:
    case VG_LITE_BGRA5658_PLANAR:
        *mul = 2;
        break;

    case VG_LITE_RGBA8888:
    case VG_LITE_BGRA8888:
    case VG_LITE_ABGR8888:
    case VG_LITE_ARGB8888:
    case VG_LITE_RGBX8888:
    case VG_LITE_BGRX8888:
    case VG_LITE_XBGR8888:
    case VG_LITE_XRGB8888:
        *mul = 4;
        break;

    case VG_LITE_NV12:
    case VG_LITE_NV12_TILED:
        *mul = 3;
        break;

    case VG_LITE_ANV12:
    case VG_LITE_ANV12_TILED:
        *mul = 4;
        break;

    case VG_LITE_INDEX_1:
        *div = 8;
        *bytes_align = 8;
        break;

    case VG_LITE_INDEX_2:
        *div = 4;
        *bytes_align = 8;
        break;

    case VG_LITE_INDEX_4:
        *div = 2;
        *bytes_align = 8;
        break;

    case VG_LITE_INDEX_8:
        *bytes_align = 1;
        break;

    case VG_LITE_RGBA2222:
    case VG_LITE_BGRA2222:
    case VG_LITE_ABGR2222:
    case VG_LITE_ARGB2222:
        *mul = 1;
        break;

    case VG_LITE_RGB888:
    case VG_LITE_BGR888:
    case VG_LITE_ABGR8565:
    case VG_LITE_BGRA5658:
    case VG_LITE_ARGB8565:
    case VG_LITE_RGBA5658:
        *mul = 3;
        break;

    /* OpenVG format*/
    case VG_sRGBX_8888:
    case VG_sRGBA_8888:
    case VG_sRGBA_8888_PRE:
    case VG_lRGBX_8888:
    case VG_lRGBA_8888:
    case VG_lRGBA_8888_PRE:
    case VG_sXRGB_8888:
    case VG_sARGB_8888:
    case VG_sARGB_8888_PRE:
    case VG_lXRGB_8888:
    case VG_lARGB_8888:
    case VG_lARGB_8888_PRE:
    case VG_sBGRX_8888:
    case VG_sBGRA_8888:
    case VG_sBGRA_8888_PRE:
    case VG_lBGRX_8888:
    case VG_lBGRA_8888:
    case VG_sXBGR_8888:
    case VG_sABGR_8888:
    case VG_lBGRA_8888_PRE:
    case VG_sABGR_8888_PRE:
    case VG_lXBGR_8888:
    case VG_lABGR_8888:
    case VG_lABGR_8888_PRE:
        *mul = 4;
        break;

    case VG_sRGBA_5551:
    case VG_sRGBA_4444:
    case VG_sARGB_1555:
    case VG_sARGB_4444:
    case VG_sBGRA_5551:
    case VG_sBGRA_4444:
    case VG_sABGR_1555:
    case VG_sABGR_4444:
    case VG_sRGB_565:
    case VG_sBGR_565:
        *mul = 2;
        break;

    case VG_sL_8:
    case VG_lL_8:
    case VG_A_8:
        break;

    case VG_BW_1:
        *div = 8;
        *bytes_align = 8;
        break;

    case VG_A_4:
    case VG_A_1:
        *div = 2;
        break;


    default:
        break;
    }
}

static VG_INLINE VGboolean isAligned(const void* ptr, int alignment)
{
    VG_ASSERT(alignment == 1 || alignment == 2 || alignment == 4);
    if (((VGuintptr)ptr) & (alignment - 1))
        return VG_FALSE;
    return VG_TRUE;
}

static VG_INLINE VGboolean isFormatAligned(const void* ptr, VGImageFormat format)
{
    VG_ASSERT(isValidImageFormat(format));
    int alignment = formatToDescriptor(format).bitsPerPixel >> 3;
    if (alignment <= 1)
        return VG_TRUE;    //one bit or byte per pixel
    return isAligned(ptr, alignment);
}

static VG_INLINE VGboolean isSupportFormat(VGContext* context, VGImageFormat format) {
        return !(format == VG_A_1 || format == VG_A_4 || format == VG_BW_1 || format == VG_sL_8 || format == VG_lL_8);
}

static VG_INLINE void clampColor(VGContext* context, Color* c)
{
    VGfloat u;
    c->a = VG_CLAMP(c->a, 0.0f, 1.0f);
    if (context->m_colorTransform) {
        u = (VGfloat)1.0f;
    }
    else {
        u = (c->m_format & PREMULTIPLIED) ? c->a : (VGfloat)1.0f;
    }
    c->r = VG_CLAMP(c->r, 0.0f, u);
    c->g = VG_CLAMP(c->g, 0.0f, u);
    c->b = VG_CLAMP(c->b, 0.0f, u);
}

__attribute__((unused)) static VG_INLINE void premultiplyColor(Color* c)
{
    if (!(c->m_format & PREMULTIPLIED))
    {
        c->r *= c->a;
        c->g *= c->a;
        c->b *= c->a;
        c->m_format = (InternalFormat)(c->m_format | PREMULTIPLIED);
    }
}

static VG_INLINE void initMatrix3x3(Matrix3x3* mtx, VGfloat m00, VGfloat m01, VGfloat m02, VGfloat m10, VGfloat m11, VGfloat m12, VGfloat m20, VGfloat m21, VGfloat m22)
{
    mtx->matrix[0][0] = m00;
    mtx->matrix[0][1] = m01;
    mtx->matrix[0][2] = m02;
    mtx->matrix[1][0] = m10;
    mtx->matrix[1][1] = m11;
    mtx->matrix[1][2] = m12;
    mtx->matrix[2][0] = m20;
    mtx->matrix[2][1] = m21;
    mtx->matrix[2][2] = m22;
}

static VG_INLINE void multMatrix3x3(Matrix3x3* matrix, Matrix3x3* mult)
{
    Matrix3x3 temp;
    int row, column;

    for (row = 0; row < 3; row++) 
    {
        for (column = 0; column < 3; column++)
        {
            temp.matrix[row][column] = (matrix->matrix[row][0] * mult->matrix[0][column])
                                     + (matrix->matrix[row][1] * mult->matrix[1][column])
                                     + (matrix->matrix[row][2] * mult->matrix[2][column]);
        }
    }

    memcpy(matrix, &temp, sizeof(temp));
}

__attribute__((unused)) static VG_INLINE void multVector3ByMatrix3x3(Vector3* vector, Matrix3x3* mult)
{
    Vector3 temp;

    temp.x = vector->x * mult->matrix[0][0] + vector->y * mult->matrix[0][1] + vector->z * mult->matrix[0][2];
    temp.y = vector->x * mult->matrix[1][0] + vector->y * mult->matrix[1][1] + vector->z * mult->matrix[1][2];
    temp.z = vector->x * mult->matrix[2][0] + vector->y * mult->matrix[2][1] + vector->z * mult->matrix[2][2];
    vector->x = temp.x;
    vector->y = temp.y;
    vector->z = temp.z;
}

static VG_INLINE void RoundMatrix3x3(Matrix3x3* target_matrix, const Matrix3x3* source_matrix)
{
    int row, column;
    for (row = 0; row < 3; row++)
    {
        for (column = 0; column < 3; column++)
        {
            if ((row != 2) && (column == 2))
            {
               
                target_matrix->matrix[row][column] = (VGfloat)round((double)source_matrix->matrix[row][column]);
            }
            else
            {
                target_matrix->matrix[row][column] = source_matrix->matrix[row][column];
            }
        }
    }
}

static VG_INLINE float getMatrixDeterminant(Matrix3x3* matrix) {
    return 
        matrix->matrix[0][0] * (matrix->matrix[1][1] * matrix->matrix[2][2] - matrix->matrix[2][1] * matrix->matrix[1][2]) +
        matrix->matrix[0][1] * (matrix->matrix[2][0] * matrix->matrix[1][2] - matrix->matrix[1][0] * matrix->matrix[2][2]) + 
        matrix->matrix[0][2] * (matrix->matrix[1][0] * matrix->matrix[2][1] - matrix->matrix[2][0] * matrix->matrix[1][1]);
}

void VG_APIENTRY vgFlush(void)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    vg_lite_flush();
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgFinish(void)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    vg_lite_finish();
    VG_RETURN(VG_NO_RETVAL);
}

VGErrorCode VG_APIENTRY vgGetError(void)
{
    VG_GET_CONTEXT(VG_NO_CONTEXT_ERROR);
    VGErrorCode error = context->m_error;
    context->m_error = VG_NO_ERROR;
    VG_RETURN(error);
}

static VG_INLINE VGfloat inputFloat(VGfloat f)
{
    //this function is used for all floating point input values
    if (VG_ISNAN(f)) return 0.0f;    //convert NaN to zero
    return VG_CLAMP(f, -VIV_FLOAT_MAX, VIV_FLOAT_MAX);    //clamp +-inf to +-VGfloat max
}

static VG_INLINE Vector2 inputVector2(const Vector2 v)
{
    Vector2 outV2;
    outV2.x = inputFloat(v.x);
    outV2.y = inputFloat(v.y);
    return outV2;
}

__attribute__((unused)) static VG_INLINE Color inputColor(const Color c)
{
    Color r = c;
    r.r = inputFloat(r.r);
    r.g = inputFloat(r.g);
    r.b = inputFloat(r.b);
    r.a = inputFloat(r.a);
    return r;
}

static VG_INLINE int inputFloatToInt(VGfloat value)
{
    double v = (double)floor(value);
    v = v > (double)VIV_INT32_MAX ? (double)VIV_INT32_MAX : v;
    v = v < (double)VIV_INT32_MIN ? (double)VIV_INT32_MIN : v;
    return (int)v;
}

static VG_INLINE int paramToInt(const void* values, VGboolean floats, int count, int i)
{
    VG_ASSERT(i >= 0);
    if (i >= count || !values)
        return 0;
    if (floats)
        return inputFloatToInt(((const VGfloat*)values)[i]);
    return (int)((const VGint*)values)[i];
}

static VG_INLINE VGfloat paramToFloat(const void* values, VGboolean floats, int count, int i)
{
    VG_ASSERT(i >= 0);
    if (i >= count || !values)
        return 0.0f;
    if (floats)
        return ((const VGfloat*)values)[i];
    return (VGfloat)((const VGint*)values)[i];
}

static VG_INLINE void floatToParam(void* output, VGboolean outputFloats, int count, int i, VGfloat value)
{
    VG_ASSERT(i >= 0);
    VG_ASSERT(output);
    if (i >= count)
        return;
    if (outputFloats)
        ((VGfloat*)output)[i] = value;
    else
        ((VGint*)output)[i] = (VGint)inputFloatToInt(value);
}

static VG_INLINE void intToParam(void* output, VGboolean outputFloats, int count, int i, VGint value)
{
    VG_ASSERT(i >= 0);
    VG_ASSERT(output);
    if (i >= count)
        return;
    if (outputFloats)
        ((VGfloat*)output)[i] = (VGfloat)value;
    else
        ((VGint*)output)[i] = value;
}

static void setifv(VGContext* context, VGParamType type, VGint count, const void* values, VGboolean floats)
{
    VG_ASSERT(context);
    VG_ASSERT(!count || (count && values));
    Drawable* drawable = context->m_eglDrawable;
    int ivalue = paramToInt(values, floats, count, 0);
    VGfloat fvalue = paramToFloat(values, floats, count, 0);

    switch(type)
    {
    case VG_MATRIX_MODE:
        if (count != 1 || ivalue < VG_MATRIX_PATH_USER_TO_SURFACE || ivalue > VG_MATRIX_GLYPH_USER_TO_SURFACE)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_matrixMode = (VGMatrixMode)ivalue;
        break;

    case VG_FILL_RULE:
        if (count != 1 || ivalue < VG_EVEN_ODD || ivalue > VG_NON_ZERO)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_fillRule = (VGFillRule)ivalue;
        break;

    case VG_IMAGE_QUALITY:
        if (count != 1 || ivalue < VG_IMAGE_QUALITY_NONANTIALIASED || ivalue > VG_IMAGE_QUALITY_BETTER)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_imageQuality = (VGImageQuality)ivalue;
        break;

    case VG_RENDERING_QUALITY:
        if (count != 1 || ivalue < VG_RENDERING_QUALITY_NONANTIALIASED || ivalue > VG_RENDERING_QUALITY_BETTER)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_renderingQuality = (VGRenderingQuality)ivalue;
        break;

    case VG_BLEND_MODE:
        if (count != 1 || ivalue < VG_BLEND_SRC || ivalue > VG_BLEND_ADDITIVE)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_blendMode = (VGBlendMode)ivalue;
        break;

    case VG_IMAGE_MODE:
        if (count != 1 || ivalue < VG_DRAW_IMAGE_NORMAL || ivalue > VG_DRAW_IMAGE_STENCIL)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_imageMode = (VGImageMode)ivalue;
        break;

    case VG_SCISSOR_RECTS:
    {
        VGint j = 0;
        if (count & 3)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }    //count must be a multiple of four

        for (int i=0; i<VG_INT_MIN(count, VIV_MAX_SCISSOR_RECTS*4); i+=4)
        {
            VGRectangle s;
            s.x = paramToInt(values, floats, count, i+0);
            s.y = paramToInt(values, floats, count, i+1);
            s.width = paramToInt(values, floats, count, i+2);
            s.height = paramToInt(values, floats, count, i+3);
            context->m_scissor[j++] = s;
        }
        context->m_scissorCount = j;
        vg_lite_buffer_t* dstbuf = (vg_lite_buffer_t*)drawable->m_color->m_image->m_vglbuf;
        vg_lite_scissor_rects(dstbuf, context->m_scissorCount, (vg_lite_rectangle_t*)context->m_scissor); 
        if (context->m_scissoring && context->m_scissorCount > 0) {
            vg_lite_enable_scissor();
        }
        break;
    }

    case VG_COLOR_TRANSFORM:
        if (count != 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_colorTransform = ivalue ? VG_TRUE : VG_FALSE;
        break;

    case VG_COLOR_TRANSFORM_VALUES:
        if (count != 8 || !values) { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        {
            for (int i=0;i<8;i++)
            {
                context->m_inputColorTransformValues[i] = paramToFloat(values, floats, count, i);
                context->m_colorTransformValues[i] = inputFloat(context->m_inputColorTransformValues[i]);
            }
        }
        break;

    case VG_STROKE_LINE_WIDTH:
        if (count != 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_inputStrokeLineWidth = fvalue;
        context->m_strokeLineWidth = inputFloat(fvalue);
        break;

    case VG_STROKE_CAP_STYLE:
        if (count != 1 || ivalue < VG_CAP_BUTT || ivalue > VG_CAP_SQUARE)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_strokeCapStyle = (VGCapStyle)ivalue;
        break;

    case VG_STROKE_JOIN_STYLE:
        if (count != 1 || ivalue < VG_JOIN_MITER || ivalue > VG_JOIN_BEVEL)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_strokeJoinStyle = (VGJoinStyle)ivalue;
        break;

    case VG_STROKE_MITER_LIMIT:
        if (count != 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_inputStrokeMiterLimit = fvalue;
        context->m_strokeMiterLimit = inputFloat(fvalue);
        break;

    case VG_STROKE_DASH_PATTERN:
    {
        VGint j = 0;
        for (int i=0; i<VG_INT_MIN(count, VG_MAX_DASH_COUNT); i++)
        {
            VGfloat v = paramToFloat(values, floats, count, i);
            context->m_inputStrokeDashPattern[j] = v;
            context->m_strokeDashPattern[j] = inputFloat(v);
            j++;
        }
        context->m_strokeDashPatternCount = j;
        break;
    }

    case VG_STROKE_DASH_PHASE:
        if (count != 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_inputStrokeDashPhase = fvalue;
        context->m_strokeDashPhase = inputFloat(fvalue);
        break;

    case VG_STROKE_DASH_PHASE_RESET:
        if (count != 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_strokeDashPhaseReset = ivalue ? VG_TRUE : VG_FALSE;
        break;

    case VG_TILE_FILL_COLOR:
        if (count != 4 || !values)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_inputTileFillColor.r = paramToFloat(values, floats, count, 0);
        context->m_inputTileFillColor.g = paramToFloat(values, floats, count, 1);
        context->m_inputTileFillColor.b = paramToFloat(values, floats, count, 2);
        context->m_inputTileFillColor.a = paramToFloat(values, floats, count, 3);
        context->m_inputTileFillColor.m_format = sRGBA;
        context->m_tileFillColor = context->m_inputTileFillColor;
        break;

    case VG_GLYPH_ORIGIN:
        if (count != 2 || !values)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_inputGlyphOrigin.x = paramToFloat(values, floats, count, 0);
        context->m_inputGlyphOrigin.y = paramToFloat(values, floats, count, 1);
        context->m_glyphOrigin = inputVector2(context->m_inputGlyphOrigin);
        break;

    case VG_CLEAR_COLOR:
        if (count != 4 || !values)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_inputClearColor.r = paramToFloat(values, floats, count, 0);
        context->m_inputClearColor.g = paramToFloat(values, floats, count, 1);
        context->m_inputClearColor.b = paramToFloat(values, floats, count, 2);
        context->m_inputClearColor.a = paramToFloat(values, floats, count, 3);
        context->m_inputClearColor.m_format = sRGBA;
        context->m_clearColor = context->m_inputClearColor;
        break;

    case VG_MASKING:
        if (count != 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_masking = ivalue ? VG_TRUE : VG_FALSE;
        if (context->m_masking)
            vg_lite_enable_masklayer();
        else
            vg_lite_disable_masklayer();
        break;

    case VG_SCISSORING:
        if (count != 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_scissoring = ivalue ? VG_TRUE : VG_FALSE;
        if (context->m_scissoring && context->m_scissorCount > 0)
            vg_lite_enable_scissor();
        else
            vg_lite_disable_scissor();
        break;

    case VG_PIXEL_LAYOUT:
        if (count != 1 || ivalue < VG_PIXEL_LAYOUT_UNKNOWN || ivalue > VG_PIXEL_LAYOUT_BGR_HORIZONTAL)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_pixelLayout = (VGPixelLayout)ivalue;
        break;

    case VG_SCREEN_LAYOUT:
        if (count != 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        break;    //setting read-only values has no effect

    case VG_FILTER_FORMAT_LINEAR:
        if (count != 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_filterFormatLinear = ivalue ? VG_TRUE : VG_FALSE;
        break;

    case VG_FILTER_FORMAT_PREMULTIPLIED:
        if (count != 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_filterFormatPremultiplied = ivalue ? VG_TRUE : VG_FALSE;
        break;

    case VG_FILTER_CHANNEL_MASK:
        if (count != 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        //undefined bits are ignored
        context->m_filterChannelMask = (VGbitfield)ivalue;
        break;

    case VG_MAX_SCISSOR_RECTS:
    case VG_MAX_DASH_COUNT:
    case VG_MAX_KERNEL_SIZE:
    case VG_MAX_SEPARABLE_KERNEL_SIZE:
    case VG_MAX_COLOR_RAMP_STOPS:
    case VG_MAX_IMAGE_WIDTH:
    case VG_MAX_IMAGE_HEIGHT:
    case VG_MAX_IMAGE_PIXELS:
    case VG_MAX_IMAGE_BYTES:
    case VG_MAX_FLOAT:
    case VG_MAX_GAUSSIAN_STD_DEVIATION:
        if (count != 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        break;    //setting read-only values has no effect

    default:
        setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
        break;
    }
}

void VG_APIENTRY vgSetf(VGParamType type, VGfloat value)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(type == VG_SCISSOR_RECTS || type == VG_STROKE_DASH_PATTERN || type == VG_TILE_FILL_COLOR ||
                type == VG_CLEAR_COLOR, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);    //vector type value
    VGfloat values[1] = {value};
    setifv(context, type, 1, values, VG_TRUE);
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgSeti(VGParamType type, VGint value)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(type == VG_SCISSOR_RECTS || type == VG_STROKE_DASH_PATTERN || type == VG_TILE_FILL_COLOR ||
                type == VG_CLEAR_COLOR, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);    //vector type value
    VGint values[1] = {value};
    setifv(context, type, 1, values, VG_FALSE);
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgSetiv(VGParamType type, VGint count, const VGint * values)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(count < 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR((!values && count > 0) || (values && !isAligned(values,4)), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    setifv(context, type, count, values, VG_FALSE);
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgSetfv(VGParamType type, VGint count, const VGfloat * values)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(count < 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR((!values && count > 0) || (values && !isAligned(values,4)), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    setifv(context, type, count, values, VG_TRUE);
    VG_RETURN(VG_NO_RETVAL);
}

static void getifv(VGContext* context, VGParamType type, VGint count, void* values, VGboolean floats)
{
    switch(type)
    {
    case VG_MATRIX_MODE:
        if (count > 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_matrixMode);
        break;

    case VG_FILL_RULE:
        if (count > 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_fillRule);
        break;

    case VG_IMAGE_QUALITY:
        if (count > 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_imageQuality);
        break;

    case VG_RENDERING_QUALITY:
        if (count > 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_renderingQuality);
        break;

    case VG_BLEND_MODE:
        if (count > 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_blendMode);
        break;

    case VG_IMAGE_MODE:
        if (count > 1)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_imageMode);
        break;

    case VG_SCISSOR_RECTS:
    {
        if (count > context->m_scissorCount*4)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        for (int i=0; i<context->m_scissorCount; i++)
        {
            intToParam(values, floats, count, i*4+0, context->m_scissor[i].x);
            intToParam(values, floats, count, i*4+1, context->m_scissor[i].y);
            intToParam(values, floats, count, i*4+2, context->m_scissor[i].width);
            intToParam(values, floats, count, i*4+3, context->m_scissor[i].height);
        }
        break;
    }

    case VG_COLOR_TRANSFORM:
        if (count != 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_colorTransform);
        break;

    case VG_COLOR_TRANSFORM_VALUES:
        if (count > 8) { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        {
            for (int i=0;i<count;i++)
            {
                floatToParam(values, floats, count, i, context->m_inputColorTransformValues[i]);
            }
        }
        break;

    case VG_STROKE_LINE_WIDTH:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        floatToParam(values, floats, count, 0, context->m_inputStrokeLineWidth);
        break;

    case VG_STROKE_CAP_STYLE:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_strokeCapStyle);
        break;

    case VG_STROKE_JOIN_STYLE:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_strokeJoinStyle);
        break;

    case VG_STROKE_MITER_LIMIT:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        floatToParam(values, floats, count, 0, context->m_inputStrokeMiterLimit);
        break;

    case VG_STROKE_DASH_PATTERN:
    {
        if (count > context->m_strokeDashPatternCount)
        { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        for (int i=0; i<context->m_strokeDashPatternCount; i++)
            floatToParam(values, floats, count, i, context->m_inputStrokeDashPattern[i]);
        break;
    }

    case VG_STROKE_DASH_PHASE:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        floatToParam(values, floats, count, 0, context->m_inputStrokeDashPhase);
        break;

    case VG_STROKE_DASH_PHASE_RESET:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_strokeDashPhaseReset);
        break;

    case VG_TILE_FILL_COLOR:
        if (count > 4)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        floatToParam(values, floats, count, 0, context->m_inputTileFillColor.r);
        floatToParam(values, floats, count, 1, context->m_inputTileFillColor.g);
        floatToParam(values, floats, count, 2, context->m_inputTileFillColor.b);
        floatToParam(values, floats, count, 3, context->m_inputTileFillColor.a);
        break;

    case VG_CLEAR_COLOR:
        if (count > 4)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        floatToParam(values, floats, count, 0, context->m_inputClearColor.r);
        floatToParam(values, floats, count, 1, context->m_inputClearColor.g);
        floatToParam(values, floats, count, 2, context->m_inputClearColor.b);
        floatToParam(values, floats, count, 3, context->m_inputClearColor.a);
        break;

    case VG_GLYPH_ORIGIN:
        if (count > 2)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        floatToParam(values, floats, count, 0, context->m_inputGlyphOrigin.x);
        floatToParam(values, floats, count, 1, context->m_inputGlyphOrigin.y);
        break;

    case VG_MASKING:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_masking);
        break;

    case VG_SCISSORING:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_scissoring);
        break;

    case VG_PIXEL_LAYOUT:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_pixelLayout);
        break;

    case VG_SCREEN_LAYOUT:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, VG_PIXEL_LAYOUT_UNKNOWN);
        break;

    case VG_FILTER_FORMAT_LINEAR:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_filterFormatLinear);
        break;

    case VG_FILTER_FORMAT_PREMULTIPLIED:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_filterFormatPremultiplied);
        break;

    case VG_FILTER_CHANNEL_MASK:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, context->m_filterChannelMask);
        break;

    case VG_MAX_SCISSOR_RECTS:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, VIV_MAX_SCISSOR_RECTS);
        break;

    case VG_MAX_DASH_COUNT:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, VIV_MAX_DASH_COUNT);
        break;

    case VG_MAX_KERNEL_SIZE:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, VIV_MAX_KERNEL_SIZE);
        break;

    case VG_MAX_SEPARABLE_KERNEL_SIZE:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, VIV_MAX_SEPARABLE_KERNEL_SIZE);
        break;

    case VG_MAX_COLOR_RAMP_STOPS:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, VIV_MAX_COLOR_RAMP_STOPS);
        break;

    case VG_MAX_IMAGE_WIDTH:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, VIV_MAX_IMAGE_WIDTH);
        break;

    case VG_MAX_IMAGE_HEIGHT:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, VIV_MAX_IMAGE_HEIGHT);
        break;

    case VG_MAX_IMAGE_PIXELS:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, VIV_MAX_IMAGE_PIXELS);
        break;

    case VG_MAX_IMAGE_BYTES:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, VIV_MAX_IMAGE_BYTES);
        break;

    case VG_MAX_FLOAT:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        floatToParam(values, floats, count, 0, VIV_FLOAT_MAX);
        break;

    case VG_MAX_GAUSSIAN_STD_DEVIATION:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        floatToParam(values, floats, count, 0, VIV_MAX_GAUSSIAN_DEVIATION);
        break;

    default:
        setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
        break;
    }
}

VGfloat VG_APIENTRY vgGetf(VGParamType type)
{
    VG_GET_CONTEXT(0.0f);
    VG_IF_ERROR(type == VG_SCISSOR_RECTS || type == VG_STROKE_DASH_PATTERN || type == VG_TILE_FILL_COLOR ||
                type == VG_CLEAR_COLOR, VG_ILLEGAL_ARGUMENT_ERROR, 0.0f);    //vector type value
    VGfloat ret = 0.0f;
    getifv(context, type, 1, &ret, VG_TRUE);
    VG_RETURN(ret);
}

VGint VG_APIENTRY vgGeti(VGParamType type)
{
    VG_GET_CONTEXT(0);
    VG_IF_ERROR(type == VG_SCISSOR_RECTS || type == VG_STROKE_DASH_PATTERN || type == VG_TILE_FILL_COLOR ||
                type == VG_CLEAR_COLOR, VG_ILLEGAL_ARGUMENT_ERROR, 0);    //vector type value
    VGint ret = 0;
    getifv(context, type, 1, &ret, VG_FALSE);
    VG_RETURN(ret);
}

void VG_APIENTRY vgGetiv(VGParamType type, VGint count, VGint * values)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(count <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!values || !isAligned(values,4), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    getifv(context, type, count, values, VG_FALSE);
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgGetfv(VGParamType type, VGint count, VGfloat * values)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(count <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!values || !isAligned(values,4), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    getifv(context, type, count, values, VG_TRUE);
    VG_RETURN(VG_NO_RETVAL);
}

VGint VG_APIENTRY vgGetVectorSize(VGParamType type)
{
    VG_GET_CONTEXT(0);
    VGint ret = 0;
    switch(type)
    {
    case VG_MATRIX_MODE:
    case VG_FILL_RULE:
    case VG_IMAGE_QUALITY:
    case VG_RENDERING_QUALITY:
    case VG_BLEND_MODE:
    case VG_IMAGE_MODE:
        ret = 1;
        break;

    case VG_SCISSOR_RECTS:
        ret = 4*context->m_scissorCount;
        break;

    case VG_COLOR_TRANSFORM:
        ret = 1;
        break;

    case VG_COLOR_TRANSFORM_VALUES:
        ret = 8;
        break;

    case VG_STROKE_LINE_WIDTH:
    case VG_STROKE_CAP_STYLE:
    case VG_STROKE_JOIN_STYLE:
    case VG_STROKE_MITER_LIMIT:
        ret = 1;
        break;

    case VG_STROKE_DASH_PATTERN:
        ret = context->m_strokeDashPatternCount;
        break;

    case VG_STROKE_DASH_PHASE:
    case VG_STROKE_DASH_PHASE_RESET:
        ret = 1;
        break;

    case VG_TILE_FILL_COLOR:
    case VG_CLEAR_COLOR:
        ret = 4;
        break;

    case VG_GLYPH_ORIGIN:
        ret = 2;
        break;

    case VG_MASKING:
    case VG_SCISSORING:
    case VG_PIXEL_LAYOUT:
    case VG_SCREEN_LAYOUT:
    case VG_FILTER_FORMAT_LINEAR:
    case VG_FILTER_FORMAT_PREMULTIPLIED:
    case VG_FILTER_CHANNEL_MASK:
    case VG_MAX_SCISSOR_RECTS:
    case VG_MAX_DASH_COUNT:
    case VG_MAX_KERNEL_SIZE:
    case VG_MAX_SEPARABLE_KERNEL_SIZE:
    case VG_MAX_COLOR_RAMP_STOPS:
    case VG_MAX_IMAGE_WIDTH:
    case VG_MAX_IMAGE_HEIGHT:
    case VG_MAX_IMAGE_PIXELS:
    case VG_MAX_IMAGE_BYTES:
    case VG_MAX_FLOAT:
    case VG_MAX_GAUSSIAN_STD_DEVIATION:
        ret = 1;
        break;

    default:
        setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
        break;
    }
    VG_RETURN(ret);
}

static void setPaintParameterifv(VGContext* context, Paint* paint, VGPaintParamType paramType, VGint count, const void* values, VGboolean floats)
{
    VG_ASSERT(context);
    VG_ASSERT(paint);

    int ivalue = paramToInt(values, floats, count, 0);

    switch(paramType)
    {
    case VG_PAINT_TYPE:
        if (count != 1 || ivalue < VG_PAINT_TYPE_COLOR || ivalue > VG_PAINT_TYPE_PATTERN)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        context->m_paintType = (VGPaintType)ivalue;
        paint->m_paintType = (VGPaintType)ivalue;
        break;

    case VG_PAINT_COLOR:
        if (count != 4)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        paint->m_inputPaintColor.r = paramToFloat(values, floats, count, 0);
        paint->m_inputPaintColor.g = paramToFloat(values, floats, count, 1),
        paint->m_inputPaintColor.b = paramToFloat(values, floats, count, 2),
        paint->m_inputPaintColor.a = paramToFloat(values, floats, count, 3),
        paint->m_inputPaintColor.m_format = sRGBA;

        paint->m_paintColor = paint->m_inputPaintColor;
        clampColor(context, &paint->m_paintColor);
        break;

    case VG_PAINT_COLOR_RAMP_SPREAD_MODE:
        if (count != 1 || ivalue < VG_COLOR_RAMP_SPREAD_PAD || ivalue > VG_COLOR_RAMP_SPREAD_REFLECT)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        paint->m_colorRampSpreadMode = (VGColorRampSpreadMode)ivalue;
        break;

    case VG_PAINT_COLOR_RAMP_STOPS:
    {
        int numStops = count/5;
        if (count != numStops*5)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }    //count must be a multiple of five

        {
            VGfloat prevOffset = -VIV_FLOAT_MAX;
            VGboolean valid = VG_TRUE;
            VGint j = 0;
            for (int i=0; i<VG_INT_MIN(numStops, VIV_MAX_COLOR_RAMP_STOPS); i++)    //NOTE: ignores the final stop if there is not enough parameters
            {
                GradientStop gs;
                gs.offset = paramToFloat(values, floats, count, i*5);
                gs.color.r = paramToFloat(values, floats, count, i*5+1);
                gs.color.g = paramToFloat(values, floats, count, i*5+2),
                gs.color.b = paramToFloat(values, floats, count, i*5+3),
                gs.color.a = paramToFloat(values, floats, count, i*5+4),
                gs.color.m_format = sRGBA;

                paint->m_inputColorRampStops[i]= gs;

                if (gs.offset < prevOffset)
                    valid = VG_FALSE;    //decreasing sequence, ignore it

                if (gs.offset >= 0.0f && gs.offset <= 1.0f)
                {
                    clampColor(context, &gs.color);
                    if (j == 0 && gs.offset > 0.0f)
                    {    //the first valid stop is not at 0, replicate the first one
                        VGfloat tmp = gs.offset;
                        gs.offset = 0.0f;
                        paint->m_colorRampStops[j++] = gs;
                        gs.offset = tmp;
                    }
                    paint->m_colorRampStops[j++] = gs;
                }
                prevOffset = gs.offset;
                paint->m_inputcolorRampStopsCount = i + 1;
            }
            if (valid && j > 0 && paint->m_colorRampStops[j-1].offset < 1.0f)
            {    //there is at least one stop, but the last one is not at 1, replicate the last one
                GradientStop gs = paint->m_colorRampStops[j-1];
                gs.offset = 1.0f;
                paint->m_colorRampStops[j++] = gs;
            }
            if (!valid || j == 0)
            {    //there are no valid stops, add implicit stops
                GradientStop gs;
                gs.offset = 0.0f;
                gs.color.r = 0;
                gs.color.g = 0;
                gs.color.b = 0;
                gs.color.a = 1;
                gs.color.m_format = sRGBA;
                paint->m_colorRampStops[j++] = gs;
                gs.offset = 1.0f;
                gs.color.r = 1;
                gs.color.g = 1;
                gs.color.b = 1;
                gs.color.a = 1;
                gs.color.m_format = sRGBA;
                paint->m_colorRampStops[j++] = gs;
            }
            VG_ASSERT(j >= 2 && j <= VG_MAX_COLOR_RAMP_STOPS);
            paint->m_colorRampStopsCount = j;
        }
        break;
    }

    case VG_PAINT_COLOR_RAMP_PREMULTIPLIED:
        if (count != 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        paint->m_colorRampPremultiplied = ivalue ? VG_TRUE : VG_FALSE;
        break;

    case VG_PAINT_LINEAR_GRADIENT:
        if (count != 4)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        paint->m_inputLinearGradientPoint0.x = paramToFloat(values, floats, count, 0);
        paint->m_inputLinearGradientPoint0.y = paramToFloat(values, floats, count, 1);
        paint->m_inputLinearGradientPoint1.x = paramToFloat(values, floats, count, 2);
        paint->m_inputLinearGradientPoint1.y = paramToFloat(values, floats, count, 3);
        paint->m_linearGradientPoint0 = inputVector2(paint->m_inputLinearGradientPoint0);
        paint->m_linearGradientPoint1 = inputVector2(paint->m_inputLinearGradientPoint1);
        break;

    case VG_PAINT_RADIAL_GRADIENT:
        if (count != 5)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        paint->m_inputRadialGradientCenter.x = paramToFloat(values, floats, count, 0);
        paint->m_inputRadialGradientCenter.y = paramToFloat(values, floats, count, 1);
        paint->m_inputRadialGradientFocalPoint.x = paramToFloat(values, floats, count, 2);
        paint->m_inputRadialGradientFocalPoint.y = paramToFloat(values, floats, count, 3);
        paint->m_inputRadialGradientRadius = paramToFloat(values, floats, count, 4);
        paint->m_radialGradientCenter = inputVector2(paint->m_inputRadialGradientCenter);
        paint->m_radialGradientFocalPoint = inputVector2(paint->m_inputRadialGradientFocalPoint);
        paint->m_radialGradientRadius = inputFloat(paint->m_inputRadialGradientRadius);
        break;

    case VG_PAINT_PATTERN_TILING_MODE:
        if (count != 1 || ivalue < VG_TILE_FILL || ivalue > VG_TILE_REFLECT)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        paint->m_patternTilingMode = (VGTilingMode)ivalue;
        break;

    default:
        setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
        break;
    }
}

void VG_APIENTRY vgSetParameterf(VGHandle object, VGint paramType, VGfloat value)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    VGboolean isImage = isValidImage(context, object);
    VGboolean isPath = isValidPath(context, object);
    VGboolean isPaint = isValidPaint(context, object);
    VGboolean isMaskLayer = isValidMaskLayer(context, object);
    VGboolean isFont = isValidFont(context, object);

    VG_IF_ERROR(!isImage && !isPath && !isPaint && !isMaskLayer && !isFont, VG_BAD_HANDLE_ERROR, VG_NO_RETVAL); //invalid object handle
    VG_IF_ERROR(paramType == VG_PAINT_COLOR || paramType == VG_PAINT_COLOR_RAMP_STOPS || paramType == VG_PAINT_LINEAR_GRADIENT ||
                paramType == VG_PAINT_RADIAL_GRADIENT, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL); //vector valued parameter
    VGfloat values[1] = {value};
    if (isImage)
    {    //read only, the function does nothing
        VG_ASSERT(!isPath && !isPaint && !isMaskLayer && !isFont);
        if (paramType < VG_IMAGE_FORMAT || paramType > VG_IMAGE_HEIGHT)
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    else if (isPath)
    {    //read only, the function does nothing
        VG_ASSERT(!isImage && !isPaint && !isMaskLayer && !isFont);
        if (paramType < VG_PATH_FORMAT || paramType > VG_PATH_NUM_COORDS)
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    else if (isPaint)
    {
        VG_ASSERT(!isImage && !isPath && !isMaskLayer && !isFont);
        setPaintParameterifv(context, (Paint*)object, (VGPaintParamType)paramType, 1, values, VG_TRUE);
    }
    else if (isMaskLayer)
    {
        VG_ASSERT(!isImage && !isPath && !isPaint && !isFont);
        setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    else
    {    //read only, the function does nothing
        VG_ASSERT(!isImage && !isPath && !isPaint && !isMaskLayer && isFont);
        if (paramType != VG_FONT_NUM_GLYPHS)
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgSetParameteri(VGHandle object, VGint paramType, VGint value)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VGboolean isImage = isValidImage(context, object);
    VGboolean isPath = isValidPath(context, object);
    VGboolean isPaint = isValidPaint(context, object);
    VGboolean isMaskLayer = isValidMaskLayer(context, object);
    VGboolean isFont = isValidFont(context, object);

    VG_IF_ERROR(!isImage && !isPath && !isPaint && !isMaskLayer && !isFont, VG_BAD_HANDLE_ERROR, VG_NO_RETVAL); //invalid object handle
    VG_IF_ERROR(paramType == VG_PAINT_COLOR || paramType == VG_PAINT_COLOR_RAMP_STOPS || paramType == VG_PAINT_LINEAR_GRADIENT ||
                paramType == VG_PAINT_RADIAL_GRADIENT, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);    //vector valued parameter
    VGint values[1] = {value};
    if (isImage)
    {    //read only, the function does nothing
        VG_ASSERT(!isPath && !isPaint && !isMaskLayer && !isFont);
        if (paramType < VG_IMAGE_FORMAT || paramType > VG_IMAGE_HEIGHT)
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    else if (isPath)
    {    //read only, the function does nothing
        VG_ASSERT(!isImage && !isPaint && !isMaskLayer && !isFont);
        if (paramType < VG_PATH_FORMAT || paramType > VG_PATH_NUM_COORDS)
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    else if (isPaint)
    {
        VG_ASSERT(!isImage && !isPath && !isMaskLayer && !isFont);
        setPaintParameterifv(context, (Paint*)object, (VGPaintParamType)paramType, 1, values, VG_FALSE);
    }
    else if (isMaskLayer)
    {
        VG_ASSERT(!isImage && !isPath && !isPaint && !isFont);
        setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    else
    {    //read only, the function does nothing
        VG_ASSERT(!isImage && !isPath && !isPaint && !isMaskLayer && isFont);
        if (paramType != VG_FONT_NUM_GLYPHS)
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgSetParameterfv(VGHandle object, VGint paramType, VGint count, const VGfloat * values)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(count < 0 || (!values && count > 0) || (values && !isAligned(values,4)), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VGboolean isImage = isValidImage(context, object);
    VGboolean isPath = isValidPath(context, object);
    VGboolean isPaint = isValidPaint(context, object);
    VGboolean isMaskLayer = isValidMaskLayer(context, object);
    VGboolean isFont = isValidFont(context, object);
    VG_IF_ERROR(!isImage && !isPath && !isPaint && !isMaskLayer && !isFont, VG_BAD_HANDLE_ERROR, VG_NO_RETVAL); //invalid object handle
    if (isImage)
    {    //read only, the function does nothing
        VG_ASSERT(!isPath && !isPaint && !isMaskLayer && !isFont);
        if (paramType < VG_IMAGE_FORMAT || paramType > VG_IMAGE_HEIGHT)
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    else if (isPath)
    {    //read only, the function does nothing
        VG_ASSERT(!isImage && !isPaint && !isMaskLayer && !isFont);
        if (paramType < VG_PATH_FORMAT || paramType > VG_PATH_NUM_COORDS)
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    else if (isPaint)
    {
        VG_ASSERT(!isImage && !isPath && !isMaskLayer && !isFont);
        setPaintParameterifv(context, (Paint*)object, (VGPaintParamType)paramType, count, values, VG_TRUE);
    }
    else if (isMaskLayer)
    {
        VG_ASSERT(!isImage && !isPath && !isPaint && !isFont);
        setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    else
    {    //read only, the function does nothing
        VG_ASSERT(!isImage && !isPath && !isPaint && !isMaskLayer && isFont);
        if (paramType != VG_FONT_NUM_GLYPHS)
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgSetParameteriv(VGHandle object, VGint paramType, VGint count, const VGint * values)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(count < 0 || (!values && count > 0) || (values && !isAligned(values,4)), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VGboolean isImage = isValidImage(context, object);
    VGboolean isPath = isValidPath(context, object);
    VGboolean isPaint = isValidPaint(context, object);
    VGboolean isMaskLayer = isValidMaskLayer(context, object);
    VGboolean isFont = isValidFont(context, object);
    VG_IF_ERROR(!isImage && !isPath && !isPaint && !isMaskLayer && !isFont, VG_BAD_HANDLE_ERROR, VG_NO_RETVAL); //invalid object handle
    if (isImage)
    {    //read only, the function does nothing
        VG_ASSERT(!isPath && !isPaint && !isMaskLayer && !isFont);
        if (paramType < VG_IMAGE_FORMAT || paramType > VG_IMAGE_HEIGHT)
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    else if (isPath)
    {    //read only, the function does nothing
        VG_ASSERT(!isImage && !isPaint && !isMaskLayer && !isFont);
        if (paramType < VG_PATH_FORMAT || paramType > VG_PATH_NUM_COORDS)
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    else if (isPaint)
    {
        VG_ASSERT(!isImage && !isPath && !isMaskLayer && !isFont);
        setPaintParameterifv(context, (Paint*)object, (VGPaintParamType)paramType, count, values, VG_FALSE);
    }
    else if (isMaskLayer)
    {
        VG_ASSERT(!isImage && !isPath && !isPaint && !isFont);
        setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    else
    {    //read only, the function does nothing
        VG_ASSERT(!isImage && !isPath && !isPaint && !isMaskLayer && isFont);
        if (paramType != VG_FONT_NUM_GLYPHS)
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
    }
    VG_RETURN(VG_NO_RETVAL);
}

static void getPaintParameterifv(VGContext* context, Paint* paint, VGPaintParamType type, VGint count, void* values, VGboolean floats)
{
    switch(type)
    {
    case VG_PAINT_TYPE:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, paint->m_paintType);
        break;

    case VG_PAINT_COLOR:
        if (count > 4)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        floatToParam(values, floats, count, 0, paint->m_inputPaintColor.r);
        floatToParam(values, floats, count, 1, paint->m_inputPaintColor.g);
        floatToParam(values, floats, count, 2, paint->m_inputPaintColor.b);
        floatToParam(values, floats, count, 3, paint->m_inputPaintColor.a);
        break;

    case VG_PAINT_COLOR_RAMP_SPREAD_MODE:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, paint->m_colorRampSpreadMode);
        break;

    case VG_PAINT_COLOR_RAMP_STOPS:
        {
            if (count > paint->m_inputcolorRampStopsCount *5)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
            VGint j = 0;
            for (int i=0; i<paint->m_inputcolorRampStopsCount; i++)
            {
                floatToParam(values, floats, count, j++, paint->m_inputColorRampStops[i].offset);
                floatToParam(values, floats, count, j++, paint->m_inputColorRampStops[i].color.r);
                floatToParam(values, floats, count, j++, paint->m_inputColorRampStops[i].color.g);
                floatToParam(values, floats, count, j++, paint->m_inputColorRampStops[i].color.b);
                floatToParam(values, floats, count, j++, paint->m_inputColorRampStops[i].color.a);
            }
            break;
        }

    case VG_PAINT_COLOR_RAMP_PREMULTIPLIED:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, paint->m_colorRampPremultiplied);
        break;

    case VG_PAINT_LINEAR_GRADIENT:
        if (count > 4)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        floatToParam(values, floats, count, 0, paint->m_inputLinearGradientPoint0.x);
        floatToParam(values, floats, count, 1, paint->m_inputLinearGradientPoint0.y);
        floatToParam(values, floats, count, 2, paint->m_inputLinearGradientPoint1.x);
        floatToParam(values, floats, count, 3, paint->m_inputLinearGradientPoint1.y);
        break;

    case VG_PAINT_RADIAL_GRADIENT:
        if (count > 5)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        floatToParam(values, floats, count, 0, paint->m_inputRadialGradientCenter.x);
        floatToParam(values, floats, count, 1, paint->m_inputRadialGradientCenter.y);
        floatToParam(values, floats, count, 2, paint->m_inputRadialGradientFocalPoint.x);
        floatToParam(values, floats, count, 3, paint->m_inputRadialGradientFocalPoint.y);
        floatToParam(values, floats, count, 4, paint->m_inputRadialGradientRadius);
        break;

    case VG_PAINT_PATTERN_TILING_MODE:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, paint->m_patternTilingMode);
        break;

    default:
        setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
        break;
    }
}

static void getPathParameterifv(VGContext* context, Path* path, VGPathParamType type, VGint count, void* values, VGboolean floats)
{
    switch(type)
    {
    case VG_PATH_FORMAT:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, path->m_format);
        break;

    case VG_PATH_DATATYPE:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, path->m_datatype);
        break;

    case VG_PATH_SCALE:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        floatToParam(values, floats, count, 0, path->m_scale);
        break;

    case VG_PATH_BIAS:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        floatToParam(values, floats, count, 0, path->m_bias);
        break;

    case VG_PATH_NUM_SEGMENTS:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, path->m_numSegments);
        break;

    case VG_PATH_NUM_COORDS: {
        if (count > 1) { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        VGint numCoordinates = 0;
        VGubyte* seg_ptr = path->m_segments;
        for (VGint i = 0; i < path->m_numSegments; ++i)
            numCoordinates += _commandSize[*seg_ptr++];
        intToParam(values, floats, count, 0, numCoordinates);
        break;
    }

    default:
        setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
        break;
    }
}

static void getImageParameterifv(VGContext* context, Image* image, VGImageParamType type, VGint count, void* values, VGboolean floats)
{
    switch(type)
    {
    case VG_IMAGE_FORMAT:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        VG_ASSERT(isValidImageFormat(image->m_desc.format));
        intToParam(values, floats, count, 0, image->m_desc.format);
        break;

    case VG_IMAGE_WIDTH:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, image->m_width);
        break;

    case VG_IMAGE_HEIGHT:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, image->m_height);
        break;
    default:
        setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
        break;
    }
}

static void getFontParameterifv(VGContext* context, Font* font, VGFontParamType type, VGint count, void* values, VGboolean floats)
{
    switch(type)
    {
    case VG_FONT_NUM_GLYPHS:
        if (count > 1)    { setError(VG_ILLEGAL_ARGUMENT_ERROR); return; }
        intToParam(values, floats, count, 0, getNumGlyphs(font));
        break;

    default:
        setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid VGParamType
        break;
    }
}

static void vglMatrixMultiply(vg_lite_matrix_t* matrix, vg_lite_matrix_t* mult)
{
    vg_lite_matrix_t temp = {0};
    int row, column;

    /* Process all rows. */
    for (row = 0; row < 3; row++) {
        /* Process all columns. */
        for (column = 0; column < 3; column++) {
            /* Compute matrix entry. */
            temp.m[row][column] = (matrix->m[row][0] * mult->m[0][column])
                + (matrix->m[row][1] * mult->m[1][column])
                + (matrix->m[row][2] * mult->m[2][column]);
        }
    }

    /* Copy temporary matrix into result. */
    memcpy(matrix, &temp, sizeof(temp));
}

static void biasScaleTransform(Path* path, VGuint startIndex, VGuint numSegments) {
    float bias = path->m_bias;
    float scale = path->m_scale;
    if (bias == 0.f && scale == 1.f) return;

    VGubyte* seg_ptr = path->m_segments + startIndex;
    VGubyte** data_list = path->m_data + startIndex;
    int8_t* data_ptr_s8;
    int16_t* data_ptr_s16;
    int32_t* data_ptr_s32;
    float* data_ptr_fp32;
    switch (path->m_datatype)
    {
    case VG_PATH_DATATYPE_S_8:
        for (VGuint i = 0; i < numSegments; ++i) {
            data_ptr_s8 = (int8_t*)(*data_list++);
            VGuint command_size = _commandSize[*seg_ptr++];
            for (VGuint j = 0; j < command_size; ++j) {
                *data_ptr_s8 = (int8_t)(*data_ptr_s8 * scale + bias);
                data_ptr_s8++;
            }
        }
        break;

    case VG_PATH_DATATYPE_S_16:
        for (VGuint i = 0; i < numSegments; ++i) {
            data_ptr_s16 = (int16_t*)(*data_list++);
            VGuint command_size = _commandSize[*seg_ptr++];
            for (VGuint j = 0; j < command_size; ++j) {
                *data_ptr_s16 = (int16_t)(*data_ptr_s16 * scale + bias);
                data_ptr_s16++;
            }
        }
        break;

    case VG_PATH_DATATYPE_S_32:
        for (VGuint i = 0; i < numSegments; ++i) {
            data_ptr_s32 = (int32_t*)(*data_list++);
            VGuint command_size = _commandSize[*seg_ptr++];
            for (VGuint j = 0; j < command_size; ++j) {
                *data_ptr_s32 = (int32_t)(*data_ptr_s32 * scale + bias);
                data_ptr_s32++;
            }
        }
        break;

    case VG_PATH_DATATYPE_F:
        for (VGuint i = 0; i < numSegments; ++i) {
            data_ptr_fp32 = (float*)(*data_list++);
            VGuint command_size = _commandSize[*seg_ptr++];
            for (VGuint j = 0; j < command_size; ++j) {
                *data_ptr_fp32 = (float)(*data_ptr_fp32 * scale + bias);
                data_ptr_fp32++;
            }
        }
        break;

    default:
        return;
    }
}

__attribute__((unused)) static VGfloat __calAngle(VGfloat x, VGfloat y) {
    VGfloat ml;
    if (y >= 0) {
        ml = sqrtf(x * x + y * y);
        return acosf(x / ml);
    }
    else {
        ml = sqrtf(x * x + y * y);
        return 2 * PI - acosf(x / ml);
    }
}

__attribute__((unused)) static VGfloat __calTrigonomatricAngle(VGfloat SIN, VGfloat COS) {
    if (COS > 0.f)
        return asinf(SIN);
    else if (SIN > 0.f)
        return acosf(COS);
    else if (COS == 0.f)
        return PI / 3.f * 2.f;
    else if (SIN == 0.f)
        return PI;
    else
        return PI - atanf(SIN / COS);
}

static void pathTransformImpl(Path* d, VGint startIndex, VGint numSegments, Matrix3x3 matrix) {
    VGubyte* seg_ptr = d->m_segments + startIndex;
    VGubyte** data_list = d->m_data + startIndex;
#define convert_to_s8(x) ((int8_t)(x))
#define convert_to_s16(x) ((int16_t)(x))
#define convert_to_s32(x) ((int32_t)(x))
#define convert_to_fp32(x) ((float)(x))
    switch (d->m_datatype) {
    case VG_PATH_DATATYPE_S_8: {
        int8_t* data_ptr_s8, sx_s8, sy_s8, ox_s8, oy_s8, x0_s8, y0_s8, x1_s8, y1_s8, x2_s8, y2_s8, rh_s8, rv_s8, rot_s8;
        sx_s8 = sy_s8 = ox_s8 = oy_s8 = 0;
        for (int i = 0; i < numSegments; ++i) {
            data_ptr_s8 = (int8_t*)(*data_list);
            switch (*seg_ptr)
            {
            case VG_CLOSE_PATH:
                ox_s8 = sx_s8;
                oy_s8 = sy_s8;
                break;

            case VG_MOVE_TO_ABS:
                x0_s8 = data_ptr_s8[0];
                y0_s8 = data_ptr_s8[1];
                sx_s8 = ox_s8 = x0_s8;
                sy_s8 = oy_s8 = y0_s8;
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x0_s8 + matrix.matrix[0][1] * y0_s8 + matrix.matrix[0][2]);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x0_s8 + matrix.matrix[1][1] * y0_s8 + matrix.matrix[1][2]);
                break;

            case VG_MOVE_TO_REL:
                x0_s8 = data_ptr_s8[0];
                y0_s8 = data_ptr_s8[1];
                sx_s8 = ox_s8 = x0_s8 + ox_s8;
                sy_s8 = oy_s8 = y0_s8 + oy_s8;
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x0_s8 + matrix.matrix[0][1] * y0_s8);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x0_s8 + matrix.matrix[1][1] * y0_s8);
                break;

            case VG_LINE_TO_ABS:
                x0_s8 = data_ptr_s8[0];
                y0_s8 = data_ptr_s8[1];
                ox_s8 = x0_s8;
                oy_s8 = y0_s8;
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x0_s8 + matrix.matrix[0][1] * y0_s8 + matrix.matrix[0][2]);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x0_s8 + matrix.matrix[1][1] * y0_s8 + matrix.matrix[1][2]);
                break;

            case VG_LINE_TO_REL:
                x0_s8 = data_ptr_s8[0];
                y0_s8 = data_ptr_s8[1];
                ox_s8 = data_ptr_s8[0] + ox_s8;
                oy_s8 = data_ptr_s8[1] + oy_s8;
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x0_s8 + matrix.matrix[0][1] * y0_s8);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x0_s8 + matrix.matrix[1][1] * y0_s8);
                break;

            case VG_HLINE_TO_ABS:
                x0_s8 = data_ptr_s8[0];
                ox_s8 = x0_s8;

                /* Convert to Line_To_ABS */
                *seg_ptr = VG_LINE_TO_ABS;
                free(data_ptr_s8);
                data_ptr_s8 = malloc(_commandSize[VG_LINE_TO_ABS] * _dataSize[VG_PATH_DATATYPE_S_8]);
                VG_IF_ERROR(data_ptr_s8 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                x0_s8 = ox_s8;
                y0_s8 = oy_s8;
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x0_s8 + matrix.matrix[0][1] * y0_s8 + matrix.matrix[0][2]);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x0_s8 + matrix.matrix[1][1] * y0_s8 + matrix.matrix[1][2]);
                *data_list = (VGubyte*)data_ptr_s8;
                break;

            case VG_HLINE_TO_REL:
                x0_s8 = data_ptr_s8[0];
                ox_s8 = x0_s8 + ox_s8;

                /* Convert to Line_To_REL */
                *seg_ptr = VG_LINE_TO_REL;
                free(data_ptr_s8);
                data_ptr_s8 = malloc(_commandSize[VG_LINE_TO_REL] * _dataSize[VG_PATH_DATATYPE_S_8]);
                VG_IF_ERROR(data_ptr_s8 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x0_s8);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x0_s8);
                *data_list = (VGubyte*)data_ptr_s8;
                break;

            case VG_VLINE_TO_ABS:
                y0_s8 = data_ptr_s8[0];
                oy_s8 = y0_s8;

                /* Convert to Line_To_ABS */
                *seg_ptr = VG_LINE_TO_ABS;
                free(data_ptr_s8);
                data_ptr_s8 = malloc(_commandSize[VG_LINE_TO_ABS] * _dataSize[VG_PATH_DATATYPE_S_8]);
                VG_IF_ERROR(data_ptr_s8 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                x0_s8 = ox_s8;
                y0_s8 = oy_s8;
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x0_s8 + matrix.matrix[0][1] * y0_s8 + matrix.matrix[0][2]);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x0_s8 + matrix.matrix[1][1] * y0_s8 + matrix.matrix[1][2]);
                *data_list = (VGubyte*)data_ptr_s8;
                break;

            case VG_VLINE_TO_REL:
                y0_s8 = data_ptr_s8[0];
                oy_s8 = y0_s8 + oy_s8;

                /* Convert to Line_To_REL */
                *seg_ptr = VG_LINE_TO_REL;
                free(data_ptr_s8);
                data_ptr_s8 = malloc(_commandSize[VG_LINE_TO_REL] * _dataSize[VG_PATH_DATATYPE_S_8]);
                VG_IF_ERROR(data_ptr_s8 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][1] * y0_s8);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][1] * y0_s8);
                *data_list = (VGubyte*)data_ptr_s8;
                break;

            case VG_QUAD_TO_ABS:
                x0_s8 = data_ptr_s8[0];
                y0_s8 = data_ptr_s8[1];
                x1_s8 = data_ptr_s8[2];
                y1_s8 = data_ptr_s8[3];
                ox_s8 = x0_s8;
                oy_s8 = y0_s8;
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x0_s8 + matrix.matrix[0][1] * y0_s8 + matrix.matrix[0][2]);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x0_s8 + matrix.matrix[1][1] * y0_s8 + matrix.matrix[1][2]);
                data_ptr_s8[2] = convert_to_s8(matrix.matrix[0][0] * x1_s8 + matrix.matrix[0][1] * y1_s8 + matrix.matrix[0][2]);
                data_ptr_s8[3] = convert_to_s8(matrix.matrix[1][0] * x1_s8 + matrix.matrix[1][1] * y1_s8 + matrix.matrix[1][2]);
                break;

            case VG_QUAD_TO_REL:
                x0_s8 = data_ptr_s8[0];
                y0_s8 = data_ptr_s8[1];
                x1_s8 = data_ptr_s8[2];
                y1_s8 = data_ptr_s8[3];
                ox_s8 = x0_s8 + ox_s8;
                oy_s8 = oy_s8 + oy_s8;
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x0_s8 + matrix.matrix[0][1] * y0_s8);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x0_s8 + matrix.matrix[1][1] * y0_s8);
                data_ptr_s8[2] = convert_to_s8(matrix.matrix[0][0] * x1_s8 + matrix.matrix[0][1] * y1_s8);
                data_ptr_s8[3] = convert_to_s8(matrix.matrix[1][0] * x1_s8 + matrix.matrix[1][1] * y1_s8);
                break;

            case VG_CUBIC_TO_ABS:
                x0_s8 = data_ptr_s8[0];
                y0_s8 = data_ptr_s8[1];
                x1_s8 = data_ptr_s8[2];
                y1_s8 = data_ptr_s8[3];
                x2_s8 = data_ptr_s8[4];
                y2_s8 = data_ptr_s8[5];
                ox_s8 = x2_s8;
                oy_s8 = y2_s8;
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x0_s8 + matrix.matrix[0][1] * y0_s8 + matrix.matrix[0][2]);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x0_s8 + matrix.matrix[1][1] * y0_s8 + matrix.matrix[1][2]);
                data_ptr_s8[2] = convert_to_s8(matrix.matrix[0][0] * x1_s8 + matrix.matrix[0][1] * y1_s8 + matrix.matrix[0][2]);
                data_ptr_s8[3] = convert_to_s8(matrix.matrix[1][0] * x1_s8 + matrix.matrix[1][1] * y1_s8 + matrix.matrix[1][2]);
                data_ptr_s8[4] = convert_to_s8(matrix.matrix[0][0] * x2_s8 + matrix.matrix[0][1] * y2_s8 + matrix.matrix[0][2]);
                data_ptr_s8[5] = convert_to_s8(matrix.matrix[1][0] * x2_s8 + matrix.matrix[1][1] * y2_s8 + matrix.matrix[1][2]);
                break;

            case VG_CUBIC_TO_REL:
                x0_s8 = data_ptr_s8[0];
                y0_s8 = data_ptr_s8[1];
                x1_s8 = data_ptr_s8[2];
                y1_s8 = data_ptr_s8[3];
                x2_s8 = data_ptr_s8[4];
                y2_s8 = data_ptr_s8[5];
                ox_s8 = x2_s8 + ox_s8;
                oy_s8 = y2_s8 + oy_s8;
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x0_s8 + matrix.matrix[0][1] * y0_s8);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x0_s8 + matrix.matrix[1][1] * y0_s8);
                data_ptr_s8[2] = convert_to_s8(matrix.matrix[0][0] * x1_s8 + matrix.matrix[0][1] * y1_s8);
                data_ptr_s8[3] = convert_to_s8(matrix.matrix[1][0] * x1_s8 + matrix.matrix[1][1] * y1_s8);
                data_ptr_s8[4] = convert_to_s8(matrix.matrix[0][0] * x2_s8 + matrix.matrix[0][1] * y2_s8);
                data_ptr_s8[5] = convert_to_s8(matrix.matrix[1][0] * x2_s8 + matrix.matrix[1][1] * y2_s8);
                break;

            case VG_SQUAD_TO_ABS:
                x1_s8 = data_ptr_s8[0];
                y1_s8 = data_ptr_s8[1];
                ox_s8 = x1_s8;
                oy_s8 = y1_s8;
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x1_s8 + matrix.matrix[0][1] * y1_s8 + matrix.matrix[0][2]);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x1_s8 + matrix.matrix[1][1] * y1_s8 + matrix.matrix[1][2]);
                break;

            case VG_SQUAD_TO_REL:
                x1_s8 = data_ptr_s8[0];
                y1_s8 = data_ptr_s8[1];
                ox_s8 = x1_s8 + ox_s8;
                oy_s8 = y1_s8 + oy_s8;
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x1_s8 + matrix.matrix[0][1] * y1_s8);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x1_s8 + matrix.matrix[1][1] * y1_s8);
                break;

            case VG_SCUBIC_TO_ABS:
                x1_s8 = data_ptr_s8[0];
                y1_s8 = data_ptr_s8[1];
                x2_s8 = data_ptr_s8[2];
                y2_s8 = data_ptr_s8[3];
                ox_s8 = x2_s8;
                oy_s8 = y2_s8;
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x1_s8 + matrix.matrix[0][1] * y1_s8 + matrix.matrix[0][2]);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x1_s8 + matrix.matrix[1][1] * y1_s8 + matrix.matrix[1][2]);
                data_ptr_s8[2] = convert_to_s8(matrix.matrix[0][0] * x2_s8 + matrix.matrix[0][1] * y2_s8 + matrix.matrix[0][2]);
                data_ptr_s8[3] = convert_to_s8(matrix.matrix[1][0] * x2_s8 + matrix.matrix[1][1] * y2_s8 + matrix.matrix[1][2]);
                break;

            case VG_SCUBIC_TO_REL:
                x1_s8 = data_ptr_s8[0];
                y1_s8 = data_ptr_s8[1];
                x2_s8 = data_ptr_s8[2];
                y2_s8 = data_ptr_s8[3];
                ox_s8 = x2_s8 + ox_s8;
                oy_s8 = y2_s8 + oy_s8;
                data_ptr_s8[0] = convert_to_s8(matrix.matrix[0][0] * x1_s8 + matrix.matrix[0][1] * y1_s8);
                data_ptr_s8[1] = convert_to_s8(matrix.matrix[1][0] * x1_s8 + matrix.matrix[1][1] * y1_s8);
                data_ptr_s8[2] = convert_to_s8(matrix.matrix[0][0] * x2_s8 + matrix.matrix[0][1] * y2_s8);
                data_ptr_s8[3] = convert_to_s8(matrix.matrix[1][0] * x2_s8 + matrix.matrix[1][1] * y2_s8);
                break;

            case VG_SCCWARC_TO_ABS:
            case VG_SCWARC_TO_ABS:
            case VG_LCCWARC_TO_ABS:
            case VG_LCWARC_TO_ABS:
                rh_s8 = data_ptr_s8[0];
                rv_s8 = data_ptr_s8[1];
                rot_s8 = data_ptr_s8[2];
                x0_s8 = data_ptr_s8[3];
                y0_s8 = data_ptr_s8[4];
                ox_s8 = x0_s8;
                oy_s8 = y0_s8;
                {
                    float COS, SIN, rot;
                    rot = rot_s8 * PI / 180.f;
                    COS = cosf(rot);
                    SIN = sinf(rot);

                    float p0 = (matrix.matrix[0][0] * COS + matrix.matrix[0][1] * SIN) * rh_s8;
                    float p1 = (matrix.matrix[1][0] * COS + matrix.matrix[1][1] * SIN) * rh_s8;
                    float q0 = (matrix.matrix[1][0] * -SIN + matrix.matrix[1][1] * COS) * rv_s8;
                    float q1 = -(matrix.matrix[0][0] * -SIN + matrix.matrix[0][1] * COS) * rv_s8;

                    VGboolean swapped = VG_FALSE;
                    if (p0 * p0 + p1 * p1 < q0 * q0 + q1 * q1) {
                        float tmp;
                        tmp = p0; p0 = q0; q0 = tmp;    // swap
                        tmp = p1; p1 = q1; q1 = tmp;    // swap
                        swapped = VG_TRUE;
                    }

                    float h0 = (p0 + q0) * 0.5f;
                    float h1 = (p1 + q1) * 0.5f;
                    float hp0 = (p0 - q0) * 0.5f;
                    float hp1 = (p1 - q1) * 0.5f;
                    float hlen = sqrtf(h0 * h0 + h1 * h1);
                    float hplen = sqrtf(hp0 * hp0 + hp1 * hp1);

                    data_ptr_s8[0] = convert_to_s8(hlen + hplen);
                    data_ptr_s8[1] = convert_to_s8(hlen - hplen);

                    h0 = hplen * h0 + hlen * hp0;
                    h1 = hplen * h1 + hlen * hp1;
                    hlen = sqrtf(h0 * h0 + h1 * h1);
                    if (hlen == 0.f)
                        rot = 0.f;
                    else {
                        float l = sqrtf(h0 * h0 + h1 * h1);
                        h0 /= l; h1 /= l;   // normalize
                        rot = acosf(h0);
                        if (h1 < 0.f) rot = 2.f * PI - rot;
                    }
                    if (swapped) rot += PI * 0.5f;

                    data_ptr_s8[2] = convert_to_s8(rot * 180.f / PI);
                }
                data_ptr_s8[3] = convert_to_s8(matrix.matrix[0][0] * x0_s8 + matrix.matrix[0][1] * y0_s8 + matrix.matrix[0][2]);
                data_ptr_s8[4] = convert_to_s8(matrix.matrix[1][0] * x0_s8 + matrix.matrix[1][1] * y0_s8 + matrix.matrix[1][2]);

                /* Flip winding if the determinant is negative. */
                if (getMatrixDeterminant(&matrix) < 0.f) {
                    switch (*seg_ptr)
                    {
                    case VG_SCCWARC_TO_ABS: *seg_ptr = VG_SCWARC_TO_ABS;     break;
                    case VG_SCWARC_TO_ABS:  *seg_ptr = VG_SCCWARC_TO_ABS;    break;
                    case VG_LCCWARC_TO_ABS: *seg_ptr = VG_LCWARC_TO_ABS;     break;
                    case VG_LCWARC_TO_ABS:  *seg_ptr = VG_LCCWARC_TO_ABS;    break;
                    default:                                         break;
                    }
                }
                break;

            case VG_SCCWARC_TO_REL:
            case VG_SCWARC_TO_REL:
            case VG_LCCWARC_TO_REL:
            case VG_LCWARC_TO_REL:
                rh_s8 = data_ptr_s8[0];
                rv_s8 = data_ptr_s8[1];
                rot_s8 = data_ptr_s8[2];
                x0_s8 = data_ptr_s8[3];
                y0_s8 = data_ptr_s8[4];
                ox_s8 = x0_s8 + ox_s8;
                oy_s8 = y0_s8 + oy_s8;
                {
                    float COS, SIN, rot;
                    rot = rot_s8 * PI / 180.f;
                    COS = cosf(rot);
                    SIN = sinf(rot);

                    float p0 = (matrix.matrix[0][0] * COS + matrix.matrix[0][1] * SIN) * rh_s8;
                    float p1 = (matrix.matrix[1][0] * COS + matrix.matrix[1][1] * SIN) * rh_s8;
                    float q0 = (matrix.matrix[1][0] * -SIN + matrix.matrix[1][1] * COS) * rv_s8;
                    float q1 = -(matrix.matrix[0][0] * -SIN + matrix.matrix[0][1] * COS) * rv_s8;

                    VGboolean swapped = VG_FALSE;
                    if (p0 * p0 + p1 * p1 < q0 * q0 + q1 * q1) {
                        float tmp;
                        tmp = p0; p0 = q0; q0 = tmp;    // swap
                        tmp = p1; p1 = q1; q1 = tmp;    // swap
                        swapped = VG_TRUE;
                    }

                    float h0 = (p0 + q0) * 0.5f;
                    float h1 = (p1 + q1) * 0.5f;
                    float hp0 = (p0 - q0) * 0.5f;
                    float hp1 = (p1 - q1) * 0.5f;
                    float hlen = sqrtf(h0 * h0 + h1 * h1);
                    float hplen = sqrtf(hp0 * hp0 + hp1 * hp1);

                    data_ptr_s8[0] = convert_to_s8(hlen + hplen);
                    data_ptr_s8[1] = convert_to_s8(hlen - hplen);

                    h0 = hplen * h0 + hlen * hp0;
                    h1 = hplen * h1 + hlen * hp1;
                    hlen = sqrtf(h0 * h0 + h1 * h1);
                    if (hlen == 0.f)
                        rot = 0.f;
                    else {
                        float l = sqrtf(h0 * h0 + h1 * h1);
                        h0 /= l; h1 /= l;   // normalize
                        rot = acosf(h0);
                        if (h1 < 0.f) rot = 2.f * PI - rot;
                    }
                    if (swapped) rot += PI * 0.5f;

                    data_ptr_s8[2] = convert_to_s8(rot * 180.f / PI);
                }
                data_ptr_s8[3] = convert_to_s8(matrix.matrix[0][0] * x0_s8 + matrix.matrix[0][1] * y0_s8);
                data_ptr_s8[4] = convert_to_s8(matrix.matrix[1][0] * x0_s8 + matrix.matrix[1][1] * y0_s8);

                /* Flip winding if the determinant is negative. */
                if (getMatrixDeterminant(&matrix) < 0.f) {
                    switch (*seg_ptr)
                    {
                    case VG_SCCWARC_TO_REL: *seg_ptr = VG_SCWARC_TO_REL;     break;
                    case VG_SCWARC_TO_REL:  *seg_ptr = VG_SCCWARC_TO_REL;    break;
                    case VG_LCCWARC_TO_REL: *seg_ptr = VG_LCWARC_TO_REL;     break;
                    case VG_LCWARC_TO_REL:  *seg_ptr = VG_LCCWARC_TO_REL;    break;
                    default:                                                 break;
                    }
                }
                break;
            }
            seg_ptr++;
            data_list++;
        }
        break;
    }

    case VG_PATH_DATATYPE_S_16: {
        int16_t* data_ptr_s16, sx_s16, sy_s16, ox_s16, oy_s16, x0_s16, y0_s16, x1_s16, y1_s16, x2_s16, y2_s16, rh_s16, rv_s16, rot_s16;
        sx_s16 = sy_s16 = ox_s16 = oy_s16 = 0;
        for (int i = 0; i < numSegments; ++i) {
            data_ptr_s16 = (int16_t*)(*data_list);
            switch (*seg_ptr)
            {
            case VG_CLOSE_PATH:
                ox_s16 = sx_s16;
                oy_s16 = sy_s16;
                break;

            case VG_MOVE_TO_ABS:
                x0_s16 = data_ptr_s16[0];
                y0_s16 = data_ptr_s16[1];
                sx_s16 = ox_s16 = x0_s16;
                sy_s16 = oy_s16 = y0_s16;
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x0_s16 + matrix.matrix[0][1] * y0_s16 + matrix.matrix[0][2]);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x0_s16 + matrix.matrix[1][1] * y0_s16 + matrix.matrix[1][2]);
                break;

            case VG_MOVE_TO_REL:
                x0_s16 = data_ptr_s16[0];
                y0_s16 = data_ptr_s16[1];
                sx_s16 = ox_s16 = x0_s16 + ox_s16;
                sy_s16 = oy_s16 = y0_s16 + oy_s16;
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x0_s16 + matrix.matrix[0][1] * y0_s16);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x0_s16 + matrix.matrix[1][1] * y0_s16);
                break;

            case VG_LINE_TO_ABS:
                x0_s16 = data_ptr_s16[0];
                y0_s16 = data_ptr_s16[1];
                ox_s16 = x0_s16;
                oy_s16 = y0_s16;
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x0_s16 + matrix.matrix[0][1] * y0_s16 + matrix.matrix[0][2]);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x0_s16 + matrix.matrix[1][1] * y0_s16 + matrix.matrix[1][2]);
                break;

            case VG_LINE_TO_REL:
                x0_s16 = data_ptr_s16[0];
                y0_s16 = data_ptr_s16[1];
                ox_s16 = data_ptr_s16[0] + ox_s16;
                oy_s16 = data_ptr_s16[1] + oy_s16;
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x0_s16 + matrix.matrix[0][1] * y0_s16);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x0_s16 + matrix.matrix[1][1] * y0_s16);
                break;

            case VG_HLINE_TO_ABS:
                x0_s16 = data_ptr_s16[0];
                ox_s16 = x0_s16;

                /* Convert to Line_To_ABS */
                *seg_ptr = VG_LINE_TO_ABS;
                free(data_ptr_s16);
                data_ptr_s16 = malloc(_commandSize[VG_LINE_TO_ABS] * _dataSize[VG_PATH_DATATYPE_S_16]);
                VG_IF_ERROR(data_ptr_s16 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                x0_s16 = ox_s16;
                y0_s16 = oy_s16;
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x0_s16 + matrix.matrix[0][1] * y0_s16 + matrix.matrix[0][2]);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x0_s16 + matrix.matrix[1][1] * y0_s16 + matrix.matrix[1][2]);
                *data_list = (VGubyte*)data_ptr_s16;
                break;

            case VG_HLINE_TO_REL:
                x0_s16 = data_ptr_s16[0];
                ox_s16 = x0_s16 + ox_s16;

                /* Convert to Line_To_REL */
                *seg_ptr = VG_LINE_TO_REL;
                free(data_ptr_s16);
                data_ptr_s16 = malloc(_commandSize[VG_LINE_TO_REL] * _dataSize[VG_PATH_DATATYPE_S_16]);
                VG_IF_ERROR(data_ptr_s16 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x0_s16);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x0_s16);
                *data_list = (VGubyte*)data_ptr_s16;
                break;

            case VG_VLINE_TO_ABS:
                y0_s16 = data_ptr_s16[0];
                oy_s16 = y0_s16;

                /* Convert to Line_To_ABS */
                *seg_ptr = VG_LINE_TO_ABS;
                free(data_ptr_s16);
                data_ptr_s16 = malloc(_commandSize[VG_LINE_TO_ABS] * _dataSize[VG_PATH_DATATYPE_S_16]);
                VG_IF_ERROR(data_ptr_s16 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                x0_s16 = ox_s16;
                y0_s16 = oy_s16;
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x0_s16 + matrix.matrix[0][1] * y0_s16 + matrix.matrix[0][2]);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x0_s16 + matrix.matrix[1][1] * y0_s16 + matrix.matrix[1][2]);
                *data_list = (VGubyte*)data_ptr_s16;
                break;

            case VG_VLINE_TO_REL:
                y0_s16 = data_ptr_s16[0];
                oy_s16 = y0_s16 + oy_s16;

                /* Convert to Line_To_REL */
                *seg_ptr = VG_LINE_TO_REL;
                free(data_ptr_s16);
                data_ptr_s16 = malloc(_commandSize[VG_LINE_TO_REL] * _dataSize[VG_PATH_DATATYPE_S_16]);
                VG_IF_ERROR(data_ptr_s16 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][1] * y0_s16);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][1] * y0_s16);
                *data_list = (VGubyte*)data_ptr_s16;
                break;

            case VG_QUAD_TO_ABS:
                x0_s16 = data_ptr_s16[0];
                y0_s16 = data_ptr_s16[1];
                x1_s16 = data_ptr_s16[2];
                y1_s16 = data_ptr_s16[3];
                ox_s16 = x0_s16;
                oy_s16 = y0_s16;
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x0_s16 + matrix.matrix[0][1] * y0_s16 + matrix.matrix[0][2]);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x0_s16 + matrix.matrix[1][1] * y0_s16 + matrix.matrix[1][2]);
                data_ptr_s16[2] = convert_to_s16(matrix.matrix[0][0] * x1_s16 + matrix.matrix[0][1] * y1_s16 + matrix.matrix[0][2]);
                data_ptr_s16[3] = convert_to_s16(matrix.matrix[1][0] * x1_s16 + matrix.matrix[1][1] * y1_s16 + matrix.matrix[1][2]);
                break;

            case VG_QUAD_TO_REL:
                x0_s16 = data_ptr_s16[0];
                y0_s16 = data_ptr_s16[1];
                x1_s16 = data_ptr_s16[2];
                y1_s16 = data_ptr_s16[3];
                ox_s16 = x0_s16 + ox_s16;
                oy_s16 = oy_s16 + oy_s16;
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x0_s16 + matrix.matrix[0][1] * y0_s16);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x0_s16 + matrix.matrix[1][1] * y0_s16);
                data_ptr_s16[2] = convert_to_s16(matrix.matrix[0][0] * x1_s16 + matrix.matrix[0][1] * y1_s16);
                data_ptr_s16[3] = convert_to_s16(matrix.matrix[1][0] * x1_s16 + matrix.matrix[1][1] * y1_s16);
                break;

            case VG_CUBIC_TO_ABS:
                x0_s16 = data_ptr_s16[0];
                y0_s16 = data_ptr_s16[1];
                x1_s16 = data_ptr_s16[2];
                y1_s16 = data_ptr_s16[3];
                x2_s16 = data_ptr_s16[4];
                y2_s16 = data_ptr_s16[5];
                ox_s16 = x2_s16;
                oy_s16 = y2_s16;
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x0_s16 + matrix.matrix[0][1] * y0_s16 + matrix.matrix[0][2]);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x0_s16 + matrix.matrix[1][1] * y0_s16 + matrix.matrix[1][2]);
                data_ptr_s16[2] = convert_to_s16(matrix.matrix[0][0] * x1_s16 + matrix.matrix[0][1] * y1_s16 + matrix.matrix[0][2]);
                data_ptr_s16[3] = convert_to_s16(matrix.matrix[1][0] * x1_s16 + matrix.matrix[1][1] * y1_s16 + matrix.matrix[1][2]);
                data_ptr_s16[4] = convert_to_s16(matrix.matrix[0][0] * x2_s16 + matrix.matrix[0][1] * y2_s16 + matrix.matrix[0][2]);
                data_ptr_s16[5] = convert_to_s16(matrix.matrix[1][0] * x2_s16 + matrix.matrix[1][1] * y2_s16 + matrix.matrix[1][2]);
                break;

            case VG_CUBIC_TO_REL:
                x0_s16 = data_ptr_s16[0];
                y0_s16 = data_ptr_s16[1];
                x1_s16 = data_ptr_s16[2];
                y1_s16 = data_ptr_s16[3];
                x2_s16 = data_ptr_s16[4];
                y2_s16 = data_ptr_s16[5];
                ox_s16 = x2_s16 + ox_s16;
                oy_s16 = y2_s16 + oy_s16;
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x0_s16 + matrix.matrix[0][1] * y0_s16);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x0_s16 + matrix.matrix[1][1] * y0_s16);
                data_ptr_s16[2] = convert_to_s16(matrix.matrix[0][0] * x1_s16 + matrix.matrix[0][1] * y1_s16);
                data_ptr_s16[3] = convert_to_s16(matrix.matrix[1][0] * x1_s16 + matrix.matrix[1][1] * y1_s16);
                data_ptr_s16[4] = convert_to_s16(matrix.matrix[0][0] * x2_s16 + matrix.matrix[0][1] * y2_s16);
                data_ptr_s16[5] = convert_to_s16(matrix.matrix[1][0] * x2_s16 + matrix.matrix[1][1] * y2_s16);
                break;

            case VG_SQUAD_TO_ABS:
                x1_s16 = data_ptr_s16[0];
                y1_s16 = data_ptr_s16[1];
                ox_s16 = x1_s16;
                oy_s16 = y1_s16;
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x1_s16 + matrix.matrix[0][1] * y1_s16 + matrix.matrix[0][2]);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x1_s16 + matrix.matrix[1][1] * y1_s16 + matrix.matrix[1][2]);
                break;

            case VG_SQUAD_TO_REL:
                x1_s16 = data_ptr_s16[0];
                y1_s16 = data_ptr_s16[1];
                ox_s16 = x1_s16 + ox_s16;
                oy_s16 = y1_s16 + oy_s16;
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x1_s16 + matrix.matrix[0][1] * y1_s16);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x1_s16 + matrix.matrix[1][1] * y1_s16);
                break;

            case VG_SCUBIC_TO_ABS:
                x1_s16 = data_ptr_s16[0];
                y1_s16 = data_ptr_s16[1];
                x2_s16 = data_ptr_s16[2];
                y2_s16 = data_ptr_s16[3];
                ox_s16 = x2_s16;
                oy_s16 = y2_s16;
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x1_s16 + matrix.matrix[0][1] * y1_s16 + matrix.matrix[0][2]);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x1_s16 + matrix.matrix[1][1] * y1_s16 + matrix.matrix[1][2]);
                data_ptr_s16[2] = convert_to_s16(matrix.matrix[0][0] * x2_s16 + matrix.matrix[0][1] * y2_s16 + matrix.matrix[0][2]);
                data_ptr_s16[3] = convert_to_s16(matrix.matrix[1][0] * x2_s16 + matrix.matrix[1][1] * y2_s16 + matrix.matrix[1][2]);
                break;

            case VG_SCUBIC_TO_REL:
                x1_s16 = data_ptr_s16[0];
                y1_s16 = data_ptr_s16[1];
                x2_s16 = data_ptr_s16[2];
                y2_s16 = data_ptr_s16[3];
                ox_s16 = x2_s16 + ox_s16;
                oy_s16 = y2_s16 + oy_s16;
                data_ptr_s16[0] = convert_to_s16(matrix.matrix[0][0] * x1_s16 + matrix.matrix[0][1] * y1_s16);
                data_ptr_s16[1] = convert_to_s16(matrix.matrix[1][0] * x1_s16 + matrix.matrix[1][1] * y1_s16);
                data_ptr_s16[2] = convert_to_s16(matrix.matrix[0][0] * x2_s16 + matrix.matrix[0][1] * y2_s16);
                data_ptr_s16[3] = convert_to_s16(matrix.matrix[1][0] * x2_s16 + matrix.matrix[1][1] * y2_s16);
                break;

            case VG_SCCWARC_TO_ABS:
            case VG_SCWARC_TO_ABS:
            case VG_LCCWARC_TO_ABS:
            case VG_LCWARC_TO_ABS:
                rh_s16 = data_ptr_s16[0];
                rv_s16 = data_ptr_s16[1];
                rot_s16 = data_ptr_s16[2];
                x0_s16 = data_ptr_s16[3];
                y0_s16 = data_ptr_s16[4];
                ox_s16 = x0_s16;
                oy_s16 = y0_s16;
                {
                    float COS, SIN, rot;
                    rot = rot_s16 * PI / 180.f;
                    COS = cosf(rot);
                    SIN = sinf(rot);

                    float p0 = (matrix.matrix[0][0] * COS + matrix.matrix[0][1] * SIN) * rh_s16;
                    float p1 = (matrix.matrix[1][0] * COS + matrix.matrix[1][1] * SIN) * rh_s16;
                    float q0 = (matrix.matrix[1][0] * -SIN + matrix.matrix[1][1] * COS) * rv_s16;
                    float q1 = -(matrix.matrix[0][0] * -SIN + matrix.matrix[0][1] * COS) * rv_s16;

                    VGboolean swapped = VG_FALSE;
                    if (p0 * p0 + p1 * p1 < q0 * q0 + q1 * q1) {
                        float tmp;
                        tmp = p0; p0 = q0; q0 = tmp;    // swap
                        tmp = p1; p1 = q1; q1 = tmp;    // swap
                        swapped = VG_TRUE;
                    }

                    float h0 = (p0 + q0) * 0.5f;
                    float h1 = (p1 + q1) * 0.5f;
                    float hp0 = (p0 - q0) * 0.5f;
                    float hp1 = (p1 - q1) * 0.5f;
                    float hlen = sqrtf(h0 * h0 + h1 * h1);
                    float hplen = sqrtf(hp0 * hp0 + hp1 * hp1);

                    data_ptr_s16[0] = convert_to_s16(hlen + hplen);
                    data_ptr_s16[1] = convert_to_s16(hlen - hplen);

                    h0 = hplen * h0 + hlen * hp0;
                    h1 = hplen * h1 + hlen * hp1;
                    hlen = sqrtf(h0 * h0 + h1 * h1);
                    if (hlen == 0.f)
                        rot = 0.f;
                    else {
                        float l = sqrtf(h0 * h0 + h1 * h1);
                        h0 /= l; h1 /= l;   // normalize
                        rot = acosf(h0);
                        if (h1 < 0.f) rot = 2.f * PI - rot;
                    }
                    if (swapped) rot += PI * 0.5f;

                    data_ptr_s16[2] = convert_to_s16(rot * 180.f / PI);
                }
                data_ptr_s16[3] = convert_to_s16(matrix.matrix[0][0] * x0_s16 + matrix.matrix[0][1] * y0_s16 + matrix.matrix[0][2]);
                data_ptr_s16[4] = convert_to_s16(matrix.matrix[1][0] * x0_s16 + matrix.matrix[1][1] * y0_s16 + matrix.matrix[1][2]);

                /* Flip winding if the determinant is negative. */
                if (getMatrixDeterminant(&matrix) < 0.f) {
                    switch (*seg_ptr)
                    {
                    case VG_SCCWARC_TO_ABS: *seg_ptr = VG_SCWARC_TO_ABS;     break;
                    case VG_SCWARC_TO_ABS:  *seg_ptr = VG_SCCWARC_TO_ABS;    break;
                    case VG_LCCWARC_TO_ABS: *seg_ptr = VG_LCWARC_TO_ABS;     break;
                    case VG_LCWARC_TO_ABS:  *seg_ptr = VG_LCCWARC_TO_ABS;    break;
                    default:                                         break;
                    }
                }
                break;

            case VG_SCCWARC_TO_REL:
            case VG_SCWARC_TO_REL:
            case VG_LCCWARC_TO_REL:
            case VG_LCWARC_TO_REL:
                rh_s16 = data_ptr_s16[0];
                rv_s16 = data_ptr_s16[1];
                rot_s16 = data_ptr_s16[2];
                x0_s16 = data_ptr_s16[3];
                y0_s16 = data_ptr_s16[4];
                ox_s16 = x0_s16 + ox_s16;
                oy_s16 = y0_s16 + oy_s16;
                {
                    float COS, SIN, rot;
                    rot = rot_s16 * PI / 180.f;
                    COS = cosf(rot);
                    SIN = sinf(rot);

                    float p0 = (matrix.matrix[0][0] * COS + matrix.matrix[0][1] * SIN) * rh_s16;
                    float p1 = (matrix.matrix[1][0] * COS + matrix.matrix[1][1] * SIN) * rh_s16;
                    float q0 = (matrix.matrix[1][0] * -SIN + matrix.matrix[1][1] * COS) * rv_s16;
                    float q1 = -(matrix.matrix[0][0] * -SIN + matrix.matrix[0][1] * COS) * rv_s16;

                    VGboolean swapped = VG_FALSE;
                    if (p0 * p0 + p1 * p1 < q0 * q0 + q1 * q1) {
                        float tmp;
                        tmp = p0; p0 = q0; q0 = tmp;    // swap
                        tmp = p1; p1 = q1; q1 = tmp;    // swap
                        swapped = VG_TRUE;
                    }

                    float h0 = (p0 + q0) * 0.5f;
                    float h1 = (p1 + q1) * 0.5f;
                    float hp0 = (p0 - q0) * 0.5f;
                    float hp1 = (p1 - q1) * 0.5f;
                    float hlen = sqrtf(h0 * h0 + h1 * h1);
                    float hplen = sqrtf(hp0 * hp0 + hp1 * hp1);

                    data_ptr_s16[0] = convert_to_s16(hlen + hplen);
                    data_ptr_s16[1] = convert_to_s16(hlen - hplen);

                    h0 = hplen * h0 + hlen * hp0;
                    h1 = hplen * h1 + hlen * hp1;
                    hlen = sqrtf(h0 * h0 + h1 * h1);
                    if (hlen == 0.f)
                        rot = 0.f;
                    else {
                        float l = sqrtf(h0 * h0 + h1 * h1);
                        h0 /= l; h1 /= l;   // normalize
                        rot = acosf(h0);
                        if (h1 < 0.f) rot = 2.f * PI - rot;
                    }
                    if (swapped) rot += PI * 0.5f;

                    data_ptr_s16[2] = convert_to_s16(rot * 180.f / PI);
                }
                data_ptr_s16[3] = convert_to_s16(matrix.matrix[0][0] * x0_s16 + matrix.matrix[0][1] * y0_s16);
                data_ptr_s16[4] = convert_to_s16(matrix.matrix[1][0] * x0_s16 + matrix.matrix[1][1] * y0_s16);

                /* Flip winding if the determinant is negative. */
                if (getMatrixDeterminant(&matrix) < 0.f) {
                    switch (*seg_ptr)
                    {
                    case VG_SCCWARC_TO_REL: *seg_ptr = VG_SCWARC_TO_REL;     break;
                    case VG_SCWARC_TO_REL:  *seg_ptr = VG_SCCWARC_TO_REL;    break;
                    case VG_LCCWARC_TO_REL: *seg_ptr = VG_LCWARC_TO_REL;     break;
                    case VG_LCWARC_TO_REL:  *seg_ptr = VG_LCCWARC_TO_REL;    break;
                    default:                                                 break;
                    }
                }
                break;
            }
            seg_ptr++;
            data_list++;
        }
        break;
    }

    case VG_PATH_DATATYPE_S_32: {
        int32_t* data_ptr_s32, sx_s32, sy_s32, ox_s32, oy_s32, x0_s32, y0_s32, x1_s32, y1_s32, x2_s32, y2_s32, rh_s32, rv_s32, rot_s32;
        sx_s32 = sy_s32 = ox_s32 = oy_s32 = 0;
        for (int i = 0; i < numSegments; ++i) {
            data_ptr_s32 = (int32_t*)(*data_list);
            switch (*seg_ptr)
            {
            case VG_CLOSE_PATH:
                ox_s32 = sx_s32;
                oy_s32 = sy_s32;
                break;

            case VG_MOVE_TO_ABS:
                x0_s32 = data_ptr_s32[0];
                y0_s32 = data_ptr_s32[1];
                sx_s32 = ox_s32 = x0_s32;
                sy_s32 = oy_s32 = y0_s32;
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x0_s32 + matrix.matrix[0][1] * y0_s32 + matrix.matrix[0][2]);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x0_s32 + matrix.matrix[1][1] * y0_s32 + matrix.matrix[1][2]);
                break;

            case VG_MOVE_TO_REL:
                x0_s32 = data_ptr_s32[0];
                y0_s32 = data_ptr_s32[1];
                sx_s32 = ox_s32 = x0_s32 + ox_s32;
                sy_s32 = oy_s32 = y0_s32 + oy_s32;
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x0_s32 + matrix.matrix[0][1] * y0_s32);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x0_s32 + matrix.matrix[1][1] * y0_s32);
                break;

            case VG_LINE_TO_ABS:
                x0_s32 = data_ptr_s32[0];
                y0_s32 = data_ptr_s32[1];
                ox_s32 = x0_s32;
                oy_s32 = y0_s32;
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x0_s32 + matrix.matrix[0][1] * y0_s32 + matrix.matrix[0][2]);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x0_s32 + matrix.matrix[1][1] * y0_s32 + matrix.matrix[1][2]);
                break;

            case VG_LINE_TO_REL:
                x0_s32 = data_ptr_s32[0];
                y0_s32 = data_ptr_s32[1];
                ox_s32 = data_ptr_s32[0] + ox_s32;
                oy_s32 = data_ptr_s32[1] + oy_s32;
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x0_s32 + matrix.matrix[0][1] * y0_s32);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x0_s32 + matrix.matrix[1][1] * y0_s32);
                break;

            case VG_HLINE_TO_ABS:
                x0_s32 = data_ptr_s32[0];
                ox_s32 = x0_s32;

                /* Convert to Line_To_ABS */
                *seg_ptr = VG_LINE_TO_ABS;
                free(data_ptr_s32);
                data_ptr_s32 = malloc(_commandSize[VG_LINE_TO_ABS] * _dataSize[VG_PATH_DATATYPE_S_32]);
                VG_IF_ERROR(data_ptr_s32 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                x0_s32 = ox_s32;
                y0_s32 = oy_s32;
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x0_s32 + matrix.matrix[0][1] * y0_s32 + matrix.matrix[0][2]);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x0_s32 + matrix.matrix[1][1] * y0_s32 + matrix.matrix[1][2]);
                *data_list = (VGubyte*)data_ptr_s32;
                break;

            case VG_HLINE_TO_REL:
                x0_s32 = data_ptr_s32[0];
                ox_s32 = x0_s32 + ox_s32;

                /* Convert to Line_To_REL */
                *seg_ptr = VG_LINE_TO_REL;
                free(data_ptr_s32);
                data_ptr_s32 = malloc(_commandSize[VG_LINE_TO_REL] * _dataSize[VG_PATH_DATATYPE_S_32]);
                VG_IF_ERROR(data_ptr_s32 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x0_s32);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x0_s32);
                *data_list = (VGubyte*)data_ptr_s32;
                break;

            case VG_VLINE_TO_ABS:
                y0_s32 = data_ptr_s32[0];
                oy_s32 = y0_s32;

                /* Convert to Line_To_ABS */
                *seg_ptr = VG_LINE_TO_ABS;
                free(data_ptr_s32);
                data_ptr_s32 = malloc(_commandSize[VG_LINE_TO_ABS] * _dataSize[VG_PATH_DATATYPE_S_32]);
                VG_IF_ERROR(data_ptr_s32 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                x0_s32 = ox_s32;
                y0_s32 = oy_s32;
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x0_s32 + matrix.matrix[0][1] * y0_s32 + matrix.matrix[0][2]);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x0_s32 + matrix.matrix[1][1] * y0_s32 + matrix.matrix[1][2]);
                *data_list = (VGubyte*)data_ptr_s32;
                break;

            case VG_VLINE_TO_REL:
                y0_s32 = data_ptr_s32[0];
                oy_s32 = y0_s32 + oy_s32;

                /* Convert to Line_To_REL */
                *seg_ptr = VG_LINE_TO_REL;
                free(data_ptr_s32);
                data_ptr_s32 = malloc(_commandSize[VG_LINE_TO_REL] * _dataSize[VG_PATH_DATATYPE_S_32]);
                VG_IF_ERROR(data_ptr_s32 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][1] * y0_s32);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][1] * y0_s32);
                *data_list = (VGubyte*)data_ptr_s32;
                break;

            case VG_QUAD_TO_ABS:
                x0_s32 = data_ptr_s32[0];
                y0_s32 = data_ptr_s32[1];
                x1_s32 = data_ptr_s32[2];
                y1_s32 = data_ptr_s32[3];
                ox_s32 = x0_s32;
                oy_s32 = y0_s32;
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x0_s32 + matrix.matrix[0][1] * y0_s32 + matrix.matrix[0][2]);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x0_s32 + matrix.matrix[1][1] * y0_s32 + matrix.matrix[1][2]);
                data_ptr_s32[2] = convert_to_s32(matrix.matrix[0][0] * x1_s32 + matrix.matrix[0][1] * y1_s32 + matrix.matrix[0][2]);
                data_ptr_s32[3] = convert_to_s32(matrix.matrix[1][0] * x1_s32 + matrix.matrix[1][1] * y1_s32 + matrix.matrix[1][2]);
                break;

            case VG_QUAD_TO_REL:
                x0_s32 = data_ptr_s32[0];
                y0_s32 = data_ptr_s32[1];
                x1_s32 = data_ptr_s32[2];
                y1_s32 = data_ptr_s32[3];
                ox_s32 = x0_s32 + ox_s32;
                oy_s32 = oy_s32 + oy_s32;
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x0_s32 + matrix.matrix[0][1] * y0_s32);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x0_s32 + matrix.matrix[1][1] * y0_s32);
                data_ptr_s32[2] = convert_to_s32(matrix.matrix[0][0] * x1_s32 + matrix.matrix[0][1] * y1_s32);
                data_ptr_s32[3] = convert_to_s32(matrix.matrix[1][0] * x1_s32 + matrix.matrix[1][1] * y1_s32);
                break;

            case VG_CUBIC_TO_ABS:
                x0_s32 = data_ptr_s32[0];
                y0_s32 = data_ptr_s32[1];
                x1_s32 = data_ptr_s32[2];
                y1_s32 = data_ptr_s32[3];
                x2_s32 = data_ptr_s32[4];
                y2_s32 = data_ptr_s32[5];
                ox_s32 = x2_s32;
                oy_s32 = y2_s32;
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x0_s32 + matrix.matrix[0][1] * y0_s32 + matrix.matrix[0][2]);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x0_s32 + matrix.matrix[1][1] * y0_s32 + matrix.matrix[1][2]);
                data_ptr_s32[2] = convert_to_s32(matrix.matrix[0][0] * x1_s32 + matrix.matrix[0][1] * y1_s32 + matrix.matrix[0][2]);
                data_ptr_s32[3] = convert_to_s32(matrix.matrix[1][0] * x1_s32 + matrix.matrix[1][1] * y1_s32 + matrix.matrix[1][2]);
                data_ptr_s32[4] = convert_to_s32(matrix.matrix[0][0] * x2_s32 + matrix.matrix[0][1] * y2_s32 + matrix.matrix[0][2]);
                data_ptr_s32[5] = convert_to_s32(matrix.matrix[1][0] * x2_s32 + matrix.matrix[1][1] * y2_s32 + matrix.matrix[1][2]);
                break;

            case VG_CUBIC_TO_REL:
                x0_s32 = data_ptr_s32[0];
                y0_s32 = data_ptr_s32[1];
                x1_s32 = data_ptr_s32[2];
                y1_s32 = data_ptr_s32[3];
                x2_s32 = data_ptr_s32[4];
                y2_s32 = data_ptr_s32[5];
                ox_s32 = x2_s32 + ox_s32;
                oy_s32 = y2_s32 + oy_s32;
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x0_s32 + matrix.matrix[0][1] * y0_s32);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x0_s32 + matrix.matrix[1][1] * y0_s32);
                data_ptr_s32[2] = convert_to_s32(matrix.matrix[0][0] * x1_s32 + matrix.matrix[0][1] * y1_s32);
                data_ptr_s32[3] = convert_to_s32(matrix.matrix[1][0] * x1_s32 + matrix.matrix[1][1] * y1_s32);
                data_ptr_s32[4] = convert_to_s32(matrix.matrix[0][0] * x2_s32 + matrix.matrix[0][1] * y2_s32);
                data_ptr_s32[5] = convert_to_s32(matrix.matrix[1][0] * x2_s32 + matrix.matrix[1][1] * y2_s32);
                break;

            case VG_SQUAD_TO_ABS:
                x1_s32 = data_ptr_s32[0];
                y1_s32 = data_ptr_s32[1];
                ox_s32 = x1_s32;
                oy_s32 = y1_s32;
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x1_s32 + matrix.matrix[0][1] * y1_s32 + matrix.matrix[0][2]);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x1_s32 + matrix.matrix[1][1] * y1_s32 + matrix.matrix[1][2]);
                break;

            case VG_SQUAD_TO_REL:
                x1_s32 = data_ptr_s32[0];
                y1_s32 = data_ptr_s32[1];
                ox_s32 = x1_s32 + ox_s32;
                oy_s32 = y1_s32 + oy_s32;
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x1_s32 + matrix.matrix[0][1] * y1_s32);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x1_s32 + matrix.matrix[1][1] * y1_s32);
                break;

            case VG_SCUBIC_TO_ABS:
                x1_s32 = data_ptr_s32[0];
                y1_s32 = data_ptr_s32[1];
                x2_s32 = data_ptr_s32[2];
                y2_s32 = data_ptr_s32[3];
                ox_s32 = x2_s32;
                oy_s32 = y2_s32;
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x1_s32 + matrix.matrix[0][1] * y1_s32 + matrix.matrix[0][2]);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x1_s32 + matrix.matrix[1][1] * y1_s32 + matrix.matrix[1][2]);
                data_ptr_s32[2] = convert_to_s32(matrix.matrix[0][0] * x2_s32 + matrix.matrix[0][1] * y2_s32 + matrix.matrix[0][2]);
                data_ptr_s32[3] = convert_to_s32(matrix.matrix[1][0] * x2_s32 + matrix.matrix[1][1] * y2_s32 + matrix.matrix[1][2]);
                break;

            case VG_SCUBIC_TO_REL:
                x1_s32 = data_ptr_s32[0];
                y1_s32 = data_ptr_s32[1];
                x2_s32 = data_ptr_s32[2];
                y2_s32 = data_ptr_s32[3];
                ox_s32 = x2_s32 + ox_s32;
                oy_s32 = y2_s32 + oy_s32;
                data_ptr_s32[0] = convert_to_s32(matrix.matrix[0][0] * x1_s32 + matrix.matrix[0][1] * y1_s32);
                data_ptr_s32[1] = convert_to_s32(matrix.matrix[1][0] * x1_s32 + matrix.matrix[1][1] * y1_s32);
                data_ptr_s32[2] = convert_to_s32(matrix.matrix[0][0] * x2_s32 + matrix.matrix[0][1] * y2_s32);
                data_ptr_s32[3] = convert_to_s32(matrix.matrix[1][0] * x2_s32 + matrix.matrix[1][1] * y2_s32);
                break;

            case VG_SCCWARC_TO_ABS:
            case VG_SCWARC_TO_ABS:
            case VG_LCCWARC_TO_ABS:
            case VG_LCWARC_TO_ABS:
                rh_s32 = data_ptr_s32[0];
                rv_s32 = data_ptr_s32[1];
                rot_s32 = data_ptr_s32[2];
                x0_s32 = data_ptr_s32[3];
                y0_s32 = data_ptr_s32[4];
                ox_s32 = x0_s32;
                oy_s32 = y0_s32;
                {
                    float COS, SIN, rot;
                    rot = rot_s32 * PI / 180.f;
                    COS = cosf(rot);
                    SIN = sinf(rot);

                    float p0 = (matrix.matrix[0][0] * COS + matrix.matrix[0][1] * SIN) * rh_s32;
                    float p1 = (matrix.matrix[1][0] * COS + matrix.matrix[1][1] * SIN) * rh_s32;
                    float q0 = (matrix.matrix[1][0] * -SIN + matrix.matrix[1][1] * COS) * rv_s32;
                    float q1 = -(matrix.matrix[0][0] * -SIN + matrix.matrix[0][1] * COS) * rv_s32;

                    VGboolean swapped = VG_FALSE;
                    if (p0 * p0 + p1 * p1 < q0 * q0 + q1 * q1) {
                        float tmp;
                        tmp = p0; p0 = q0; q0 = tmp;    // swap
                        tmp = p1; p1 = q1; q1 = tmp;    // swap
                        swapped = VG_TRUE;
                    }

                    float h0 = (p0 + q0) * 0.5f;
                    float h1 = (p1 + q1) * 0.5f;
                    float hp0 = (p0 - q0) * 0.5f;
                    float hp1 = (p1 - q1) * 0.5f;
                    float hlen = sqrtf(h0 * h0 + h1 * h1);
                    float hplen = sqrtf(hp0 * hp0 + hp1 * hp1);

                    data_ptr_s32[0] = convert_to_s32(hlen + hplen);
                    data_ptr_s32[1] = convert_to_s32(hlen - hplen);

                    h0 = hplen * h0 + hlen * hp0;
                    h1 = hplen * h1 + hlen * hp1;
                    hlen = sqrtf(h0 * h0 + h1 * h1);
                    if (hlen == 0.f)
                        rot = 0.f;
                    else {
                        float l = sqrtf(h0 * h0 + h1 * h1);
                        h0 /= l; h1 /= l;   // normalize
                        rot = acosf(h0);
                        if (h1 < 0.f) rot = 2.f * PI - rot;
                    }
                    if (swapped) rot += PI * 0.5f;

                    data_ptr_s32[2] = convert_to_s32(rot * 180.f / PI);
                }
                data_ptr_s32[3] = convert_to_s32(matrix.matrix[0][0] * x0_s32 + matrix.matrix[0][1] * y0_s32 + matrix.matrix[0][2]);
                data_ptr_s32[4] = convert_to_s32(matrix.matrix[1][0] * x0_s32 + matrix.matrix[1][1] * y0_s32 + matrix.matrix[1][2]);

                /* Flip winding if the determinant is negative. */
                if (getMatrixDeterminant(&matrix) < 0.f) {
                    switch (*seg_ptr)
                    {
                    case VG_SCCWARC_TO_ABS: *seg_ptr = VG_SCWARC_TO_ABS;     break;
                    case VG_SCWARC_TO_ABS:  *seg_ptr = VG_SCCWARC_TO_ABS;    break;
                    case VG_LCCWARC_TO_ABS: *seg_ptr = VG_LCWARC_TO_ABS;     break;
                    case VG_LCWARC_TO_ABS:  *seg_ptr = VG_LCCWARC_TO_ABS;    break;
                    default:                                         break;
                    }
                }
                break;

            case VG_SCCWARC_TO_REL:
            case VG_SCWARC_TO_REL:
            case VG_LCCWARC_TO_REL:
            case VG_LCWARC_TO_REL:
                rh_s32 = data_ptr_s32[0];
                rv_s32 = data_ptr_s32[1];
                rot_s32 = data_ptr_s32[2];
                x0_s32 = data_ptr_s32[3];
                y0_s32 = data_ptr_s32[4];
                ox_s32 = x0_s32 + ox_s32;
                oy_s32 = y0_s32 + oy_s32;
                {
                    float COS, SIN, rot;
                    rot = rot_s32 * PI / 180.f;
                    COS = cosf(rot);
                    SIN = sinf(rot);

                    float p0 = (matrix.matrix[0][0] * COS + matrix.matrix[0][1] * SIN) * rh_s32;
                    float p1 = (matrix.matrix[1][0] * COS + matrix.matrix[1][1] * SIN) * rh_s32;
                    float q0 = (matrix.matrix[1][0] * -SIN + matrix.matrix[1][1] * COS) * rv_s32;
                    float q1 = -(matrix.matrix[0][0] * -SIN + matrix.matrix[0][1] * COS) * rv_s32;

                    VGboolean swapped = VG_FALSE;
                    if (p0 * p0 + p1 * p1 < q0 * q0 + q1 * q1) {
                        float tmp;
                        tmp = p0; p0 = q0; q0 = tmp;    // swap
                        tmp = p1; p1 = q1; q1 = tmp;    // swap
                        swapped = VG_TRUE;
                    }

                    float h0 = (p0 + q0) * 0.5f;
                    float h1 = (p1 + q1) * 0.5f;
                    float hp0 = (p0 - q0) * 0.5f;
                    float hp1 = (p1 - q1) * 0.5f;
                    float hlen = sqrtf(h0 * h0 + h1 * h1);
                    float hplen = sqrtf(hp0 * hp0 + hp1 * hp1);

                    data_ptr_s32[0] = convert_to_s32(hlen + hplen);
                    data_ptr_s32[1] = convert_to_s32(hlen - hplen);

                    h0 = hplen * h0 + hlen * hp0;
                    h1 = hplen * h1 + hlen * hp1;
                    hlen = sqrtf(h0 * h0 + h1 * h1);
                    if (hlen == 0.f)
                        rot = 0.f;
                    else {
                        float l = sqrtf(h0 * h0 + h1 * h1);
                        h0 /= l; h1 /= l;   // normalize
                        rot = acosf(h0);
                        if (h1 < 0.f) rot = 2.f * PI - rot;
                    }
                    if (swapped) rot += PI * 0.5f;

                    data_ptr_s32[2] = convert_to_s32(rot * 180.f / PI);
                }
                data_ptr_s32[3] = convert_to_s32(matrix.matrix[0][0] * x0_s32 + matrix.matrix[0][1] * y0_s32);
                data_ptr_s32[4] = convert_to_s32(matrix.matrix[1][0] * x0_s32 + matrix.matrix[1][1] * y0_s32);

                /* Flip winding if the determinant is negative. */
                if (getMatrixDeterminant(&matrix) < 0.f) {
                    switch (*seg_ptr)
                    {
                    case VG_SCCWARC_TO_REL: *seg_ptr = VG_SCWARC_TO_REL;     break;
                    case VG_SCWARC_TO_REL:  *seg_ptr = VG_SCCWARC_TO_REL;    break;
                    case VG_LCCWARC_TO_REL: *seg_ptr = VG_LCWARC_TO_REL;     break;
                    case VG_LCWARC_TO_REL:  *seg_ptr = VG_LCCWARC_TO_REL;    break;
                    default:                                                 break;
                    }
                }
                break;
            }
            seg_ptr++;
            data_list++;
        }
        break;
    }

    case VG_PATH_DATATYPE_F: {
        float* data_ptr_fp32, sx_fp32, sy_fp32, ox_fp32, oy_fp32, x0_fp32, y0_fp32, x1_fp32, y1_fp32, x2_fp32, y2_fp32, rh_fp32, rv_fp32, rot_fp32;
        sx_fp32 = sy_fp32 = ox_fp32 = oy_fp32 = 0;
        for (int i = 0; i < numSegments; ++i) {
            data_ptr_fp32 = (float*)(*data_list);
            switch (*seg_ptr)
            {
            case VG_CLOSE_PATH:
                ox_fp32 = sx_fp32;
                oy_fp32 = sy_fp32;
                break;

            case VG_MOVE_TO_ABS:
                x0_fp32 = data_ptr_fp32[0];
                y0_fp32 = data_ptr_fp32[1];
                sx_fp32 = ox_fp32 = x0_fp32;
                sy_fp32 = oy_fp32 = y0_fp32;
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x0_fp32 + matrix.matrix[0][1] * y0_fp32 + matrix.matrix[0][2]);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x0_fp32 + matrix.matrix[1][1] * y0_fp32 + matrix.matrix[1][2]);
                break;

            case VG_MOVE_TO_REL:
                x0_fp32 = data_ptr_fp32[0];
                y0_fp32 = data_ptr_fp32[1];
                sx_fp32 = ox_fp32 = x0_fp32 + ox_fp32;
                sy_fp32 = oy_fp32 = y0_fp32 + oy_fp32;
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x0_fp32 + matrix.matrix[0][1] * y0_fp32);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x0_fp32 + matrix.matrix[1][1] * y0_fp32);
                break;

            case VG_LINE_TO_ABS:
                x0_fp32 = data_ptr_fp32[0];
                y0_fp32 = data_ptr_fp32[1];
                ox_fp32 = x0_fp32;
                oy_fp32 = y0_fp32;
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x0_fp32 + matrix.matrix[0][1] * y0_fp32 + matrix.matrix[0][2]);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x0_fp32 + matrix.matrix[1][1] * y0_fp32 + matrix.matrix[1][2]);
                break;

            case VG_LINE_TO_REL:
                x0_fp32 = data_ptr_fp32[0];
                y0_fp32 = data_ptr_fp32[1];
                ox_fp32 = data_ptr_fp32[0] + ox_fp32;
                oy_fp32 = data_ptr_fp32[1] + oy_fp32;
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x0_fp32 + matrix.matrix[0][1] * y0_fp32);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x0_fp32 + matrix.matrix[1][1] * y0_fp32);
                break;

            case VG_HLINE_TO_ABS:
                x0_fp32 = data_ptr_fp32[0];
                ox_fp32 = x0_fp32;

                /* Convert to Line_To_ABS */
                *seg_ptr = VG_LINE_TO_ABS;
                free(data_ptr_fp32);
                data_ptr_fp32 = malloc(_commandSize[VG_LINE_TO_ABS] * _dataSize[VG_PATH_DATATYPE_F]);
                VG_IF_ERROR(data_ptr_fp32 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                x0_fp32 = ox_fp32;
                y0_fp32 = oy_fp32;
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x0_fp32 + matrix.matrix[0][1] * y0_fp32 + matrix.matrix[0][2]);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x0_fp32 + matrix.matrix[1][1] * y0_fp32 + matrix.matrix[1][2]);
                *data_list = (VGubyte*)data_ptr_fp32;
                break;

            case VG_HLINE_TO_REL:
                x0_fp32 = data_ptr_fp32[0];
                ox_fp32 = x0_fp32 + ox_fp32;

                /* Convert to Line_To_REL */
                *seg_ptr = VG_LINE_TO_REL;
                free(data_ptr_fp32);
                data_ptr_fp32 = malloc(_commandSize[VG_LINE_TO_REL] * _dataSize[VG_PATH_DATATYPE_F]);
                VG_IF_ERROR(data_ptr_fp32 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x0_fp32);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x0_fp32);
                *data_list = (VGubyte*)data_ptr_fp32;
                break;

            case VG_VLINE_TO_ABS:
                y0_fp32 = data_ptr_fp32[0];
                oy_fp32 = y0_fp32;

                /* Convert to Line_To_ABS */
                *seg_ptr = VG_LINE_TO_ABS;
                free(data_ptr_fp32);
                data_ptr_fp32 = malloc(_commandSize[VG_LINE_TO_ABS] * _dataSize[VG_PATH_DATATYPE_F]);
                VG_IF_ERROR(data_ptr_fp32 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                x0_fp32 = ox_fp32;
                y0_fp32 = oy_fp32;
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x0_fp32 + matrix.matrix[0][1] * y0_fp32 + matrix.matrix[0][2]);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x0_fp32 + matrix.matrix[1][1] * y0_fp32 + matrix.matrix[1][2]);
                *data_list = (VGubyte*)data_ptr_fp32;
                break;

            case VG_VLINE_TO_REL:
                y0_fp32 = data_ptr_fp32[0];
                oy_fp32 = y0_fp32 + oy_fp32;

                /* Convert to Line_To_REL */
                *seg_ptr = VG_LINE_TO_REL;
                free(data_ptr_fp32);
                data_ptr_fp32 = malloc(_commandSize[VG_LINE_TO_REL] * _dataSize[VG_PATH_DATATYPE_F]);
                VG_IF_ERROR(data_ptr_fp32 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][1] * y0_fp32);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][1] * y0_fp32);
                *data_list = (VGubyte*)data_ptr_fp32;
                break;

            case VG_QUAD_TO_ABS:
                x0_fp32 = data_ptr_fp32[0];
                y0_fp32 = data_ptr_fp32[1];
                x1_fp32 = data_ptr_fp32[2];
                y1_fp32 = data_ptr_fp32[3];
                ox_fp32 = x0_fp32;
                oy_fp32 = y0_fp32;
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x0_fp32 + matrix.matrix[0][1] * y0_fp32 + matrix.matrix[0][2]);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x0_fp32 + matrix.matrix[1][1] * y0_fp32 + matrix.matrix[1][2]);
                data_ptr_fp32[2] = convert_to_fp32(matrix.matrix[0][0] * x1_fp32 + matrix.matrix[0][1] * y1_fp32 + matrix.matrix[0][2]);
                data_ptr_fp32[3] = convert_to_fp32(matrix.matrix[1][0] * x1_fp32 + matrix.matrix[1][1] * y1_fp32 + matrix.matrix[1][2]);
                break;

            case VG_QUAD_TO_REL:
                x0_fp32 = data_ptr_fp32[0];
                y0_fp32 = data_ptr_fp32[1];
                x1_fp32 = data_ptr_fp32[2];
                y1_fp32 = data_ptr_fp32[3];
                ox_fp32 = x0_fp32 + ox_fp32;
                oy_fp32 = oy_fp32 + oy_fp32;
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x0_fp32 + matrix.matrix[0][1] * y0_fp32);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x0_fp32 + matrix.matrix[1][1] * y0_fp32);
                data_ptr_fp32[2] = convert_to_fp32(matrix.matrix[0][0] * x1_fp32 + matrix.matrix[0][1] * y1_fp32);
                data_ptr_fp32[3] = convert_to_fp32(matrix.matrix[1][0] * x1_fp32 + matrix.matrix[1][1] * y1_fp32);
                break;

            case VG_CUBIC_TO_ABS:
                x0_fp32 = data_ptr_fp32[0];
                y0_fp32 = data_ptr_fp32[1];
                x1_fp32 = data_ptr_fp32[2];
                y1_fp32 = data_ptr_fp32[3];
                x2_fp32 = data_ptr_fp32[4];
                y2_fp32 = data_ptr_fp32[5];
                ox_fp32 = x2_fp32;
                oy_fp32 = y2_fp32;
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x0_fp32 + matrix.matrix[0][1] * y0_fp32 + matrix.matrix[0][2]);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x0_fp32 + matrix.matrix[1][1] * y0_fp32 + matrix.matrix[1][2]);
                data_ptr_fp32[2] = convert_to_fp32(matrix.matrix[0][0] * x1_fp32 + matrix.matrix[0][1] * y1_fp32 + matrix.matrix[0][2]);
                data_ptr_fp32[3] = convert_to_fp32(matrix.matrix[1][0] * x1_fp32 + matrix.matrix[1][1] * y1_fp32 + matrix.matrix[1][2]);
                data_ptr_fp32[4] = convert_to_fp32(matrix.matrix[0][0] * x2_fp32 + matrix.matrix[0][1] * y2_fp32 + matrix.matrix[0][2]);
                data_ptr_fp32[5] = convert_to_fp32(matrix.matrix[1][0] * x2_fp32 + matrix.matrix[1][1] * y2_fp32 + matrix.matrix[1][2]);
                break;

            case VG_CUBIC_TO_REL:
                x0_fp32 = data_ptr_fp32[0];
                y0_fp32 = data_ptr_fp32[1];
                x1_fp32 = data_ptr_fp32[2];
                y1_fp32 = data_ptr_fp32[3];
                x2_fp32 = data_ptr_fp32[4];
                y2_fp32 = data_ptr_fp32[5];
                ox_fp32 = x2_fp32 + ox_fp32;
                oy_fp32 = y2_fp32 + oy_fp32;
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x0_fp32 + matrix.matrix[0][1] * y0_fp32);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x0_fp32 + matrix.matrix[1][1] * y0_fp32);
                data_ptr_fp32[2] = convert_to_fp32(matrix.matrix[0][0] * x1_fp32 + matrix.matrix[0][1] * y1_fp32);
                data_ptr_fp32[3] = convert_to_fp32(matrix.matrix[1][0] * x1_fp32 + matrix.matrix[1][1] * y1_fp32);
                data_ptr_fp32[4] = convert_to_fp32(matrix.matrix[0][0] * x2_fp32 + matrix.matrix[0][1] * y2_fp32);
                data_ptr_fp32[5] = convert_to_fp32(matrix.matrix[1][0] * x2_fp32 + matrix.matrix[1][1] * y2_fp32);
                break;

            case VG_SQUAD_TO_ABS:
                x1_fp32 = data_ptr_fp32[0];
                y1_fp32 = data_ptr_fp32[1];
                ox_fp32 = x1_fp32;
                oy_fp32 = y1_fp32;
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x1_fp32 + matrix.matrix[0][1] * y1_fp32 + matrix.matrix[0][2]);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x1_fp32 + matrix.matrix[1][1] * y1_fp32 + matrix.matrix[1][2]);
                break;

            case VG_SQUAD_TO_REL:
                x1_fp32 = data_ptr_fp32[0];
                y1_fp32 = data_ptr_fp32[1];
                ox_fp32 = x1_fp32 + ox_fp32;
                oy_fp32 = y1_fp32 + oy_fp32;
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x1_fp32 + matrix.matrix[0][1] * y1_fp32);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x1_fp32 + matrix.matrix[1][1] * y1_fp32);
                break;

            case VG_SCUBIC_TO_ABS:
                x1_fp32 = data_ptr_fp32[0];
                y1_fp32 = data_ptr_fp32[1];
                x2_fp32 = data_ptr_fp32[2];
                y2_fp32 = data_ptr_fp32[3];
                ox_fp32 = x2_fp32;
                oy_fp32 = y2_fp32;
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x1_fp32 + matrix.matrix[0][1] * y1_fp32 + matrix.matrix[0][2]);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x1_fp32 + matrix.matrix[1][1] * y1_fp32 + matrix.matrix[1][2]);
                data_ptr_fp32[2] = convert_to_fp32(matrix.matrix[0][0] * x2_fp32 + matrix.matrix[0][1] * y2_fp32 + matrix.matrix[0][2]);
                data_ptr_fp32[3] = convert_to_fp32(matrix.matrix[1][0] * x2_fp32 + matrix.matrix[1][1] * y2_fp32 + matrix.matrix[1][2]);
                break;

            case VG_SCUBIC_TO_REL:
                x1_fp32 = data_ptr_fp32[0];
                y1_fp32 = data_ptr_fp32[1];
                x2_fp32 = data_ptr_fp32[2];
                y2_fp32 = data_ptr_fp32[3];
                ox_fp32 = x2_fp32 + ox_fp32;
                oy_fp32 = y2_fp32 + oy_fp32;
                data_ptr_fp32[0] = convert_to_fp32(matrix.matrix[0][0] * x1_fp32 + matrix.matrix[0][1] * y1_fp32);
                data_ptr_fp32[1] = convert_to_fp32(matrix.matrix[1][0] * x1_fp32 + matrix.matrix[1][1] * y1_fp32);
                data_ptr_fp32[2] = convert_to_fp32(matrix.matrix[0][0] * x2_fp32 + matrix.matrix[0][1] * y2_fp32);
                data_ptr_fp32[3] = convert_to_fp32(matrix.matrix[1][0] * x2_fp32 + matrix.matrix[1][1] * y2_fp32);
                break;

            case VG_SCCWARC_TO_ABS:
            case VG_SCWARC_TO_ABS:
            case VG_LCCWARC_TO_ABS:
            case VG_LCWARC_TO_ABS:
                rh_fp32 = data_ptr_fp32[0];
                rv_fp32 = data_ptr_fp32[1];
                rot_fp32 = data_ptr_fp32[2];
                x0_fp32 = data_ptr_fp32[3];
                y0_fp32 = data_ptr_fp32[4];
                ox_fp32 = x0_fp32;
                oy_fp32 = y0_fp32;
                {
                    float COS, SIN;
                    rot_fp32 *= PI / 180.f;
                    COS = cosf(rot_fp32);
                    SIN = sinf(rot_fp32);

                    float p0 = (matrix.matrix[0][0] * COS + matrix.matrix[0][1] * SIN) * rh_fp32;
                    float p1 = (matrix.matrix[1][0] * COS + matrix.matrix[1][1] * SIN) * rh_fp32;
                    float q0 = (matrix.matrix[1][0] * -SIN + matrix.matrix[1][1] * COS) * rv_fp32;
                    float q1 = -(matrix.matrix[0][0] * -SIN + matrix.matrix[0][1] * COS) * rv_fp32;

                    VGboolean swapped = VG_FALSE;
                    if (p0 * p0 + p1 * p1 < q0 * q0 + q1 * q1) {
                        float tmp;
                        tmp = p0; p0 = q0; q0 = tmp;    // swap
                        tmp = p1; p1 = q1; q1 = tmp;    // swap
                        swapped = VG_TRUE;
                    }

                    float h0 = (p0 + q0) * 0.5f;
                    float h1 = (p1 + q1) * 0.5f;
                    float hp0 = (p0 - q0) * 0.5f;
                    float hp1 = (p1 - q1) * 0.5f;
                    float hlen = sqrtf(h0 * h0 + h1 * h1);
                    float hplen = sqrtf(hp0 * hp0 + hp1 * hp1);

                    data_ptr_fp32[0] = convert_to_fp32(hlen + hplen);
                    data_ptr_fp32[1] = convert_to_fp32(hlen - hplen);

                    h0 = hplen * h0 + hlen * hp0;
                    h1 = hplen * h1 + hlen * hp1;
                    hlen = sqrtf(h0 * h0 + h1 * h1);
                    float rot;
                    if (hlen == 0.f)
                        rot = 0.f;
                    else {
                        float l = sqrtf(h0 * h0 + h1 * h1);
                        h0 /= l; h1 /= l;   // normalize
                        rot = acosf(h0);
                        if (h1 < 0.f) rot = 2.f * PI - rot;
                    }
                    if (swapped) rot += PI * 0.5f;

                    data_ptr_fp32[2] = convert_to_fp32(rot * 180.f / PI);
                }
                data_ptr_fp32[3] = convert_to_fp32(matrix.matrix[0][0] * x0_fp32 + matrix.matrix[0][1] * y0_fp32 + matrix.matrix[0][2]);
                data_ptr_fp32[4] = convert_to_fp32(matrix.matrix[1][0] * x0_fp32 + matrix.matrix[1][1] * y0_fp32 + matrix.matrix[1][2]);

                /* Flip winding if the determinant is negative. */
                if (getMatrixDeterminant(&matrix) < 0.f) {
                    switch (*seg_ptr)
                    {
                    case VG_SCCWARC_TO_ABS: *seg_ptr = VG_SCWARC_TO_ABS;     break;
                    case VG_SCWARC_TO_ABS:  *seg_ptr = VG_SCCWARC_TO_ABS;    break;
                    case VG_LCCWARC_TO_ABS: *seg_ptr = VG_LCWARC_TO_ABS;     break;
                    case VG_LCWARC_TO_ABS:  *seg_ptr = VG_LCCWARC_TO_ABS;    break;
                    default:                                         break;
                    }
                }
                break;

            case VG_SCCWARC_TO_REL:
            case VG_SCWARC_TO_REL:
            case VG_LCCWARC_TO_REL:
            case VG_LCWARC_TO_REL:
                rh_fp32 = data_ptr_fp32[0];
                rv_fp32 = data_ptr_fp32[1];
                rot_fp32 = data_ptr_fp32[2];
                x0_fp32 = data_ptr_fp32[3];
                y0_fp32 = data_ptr_fp32[4];
                ox_fp32 = x0_fp32 + ox_fp32;
                oy_fp32 = y0_fp32 + oy_fp32;
                {
                    float COS, SIN;
                    rot_fp32 *= PI / 180.f;
                    COS = cosf(rot_fp32);
                    SIN = sinf(rot_fp32);

                    float p0 = (matrix.matrix[0][0] * COS + matrix.matrix[0][1] * SIN) * rh_fp32;
                    float p1 = (matrix.matrix[1][0] * COS + matrix.matrix[1][1] * SIN) * rh_fp32;
                    float q0 = (matrix.matrix[1][0] * -SIN + matrix.matrix[1][1] * COS) * rv_fp32;
                    float q1 = -(matrix.matrix[0][0] * -SIN + matrix.matrix[0][1] * COS) * rv_fp32;

                    VGboolean swapped = VG_FALSE;
                    if (p0 * p0 + p1 * p1 < q0 * q0 + q1 * q1) {
                        float tmp;
                        tmp = p0; p0 = q0; q0 = tmp;    // swap
                        tmp = p1; p1 = q1; q1 = tmp;    // swap
                        swapped = VG_TRUE;
                    }

                    float h0 = (p0 + q0) * 0.5f;
                    float h1 = (p1 + q1) * 0.5f;
                    float hp0 = (p0 - q0) * 0.5f;
                    float hp1 = (p1 - q1) * 0.5f;
                    float hlen = sqrtf(h0 * h0 + h1 * h1);
                    float hplen = sqrtf(hp0 * hp0 + hp1 * hp1);

                    data_ptr_fp32[0] = convert_to_fp32(hlen + hplen);
                    data_ptr_fp32[1] = convert_to_fp32(hlen - hplen);

                    h0 = hplen * h0 + hlen * hp0;
                    h1 = hplen * h1 + hlen * hp1;
                    hlen = sqrtf(h0 * h0 + h1 * h1);
                    float rot;
                    if (hlen == 0.f)
                        rot = 0.f;
                    else {
                        float l = sqrtf(h0 * h0 + h1 * h1);
                        h0 /= l; h1 /= l;   // normalize
                        rot = acosf(h0);
                        if (h1 < 0.f) rot = 2.f * PI - rot;
                    }
                    if (swapped) rot += PI * 0.5f;

                    data_ptr_fp32[2] = convert_to_fp32(rot * 180.f / PI);
                }
                data_ptr_fp32[3] = convert_to_fp32(matrix.matrix[0][0] * x0_fp32 + matrix.matrix[0][1] * y0_fp32);
                data_ptr_fp32[4] = convert_to_fp32(matrix.matrix[1][0] * x0_fp32 + matrix.matrix[1][1] * y0_fp32);

                /* Flip winding if the determinant is negative. */
                if (getMatrixDeterminant(&matrix) < 0.f) {
                    switch (*seg_ptr)
                    {
                    case VG_SCCWARC_TO_REL: *seg_ptr = VG_SCWARC_TO_REL;     break;
                    case VG_SCWARC_TO_REL:  *seg_ptr = VG_SCCWARC_TO_REL;    break;
                    case VG_LCCWARC_TO_REL: *seg_ptr = VG_LCWARC_TO_REL;     break;
                    case VG_LCWARC_TO_REL:  *seg_ptr = VG_LCCWARC_TO_REL;    break;
                    default:                                                 break;
                    }
                }
                break;
            }
            seg_ptr++;
            data_list++;
        }
        break;
    }

    default:
        break;
    }
#undef convert_to_s8
#undef convert_to_s16
#undef convert_to_s32
#undef convert_to_fp32
}

static void appendPathImpl(Path* dst, Path* src) {
    void* src_segments_copy = malloc(src->m_numSegments * sizeof(VGubyte));
    VG_IF_ERROR(src_segments_copy == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
    memcpy(src_segments_copy, src->m_segments, src->m_numSegments * sizeof(VGubyte));
    void* tmp = realloc(dst->m_segments, (dst->m_numSegments + src->m_numSegments) * sizeof(VGubyte));
    if (tmp == NULL) {
        free(src_segments_copy);
        setError(VG_OUT_OF_MEMORY_ERROR);
        OSReleaseMutex();
        return VG_NO_RETVAL;
    }
    dst->m_segments = tmp;
    memcpy(dst->m_segments + dst->m_numSegments, src_segments_copy, src->m_numSegments * sizeof(VGubyte));
    free(src_segments_copy);

    void* src_data_list_copy = malloc(src->m_numSegments * sizeof(VGubyte*));
    VG_IF_ERROR(src_data_list_copy == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
    memcpy(src_data_list_copy, src->m_data, src->m_numSegments * sizeof(VGubyte*));

    tmp = realloc(dst->m_data, sizeof(VGubyte*) * (dst->m_numSegments + src->m_numSegments));
    if (tmp == NULL) {
        free(src_data_list_copy);
        setError(VG_OUT_OF_MEMORY_ERROR);
        OSReleaseMutex();
        return VG_NO_RETVAL;
    }
    dst->m_data = tmp;

    VGuint grid_size = _dataSize[dst->m_datatype];
    VGuint bytes, commandSize;
    VGubyte* dst_seg_ptr = dst->m_segments + dst->m_numSegments;
    VGubyte** dst_data_list_ptr = dst->m_data + dst->m_numSegments;
    VGubyte** src_data_list_ptr = src_data_list_copy;
    int8_t* dst_data_s8_ptr, * src_data_s8_ptr;
    int16_t* dst_data_s16_ptr, * src_data_s16_ptr;
    int32_t* dst_data_s32_ptr, * src_data_s32_ptr;
    float* dst_data_fp32_ptr, * src_data_fp32_ptr;
    switch (dst->m_datatype)
    {
    case VG_PATH_DATATYPE_S_8:
        switch (src->m_datatype)
        {
        case VG_PATH_DATATYPE_S_8:
            /* src_s8, dst_s8 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                bytes = grid_size * _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(bytes);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                memcpy(*dst_data_list_ptr++, *src_data_list_ptr++, bytes);
            }
            break;

        case VG_PATH_DATATYPE_S_16:
            /* src_s16, dst_s8 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                commandSize = _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(commandSize * grid_size);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                dst_data_s8_ptr = (int8_t*)(*dst_data_list_ptr++);
                src_data_s16_ptr = (int16_t*)(*src_data_list_ptr++);
                for (VGuint j = 0; j < commandSize; ++j) {
                    *dst_data_s8_ptr++ = (int8_t)(*src_data_s16_ptr++);
                }
            }
            break;

        case VG_PATH_DATATYPE_S_32:
            /* src_s32, dst_s8 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                commandSize = _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(commandSize * grid_size);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                dst_data_s8_ptr = (int8_t*)(*dst_data_list_ptr++);
                src_data_s32_ptr = (int32_t*)(*src_data_list_ptr++);
                for (VGuint j = 0; j < commandSize; ++j) {
                    *dst_data_s8_ptr++ = (int8_t)(*src_data_s32_ptr++);
                }
            }
            break;

        case VG_PATH_DATATYPE_F:
            /* src_f32, dst_s8 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                commandSize = _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(commandSize * grid_size);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                dst_data_s8_ptr = (int8_t*)(*dst_data_list_ptr++);
                src_data_fp32_ptr = (float*)(*src_data_list_ptr++);
                for (VGuint j = 0; j < commandSize; ++j) {
                    *dst_data_s8_ptr++ = (int8_t)(*src_data_fp32_ptr++);
                }
            }
            break;

        default:
            break;
        }
        break;

    case VG_PATH_DATATYPE_S_16:
        switch (src->m_datatype)
        {
        case VG_PATH_DATATYPE_S_8:
            /* src_s8, dst_s16 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                commandSize = _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(commandSize * grid_size);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                dst_data_s16_ptr = (int16_t*)(*dst_data_list_ptr++);
                src_data_s8_ptr = (int8_t*)(*src_data_list_ptr++);
                for (VGuint j = 0; j < commandSize; ++j) {
                    *dst_data_s16_ptr++ = (int16_t)(*src_data_s8_ptr++);
                }
            }
            break;

        case VG_PATH_DATATYPE_S_16:
            /* src_s16, dst_s16 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                bytes = grid_size * _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(bytes);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                memcpy(*dst_data_list_ptr++, *src_data_list_ptr++, bytes);
            }
            break;

        case VG_PATH_DATATYPE_S_32:
            /* src_s32, dst_s16 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                commandSize = _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(commandSize * grid_size);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                dst_data_s16_ptr = (int16_t*)(*dst_data_list_ptr++);
                src_data_s32_ptr = (int32_t*)(*src_data_list_ptr++);
                for (VGuint j = 0; j < commandSize; ++j) {
                    *dst_data_s16_ptr++ = (int16_t)(*src_data_s32_ptr++);
                }
            }
            break;

        case VG_PATH_DATATYPE_F:
            /* src_f32, dst_s16 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                commandSize = _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(commandSize * grid_size);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                dst_data_s16_ptr = (int16_t*)(*dst_data_list_ptr++);
                src_data_fp32_ptr = (float*)(*src_data_list_ptr++);
                for (VGuint j = 0; j < commandSize; ++j) {
                    *dst_data_s16_ptr++ = (int16_t)(*src_data_fp32_ptr++);
                }
            }
            break;

        default:
            break;
        }
        break;

    case VG_PATH_DATATYPE_S_32:
        switch (src->m_datatype)
        {
        case VG_PATH_DATATYPE_S_8:
            /* src_s8, dst_s32 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                commandSize = _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(commandSize * grid_size);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                dst_data_s32_ptr = (int32_t*)(*dst_data_list_ptr++);
                src_data_s8_ptr = (int8_t*)(*src_data_list_ptr++);
                for (VGuint j = 0; j < commandSize; ++j) {
                    *dst_data_s32_ptr++ = (int32_t)(*src_data_s8_ptr++);
                }
            }
            break;

        case VG_PATH_DATATYPE_S_16:
            /* src_s16, dst_s32 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                commandSize = _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(commandSize * grid_size);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                dst_data_s32_ptr = (int32_t*)(*dst_data_list_ptr++);
                src_data_s16_ptr = (int16_t*)(*src_data_list_ptr++);
                for (VGuint j = 0; j < commandSize; ++j) {
                    *dst_data_s32_ptr++ = (int32_t)(*src_data_s16_ptr++);
                }
            }
            break;

        case VG_PATH_DATATYPE_S_32:
            /* src_s32, dst_s32 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                bytes = grid_size * _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(bytes);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                memcpy(*dst_data_list_ptr++, *src_data_list_ptr++, bytes);
            }
            break;

        case VG_PATH_DATATYPE_F:
            /* src_f32, dst_s32 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                commandSize = _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(commandSize * grid_size);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                dst_data_s32_ptr = (int32_t*)(*dst_data_list_ptr++);
                src_data_fp32_ptr = (float*)(*src_data_list_ptr++);
                for (VGuint j = 0; j < commandSize; ++j) {
                    *dst_data_s32_ptr++ = (int32_t)(*src_data_fp32_ptr++);
                }
            }
            break;

        default:
            break;
        }
        break;

    case VG_PATH_DATATYPE_F:
        switch (src->m_datatype)
        {
        case VG_PATH_DATATYPE_S_8:
            /* src_s8, dst_fp32 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                commandSize = _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(commandSize * grid_size);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                dst_data_fp32_ptr = (float*)(*dst_data_list_ptr++);
                src_data_s8_ptr = (int8_t*)(*src_data_list_ptr++);
                for (VGuint j = 0; j < commandSize; ++j) {
                    *dst_data_fp32_ptr++ = (float)(*src_data_s8_ptr++);
                }
            }
            break;

        case VG_PATH_DATATYPE_S_16:
            /* src_s16, dst_fp32 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                commandSize = _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(commandSize * grid_size);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                dst_data_fp32_ptr = (float*)(*dst_data_list_ptr++);
                src_data_s16_ptr = (int16_t*)(*src_data_list_ptr++);
                for (VGuint j = 0; j < commandSize; ++j) {
                    *dst_data_fp32_ptr++ = (float)(*src_data_s16_ptr++);
                }
            }
            break;

        case VG_PATH_DATATYPE_S_32:
            /* src_s32, dst_fp32 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                commandSize = _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(commandSize * grid_size);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                dst_data_fp32_ptr = (float*)(*dst_data_list_ptr++);
                src_data_s32_ptr = (int32_t*)(*src_data_list_ptr++);
                for (VGuint j = 0; j < commandSize; ++j) {
                    *dst_data_fp32_ptr++ = (float)(*src_data_s32_ptr++);
                }
            }
            break;

        case VG_PATH_DATATYPE_F:
            /* src_f32, dst_s32 */
            for (VGint i = 0; i < src->m_numSegments; ++i) {
                bytes = grid_size * _commandSize[*dst_seg_ptr++];
                *dst_data_list_ptr = malloc(bytes);
                if(*dst_data_list_ptr == NULL) goto out_of_memory;
                memcpy(*dst_data_list_ptr++, *src_data_list_ptr++, bytes);
            }
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    goto done;

out_of_memory:
    setError(VG_OUT_OF_MEMORY_ERROR);
    OSReleaseMutex();

done:
    free(src_data_list_copy);
}

static void findEllipse(VGfloat* cx, VGfloat* cy, VGfloat* rh, VGfloat* rv, VGfloat* theta, VGfloat* theta_span, 
    VGfloat rot, VGfloat x0, VGfloat y0, VGfloat x1, VGfloat y1, VGboolean is_large, VGboolean is_clockwise, VGboolean is_relative) {
    VGfloat phi = rot * PI / 180.f;
    VGfloat cosPhi = cosf(phi);
    VGfloat sinPhi = sinf(phi);
    VGfloat dxHalf = is_relative ? -x1 / 2.f : (x0 - x1) / 2.f;
    VGfloat dyHalf = is_relative ? -y1 / 2.f : (y0 - y1) / 2.f;
    VGfloat x1Prime = cosPhi * dxHalf + sinPhi * dyHalf;
    VGfloat y1Prime = -sinPhi * dxHalf + cosPhi * dyHalf;
    VGfloat rx = fabsf(*rh);
    VGfloat ry = fabsf(*rv);
    VGfloat x1PrimeSquare = x1Prime * x1Prime;
    VGfloat y1PrimeSquare = y1Prime * y1Prime;
    VGfloat lambda = x1PrimeSquare / (rx * rx) + y1PrimeSquare / (ry * ry);
    if (lambda > 1.0f)
    {
        rx *= sqrtf(lambda);
        ry *= sqrtf(lambda);
    }
    VGfloat rxSquare = rx * rx;
    VGfloat rySquare = ry * ry;
    VGfloat sign = (is_large != is_clockwise) ? -1.f : 1.f;
    VGfloat sq = (rxSquare * rySquare
        - rxSquare * y1PrimeSquare
        - rySquare * x1PrimeSquare
        )
        /
        (rxSquare * y1PrimeSquare
            + rySquare * x1PrimeSquare
            );
    VGfloat signedSq = sign * ((sq < 0) ? 0 : sqrtf(sq));
    VGfloat cxPrime = signedSq * (rx * y1Prime / ry);
    VGfloat cyPrime = signedSq * -(ry * x1Prime / rx);
#define _angle(angle, _Ux, _Uy, _Vx, _Vy) \
    { \
        VGfloat Ux = _Ux, Uy = _Uy, Vx = _Vx, Vy = _Vy; \
        VGfloat  dot, length, cosVal; \
        int32_t sign; \
        dot = Ux * Vx + Uy * Vy; \
        length = (VGfloat)sqrt(Ux * Ux + Uy * Uy) * (VGfloat)sqrt(Vx * Vx + Vy * Vy); \
        sign = (Ux * Vy - Uy * Vx < 0) ? -1 : 1; \
        cosVal = dot / length; \
        cosVal = (((cosVal) < (-1.0f)) ? (-1.0f) : ((cosVal) > (1.0f)) ? (1.0f) : (cosVal)); \
        angle = sign * (VGfloat)acos(cosVal); \
    }
    VGfloat theta1 = 0.f;
    _angle(theta1, 1, 0, (x1Prime - cxPrime) / rx, (y1Prime - cyPrime) / ry);
    theta1 = fmodf(theta1, 2 * PI);
    VGfloat thetaSpan = 0.f;
    _angle(thetaSpan, (x1Prime - cxPrime) / rx, (y1Prime - cyPrime) / ry,
        (-x1Prime - cxPrime) / rx, (-y1Prime - cyPrime) / ry);
#undef _angle
    if (is_clockwise && (thetaSpan > 0))
    {
        thetaSpan -= 2 * PI;
    }
    else if (!is_clockwise && (thetaSpan < 0))
    {
        thetaSpan += 2 * PI;
    }
    thetaSpan = fmodf(thetaSpan, 2 * PI);

    *cx = cxPrime;
    *cy = cyPrime;
    *rh = rx;
    *rv = ry;
    *theta = theta1;
    *theta_span = thetaSpan;
}

static void pointAlongLine(VGfloat x0, VGfloat y0, VGfloat x1, VGfloat y1, VGfloat distance,
    VGfloat* x, VGfloat* y, VGfloat* tangentX, VGfloat* tangentY, VGfloat *factor) {
    VGfloat seg_length = sqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    if (seg_length == 0.f) {    /* (distance == 0 && seg_length == 0) is invalid param. */
        if (x) *x = x0;
        if (y) *y = y0;
        if (tangentX) *tangentX = 1.f;
        if (tangentY) *tangentY = 0.f;
    }
    else {
        *factor = distance / seg_length;
        if (x) *x = x0 + (x1 - x0) * *factor;
        if (y) *y = y0 + (y1 - y0) * *factor;
        if (tangentX) *tangentX = (x1 - x0) / seg_length;
        if (tangentY) *tangentY = (y1 - y0) / seg_length;
    }
}

static void normalizePath(Path* p) {
    VGubyte* seg_ptr = p->m_segments;
    VGubyte** data_list = p->m_data;
    float* data_ptr, px = 0, py = 0, ox = 0, oy = 0, sx = 0, sy = 0, x0 = 0, y0 = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    for (int i = 0; i < p->m_numSegments; ++i) {
        data_ptr = (float*)(*data_list);
        switch (*seg_ptr)
        {
        case VG_CLOSE_PATH:
            px = ox = sx;
            py = oy = sy;
            break;

        case VG_MOVE_TO_ABS:
            x0 = data_ptr[0];
            y0 = data_ptr[1];

            sx = px = ox = x0;
            sy = py = oy = y0;
            break;

        case VG_MOVE_TO_REL:
            x0 = data_ptr[0] + ox;
            y0 = data_ptr[1] + oy;

            /* Convert to VG_MOVE_TO_ABS */
            *seg_ptr = VG_MOVE_TO_ABS;
            data_ptr[0] = x0;
            data_ptr[1] = y0;

            sx = px = ox = x0;
            sy = py = oy = y0;
            break;

        case VG_LINE_TO_ABS:
            x0 = data_ptr[0];
            y0 = data_ptr[1];

            px = ox = x0;
            py = oy = y0;
            break;

        case VG_LINE_TO_REL:
            x0 = data_ptr[0] + ox;
            y0 = data_ptr[1] + oy;

            /* Convert to VG_LINE_TO_ABS */
            *seg_ptr = VG_LINE_TO_ABS;
            data_ptr[0] = x0;
            data_ptr[1] = y0;

            px = ox = x0;
            py = oy = y0;
            break;

        case VG_HLINE_TO_ABS:
            x0 = data_ptr[0];

            /* Convert to VG_LINE_TO_ABS */
            *seg_ptr = VG_LINE_TO_ABS;
            free(data_ptr);
            data_ptr = malloc(_commandSize[VG_LINE_TO_ABS] * _dataSize[VG_PATH_DATATYPE_F]);
            VG_IF_ERROR(data_ptr == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
            data_ptr[0] = x0;
            data_ptr[1] = oy;
            *data_list = (VGubyte*)data_ptr;

            px = x0;
            py = oy;
            ox = x0;
            break;

        case VG_HLINE_TO_REL:
            x0 = data_ptr[0] + ox;

            /* Convert to VG_LINE_TO_ABS */
            *seg_ptr = VG_LINE_TO_ABS;
            free(data_ptr);
            data_ptr = malloc(_commandSize[VG_LINE_TO_ABS] * _dataSize[VG_PATH_DATATYPE_F]);
            VG_IF_ERROR(data_ptr == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
            data_ptr[0] = x0;
            data_ptr[1] = oy;
            *data_list = (VGubyte*)data_ptr;

            px = x0;
            py = oy;
            ox = x0;
            break;

        case VG_VLINE_TO_ABS:
            y0 = data_ptr[0];

            /* Convert to VG_LINE_TO_ABS */
            *seg_ptr = VG_LINE_TO_ABS;
            free(data_ptr);
            data_ptr = malloc(_commandSize[VG_LINE_TO_ABS] * _dataSize[VG_PATH_DATATYPE_F]);
            VG_IF_ERROR(data_ptr == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
            data_ptr[0] = ox;
            data_ptr[1] = y0;
            *data_list = (VGubyte*)data_ptr;

            px = ox;
            py = y0;
            oy = y0;
            break;

        case VG_VLINE_TO_REL:
            y0 = data_ptr[0] + oy;

            /* Convert to VG_LINE_TO_ABS */
            *seg_ptr = VG_LINE_TO_ABS;
            free(data_ptr);
            data_ptr = malloc(_commandSize[VG_LINE_TO_ABS] * _dataSize[VG_PATH_DATATYPE_F]);
            VG_IF_ERROR(data_ptr == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
            data_ptr[0] = ox;
            data_ptr[1] = y0;
            *data_list = (VGubyte*)data_ptr;

            px = ox;
            py = y0;
            oy = y0;
            break;

        case VG_QUAD_TO_ABS:
            x0 = data_ptr[0];
            y0 = data_ptr[1];
            x1 = data_ptr[2];
            y1 = data_ptr[3];

            /* Convert to VG_CUBIC_TO_ABS */
            *seg_ptr = VG_CUBIC_TO_ABS;
            free(data_ptr);
            data_ptr = malloc(_commandSize[VG_CUBIC_TO_ABS] * _dataSize[VG_PATH_DATATYPE_F]);
            VG_IF_ERROR(data_ptr == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
            data_ptr[0] = (ox + 2.f * x0) / 3.f;
            data_ptr[1] = (oy + 2.f * y0) / 3.f;
            data_ptr[2] = (2.f * x0 + x1) / 3.f;
            data_ptr[3] = (2.f * y0 + y1) / 3.f;
            data_ptr[4] = x1;
            data_ptr[5] = y1;
            *data_list = (VGubyte*)data_ptr;

            px = x0;
            py = y0;
            ox = x1;
            oy = y1;
            break;

        case VG_QUAD_TO_REL:
            x0 = data_ptr[0] + ox;
            y0 = data_ptr[1] + oy;
            x1 = data_ptr[2] + ox;
            y1 = data_ptr[3] + oy;

            /* Convert to VG_CUBIC_TO_ABS */
            *seg_ptr = VG_CUBIC_TO_ABS;
            free(data_ptr);
            data_ptr = malloc(_commandSize[VG_CUBIC_TO_ABS] * _dataSize[VG_PATH_DATATYPE_F]);
            VG_IF_ERROR(data_ptr == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
            data_ptr[0] = (ox + 2.f * x0) / 3.f;
            data_ptr[1] = (oy + 2.f * y0) / 3.f;
            data_ptr[2] = (2.f * x0 + x1) / 3.f;
            data_ptr[3] = (2.f * y0 + y1) / 3.f;
            data_ptr[4] = x1;
            data_ptr[5] = y1;
            *data_list = (VGubyte*)data_ptr;

            px = x0;
            py = y0;
            ox = x1;
            oy = y1;
            break;

        case VG_CUBIC_TO_ABS:
            x0 = data_ptr[0];
            y0 = data_ptr[1];
            x1 = data_ptr[2];
            y1 = data_ptr[3];
            x2 = data_ptr[4];
            y2 = data_ptr[5];

            px = x1;
            py = y1;
            ox = x2;
            oy = y2;
            break;

        case VG_CUBIC_TO_REL:
            x0 = data_ptr[0] + ox;
            y0 = data_ptr[1] + oy;
            x1 = data_ptr[2] + ox;
            y1 = data_ptr[3] + oy;
            x2 = data_ptr[4] + ox;
            y2 = data_ptr[5] + oy;

            /* Convert to VG_CUBIC_TO_ABS */
            *seg_ptr = VG_CUBIC_TO_ABS;
            data_ptr[0] = x0;
            data_ptr[1] = y0;
            data_ptr[2] = x1;
            data_ptr[3] = y1;
            data_ptr[4] = x2;
            data_ptr[5] = y2;

            px = x1;
            py = y1;
            ox = x2;
            oy = y2;
            break;

        case VG_SQUAD_TO_ABS:
            x0 = 2.f * ox - px;
            y0 = 2.f * oy - py;
            x1 = data_ptr[0];
            y1 = data_ptr[1];

            /* Convert to VG_CUBIC_TO_ABS */
            *seg_ptr = VG_CUBIC_TO_ABS;
            free(data_ptr);
            data_ptr = malloc(_commandSize[VG_CUBIC_TO_ABS] * _dataSize[VG_PATH_DATATYPE_F]);
            VG_IF_ERROR(data_ptr == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
            data_ptr[0] = (ox + 2.f * x0) / 3.f;
            data_ptr[1] = (oy + 2.f * y0) / 3.f;
            data_ptr[2] = (2.f * x0 + x1) / 3.f;
            data_ptr[3] = (2.f * y0 + y1) / 3.f;
            data_ptr[4] = x1;
            data_ptr[5] = y1;
            *data_list = (VGubyte*)data_ptr;

            px = x0;
            py = y0;
            ox = x1;
            oy = y1;
            break;

        case VG_SQUAD_TO_REL:
            x0 = 2.f * ox - px;
            y0 = 2.f * oy - py;
            x1 = data_ptr[0] + ox;
            y1 = data_ptr[1] + oy;

            /* Convert to VG_CUBIC_TO_ABS */
            *seg_ptr = VG_CUBIC_TO_ABS;
            free(data_ptr);
            data_ptr = malloc(_commandSize[VG_CUBIC_TO_ABS] * _dataSize[VG_PATH_DATATYPE_F]);
            VG_IF_ERROR(data_ptr == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
            data_ptr[0] = (ox + 2.f * x0) / 3.f;
            data_ptr[1] = (oy + 2.f * y0) / 3.f;
            data_ptr[2] = (2.f * x0 + x1) / 3.f;
            data_ptr[3] = (2.f * y0 + y1) / 3.f;
            data_ptr[4] = x1;
            data_ptr[5] = y1;
            *data_list = (VGubyte*)data_ptr;

            px = x0;
            py = y0;
            ox = x1;
            oy = y1;
            break;

        case VG_SCUBIC_TO_ABS:
            x0 = 2.f * ox - px;
            y0 = 2.f * oy - py;
            x1 = data_ptr[0];
            y1 = data_ptr[1];
            x2 = data_ptr[2];
            y2 = data_ptr[3];

            /* Convert to VG_CUBIC_TO_ABS */
            *seg_ptr = VG_CUBIC_TO_ABS;
            free(data_ptr);
            data_ptr = malloc(_commandSize[VG_CUBIC_TO_ABS] * _dataSize[VG_PATH_DATATYPE_F]);
            VG_IF_ERROR(data_ptr == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
            data_ptr[0] = x0;
            data_ptr[1] = y0;
            data_ptr[2] = x1;
            data_ptr[3] = y1;
            data_ptr[4] = x2;
            data_ptr[5] = y2;
            *data_list = (VGubyte*)data_ptr;

            px = x1;
            py = y1;
            ox = x2;
            oy = y2;
            break;

        case VG_SCUBIC_TO_REL:
            x0 = 2.f * ox - px;
            y0 = 2.f * oy - py;
            x1 = data_ptr[0] + ox;
            y1 = data_ptr[1] + oy;
            x2 = data_ptr[2] + ox;
            y2 = data_ptr[3] + oy;

            /* Convert to VG_CUBIC_TO_ABS */
            *seg_ptr = VG_CUBIC_TO_ABS;
            free(data_ptr);
            data_ptr = malloc(_commandSize[VG_CUBIC_TO_ABS] * _dataSize[VG_PATH_DATATYPE_F]);
            VG_IF_ERROR(data_ptr == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
            data_ptr[0] = x0;
            data_ptr[1] = y0;
            data_ptr[2] = x1;
            data_ptr[3] = y1;
            data_ptr[4] = x2;
            data_ptr[5] = y2;
            *data_list = (VGubyte*)data_ptr;

            px = x1;
            py = y1;
            ox = x2;
            oy = y2;
            break;

        case VG_SCCWARC_TO_ABS:
        case VG_SCWARC_TO_ABS:
        case VG_LCCWARC_TO_ABS:
        case VG_LCWARC_TO_ABS:
            x0 = data_ptr[3];
            y0 = data_ptr[4];

            px = ox = x0;
            py = oy = y0;
            break;

        case VG_SCCWARC_TO_REL:
        case VG_SCWARC_TO_REL:
        case VG_LCCWARC_TO_REL:
        case VG_LCWARC_TO_REL:
            x0 = data_ptr[3] + ox;
            y0 = data_ptr[4] + oy;

            px = ox = x0;
            py = oy = y0;
            break;
        }

        seg_ptr++;
        data_list++;
    }
}

static __inline char ptInBound(const float* pt, float* bound) {
    return pt[0] >= bound[0] && pt[0] <= bound[2] && pt[1] >= bound[1] && pt[1] <= bound[3];
}

static void findQuadBound(float* bound, const float* curve) {
    unsigned char i;
    double n, d, t, t2, mt, mt2, b;
    const float* v0 = &curve[0];
    const float* v1 = &curve[2];
    const float* v2 = &curve[4];

    // Start the bounding box by end points
    bound[0] = min(v0[0], v2[0]);
    bound[1] = min(v0[1], v2[1]);
    bound[2] = max(v0[0], v2[0]);
    bound[3] = max(v0[1], v2[1]);

    // Bezier curve fits inside the convex hull of it's control points.
    // If control points are inside the bounds, we're done.
    if (ptInBound(v1, bound))
        return;

    for (i = 0; i < 2; ++i) {
        n = (double)v0[i] - (double)v1[i];
        d = (double)v0[i] - 2.0 * (double)v1[i] + (double)v2[i];
        if (d == 0.f) continue;
        t = n / d;
        if (t >= 1.f || t <= 0.f) continue;
        t2 = t * t;
        mt = 1 - t;
        mt2 = mt * mt;
        b = (double)v0[i] * mt2 + 2.0 * (double)v1[i] * mt * t + (double)v2[i] * t2;
        bound[0 + i] = min(bound[0 + i], (float)b);
        bound[2 + i] = max(bound[2 + i], (float)b);
    }
}

static void findCubicBound(float* bound, const float* curve) {
    unsigned char i, j, count;
    double roots[2], a, b, c, b2ac, t, v, mt;
    const float* v0 = &curve[0];
    const float* v1 = &curve[2];
    const float* v2 = &curve[4];
    const float* v3 = &curve[6];

    // Start the bounding box by end points
    bound[0] = min(v0[0], v3[0]);
    bound[1] = min(v0[1], v3[1]);
    bound[2] = max(v0[0], v3[0]);
    bound[3] = max(v0[1], v3[1]);

    // Bezier curve fits inside the convex hull of it's control points.
    // If control points are inside the bounds, we're done.
    if (ptInBound(v1, bound) && ptInBound(v2, bound))
        return;

    for (i = 0; i < 2; ++i) {
        a = -3.0 * v0[i] + 9.0 * v1[i] - 9.0 * v2[i] + 3.0 * v3[i];
        b = 6.0 * v0[i] - 12.0 * v1[i] + 6.0 * v2[i];
        c = -3.0 * v0[i] + 3.0 * v1[i];
        count = 0;
        if (fabs(a) < VG_FLOAT_EPSILON) {
            if (fabs(b) > VG_FLOAT_EPSILON) {
                t = -c / b;
                if (t > VG_FLOAT_EPSILON && t < 1.0 - VG_FLOAT_EPSILON)
                    roots[count++] = t;
            }
        }
        else {
            b2ac = b * b - 4.0 * c * a;
            if (b2ac > VG_FLOAT_EPSILON) {
                t = (-b + sqrt(b2ac)) / (2.0 * a);
                if (t > VG_FLOAT_EPSILON && t < 1.0 - VG_FLOAT_EPSILON)
                    roots[count++] = t;
                t = (-b - sqrt(b2ac)) / (2.0 * a);
                if (t > VG_FLOAT_EPSILON && t < 1.0 - VG_FLOAT_EPSILON)
                    roots[count++] = t;
            }
        }
        for (j = 0; j < count; j++) {
            t = roots[j];
            mt = 1.0 - t;
            v = mt * mt * mt * v0[i] + 3.0 * mt * mt * t * v1[i] + 
                3.0 * mt * t * t * v2[i] + t * t * t * v3[i];
            bound[0 + i] = min(bound[0 + i], (float)v);
            bound[2 + i] = max(bound[2 + i], (float)v);
        }
    }
}

static uint8_t PackColorComponent(vg_lite_float_t value)
{
    /* Compute the rounded normalized value. */
    vg_lite_float_t rounded = value * 255.0f + 0.5f;

    /* Get the integer part. */
    int32_t roundedInt = (int32_t)rounded;
    /* Clamp to 0..1 range. */
    //uint8_t clamped = (uint8_t)CLAMP(roundedInt, 0, 255);
    uint8_t clamped = (uint8_t)(((roundedInt) < (0)) ? (0) : ((roundedInt) > (255)) ? (255) : (roundedInt));

    /* Return result. */
    return clamped;
}

static void setLinearGrad(vg_lite_ext_linear_gradient_t* grad,
    vg_lite_uint32_t count,
    vg_lite_color_ramp_t* color_ramp,
    vg_lite_linear_gradient_parameter_t linear_gradient,
    vg_lite_gradient_spreadmode_t spread_mode,
    vg_lite_uint8_t pre_multiplied)
{
    static vg_lite_color_ramp_t default_ramp[] =
    {
        {
            0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        },
        {
            1.0f,
            1.0f, 1.0f, 1.0f, 1.0f
        }
    };

    uint32_t i, trg_count;
    vg_lite_float_t prev_stop;
    vg_lite_color_ramp_t* src_ramp;
    vg_lite_color_ramp_t* src_ramp_last;
    vg_lite_color_ramp_t* trg_ramp;

    /* Reset the count. */
    trg_count = 0;

    grad->linear_grad = linear_gradient;
    grad->pre_multiplied = pre_multiplied;
    grad->spread_mode = spread_mode;

    if (!count || count > VLC_MAX_COLOR_RAMP_STOPS || color_ramp == NULL) {
        memcpy(grad->converted_ramp, default_ramp, sizeof(default_ramp));
        grad->converted_length = sizeof(default_ramp) / 5;
        return;
    }

    for (i = 0; i < count; i++)
        grad->color_ramp[i] = color_ramp[i];
    grad->ramp_length = count;

    /* Determine the last source ramp. */
    src_ramp_last
        = grad->color_ramp
        + grad->ramp_length;

    /* Set the initial previous stop. */
    prev_stop = -1;

    /* Reset the count. */
    trg_count = 0;

    /* Walk through the source ramp. */
    for (
        src_ramp = grad->color_ramp, trg_ramp = grad->converted_ramp;
        (src_ramp < src_ramp_last) && (trg_count < VLC_MAX_COLOR_RAMP_STOPS + 2);
        src_ramp += 1
        )
    {
        /* Must be in increasing order. */
        if (src_ramp->stop < prev_stop)
        {
            /* Ignore the entire sequence. */
            trg_count = 0;
            break;
        }

        /* Update the previous stop value. */
        prev_stop = src_ramp->stop;

        /* Must be within [0..1] range. */
        if ((src_ramp->stop < 0.0f) || (src_ramp->stop > 1.0f))
        {
            /* Ignore. */
            continue;
        }

        /* Clamp color. */
        {
            vg_lite_float_t *Source, *Target;
            Source = (((vg_lite_float_t*)src_ramp) + 1);
            Target = (((vg_lite_float_t*)trg_ramp) + 1);
            /* Clamp the color channels. */
            Target[0] = VG_CLAMP(Source[0], 0.0f, 1.f);
            Target[1] = VG_CLAMP(Source[1], 0.0f, 1.f);
            Target[2] = VG_CLAMP(Source[2], 0.0f, 1.f);
            /* Clamp the alpha channel. */
            Target[3] = VG_CLAMP(Source[3], 0.0f, 1.f);
        }

        /* First stop greater then zero? */
        if ((trg_count == 0) && (src_ramp->stop > 0.0f))
        {
            /* Force the first stop to 0.0f. */
            trg_ramp->stop = 0.0f;

            /* Replicate the entry. */
            trg_ramp[1] = *trg_ramp;
            trg_ramp[1].stop = src_ramp->stop;

            /* Advance. */
            trg_ramp += 2;
            trg_count += 2;
        }
        else
        {
            /* Set the stop value. */
            trg_ramp->stop = src_ramp->stop;

            /* Advance. */
            trg_ramp += 1;
            trg_count += 1;
        }
    }

    /* Empty sequence? */
    if (trg_count == 0)
    {
        memcpy(grad->converted_ramp, default_ramp, sizeof(default_ramp));
        grad->converted_length = sizeof(default_ramp) / 5;
    }
    else
    {
        /* The last stop must be at 1.0. */
        if (trg_ramp[-1].stop != 1.0f)
        {
            /* Replicate the last entry. */
            *trg_ramp = trg_ramp[-1];

            /* Force the last stop to 1.0f. */
            trg_ramp->stop = 1.0f;

            /* Update the final entry count. */
            trg_count += 1;
        }

        /* Set new length. */
        grad->converted_length = trg_count;
    }
}

static void updateLinearGrad(VGContext* context, vg_lite_ext_linear_gradient_t* grad, Matrix3x3* user_m) {
    vg_lite_uint32_t ramp_length, width, stop;
    vg_lite_color_ramp_t* ramp;
    vg_lite_uint32_t chip_id;
    vg_lite_float_t x0, y0, x1, y1, dx, dy, length, i;
    uint8_t* bits;
    vg_lite_linear_gradient_parameter_t* grad_param;
    vg_lite_matrix_t* grad_m;

    /* Get shortcuts to the color ramp. */
    ramp_length = grad->converted_length;
    ramp = grad->converted_ramp;
    grad_param = &(grad->linear_grad);
    grad_m = &(grad->matrix);

    /* Compute the grad parameters. */
    x0 = user_m->matrix[0][0] * grad_param->X0 + user_m->matrix[0][1] * grad_param->Y0 + user_m->matrix[0][2];
    y0 = user_m->matrix[1][0] * grad_param->X0 + user_m->matrix[1][1] * grad_param->Y0 + user_m->matrix[1][2];
    x1 = user_m->matrix[0][0] * grad_param->X1 + user_m->matrix[0][1] * grad_param->Y1 + user_m->matrix[0][2];
    y1 = user_m->matrix[1][0] * grad_param->X1 + user_m->matrix[1][1] * grad_param->Y1 + user_m->matrix[1][2];
    dx = x1 - x0;
    dy = y1 - y0;
    length = (vg_lite_float_t)sqrt(dx * dx + dy * dy);
    vg_lite_get_product_info(NULL, &chip_id, NULL);
    if (chip_id == 0x555) {
        width = ramp_length * 256;
    }
    else {
        width = (vg_lite_uint32_t)length + 1;
    }

    // TODO: free old color ramp surface buffer.

    /* Deal with sepcial boundary condition. */
    if (length < VG_FLOAT_EPSILON) {
        /* Compute transform matrix from ramp surface to grad.*/
        vg_lite_identity(grad_m);
        /* Set grad to ramp surface. */
        grad_param->X0 = 0.f;
        grad_param->Y0 = 0.f;
        grad_param->X1 = 1.f;
        grad_param->Y1 = 0.f;

        /* Allocate the color ramp surface. */
        memset(&grad->image, 0, sizeof(vg_lite_buffer_t));
        grad->image.width = 1;
        grad->image.height = 1;
        grad->image.stride = 0;
        grad->image.image_mode = VG_LITE_NONE_IMAGE_MODE;
        grad->image.format = VG_LITE_ABGR8888;
        vg_lite_allocate(&grad->image);

        /* Set pointer to color array. */
        bits = (uint8_t*)grad->image.memory;
        /* Fill the color array */
        vg_lite_float_t color[4];
        stop = (grad->spread_mode == VG_LITE_GRADIENT_SPREAD_REPEAT) ? 0 : (ramp_length - 1);
        color[3] = ramp[stop].alpha;
        color[2] = ramp[stop].blue;
        color[1] = ramp[stop].green;
        color[0] = ramp[stop].red;
        
        *bits++ = PackColorComponent(color[3]);
        *bits++ = PackColorComponent(color[2]);
        *bits++ = PackColorComponent(color[1]);
        *bits++ = PackColorComponent(color[0]);
        return;
    }

    /* Compute transform matrix from ramp surface to grad.*/
    vg_lite_identity(grad_m);
    vg_lite_translate(x0, y0, grad_m);
    vg_lite_rotate(
        ((dy >= 0) ? acosf(dx / length) : (2 * PI - acosf(dx / length))) * 180.f / PI,
        grad_m
    );
    vg_lite_scale(length / width, 1.f, grad_m);

    /* Set grad to ramp surface. */
    grad_param->X0 = 0.f;
    grad_param->Y0 = 0.f;
    grad_param->X1 = (float)width;
    grad_param->Y1 = 0.f;

    /* Allocate the color ramp surface. */
    memset(&grad->image, 0, sizeof(vg_lite_buffer_t));
    grad->image.width = width;
    grad->image.height = 1;
    grad->image.stride = 0;
    grad->image.image_mode = VG_LITE_NONE_IMAGE_MODE;
    grad->image.format = VG_LITE_ABGR8888;
    vg_lite_allocate(&grad->image);
    memset(grad->image.memory, 0, grad->image.stride * grad->image.height);

    /* Set pointer to color array. */
    bits = (uint8_t*)grad->image.memory;

    /* Start filling the color array. */
    stop = 0;
    vg_lite_float_t gradient;
    vg_lite_float_t color[4];
    vg_lite_float_t color1[4];
    vg_lite_float_t color2[4];
    vg_lite_float_t weight, baseLength;
    baseLength = (float)width;
    for (i = 0; i < width; ++i) {
        /* Compute gradient for current color array entry. */
        if (baseLength > VG_FLOAT_EPSILON) {
            if (chip_id == 0x555) {
                gradient = i / (baseLength);
            }
            else {
                gradient = i / (baseLength - 1);
            }

        }
        else {
            if (grad->spread_mode == VG_LITE_GRADIENT_SPREAD_REPEAT)
                gradient = 0.f;
            else
                gradient = 1.f;
        }

        /* Find the entry in the color ramp that matches or exceeds this
        ** gradient. */
        while (gradient > ramp[stop].stop)
            if (stop < ramp_length - 1) {
                ++stop;
            }
            else {  // exceeding
                gradient = ramp[stop].stop;
                break;
            }

        if (gradient == ramp[stop].stop || stop == 0)
        {
            /* Perfect match weight 1.0. */
            weight = 1.0f;

            /* Use color ramp color. */
            color1[3] = ramp[stop].alpha;
            color1[2] = ramp[stop].blue;
            color1[1] = ramp[stop].green;
            color1[0] = ramp[stop].red;

            color2[3] =
                color2[2] =
                color2[1] =
                color2[0] = 0.0f;
        }
        else
        {
            /* Compute weight. */
            weight = (ramp[stop].stop - gradient)
                / (ramp[stop].stop - ramp[stop - 1].stop);

            /* Grab color ramp color of previous stop. */
            color1[3] = ramp[stop - 1].alpha;
            color1[2] = ramp[stop - 1].blue;
            color1[1] = ramp[stop - 1].green;
            color1[0] = ramp[stop - 1].red;

            /* Grab color ramp color of current stop. */
            color2[3] = ramp[stop].alpha;
            color2[2] = ramp[stop].blue;
            color2[1] = ramp[stop].green;
            color2[0] = ramp[stop].red;
        }

        if (grad->pre_multiplied)
        {
            /* Pre-multiply the first color. */
            color1[2] *= color1[3];
            color1[1] *= color1[3];
            color1[0] *= color1[3];

            /* Pre-multiply the second color. */
            color2[2] *= color2[3];
            color2[1] *= color2[3];
            color2[0] *= color2[3];
        }

#define LERP(v1, v2, w)             ((v1) * (w) + (v2) * (1.0f - (w)))
        /* Filter the colors per channel. */
        color[3] = LERP(color1[3], color2[3], weight);
        color[2] = LERP(color1[2], color2[2], weight);
        color[1] = LERP(color1[1], color2[1], weight);
        color[0] = LERP(color1[0], color2[0], weight);
#undef LERP

        if (grad->pre_multiplied) {
            /* Pre-multiply the final color. */
            color[2] /= color[3];
            color[1] /= color[3];
            color[0] /= color[3];
        }
        *bits++ = PackColorComponent(color[3]);
        *bits++ = PackColorComponent(color[2]);
        *bits++ = PackColorComponent(color[1]);
        *bits++ = PackColorComponent(color[0]);
    }
}

static void setRadialGrad(vg_lite_radial_gradient_t* grad,
    vg_lite_uint32_t count,
    vg_lite_color_ramp_t* color_ramp,
    vg_lite_radial_gradient_parameter_t radial_grad,
    vg_lite_gradient_spreadmode_t spread_mode,
    vg_lite_uint8_t pre_multiplied)
{
    static vg_lite_color_ramp_t defaultRamp[] =
    {
        {
            0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        },
        {
            1.0f,
            1.0f, 1.0f, 1.0f, 1.0f
        }
    };

    uint32_t i, trgCount;
    vg_lite_float_t prevStop;
    vg_lite_color_ramp_t* srcRamp;
    vg_lite_color_ramp_t* srcRampLast;
    vg_lite_color_ramp_t* trgRamp;

    /* Reset the count. */
    trgCount = 0;

    grad->radial_grad = radial_grad;
    grad->pre_multiplied = pre_multiplied;
    grad->spread_mode = spread_mode;

    if (!count || count > VLC_MAX_COLOR_RAMP_STOPS || color_ramp == NULL)
    {
        memcpy(grad->converted_ramp, defaultRamp, sizeof(defaultRamp));
        grad->converted_length = sizeof(defaultRamp) / 5;
        return;
    }

    for (i = 0; i < count; i++)
        grad->color_ramp[i] = color_ramp[i];
    grad->ramp_length = count;

    /* Determine the last source ramp. */
    srcRampLast
        = grad->color_ramp
        + grad->ramp_length;

    /* Set the initial previous stop. */
    prevStop = -1;

    /* Reset the count. */
    trgCount = 0;

    /* Walk through the source ramp. */
    for (
        srcRamp = grad->color_ramp, trgRamp = grad->converted_ramp;
        (srcRamp < srcRampLast) && (trgCount < VLC_MAX_COLOR_RAMP_STOPS + 2);
        srcRamp += 1
        )
    {
        /* Must be in increasing order. */
        if (srcRamp->stop < prevStop)
        {
            /* Ignore the entire sequence. */
            trgCount = 0;
            break;
        }

        /* Update the previous stop value. */
        prevStop = srcRamp->stop;

        /* Must be within [0..1] range. */
        if ((srcRamp->stop < 0.0f) || (srcRamp->stop > 1.0f))
        {
            /* Ignore. */
            continue;
        }

        /* Clamp color. */
        {
            vg_lite_float_t* Source, * Target;
            Source = (((vg_lite_float_t*)srcRamp) + 1);
            Target = (((vg_lite_float_t*)trgRamp) + 1);
            /* Clamp the color channels. */
            Target[0] = VG_CLAMP(Source[0], 0.0f, 1.f);
            Target[1] = VG_CLAMP(Source[1], 0.0f, 1.f);
            Target[2] = VG_CLAMP(Source[2], 0.0f, 1.f);
            /* Clamp the alpha channel. */
            Target[3] = VG_CLAMP(Source[3], 0.0f, 1.f);
        }

        /* First stop greater then zero? */
        if ((trgCount == 0) && (srcRamp->stop > 0.0f))
        {
            /* Force the first stop to 0.0f. */
            trgRamp->stop = 0.0f;

            /* Replicate the entry. */
            trgRamp[1] = *trgRamp;
            trgRamp[1].stop = srcRamp->stop;

            /* Advance. */
            trgRamp += 2;
            trgCount += 2;
        }
        else
        {
            /* Set the stop value. */
            trgRamp->stop = srcRamp->stop;

            /* Advance. */
            trgRamp += 1;
            trgCount += 1;
        }
    }

    /* Empty sequence? */
    if (trgCount == 0)
    {
        memcpy(grad->converted_ramp, defaultRamp, sizeof(defaultRamp));
        grad->converted_length = sizeof(defaultRamp) / 5;
    }
    else
    {
        /* The last stop must be at 1.0. */
        if (trgRamp[-1].stop != 1.0f)
        {
            /* Replicate the last entry. */
            *trgRamp = trgRamp[-1];

            /* Force the last stop to 1.0f. */
            trgRamp->stop = 1.0f;

            /* Update the final entry count. */
            trgCount += 1;
        }

        /* Set new length. */
        grad->converted_length = trgCount;
    }
}

static void updateRadialGrad(VGContext* context, vg_lite_radial_gradient_t* grad, Matrix3x3* user_m) {
    uint32_t ramp_length;
    vg_lite_color_ramp_t* ramp;
    uint32_t common, stop;
    uint32_t i, width;
    uint8_t* bits;
    vg_lite_float_t r;
    vg_lite_matrix_t* grad_m;
    vg_lite_radial_gradient_parameter_t* grad_param;
    vg_lite_matrix_t tmp_m;

    /* Get shortcuts to the color ramp. */
    ramp_length = grad->converted_length;
    ramp = grad->converted_ramp;
    grad_m = &grad->matrix;
    grad_param = &grad->radial_grad;
    r = grad_param->r;

    // TODO: free old color ramp surface buffer.

    /* Deal with sepcial boundary condition. */
    if (r < VG_FLOAT_EPSILON) {
        /* Compute transform matrix from ramp surface to grad.*/
        vg_lite_identity(grad_m);
        /* Set grad to ramp surface. */
        grad_param->cx = 0.f;
        grad_param->cy = 0.f;
        grad_param->fx = 0.f;
        grad_param->fy = 0.f;

        /* Allocate the color ramp surface. */
        memset(&grad->image, 0, sizeof(vg_lite_buffer_t));
        grad->image.width = 1;
        grad->image.height = 1;
        grad->image.stride = 0;
        grad->image.image_mode = VG_LITE_NONE_IMAGE_MODE;
        grad->image.format = VG_LITE_ABGR8888;
        vg_lite_allocate(&grad->image);

        /* Set pointer to color array. */
        bits = (uint8_t*)grad->image.memory;
        /* Fill the color array */
        vg_lite_float_t color[4];
        stop = (grad->spread_mode == VG_LITE_GRADIENT_SPREAD_REPEAT) ? 0 : (ramp_length - 1);
        color[3] = ramp[stop].alpha;
        color[2] = ramp[stop].blue;
        color[1] = ramp[stop].green;
        color[0] = ramp[stop].red;
       
        *bits++ = PackColorComponent(color[3]);
        *bits++ = PackColorComponent(color[2]);
        *bits++ = PackColorComponent(color[1]);
        *bits++ = PackColorComponent(color[0]);
        return;
    }

    /* Compute color ramp surface parameters. */
    if (grad->radial_grad.r < 1)
    {
        common = 1;
    }
    else 
    {
        common = (uint32_t)grad->radial_grad.r;
    }

    for (i = 0; i < ramp_length; ++i)
    {
        if (ramp[i].stop != 0.0f)
        {
            vg_lite_float_t mul = common * ramp[i].stop;
            vg_lite_float_t frac = mul - (vg_lite_float_t)floor(mul);
            if (frac > 0.00013f)    /* Suppose error for zero is 0.00013 */
            {
                common = VG_INT_MAX(common, (uint32_t)(1.0f / frac + 0.5f));
            }
        }
    }

    /* Compute the width of the required color array. */
    width = common + 1;
    width = (width + 15) & (~0xf);

    /* Set gradient matrix. */
    vg_lite_identity(grad_m);
    tmp_m.m[0][0] = user_m->matrix[0][0];
    tmp_m.m[0][1] = user_m->matrix[0][1];
    tmp_m.m[0][2] = user_m->matrix[0][2];
    tmp_m.m[1][0] = user_m->matrix[1][0];
    tmp_m.m[1][1] = user_m->matrix[1][1];
    tmp_m.m[1][2] = user_m->matrix[1][2];
    tmp_m.m[2][0] = user_m->matrix[2][0];
    tmp_m.m[2][1] = user_m->matrix[2][1];
    tmp_m.m[2][2] = user_m->matrix[2][2];
    vglMatrixMultiply(grad_m, &tmp_m);

    /* Allocate the color ramp surface. */
    memset(&grad->image, 0, sizeof(vg_lite_buffer_t));
    grad->image.width = width;
    grad->image.height = 1;
    grad->image.stride = 0;
    grad->image.image_mode = VG_LITE_NONE_IMAGE_MODE;
    grad->image.format = VG_LITE_ABGR8888;
    vg_lite_allocate(&grad->image);
    memset(grad->image.memory, 0, grad->image.stride * grad->image.height);

    /* Set pointer to color array. */
    bits = (uint8_t*)grad->image.memory;

    /* Start filling the color array. */
    stop = 0;
    vg_lite_float_t gradient;
    vg_lite_float_t color[4];
    vg_lite_float_t color1[4];
    vg_lite_float_t color2[4];
    vg_lite_float_t weight, baseLength;
    baseLength = (float)width;
    for (i = 0; i < width; ++i) {
        /* Compute gradient for current color array entry. */
        if (baseLength > VG_FLOAT_EPSILON) {
            gradient = i / baseLength;
        }
        else {
            if (grad->spread_mode == VG_LITE_GRADIENT_SPREAD_REPEAT)
                gradient = 0.f;
            else
                gradient = 1.f;
        }

        /* Find the entry in the color ramp that matches or exceeds this
        ** gradient. */
        while (gradient > ramp[stop].stop)
            if (stop < ramp_length - 1) {
                ++stop;
            }
            else {  // exceeding
                gradient = ramp[stop].stop;
                break;
            }

        if (gradient == ramp[stop].stop || stop == 0)
        {
            /* Perfect match weight 1.0. */
            weight = 1.0f;

            /* Use color ramp color. */
            color1[3] = ramp[stop].alpha;
            color1[2] = ramp[stop].blue;
            color1[1] = ramp[stop].green;
            color1[0] = ramp[stop].red;

            color2[3] =
                color2[2] =
                color2[1] =
                color2[0] = 0.0f;
        }
        else
        {
            /* Compute weight. */
            weight = (ramp[stop].stop - gradient)
                / (ramp[stop].stop - ramp[stop - 1].stop);

            /* Grab color ramp color of previous stop. */
            color1[3] = ramp[stop - 1].alpha;
            color1[2] = ramp[stop - 1].blue;
            color1[1] = ramp[stop - 1].green;
            color1[0] = ramp[stop - 1].red;

            /* Grab color ramp color of current stop. */
            color2[3] = ramp[stop].alpha;
            color2[2] = ramp[stop].blue;
            color2[1] = ramp[stop].green;
            color2[0] = ramp[stop].red;
        }

        if (grad->pre_multiplied)
        {
            /* Pre-multiply the first color. */
            color1[2] *= color1[3];
            color1[1] *= color1[3];
            color1[0] *= color1[3];

            /* Pre-multiply the second color. */
            color2[2] *= color2[3];
            color2[1] *= color2[3];
            color2[0] *= color2[3];
        }

#define LERP(v1, v2, w)             ((v1) * (w) + (v2) * (1.0f - (w)))
        /* Filter the colors per channel. */
        color[3] = LERP(color1[3], color2[3], weight);
        color[2] = LERP(color1[2], color2[2], weight);
        color[1] = LERP(color1[1], color2[1], weight);
        color[0] = LERP(color1[0], color2[0], weight);
#undef LERP

        if (grad->pre_multiplied)
        {
            color[2] /= color[3];
            color[1] /= color[3];
            color[0] /= color[3];
        }
        *bits++ = PackColorComponent(color[3]);
        *bits++ = PackColorComponent(color[2]);
        *bits++ = PackColorComponent(color[1]);
        *bits++ = PackColorComponent(color[0]);
    }
}

VGfloat VG_APIENTRY vgGetParameterf(VGHandle object, VGint paramType)
{
    VG_GET_CONTEXT(0.0f);
    VG_IF_ERROR(paramType == VG_PAINT_COLOR || paramType == VG_PAINT_COLOR_RAMP_STOPS || paramType == VG_PAINT_LINEAR_GRADIENT ||
                paramType == VG_PAINT_RADIAL_GRADIENT, VG_ILLEGAL_ARGUMENT_ERROR, 0.0f);    //vector valued parameter
    VGboolean isImage = isValidImage(context, object);
    VGboolean isPath = isValidPath(context, object);
    VGboolean isPaint = isValidPaint(context, object);
    VGboolean isFont = isValidFont(context, object);
    VG_IF_ERROR(!isImage && !isPath && !isPaint && !isFont, VG_BAD_HANDLE_ERROR, 0.0f);    //invalid object handle
    VGfloat ret = 0.0f;
    if (isImage)
    {
        VG_ASSERT(!isPath && !isPaint && !isFont);
        getImageParameterifv(context, (Image*)object, (VGImageParamType)paramType, 1, &ret, VG_TRUE);
    }
    else if (isPath)
    {
        VG_ASSERT(!isImage && !isPaint && !isFont);
        getPathParameterifv(context, (Path*)object, (VGPathParamType)paramType, 1, &ret, VG_TRUE);
    }
    else if (isPaint)
    {
        VG_ASSERT(!isImage && !isPath && !isFont);
        getPaintParameterifv(context, (Paint*)object, (VGPaintParamType)paramType, 1, &ret, VG_TRUE);
    }
    else
    {
        VG_ASSERT(!isImage && !isPath && !isPaint && isFont);
        getFontParameterifv(context, (Font*)object, (VGFontParamType)paramType, 1, &ret, VG_TRUE);
    }
    VG_RETURN(ret);
}

VGint VG_APIENTRY vgGetParameteri(VGHandle object, VGint paramType)
{
    VG_GET_CONTEXT(0);
    VG_IF_ERROR(paramType == VG_PAINT_COLOR || paramType == VG_PAINT_COLOR_RAMP_STOPS || paramType == VG_PAINT_LINEAR_GRADIENT ||
                paramType == VG_PAINT_RADIAL_GRADIENT, VG_ILLEGAL_ARGUMENT_ERROR, 0);    //vector valued parameter
    VGboolean isImage = isValidImage(context, object);
    VGboolean isPath = isValidPath(context, object);
    VGboolean isPaint = isValidPaint(context, object);
    VGboolean isFont = isValidFont(context, object);

    VG_IF_ERROR(!isImage && !isPath && !isPaint && !isFont, VG_BAD_HANDLE_ERROR, 0);    //invalid object handle
    VGint ret = 0;
    if (isImage)
    {
        VG_ASSERT(!isPath && !isPaint && !isFont);
        getImageParameterifv(context, (Image*)object, (VGImageParamType)paramType, 1, &ret, VG_FALSE);
    }
    else if (isPath)
    {
        VG_ASSERT(!isImage && !isPaint && !isFont);
        getPathParameterifv(context, (Path*)object, (VGPathParamType)paramType, 1, &ret, VG_FALSE);
    }
    else if (isPaint)
    {
        VG_ASSERT(!isImage && !isPath && !isFont);
        getPaintParameterifv(context, (Paint*)object, (VGPaintParamType)paramType, 1, &ret, VG_FALSE);
    }
    else
    {
        VG_ASSERT(!isImage && !isPath && !isPaint && isFont);
        getFontParameterifv(context, (Font*)object, (VGFontParamType)paramType, 1, &ret, VG_FALSE);
    }
    VG_RETURN(ret);
}

void VG_APIENTRY vgGetParameterfv(VGHandle object, VGint paramType, VGint count, VGfloat * values)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(count <= 0 || !values || !isAligned(values,4), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VGboolean isImage = isValidImage(context, object);
    VGboolean isPath = isValidPath(context, object);
    VGboolean isPaint = isValidPaint(context, object);
    VGboolean isFont = isValidFont(context, object);
    VG_IF_ERROR(!isImage && !isPath && !isPaint && !isFont, VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid object handle
    if (isImage)
    {
        VG_ASSERT(!isPath && !isPaint && !isFont);
        getImageParameterifv(context, (Image*)object, (VGImageParamType)paramType, count, values, VG_TRUE);
    }
    else if (isPath)
    {
        VG_ASSERT(!isImage && !isPaint && !isFont);
        getPathParameterifv(context, (Path*)object, (VGPathParamType)paramType, count, values, VG_TRUE);
    }
    else if (isPaint)
    {
        VG_ASSERT(!isImage && !isPath && !isFont);
        getPaintParameterifv(context, (Paint*)object, (VGPaintParamType)paramType, count, values, VG_TRUE);
    }
    else
    {
        VG_ASSERT(!isImage && !isPath && !isPaint && isFont);
        getFontParameterifv(context, (Font*)object, (VGFontParamType)paramType, count, values, VG_TRUE);
    }
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgGetParameteriv(VGHandle object, VGint paramType, VGint count, VGint * values)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(count <= 0 || !values || !isAligned(values,4), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VGboolean isImage = isValidImage(context, object);
    VGboolean isPath = isValidPath(context, object);
    VGboolean isPaint = isValidPaint(context, object);
    VGboolean isFont = isValidFont(context, object);
    VG_IF_ERROR(!isImage && !isPath && !isPaint && !isFont, VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid object handle
    if (isImage)
    {
        VG_ASSERT(!isPath && !isPaint && !isFont);
        getImageParameterifv(context, (Image*)object, (VGImageParamType)paramType, count, values, VG_FALSE);
    }
    else if (isPath)
    {
        VG_ASSERT(!isImage && !isPaint && !isFont);
        getPathParameterifv(context, (Path*)object, (VGPathParamType)paramType, count, values, VG_FALSE);
    }
    else if (isPaint)
    {
        VG_ASSERT(!isImage && !isPath && !isFont);
        getPaintParameterifv(context, (Paint*)object, (VGPaintParamType)paramType, count, values, VG_FALSE);
    }
    else
    {
        VG_ASSERT(!isImage && !isPath && !isPaint && isFont);
        getFontParameterifv(context, (Font*)object, (VGFontParamType)paramType, count, values, VG_FALSE);
    }
    VG_RETURN(VG_NO_RETVAL);
}

VGint VG_APIENTRY vgGetParameterVectorSize(VGHandle object, VGint paramType)
{
    VG_GET_CONTEXT(0);
    VGboolean isImage = isValidImage(context, object);
    VGboolean isPath = isValidPath(context, object);
    VGboolean isPaint = isValidPaint(context, object);
    VGboolean isFont = isValidFont(context, object);
    VG_IF_ERROR(!isImage && !isPath && !isPaint && !isFont, VG_BAD_HANDLE_ERROR, 0);    //invalid object handle
    int ret = 0;
    if (isImage)
    {
        VG_ASSERT(!isPath && !isPaint && !isFont);
        switch(paramType)
        {
        case VG_IMAGE_FORMAT:
        case VG_IMAGE_WIDTH:
        case VG_IMAGE_HEIGHT:
            ret = 1;
            break;

        default:
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid paramType
            break;
        }
    }
    else if (isPath)
    {
        VG_ASSERT(!isImage && !isPaint && !isFont);
        switch(paramType)
        {
        case VG_PATH_FORMAT:
        case VG_PATH_DATATYPE:
        case VG_PATH_SCALE:
        case VG_PATH_BIAS:
        case VG_PATH_NUM_SEGMENTS:
        case VG_PATH_NUM_COORDS:
            ret = 1;
            break;

        default:
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid paramType
            break;
        }
    }
    else if (isPaint)
    {
        VG_ASSERT(!isImage && !isPath && !isFont);
        switch(paramType)
        {
        case VG_PAINT_TYPE:
        case VG_PAINT_COLOR_RAMP_SPREAD_MODE:
        case VG_PAINT_PATTERN_TILING_MODE:
            ret = 1;
            break;

        case VG_PAINT_COLOR:
        case VG_PAINT_LINEAR_GRADIENT:
            ret = 4;
            break;

        case VG_PAINT_COLOR_RAMP_STOPS:
            ret = ((Paint*)object)->m_inputcolorRampStopsCount * 5;
            break;

        case VG_PAINT_COLOR_RAMP_PREMULTIPLIED:
            ret = 1;
            break;

        case VG_PAINT_RADIAL_GRADIENT:
            ret = 5;
            break;
            
        default:
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid paramType
            break;
        }
    }
    else
    {
        VG_ASSERT(!isImage && !isPath && !isPaint && isFont);
        switch(paramType)
        {
        case VG_FONT_NUM_GLYPHS:
            ret = 1;
            break;

        default:
            setError(VG_ILLEGAL_ARGUMENT_ERROR);    //invalid paramType
            break;
        }
    }
    VG_RETURN(ret);
}

static Matrix3x3* getCurrentMatrix(VGContext* context)
{
    VG_ASSERT(context);
    switch(context->m_matrixMode)
    {
    case VG_MATRIX_PATH_USER_TO_SURFACE:
        return &context->m_pathUserToSurface;

    case VG_MATRIX_IMAGE_USER_TO_SURFACE:
        return &context->m_imageUserToSurface;

    case VG_MATRIX_FILL_PAINT_TO_USER:
        return &context->m_fillPaintToUser;

    case VG_MATRIX_STROKE_PAINT_TO_USER:
        return &context->m_strokePaintToUser;

    default:
        VG_ASSERT(context->m_matrixMode == VG_MATRIX_GLYPH_USER_TO_SURFACE);
        return &context->m_glyphUserToSurface;
    }
}

void VG_APIENTRY vgLoadIdentity(void)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    Matrix3x3* d = getCurrentMatrix(context);

    // d->identity();
    d->matrix[0][0] = 1.0f;
    d->matrix[0][1] = 0.0f;
    d->matrix[0][2] = 0.0f;
    d->matrix[1][0] = 0.0f;
    d->matrix[1][1] = 1.0f;
    d->matrix[1][2] = 0.0f;
    d->matrix[2][0] = 0.0f;
    d->matrix[2][1] = 0.0f;
    d->matrix[2][2] = 1.0f;

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgLoadMatrix(const VGfloat * m)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!m || !isAligned(m,4), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    Matrix3x3* d = getCurrentMatrix(context);

    d->matrix[0][0] = inputFloat(m[0]);
    d->matrix[0][1] = inputFloat(m[3]);
    d->matrix[0][2] = inputFloat(m[6]);
    d->matrix[1][0] = inputFloat(m[1]);
    d->matrix[1][1] = inputFloat(m[4]);
    d->matrix[1][2] = inputFloat(m[7]);
    d->matrix[2][0] = inputFloat(m[2]);
    d->matrix[2][1] = inputFloat(m[5]);
    d->matrix[2][2] = inputFloat(m[8]);

    if (context->m_matrixMode != VG_MATRIX_IMAGE_USER_TO_SURFACE)
    {
        d->matrix[2][0] = 0;
        d->matrix[2][1] = 0;
        d->matrix[2][2] = 1;
    }

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgGetMatrix(VGfloat * m)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!m || !isAligned(m,4), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    Matrix3x3* d = getCurrentMatrix(context);
    m[0] = d->matrix[0][0];
    m[1] = d->matrix[1][0];
    m[2] = d->matrix[2][0];
    m[3] = d->matrix[0][1];
    m[4] = d->matrix[1][1];
    m[5] = d->matrix[2][1];
    m[6] = d->matrix[0][2];
    m[7] = d->matrix[1][2];
    m[8] = d->matrix[2][2];
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgMultMatrix(const VGfloat * m)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!m || !isAligned(m,4), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

    {
        Matrix3x3* d = getCurrentMatrix(context);
        Matrix3x3 n;

        initMatrix3x3(&n, inputFloat(m[0]), inputFloat(m[3]), inputFloat(m[6]),
            inputFloat(m[1]), inputFloat(m[4]), inputFloat(m[7]),
            inputFloat(m[2]), inputFloat(m[5]), inputFloat(m[8]));
        if (context->m_matrixMode != VG_MATRIX_IMAGE_USER_TO_SURFACE)
        {
            n.matrix[2][0] = 0;
            n.matrix[2][1] = 0;
            n.matrix[2][2] = 1;
        }

        multMatrix3x3(d, &n);

        if (context->m_matrixMode != VG_MATRIX_IMAGE_USER_TO_SURFACE)
        {    //force affinity
            d->matrix[2][0] = 0;
            d->matrix[2][1] = 0;
            d->matrix[2][2] = 1;
        }
    }

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgTranslate(VGfloat tx, VGfloat ty)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    {
        Matrix3x3 *d = getCurrentMatrix(context);
        Matrix3x3 n;

        initMatrix3x3(&n, 1, 0, inputFloat(tx),
                          0, 1, inputFloat(ty),
                          0, 0, 1 );

        multMatrix3x3(d, &n);

        if (context->m_matrixMode != VG_MATRIX_IMAGE_USER_TO_SURFACE)
        {    //force affinity
            d->matrix[2][0] = 0;
            d->matrix[2][1] = 0;
            d->matrix[2][2] = 1;
        }
    }

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgScale(VGfloat sx, VGfloat sy)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    {
        Matrix3x3* d = getCurrentMatrix(context);
        Matrix3x3 n;
        initMatrix3x3(&n, inputFloat(sx), 0, 0,
                          0, inputFloat(sy), 0,
                          0, 0, 1);

        multMatrix3x3(d, &n);

        if (context->m_matrixMode != VG_MATRIX_IMAGE_USER_TO_SURFACE)
        {    //force affinity
            d->matrix[2][0] = 0;
            d->matrix[2][1] = 0;
            d->matrix[2][2] = 1;
        }
    }

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgShear(VGfloat shx, VGfloat shy)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    {
        Matrix3x3* d = getCurrentMatrix(context);
        Matrix3x3 n;
        initMatrix3x3(&n, 1, inputFloat(shx), 0,
                          inputFloat(shy), 1, 0,
                          0, 0, 1);

        multMatrix3x3(d, &n);

        if (context->m_matrixMode != VG_MATRIX_IMAGE_USER_TO_SURFACE)
        {    //force affinity
            d->matrix[2][0] = 0;
            d->matrix[2][1] = 0;
            d->matrix[2][2] = 1;
        }
    }

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgRotate(VGfloat angle)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    {
        VGfloat a = VG_DEG_TO_RAD(inputFloat(angle));
        Matrix3x3* d = getCurrentMatrix(context);
        Matrix3x3 n;
        initMatrix3x3(&n, (VGfloat)cos(a), -(VGfloat)sin(a), 0,
                          (VGfloat)sin(a), (VGfloat)cos(a), 0,
                          0, 0, 1);

        multMatrix3x3(d, &n);

        if (context->m_matrixMode != VG_MATRIX_IMAGE_USER_TO_SURFACE)
        {    //force affinity
            d->matrix[2][0] = 0;
            d->matrix[2][1] = 0;
            d->matrix[2][2] = 1;
        }
    }

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgMask(VGHandle mask, VGMaskOperation operation, VGint x, VGint y, VGint width, VGint height)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    VGboolean isImage = isValidImage(context, mask);
    VGboolean isMaskLayer = isValidMaskLayer(context, mask);
    VG_IF_ERROR(operation != VG_CLEAR_MASK && operation != VG_FILL_MASK && !isImage && !isMaskLayer, VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(operation != VG_CLEAR_MASK && operation != VG_FILL_MASK && isImage && isEglVgInUse((Image*)mask), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(operation < VG_CLEAR_MASK || operation > VG_SUBTRACT_MASK, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(width <= 0 || height <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

    Drawable* drawable = context->m_eglDrawable;

    VG_IF_ERROR(isMaskLayer && drawable->m_color->m_numSamples != ((Surface*)mask)->m_numSamples, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    if (!drawable || !drawable->m_mask)
    {
        VG_RETURN(VG_NO_RETVAL);    //no EGL surface is current at the moment or context has no mask buffer
    }
    if (context->m_masking == VG_TRUE) {
        vg_lite_disable_masklayer();
    }
    if (context->m_scissoring == VG_TRUE) {
        vg_lite_disable_scissor();
    }

    vg_lite_filter_t filter = VG_LITE_FILTER_POINT;
    vg_lite_rectangle_t rect;
    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;
    vg_lite_buffer_t* masklayer = drawable->m_mask->m_image->m_vglbuf;
    vg_lite_buffer_format_t temp_format;
    if (isImage) {      
        vg_lite_buffer_t* srcbuf = (vg_lite_buffer_t*)((Image*)mask)->m_vglbuf;
        temp_format = srcbuf->format;
        if (srcbuf->format == VG_sRGBX_8888) {
            srcbuf->format = VG_sARGB_8888;
            vg_lite_blend_masklayer(masklayer, srcbuf, operation, &rect);
            srcbuf->format = temp_format;
        }
        /* RGB565 format convert to RGBA8888 ,so 5bit R channel convert to 8bit,modify swizzle to blit R channel to A8 channel. */
        else if (srcbuf->format == VG_sRGB_565 || srcbuf->format == VG_sL_8) {
            vg_lite_buffer_t tempbuf = *srcbuf;
            tempbuf.format = VG_sRGBA_8888;
            vg_lite_allocate(&tempbuf);
            vg_lite_blit(&tempbuf, srcbuf, NULL, VG_LITE_BLEND_NONE, 0, filter);
            tempbuf.format = VG_sABGR_8888;
            vg_lite_blend_masklayer(masklayer, &tempbuf, operation, &rect);
        }
        else if (srcbuf->format == VG_lRGBX_8888) {
            srcbuf->format = VG_lARGB_8888;
            vg_lite_blend_masklayer(masklayer, srcbuf, operation, &rect);
            srcbuf->format = temp_format;
        }
        else if (srcbuf->format == VG_lL_8) {
            vg_lite_buffer_t tempbuf = *srcbuf;
            tempbuf.format = VG_lRGBA_8888;
            vg_lite_allocate(&tempbuf);
            vg_lite_blit(&tempbuf, srcbuf, NULL, VG_LITE_BLEND_NONE, 0, filter);
            tempbuf.format = VG_lABGR_8888;
            vg_lite_blend_masklayer(masklayer, &tempbuf, operation, &rect);
        }
        else if (srcbuf->format == VG_A_1 || srcbuf->format == VG_BW_1) {
            vg_lite_buffer_t tempbuf = *srcbuf;
            char* buf1 = (char*)srcbuf->memory;

            tempbuf.format = VG_A_8;
            vg_lite_allocate(&tempbuf);
            char* buf2 = (char*)tempbuf.memory;
            for (int i = 0; i < srcbuf->height; i++) {

                for (int j = 1; j <= srcbuf->width; j++) {

                    if (*(buf1) & (0x1 << (j%8-1))) {
                        *buf2 = 0xFF;
                    }
                    else{
                        *buf2 = 0x0;
                    }
                    if (j % 8 == 0) {
                        buf1++;
                    }
                    buf2++;
                }
                buf2 += tempbuf.stride - tempbuf.width;
                buf1 += srcbuf->stride - srcbuf->width/8;
            }
            vg_lite_blend_masklayer(masklayer, &tempbuf, operation, &rect);
        }

        else {
            vg_lite_blend_masklayer(masklayer, srcbuf, operation, &rect);
        }

    }
    else {
        //vg_lite_buffer_t* srcbuf = (vg_lite_buffer_t*)((Surface*)mask)->m_image->m_vglbuf;
        //vg_lite_blend_masklayer(masklayer, srcbuf, operation, &rect);
        maskSurface(drawable, (Surface*)mask, operation, x, y, width, height);
    }
    vg_lite_flush();
    vg_lite_enable_masklayer();
    context->m_masking = VG_TRUE;
    if (context->m_scissoring == VG_TRUE) {
        vg_lite_enable_scissor();
    }
    vg_lite_set_masklayer((vg_lite_buffer_t*)drawable->m_mask->m_image->m_vglbuf);
    VG_RETURN(VG_NO_RETVAL);
}

__attribute__((unused)) static void renderStroke(const VGContext* context, int w, int h, int numSamples, Path* path, Rasterizer* rasterizer, const PixelPipe* pixelPipe, __attribute__((unused)) const Matrix3x3 userToSurface)
{
    VG_ASSERT(context);
    VG_ASSERT(w > 0 && h > 0 && numSamples >= 1 && numSamples <= 32);
    VGuint* covBuffer = (VGuint*)malloc(w*h);
    memset(covBuffer, 0, w * h * sizeof(VGuint));
    setupRasterizer(rasterizer, 0, 0, w, h, VG_NON_ZERO, NULL, covBuffer);
}

void VG_APIENTRY vgRenderToMask(VGPath path, VGbitfield paintModes, VGMaskOperation operation)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    VG_IF_ERROR(!isValidPath(context, path), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid path handle
    VG_IF_ERROR(!paintModes || (paintModes & ~(VG_FILL_PATH | VG_STROKE_PATH)), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);    //invalid paint mode
    VG_IF_ERROR(operation < VG_CLEAR_MASK || operation > VG_SUBTRACT_MASK, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    Drawable* curr = context->m_eglDrawable;
    if (!curr || !curr->m_mask)
    {
        VG_RETURN(VG_NO_RETVAL);    //no EGL surface is current at the moment or context has no mask buffer
    }

    if (context->m_masking == VG_TRUE) {
        vg_lite_disable_masklayer();
    }
    if (context->m_scissoring == VG_TRUE) {
        vg_lite_disable_scissor();
    }

    Path* p = (Path*)path;
    vg_lite_path_t* vglpath = (vg_lite_path_t*)((Path*)path)->m_vglPath;
    vg_lite_buffer_t* masklayer = curr->m_mask->m_image->m_vglbuf;
    ColorDescriptor desc = formatToDescriptor(VG_A_8);
    vg_lite_rectangle_t rect = { 0, 0, masklayer->width, masklayer->height };
    Surface* drawsurface = createSurface(&desc, curr->m_color->m_width, curr->m_color->m_height, curr->m_color->m_numSamples);
    if (drawsurface)
    {
        drawsurface->m_referenceCount++;
    }
    vg_lite_buffer_t* drawbuffer = drawsurface->m_image->m_vglbuf;

    if (operation == VG_CLEAR_MASK) {
        vg_lite_fill_masklayer(masklayer, NULL, 0x0);
    }
    else if (operation == VG_FILL_MASK) {
        vg_lite_fill_masklayer(masklayer, NULL, 0xFF);
    }
    else{
        if (context->m_scissoring == VG_TRUE) {
            vg_lite_enable_scissor();
        }
        if (p->m_pathChanged) {
            if (vglpath)
                vg_lite_clear_path(vglpath);

            VGuint data_num = 0;
            VGubyte* seg_ptr = p->m_segments;
            VGuint grid_size = _dataSize[p->m_datatype];
            for (VGint i = 0; i < p->m_numSegments; ++i)
                data_num += _commandSize[*seg_ptr++];
            VGubyte* dst_data = malloc(data_num * grid_size);
            uint8_t* vgl_opcodes = malloc(sizeof(uint8_t) * p->m_numSegments);
            VGuint bytes;
            seg_ptr = p->m_segments;
            VGubyte** src_data_list_ptr = p->m_data;
            VGubyte* dst_data_ptr = dst_data;
            uint8_t* vgl_opcodes_ptr = vgl_opcodes;
            for (VGint i = 0; i < p->m_numSegments; ++i) {
                *vgl_opcodes_ptr++ = _vglOpcodes[*seg_ptr];

                bytes = _commandSize[*seg_ptr++] * grid_size;
                memcpy(dst_data_ptr, *src_data_list_ptr, bytes);
                dst_data_ptr += bytes;
                src_data_list_ptr++;
            }

            vglpath->format = (vg_lite_format_t)p->m_datatype;

            switch (context->m_renderingQuality){
                case VG_RENDERING_QUALITY_NONANTIALIASED:
                    vglpath->quality = VG_LITE_LOW;
                    break;
                case VG_RENDERING_QUALITY_FASTER:
                    vglpath->quality = VG_LITE_MEDIUM;
                    break;
                case VG_RENDERING_QUALITY_BETTER:
                    vglpath->quality = VG_LITE_HIGH;
                    break;
                default:
                    break;
            }
            vg_lite_append_path(vglpath, vgl_opcodes, (vg_lite_pointer)dst_data, (vg_lite_uint32_t)p->m_numSegments);

            free(dst_data);
            free(vgl_opcodes);

            p->m_pathChanged = VG_FALSE;
        }

        vg_lite_fill_t fill_rule = context->m_fillRule;
        if (paintModes & VG_FILL_PATH) {
            vglpath->path_type = VG_LITE_DRAW_FILL_PATH;
            vg_lite_draw(drawbuffer, vglpath, fill_rule, (vg_lite_matrix_t*)&context->m_pathUserToSurface, VG_LITE_BLEND_NONE, (vg_lite_color_t)0xFF << 24);
            vg_lite_disable_scissor();
            vg_lite_blend_masklayer(masklayer, drawbuffer, operation, &rect);
            vg_lite_finish();
        }
        if (paintModes & VG_STROKE_PATH) {
            vg_lite_disable_scissor();
            vg_lite_clear(drawbuffer, NULL, 0x0);
            if (context->m_scissoring == VG_TRUE) {
                vg_lite_enable_scissor();
            }
            vglpath->path_type = VG_LITE_DRAW_STROKE_PATH;
            vg_lite_set_stroke(vglpath, context->m_strokeCapStyle,
                context->m_strokeJoinStyle,
                context->m_strokeLineWidth,
                context->m_strokeMiterLimit,
                context->m_strokeDashPattern,
                context->m_strokeDashPatternCount,
                context->m_strokeDashPhase,
                0XFFFFFFFF);
            vglpath->stroke->dash_reset = context->m_strokeDashPhaseReset;
            vg_lite_update_stroke(vglpath);
            vg_lite_draw(drawbuffer, vglpath, fill_rule, (vg_lite_matrix_t*)&context->m_pathUserToSurface, VG_LITE_BLEND_NONE, (vg_lite_color_t)0xFF << 24);
            vg_lite_disable_scissor();
            vg_lite_blend_masklayer(masklayer, drawbuffer, operation, &rect);
            vg_lite_finish();
        }
    }
    if (context->m_masking == VG_TRUE) {
        vg_lite_enable_masklayer();
    }
    if (context->m_scissoring == VG_TRUE) {
        vg_lite_enable_scissor();
    }
    if (drawsurface)
    {
        drawsurface->m_referenceCount--;
        VG_ASSERT(drawsurface->m_referenceCount >= 0);
        if (!drawsurface->m_referenceCount)
        {
            destroySurface(drawsurface);
        }
    }
    VG_RETURN(VG_NO_RETVAL);
}

VGMaskLayer VG_APIENTRY vgCreateMaskLayer(VGint width, VGint height)
{
    VG_GET_CONTEXT(VG_INVALID_HANDLE);
    VG_IF_ERROR(width <= 0 || height <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);
    VG_IF_ERROR(width > VIV_MAX_IMAGE_WIDTH || height > VIV_MAX_IMAGE_HEIGHT || width*height > VIV_MAX_IMAGE_PIXELS ||
                width*height > VIV_MAX_IMAGE_BYTES, VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);

    Drawable* curr = context->m_eglDrawable;
    if (!curr || !curr->m_mask)
        VG_RETURN(VG_INVALID_HANDLE);   //no current drawing surface

    Surface* layer = NULL;
    ColorDescriptor desc = formatToDescriptor(VG_A_8);
    layer = createSurface(&desc, width, height, curr->m_color->m_numSamples);
    if (!layer)
    {
        setError(VG_OUT_OF_MEMORY_ERROR);
        VG_RETURN(VG_INVALID_HANDLE);
    }
    VG_ASSERT(layer);

    // layer->clear(Color(1, 1, 1, 1, Color::sRGBA), 0, 0, width, height);
    vg_lite_buffer_t* dstbuffer = (vg_lite_buffer_t*)layer->m_image->m_vglbuf;
    vg_lite_rectangle_t rect = { 0, 0, width, height };
    vg_lite_clear(dstbuffer, &rect, 0xFFFFFFFF);
    vg_lite_flush();
    addMaskLayerResource(context, layer);

    VG_RETURN((VGMaskLayer)layer);
}

void VG_APIENTRY vgDestroyMaskLayer(VGMaskLayer maskLayer)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidMaskLayer(context, maskLayer), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid handle

    removeMaskLayerResource(context, (Surface*)maskLayer);

    Surface* layer = (Surface*)maskLayer;
    destroySurface(layer);

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgFillMaskLayer(VGMaskLayer maskLayer, VGint x, VGint y, VGint width, VGint height, VGfloat value)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    VG_IF_ERROR(!isValidMaskLayer(context, maskLayer), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid handle
    VG_IF_ERROR(value < 0.0f || value > 1.0f, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    Surface* layer = (Surface*)maskLayer;
    VG_IF_ERROR(width <= 0 || height <= 0 || x < 0 || y < 0 || x > layer->m_width-width || y > layer->m_height-height, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

    vg_lite_buffer_t* vglbuffer = (vg_lite_buffer_t*)layer->m_image->m_vglbuf;
    x = x < 0 ? 0 : x;
    y = y < 0 ? 0 : y;
    vg_lite_rectangle_t rect = { x, y, width, height };
    vg_lite_uint8_t maskval = VG_FLOAT_TO_UB(value);

    vg_lite_fill_masklayer(vglbuffer, &rect, maskval);

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgCopyMask(VGMaskLayer maskLayer, VGint dx, VGint dy, VGint sx, VGint sy, VGint width, VGint height)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    VG_IF_ERROR(!isValidMaskLayer(context, maskLayer), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid handle
    Drawable* drawable = context->m_eglDrawable;
    if (!drawable || !drawable->m_mask)
    {
        VG_RETURN(VG_NO_RETVAL);    //no EGL surface is current at the moment or context has no mask buffer
    }
    Surface* layer = (Surface*)maskLayer;
    VG_IF_ERROR(width <= 0 || height <= 0 || drawable->m_color->m_numSamples != layer->m_numSamples, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

    if (context->m_masking == VG_TRUE) {
        vg_lite_disable_masklayer();
    }
    if (context->m_scissoring == VG_TRUE) {
        vg_lite_disable_scissor();
    }
    dx = dx > 0 ? dx : 0;
    dy = dy > 0 ? dy : 0;
    sx = sx > 0 ? sx : 0;
    sy = sy > 0 ? sy : 0;

    vg_lite_buffer_t* srcbuf = drawable->m_mask->m_image->m_vglbuf;
    vg_lite_buffer_t* dstbuf = (vg_lite_buffer_t*)((Surface*)maskLayer)->m_image->m_vglbuf;
    Image* srcimg = drawable->m_mask->m_image;
    Image* dstimg = ((Surface*)maskLayer)->m_image;

    if (descriptorIsPremultiplied(&(srcimg->m_desc)) && descriptorIsPremultiplied(&(dstimg->m_desc))) {
        vg_lite_blit(srcbuf, srcbuf, NULL, OPENVG_BLEND_SRC, 0, VG_LITE_FILTER_POINT);
    }

    vg_lite_copy_image(dstbuf, srcbuf, dx, dy, sx, sy, width, height);
    vg_lite_flush();
    if (context->m_masking == VG_TRUE) {
        vg_lite_enable_masklayer();
    }
    if (context->m_scissoring == VG_TRUE) {
        vg_lite_enable_scissor();
    }
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgClear(VGint x, VGint y, VGint width, VGint height)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(width <= 0 || height <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

    Drawable* drawable = context->m_eglDrawable;
    if (!drawable)
    {   //no EGL surface is current at the moment
        VG_RETURN(VG_NO_RETVAL);
    }

    // Use vg_lite_clear to implement vgClear
    vg_lite_buffer_t *dstbuf = (vg_lite_buffer_t*)drawable->m_color->m_image->m_vglbuf;
    vg_lite_rectangle_t rect = { x, y, width, height };
    context->m_clearColor.a = (context->m_clearColor.a > 1) ? 1 : context->m_clearColor.a;
    context->m_clearColor.a = (context->m_clearColor.a < 0) ? 0 : context->m_clearColor.a;
    context->m_clearColor.b = (context->m_clearColor.b > 1) ? 1 : context->m_clearColor.b;
    context->m_clearColor.b = (context->m_clearColor.b < 0) ? 0 : context->m_clearColor.b;
    context->m_clearColor.g = (context->m_clearColor.g > 1) ? 1 : context->m_clearColor.g;
    context->m_clearColor.g = (context->m_clearColor.g < 0) ? 0 : context->m_clearColor.g;
    context->m_clearColor.r = (context->m_clearColor.r > 1) ? 1 : context->m_clearColor.r;
    context->m_clearColor.r = (context->m_clearColor.r < 0) ? 0 : context->m_clearColor.r;


    VGuint rgba;
   
    vg_lite_uint8_t a = VG_FLOAT_TO_UB(context->m_clearColor.a);
    vg_lite_uint8_t b = VG_FLOAT_TO_UB(context->m_clearColor.b);
    vg_lite_uint8_t g = VG_FLOAT_TO_UB(context->m_clearColor.g);
    vg_lite_uint8_t r = VG_FLOAT_TO_UB(context->m_clearColor.r);
    rgba = (a << 24) | (b << 16) | (g << 8) | (r << 0);

    vg_lite_clear(dstbuf, &rect, rgba);
    vg_lite_flush();

    VG_RETURN(VG_NO_RETVAL);
}

VGPath VG_APIENTRY vgCreatePath(VGint pathFormat, VGPathDatatype datatype, VGfloat scale, VGfloat bias, VGint segmentCapacityHint, VGint coordCapacityHint, VGbitfield capabilities)
{
    VG_GET_CONTEXT(VG_INVALID_HANDLE);
    VG_IF_ERROR(pathFormat != VG_PATH_FORMAT_STANDARD, VG_UNSUPPORTED_PATH_FORMAT_ERROR, VG_INVALID_HANDLE);
    VG_IF_ERROR(datatype < VG_PATH_DATATYPE_S_8 || datatype > VG_PATH_DATATYPE_F, VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);
    VGfloat s = inputFloat(scale);
    VGfloat b = inputFloat(bias);
    VG_IF_ERROR(s == 0.0f, VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);
    capabilities &= VG_PATH_CAPABILITY_ALL;    //undefined bits are ignored

    Path* path = (Path*)malloc(sizeof(Path));
    if (!path)
    {
        setError(VG_OUT_OF_MEMORY_ERROR);
        VG_RETURN(VG_INVALID_HANDLE);
    }
    memset(path, 0, sizeof(Path));
    path->m_format = pathFormat;
    path->m_datatype = datatype;
    path->m_scale = s;
    path->m_bias = b;
    path->m_capabilities = capabilities;
    path->object.type = OBJECTTYPE_PATH;
    path->m_referenceCount = 0;

    VG_ASSERT(pathFormat == VG_PATH_FORMAT_STANDARD);
    VG_ASSERT(datatype >= VG_PATH_DATATYPE_S_8 && datatype <= VG_PATH_DATATYPE_F);

    vg_lite_path_t* vglpath = (vg_lite_path_t*)malloc(sizeof(vg_lite_path_t));
    if (!vglpath)
    {
        freePathImpl(path);
        setError(VG_OUT_OF_MEMORY_ERROR);
        VG_RETURN(VG_INVALID_HANDLE);
    }
    if (context->m_renderingQuality == VG_RENDERING_QUALITY_NONANTIALIASED)
    {
        vg_lite_init_path(vglpath, path->m_datatype, VG_LITE_LOW, 0, NULL, 0.0f, 0.0f, 0.0f, 0.0f);
    }
    else if (context->m_renderingQuality == VG_RENDERING_QUALITY_FASTER)
    {
        vg_lite_init_path(vglpath, path->m_datatype, VG_LITE_MEDIUM, 0, NULL, 0.0f, 0.0f, 0.0f, 0.0f);
    }
    else
    {
        vg_lite_init_path(vglpath, path->m_datatype, VG_LITE_HIGH, 0, NULL, 0.0f, 0.0f, 0.0f, 0.0f);
    }

    path->m_vglPath = vglpath;

    VG_ASSERT(path);
    addPathResource(context, path);
    path->m_referenceCount++;
    VG_RETURN((VGPath)path);
}

void VG_APIENTRY vgClearPath(VGPath path, VGbitfield capabilities)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidPath(context, path), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid path handle
    capabilities &= VG_PATH_CAPABILITY_ALL;    //undefined bits are ignored
    Path* p = (Path*)path;
    p->m_capabilities = capabilities;

    if (p->m_vglPath) {
        vg_lite_clear_path(p->m_vglPath);
    }

    for (VGint i = 0; i < p->m_numSegments; ++i) {
        free(p->m_data[i]);
    }

    if (p->m_segments) {
        free(p->m_segments);
        p->m_segments = NULL;
    }

    if (p->m_data) {
        free(p->m_data);
        p->m_data = NULL;
    }

    p->m_numSegments = 0;
    p->m_pathChanged = VG_TRUE;
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgDestroyPath(VGPath path)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidPath(context, path), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid path handle
    vg_lite_finish();
    removePathResource(context, (Path*)path);

    Path* p = (Path*)path;
    p->m_referenceCount--;
    if (path != VG_INVALID_HANDLE && !p->m_referenceCount) {
        freePathImpl(p);
    }
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgRemovePathCapabilities(VGPath path, VGbitfield capabilities)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidPath(context, path), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid path handle
    capabilities &= VG_PATH_CAPABILITY_ALL;    //undefined bits are ignored

    VGbitfield caps = ((Path*)path)->m_capabilities;
    caps &= ~capabilities;
    ((Path*)path)->m_capabilities = caps;
    VG_RETURN(VG_NO_RETVAL);
}

VGbitfield VG_APIENTRY vgGetPathCapabilities(VGPath path)
{
    VG_GET_CONTEXT(0);

    VG_IF_ERROR(!isValidPath(context, path), VG_BAD_HANDLE_ERROR, 0);    //invalid path handle
    VGbitfield ret = ((Path*)path)->m_capabilities;
    VG_RETURN(ret);
}

void VG_APIENTRY vgAppendPathData(VGPath dstPath, VGint numSegments, const VGubyte * pathSegments, const void * pathData)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidPath(context, dstPath), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid path handle
    Path* p = (Path*)dstPath;
    VG_IF_ERROR(!(p->m_capabilities & VG_PATH_CAPABILITY_APPEND_TO), VG_PATH_CAPABILITY_ERROR, VG_NO_RETVAL);    //no append cap
    VG_IF_ERROR(numSegments <= 0 || !pathSegments || !pathData, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);    //no segments or data
    VG_IF_ERROR((p->m_datatype == VG_PATH_DATATYPE_S_16 && !isAligned(pathData,2)) ||
                ((p->m_datatype == VG_PATH_DATATYPE_S_32 || p->m_datatype == VG_PATH_DATATYPE_F) && !isAligned(pathData,4)),
                VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);    //invalid alignment
    
    VGuint dataSize = _dataSize[p->m_datatype];

    void* tmp = realloc(p->m_segments, (p->m_numSegments + numSegments) * sizeof(VGubyte));
    VG_IF_ERROR(tmp == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
    p->m_segments = tmp;
    memcpy(p->m_segments + p->m_numSegments, pathSegments, numSegments);

    tmp = realloc(p->m_data, (p->m_numSegments + numSegments) * sizeof(VGubyte*));
    VG_IF_ERROR(tmp == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
    p->m_data = tmp;
    VGubyte** data_ptr = p->m_data + p->m_numSegments;
    VGubyte* src_ptr = (VGubyte*)pathData;
    VGuint size = 0;
    for (VGint i = 0; i < numSegments; ++i) {
        VGPathSegment c = (VGPathSegment)(pathSegments[i] & 0x1f);
        if (c < VG_CLOSE_PATH || c > VG_LCWARC_TO_REL || (c & ~0x1f)) {
            setError(VG_ILLEGAL_ARGUMENT_ERROR);
            numSegments = i;
            break;
        }
        size = _commandSize[c] * dataSize;
        tmp = malloc(size);
        if (tmp == NULL) {
            setError(VG_OUT_OF_MEMORY_ERROR);
            numSegments = i;
            break;
        }
        memcpy(tmp, src_ptr, size);
        *data_ptr++ = tmp;
        src_ptr += size;
    }

    biasScaleTransform(p, p->m_numSegments, numSegments);

    p->m_numSegments += numSegments;
    p->m_pathChanged = VG_TRUE;

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgModifyPathCoords(VGPath dstPath, VGint startIndex, VGint numSegments, const void* pathData)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidPath(context, dstPath), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid path handle
    Path* p = (Path*)dstPath;
    VG_IF_ERROR(!(p->m_capabilities & VG_PATH_CAPABILITY_MODIFY), VG_PATH_CAPABILITY_ERROR, VG_NO_RETVAL);    //no modify cap
    VG_IF_ERROR(!pathData || startIndex < 0 || numSegments <= 0 || VG_INT_ADDSATURATE(startIndex, numSegments) > p->m_numSegments, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);    //no segments
    VG_IF_ERROR((p->m_datatype == VG_PATH_DATATYPE_S_16 && !isAligned(pathData, 2)) ||
        ((p->m_datatype == VG_PATH_DATATYPE_S_32 || p->m_datatype == VG_PATH_DATATYPE_F) && !isAligned(pathData, 4)),
        VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);    //invalid alignment

    VGubyte* seg_ptr = p->m_segments + startIndex;
    VGubyte** data_list = p->m_data + startIndex;
    VGubyte* path_data_ptr = (VGubyte*)pathData;
    VGuint grid_size = _dataSize[p->m_datatype];
    VGuint bytes;
    for (int i = 0; i < numSegments; ++i) {
        bytes = _commandSize[*seg_ptr++] * grid_size;
        memcpy(*data_list++, path_data_ptr, bytes);
        path_data_ptr += bytes;
    }

    biasScaleTransform(p, startIndex, numSegments);

    p->m_pathChanged = VG_TRUE;

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgAppendPath(VGPath dstPath, VGPath srcPath)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidPath(context, dstPath) || !isValidPath(context, srcPath), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid path handle
    VG_IF_ERROR(!(((Path*)dstPath)->m_capabilities & VG_PATH_CAPABILITY_APPEND_TO) ||
        !(((Path*)srcPath)->m_capabilities & VG_PATH_CAPABILITY_APPEND_FROM), VG_PATH_CAPABILITY_ERROR, VG_NO_RETVAL);    //invalid caps
    Path* d = (Path*)dstPath;
    Path* s = (Path*)srcPath;

    appendPathImpl(d, s);
    d->m_numSegments += s->m_numSegments;

    d->m_pathChanged = VG_TRUE;

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgTransformPath(VGPath dstPath, VGPath srcPath)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    VG_IF_ERROR(!isValidPath(context, dstPath) || !isValidPath(context, srcPath), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid path handle
    VG_IF_ERROR(!(((Path*)dstPath)->m_capabilities & VG_PATH_CAPABILITY_TRANSFORM_TO) ||
        !(((Path*)srcPath)->m_capabilities & VG_PATH_CAPABILITY_TRANSFORM_FROM), VG_PATH_CAPABILITY_ERROR, VG_NO_RETVAL);    //invalid caps
    Path* d = (Path*)dstPath;
    Path* s = (Path*)srcPath;

    appendPathImpl(d, s);

    /* Convert HLine_To_* and VLine_To_* to Line_To_*, then applying transformation */
    pathTransformImpl(d, d->m_numSegments, s->m_numSegments, context->m_pathUserToSurface);

    d->m_numSegments += s->m_numSegments;
    d->m_pathChanged = VG_TRUE;

    VG_RETURN(VG_NO_RETVAL);
}

static VGboolean drawPath(VGContext* context, VGPath path, const Matrix3x3 userToSurfaceMatrix, VGbitfield paintModes)
{
    //set up rendering surface and mask buffer
    Drawable* drawable = context->m_eglDrawable;
    if (!drawable)
        return VG_FALSE;   //no EGL surface is current at the moment

    // Update vglPath
    Path* p = (Path*)path;
    vg_lite_path_t* vglpath = (vg_lite_path_t*)((Path*)path)->m_vglPath;
    if (vglpath == NULL) return VG_FALSE;
    if (p->m_pathChanged) {
        if (vglpath)
            vg_lite_clear_path(vglpath);

        VGuint data_num = 0;
        VGubyte* seg_ptr = p->m_segments;
        VGuint grid_size = _dataSize[p->m_datatype];
        for (VGint i = 0; i < p->m_numSegments; ++i) {
            data_num += _commandSize[*seg_ptr];
            seg_ptr++;
        }

        VGubyte* dst_data = malloc(data_num * grid_size);
        VG_IF_ERROR(dst_data == NULL, VG_OUT_OF_MEMORY_ERROR, VG_FALSE);
        uint8_t* vgl_opcodes = malloc(sizeof(uint8_t) * p->m_numSegments);
        if (vgl_opcodes == NULL) {
            free(dst_data);
            setError(VG_OUT_OF_MEMORY_ERROR);
            OSReleaseMutex();
            return VG_FALSE;
        }

        VGuint bytes;
        seg_ptr = p->m_segments;
        VGubyte** src_data_list_ptr = p->m_data;
        VGubyte* dst_data_ptr = dst_data;
        uint8_t* vgl_opcodes_ptr = vgl_opcodes;
        for (VGint i = 0; i < p->m_numSegments; ++i) {
            *vgl_opcodes_ptr = _vglOpcodes[*seg_ptr];
            vgl_opcodes_ptr++;

            bytes = _commandSize[*seg_ptr] * grid_size;
            seg_ptr++;
            memcpy(dst_data_ptr, *src_data_list_ptr, bytes);
            dst_data_ptr += bytes;
            src_data_list_ptr++;
        }

        vglpath->format = (vg_lite_format_t)p->m_datatype;
        vg_lite_append_path(vglpath, vgl_opcodes, (vg_lite_pointer)dst_data, (vg_lite_uint32_t)p->m_numSegments);

        free(dst_data);
        free(vgl_opcodes);

        p->m_pathChanged = VG_FALSE;
    }

    if (context->m_renderingQuality == VG_RENDERING_QUALITY_NONANTIALIASED)
    {
        vglpath->quality = VG_LITE_LOW;
    }
    else if (context->m_renderingQuality == VG_RENDERING_QUALITY_FASTER)
    {
        vglpath->quality = VG_LITE_MEDIUM;
    }
    else
    {
        vglpath->quality = VG_LITE_HIGH;
    }

    vg_lite_buffer_t* fbbuf = (vg_lite_buffer_t*)drawable->m_color->m_image->m_vglbuf;
    vg_lite_fill_t fill_rule = context->m_fillRule;
    vg_lite_blend_t blend = context->m_blendMode;
    Matrix3x3 userToSurfaceFill = userToSurfaceMatrix;
    Matrix3x3 userToSurfaceStroke = userToSurfaceMatrix;
    vg_lite_color_t fill_color = 0xFF000000;
    vg_lite_color_t stroke_color = 0xFF000000;

    if (context->m_masking) {
        vg_lite_enable_masklayer();
    }
    else {
        vg_lite_disable_masklayer();
    }

    if (context->m_colorTransform)
    {
        vg_lite_color_transform_t color_transformations;

        color_transformations.r_scale = context->m_colorTransformValues[0];
        color_transformations.g_scale = context->m_colorTransformValues[1];
        color_transformations.b_scale = context->m_colorTransformValues[2];
        color_transformations.a_scale = context->m_colorTransformValues[3];
        color_transformations.r_bias = context->m_colorTransformValues[4];
        color_transformations.g_bias = context->m_colorTransformValues[5];
        color_transformations.b_bias = context->m_colorTransformValues[6];
        color_transformations.a_bias = context->m_colorTransformValues[7];

        vg_lite_enable_color_transform();
        vg_lite_set_color_transform(&color_transformations);
    }

    if (paintModes & VG_FILL_PATH)
    {
        Paint* paint = (Paint*)context->m_fillPaint;
        vglpath->path_type = VG_LITE_DRAW_FILL_PATH;
        multMatrix3x3(&userToSurfaceFill, &(context->m_fillPaintToUser));
        if (paint) {
            switch (paint->m_paintType)
            {
            case VG_PAINT_TYPE_COLOR:
            {
                vg_lite_uint8_t a = VG_FLOAT_TO_UB(paint->m_paintColor.a);
                vg_lite_uint8_t b = VG_FLOAT_TO_UB(paint->m_paintColor.b);
                vg_lite_uint8_t g = VG_FLOAT_TO_UB(paint->m_paintColor.g);
                vg_lite_uint8_t r = VG_FLOAT_TO_UB(paint->m_paintColor.r);
                fill_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                vg_lite_draw(fbbuf, vglpath, fill_rule, (vg_lite_matrix_t*)&userToSurfaceFill, blend, fill_color);
                vg_lite_flush();
                break;
            }

            case VG_PAINT_TYPE_LINEAR_GRADIENT:
            {
                vg_lite_ext_linear_gradient_t grad;
                vg_lite_linear_gradient_parameter_t linear_gradient;
                vg_lite_color_ramp_t vg_color_ramp[VIV_MAX_COLOR_RAMP_STOPS];
                GradientStop* gradientstop = paint->m_colorRampStops;

                linear_gradient.X0 = paint->m_linearGradientPoint0.x;
                linear_gradient.Y0 = paint->m_linearGradientPoint0.y;
                linear_gradient.X1 = paint->m_linearGradientPoint1.x;
                linear_gradient.Y1 = paint->m_linearGradientPoint1.y;

                for (int i = 0; i < paint->m_colorRampStopsCount; i++)
                {
                    vg_color_ramp[i].stop = gradientstop[i].offset;
                    vg_color_ramp[i].red = gradientstop[i].color.r;
                    vg_color_ramp[i].green = gradientstop[i].color.g;
                    vg_color_ramp[i].blue = gradientstop[i].color.b;
                    vg_color_ramp[i].alpha = gradientstop[i].color.a;
                }
                if((linear_gradient.X0 != linear_gradient.X1) || (linear_gradient.Y0 != linear_gradient.Y1))
                {
                    memset(&grad, 0, sizeof(grad));
                    if (paint->grad_image.handle != NULL)
                    {
                        vg_lite_finish();
                        vg_lite_free(&paint->grad_image);
                    }
                    setLinearGrad(&grad, paint->m_colorRampStopsCount, vg_color_ramp, linear_gradient, paint->m_colorRampSpreadMode, paint->m_colorRampPremultiplied);
                    updateLinearGrad(context, &grad, &userToSurfaceFill);
                    vg_lite_draw_linear_grad(fbbuf, vglpath, fill_rule, (vg_lite_matrix_t*)&(context->m_pathUserToSurface), &grad, 0, blend, VG_LITE_FILTER_LINEAR);
                    grad.count = 0;
                    paint->grad_image = grad.image;                
                }
                else {
                    if (paint->m_colorRampSpreadMode == VG_COLOR_RAMP_SPREAD_REPEAT) {
                        vg_lite_uint8_t a = VG_FLOAT_TO_UB(gradientstop[0].color.a);
                        vg_lite_uint8_t b = VG_FLOAT_TO_UB(gradientstop[0].color.b);
                        vg_lite_uint8_t g = VG_FLOAT_TO_UB(gradientstop[0].color.g);
                        vg_lite_uint8_t r = VG_FLOAT_TO_UB(gradientstop[0].color.r);
                        fill_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                    }
                    else {
                        vg_lite_uint8_t a = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.a);
                        vg_lite_uint8_t b = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.b);
                        vg_lite_uint8_t g = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.g);
                        vg_lite_uint8_t r = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.r);
                        fill_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                    }
                    vg_lite_draw(fbbuf, vglpath, fill_rule, (vg_lite_matrix_t*)&(context->m_pathUserToSurface), blend, fill_color);
                }

                break;
            }

            case VG_PAINT_TYPE_PATTERN:
            {
                if (paint->m_pattern)
                {
                    vg_lite_buffer_t* srcbuf = (vg_lite_buffer_t*)(paint->m_pattern->m_vglbuf);
                    vg_lite_color_t tile_color;

                    if (srcbuf->format == VG_LITE_A8) {
                        srcbuf->image_mode = VG_LITE_MULTIPLY_IMAGE_MODE;
                    }

                    vg_lite_uint8_t a = VG_FLOAT_TO_UB(context->m_tileFillColor.a);
                    vg_lite_uint8_t b = VG_FLOAT_TO_UB(context->m_tileFillColor.b);
                    vg_lite_uint8_t g = VG_FLOAT_TO_UB(context->m_tileFillColor.g);
                    vg_lite_uint8_t r = VG_FLOAT_TO_UB(context->m_tileFillColor.r);
                    tile_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                    vg_lite_image_mode_t tmp_image_mode = srcbuf->image_mode;

                    vg_lite_draw_pattern(fbbuf, vglpath, fill_rule, (vg_lite_matrix_t*)&(context->m_pathUserToSurface), srcbuf, (vg_lite_matrix_t*)&userToSurfaceFill, VG_LITE_BLEND_NONE, paint->m_patternTilingMode, tile_color, 0xFFFFFFFF, VG_LITE_FILTER_POINT);
                    vg_lite_flush();
                    srcbuf->image_mode = tmp_image_mode;
                    if (context->m_colorTransform) {
                        vg_lite_disable_color_transform();
                    }
                }
                else
                {
                    vg_lite_uint8_t a = VG_FLOAT_TO_UB(paint->m_paintColor.a);
                    vg_lite_uint8_t b = VG_FLOAT_TO_UB(paint->m_paintColor.b);
                    vg_lite_uint8_t g = VG_FLOAT_TO_UB(paint->m_paintColor.g);
                    vg_lite_uint8_t r = VG_FLOAT_TO_UB(paint->m_paintColor.r);
                    fill_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                    vg_lite_draw(fbbuf, vglpath, fill_rule, (vg_lite_matrix_t*)&userToSurfaceFill, blend, fill_color);
                }
                break;
            }

            case VG_PAINT_TYPE_RADIAL_GRADIENT:
            {
                vg_lite_radial_gradient_t grad;
                vg_lite_radial_gradient_parameter_t radialGradient;
                vg_lite_color_ramp_t vg_color_ramp[VIV_MAX_COLOR_RAMP_STOPS];
                GradientStop* gradientstop = paint->m_colorRampStops;

                radialGradient.cx = paint->m_radialGradientCenter.x;
                radialGradient.cy = paint->m_radialGradientCenter.y;
                radialGradient.fx = paint->m_radialGradientFocalPoint.x;
                radialGradient.fy = paint->m_radialGradientFocalPoint.y;
                radialGradient.r = paint->m_radialGradientRadius;

                for (int i = 0; i < paint->m_colorRampStopsCount; i++)
                {
                    vg_color_ramp[i].stop = gradientstop[i].offset;
                    vg_color_ramp[i].red = gradientstop[i].color.r;
                    vg_color_ramp[i].green = gradientstop[i].color.g;
                    vg_color_ramp[i].blue = gradientstop[i].color.b;
                    vg_color_ramp[i].alpha = gradientstop[i].color.a;
                }
                if (radialGradient.r > 0)
                {
                    memset(&grad, 0, sizeof(grad));
                    if (paint->grad_image.handle != NULL)
                    {
                        vg_lite_finish();
                        vg_lite_free(&paint->grad_image);
                    }
                    setRadialGrad(&grad, paint->m_colorRampStopsCount, vg_color_ramp, radialGradient, paint->m_colorRampSpreadMode, paint->m_colorRampPremultiplied);
                    updateRadialGrad(context, &grad, &userToSurfaceFill);
                    vg_lite_draw_radial_grad(fbbuf, vglpath, fill_rule, (vg_lite_matrix_t*)&(context->m_pathUserToSurface), &grad, 0, blend, VG_LITE_FILTER_LINEAR);
                    grad.count = 0;
                    paint->grad_image = grad.image;
                }
                else {
                    if (paint->m_colorRampSpreadMode == VG_COLOR_RAMP_SPREAD_REPEAT) {
                        vg_lite_uint8_t a = VG_FLOAT_TO_UB(gradientstop[0].color.a);
                        vg_lite_uint8_t b = VG_FLOAT_TO_UB(gradientstop[0].color.b);
                        vg_lite_uint8_t g = VG_FLOAT_TO_UB(gradientstop[0].color.g);
                        vg_lite_uint8_t r = VG_FLOAT_TO_UB(gradientstop[0].color.r);
                        fill_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                    }
                    else {
                        vg_lite_uint8_t a = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.a);
                        vg_lite_uint8_t b = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.b);
                        vg_lite_uint8_t g = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.g);
                        vg_lite_uint8_t r = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.r);
                        fill_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                    }
                    vg_lite_draw(fbbuf, vglpath, fill_rule, (vg_lite_matrix_t*)&(context->m_pathUserToSurface), blend, fill_color);
                }
                break;
            }

            default:
                break;
            }
        }
        else {
            vg_lite_draw(fbbuf, vglpath, fill_rule, (vg_lite_matrix_t*)&userToSurfaceFill, blend, fill_color);
        }
    }

    if (paintModes & VG_STROKE_PATH)
    {
        vg_lite_int8_t status = 0;
        vg_lite_int8_t cmp = 0;
        do 
        {
            if (vglpath->stroke == NULL) {
                break;
            }

            if (!vglpath->stroke_valid) {
                break;
            }

            if ((vglpath->stroke->line_width != context->m_strokeLineWidth) ||
                (vglpath->stroke->miter_limit != context->m_strokeMiterLimit) ||
                (vglpath->stroke->cap_style != context->m_strokeCapStyle) ||
                (vglpath->stroke->join_style != context->m_strokeJoinStyle)) {
                break;
            }

            if ((vglpath->stroke->pattern_count != (vg_lite_uint32_t)context->m_strokeDashPatternCount) ||
                (vglpath->stroke->dash_reset != (vg_lite_uint8_t)context->m_strokeDashPhaseReset) ||
                (vglpath->stroke->dash_phase != context->m_strokeDashPhase)) {
                break;
            }

            if (vglpath->stroke->pattern_count != 0) {
                cmp = memcmp(
                    vglpath->stroke->dash_pattern,
                    context->m_strokeDashPattern,
                    sizeof(float_t) * vglpath->stroke->pattern_count
                );

                if (cmp != 0) {
                    break;
                }
            }
            status = 1;

        } while (VG_FALSE);

        if (context->m_strokeLineWidth > 0.0f) {
            Paint* paint = (Paint*)context->m_strokePaint;

            vglpath->path_type = VG_LITE_DRAW_STROKE_PATH;
            multMatrix3x3(&userToSurfaceStroke, &(context->m_strokePaintToUser));

            if (paint) {
                switch (paint->m_paintType)
                {
                case VG_PAINT_TYPE_COLOR:
                {
                    vg_lite_uint8_t a = VG_FLOAT_TO_UB(paint->m_paintColor.a);
                    vg_lite_uint8_t b = VG_FLOAT_TO_UB(paint->m_paintColor.b);
                    vg_lite_uint8_t g = VG_FLOAT_TO_UB(paint->m_paintColor.g);
                    vg_lite_uint8_t r = VG_FLOAT_TO_UB(paint->m_paintColor.r);
                    stroke_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);

                    do 
                    {
                        if (status == 1) {
                            break;
                        }
                        vg_lite_set_stroke(vglpath, context->m_strokeCapStyle,
                            context->m_strokeJoinStyle,
                            context->m_strokeLineWidth,
                            context->m_strokeMiterLimit,
                            context->m_strokeDashPattern,
                            context->m_strokeDashPatternCount,
                            context->m_strokeDashPhase,
                            stroke_color);
                        vglpath->stroke->dash_reset = (vg_lite_uint8_t)context->m_strokeDashPhaseReset;
                        vg_lite_update_stroke(vglpath);
                        vglpath->stroke_valid = 1;
                    } while (VG_FALSE);

                    vg_lite_draw(fbbuf, vglpath, fill_rule, (vg_lite_matrix_t*)&userToSurfaceStroke, blend, stroke_color);
                    vg_lite_flush();
                    break;
                }

                case VG_PAINT_TYPE_LINEAR_GRADIENT:
                {
                    vg_lite_ext_linear_gradient_t grad;
                    vg_lite_linear_gradient_parameter_t linear_gradient;
                    vg_lite_color_ramp_t vg_color_ramp[VIV_MAX_COLOR_RAMP_STOPS];
                    GradientStop* gradientstop = paint->m_colorRampStops;

                    linear_gradient.X0 = paint->m_linearGradientPoint0.x;
                    linear_gradient.Y0 = paint->m_linearGradientPoint0.y;
                    linear_gradient.X1 = paint->m_linearGradientPoint1.x;
                    linear_gradient.Y1 = paint->m_linearGradientPoint1.y;

                    for (int i = 0; i < paint->m_colorRampStopsCount; i++)
                    {
                        vg_color_ramp[i].stop = gradientstop[i].offset;
                        vg_color_ramp[i].red = gradientstop[i].color.r;
                        vg_color_ramp[i].green = gradientstop[i].color.g;
                        vg_color_ramp[i].blue = gradientstop[i].color.b;
                        vg_color_ramp[i].alpha = gradientstop[i].color.a;
                    }
                    do
                    {
                        if (status == 1) {
                            break;
                        }
                        vg_lite_set_stroke(vglpath, context->m_strokeCapStyle,
                            context->m_strokeJoinStyle,
                            context->m_strokeLineWidth,
                            context->m_strokeMiterLimit,
                            context->m_strokeDashPattern,
                            context->m_strokeDashPatternCount,
                            context->m_strokeDashPhase,
                            stroke_color);
                        vglpath->stroke->dash_reset = (vg_lite_uint8_t)context->m_strokeDashPhaseReset;
                        vg_lite_update_stroke(vglpath);
                        vglpath->stroke_valid = 1;

                    } while (VG_FALSE);

                    if ((linear_gradient.X0 != linear_gradient.X1) || (linear_gradient.Y0 != linear_gradient.Y1))
                    {
                        memset(&grad, 0, sizeof(grad));
                        if (paint->grad_image.handle != NULL)
                        {
                            vg_lite_finish();
                            vg_lite_free(&paint->grad_image);
                        }
                        setLinearGrad(&grad, paint->m_colorRampStopsCount, vg_color_ramp, linear_gradient, paint->m_colorRampSpreadMode, paint->m_colorRampPremultiplied);
                        updateLinearGrad(context, &grad, &userToSurfaceFill);
                        vg_lite_draw_linear_grad(fbbuf, vglpath, fill_rule, (vg_lite_matrix_t*)&(context->m_pathUserToSurface), &grad, 0, blend, VG_LITE_FILTER_LINEAR);
                        grad.count = 0;
                        paint->grad_image = grad.image;
                    }
                    else {
                        if (paint->m_colorRampSpreadMode == VG_COLOR_RAMP_SPREAD_REPEAT) {
                            vg_lite_uint8_t a = VG_FLOAT_TO_UB(gradientstop[0].color.a);
                            vg_lite_uint8_t b = VG_FLOAT_TO_UB(gradientstop[0].color.b);
                            vg_lite_uint8_t g = VG_FLOAT_TO_UB(gradientstop[0].color.g);
                            vg_lite_uint8_t r = VG_FLOAT_TO_UB(gradientstop[0].color.r);
                            stroke_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                        }
                        else {
                            vg_lite_uint8_t a = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.a);
                            vg_lite_uint8_t b = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.b);
                            vg_lite_uint8_t g = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.g);
                            vg_lite_uint8_t r = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.r);
                            stroke_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                        }

                        vglpath->stroke_color = stroke_color;
                        vg_lite_draw(fbbuf, vglpath, fill_rule, (vg_lite_matrix_t*)&(context->m_pathUserToSurface), blend, fill_color);
                    }
                    break;
                }

                case VG_PAINT_TYPE_RADIAL_GRADIENT:
                {
                    vg_lite_radial_gradient_t grad;
                    vg_lite_radial_gradient_parameter_t radialGradient;
                    vg_lite_color_ramp_t vg_color_ramp[VIV_MAX_COLOR_RAMP_STOPS];
                    GradientStop* gradientstop = paint->m_colorRampStops;

                    radialGradient.cx = paint->m_radialGradientCenter.x;
                    radialGradient.cy = paint->m_radialGradientCenter.y;
                    radialGradient.fx = paint->m_radialGradientFocalPoint.x;
                    radialGradient.fy = paint->m_radialGradientFocalPoint.y;
                    radialGradient.r = paint->m_radialGradientRadius;

                    for (int i = 0; i < paint->m_colorRampStopsCount; i++)
                    {
                        vg_color_ramp[i].stop = gradientstop[i].offset;
                        vg_color_ramp[i].red = gradientstop[i].color.r;
                        vg_color_ramp[i].green = gradientstop[i].color.g;
                        vg_color_ramp[i].blue = gradientstop[i].color.b;
                        vg_color_ramp[i].alpha = gradientstop[i].color.a;
                    }

                    do
                    {
                        if (status == 1) {
                            break;
                        }
                        vg_lite_set_stroke(vglpath, context->m_strokeCapStyle,
                            context->m_strokeJoinStyle,
                            context->m_strokeLineWidth,
                            context->m_strokeMiterLimit,
                            context->m_strokeDashPattern,
                            context->m_strokeDashPatternCount,
                            context->m_strokeDashPhase,
                            stroke_color);
                        vglpath->stroke->dash_reset = (vg_lite_uint8_t)context->m_strokeDashPhaseReset;
                        vg_lite_update_stroke(vglpath);
                        vglpath->stroke_valid = 1;

                    } while (VG_FALSE);

                    if (radialGradient.r > 0)
                    {
                        memset(&grad, 0, sizeof(grad));
                        if (paint->grad_image.handle != NULL)
                        {
                            vg_lite_finish();
                            vg_lite_free(&paint->grad_image);
                        }
                        setRadialGrad(&grad, paint->m_colorRampStopsCount, vg_color_ramp, radialGradient, paint->m_colorRampSpreadMode, paint->m_colorRampPremultiplied);
                        updateRadialGrad(context, &grad, &userToSurfaceFill);
                        vg_lite_draw_radial_grad(fbbuf, vglpath, fill_rule, (vg_lite_matrix_t*)&(context->m_pathUserToSurface), &grad, 0, blend, VG_LITE_FILTER_LINEAR);
                        grad.count = 0;
                        paint->grad_image = grad.image;
                    }
                    else {
                        if (paint->m_colorRampSpreadMode == VG_COLOR_RAMP_SPREAD_REPEAT) {
                            vg_lite_uint8_t a = VG_FLOAT_TO_UB(gradientstop[0].color.a);
                            vg_lite_uint8_t b = VG_FLOAT_TO_UB(gradientstop[0].color.b);
                            vg_lite_uint8_t g = VG_FLOAT_TO_UB(gradientstop[0].color.g);
                            vg_lite_uint8_t r = VG_FLOAT_TO_UB(gradientstop[0].color.r);
                            stroke_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                        }
                        else {
                            vg_lite_uint8_t a = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.a);
                            vg_lite_uint8_t b = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.b);
                            vg_lite_uint8_t g = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.g);
                            vg_lite_uint8_t r = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.r);
                            stroke_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                        }

                        vglpath->stroke_color = stroke_color;
                        vg_lite_draw(fbbuf, vglpath, fill_rule, (vg_lite_matrix_t*)&(context->m_pathUserToSurface), blend, fill_color);
                    }
                    break;
                }

                default:
                    break;
                }
            }
            else {
                do
                {
                    if (status == 1) {
                        break;
                    }
                    vg_lite_set_stroke(vglpath, context->m_strokeCapStyle,
                    context->m_strokeJoinStyle,
                    context->m_strokeLineWidth,
                    context->m_strokeMiterLimit,
                    context->m_strokeDashPattern,
                    context->m_strokeDashPatternCount,
                    context->m_strokeDashPhase,
                    stroke_color);
                    vglpath->stroke->dash_reset = (vg_lite_uint8_t)context->m_strokeDashPhaseReset;
                    vg_lite_update_stroke(vglpath);
                    vglpath->stroke_valid = 1;

                } while (VG_FALSE);

                vg_lite_draw(fbbuf, vglpath, fill_rule, (vg_lite_matrix_t*)&userToSurfaceStroke, blend, stroke_color);
            }
        }
    }

    if (context->m_colorTransform) {
        vg_lite_disable_color_transform();
    }

    return VG_TRUE;
}

void VG_APIENTRY vgDrawPath(VGPath path, VGbitfield paintModes)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidPath(context, path), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid path handle
    VG_IF_ERROR(!paintModes || (paintModes & ~(VG_FILL_PATH | VG_STROKE_PATH)), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);    //invalid paint mode

    if (!drawPath(context, path, context->m_pathUserToSurface, paintModes))
    {
        VG_RETURN(VG_NO_RETVAL);
    }

    VG_RETURN(VG_NO_RETVAL);
}

VGfloat VG_APIENTRY vgPathLength(VGPath path, VGint startSegment, VGint numSegments)
{
    VG_GET_CONTEXT(-1.0f);
    VG_IF_ERROR(!isValidPath(context, path), VG_BAD_HANDLE_ERROR, -1.0f);    //invalid path handle
    Path* p = (Path*)path;
    VG_IF_ERROR(!(p->m_capabilities & VG_PATH_CAPABILITY_PATH_LENGTH), VG_PATH_CAPABILITY_ERROR, -1.0f);    //invalid caps
    VG_IF_ERROR(startSegment < 0 || numSegments <= 0 || VG_INT_ADDSATURATE(startSegment, numSegments) > p->m_numSegments, VG_ILLEGAL_ARGUMENT_ERROR, -1.0f);

    /* Convert path to float format. */
    Path* p_cpy = malloc(sizeof(Path));
    VG_IF_ERROR(p_cpy == NULL, VG_OUT_OF_MEMORY_ERROR, -1.0f);
    memcpy(p_cpy, p, sizeof(Path));
    p_cpy->m_numSegments = startSegment + numSegments;
    Path* p_fp32 = calloc(1, sizeof(Path));
    if (p_fp32 == NULL) {
        free(p_cpy);
        setError(VG_OUT_OF_MEMORY_ERROR);
        OSReleaseMutex();
        return -1.0f;
    }
    p_fp32->m_datatype = VG_PATH_DATATYPE_F;
    appendPathImpl(p_fp32, p_cpy);
    p_fp32->m_numSegments = p_cpy->m_numSegments;
    free(p_cpy);

    /* Calculate path length. */
    VGfloat pathLength = 0.f;
    VGubyte* seg_ptr = p_fp32->m_segments;
    VGubyte** data_list = p_fp32->m_data;
    float* data_ptr, px = 0, py = 0, ox = 0, oy = 0, sx = 0, sy = 0, x0 = 0, y0 = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0,
        t, x_old, y_old, x_new, y_new, seg_len;
    VGboolean is_large = VG_FALSE, is_clockwise = VG_FALSE, is_relative = VG_FALSE;
    VGPathCommand cmd;
    for (int i = 0; i < startSegment + numSegments; ++i) {
        data_ptr = (float*)(*data_list++);
        cmd = *seg_ptr++;
        switch (CMD_TO_SEGMENT(cmd))
        {
        case VG_CLOSE_PATH:
            if (i >= startSegment)
                pathLength += sqrtf((sx - ox) * (sx - ox) + (sy - oy) * (sy - oy));
            px = ox = sx;
            py = oy = sy;
            break;

        case VG_MOVE_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
            }

            sx = px = ox = x0;
            sy = py = oy = y0;
            break;

        case VG_LINE_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
            }

            if (i >= startSegment)
                pathLength += sqrtf((x0 - ox) * (x0 - ox) + (y0 - oy) * (y0 - oy));

            px = ox = x0;
            py = oy = y0;
            break;

        case VG_HLINE_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
            }
            else {
                x0 = data_ptr[0] + ox;
            }

            if (i >= startSegment)
                pathLength += (x0 > ox) ? (x0 - ox) : (ox - x0);

            px = x0;
            py = oy;
            ox = x0;
            break;

        case VG_VLINE_TO:
            if (CMD_IS_ABS(cmd)) {
                y0 = data_ptr[0];
            }
            else {
                y0 = data_ptr[0] + oy;
            }

            if (i >= startSegment)
                pathLength += (y0 > oy) ? (y0 - oy) : (oy - y0);

            px = ox;
            py = y0;
            oy = y0;
            break;

        case VG_QUAD_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
                x1 = data_ptr[2];
                y1 = data_ptr[3];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
                x1 = data_ptr[2] + ox;
                y1 = data_ptr[3] + oy;
            }

            if (i >= startSegment) {
                x_old = ox;
                y_old = oy;
                for (VGuint j = 1; j < 256; ++j) {
                    t = (VGfloat)j / 256.f;
                    x_new = ox * (1.f - t) * (1.f - t) + 2 * x0 * (1.f - t) * t + x1 * t * t;
                    y_new = oy * (1.f - t) * (1.f - t) + 2 * y0 * (1.f - t) * t + y1 * t * t;
                    pathLength += sqrtf((x_new - x_old) * (x_new - x_old) + (y_new - y_old) * (y_new - y_old));
                    x_old = x_new;
                    y_old = y_new;
                }
            }

            px = x0;
            py = y0;
            ox = x1;
            oy = y1;
            break;

        case VG_CUBIC_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
                x1 = data_ptr[2];
                y1 = data_ptr[3];
                x2 = data_ptr[4];
                y2 = data_ptr[5];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
                x1 = data_ptr[2] + ox;
                y1 = data_ptr[3] + oy;
                x2 = data_ptr[4] + ox;
                y2 = data_ptr[5] + oy;
            }

            if (i >= startSegment) {
                x_old = ox;
                y_old = oy;
                for (VGuint j = 1; j < 256; ++j) {
                    t = (VGfloat)j / 256.f;
                    x_new = ox * (1.f - t) * (1.f - t) * (1.f - t)
                        + 3 * x0 * (1.f - t) * (1.f - t) * t
                        + 3 * x1 * (1.f - t) * t * t
                        + x2 * t * t * t;
                    y_new = oy * (1.f - t) * (1.f - t) * (1.f - t)
                        + 3 * y0 * (1.f - t) * (1.f - t) * t
                        + 3 * y1 * (1.f - t) * t * t
                        + y2 * t * t * t;
                    pathLength += sqrtf((x_new - x_old) * (x_new - x_old) + (y_new - y_old) * (y_new - y_old));
                    x_old = x_new;
                    y_old = y_new;
                }
            }

            px = x1;
            py = y1;
            ox = x2;
            oy = y2;
            break;

        case VG_SQUAD_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = 2.f * ox - px;
                y0 = 2.f * oy - py;
                x1 = data_ptr[0];
                y1 = data_ptr[1];
            }
            else {
                x0 = 2.f * ox - px + ox;
                y0 = 2.f * oy - py + oy;
                x1 = data_ptr[0] + ox;
                y1 = data_ptr[1] + oy;
            }

            if (i >= startSegment) {
                x_old = ox;
                y_old = oy;
                for (VGuint j = 1; j < 256; ++j) {
                    t = (VGfloat)j / 256.f;
                    x_new = ox * (1.f - t) * (1.f - t) + 2 * x0 * (1.f - t) * t + x1 * t * t;
                    y_new = oy * (1.f - t) * (1.f - t) + 2 * y0 * (1.f - t) * t + y1 * t * t;
                    pathLength += sqrtf((x_new - x_old) * (x_new - x_old) + (y_new - y_old) * (y_new - y_old));
                    x_old = x_new;
                    y_old = y_new;
                }
            }

            px = x0;
            py = y0;
            ox = x1;
            oy = y1;
            break;

        case VG_SCUBIC_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = 2.f * ox - px;
                y0 = 2.f * oy - py;
                x1 = data_ptr[0];
                y1 = data_ptr[1];
                x2 = data_ptr[2];
                y2 = data_ptr[3];
            }
            else {
                x0 = 2.f * ox - px + ox;
                y0 = 2.f * oy - py + oy;
                x1 = data_ptr[0] + ox;
                y1 = data_ptr[1] + oy;
                x2 = data_ptr[2] + ox;
                y2 = data_ptr[3] + oy;
            }

            if (i >= startSegment) {
                x_old = ox;
                y_old = oy;
                for (VGuint j = 1; j < 256; ++j) {
                    t = (VGfloat)j / 256.f;
                    x_new = ox * (1.f - t) * (1.f - t) * (1.f - t)
                        + 3 * x0 * (1.f - t) * (1.f - t) * t
                        + 3 * x1 * (1.f - t) * t * t
                        + x2 * t * t * t;
                    y_new = oy * (1.f - t) * (1.f - t) * (1.f - t)
                        + 3 * y0 * (1.f - t) * (1.f - t) * t
                        + 3 * y1 * (1.f - t) * t * t
                        + y2 * t * t * t;
                    pathLength += sqrtf((x_new - x_old) * (x_new - x_old) + (y_new - y_old) * (y_new - y_old));
                    x_old = x_new;
                    y_old = y_new;
                }
            }

            px = x1;
            py = y1;
            ox = x2;
            oy = y2;
            break;

        case VG_SCCWARC_TO:
        case VG_SCWARC_TO:
        case VG_LCCWARC_TO:
        case VG_LCWARC_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[3];
                y0 = data_ptr[4];
                is_relative = VG_FALSE;
            }
            else {
                x0 = data_ptr[3] + ox;
                y0 = data_ptr[4] + oy;
                is_relative = VG_TRUE;;
            }

            if (x0 == ox && y0 == oy)
                break;

            switch (CMD_TO_SEGMENT(cmd))
            {
            case VG_SCCWARC_TO:
                is_large = VG_FALSE;
                is_clockwise = VG_FALSE;
                break;

            case VG_SCWARC_TO:
                is_large = VG_FALSE;
                is_clockwise = VG_TRUE;
                break;

            case VG_LCCWARC_TO:
                is_large = VG_TRUE;
                is_clockwise = VG_FALSE;
                break;

            case VG_LCWARC_TO:
                is_large = VG_TRUE;
                is_clockwise = VG_TRUE;
                break;
            }
            if (i >= startSegment) {
                VGfloat rh = data_ptr[0], rv = data_ptr[1], rot = data_ptr[2];
                /* Find the center of ellipse. */
                VGfloat cx, cy, theta0, theta_range;
                findEllipse(&cx, &cy, &rh, &rv, &theta0, &theta_range, rot, ox, oy, data_ptr[3], data_ptr[4], is_large, is_clockwise, is_relative);

                /* Update boundary. */
                if (rh != 0 && rv != 0){
                    VGfloat ax = ox - (VGfloat)cos(theta0) * rh;
                    VGfloat ay = oy - (VGfloat)sin(theta0) * rv;
                    VGint segs = (VGint)((VGfloat)ceil(((VGfloat)fabs(theta_range)) / (45.0f / 180.0f * PI)));
                    VGfloat theta_step = theta_range / segs;
                    VGfloat COS = (VGfloat)cos(rot * PI / 180.f);
                    VGfloat SIN = (VGfloat)sin(rot * PI / 180.f);
                    while (segs-- > 0) {
                        theta0 += theta_step;
                        VGfloat controlX = ax + (VGfloat)cos(theta0 - (theta_step / 2.0f)) * rh / (VGfloat)cos(theta_step / 2.0f);
                        VGfloat controlY = ay + (VGfloat)sin(theta0 - (theta_step / 2.0f)) * rv / (VGfloat)cos(theta_step / 2.0f);
                        VGfloat anchorX = ax + (VGfloat)cos(theta0) * rh;
                        VGfloat anchorY = ay + (VGfloat)sin(theta0) * rv;
                        if (rot != 0) {
                            VGfloat x = ox + COS * (controlX - ox) - SIN * (controlY - oy);
                            VGfloat y = oy + SIN * (controlX - ox) + COS * (controlY - oy);
                            controlX = x;
                            controlY = y;
                            x = ox + COS * (anchorX - ox) - SIN * (anchorY - oy);
                            y = oy + SIN * (anchorX - ox) + COS * (anchorY - oy);
                            anchorX = x;
                            anchorY = y;
                        }
                        if (segs == 0)
                        {
                            /* Use end point directly to avoid accumulated errors. */
                            anchorX = x0;
                            anchorY = y0;
                        }
                        VGfloat x_old = ox, y_old = oy, x_new, y_new;
                        seg_len = 0.f;
                        for (VGuint j = 0; j < 256; ++j) {
                            t = (VGfloat)j / 256.f;
                            x_new = ox * (1.f - t) * (1.f - t) + 2 * controlX * (1.f - t) * t + anchorX * t * t;
                            y_new = oy * (1.f - t) * (1.f - t) + 2 * controlY * (1.f - t) * t + anchorY * t * t;
                            seg_len += sqrtf((x_new - x_old) * (x_new - x_old) + (y_new - y_old) * (y_new - y_old));
                            x_old = x_new;
                            y_old = y_new;
                        }
                        pathLength += seg_len;
                        ox = anchorX;
                        oy = anchorY;
                    }
                }
                else {
                    pathLength += sqrtf((x0 - ox) * (x0- ox) + (y0 - oy) * (y0 - oy));
                }
            }

            px = ox = x0;
            py = oy = y0;
            break;
        }
    }

    freePathImpl(p_fp32);

    VG_RETURN(pathLength);
}

void VG_APIENTRY vgPointAlongPath(VGPath path, VGint startSegment, VGint numSegments, VGfloat distance, VGfloat* x, VGfloat* y, VGfloat* tangentX, VGfloat* tangentY)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    VG_IF_ERROR(!isValidPath(context, path), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid path handle
    Path* p = (Path*)path;
    VG_IF_ERROR((x && y && !(p->m_capabilities & VG_PATH_CAPABILITY_POINT_ALONG_PATH)) ||
        (tangentX && tangentY && !(p->m_capabilities & VG_PATH_CAPABILITY_TANGENT_ALONG_PATH)), VG_PATH_CAPABILITY_ERROR, VG_NO_RETVAL);    //invalid caps
    VG_IF_ERROR(startSegment < 0 || numSegments <= 0 || VG_INT_ADDSATURATE(startSegment, numSegments) > p->m_numSegments, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!isAligned(x, 4) || !isAligned(y, 4) || !isAligned(tangentX, 4) || !isAligned(tangentY, 4), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VGfloat factor = 0;
    VGfloat incomingTangentX = 0;
    VGfloat incomingTangentY = 0;
    VGfloat incomingX = 0;
    VGfloat incomingY = 0;

    /* Clamp distance. */
    VGfloat distance_max = vgPathLength(path, startSegment, numSegments);
    distance = min(max(distance, 0.f), distance_max);

    /* Convert path to float format. */
    Path* p_cpy = malloc(sizeof(Path));
    VG_IF_ERROR(p_cpy == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
    memcpy(p_cpy, p, sizeof(Path));
    p_cpy->m_numSegments = startSegment + numSegments;
    Path* p_fp32 = calloc(1, sizeof(Path));
    if (p_fp32 == NULL) {
        free(p_cpy);
        setError(VG_OUT_OF_MEMORY_ERROR);
        OSReleaseMutex();
        return VG_NO_RETVAL;
    }
    p_fp32->m_datatype = VG_PATH_DATATYPE_F;
    appendPathImpl(p_fp32, p_cpy);
    p_fp32->m_numSegments = p_cpy->m_numSegments;
    free(p_cpy);

    /* Get point along path */
    VGfloat path_length = 0.f;
    VGubyte* seg_ptr = p_fp32->m_segments;
    VGubyte** data_list = p_fp32->m_data;
    float* data_ptr, px = 0, py = 0, ox = 0, oy = 0, sx = 0, sy = 0, x0 = 0, y0 = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0,
        t, x_old, y_old, x_new, y_new, seg_length;
    VGint endSegment = startSegment + numSegments - 1;
    VGPathCommand cmd, next_cmd;
    VGboolean is_large = VG_FALSE, is_clockwise = VG_FALSE, is_relative = VG_FALSE;
    for (VGint i = 0; i <= endSegment; ++i) {
        data_ptr = (float*)(*data_list++);
        cmd = *seg_ptr++;
        next_cmd = (i == endSegment) ? 0 : *seg_ptr;
        switch (CMD_TO_SEGMENT(cmd)) {
        case VG_CLOSE_PATH:
            if (i >= startSegment) {
                seg_length = sqrtf((sx - ox) * (sx - ox) + (sy - oy) * (sy - oy));
                if (path_length + seg_length >= distance || i == endSegment) {
                    pointAlongLine(ox, oy, sx, sy, distance - path_length, x, y, tangentX, tangentY, &factor);
                    freePathImpl(p_fp32);
                    VG_RETURN(VG_NO_RETVAL);
                }
                path_length += seg_length;
            }

            px = ox = sx;
            py = oy = sy;
            break;

        case VG_MOVE_TO:
            if (i == endSegment) {
                if (x) *x = ox;
                if (y) *y = oy;

                if (tangentX) *tangentX = 1.f;
                if (tangentY) *tangentY = 0.f;
                freePathImpl(p_fp32);
                VG_RETURN(VG_NO_RETVAL);
            }
            if (CMD_TO_SEGMENT(next_cmd) == VG_MOVE_TO) // invalid move_to
                break;

            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
            }

            sx = px = ox = x0;
            sy = py = oy = y0;
            incomingX = x0;
            incomingY = y0;
            break;

        case VG_LINE_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
            }

            if (i >= startSegment) {
                seg_length = sqrtf((x0 - ox) * (x0 - ox) + (y0 - oy) * (y0 - oy));
                if (path_length + seg_length >= distance || i == endSegment) {
                    pointAlongLine(ox, oy, x0, y0, distance - path_length, x, y, tangentX, tangentY, &factor);
                    freePathImpl(p_fp32);
                    VG_RETURN(VG_NO_RETVAL);
                }
                path_length += seg_length;
            }

            px = ox = x0;
            py = oy = y0;
            break;

        case VG_HLINE_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
            }
            else {
                x0 = data_ptr[0] + ox;
            }

            if (i >= startSegment) {
                seg_length = (x0 > ox) ? (x0 - ox) : (ox - x0);
                if (path_length + seg_length >= distance || i == endSegment) {
                    pointAlongLine(ox, oy, x0, oy, distance - path_length, x, y, tangentX, tangentY, &factor);
                    if (tangentX) *tangentX = 1.f;
                    if (tangentY) *tangentY = 0.f;
                    freePathImpl(p_fp32);
                    VG_RETURN(VG_NO_RETVAL);
                }
                path_length += seg_length;
            }

            px = x0;
            py = oy;
            ox = x0;
            break;

        case VG_VLINE_TO:
            if (CMD_IS_ABS(cmd)) {
                y0 = data_ptr[0];
            }
            else {
                y0 = data_ptr[0] + oy;
            }

            if (i >= startSegment) {
                seg_length = (y0 > oy) ? (y0 - oy) : (oy - y0);
                if (path_length + seg_length >= distance || i == endSegment) {
                    pointAlongLine(ox, oy, ox, y0, distance - path_length, x, y, tangentX, tangentY, &factor);
                    if (tangentX) *tangentX = 0.f;
                    if (tangentY) *tangentY = 1.f;
                    freePathImpl(p_fp32);
                    VG_RETURN(VG_NO_RETVAL);
                }
                path_length += seg_length;
            }

            px = ox;
            py = y0;
            oy = y0;
            break;

        case VG_QUAD_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
                x1 = data_ptr[2];
                y1 = data_ptr[3];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
                x1 = data_ptr[2] + ox;
                y1 = data_ptr[3] + oy;
            }
            incomingTangentX = (VGfloat)((double)(x0 - incomingX) / sqrt((x0 - incomingX) * (x0 - incomingX) + (y0 - incomingY) * (y0 - incomingY)));
            incomingTangentY = (VGfloat)((double)(y0 - incomingY) / sqrt((x0 - incomingX) * (x0 - incomingX) + (y0 - incomingY) * (y0 - incomingY)));
            if (i >= startSegment) {
                x_old = ox;
                y_old = oy;
                for (VGuint j = 1; j < 256; ++j) {
                    t = (VGfloat)j / 256.f;
                    x_new = ox * (1.f - t) * (1.f - t) + 2 * x0 * (1.f - t) * t + x1 * t * t;
                    y_new = oy * (1.f - t) * (1.f - t) + 2 * y0 * (1.f - t) * t + y1 * t * t;
                    seg_length = sqrtf((x_new - x_old) * (x_new - x_old) + (y_new - y_old) * (y_new - y_old));
                    float tangentX1 = (-1.0f + t) * ox + (1.0f - 2.0f * t) * x0 + t * x1;
                    float tangentY1 = (-1.0f + t) * oy + (1.0f - 2.0f * t) * y0 + t * y1;
                    float tangentX2 = (VGfloat)((double)tangentX1 / sqrt(tangentX1 * tangentX1 + tangentY1 * tangentY1));
                    float tangentY2 = (VGfloat)((double)tangentY1 / sqrt(tangentX1 * tangentX1 + tangentY1 * tangentY1));
                    if (path_length + seg_length >= distance || (i == endSegment && j == 255)) {
                        if (seg_length == 0.f) {    /* (distance == 0 && seg_length == 0) is invalid param. */
                            if (x) *x = x0;
                            if (y) *y = y0;
                            if (tangentX) *tangentX = 1.f;
                            if (tangentY) *tangentY = 0.f;
                        }
                        else {
                            factor = distance / seg_length;
                            if (x) *x = x_old + (x_new - x_old) * factor;
                            if (y) *y = y_old + (y_new - y_old) * factor;
                            //if (tangentX) *tangentX = (x1 - x0) / seg_length;
                            //if (tangentY) *tangentY = (y1 - y0) / seg_length;
                            if (tangentX) *tangentX = incomingTangentX + (tangentX2 - incomingTangentX) * factor;
                            if (tangentY) *tangentY = incomingTangentY + (tangentY2 - incomingTangentY) * factor;
                        }

                        freePathImpl(p_fp32);
                        VG_RETURN(VG_NO_RETVAL);
                    }
                    incomingTangentX = *tangentX;
                    incomingTangentY = *tangentY;
                    path_length += seg_length;
                    x_old = x_new;
                    y_old = y_new;
                }
            }

            px = x0;
            py = y0;
            ox = x1;
            oy = y1;
            incomingX = x1;
            incomingY = y1;
            break;

        case VG_CUBIC_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
                x1 = data_ptr[2];
                y1 = data_ptr[3];
                x2 = data_ptr[4];
                y2 = data_ptr[5];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
                x1 = data_ptr[2] + ox;
                y1 = data_ptr[3] + oy;
                x2 = data_ptr[4] + ox;
                y2 = data_ptr[5] + oy;
            }

            if (i >= startSegment) {
                x_old = ox;
                y_old = oy;
                for (VGuint j = 1; j < 256; ++j) {
                    t = (VGfloat)j / 256.f;
                    x_new = ox * (1.f - t) * (1.f - t) * (1.f - t)
                        + 3 * x0 * (1.f - t) * (1.f - t) * t
                        + 3 * x1 * (1.f - t) * t * t
                        + x2 * t * t * t;
                    y_new = oy * (1.f - t) * (1.f - t) * (1.f - t)
                        + 3 * y0 * (1.f - t) * (1.f - t) * t
                        + 3 * y1 * (1.f - t) * t * t
                        + y2 * t * t * t;
                    seg_length = sqrtf((x_new - x_old) * (x_new - x_old) + (y_new - y_old) * (y_new - y_old));
                    if (path_length + seg_length >= distance || (i == endSegment && j == 255)) {
                        pointAlongLine(x_old, y_old, x_new, y_new, distance - path_length, x, y, tangentX, tangentY, &factor);
                        freePathImpl(p_fp32);
                        VG_RETURN(VG_NO_RETVAL);
                    }
                    path_length += seg_length;
                    x_old = x_new;
                    y_old = y_new;
                }
            }

            px = x1;
            py = y1;
            ox = x2;
            oy = y2;
            break;

        case VG_SQUAD_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = 2.f * ox - px;
                y0 = 2.f * oy - py;
                x1 = data_ptr[0];
                y1 = data_ptr[1];
            }
            else {
                x0 = 2.f * ox - px + ox;
                y0 = 2.f * oy - py + oy;
                x1 = data_ptr[0] + ox;
                y1 = data_ptr[1] + oy;
            }

            incomingTangentX = (VGfloat)((double)(x0 - incomingX) / sqrt((x0 - incomingX) * (x0 - incomingX) + (y0 - incomingY) * (y0 - incomingY)));
            incomingTangentY = (VGfloat)((double)(y0 - incomingY) / sqrt((x0 - incomingX) * (x0 - incomingX) + (y0 - incomingY) * (y0 - incomingY)));
            if (i >= startSegment) {
                x_old = ox;
                y_old = oy;
                for (VGuint j = 1; j < 256; ++j) {
                    t = (VGfloat)j / 256.f;
                    x_new = ox * (1.f - t) * (1.f - t) + 2 * x0 * (1.f - t) * t + x1 * t * t;
                    y_new = oy * (1.f - t) * (1.f - t) + 2 * y0 * (1.f - t) * t + y1 * t * t;
                    seg_length = sqrtf((x_new - x_old) * (x_new - x_old) + (y_new - y_old) * (y_new - y_old));
                    float tangentX1 = (-1.0f + t) * ox + (1.0f - 2.0f * t) * x0 + t * x1;
                    float tangentY1 = (-1.0f + t) * oy + (1.0f - 2.0f * t) * y0 + t * y1;
                    float tangentX2 = (VGfloat)((double)tangentX1 / sqrt(tangentX1 * tangentX1 + tangentY1 * tangentY1));
                    float tangentY2 = (VGfloat)((double)tangentY1 / sqrt(tangentX1 * tangentX1 + tangentY1 * tangentY1));
                    if (path_length + seg_length >= distance || (i == endSegment && j == 255)) {
                        if (seg_length == 0.f) {    /* (distance == 0 && seg_length == 0) is invalid param. */
                            if (x) *x = x0;
                            if (y) *y = y0;
                            if (tangentX) *tangentX = 1.f;
                            if (tangentY) *tangentY = 0.f;
                        }
                        else {
                            factor = distance / seg_length;
                            if (x) *x = x_old + (x_new - x_old) * factor;
                            if (y) *y = y_old + (y_new - y_old) * factor;
                            if (tangentX) *tangentX = incomingTangentX + (tangentX2 - incomingTangentX) * factor;
                            if (tangentY) *tangentY = incomingTangentY + (tangentY2 - incomingTangentY) * factor;
                        }

                        freePathImpl(p_fp32);
                        VG_RETURN(VG_NO_RETVAL);
                    }
                }
            }

            px = x0;
            py = y0;
            ox = x1;
            oy = y1;
            break;

        case VG_SCUBIC_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = 2.f * ox - px;
                y0 = 2.f * oy - py;
                x1 = data_ptr[0];
                y1 = data_ptr[1];
                x2 = data_ptr[2];
                y2 = data_ptr[3];
            }
            else {
                x0 = 2.f * ox - px + ox;
                y0 = 2.f * oy - py + oy;
                x1 = data_ptr[0] + ox;
                y1 = data_ptr[1] + oy;
                x2 = data_ptr[2] + ox;
                y2 = data_ptr[3] + oy;
            }

            if (i >= startSegment) {
                x_old = ox;
                y_old = oy;
                for (VGuint j = 1; j < 256; ++j) {
                    t = (VGfloat)j / 256.f;
                    x_new = ox * (1.f - t) * (1.f - t) * (1.f - t)
                        + 3 * x0 * (1.f - t) * (1.f - t) * t
                        + 3 * x1 * (1.f - t) * t * t
                        + x2 * t * t * t;
                    y_new = oy * (1.f - t) * (1.f - t) * (1.f - t)
                        + 3 * y0 * (1.f - t) * (1.f - t) * t
                        + 3 * y1 * (1.f - t) * t * t
                        + y2 * t * t * t;
                    seg_length = sqrtf((x_new - x_old) * (x_new - x_old) + (y_new - y_old) * (y_new - y_old));
                    if (path_length + seg_length >= distance || (i == endSegment && j == 255)) {
                        pointAlongLine(x_old, y_old, x_new, y_new, distance - path_length, x, y, tangentX, tangentY, &factor);
                        freePathImpl(p_fp32);
                        VG_RETURN(VG_NO_RETVAL);
                    }
                    path_length += seg_length;
                    x_old = x_new;
                    y_old = y_new;
                }
            }

            px = x1;
            py = y1;
            ox = x2;
            oy = y2;
            break;

        case VG_SCCWARC_TO:
        case VG_SCWARC_TO:
        case VG_LCCWARC_TO:
        case VG_LCWARC_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[3];
                y0 = data_ptr[4];
                is_relative = VG_FALSE;
            }
            else {
                x0 = data_ptr[3] + ox;
                y0 = data_ptr[4] + oy;
                is_relative = VG_TRUE;;
            }

            if (x0 == ox && y0 == oy) {
                if (i == endSegment) {
                    if (x) *x = x0;
                    if (y) *y = y0;
                    if (tangentX) *tangentX = 1.f;
                    if (tangentY) *tangentY = 0.f;
                    freePathImpl(p_fp32);
                    VG_RETURN(VG_NO_RETVAL);
                }
                else
                    break;
            }

            switch (CMD_TO_SEGMENT(cmd))
            {
            case VG_SCCWARC_TO:
                is_large = VG_FALSE;
                is_clockwise = VG_FALSE;
                break;

            case VG_SCWARC_TO:
                is_large = VG_FALSE;
                is_clockwise = VG_TRUE;
                break;

            case VG_LCCWARC_TO:
                is_large = VG_TRUE;
                is_clockwise = VG_FALSE;
                break;

            case VG_LCWARC_TO:
                is_large = VG_TRUE;
                is_clockwise = VG_TRUE;
                break;
            }
            if (i >= startSegment) {
                VGfloat rh = data_ptr[0], rv = data_ptr[1], rot = data_ptr[2];
                /* Find the center of ellipse. */
                VGfloat cx, cy, theta0, theta_range;
                findEllipse(&cx, &cy, &rh, &rv, &theta0, &theta_range, rot, ox, oy, data_ptr[3], data_ptr[4], is_large, is_clockwise, is_relative);

                if (rh != 0 && rv != 0) {
                    VGfloat ax = ox - (VGfloat)cos(theta0) * rh;
                    VGfloat ay = oy - (VGfloat)sin(theta0) * rv;
                    VGint segs = (VGint)((VGfloat)ceil(((VGfloat)fabs(theta_range)) / (45.0f / 180.0f * PI)));
                    VGfloat theta_step = theta_range / segs;
                    VGfloat COS = (VGfloat)cos(rot * PI / 180.f);
                    VGfloat SIN = (VGfloat)sin(rot * PI / 180.f);
                    while (segs-- > 0) {
                        theta0 += theta_step;
                        VGfloat controlX = ax + (VGfloat)cos(theta0 - (theta_step / 2.0f)) * rh / (VGfloat)cos(theta_step / 2.0f);
                        VGfloat controlY = ay + (VGfloat)sin(theta0 - (theta_step / 2.0f)) * rv / (VGfloat)cos(theta_step / 2.0f);
                        VGfloat anchorX = ax + (VGfloat)cos(theta0) * rh;
                        VGfloat anchorY = ay + (VGfloat)sin(theta0) * rv;
                        if (rot != 0) {
                            VGfloat x = ox + COS * (controlX - ox) - SIN * (controlY - oy);
                            VGfloat y = oy + SIN * (controlX - ox) + COS * (controlY - oy);
                            controlX = x;
                            controlY = y;
                            x = ox + COS * (anchorX - ox) - SIN * (anchorY - oy);
                            y = oy + SIN * (anchorX - ox) + COS * (anchorY - oy);
                            anchorX = x;
                            anchorY = y;
                        }
                        if (segs == 0)
                        {
                            /* Use end point directly to avoid accumulated errors. */
                            anchorX = x0;
                            anchorY = y0;
                        }
                        VGfloat seg_length;
                        VGfloat x_old = ox, y_old = oy, x_new, y_new;
                        for (VGuint j = 0; j < 256; ++j) {
                            t = (VGfloat)j / 256.f;
                            x_new = ox * (1.f - t) * (1.f - t) + 2 * controlX * (1.f - t) * t + anchorX * t * t;
                            y_new = oy * (1.f - t) * (1.f - t) + 2 * controlY * (1.f - t) * t + anchorY * t * t;
                            seg_length = sqrtf((x_new - x_old) * (x_new - x_old) + (y_new - y_old) * (y_new - y_old));
                            if (path_length + seg_length >= distance || (i == endSegment && j == 255)) {
                                pointAlongLine(x_old, y_old, x_new, y_new, distance - path_length, x, y, tangentX, tangentY, &factor);
                                freePathImpl(p_fp32);
                                VG_RETURN(VG_NO_RETVAL);
                            }
                            path_length += seg_length;
                            x_old = x_new;
                            y_old = y_new;
                        }
                        ox = anchorX;
                        oy = anchorY;
                    }
                }
                else {
                    VGfloat seg_length = sqrtf((x0 - ox) * (x0 - ox) + (y0 - oy) * (y0 - oy));
                    if (path_length + seg_length >= distance || i == endSegment) {
                        pointAlongLine(ox, oy, x0, y0, distance - path_length, x, y, tangentX, tangentY, &factor);
                        freePathImpl(p_fp32);
                        VG_RETURN(VG_NO_RETVAL);
                    }
                    path_length += seg_length;
                }
            }

            px = ox = x0;
            py = oy = y0;
            break;
        }
    }

    freePathImpl(p_fp32);
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgPathBounds(VGPath path, VGfloat* minx, VGfloat* miny, VGfloat* width, VGfloat* height)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidPath(context, path), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid path handle
    VG_IF_ERROR(!(((Path*)path)->m_capabilities & VG_PATH_CAPABILITY_PATH_BOUNDS), VG_PATH_CAPABILITY_ERROR, VG_NO_RETVAL);    //invalid caps
    VG_IF_ERROR(!minx || !miny || !width || !height, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!isAligned(minx, 4) || !isAligned(miny, 4) || !isAligned(width, 4) || !isAligned(height, 4), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    Path* p = (Path*)path;

    /* Check if path is empty. */
    if (p->m_numSegments == 0) {
        if (minx) *minx = 0.f;
        if (miny) *miny = 0.f;
        if (width) *width = -1.f;
        if (height) *height = -1.f;
        VG_RETURN(VG_NO_RETVAL);
    }

    /* Convert path to float format. */
    Path* p_fp32 = calloc(1, sizeof(Path));
    VG_IF_ERROR(p_fp32 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
    p_fp32->m_datatype = VG_PATH_DATATYPE_F;
    appendPathImpl(p_fp32, p);
    p_fp32->m_numSegments = p->m_numSegments;

    /* Set the basic point. */
    VGubyte* seg_ptr = p_fp32->m_segments;
    VGubyte** data_list = p_fp32->m_data;
    VGfloat x_min = 0.f, y_min = 0.f, x_max = 0.f, y_max = 0.f, * data_ptr;
    data_ptr = (float*)(*data_list);
    switch (*seg_ptr)
    {
    case VG_MOVE_TO_ABS:
    case VG_MOVE_TO_REL:
        x_min = x_max = data_ptr[0];
        y_min = y_max = data_ptr[1];
        break;
}

    /* Calculate bounds. */
    float px = 0, py = 0, ox = 0, oy = 0, sx = 0, sy = 0, x0 = 0, y0 = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0,
        x, y;
    VGboolean is_large = VG_FALSE, is_clockwise = VG_FALSE, is_relative = VG_FALSE;
    VGPathCommand cmd;
    for (int i = 0; i < p_fp32->m_numSegments; ++i) {
        data_ptr = (float*)(*data_list++);
        cmd = *seg_ptr++;
        switch (CMD_TO_SEGMENT(cmd)) {
        case VG_CLOSE_PATH:
            px = ox = sx;
            py = oy = sy;
            break;

        case VG_MOVE_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
            }

            x_min = min(x0, x_min);
            y_min = min(y0, y_min);
            x_max = max(x0, x_max);
            y_max = max(y0, y_max);

            sx = px = ox = x0;
            sy = py = oy = y0;
            break;

        case VG_LINE_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
            }

            x_min = min(x0, x_min);
            y_min = min(y0, y_min);
            x_max = max(x0, x_max);
            y_max = max(y0, y_max);

            px = ox = x0;
            py = oy = y0;
            break;

        case VG_HLINE_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
            }
            else {
                x0 = data_ptr[0] + ox;
            }

            x_min = min(x0, x_min);
            x_max = max(x0, x_max);

            px = x0;
            py = oy;
            ox = x0;
            break;

        case VG_VLINE_TO:
            if (CMD_IS_ABS(cmd)) {
                y0 = data_ptr[0];
            }
            else {
                y0 = data_ptr[0] + oy;
            }

            y_min = min(y0, y_min);
            y_max = max(y0, y_max);

            px = ox;
            py = y0;
            oy = y0;
            break;

        case VG_QUAD_TO: {
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
                x1 = data_ptr[2];
                y1 = data_ptr[3];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
                x1 = data_ptr[2] + ox;
                y1 = data_ptr[3] + oy;
            }

            float bound[4];
            float curve[6] = { ox, oy, x0, y0, x1, y1 };
            findQuadBound(bound, curve);
            x_min = min(bound[0], x_min);
            y_min = min(bound[1], y_min);
            x_max = max(bound[2], x_max);
            y_max = max(bound[3], y_max);

            px = x0;
            py = y0;
            ox = x1;
            oy = y1;
            break; 
        }

        case VG_CUBIC_TO: {
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
                x1 = data_ptr[2];
                y1 = data_ptr[3];
                x2 = data_ptr[4];
                y2 = data_ptr[5];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
                x1 = data_ptr[2] + ox;
                y1 = data_ptr[3] + oy;
                x2 = data_ptr[4] + ox;
                y2 = data_ptr[5] + oy;
            }

            float bound[4];
            float curve[8] = { ox, oy, x0, y0, x1, y1, x2, y2 };
            findCubicBound(bound, curve);
            x_min = min(bound[0], x_min);
            y_min = min(bound[1], y_min);
            x_max = max(bound[2], x_max);
            y_max = max(bound[3], y_max);

            px = x1;
            py = y1;
            ox = x2;
            oy = y2;
            break;
        }

        case VG_SQUAD_TO: {
            if (CMD_IS_ABS(cmd)) {
                x0 = 2.f * ox - px;
                y0 = 2.f * oy - py;
                x1 = data_ptr[0];
                y1 = data_ptr[1];
            }
            else {
                x0 = 2.f * ox - px + ox;
                y0 = 2.f * oy - py + oy;
                x1 = data_ptr[0] + ox;
                y1 = data_ptr[1] + oy;
            }

            float bound[4];
            float curve[6] = { ox, oy, x0, y0, x1, y1 };
            findQuadBound(bound, curve);
            x_min = min(bound[0], x_min);
            y_min = min(bound[1], y_min);
            x_max = max(bound[2], x_max);
            y_max = max(bound[3], y_max);

            px = x0;
            py = y0;
            ox = x1;
            oy = y1;
            break;
        }

        case VG_SCUBIC_TO: {
            if (CMD_IS_ABS(cmd)) {
                x0 = 2.f * ox - px;
                y0 = 2.f * oy - py;
                x1 = data_ptr[0];
                y1 = data_ptr[1];
                x2 = data_ptr[2];
                y2 = data_ptr[3];
            }
            else {
                x0 = 2.f * ox - px + ox;
                y0 = 2.f * oy - py + oy;
                x1 = data_ptr[0] + ox;
                y1 = data_ptr[1] + oy;
                x2 = data_ptr[2] + ox;
                y2 = data_ptr[3] + oy;
            }

            float bound[4];
            float curve[8] = { ox, oy, x0, y0, x1, y1, x2, y2 };
            findCubicBound(bound, curve);
            x_min = min(bound[0], x_min);
            y_min = min(bound[1], y_min);
            x_max = max(bound[2], x_max);
            y_max = max(bound[3], y_max);

            px = x1;
            py = y1;
            ox = x2;
            oy = y2;
            break;
        }

        case VG_SCCWARC_TO:
        case VG_SCWARC_TO:
        case VG_LCCWARC_TO:
        case VG_LCWARC_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[3];
                y0 = data_ptr[4];
                is_relative = VG_FALSE;
            }
            else {
                x0 = data_ptr[3] + ox;
                y0 = data_ptr[4] + oy;
                is_relative = VG_TRUE;;
            }

            if (x0 == ox && y0 == oy)
                break;

            switch (CMD_TO_SEGMENT(cmd))
            {
            case VG_SCCWARC_TO:
                is_large = VG_FALSE;
                is_clockwise = VG_FALSE;
                break;

            case VG_SCWARC_TO:
                is_large = VG_FALSE;
                is_clockwise = VG_TRUE;
                break;

            case VG_LCCWARC_TO:
                is_large = VG_TRUE;
                is_clockwise = VG_FALSE;
                break;

            case VG_LCWARC_TO:
                is_large = VG_TRUE;
                is_clockwise = VG_TRUE;
                break;
            }
            {
                VGfloat rh = data_ptr[0], rv = data_ptr[1], rot = data_ptr[2];
                /* Find the center of ellipse. */
                VGfloat cx, cy, theta0, theta_range;
                findEllipse(&cx, &cy, &rh, &rv, &theta0, &theta_range, rot, ox, oy, data_ptr[3], data_ptr[4], is_large, is_clockwise, is_relative);
                
                /* Update boundary. */
                if(rh != 0 && rv != 0)
                {
                    VGfloat ax = ox - (VGfloat)cos(theta0) * rh;
                    VGfloat ay = oy - (VGfloat)sin(theta0) * rv;
                    VGint segs = (VGint)((VGfloat)ceil(((VGfloat)fabs(theta_range)) / (45.0f / 180.0f * PI)));
                    VGfloat theta_step = theta_range / segs;
                    VGfloat COS = (VGfloat)cos(rot * PI / 180.f);
                    VGfloat SIN = (VGfloat)sin(rot * PI / 180.f);
                    while (segs-- > 0) {
                        theta0 += theta_step;
                        VGfloat controlX = ax + (VGfloat)cos(theta0 - (theta_step / 2.0f)) * rh / (VGfloat)cos(theta_step / 2.0f);
                        VGfloat controlY = ay + (VGfloat)sin(theta0 - (theta_step / 2.0f)) * rv / (VGfloat)cos(theta_step / 2.0f);
                        VGfloat anchorX = ax + (VGfloat)cos(theta0) * rh;
                        VGfloat anchorY = ay + (VGfloat)sin(theta0) * rv;
                        if (rot != 0) {
                            x = ox + COS * (controlX - ox) - SIN * (controlY - oy);
                            y = oy + SIN * (controlX - ox) + COS * (controlY - oy);
                            controlX = x;
                            controlY = y;
                            x = ox + COS * (anchorX - ox) - SIN * (anchorY - oy);
                            y = oy + SIN * (anchorX - ox) + COS * (anchorY - oy);
                            anchorX = x;
                            anchorY = y;
                        }
                        if (segs == 0)
                        {
                            /* Use end point directly to avoid accumulated errors. */
                            anchorX = x0;
                            anchorY = y0;
                        }
                        float bound[4];
                        float curve[6] = { ox, oy, controlX, controlY, anchorX, anchorY };
                        findQuadBound(bound, curve);
                        x_min = min(bound[0], x_min);
                        y_min = min(bound[1], y_min);
                        x_max = max(bound[2], x_max);
                        y_max = max(bound[3], y_max);
                        ox = anchorX;
                        oy = anchorY;
                    }
                }
                else {
                    x_min = min(x0, x_min);
                    y_min = min(y0, y_min);
                    x_max = max(x0, x_max);
                    y_max = max(y0, y_max);
                }
            }

            px = ox = x0;
            py = oy = y0;
            break;
        }
    }

    freePathImpl(p_fp32);

    *minx = x_min;
    *miny = y_min;
    *width = x_max - x_min;
    *height = y_max - y_min;

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgPathTransformedBounds(VGPath path, VGfloat* minx, VGfloat* miny, VGfloat* width, VGfloat* height)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidPath(context, path), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid path handle
    VG_IF_ERROR(!(((Path*)path)->m_capabilities & VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS), VG_PATH_CAPABILITY_ERROR, VG_NO_RETVAL);    //invalid caps
    VG_IF_ERROR(!minx || !miny || !width || !height, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!isAligned(minx, 4) || !isAligned(miny, 4) || !isAligned(width, 4) || !isAligned(height, 4), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    Path* p = (Path*)path;

    /* Check if path is empty. */
    if (p->m_numSegments == 0) {
        if (minx) *minx = 0.f;
        if (miny) *miny = 0.f;
        if (width) *width = -1.f;
        if (height) *height = -1.f;
        VG_RETURN(VG_NO_RETVAL);
    }

    /* Implement transform (Convert path to float format). */
    Path* p_fp32 = calloc(1, sizeof(Path));
    VG_IF_ERROR(p_fp32 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_NO_RETVAL);
    p_fp32->m_datatype = VG_PATH_DATATYPE_F;
    appendPathImpl(p_fp32, p);
    pathTransformImpl(p_fp32, p_fp32->m_numSegments, p->m_numSegments, context->m_pathUserToSurface);
    p_fp32->m_numSegments = p->m_numSegments;

    /* Set the basic point. */
    VGubyte* seg_ptr = p_fp32->m_segments;
    VGubyte** data_list = p_fp32->m_data;
    VGfloat x_min = 0.f, y_min = 0.f, x_max = 0.f, y_max = 0.f, * data_ptr;
    data_ptr = (float*)(*data_list);
    switch (*seg_ptr)
    {
    case VG_MOVE_TO_ABS:
    case VG_MOVE_TO_REL:
        x_min = x_max = data_ptr[0];
        y_min = y_max = data_ptr[1];
        break;
    }

    /* Calculate bounds. */
    float px = 0, py = 0, ox = 0, oy = 0, sx = 0, sy = 0, x0 = 0, y0 = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0,
        x, y;
    VGboolean is_large = VG_FALSE, is_clockwise = VG_FALSE, is_relative = VG_FALSE;
    VGPathCommand cmd;
    for (int i = 0; i < p_fp32->m_numSegments; ++i) {
        data_ptr = (float*)(*data_list++);
        cmd = *seg_ptr++;
        switch (CMD_TO_SEGMENT(cmd)) {
        case VG_CLOSE_PATH:
            px = ox = sx;
            py = oy = sy;
            break;

        case VG_MOVE_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
            }

            x_min = min(x0, x_min);
            y_min = min(y0, y_min);
            x_max = max(x0, x_max);
            y_max = max(y0, y_max);

            sx = px = ox = x0;
            sy = py = oy = y0;
            break;

        case VG_LINE_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
            }

            x_min = min(x0, x_min);
            y_min = min(y0, y_min);
            x_max = max(x0, x_max);
            y_max = max(y0, y_max);

            px = ox = x0;
            py = oy = y0;
            break;

        case VG_HLINE_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
            }
            else {
                x0 = data_ptr[0] + ox;
            }

            x_min = min(x0, x_min);
            x_max = max(x0, x_max);

            px = x0;
            py = oy;
            ox = x0;
            break;

        case VG_VLINE_TO:
            if (CMD_IS_ABS(cmd)) {
                y0 = data_ptr[0];
            }
            else {
                y0 = data_ptr[0] + oy;
            }

            y_min = min(y0, y_min);
            y_max = max(y0, y_max);

            px = ox;
            py = y0;
            oy = y0;
            break;

        case VG_QUAD_TO: {
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
                x1 = data_ptr[2];
                y1 = data_ptr[3];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
                x1 = data_ptr[2] + ox;
                y1 = data_ptr[3] + oy;
            }

            float bound[4];
            float curve[6] = { ox, oy, x0, y0, x1, y1 };
            findQuadBound(bound, curve);
            x_min = min(bound[0], x_min);
            y_min = min(bound[1], y_min);
            x_max = max(bound[2], x_max);
            y_max = max(bound[3], y_max);

            px = x0;
            py = y0;
            ox = x1;
            oy = y1;
            break;
        }

        case VG_CUBIC_TO: {
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[0];
                y0 = data_ptr[1];
                x1 = data_ptr[2];
                y1 = data_ptr[3];
                x2 = data_ptr[4];
                y2 = data_ptr[5];
            }
            else {
                x0 = data_ptr[0] + ox;
                y0 = data_ptr[1] + oy;
                x1 = data_ptr[2] + ox;
                y1 = data_ptr[3] + oy;
                x2 = data_ptr[4] + ox;
                y2 = data_ptr[5] + oy;
            }

            float bound[4];
            float curve[8] = { ox, oy, x0, y0, x1, y1, x2, y2 };
            findCubicBound(bound, curve);
            x_min = min(bound[0], x_min);
            y_min = min(bound[1], y_min);
            x_max = max(bound[2], x_max);
            y_max = max(bound[3], y_max);

            px = x1;
            py = y1;
            ox = x2;
            oy = y2;
            break;
        }

        case VG_SQUAD_TO: {
            if (CMD_IS_ABS(cmd)) {
                x0 = 2.f * ox - px;
                y0 = 2.f * oy - py;
                x1 = data_ptr[0];
                y1 = data_ptr[1];
            }
            else {
                x0 = 2.f * ox - px + ox;
                y0 = 2.f * oy - py + oy;
                x1 = data_ptr[0] + ox;
                y1 = data_ptr[1] + oy;
            }

            float bound[4];
            float curve[6] = { ox, oy, x0, y0, x1, y1 };
            findQuadBound(bound, curve);
            x_min = min(bound[0], x_min);
            y_min = min(bound[1], y_min);
            x_max = max(bound[2], x_max);
            y_max = max(bound[3], y_max);

            px = x0;
            py = y0;
            ox = x1;
            oy = y1;
            break;
        }

        case VG_SCUBIC_TO: {
            if (CMD_IS_ABS(cmd)) {
                x0 = 2.f * ox - px;
                y0 = 2.f * oy - py;
                x1 = data_ptr[0];
                y1 = data_ptr[1];
                x2 = data_ptr[2];
                y2 = data_ptr[3];
            }
            else {
                x0 = 2.f * ox - px + ox;
                y0 = 2.f * oy - py + oy;
                x1 = data_ptr[0] + ox;
                y1 = data_ptr[1] + oy;
                x2 = data_ptr[2] + ox;
                y2 = data_ptr[3] + oy;
            }

            float bound[4];
            float curve[8] = { ox, oy, x0, y0, x1, y1, x2, y2 };
            findCubicBound(bound, curve);
            x_min = min(bound[0], x_min);
            y_min = min(bound[1], y_min);
            x_max = max(bound[2], x_max);
            y_max = max(bound[3], y_max);

            px = x1;
            py = y1;
            ox = x2;
            oy = y2;
            break;
        }

        case VG_SCCWARC_TO:
        case VG_SCWARC_TO:
        case VG_LCCWARC_TO:
        case VG_LCWARC_TO:
            if (CMD_IS_ABS(cmd)) {
                x0 = data_ptr[3];
                y0 = data_ptr[4];
                is_relative = VG_FALSE;
            }
            else {
                x0 = data_ptr[3] + ox;
                y0 = data_ptr[4] + oy;
                is_relative = VG_TRUE;;
            }

            if (x0 == ox && y0 == oy)
                break;

            switch (CMD_TO_SEGMENT(cmd))
            {
            case VG_SCCWARC_TO:
                is_large = VG_FALSE;
                is_clockwise = VG_FALSE;
                break;

            case VG_SCWARC_TO:
                is_large = VG_FALSE;
                is_clockwise = VG_TRUE;
                break;

            case VG_LCCWARC_TO:
                is_large = VG_TRUE;
                is_clockwise = VG_FALSE;
                break;

            case VG_LCWARC_TO:
                is_large = VG_TRUE;
                is_clockwise = VG_TRUE;
                break;
            }
            {
                VGfloat rh = data_ptr[0], rv = data_ptr[1], rot = data_ptr[2];
                /* Find the center of ellipse. */
                VGfloat cx, cy, theta0, theta_range;
                findEllipse(&cx, &cy, &rh, &rv, &theta0, &theta_range, rot, ox, oy, data_ptr[3], data_ptr[4], is_large, is_clockwise, is_relative);

                /* Update boundary. */
                if (rh != 0 && rv != 0)
                {
                    VGfloat ax = ox - (VGfloat)cos(theta0) * rh;
                    VGfloat ay = oy - (VGfloat)sin(theta0) * rv;
                    VGint segs = (VGint)((VGfloat)ceil(((VGfloat)fabs(theta_range)) / (45.0f / 180.0f * PI)));
                    VGfloat theta_step = theta_range / segs;
                    VGfloat COS = (VGfloat)cos(rot * PI / 180.f);
                    VGfloat SIN = (VGfloat)sin(rot * PI / 180.f);
                    while (segs-- > 0) {
                        theta0 += theta_step;
                        VGfloat controlX = ax + (VGfloat)cos(theta0 - (theta_step / 2.0f)) * rh / (VGfloat)cos(theta_step / 2.0f);
                        VGfloat controlY = ay + (VGfloat)sin(theta0 - (theta_step / 2.0f)) * rv / (VGfloat)cos(theta_step / 2.0f);
                        VGfloat anchorX = ax + (VGfloat)cos(theta0) * rh;
                        VGfloat anchorY = ay + (VGfloat)sin(theta0) * rv;
                        if (rot != 0) {
                            x = ox + COS * (controlX - ox) - SIN * (controlY - oy);
                            y = oy + SIN * (controlX - ox) + COS * (controlY - oy);
                            controlX = x;
                            controlY = y;
                            x = ox + COS * (anchorX - ox) - SIN * (anchorY - oy);
                            y = oy + SIN * (anchorX - ox) + COS * (anchorY - oy);
                            anchorX = x;
                            anchorY = y;
                        }
                        if (segs == 0)
                        {
                            /* Use end point directly to avoid accumulated errors. */
                            anchorX = x0;
                            anchorY = y0;
                        }
                        float bound[4];
                        float curve[6] = { ox, oy, controlX, controlY, anchorX, anchorY };
                        findQuadBound(bound, curve);
                        x_min = min(bound[0], x_min);
                        y_min = min(bound[1], y_min);
                        x_max = max(bound[2], x_max);
                        y_max = max(bound[3], y_max);
                        ox = anchorX;
                        oy = anchorY;
                    }
                }
                else {
                    x_min = min(x0, x_min);
                    y_min = min(y0, y_min);
                    x_max = max(x0, x_max);
                    y_max = max(y0, y_max);
                }
            }


            px = ox = x0;
            py = oy = y0;
            break;
}
    }

    freePathImpl(p_fp32);

    *minx = x_min;
    *miny = y_min;
    *width = x_max - x_min;
    *height = y_max - y_min;

    VG_RETURN(VG_NO_RETVAL);
}

VGboolean VG_APIENTRY vgInterpolatePath(VGPath dstPath, VGPath startPath, VGPath endPath, VGfloat amount)
{
    VG_GET_CONTEXT(VG_FALSE);
    VG_IF_ERROR(!isValidPath(context, dstPath) || !isValidPath(context, startPath) || !isValidPath(context, endPath), VG_BAD_HANDLE_ERROR, VG_FALSE);    //invalid path handle
    VG_IF_ERROR(!(((Path*)dstPath)->m_capabilities & VG_PATH_CAPABILITY_INTERPOLATE_TO) ||
        !(((Path*)startPath)->m_capabilities & VG_PATH_CAPABILITY_INTERPOLATE_FROM) ||
        !(((Path*)endPath)->m_capabilities & VG_PATH_CAPABILITY_INTERPOLATE_FROM), VG_PATH_CAPABILITY_ERROR, VG_FALSE);    //invalid caps

    Path* d = (Path*)dstPath;
    Path* s = (Path*)startPath;
    Path* e = (Path*)endPath;

    if (s->m_numSegments != e->m_numSegments) {
        return VG_FALSE;
    }
    VGint num_segments = s->m_numSegments;

    /* Convert startPath and endPath to float format. */
    Path* s_fp32 = calloc(1, sizeof(Path));
    VG_IF_ERROR(s_fp32 == NULL, VG_OUT_OF_MEMORY_ERROR, VG_FALSE);
    Path* e_fp32 = calloc(1, sizeof(Path));
    if (e_fp32 == NULL) {
        free(s_fp32);
        setError(VG_OUT_OF_MEMORY_ERROR);
        OSReleaseMutex();
        return VG_FALSE;
    }
    s_fp32->m_datatype = VG_PATH_DATATYPE_F;
    e_fp32->m_datatype = VG_PATH_DATATYPE_F;
    appendPathImpl(s_fp32, s);
    appendPathImpl(e_fp32, e);
    s_fp32->m_numSegments = num_segments;
    e_fp32->m_numSegments = num_segments;

    /* Normalize: convert relative segments to absolute form, H(V)LINE_TO_* to LINE_TO_*, (S)QUARD_TO_* / SCUBIC_TO_* to CUBIC_TO_*. */
    normalizePath(s_fp32);
    normalizePath(e_fp32);

    /* Check if both paths have the same sequence of segments type. */
    VGubyte* s_seg_ptr = s_fp32->m_segments;
    VGubyte* e_seg_ptr = e_fp32->m_segments;
    VGPathCommand s_seg, e_seg;
    for (int i = 0; i < num_segments; ++i) {
        s_seg = *s_seg_ptr++;
        e_seg = *e_seg_ptr++;
        if ((s_seg < VG_SCCWARC_TO_ABS) ? (s_seg != e_seg) : (e_seg < VG_SCCWARC_TO_ABS)) {
            freePathImpl(s_fp32);
            freePathImpl(e_fp32);
            return VG_FALSE;
        }
    }

    /* Linear lnterpolate. */
    VGubyte* seg_ptr = (VGubyte*)((amount < 0.5) ? s_fp32->m_segments : e_fp32->m_segments);
    VGbyte** data_list = (VGbyte**)((amount < 0.5) ? s_fp32->m_data : e_fp32->m_data);
    VGubyte** s_data_list = s_fp32->m_data;
    VGubyte** e_data_list = e_fp32->m_data;
    float* data_ptr, * s_data_ptr, * e_data_ptr;
    for (int i = 0; i < num_segments; ++i) {
        data_ptr = (float*)(*data_list++);
        s_data_ptr = (float*)(*s_data_list++);
        e_data_ptr = (float*)(*e_data_list++);
        switch (*seg_ptr++)
        {
        case VG_CUBIC_TO_ABS:
            data_ptr[5] = s_data_ptr[5] * amount + e_data_ptr[5] * (1 - amount);
            data_ptr[4] = s_data_ptr[4] * amount + e_data_ptr[4] * (1 - amount);
            data_ptr[3] = s_data_ptr[3] * amount + e_data_ptr[3] * (1 - amount);
            data_ptr[2] = s_data_ptr[2] * amount + e_data_ptr[2] * (1 - amount);
        case VG_MOVE_TO_ABS:
        case VG_LINE_TO_ABS:
            data_ptr[1] = s_data_ptr[1] * amount + e_data_ptr[1] * (1 - amount);
            data_ptr[0] = s_data_ptr[0] * amount + e_data_ptr[0] * (1 - amount);
            break;

        case VG_SCCWARC_TO_ABS:
        case VG_SCWARC_TO_ABS:
        case VG_LCCWARC_TO_ABS:
        case VG_LCWARC_TO_ABS:
            data_ptr[4] = s_data_ptr[4] * amount + e_data_ptr[4] * (1 - amount);
            data_ptr[3] = s_data_ptr[3] * amount + e_data_ptr[3] * (1 - amount);
            break;

        default:
            break;
        }
    }

    /* Append to dst */
    appendPathImpl(d, (amount < 0.5) ? s_fp32 : e_fp32);
    d->m_numSegments += num_segments;

    freePathImpl(s_fp32);
    freePathImpl(e_fp32);

    d->m_pathChanged = VG_TRUE;

    VG_RETURN(VG_TRUE);
}

VGPaint VG_APIENTRY vgCreatePaint(void)
{
    VG_GET_CONTEXT(VG_INVALID_HANDLE);
    Paint* paint = createdefaultPaint();
    addPaintResource(context, paint);
    VG_RETURN((VGPaint)paint);
}

void VG_APIENTRY vgDestroyPaint(VGPaint paint)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidPaint(context, paint), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid paint handle
    vg_lite_finish();
    removePaintResource(context, (Paint*)paint);
    ((Paint*)paint)->m_referenceCount--;

    if (paint != VG_INVALID_HANDLE && !((Paint*)paint)->m_referenceCount)
    {
        freePaintImpl((Paint*)paint);
        if (paint == context->m_fillPaint)
        {
            context->m_fillPaint = VG_INVALID_HANDLE;
        }
        else if (paint == context->m_strokePaint)
        {
            context->m_strokePaint = VG_INVALID_HANDLE;
        }
    }
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgSetPaint(VGPaint paint, VGbitfield paintModes)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(paint && !isValidPaint(context, paint), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid paint handle
    VG_IF_ERROR(!paintModes || paintModes & ~(VG_FILL_PATH | VG_STROKE_PATH), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);    //invalid paint mode
    
    if ((context->m_fillPaint != paint || !paint) && (context->m_strokePaint != paint || !paint)) {
        releasePaint(context, paintModes);

        if (paintModes & VG_FILL_PATH)
        {
            if (paint)
                ((Paint*)paint)->m_referenceCount++;
            context->m_fillPaint = paint;
        }
        if (paintModes & VG_STROKE_PATH)
        {
            if (paint)
                ((Paint*)paint)->m_referenceCount++;
            context->m_strokePaint = paint;
        }
    }
    VG_RETURN(VG_NO_RETVAL);
}

static VG_INLINE VGuint colorToInt(VGfloat c, VGint maxc)
{
    return VG_INT_MIN(VG_INT_MAX((VGint)floor(c * (VGfloat)maxc + 0.5f), 0), maxc);
}

static VG_INLINE VGfloat intToColor(VGuint i, VGuint maxi)
{
    return (VGfloat)(i & maxi) / (VGfloat)maxi;
}

static void unpackColor(Color *c, VGuint inputData, const ColorDescriptor inputDesc)
{
    VGint rb = inputDesc.redBits;
    VGint gb = inputDesc.greenBits;
    VGint bb = inputDesc.blueBits;
    VGint ab = inputDesc.alphaBits;
    VGint lb = inputDesc.luminanceBits;
    VGint rs = inputDesc.redShift;
    VGint gs = inputDesc.greenShift;
    VGint bs = inputDesc.blueShift;
    VGint as = inputDesc.alphaShift;
    VGint ls = inputDesc.luminanceShift;

    c->m_format = inputDesc.internalFormat;

    if (lb)
    {    //luminance
        c->r = c->g = c->b = intToColor(inputData >> ls, (1 << lb) - 1);
        c->a = 1.0f;
    }
    else
    {    //rgba
        c->r = rb ? intToColor(inputData >> rs, (1 << rb) - 1) : (VGfloat)1.0f;
        c->g = gb ? intToColor(inputData >> gs, (1 << gb) - 1) : (VGfloat)1.0f;
        c->b = bb ? intToColor(inputData >> bs, (1 << bb) - 1) : (VGfloat)1.0f;
        c->a = ab ? intToColor(inputData >> as, (1 << ab) - 1) : (VGfloat)1.0f;

        if (c->m_format & PREMULTIPLIED)
        {    //clamp premultiplied color to alpha to enforce consistency
            c->r = VG_MIN(c->r, c->a);
            c->g = VG_MIN(c->g, c->a);
            c->b = VG_MIN(c->b, c->a);
        }
    }
}

VGuint packColor(Color* c, const ColorDescriptor outputDesc)
{
    VGint rb = outputDesc.redBits;
    VGint gb = outputDesc.greenBits;
    VGint bb = outputDesc.blueBits;
    VGint ab = outputDesc.alphaBits;
    VGint lb = outputDesc.luminanceBits;
    VGint rs = outputDesc.redShift;
    VGint gs = outputDesc.greenShift;
    VGint bs = outputDesc.blueShift;
    VGint as = outputDesc.alphaShift;
    VGint ls = outputDesc.luminanceShift;

    if (lb)
    {    //luminance
        VG_ASSERT((c->m_format & LUMINANCE));
        return colorToInt(c->r, (1 << lb) - 1) << ls;
    }
    else
    {    //rgb
        VG_ASSERT(!(c->m_format & LUMINANCE));
        VGuint cr = rb ? colorToInt(c->r, (1 << rb) - 1) : 0;
        VGuint cg = gb ? colorToInt(c->g, (1 << gb) - 1) : 0;
        VGuint cb = bb ? colorToInt(c->b, (1 << bb) - 1) : 0;
        VGuint ca = ab ? colorToInt(c->a, (1 << ab) - 1) : 0;
        return (cr << rs) | (cg << gs) | (cb << bs) | (ca << as);
    }
}

void VG_APIENTRY vgSetColor(VGPaint paint, VGuint rgba)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidPaint(context, paint), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid paint handle
    Paint* p = (Paint*)paint;
    unpackColor(&p->m_inputPaintColor, rgba, formatToDescriptor(VG_sRGBA_8888));
    p->m_paintColor = p->m_inputPaintColor;
    context->m_paintColor = p->m_inputPaintColor;
    clampColor(context, &p->m_paintColor);
    clampColor(context, &context->m_paintColor);

    VG_RETURN(VG_NO_RETVAL);
}

VGuint VG_APIENTRY vgGetColor(VGPaint paint)
{
    VG_GET_CONTEXT(0);
    VG_IF_ERROR(!isValidPaint(context, paint), VG_BAD_HANDLE_ERROR, 0);    //invalid paint handle
    VGuint ret = packColor(&((Paint*)paint)->m_inputPaintColor, formatToDescriptor(VG_sRGBA_8888));

    VG_RETURN(ret);
}

VGPaint VG_APIENTRY vgGetPaint(VGPaintMode paintMode)
{
    VG_GET_CONTEXT(VG_INVALID_HANDLE);
    VG_IF_ERROR(paintMode != VG_FILL_PATH && paintMode != VG_STROKE_PATH, VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);    //invalid paint mode

    if (paintMode == VG_FILL_PATH)
    {
        VG_RETURN(context->m_fillPaint);
    }
    VG_RETURN(context->m_strokePaint);
}

void VG_APIENTRY vgPaintPattern(VGPaint paint, VGImage image)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidPaint(context, paint) || (image != VG_INVALID_HANDLE && !isValidImage(context, image)), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid handle
    Image* img = (Image*)image;
    Paint* pnt = (Paint*)paint;
    VG_IF_ERROR(image != VG_INVALID_HANDLE && isEglVgInUse(img), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);

    Image* pattern = pnt->m_pattern;
    if (pattern)
    {
        VG_ASSERT(pattern->m_inUse > 0);
        pattern->m_inUse--;
        if (!pattern->m_referenceCount--)
            free(pattern);
    }
    context->m_pattern = img;
    pnt->m_pattern = img;
    if (img)
    {
        img->m_referenceCount++;
        img->m_inUse++;
    }

    VG_RETURN(VG_NO_RETVAL);
}

VGImage VG_APIENTRY vgCreateImage(VGImageFormat format, VGint width, VGint height, VGbitfield allowedQuality)
{
    VG_GET_CONTEXT(VG_INVALID_HANDLE);
    VG_IF_ERROR(!isValidImageFormat(format), VG_UNSUPPORTED_IMAGE_FORMAT_ERROR, VG_INVALID_HANDLE);
    VG_IF_ERROR(width <= 0 || height <= 0 || !allowedQuality ||
                (allowedQuality & ~(VG_IMAGE_QUALITY_NONANTIALIASED | VG_IMAGE_QUALITY_FASTER | VG_IMAGE_QUALITY_BETTER)), VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);
    VG_IF_ERROR(width > vgGeti(VG_MAX_IMAGE_WIDTH) || height > vgGeti(VG_MAX_IMAGE_HEIGHT) || width*height > VIV_MAX_IMAGE_PIXELS ||
                ((width*formatToDescriptor(format).bitsPerPixel+7)/8)*height > VIV_MAX_IMAGE_BYTES, VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);

    ColorDescriptor desc = formatToDescriptor(format);
    Image* image = createBlankImage(&desc, width, height, allowedQuality);
    if (!image)
    {
        setError(VG_OUT_OF_MEMORY_ERROR);
        VG_RETURN(VG_INVALID_HANDLE);
    }
    addImageResource(context, image);
    VG_RETURN((VGImage)image);
}

void VG_APIENTRY vgDestroyImage(VGImage image)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidImage(context, image), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    vg_lite_finish();
    Image* img = (Image*)image;

    /***
    Skip Image destroy if the image is currently in use as a rendering target, is the
    ancestor of another image (see vgChildImage), is set as a paint pattern image on
    a VGPaint object, or is set as a glyph an a VGFont object, its definition remains
    available to those consumers as long as they remain valid, but the handle may no
    longer be used. When those uses cease, the images resources will automatically
    be deallocated.
    */
    removeImageResource(context, (Image*)image);
    img->m_referenceCount--;
    VG_ASSERT(img->m_referenceCount >= 0);

    if (!img->m_referenceCount)
    {
        destroyImage(&img, context);
    }

    VG_RETURN(VG_NO_RETVAL);
}

static void writePixel(Image* dst, int x, int y, Color c, VGboolean display)
{
    int rb = dst->m_desc.redBits;
    int gb = dst->m_desc.greenBits;
    int bb = dst->m_desc.blueBits;
    int ab = dst->m_desc.alphaBits;
    int lb = dst->m_desc.luminanceBits;
    int rs = dst->m_desc.redShift;
    int gs = dst->m_desc.greenShift;
    int bs = dst->m_desc.blueShift;
    int as = dst->m_desc.alphaShift;
    int ls = dst->m_desc.luminanceShift;
    unsigned int p;
    if (dst->m_desc.format == OPENVG_lRGBX_8888 || dst->m_desc.format == OPENVG_sRGBX_8888 ||
        dst->m_desc.format == OPENVG_lRGBX_8888_PRE || dst->m_desc.format == OPENVG_sRGBX_8888_PRE)
    {
        ab = 8;
        lb = dst->m_desc.luminanceBits;
        rs = 24;
        gs = 16;
        bs = 8;
    }
    if (lb)
    {    //luminance
        p = colorToInt(c.r, (1 << lb) - 1) << ls;
    }
    else
    {    //rgb
        unsigned int cr = rb ? colorToInt(c.r, (1 << rb) - 1) : 0;
        unsigned int cg = gb ? colorToInt(c.g, (1 << gb) - 1) : 0;
        unsigned int cb = bb ? colorToInt(c.b, (1 << bb) - 1) : 0;
        unsigned int ca = ab ? colorToInt(c.a, (1 << ab) - 1) : 0;
        p = (cr << rs) | (cg << gs) | (cb << bs) | (ca << as);
    }
    uint8_t* scanline;
    x += dst->m_storageOffsetX;
    y += dst->m_storageOffsetY;
    if (!display) {
        scanline = dst->m_data + y * dst->m_stride;
    }
    else {
        scanline = (uint8_t*)((vg_lite_buffer_t*)(dst->m_vglbuf))->memory + y * dst->m_stride;
    }
    switch (dst->m_desc.bitsPerPixel)
    {
    case 32:
    {
        uint32_t* s = ((uint32_t*)scanline) + x;
        *s = (uint32_t)p;
        break;
    }
    case 16:
    {
        uint16_t* s = ((uint16_t*)scanline) + x;
        *s = (uint16_t)p;
        break;
    }
    case 8:
    {
        uint8_t* s = ((uint8_t*)scanline) + x;
        *s = (uint8_t)p;
        break;
    }
    case 4:
    {
        uint8_t* s = ((uint8_t*)scanline) + (x >> 1);
        *s = (uint8_t)((p << ((x & 1) << 2)) | ((unsigned int)*s & ~(0xf << ((x & 1) << 2))));
        break;
    }
    case 2:
    {
        uint8_t* s = ((uint8_t*)scanline) + (x >> 2);
        *s = (uint8_t)((p << ((x & 3) << 1)) | ((unsigned int)*s & ~(0x3 << ((x & 3) << 1))));
        break;
    }
    default:
    {
        uint8_t* s = ((uint8_t*)scanline) + (x >> 3);
        *s = (uint8_t)((p << (x & 7)) | ((unsigned int)*s & ~(0x1 << (x & 7))));
        break;
    }
    }
}

static void imgClear(Image* dst, Color c, int x, int y, int w, int h, VGboolean display)
{
    if (x < 0)
        w += x;
    if (y < 0)
        h += y;
    x = max(0, x);
    y = max(0, y);
    w = min(w, dst->m_width);
    h = min(h, dst->m_height);

    convert(c.m_format, dst->m_desc.internalFormat, &c);

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            writePixel(dst, x + i, y + j, c, display);
        }
    }
}

void VG_APIENTRY vgClearImage(VGImage image, VGint x, VGint y, VGint width, VGint height)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidImage(context, image), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    Image* img = (Image*)image;
    VG_IF_ERROR(isEglVgInUse(img), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(width <= 0 || height <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

    vg_lite_buffer_t* dstbuf = (vg_lite_buffer_t*)img->m_vglbuf;
  
    if (!isSupportFormat(context, dstbuf->format)) {
        imgClear(img, context->m_clearColor, x, y, width, height, VG_TRUE);
    }
    else {

        /* Map clear region into parent image. */
        width = min(width + x, img->m_width) - x;
        height = min(height + y, img->m_height) - y;
        x += img->m_storageOffsetX;
        y += img->m_storageOffsetY;
        /* Use vg_lite_clear to implement vgClearImage */
        vg_lite_rectangle_t rect = { x, y, width, height };
        Color c;

        c.r = context->m_clearColor.r;
        if (c.r > 1.0f) {
            c.r = 1.0f;
        }
        c.g = context->m_clearColor.g;
        if (c.g > 1.0f) {
            c.g = 1.0f;
        }
        c.b = context->m_clearColor.b;
        if (c.b > 1.0f) {
            c.b = 1.0f;
        }
        c.a = context->m_clearColor.a;
        if (c.a > 1.0f) {
            c.a = 1.0f;
        }
        vg_lite_uint8_t a = VG_FLOAT_TO_UB(c.a);
        vg_lite_uint8_t b = VG_FLOAT_TO_UB(c.b);
        vg_lite_uint8_t g = VG_FLOAT_TO_UB(c.g);
        vg_lite_uint8_t r = VG_FLOAT_TO_UB(c.r);
        VGuint rgba = (a << 24) | (b << 16) | (g << 8) | (r << 0);

        vg_lite_clear(dstbuf, &rect, rgba);
        vg_lite_flush();
    }
    VG_RETURN(VG_NO_RETVAL);
}

void computeBlitRegion(int* sx, int* sy, int* dx, int* dy, int* w, int* h, int srcWidth, int srcHeight, int dstWidth, int dstHeight)
{
    *sx = min(max(*sx, (int)(MIN_INT32 >> 2)), (int)(MAX_INT32 >> 2));
    *sy = min(max(*sy, (int)(MIN_INT32 >> 2)), (int)(MAX_INT32 >> 2));
    *dx = min(max(*dx, (int)(MIN_INT32 >> 2)), (int)(MAX_INT32 >> 2));
    *dy = min(max(*dy, (int)(MIN_INT32 >> 2)), (int)(MAX_INT32 >> 2));
    *w = min(*w, (int)(MAX_INT32 >> 2));
    *h = min(*h, (int)(MAX_INT32 >> 2));
    int srcsx = *sx, srcex = *sx + *w, dstsx = *dx, dstex = *dx + *w;
    if (srcsx < 0)
    {
        dstsx -= srcsx;
        srcsx = 0;
    }
    if (srcex > srcWidth)
    {
        dstex -= srcex - srcWidth;
        srcex = srcWidth;
    }
    if (dstsx < 0)
    {
        srcsx -= dstsx;
        dstsx = 0;
    }
    if (dstex > dstWidth)
    {
        srcex -= dstex - dstWidth;
        dstex = dstWidth;
    }

    *w = srcex - srcsx;

    int srcsy = *sy, srcey = *sy + *h, dstsy = *dy, dstey = *dy + *h;
    if (srcsy < 0)
    {
        dstsy -= srcsy;
        srcsy = 0;
    }
    if (srcey > srcHeight)
    {
        dstey -= srcey - srcHeight;
        srcey = srcHeight;
    }
    if (dstsy < 0)
    {
        srcsy -= dstsy;
        dstsy = 0;
    }
    if (dstey > dstHeight)
    {
        srcey -= dstey - dstHeight;
        dstey = dstHeight;
    }

    *h = srcey - srcsy;
    *sx = srcsx;
    *sy = srcsy;
    *dx = dstsx;
    *dy = dstsy;
}
VGfloat vg_gamma(VGfloat c)
{
    if( c <= 0.00304f )
        c *= 12.92f;
    else
        c = 1.0556f * (VGfloat)pow(c, 1.0f/2.4f) - 0.0556f;
    return c;
}

VGfloat inv_vg_gamma(VGfloat c)
{
    if (c <= 0.03928f)
        c /= 12.92f;
    else
        c = (VGfloat)pow((c + 0.0556f) / 1.0556f, 2.4f);
    return c;
}

VGfloat lRGBtoL(VGfloat r, VGfloat g, VGfloat b)
{
    return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

void unpremultiply(Color* c)
{
    if ((c->m_format & PREMULTIPLIED))
    {   //unpremultiply
        VGfloat ooa = (c->a != 0.0f) ? 1.0f / c->a : (VGfloat)0.0f;
        c->r *= ooa;
        c->g *= ooa;
        c->b *= ooa;
        c->m_format = (InternalFormat)(c->m_format & ~PREMULTIPLIED);
    }
}

void luminanceToRGB(Color* c)
{
    if ((c->m_format & LUMINANCE))
    {
        VG_ASSERT(c->r == c->g && c->g == c->b);
        c->m_format = (InternalFormat)(c->m_format & ~LUMINANCE);
    }
}

void convert(InternalFormat srcFormat, InternalFormat dstFormat, Color* c)
{

    if (srcFormat == dstFormat)
        return;

    if ((srcFormat & PREMULTIPLIED))
    {   //unpremultiply
        VGfloat ooa = (c->a != 0.0f) ? 1.0f / c->a : (VGfloat)0.0f;
        c->r *= ooa;
        c->g *= ooa;
        c->b *= ooa;
    }

    //From Section 3.4.2 of OpenVG spec
    //1: sRGB = vg_gamma(lRGB)
    //2: lRGB = inv_vg_gamma(sRGB)
    //3: lL = 0.2126 lR + 0.7152 lG + 0.0722 lB
    //4: lRGB = lL
    //5: sL = vg_gamma(lL)
    //6: lL = inv_vg_gamma(sL)
    //7: sRGB = sL

    unsigned int conversion = (srcFormat & (NONLINEAR | LUMINANCE)) | ((dstFormat & (NONLINEAR | LUMINANCE)) << 3);

    switch (conversion)
    {
        case lRGBA | (sRGBA << 3) : c->r = vg_gamma(c->r); c->g = vg_gamma(c->g); c->b = vg_gamma(c->b); break;
        case lRGBA | (lLA << 3) : c->r = c->g = c->b = lRGBtoL(c->r, c->g, c->b); break;
        case lRGBA | (sLA << 3) : c->r = c->g = c->b = vg_gamma(lRGBtoL(c->r, c->g, c->b)); break;
        case sRGBA | (lRGBA << 3) : c->r = inv_vg_gamma(c->r); c->g = inv_vg_gamma(c->g); c->b = inv_vg_gamma(c->b); break;
        case sRGBA | (lLA << 3) : c->r = c->g = c->b = lRGBtoL(inv_vg_gamma(c->r), inv_vg_gamma(c->g), inv_vg_gamma(c->b)); break;
        case sRGBA | (sLA << 3) : c->r = c->g = c->b = vg_gamma(lRGBtoL(inv_vg_gamma(c->r), inv_vg_gamma(c->g), inv_vg_gamma(c->b))); break;
        case lLA | (lRGBA << 3) : break;
        case lLA | (sRGBA << 3) : c->r = vg_gamma(c->r); c->g = vg_gamma(c->g); c->b = vg_gamma(c->b); break;
        case lLA | (sLA << 3) : c->r = c->g = c->b = vg_gamma(c->r); break;
        case sLA | (lRGBA << 3) : c->r = c->g = c->b = inv_vg_gamma(c->r); break;
        case sLA | (sRGBA << 3) : break;
        case sLA | (lLA << 3) : c->r = c->g = c->b = inv_vg_gamma(c->r); break;
        default:  break;
    }

    if (dstFormat & PREMULTIPLIED)
    {   //premultiply
        c->r *= c->a;
        c->g *= c->a;
        c->b *= c->a;
    }

}

static Color readPixel(Image* src, int x, int y)
{

    unsigned int p = 0;
    x += src->m_storageOffsetX;
    y += src->m_storageOffsetY;
    uint8_t* scanline = src->m_data + y * src->m_stride;
    src->m_desc.bitsPerPixel = formatToDescriptor(src->m_desc.format).bitsPerPixel;
    switch (src->m_desc.bitsPerPixel)
    {
    case 32:
    {
        uint32_t* s = (((uint32_t*)scanline) + x);
        p = (unsigned int)*s;
        break;
    }

    case 16:
    {
        uint16_t* s = ((uint16_t*)scanline) + x;
        p = (unsigned int)*s;
        break;
    }

    case 8:
    {
        uint8_t* s = ((uint8_t*)scanline) + x;
        p = (unsigned int)*s;
        break;
    }
    case 4:
    {
        uint8_t* s = ((uint8_t*)scanline) + (x >> 1);
        p = (unsigned int)(*s >> ((x & 1) << 2)) & 0xf;
        break;
    }
    case 2:
    {
        uint8_t* s = ((uint8_t*)scanline) + (x >> 2);
        p = (unsigned int)(*s >> ((x & 3) << 1)) & 0x3;
        break;
    }
    default:
    {
        uint8_t* s = ((uint8_t*)scanline) + (x >> 3);
        p = (unsigned int)(*s >> (x & 7)) & 0x1;
        break;
    }
    }

    int rb = src->m_desc.redBits;
    int gb = src->m_desc.greenBits;
    int bb = src->m_desc.blueBits;
    int ab = src->m_desc.alphaBits;
    int lb = src->m_desc.luminanceBits;
    int rs = src->m_desc.redShift;
    int gs = src->m_desc.greenShift;
    int bs = src->m_desc.blueShift;
    int as = src->m_desc.alphaShift;
    int ls = src->m_desc.luminanceShift;
    if (src->m_desc.format == OPENVG_lRGBX_8888 || src->m_desc.format == OPENVG_sRGBX_8888 ||
        src->m_desc.format == OPENVG_lRGBX_8888_PRE || src->m_desc.format == OPENVG_sRGBX_8888_PRE)
    {
        ab = 0;
        lb = src->m_desc.luminanceBits;
        rs = 24;
        gs = 16;
        bs = 8;
    }
    Color c = {0};
    if (lb)
    {   //luminance
        c.r = c.g = c.b = intToColor(p >> ls, (1 << lb) - 1);
        c.a = 1.0f;
    }
    else
    {   //rgba
        c.r = rb ? intToColor(p >> rs, (1 << rb) - 1) : (VGfloat)1.0f;
        c.g = gb ? intToColor(p >> gs, (1 << gb) - 1) : (VGfloat)1.0f;
        c.b = bb ? intToColor(p >> bs, (1 << bb) - 1) : (VGfloat)1.0f;
        c.a = ab ? intToColor(p >> as, (1 << ab) - 1) : (VGfloat)1.0f;

        if (src->m_desc.internalFormat & PREMULTIPLIED)
        {   //clamp premultiplied color to alpha to enforce consistency
            c.r = min(c.r, c.a);
            c.g = min(c.g, c.a);
            c.b = min(c.b, c.a);
        }
    }
    return c;

}

 static void imgSetPixel(Image* dst, Image* src, int sx, int sy, int dx, int dy, int w, int h, VGboolean display)
 {
     int tmp_sx = sx;
     int tmp_sy = sy;
     int tmp_dx = dx;
     int tmp_dy = dy;
     int tmp_w = w;
     int tmp_h = h;
     Color tmp;

     computeBlitRegion(&tmp_sx, &tmp_sy, &tmp_dx, &tmp_dy, &tmp_w, &tmp_h, src->m_width, src->m_height, dst->m_width, dst->m_height);

     if (w <= 0 || h <= 0) {
         return;
     }
     //copy source region to tmp
     for (int j = 0; j < tmp_h; j++)
     {
         for (int i = 0; i < tmp_w; i++)
         {
             Color c = readPixel(src, tmp_sx + i, tmp_sy + j);
             convert(src->m_desc.internalFormat, dst->m_desc.internalFormat, &c);
             tmp = c;
             writePixel(dst, tmp_dx + i, tmp_dy + j, tmp, display);
         }
     }
     return;
 }

 VGboolean inverseMatrix3x3(Matrix3x3* result, const Matrix3x3 matrix, VGboolean isAffine)
 {
     vg_lite_float_t det00, det01, det02;
     vg_lite_float_t d;

     det00 = (matrix.matrix[1][1] * matrix.matrix[2][2]) - (matrix.matrix[2][1] * matrix.matrix[1][2]);
     det01 = (matrix.matrix[2][0] * matrix.matrix[1][2]) - (matrix.matrix[1][0] * matrix.matrix[2][2]);
     det02 = (matrix.matrix[1][0] * matrix.matrix[2][1]) - (matrix.matrix[2][0] * matrix.matrix[1][1]);

     /* Compute determinant. */
     d = (matrix.matrix[0][0] * det00) + (matrix.matrix[0][1] * det01) + (matrix.matrix[0][2] * det02);

     /* Return 0 if there is no inverse matrix. */
     if (d == 0.0f)
         return VG_FALSE;

     /* Compute reciprocal. */
     d = 1.0f / d;

     /* Determine if the matrix is affine. */
     isAffine = (matrix.matrix[2][0] == 0.0f) && (matrix.matrix[2][1] == 0.0f) && (matrix.matrix[2][2] == 1.0f);

     result->matrix[0][0] = d * det00;
     result->matrix[0][1] = d * ((matrix.matrix[2][1] * matrix.matrix[0][2]) - (matrix.matrix[0][1] * matrix.matrix[2][2]));
     result->matrix[0][2] = d * ((matrix.matrix[0][1] * matrix.matrix[1][2]) - (matrix.matrix[1][1] * matrix.matrix[0][2]));
     result->matrix[1][0] = d * det01;
     result->matrix[1][1] = d * ((matrix.matrix[0][0] * matrix.matrix[2][2]) - (matrix.matrix[2][0] * matrix.matrix[0][2]));
     result->matrix[1][2] = d * ((matrix.matrix[1][0] * matrix.matrix[0][2]) - (matrix.matrix[0][0] * matrix.matrix[1][2]));
     result->matrix[2][0] = isAffine ? 0.0f : d * det02;
     result->matrix[2][1] = isAffine ? 0.0f : d * ((matrix.matrix[2][0] * matrix.matrix[0][1]) - (matrix.matrix[0][0] * matrix.matrix[2][1]));
     result->matrix[2][2] = isAffine ? 1.0f : d * ((matrix.matrix[0][0] * matrix.matrix[1][1]) - (matrix.matrix[1][0] * matrix.matrix[0][1]));

     /* Success. */
     return VG_TRUE;
 }

 static void convertFormat(VGContext* context, Image* dst, Image* src) {
     for (int dy = 0; dy < dst->m_height; ++dy) {
         for (int dx = 0; dx < dst->m_width; ++dx) {
             if (dx >= src->m_width || dy >= src->m_height || dx < 0.f || dy < 0.f)
                 continue;
             Color c;
             c = readPixel(src, dx, dy);
             c.m_format = src->m_desc.internalFormat;

             writePixel(dst, dx, dy, c, 1);
         }
     }
 }

 static void imgBlit(VGContext* context, Image* dst, Image* src, const Matrix3x3 matrix, VGboolean display, VGImageQuality quality) {
     int sxmax, symax;
     Matrix3x3 inverseMatrix;
     VGboolean premultiplied;
     if (!inverseMatrix3x3(&inverseMatrix, matrix, 1))
         return; // No solution.
     VGbitfield aq = src->m_allowedQuality;
     aq &= (VGbitfield)quality;

     sxmax = src->m_width - 1; symax = src->m_height - 1;
     premultiplied = src->m_desc.internalFormat & PREMULTIPLIED;

     for (int i = 0; i < 8; i++) {
         if (i < 4) {
             context->m_colorTransformValues[i] = VG_CLAMP(context->m_colorTransformValues[i], -127.0f, 127.0f);
         }
         else {
             context->m_colorTransformValues[i] = VG_CLAMP(context->m_colorTransformValues[i], -1.0f, 1.0f);
         }
     }

#define readOnePixel(context, _c, _x, _y) { \
    if ((_x >= 0) && (_y >= 0) && (_x <= sxmax) && (_y <= symax)) { \
        _c = readPixel(src, _x, _y); \
        if (context->m_paintType == VG_PAINT_TYPE_COLOR && ((vg_lite_buffer_t*)src->m_vglbuf)->image_mode == VG_DRAW_IMAGE_MULTIPLY) {\
                 _c.r = _c.r * context->m_paintColor.r;\
                 _c.g = _c.g * context->m_paintColor.g;\
                 _c.b = _c.b * context->m_paintColor.b;\
                 _c.a = _c.a * context->m_paintColor.a;\
        }\
        if (context->m_colorTransform) {\
            _c.r = _c.r * context->m_colorTransformValues[0] + context->m_colorTransformValues[4];\
            _c.g = _c.g * context->m_colorTransformValues[1] + context->m_colorTransformValues[5];\
            _c.b = _c.b * context->m_colorTransformValues[2] + context->m_colorTransformValues[6];\
            _c.a = _c.a * context->m_colorTransformValues[3] + context->m_colorTransformValues[7];\
            clampColor(context, &_c);\
        }\
        if(!premultiplied){ _c.r *= _c.a; _c.g *= _c.a; _c.b *= _c.a; _c.m_format |= PREMULTIPLIED; \
            convert(src->m_desc.internalFormat | PREMULTIPLIED, dst->m_desc.internalFormat, &_c); }\
        else{ convert(src->m_desc.internalFormat, dst->m_desc.internalFormat, &_c); } \
    } \
    else { \
        _c.a = _c.r = _c.g = _c.b = 0.f; \
    } \
}
     if (aq & VG_IMAGE_QUALITY_FASTER)
     {
         for (int dy = 0; dy < dst->m_height; ++dy) {
             for (int dx = 0; dx < dst->m_width; ++dx) {
                 float px, py, w00, w01, w10, w11;
                 int x00, y00, x01, y01, x10, y10, x11, y11;
                 Color c00, c01, c10, c11, c;
                 px = dx * inverseMatrix.matrix[0][0] + dy * inverseMatrix.matrix[0][1] + inverseMatrix.matrix[0][2];
                 py = dx * inverseMatrix.matrix[1][0] + dy * inverseMatrix.matrix[1][1] + inverseMatrix.matrix[1][2];
                 /* Bilinear interpolation:
                         p00     p01
                              p
                         p10     p11
                 */
                 if (px >= src->m_width || py >= src->m_height || px < 0.f || py < 0.f)
                     continue;   // out of bound
                 x00 = (int)px;  y00 = (int)py;
                 x01 = x00 + 1;  y01 = y00;
                 x10 = x00;      y10 = y00 + 1;
                 x11 = x00 + 1;  y11 = y00 + 1;

                 readOnePixel(context, c00, x00, y00);
                 readOnePixel(context, c01, x01, y01);
                 readOnePixel(context, c10, x10, y10);
                 readOnePixel(context, c11, x11, y11);

                 w00 = (y11 - py) * (x11 - px);
                 w01 = (y10 - py) * (px - x10);
                 w10 = (py - y01) * (x01 - px);
                 w11 = (py - y00) * (px - x00);
                 c.a = w00 * c00.a + w01 * c01.a + w10 * c10.a + w11 * c11.a;
                 c.r = w00 * c00.r + w01 * c01.r + w10 * c10.r + w11 * c11.r;
                 c.g = w00 * c00.g + w01 * c01.g + w10 * c10.g + w11 * c11.g;
                 c.b = w00 * c00.b + w01 * c01.b + w10 * c10.b + w11 * c11.b;
                 c.m_format = dst->m_desc.format;

                 writePixel(dst, dx, dy, c, display);
             }
         }
     }
     else if (aq & VG_IMAGE_QUALITY_BETTER)
     {
         //to do
     }
     else
     {
         for (int dy = 0; dy < dst->m_height; ++dy) {
             for (int dx = 0; dx < dst->m_width; ++dx) {
                 float px, py;
                 px = dx * inverseMatrix.matrix[0][0] + dy * inverseMatrix.matrix[0][1] + inverseMatrix.matrix[0][2];
                 py = dx * inverseMatrix.matrix[1][0] + dy * inverseMatrix.matrix[1][1] + inverseMatrix.matrix[1][2];

                 if (px >= src->m_width || py >= src->m_height || px < 0.f || py < 0.f)
                     continue;   // out of bound
                 Color c;
                 //readOnePixel(context, c, (int)px, (int)py);
                 c = readPixel(src, (int)px, (int)py);
                 if (context->m_paintType == VG_PAINT_TYPE_COLOR && ((vg_lite_buffer_t*)src->m_vglbuf)->image_mode == VG_DRAW_IMAGE_MULTIPLY) {
                     c.r = c.r * context->m_paintColor.r;
                     c.g = c.g * context->m_paintColor.g;
                     c.b = c.b * context->m_paintColor.b;
                     c.a = c.a * context->m_paintColor.a;
                 }
                 c.m_format = src->m_desc.internalFormat;
                 if (context->m_colorTransform) {
                     c.r = c.r * context->m_colorTransformValues[0] + context->m_colorTransformValues[4];
                     c.g = c.g * context->m_colorTransformValues[1] + context->m_colorTransformValues[5];
                     c.b = c.b * context->m_colorTransformValues[2] + context->m_colorTransformValues[6];
                     c.a = c.a * context->m_colorTransformValues[3] + context->m_colorTransformValues[7];
                     clampColor(context, &c);
                 }
                 convert(c.m_format, dst->m_desc.internalFormat, &c);
                 writePixel(dst, dx, dy, c, display);
             }
         }
     }
     VG_RETURN(VG_NO_RETVAL);
 }

void VG_APIENTRY vgImageSubData(VGImage image, const void* data, VGint dataStride, VGImageFormat dataFormat, VGint x, VGint y, VGint width, VGint height)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    VG_IF_ERROR(!isValidImage(context, image), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    Image* img = (Image*)image;

    VG_IF_ERROR(isEglVgInUse(img), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!isValidImageFormat(dataFormat), VG_UNSUPPORTED_IMAGE_FORMAT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!data || !isFormatAligned(data, dataFormat) || width <= 0 || height <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

    vg_lite_buffer_t srcbuf;
    vg_lite_buffer_t* dstbuf ;
    uint32_t mul, div, align;
    dstbuf = (vg_lite_buffer_t*)(img->m_vglbuf);
    ColorDescriptor src_desc = formatToDescriptor(dataFormat);

    VGint byteOneLine;
    uint8_t* temp_memory = NULL;
    /* Special format not support blit, so cpu operation. Currently only support start at (0,0). */
    if ((!isSupportFormat(context, dataFormat)) || (!isSupportFormat(context, dstbuf->format))) {

        Image srcimg = {0};
        srcimg.m_data = (VGubyte*)data;
        srcimg.m_width = width;
        srcimg.m_height = height;
        srcimg.m_stride = dataStride;
        srcimg.m_desc = formatToDescriptor(dataFormat);
        imgSetPixel(img, &srcimg, 0, 0, x, y, width, height, VG_FALSE);

        VG_RETURN(VG_NO_RETVAL);
    }
    int tmp_sx = 0;
    int tmp_sy = 0;
    int tmp_dx = x;
    int tmp_dy = y;
    int tmp_w = width;
    int tmp_h = height;
    get_format_bytes(dataFormat, &mul, &div, &align);
    computeBlitRegion(&tmp_sx, &tmp_sy, &tmp_dx, &tmp_dy, &tmp_w, &tmp_h, width, height, ((Image*)img)->m_width, ((Image*)img)->m_height);

    memset(&srcbuf, 0, sizeof(vg_lite_buffer_t));
    srcbuf.format = dataFormat;
    srcbuf.width = width;
    srcbuf.height = height;
    vg_lite_allocate(&srcbuf);
    srcbuf.tiled = VG_LITE_LINEAR;
    srcbuf.image_mode = VG_LITE_NORMAL_IMAGE_MODE;
    srcbuf.compress_mode = VG_LITE_DEC_DISABLE;
    /* Construct a src buffer for blit operation. */
    get_format_bytes(srcbuf.format, &mul, &div, &align);
    byteOneLine = tmp_w * mul / div;
    memset(srcbuf.memory, 0, srcbuf.stride * srcbuf.height);

    temp_memory = srcbuf.memory;
    /* Make a complete copy from the start address of data. */
    for (int j = 0; j < tmp_h; j++) {
        memcpy((uint8_t*)temp_memory + j * srcbuf.stride, (uint8_t*)data + tmp_sx * mul / div + tmp_sy * dataStride + j * dataStride, byteOneLine);
    }
    /* Adjust the coordinate position of blit according to tmp_sx, tmp_sy, tmp_w, tmp_h. */
    if (context->m_masking == VG_TRUE) {
        vg_lite_disable_masklayer();
    }

    if (descriptorIsPremultiplied(&src_desc) && descriptorIsPremultiplied(&(img->m_desc))) {
        vg_lite_blit(&srcbuf, &srcbuf, NULL, OPENVG_BLEND_SRC, 0, VG_LITE_FILTER_POINT);
    }

    vg_lite_copy_image(dstbuf, &srcbuf, (VGint)(tmp_dx + img->m_storageOffsetX), (VGint)(tmp_dy + img->m_storageOffsetY), 0, 0, tmp_w, tmp_h);
    vg_lite_finish();

    vg_lite_free(&srcbuf);

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgGetImageSubData(VGImage image, void* data, VGint dataStride, VGImageFormat dataFormat, VGint x, VGint y, VGint width, VGint height)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidImage(context, image), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    Image* img = (Image*)image;
    VG_IF_ERROR(isEglVgInUse(img), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!isValidImageFormat(dataFormat), VG_UNSUPPORTED_IMAGE_FORMAT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!data || !isFormatAligned(data, dataFormat) || width <= 0 || height <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    uint32_t mul, div, align;
    vg_lite_buffer_t dstbuf;
    vg_lite_buffer_t* srcbuf = img->m_vglbuf;
    vg_lite_matrix_t n;
    vg_lite_filter_t filter = VG_LITE_FILTER_POINT;
    vg_lite_blend_t blend = VG_LITE_BLEND_NONE;

    memset(&dstbuf, 0, sizeof(vg_lite_buffer_t));
    dstbuf.width = width;
    dstbuf.height = height;
    dstbuf.format = dataFormat;
    vg_lite_allocate(&dstbuf);
    dstbuf.compress_mode = VG_LITE_DEC_DISABLE;
    dstbuf.image_mode = VG_LITE_NORMAL_IMAGE_MODE;
    dstbuf.tiled = VG_LITE_LINEAR;

    /* Special format not support blit, so cpu operation. Currently only support start at (0,0). */
    if ((!isSupportFormat(context, srcbuf->format)) || (!isSupportFormat(context, dataFormat))) {
        Image dst;
        dst.m_allowedQuality = 0;
        dst.m_data = NULL;
        dst.m_desc = formatToDescriptor(dataFormat);
        dst.m_height = height;
        dst.m_inUse = 0;
        dst.m_mipmapsValid = VG_FALSE;
        dst.m_ownsData = VG_FALSE;
        dst.m_parent = NULL;
        dst.m_referenceCount = 0;
        dst.m_storageOffsetX = 0;
        dst.m_storageOffsetY = 0;
        dst.m_stride = dataStride;
        dst.m_vglbuf = &dstbuf;
        dst.m_width = width;
        imgSetPixel(&dst, img, x, y, 0, 0, width, height, VG_TRUE);
        get_format_bytes(dataFormat, &mul, &div, &align);
        int size = (size_t)ceil((double)(dstbuf.width * mul / div));
        for (int i = 0; i < dstbuf.height; i++) {
            memcpy((uint8_t*)data + i*dataStride, (uint8_t*)dstbuf.memory + i*dst.m_stride, size);
        }
    }
    else {
        int tmp_sx = x, tmp_sy = y, tmp_dx = 0, tmp_dy = 0, tmp_w = width, tmp_h = height;

        get_format_bytes(dataFormat, &mul, &div, &align);
        computeBlitRegion(&tmp_sx, &tmp_sy, &tmp_dx, &tmp_dy, &tmp_w, &tmp_h, ((Image*)img)->m_width, ((Image*)img)->m_height, width, height);
        vg_lite_identity(&n);
        vg_lite_translate((VGfloat)0, (VGfloat)0, &n);
        vg_lite_rectangle_t rect = { tmp_sx + img->m_storageOffsetX, tmp_sy + img->m_storageOffsetY, tmp_w, tmp_h };
        vg_lite_blit_rect(&dstbuf, srcbuf, &rect, &n, blend, 0, filter);
        vg_lite_finish();
        for (int i = 0; i < dstbuf.height; i++) {
            memcpy((uint8_t*)data + i*dataStride, (uint8_t*)dstbuf.memory + i*dstbuf.stride, dstbuf.width* mul / div);
        }
    }
    vg_lite_free(&dstbuf);

    VG_RETURN(VG_NO_RETVAL);
}

VGImage VG_APIENTRY vgChildImage(VGImage parent, VGint x, VGint y, VGint width, VGint height)
{
    VG_GET_CONTEXT(VG_INVALID_HANDLE);

    VG_IF_ERROR(!isValidImage(context, parent), VG_BAD_HANDLE_ERROR, VG_INVALID_HANDLE);
    Image* p = (Image*)parent;
    VG_IF_ERROR(isEglVgInUse((Image*)parent), VG_IMAGE_IN_USE_ERROR, VG_INVALID_HANDLE);
    VG_IF_ERROR(x < 0 || x >= p->m_width || y < 0 || y >= p->m_height ||
                width <= 0 || height <= 0 || VG_INT_ADDSATURATE(x, width) > p->m_width || VG_INT_ADDSATURATE(y, height) > p->m_height, VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);

    Image* child = (Image*)malloc(sizeof(Image));

    if (!child)
    {
        setError(VG_OUT_OF_MEMORY_ERROR);
        VG_RETURN(VG_INVALID_HANDLE);
    }
    createChildImage(child, p, x, y, width, height);
    addImageResource(context, child);
    VG_RETURN((VGImage)child);
}

VGImage VG_APIENTRY vgGetParent(VGImage image)
{
    VG_GET_CONTEXT(VG_INVALID_HANDLE);
    VG_IF_ERROR(!isValidImage(context, image), VG_BAD_HANDLE_ERROR, VG_INVALID_HANDLE);
    VGImage ret = image;    //if image has no ancestors, image is returned.

    // The vgGetParent function returns the closest valid ancestor of the given image.
    // (i.e., one that has not been the target of a vgDestroyImage call)
    Image* im = ((Image*)image)->m_parent;
    for (; im; im = im->m_parent)
    {
        if (isValidImage(context, (VGImage)im))
        {    //the parent is valid and alive
            ret = (VGImage)im;
            break;
        }
    }

    VG_RETURN(ret);
}

void VG_APIENTRY vgCopyImage(VGImage dst, VGint dx, VGint dy, VGImage src, VGint sx, VGint sy, VGint width, VGint height, VGboolean dither)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidImage(context, dst) || !isValidImage(context, src), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(isEglVgInUse((Image*)dst) || isEglVgInUse((Image*)src), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(width <= 0 || height <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

    vg_lite_buffer_t* dstbuf = (vg_lite_buffer_t*)((Image*)dst)->m_vglbuf;
    vg_lite_buffer_t* srcbuf = (vg_lite_buffer_t*)((Image*)src)->m_vglbuf;
    vg_lite_matrix_t n;
    vg_lite_identity(&n);
    vg_lite_translate((VGfloat)dx, (VGfloat)dy, &n);

    /* Special format not support blit, so cpu operation. Currently only support start at (0,0). */
    if (dither) {
        vg_lite_enable_dither();
    }

    if ((!isSupportFormat(context, srcbuf->format)) || (!isSupportFormat(context, dstbuf->format))) {
        imgSetPixel((Image*)dst, (Image*)src, sx, sy, dx, dy, width, height, VG_TRUE);
    }
    else {
        int tmp_sx = sx;
        int tmp_sy = sy;
        int tmp_dx = dx;
        int tmp_dy = dy;
        int tmp_w = width;
        int tmp_h = height;

        if (((Image*)src)->m_storageOffsetX || ((Image*)src)->m_storageOffsetY) {
            computeBlitRegion(&tmp_sx, &tmp_sy, &tmp_dx, &tmp_dy, &tmp_w, &tmp_h, ((Image*)src)->m_parent->m_width, ((Image*)src)->m_parent->m_height, ((Image*)dst)->m_width, ((Image*)dst)->m_height);
        }
        else {
            computeBlitRegion(&tmp_sx, &tmp_sy, &tmp_dx, &tmp_dy, &tmp_w, &tmp_h, ((Image*)src)->m_width, ((Image*)src)->m_height, ((Image*)dst)->m_width, ((Image*)dst)->m_height);
        }

        if (tmp_w <= 0 || tmp_h <= 0) {
            VG_RETURN(VG_NO_RETVAL);
        }
     
        vg_lite_rectangle_t rect;
        if (((Image*)src)->m_storageOffsetX || ((Image*)src)->m_storageOffsetY) {
            rect.x = tmp_sx + ((Image*)src)->m_storageOffsetX;
            rect.y = tmp_sy + ((Image*)src)->m_storageOffsetY;
            rect.width = tmp_w;
            rect.height = tmp_h;
        }
        else {
            rect.x = tmp_sx;
            rect.y = tmp_sy;
            rect.width = tmp_w;
            rect.height = tmp_h;
        }

        if (descriptorIsPremultiplied(&(((Image*)src)->m_desc)) && descriptorIsPremultiplied(&(((Image*)dst)->m_desc))) {
            vg_lite_blit(srcbuf, srcbuf, NULL, OPENVG_BLEND_SRC, 0, VG_LITE_FILTER_POINT);
        }

        vg_lite_copy_image(dstbuf, srcbuf, (VGint)(tmp_dx + ((Image*)dst)->m_storageOffsetX), (VGint)(tmp_dy + ((Image*)dst)->m_storageOffsetX), rect.x, rect.y, rect.width, rect.height);
        vg_lite_flush();
    }

    VG_RETURN(VG_NO_RETVAL);
}

static VGboolean drawImage(VGContext* context, VGImage image, const Matrix3x3 userToSurfaceMatrix)
{
    Drawable* drawable = context->m_eglDrawable;
    if (!drawable)
        return VG_FALSE;
    Image* img = (Image*)image;
    context->m_image = VG_TRUE;
    vg_lite_buffer_t* dstbuf = (vg_lite_buffer_t*)drawable->m_color->m_image->m_vglbuf;
    vg_lite_buffer_t tmpbuf;
    vg_lite_buffer_t* srcbuf = (vg_lite_buffer_t*)img->m_vglbuf;
    vg_lite_buffer_t child = { 0 };
    vg_lite_filter_t filter = VG_LITE_FILTER_POINT;
    /* openvg default blend mode is SRC OVER. */
    vg_lite_blend_t blend = context->m_blendMode;
    vg_lite_color_t srcColor = 0xFF000000;
    VGboolean flag = VG_TRUE;
    VGImage tmpImage = 0;
    Matrix3x3 userToSurfaceMatrix_temp;
    RoundMatrix3x3(&userToSurfaceMatrix_temp, &userToSurfaceMatrix);
    Matrix3x3 fillPaintToSurfaceFill = userToSurfaceMatrix_temp;
    multMatrix3x3(&fillPaintToSurfaceFill, &(context->m_fillPaintToUser));

    srcbuf->image_mode = context->m_imageMode;

    memset(&tmpbuf, 0, sizeof(vg_lite_buffer_t));
    tmpbuf.width = dstbuf->width;
    tmpbuf.height = dstbuf->height;
    tmpbuf.format = dstbuf->format;
    vg_lite_allocate(&tmpbuf);

    Paint* paint = (Paint*)context->m_fillPaint;
    if (paint)
    {
        Color dstColor;
        dstColor.r = paint->m_paintColor.r;
        dstColor.g = paint->m_paintColor.g;
        dstColor.b = paint->m_paintColor.b;
        dstColor.a = paint->m_paintColor.a;
        dstColor.m_format = paint->m_paintColor.m_format;
        vg_lite_uint8_t a = VG_FLOAT_TO_UB(dstColor.a);
        vg_lite_uint8_t b = VG_FLOAT_TO_UB(dstColor.b);
        vg_lite_uint8_t g = VG_FLOAT_TO_UB(dstColor.g);
        vg_lite_uint8_t r = VG_FLOAT_TO_UB(dstColor.r);
        srcColor = (a << 24) | (b << 16) | (g << 8) | (r << 0);
        srcbuf->paintType = paint->m_paintType;
    }

    switch (context->m_imageQuality)
    {
    case VG_IMAGE_QUALITY_NONANTIALIASED:
        filter = VG_LITE_FILTER_POINT;
        break;
    case VG_IMAGE_QUALITY_FASTER:
        filter = VG_LITE_FILTER_LINEAR;
        break;
    case VG_IMAGE_QUALITY_BETTER:
        filter = VG_LITE_FILTER_BI_LINEAR;
        break;
    default:
        break;
    }

    if (context->m_colorTransform)
    {
        vg_lite_color_transform_t color_transformations;

        color_transformations.r_scale = context->m_colorTransformValues[0];
        color_transformations.g_scale = context->m_colorTransformValues[1];
        color_transformations.b_scale = context->m_colorTransformValues[2];
        color_transformations.a_scale = context->m_colorTransformValues[3];
        color_transformations.r_bias = context->m_colorTransformValues[4];
        color_transformations.g_bias = context->m_colorTransformValues[5];
        color_transformations.b_bias = context->m_colorTransformValues[6];
        color_transformations.a_bias = context->m_colorTransformValues[7];

        vg_lite_enable_color_transform();
        vg_lite_set_color_transform(&color_transformations);
    }

    /* Split child image from parent image. */
    if (img->m_storageOffsetX > 0 || img->m_storageOffsetY > 0 || img->m_width != srcbuf->width || img->m_height != srcbuf->height)
    {
        vg_lite_rectangle_t rect = { img->m_storageOffsetX, img->m_storageOffsetY, img->m_width, img->m_height };

        memset(&child, 0, sizeof(vg_lite_buffer_t));
        child.width = img->m_width;
        child.height = img->m_height;
        child.format = srcbuf->format;
        child.image_mode = srcbuf->image_mode;
        vg_lite_allocate(&child);
        child.compress_mode = 0;
        child.tiled = 0;

        if ((!isSupportFormat(context, srcbuf->format)) || (!isSupportFormat(context, dstbuf->format))) {
            Image childImage = { 0 };
            childImage.m_data = child.memory;
            childImage.m_width = child.width;
            childImage.m_height = child.height;
            childImage.m_stride = child.stride;
            childImage.m_desc = formatToDescriptor(img->m_desc.format);
            imgSetPixel(&childImage, img, rect.x, rect.y, 0, 0, rect.width, rect.height, VG_TRUE);
        }
        else {
            vg_lite_disable_scissor();
            vg_lite_blit_rect(&child, srcbuf, &rect, NULL, VG_LITE_BLEND_NONE, 0, filter);
            vg_lite_flush();
            if (context->m_scissoring && context->m_scissorCount > 0) {
                vg_lite_enable_scissor();
            }
        }
        srcbuf = &child;
    }

    /* Special format not support blit, so cpu operation. Currently only support start at (0,0). */
    if ((!isSupportFormat(context, srcbuf->format)) || (!isSupportFormat(context, dstbuf->format))) {
        if (isSupportFormat(context, dstbuf->format)) {
            vg_lite_buffer_format_t format;
            if (srcbuf->format == VG_sL_8) {
                format = VG_sRGBA_8888;
            }
            else {
                format = VG_lRGBA_8888;
            }

            tmpImage = vgCreateImage(format, img->m_width, img->m_height, VG_IMAGE_QUALITY_NONANTIALIASED);
            Image* tmpImg = (Image*)tmpImage;
            vg_lite_buffer_t* tmpbuf;
            convertFormat(context, tmpImg, img);

            tmpbuf = (vg_lite_buffer_t*)tmpImg->m_vglbuf;
            tmpbuf->image_mode = srcbuf->image_mode;
            tmpbuf->paintType = srcbuf->paintType;

            srcbuf = tmpbuf;
            flag = VG_TRUE;
        }
        else {
            imgBlit(context, drawable->m_color->m_image, img, userToSurfaceMatrix, VG_TRUE, context->m_imageQuality);
            flag = VG_FALSE;
        }
    }

    if (flag) {
        if (srcbuf->image_mode == VG_LITE_MULTIPLY_IMAGE_MODE || srcbuf->image_mode == VG_LITE_STENCIL_MODE) {
            if (srcbuf->paintType == VG_PAINT_TYPE_LINEAR_GRADIENT) {
                vg_lite_ext_linear_gradient_t grad;
                vg_lite_linear_gradient_parameter_t linear_gradient;
                vg_lite_color_ramp_t vg_color_ramp[VIV_MAX_COLOR_RAMP_STOPS];
                GradientStop* gradientstop = paint->m_colorRampStops;
                vg_lite_color_t fill_color = 0xFF000000;
                linear_gradient.X0 = paint->m_linearGradientPoint0.x;
                linear_gradient.Y0 = paint->m_linearGradientPoint0.y;
                linear_gradient.X1 = paint->m_linearGradientPoint1.x;
                linear_gradient.Y1 = paint->m_linearGradientPoint1.y;

                for (int i = 0; i < paint->m_colorRampStopsCount; i++)
                {
                    vg_color_ramp[i].stop = gradientstop[i].offset;
                    vg_color_ramp[i].red = gradientstop[i].color.r;
                    vg_color_ramp[i].green = gradientstop[i].color.g;
                    vg_color_ramp[i].blue = gradientstop[i].color.b;
                    vg_color_ramp[i].alpha = gradientstop[i].color.a;
                }
                char path_data[] = {
                    2, 0, 0,
                    4, srcbuf->width, 0,
                    4, srcbuf->width, srcbuf->height,
                    4, 0, srcbuf->height,
                    0,
                };
                vg_lite_path_t vglpath = {
                    {0, 0, (vg_lite_float_t)srcbuf->width, (vg_lite_float_t)srcbuf->width},
                    VG_LITE_HIGH,
                    VG_LITE_S8,
                    {0},
                    sizeof(path_data), // path length
                    path_data, // path data
                    1
                };
                if ((linear_gradient.X0 != linear_gradient.X1) || (linear_gradient.Y0 != linear_gradient.Y1))
                {
                    memset(&grad, 0, sizeof(grad));
                    if (paint->grad_image.handle != NULL)
                    {
                        vg_lite_finish();
                        vg_lite_free(&paint->grad_image);
                    }
                    setLinearGrad(&grad, paint->m_colorRampStopsCount, vg_color_ramp, linear_gradient, paint->m_colorRampSpreadMode, paint->m_colorRampPremultiplied);
                    updateLinearGrad(context, &grad, (Matrix3x3*)&userToSurfaceMatrix_temp);
                    tmpbuf.image_mode = srcbuf->image_mode;
                    vg_lite_draw_linear_grad(&tmpbuf, &vglpath, VG_LITE_FILL_EVEN_ODD, (vg_lite_matrix_t*)&userToSurfaceMatrix_temp, &grad, 0x0, blend, VG_LITE_FILTER_LINEAR);
                    grad.count = 0;
                    paint->grad_image = grad.image;
                }
                else {
                    if (paint->m_colorRampSpreadMode == VG_COLOR_RAMP_SPREAD_REPEAT) {
                        vg_lite_uint8_t a = VG_FLOAT_TO_UB(gradientstop[0].color.a);
                        vg_lite_uint8_t b = VG_FLOAT_TO_UB(gradientstop[0].color.b);
                        vg_lite_uint8_t g = VG_FLOAT_TO_UB(gradientstop[0].color.g);
                        vg_lite_uint8_t r = VG_FLOAT_TO_UB(gradientstop[0].color.r);
                        fill_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                    }
                    else {
                        vg_lite_uint8_t a = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.a);
                        vg_lite_uint8_t b = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.b);
                        vg_lite_uint8_t g = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.g);
                        vg_lite_uint8_t r = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.r);
                        fill_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                    }
                    tmpbuf.image_mode = srcbuf->image_mode;
                    vg_lite_draw(&tmpbuf, &vglpath, VG_LITE_FILL_EVEN_ODD, (vg_lite_matrix_t*)&userToSurfaceMatrix_temp, blend, fill_color);
                }

            }
            else if (srcbuf->paintType == VG_PAINT_TYPE_RADIAL_GRADIENT) {
                vg_lite_radial_gradient_t grad;
                vg_lite_radial_gradient_parameter_t radialGradient;
                vg_lite_color_ramp_t vg_color_ramp[VIV_MAX_COLOR_RAMP_STOPS];
                GradientStop* gradientstop = paint->m_colorRampStops;
                vg_lite_color_t fill_color = 0xFF000000;
                radialGradient.cx = paint->m_radialGradientCenter.x;
                radialGradient.cy = paint->m_radialGradientCenter.y;
                radialGradient.fx = paint->m_radialGradientFocalPoint.x;
                radialGradient.fy = paint->m_radialGradientFocalPoint.y;
                radialGradient.r = paint->m_radialGradientRadius;

                for (int i = 0; i < paint->m_colorRampStopsCount; i++)
                {
                    vg_color_ramp[i].stop = gradientstop[i].offset;
                    vg_color_ramp[i].red = gradientstop[i].color.r;
                    vg_color_ramp[i].green = gradientstop[i].color.g;
                    vg_color_ramp[i].blue = gradientstop[i].color.b;
                    vg_color_ramp[i].alpha = gradientstop[i].color.a;
                }
                char path_data[] = {
                    2, 0, 0,
                    4, srcbuf->width, 0,
                    4, srcbuf->width, srcbuf->height,
                    4, 0, srcbuf->height,
                    0,
                };
                vg_lite_path_t vglpath = {
                    {0, 0, (vg_lite_float_t)srcbuf->width, (vg_lite_float_t)srcbuf->width},
                    VG_LITE_HIGH,
                    VG_LITE_S8,
                    {0},
                    sizeof(path_data), // path length
                    path_data, // path data
                    1
                };
                if (radialGradient.r > 0)
                {
                    memset(&grad, 0, sizeof(grad));
                    if (paint->grad_image.handle != NULL)
                    {
                        vg_lite_finish();
                        vg_lite_free(&paint->grad_image);
                    }
                    setRadialGrad(&grad, paint->m_colorRampStopsCount, vg_color_ramp, radialGradient, paint->m_colorRampSpreadMode, paint->m_colorRampPremultiplied);
                    updateRadialGrad(context, &grad, (Matrix3x3*)&userToSurfaceMatrix_temp);
                    tmpbuf.image_mode = srcbuf->image_mode;
                    vg_lite_draw_radial_grad(&tmpbuf, &vglpath, VG_LITE_FILL_EVEN_ODD, (vg_lite_matrix_t*)&userToSurfaceMatrix_temp, &grad, 0, blend, VG_LITE_FILTER_LINEAR);
                    grad.count = 0;
                    paint->grad_image = grad.image;
                }
                else {
                    if (paint->m_colorRampSpreadMode == VG_COLOR_RAMP_SPREAD_REPEAT) {
                        vg_lite_uint8_t a = VG_FLOAT_TO_UB(gradientstop[0].color.a);
                        vg_lite_uint8_t b = VG_FLOAT_TO_UB(gradientstop[0].color.b);
                        vg_lite_uint8_t g = VG_FLOAT_TO_UB(gradientstop[0].color.g);
                        vg_lite_uint8_t r = VG_FLOAT_TO_UB(gradientstop[0].color.r);
                        fill_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                    }
                    else {
                        vg_lite_uint8_t a = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.a);
                        vg_lite_uint8_t b = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.b);
                        vg_lite_uint8_t g = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.g);
                        vg_lite_uint8_t r = VG_FLOAT_TO_UB(gradientstop[paint->m_colorRampStopsCount - 1].color.r);
                        fill_color = (a << 24) | (b << 16) | (g << 8) | (r << 0);
                    }
                    tmpbuf.image_mode = srcbuf->image_mode;
                    vg_lite_draw(&tmpbuf, &vglpath, VG_LITE_FILL_EVEN_ODD, (vg_lite_matrix_t*)&userToSurfaceMatrix_temp, blend, fill_color);
                }

            }
            else if (srcbuf->paintType == VG_PAINT_TYPE_PATTERN)
            {
                VGint m_width = context->m_pattern->m_width;
                VGint m_height = context->m_pattern->m_height;
                vg_lite_buffer_t* source = (vg_lite_buffer_t*)context->m_pattern->m_vglbuf;

                char path_data[] = {
                    2, 0, 0,
                    4, m_width, 0,
                    4, m_width, m_height,
                    4, 0, m_height,
                    0,
                };

                vg_lite_path_t path = {
                    {0, 0, (vg_lite_float_t)m_width, (vg_lite_float_t)m_height},
                    VG_LITE_HIGH,
                    VG_LITE_S8,
                    {0},
                    sizeof(path_data),
                    path_data,
                    1
                };
                vg_lite_image_mode_t tmp_image_mode = source->image_mode;
                vg_lite_paint_type_t tmp_paintType = source->paintType;
                path.path_type = VG_LITE_DRAW_FILL_PATH;
                source->image_mode = VG_LITE_NONE_IMAGE_MODE;
                source->paintType = VG_LITE_PAINT_PATTERN;
                tmpbuf.image_mode = srcbuf->image_mode;
                vg_lite_draw_pattern(&tmpbuf, &path, VG_LITE_FILL_EVEN_ODD, (vg_lite_matrix_t*)&fillPaintToSurfaceFill, source, (vg_lite_matrix_t*)&fillPaintToSurfaceFill, blend, VG_LITE_PATTERN_COLOR, 0, 0, VG_LITE_FILTER_POINT);
                source->image_mode = tmp_image_mode;
                source->paintType = tmp_paintType;
            }
        }

        if (!context->m_colorTransform && blend > OPENVG_BLEND_SRC && (dstbuf->format == OPENVG_sRGBA_5551_PRE || dstbuf->format == OPENVG_lRGBA_5551_PRE)) {
            vg_lite_blit(dstbuf, dstbuf, NULL, OPENVG_BLEND_SRC, 0, VG_LITE_FILTER_POINT);
        }
        vg_lite_blit(dstbuf, srcbuf, (vg_lite_matrix_t*)&userToSurfaceMatrix_temp, blend, srcColor, filter);
        vg_lite_flush();
    }

    context->m_image = VG_FALSE;
    if (context->m_colorTransform)
        vg_lite_disable_color_transform();

    if (child.handle) {
        vg_lite_free(&child);
    }

    if (tmpbuf.handle) {
        vg_lite_free(&tmpbuf);
    }

    if (tmpImage) {
        vgDestroyImage(tmpImage);
    }

    return VG_TRUE;
}

void VG_APIENTRY vgDrawImage(VGImage image)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidImage(context, image), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    Image* img = (Image*)image;
    VG_IF_ERROR(isEglVgInUse(img), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);

    if (!drawImage(context, image, context->m_imageUserToSurface))
    {
        VG_RETURN(VG_NO_RETVAL);
    }
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgSetPixels(VGint dx, VGint dy, VGImage src, VGint sx, VGint sy, VGint width, VGint height)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    VG_IF_ERROR(!isValidImage(context, src), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(isEglVgInUse((Image*)src), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(width <= 0 || height <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    Drawable* drawable = context->m_eglDrawable;
    if (!drawable)
    {
        VG_RETURN(VG_NO_RETVAL);    //no EGL surface is current at the moment
    }
    vg_lite_buffer_t* dstbuf = (vg_lite_buffer_t*)drawable->m_color->m_image->m_vglbuf;
    vg_lite_buffer_t* srcbuf = (vg_lite_buffer_t*)((Image*)src)->m_vglbuf;
    Image* img = (Image*)src;
    Image* dstimg = drawable->m_color->m_image;

    /* Special format not support blit, so cpu operation. Currently only support start at (0,0). */
    if ((!isSupportFormat(context, srcbuf->format)) || (!isSupportFormat(context, dstbuf->format))) {

        imgSetPixel(drawable->m_color->m_image, (Image*)src, sx, sy, dx, dy, width, height, VG_FALSE);

        VG_RETURN(VG_NO_RETVAL);
    }

    int tmp_sx = sx;
    int tmp_sy = sy;
    int tmp_dx = dx;
    int tmp_dy = dy;
    int tmp_w = width;
    int tmp_h = height;

    computeBlitRegion(&tmp_sx, &tmp_sy, &tmp_dx, &tmp_dy, &tmp_w, &tmp_h, ((Image*)src)->m_width, ((Image*)src)->m_height, dstbuf->width, dstbuf->height);
    vg_lite_rectangle_t rect = { tmp_sx + img->m_storageOffsetX, tmp_sy + img->m_storageOffsetY, tmp_w, tmp_h };
    vg_lite_disable_masklayer();

    if (descriptorIsPremultiplied(&(img->m_desc)) && descriptorIsPremultiplied(&(dstimg->m_desc))) {
        vg_lite_blit(srcbuf, srcbuf, NULL, OPENVG_BLEND_SRC, 0, VG_LITE_FILTER_POINT);
    }
    vg_lite_copy_image(dstbuf, srcbuf, (VGint)tmp_dx, (VGint)tmp_dy, rect.x, rect.y, rect.width, rect.height);
    vg_lite_flush();
    if (context->m_masking) {
        vg_lite_enable_masklayer();
    }
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgWritePixels(const void * data, VGint dataStride, VGImageFormat dataFormat, VGint dx, VGint dy, VGint width, VGint height)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidImageFormat(dataFormat), VG_UNSUPPORTED_IMAGE_FORMAT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!data || !isFormatAligned(data, dataFormat) || width <= 0 || height <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

    Drawable* drawable = context->m_eglDrawable;
    if (!drawable)
    {
        VG_RETURN(VG_NO_RETVAL);    //no EGL surface is current at the moment
    }

    vg_lite_buffer_t* dstbuf = (vg_lite_buffer_t*)drawable->m_color->m_image->m_vglbuf;

    vg_lite_buffer_t srcbuf;
    uint32_t mul, div, align, byteOneLine;
    Image* dstimg = drawable->m_color->m_image;
    ColorDescriptor src_desc = formatToDescriptor(dataFormat);

    /* Special format not support blit, so cpu operation. Currently only support start at (0,0). */
    if ((!isSupportFormat(context, dstbuf->format)) || (!isSupportFormat(context, dataFormat))) {
        Image srcimg = { 0 };
        srcimg.m_data = (VGubyte*)data;
        srcimg.m_width = width;
        srcimg.m_height = height;
        srcimg.m_stride = dataStride;
        srcimg.m_desc = formatToDescriptor(dataFormat);
        imgSetPixel(drawable->m_color->m_image, &srcimg, 0, 0, dx, dy, width, height, VG_TRUE);
        VG_RETURN(VG_NO_RETVAL);
    }
    int tmp_sx = 0;
    int tmp_sy = 0;
    int tmp_dx = dx;
    int tmp_dy = dy;
    int tmp_w = width;
    int tmp_h = height;
    computeBlitRegion(&tmp_sx, &tmp_sy, &tmp_dx, &tmp_dy, &tmp_w, &tmp_h, width, height, dstbuf->width, dstbuf->height);
    get_format_bytes(dataFormat, &mul, &div, &align);

    memset(&srcbuf, 0, sizeof(vg_lite_buffer_t));
    srcbuf.format = dataFormat;
    srcbuf.width = width;
    srcbuf.height = height;
    vg_lite_allocate(&srcbuf);
    srcbuf.tiled = VG_LITE_LINEAR;
    srcbuf.image_mode = VG_LITE_NORMAL_IMAGE_MODE;
    srcbuf.compress_mode = VG_LITE_DEC_DISABLE;
    /* Construct a src buffer for blit operation. */
    get_format_bytes(srcbuf.format, &mul, &div, &align);
    byteOneLine = srcbuf.width * mul / div;
    memset(srcbuf.memory, 0, srcbuf.stride * srcbuf.height);

    for (int j = 0; j < height; j++) {
        memcpy((uint8_t*)srcbuf.memory + j * srcbuf.stride, (uint8_t*)data + j * dataStride, byteOneLine);
    }

    vg_lite_disable_masklayer();
    /* Adjust the coordinate position of blit according to tmp_sx, tmp_sy, tmp_w, tmp_h. */
    vg_lite_rectangle_t rect = { tmp_sx, tmp_sy, tmp_w, tmp_h };

    if (descriptorIsPremultiplied(&src_desc) && descriptorIsPremultiplied(&(dstimg->m_desc))) {
        vg_lite_blit(&srcbuf, &srcbuf, NULL, OPENVG_BLEND_SRC, 0, VG_LITE_FILTER_POINT);
    }

    vg_lite_copy_image(dstbuf, &srcbuf, (VGint)(tmp_dx + drawable->m_color->m_image->m_storageOffsetX), (VGint)(tmp_dy + drawable->m_color->m_image->m_storageOffsetY), rect.x, rect.y, rect.width, rect.height);
    vg_lite_finish();
    if (context->m_masking) {
        vg_lite_enable_masklayer();
    }
    vg_lite_free(&srcbuf);

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgGetPixels(VGImage dst, VGint dx, VGint dy, VGint sx, VGint sy, VGint width, VGint height)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidImage(context, dst), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(isEglVgInUse((Image*)dst), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(width <= 0 || height <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

    Drawable* drawable = context->m_eglDrawable;
    if (!drawable)
    {
        VG_RETURN(VG_NO_RETVAL);    //no EGL surface is current at the moment
    }
    vg_lite_buffer_t* srcbuf = (vg_lite_buffer_t*)drawable->m_color->m_image->m_vglbuf;
    vg_lite_buffer_t* dstbuf = (vg_lite_buffer_t*)((Image*)dst)->m_vglbuf;
    Image* img = (Image*)dst;
    Image* srcimg = (Image*)drawable->m_color->m_image;

    /* Special format not support blit, so cpu operation. Currently only support start at (0,0). */
    if ((!isSupportFormat(context, srcbuf->format)) || (!isSupportFormat(context, dstbuf->format))) {
        vg_lite_finish();
        imgSetPixel((Image*)dst, drawable->m_color->m_image, sx, sy, dx, dy, width, height, VG_FALSE);

        VG_RETURN(VG_NO_RETVAL);
    }

    int tmp_sx = sx;
    int tmp_sy = sy;
    int tmp_dx = dx;
    int tmp_dy = dy;
    int tmp_w = width;
    int tmp_h = height;
    computeBlitRegion(&tmp_sx, &tmp_sy, &tmp_dx, &tmp_dy, &tmp_w, &tmp_h, srcbuf->width, srcbuf->height, dstbuf->width, dstbuf->height);

    vg_lite_rectangle_t rect = { tmp_sx, tmp_sy, tmp_w, tmp_h };

    if (descriptorIsPremultiplied(&(srcimg->m_desc)) && descriptorIsPremultiplied(&(img->m_desc))) {
        vg_lite_blit(srcbuf, srcbuf, NULL, OPENVG_BLEND_SRC, 0, VG_LITE_FILTER_POINT);
    }

    vg_lite_copy_image(dstbuf, srcbuf, (VGint)(tmp_dx + img->m_storageOffsetX), (VGint)(tmp_dy + img->m_storageOffsetY), rect.x, rect.y, rect.width, rect.height);
    vg_lite_flush();

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgReadPixels(void* data, VGint dataStride, VGImageFormat dataFormat, VGint sx, VGint sy, VGint width, VGint height)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidImageFormat(dataFormat), VG_UNSUPPORTED_IMAGE_FORMAT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!data || !isFormatAligned(data, dataFormat) || width <= 0 || height <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

    Drawable* drawable = context->m_eglDrawable;
    if (!drawable)
    {
        VG_RETURN(VG_NO_RETVAL);
    }
    uint32_t mul, div, align, byteOneLine;
    vg_lite_buffer_t* srcbuf = (vg_lite_buffer_t*)drawable->m_color->m_image->m_vglbuf;
    vg_lite_buffer_t dstbuf;
    Image* srcimage = (Image *)drawable->m_color->m_image;
    ColorDescriptor dst_desc = formatToDescriptor(dataFormat);
    /* Special format not support blit, so cpu operation. Currently only support start at (0,0). */
    if ((!isSupportFormat(context, srcbuf->format)) || (!isSupportFormat(context, dataFormat))) {
        Image dstimg = { 0 };
        dstimg.m_data = (VGubyte*)data;
        dstimg.m_width = width;
        dstimg.m_height = height;
        dstimg.m_stride = dataStride;
        dstimg.m_desc = formatToDescriptor(dataFormat);
        imgSetPixel(&dstimg, drawable->m_color->m_image, sx, sy, 0, 0, width, height, VG_FALSE);
        VG_RETURN(VG_NO_RETVAL);
    }
    int tmp_sx = sx;
    int tmp_sy = sy;
    int tmp_dx = 0;
    int tmp_dy = 0;
    int tmp_w = width;
    int tmp_h = height;
    computeBlitRegion(&tmp_sx, &tmp_sy, &tmp_dx, &tmp_dy, &tmp_w, &tmp_h, srcbuf->width, srcbuf->height, width, height);
    get_format_bytes(dataFormat, &mul, &div, &align);

    memset(&dstbuf, 0, sizeof(vg_lite_buffer_t));
    dstbuf.format = dataFormat;
    dstbuf.width = width;
    dstbuf.height = height;
    vg_lite_allocate(&dstbuf);
    dstbuf.tiled = VG_LITE_LINEAR;
    dstbuf.image_mode = VG_LITE_NORMAL_IMAGE_MODE;
    dstbuf.compress_mode = VG_LITE_DEC_DISABLE;
    /* Construct a src buffer for blit operation. */
    get_format_bytes(dstbuf.format, &mul, &div, &align);
    byteOneLine = dstbuf.width * mul / div;
    memset(dstbuf.memory, 0, dstbuf.stride * dstbuf.height);
    if (context->m_masking) {
        vg_lite_disable_masklayer();
    }
    if (context->m_scissoring) {
        vg_lite_disable_scissor();
    }
    /* Adjust the coordinate position of blit according to tmp_sx, tmp_sy, tmp_w, tmp_h. */
    vg_lite_rectangle_t rect = { tmp_sx + drawable->m_color->m_image->m_storageOffsetX, tmp_sy + drawable->m_color->m_image->m_storageOffsetY, tmp_w, tmp_h };

    if (descriptorIsPremultiplied(&(srcimage->m_desc)) && descriptorIsPremultiplied(&dst_desc)) {
        vg_lite_blit(srcbuf, srcbuf, NULL, OPENVG_BLEND_SRC, 0, VG_LITE_FILTER_POINT);
    }

    vg_lite_copy_image(&dstbuf, srcbuf, (VGint)(tmp_dx), (VGint)(tmp_dy), rect.x, rect.y, rect.width, rect.height);
    vg_lite_finish();
    srcbuf->transparency_mode = 0;
    for (int j = 0; j < height; j++) {
        memcpy((uint8_t*)data + j * dataStride, (uint8_t*)dstbuf.memory + j * dstbuf.stride, byteOneLine);
    }

    if (context->m_masking) {
        vg_lite_enable_masklayer();
    }
    if (context->m_scissoring) {
        vg_lite_enable_scissor();
    }
    vg_lite_free(&dstbuf);

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgCopyPixels(VGint dx, VGint dy, VGint sx, VGint sy, VGint width, VGint height)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(width <= 0 || height <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);

    Drawable* drawable = context->m_eglDrawable;
    if (!drawable)
    {
        VG_RETURN(VG_NO_RETVAL);    //no EGL surface is current at the moment
    }

    vg_lite_buffer_t* srcbuf = (vg_lite_buffer_t*)drawable->m_color->m_image->m_vglbuf;
    vg_lite_buffer_t dstbuf;
    memcpy(&dstbuf, srcbuf, sizeof(vg_lite_buffer_t));
    vg_lite_allocate(&dstbuf);
    vg_lite_matrix_t n;
    vg_lite_filter_t filter = VG_LITE_FILTER_POINT;
    vg_lite_blend_t blend = VG_LITE_BLEND_NONE;

    /* Special format not support blit, so cpu operation. Currently only support start at (0,0). */
    if (!isSupportFormat(context, dstbuf.format)) {
        imgSetPixel(drawable->m_color->m_image, drawable->m_color->m_image, sx, sy, dx, dy, width, height, VG_TRUE);
        VG_RETURN(VG_NO_RETVAL);
    }

    int tmp_sx = sx;
    int tmp_sy = sy;
    int tmp_dx = dx;
    int tmp_dy = dy;
    int tmp_w = width;
    int tmp_h = height;
    computeBlitRegion(&tmp_sx, &tmp_sy, &tmp_dx, &tmp_dy, &tmp_w, &tmp_h, srcbuf->width, srcbuf->height, dstbuf.width, dstbuf.height);

    vg_lite_disable_masklayer();
    /* Adjust the coordinate position of blit according to tmp_sx, tmp_sy, tmp_w, tmp_h. */
    vg_lite_rectangle_t rect = { tmp_sx + drawable->m_color->m_image->m_storageOffsetX, tmp_sy + drawable->m_color->m_image->m_storageOffsetY, tmp_w, tmp_h };
    vg_lite_identity(&n);
    vg_lite_translate((VGfloat)(tmp_dx + drawable->m_color->m_image->m_storageOffsetX), (VGfloat)(tmp_dy + drawable->m_color->m_image->m_storageOffsetY), &n);

    vg_lite_blit_rect(&dstbuf, srcbuf, &rect, &n, blend, 0, filter);

    vg_lite_rectangle_t rect1 = { tmp_dx + drawable->m_color->m_image->m_storageOffsetX, tmp_dy + drawable->m_color->m_image->m_storageOffsetY, tmp_w, tmp_h };
    vg_lite_identity(&n);
    vg_lite_translate((VGfloat)(tmp_dx + drawable->m_color->m_image->m_storageOffsetX), (VGfloat)(tmp_dy + drawable->m_color->m_image->m_storageOffsetY), &n);
    vg_lite_blit_rect(srcbuf, &dstbuf, &rect1, &n, blend, 0, filter);

    vg_lite_finish();
    if (context->m_masking) {
        vg_lite_enable_masklayer();
    }
    vg_lite_free(&dstbuf);
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgColorMatrix(VGImage dst, VGImage src, const VGfloat * matrix)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    VG_IF_ERROR(!isValidImage(context, dst) || !isValidImage(context, src), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    Image* d = (Image*)dst;
    Image* s = (Image*)src;
    VG_IF_ERROR(isEglVgInUse(d) || isEglVgInUse(s), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(imageOverlaps(d, s), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!matrix || !isAligned(matrix,4), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    unsigned int channelMask = context->m_filterChannelMask & (VG_RED|VG_GREEN|VG_BLUE|VG_ALPHA);    //undefined bits are ignored
    vg_lite_buffer_t* dstbuf = (vg_lite_buffer_t*)d->m_vglbuf;
    vg_lite_buffer_t* srcbuf = (vg_lite_buffer_t*)s->m_vglbuf;
    vg_lite_error_t state;
    VGfloat m[20];
    for (int i=0;i<20;i++)
    {
        m[i] = inputFloat(matrix[i]);
    }
    state = vg_lite_set_pixel_matrix(m, (vg_lite_pixel_channel_enable_t*)&channelMask);
    if (state == VG_LITE_NOT_SUPPORT)
    {
        vg_lite_finish();
        colorMatrix(m, s, d, context->m_filterFormatLinear ? VG_TRUE : VG_FALSE, context->m_filterFormatPremultiplied ? VG_TRUE : VG_FALSE, channelMask);
        VG_RETURN(VG_NO_RETVAL);
    }
    vg_lite_rectangle_t rect = { 0, 0, srcbuf->width, srcbuf->height };

    if (descriptorIsPremultiplied(&(s->m_desc)) && descriptorIsPremultiplied(&(d->m_desc))) {
        vg_lite_blit(srcbuf, srcbuf, NULL, OPENVG_BLEND_SRC, 0, VG_LITE_FILTER_POINT);
    }

    vg_lite_copy_image(dstbuf, srcbuf, 0, 0, rect.x, rect.y, rect.width, rect.height);
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgConvolve(VGImage dst, VGImage src, VGint kernelWidth, VGint kernelHeight, VGint shiftX, VGint shiftY, const VGshort * kernel, VGfloat scale, VGfloat bias, VGTilingMode tilingMode)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    VG_IF_ERROR(!isValidImage(context, dst) || !isValidImage(context, src), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    Image* d = (Image*)dst;
    Image* s = (Image*)src;
    VG_IF_ERROR(isEglVgInUse(d) || isEglVgInUse(s), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(imageOverlaps(d, s), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!kernel || !isAligned(kernel,2) || kernelWidth <= 0 || kernelHeight <= 0 || kernelWidth > VIV_MAX_KERNEL_SIZE || kernelHeight > VIV_MAX_KERNEL_SIZE, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(tilingMode < VG_TILE_FILL || tilingMode > VG_TILE_REFLECT, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    unsigned int channelMask = context->m_filterChannelMask & (VG_RED|VG_GREEN|VG_BLUE|VG_ALPHA);    //undefined bits are ignored
    vg_lite_finish();
    convolve(d, s, kernelWidth, kernelHeight, shiftX, shiftY, (const VGshort*)kernel, inputFloat(scale), inputFloat(bias),
        tilingMode, &context->m_tileFillColor, context->m_filterFormatLinear ? VG_TRUE : VG_FALSE,
        context->m_filterFormatPremultiplied ? VG_TRUE : VG_FALSE, channelMask);

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgSeparableConvolve(VGImage dst, VGImage src, VGint kernelWidth, VGint kernelHeight, VGint shiftX, VGint shiftY, const VGshort * kernelX, const VGshort * kernelY, VGfloat scale, VGfloat bias, VGTilingMode tilingMode)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    VG_IF_ERROR(!isValidImage(context, dst) || !isValidImage(context, src), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    Image* d = (Image*)dst;
    Image* s = (Image*)src;
    VG_IF_ERROR(isEglVgInUse(d) || isEglVgInUse(s), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(imageOverlaps(d, s), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!kernelX || !kernelY || !isAligned(kernelX,2) || !isAligned(kernelY,2) || kernelWidth <= 0 || kernelHeight <= 0 || kernelWidth > VIV_MAX_SEPARABLE_KERNEL_SIZE || kernelHeight > VIV_MAX_SEPARABLE_KERNEL_SIZE, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(tilingMode < VG_TILE_FILL || tilingMode > VG_TILE_REFLECT, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    unsigned int channelMask = context->m_filterChannelMask & (VG_RED|VG_GREEN|VG_BLUE|VG_ALPHA);    //undefined bits are ignored
    vg_lite_finish();
    separableConvolve(d, s, kernelWidth, kernelHeight, shiftX, shiftY, (const VGshort*)kernelX, (const VGshort*)kernelY,
        inputFloat(scale), inputFloat(bias), tilingMode, &context->m_tileFillColor, context->m_filterFormatLinear ? VG_TRUE : VG_FALSE,
        context->m_filterFormatPremultiplied ? VG_TRUE : VG_FALSE, channelMask);

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgGaussianBlur(VGImage dst, VGImage src, VGfloat stdDeviationX, VGfloat stdDeviationY, VGTilingMode tilingMode)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidImage(context, dst) || !isValidImage(context, src), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    Image* d = (Image*)dst;
    Image* s = (Image*)src;
    VG_IF_ERROR(isEglVgInUse(d) || isEglVgInUse(s), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(imageOverlaps(d, s), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VGfloat sx = inputFloat(stdDeviationX);
    VGfloat sy = inputFloat(stdDeviationY);
    VG_IF_ERROR(sx <= 0.0f || sy <= 0.0f || sx > (VGfloat)VIV_MAX_GAUSSIAN_DEVIATION || sy > (VGfloat)VIV_MAX_GAUSSIAN_DEVIATION, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(tilingMode < VG_TILE_FILL || tilingMode > VG_TILE_REFLECT, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    //unsigned int channelMask = context->m_filterChannelMask & (VG_RED | VG_GREEN | VG_BLUE | VG_ALPHA);    //undefined bits are ignored
    vg_lite_buffer_t* dstbuf = (vg_lite_buffer_t*)d->m_vglbuf;
    vg_lite_buffer_t* srcbuf = (vg_lite_buffer_t*)s->m_vglbuf;
    vg_lite_filter_t filter = VG_LITE_FILTER_GAUSSIAN;
    vg_lite_blend_t blend = VG_LITE_BLEND_NONE;
    vg_lite_color_t color = 0;
    Matrix3x3 n;
    initMatrix3x3(&n, 1, 0, 0, 0, 1, 0, 0, 0, 1);
    VGfloat expX = -1.0f / (2.0f * sx * sx);
    vg_lite_error_t state;
    VGboolean flag = VG_TRUE;

    /* If the standard deviation in the xand y directions is the same, use hardware processing. */
    if (sx == sy) {
        /* Calculate the operator matrix according to the two-dimensional Gaussian filter formula. */
        VGfloat w0 = (VGfloat)(-expX / 3.1415926 * exp(0 * expX));
        VGfloat w1 = (VGfloat)(-expX / 3.1415926 * exp(1 * expX));
        VGfloat w2 = (VGfloat)(-expX / 3.1415926 * exp(2 * expX));

        w0 = w0 / (4 * w2 + 4 * w1 + w0);
        w1 = w1 / (4 * w2 + 4 * w1 + w0);
        w2 = w2 / (4 * w2 + 4 * w1 + w0);

        state = vg_lite_gaussian_filter(w0, w1, w2);
        if (state != VG_LITE_NOT_SUPPORT){
            vg_lite_blit(dstbuf, srcbuf, (vg_lite_matrix_t*)&n, blend, color, filter);
            vg_lite_flush();
            flag = VG_FALSE;
        }
    }

    /* If the standard deviation in the xand y directions is not the same, use software processing. */
    if (flag) {
        vg_lite_finish();
        gaussianBlur(dst, src, stdDeviationX, stdDeviationY, context->m_filterFormatLinear ? VG_TRUE : VG_FALSE, context->m_filterFormatPremultiplied ? VG_TRUE : VG_FALSE, 
                     tilingMode, context->m_tileFillColor, context->m_filterChannelMask);
    }

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgLookup(VGImage dst, VGImage src, const VGubyte * redLUT, const VGubyte * greenLUT, const VGubyte * blueLUT, const VGubyte * alphaLUT, VGboolean outputLinear, VGboolean outputPremultiplied)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidImage(context, dst) || !isValidImage(context, src), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    Image* d = (Image*)dst;
    Image* s = (Image*)src;
    VG_IF_ERROR(isEglVgInUse(d) || isEglVgInUse(s), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(imageOverlaps(d, s), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!redLUT || !greenLUT || !blueLUT || !alphaLUT, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    unsigned int channelMask = context->m_filterChannelMask & (VG_RED|VG_GREEN|VG_BLUE|VG_ALPHA);    //undefined bits are ignored
    vg_lite_finish();
    lookup(d, s, (const VGubyte*)redLUT, (const VGubyte*)greenLUT, (const VGubyte*)blueLUT, (const VGubyte*)alphaLUT,
           outputLinear ? VG_TRUE : VG_FALSE, outputPremultiplied ? VG_TRUE : VG_FALSE, context->m_filterFormatLinear ? VG_TRUE : VG_FALSE,
           context->m_filterFormatPremultiplied ? VG_TRUE : VG_FALSE, channelMask);

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgLookupSingle(VGImage dst, VGImage src, const VGuint * lookupTable, VGImageChannel sourceChannel, VGboolean outputLinear, VGboolean outputPremultiplied)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidImage(context, dst) || !isValidImage(context, src), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);
    Image* d = (Image*)dst;
    Image* s = (Image*)src;
    VG_IF_ERROR(isEglVgInUse(d) || isEglVgInUse(s), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(imageOverlaps(d, s), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(!lookupTable || !isAligned(lookupTable,4), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    ColorDescriptor *desc = &s->m_desc;
    VG_ASSERT(isValidDescriptor(desc));
    //give an error if src is in rgb format and the source channel is not valid
    VG_IF_ERROR((!(desc->internalFormat & LUMINANCE) && !isAlphaOnly(desc)) && (sourceChannel != VG_RED && sourceChannel != VG_GREEN && sourceChannel != VG_BLUE && sourceChannel != VG_ALPHA), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    unsigned int channelMask = context->m_filterChannelMask & (VG_RED|VG_GREEN|VG_BLUE|VG_ALPHA);    //undefined bits are ignored
    vg_lite_finish();
    lookupSingle(d, s, (const VGuint*)lookupTable, sourceChannel, outputLinear ? VG_TRUE : VG_FALSE, outputPremultiplied ? VG_TRUE : VG_FALSE,
                 context->m_filterFormatLinear ? VG_TRUE : VG_FALSE, context->m_filterFormatPremultiplied ? VG_TRUE : VG_FALSE, channelMask); 

    VG_RETURN(VG_NO_RETVAL);
}

VGHardwareQueryResult VG_APIENTRY vgHardwareQuery(VGHardwareQueryType key, VGint setting)
{
    VG_GET_CONTEXT(VG_HARDWARE_UNACCELERATED);
    VG_IF_ERROR(key != VG_IMAGE_FORMAT_QUERY && key != VG_PATH_DATATYPE_QUERY, VG_ILLEGAL_ARGUMENT_ERROR, VG_HARDWARE_UNACCELERATED);
    VG_IF_ERROR(key == VG_IMAGE_FORMAT_QUERY && !isValidImageFormat(setting), VG_ILLEGAL_ARGUMENT_ERROR, VG_HARDWARE_UNACCELERATED);
    VG_IF_ERROR(key == VG_PATH_DATATYPE_QUERY && (setting < VG_PATH_DATATYPE_S_8 || setting > VG_PATH_DATATYPE_F), VG_ILLEGAL_ARGUMENT_ERROR, VG_HARDWARE_UNACCELERATED);
    VG_RETURN(VG_HARDWARE_UNACCELERATED);
}

const VGubyte * VG_APIENTRY vgGetString(VGStringID name)
{
    static const VGubyte vendor[] = "Vivante Corporation";
    static const VGubyte renderer[] = {"OpenVG 1.1 Implementation Oct, 2023"};
    static const VGubyte version[] = "1.1";
    static const VGubyte extensions[] = "";
    const VGubyte* r = NULL;
    VG_GET_CONTEXT(NULL);
    switch(name)
    {
    case VG_VENDOR:
        r = vendor;
        break;
    case VG_RENDERER:
        r = renderer;
        break;
    case VG_VERSION:
        r = version;
        break;
    case VG_EXTENSIONS:
        r = extensions;
        break;
    default:
        break;
    }
    VG_RETURN(r);
}

VGFont VG_APIENTRY vgCreateFont(VGint glyphCapacityHint)
{
    VG_GET_CONTEXT(VG_INVALID_HANDLE);
    VG_IF_ERROR(glyphCapacityHint < 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_INVALID_HANDLE);

    Font* font = (Font*)malloc(sizeof(Font));
    if (!font)
    {
        setError(VG_OUT_OF_MEMORY_ERROR);
        VG_RETURN(VG_INVALID_HANDLE);
    }
    memset(font, 0, sizeof(Font));

    VG_ASSERT(glyphCapacityHint >= 0);
    if (glyphCapacityHint == 0) {
        glyphCapacityHint = 1;
    }
    font->m_glyphs = (Glyph*)malloc(glyphCapacityHint * sizeof(Glyph));
    font->m_glyphsAllocSize = glyphCapacityHint;
    memset(font->m_glyphs, 0, glyphCapacityHint * sizeof(Glyph));
    if (!font->m_glyphs)
    {
        free(font);
        setError(VG_OUT_OF_MEMORY_ERROR);
        VG_RETURN(VG_INVALID_HANDLE);
    }
    font->m_glyphArraySize = glyphCapacityHint;
    font->object.type = OBJECTTYPE_FONT;
    addFontResource(context, font);

    VG_RETURN((VGFont)font);
}

void VG_APIENTRY vgDestroyFont(VGFont font)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidFont(context, font), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid font handle
    vg_lite_finish();
    removeFontResource(context, (Font*)font);
    if (font != VG_INVALID_HANDLE)
    {
        freeFontImpl((Font*)font);
    }
    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgSetGlyphToPath(VGFont font, VGuint glyphIndex, VGPath path, VGboolean isHinted, const VGfloat glyphOrigin[2], const VGfloat escapement[2])
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidFont(context, font), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid font handle
    VG_IF_ERROR((!isValidPath(context, path) && path != VG_INVALID_HANDLE), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid path handle
    VG_IF_ERROR(!glyphOrigin || !escapement || !isAligned(glyphOrigin,sizeof(VGfloat)) || !isAligned(escapement,sizeof(VGfloat)), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    Vector2 tmp_glyphorigin, tmp_escapment;
    tmp_glyphorigin.x = inputFloat(glyphOrigin[0]);
    tmp_glyphorigin.y = inputFloat(glyphOrigin[1]);
    tmp_escapment.x = inputFloat(escapement[0]);
    tmp_escapment.y = inputFloat(escapement[1]);
    if ((Path*)path != NULL) {
        Path* p = (Path*)path;
        vg_lite_path_t* vglpath = (vg_lite_path_t*)((Path*)path)->m_vglPath;
        if (p->m_pathChanged) {
            if (vglpath)
                vg_lite_clear_path(vglpath);

            VGuint data_num = 0;
            VGubyte* seg_ptr = p->m_segments;
            VGuint grid_size = _dataSize[p->m_datatype];
            for (VGint i = 0; i < p->m_numSegments; ++i)
                data_num += _commandSize[*seg_ptr++];
            VGubyte* dst_data = malloc(data_num * grid_size);
            uint8_t* vgl_opcodes = malloc(sizeof(uint8_t) * p->m_numSegments);

            VGuint bytes;
            seg_ptr = p->m_segments;
            VGubyte** src_data_list_ptr = p->m_data;
            VGubyte* dst_data_ptr = dst_data;
            uint8_t* vgl_opcodes_ptr = vgl_opcodes;
            for (VGint i = 0; i < p->m_numSegments; ++i) {
                *vgl_opcodes_ptr++ = _vglOpcodes[*seg_ptr];

                bytes = _commandSize[*seg_ptr++] * grid_size;
                memcpy(dst_data_ptr, *src_data_list_ptr, bytes);
                dst_data_ptr += bytes;
                src_data_list_ptr++;
            }

            vglpath->format = (vg_lite_format_t)p->m_datatype;
            vg_lite_append_path(vglpath, vgl_opcodes, (vg_lite_pointer)dst_data, (vg_lite_uint32_t)p->m_numSegments);

            free(dst_data);
            free(vgl_opcodes);

            p->m_pathChanged = VG_FALSE;
        }
    }
    setGlyphToPath(font, glyphIndex, path, isHinted ? VG_TRUE : VG_FALSE, &tmp_glyphorigin, &tmp_escapment);

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgSetGlyphToImage(VGFont font, VGuint glyphIndex, VGImage image, const VGfloat glyphOrigin[2], const VGfloat escapement[2])
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidFont(context, font), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid font handle
    if (image != VG_INVALID_HANDLE)
    {
        VG_IF_ERROR(!isValidImage(context, image), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid image handle
        VG_IF_ERROR(isEglVgInUse((Image*)image), VG_IMAGE_IN_USE_ERROR, VG_NO_RETVAL); //image in use
    }
    VG_IF_ERROR(!glyphOrigin || !escapement || !isAligned(glyphOrigin,sizeof(VGfloat)) || !isAligned(escapement,sizeof(VGfloat)), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    Vector2 tmp_origin, tmp_escapement;
    tmp_origin.x = inputFloat(glyphOrigin[0]);
    tmp_origin.y = inputFloat(glyphOrigin[1]);
    tmp_escapement.x = inputFloat(escapement[0]);
    tmp_escapement.y = inputFloat(escapement[1]);
    setGlyphToImage(font, glyphIndex, image, &tmp_origin, &tmp_escapement);

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgClearGlyph(VGFont font, VGuint glyphIndex)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidFont(context, font), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid font handle
    Font* f = (Font*)font;
    Glyph* g = findGlyph(f, glyphIndex);
    VG_IF_ERROR(!g, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);   //glyphIndex not defined
    clearGlyph(g);

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgDrawGlyph(VGFont font, VGuint glyphIndex, VGbitfield paintModes, VGboolean allowAutoHinting)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidFont(context, font), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid font handle
    VG_IF_ERROR(paintModes & ~(VG_FILL_PATH | VG_STROKE_PATH), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);    //invalid paint mode
    Font* f = (Font*)font;
    Glyph* g = findGlyph(f, glyphIndex);
    VG_IF_ERROR(!g, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);   //glyphIndex not defined
    //VG_UNREF(allowAutoHinting); //VG doesn't implement autohinting

    {
        if (paintModes)
        {
            VGboolean ret = VG_TRUE;
            Matrix3x3 userToSurfaceMatrix = context->m_glyphUserToSurface;
            Matrix3x3 n;
            Vector2 t;

            t.x = context->m_glyphOrigin.x - g->m_origin.x;
            t.y = context->m_glyphOrigin.y - g->m_origin.y;
            initMatrix3x3(&n, 1, 0, t.x, 0, 1, t.y, 0, 0, 1);
            multMatrix3x3(&userToSurfaceMatrix, &n);
            userToSurfaceMatrix.matrix[2][0] = 0; //force affinity
            userToSurfaceMatrix.matrix[2][1] = 0;
            userToSurfaceMatrix.matrix[2][2] = 1;

            if (g->m_image != VG_INVALID_HANDLE)
                ret = drawImage(context, g->m_image, userToSurfaceMatrix);
            else if (g->m_path != VG_INVALID_HANDLE)
                ret = drawPath(context, g->m_path, userToSurfaceMatrix, paintModes);
            if (!ret)
            {
                VG_RETURN(VG_NO_RETVAL);
            }
        }

        context->m_glyphOrigin.x += g->m_escapement.x;
        context->m_glyphOrigin.y += g->m_escapement.y;
        context->m_inputGlyphOrigin = context->m_glyphOrigin;
    }

    VG_RETURN(VG_NO_RETVAL);
}

void VG_APIENTRY vgDrawGlyphs(VGFont font, VGint glyphCount, const VGuint *glyphIndices, const VGfloat *adjustments_x, const VGfloat *adjustments_y, VGbitfield paintModes, VGboolean allowAutoHinting)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    VG_IF_ERROR(!isValidFont(context, font), VG_BAD_HANDLE_ERROR, VG_NO_RETVAL);    //invalid font handle
    VG_IF_ERROR(!glyphIndices || !isAligned(glyphIndices, sizeof(VGuint)) || glyphCount <= 0, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR((adjustments_x && !isAligned(adjustments_x, sizeof(VGfloat))) || (adjustments_y && !isAligned(adjustments_y, sizeof(VGfloat))), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);
    VG_IF_ERROR(paintModes & ~(VG_FILL_PATH | VG_STROKE_PATH), VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);    //invalid paint mode
    Font* f = (Font*)font;
    for (int i=0;i<glyphCount;i++)
    {
        Glyph* g = findGlyph(f, glyphIndices[i]);
        VG_IF_ERROR(!g, VG_ILLEGAL_ARGUMENT_ERROR, VG_NO_RETVAL);   //glyphIndex not defined
    }
    //VG_UNREF(allowAutoHinting); //VG doesn't implement autohinting

    {
        for (int i=0;i<glyphCount;i++)
        {
            Glyph* g = findGlyph(f, glyphIndices[i]);

            if (paintModes)
            {
                VGboolean ret = VG_TRUE;
                Matrix3x3 userToSurfaceMatrix = context->m_glyphUserToSurface;
                Matrix3x3 n;
                Vector2 t;

                t.x = context->m_glyphOrigin.x - g->m_origin.x;
                t.y = context->m_glyphOrigin.y - g->m_origin.y;
                initMatrix3x3(&n, 1, 0, t.x, 0, 1, t.y, 0, 0, 1 );

                multMatrix3x3(&userToSurfaceMatrix, &n);
                userToSurfaceMatrix.matrix[2][0] = 0; //force affinity
                userToSurfaceMatrix.matrix[2][1] = 0;
                userToSurfaceMatrix.matrix[2][2] = 1;

                if (g->m_image != VG_INVALID_HANDLE)
                    ret = drawImage(context, g->m_image, userToSurfaceMatrix);
                else if (g->m_path != VG_INVALID_HANDLE)
                    ret = drawPath(context, g->m_path, userToSurfaceMatrix, paintModes);
                if (!ret)
                {
                    VG_RETURN(VG_NO_RETVAL);
                }
            }

            context->m_glyphOrigin.x += g->m_escapement.x;
            context->m_glyphOrigin.y += g->m_escapement.y;
            if (adjustments_x)
                context->m_glyphOrigin.x += inputFloat(adjustments_x[i]);
            if (adjustments_y)
                context->m_glyphOrigin.y += inputFloat(adjustments_y[i]);
            context->m_inputGlyphOrigin = context->m_glyphOrigin;
        }
    }

    VG_RETURN(VG_NO_RETVAL);
}
