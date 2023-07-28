/*! *********************************************************************************
 * \defgroup ECDH
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
 * \file
 *
 * Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018, 2022 NXP
 *
 * NXP Confidential Proprietary
 *
 * No part of this document must be reproduced in any form - including copied,
 * transcribed, printed or by any electronic means - without specific written
 * permission from NXP.
 ********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ec_p_256.h"
#include "SecLib.h"

#if (!defined(mUseBigIntMacros_d) || (mUseBigIntMacros_d == 0))
#include "big_integer.h"

/************************************************************************************
*************************************************************************************
* Public constants
*************************************************************************************
************************************************************************************/
/* FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF */
const uint32_t P256_prime_m[8] = {0xffffffffu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                                  0x00000000u, 0x00000000u, 0x00000001u, 0xffffffffu};

/* This is added after an static byte addition if the answer has a carry in MSB*/
const uint32_t P256_prime_r[8] = {0x00000001u, 0x00000000u, 0x00000000u, 0xffffffffu,
                                  0xffffffffu, 0xffffffffu, 0xfffffffeu, 0x00000000u};

/* ffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551 */
const uint32_t P256_order_m[9] = {0xFC632551u, 0xF3B9CAC2u, 0xA7179E84u, 0xBCE6FAADu, 0xFFFFFFFFu,
                                  0xFFFFFFFFu, 0x00000000u, 0xFFFFFFFFu, 0x00000000u};

const uint32_t P256_g_point_x[8]     = {0xD898C296u, 0xF4A13945u, 0x2DEB33A0u, 0x77037D81u,
                                    0x63A440F2u, 0xF8BCE6E5u, 0xE12C4247u, 0x6B17D1F2u};
const uint32_t P256_g_point_y[8]     = {0x37BF51F5u, 0xCBB64068u, 0x6B315ECEu, 0x2BCE3357u,
                                    0x7C0F9E16u, 0x8EE7EB4Au, 0xFE1A7F9Bu, 0x4FE342E2u};
const uint32_t P256_orderMinusOne[8] = {0xFC632550u, 0xF3B9CAC2u, 0xA7179E84u, 0xBCE6FAADu,
                                        0xFFFFFFFFu, 0xFFFFFFFFu, 0x00000000u, 0xFFFFFFFFu};

const uint32_t P256_one[8] = {0x00000001u, 0x00000000u, 0x00000000u, 0x00000000u,
                              0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u};

extern secLibCallback_t pfSecLibMultCallback;

/************************************************************************************
*************************************************************************************
* Private constants
*************************************************************************************
************************************************************************************/
#ifdef _KeepUnused
static const uint32_t P256_order_mu[9] = {0xEEDF9BFEu, 0x012FFD85u, 0xDF1A6C21u, 0x43190552u, 0xFFFFFFFFu,
                                          0xFFFFFFFEu, 0xFFFFFFFFu, 0x00000000u, 0x00000001u};

static const uint8_t P256_order_k = 8;
#endif

/* Parameter 'a' of the elliptic curve, equal to -3 mod p */
static const uint32_t P256_curve_a[8] = {0xfffffffcu, 0xffffffffu, 0xffffffffu, 0x00000000u,
                                         0x00000000u, 0x00000000u, 0x00000001u, 0xffffffffu};

/* Parameter 'b' of the elliptic curve. */
static const uint32_t P256_curve_b[8] = {0x27d2604bu, 0x3bce3c3eu, 0xcc53b0f6u, 0x651d06b0u,
                                         0x769886bcu, 0xb3ebbd55u, 0xaa3a93e7u, 0x5ac635d8u};

/************************************************************************************
*************************************************************************************
* Private function prototypes
*************************************************************************************
************************************************************************************/
static void ec_JacobiAdd(const uint32_t *qx, const uint32_t *qy, multiplicationCtx_t *ctx);
static void ec_JacobiDouble(multiplicationCtx_t *ctx);

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

void EcP256_FieldAdd(const uint32_t *x, const uint32_t *y, const uint32_t *reducer, uint32_t *result)
{
    if (Bi_Add(x, y, result, KEY_UINT32_ARRAY_LENGTH) != 0u)
    { // add prime if carry is still set!
        (void)Bi_Add(result, reducer, result, KEY_UINT32_ARRAY_LENGTH);
    }
}

void EcP256_FieldSub(const uint32_t *x, const uint32_t *y, const uint32_t *modulus, uint32_t *result)
{
    if (Bi_Sub(x, y, result, KEY_UINT32_ARRAY_LENGTH) != 0u)
    { // add modulus if carry is set
        (void)Bi_Add(result, modulus, result, KEY_UINT32_ARRAY_LENGTH);
    }
}

