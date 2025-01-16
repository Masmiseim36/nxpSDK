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

#include "vg_context.h"


VGContext* createVgContext(VGContext* shareContext)
{
    VGContext* vgctx = (VGContext*)malloc(sizeof(VGContext));

    if (!vgctx)
    {
        // Out of memory
        return NULL;
    }
    memset(vgctx, 0, sizeof(VGContext));

    // Mode settings
    vgctx->m_matrixMode = VG_MATRIX_PATH_USER_TO_SURFACE;
    vgctx->m_fillRule = VG_EVEN_ODD;
    vgctx->m_imageQuality = VG_IMAGE_QUALITY_FASTER;
    vgctx->m_renderingQuality = VG_RENDERING_QUALITY_BETTER;
    vgctx->m_blendMode = VG_BLEND_SRC_OVER;
    vgctx->m_imageMode = VG_DRAW_IMAGE_NORMAL;

    // Scissor rectangles
    // vgctx->m_scissor();
        
    // Stroke parameters
    vgctx->m_strokeLineWidth = 1.0f;
    vgctx->m_inputStrokeLineWidth = 1.0f;
    vgctx->m_strokeCapStyle = VG_CAP_BUTT;
    vgctx->m_strokeJoinStyle = VG_JOIN_MITER;
    vgctx->m_strokeMiterLimit = 4.0f;
    vgctx->m_inputStrokeMiterLimit = 4.0f;
    // vgctx->m_strokeDashPattern();
    // vgctx->m_inputStrokeDashPattern();
    vgctx->m_strokeDashPhase = 0.0f;
    vgctx->m_inputStrokeDashPhase = 0.0f;
    vgctx->m_strokeDashPhaseReset = VG_FALSE;
    // Edge fill color for vgConvolve and pattern paint
    vgctx->m_tileFillColor.r = 0;
    vgctx->m_tileFillColor.g = 0;
    vgctx->m_tileFillColor.b = 0;
    vgctx->m_tileFillColor.a = 0;
    vgctx->m_tileFillColor.m_format = sRGBA;

    vgctx->m_inputTileFillColor.r = 0;
    vgctx->m_inputTileFillColor.g = 0;
    vgctx->m_inputTileFillColor.b = 0;
    vgctx->m_inputTileFillColor.a = 0;
    vgctx->m_inputTileFillColor.m_format = sRGBA;
    // Color for vgClear
    vgctx->m_clearColor.r = 0;
    vgctx->m_clearColor.g = 0;
    vgctx->m_clearColor.b = 0;
    vgctx->m_clearColor.a = 0;
    vgctx->m_clearColor.m_format = sRGBA;
    vgctx->m_inputClearColor.r = 0;
    vgctx->m_inputClearColor.g = 0;
    vgctx->m_inputClearColor.b = 0;
    vgctx->m_inputClearColor.a = 0;
    vgctx->m_inputClearColor.m_format = sRGBA;

    vgctx->m_glyphOrigin.x = 0.0f;
    vgctx->m_glyphOrigin.y = 0.0f;
    vgctx->m_inputGlyphOrigin.x = 0.0f;
    vgctx->m_inputGlyphOrigin.y = 0.0f;

    vgctx->m_masking = VG_FALSE;
    vgctx->m_scissoring = VG_FALSE;

    vgctx->m_pixelLayout = VG_PIXEL_LAYOUT_UNKNOWN;

    vgctx->m_filterFormatLinear = VG_FALSE;
    vgctx->m_filterFormatPremultiplied = VG_FALSE;
    vgctx->m_filterChannelMask = VG_RED | VG_GREEN | VG_BLUE | VG_ALPHA;

    // Matrices
    vgctx->m_pathUserToSurface.matrix[0][0] = 1.0f;
    vgctx->m_pathUserToSurface.matrix[1][1] = 1.0f;
    vgctx->m_pathUserToSurface.matrix[2][2] = 1.0f;

    vgctx->m_imageUserToSurface.matrix[0][0] = 1.0f;
    vgctx->m_imageUserToSurface.matrix[1][1] = 1.0f;
    vgctx->m_imageUserToSurface.matrix[2][2] = 1.0f;

    vgctx->m_glyphUserToSurface.matrix[0][0] = 1.0f;
    vgctx->m_glyphUserToSurface.matrix[1][1] = 1.0f;
    vgctx->m_glyphUserToSurface.matrix[2][2] = 1.0f;

    vgctx->m_fillPaintToUser.matrix[0][0] = 1.0f;
    vgctx->m_fillPaintToUser.matrix[1][1] = 1.0f;
    vgctx->m_fillPaintToUser.matrix[2][2] = 1.0f;

    vgctx->m_strokePaintToUser.matrix[0][0] = 1.0f;
    vgctx->m_strokePaintToUser.matrix[1][1] = 1.0f;
    vgctx->m_strokePaintToUser.matrix[2][2] = 1.0f;

    vgctx->m_fillPaint = VG_INVALID_HANDLE;
    vgctx->m_strokePaint = VG_INVALID_HANDLE;

    vgctx->m_colorTransform = VG_FALSE;
    vgctx->m_inputColorTransformValues[0] = 1.0f;
    vgctx->m_inputColorTransformValues[1] = 1.0f;
    vgctx->m_inputColorTransformValues[2] = 1.0f;
    vgctx->m_inputColorTransformValues[3] = 1.0f;
    vgctx->m_inputColorTransformValues[4] = 0.0f;
    vgctx->m_inputColorTransformValues[5] = 0.0f;
    vgctx->m_inputColorTransformValues[6] = 0.0f;
    vgctx->m_inputColorTransformValues[7] = 0.0f;
    vgctx->m_colorTransformValues[0] = 1.0f;
    vgctx->m_colorTransformValues[1] = 1.0f;
    vgctx->m_colorTransformValues[2] = 1.0f;
    vgctx->m_colorTransformValues[3] = 1.0f;
    vgctx->m_colorTransformValues[4] = 0.0f;
    vgctx->m_colorTransformValues[5] = 0.0f;
    vgctx->m_colorTransformValues[6] = 0.0f;
    vgctx->m_colorTransformValues[7] = 0.0f;

    vgctx->m_error = VG_NO_ERROR;

    vgctx->m_imageManager = NULL;
    vgctx->m_pathManager = NULL;
    vgctx->m_paintManager = NULL;
    vgctx->m_fontManager = NULL;
    vgctx->m_maskLayerManager = NULL;

    vgctx->m_eglDrawable = NULL;

    if (shareContext)
    {
        vgctx->m_imageManager = shareContext->m_imageManager;
        vgctx->m_pathManager = shareContext->m_pathManager;
        vgctx->m_paintManager = shareContext->m_paintManager;
        vgctx->m_fontManager = shareContext->m_fontManager;
        vgctx->m_maskLayerManager = shareContext->m_maskLayerManager;
    }
    else
    {
        vgctx->m_imageManager = (VGImageManager*)malloc(sizeof(VGImageManager));
        if (!vgctx->m_imageManager)
        {
            // Out of memory
            goto VgContextErr;
        }
        memset(vgctx->m_imageManager, 0, sizeof(VGImageManager));
        vgctx->m_imageManager->m_resources = (VGImageEntry*)malloc(sizeof(VGImageEntry));
        if (!vgctx->m_imageManager->m_resources)
        {
            // Out of memory
            goto VgContextErr;
        }
        memset(vgctx->m_imageManager->m_resources, 0, sizeof(VGImageEntry));

        vgctx->m_pathManager = (VGPathManager*)malloc(sizeof(VGPathManager));
        if (!vgctx->m_pathManager)
        {
            // Out of memory
            goto VgContextErr;
        }
        memset(vgctx->m_pathManager, 0, sizeof(VGPathManager));
        vgctx->m_pathManager->m_resources = (VGPathEntry*)malloc(sizeof(VGPathEntry));
        if (!vgctx->m_pathManager->m_resources)
        {
            // Out of memory
            goto VgContextErr;
        }
        memset(vgctx->m_pathManager->m_resources, 0, sizeof(VGPathEntry));

        vgctx->m_paintManager = (VGPaintManager*)malloc(sizeof(VGPaintManager));
        if (!vgctx->m_paintManager)
        {
            // Out of memory
            goto VgContextErr;
        }
        memset(vgctx->m_paintManager, 0, sizeof(VGPaintManager));

        vgctx->m_fontManager = (VGFontManager*)malloc(sizeof(VGFontManager));
        if (!vgctx->m_fontManager)
        {
            // Out of memory
            goto VgContextErr;
        }
        memset(vgctx->m_fontManager, 0, sizeof(VGFontManager));
        vgctx->m_fontManager->m_resources = (VGFontEntry*)malloc(sizeof(VGFontEntry));
        if (!vgctx->m_fontManager->m_resources)
        {
            // Out of memory
            goto VgContextErr;
        }
        memset(vgctx->m_fontManager->m_resources, 0, sizeof(VGFontEntry));

        vgctx->m_maskLayerManager = (VGMaskLayerManager*)malloc(sizeof(VGMaskLayerManager));
        if (!vgctx->m_maskLayerManager)
        {
            // Out of memory
            goto VgContextErr;
        }
        memset(vgctx->m_maskLayerManager, 0, sizeof(VGMaskLayerManager));
        vgctx->m_maskLayerManager->m_resources = (VGMaskLayerEntry*)malloc(sizeof(VGMaskLayerEntry));
        if (!vgctx->m_maskLayerManager->m_resources)
        {
            // Out of memory
            goto VgContextErr;
        }
        memset(vgctx->m_maskLayerManager->m_resources, 0, sizeof(VGMaskLayerEntry));
    }
    VG_ASSERT(vgctx->m_imageManager);
    VG_ASSERT(vgctx->m_pathManager);
    VG_ASSERT(vgctx->m_paintManager);
    VG_ASSERT(vgctx->m_fontManager);
    VG_ASSERT(vgctx->m_maskLayerManager);

    vgctx->m_imageManager->m_referenceCount++;
    vgctx->m_pathManager->m_referenceCount++;
    vgctx->m_paintManager->m_referenceCount++;
    vgctx->m_fontManager->m_referenceCount++;
    vgctx->m_maskLayerManager->m_referenceCount++;

    return vgctx;

VgContextErr:
    if (vgctx->m_imageManager && vgctx->m_imageManager->m_resources)
        free(vgctx->m_imageManager->m_resources);
    if (vgctx->m_imageManager)
        free(vgctx->m_imageManager);
    if (vgctx->m_pathManager && vgctx->m_pathManager->m_resources)
        free(vgctx->m_pathManager->m_resources);
    if (vgctx->m_pathManager)
        free(vgctx->m_pathManager);
    if (vgctx->m_paintManager)
        free(vgctx->m_paintManager);
    if (vgctx->m_fontManager && vgctx->m_fontManager->m_resources)
        free(vgctx->m_fontManager->m_resources);
    if (vgctx->m_fontManager)
        free(vgctx->m_fontManager);
    if (vgctx->m_maskLayerManager && vgctx->m_maskLayerManager->m_resources)
        free(vgctx->m_maskLayerManager->m_resources);
    if (vgctx->m_maskLayerManager)
        free(vgctx->m_maskLayerManager);
    free(vgctx);

    return NULL;
}

