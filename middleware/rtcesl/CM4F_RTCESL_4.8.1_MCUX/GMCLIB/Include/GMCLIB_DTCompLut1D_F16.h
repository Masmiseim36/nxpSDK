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
* @brief  Dead Time Compensation Custom Calculation
* 
*******************************************************************************/

#ifndef _GMCLIB_DTCOMPLUT1D_F16_H_
#define _GMCLIB_DTCOMPLUT1D_F16_H_

#if defined(__cplusplus) 
extern "C" {
#endif
  
/****************************************************************************
* Includes
****************************************************************************/  
#include "mlib.h"
#include "gflib.h"  
#include "gmclib_types.h" 
  
/******************************************************************************
* Macros 
******************************************************************************/   
#define GMCLIB_DTCompLut1D_F16_C(psIABC, psUAlBe, f16U_DCB, psParam, psUAlBeDtComp) \
        GMCLIB_DTCompLut1D_F16_FC(psIABC, psUAlBe, f16U_DCB, psParam, psUAlBeDtComp)

/******************************************************************************
* Types 
******************************************************************************/     
typedef struct
{
    const frac16_t *pf16Table;
    uint16_t u16TableSize;
} GMCLIB_DTCOMPLUT1D_T_F16;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
extern void GMCLIB_DTCompLut1D_F16_FC(const GMCLIB_3COOR_T_F16 *psIABC,
                                      const GMCLIB_2COOR_ALBE_T_F16 *psUAlBe,
                                      frac16_t f16UDCBus,
                                      const GMCLIB_DTCOMPLUT1D_T_F16 *psParam,
                                      GMCLIB_2COOR_ALBE_T_F16 *psUAlBeDTComp);

#if defined(__cplusplus) 
}
#endif

#endif /* _GMCLIB_DTCOMPLUT1D_F16_H_ */
