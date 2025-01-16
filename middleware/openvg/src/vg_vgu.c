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
 *
 * VGU library for OpenVG 1.1 Reference Implementation
 * ---------------------------------------------------
 *
 * Copyright (c) 2007 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and /or associated documentation files
 * (the "Materials "), to deal in the Materials without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Materials,
 * and to permit persons to whom the Materials are furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR
 * THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 *//**
 * \file
 * \brief    Implementation of the VGU utility library for OpenVG
 *//*-------------------------------------------------------------------*/

#include "vg_context.h"
#include <vgu.h>


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

static VG_INLINE void matrixMultiply(Matrix3x3* tm, Matrix3x3* m1, Matrix3x3* m2)
{
    tm->matrix[0][0] = m1->matrix[0][0] * m2->matrix[0][0] + m1->matrix[0][1] * m2->matrix[1][0] + m1->matrix[0][2] * m2->matrix[2][0];
    tm->matrix[0][1] = m1->matrix[0][0] * m2->matrix[0][1] + m1->matrix[0][1] * m2->matrix[1][1] + m1->matrix[0][2] * m2->matrix[2][1];
    tm->matrix[0][2] = m1->matrix[0][0] * m2->matrix[0][2] + m1->matrix[0][1] * m2->matrix[1][2] + m1->matrix[0][2] * m2->matrix[2][2];

    tm->matrix[1][0] = m1->matrix[1][0] * m2->matrix[0][0] + m1->matrix[1][1] * m2->matrix[1][0] + m1->matrix[1][2] * m2->matrix[2][0];
    tm->matrix[1][1] = m1->matrix[1][0] * m2->matrix[0][1] + m1->matrix[1][1] * m2->matrix[1][1] + m1->matrix[1][2] * m2->matrix[2][1];
    tm->matrix[1][2] = m1->matrix[1][0] * m2->matrix[0][2] + m1->matrix[1][1] * m2->matrix[1][2] + m1->matrix[1][2] * m2->matrix[2][2];

    tm->matrix[2][0] = m1->matrix[2][0] * m2->matrix[0][0] + m1->matrix[2][1] * m2->matrix[1][0] + m1->matrix[2][2] * m2->matrix[2][0];
    tm->matrix[2][1] = m1->matrix[2][0] * m2->matrix[0][1] + m1->matrix[2][1] * m2->matrix[1][1] + m1->matrix[2][2] * m2->matrix[2][1];
    tm->matrix[2][2] = m1->matrix[2][0] * m2->matrix[0][2] + m1->matrix[2][1] * m2->matrix[1][2] + m1->matrix[2][2] * m2->matrix[2][2];
}
static VG_INLINE VGUErrorCode inverseMatrix3x3(Matrix3x3* result, Matrix3x3* matrix)
{
    VGfloat det00, det01, det02;
    VGfloat d;
    int isAffine;

    /* Test for identity matrix. */
    if (matrix == NULL) {
        result->matrix[0][0] = 1.0f;
        result->matrix[0][1] = 0.0f;
        result->matrix[0][2] = 0.0f;
        result->matrix[1][0] = 0.0f;
        result->matrix[1][1] = 1.0f;
        result->matrix[1][2] = 0.0f;
        result->matrix[2][0] = 0.0f;
        result->matrix[2][1] = 0.0f;
        result->matrix[2][2] = 1.0f;
        /* Success. */
        return VGU_ILLEGAL_ARGUMENT_ERROR;
    }
    det00 = (matrix->matrix[1][1] * matrix->matrix[2][2]) - (matrix->matrix[2][1] * matrix->matrix[1][2]);
    det01 = (matrix->matrix[2][0] * matrix->matrix[1][2]) - (matrix->matrix[1][0] * matrix->matrix[2][2]);
    det02 = (matrix->matrix[1][0] * matrix->matrix[2][1]) - (matrix->matrix[2][0] * matrix->matrix[1][1]);

    /* Compute determinant. */
    d = (matrix->matrix[0][0] * det00) + (matrix->matrix[0][1] * det01) + (matrix->matrix[0][2] * det02);

    /* Return 0 if there is no inverse matrix. */
    if (d == 0.0f)
        return VGU_ILLEGAL_ARGUMENT_ERROR;

    /* Compute reciprocal. */
    d = 1.0f / d;
    /* Determine if the matrix is affine. */
    isAffine = (matrix->matrix[2][0] == 0.0f) && (matrix->matrix[2][1] == 0.0f) && (matrix->matrix[2][2] == 1.0f);
    result->matrix[0][0] = d * det00;
    result->matrix[0][1] = d * ((matrix->matrix[2][1] * matrix->matrix[0][2]) - (matrix->matrix[0][1] * matrix->matrix[2][2]));
    result->matrix[0][2] = d * ((matrix->matrix[0][1] * matrix->matrix[1][2]) - (matrix->matrix[1][1] * matrix->matrix[0][2]));
    result->matrix[1][0] = d * det01;
    result->matrix[1][1] = d * ((matrix->matrix[0][0] * matrix->matrix[2][2]) - (matrix->matrix[2][0] * matrix->matrix[0][2]));
    result->matrix[1][2] = d * ((matrix->matrix[1][0] * matrix->matrix[0][2]) - (matrix->matrix[0][0] * matrix->matrix[1][2]));
    result->matrix[2][0] = isAffine ? 0.0f : d * det02;
    result->matrix[2][1] = isAffine ? 0.0f : d * ((matrix->matrix[2][0] * matrix->matrix[0][1]) - (matrix->matrix[0][0] * matrix->matrix[2][1]));
    result->matrix[2][2] = isAffine ? 1.0f : d * ((matrix->matrix[0][0] * matrix->matrix[1][1]) - (matrix->matrix[1][0] * matrix->matrix[0][1]));
    return VGU_NO_ERROR;
}
static void append(VGPath path, int numSegments, const VGubyte* segments, int numCoordinates, const VGfloat* coordinates)
{
    VG_ASSERT(numCoordinates <= 26);

    VGPathDatatype datatype = (VGPathDatatype)vgGetParameteri(path, VG_PATH_DATATYPE);
    VGfloat scale = vgGetParameterf(path, VG_PATH_SCALE);
    VGfloat bias = vgGetParameterf(path, VG_PATH_BIAS);

    switch(datatype)
    {
    case VG_PATH_DATATYPE_S_8:
    {
        VGbyte data[26];
        for (int i=0;i<numCoordinates;i++)
            data[i] = (VGbyte)floor((coordinates[i] - bias) / scale + 0.5f);    //add 0.5 for correct rounding
        vgAppendPathData(path, numSegments, segments, data);
        break;
    }

    case VG_PATH_DATATYPE_S_16:
    {
        VGshort data[26];
        for (int i=0;i<numCoordinates;i++)
            data[i] = (VGshort)floor((coordinates[i] - bias) / scale + 0.5f);    //add 0.5 for correct rounding
        vgAppendPathData(path, numSegments, segments, data);
        break;
    }

    case VG_PATH_DATATYPE_S_32:
    {
        VGint data[26];
        for (int i=0;i<numCoordinates;i++)
            data[i] = (VGint)floor((coordinates[i] - bias) / scale + 0.5f);    //add 0.5 for correct rounding
        vgAppendPathData(path, numSegments, segments, data);
        break;
    }

    default:
    {
        VG_ASSERT(datatype == VG_PATH_DATATYPE_F);
        VGfloat data[26];
        for (int i=0;i<numCoordinates;i++)
            data[i] = (VGfloat)((coordinates[i] - bias) / scale);
        vgAppendPathData(path, numSegments, segments, data);
        break;
    }
    }
}

