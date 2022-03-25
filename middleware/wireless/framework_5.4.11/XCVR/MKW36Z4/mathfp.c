/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */
 
#include "fsl_common.h"
#include "mathfp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* constants for fixed point atan2 approximation function */
#define addfac      0x10EF
#define zp273       0x045E
#define neg_zp273   0xFBA2
#define piover2     0x1921
#define neg_piover2 0xE6DF
#define pi          0x3243
#define neg_pi      0xCDBC

/* macros for fixed point math in 16 bit signed integers */
#define NUMQBITS    (12) /* define the number of fractional bits to be used. Leaves 3 integer bits & a sign bit */
#define FPABS(x)    ((x) < 0 ? (~(x))+1 : (x))
#define FPADD(x,y)  ((x) + (y))
#define FPSUB(x,y)  ((x) - (y))
#define FPMUL(x,y,q)(((x) * (y)) >> (q))
#define FPDIV(x,y,q) (((x) << (q)) / (y))
#define FLTTOFP(f,q)    ((int16_t)((f)*(double)(1<<q)))
#define FPTOFLT(x,q)    ((double)(x) / (double)(1<<(q)))

/* custom inner calculation macro for atan approximation */
#define INNERCALC(iqd,sgz)  (FPMUL((iqd),FPADD(addfac,FPMUL((sgz),(iqd),NUMQBITS)),NUMQBITS))

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/
/* This is a fixed point implementation of the arctan function as described in "Efficient Approximations */
/* for the Arctangent Function", by Sreeraman Rajan, Sichun Wang, Robert Inkol, and Alain Joyal in */
/* IEEE SIGNAL PROCESSING MAGAZINE, MAY 2006 */
int16_t atan2fp(int16_t y, int16_t x)
{
    int16_t iqdiv; /* I/Q or Q/I, depending upon which equation is being calculated */
    int16_t sgn_zp273; /* holds sign(I)*0.273 or sign(Q)*0x273 */
    uint8_t calc_done = 0; /* flag to control whether to perform approximation or if the error checks have triggered */
    int16_t retval = 0xFFFF;
    
    /* handle error cases and cases that will cause divide by zero in approximation equations */
    if ((y == 0) && (x == 0))
    {
        retval = 0xFFFF; /* invalid return value indicating an error */
        calc_done = 1; /* don't do approx */
    }
    if (x == 0)
    {
        retval = (y > 0 ? piover2 : neg_piover2);
        calc_done = 1; /* don't do approx */
    }
    if (y == 0)
    {
        retval = (x > 0 ? 0 : pi);
        calc_done = 1; /* don't do approx */
    }

    /* 4 quadrant approximation of atan() function */
    if ((x > 0) && (x >= FPABS(y)) && !calc_done) /* equation #1 */
    {
        iqdiv = FPDIV(y, x, NUMQBITS);
        if (y < 0) /* choose 0.273 (or negated) factor so that FPADD can be used in all INNERCALC calls */
        {
            sgn_zp273 = zp273;
        }
        else
        {
            sgn_zp273 = neg_zp273;
        }

        retval = INNERCALC(iqdiv,sgn_zp273);
        calc_done = 1; /* prevent any further evaluations */
    }

    if ((y > 0) && (y >= FPABS(x)) && !calc_done) /* equation #2 */
    {
        iqdiv = FPDIV(x, y, NUMQBITS);
        if (x < 0) /* choose 0.273 (or negated) factor so that FPADD can be used in all INNERCALC calls */
        {
            sgn_zp273 = zp273;
        }
        else
        {
            sgn_zp273 = neg_zp273;
        }

        retval = FPSUB(piover2,INNERCALC(iqdiv,sgn_zp273));
        calc_done = 1; /* prevent any further evaluations */
    }

    if ((x < 0) && (FPABS(x) >= FPABS(y)) && !calc_done) /* equation #3 */
    {
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

        retval = FPADD(pifac,INNERCALC(iqdiv,sgn_zp273));
        calc_done = 1; /* prevent any further evaluations */
    }

    if ((y < 0) && (FPABS(y) >= FPABS(x)) && !calc_done) /* equation #4 */
    {
        iqdiv = FPDIV(x, y, NUMQBITS);
        if (x < 0) /* choose 0.273 (or negated) factor so that FPADD can be used in all INNERCALC calls */
        {
            sgn_zp273 = neg_zp273;
        }
        else
        {
            sgn_zp273 = zp273;
        }

        retval = FPSUB(neg_piover2,INNERCALC(iqdiv,sgn_zp273));
    }
    
    return retval;
}