/*
 *
 * INPUT: Integers a,b ? [0, p-1].
   OUTPUT: c = a * b.
    1. R0=0, R1=0, R2=0.
    2. For k from 0 to 2t -2 do
        2.1 For each element of {(i, j ) | i + j = k, 0 = i, j = t -1} do
            (UV)=A[i] * B[ j ].
            (e, R0)=R0 +V .
            (e, R1)=R1 +U +e.
            R2 = R2 +e.
        2.2 C[k]= R0, R0=R1, R1=R2, R2=0.
    3. c[2t -1]=R0.
    4. Return(c).
 *
 */
void EcP256_FieldMult(const uint32_t *x, const uint32_t *y, uint32_t *result, uint8_t length)
{
    uint32_t r0 = 0u, r1 = 0u, r2 = 0u, p0, p1, t0;
    uint32_t xhi, xlo, yhi, ylo;
    uint32_t k, i, j, klimit, jlimit;

    klimit = (uint32_t)length * 2u;

    for (k = 0; k < klimit; k++)
    {
        jlimit = (uint32_t)length - 1u;
        i      = (k <= ((uint32_t)length - 1u)) ? k : ((uint32_t)length - 1u);
        j      = k - i;

        if (k <= ((uint32_t)length - 1u))
        {
            jlimit = k;
        }

        while (j <= jlimit)
        {
            xhi = x[i] >> HIGH_HALF;
            xlo = x[i] & LOW_MASK;
            yhi = y[j] >> HIGH_HALF;
            ylo = y[j] & LOW_MASK;

            p0  = ylo * xlo;
            p1  = yhi * xlo;
            t0  = ylo * xhi;
            xhi = yhi * xhi;

            p1 += t0;
            if (p1 < t0)
            {
                xhi += ((uint32_t)1u << HIGH_HALF);
            }
            xhi += (p1 >> HIGH_HALF);
            t0  = p1 << HIGH_HALF;
            xlo = p0 + t0;
            if (xlo < t0)
            {
                xhi++;
            }
            r0 += xlo;
            if (r0 < xlo)
            {
                xhi++;
            }
            r1 += xhi;
            if (r1 < xhi)
            {
                r2++;
            }

            i--;
            j++;
        }
        result[k] = r0;
        r0        = r1;
        r1        = r2;
        r2        = 0;
    }
}

void EcP256_SubModN(const uint32_t *x, const uint32_t *y, uint32_t *result)
{
    EcP256_FieldSub(x, y, P256_order_m, result);
}

#ifdef _keepUnused
void EcP256_FieldModO(const uint32_t *A, uint32_t *result, uint8_t length)
{
    // This is used for value q1 and q3
    uint32_t q1_q3[9];
    // This is used for q2 and a temp var
    uint32_t q2_tmp[18];
    uint32_t tmpresult[10];

    // return if the given value is smaller than the modulus
    if (length == KEY_UINT32_ARRAY_LENGTH && Bi_Compare(A, P256_order_m, KEY_UINT32_ARRAY_LENGTH) <= 0)
    {
        if (A != result)
        {
            Bi_Copy(A, result, length);
        }
    }
    else
    {
        Bi_RightShiftBy(A, length, q1_q3, 9, P256_order_k - 1u);

        EcP256_FieldMult(P256_order_mu, q1_q3, q2_tmp, 9);

        Bi_RightShiftBy(q2_tmp, 18, q1_q3, 8, P256_order_k + 1u);

        // r1 = first 9 blocks of A

        EcP256_FieldMult(q1_q3, P256_order_m, q2_tmp, 8);

        // r2 = first 9 blocks of q2_tmp

        (void)Bi_Sub(A, q2_tmp, tmpresult, 9);

        while (Bi_Compare(tmpresult, P256_order_m, 9) >= 0)
        {
            (void)Bi_Sub(tmpresult, P256_order_m, tmpresult, 9);
        }
        Bi_Copy(tmpresult, result, 8);
    }
}
#endif