VGUErrorCode VG_APIENTRY vguLine(VGPath path, VGfloat x0, VGfloat y0, VGfloat x1, VGfloat y1)
{
    VGErrorCode error = vgGetError();    //clear the error state
    static const VGubyte segments[2] = {VG_MOVE_TO | VG_ABSOLUTE, VG_LINE_TO | VG_ABSOLUTE};
    const VGfloat data[4] = {x0, y0, x1, y1};
    append(path, 2, segments, 4, data);

    error = vgGetError();
    if (error == VG_BAD_HANDLE_ERROR)
        return VGU_BAD_HANDLE_ERROR;
    else if (error == VG_PATH_CAPABILITY_ERROR)
        return VGU_PATH_CAPABILITY_ERROR;
    return VGU_NO_ERROR;
}

VGUErrorCode VG_APIENTRY vguPolygon(VGPath path, const VGfloat * points, VGint count, VGboolean closed)
{
    VGErrorCode error = vgGetError();    //clear the error state
    if (!points || (((VGuintptr)points) & 3) || count <= 0)
        return VGU_ILLEGAL_ARGUMENT_ERROR;

    VGubyte segments[1] = {VG_MOVE_TO | VG_ABSOLUTE};
    VGfloat data[2];
    for (int i=0;i<count;i++)
    {
        data[0] = points[i*2+0];
        data[1] = points[i*2+1];
        append(path, 1, segments, 2, data);
        segments[0] = VG_LINE_TO | VG_ABSOLUTE;
    }
    if (closed)
    {
        segments[0] = VG_CLOSE_PATH;
        append(path, 1, segments, 0, data);
    }

    error = vgGetError();
    if (error == VG_BAD_HANDLE_ERROR)
        return VGU_BAD_HANDLE_ERROR;
    else if (error == VG_PATH_CAPABILITY_ERROR)
        return VGU_PATH_CAPABILITY_ERROR;
    return VGU_NO_ERROR;
}

