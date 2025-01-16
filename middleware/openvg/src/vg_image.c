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

#define VG_FLOAT_TO_UB(x)   ((VGubyte)((x)*255 + 0.5))

VGuint vgFormatTovgliteFormat(VGImageFormat format)
{
    switch (format)
    {
    case VG_sRGBX_8888:
        return VG_LITE_RGBX8888;
        break;
    case VG_sRGBA_8888:
    case VG_sRGBA_8888_PRE:
        return VG_LITE_RGBA8888;
        break;
    case VG_sRGB_565:
        return VG_LITE_RGB565;
        break;
    case VG_sRGBA_5551:
        return VG_LITE_RGBA5551;
        break;
    case VG_sRGBA_4444:
        return VG_LITE_RGBA4444;
        break;
    case VG_sL_8:
        return VG_LITE_L8;
        break;
    case VG_lRGBX_8888:
        return VG_LITE_RGBX8888;
        break;
    case VG_lRGBA_8888:
    case VG_lRGBA_8888_PRE:
        return VG_LITE_RGBA8888;
        break;
    case VG_lL_8:
        return VG_LITE_L8;
        break;
    case VG_A_8:
        return VG_LITE_A8;
        break;
    case VG_BW_1:
        return VG_LITE_INDEX_1;
        break;
    case VG_A_1:
        return VG_LITE_A4;
        break;
    case VG_A_4:
        return VG_LITE_A4;
        break;
    case VG_sXRGB_8888:
        return VG_LITE_XRGB8888;
        break;
    case VG_sARGB_8888:
        return VG_LITE_ARGB8888;
        break;
    case VG_sARGB_8888_PRE:
        return VG_LITE_ARGB8888;
        break;
    case VG_sARGB_1555:
        return VG_LITE_ARGB1555;
        break;
    case VG_sARGB_4444:
        return VG_LITE_ARGB4444;
        break;
    case VG_lXRGB_8888:
        return VG_LITE_XRGB8888;
        break;
    case VG_lARGB_8888:
        return VG_LITE_ARGB8888;
        break;
    case VG_lARGB_8888_PRE:
        return VG_LITE_ARGB8888;
        break;
    case VG_sBGRX_8888:
        return VG_LITE_BGRX8888;
        break;
    case VG_sBGRA_8888:
        return VG_LITE_BGRA8888;
        break;
    case VG_sBGRA_8888_PRE:
        return VG_LITE_BGRA8888;
        break;
    case VG_sBGR_565:
        return VG_LITE_BGR565;
        break;
    case VG_sBGRA_5551:
        return VG_LITE_BGRA5551;
        break;
    case VG_sBGRA_4444:
        return VG_LITE_BGRA4444;
        break;
    case VG_lBGRX_8888:
        return VG_LITE_BGRX8888;
        break;
    case VG_lBGRA_8888:
        return VG_LITE_BGRA8888;
        break;
    case VG_lBGRA_8888_PRE:
        return VG_LITE_BGRA8888;
        break;
    case VG_sXBGR_8888:
        return VG_LITE_XBGR8888;
        break;
    case VG_sABGR_8888:
        return VG_LITE_ABGR8888;
        break;
    case VG_sABGR_8888_PRE:
        return VG_LITE_ABGR8888;
        break;
    case VG_sABGR_1555:
        return VG_LITE_ARGB1555;
        break;
    case VG_sABGR_4444:
        return VG_LITE_ABGR4444;
        break;
    case VG_lXBGR_8888:
        return VG_LITE_XBGR8888;
        break;
    case VG_lABGR_8888:
        return VG_LITE_ABGR8888;
        break;
    case VG_lABGR_8888_PRE:
        return VG_LITE_ABGR8888;
        break;
    default:
        return 0xFFFF; // Invalid VGLite enum
    }
}

/*-------------------------------------------------------------------*//*!
* \brief    Creates a pixel format descriptor out of VGImageFormat
* \param
* \return
* \note
*//*-------------------------------------------------------------------*/