void EcP256_FieldModP(uint32_t *r, const uint32_t *c)
{
    uint32_t s[8];

    /* r = s1 */
    Bi_Copy(c, r, KEY_UINT32_ARRAY_LENGTH);

    /* r += 2 * s2 */
    s[0] = 0;
    s[1] = 0;
    s[2] = 0;
    s[3] = c[11];
    s[4] = c[12];
    s[5] = c[13];
    s[6] = c[14];
    s[7] = c[15];

    EcP256_FieldAdd(r, s, P256_prime_r, r);
    EcP256_FieldAdd(r, s, P256_prime_r, r);

    /* r += 2 * s3 */
    s[7] = 0; /* s[0] = s[1] = s[2] = 0; already done before */
    s[3] = c[12];
    s[4] = c[13];
    s[5] = c[14];
    s[6] = c[15];

    EcP256_FieldAdd(r, s, P256_prime_r, r);
    EcP256_FieldAdd(r, s, P256_prime_r, r);

    /* r += s4 */
    s[7] = c[15];
    s[6] = c[14];
    s[5] = 0;
    s[4] = 0;
    s[3] = 0;
    s[2] = c[10];
    s[1] = c[9];
    s[0] = c[8];
    EcP256_FieldAdd(r, s, P256_prime_r, r);

    /* r -= s6 */
    s[7] = c[10];
    s[6] = c[8];
    /* s[5] = s[4] = s[3] = 0; already done before */
    s[2] = c[13];
    s[1] = c[12];
    s[0] = c[11];
    EcP256_FieldSub(r, s, P256_prime_m, r);

    /* r += s5 */
    s[0] = c[9];
    s[1] = c[10];
    s[2] = c[11];
    s[3] = c[13];
    s[4] = c[14];
    s[5] = c[15];
    s[6] = c[13];
    s[7] = c[8];
    EcP256_FieldAdd(r, s, P256_prime_r, r);

    /* r -= s7 */
    s[7] = c[11];
    s[6] = c[9];
    s[5] = 0;
    s[4] = 0;
    s[0] = c[12];
    s[1] = c[13];
    s[2] = c[14];
    s[3] = c[15];

    EcP256_FieldSub(r, s, P256_prime_m, r);

    /* r -= s8 */
    s[7] = c[12];
    s[6] = 0;
    s[5] = c[10];
    s[4] = c[9];
    s[3] = c[8];
    s[2] = c[15];
    s[1] = c[14];
    s[0] = c[13];
    EcP256_FieldSub(r, s, P256_prime_m, r);

    /* r -= s9 */
    s[7] = c[13];
    s[6] = 0;
    s[2] = 0;
    s[5] = c[11];
    s[4] = c[10];
    s[3] = c[9];
    s[1] = c[15];
    s[0] = c[14];
    EcP256_FieldSub(r, s, P256_prime_m, r);
}

void EcP256_FieldInv(const uint32_t *A, uint32_t *B)
{
    uint32_t u[8], v[8], x1[8], x2[8], n;

    Bi_Copy(A, u, KEY_UINT32_ARRAY_LENGTH);
    Bi_Copy(P256_prime_m, v, KEY_UINT32_ARRAY_LENGTH);
    Bi_SetZero((uint32_t *)x1, 8);
    Bi_SetZero((uint32_t *)x2, 8);
    x1[0] = 1;
    /* While u !=1 and v !=1 */
    while (!((Bi_IsOne(u) != 0) || (Bi_IsOne(v) != 0)))
    {
        while ((u[0] & 1u) == 0u) /* While u is even */
        {
            Bi_RightShift(u); /* divide by 2 */
            if ((x1[0] & 1u) == 0u)
            {                      /*ifx1iseven*/
                Bi_RightShift(x1); /* Divide by 2 */
            }
            else
            {
                n = Bi_Add(x1, P256_prime_m, x1, KEY_UINT32_ARRAY_LENGTH);
                Bi_RightShift(x1);
                x1[7] |= (n << (SIZE - 1u));
            }
        }
        while ((v[0] & 1u) == 0u)
        {                           /* While v is even */
            Bi_RightShift(v);       /* divide by 2 */
            if ((x2[0] & 1u) == 0u) /*ifx1iseven*/
            {
                Bi_RightShift(x2); /* Divide by 2 */
            }
            else
            {
                n = Bi_Add(x2, P256_prime_m, x2, KEY_UINT32_ARRAY_LENGTH);
                Bi_RightShift(x2);
                x2[7] |= (n << (SIZE - 1u));
            }
        }
        if (Bi_Compare(u, v, KEY_UINT32_ARRAY_LENGTH) >= 0)
        {                                                   /* If u > 0 */
            (void)Bi_Sub(u, v, u, KEY_UINT32_ARRAY_LENGTH); /* u=u-v */
            EcP256_FieldSub(x1, x2, P256_prime_m, x1);      /* x1=x1-x2 */
        }
        else
        {
            (void)Bi_Sub(v, u, v, KEY_UINT32_ARRAY_LENGTH); /* v=v-u */
            EcP256_FieldSub(x2, x1, P256_prime_m, x2);      /* x2=x2-x1 */
        }
    }
    if (Bi_IsOne(u) != 0)
    {
        Bi_Copy(x1, B, KEY_UINT32_ARRAY_LENGTH);
    }
    else
    {
        Bi_Copy(x2, B, KEY_UINT32_ARRAY_LENGTH);
    }
}

