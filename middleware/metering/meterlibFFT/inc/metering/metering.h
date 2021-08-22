/************************************************************************************************
*
* Copyright 2013-2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*
*********************************************************************************************//*!
*
* @file      metering.h
*
* @author    B19186
* 
* @version   4.0.0.0
* 
* @date      March-23-2014
* 
* @brief     Basic FFT metering calculation header optimized by fractional arithmetic.
*
************************************************************************************************/
#ifndef __METERING_H__
#define __METERING_H__ 

/************************************************************************************************
 * internal macro definitions                                                                   *
 ***********************************************************************************************/
#define RES10    (long)10     /* integer voltage/current resolution is 1/10 [V,A] */
#define RES100   (long)100    /* integer voltage/current resolution is 1/100 [V,A] */
#define RES1000  (long)1000   /* integer voltage/current resolution is 1/1000 [V,A] - default */
#define RES10000 (long)10000  /* integer voltage/current resolution is 1/10000 [V,A] */

/************************************************************************************************
 * external macro definitions                                                                   *
 ***********************************************************************************************/
/************************************************************************************************
* Integer resolution of non-billing values (U,I,P,Q,S)
*
*//*! @addtogroup value_resolution
* @{
************************************************************************************************/
#define U_RES  (long)RES1000            ///< Integer voltage resolution is 1/U_RES [V] 
#define I_RES  (long)RES1000            ///< Integer current resolution is 1/I_RES [A] 
#define P_RES  (long)(U_RES*I_RES/10000)///< Integer power resolution is 100/(U_RES*I_RES) [W] */
/*! @} End of value_resolution                                                                 */

/************************************************************************************************
* Averaging macro definitions
*
*//*! @addtogroup aver_macro
* @{
************************************************************************************************/
/********************************************************************************************//*!
 * @brief   Two input values averaging
 * @details This function adds two input instantaneous values to two output respective registers. 
 * @param   aver1     64-bit averaged value 1 (this value will be increased by inst1 value)
 * @param   aver2     64-bit averaged value 2 (this value will be increased by inst2 value)
 * @param   ins1t     32/64-bit instantaneous value 1
 * @param   inst2     32/64-bit instantaneous value 2 
 * @note    Implemented as a function call.
 ***********************************************************************************************/ 
#define AVER(aver1, aver2, inst1, inst2) {(aver1) = LL_add((inst1),(aver1)); (aver2) = LL_add((inst2),(aver2)); }
/*! @} End of aver_macro                                                                       */

/************************************************************************************************
 * public function prototypes                                                                   *
 ***********************************************************************************************/    
/********************************************************************************************//*!
*
* @brief   Non-billing (P,Q) calculation function
*
* @return  u_re        pointer to the real part of the voltage buffer (frequency domain)
* @return  i_re        pointer to the real part of the current buffer (frequency domain)
* @return  u_im        pointer to the imaginary part of the voltage buffer (frequency domain)
* @return  i_im        pointer to the imaginary part of the current buffer (frequency domain)
* @param   p           pointer to the non-scaled active power
* @param   q           pointer to the non-scaled reactive power
* @param   harm        number of FFT harmonics (half of the input buffers length, power-of-two)
*         
* @remarks Function returns non-billing P and Q values in integer non-scaled form. 
*          These values will be used for billing calculation later. The first buffer position 
*          matches with zero harmonic, etc.
*
* @note    Total apparent power will be computed from both U-rms and I-rms values in the
*          @ref Scaling() function later.
*
************************************************************************************************/
extern void PQCalculation(frac24 *u_re, frac24 *i_re, frac24 *u_im, frac24 *i_im, frac64 *p, frac64 *q, unsigned long harm);

/********************************************************************************************//*!
*
* @brief   Non-billing (True Irms, THDi) calculation function
*
* @return  i_re        pointer to the real part of the current buffer (frequency domain)
* @return  i_im        pointer to the imaginary part of the current buffer (frequency domain)
* @param   i           pointer to the non-scaled Irms with offset correction
* @param   thdi        pointer to the non-scaled THDi
* @param   thdi_scl    pointer to the THDi dynamic scaling factor (thdi is scaled to FRAC32(1)>>thdi_scl)
* @param   harm        number of FFT harmonics (half of the input buffers length, power-of-two)
* @param   i_offs      pointer to the offset correction value (Q0.23 data format). If the offset    
*                      correction is not used, this pointer should be assigned to NULL (non True 
*                      Irms computing in this case). 
*         
* @remarks Function returns non-billing True Irms and THDi values in integer non-scaled form. 
*          These values may be used for data recording, visualization, S-power computing, and 
*          other auxiliary computation. The first buffer position matches with zero harmonic,etc.
* @note Irms calculation applies the DC (AFE) offsets due to right Irms calculation in case of 
*       non-harmonic current signals. The function also corrects i_re[0] cell.
*
************************************************************************************************/
extern void ICalculation(frac24 *i_re, frac24 *i_im, frac32 *i, frac32 *thdi, unsigned short *thdi_scl, unsigned long harm, frac24 *i_offs);

/********************************************************************************************//*!
*
* @brief   Non-billing (Urms, THDu) calculation function
*
* @return  u_re        pointer to the real part of the voltage buffer (frequency domain)
* @return  u_im        pointer to the imaginary part of the voltage buffer (frequency domain)
* @param   u           pointer to the non-scaled Urms
* @param   thdu        pointer to the non-scaled THDu
* @param   thdu_scl    pointer to the THDu dynamic scaling factor (thdu is scaled to FRAC32(1)>>thdu_scl)
* @param   harm        number of FFT harmonics (half of the input buffers length, power-of-two)
*         
* @remarks Function returns non-billing Urms and THDu values in integer non-scaled form. 
*          These values may be used for data recording, visualization, S-power computing, and 
*          other auxiliary computation. The first buffer position matches with zero harmonic,etc.
* @note Urms calculation ignore DC offsets. It is simplification due to zero DC offset in the mains.
*
************************************************************************************************/
extern void UCalculation(frac24 *u_re, frac24 *u_im, frac32 *u, frac32 *thdu, unsigned short *thdu_scl, unsigned long harm);