ColorDescriptor formatToDescriptor(VGImageFormat format)
{
    /* base formats
    VG_sRGBX_8888                               =  0,
    VG_sRGBA_8888                               =  1,
    VG_sRGBA_8888_PRE                           =  2,
    VG_sRGB_565                                 =  3,
    VG_sRGBA_5551                               =  4,
    VG_sRGBA_4444                               =  5,
    VG_sL_8                                     =  6,
    VG_lRGBX_8888                               =  7,
    VG_lRGBA_8888                               =  8,
    VG_lRGBA_8888_PRE                           =  9,
    VG_lL_8                                     = 10,
    VG_A_8                                      = 11,
    VG_BW_1                                     = 12,
    VG_A_1                                      = 13,
    VG_A_4                                      = 14,
    VG_sRGBX_8888_PRE                           = 15,
    VG_sRGB_565_PRE                             = 16,
    VG_sRGBA_5551_PRE                           = 17,
    VG_sRGBA_4444_PRE                           = 18,
    VG_lRGBX_8888_PRE                           = 19,
    VG_lRGB_565                                 = 20,
    VG_lRGB_565_PRE                             = 21,
    VG_lRGBA_5551                               = 22,
    VG_lRGBA_5551_PRE                           = 23,
    VG_lRGBA_4444                               = 24,
    VG_lRGBA_4444_PRE                           = 25,
    */
    static const int redBits[26] = { 8, 8, 8, 5, 5, 4, 0, 8, 8, 8, 0, 0, 0, 0, 0, 8, 5, 5, 4, 8, 5, 5, 5, 5, 4, 4 };
    static const int greenBits[26] = { 8, 8, 8, 6, 5, 4, 0, 8, 8, 8, 0, 0, 0, 0, 0, 8, 6, 5, 4, 8, 6, 6, 5, 5, 4, 4 };
    static const int blueBits[26] = { 8, 8, 8, 5, 5, 4, 0, 8, 8, 8, 0, 0, 0, 0, 0, 8, 5, 5, 4, 8, 5, 5, 5, 5, 4, 4 };
    static const int alphaBits[26] = { 0, 8, 8, 0, 1, 4, 0, 0, 8, 8, 0, 8, 0, 1, 4, 0, 0, 1, 4, 0, 0, 0, 1, 1, 4, 4 };
    static const int luminanceBits[26] = { 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    static const int redShifts[4 * 26] = { 16, 24, 24, 11, 11, 12, 0, 16, 24, 24, 0, 0, 0, 0, 0, 16, 11, 11, 12, 16, 11, 11, 11, 11, 12, 12,    //RGBA
                                                   16, 16, 16, 11, 10, 8,  0, 16, 16, 16, 0, 0, 0, 0, 0, 16, 11, 10, 8, 16, 11, 11, 10, 10, 8, 8,   //ARGB
                                                   8,  8,  8,  0,  1,  4,  0, 8,  8,  8,  0, 0, 0, 0, 0, 8, 0, 1, 4, 8, 0, 0, 1, 1, 4, 4,    //BGRA
                                                   0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };    //ABGR

    static const int greenShifts[4 * 26] = { 8, 16, 16, 5,  6,  8,  0, 8, 16, 16, 0, 0, 0, 0, 0, 8, 5, 6, 8, 8, 5, 5, 6, 6, 8, 8,    //RGBA
                                                     8,  8,  8, 5,  5,  4,  0, 8,  8,  8, 0, 0, 0, 0, 0, 8, 5, 5, 4, 8, 5, 5, 5, 5, 4, 4,   //ARGB
                                                     16, 16, 16, 5,  6,  8,  0, 16, 16, 16, 0, 0, 0, 0, 0, 16, 8, 6, 8, 16, 5, 5, 6, 6, 8, 8,    //BGRA
                                                     8,  8,  8,  5,  5,  4,  0, 8,  8,  8,  0, 0, 0, 0, 0, 8, 5, 5, 4, 8, 5, 5, 5, 5, 4, 4 };//ABGR

    static const int blueShifts[4 * 26] = { 0,  8,  8,  0,  1,  4,  0, 0,  8,  8,  0, 0, 0, 0, 0, 0, 0, 1, 4, 0, 0, 0, 1, 1, 4, 4,    //RGBA
                                                     0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //ARGB
                                                     24, 24, 24, 11, 11, 12, 0, 24, 24, 24, 0, 0, 0, 0, 0, 24, 11, 11, 12, 24, 11, 11, 10, 10, 8, 8,    //BGRA
                                                     16, 16, 16, 11, 10, 8,  0, 16, 16, 16, 0, 0, 0, 0, 0, 16, 11, 10, 8, 16, 11, 11, 10, 10, 8, 8 };//ABGR

    static const int alphaShifts[4 * 26] = { 0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //RGBA
                                                      0,  24, 24, 0,  15, 12, 0, 0,  24, 24, 0, 0, 0, 0, 0, 0, 0, 15, 12, 0, 0, 0, 15, 15, 12, 12,   //ARGB
                                                      0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //BGRA
                                                      0,  24, 24, 0,  15, 12, 0, 0,  24, 24, 0, 0, 0, 0, 0, 0, 0, 15, 12, 0, 0, 0, 15, 15, 12, 12 };//ABGR

    static const int bpps[26] = { 32, 32, 32, 16, 16, 16, 8, 32, 32, 32, 8, 8, 1, 1, 4, 32, 16, 16, 16, 32, 16, 16, 16, 16, 16, 16 };

    static const InternalFormat internalFormats[26] = { sRGBA, sRGBA, sRGBA_PRE, sRGBA, sRGBA, sRGBA, sLA, lRGBA, lRGBA, lRGBA_PRE, lLA, lRGBA, lLA, lRGBA, lRGBA, sRGBA_PRE, sRGBA_PRE, sRGBA_PRE, sRGBA_PRE, lRGBA_PRE, lRGBA, lRGBA_PRE, lRGBA, lRGBA_PRE, lRGBA, lRGBA_PRE };

    ColorDescriptor desc;
    VGint baseFormat = (VGint)format & 31;
    const int numBaseFormats = 26;
    int swizzleBits = ((int)format >> 6) & 3;

    memset(&desc, 0, sizeof(ColorDescriptor));
    VG_ASSERT(isValidImageFormat(format));
    VG_ASSERT(baseFormat >= 0 && baseFormat < numBaseFormats);

    desc.redBits = redBits[baseFormat];
    desc.greenBits = greenBits[baseFormat];
    desc.blueBits = blueBits[baseFormat];
    desc.alphaBits = alphaBits[baseFormat];
    desc.luminanceBits = luminanceBits[baseFormat];

    desc.redShift = redShifts[swizzleBits * numBaseFormats + baseFormat];
    desc.greenShift = greenShifts[swizzleBits * numBaseFormats + baseFormat];
    desc.blueShift = blueShifts[swizzleBits * numBaseFormats + baseFormat];
    desc.alphaShift = alphaShifts[swizzleBits * numBaseFormats + baseFormat];
    desc.luminanceShift = 0;    //always zero

    desc.format = format;
    desc.bitsPerPixel = bpps[baseFormat];
    desc.internalFormat = internalFormats[baseFormat];

    return desc;
}

VGboolean isAlphaOnly(ColorDescriptor* desc)
{
    return (desc->alphaBits && (desc->redBits + desc->greenBits + desc->blueBits + desc->luminanceBits) == 0) ? VG_TRUE : VG_FALSE;
}

VGboolean isValidDescriptor(ColorDescriptor *desc)
{
    //A valid descriptor has 1, 2, 4, 8, 16, or 32 bits per pixel, and either luminance or rgba channels, but not both.
    //Any of the rgba channels can be missing, and not all bits need to be used. Maximum channel bit depth is 8.
    VGint rb = desc->redBits;
    VGint gb = desc->greenBits;
    VGint bb = desc->blueBits;
    VGint ab = desc->alphaBits;
    VGint lb = desc->luminanceBits;
    VGint rs = desc->redShift;
    VGint gs = desc->greenShift;
    VGint bs = desc->blueShift;
    VGint as = desc->alphaShift;
    VGint ls = desc->luminanceShift;
    VGint bpp = desc->bitsPerPixel;

    int rgbaBits = rb + gb + bb + ab;
    if (rb < 0 || rb > 8 || rs < 0 || rs + rb > bpp || !(rb || !rs))
        return VG_FALSE;    //invalid channel description
    if (gb < 0 || gb > 8 || gs < 0 || gs + gb > bpp || !(gb || !gs))
        return VG_FALSE;    //invalid channel description
    if (bb < 0 || bb > 8 || bs < 0 || bs + bb > bpp || !(bb || !bs))
        return VG_FALSE;    //invalid channel description
    if (ab < 0 || ab > 8 || as < 0 || as + ab > bpp || !(ab || !as))
        return VG_FALSE;    //invalid channel description
    if (lb < 0 || lb > 8 || ls < 0 || ls + lb > bpp || !(lb || !ls))
        return VG_FALSE;    //invalid channel description

    if (rgbaBits && lb)
        return VG_FALSE;    //can't have both rgba and luminance
    if (!rgbaBits && !lb)
        return VG_FALSE;    //must have either rgba or luminance
    if (rgbaBits)
    {    //rgba
        if (rb + gb + bb == 0)
        {    //alpha only
            if (rs || gs || bs || as || ls)
                return VG_FALSE;    //wrong shifts (even alpha shift must be zero)
            if ((ab != 1 && ab != 2 && ab != 4 && ab != 8) || bpp != ab)
                return VG_FALSE;    //alpha size must be 1, 2, 4, or, 8, bpp must match
        }
        else
        {    //rgba
            if (rgbaBits > bpp)
                return VG_FALSE;    //bpp must be greater than or equal to the sum of rgba bits
            if (!(bpp == 32 || bpp == 16 || bpp == 8))
                return VG_FALSE;    //only 1, 2, and 4 byte formats are supported for rgba

            unsigned int rm = bitsToMask((unsigned int)rb, (unsigned int)rs);
            unsigned int gm = bitsToMask((unsigned int)gb, (unsigned int)gs);
            unsigned int bm = bitsToMask((unsigned int)bb, (unsigned int)bs);
            unsigned int am = bitsToMask((unsigned int)ab, (unsigned int)as);
            if ((rm & gm) || (rm & bm) || (rm & am) || (gm & bm) || (gm & am) || (bm & am))
                return VG_FALSE;    //channels overlap
        }
    }
    else
    {    //luminance
        if (rs || gs || bs || as || ls)
            return VG_FALSE;    //wrong shifts (even luminance shift must be zero)
        if (!(lb == 1 || lb == 8) || bpp != lb)
            return VG_FALSE;    //luminance size must be either 1 or 8, bpp must match
    }

    if (desc->format != -1)
    {
        if (!isValidImageFormat(desc->format))
            return VG_FALSE;    //invalid image format

        ColorDescriptor d = formatToDescriptor(desc->format);
        if (d.redBits != rb || d.greenBits != gb || d.blueBits != bb || d.alphaBits != ab || d.luminanceBits != lb ||
            d.redShift != rs || d.greenShift != gs || d.blueShift != bs || d.alphaShift != as || d.luminanceShift != ls ||
            d.bitsPerPixel != bpp)
            return VG_FALSE;    //if the descriptor has a VGImageFormat, it must match the bits, shifts, and bpp
    }

    if ((unsigned int)desc->internalFormat & ~(PREMULTIPLIED | NONLINEAR | LUMINANCE))
        return VG_FALSE;    //invalid internal format

    return VG_TRUE;
}

/*-------------------------------------------------------------------*//*!
* \brief    Constructs a blank image.
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

Image* createBlankImage(ColorDescriptor *desc, VGint width, VGint height, VGbitfield allowedQuality)
{
    VG_ASSERT(isValidDescriptor(desc));
    VG_ASSERT(width > 0 && height > 0);

    Image* im = (Image*)malloc(sizeof(Image));
    if (im)
    {
        memset(im, 0, sizeof(Image));
    }
    else
    {
        // Error out of memory.
        return NULL;
    }

    im->m_desc = *desc;
    im->m_width = width;
    im->m_height = height;
    im->m_allowedQuality = allowedQuality;
    im->object.type = OBJECTTYPE_IMAGE;

    vg_lite_buffer_t *imgbuf = (vg_lite_buffer_t*)malloc(sizeof(vg_lite_buffer_t));
    if (!imgbuf) {
        // Error out of memory.
        free(im);
        return NULL;
    }

    memset(imgbuf, 0, sizeof(vg_lite_buffer_t));
    imgbuf->width = im->m_width;
    imgbuf->height = im->m_height;
    imgbuf->format = (desc->format == -1) ? VG_sRGBA_8888 : desc->format;
    vg_lite_allocate(imgbuf);
    memset(imgbuf->memory, 0, imgbuf->stride * imgbuf->height);

    // Attach vg_lite_buffer structure to EGL im->m_vglbuf pointer
    im->m_stride = imgbuf->stride;
    im->m_vglbuf = (void*)imgbuf;
    im->m_data = (VGubyte*)imgbuf->memory;
    im->m_ownsData = VG_TRUE;
    return im;
}


/*-------------------------------------------------------------------*//*!
* \brief    Constructs an image that uses an external array for its data
*            storage.
* \param    
* \return    
* \note        this is meant for internal use to make blitting easier
*//*-------------------------------------------------------------------*/

Image* createExtDataImage(ColorDescriptor* desc, VGint width, VGint height, VGint stride, VGubyte* data)
{
    VG_ASSERT(isValidDescriptor(desc));
    VG_ASSERT(width > 0 && height > 0);
    VG_ASSERT(data);

    Image* im = (Image*)malloc(sizeof(Image));
    if (im)
    {
        memset(im, 0, sizeof(Image));
    }
    else
    {
        // Error out of memory.
        return NULL;
    }

    im->m_desc = *desc;
    im->m_width = width;
    im->m_height = height;
    im->m_stride = stride;
    im->m_data = data;

    return im;
}

/*-------------------------------------------------------------------*//*!
* \brief    Construcs a child image.
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

void createChildImage(Image* im, Image* parent, int x, int y, int width, int height)
{
    VG_ASSERT(parent);
    VG_ASSERT(x >= 0 && y >= 0 && width > 0 && height > 0);
    VG_ASSERT(VG_INT_ADDSATURATE(x,width) <= parent->m_width && VG_INT_ADDSATURATE(y,height) <= parent->m_height);    //child image must be contained in parent

    memset(im, 0, sizeof(Image));

    im->m_parent = parent;
    im->m_width = width;
    im->m_height = height;
    im->m_desc = parent->m_desc;
    VG_ASSERT(isValidDescriptor(&im->m_desc));
    im->m_allowedQuality = parent->m_allowedQuality;
    im->m_stride = parent->m_stride;
    im->m_data = parent->m_data;
    im->m_storageOffsetX = parent->m_storageOffsetX + x;
    im->m_storageOffsetY = parent->m_storageOffsetY + y;
    im->m_vglbuf = parent->m_vglbuf;

    //increase the reference and use count of the parent
    im->m_inUse++;
    parent->m_inUse++;
    parent->m_referenceCount++;
    im->m_ownsData = VG_FALSE;
    im->object.type = OBJECTTYPE_IMAGE;
}

/*-------------------------------------------------------------------*//*!
* \brief    Image destructor.
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

void destroyImage(Image** im, VGContext *context)
{
    if ((*im)->m_parent) {
        //decrease the reference and use count of the parent
        (*im)->m_inUse--; VG_ASSERT((*im)->m_inUse >= 0);
        (*im)->m_parent->m_inUse--;
        (*im)->m_parent->m_referenceCount--; VG_ASSERT((*im)->m_parent->m_referenceCount >= 0);
        /* Only the parent image is released recursively when no other children occupy it. */
        if ((*im)->m_parent->m_inUse <= 1) {
            destroyImage(&((*im)->m_parent), context);
        }
        if ((*im)->m_referenceCount == 0)
        {
            if ((*im)->m_ownsData && (*im)->m_vglbuf)
            {   // Free the VGLite buffer attached to im->m_vglbuf
                vg_lite_buffer_t* vglbuf = (vg_lite_buffer_t*)(*im)->m_vglbuf;
                if (vglbuf->address) {
                    vg_lite_free(vglbuf);
                    free(vglbuf);
                    (*im)->m_vglbuf = NULL;
                    (*im)->m_data = NULL;
                }
            }
            free((*im));
            (*im) = NULL;
        }
    }
    else
    {
        if ((*im)->m_referenceCount == 0 && (*im)->m_inUse == 0)
        {

            if ((*im)->m_ownsData && (*im)->m_vglbuf)
            {   // Free the VGLite buffer attached to im->m_vglbuf
                vg_lite_buffer_t* vglbuf = (vg_lite_buffer_t*)(*im)->m_vglbuf;
                if (vglbuf->address) {
                    vg_lite_free(vglbuf);
                    free(vglbuf);
                    (*im)->m_vglbuf = NULL;
                    (*im)->m_data = NULL;
                }
            }
            free((*im));
            (*im) = NULL;
        }
    }
}

void rectIntersect(VGRectangle* d, VGRectangle* r)
{
    if (d->width >= 0 && r->width >= 0 && d->height >= 0 && r->height >= 0)
    {
        int x1 = VG_INT_MIN(VG_INT_ADDSATURATE(d->x, d->width), VG_INT_ADDSATURATE(r->x, r->width));
        d->x = VG_INT_MAX(d->x, r->x);
        d->width = VG_INT_MAX(x1 - d->x, 0);

        int y1 = VG_INT_MIN(VG_INT_ADDSATURATE(d->y, d->height), VG_INT_ADDSATURATE(r->y, r->height));
        d->y = VG_INT_MAX(d->y, r->y);
        d->height = VG_INT_MAX(y1 - d->y, 0);
    }
    else
    {
        d->x = 0;
        d->y = 0;
        d->width = 0;
        d->height = 0;
    }
}