VGUErrorCode VG_APIENTRY vguRect(VGPath path, VGfloat x, VGfloat y, VGfloat width, VGfloat height)
{
    VGErrorCode error = vgGetError();    //clear the error state
    if (width <= 0 || height <= 0)
        return VGU_ILLEGAL_ARGUMENT_ERROR;

    static const VGubyte segments[5] = {VG_MOVE_TO | VG_ABSOLUTE,
                                        VG_HLINE_TO | VG_ABSOLUTE,
                                        VG_VLINE_TO | VG_ABSOLUTE,
                                        VG_HLINE_TO | VG_ABSOLUTE,
                                        VG_CLOSE_PATH};
    const VGfloat data[5] = {x, y, x + width, y + height, x};
    append(path, 5, segments, 5, data);

    error = vgGetError();
    if (error == VG_BAD_HANDLE_ERROR)
        return VGU_BAD_HANDLE_ERROR;
    else if (error == VG_PATH_CAPABILITY_ERROR)
        return VGU_PATH_CAPABILITY_ERROR;
    return VGU_NO_ERROR;
}

VGUErrorCode VG_APIENTRY vguRoundRect(VGPath path, VGfloat x, VGfloat y, VGfloat width, VGfloat height, VGfloat arcWidth, VGfloat arcHeight)
{
    VGErrorCode error = vgGetError();    //clear the error state
    if (width <= 0 || height <= 0)
        return VGU_ILLEGAL_ARGUMENT_ERROR;

    arcWidth = VG_CLAMP(arcWidth, 0.0f, width);
    arcHeight = VG_CLAMP(arcHeight, 0.0f, height);

    static const VGubyte segments[10] = {VG_MOVE_TO | VG_ABSOLUTE,
                                         VG_HLINE_TO | VG_ABSOLUTE,
                                         VG_SCCWARC_TO | VG_ABSOLUTE,
                                         VG_VLINE_TO | VG_ABSOLUTE,
                                         VG_SCCWARC_TO | VG_ABSOLUTE,
                                         VG_HLINE_TO | VG_ABSOLUTE,
                                         VG_SCCWARC_TO | VG_ABSOLUTE,
                                         VG_VLINE_TO | VG_ABSOLUTE,
                                         VG_SCCWARC_TO | VG_ABSOLUTE,
                                         VG_CLOSE_PATH};
    const VGfloat data[26] = {x + arcWidth/2, y,
                              x + width - arcWidth/2,
                              arcWidth/2, arcHeight/2, 0, x + width, y + arcHeight/2,
                              y + height - arcHeight/2,
                              arcWidth/2, arcHeight/2, 0, x + width - arcWidth/2, y + height,
                              x + arcWidth/2,
                              arcWidth/2, arcHeight/2, 0, x, y + height - arcHeight/2,
                              y + arcHeight/2,
                              arcWidth/2, arcHeight/2, 0, x + arcWidth/2, y};
    append(path, 10, segments, 26, data);

    error = vgGetError();
    if (error == VG_BAD_HANDLE_ERROR)
        return VGU_BAD_HANDLE_ERROR;
    else if (error == VG_PATH_CAPABILITY_ERROR)
        return VGU_PATH_CAPABILITY_ERROR;
    return VGU_NO_ERROR;
}