#ifdef gTestEcP256
void EcP256_PointDouble(const uint32_t *px, const uint32_t *py, uint32_t *Dx, uint32_t *Dy)
{
    uint32_t tempA[8];
    uint32_t tempB[8];
    uint32_t tempC[8];
    uint32_t tempD[16];

    if ((Bi_IsZero(px) != 0) && (Bi_IsZero(py) != 0))
    {
        Bi_Copy(px, Dx, KEY_UINT32_ARRAY_LENGTH);
        Bi_Copy(py, Dy, KEY_UINT32_ARRAY_LENGTH);
    }
    else
    {
        EcP256_FieldMult(px, px, tempD, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(tempA, tempD);
        Bi_SetZero(tempB, 8);
        tempB[0] = 0x00000001;
        EcP256_FieldSub(tempA, tempB, P256_prime_m, tempC); // tempC = (qx^2-1)
        tempB[0] = 0x00000003;
        EcP256_FieldMult(tempC, tempB, tempD, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldMult(tempC, tempB, tempD, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(tempA, tempD);                                 // tempA = 3*(qx^2-1)
        EcP256_FieldAdd(py, py, P256_prime_r, tempB);                   // tempB = 2*qy
        EcP256_FieldInv(tempB, tempC);                                  // tempC = 1/(2*qy)
        EcP256_FieldMult(tempA, tempC, tempD, KEY_UINT32_ARRAY_LENGTH); // tempB = lambda = (3*(qx^2-1))/(2*qy)
        EcP256_FieldModP(tempB, tempD);

        EcP256_FieldMult(tempB, tempB, tempD, KEY_UINT32_ARRAY_LENGTH); // tempC = lambda^2
        EcP256_FieldModP(tempC, tempD);
        EcP256_FieldSub(tempC, px, P256_prime_m, tempA); // lambda^2 - Px
        EcP256_FieldSub(tempA, px, P256_prime_m, Dx);    // lambda^2 - Px - Qx

        EcP256_FieldSub(px, Dx, P256_prime_m, tempA);                   // tempA = qx-dx
        EcP256_FieldMult(tempB, tempA, tempD, KEY_UINT32_ARRAY_LENGTH); // tempC = lambda * (qx-dx)
        EcP256_FieldModP(tempC, tempD);
        EcP256_FieldSub(tempC, py, P256_prime_m, Dy); // Dy = lambda * (qx-dx) - px
    }
}

void EcP256_PointAdd(
    const uint32_t *px, const uint32_t *py, const uint32_t *qx, const uint32_t *qy, uint32_t *Sx, uint32_t *Sy)
{
    uint32_t tempA[8];
    uint32_t tempB[8];
    uint32_t tempC[8];
    uint32_t tempD[16];

    if ((Bi_IsZero(px) != 0) && (Bi_IsZero(py) != 0))
    {
        Bi_Copy(qx, Sx, KEY_UINT32_ARRAY_LENGTH);
        Bi_Copy(qy, Sy, KEY_UINT32_ARRAY_LENGTH);
    }
    else
    {
        if ((Bi_IsZero(qx) != 0) && (Bi_IsZero(qy) != 0))
        {
            Bi_Copy(px, Sx, KEY_UINT32_ARRAY_LENGTH);
            Bi_Copy(py, Sy, KEY_UINT32_ARRAY_LENGTH);
        }
        else
        {
            if (Bi_IsSame(px, qx, KEY_UINT32_ARRAY_LENGTH) != 0)
            {
                if (Bi_IsSame(py, qy, KEY_UINT32_ARRAY_LENGTH) == 0)
                {
                    Bi_SetZero(Sx, 8);
                    Bi_SetZero(Sy, 8);
                }
                else
                {
                    EcP256_PointDouble(px, py, Sx, Sy);
                }
            }
            else
            {
                EcP256_FieldSub(py, qy, P256_prime_m, tempA);
                EcP256_FieldSub(px, qx, P256_prime_m, tempB);
                EcP256_FieldInv(tempB, tempB);
                EcP256_FieldMult(tempA, tempB, tempD, KEY_UINT32_ARRAY_LENGTH);
                EcP256_FieldModP(tempC, tempD); // tempC = lambda

                EcP256_FieldMult(tempC, tempC, tempD, KEY_UINT32_ARRAY_LENGTH); // tempA = lambda^2
                EcP256_FieldModP(tempA, tempD);
                EcP256_FieldSub(tempA, px, P256_prime_m, tempB); // lambda^2 - Px
                EcP256_FieldSub(tempB, qx, P256_prime_m, Sx);    // lambda^2 - Px - Qx

                EcP256_FieldSub(qx, Sx, P256_prime_m, tempB);
                EcP256_FieldMult(tempC, tempB, tempD, KEY_UINT32_ARRAY_LENGTH);
                EcP256_FieldModP(tempC, tempD);
                EcP256_FieldSub(tempC, qy, P256_prime_m, Sy);
            }
        }
    }
}
#endif

/* Jacobian point scalar multiplication
 *
 * Input P in affine coordinates and secret k
 * Output k*P in affine coordinates
 *
 * The point P is transformed in Jacobian coordinates. Will need R0 and R1
 * as temporary points to compute kP. R0 will be 0 and R1 will be P in
 * Jacobian coordinates.
 *
 * The secret k is used as its binary form and the following operations will be
 * executed:
 *      1. For every bit of k R0 <- 2*R0
 *      2. If ki (ith bit of k) is 1, then R0 <- R0 + R1
 *
 * At the end of the computation R0 will hold the result of k*P
 *
 */
void EcP256_PointMult(const uint32_t *px,
                      const uint32_t *py,
                      const uint32_t *secret,
                      uint32_t *      resultx,
                      uint32_t *      resulty,
                      void *          pMultiplicationBuffer)
{
    multiplicationCtx_t *buffer = (multiplicationCtx_t *)pMultiplicationBuffer;

    uint32_t *pointer          = (uint32_t *)((uint32_t)buffer + sizeof(multiplicationCtx_t));
    buffer->multiplicationTemp = pointer;
    pointer += 2u * KEY_UINT32_ARRAY_LENGTH;
    buffer->temp1 = pointer;
    pointer += KEY_UINT32_ARRAY_LENGTH;
    buffer->temp2 = pointer;
    pointer += KEY_UINT32_ARRAY_LENGTH;
    buffer->temp3 = pointer;
    pointer += KEY_UINT32_ARRAY_LENGTH;
    buffer->temp4 = pointer;
    pointer += KEY_UINT32_ARRAY_LENGTH;
    buffer->temp5 = pointer;
    pointer += KEY_UINT32_ARRAY_LENGTH;
    buffer->temp6 = pointer;
    pointer += KEY_UINT32_ARRAY_LENGTH;
    buffer->pR0x = pointer;
    pointer += KEY_UINT32_ARRAY_LENGTH;
    buffer->pR0y = pointer;
    pointer += KEY_UINT32_ARRAY_LENGTH;
    buffer->pR0z = pointer;

    int16_t i;

    Bi_SetZero(buffer->pR0x, (int)KEY_UINT32_ARRAY_LENGTH);
    Bi_SetZero(buffer->pR0y, (int)KEY_UINT32_ARRAY_LENGTH);
    Bi_SetZero(buffer->pR0z, (int)KEY_UINT32_ARRAY_LENGTH);
#if !mUseImprovedDoubleAndAdd_d
    for (i = 255; i >= 0; i--)
    {
        ec_JacobiDouble(buffer);

        if ((((secret[i / 32]) >> ((uint16_t)i % 32u)) & 0x01u) != 0u)
        {
            ec_JacobiAdd(px, py, buffer);
        }
    }
#else

#endif

    EcP256_FieldInv(buffer->pR0z, buffer->temp1); // pTempZ = 1/Z

    EcP256_FieldMult(buffer->pR0x, buffer->temp1, buffer->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
    EcP256_FieldModP(buffer->pR0x, buffer->multiplicationTemp); // pR0x = R0x/Z
    EcP256_FieldMult(buffer->pR0x, buffer->temp1, buffer->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
    EcP256_FieldModP(resultx, buffer->multiplicationTemp); // resultx = R0x/Z^2

    EcP256_FieldMult(buffer->pR0y, buffer->temp1, buffer->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
    EcP256_FieldModP(buffer->pR0y, buffer->multiplicationTemp);
    EcP256_FieldMult(buffer->pR0y, buffer->temp1, buffer->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
    EcP256_FieldModP(buffer->pR0y, buffer->multiplicationTemp);
    EcP256_FieldMult(buffer->pR0y, buffer->temp1, buffer->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
    EcP256_FieldModP(resulty, buffer->multiplicationTemp);
}

/************************************************************************************
 * \brief This function is used to compute the one intermediate step in a Jacobi
 *        multiplication. It functions the same as \ref EcP256_PointMult function.
 *
 * \param[in]  pData Pointer to the structure holding information about the
 *                   multiplication
 ************************************************************************************/
void Ecdh_ComputeJacobiChunk(uint8_t index, uint8_t stepSize, computeDhKeyParam_t *pData)
{
    /* Update for each step */
    for (; stepSize >= 1u; stepSize--)
    {
        ec_JacobiDouble(pData->pWorkBuffer);

        /* Get index in the private key by iterating using 32 bits */
        if ((((pData->privateKey.raw_32bit[index / 32u]) >> (index % 32u)) & 0x01u) != 0u)
        {
            ec_JacobiAdd((uint32_t *)pData->peerPublicKey.components_32bit.x,
                         (uint32_t *)pData->peerPublicKey.components_32bit.y, pData->pWorkBuffer);
        }
        index--;
    }
}

/************************************************************************************
 * \brief This function is used to compute the last step in a Jacobi multiplication.
 *        It functions the same as the last part of \ref EcP256_PointMult function.
 *
 * \param[in]  pData Pointer to the structure holding information about the
 *                   multiplication
 ************************************************************************************/
void Ecdh_JacobiCompleteMult(computeDhKeyParam_t *pData)
{
    multiplicationCtx_t *buffer = (multiplicationCtx_t *)pData->pWorkBuffer;

    EcP256_FieldInv(buffer->pR0z, buffer->temp1); // pTempZ = 1/Z

    EcP256_FieldMult(buffer->pR0x, buffer->temp1, buffer->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);

    EcP256_FieldModP(buffer->pR0x, buffer->multiplicationTemp); // pR0x = R0x/Z

    EcP256_FieldMult(buffer->pR0x, buffer->temp1, buffer->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
    EcP256_FieldModP((uint32_t *)pData->outPoint.components_32bit.x,
                     buffer->multiplicationTemp); // pData->outX = R0x/Z^2

    EcP256_FieldMult(buffer->pR0y, buffer->temp1, buffer->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
    EcP256_FieldModP(buffer->pR0y, buffer->multiplicationTemp);
    EcP256_FieldMult(buffer->pR0y, buffer->temp1, buffer->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
    EcP256_FieldModP(buffer->pR0y, buffer->multiplicationTemp);
    EcP256_FieldMult(buffer->pR0y, buffer->temp1, buffer->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
    EcP256_FieldModP((uint32_t *)pData->outPoint.components_32bit.y, buffer->multiplicationTemp);
}

/************************************************************************************
 * \brief This function is used to compute the first step in a Jacobi multipication.
 *        It functions the same as the first part of \ref EcP256_PointMult function.
 *
 * \param[in]  pData Pointer to the structure holding information about the
 *                   multiplication
 ************************************************************************************/
void EcP256_PointMultSeg(computeDhKeyParam_t *pDhKeyData)
{
    if (NULL != pDhKeyData->pWorkBuffer)
    {
        /* Store the previous allocated working buffer in a temporary pointer */
        multiplicationCtx_t *buffer = (multiplicationCtx_t *)pDhKeyData->pWorkBuffer;

        /* Keep a temporary pointer after the header of the buffer */
        uint32_t *pointer = (uint32_t *)((uint32_t)buffer + sizeof(multiplicationCtx_t));

        /* Setup pointers inside the buffer */
        buffer->multiplicationTemp = pointer;
        pointer += 2u * KEY_UINT32_ARRAY_LENGTH;
        buffer->temp1 = pointer;
        pointer += KEY_UINT32_ARRAY_LENGTH;
        buffer->temp2 = pointer;
        pointer += KEY_UINT32_ARRAY_LENGTH;
        buffer->temp3 = pointer;
        pointer += KEY_UINT32_ARRAY_LENGTH;
        buffer->temp4 = pointer;
        pointer += KEY_UINT32_ARRAY_LENGTH;
        buffer->temp5 = pointer;
        pointer += KEY_UINT32_ARRAY_LENGTH;
        buffer->temp6 = pointer;
        pointer += KEY_UINT32_ARRAY_LENGTH;
        buffer->pR0x = pointer;
        pointer += KEY_UINT32_ARRAY_LENGTH;
        buffer->pR0y = pointer;
        pointer += KEY_UINT32_ARRAY_LENGTH;
        buffer->pR0z = pointer;

        /* Clear the XYZ buffers */
        Bi_SetZero(buffer->pR0x, (int)KEY_UINT32_ARRAY_LENGTH);
        Bi_SetZero(buffer->pR0y, (int)KEY_UINT32_ARRAY_LENGTH);
        Bi_SetZero(buffer->pR0z, (int)KEY_UINT32_ARRAY_LENGTH);

        /* Initiate the steps */
        pDhKeyData->procStep = 0;
        if (pfSecLibMultCallback != NULL)
        {
            /* Send app calls to execute each intermmediate step */
            pfSecLibMultCallback(pDhKeyData);
        }
    }
}

#ifdef gTestEcP256
/*
 * This exports the low level functions so the tests can use them.
 * In real use the compiler is now bale to optimice the code better.
 */
void EcP256_PointSub(
    const uint32_t *px, const uint32_t *py, const uint32_t *qx, const uint32_t *qy, uint32_t *Sx, uint32_t *Sy)
{
    uint32_t tmp[8];
    Bi_SetZero(tmp, 8);

    /* -Q=(qx, -qy) */
    EcP256_FieldSub(&tmp[0], qy, P256_prime_m, &tmp[0]);
    EcP256_PointAdd(px, py, qx, &tmp[0], Sx, Sy);
}
#endif
/*
 * Validate a point (public key) by directly checking the curve equation.
 * A valid public key (X, Y) is one where X and Y are both in the range 0 to p - 1 and satisfy the
 * equation (Y)^2 = (X)^3 + aX + b (mod p)
 */
bool_t EcP256_IsPointOnCurve(const uint32_t *X, const uint32_t *Y)
{
    uint32_t lhs[16], rhs[16], modulus[KEY_UINT32_ARRAY_LENGTH];
    bool_t   bIsValid = TRUE;

    /* Check whether X and Y are both in the range 0 to p - 1 */
    if ((Bi_Compare(X, P256_prime_m, KEY_UINT32_ARRAY_LENGTH) >= 0) ||
        (Bi_Compare(Y, P256_prime_m, KEY_UINT32_ARRAY_LENGTH) >= 0))
    {
        bIsValid = FALSE;
    }

    if (bIsValid)
    {
        /*
         * Compute (X)^3 + aX + b (mod p)
         * Use lhs as intermediary variable for (X)^2 + a
         */

        /* modulus = (X)^2 (mod p) */
        EcP256_FieldMult(X, X, (uint32_t *)lhs, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(modulus, lhs);
        Bi_SetZero(lhs, 16);

        /* lhs = modulus + a = (X)^2 + a (mod p) */
        EcP256_FieldAdd(modulus, P256_curve_a, P256_prime_r, lhs);

        /* if result is larger than p, apply mod */
        if ((lhs[KEY_UINT32_ARRAY_LENGTH] > 0u) || (Bi_Compare(lhs, P256_prime_m, KEY_UINT32_ARRAY_LENGTH) > 0))
        {
            EcP256_SubModN(lhs, P256_prime_m, lhs);
        }

        /* modulus = X * lhs = X * ((X)^2 + a) (mod p) */
        EcP256_FieldMult(lhs, X, rhs, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(modulus, rhs);
        Bi_SetZero(rhs, 16);

        /* rhs = X * ((X)^2 + a) + b (mod p) */
        EcP256_FieldAdd(modulus, P256_curve_b, P256_prime_r, rhs);

        /* if result is larger than p, apply mod */
        if ((rhs[KEY_UINT32_ARRAY_LENGTH] > 0u) || (Bi_Compare(rhs, P256_prime_m, KEY_UINT32_ARRAY_LENGTH) > 0))
        {
            EcP256_SubModN(rhs, P256_prime_m, rhs);
        }

        /*
         * Compute (Y)^2 (mod p), result in modulus
         */
        EcP256_FieldMult(Y, Y, lhs, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(modulus, lhs);

        /* Compare left-hand side with right-hand side */
        bIsValid = (Bi_Compare(modulus, rhs, KEY_UINT32_ARRAY_LENGTH) == 0) ? TRUE : FALSE;
    }

    return bIsValid;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*
 * Input P = (X1,Y1,Z1), Q=(X2,Y2)
 * Output P+Q = (X3,Y3,Z3)
 *
 * X3 = F^2 - E^3 - 2*B*E^2; Y3 = F*(B*E^2 - X3) - D*E^3; Z3 = Z1*E
 * B = X2*Z1^2; D = Y2*Z1^3; E = X1 - X2; F = Y1 - D;
 *
 * refer to Appendix 2, Z1=1 in "Improved Elliptic Curve Multiplication Methods Resistant
 * against Side Channel Attacks" by Tetsuya Izu, Bodo Moller and Tsuyoshi Takagi
 *
 */
static void ec_JacobiAdd(const uint32_t *qx, const uint32_t *qy, multiplicationCtx_t *ctx)
{
    if (Bi_IsZero(ctx->pR0y) != 0)
    {
        Bi_Copy(qx, ctx->pR0x, KEY_UINT32_ARRAY_LENGTH);
        Bi_Copy(qy, ctx->pR0y, KEY_UINT32_ARRAY_LENGTH);
        Bi_SetZero(ctx->pR0z, (int)KEY_UINT32_ARRAY_LENGTH);
        ctx->pR0z[0] = 0x00000001u;
    }
    else
    {
        EcP256_FieldMult(ctx->pR0z, ctx->pR0z, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp1, ctx->multiplicationTemp); // T1 = Z1^2
        EcP256_FieldMult(ctx->temp1, qx, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp2, ctx->multiplicationTemp); // T2 = B
        EcP256_FieldMult(qy, ctx->pR0z, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp3, ctx->multiplicationTemp); // T3 = Y2*Z1
        EcP256_FieldMult(ctx->temp3, ctx->temp1, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp3, ctx->multiplicationTemp);            // T3 = D
        EcP256_FieldSub(ctx->pR0x, ctx->temp2, P256_prime_m, ctx->temp5); // T5 = E
        EcP256_FieldMult(ctx->temp5, ctx->pR0z, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->pR0z, ctx->multiplicationTemp);             // T7 = Z1*E = Z3
        EcP256_FieldSub(ctx->pR0y, ctx->temp3, P256_prime_m, ctx->temp6); // T6 = F
        EcP256_FieldMult(ctx->temp5, ctx->temp5, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp1, ctx->multiplicationTemp); // T1 = E^2
        EcP256_FieldMult(ctx->temp6, ctx->temp6, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp4, ctx->multiplicationTemp); // T4 = F^2
        EcP256_FieldMult(ctx->temp2, ctx->temp1, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp2, ctx->multiplicationTemp); // T2 = B*E^2
        EcP256_FieldMult(ctx->temp1, ctx->temp5, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp5, ctx->multiplicationTemp);             // T5 = E^3
        EcP256_FieldSub(ctx->temp4, ctx->temp5, P256_prime_m, ctx->temp4); // T4 = F^2-E^3
        EcP256_FieldAdd(ctx->temp2, ctx->temp2, P256_prime_r, ctx->temp1); // T1 = 2*B*E^2
        EcP256_FieldSub(ctx->temp4, ctx->temp1, P256_prime_m, ctx->pR0x);  // T4 = X3
        EcP256_FieldSub(ctx->temp2, ctx->pR0x, P256_prime_m, ctx->temp2);  // T2 = B*E^2 - X^3
        EcP256_FieldMult(ctx->temp6, ctx->temp2, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp6, ctx->multiplicationTemp); // T6 = F*(B*E^2 - X^3)
        EcP256_FieldMult(ctx->temp3, ctx->temp5, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp1, ctx->multiplicationTemp);            // T1 = D*E^3
        EcP256_FieldSub(ctx->temp6, ctx->temp1, P256_prime_m, ctx->pR0y); // T6 = Y3
    }
}

/*
 * Input P = (X1,Y1,Z1)
 * Output P+P = (X3,Y3,Z3)
 *
 * X3 = M^2 - 2*S; Y3 = M*(S-X3) - 8*Y1^4; Z3 = 2*Y1*Z1
 * M = 3*(X1 + Z1^2)(X1 - Z1^2) because a = -3
 * S = 4*X1*Y1^2
 *
 * refer to Appendix 1 in "Improved Elliptic Curve Multiplication Methods Resistant
 * against Side Channel Attacks" by Tetsuya Izu, Bodo Moller and Tsuyoshi Takagi
 *
 */

static void ec_JacobiDouble(multiplicationCtx_t *ctx)
{
    if (Bi_IsZero(ctx->pR0y) != 0)
    {
        Bi_SetZero(ctx->pR0x, (int)KEY_UINT32_ARRAY_LENGTH);
        Bi_SetZero(ctx->pR0y, (int)KEY_UINT32_ARRAY_LENGTH);
        Bi_SetZero(ctx->pR0z, (int)KEY_UINT32_ARRAY_LENGTH);
    }
    else
    {
        EcP256_FieldMult(ctx->pR0y, ctx->pR0y, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp2, ctx->multiplicationTemp);             // T2 = Y1^2
        EcP256_FieldAdd(ctx->temp2, ctx->temp2, P256_prime_r, ctx->temp2); // T2 = 2*Y1^2
        EcP256_FieldMult(ctx->pR0x, ctx->temp2, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp3, ctx->multiplicationTemp);             // T3 = 2*Y1^2*X1
        EcP256_FieldAdd(ctx->temp3, ctx->temp3, P256_prime_r, ctx->temp3); // T3 = 4*Y1^2*X1 = S
        EcP256_FieldMult(ctx->temp2, ctx->temp2, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp2, ctx->multiplicationTemp);             // T2 = 4*Y1^4
        EcP256_FieldAdd(ctx->temp2, ctx->temp2, P256_prime_r, ctx->temp2); // T2 = 8*Y1^4
        EcP256_FieldMult(ctx->pR0y, ctx->pR0z, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp5, ctx->multiplicationTemp); // T5 = Y1*Z1
        EcP256_FieldMult(ctx->pR0z, ctx->pR0z, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp6, ctx->multiplicationTemp);             // T6 = Z1^2
        EcP256_FieldAdd(ctx->temp5, ctx->temp5, P256_prime_r, ctx->pR0z);  // T5 = 2*Y1*Z1 = Z3
        EcP256_FieldAdd(ctx->pR0x, ctx->temp6, P256_prime_r, ctx->temp4);  // T4 = X1 + Z1^2
        EcP256_FieldAdd(ctx->temp6, ctx->temp6, P256_prime_r, ctx->temp6); // T6 = 2*Z1^2
        EcP256_FieldSub(ctx->temp4, ctx->temp6, P256_prime_m, ctx->temp6); // T6 = X1 - Z1^2
        EcP256_FieldMult(ctx->temp4, ctx->temp6, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp4, ctx->multiplicationTemp); // T4 = (X1 + Z1^2)(X1 - Z1^2)
        EcP256_FieldAdd(ctx->temp4, ctx->temp4, P256_prime_r, ctx->temp6);
        EcP256_FieldAdd(ctx->temp4, ctx->temp6, P256_prime_r, ctx->temp4); // T4 = 3*(X1+Z1^2)(X1-Z1^2)
        EcP256_FieldMult(ctx->temp4, ctx->temp4, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp6, ctx->multiplicationTemp);             // T6 = (3*(X1+Z1^2)(X1-Z1^2))^2 = M^2
        EcP256_FieldSub(ctx->temp6, ctx->temp3, P256_prime_m, ctx->temp6); // T6 = M^2 - S
        EcP256_FieldSub(ctx->temp6, ctx->temp3, P256_prime_m, ctx->pR0x);  // T6 = M^2 - 2*S = X3
        EcP256_FieldSub(ctx->temp3, ctx->pR0x, P256_prime_m, ctx->temp3);  // T3 = S - X3
        EcP256_FieldMult(ctx->temp4, ctx->temp3, ctx->multiplicationTemp, KEY_UINT32_ARRAY_LENGTH);
        EcP256_FieldModP(ctx->temp4, ctx->multiplicationTemp);            // T4 = M*(S-X3)
        EcP256_FieldSub(ctx->temp4, ctx->temp2, P256_prime_m, ctx->pR0y); // T4 = M*(S-X3) - 8*Y1^4
    }
}

#endif /* mUseBigIntMacros_d */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