/*-------------------------------------------------------------------*//*!
* \brief    Returns true if the two images share pixels.
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

VGboolean imageOverlaps(Image* dst, Image* src)
{
    VG_ASSERT(src);

    if (dst->m_data != src->m_data)
        return VG_FALSE;    //images don't share data

    //check if the image storage regions overlap
    VGRectangle d, r;
    d.x = dst->m_storageOffsetX;
    d.y = dst->m_storageOffsetY;
    d.width = dst->m_width;
    d.height = dst->m_height;

    r.x = src->m_storageOffsetX;
    r.y = src->m_storageOffsetY;
    r.width = src->m_width;
    r.height = src->m_height;

    rectIntersect(&d, &r);

    if (!d.width || !d.height)
        return VG_FALSE;    //intersection is empty, images don't overlap

    return VG_TRUE;
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

Surface* createSurface(ColorDescriptor* desc, VGint width, VGint height, VGint numSamples)
{
    VG_ASSERT(width > 0 && height > 0 && numSamples > 0 && numSamples <= 32);

    Surface* surf = (Surface*)malloc(sizeof(Surface));

    if (surf)
    {
        surf->m_width = width;
        surf->m_height = height;
        surf->m_numSamples = numSamples;
        surf->m_referenceCount = 0;
        surf->object.type = OBJECTTYPE_MASK;

        surf->m_image = createBlankImage(desc, width * numSamples, height, 0);
        if (surf->m_image)
        {
            surf->m_image->m_referenceCount++;
        }
        else
        {
            // Out of memory
            free(surf);
            return NULL;
        }
    }
    else
    {
        // Out of memory
        return NULL;
    }

    return surf;
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

Surface* createImageSurface(Image* image)
{
    VG_ASSERT(image);

    Surface* surf = (Surface*)malloc(sizeof(Surface));
    if (surf)
    {
        surf->m_width = image->m_width;
        surf->m_height = image->m_height;
        surf->m_numSamples = 1;
        surf->m_referenceCount = 0;

        surf->m_image = image;
        surf->m_image->m_referenceCount++;
    }
    else
    {
        // Out of memory
        return NULL;
    }

    return surf;
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

Surface* createDataImgSurface(ColorDescriptor* desc, VGint width, VGint height, VGint stride, VGubyte* data)
{
    VG_ASSERT(width > 0 && height > 0);

    Surface* surf = (Surface*)malloc(sizeof(Surface));
    if (surf)
    {
        surf->m_width = width;
        surf->m_height = height;
        surf->m_numSamples = 1;
        surf->m_referenceCount = 0;

        surf->m_image = createExtDataImage(desc, width, height, stride, data);
        if (surf->m_image)
        {
            surf->m_image->m_referenceCount++;
        }
        else
        {
            // Out of memory
            free(surf);
            return NULL;
        }
    }
    else
    {
        // Out of memory
        return NULL;
    }

    return surf;
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

void destroySurface(Surface* surf)
{
    VG_ASSERT(surf->m_referenceCount == 0);

    surf->m_image->m_referenceCount--;
    if (!surf->m_image->m_referenceCount)
    {
        destroyImage(&(surf->m_image), NULL);
        surf->m_image = NULL;
    }

    free(surf);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

Drawable* createDrawable(ColorDescriptor* desc, VGint width, VGint height, VGint numSamples, VGint maskBits)
{
    VG_ASSERT(width > 0 && height > 0 && numSamples > 0 && numSamples <= 32);
    VG_ASSERT(maskBits == 0 || maskBits == 1 || maskBits == 4 || maskBits == 8);

    Drawable* draw = (Drawable*)malloc(sizeof(Drawable));
    if (!draw)
        return NULL;

    draw->m_referenceCount = 0;

    draw->m_color = createSurface(desc, width, height, numSamples);
    if (draw->m_color)
    {
        draw->m_color->m_referenceCount++;
    }
    else
    {
        // Out of memory
        free(draw);
        return NULL;
    }

    draw->m_mask = NULL;
    if (maskBits)
    {
        VGImageFormat mf = VG_A_8;
        ColorDescriptor md = formatToDescriptor(mf);
        draw->m_mask = createSurface(&md, width, height, numSamples);
        if (draw->m_mask)
        {
            draw->m_mask->m_referenceCount++;
        }
        else
        {
            // Out of memory
            destroySurface(draw->m_color);
            free(draw);
            return NULL;;
        }
        //draw->m_mask->clear(Color(1,1,1,1,Color::sRGBA), 0, 0, width, height);
    }

    return draw;
}

/*-------------------------------------------------------------------*//*!
* \brief
* \param
* \return
* \note
*//*-------------------------------------------------------------------*/

