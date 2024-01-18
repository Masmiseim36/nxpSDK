/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PL_MEM_COPY_H
#define PL_MEM_COPY_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************************/
/*                                                                                      */
/*  Standard Enumerated types                                                           */
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*  Standard function interfaces                                                        */
/*                                                                                      */
/****************************************************************************************/

void PL_Copy_INT8(const PL_INT8 *src, PL_INT8 *dst, PL_UINT32 n);

void PL_Copy_INT16(const PL_INT16 *src, PL_INT16 *dst, PL_UINT32 n);

void PL_Copy_INT32(const PL_INT32 *pSrc, PL_INT32 *pDst, PL_UINT32 n);

/****************************************************************************************/
/*                                                                                      */
/*  End of file                                                                         */
/*                                                                                      */
/****************************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PL_MEM_COPY_H */