VGUErrorCode VG_APIENTRY vguEllipse(VGPath path, VGfloat cx, VGfloat cy, VGfloat width, VGfloat height)
{
    VGErrorCode error = vgGetError();    //clear the error state
    if (width <= 0 || height <= 0)
        return VGU_ILLEGAL_ARGUMENT_ERROR;

    static const VGubyte segments[4] = {VG_MOVE_TO | VG_ABSOLUTE,
                                        VG_SCCWARC_TO | VG_ABSOLUTE,
                                        VG_SCCWARC_TO | VG_ABSOLUTE,
                                        VG_CLOSE_PATH};
    const VGfloat data[12] = {cx + width/2, cy,
                              width/2, height/2, 0, cx - width/2, cy,
                              width/2, height/2, 0, cx + width/2, cy};
    append(path, 4, segments, 12, data);

    error = vgGetError();
    if (error == VG_BAD_HANDLE_ERROR)
        return VGU_BAD_HANDLE_ERROR;
    else if (error == VG_PATH_CAPABILITY_ERROR)
        return VGU_PATH_CAPABILITY_ERROR;
    return VGU_NO_ERROR;
}

VGUErrorCode VG_APIENTRY vguArc(VGPath path, VGfloat x, VGfloat y, VGfloat width, VGfloat height, VGfloat startAngle, VGfloat angleExtent, VGUArcType arcType)
{
    VGErrorCode error = vgGetError();    //clear the error state
    if ((arcType != VGU_ARC_OPEN && arcType != VGU_ARC_CHORD && arcType != VGU_ARC_PIE) || width <= 0.0f || height <= 0.0f)
        return VGU_ILLEGAL_ARGUMENT_ERROR;

    startAngle = VG_DEG_TO_RAD(startAngle);
    angleExtent = VG_DEG_TO_RAD(angleExtent);

    VGfloat w = width/2.0f;
    VGfloat h = height/2.0f;

    VGubyte segments[1];
    VGfloat data[5];

    segments[0] = VG_MOVE_TO | VG_ABSOLUTE;
    data[0] = x + w * (VGfloat)cos(startAngle);
    data[1] = y + h * (VGfloat)sin(startAngle);
    append(path, 1, segments, 2, data);

    data[0] = w;
    data[1] = h;
    data[2] = 0.0f;
    VGfloat endAngle = startAngle + angleExtent;
    if (angleExtent >= 0.0f)
    {
        segments[0] = VG_SCCWARC_TO | VG_ABSOLUTE;
        for (VGfloat a = startAngle + PI;a < endAngle; a += PI)
        {
            data[3] = x + w * (VGfloat)cos(a);
            data[4] = y + h * (VGfloat)sin(a);
            append(path, 1, segments, 5, data);
        }
    }
    else
    {
        segments[0] = VG_SCWARC_TO | VG_ABSOLUTE;
        for (VGfloat a = startAngle - PI;a > endAngle; a -= PI)
        {
            data[3] = x + w * (VGfloat)cos(a);
            data[4] = y + h * (VGfloat)sin(a);
            append(path, 1, segments, 5, data);
        }
    }
    data[3] = x + w * (VGfloat)cos(endAngle);
    data[4] = y + h * (VGfloat)sin(endAngle);
    append(path, 1, segments, 5, data);

    if (arcType == VGU_ARC_CHORD)
    {
        segments[0] = VG_CLOSE_PATH;
        append(path, 1, segments, 0, data);
    }
    else if (arcType == VGU_ARC_PIE)
    {
        segments[0] = VG_LINE_TO | VG_ABSOLUTE;
        data[0] = x;
        data[1] = y;
        append(path, 1, segments, 2, data);
        segments[0] = VG_CLOSE_PATH;
        append(path, 1, segments, 0, data);
    }

    error = vgGetError();
    if (error == VG_BAD_HANDLE_ERROR)
        return VGU_BAD_HANDLE_ERROR;
    else if (error == VG_PATH_CAPABILITY_ERROR)
        return VGU_PATH_CAPABILITY_ERROR;
    return VGU_NO_ERROR;
}