void clearSurface(Surface* surface, VGuint x, VGuint y, VGuint w, VGuint h)
{
    vg_lite_buffer_t* dstbuf = (vg_lite_buffer_t*)surface->m_image->m_vglbuf;
    dstbuf->width = surface->m_image->m_width;
    dstbuf->width = surface->m_image->m_height;
    dstbuf->stride = surface->m_image->m_stride;
    vg_lite_rectangle_t rect = { x, y, w, h };
    vg_lite_clear(dstbuf, &rect, 0xffffffff); //temporarily set to white, and class "Color" to do
    vg_lite_flush();
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

Drawable* createImageDrawable(Image* image, int maskBits)
{
    VG_ASSERT(maskBits == 0 || maskBits == 1 || maskBits == 4 || maskBits == 8);
    VG_ASSERT(image);

    Drawable* draw = (Drawable*)malloc(sizeof(Drawable));
    if (!draw)
        return NULL;

    draw->m_referenceCount = 0;

    draw->m_color = createImageSurface(image);
    if (draw->m_color)
    {
        draw->m_color->m_referenceCount++;
    }
    else
    {
        // Out of memory
        free(draw);
        return NULL;
    }

    draw->m_mask = NULL;
    if (maskBits)
    {
        VGImageFormat mf = VG_A_1;
        if (maskBits == 4)
            mf = VG_A_4;
        else if (maskBits == 8)
            mf = VG_A_8;
        ColorDescriptor md = formatToDescriptor(mf);
        draw->m_mask = createSurface(&md, image->m_width, image->m_height, 1);
        if (draw->m_mask)
        {
            draw->m_mask->m_referenceCount++;
        }
        else
        {
            // Out of memory
            destroySurface(draw->m_color);
            free(draw);
            return NULL;
        }
        //draw->m_mask->clear(Color(1,1,1,1,Color::sRGBA), 0, 0, width, height);
    }

    return draw;
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

Drawable* createDescDrawable(ColorDescriptor* desc, VGint width, VGint height, VGint stride, VGubyte* data, VGint maskBits)
{
    VG_ASSERT(width > 0 && height > 0);
    VG_ASSERT(maskBits == 0 || maskBits == 1 || maskBits == 4 || maskBits == 8);

    Drawable* draw = (Drawable*)malloc(sizeof(Drawable));
    if (!draw)
        return NULL;

    draw->m_referenceCount = 0;

    draw->m_color = createDataImgSurface(desc, width, height, stride, data);
    if (draw->m_color)
    {
        draw->m_color->m_referenceCount++;
    }
    else
    {
        free(draw);
        return NULL;
    }

    draw->m_mask = NULL;
    if (maskBits)
    {
        VGImageFormat mf = VG_A_1;
        if (maskBits == 4)
            mf = VG_A_4;
        else if (maskBits == 8)
            mf = VG_A_8;
        ColorDescriptor md = formatToDescriptor(mf);
        draw->m_mask = createSurface(&md, width, height, 1);
        if (draw->m_mask)
        {
            draw->m_mask->m_referenceCount++;
        }
        else
        {
            // Out of memory
            destroySurface(draw->m_color);
            free(draw);
            return NULL;
        }
        //draw->m_mask->clear(Color(1,1,1,1,Color::sRGBA), 0, 0, width, height);
    }

    return draw;
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

void destroyDrawable(Drawable* draw)
{
    VG_ASSERT(draw->m_referenceCount == 0);

    draw->m_color->m_referenceCount--; VG_ASSERT(draw->m_color->m_referenceCount >= 0);

    if (!draw->m_color->m_referenceCount)
        destroySurface(draw->m_color);

    if (draw->m_mask)
    {
        draw->m_mask->m_referenceCount--; VG_ASSERT(draw->m_mask->m_referenceCount >= 0);
        if (!draw->m_mask->m_referenceCount)
            destroySurface(draw->m_mask);
    }

    free(draw);
}

/*-------------------------------------------------------------------*//*!
* \brief    
* \param    
* \return    
* \note        
*//*-------------------------------------------------------------------*/

void resizeDrawable(Drawable* draw, VGint newWidth, VGint newHeight)
{
    Surface* oldcolor = draw->m_color;
    Surface* oldmask = draw->m_mask;

    //TODO check that image is not a proxy
    draw->m_color = createSurface(&draw->m_color->m_image->m_desc, newWidth, newHeight, draw->m_color->m_numSamples);
    if (draw->m_color)
        draw->m_color->m_referenceCount++;
    else
    {
        // out of memory
        return;
    }

    if (draw->m_mask)
    {
        draw->m_mask = createSurface(&draw->m_mask->m_image->m_desc, newWidth, newHeight, draw->m_mask->m_numSamples);
        if (draw->m_mask)
            draw->m_mask->m_referenceCount++;
        else
        {
            // Out of memory
            return;
        }
    }

    oldcolor->m_referenceCount--; VG_ASSERT(oldcolor->m_referenceCount >= 0);
    if (!oldcolor->m_referenceCount)
        destroySurface(oldcolor);
    if (oldmask)
    {
        oldmask->m_referenceCount--; VG_ASSERT(oldmask->m_referenceCount >= 0);
        if (!oldmask->m_referenceCount)
            destroySurface(oldmask);
    }
}

void initImage(Image* image, ColorDescriptor desc, VGuint width, VGuint height, VGbitfield allowedQuality)
{
    image->m_desc = desc;
    image->m_width = width;
    image->m_height = height;
    image->m_allowedQuality = allowedQuality;
    image->m_inUse = 0;
    image->m_stride = 0;
    image->m_vglbuf = NULL;
    image->m_referenceCount = 0;
    image->m_ownsData = VG_TRUE;
    image->m_parent = NULL;
    image->m_storageOffsetX = 0;
    image->m_storageOffsetY = 0;
    image->m_mipmapsValid = VG_FALSE;
    //image->m_mipmaps = NULL;

    VG_ASSERT(isValidDescriptor(&(image->m_desc)));
    VG_ASSERT(width > 0 && height > 0);

    image->m_stride = (image->m_width * desc.bitsPerPixel + 7) / 8;
    image->m_vglbuf = (VGubyte*)malloc(image->m_stride * image->m_height);
    memset(image->m_vglbuf, 0, image->m_stride * image->m_height);
}

void initSurface(Surface* surface, ColorDescriptor desc, VGuint width, VGuint height, VGuint numSamples)
{
    surface->m_width = width;
    surface->m_height = height;
    surface->m_numSamples = numSamples;
    surface->m_referenceCount = 0;
    surface->m_image = NULL;

    VG_ASSERT(width > 0 && height > 0 && numSamples > 0 && numSamples <= 32);

    surface->m_image = (Image*)malloc(sizeof(Image));
    initImage(surface->m_image, desc, width*numSamples, height, 0);
    surface->m_image->m_referenceCount++;
}

void initDrawable(Drawable* drawable, ColorDescriptor desc, VGuint width, VGuint height, VGuint numSamples, VGuint maskBits)
{
    drawable->m_referenceCount = 0;
    drawable->m_color = NULL;
    drawable->m_mask = NULL;

    VG_ASSERT(width > 0 && height > 0 && numSamples > 0 && numSamples <= 32);
    VG_ASSERT(maskBits == 0 || maskBits == 1 || maskBits == 4 || maskBits == 8);

    drawable->m_color = (Surface*)malloc(sizeof(Surface));
    initSurface(drawable->m_color, desc, width, height, numSamples);
    drawable->m_referenceCount++;
    if (maskBits)
    {
        VGImageFormat mf = VG_A_1;
        if (maskBits == 4)
            mf = VG_A_4;
        else if (maskBits == 8)
            mf = VG_A_8;

        drawable->m_mask = (Surface*)malloc(sizeof(Surface));
        initSurface(drawable->m_mask, formatToDescriptor(mf), width, height, numSamples);
        drawable->m_mask->m_referenceCount++;
        clearSurface(drawable->m_mask, 0, 0, width, height);
    }
}

static VGfloat vg_gamma(VGfloat c)
{
    if (c <= 0.00304f)
        c *= 12.92f;
    else
        c = 1.0556f * (VGfloat)pow(c, 1.0f / 2.4f) - 0.0556f;
    return c;
}

static VGfloat inv_vg_gamma(VGfloat c)
{
    if (c <= 0.03928f)
        c /= 12.92f;
    else
        c = (VGfloat)pow((c + 0.0556f) / 1.0556f, 2.4f);
    return c;
}

static VGfloat lRGBtoL(VGfloat r, VGfloat g, VGfloat b)
{
    return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

VGboolean isPremultiplied(Color* color)
{
    return (color->m_format & PREMULTIPLIED) ? VG_TRUE : VG_FALSE;
}

VGboolean isLuminance(Color* color)
{
    return (color->m_format & LUMINANCE) ? VG_TRUE : VG_FALSE;
}

VGboolean descriptorIsPremultiplied(const ColorDescriptor* dec)
{
    return (dec->internalFormat & PREMULTIPLIED) ? VG_TRUE : VG_FALSE;
}

VGboolean descriptorIsLuminance(const ColorDescriptor* dec)
{
    return (dec->internalFormat & LUMINANCE) ? VG_TRUE : VG_FALSE;
}

void colorPremultiply(Color* color)
{
    if (!(color->m_format & PREMULTIPLIED))
    {
        color->r *= color->a;
        color->g *= color->a;
        color->b *= color->a;
        color->m_format = (InternalFormat)(color->m_format | PREMULTIPLIED);
    }
}

void colorConvert(Color* color, InternalFormat outputFormat)
{
    if (color->m_format == outputFormat)
        return;

    if (isPremultiplied(color))
    {
        VGfloat ooa = (color->a != 0.0f) ? 1.0f / color->a : (VGfloat)0.0f;
        color->r *= ooa;
        color->g *= ooa;
        color->b *= ooa;
    }
#define shift                       3
    VGuint conversion = (color->m_format & (NONLINEAR | LUMINANCE)) | ((outputFormat & (NONLINEAR | LUMINANCE)) << shift);
    switch (conversion)
    {
        case lRGBA | (sRGBA << shift) : color->r = vg_gamma(color->r); color->g = vg_gamma(color->g); color->b = vg_gamma(color->b); break;
        case lRGBA | (lLA << shift) : color->r = color->g = color->b = lRGBtoL(color->r, color->g, color->b); break;
        case lRGBA | (sLA << shift) : color->r = color->g = color->b = vg_gamma(lRGBtoL(color->r, color->g, color->b)); break;
        case sRGBA | (lRGBA << shift) : color->r = inv_vg_gamma(color->r); color->g = inv_vg_gamma(color->g); color->b = inv_vg_gamma(color->b); break;
        case sRGBA | (lLA << shift) : color->r = color->g = color->b = lRGBtoL(inv_vg_gamma(color->r), inv_vg_gamma(color->g), inv_vg_gamma(color->b)); break;
        case sRGBA | (sLA << shift) : color->r = color->g = color->b = vg_gamma(lRGBtoL(inv_vg_gamma(color->r), inv_vg_gamma(color->g), inv_vg_gamma(color->b))); break;
        case lLA | (lRGBA << shift) : break;
        case lLA | (sRGBA << shift) : color->r = color->g = color->b = vg_gamma(color->r); break;
        case lLA | (sLA << shift) : color->r = color->g = color->b = vg_gamma(color->r); break;
        case sLA | (lRGBA << shift) : color->r = color->g = color->b = inv_vg_gamma(color->r); break;
        case sLA | (sRGBA << shift) : break;
        case sLA | (lLA << shift) : color->r = color->g = color->b = inv_vg_gamma(color->r); break;
        default: VG_ASSERT((color->m_format & (LUMINANCE | NONLINEAR)) == (outputFormat & (LUMINANCE | NONLINEAR))); break;
    }

    if (outputFormat & PREMULTIPLIED)
    {//premultiply
        color->r *= color->a;
        color->g *= color->a;
        color->b *= color->a;
    }
    color->m_format = outputFormat;
}

static InternalFormat getProcessingFormat(InternalFormat srcFormat, VGboolean filterFormatLinear, VGboolean filterFormatPremultiplied)
{
    InternalFormat procFormat = (InternalFormat)(srcFormat & ~LUMINANCE);
    if (filterFormatLinear)
        procFormat = (InternalFormat)(procFormat & ~NONLINEAR);
    else
        procFormat = (InternalFormat)(procFormat | NONLINEAR);

    if (filterFormatPremultiplied)
        procFormat = (InternalFormat)(procFormat | PREMULTIPLIED);
    else
        procFormat = (InternalFormat)(procFormat & ~PREMULTIPLIED);

    return procFormat;
}

static InternalFormat getLUTFormat(VGboolean outputLinear, VGboolean outputPremultiplied)
{
    InternalFormat lutFormat = lRGBA;
    if (outputLinear && outputPremultiplied)
        lutFormat = lRGBA_PRE;
    else if (!outputLinear && !outputPremultiplied)
        lutFormat = sRGBA;
    else if (!outputLinear && outputPremultiplied)
        lutFormat = sRGBA_PRE;

    return lutFormat;
}

static VGfloat intToColor(unsigned int i, unsigned int maxi)
{
    return (VGfloat)(i & maxi) / (VGfloat)maxi;
}

static VGuint colorToInt(VGfloat c, VGint maxc)
{
    return VG_INT_MIN(VG_INT_MAX((int)floor(c * (VGfloat)maxc + 0.5f), 0), maxc);
}

void colorUnpack(Color* color, unsigned int inputData, const ColorDescriptor* inputDesc)
{
    int rb = inputDesc->redBits;
    int gb = inputDesc->greenBits;
    int bb = inputDesc->blueBits;
    int ab = inputDesc->alphaBits;
    int lb = inputDesc->luminanceBits;
    int rs = inputDesc->redShift;
    int gs = inputDesc->greenShift;
    int bs = inputDesc->blueShift;
    int as = inputDesc->alphaShift;
    int ls = inputDesc->luminanceShift;

    if (inputDesc->format == OPENVG_lRGBX_8888 || inputDesc->format == OPENVG_sRGBX_8888 ||
        inputDesc->format == OPENVG_lRGBX_8888_PRE || inputDesc->format == OPENVG_sRGBX_8888_PRE)
    {
        ab = 8;
        lb = inputDesc->luminanceBits;
        rs = 24;
        gs = 16;
        bs = 8;
    }

    color->m_format = inputDesc->internalFormat;
    if (lb)
    {//luminance
        color->r = color->g = color->b = intToColor(inputData >> ls, (1 << lb) - 1);
        color->a = 1.0f;
    }
    else
    {//rgba
        color->r = rb ? intToColor(inputData >> rs, (1 << rb) - 1) : (VGfloat)1.0f;
        color->g = gb ? intToColor(inputData >> gs, (1 << gb) - 1) : (VGfloat)1.0f;
        color->b = bb ? intToColor(inputData >> bs, (1 << bb) - 1) : (VGfloat)1.0f;
        color->a = ab ? intToColor(inputData >> as, (1 << ab) - 1) : (VGfloat)1.0f;

        if (isPremultiplied(color))
        {//clamp premultiplied color to alpha to enforce consistency
            color->r = VG_MIN(color->r, color->a);
            color->g = VG_MIN(color->g, color->a);
            color->b = VG_MIN(color->b, color->a);
        }
    }
}

unsigned int colorPack(Color* color, const ColorDescriptor* outputDesc)
{
    int rb = outputDesc->redBits;
    int gb = outputDesc->greenBits;
    int bb = outputDesc->blueBits;
    int ab = outputDesc->alphaBits;
    int lb = outputDesc->luminanceBits;
    int rs = outputDesc->redShift;
    int gs = outputDesc->greenShift;
    int bs = outputDesc->blueShift;
    int as = outputDesc->alphaShift;
    int ls = outputDesc->luminanceShift;

    if (outputDesc->format == OPENVG_lRGBX_8888 || outputDesc->format == OPENVG_sRGBX_8888 ||
        outputDesc->format == OPENVG_lRGBX_8888_PRE || outputDesc->format == OPENVG_sRGBX_8888_PRE)
    {
        ab = 8;
        lb = outputDesc->luminanceBits;
        rs = 24;
        gs = 16;
        bs = 8;
    }

    if (lb)
    {//luminance
        VG_ASSERT(isLuminance(color));
        return colorToInt(color->r, (1 << lb) - 1) << ls;
    }
    else
    {//rgb
        VG_ASSERT(!isLuminance(color));
        unsigned int cr = rb ? colorToInt(color->r, (1 << rb) - 1) : 0;
        unsigned int cg = gb ? colorToInt(color->g, (1 << gb) - 1) : 0;
        unsigned int cb = bb ? colorToInt(color->b, (1 << bb) - 1) : 0;
        unsigned int ca = ab ? colorToInt(color->a, (1 << ab) - 1) : 0;
        return (cr << rs) | (cg << gs) | (cb << bs) | (ca << as);
    }
}

Color imageReadPixel(const Image* img, VGint x, VGint y)
{
    VG_ASSERT(img->m_data);
    VG_ASSERT(x >= 0 && x < img->m_width);
    VG_ASSERT(y >= 0 && y < img->m_height);
    VG_ASSERT(img->m_referenceCount > 0);
    x += img->m_storageOffsetX;
    y += img->m_storageOffsetY;

    unsigned int p = 0;
    VGubyte* scanline = img->m_data + y * img->m_stride;
    switch (img->m_desc.bitsPerPixel)
    {
        case 32:
        {
            VGuint* s = (((VGuint*)scanline) + x);
            p = (unsigned int)*s;
            break;
        }

        case 16:
        {
            VGushort* s = ((VGushort*)scanline) + x;
            p = (unsigned int)*s;
            break;
        }

        case 8:
        {
            VGubyte* s = ((VGubyte*)scanline) + x;
            p = (unsigned int)*s;
            break;
        }

        case 4:
        {
            VGubyte* s = ((VGubyte*)scanline) + (x >> 1);
            p = (unsigned int)(*s >> ((x & 1) << 2)) & 0xf;
            break;
        }

        case 2:
        {
            VGubyte* s = ((VGubyte*)scanline) + (x >> 2);
            p = (unsigned int)(*s >> ((x & 3) << 1)) & 0x3;
            break;
        }

        default:
        {
            VG_ASSERT(img->m_desc.bitsPerPixel == 1);
            VGubyte* s = ((VGubyte*)scanline) + (x >> 3);
            p = (unsigned int)(*s >> (x & 7)) & 0x1;
            break;
        }
    }
    Color c;
    colorUnpack(&c, p, &img->m_desc);
    return c;
}
void colorClamp(Color* color) 
{
    color->a = VG_CLAMP(color->a, 0.0f, 1.0f);
    VGfloat u = (color->m_format & PREMULTIPLIED) ? color->a : (VGfloat)1.0f;
    color->r = VG_CLAMP(color->r, 0.0f, u);
    color->g = VG_CLAMP(color->g, 0.0f, u);
    color->b = VG_CLAMP(color->b, 0.0f, u);
}

void colorUnpremultiply(Color* color) 
{ 
    if (color->m_format & PREMULTIPLIED)
    { 
        VGfloat ooa = (color->a != 0.0f) ? 1.0f / color->a : (VGfloat)0.0f;
        color->r *= ooa; 
        color->g *= ooa; 
        color->b *= ooa;
        color->m_format = (InternalFormat)(color->m_format & ~PREMULTIPLIED);
    } 
}

void imageWritePixel(Image* image, int x, int y, Color* c)
{
    VG_ASSERT(image->m_data);
    VG_ASSERT(x >= 0 && x < image->m_width);
    VG_ASSERT(y >= 0 && y < image->m_height);
    VG_ASSERT(image->m_referenceCount > 0);
    VG_ASSERT(c->m_format == image->m_desc.internalFormat);
    x += image->m_storageOffsetX;
    y += image->m_storageOffsetY;

    unsigned int p = colorPack(c, &image->m_desc);
    VGubyte* scanline = image->m_data + y * image->m_stride;
    switch (image->m_desc.bitsPerPixel)
    {
    case 32:
    {
        VGuint* s = ((VGuint*)scanline) + x;
        *s = (VGuint)p;
        break;
    }

    case 16:
    {
        VGushort* s = ((VGushort*)scanline) + x;
        *s = (VGushort)p;
        break;
    }

    case 8:
    {
        VGbyte* s = ((VGbyte*)scanline) + x;
        *s = (VGbyte)p;
        break;
    }
    case 4:
    {
        VGubyte* s = ((VGubyte*)scanline) + (x >> 1);
        *s = (VGubyte)((p << ((x & 1) << 2)) | ((unsigned int)*s & ~(0xf << ((x & 1) << 2))));
        break;
    }

    case 2:
    {
        VGubyte* s = ((VGubyte*)scanline) + (x >> 2);
        *s = (VGubyte)((p << ((x & 3) << 1)) | ((unsigned int)*s & ~(0x3 << ((x & 3) << 1))));
        break;
    }

    default:
    {
        VG_ASSERT(image->m_desc.bitsPerPixel == 1);
        VGubyte* s = ((VGubyte*)scanline) + (x >> 3);
        *s = (VGubyte)((p << (x & 7)) | ((unsigned int)*s & ~(0x1 << (x & 7))));
        break;
    }
    }
    image->m_mipmapsValid = VG_FALSE;
}

void writeFilteredPixel(Image* dst, int i, int j, Color* color, VGbitfield channelMask)
{
    Color* f = color;
    colorClamp(color);//vgColorMatrix and vgLookups can produce colors that exceed alpha or [0,1] range

    //step 2: color space conversion
    colorConvert(f, (InternalFormat)(dst->m_desc.internalFormat & (NONLINEAR | LUMINANCE)));

    //step 3: read the destination color and convert it to nonpremultiplied
    Color d = imageReadPixel(dst, i, j);
    colorUnpremultiply(&d);
    VG_ASSERT(d.m_format == f->m_format);

    //step 4: replace the destination channels specified by the channelMask (channelmask is ignored for luminance formats)
    if (!descriptorIsLuminance(&dst->m_desc))
    {   //rgba format => use channelmask
        if (channelMask & VG_RED)
            d.r = f->r;
        if (channelMask & VG_GREEN)
            d.g = f->g;
        if (channelMask & VG_BLUE)
            d.b = f->b;
        if (channelMask & VG_ALPHA)
            d.a = f->a;
    }
    else
    {
        memcpy(&d, f, sizeof(d));
    }

    //step 5: if destination is premultiplied, convert to premultiplied format
    if (descriptorIsPremultiplied(&dst->m_desc))
        colorPremultiply(&d);
    //write the color to destination
    imageWritePixel(dst, i, j, &d);
}

void lookup(Image* dst, const Image* src, const VGubyte* redLUT, const VGubyte* greenLUT, const VGubyte* blueLUT, const VGubyte* alphaLUT, VGboolean outputLinear, VGboolean outputPremultiplied, VGboolean filterFormatLinear, VGboolean filterFormatPremultiplied, VGbitfield channelMask)
{
    VG_ASSERT(src->m_data);
    VG_ASSERT(dst->m_data);
    VG_ASSERT(redLUT && greenLUT && blueLUT && alphaLUT);
    VG_ASSERT(dst->m_referenceCount > 0 && src->m_referenceCount > 0);

    int w = VG_INT_MIN(dst->m_width, src->m_width);
    int h = VG_INT_MIN(dst->m_height, src->m_height);
    VG_ASSERT(w > 0 && h > 0);
    InternalFormat lutFormat = getLUTFormat(outputLinear, outputPremultiplied);
    InternalFormat procFormat;

    if (src->m_desc.format == VG_sRGBX_8888 || src->m_desc.format == VG_lRGBX_8888) {
        procFormat = getProcessingFormat(src->m_desc.internalFormat, filterFormatLinear, 0);
    }
    else {
        procFormat = getProcessingFormat(src->m_desc.internalFormat, filterFormatLinear, filterFormatPremultiplied);
    }

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            Color s = imageReadPixel(src, i, j);
            colorConvert(&s, procFormat);

            Color d;
            d.r = d.g = d.b = d.a = 0;
            d.m_format = lutFormat;
            VG_ASSERT(d.m_format == lRGBA || d.m_format == sRGBA || d.m_format == lRGBA_PRE || 
                      d.m_format == sRGBA_PRE || d.m_format == lLA || d.m_format == sLA || 
                      d.m_format == lLA_PRE || d.m_format == sLA_PRE);
            d.r = intToColor(  redLUT[colorToInt(s.r, 255)], 255);
            d.g = intToColor(greenLUT[colorToInt(s.g, 255)], 255);
            d.b = intToColor( blueLUT[colorToInt(s.b, 255)], 255);
            d.a = intToColor(alphaLUT[colorToInt(s.a, 255)], 255);
            
            writeFilteredPixel(dst, i, j, &d, channelMask);
        }
    }
}

void lookupSingle(Image* dst, const Image* src, const VGuint* lookupTable, VGImageChannel sourceChannel, VGboolean outputLinear, VGboolean outputPremultiplied, VGboolean filterFormatLinear, VGboolean filterFormatPremultiplied, VGbitfield channelMask)
{
    VG_ASSERT(src->m_data);//source exists
    VG_ASSERT(dst->m_data);//destination exists
    VG_ASSERT(lookupTable);
    VG_ASSERT(dst->m_referenceCount > 0 && src->m_referenceCount > 0);

    //the area to be written is an intersection of source and destination image areas.
    //lower-left corners of the images are aligned.
    int w = VG_INT_MIN(dst->m_width, src->m_width);
    int h = VG_INT_MIN(dst->m_height, src->m_height);
    VG_ASSERT(w > 0 && h > 0);
    InternalFormat lutFormat = getLUTFormat(outputLinear, outputPremultiplied);
    InternalFormat procFormat;

    if (descriptorIsLuminance(&src->m_desc))
        sourceChannel = VG_RED;
    else if (src->m_desc.redBits + src->m_desc.greenBits + src->m_desc.blueBits == 0)
    {
        VG_ASSERT(src->m_desc.alphaBits);
        sourceChannel = VG_ALPHA;
    }

    if (src->m_desc.format == VG_sRGBX_8888 || src->m_desc.format == VG_lRGBX_8888) {
        procFormat = getProcessingFormat(src->m_desc.internalFormat, filterFormatLinear, 0);
    }
    else {
        procFormat = getProcessingFormat(src->m_desc.internalFormat, filterFormatLinear, filterFormatPremultiplied);
    }

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            Color s = imageReadPixel(src, i, j);//convert to RGBA [0,1]
            colorConvert(&s, procFormat);
            int e;
            switch (sourceChannel)
            {
            case VG_RED:
                e = colorToInt(s.r, 255);
                break;
            case VG_GREEN:
                e = colorToInt(s.g, 255);
                break;
            case VG_BLUE:
                e = colorToInt(s.b, 255);
                break;
            default:
                VG_ASSERT(sourceChannel == VG_ALPHA);
                e = colorToInt(s.a, 255);
                break;
            }

            VGuint l = ((const VGuint*)lookupTable)[e];
            Color d;
            d.r = d.g = d.b = d.a = 0;
            d.m_format = lutFormat;
            d.r = intToColor((l >> 24), 255);
            d.g = intToColor((l >> 16), 255);
            d.b = intToColor((l >> 8), 255);
            d.a = intToColor((l), 255);

            writeFilteredPixel(dst, i, j, &d, channelMask);
        }
    }
}