void setDefaultDrawable(VGContext* ctx, Drawable* drawable)
{
    if (ctx->m_eglDrawable)
    {
        ctx->m_eglDrawable->m_referenceCount--; VG_ASSERT(ctx->m_eglDrawable->m_referenceCount >= 0);
        if (!ctx->m_eglDrawable->m_referenceCount)
            destroyDrawable(ctx->m_eglDrawable);
    }
    ctx->m_eglDrawable = drawable;
    if (ctx->m_eglDrawable)
    {
        ctx->m_eglDrawable->m_referenceCount++;
    }
}

VGboolean isValidImage(VGContext* ctx, VGImage image)
{
    if (image == NULL)
        return VG_FALSE;
    VGuint handle;;
    VGint index;
    Objectptr current;
    handle = (VGuint)image;
    if ((handle < ctx->m_imageManager->lohandle) ||
        (handle > ctx->m_imageManager->hihandle))
    {
        return VG_FALSE;
    }
    Objectptr object_image = (Objectptr)image;
     if ((object_image->type >= OBJECTTYPE_COUNT) || (object_image->type != OBJECTTYPE_IMAGE))
         return VG_FALSE;
    index = object_image->name % VIV_OBJECTS_HASH;
    current = ctx->m_imageManager->m_resources->m_array[index];

    /* Search teh cache for the object. */
    while (current != NULL)
    {
        if (object_image == current)
        {
            break;
        }
        current = current->next;
    }
    if (current == NULL)
        return VG_FALSE;
    /* Move to the head of teh list. */
    if (current->prev != NULL)
    {
        /*Remove from the chain*/
        current->prev->next = current->next;
        if (current->next != NULL)
        {
            current->next->prev = current->prev;
        }
        /* Insert to the head. */
        current->prev = NULL;
        current->next = ctx->m_imageManager->m_resources->m_array[index];
        ctx->m_imageManager->m_resources->m_array[index]->prev = current;
        ctx->m_imageManager->m_resources->m_array[index] = current;
    }
    return VG_TRUE;
}