/********************************************************************************************//*!
*
* @brief   Non-billing (P,Q,S,U,I) scaling function
*
* @param   p           input/output pointer to the active power (non-scaled/scaled value) with offset correction
* @param   q           input/output pointer to the reactive power (non-scaled/scaled value) with offset correction
* @param   s           input/output pointer to the apparent power (non-scaled/scaled value)
* @param   u           input/output pointer to the Urms (non-scaled/scaled value)
* @param   i           input/output pointer to the Irms (non-scaled/scaled value)
* @param   u_gain      voltage gain scaling factor (double precision) [V]
* @param   i_gain      current gain scaling factor (double precision) [A]
* @param   p_offs      active power correction offset value (double precision) [W]
* @param   q_offs      reactive power correction offset value (double precision) [VAr]
*         
* @remarks Input non-billing values will be scaled to engineering units after calling this 
*          function. Powers resolution is given by @ref P_RES. Voltage resolution is given by
*          @ref U_RES. Current resolution is given by @ref I_RES.
*
************************************************************************************************/
extern void Scaling(frac64 *p, frac64 *q, frac64 *s, frac32 *u, frac32 *i, double u_gain, double i_gain, double p_offs, double q_offs);

/********************************************************************************************//*!
*
* @brief   Billing (energy) calculation function
*
* @param   energy      input/output pointer to the accumulated energy (active or reactive)
* @param   en_rem      input/output pointer to the energy remainder (active or reactive)
* @param   power       absolute value of the instantaneous power (active or reactive) 
* @param   coef        energy coefficient: 360*frequency [mHz] for 0.01Wh/VARh resolution
*                      or 3600*frequency [mHz] for 0.1Wh/VARh resolution
*
* @remarks It can be used for both active and reactive billing energy calculation. Both input
*          parameters set by energy and energyrem pointers will be rewrited after calling this function.
*
************************************************************************************************/
extern void EnergyCalculation (unsigned long *energy, unsigned long *en_rem, unsigned long power, unsigned long coef);

/********************************************************************************************//*!
*
* @brief   Phase shift correction function
*
* @param   i_re        input/output pointer to the real part of the current buffer (frequency domain)
* @param   i_im        input/output pointer to the imaginary part of the current buffer (frequency domain)
* @param   phase       input pointer to the phase shift buffer, should be assigned to NULL if the  
*                      phase shift compensation is not used
* @param   harm_fft    number of FFT harmonics (half of the input buffers length, power-of-two)
*
* @remarks It can be used for U-I phase shift correction in the frequency domain due to parasitic 
*          phase harmonic shifts of used current sensor. This idea is being patented (US 14471107). 
*          Both input current buffers will be rewrited after calling this function. The first 
*          buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void PhaseCorrection(frac24 *i_re, frac24 *i_im, frac32 *phase, unsigned long harm_fft);

/********************************************************************************************//*!
*
* @brief   Integration function
*
* @param   i_re        input/output pointer to the real part of the current buffer (frequency domain)
* @param   i_im        input/output pointer to the imaginary part of the current buffer (frequency domain)
* @param   harm_fft    number of FFT harmonics (half of the input buffers length, power-of-two)
*
* @remarks It's used for S/W integration in the frequency domain for derivative type of current
*          sensors (Rogowski coils). Both input current buffers will be rewrited after calling  
*          this function. The first buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void Integration(frac24 *i_re, frac24 *i_im, unsigned long harm_fft);

/********************************************************************************************//*!
*
* @brief   Harmonic magnitudes calculation function
*
* @param   u_re        input pointer to the real part of the voltage buffer (frequency domain)
* @param   i_re        input pointer to the real part of the current buffer (frequency domain)
* @param   u_im        input pointer to the imaginary part of the voltage buffer (frequency domain)
* @param   i_im        input pointer to the imaginary part of the current buffer (frequency domain)
* @param   u_mag       pointer to the output voltage magnitudes buffer (frequency domain)
* @param   i_mag       pointer to the output current magnitudes buffer (frequency domain)
* @param   magn_fft    number of computed magnitudes in the range <1...half of the input buffers length>
*
* @remarks It is used for auxiliary computing only (data visualization and monitoring).
*          The first buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void MagnCalculation(frac24 *u_re, frac24 *i_re, frac24 *u_im, frac24 *i_im, frac24 *u_mag, frac24 *i_mag, unsigned long magn_fft);

/********************************************************************************************//*!
*
* @brief   Harmonic phase shifts calculation function
*
* @param   u_re        input pointer to the real part of the voltage buffer (frequency domain)
* @param   i_re        input pointer to the real part of the current buffer (frequency domain)
* @param   u_im        input pointer to the imaginary part of the voltage buffer (frequency domain)
* @param   i_im        input pointer to the imaginary part of the current buffer (frequency domain)
* @return  u_ph        pointer to the voltage phase shifts buffer (frequency domain)
* @return  i_ph        pointer to the current phase shifts buffer (frequency domain)
* @param   ph_fft      number of computed phase shifts in the range <1...half of the input buffers length>
*
* @remarks It is used for auxiliary purposes only (data visualization and monitoring).
*          The first buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void PhaseCalculation(frac24 *u_re, frac24 *i_re, frac24 *u_im, frac24 *i_im, frac32 *u_ph, frac32 *i_ph, unsigned long ph_fft);

#endif /* __METERING_H__ */ 