static Color readTiledPixel(int x, int y, int w, int h, VGTilingMode tilingMode, const Color* image, const Color* edge)
{
    Color s;
    if (x < 0 || x >= w || y < 0 || y >= h)
    {//apply tiling mode
        switch (tilingMode)
        {
        case VG_TILE_FILL:
            s.r = edge->r;
            s.g = edge->g;
            s.b = edge->b;
            s.a = edge->a;
            s.m_format = edge->m_format;
            break;
        case VG_TILE_PAD:
            x = VG_INT_MIN(VG_INT_MAX(x, 0), w - 1);
            y = VG_INT_MIN(VG_INT_MAX(y, 0), h - 1);
            VG_ASSERT(x >= 0 && x < w&& y >= 0 && y < h);
            s.r = image[y * w + x].r;
            s.g = image[y * w + x].g;
            s.b = image[y * w + x].b;
            s.a = image[y * w + x].a;
            s.m_format = image[y * w + x].m_format;
            break;
        case VG_TILE_REPEAT:
            x = VG_INT_MOD(x, w);
            y = VG_INT_MOD(y, h);
            VG_ASSERT(x >= 0 && x < w&& y >= 0 && y < h);
            s.r = image[y * w + x].r;
            s.g = image[y * w + x].g;
            s.b = image[y * w + x].b;
            s.a = image[y * w + x].a;
            s.m_format = image[y * w + x].m_format;
            break;
        default:
            VG_ASSERT(tilingMode == VG_TILE_REFLECT);
            x = VG_INT_MOD(x, w * 2);
            y = VG_INT_MOD(y, h * 2);
            if (x >= w) x = w * 2 - 1 - x;
            if (y >= h) y = h * 2 - 1 - y;
            VG_ASSERT(x >= 0 && x < w&& y >= 0 && y < h);
            s.r = image[y * w + x].r;
            s.g = image[y * w + x].g;
            s.b = image[y * w + x].b;
            s.a = image[y * w + x].a;
            s.m_format = image[y * w + x].m_format;
            break;
        }
    }
    else
    {
        VG_ASSERT(x >= 0 && x < w&& y >= 0 && y < h);
        s.r = image[y * w + x].r;
        s.g = image[y * w + x].g;
        s.b = image[y * w + x].b;
        s.a = image[y * w + x].a;
        s.m_format = image[y * w + x].m_format;
    }
    return s;
}
void gaussianBlur(VGImage dst, VGImage src, VGfloat stdDeviationX, VGfloat stdDeviationY, VGboolean filterFormatLinear, VGboolean filterFormatPremultiplied, VGTilingMode tilingMode, Color tileFillColor, VGbitfield filterChannelmask)
{
    Image* dstImage = (Image*)dst;
    Image* srcImage = (Image*)src;
    VGint height = srcImage->m_height;
    VGint width = srcImage->m_width;
    VGint x, y, yk, xk;
    VGint halfKernelX, halfKernelY, kernelWidth, kernelHeight;
    VGfloat scaleX, scaleY, expScaleX, expScaleY;
    VGfloat kernelX[131];
    VGfloat kernelY[131];
    VGfloat kernelValue;
    InternalFormat procFormat;
    Color* tmp;
    Color* edge = &tileFillColor;

    if (srcImage->m_desc.format == VG_sRGBX_8888 || srcImage->m_desc.format == VG_lRGBX_8888) {
        procFormat = getProcessingFormat(srcImage->m_desc.internalFormat, filterFormatLinear, 0);
    }
    else {
        procFormat = getProcessingFormat(srcImage->m_desc.internalFormat, filterFormatLinear, filterFormatPremultiplied);
    }

    colorClamp(edge);
    colorConvert(edge, procFormat);

    /* Determine kernel parameters. */
    expScaleX = -1.0f / (2.0f * stdDeviationX * stdDeviationX);
    expScaleY = -1.0f / (2.0f * stdDeviationY * stdDeviationY);

    halfKernelX = (VGuint)(stdDeviationX * 4.0f + 1.0f);
    halfKernelY = (VGuint)(stdDeviationY * 4.0f + 1.0f);

    kernelWidth = halfKernelX * 2 + 1;
    kernelHeight = halfKernelY * 2 + 1;

    scaleX = 0.0f;
    for (xk = 0; xk < kernelWidth; xk++)
    {
        x = xk - halfKernelX;
        kernelX[xk] = (VGfloat)exp(expScaleX * x * x);
        scaleX += kernelX[xk];
    }

    scaleY = 0.0f;
    for (yk = 0; yk < kernelHeight; yk++)
    {
        y = yk - halfKernelY;
        kernelY[yk] = (VGfloat)exp(expScaleY * y * y);
        scaleY += kernelY[yk];
    }

    scaleX = 1.0f / scaleX;
    scaleY = 1.0f / scaleY;

    for (xk = 0; xk < kernelWidth; xk++)
    {
        kernelX[xk] *= scaleX;
    }

    for (yk = 0; yk < kernelHeight; yk++)
    {
        kernelY[yk] *= scaleY;
    }

    tmp = (Color*)malloc(width * height * sizeof(Color));
    memset(tmp, 0, width * height * sizeof(Color));

    //copy source region to tmp and do conversion
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Color s = imageReadPixel(srcImage, i, j);
            colorConvert(&s, procFormat);

            tmp[j * width + i].r = s.r;
            tmp[j * width + i].g = s.g;
            tmp[j * width + i].b = s.b;
            tmp[j * width + i].a = s.a;
        }
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            Color sum;
            sum.r = sum.g = sum.b = sum.a = 0;
            sum.m_format = procFormat;

            for (int kj = 0; kj < kernelHeight; kj++)
            {
                for (int ki = 0; ki < kernelWidth; ki++)
                {
                    int x = i + ki - halfKernelX;
                    int y = j + kj - halfKernelY;
                    Color s = readTiledPixel(x, y, width, height, tilingMode, tmp, edge);
                    Color tmpcolor = {0};

                    kernelValue = kernelX[ki] * kernelY[kj];
                    tmpcolor.r += kernelValue * s.r;
                    tmpcolor.g += kernelValue * s.g;
                    tmpcolor.b += kernelValue * s.b;
                    tmpcolor.a += kernelValue * s.a;
                    tmpcolor.m_format = s.m_format;

                    sum.r += tmpcolor.r;
                    sum.g += tmpcolor.g;
                    sum.b += tmpcolor.b;
                    sum.a += tmpcolor.a;
                }
            }

            if (i < dstImage->m_width && j < dstImage->m_height)
                writeFilteredPixel(dstImage, i, j, &sum, filterChannelmask);
        }
    }

    free(tmp);
}