void addImageResource(VGContext* ctx, Image* image)
{
    VGint index;
    VGuint handle;
    Objectptr head;
    Objectptr object_image = (Objectptr)image;
    ctx->m_imageManager->m_resources->count++;
    object_image->name = ctx->m_imageManager->m_resources->count;
    VG_ASSERT(object_image->name);
    VG_ASSERT(object_image->type < OBJECTTYPE_COUNT);
    index = object_image->name % VIV_OBJECTS_HASH;
    head = ctx->m_imageManager->m_resources->m_array[index];

    /* Insert to the head*/
    object_image->prev = NULL;
    if (head == NULL)
    {
        object_image->next = NULL;
    }
    else
    {
        object_image->next = head;
        head->prev = object_image;
    }
    ctx->m_imageManager->m_resources->m_array[index] = object_image;
    image->m_referenceCount++;
    handle = (VGuint)object_image;
    if (handle < ctx->m_imageManager->lohandle)
    {
        ctx->m_imageManager->lohandle = handle;
    }
    if (handle > ctx->m_imageManager->hihandle)
    {
        ctx->m_imageManager->hihandle = handle;
    }
}

void removeImageResource(VGContext* ctx, Image* image)
{
    VGint index;
    Objectptr object_image = (Objectptr)image;
    index = object_image->name % VIV_OBJECTS_HASH;
    /* Remove from the chain */
    if (object_image == ctx->m_imageManager->m_resources->m_array[index])
    {
        ctx->m_imageManager->m_resources->m_array[index] = object_image->next;
    }
    else
    {
        if (object_image->prev)
        {
            object_image->prev->next = object_image->next;
        }
    }
    if (object_image->next)
    {
        object_image->next->prev = object_image->prev;
    }
    object_image->prev = object_image->next = NULL;
}

