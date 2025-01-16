/*******************************************************************************
*
* Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
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
#ifndef _GFLIB_ATANYXPQ_F16_H_
#define _GFLIB_ATANYXPQ_F16_H_

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
#define GFLIB_AtanYXPQ_F16_Ci(f16Y, f16X, pbErrFlag)                         \
        GFLIB_AtanYXPQ_F16_FCi(f16Y, f16X, pbErrFlag)


/****************************************************************************
* Inline functions 
****************************************************************************/              
/***************************************************************************//*!
*
* @brief     The GFLIB_AtanYX function computes the AtanYX(f16Y,f16X)
*            using piece-wise polynomial approximation.
*
* @param     ptr r2 - *pbErrFlag - The pointed variable is 1, if the inputs were 0, 0
*                              otherwise it is 0.
*                r3 - Pointer to the Atan table
*
* @param     in  r0 - f16Y in the range of [-1,1).
*                r1 - f16X in the range of [-1,1).
*
* @return    The function returns
*                r0 - AtanYX(f16Y,f16X).The output data value
*            is in range [-0.25, 0.25), which corresponds to the angle
*            in the range of [-pi/4,pi/4).
*
* @remarks   The GFLIB_Atan function computes the atan(x)/pi
*            using PQ module.
*
*            In case of the inputs 0, 0 the error flag is set to 1 and the output
*            of the function is 0.
*******************************************************************************/
static inline frac16_t GFLIB_AtanYXPQ_F16_FCi(frac16_t f16Y, frac16_t f16X, 
                                              bool_t *pbErrFlag)
{
    frac16_t f16Temp = 1;
    *pbErrFlag = FALSE;
    
    if(((uint16_t)f16Y | (uint16_t)f16X) == 0U)
    {                                      /* y=0, x=0   return  0 */
        *pbErrFlag = TRUE;
        f16Temp = ((frac16_t)0);      
    }     
    else if(f16Y == (frac16_t)0)           /* y=0, x>0   return  0 */
    {
        if(f16X > (frac16_t)0)
        {
            f16Temp =((frac16_t)0);
        }
        else
        {
            f16Temp =((frac16_t)INT16_MAX); /* y=0, x<0   return  pi */
        }
    }
    else if(f16X == (frac16_t)0)     
    {                           
        if(f16Y > (frac16_t)0)    
        {                                    /* x=0, y>0    return  pi/2 */
            f16Temp =((frac16_t) 0x4000);     
        }                         
        else
        {                         
            f16Temp =(-((frac16_t) 0x4000)); /* x=0, y<0    return -pi/2 */
        }                         
    } 
    else
    /************************* inputs without zero value  *********************/
    {
        RTCESL_PQ_CORDIC_X = (uint32_t)MLIB_ShL_F32(f16X, 14U);
        RTCESL_PQ_CORDIC_Y = (uint32_t)MLIB_ShL_F32(f16Y, 14U);
        RTCESL_PQ_CORDIC_Z = 0;
        RTCESL_PQ_CONTROL  = 0x00000055;
        RTCESL_PQ_WaitDone();      
        f16Temp = (frac16_t)MLIB_ShR_F32((frac32_t)RTCESL_PQ_CORDIC_Z, 12U);  /* function is scaled 2^27 = PI so 2^12 = 2^(27-15) */
        if (f16Y > 0 && f16X < 0)    /* move angle to II quadrant */
        {   
            f16Temp += (frac16_t)32768;
        }
        if (f16Y < 0 && f16X < 0)    /* move angle to III quadrant */
        {   
            f16Temp -= (frac16_t)32768;
        }                
    }
    return f16Temp;
}
        
#if defined(__cplusplus)
}
#endif
#endif /* _GFLIB_ATANYXPQ_F16_H_ */