void colorMatrix(const VGfloat* matrix, Image* src, Image* dst, VGboolean filterFormatLinear, VGboolean filterFormatPremultiplied, VGbitfield channelMask)
{
    vg_lite_int32_t x, y;
    InternalFormat procFormat = getProcessingFormat(src->m_desc.internalFormat, filterFormatLinear, filterFormatPremultiplied);

    for (y = 0; y < src->m_height; y++)
    {
        for (x = 0; x < src->m_width; x++)
        {
            Color s, sum;
            sum.m_format = procFormat;
            s = imageReadPixel(src, x, y);
            colorConvert(&s, procFormat);

            /* Compute the target pixel. */
            sum.r = matrix[0] * s.r + matrix[4] * s.g + matrix[8] * s.b + matrix[12] * s.a + matrix[16];
            sum.g = matrix[1] * s.r + matrix[5] * s.g + matrix[9] * s.b + matrix[13] * s.a + matrix[17];
            sum.b = matrix[2] * s.r + matrix[6] * s.g + matrix[10] * s.b + matrix[14] * s.a + matrix[18];
            sum.a = matrix[3] * s.r + matrix[7] * s.g + matrix[11] * s.b + matrix[15] * s.a + matrix[19];

            if (x < dst->m_width && y < dst->m_height)
                writeFilteredPixel(dst, x, y, &sum, channelMask);
        }
    }
}

void convolve(Image* dst, Image* src, int kernelWidth, int kernelHeight, int shiftX, int shiftY, const VGshort* kernel, VGfloat scale, VGfloat bias, VGTilingMode tilingMode, Color* edgeFillColor, VGboolean filterFormatLinear, VGboolean filterFormatPremultiplied, VGbitfield channelMask)
{
    VG_ASSERT(src->m_data);//source exists
    VG_ASSERT(dst->m_data);//destination exists
    VG_ASSERT(kernel && kernelWidth > 0 && kernelHeight > 0);
    VG_ASSERT(dst->m_referenceCount > 0 && src->m_referenceCount > 0);

    //the area to be written is an intersection of source and destination image areas.
    //lower-left corners of the images are aligned.
    int w = VG_INT_MIN(dst->m_width, src->m_width);
    int h = VG_INT_MIN(dst->m_height, src->m_height);
    VG_ASSERT(w > 0 && h > 0);
    InternalFormat procFormat;

    if (src->m_desc.format == VG_sRGBX_8888 || src->m_desc.format == VG_lRGBX_8888) {
        procFormat = getProcessingFormat(src->m_desc.internalFormat, filterFormatLinear, 0);
    }
    else {
        procFormat = getProcessingFormat(src->m_desc.internalFormat, filterFormatLinear, filterFormatPremultiplied);
    }


    Color* edge = edgeFillColor;
    colorClamp(edge);
    colorConvert(edge, procFormat);

    Color* tmp = (Color*)malloc(src->m_width * src->m_height * sizeof(Color));
    memset(tmp, 0, src->m_width * src->m_height * sizeof(Color));

    //copy source region to tmp and do conversion
    for (int j = 0; j < src->m_height; j++)
    {
        for (int i = 0; i < src->m_width; i++)
        {
            Color s = imageReadPixel(src, i, j);
            colorConvert(&s, procFormat);

            tmp[j * src->m_width + i].r = s.r;
            tmp[j * src->m_width + i].g = s.g;
            tmp[j * src->m_width + i].b = s.b;
            tmp[j * src->m_width + i].a = s.a;
        }
    }

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            Color sum;
            sum.r = sum.g = sum.b = sum.a = 0;
            sum.m_format = procFormat;

            for (int kj = 0; kj < kernelHeight; kj++)
            {
                for (int ki = 0; ki < kernelWidth; ki++)
                {
                    int x = i + ki - shiftX;
                    int y = j + kj - shiftY;
                    Color s = readTiledPixel(x, y, src->m_width, src->m_height, tilingMode, tmp, edge);

                    int kx = kernelWidth - ki - 1;
                    int ky = kernelHeight - kj - 1;
                    VG_ASSERT(kx >= 0 && kx < kernelWidth&& ky >= 0 && ky < kernelHeight);
                    
                    Color tmpcolor;
                    tmpcolor.r = s.r * (VGfloat)kernel[kx * kernelHeight + ky];
                    tmpcolor.g = s.g * (VGfloat)kernel[kx * kernelHeight + ky];
                    tmpcolor.b = s.b * (VGfloat)kernel[kx * kernelHeight + ky];
                    tmpcolor.a = s.a * (VGfloat)kernel[kx * kernelHeight + ky];
                    tmpcolor.m_format = s.m_format;

                    sum.r += tmpcolor.r;
                    sum.g += tmpcolor.g;
                    sum.b += tmpcolor.b;
                    sum.a += tmpcolor.a;
                }
            }

            sum.r *= scale;
            sum.g *= scale;
            sum.b *= scale;
            sum.a *= scale;
            sum.r += bias;
            sum.g += bias;
            sum.b += bias;
            sum.a += bias;

            writeFilteredPixel(dst, i, j, &sum, channelMask);
        }
    }

    free(tmp);
}