void destroyImageManager(VGContext* ctx)
{
    Objectptr object_image;
    if (ctx && ctx->m_imageManager)
    {
        for (int i = 0; i < VIV_OBJECTS_HASH; i++)
        {
            if (ctx->m_imageManager->m_resources->m_array[i] != NULL)
                /* Delete the objects*/
                while (ctx->m_imageManager->m_resources->m_array[i])
                {
                    /* Copy the head object */
                    object_image = ctx->m_imageManager->m_resources->m_array[i];

                    /* Remove from the chain */
                    if (object_image == ctx->m_imageManager->m_resources->m_array[i])
                    {
                        ctx->m_imageManager->m_resources->m_array[i] = object_image->next;
                    }
                    else
                    {
                        if (object_image->prev)
                        {
                            object_image->prev->next = object_image->prev;
                        }
                    }
                    if (object_image->next)
                    {
                        object_image->next->prev = object_image->prev;
                    }
                    object_image->prev = object_image->next = NULL;
                }
        }
        free(ctx->m_imageManager->m_resources);
        free(ctx->m_imageManager);
        ctx->m_imageManager = NULL;
    }
}

VGboolean isValidMaskLayer(VGContext* ctx, VGHandle mask)
{
    if (mask == NULL)
        return VG_FALSE;
    VGint index;
    VGuint handle;
    Objectptr current;
    handle = (VGuint)mask;
    if ((handle < ctx->m_maskLayerManager->lohandle) ||
        (handle > ctx->m_maskLayerManager->hihandle))
    {
        return VG_FALSE;
    }
    Objectptr object_mask = (Objectptr)mask;
    if ((object_mask->type >= OBJECTTYPE_COUNT) || (object_mask->type != OBJECTTYPE_MASK))
          return VG_FALSE;
    index = object_mask->name % VIV_OBJECTS_HASH;
    current = ctx->m_maskLayerManager->m_resources->m_array[index];

    /* Search teh cache for the object. */
    while (current != NULL)
    {
        if (object_mask == current)
        {
            break;
        }
        current = current->next;
    }
    if (current == NULL)
        return VG_FALSE;
    /* Move to the head of teh list. */
    if (current->prev != NULL)
    {
        /*Remove from the chain*/
        current->prev->next = current->next;
        if (current->next != NULL)
        {
            current->next->prev = current->prev;
        }
        /* Insert to the head. */
        current->prev = NULL;
        current->next = ctx->m_maskLayerManager->m_resources->m_array[index];
        ctx->m_maskLayerManager->m_resources->m_array[index]->prev = current;
        ctx->m_maskLayerManager->m_resources->m_array[index] = current;
    }
    return VG_TRUE;
}