VGUErrorCode VG_APIENTRY vguComputeWarpQuadToSquare(VGfloat sx0, VGfloat sy0, VGfloat sx1, VGfloat sy1, VGfloat sx2, VGfloat sy2, VGfloat sx3, VGfloat sy3, VGfloat * matrix)
{
    if (!matrix || ((VGuintptr)matrix) & 3)
        return VGU_ILLEGAL_ARGUMENT_ERROR;

    VGfloat mat[9];
    VGUErrorCode ret = vguComputeWarpSquareToQuad(sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3, mat);
    if (ret == VGU_BAD_WARP_ERROR)
        return VGU_BAD_WARP_ERROR;

    /* Matrix3x3 m(mat[0], mat[3], mat[6], mat[1], mat[4], mat[7], mat[2], mat[5], mat[8]); */
    Matrix3x3 m;
    initMatrix3x3(&m, mat[0], mat[3], mat[6], mat[1], mat[4], mat[7], mat[2], mat[5], mat[8]);
    Matrix3x3 result = {0};
    inverseMatrix3x3(&result, &m);
#if 0 //TODO
    VGbool nonsingular = m.invert();
    if (!nonsingular)
        return VGU_BAD_WARP_ERROR;
#endif
    matrix[0] = result.matrix[0][0];
    matrix[1] = result.matrix[1][0];
    matrix[2] = result.matrix[2][0];
    matrix[3] = result.matrix[0][1];
    matrix[4] = result.matrix[1][1];
    matrix[5] = result.matrix[2][1];
    matrix[6] = result.matrix[0][2];
    matrix[7] = result.matrix[1][2];
    matrix[8] = result.matrix[2][2];

    return VGU_NO_ERROR;
}