void separableConvolve(Image* dst, Image* src, int kernelWidth, int kernelHeight, int shiftX, int shiftY, const VGshort* kernelX, const VGshort* kernelY, VGfloat scale, VGfloat bias, VGTilingMode tilingMode, Color* edgeFillColor, VGboolean filterFormatLinear, VGboolean filterFormatPremultiplied, VGbitfield channelMask)
{
    VG_ASSERT(src->m_data);//source exists
    VG_ASSERT(dst->m_data);//destination exists
    VG_ASSERT(kernelX && kernelY && kernelWidth > 0 && kernelHeight > 0);
    VG_ASSERT(dst->m_referenceCount > 0 && src->m_referenceCount > 0);

    //the area to be written is an intersection of source and destination image areas.
    //lower-left corners of the images are aligned.
    int w = VG_INT_MIN(dst->m_width, src->m_width);
    int h = VG_INT_MIN(dst->m_height, src->m_height);
    VG_ASSERT(w > 0 && h > 0);
    InternalFormat procFormat;

    if (src->m_desc.format == VG_sRGBX_8888 || src->m_desc.format == VG_lRGBX_8888) {
        procFormat = getProcessingFormat(src->m_desc.internalFormat, filterFormatLinear, 0);
    }
    else {
        procFormat = getProcessingFormat(src->m_desc.internalFormat, filterFormatLinear, filterFormatPremultiplied);
    }

    Color* edge = edgeFillColor;
    colorClamp(edge);
    colorConvert(edge, procFormat);

    Color* tmp = (Color*)malloc(src->m_width * src->m_height * sizeof(Color));
    memset(tmp, 0, src->m_width * src->m_height * sizeof(Color));

    //copy source region to tmp and do conversion
    for (int j = 0; j < src->m_height; j++)
    {
        for (int i = 0; i < src->m_width; i++)
        {
            Color s = imageReadPixel(src, i, j);
            colorConvert(&s, procFormat);
            tmp [j * src->m_width + i].r = s.r;
            tmp [j * src->m_width + i].g = s.g;
            tmp [j * src->m_width + i].b = s.b;
            tmp [j * src->m_width + i].a = s.a;
        }
    }

    Color* tmp2 = (Color*)malloc(w * src->m_height * sizeof(Color));
    memset(tmp2, 0, w * src->m_height * sizeof(Color));

    for (int j = 0; j < src->m_height; j++)
    {
        for (int i = 0; i < w; i++)
        {
            Color sum;
            sum.a = sum.r = sum.g = sum.b = 0;
            sum.m_format = procFormat;
            for (int ki = 0; ki < kernelWidth; ki++)
            {
                int x = i + ki - shiftX;
                Color s = readTiledPixel(x, j, src->m_width, src->m_height, tilingMode, tmp, edge);

                int kx = kernelWidth - ki - 1;
                VG_ASSERT(kx >= 0 && kx < kernelWidth);

                Color tmpcolor;
                tmpcolor.r = s.r * (VGfloat)kernelX[kx];
                tmpcolor.g = s.g * (VGfloat)kernelX[kx];
                tmpcolor.b = s.b * (VGfloat)kernelX[kx];
                tmpcolor.a = s.a * (VGfloat)kernelX[kx];
                tmpcolor.m_format = s.m_format;

                sum.r += tmpcolor.r;
                sum.g += tmpcolor.g;
                sum.b += tmpcolor.b;
                sum.a += tmpcolor.a;
            }
            tmp2[j * w + i].r = sum.r;
            tmp2[j * w + i].g = sum.g;
            tmp2[j * w + i].b = sum.b;
            tmp2[j * w + i].a = sum.a;
        }
    }

    if (tilingMode == VG_TILE_FILL)
    {//convolve the edge color
        Color sum;
        sum.r = sum.g = sum.b = sum.a = 0;
        sum.m_format = procFormat;
        for (int ki = 0; ki < kernelWidth; ki++)
        {
            Color tmpcolor1;
            tmpcolor1.r = (VGfloat)kernelX[ki] * edge->r;
            tmpcolor1.g = (VGfloat)kernelX[ki] * edge->g;
            tmpcolor1.b = (VGfloat)kernelX[ki] * edge->b;
            tmpcolor1.a = (VGfloat)kernelX[ki] * edge->a;

            sum.r += tmpcolor1.r;
            sum.g += tmpcolor1.g;
            sum.b += tmpcolor1.b;
            sum.a += tmpcolor1.a;
        }
        edge->r = sum.r;
        edge->g = sum.g;
        edge->b = sum.b;
        edge->a = sum.a;
        edge->m_format = sum.m_format;
    }

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            Color sum;
            sum.r = sum.g = sum.b = sum.a = 0;
            sum.m_format = procFormat;
            for (int kj = 0; kj < kernelHeight; kj++)
            {
                int y = j + kj - shiftY;
                Color s = readTiledPixel(i, y, w, src->m_height, tilingMode, tmp2, edge);

                int ky = kernelHeight - kj - 1;
                VG_ASSERT(ky >= 0 && ky < kernelHeight);

                Color tmpcolor2;
                tmpcolor2.r = (VGfloat)kernelY[ky] * s.r;
                tmpcolor2.g = (VGfloat)kernelY[ky] * s.g;
                tmpcolor2.b = (VGfloat)kernelY[ky] * s.b;
                tmpcolor2.a = (VGfloat)kernelY[ky] * s.a;

                sum.r += tmpcolor2.r;
                sum.g += tmpcolor2.g;
                sum.b += tmpcolor2.b;
                sum.a += tmpcolor2.a;
            }

            sum.r *= scale;
            sum.g *= scale;
            sum.b *= scale;
            sum.a *= scale;
            sum.r += bias;
            sum.g += bias;
            sum.b += bias;
            sum.a += bias;

            writeFilteredPixel(dst, i, j, &sum, channelMask);
        }
    }

    free(tmp);
    free(tmp2);
}

void maskSurface(Drawable* drawable, const Surface* src, VGMaskOperation operation, int x, int y, int w, int h)
{
    VG_ASSERT(w > 0 && h > 0);

    vg_lite_buffer_t* dst_masklayer = (vg_lite_buffer_t*)drawable->m_mask->m_image->m_vglbuf;
    if (operation == VG_CLEAR_MASK || operation == VG_FILL_MASK)
    {
        vg_lite_color_t color = 0;
        vg_lite_rectangle_t rect;
        if (operation == VG_FILL_MASK)
            color = (vg_lite_color_t)(0xFF << 24);
        rect.x = x;
        rect.y = y;
        rect.width = w;
        rect.height = h;
        vg_lite_clear(dst_masklayer, &rect, color);
    }
    else
    {
        vg_lite_matrix_t matrix;
        vg_lite_filter_t filter = VG_LITE_FILTER_POINT;
        vg_lite_rectangle_t rect;
        vg_lite_buffer_t* src_masklayer = (vg_lite_buffer_t*)(src->m_image)->m_vglbuf;
        rect.x = 0;
        rect.y = 0;
        rect.width = w;
        rect.height = h;
        vg_lite_identity(&matrix);
        vg_lite_translate((VGfloat)x, (VGfloat)y, &matrix);
        switch (operation)
        {
        case VG_LITE_SET_MASK:
            vg_lite_blit_rect(dst_masklayer, src_masklayer, &rect, &matrix, VG_LITE_BLEND_NONE, 0, filter);
            break;
        case VG_LITE_UNION_MASK:
            vg_lite_blit_rect(dst_masklayer, src_masklayer, &rect, &matrix, VG_LITE_BLEND_SCREEN, 0, filter);
            break;
        case VG_LITE_INTERSECT_MASK:
            vg_lite_blit_rect(dst_masklayer, src_masklayer, &rect, &matrix, VG_LITE_BLEND_DST_IN, 0, filter);
            break;
        case VG_LITE_SUBTRACT_MASK:
            vg_lite_blit_rect(dst_masklayer, src_masklayer, &rect, &matrix, VG_LITE_BLEND_SUBTRACT, 0, filter);
            break;
        default:
            break;
        }
    }

}

/********************************************      font      ********************************************/
Glyph* newGlyph(Font* font)
{
    for (int i = 0; i < font->m_glyphSize; i++)
    {
        if (font->m_glyphs[i].m_state == GLYPH_UNINITIALIZED)
            return &font->m_glyphs[i];
    }

    VGint currentsize = font->m_glyphSize + 1;
    if (currentsize <= font->m_glyphsAllocSize)
    {
        font->m_glyphSize++;
        return &font->m_glyphs[font->m_glyphSize - 1];
    }
    VG_ASSERT(currentsize > font->m_glyphsAllocSize);
    Glyph* new_glyphs = (Glyph*)malloc(currentsize * sizeof(Glyph));
    for (int i = 0; i < font->m_glyphArraySize; i++)
    {
        new_glyphs[i] = font->m_glyphs[i];
    }
    if (font->m_glyphs)
    {
        free(font->m_glyphs);
    }
    font->m_glyphSize++;
    font->m_glyphsAllocSize = currentsize;
    font->m_glyphs = new_glyphs;
    return &font->m_glyphs[font->m_glyphSize - 1];
}

void clearGlyph(Glyph* g)
{
    VG_ASSERT(g);
    if (g->m_path != VG_INVALID_HANDLE)
    {
        Path* p = (Path*)g->m_path;
        p->m_referenceCount--;
        VG_ASSERT(p->m_referenceCount >= 0);
        if (!p->m_referenceCount) {
            freePathImpl(p);
        }
    }
    if (g->m_image != VG_INVALID_HANDLE)
    {
        Image* p = (Image*)g->m_image;
        VG_ASSERT(p->m_inUse > 0);
        p->m_inUse--;
        p->m_referenceCount--;
        VG_ASSERT(p->m_referenceCount >= 0);
        if (!p->m_referenceCount)
            destroyImage(&p, NULL);
    }
    Glyph a = {0};
    a.m_state = GLYPH_UNINITIALIZED;
    a.m_path = VG_INVALID_HANDLE;
    a.m_image = VG_INVALID_HANDLE;
    a.m_origin.x = 0.0f;
    a.m_origin.y = 0.0f;
    a.m_escapement.x = 0.0f;
    a.m_escapement.y = 0.0f;
    a.m_isHinted = VG_FALSE;
    *g = a;
}

