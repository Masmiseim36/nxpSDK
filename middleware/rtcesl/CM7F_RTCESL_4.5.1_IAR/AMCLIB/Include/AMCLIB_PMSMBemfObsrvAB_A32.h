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
* @brief  Algorithm of PMSM Back Electromotive Force observer in stationary 
*         reference frame
* 
*******************************************************************************/
#ifndef _AMCLIB_PMSM_BEMF_OBSRV_A32_AB_H_
#define _AMCLIB_PMSM_BEMF_OBSRV_A32_AB_H_

#if defined(__cplusplus) 
extern "C" { 
#endif 
/******************************************************************************
* Includes
******************************************************************************/
#include "gflib.h"
#include "gmclib.h"

/******************************************************************************
* Macros 
******************************************************************************/
#define AMCLIB_PMSMBemfObsrvAB_F16_C(psIAlBe, psUAlBe, f16Speed, psCtrl)       \
        AMCLIB_PMSMBemfObsrvAB_F16_FC(psIAlBe, psUAlBe, f16Speed, psCtrl)
#define AMCLIB_PMSMBemfObsrvAB_F16_CRam(psIAlBe, psUAlBe, f16Speed, psCtrl)    \
        AMCLIB_PMSMBemfObsrvAB_F16_FCRam(psIAlBe, psUAlBe, f16Speed, psCtrl)
#define AMCLIB_PMSMBemfObsrvABInit_F16_Ci(psCtrl)                              \
        AMCLIB_PMSMBemfObsrvABInit_F16_FCi(psCtrl)            
/******************************************************************************
* Types
******************************************************************************/
typedef struct
{
    GMCLIB_2COOR_ALBE_T_F32 sEObsrv;  /* Estimated back-EMF voltage - alpha,beta */            
    GMCLIB_2COOR_ALBE_T_F32 sIObsrv;  /* Estimated current  - alpha,beta */            
    
    /* Observer parameters for controllers */
    struct
    {
        frac32_t f32IAlpha_1;  /* Integral part state variable for alpha coefficient */     
        frac32_t f32IBeta_1;   /* Integral part state variable for beta coefficient */ 
        acc32_t  a32PGain;     /* Observer proportional gain coefficient */                           
        acc32_t  a32IGain;     /* Observer integral gain coefficient */                             
    } sCtrl;
    
    /* Configuration parameters */ 
    acc32_t  a32IGain;         /* Current scaling coefficient */           
    acc32_t  a32UGain;         /* Voltage scaling coefficient */               
    acc32_t  a32WIGain;        /* Angular speed scaling coefficient */         
    acc32_t  a32EGain;         /* Back-emf scaling coefficient */              
    
    /* Unity vector */
    GMCLIB_2COOR_SINCOS_T_F16 sUnityVctr;         
    
} AMCLIB_BEMF_OBSRV_AB_T_A32;
   
/******************************************************************************
* Exported function prototypes
******************************************************************************/
RAM_FUNC_LIB 
extern void AMCLIB_PMSMBemfObsrvAB_F16_FCRam(const GMCLIB_2COOR_ALBE_T_F16 *psIAlBe,
                                             const GMCLIB_2COOR_ALBE_T_F16 *psUAlBe,
                                             frac16_t f16Speed,
                                             AMCLIB_BEMF_OBSRV_AB_T_A32 *psCtrl);
										 
extern void AMCLIB_PMSMBemfObsrvAB_F16_FC(const GMCLIB_2COOR_ALBE_T_F16 *psIAlBe,
                                          const GMCLIB_2COOR_ALBE_T_F16 *psUAlBe,
                                          frac16_t f16Speed,
                                          AMCLIB_BEMF_OBSRV_AB_T_A32 *psCtrl);

/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief    PMSM BEMF in AB reference frame initialization
*
* @param    ptr   AMCLIB_BEMF_OBSRV_AB_T_A32 *psCtrl - pointer to the parameters of the observer
*
* @param  in    None
*
* @return       None
*                         
* @remarks  Initializes the structure of the PMSM BEMF in AB reference frame
*           
*    sIObsrv_f32Alpha = 0;
*    sIObsrv_f32beta  = 0;  
*    sCtrl_f32IAlpha_1= 0;     
*    sCtrl_f32IBeta_1 = 0; 
*
****************************************************************************/
RAM_FUNC_LIB 
static inline void AMCLIB_PMSMBemfObsrvABInit_F16_FCi(AMCLIB_BEMF_OBSRV_AB_T_A32 *psCtrl)
{
    psCtrl -> sIObsrv.f32Alpha = FRAC32(0.0);
    psCtrl -> sIObsrv.f32Beta  = FRAC32(0.0);  
    psCtrl -> sCtrl.f32IAlpha_1= FRAC32(0.0);     
    psCtrl -> sCtrl.f32IBeta_1 = FRAC32(0.0);      
}
 
#if defined(__cplusplus) 
} 
#endif 

#endif /* _AMCLIB_PMSM_BEMF_OBSRV_A32_AB_H_ */    
