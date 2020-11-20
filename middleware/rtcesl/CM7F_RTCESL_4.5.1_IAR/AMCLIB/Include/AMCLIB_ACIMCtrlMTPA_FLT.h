/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
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
* @brief ACIM flux MTPA algotitm   
* 
*******************************************************************************/
#ifndef _AMCLIB_ACIMFLUXMTPA_FLT_H_
#define _AMCLIB_ACIMFLUXMTPA_FLT_H_

#if defined(__cplusplus) 
extern "C" {
#endif
  
/****************************************************************************
* Includes
****************************************************************************/   
#include "amclib_types.h" 
#include "gdflib_FP.h"  
#include "gflib_FP.h"     
#include "gmclib_FP.h"     
#include "mlib_FP.h"

/******************************************************************************
* Macros 
******************************************************************************/      
#define AMCLIB_ACIMCtrlMTPAInit_FLT_Ci( fltIDMin, fltIDMax, psCtrl)           \
        AMCLIB_ACIMCtrlMTPAInit_FLT_FCi(fltIDMin, fltIDMax, psCtrl)                            
#define AMCLIB_ACIMCtrlMTPA_FLT_C( fltIq, psCtrl)                             \
        AMCLIB_ACIMCtrlMTPA_FLT_FC(fltIq, psCtrl)                              
#define AMCLIB_ACIMCtrlMTPA_FLT_CRam( fltIq, psCtrl)                          \
        AMCLIB_ACIMCtrlMTPA_FLT_FCRam(fltIq, psCtrl)
          
/******************************************************************************
* Types 
******************************************************************************/     
typedef struct
{   
    GDFLIB_FILTER_EXP_T_FLT fltIdExpParam;   /* Exponential filter structure for Id measured */ 
    float_t fltLowerLim;                     /* Min value of Id required */
    float_t fltUpperLim;                     /* Max value of Id required */ 
} AMCLIB_ACIM_CTRL_MTPA_T_FLT;   
   
/****************************************************************************
* Exported function prototypes
****************************************************************************/   
extern float_t AMCLIB_ACIMCtrlMTPA_FLT_FC(float_t fltIq,
                                          AMCLIB_ACIM_CTRL_MTPA_T_FLT *psCtrl); 
RAM_FUNC_LIB 
extern float_t AMCLIB_ACIMCtrlMTPA_FLT_FCRam(float_t fltIq,
                                             AMCLIB_ACIM_CTRL_MTPA_T_FLT *psCtrl); 
                                        
/****************************************************************************
* Inline functions 
****************************************************************************/                                 

/***************************************************************************//*!
* @brief  The function initializes the actual values of float AMCLIB_ACIMCtrlMTPA_FLT function.
*
* @params:      ptr  AMCLIB_ACIM_CTRL_MTPA_T_FLT *psParam - Pointer to ACIM flux MTPA 
*                                                           function structure                
*
* @return       N/A
* 
*******************************************************************************/
RAM_FUNC_LIB 
static inline void AMCLIB_ACIMCtrlMTPAInit_FLT_FCi(float_t fltIDMin, float_t fltIDMax, 
                                                   AMCLIB_ACIM_CTRL_MTPA_T_FLT *psCtrl)
{
    GDFLIB_FilterExpInit_FLT(0.0F, &psCtrl->fltIdExpParam);
    psCtrl->fltLowerLim = fltIDMin;
    psCtrl->fltUpperLim = fltIDMax;    
}
   
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_ACIMFLUXMTPA_FLT_H_ */
