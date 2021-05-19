/****************************************************************************
 *
 *    Copyright 2012 - 2019 Vivante Corporation, Santa Clara, California.
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

#ifndef __vgrename_h_
#define __vgrename_h_

#if defined(_VG_APPENDIX)

#define _VG_RENAME_2(api, appendix) api##appendix
#define _VG_RENAME_1(api, appendix) _VG_RENAME_2(api, appendix)
#define gcmVG11(api)                _VG_RENAME_1(api, _VG_APPENDIX)

#define vgAppendPath              gcmVG11(vgAppendPath)
#define vgAppendPathData          gcmVG11(vgAppendPathData)
#define vgChildImage              gcmVG11(vgChildImage)
#define vgClear                   gcmVG11(vgClear)
#define vgClearGlyph              gcmVG11(vgClearGlyph)
#define vgClearImage              gcmVG11(vgClearImage)
#define vgClearPath               gcmVG11(vgClearPath)
#define vgColorMatrix             gcmVG11(vgColorMatrix)
#define vgConvolve                gcmVG11(vgConvolve)
#define vgCopyImage               gcmVG11(vgCopyImage)
#define vgCopyMask                gcmVG11(vgCopyMask)
#define vgCopyPixels              gcmVG11(vgCopyPixels)
#define vgCreateEGLImageTargetKHR gcmVG11(vgCreateEGLImageTargetKHR)
#define vgCreateFont              gcmVG11(vgCreateFont)
#define vgCreateImage             gcmVG11(vgCreateImage)
#define vgCreateMaskLayer         gcmVG11(vgCreateMaskLayer)
#define vgCreatePaint             gcmVG11(vgCreatePaint)
#define vgCreatePath              gcmVG11(vgCreatePath)
#define vgDestroyFont             gcmVG11(vgDestroyFont)
#define vgDestroyImage            gcmVG11(vgDestroyImage)
#define vgDestroyMaskLayer        gcmVG11(vgDestroyMaskLayer)
#define vgDestroyPaint            gcmVG11(vgDestroyPaint)
#define vgDestroyPath             gcmVG11(vgDestroyPath)
#define vgDrawGlyph               gcmVG11(vgDrawGlyph)
#define vgDrawGlyphs              gcmVG11(vgDrawGlyphs)
#define vgDrawImage               gcmVG11(vgDrawImage)
#define vgDrawPath                gcmVG11(vgDrawPath)
#define vgFillMaskLayer           gcmVG11(vgFillMaskLayer)
#define vgFinish                  gcmVG11(vgFinish)
#define vgFlush                   gcmVG11(vgFlush)
#define vgGaussianBlur            gcmVG11(vgGaussianBlur)
#define vgGetColor                gcmVG11(vgGetColor)
#define vgGetError                gcmVG11(vgGetError)
#define vgGetImageSubData         gcmVG11(vgGetImageSubData)
#define vgGetMatrix               gcmVG11(vgGetMatrix)
#define vgGetPaint                gcmVG11(vgGetPaint)
#define vgGetParameterVectorSize  gcmVG11(vgGetParameterVectorSize)
#define vgGetParameterf           gcmVG11(vgGetParameterf)
#define vgGetParameterfv          gcmVG11(vgGetParameterfv)
#define vgGetParameteri           gcmVG11(vgGetParameteri)
#define vgGetParameteriv          gcmVG11(vgGetParameteriv)
#define vgGetParent               gcmVG11(vgGetParent)
#define vgGetPathCapabilities     gcmVG11(vgGetPathCapabilities)
#define vgGetPixels               gcmVG11(vgGetPixels)
#define vgGetString               gcmVG11(vgGetString)
#define vgGetVectorSize           gcmVG11(vgGetVectorSize)
#define vgGetf                    gcmVG11(vgGetf)
#define vgGetfv                   gcmVG11(vgGetfv)
#define vgGeti                    gcmVG11(vgGeti)
#define vgGetiv                   gcmVG11(vgGetiv)
#define vgHardwareQuery           gcmVG11(vgHardwareQuery)
#define vgImageSubData            gcmVG11(vgImageSubData)
#define vgInterpolatePath         gcmVG11(vgInterpolatePath)
#define vgIterativeAverageBlurKHR gcmVG11(vgIterativeAverageBlurKHR)
#define vgLoadIdentity            gcmVG11(vgLoadIdentity)
#define vgLoadMatrix              gcmVG11(vgLoadMatrix)
#define vgLookup                  gcmVG11(vgLookup)
#define vgLookupSingle            gcmVG11(vgLookupSingle)
#define vgMask                    gcmVG11(vgMask)
#define vgModifyPathCoords        gcmVG11(vgModifyPathCoords)
#define vgMultMatrix              gcmVG11(vgMultMatrix)
#define vgPaintPattern            gcmVG11(vgPaintPattern)
#define vgParametricFilterKHR     gcmVG11(vgParametricFilterKHR)
#define vgPathBounds              gcmVG11(vgPathBounds)
#define vgPathLength              gcmVG11(vgPathLength)
#define vgPathTransformedBounds   gcmVG11(vgPathTransformedBounds)
#define vgPointAlongPath          gcmVG11(vgPointAlongPath)
#define vgProjectiveMatrixNDS     gcmVG11(vgProjectiveMatrixNDS)
#define vgReadPixels              gcmVG11(vgReadPixels)
#define vgRemovePathCapabilities  gcmVG11(vgRemovePathCapabilities)
#define vgRenderToMask            gcmVG11(vgRenderToMask)
#define vgRotate                  gcmVG11(vgRotate)
#define vgScale                   gcmVG11(vgScale)
#define vgSeparableConvolve       gcmVG11(vgSeparableConvolve)
#define vgSetColor                gcmVG11(vgSetColor)
#define vgSetGlyphToImage         gcmVG11(vgSetGlyphToImage)
#define vgSetGlyphToPath          gcmVG11(vgSetGlyphToPath)
#define vgSetPaint                gcmVG11(vgSetPaint)
#define vgSetParameterf           gcmVG11(vgSetParameterf)
#define vgSetParameterfv          gcmVG11(vgSetParameterfv)
#define vgSetParameteri           gcmVG11(vgSetParameteri)
#define vgSetParameteriv          gcmVG11(vgSetParameteriv)
#define vgSetPixels               gcmVG11(vgSetPixels)
#define vgSetf                    gcmVG11(vgSetf)
#define vgSetfv                   gcmVG11(vgSetfv)
#define vgSeti                    gcmVG11(vgSeti)
#define vgSetiv                   gcmVG11(vgSetiv)
#define vgShear                   gcmVG11(vgShear)
#define vgTransformPath           gcmVG11(vgTransformPath)
#define vgTranslate               gcmVG11(vgTranslate)
#define vgWritePixels             gcmVG11(vgWritePixels)

#define vguArc                     gcmVG11(vguArc)
#define vguBevelKHR                gcmVG11(vguBevelKHR)
#define vguComputeWarpQuadToQuad   gcmVG11(vguComputeWarpQuadToQuad)
#define vguComputeWarpQuadToSquare gcmVG11(vguComputeWarpQuadToSquare)
#define vguComputeWarpSquareToQuad gcmVG11(vguComputeWarpSquareToQuad)
#define vguDropShadowKHR           gcmVG11(vguDropShadowKHR)
#define vguEllipse                 gcmVG11(vguEllipse)
#define vguGlowKHR                 gcmVG11(vguGlowKHR)
#define vguGradientBevelKHR        gcmVG11(vguGradientBevelKHR)
#define vguGradientGlowKHR         gcmVG11(vguGradientGlowKHR)
#define vguLine                    gcmVG11(vguLine)
#define vguPolygon                 gcmVG11(vguPolygon)
#define vguRect                    gcmVG11(vguRect)
#define vguRoundRect               gcmVG11(vguRoundRect)
#define vguTransformClipLineNDS    gcmVG11(vguTransformClipLineNDS)

#endif /* _VG_APPENDIX */
#endif /* __vgrename_h_ */
