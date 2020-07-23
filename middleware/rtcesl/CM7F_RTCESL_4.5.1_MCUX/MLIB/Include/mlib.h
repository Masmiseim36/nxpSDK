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
* @brief  Main MLIB header file for devices without FPU.
* 
*******************************************************************************/
#ifndef _MLIB_H_
#define _MLIB_H_

#if defined(__cplusplus)
extern "C" {
#endif

/******************************************************************************
* Includes
******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Abs_F16.h"
#include "MLIB_Abs_F16_Asmi.h"
#include "MLIB_Abs_F32.h"
#include "MLIB_Abs_F32_Asmi.h"
#include "MLIB_Add4_F16_Asmi.h"
#include "MLIB_Add4_F32_Asmi.h"
#include "MLIB_Add_F16.h"
#include "MLIB_Add_F16_Asmi.h"
#include "MLIB_Add_F32.h"
#include "MLIB_Add_F32_Asmi.h"  
#include "MLIB_Add_A32.h"  
#include "MLIB_Clb_F16.h"
#include "MLIB_Clb_F32.h"
#include "MLIB_Conv_F32.h"
#include "MLIB_Div1Q_F16.h"
#include "MLIB_Div1Q_F32.h"
#include "MLIB_Div1Q_A32.h"
#include "MLIB_Div_F16.h"
#include "MLIB_Div_F32.h"
#include "MLIB_Div_A32.h"
#include "MLIB_Log2_U16.h"
#include "MLIB_Mac4Rnd_F16_Asmi.h"
#include "MLIB_Mac4Rnd_F32.h"
#include "MLIB_Mac4Rnd_F32_Asmi.h"
#include "MLIB_Mac4_F32.h"
#include "MLIB_Mac4_F32_Asmi.h"
#include "MLIB_MacRnd_F16_Asmi.h"
#include "MLIB_MacRnd_F32.h"
#include "MLIB_MacRnd_F32_Asmi.h"
#include "MLIB_MacRnd_A32.h"  
#include "MLIB_Mac_F16_Asmi.h"  
#include "MLIB_Mac_F32_Asmi.h"  
#include "MLIB_Mac_A32.h"  
#include "MLIB_Mnac_F32.h"   
#include "MLIB_Mnac_A32.h"
#include "MLIB_Mnac_F16_Asmi.h"
#include "MLIB_MnacRnd_F16_Asmi.h"
#include "MLIB_MnacRnd_F32.h"
#include "MLIB_MnacRnd_F32_Asmi.h"
#include "MLIB_Mnac_F32_Asmi.h"
#include "MLIB_MnacRnd_A32.h"
#include "MLIB_Mnac_F32.h"     
#include "MLIB_Msu4Rnd_F16.h"
#include "MLIB_Msu4Rnd_F16_Asmi.h"
#include "MLIB_Msu4Rnd_F32.h"
#include "MLIB_Msu4Rnd_F32_Asmi.h"
#include "MLIB_Msu4_F32.h"
#include "MLIB_Msu4_F32_Asmi.h"
#include "MLIB_MsuRnd_F16_Asmi.h"
#include "MLIB_MsuRnd_F32.h"
#include "MLIB_MsuRnd_F32_Asmi.h"
#include "MLIB_MsuRnd_A32.h"
#include "MLIB_Msu_F16_Asmi.h"
#include "MLIB_Msu_F32_Asmi.h"
#include "MLIB_Msu_A32.h"
#include "MLIB_MulRnd_F16.h"
#include "MLIB_MulRnd_F16_Asmi.h"
#include "MLIB_MulRnd_F32.h"
#include "MLIB_MulRnd_F32_Asmi.h"
#include "MLIB_MulRnd_A32.h"
#include "MLIB_MulRnd_A32_Asmi.h"
#include "MLIB_Mul_F16.h"
#include "MLIB_Mul_F16_Asmi.h"
#include "MLIB_Mul_F32.h"
#include "MLIB_Mul_F32_Asmi.h"
#include "MLIB_Mul_A32.h"
#include "MLIB_Mul_A32_Asmi.h"
#include "MLIB_Neg_F16.h"
#include "MLIB_Neg_F16_Asmi.h"
#include "MLIB_Neg_F32.h"
#include "MLIB_Neg_F32_Asmi.h"  
#include "MLIB_Rcp1Q_A32.h"
#include "MLIB_Rcp_A32.h"
#include "MLIB_Rnd_F32.h"
#include "MLIB_Rnd_F32_Asmi.h"
#include "MLIB_Sat_A32_Asmi.h"
#include "MLIB_Shift_F16.h"
#include "MLIB_Shift_F16_Asmi.h"
#include "MLIB_Shift_F32.h"
#include "MLIB_Shift_F32_Asmi.h"
#include "MLIB_BiShift_F16.h"
#include "MLIB_BiShift_F32.h"
#include "MLIB_Sign_F16_Asmi.h"
#include "MLIB_Sign_F32_Asmi.h"
#include "MLIB_Sub4_F16_Asmi.h"
#include "MLIB_Sub4_F32_Asmi.h"
#include "MLIB_Sub_F16.h"
#include "MLIB_Sub_F16_Asmi.h"
#include "MLIB_Sub_F32.h"
#include "MLIB_Sub_F32_Asmi.h"
#include "MLIB_Sub_A32.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#if (defined(RAM_OPTIM_HIGH) || defined(RAM_OPTIM_MEDIUM) || defined(RAM_OPTIM_LOW)) /* placed to RAM */

#define MLIB_Div1QSat_F16ll(f32Num, f32Denom)                   MLIB_Div1QSat_F16ll_AsmRam(f32Num, f32Denom)
#define MLIB_Div1QSat_F32(f32Num, f32Denom)                     MLIB_Div1QSat_F32_AsmRam(f32Num, f32Denom)  
#define MLIB_Div1QSat_F32ls(f32Num, f16Denom)                   MLIB_Div1QSat_F32ls_CRam(f32Num, f16Denom)
#define MLIB_Div1QSat_A32as(a32Num, f16Denom)                   MLIB_Div1QSat_F32ls_CRam(a32Num, f16Denom)
#define MLIB_Div1Q_A32ll(f32Num, f32Denom)                      MLIB_Div1Q_A32ll_AsmRam(f32Num, f32Denom)
#define MLIB_Div1Q_F16ll(f32Num, f32Denom)                      MLIB_Div1Q_F16ll_AsmRam(f32Num, f32Denom)
#define MLIB_Div1Q_F32(f32Num, f32Denom)                        MLIB_Div1Q_F32_AsmRam(f32Num, f32Denom)  
#define MLIB_Div1Q_F32ls(f32Num, f16Denom)                      MLIB_Div1Q_F32ls_CRam(f32Num, f16Denom) 
#define MLIB_Div1Q_A32as(a32Num, f16Denom)                      MLIB_Div1Q_F32ls_CRam(a32Num, f16Denom)
#define MLIB_DivSat_F16ll(f32Num, f32Denom)                     MLIB_DivSat_F16ll_AsmRam(f32Num, f32Denom)
#define MLIB_DivSat_F32(f32Num, f32Denom)                       MLIB_DivSat_F32_AsmRam(f32Num, f32Denom)  
#define MLIB_DivSat_F32ls(f32Num, f16Denom)                     MLIB_DivSat_F32ls_CRam(f32Num, f16Denom)
#define MLIB_DivSat_A32as(a32Num, f16Denom)                     MLIB_DivSat_F32ls_CRam(a32Num, f16Denom)
#define MLIB_Div_A32ll(f32Num, f32Denom)                        MLIB_Div_A32ll_AsmRam(f32Num, f32Denom)
#define MLIB_Div_A32as(a32Num, f16Denom)                        MLIB_Div_F32ls_CRam(a32Num, f16Denom)
#define MLIB_Div_F16ll(f32Num, f32Denom)                        MLIB_Div_F16ll_AsmRam(f32Num, f32Denom)
#define MLIB_Div_F32(f32Num, f32Denom)                          MLIB_Div_F32_AsmRam(f32Num, f32Denom)  
#define MLIB_Div_F32ls(f32Num, f16Denom)                        MLIB_Div_F32ls_CRam(f32Num, f16Denom)

#else
	
#define MLIB_Div1QSat_F16ll(f32Num, f32Denom)                   MLIB_Div1QSat_F16ll_Asm(f32Num, f32Denom)
#define MLIB_Div1QSat_F32(f32Num, f32Denom)                     MLIB_Div1QSat_F32_Asm(f32Num, f32Denom)  
#define MLIB_Div1QSat_F32ls(f32Num, f16Denom)                   MLIB_Div1QSat_F32ls_C(f32Num, f16Denom)
#define MLIB_Div1QSat_A32as(a32Num, f16Denom)                   MLIB_Div1QSat_F32ls_C(a32Num, f16Denom)
#define MLIB_Div1Q_A32ll(f32Num, f32Denom)                      MLIB_Div1Q_A32ll_Asm(f32Num, f32Denom)
#define MLIB_Div1Q_F16ll(f32Num, f32Denom)                      MLIB_Div1Q_F16ll_Asm(f32Num, f32Denom)
#define MLIB_Div1Q_F32(f32Num, f32Denom)                        MLIB_Div1Q_F32_Asm(f32Num, f32Denom)  
#define MLIB_Div1Q_F32ls(f32Num, f16Denom)                      MLIB_Div1Q_F32ls_C(f32Num, f16Denom) 
#define MLIB_Div1Q_A32as(a32Num, f16Denom)                      MLIB_Div1Q_F32ls_C(a32Num, f16Denom)
#define MLIB_DivSat_F16ll(f32Num, f32Denom)                     MLIB_DivSat_F16ll_Asm(f32Num, f32Denom)
#define MLIB_DivSat_F32(f32Num, f32Denom)                       MLIB_DivSat_F32_Asm(f32Num, f32Denom)  
#define MLIB_DivSat_F32ls(f32Num, f16Denom)                     MLIB_DivSat_F32ls_C(f32Num, f16Denom)
#define MLIB_DivSat_A32as(a32Num, f16Denom)                     MLIB_DivSat_F32ls_C(a32Num, f16Denom)
#define MLIB_Div_A32ll(f32Num, f32Denom)                        MLIB_Div_A32ll_Asm(f32Num, f32Denom)
#define MLIB_Div_A32as(a32Num, f16Denom)                        MLIB_Div_F32ls_C(a32Num, f16Denom)
#define MLIB_Div_F16ll(f32Num, f32Denom)                        MLIB_Div_F16ll_Asm(f32Num, f32Denom)
#define MLIB_Div_F32(f32Num, f32Denom)                          MLIB_Div_F32_Asm(f32Num, f32Denom)  
#define MLIB_Div_F32ls(f32Num, f16Denom)                        MLIB_Div_F32ls_C(f32Num, f16Denom)

#endif /* defined(RAM_OPTIM_HIGH) || defined(RAM_OPTIM_MEDIUM) || defined(RAM_OPTIM_LOW) */

#define MLIB_AbsSat_F16(f16Val)                                 MLIB_AbsSat_F16_Asmi(f16Val)
#define MLIB_AbsSat_F32(f32Val)                                 MLIB_AbsSat_F32_Asmi(f32Val)
#define MLIB_Abs_F16(f16Val)                                    MLIB_Abs_F16_Ci(f16Val)  
#define MLIB_Abs_F32(f32Val)                                    MLIB_Abs_F32_Ci(f32Val)
#define MLIB_Add4Sat_F16(f16Add1, f16Add2, f16Add3, f16Add4)    MLIB_Add4Sat_F16_Asmi(f16Add1, f16Add2, f16Add3, f16Add4)
#define MLIB_Add4Sat_F32(f32Add1, f32Add2, f32Add3, f32Add4)    MLIB_Add4Sat_F32_Asmi(f32Add1, f32Add2, f32Add3, f32Add4)
#define MLIB_Add4_F16(f16Add1, f16Add2, f16Add3, f16Add4)       MLIB_Add4_F16_Asmi(f16Add1, f16Add2, f16Add3, f16Add4)
#define MLIB_Add4_F32(f32Add1, f32Add2, f32Add3, f32Add4)       MLIB_Add4_F32_Asmi(f32Add1, f32Add2, f32Add3, f32Add4)
#define MLIB_AddSat_F16(f16Add1, f16Add2)                       MLIB_AddSat_F16_Asmi(f16Add1, f16Add2)
#define MLIB_AddSat_F32(f32Add1, f32Add2)                       MLIB_AddSat_F32_Asmi(f32Add1, f32Add2)
#define MLIB_Add_A32as(a32Accum, f16Add)                        MLIB_Add_A32as_Ci(a32Accum, f16Add)
#define MLIB_Add_A32ss(f16Add1, f16Add2)                        MLIB_Add_A32ss_Ci(f16Add1, f16Add2) 
#define MLIB_Add_F16(f16Add1, f16Add2)                          MLIB_Add_F16_Ci(f16Add1, f16Add2) 
#define MLIB_Add_F32(f32Add1, f32Add2)                          MLIB_Add_F32_Ci(f32Add1, f32Add2) 
#define MLIB_Clb_U16l(f32Val)                                   MLIB_Clb_U16l_Ci(f32Val)   
#define MLIB_Clb_U16s(f16Val)                                   MLIB_Clb_U16s_Ci(f16Val)  
#define MLIB_Conv_F16l(f32Val)                                  MLIB_Conv_F16l_Ci(f32Val) 
#define MLIB_Conv_F32s(f16Val)                                  MLIB_Conv_F32s_Ci(f16Val)
#define MLIB_Div1QSat_F16(f16Num, f16Denom)                     MLIB_Div1QSat_F16_Ci(f16Num, f16Denom)
#define MLIB_Div1QSat_F16ls(f32Num, f16Denom)                   MLIB_Div1QSat_F16ls_Ci(f32Num, f16Denom) 
#define MLIB_Div1Q_A32ls(f32Num, f16Denom)                      MLIB_Div1Q_A32ls_Ci(f32Num, f16Denom)  
#define MLIB_Div1Q_A32ss(f16Num, f16Denom)                      MLIB_Div1Q_A32ss_Ci(f16Num, f16Denom)   
#define MLIB_Div1Q_F16(f16Num, f16Denom)                        MLIB_Div1Q_F16_Ci(f16Num, f16Denom)
#define MLIB_Div1Q_F16ls(f32Num, f16Denom)                      MLIB_Div1Q_F16ls_Ci(f32Num, f16Denom)    
#define MLIB_DivSat_F16(f16Num, f16Denom)                       MLIB_DivSat_F16_Ci(f16Num, f16Denom)  
#define MLIB_DivSat_F16ls(f32Num, f16Denom)                     MLIB_DivSat_F16ls_Ci(f32Num, f16Denom)    
#define MLIB_Div_A32ls(f32Num, f16Denom)                        MLIB_Div_A32ls_Ci(f32Num, f16Denom)
#define MLIB_Div_A32ss(f16Num, f16Denom)                        MLIB_Div_A32ss_Ci(f16Num, f16Denom)
#define MLIB_Div_F16(f16Num, f16Denom)                          MLIB_Div_F16_Ci(f16Num, f16Denom) 
#define MLIB_Div_F16ls(f32Num, f16Denom)                        MLIB_Div_F16ls_Ci(f32Num, f16Denom)
#define MLIB_Log2_U16(u16Val)                                   MLIB_Log2_U16_Ci(u16Val)
#define MLIB_Mac4RndSat_F16(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)     MLIB_Mac4RndSat_F16_Asmi(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)   
#define MLIB_Mac4RndSat_F32(f32Add1Mul1, f32Add1Mul2, f32Add2Mul1, f32Add2Mul2)     MLIB_Mac4RndSat_F32_Asmi(f32Add1Mul1, f32Add1Mul2, f32Add2Mul1, f32Add2Mul2)   
#define MLIB_Mac4Rnd_F16(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)        MLIB_Mac4Rnd_F16_Asmi(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)   
#define MLIB_Mac4Rnd_F32(f32Add1Mul1, f32Add1Mul2, f32Add2Mul1, f32Add2Mul2)        MLIB_Mac4Rnd_F32_Ci(f32Add1Mul1, f32Add1Mul2, f32Add2Mul1, f32Add2Mul2)     
#define MLIB_Mac4Sat_F32ssss(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)    MLIB_Mac4Sat_F32ssss_Asmi(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)   
#define MLIB_Mac4_F32ssss(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)       MLIB_Mac4_F32ssss_Ci(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)   
#define MLIB_MacRndSat_F16( f16Accum, f16Mult1, f16Mult2)                           MLIB_MacRndSat_F16_Asmi( f16Accum, f16Mult1, f16Mult2)    
#define MLIB_MacRndSat_F32( f32Accum, f32Mult1, f32Mult2)                           MLIB_MacRndSat_F32_Asmi( f32Accum, f32Mult1, f32Mult2)   
#define MLIB_MacRndSat_F32lls( f32Accum, f32Mult1, f16Mult2)                        MLIB_MacRndSat_F32lls_Asmi( f32Accum, f32Mult1, f16Mult2)  
#define MLIB_MacRnd_A32ass(a32Accum, f16Mult1, f16Mult2)                            MLIB_MacRnd_A32ass_Ci(a32Accum, f16Mult1, f16Mult2)  
#define MLIB_MacRnd_F16( f16Accum, f16Mult1, f16Mult2)                              MLIB_MacRnd_F16_Asmi( f16Accum, f16Mult1, f16Mult2) 
#define MLIB_MacRnd_F32( f32Accum, f32Mult1, f32Mult2)                              MLIB_MacRnd_F32_Asmi( f32Accum, f32Mult1, f32Mult2) 
#define MLIB_MacRnd_F32lls( f32Accum, f32Mult1, f16Mult2)                           MLIB_MacRnd_F32lls_Ci( f32Accum, f32Mult1, f16Mult2)
#define MLIB_MacSat_F16( f16Accum, f16Mult1, f16Mult2)                              MLIB_MacSat_F16_Asmi( f16Accum, f16Mult1, f16Mult2)  
#define MLIB_MacSat_F32( f32Accum, f32Mult1, f32Mult2)                              MLIB_MacSat_F32_Asmi( f32Accum, f32Mult1, f32Mult2) 
#define MLIB_MacSat_F32lss( f32Accum, f16Mult1, f16Mult2)                           MLIB_MacSat_F32lss_Asmi( f32Accum, f16Mult1, f16Mult2)
#define MLIB_Mac_A32ass(a32Accum, f16Mult1, f16Mult2)                               MLIB_Mac_A32ass_Ci(a32Accum, f16Mult1, f16Mult2)
#define MLIB_Mac_F16( f16Accum, f16Mult1, f16Mult2)                                 MLIB_Mac_F16_Asmi( f16Accum, f16Mult1, f16Mult2) 
#define MLIB_Mac_F32( f32Accum, f32Mult1, f32Mult2)                                 MLIB_Mac_F32_Asmi( f32Accum, f32Mult1, f32Mult2) 
#define MLIB_Mac_F32lss( f32Accum, f16Mult1, f16Mult2)                              MLIB_Mac_F32lss_Asmi( f32Accum, f16Mult1, f16Mult2) 
#define MLIB_MnacRndSat_F16( f16Accum, f16Mult1, f16Mult2)                          MLIB_MnacRndSat_F16_Asmi( f16Accum, f16Mult1, f16Mult2)     
#define MLIB_MnacRndSat_F32( f32Accum, f32Mult1, f32Mult2)                          MLIB_MnacRndSat_F32_Asmi( f32Accum, f32Mult1, f32Mult2)     
#define MLIB_MnacRndSat_F32lls( f32Accum, f32Mult1, f16Mult2)                       MLIB_MnacRndSat_F32lls_Asmi( f32Accum, f32Mult1, f16Mult2) 
#define MLIB_MnacRnd_A32ass(a32Accum, f16Mult1, f16Mult2)                           MLIB_MnacRnd_A32ass_Ci(a32Accum, f16Mult1, f16Mult2)        
#define MLIB_MnacRnd_F16( f16Accum, f16Mult1, f16Mult2)                             MLIB_MnacRnd_F16_Asmi( f16Accum, f16Mult1, f16Mult2)        
#define MLIB_MnacRnd_F32( f32Accum, f32Mult1, f32Mult2)                             MLIB_MnacRnd_F32_Asmi( f32Accum, f32Mult1, f32Mult2)        
#define MLIB_MnacRnd_F32lls( f32Accum, f32Mult1, f16Mult2)                          MLIB_MnacRnd_F32lls_Ci( f32Accum, f32Mult1, f16Mult2)       
#define MLIB_MnacSat_F16( f16Accum, f16Mult1, f16Mult2)                             MLIB_MnacSat_F16_Asmi( f16Accum, f16Mult1, f16Mult2)        
#define MLIB_MnacSat_F32( f32Accum, f32Mult1, f32Mult2)                             MLIB_MnacSat_F32_Asmi( f32Accum, f32Mult1, f32Mult2)        
#define MLIB_MnacSat_F32lss( f32Accum, f16Mult1, f16Mult2)                          MLIB_MnacSat_F32lss_Asmi( f32Accum, f16Mult1, f16Mult2)     
#define MLIB_Mnac_A32ass(a32Accum, f16Mult1, f16Mult2)                              MLIB_Mnac_A32ass_Ci(a32Accum, f16Mult1, f16Mult2)           
#define MLIB_Mnac_F16( f16Accum, f16Mult1, f16Mult2)                                MLIB_Mnac_F16_Asmi( f16Accum, f16Mult1, f16Mult2)           
#define MLIB_Mnac_F32( f32Accum, f32Mult1, f32Mult2)                                MLIB_Mnac_F32_Asmi( f32Accum, f32Mult1, f32Mult2)           
#define MLIB_Mnac_F32lss( f32Accum, f16Mult1, f16Mult2)                             MLIB_Mnac_F32lss_Asmi( f32Accum, f16Mult1, f16Mult2)           
#define MLIB_Msu4RndSat_F16( f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2)        MLIB_Msu4RndSat_F16_Asmi( f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2)  
#define MLIB_Msu4RndSat_F32( f32MinMul1, f32MinMul2, f32SubMul1, f32SubMul2)        MLIB_Msu4RndSat_F32_Asmi( f32MinMul1, f32MinMul2, f32SubMul1, f32SubMul2)
#define MLIB_Msu4Rnd_F16( f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2)           MLIB_Msu4Rnd_F16_Ci( f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2) 
#define MLIB_Msu4Rnd_F32( f32MinMul1, f32MinMul2, f32SubMul1, f32SubMul2)           MLIB_Msu4Rnd_F32_Ci( f32MinMul1, f32MinMul2, f32SubMul1, f32SubMul2) 
#define MLIB_Msu4Sat_F32ssss(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2)        MLIB_Msu4Sat_F32ssss_Asmi(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2) 
#define MLIB_Msu4_F32ssss(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2)           MLIB_Msu4_F32ssss_Ci(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2) 
#define MLIB_MsuRndSat_F16( f16Accum, f16Mult1, f16Mult2)                           MLIB_MsuRndSat_F16_Asmi( f16Accum, f16Mult1, f16Mult2)
#define MLIB_MsuRndSat_F32( f32Accum, f32Mult1, f32Mult2)                           MLIB_MsuRndSat_F32_Asmi( f32Accum, f32Mult1, f32Mult2)  
#define MLIB_MsuRndSat_F32lls( f32Accum, f32Mult1, f16Mult2)                        MLIB_MsuRndSat_F32lls_Asmi( f32Accum, f32Mult1, f16Mult2)
#define MLIB_MsuRnd_A32ass( a32Accum, f16Mult1, f16Mult2)                           MLIB_MsuRnd_A32ass_Ci( a32Accum, f16Mult1, f16Mult2)
#define MLIB_MsuRnd_F16( f16Accum, f16Mult1, f16Mult2)                              MLIB_MsuRnd_F16_Asmi( f16Accum, f16Mult1, f16Mult2) 
#define MLIB_MsuRnd_F32( f32Accum, f32Mult1, f32Mult2)                              MLIB_MsuRnd_F32_Ci( f32Accum, f32Mult1, f32Mult2)    
#define MLIB_MsuRnd_F32lls( f32Accum, f32Mult1, f16Mult2)                           MLIB_MsuRnd_F32lls_Asmi( f32Accum, f32Mult1, f16Mult2)  
#define MLIB_MsuSat_F16( f16Accum, f16Mult1, f16Mult2)                              MLIB_MsuSat_F16_Asmi( f16Accum, f16Mult1, f16Mult2)
#define MLIB_MsuSat_F32( f32Accum, f32Mult1, f32Mult2)                              MLIB_MsuSat_F32_Asmi( f32Accum, f32Mult1, f32Mult2)  
#define MLIB_MsuSat_F32lss( f32Accum, f16Mult1, f16Mult2)                           MLIB_MsuSat_F32lss_Asmi( f32Accum, f16Mult1, f16Mult2)
#define MLIB_Msu_A32ass( a32Accum, f16Mult1, f16Mult2)                              MLIB_Msu_A32ass_Ci( a32Accum, f16Mult1, f16Mult2)
#define MLIB_Msu_F16( f16Accum, f16Mult1, f16Mult2)                                 MLIB_Msu_F16_Asmi( f16Accum, f16Mult1, f16Mult2) 
#define MLIB_Msu_F32( f32Accum, f32Mult1, f32Mult2)                                 MLIB_Msu_F32_Asmi( f32Accum, f32Mult1, f32Mult2)
#define MLIB_Msu_F32lss( f32Accum, f16Mult1, f16Mult2)                              MLIB_Msu_F32lss_Asmi( f32Accum, f16Mult1, f16Mult2)  
#define MLIB_MulNegRndSat_A32( a32Mult1, a32Mult2)              MLIB_MulNegRndSat_A32_Asmi( a32Mult1, a32Mult2)
#define MLIB_MulNegRndSat_F16as( a32Accum, f16Mult)             MLIB_MulNegRndSat_F16as_Asmi( a32Accum, f16Mult)
#define MLIB_MulNegRnd_F16( f16Mult1, f16Mult2)                 MLIB_MulNegRnd_F16_Ci( f16Mult1, f16Mult2)
#define MLIB_MulNegRnd_F16as( a32Accum, f16Mult)                MLIB_MulNegRnd_F16as_Ci( a32Accum, f16Mult)
#define MLIB_MulNegRnd_F32( f32Mult1, f32Mult2)                 MLIB_MulNegRnd_F32_Ci( f32Mult1, f32Mult2) 
#define MLIB_MulNegRnd_F32ls( f32Mult1, f16Mult2)               MLIB_MulNegRnd_F32ls_Ci( f32Mult1, f16Mult2)
#define MLIB_MulNegRnd_A32( a32Mult1, a32Mult2)                 MLIB_MulNegRnd_A32_Ci( a32Mult1, a32Mult2)
#define MLIB_MulNeg_F16( f16Mult1, f16Mult2)                    MLIB_MulNeg_F16_Ci( f16Mult1, f16Mult2)
#define MLIB_MulNeg_F16as( a32Accum, f16Mult)                   MLIB_MulNeg_F16as_Ci( a32Accum, f16Mult)
#define MLIB_MulNeg_F32( f32Mult1, f32Mult2)                    MLIB_MulNeg_F32_Ci( f32Mult1, f32Mult2)   
#define MLIB_MulNeg_F32ss( f16Mult1, f16Mult2)                  MLIB_MulNeg_F32ss_Ci( f16Mult1, f16Mult2)
#define MLIB_MulNeg_A32( a32Mult1, a32Mult2)                    MLIB_MulNeg_A32_Ci( a32Mult1, a32Mult2)
#define MLIB_MulNegSat_A32( a32Mult1, a32Mult2)                 MLIB_MulNegSat_A32_Asmi( a32Mult1, a32Mult2)
#define MLIB_MulNegSat_F16as( a32Accum, f16Mult)                MLIB_MulNegSat_F16as_Asmi( a32Accum, f16Mult)
#define MLIB_MulRndSat_F16( f16Mult1, f16Mult2)                 MLIB_MulRndSat_F16_Asmi( f16Mult1, f16Mult2)  
#define MLIB_MulRndSat_F16as( a32Accum, f16Mult)                MLIB_MulRndSat_F16as_Asmi( a32Accum, f16Mult)  
#define MLIB_MulRndSat_F32( f32Mult1, f32Mult2)                 MLIB_MulRndSat_F32_Asmi( f32Mult1, f32Mult2)    
#define MLIB_MulRndSat_F32ls( f32Mult1, f16Mult2)               MLIB_MulRndSat_F32ls_Asmi( f32Mult1, f16Mult2)
#define MLIB_MulRndSat_A32( a32Mult1, a32Mult2)                 MLIB_MulRndSat_A32_Asmi( a32Mult1, a32Mult2)
#define MLIB_MulRnd_F16( f16Mult1, f16Mult2)                    MLIB_MulRnd_F16_Ci( f16Mult1, f16Mult2) 
#define MLIB_MulRnd_F16as( a32Accum, f16Mult)                   MLIB_MulRnd_F16as_Ci( a32Accum, f16Mult) 
#define MLIB_MulRnd_F32( f32Mult1, f32Mult2)                    MLIB_MulRnd_F32_Ci( f32Mult1, f32Mult2)  
#define MLIB_MulRnd_F32ls( f32Mult1, f16Mult2)                  MLIB_MulRnd_F32ls_Ci( f32Mult1, f16Mult2)
#define MLIB_MulRnd_A32( a32Mult1, a32Mult2)                    MLIB_MulRnd_A32_Ci( a32Mult1, a32Mult2)
#define MLIB_MulSat_F16( f16Mult1, f16Mult2)                    MLIB_MulSat_F16_Asmi( f16Mult1, f16Mult2)  
#define MLIB_MulSat_F16as( a32Accum, f16Mult)                   MLIB_MulSat_F16as_Asmi( a32Accum, f16Mult)  
#define MLIB_MulSat_F32( f32Mult1, f32Mult2)                    MLIB_MulSat_F32_Asmi( f32Mult1, f32Mult2)    
#define MLIB_MulSat_F32ss( f16Mult1, f16Mult2)                  MLIB_MulSat_F32ss_Asmi( f16Mult1, f16Mult2)
#define MLIB_MulSat_A32( a32Mult1, a32Mult2)                    MLIB_MulSat_A32_Asmi( a32Mult1, a32Mult2)
#define MLIB_Mul_F16( f16Mult1, f16Mult2)                       MLIB_Mul_F16_Ci( f16Mult1, f16Mult2) 
#define MLIB_Mul_F16as( a32Accum, f16Mult)                      MLIB_Mul_F16as_Ci( a32Accum, f16Mult) 
#define MLIB_Mul_F32( f32Mult1, f32Mult2)                       MLIB_Mul_F32_Ci( f32Mult1, f32Mult2)  
#define MLIB_Mul_F32ss( f16Mult1, f16Mult2)                     MLIB_Mul_F32ss_Ci( f16Mult1, f16Mult2)   
#define MLIB_Mul_A32( a32Mult1, a32Mult2)                       MLIB_Mul_A32_Ci( a32Mult1, a32Mult2)
#define MLIB_NegSat_F16(f16Val)                                 MLIB_NegSat_F16_Asmi(f16Val)
#define MLIB_NegSat_F32(f32Val)                                 MLIB_NegSat_F32_Asmi(f32Val)
#define MLIB_Neg_F16(f16Val)                                    MLIB_Neg_F16_Ci(f16Val) 
#define MLIB_Neg_F32(f32Val)                                    MLIB_Neg_F32_Ci(f32Val) 
#define MLIB_Rcp1Q1_A32s(f16Denom)                              MLIB_Rcp1Q1_A32s_Ci(f16Denom)
#define MLIB_Rcp1Q_A32s(f16Denom)                               MLIB_Rcp1Q_A32s_Ci(f16Denom)
#define MLIB_Rcp1_A32s(f16Denom)                                MLIB_Rcp1_A32s_Ci(f16Denom)
#define MLIB_Rcp_A32s(f16Denom)                                 MLIB_Rcp_A32s_Ci(f16Denom) 
#define MLIB_RndSat_F16l(f32Val)                                MLIB_RndSat_F16l_Asmi(f32Val)
#define MLIB_Rnd_F16l(f32Val)                                   MLIB_Rnd_F16l_Ci(f32Val)
#define MLIB_Sat_F16a(a32Accum)                                 MLIB_Sat_F16a_Asmi(a32Accum)
#define MLIB_Sh1LSat_F16(f16Val)                                MLIB_Sh1LSat_F16_Asmi(f16Val)  
#define MLIB_Sh1LSat_F32(f32Val)                                MLIB_Sh1LSat_F32_Asmi(f32Val)
#define MLIB_Sh1L_F16(f16Val)                                   MLIB_Sh1L_F16_Ci(f16Val)
#define MLIB_Sh1L_F32(f32Val)                                   MLIB_Sh1L_F32_Ci(f32Val) 
#define MLIB_Sh1R_F16(f16Val)                                   MLIB_Sh1R_F16_Ci(f16Val) 
#define MLIB_Sh1R_F32(f32Val)                                   MLIB_Sh1R_F32_Ci(f32Val)   
#define MLIB_ShLBiSat_F16(f16Val, i16Sh)                        MLIB_ShLBiSat_F16_Ci(f16Val, i16Sh) 
#define MLIB_ShLBiSat_F32(f32Val, i16Sh)                        MLIB_ShLBiSat_F32_Ci(f32Val, i16Sh) 
#define MLIB_ShLBi_F16(f16Val, i16Sh)                           MLIB_ShLBi_F16_Ci(f16Val, i16Sh) 
#define MLIB_ShLBi_F32(f32Val, i16Sh)                           MLIB_ShLBi_F32_Ci(f32Val, i16Sh)
#define MLIB_ShLSat_F16(f16Val, u16Sh)                          MLIB_ShLSat_F16_Asmi(f16Val, u16Sh)
#define MLIB_ShLSat_F32(f32Val, u16Sh)                          MLIB_ShLSat_F32_Asmi(f32Val, u16Sh) 
#define MLIB_ShL_F16(f16Val, u16Sh)                             MLIB_ShL_F16_Ci(f16Val, u16Sh)
#define MLIB_ShL_F32(f32Val, u16Sh)                             MLIB_ShL_F32_Ci(f32Val, u16Sh)
#define MLIB_ShRBiSat_F16(f16Val, i16Sh)                        MLIB_ShRBiSat_F16_Ci(f16Val, i16Sh) 
#define MLIB_ShRBiSat_F32(f32Val, i16Sh)                        MLIB_ShRBiSat_F32_Ci(f32Val, i16Sh) 
#define MLIB_ShRBi_F16(f16Val, i16Sh)                           MLIB_ShRBi_F16_Ci(f16Val, i16Sh) 
#define MLIB_ShRBi_F32(f32Val, i16Sh)                           MLIB_ShRBi_F32_Ci(f32Val, i16Sh)
#define MLIB_ShR_F16(f16Val, u16Sh)                             MLIB_ShR_F16_Ci(f16Val, u16Sh)
#define MLIB_ShR_F32(f32Val, u16Sh)                             MLIB_ShR_F32_Ci(f32Val, u16Sh)
#define MLIB_Sign_F16(f16Val)                                   MLIB_Sign_F16_Asmi(f16Val)
#define MLIB_Sign_F32(f32Val)                                   MLIB_Sign_F32_Asmi(f32Val)
#define MLIB_Sub4Sat_F16(f16Min, f16Sub1, f16Sub2, f16Sub3)     MLIB_Sub4Sat_F16_Asmi(f16Min, f16Sub1, f16Sub2, f16Sub3)
#define MLIB_Sub4Sat_F32(f32Min, f32Sub1, f32Sub2, f32Sub3)     MLIB_Sub4Sat_F32_Asmi(f32Min, f32Sub1, f32Sub2, f32Sub3)
#define MLIB_Sub4_F16(f16Min, f16Sub1, f16Sub2, f16Sub3)        MLIB_Sub4_F16_Asmi(f16Min, f16Sub1, f16Sub2, f16Sub3) 
#define MLIB_Sub4_F32(f32Min, f32Sub1, f32Sub2, f32Sub3)        MLIB_Sub4_F32_Asmi(f32Min, f32Sub1, f32Sub2, f32Sub3) 
#define MLIB_SubSat_F16(f16Min, f16Sub)                         MLIB_SubSat_F16_Asmi(f16Min, f16Sub)
#define MLIB_SubSat_F32(f32Min, f32Sub)                         MLIB_SubSat_F32_Asmi(f32Min, f32Sub)
#define MLIB_Sub_A32as(a32Accum, f16Sub)                        MLIB_Sub_A32as_Ci(a32Accum, f16Sub)
#define MLIB_Sub_A32ss(f16Min, f16Sub)                          MLIB_Sub_A32ss_Ci(f16Min, f16Sub) 
#define MLIB_Sub_F16(f16Min, f16Sub)                            MLIB_Sub_F16_Ci(f16Min, f16Sub) 
#define MLIB_Sub_F32(f32Min, f32Sub)                            MLIB_Sub_F32_Ci(f32Min, f32Sub)

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_H_ */
