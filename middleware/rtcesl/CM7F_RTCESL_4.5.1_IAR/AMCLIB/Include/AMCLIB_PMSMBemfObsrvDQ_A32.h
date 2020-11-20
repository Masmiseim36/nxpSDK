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
* @brief  Algorithm of PMSM Back Electromotive Force observer in rotating 
*         reference frame     
* 
*******************************************************************************/
#ifndef _AMCLIB_PMSM_BEMF_OBSRV_A32_DQ_H_
#define _AMCLIB_PMSM_BEMF_OBSRV_A32_DQ_H_

#if defined(__cplusplus) 
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "amclib_types.h"
#include "gflib.h"

/*******************************************************************************
* Macros 
*******************************************************************************/    
#define AMCLIB_PMSMBemfObsrvDQ_F16_Asm(psIDQ, psUDQ, f16Speed, psCtrl)         \
        AMCLIB_PMSMBemfObsrvDQ_F16_FAsm(psIDQ, psUDQ, f16Speed, psCtrl)
#define AMCLIB_PMSMBemfObsrvDQ_F16_AsmRam(psIDQ, psUDQ, f16Speed, psCtrl)      \
        AMCLIB_PMSMBemfObsrvDQ_F16_FAsmRam(psIDQ, psUDQ, f16Speed, psCtrl)
#define AMCLIB_PMSMBemfObsrvDQInit_F16_Ci(psCtrl)                              \
        AMCLIB_PMSMBemfObsrvDQInit_F16_FCi( psCtrl)
        
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{       
    GMCLIB_2COOR_DQ_T_F32 sEObsrv;  /* Estimated back-EMF voltage - d/q */
    GMCLIB_2COOR_DQ_T_F32 sIObsrv;  /* Estimated current  - d/q */
    
    /* Observer parameters for controllers */
    struct
    {
        frac32_t   f32ID_1;      /* Integral part state variable for D coefficient */           
        frac32_t   f32IQ_1;      /* Integral part state variable for Q coefficient */             
        acc32_t    a32PGain;     /* Observer proportional gain coefficient */                               
        acc32_t    a32IGain;     /* Observer integral gain coefficient */                           
    } sCtrl;
    
    /* Configuration parameters */
    acc32_t      a32IGain;     /* Current scaling coefficient */                        
    acc32_t      a32UGain;     /* Voltage scaling coefficient */                        
    acc32_t      a32WIGain;    /* Angular speed scaling coefficient */              
    acc32_t      a32EGain;     /* Back-emf scaling coefficient */                  
    
    /* Estimated phase error between real d/q frame system and estimated d/q 
    reference system */
    frac16_t      f16Error;                                       
       
} AMCLIB_BEMF_OBSRV_DQ_T_A32;
   
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
RAM_FUNC_LIB 
extern frac16_t AMCLIB_PMSMBemfObsrvDQ_F16_FAsmRam(const GMCLIB_2COOR_DQ_T_F16 *psIDQ,
                                                   const GMCLIB_2COOR_DQ_T_F16 *psUDQ,
                                                   frac16_t f16Speed,
                                                   AMCLIB_BEMF_OBSRV_DQ_T_A32 *psCtrl);
												
extern frac16_t AMCLIB_PMSMBemfObsrvDQ_F16_FAsm(const GMCLIB_2COOR_DQ_T_F16 *psIDQ,
                                                const GMCLIB_2COOR_DQ_T_F16 *psUDQ,
                                                frac16_t f16Speed,
                                                AMCLIB_BEMF_OBSRV_DQ_T_A32 *psCtrl);
/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief    PMSM BEMF in DQ reference frame initialization
*
* @param    ptr -  AMCLIB_BEMF_OBSRV_DQ_T_A32 *psCtrl - pointer to the parameters of the observer
*
* @return   None
*                         
* @remarks  Initializes the structure of the PMSM BEMF in DQ reference frame
*           
*    sIObsrv_f32D  = 0;
*    sIObsrv_f32Q  = 0;  
*    sCtrl_f32ID_1 = 0;     
*    sCtrl_f32IQ_1 = 0; 
*
****************************************************************************/
RAM_FUNC_LIB 
static inline void AMCLIB_PMSMBemfObsrvDQInit_F16_FCi(AMCLIB_BEMF_OBSRV_DQ_T_A32 *psCtrl) 
{
    psCtrl -> sIObsrv.f32D  = FRAC32(0.0);
    psCtrl -> sIObsrv.f32Q  = FRAC32(0.0);  
    psCtrl -> sCtrl.f32ID_1 = FRAC32(0.0);     
    psCtrl -> sCtrl.f32IQ_1 = FRAC32(0.0);      
}
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_PMSM_BEMF_OBSRV_A32_DQ_H_ */       