VGUErrorCode VG_APIENTRY vguComputeWarpSquareToQuad(VGfloat dx0, VGfloat dy0, VGfloat dx1, VGfloat dy1, VGfloat dx2, VGfloat dy2, VGfloat dx3, VGfloat dy3, VGfloat * matrix)
{
    if (!matrix || ((VGuintptr)matrix) & 3)
        return VGU_ILLEGAL_ARGUMENT_ERROR;

    //from Heckbert:Fundamentals of Texture Mapping and Image Warping
    //Note that his mapping of vertices is different from OpenVG's
    //(0,0) => (dx0,dy0)
    //(1,0) => (dx1,dy1)
    //(0,1) => (dx2,dy2)
    //(1,1) => (dx3,dy3)

    VGfloat diffx1 = dx1 - dx3;
    VGfloat diffy1 = dy1 - dy3;
    VGfloat diffx2 = dx2 - dx3;
    VGfloat diffy2 = dy2 - dy3;

    VGfloat det = diffx1*diffy2 - diffx2*diffy1;
    if (det == 0.0f)
        return VGU_BAD_WARP_ERROR;

    VGfloat sumx = dx0 - dx1 + dx3 - dx2;
    VGfloat sumy = dy0 - dy1 + dy3 - dy2;

    if (sumx == 0.0f && sumy == 0.0f)
    {    //affine mapping
        matrix[0] = dx1 - dx0;
        matrix[1] = dy1 - dy0;
        matrix[2] = 0.0f;
        matrix[3] = dx3 - dx1;
        matrix[4] = dy3 - dy1;
        matrix[5] = 0.0f;
        matrix[6] = dx0;
        matrix[7] = dy0;
        matrix[8] = 1.0f;
        return VGU_NO_ERROR;
    }

    VGfloat oodet = 1.0f / det;
    VGfloat g = (sumx*diffy2 - diffx2*sumy) * oodet;
    VGfloat h = (diffx1*sumy - sumx*diffy1) * oodet;

    matrix[0] = dx1-dx0+g*dx1;
    matrix[1] = dy1-dy0+g*dy1;
    matrix[2] = g;
    matrix[3] = dx2-dx0+h*dx2;
    matrix[4] = dy2-dy0+h*dy2;
    matrix[5] = h;
    matrix[6] = dx0;
    matrix[7] = dy0;
    matrix[8] = 1.0f;
    return VGU_NO_ERROR;
}

VGUErrorCode VG_APIENTRY vguComputeWarpQuadToQuad(VGfloat dx0, VGfloat dy0, VGfloat dx1, VGfloat dy1, VGfloat dx2, VGfloat dy2, VGfloat dx3, VGfloat dy3, VGfloat sx0, VGfloat sy0, VGfloat sx1, VGfloat sy1, VGfloat sx2, VGfloat sy2, VGfloat sx3, VGfloat sy3, VGfloat * matrix)
{
    if (!matrix || ((VGuintptr)matrix) & 3)
        return VGU_ILLEGAL_ARGUMENT_ERROR;

    VGfloat qtos[9];
    VGUErrorCode ret1 = vguComputeWarpQuadToSquare(sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3, qtos);
    if (ret1 == VGU_BAD_WARP_ERROR)
        return VGU_BAD_WARP_ERROR;

    VGfloat stoq[9];
    VGUErrorCode ret2 = vguComputeWarpSquareToQuad(dx0, dy0, dx1, dy1, dx2, dy2, dx3, dy3, stoq);
    if (ret2 == VGU_BAD_WARP_ERROR)
        return VGU_BAD_WARP_ERROR;

    /* Matrix3x3 m1(qtos[0], qtos[3], qtos[6], qtos[1], qtos[4], qtos[7], qtos[2], qtos[5], qtos[8]); */
    Matrix3x3 m1;
    initMatrix3x3(&m1, qtos[0], qtos[3], qtos[6], qtos[1], qtos[4], qtos[7], qtos[2], qtos[5], qtos[8]);

    /* Matrix3x3 m2(stoq[0], stoq[3], stoq[6], stoq[1], stoq[4], stoq[7], stoq[2], stoq[5], stoq[8]); */
    Matrix3x3 m2;
    initMatrix3x3(&m2, stoq[0], stoq[3], stoq[6], stoq[1], stoq[4], stoq[7], stoq[2], stoq[5], stoq[8]);

    /* Matrix3x3 r = m2 * m1; */
    Matrix3x3 r;
    matrixMultiply(&r, &m2, &m1);

    matrix[0] = r.matrix[0][0];
    matrix[1] = r.matrix[1][0];
    matrix[2] = r.matrix[2][0];
    matrix[3] = r.matrix[0][1];
    matrix[4] = r.matrix[1][1];
    matrix[5] = r.matrix[2][1];
    matrix[6] = r.matrix[0][2];
    matrix[7] = r.matrix[1][2];
    matrix[8] = r.matrix[2][2];

    return VGU_NO_ERROR;
}