void addMaskLayerResource(VGContext* ctx, Surface* layer)
{
    VGint index;
    VGuint handle;
    Objectptr head;
    Objectptr object_mask = (Objectptr)layer;
    ctx->m_maskLayerManager->m_resources->count++;
    object_mask->name = ctx->m_maskLayerManager->m_resources->count;
    VG_ASSERT(object_mask->name);
    VG_ASSERT(object_mask->type < OBJECTTYPE_COUNT);
    index = object_mask->name % VIV_OBJECTS_HASH;
    head = ctx->m_maskLayerManager->m_resources->m_array[index];

    /* Insert to the head*/
    object_mask->prev = NULL;
    if (head == NULL)
    {
        object_mask->next = NULL;
    }
    else
    {
        object_mask->next = head;
        head->prev = object_mask;
    }
    ctx->m_maskLayerManager->m_resources->m_array[index] = object_mask;
    handle = (VGuint)object_mask;
    if (handle < ctx->m_maskLayerManager->lohandle)
    {
        ctx->m_maskLayerManager->lohandle = handle;
    }
    if (handle > ctx->m_maskLayerManager->hihandle)
    {
        ctx->m_maskLayerManager->hihandle = handle;
    }
}

void removeMaskLayerResource(VGContext* ctx, Surface* layer)
{
    VGint index;
    Objectptr object_mask = (Objectptr)layer;
    index = object_mask->name % VIV_OBJECTS_HASH;
    /* Remove from the chain */
    if (object_mask == ctx->m_maskLayerManager->m_resources->m_array[index])
    {
        ctx->m_maskLayerManager->m_resources->m_array[index] = object_mask->next;
    }
    else
    {
        if (object_mask->prev)
        {
            object_mask->prev->next = object_mask->next;
        }
    }
    if (object_mask->next)
    {
        object_mask->next->prev = object_mask->prev;
    }
    object_mask->prev = object_mask->next = NULL;
}

void destroyMaskLayerManager(VGContext* ctx)
{
    Objectptr object_mask;
    if (ctx && ctx->m_maskLayerManager)
    {
        for (int i = 0; i < VIV_OBJECTS_HASH; i++)
        {
            if (ctx->m_maskLayerManager->m_resources->m_array[i] != NULL)
                /* Delete the objects*/
                while (ctx->m_maskLayerManager->m_resources->m_array[i])
                {
                    /* Copy the head object */
                    object_mask = ctx->m_maskLayerManager->m_resources->m_array[i];

                    /* Remove from the chain */
                    if (object_mask == ctx->m_maskLayerManager->m_resources->m_array[i])
                    {
                        ctx->m_maskLayerManager->m_resources->m_array[i] = object_mask->next;
                    }
                    else
                    {
                        if (object_mask->prev)
                        {
                            object_mask->prev->next = object_mask->prev;
                        }
                    }
                    if (object_mask->next)
                    {
                        object_mask->next->prev = object_mask->prev;
                    }
                    object_mask->prev = object_mask->next = NULL;
                }
        }
        free(ctx->m_maskLayerManager->m_resources);
        free(ctx->m_maskLayerManager);
        ctx->m_maskLayerManager = NULL;
    }
}

VGboolean isValidPath(VGContext* ctx, VGPath path)
{
    if (path == NULL)
        return VG_FALSE;
    VGint index;
    VGuint handle;
    Objectptr current;
    handle = (VGuint)path;
    if ((handle < ctx->m_pathManager->lohandle) ||
        (handle > ctx->m_pathManager->hihandle))
    {
        return VG_FALSE;
    }
    Objectptr object_path = (Objectptr)path;
     if ((object_path->type >= OBJECTTYPE_COUNT) || (object_path->type != OBJECTTYPE_PATH))
         return VG_FALSE;
    index = object_path->name % VIV_OBJECTS_HASH;
    current = ctx->m_pathManager->m_resources->m_array[index];

    /* Search teh cache for the object. */
    while (current != NULL)
    {
        if (object_path == current)
        {
            break;
        }
        current = current->next;
    }
    if (current == NULL)
        return VG_FALSE;
    /* Move to the head of teh list. */
    if (current->prev != NULL)
    {
        /*Remove from the chain*/
        current->prev->next = current->next;
        if (current->next != NULL)
        {
            current->next->prev = current->prev;
        }
        /* Insert to the head. */
        current->prev = NULL;
        current->next = ctx->m_pathManager->m_resources->m_array[index];
        ctx->m_pathManager->m_resources->m_array[index]->prev = current;
        ctx->m_pathManager->m_resources->m_array[index] = current;
    }
    return VG_TRUE;
}

