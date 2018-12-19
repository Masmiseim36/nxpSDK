/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
* 
*
****************************************************************************//*!
*
* @brief  Park, Inverse Park Transformation 
* 
*******************************************************************************/
#ifndef _GMCLIB_PARK_FLT_H_
#define _GMCLIB_PARK_FLT_H_

#if defined(__cplusplus) 
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"
#include "gmclib_types.h"    

/*******************************************************************************
* Macros 
*******************************************************************************/   
#define GMCLIB_Park_FLT_Ci(psIn, psAnglePos, psOut)                            \
        GMCLIB_Park_FLT_FCi(psIn, psAnglePos, psOut)                           
#define GMCLIB_ParkInv_FLT_Ci(psIn, psAnglePos, psOut)                         \
        GMCLIB_ParkInv_FLT_FCi(psIn, psAnglePos, psOut)

/***************************************************************************//*!
*
* @brief       The function calculates Park Transformation which is used for 
*              transforming values (current, voltage, flux) from 
*              alpha-beta stationary orthogonal coordination system 
*              to d-q rotating orthogonal coordination system
*
* @param  ptr -  GMCLIB_2COOR_ALBE_T_FLT *psOut - OUT - pointer to structure containing 
*                data of two phase stationary orthogonal system 
*             -  GMCLIB_2COOR_DQ_T_FLT *psIn - IN - pointer to structure containing  
*                data of DQ coordinate two-phase stationary orthogonal system              
*             -  GMCLIB_2COOR_SINCOS_T_FLT *psAnglePos - IN - pointer to structure 
*                where the values of sine and cosine are stored         
*
* @remarks     Modifies the structure pointed by pDQ pointer
*              according to the following equations
*
*              d = alpha * cos(theta) + beta * sin(theta)
*              q = beta * cos(theta) - alpha * sin(theta)
*
*******************************************************************************/
static inline void GMCLIB_Park_FLT_FCi(const GMCLIB_2COOR_ALBE_T_FLT *psIn,
                                       const GMCLIB_2COOR_SINCOS_T_FLT *psAnglePos,
                                       GMCLIB_2COOR_DQ_T_FLT *psOut)
{
    register float_t fltA;
    register float_t fltB;
    register float_t fltSin;
    register float_t fltCos;
    
    /* Store values for calculation */
    fltA    = (psIn->fltAlpha);
    fltB    = (psIn->fltBeta);
    fltSin  = (psAnglePos->fltSin);
    fltCos  = (psAnglePos->fltCos);
    
    /* Calculate d = alpha * cos(theta) + beta * sin(theta) */
    psOut->fltD = MLIB_Mac4_FLT (fltA, fltCos, fltB, fltSin) ;
    
    /* Calculate q = beta * cos(theta) - alpha * sin(theta) */  
    psOut->fltQ = MLIB_Msu4_FLT (fltB, fltCos, fltA, fltSin) ;   
}
 
/***************************************************************************//*!
*
* @brief        The function calculates Inverse Park Transformation which is used 
*               for transforming values (current, voltage, flux) from 
*               d-q rotating orthogonal coordination system to alpha-beta 
*               stationary orthogonal coordination system.
*
* @param  ptr -  GMCLIB_2COOR_DQ_T_FLT *psIn - IN - pointer to structure containing  
*                data of DQ coordinate two-phase stationary orthogonal system              
*             -  GMCLIB_2COOR_SINCOS_T_FLT *psAnglePos - IN - pointer to structure 
*                where the values of sine and cosine are stored
*             -  GMCLIB_2COOR_ALBE_T_FLT *psOut - OUT - pointer to structure containing 
*                data of two phase stationary orthogonal system
*                       
* @remarks      Modifies the structure pointed by pAlphaBeta pointer 
*               according following equations:
*
*               alpha = d * cos(theta) - q * sin(theta)
*               beta  = d * sin(theta) + q * cos(theta)
*
***********************************1*******************************************/
static inline void GMCLIB_ParkInv_FLT_FCi(const GMCLIB_2COOR_DQ_T_FLT *psIn,
                                          const GMCLIB_2COOR_SINCOS_T_FLT *psAnglePos,
                                          GMCLIB_2COOR_ALBE_T_FLT *psOut)
{
    register float_t fltD;
    register float_t fltQ;
    register float_t fltSin;
    register float_t fltCos;
    
    /* Store values for calculation */
    fltD    = (psIn->fltD);
    fltQ    = (psIn->fltQ);
    fltSin  = (psAnglePos->fltSin);
    fltCos  = (psAnglePos->fltCos);
    
    /* Calculate alpha = d * cos(theta) - q * sin(theta) */   
    psOut->fltAlpha = MLIB_Msu4_FLT (fltD, fltCos, fltQ, fltSin); 
    /* Calculate beta  = d * sin(theta) + q * cos(theta) */ 
    psOut->fltBeta = MLIB_Mac4_FLT (fltD, fltSin, fltQ, fltCos);
}
 
#if defined(__cplusplus) 
}
#endif

#endif /* _GMCLIB_PARK_FLT_H_ */
