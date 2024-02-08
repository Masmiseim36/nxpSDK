/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