void addPathResource(VGContext* ctx, Path* path)
{
    VGint index;
    VGuint handle;
    Objectptr head;
    Objectptr object_path = (Objectptr)path;
    ctx->m_pathManager->m_resources->count++;
    object_path->name = ctx->m_pathManager->m_resources->count;
    VG_ASSERT(object_path->name);
    VG_ASSERT(object_path->type < OBJECTTYPE_COUNT);
    index = object_path->name % VIV_OBJECTS_HASH;
    head = ctx->m_pathManager->m_resources->m_array[index];

    /* Insert to the head*/
    object_path->prev = NULL;
    if (head == NULL)
    {
        object_path->next = NULL;
    }
    else
    {
        object_path->next = head;
        head->prev = object_path;
    }
    ctx->m_pathManager->m_resources->m_array[index] = object_path;
    handle = (VGuint)object_path;
    if (handle < ctx->m_pathManager->lohandle)
    {
        ctx->m_pathManager->lohandle = handle;
    }
    if (handle > ctx->m_pathManager->hihandle)
    {
        ctx->m_pathManager->hihandle = handle;
    }

}

void removePathResource(VGContext* ctx, Path* path)
{
    VGint index;
    Objectptr object_path = (Objectptr)path;
    index = object_path->name % VIV_OBJECTS_HASH;
    /* Remove from the chain */
    if (object_path == ctx->m_pathManager->m_resources->m_array[index])
    {
        ctx->m_pathManager->m_resources->m_array[index] = object_path->next;
    }
    else
    {
        if (object_path->prev)
        {
            object_path->prev->next = object_path->next;
        }
    }
    if (object_path->next)
    {
        object_path->next->prev = object_path->prev;
    }
    object_path->prev = object_path->next = NULL;
}

void destroyPathManager(VGContext* ctx)
{
    Objectptr object_path;
    if (ctx && ctx->m_pathManager)
    {
        for (int i = 0; i < VIV_OBJECTS_HASH; i++)
        {
            if (ctx->m_pathManager->m_resources->m_array[i] != NULL)
                /* Delete the objects*/
                while (ctx->m_pathManager->m_resources->m_array[i])
                {
                    /* Copy the head object */
                    object_path = ctx->m_pathManager->m_resources->m_array[i];

                    /* Remove from the chain */
                    if (object_path == ctx->m_pathManager->m_resources->m_array[i])
                    {
                        ctx->m_pathManager->m_resources->m_array[i] = object_path->next;
                    }
                    else
                    {
                        if (object_path->prev)
                        {
                            object_path->prev->next = object_path->prev;
                        }
                    }
                    if (object_path->next)
                    {
                        object_path->next->prev = object_path->prev;
                    }
                    object_path->prev = object_path->next = NULL;
                    freePathImpl((Path*)object_path);
                }
        }
        free(ctx->m_pathManager->m_resources);
        free(ctx->m_pathManager);
        ctx->m_pathManager = NULL;
    }
}

VGboolean isValidPaint(VGContext* ctx, VGPaint paint)
{
    VGPaintEntry* resptr = NULL;

    if (ctx && ctx->m_paintManager && ctx->m_paintManager->m_resources)
    {
        resptr = ctx->m_paintManager->m_resources;
        while (resptr)
        {
            if (resptr->resource == (Paint*)paint)
            {
                return VG_TRUE;
            }
            resptr = resptr->next;
        }
    }

    return VG_FALSE;
}

void addPaintResource(VGContext* ctx, Paint* paint)
{
    VGPaintEntry* resptr = NULL;

    if (ctx && ctx->m_paintManager)
    {
        resptr = ctx->m_paintManager->m_resources;
        while (resptr)
        {
            if (resptr->resource == paint) return;
            resptr = resptr->next;
        }

        resptr = (VGPaintEntry*)malloc(sizeof(VGPaintEntry));
        if (!resptr)
        {
            setError(VG_OUT_OF_MEMORY_ERROR);
            return;
        }
        resptr->resource = paint;
        resptr->next = ctx->m_paintManager->m_resources;
        ctx->m_paintManager->m_resources = resptr;
        paint->m_referenceCount++;
    }
}

void removePaintResource(VGContext* ctx, Paint* paint)
{
    VGPaintEntry* resptr, * prep;

    if (ctx && ctx->m_paintManager)
    {
        resptr = ctx->m_paintManager->m_resources;
        prep = resptr;

        while (resptr)
        {
            if (resptr->resource == paint)
            {
                if (resptr == ctx->m_paintManager->m_resources)
                {
                    ctx->m_paintManager->m_resources = resptr->next;
                }
                else
                {
                    prep->next = resptr->next;
                }
                free(resptr);
                break;
            }
            prep = resptr;
            resptr = resptr->next;
        }
    }
}

