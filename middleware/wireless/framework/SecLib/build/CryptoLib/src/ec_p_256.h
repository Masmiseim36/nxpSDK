/*! *********************************************************************************
 * \defgroup ECDH
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
 * \file
 *
 * Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 * NXP Confidential Proprietary
 *
 * No part of this document must be reproduced in any form - including copied,
 * transcribed, printed or by any electronic means - without specific written
 * permission from NXP.
 ********************************************************************************** */

#ifndef _EC_P_256_H
#define _EC_P_256_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"
#include "CryptoLibSW.h"

/************************************************************************************
*************************************************************************************
* Public constants and macros
*************************************************************************************
************************************************************************************/
#define mUseBigIntMacros_d         0
#define mUseImprovedDoubleAndAdd_d 0

#define SIZE      (8u * sizeof(uint32_t))
#define LOW_MASK  ((((uint32_t)1u) << (SIZE / 2u)) - 1u)
#define HIGH_HALF (SIZE / 2u)

#define KEY_UINT8_ARRAY_LENGTH  32u
#define KEY_UINT32_ARRAY_LENGTH 8u

extern const uint32_t P256_g_point_x[8];
extern const uint32_t P256_g_point_y[8];
extern const uint32_t P256_orderMinusOne[8];
extern const uint32_t P256_one[8];
extern const uint32_t P256_prime_m[8];
extern const uint32_t P256_prime_r[8];
extern const uint32_t P256_order_m[9];

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef struct multiplicationCtx_tag
{
    uint32_t *temp1, *temp2, *temp3, *temp4, *temp5, *temp6;
    uint32_t *pR0x, *pR0y, *pR0z;
    uint32_t *multiplicationTemp;
} multiplicationCtx_t;

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void EcP256_FieldAdd(const uint32_t *x, const uint32_t *y, const uint32_t *reducer, uint32_t *result);
void EcP256_FieldSub(const uint32_t *x, const uint32_t *y, const uint32_t *modulus, uint32_t *result);
void EcP256_FieldMult(const uint32_t *x, const uint32_t *y, uint32_t *result, uint8_t length);

void EcP256_SubModN(const uint32_t *x, const uint32_t *y, uint32_t *result);

void EcP256_FieldModP(uint32_t *r, const uint32_t *c);
#ifdef _keepUnused
void EcP256_FieldModO(const uint32_t *A, uint32_t *result, uint8_t length);
#endif
void EcP256_FieldInv(const uint32_t *A, uint32_t *B);

void EcP256_PointMult(const uint32_t *px,
                      const uint32_t *py,
                      const uint32_t *secret,
                      uint32_t *      resultx,
                      uint32_t *      resulty,
                      void *          pMultiplicationBuffer);
void EcP256_PointMultSeg(computeDhKeyParam_t *pDhKeyData);
#ifdef _keepUnused
void EcP256_PointAdd(
    const uint32_t *px, const uint32_t *py, const uint32_t *qx, const uint32_t *qy, uint32_t *Sx, uint32_t *Sy);
void EcP256_PointSub(
    const uint32_t *px, const uint32_t *py, const uint32_t *qx, const uint32_t *qy, uint32_t *Sx, uint32_t *Sy);
void EcP256_PointDouble(const uint32_t *px, const uint32_t *py, uint32_t *Dx, uint32_t *Dy);
#endif
bool_t EcP256_IsPointOnCurve(const uint32_t *X, const uint32_t *Y);

#ifdef __cplusplus
}
#endif

#endif /* _EC_P_256_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
