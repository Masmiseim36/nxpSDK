/*
 * Copyright (c) 2017, Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_common.h"
#include "mathfp.h"
#include "stdbool.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* constants for fixed point atan2 approximation function */
#define addfac      ((int16_t)0x10EF)
#define zp273       ((int16_t)0x045E)
#define neg_zp273   ((int16_t)0xFBA2)
#define piover2     ((int16_t)0x1921)
#define neg_piover2 ((int16_t)0xE6DF)
#define pi          ((int16_t)0x3243)
#define neg_pi      ((int16_t)0xCDBC)

#define USE_FPINLINE (1)
#define NUMQBITS    (12U) /* define the number of fractional bits to be used. Leaves 3 integer bits & a sign bit */

#if (USE_FPINLINE == 1) /* use inline functions instead of macros */

static inline int16_t FPABS(int16_t x)
{
    int16_t retval = x; /* positive return value */
    if (x<0)
    {
        if (x == -32768)
        {
          retval = 32767; /* special case, manually negate -32768 to +32767 */
        }
        else
        {
          retval = -(x); /* negative return value */
        }
    }
    return retval;
};
    

static inline int16_t FPADD(int16_t x, int16_t y)
{
    return x + y; /* simple addition works in fixed point */
};

static inline int16_t FPSUB(int16_t x, int16_t y)
{
    return x - y; /* simple subtraction works in fixed point */
};

static inline int16_t FPMUL(int16_t x, int16_t y, uint8_t q)
{
    int32_t retval = (int32_t)x * (int32_t)y; /* perform the multiply */
    uint32_t temp_prod_u32 = (uint32_t)retval; /* now cast the int32 to uint32. Upper 16 bits will be zeros or 1s */
    /* scale back to the original scale */
    temp_prod_u32 = temp_prod_u32>>q; /* assert above guarantees shift strictly less than 16 */
    temp_prod_u32 &= 0x0000FFFFU; /* mask off the upper bits to get within 16 bit size */
    int16_t temp_i16 = (int16_t)temp_prod_u32; /* cast back to int16 */
    return temp_i16; 
};

static inline int16_t FPDIV(int16_t x, int16_t y, uint8_t q)
{
    int32_t temp_x_i32 = (int32_t)x; /* cast from int16 to int32 type, upper 16 bits will be zeros or 1s based on sign of x */
    uint32_t temp_x_u32 = (uint32_t)temp_x_i32;
    temp_x_u32 = temp_x_u32<<q; /* Scale up, shift strictly less than 16 due to assert above. Sign bits preserved */
    int32_t retval = (int32_t)(temp_x_u32); /*  cast the result back to an int32 type */
    retval = retval/y;   
    return (int16_t)(retval); /* mask off upper bits and cast back to int16 */
};

static inline int16_t INNERCALC(int16_t iqd, int16_t sgz, uint8_t q)
{
    /* Implements the core calculation for the 4 octant appproximations */
    /* in the form of A*(B+(C*D)) */
    int16_t retval = FPMUL(iqd, sgz, q);
    retval = FPADD(addfac, retval);
    return FPMUL(iqd, retval, q);
}


    
#else
/* macros for fixed point math in 16 bit signed integers */
#define FPABS(x)    ((x) < 0 ? (int16_t)(~((uint16_t)x))+1 : (x))
#define FPADD(x,y)  ((x) + (y))
#define FPSUB(x,y)  ((x) - (y))
#define FPMUL(x,y,q)(((x) * (y)) >> (q))
#define FPDIV(x,y,q) (((x) << (q)) / (y))

/* custom inner calculation macro for atan approximation */
#define INNERCALC(iqd,sgz,q)  (FPMUL((iqd),FPADD(addfac,FPMUL((sgz),(iqd),(q))),q))
#endif
/* macros for converting to/from fixed point math in 16 bit signed integers */
//#define FLTTOFP(f,q)    ((int16_t)((f)*(float64_t)(1<<q)))
//#define FPTOFLT(x,q)    ((float64_t)(x) / (float64_t)(1<<(q)))

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static int16_t calculate_octant_1_8(int16_t x, int16_t y);
static int16_t calculate_octant_2_3(int16_t x, int16_t y);
static int16_t calculate_octant_4_5(int16_t x, int16_t y);
static int16_t calculate_octant_6_7(int16_t x, int16_t y);

/*******************************************************************************
 * Code
 ******************************************************************************/