void releasePaint(VGContext* ctx, VGbitfield paintModes)
{
    if (paintModes & VG_FILL_PATH)
    {
        //release previous paint
        Paint* prev = (Paint*)ctx->m_fillPaint;
        if (prev)
        {
            prev->m_referenceCount--;

            if (!prev->m_referenceCount)
                freePaintImpl(prev);
        }
        if (ctx->m_strokePaint == ctx->m_fillPaint)
        {
            ctx->m_strokePaint = VG_INVALID_HANDLE;
        }
        ctx->m_fillPaint = VG_INVALID_HANDLE;
    }
    if (paintModes & VG_STROKE_PATH)
    {
        //release previous paint
        Paint* prev = (Paint*)ctx->m_strokePaint;
        if (prev)
        {
            prev->m_referenceCount--;
            if (!prev->m_referenceCount)
                freePaintImpl(prev);
        }
        if (ctx->m_fillPaint == ctx->m_strokePaint)
        {
            ctx->m_fillPaint = VG_INVALID_HANDLE;
        }
        ctx->m_strokePaint = VG_INVALID_HANDLE;
    }
}

VGboolean isValidFont(VGContext* ctx, VGFont font)
{
    if (font == NULL)
        return VG_FALSE;
    VGint index;
    VGuint handle;
    Objectptr current;
    handle = (VGuint)font;
    if ((handle < ctx->m_fontManager->lohandle) ||
        (handle > ctx->m_fontManager->hihandle))
    {
        return VG_FALSE;
    }
    Objectptr object_font = (Objectptr)font;
    if ((object_font->type >= OBJECTTYPE_COUNT) || (object_font->type != OBJECTTYPE_FONT))
          return VG_FALSE;
    index = object_font->name % VIV_OBJECTS_HASH;
    current = ctx->m_fontManager->m_resources->m_array[index];

    /* Search teh cache for the object. */
    while (current != NULL)
    {
        if (object_font == current)
        {
            break;
        }
        current = current->next;
    }
    if (current == NULL)
        return VG_FALSE;
    /* Move to the head of teh list. */
    if (current->prev != NULL)
    {
        /*Remove from the chain*/
        current->prev->next = current->next;
        if (current->next != NULL)
        {
            current->next->prev = current->prev;
        }
        /* Insert to the head. */
        current->prev = NULL;
        current->next = ctx->m_fontManager->m_resources->m_array[index];
        ctx->m_fontManager->m_resources->m_array[index]->prev = current;
        ctx->m_fontManager->m_resources->m_array[index] = current;
    }
    return VG_TRUE;
}

void destroyPaintManager(VGContext* ctx)
{
    VGPaintEntry* resptr;

    if (ctx && ctx->m_paintManager)
    {
        resptr = ctx->m_paintManager->m_resources;
        while (resptr)
        {
            ctx->m_paintManager->m_resources = resptr->next;
            if (resptr->resource)
            {
                freePaintImpl((Paint*)resptr->resource);
            }
            free(resptr);
            resptr = ctx->m_paintManager->m_resources;
        }

        free(ctx->m_paintManager);
        ctx->m_paintManager = NULL;
    }
}

void addFontResource(VGContext* ctx, Font* font)
{
    VGint index;
    VGuint handle;
    Objectptr head;
    Objectptr object_font = (Objectptr)font;
    ctx->m_fontManager->m_resources->count++;
    object_font->name = ctx->m_fontManager->m_resources->count;
    VG_ASSERT(object_font->name);
    VG_ASSERT(object_font->type < OBJECTTYPE_COUNT);
    index = object_font->name % VIV_OBJECTS_HASH;
    head = ctx->m_fontManager->m_resources->m_array[index];

    /* Insert to the head*/
    object_font->prev = NULL;
    if (head == NULL)
    {
        object_font->next = NULL;
    }
    else
    {
        object_font->next = head;
        head->prev = object_font;
    }
    ctx->m_fontManager->m_resources->m_array[index] = object_font;
    font->m_referenceCount++;
    handle = (VGuint)object_font;
    if (handle < ctx->m_fontManager->lohandle)
    {
        ctx->m_fontManager->lohandle = handle;
    }
    if (handle > ctx->m_fontManager->hihandle)
    {
        ctx->m_fontManager->hihandle = handle;
    }
}

void removeFontResource(VGContext* ctx, Font* font)
{
    VGint index;
    Objectptr object_font = (Objectptr)font;
    index = object_font->name % VIV_OBJECTS_HASH;
    /* Remove from the chain */
    if (object_font == ctx->m_fontManager->m_resources->m_array[index])
    {
        ctx->m_fontManager->m_resources->m_array[index] = object_font->next;
    }
    else
    {
        if (object_font->prev)
        {
            object_font->prev->next = object_font->next;
        }
    }
    if (object_font->next)
    {
        object_font->next->prev = object_font->prev;
    }
    object_font->prev = object_font->next = NULL;
}

