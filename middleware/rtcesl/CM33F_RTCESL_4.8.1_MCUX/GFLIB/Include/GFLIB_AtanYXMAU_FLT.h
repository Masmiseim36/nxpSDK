/*******************************************************************************
*
* Copyright 2013-2016, Freescale Semiconductor, Inc.
* Copyright 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
* 
*
****************************************************************************//*!
*
* @brief  Arcus tangent function based on the provided x,y coordinates as arguments
*        using division and piece-wise polynomial approximation
* 
*******************************************************************************/
#ifndef _GFLIB_ATANYXPQ_FLT_H_
#define _GFLIB_ATANYXPQ_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/******************************************************************************
* Includes
******************************************************************************/
#include "gflib.h"    

/*******************************************************************************
* Global variables
*******************************************************************************/

/******************************************************************************
* Macros 
******************************************************************************/                         
#define GFLIB_AtanYXMAU_FLT_Ci(fltY, fltX, pbErrFlag)                         \
        GFLIB_AtanYXMAU_FLT_FCi(fltY, fltX, pbErrFlag)

/****************************************************************************
* Inline functions 
****************************************************************************/              
/***************************************************************************//*!
*
* @brief     The GFLIB_AtanYX function computes the AtanYX(fltY,fltX)
*            using piece-wise polynomial approximation.
*
* @param     ptr r2 - *pbErrFlag - The pointed variable is 1, if the inputs were 0, 0
*                              otherwise it is 0.
*
* @param     in  r0 - fltY in the range of [-1,1).
*                r1 - fltX in the range of [-1,1).
*
* @return    The function returns
*                r0 - AtanYX(fltY,fltX).The output data value
*            is in range [-0.25, 0.25), which corresponds to the angle
*            in the range of [-pi/4,pi/4).
*
* @remarks   The GFLIB_Atan function computes the atan(x)/pi
*            using MAU module.
*
*            In case of the inputs 0, 0 the error flag is set to 1 and the output
*            of the function is 0.
*******************************************************************************/
static inline float_t GFLIB_AtanYXMAU_FLT_FCi(float_t fltY, float_t fltX, bool_t *pbErrFlag)
{
    float_t fltAng1;
    float_t fltAng2;
    float_t fltO1Tan;

    *pbErrFlag = FALSE;                     /* For (0, 0) vector return 0 */
    if((fltY == 0.0F) && (fltX == 0.0F))
    {
        *pbErrFlag = TRUE;
        return(RTCESL_NAN);                 /* Return NaN */
    }
    else
    {
        /* Firstly,  position the angle within a quarter */
        fltAng1 = 0.0F;

        if( fltX < 0.0F )
        {
            fltAng1 = FLOAT_PI;        
            if( fltY < 0.0F )
            {
                fltAng1 = MLIB_Neg_FLT(fltAng1); /* 3rd quadrant sign correction */
            }
        }
        /* Secondly, compute the angle offset within half-quarter */

        /* NOTE:
        *   For fltInX = 0 the division would get division by 0. 
            Because division by 0 may be treated differently on
        *   different platform, the case is treated by a separate condition. */
        if (fltY == fltX)
        {
            fltAng1 = MLIB_Add_FLT(fltAng1, 0.7853981633974480F);       
            fltAng2 = 0.0F;
        }
        else
        {
            if (fltY == MLIB_Neg_FLT(fltX))
            {
                fltAng1 = MLIB_Add_FLT(fltAng1, -0.7853981633974480F);      
                fltAng2 = 0.0F;
            }
            else
            {
                if (fltX == 0.0F)                /* Handle the division by 0 */
                {
                    /* Prepare the PI/2 */
                    fltAng2 = ( 1.57079632679490F);    
                    if (fltY < 0.0F)
                    {
                        fltAng2 = MLIB_Neg_FLT(fltAng2);  /* Correct sign for fltY < 0 */
                    }
                }
                else
                {   /* Division by 0 handled in previous step, MLIB_Div function not used here */
                    fltO1Tan = (fltY / fltX);    
                    /* Calculate arcus tangent */
                    fltAng2 = GFLIB_AtanMAU_FLT_FCi(fltO1Tan);    
                }
            }
        }
        /* The final angle is the sum of the angle located within 45 deg and the angle offset */
        fltAng1 = MLIB_Add_FLT(fltAng1, fltAng2);

        return(fltAng1);
    }
}

static inline acc32_t GFLIB_AtanYXMAU_A32f_FCi(float_t fltY, float_t fltX, bool_t *pbErrFlag)
{
    acc32_t a32Out;

    fltY = GFLIB_AtanYXMAU_FLT_FCi(fltY, fltX, pbErrFlag);

#if defined(__GNUC__) && !defined(__ARMCC_VERSION)   /* GCC(KDS) compiler */
    if (fltY == RTCESL_NAN)
        a32Out = ((acc32_t) 0);
    else
        a32Out = (acc32_t)((float_t)fltY *((float_t)32768.0F / (float_t)FLOAT_PI));
#else                     /* others compilers */
    a32Out =  (acc32_t)((float_t)fltY *((float_t)32768.0F / (float_t)FLOAT_PI));
#endif

    return (a32Out);
}
        
#if defined(__cplusplus)
}
#endif
#endif /* _GFLIB_ATANYXMAU_FLT_H_ */