void setGlyphToPath(VGFont font, VGuint index, VGPath path, VGboolean isHinted, const Vector2* origin, const Vector2* escapement)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    Font* tmpfont = (Font*)font;
    Glyph* g = findGlyph(tmpfont, index);
    if (g)
    {
        clearGlyph(g);
    }
    else
    {
        g = newGlyph(tmpfont);
    }

    if (path)
    {
        Path* src_path = (Path*)path;
        //Path* dst_path = malloc(sizeof(Path));
        src_path->m_pathFont = VG_TRUE;
        //vg_lite_path_t* dst_vglpath = malloc(sizeof(vg_lite_path_t));
        //vg_lite_path_t* src_vglpath = src_path->m_vglPath;

        //memcpy(dst_vglpath, src_vglpath, sizeof(vg_lite_path_t));
        //memcpy(dst_path, src_path, sizeof(Path));
        //dst_path->m_vglPath = dst_vglpath;
        g->m_path = (VGPath)src_path;
    }
    else
    {
        g->m_path = path;
    }
    g->m_index = index;
    g->m_state = GLYPH_PATH;
    g->m_image = VG_INVALID_HANDLE;
    g->m_isHinted = isHinted;
    g->m_origin.x = origin->x;
    g->m_origin.y = origin->y;
    g->m_escapement.x = escapement->x;
    g->m_escapement.y = escapement->y;

    if (path != VG_INVALID_HANDLE)
    {
        Path* p = (Path*)path;
        p->m_referenceCount++;
    }
}

void setGlyphToImage(VGFont font, VGuint index, VGImage image, const Vector2* origin, const Vector2* escapement)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);
    Font* tmpfont = (Font*)font;
    Glyph* g = findGlyph(tmpfont, index);
    if (g)
    {
        clearGlyph(g);
    }
    else
    {
        g = newGlyph(tmpfont);
    }

    g->m_index = index;
    g->m_state = GLYPH_IMAGE;
    g->m_path = VG_INVALID_HANDLE;
    g->m_image = image;
    g->m_isHinted = VG_FALSE;
    g->m_origin.x = origin->x;
    g->m_origin.y = origin->y;
    g->m_escapement.x = escapement->x;
    g->m_escapement.y = escapement->y;

    if (image != VG_INVALID_HANDLE)
    {
        Image* p = (Image*)image;
        p->m_referenceCount++;
        p->m_inUse++;
    }
}

int getNumGlyphs(Font* font)
{
    int n = 0;
    for (int i = 0; i < font->m_glyphSize; i++)
    {
        if (font->m_glyphs[i].m_state != GLYPH_UNINITIALIZED)
            n++;
    }
    return n;
}

/********************************************      font      ********************************************/

/********************************************      rasterizer      ********************************************/
void setupRasterizer(Rasterizer* raster, VGint vpx, VGint vpy, VGint vpwidth, VGint vpheight, VGFillRule fillRule, const PixelPipe* pixelPipe, VGuint* covBuffer)
{
    VG_ASSERT(vpwidth >= 0 && vpheight >= 0);
    VG_ASSERT(vpx + vpwidth >= vpx && vpy + vpheight >= vpy);
    VG_ASSERT(fillRule == VG_EVEN_ODD || fillRule == VG_NON_ZERO);
    VG_ASSERT(pixelPipe || covBuffer);

    raster->m_vpx = vpx;
    raster->m_vpy = vpy;
    raster->m_vpheight = vpheight;
    raster->m_vpwidth = vpwidth;
    raster->m_fillRule = fillRule;
    //raster->m_pixelPipe = pixelPipe;
    raster->m_covBuffer = covBuffer;
    raster->m_covMinx = vpx + vpwidth;
    raster->m_covMiny = vpy + vpheight;
    raster->m_covMaxx = vpx;
    raster->m_covMaxy = vpy;
}

static double radicalInverseBase2(unsigned int i)
{
    if (i == 0)
        return 0.0;
    double p = 0.0;
    double f = 0.5;
    double ff = f;
    for (unsigned int j = 0; j < 32; j++)
    {
        if (i & (1 << j))
            p += f;
        f *= ff;
    }
    return p;
}

VGint setupSamplingPattern(Rasterizer* raster, VGRenderingQuality renderingQuality, VGint numFSAASamples)
{
    VG_ASSERT(renderingQuality == VG_RENDERING_QUALITY_NONANTIALIASED ||
        renderingQuality == VG_RENDERING_QUALITY_FASTER ||
        renderingQuality == VG_RENDERING_QUALITY_BETTER);

    raster->m_sumWeights = 0.0f;
    raster->m_sampleRadius = 0.0f;
    raster->m_numFSAASamples = numFSAASamples;

    if (numFSAASamples == 1)
    {
        if (renderingQuality == VG_RENDERING_QUALITY_NONANTIALIASED)
        {
            raster->m_numSamples = 1;
            raster->m_samples[0].x = 0.0f;
            raster->m_samples[0].y = 0.0f;
            raster->m_samples[0].weight = 1.0f;
            raster->m_sampleRadius = 0.0f;
            raster->m_sumWeights = 1.0f;
        }
        else if (renderingQuality == VG_RENDERING_QUALITY_FASTER)
        {
            raster->m_numSamples = 8;
            raster->m_samples[0].x = 3;
            raster->m_samples[1].x = 7;
            raster->m_samples[2].x = 0;
            raster->m_samples[3].x = 2;
            raster->m_samples[4].x = 5;
            raster->m_samples[5].x = 1;
            raster->m_samples[6].x = 6;
            raster->m_samples[7].x = 4;
            for (int i = 0; i < raster->m_numSamples; i++)
            {
                raster->m_samples[i].x = (raster->m_samples[i].x + 0.5f) / (VGfloat)raster->m_numSamples - 0.5f;
                raster->m_samples[i].y = ((VGfloat)i + 0.5f) / (VGfloat)raster->m_numSamples - 0.5f;
                raster->m_samples[i].weight = 1.0f / (VGfloat)raster->m_numSamples;
                raster->m_sumWeights += raster->m_samples[i].weight;
            }
            raster->m_sampleRadius = 0.5f;
        }
        else
        {
            VG_ASSERT(renderingQuality == VG_RENDERING_QUALITY_BETTER);
            raster->m_numSamples = VIV_MAX_SAMPLES;
            raster->m_sampleRadius = 0.75f;
            for (int i = 0; i < raster->m_numSamples; i++)
            {
                VGfloat x = (VGfloat)radicalInverseBase2(i);
                VGfloat y = ((VGfloat)i + 0.5f) / (VGfloat)raster->m_numSamples;
                VG_ASSERT(x >= 0.0f && x < 1.0f);
                VG_ASSERT(y >= 0.0f && y < 1.0f);

                VGfloat r = (VGfloat)sqrt(x) * raster->m_sampleRadius;
                x = r * (VGfloat)sin(y * 2.0f * PI);
                y = r * (VGfloat)cos(y * 2.0f * PI);
                raster->m_samples[i].weight = (VGfloat)exp(-0.5f * VG_SQR(r / raster->m_sampleRadius));

                VG_ASSERT(x >= -1.5f && x <= 1.5f && y >= -1.5f && y <= 1.5f);

                raster->m_samples[i].x = x;
                raster->m_samples[i].y = y;
                raster->m_sumWeights += raster->m_samples[i].weight;
            }
        }
    }
    else
    {
        raster->m_numSamples = numFSAASamples;
        VG_ASSERT(numFSAASamples >= 1 && numFSAASamples <= 32);
        for (int i = 0; i < raster->m_numSamples; i++)
        {
            raster->m_samples[i].x = (VGfloat)radicalInverseBase2(i) + 1.0f / (VGfloat)(raster->m_numSamples << 1) - 0.5f;
            raster->m_samples[i].y = ((VGfloat)i + 0.5f) / (VGfloat)raster->m_numSamples - 0.5f;
            raster->m_samples[i].weight = 1.0f;
            VG_ASSERT(raster->m_samples[i].x > -0.5f && raster->m_samples[i].x < 0.5f);
            VG_ASSERT(raster->m_samples[i].y > -0.5f && raster->m_samples[i].y < 0.5f);
        }
        raster->m_sumWeights = (VGfloat)raster->m_numSamples;
        raster->m_sampleRadius = 0.5f;
    }

    return raster->m_numSamples;
}

/********************************************      rasterizer      ********************************************/

/********************************************      pixelpipe      ********************************************/

void setDrawable(PixelPipe* pixelpipe, Drawable* drawable)
{
    VG_ASSERT(drawable);
    pixelpipe->m_drawable = drawable;
}

void setMask(PixelPipe* pixelpipe, VGboolean masking)
{
    pixelpipe->m_masking = masking;
}

void setPaint(PixelPipe* pixelpipe, const Paint* paint)
{
    pixelpipe->m_paint = paint;
    if (!pixelpipe->m_paint)
        pixelpipe->m_paint = &pixelpipe->m_defaultPaint;
    if (pixelpipe->m_paint->m_pattern)
        colorConvert(&pixelpipe->m_tileFillColor, pixelpipe->m_paint->m_pattern->m_desc.internalFormat);
}

void setBlendMode(PixelPipe* pixelpipe, VGBlendMode blendMode)
{
    VG_ASSERT(blendMode >= VG_BLEND_SRC && blendMode <= VG_BLEND_ADDITIVE);
    pixelpipe->m_blendMode = blendMode;
}

Paint* createdefaultPaint()
{
    Paint* paint = (Paint*)malloc(sizeof(Paint));
    if (!paint)
    {
        setError(VG_OUT_OF_MEMORY_ERROR);
        return VG_INVALID_HANDLE;
    }
    memset(paint, 0, sizeof(Paint));

    paint->m_paintType = VG_PAINT_TYPE_COLOR;
    paint->m_inputPaintColor.a = 1;
    paint->m_inputPaintColor.m_format = sRGBA;
    paint->m_paintColor.a = 1;
    paint->m_paintColor.m_format = sRGBA_PRE;
    paint->m_colorRampSpreadMode = VG_COLOR_RAMP_SPREAD_PAD;
    paint->m_colorRampPremultiplied = VG_TRUE;
    paint->m_inputLinearGradientPoint1.x = 1;
    paint->m_linearGradientPoint1.x = 1;
    paint->m_inputRadialGradientRadius = 1.0f;
    paint->m_radialGradientRadius = 1.0f;
    paint->m_patternTilingMode = VG_TILE_FILL;
    paint->object.type = OBJECTTYPE_PAINT;

    GradientStop gs;
    gs.offset = 0.0f;
    gs.color.r = 0;
    gs.color.g = 0;
    gs.color.b = 0;
    gs.color.a = 1;
    gs.color.m_format = sRGBA;
    paint->m_colorRampStops[0] = gs;
    gs.offset = 1.0f;
    gs.color.r = 1;
    gs.color.g = 1;
    gs.color.b = 1;
    gs.color.a = 1;
    gs.color.m_format = sRGBA;
    paint->m_colorRampStops[1] = gs;

    VG_ASSERT(paint);
    return paint;
}

/********************************************      pixelpipe      ********************************************/