/* This is a fixed point implementation of the arctan function as described in "Efficient Approximations */
/* for the Arctangent Function", by Sreeraman Rajan, Sichun Wang, Robert Inkol, and Alain Joyal in */
/* IEEE SIGNAL PROCESSING MAGAZINE, MAY 2006 */
int16_t atan2fp(int16_t y, int16_t x)
{
#if (NUMQBITS>=16U)
#error "Invalid number of fractional bits."
#endif

    bool calc_done = false; /* flag to control whether to perform approximation or if the error checks have triggered */
    int16_t retval = (int16_t)0xFFFFU;
    int16_t absx = FPABS(x);
    int16_t absy = FPABS(y);
    
    /* handle error cases and cases that will cause divide by zero in approximation equations */
    if ((y == 0) && (x == 0))
    {
        retval = (int16_t)0xFFFFU; /* invalid return value indicating an error */
    }
    else    
    {
        /* handle special cases that fall directly on X or Y axis */
        if (x == 0)
        {
            retval = ((y > 0) ? piover2 : neg_piover2);
            calc_done = true; /* prevent any further evaluations */
        }
        
        if (y == 0)
        {
            retval = ((x > 0) ? 0 : pi);
            calc_done = true; /* prevent any further evaluations */
        }
    
        /* 4 quadrant approximation of atan() function */
        if ((x > 0) && (x >= absy) && !calc_done) /* equation #1 */
        {
            retval = calculate_octant_1_8(x, y);
            calc_done = true; /* prevent any further evaluations */
        }

        if ((y > 0) && (y >= absx) && !calc_done) /* equation #2 */
        {
            retval = calculate_octant_2_3(x, y);
            calc_done = true; /* prevent any further evaluations */
        }

        if ((x < 0) && (absx >= absy) && !calc_done) /* equation #3 */
        {
            retval = calculate_octant_4_5(x, y);
            calc_done = true; /* prevent any further evaluations */
        }

        if ((y < 0) && (absy >= absx) && !calc_done) /* equation #4 */
        {
            retval = calculate_octant_6_7(x, y);
        }
    }
    
    return retval;
}

static int16_t calculate_octant_1_8(int16_t x, int16_t y)
{
    int16_t iqdiv; /* I/Q or Q/I, depending upon which equation is being calculated */
    int16_t sgn_zp273; /* holds sign(I)*0.273 or sign(Q)*0x273 */
    iqdiv = FPDIV(y, x, NUMQBITS);
    if (y < 0) /* choose 0.273 (or negated) factor so that FPADD can be used in all INNERCALC calls */
    {
        sgn_zp273 = zp273;
    }
    else
    {
        sgn_zp273 = neg_zp273;
    }
    
    return INNERCALC(iqdiv,sgn_zp273,NUMQBITS);
}

static int16_t calculate_octant_2_3(int16_t x, int16_t y)
{
    int16_t iqdiv; /* I/Q or Q/I, depending upon which equation is being calculated */
    int16_t sgn_zp273; /* holds sign(I)*0.273 or sign(Q)*0x273 */
    iqdiv = FPDIV(x, y, NUMQBITS);
    if (x < 0) /* choose 0.273 (or negated) factor so that FPADD can be used in all INNERCALC calls */
    {
        sgn_zp273 = zp273;
    }
    else
    {
        sgn_zp273 = neg_zp273;
    }
    
    return FPSUB(piover2,INNERCALC(iqdiv,sgn_zp273,NUMQBITS));
}
    
static int16_t calculate_octant_4_5(int16_t x, int16_t y)
{
    int16_t iqdiv; /* I/Q or Q/I, depending upon which equation is being calculated */
    int16_t sgn_zp273; /* holds sign(I)*0.273 or sign(Q)*0x273 */
    int16_t pifac;
    iqdiv = FPDIV(y, x, NUMQBITS);
    if (y < 0) /* choose 0.273 (or negated) factor so that FPADD can be used in all INNERCALC calls */
    {
        sgn_zp273 = neg_zp273;
        pifac = neg_pi;
    }
    else
    {
        sgn_zp273 = zp273;
        pifac = pi;
    }
    
    return FPADD(pifac,INNERCALC(iqdiv,sgn_zp273,NUMQBITS));
}
    
static int16_t calculate_octant_6_7(int16_t x, int16_t y)
{
    int16_t iqdiv; /* I/Q or Q/I, depending upon which equation is being calculated */
    int16_t sgn_zp273; /* holds sign(I)*0.273 or sign(Q)*0x273 */
    iqdiv = FPDIV(x, y, NUMQBITS);
    if (x < 0) /* choose 0.273 (or negated) factor so that FPADD can be used in all INNERCALC calls */
    {
        sgn_zp273 = neg_zp273;
    }
    else
    {
        sgn_zp273 = zp273;
    }
    
    return FPSUB(neg_piover2,INNERCALC(iqdiv,sgn_zp273,NUMQBITS));
}

