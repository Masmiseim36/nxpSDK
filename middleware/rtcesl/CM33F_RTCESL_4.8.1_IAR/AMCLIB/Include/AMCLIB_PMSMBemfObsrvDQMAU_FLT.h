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
* @brief  Algorithm of float PMSM Back Electromotive Force observer in rotating 
*         reference frame       
* 
*******************************************************************************/
#ifndef _AMCLIB_PMSM_BEMF_OBSRVMAU_FLT_DQ_H_
#define _AMCLIB_PMSM_BEMF_OBSRVMAU_FLT_DQ_H_

#if defined(__cplusplus) 
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_FP.h"   

/*******************************************************************************
* Macros 
*******************************************************************************/    
#define AMCLIB_PMSMBemfObsrvDQMAU_A32fff_Ci(psIDQ, psUDQ, fltSpeed, psCtrl)    \
        AMCLIB_PMSMBemfObsrvDQMAU_A32fff_FCi(psIDQ, psUDQ, fltSpeed, psCtrl)     

/******************************************************************************
* Inline functions
******************************************************************************/

/***************************************************************************//*!
*
* @brief    Float BEMF observer using D, Q components to calculate error BEMF gamma/delta.
*
* @param    ptr - GMCLIB_2COOR_DQ_T_FLT *psIDQ - pointer to the D, Q current                      
*               - GMCLIB_2COOR_DQ_T_FLT *psUDQ - pointer to the D, Q voltage
*               - AMCLIB_BEMF_OBSRV_DQ_T_FLT *psCtrl - pointer to the parameters of the observer
*
* @return   float_t - error between the real and estimated position
*       
* @remarks  The function calculates the algorithm of back electromotive force observer
*           in rotating reference frame by following equations:
*
*    fltTmpEObsrvD = psCtrl_sCtrl_fltID_1 + 
*                   (psCtrl_sCtrl_fltIGain + psCtrl_sCtrl_fltPGain) * (psUDQ_fltD - psCtrl_sIObsrv_fltD) 
*    fltTmpEObsrvQ = psCtrl_sCtrl_fltIQ_1 +
*                   (psCtrl_sCtrl_fltIGain + psCtrl_sCtrl_fltPGain) * (psUDQ_fltQ - psCtrl_sIObsrv_fltQ)
*
*    fltTmpIObsrvD = fltIGain * psCtrl_sIObsrv_fltD + fltUGain * psUDQ_fltD + 
*                    psIDQ_fltQ * fltSpeed * fltWIGain + fltEGain * fltTmpEObsrvD         
*    fltTmpIObsrvQ = fltIGain * psCtrl_sIObsrv_fltQ + fltUGain * psUDQ_fltQ - 
*                    psIDQ_fltD * fltSpeed * fltWIGain + fltEGain * fltTmpEObsrvQ
*
*
****************************************************************************/

static inline acc32_t AMCLIB_PMSMBemfObsrvDQMAU_A32fff_FCi(const GMCLIB_2COOR_DQ_T_FLT *psIDQ,
                                                           const GMCLIB_2COOR_DQ_T_FLT *psUDQ,
                                                           float_t fltSpeed, 
                                                           AMCLIB_BEMF_OBSRV_DQ_T_FLT *psCtrl)
{    
    register float_t fltTmpEObsrvD;
    register float_t fltTmpEObsrvQ;
    bool_t bErrFlag;
    
    AMCLIB_PMSMBemfObsrvDQNoAtan_A32fff_FC(psIDQ, psUDQ, fltSpeed, psCtrl);
    
    /* transfer BEMF to variables */
    fltTmpEObsrvD = psCtrl->sEObsrv.fltD;
    fltTmpEObsrvQ = psCtrl->sEObsrv.fltQ;
    
    /* BEMF determination according to speed sign */
    if (fltSpeed < 0.0F)
    {   fltTmpEObsrvD = MLIB_Neg_FLT(fltTmpEObsrvD);
    }
    else
    {   fltTmpEObsrvQ = MLIB_Neg_FLT(fltTmpEObsrvQ);
    }
    
    /* Error BEMF calculation in gamma/delta coordinates */
    psCtrl->a32Error = GFLIB_AtanYXMAU_A32f_FCi(fltTmpEObsrvD, fltTmpEObsrvQ, &bErrFlag); 
    
    return((acc32_t)psCtrl->a32Error);
} 
    
    
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_PMSM_BEMF_OBSRVMAU_FLT_DQ_H_ */       