void destroyFontManager(VGContext* ctx)
{
    Objectptr object_font;
    if (ctx && ctx->m_fontManager)
    {
        for (int i = 0; i < VIV_OBJECTS_HASH; i++)
        {
            if (ctx->m_fontManager->m_resources->m_array[i] != NULL)
                /* Delete the objects*/
                while (ctx->m_fontManager->m_resources->m_array[i])
                {
                    /* Copy the head object */
                    object_font = ctx->m_fontManager->m_resources->m_array[i];

                    /* Remove from the chain */
                    if (object_font == ctx->m_fontManager->m_resources->m_array[i])
                    {
                        ctx->m_fontManager->m_resources->m_array[i] = object_font->next;
                    }
                    else
                    {
                        if (object_font->prev)
                        {
                            object_font->prev->next = object_font->prev;
                        }
                    }
                    if (object_font->next)
                    {
                        object_font->next->prev = object_font->prev;
                    }
                    object_font->prev = object_font->next = NULL;
                    freeFontImpl((Font*)object_font);
                }
        }
        free(ctx->m_fontManager->m_resources);
        free(ctx->m_fontManager);
        ctx->m_fontManager = NULL;
    }
}

Glyph* findGlyph(Font* font, unsigned int index)
{
    for (int i = 0; i < font->m_glyphSize; i++)
    {
        if (font->m_glyphs[i].m_state != GLYPH_UNINITIALIZED && font->m_glyphs[i].m_index == index)
            return &font->m_glyphs[i];
    }
    return NULL;
}

void destroyVgContext(VGContext* ctx)
{
    releasePaint(ctx, (VG_FILL_PATH | VG_STROKE_PATH));
    setDefaultDrawable(ctx, NULL);

    //decrease the reference count of resource managers
    ctx->m_imageManager->m_referenceCount--;
    if (!ctx->m_imageManager->m_referenceCount)
        destroyImageManager(ctx);

    ctx->m_pathManager->m_referenceCount--;
    if (!ctx->m_pathManager->m_referenceCount)
        destroyPathManager(ctx);

    ctx->m_paintManager->m_referenceCount--;
    if (!ctx->m_paintManager->m_referenceCount)
        destroyPaintManager(ctx);

    ctx->m_fontManager->m_referenceCount--;
    if (!ctx->m_fontManager->m_referenceCount)
        destroyFontManager(ctx);

    ctx->m_maskLayerManager->m_referenceCount--;
    if (!ctx->m_maskLayerManager->m_referenceCount)
        destroyMaskLayerManager(ctx);
    if (ctx)
        free(ctx);
}

void freePathImpl(Path* path) {
    if (path->m_vglPath) {
        vg_lite_clear_path(path->m_vglPath);
        free(path->m_vglPath);
        path->m_vglPath = NULL;
    }

    for (VGint i = 0; i < path->m_numSegments; ++i) {
        free(path->m_data[i]);
    }

    if (path->m_segments) {
        free(path->m_segments);
        path->m_segments = NULL;
    }

    if (path->m_data) {
        free(path->m_data);
        path->m_data = NULL;
    }
    free(path);
}

void freePaintImpl(Paint* paint)
{
    if (paint->m_pattern)
    {
        paint->m_pattern->m_referenceCount--;
        paint->m_pattern->m_inUse--;
        paint->m_pattern = NULL;
    }
    if (paint->grad_image.handle != NULL)
    {
        vg_lite_free(&paint->grad_image);
    }
    free(paint);

}

void freeFontImpl(Font* font)
{
    if (font->m_glyphs)
    {
        for (int i = 0; i < font->m_glyphSize; i++)
        {
            Path* temp = (Path*)font->m_glyphs[i].m_path;
            if (temp)
            {
                temp->m_referenceCount--;
                if (!temp->m_referenceCount) {
                    freePathImpl(temp);
                }
            }
            Image* temp1 = (Image*)font->m_glyphs[i].m_image;
            if (temp1)
            {
                temp1->m_referenceCount--;
                temp1->m_inUse--;
                if (!temp1->m_referenceCount){
                    if (temp1->m_vglbuf)
                    {
                        vg_lite_free((vg_lite_buffer_t*)temp1->m_vglbuf);
                        free((vg_lite_buffer_t*)temp1->m_vglbuf);
                    }
                    free(temp1);
                }
            }
        }
        free((Glyph*)font->m_glyphs);
    }
    free(font);
}


void  setError(int errcode)
{
    VG_GET_CONTEXT(VG_NO_RETVAL);

    if (context->m_error == VG_NO_ERROR)
    {
        context->m_error = errcode;
    }
}
