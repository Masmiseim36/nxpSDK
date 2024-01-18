/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************
   INCLUDE FILES
*************************************************************************/

#include "PL_platformTypesPrivate.h"

/************************************************************************/
/*                                                                      */
/*    Define code section                                               */
/*                                                                      */
/************************************************************************/
#define ProcessSection
#define LibVectorArithmetic
// #include     "LVC_CodeSectionDef.h"

/**********************************************************************************
   FUNCTION COPY_PL_INT8
***********************************************************************************/

void PL_Copy_INT8(const PL_INT8 *src, PL_INT8 *dst, PL_UINT32 n)
{
    PL_UINT32 ii;

    if (src > dst)
    {
        for (ii = n; ii > 0; ii--)
        {
            *dst = *src;
            dst++;
            src++;
        }
    }
    else
    {
        src += n - 1;
        dst += n - 1;
        for (ii = n; ii > 0; ii--)
        {
            *dst = *src;
            dst--;
            src--;
        }
    }

    return;
}

/**********************************************************************************
   FUNCTION COPY_PL_INT16
***********************************************************************************/
void PL_Copy_INT16(const PL_INT16 *src, PL_INT16 *dst, PL_UINT32 n)
{
    PL_UINT32 ii;

    if (src > dst)
    {
        for (ii = n; ii > 0; ii--)
        {
            *dst = *src;
            dst++;
            src++;
        }
    }
    else
    {
        src += n - 1;
        dst += n - 1;
        for (ii = n; ii > 0; ii--)
        {
            *dst = *src;
            dst--;
            src--;
        }
    }

    return;
}

/************************************************************************
   FUNCTION COPY_PL_INT32
*************************************************************************/

void PL_Copy_INT32(const PL_INT32 *pSrc, PL_INT32 *pDst, PL_UINT32 n)
{
    PL_UINT32 ii;

    if (pSrc >= pDst)
    {
        for (ii = n; ii > 0; ii--)
        {
            *pDst = *pSrc;
            pDst++;
            pSrc++;
        }
    }
    else
    {
        pSrc += (n - 1);
        pDst += (n - 1);
        for (ii = n; ii > 0; ii--)
        {
            *pDst = *pSrc;
            pDst--;
            pSrc--;
        }
    }

    return;
}

/************************************************************************/
