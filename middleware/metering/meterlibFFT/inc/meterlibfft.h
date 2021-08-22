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
* @file      meterlibfft.h
*
* @author    B19186
* 
* @version   4.0.0.2
* 
* @date      June-22-2015
* 
* @brief     Common FFT Metering Library Header. 
*
************************************************************************************************/
#ifndef __METERLIBFFT_H__
#define __METERLIBFFT_H__ 

/************************************************************************************************
 * user data types and macro definitions                                                        *
 ***********************************************************************************************/
/*! FFT length (number of input samples, number of output harmonics */ 
typedef struct
{
    unsigned long samples;  /*!< Number of input samples-should be one of the @ref fft_samples */
    unsigned long harmonics;/*!< Number of output FFT harmonics - half of the FFT samples */
    unsigned long log_2;    /*!< log2(samples) */
} tFFT_LENGTH;

/*! Instantaneous power vector (U,I,P,Q,S,THD) data structure definition [engineering units] */ 
typedef struct
{
    frac32 urms;             /*!< Urms [mV] */
    frac32 irms;             /*!< Irms [mA] */
    frac64 p;                /*!< Signed Active Power [0.1 mW] */
    frac64 q;                /*!< Signed Reactive Power [0.1 mVAR] */
    frac64 s;                /*!< Total apparent Power [0.1 mVA] */
    frac32 thdu;             /*!< THDu [-] (scaled to FRAC32(1)>>thdu_scl) */
    frac32 thdi;             /*!< THDi [-] (scaled to FRAC32(1)>>thdi_scl) */
    unsigned short thdu_scl; /*!< THDu dynamic scaling factor */
    unsigned short thdi_scl; /*!< THDi dynamic scaling factor */
} tFFT_INST;

/*! Averaged power vector (U,I,P,Q,S,THD) data structure definition [engineering units] */ 
typedef struct
{
    frac64 urms;     /*!< Urms [mV] */
    frac64 irms;     /*!< Irms [mA] */
    frac64 p;        /*!< Signed Active Power [0.1 mW] */
    frac64 q;        /*!< Signed Reactive Power [0.1 mVAR] */
    frac64 s;        /*!< Total apparent Power [0.1 mVA] */
    frac64 thdu;     /*!< Total Harmonic Distortion of U [-] */
    frac64 thdi;     /*!< Total Harmonic Distortion of I [-] */
} tFFT_AVRG;

/*! Cartesian data structure definition [not scaled] */ 
typedef struct {
    frac24 *re;      /*!< Pointer to the real complex part of the U or I buffer in Q0.23 data format [bit] */
    frac24 *im;      /*!< Pointer to the imaginary complex part of the U or I buffer in Q0.23 data format [bit] */
} tFFT_CARTESIAN;

/*! Polar data structure definition [bit/0.001°] */ 
typedef struct {
    frac24 *magn;    /*!< Pointer to the U or I magnitudes buffer in Q0.23 data format [bit] */
    long   *phase;   /*!< Pointer to the U or I phase shifts buffer [0.001°] */ 
} tFFT_POLAR;

/*! Gain and offset calibration coefficients [V,A,bit] */ 
typedef struct {
    double u_max;    /*!< Maximum voltage [V] per maximum (23-bit) AFE range */
    double i_max;    /*!< Maximum current [A] per maximum (23-bit) AFE range */
    frac24 *i_offs;  /*!< Pointer to the current AFE offset in Q0.23 data format [bit] */
    double p_offs;   /*!< Pointer to the active power offset [W] */
    double q_offs;   /*!< Pointer to the reactive power offset [VAr] */
} tFFT_CALIB;

/*! Mandatory FFT-U and FFT-I structure definition (Cartesian data form, not scaled) */ 
typedef struct {
    tFFT_CARTESIAN u; /*!< Main voltage vector (re+im) */
    tFFT_CARTESIAN i; /*!< Main current vector (re+im) */
} tFFT_MAINS;

/*! Auxiliary FFT-U and FFT-I structure definition (Polar data form, bit/0.001°) */ 
typedef struct {
    tFFT_POLAR u;     /*!< Auxiliary voltage vector (magn+phase) */
    tFFT_POLAR i;     /*!< Auxiliary current vector (magn+phase) */
} tFFT_AUX;

/*! Accumulative energy counter structure definition [engineering units] */ 
typedef struct
{
    unsigned long wh_t;   /*!< Total Active Energy (see @ref energy_res for resolution) or its remainder value */
    unsigned long wh_i;   /*!< Import Active Energy (see @ref energy_res for resolution) or its remainder value */
    unsigned long wh_e;   /*!< Export Active Energy (see @ref energy_res for resolution) or its remainder value */
    unsigned long varh_t; /*!< Total Reactive Energy (see @ref energy_res for resolution) or its remainder value */
    unsigned long varh_i; /*!< Import Reactive Energy (see @ref energy_res for resolution) or its remainder value */
    unsigned long varh_e; /*!< Export Reactive Energy (see @ref energy_res for resolution) or its remainder value */
} tENERGY_REG;

/*! Energy LEDs structure definition */ 
typedef struct
{
    unsigned long kwh_cnt;    /*!< kWh impulse number - should be one of the following @ref impulse_num */                
    unsigned long wh_flash;   /*!< Latest Wh energy value when kWh LED flashed */
    unsigned long wh_incr;    /*!< Wh increment (depends on @ref energy_res and @ref impulse_num) */
    unsigned long kvarh_cnt;  /*!< kVARh impulse number - should be one of the following @ref impulse_num */       
    unsigned long varh_flash; /*!< Latest VARh energy value when kVARh LED flashed */            
    unsigned long varh_incr;  /*!< VARh increment (depends on @ref energy_res and @ref impulse_num) */
    unsigned long coeff;      /*!< Hour coefficient (depends on @ref energy_res) */
} tENERGY_PULSE;

/*! Global energy structure definition */ 
typedef struct
{
    tENERGY_REG   cnt;    /*!< Energy counters structure (should be saved to NVMEM) */
    tENERGY_REG   rem;    /*!< Energy remainders structure (don't have to be saved to NVMEM) */  
    tENERGY_PULSE led;    /*!< Energy LED (pulse) registers structure */
} tENERGY;

/*! Metering library data structure definition for the 1-ph METER */
typedef struct
{
    tFFT_LENGTH   fft;       /*!< FFT length structure */
    unsigned long sensor;    /*!< Used current sensor - should be one of the following @ref sensor_type */
    tFFT_CALIB    calib;     /*!< Gain and offset calibration coefficients (U and I) */
    tFFT_MAINS    mains;     /*!< Main U and I input/output buffers (time/frequency domain) */   
    tFFT_AUX      aux;       /*!< Auxiliary U and I output buffers in the frequecy domain (Polar form) */   
    tFFT_INST     inst;      /*!< Instantaneous non-billing values: Urms, Irms, P, Q, S, THD */   
    tFFT_AVRG     avrg;      /*!< Averaged non-billing values: Urms, Irms, P, Q, S, THD */  
    unsigned long avrg_cnt;  /*!< Average counter (for averaging of non-billing values) */   
    long          *shift;    /*!< Phase shifts correction buffer - angle values [0.001°] for S/W phase 
                                  shift correction. Should be assigned to NULL if it is not used */
    tENERGY       energy;    /*!< Energy data structure */
} tMETERLIBFFT1PH_DATA;

/*! Metering library data structure definition for the 2-ph METER */
typedef struct
{
    tFFT_LENGTH   fft;       /*!< FFT length structure */
    unsigned long sensor;    /*!< Used current sensors - should be one of the following @ref sensor_type */
    tFFT_CALIB    calib1;    /*!< Gain and offset calibration coefficients (U1 and I1) */
    tFFT_MAINS    mains1;    /*!< Main U1 and I1 input/output buffers (time/frequency domain) */     
    tFFT_AUX      aux1;      /*!< Auxiliary U1 and I1 output buffers in the frequecy domain (Polar form) */   
    tFFT_INST     inst1;     /*!< instantaneous non-billing values: Urms1, Irms1, P1, Q1, S1, THD1 */   
    tFFT_AVRG     avrg1;     /*!< averaged non-billing values: Urms1, Irms1, P1, Q1, S1, THD1 */   
    unsigned long avrg_cnt1; /*!< Average counter (for averaging of phase1 non-billing values) */   
    long          *shift1;   /*!< Phase shifts correction buffer1 - angle values [0.001°] for S/W phase 
                                  shift correction. Should be assigned to NULL if it is not used */
    tFFT_CALIB    calib2;    /*!< Gain and offset calibration coefficients (U2 and I2) */
    tFFT_MAINS    mains2;    /*!< Main U2 and I2 input/output buffers (time/frequency domain) */   
    tFFT_AUX      aux2;      /*!< Auxiliary U2 and I2 output buffers in the frequecy domain (Polar form) */   
    tFFT_INST     inst2;     /*!< Instantaneous non-billing values: Urms2, Irms2, P2, Q2, S2, THD2 */   
    tFFT_AVRG     avrg2;     /*!< Averaged non-billing values: Urms2, Irms2, P2, Q2, S2, THD2 */   
    unsigned long avrg_cnt2; /*!< Average counter (for averaging of phase2 non-billing values) */   
    long          *shift2;   /*!< Phase shifts correction buffer2 - angle values [0.001°] for S/W phase 
                                  shift correction. Should be assigned to NULL if it is not used */
    tENERGY       energy;    /*!< Energy data structure (for both phases) */
} tMETERLIBFFT2PH_DATA;

/*! Metering library data structure definition for the 3-ph METER */
typedef struct
{
    tFFT_LENGTH   fft;       /*!< FFT length structure */
    unsigned long sensor;    /*!< Used current sensors - should be one of the following @ref sensor_type */
    tFFT_CALIB    calib1;    /*!< Gain and offset calibration coefficients (U1 and I1) */
    tFFT_MAINS    mains1;    /*!< Main U1 and I1 input/output buffers (time/frequency domain) */     
    tFFT_AUX      aux1;      /*!< Auxiliary U1 and I1 output buffers in the frequecy domain (Polar form) */   
    tFFT_INST     inst1;     /*!< instantaneous non-billing values: Urms1, Irms1, P1, Q1, S1, THD1 */   
    tFFT_AVRG     avrg1;     /*!< averaged non-billing values: Urms1, Irms1, P1, Q1, S1, THD1 */   
    unsigned long avrg_cnt1; /*!< Average counter (for averaging of phase1 non-billing values) */   
    long          *shift1;   /*!< Phase shifts correction buffer1 - angle values [0.001°] for S/W phase 
                                  shift correction. Should be assigned to NULL if it is not used */
    tFFT_CALIB    calib2;    /*!< Gain and offset calibration coefficients (U2 and I2) */
    tFFT_MAINS    mains2;    /*!< Main U2 and I2 input/output buffers (time/frequency domain) */   
    tFFT_AUX      aux2;      /*!< Auxiliary U2 and I2 output buffers in the frequecy domain (Polar form) */   
    tFFT_INST     inst2;     /*!< Instantaneous non-billing values: Urms2, Irms2, P2, Q2, S2, THD2 */   
    tFFT_AVRG     avrg2;     /*!< Averaged non-billing values: Urms2, Irms2, P2, Q2, S2, THD2 */   
    unsigned long avrg_cnt2; /*!< Average counter (for averaging of phase2 non-billing values) */   
    long          *shift2;   /*!< Phase shifts correction buffer2 - angle values [0.001°] for S/W phase 
                                  shift correction. Should be assigned to NULL if it is not used */
    tFFT_CALIB    calib3;    /*!< Gain and offset calibration coefficients (U3 and I3) */
    tFFT_MAINS    mains3;    /*!< Main U3 and I3 input/output buffers (time/frequency domain) */     
    tFFT_AUX      aux3;      /*!< Auxiliary U3 and I3 output buffers in the frequecy domain (Polar form) */   
    tFFT_INST     inst3;     /*!< instantaneous non-billing values: Urms3, Irms3, P3, Q3, S3, THD3 */   
    tFFT_AVRG     avrg3;     /*!< averaged non-billing values: Urms3, Irms3, P3, Q3, S3, THD3 */   
    unsigned long avrg_cnt3; /*!< Average counter (for averaging of phase3 non-billing values) */   
    long          *shift3;   /*!< Phase shifts correction buffer3 - angle values [0.001°] for S/W phase 
                                  shift correction. Should be assigned to NULL if it is not used */
    tENERGY       energy;    /*!< Energy data structure (for all phases) */
} tMETERLIBFFT3PH_DATA;

/***********************************************************************************************
* Number of FFT samples definitions used by METERLIBFFTxPH_InitParam() functions
*
*//*! @addtogroup fft_samples
* @{
************************************************************************************************/
#define SAMPLES8    (unsigned long)8    ///< 8 input samples, 4 output harmonics
#define SAMPLES16   (unsigned long)16   ///< 16 input samples, 8 output harmonics
#define SAMPLES32   (unsigned long)32   ///< 32 input samples, 16 output harmonics
#define SAMPLES64   (unsigned long)64   ///< 64 input samples, 32 output harmonics
#define SAMPLES128  (unsigned long)128  ///< 128 input samples, 64 output harmonics
#define SAMPLES256  (unsigned long)256  ///< 256 input samples, 128 output harmonics
#define SAMPLES512  (unsigned long)512  ///< 512 input samples, 256 output harmonics
/*! @} End of fft_samples */

/************************************************************************************************
* Energy resolution definitions used by METERLIBFFTxPH_InitParam() functions
*
*//*! @addtogroup energy_res
* @{
************************************************************************************************/
#define EN_RES1    (unsigned long)1    ///< Energy resolution is 1 Wh/VARh, supports @ref impulse_num <= 1000
#define EN_RES10   (unsigned long)10   ///< Energy resolution is 0.1 Wh/VARh, supports @ref impulse_num <= 10000
#define EN_RES100  (unsigned long)100  ///< Energy resolution is 0.01 Wh/VARh, supports @ref impulse_num <= 100000
/*! @} End of energy_res */

/************************************************************************************************
* Sensor type definitions used by METERLIBFFTxPH_InitParam() functions
*
*//*! @addtogroup sensor_type
* @{
************************************************************************************************/
#define SENS_PROP   (unsigned long)1   ///< Proportional type of the current sensor (shunt, CT)
#define SENS_DERIV  (unsigned long)2   ///< Derivative type of the current sensor (RC)
/*! @} End of sensor_type */

/************************************************************************************************
* Interpolation order definitions used by METERLIBFFTxPH_Interpolation() functions
*
*//*! @addtogroup interp_order
* @{
************************************************************************************************/
#define ORD1   (unsigned long)1   ///< 1-st order (linear) interpolation
#define ORD2   (unsigned long)2   ///< 2-nd order (quadratic) interpolation
#define ORD3   (unsigned long)3   ///< 3-rd order (cubic) interpolation
/*! @} End of interp_order */

/************************************************************************************************
* Impulse number settings definitions used by METERLIBFFTxPH_InitParam() functions
*
*//*! @addtogroup impulse_num
* @{
************************************************************************************************/
#define IMP200    (unsigned long)200    ///< 200 imp/kWh or 200 imp/kVARh
#define IMP250    (unsigned long)250    ///< 250 imp/kWh or 250 imp/kVARh
#define IMP500    (unsigned long)500    ///< 500 imp/kWh or 500 imp/kVARh
#define IMP1000   (unsigned long)1000   ///< 1000 imp/kWh or 1000 imp/kVARh
#define IMP1250   (unsigned long)1250   ///< 1250 imp/kWh or 1250 imp/kVARh
#define IMP2000   (unsigned long)2000   ///< 2000 imp/kWh or 2000 imp/kVARh
#define IMP2500   (unsigned long)2500   ///< 2500 imp/kWh or 2500 imp/kVARh
#define IMP5000   (unsigned long)5000   ///< 5000 imp/kWh or 5000 imp/kVARh
#define IMP10000  (unsigned long)10000  ///< 10000 imp/kWh or 10000 imp/kVARh
#define IMP12500  (unsigned long)12500  ///< 12500 imp/kWh or 12500 imp/kVARh
#define IMP20000  (unsigned long)20000  ///< 20000 imp/kWh or 20000 imp/kVARh
#define IMP25000  (unsigned long)25000  ///< 25000 imp/kWh or 25000 imp/kVARh
#define IMP50000  (unsigned long)50000  ///< 50000 imp/kWh or 50000 imp/kVARh
#define IMP100000 (unsigned long)100000 ///< 100000 imp/kWh or 100000 imp/kVARh
/*! @} End of impulse_num */

/************************************************************************************************
* FFT output function state definitions 
*
*//*! @addtogroup fcn_state
* @{
************************************************************************************************/
#define FFT_OK      (long)0 ///< All input parameters are OK, function output is valid
#define FFT_ERROR   (long)1 ///< Some of input parameters is not OK, function output is not valid
/*! @} End of fcn_state */

/************************************************************************************************
* 3-PH sense of rotation definitions 
*
*//*! @addtogroup sense_rotation
* @{
************************************************************************************************/
#define ROT_FORWARD  (long)1  ///< clockwise or forward sense of rotation of the 3ph mains, that is 1-2-3, 2-3-1, or 3-1-2.
#define ROT_REVERSE  (long)-1 ///< counter-clockwise or reverse sense of rotation of the 3ph mains, that is 2-1-3, 1-3-2, or 3-2-1.
#define ROT_UNKNOWN  (long)0  ///< loss of phase, sense of rotation cannot be recognized.
/*! @} End of sense_rotation */

/********************************************************************************************//*!
*
* @brief  Negative Number Evaluation
*
* @param  number    Signed instantaneous power (active or reactive)
*         
* @return If the number is negative, it returns its absolute value, otherwise returns zero.
*
* @remarks This macro is used for export active/reactive power increment evaluation inside the 
*          METERLIBFFTxPH_CalcWattHours() and METERLIBFFTxPH_CalcVarHours() functions.
*
************************************************************************************************/
#define NUM_NEG(number) ((number) < 0 ? (unsigned long)(-number) : 0)      

/********************************************************************************************//*!
*
* @brief  Positive Number Evaluation
*
* @param  number    Signed instantaneous power (active or reactive)
*         
* @return If the number is positive, it returns the same value, otherwise returns zero.
*
* @remarks This macro is used for import active/reactive power increment evaluation inside the 
*          METERLIBFFTxPH_CalcWattHours() and METERLIBFFTxPH_CalcVarHours() functions.
*
************************************************************************************************/
#define NUM_POS(number) ((number) > 0 ? (unsigned long)(number) : 0)       

/********************************************************************************************//*!
*
* @brief  Absolute Number Evaluation
*
* @param  number    Signed instantaneous power (active or reactive)
*         
* @return Returns the absolute value of the input number.
*
* @remarks This macro is used for total active/reactive power increment evaluation inside the 
*          METERLIBFFTxPH_CalcWattHours() and METERLIBFFTxPH_CalcVarHours() functions.
*
************************************************************************************************/
#define NUM_ABS(number) ((number) > 0 ? (unsigned long)(number) : (unsigned long)(-number)) 


/********************************************************************************************//*!
*
* @brief   Common Set/Clear Energy Counters Function
*
* @param   p          Pointer to the metering library data structure (1-ph, 2-ph or 3-ph)
* @param   whi        Import active energy (unsigned value)
* @param   whe        Export active energy (unsigned value)
* @param   varhi      Import reactive energy (unsigned value)
* @param   varhe      Export reactive energy (unsigned value)
*         
* @remarks This function sets all energy counters and clears all remainders. Alternatively, it 
*          may be used for clearing all energy counters too. The function should be used in the 
*          initialization section only. The energy resolution is set by en_res parameter in some
*          parameter initialization functions (1-ph, 2-ph or 3-ph initialization). 
*
************************************************************************************************/
#define METERLIBFFT_SetEnergy(p,whi,whe,varhi,varhe)                                        \
{                                                                                           \
    /* setting/clearing all energy counters */                                              \
    p.energy.cnt.wh_i       = whi;                                                          \
    p.energy.cnt.wh_e       = whe;                                                          \
    p.energy.cnt.wh_t       = whi + whe;     /* total active energy for LED flashing */     \
    p.energy.cnt.varh_i     = varhi;                                                        \
    p.energy.cnt.varh_e     = varhe;                                                        \
    p.energy.cnt.varh_t     = varhi + varhe; /* total reactive energy for LED flashing */   \
    /* merging both LED flash counters with both total energy counters */                   \
    p.energy.led.wh_flash      = p.energy.cnt.wh_t;                                         \
    p.energy.led.varh_flash    = p.energy.cnt.varh_t;                                       \
    /* clearing all energy remainders */                                                    \
    p.energy.rem.wh_t   = 0l;                                                               \
    p.energy.rem.wh_i   = 0l;                                                               \
    p.energy.rem.wh_e   = 0l;                                                               \
    p.energy.rem.varh_t = 0l;                                                               \
    p.energy.rem.varh_i = 0l;                                                               \
    p.energy.rem.varh_e = 0l;                                                               \
}

/************************************************************************************************
 * public function prototypes                                                                   *
************************************************************************************************/

/************************************************************************************************
 ************************************************************************************************
 *                                        1-PH METER API
 ************************************************************************************************
************************************************************************************************/

/********************************************************************************************//*!
*
* @brief    1-Phase Parameters Initialization Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT1PH_DATA)
* @param   samples     Select @ref fft_samples
* @param   sensor      Select @ref sensor_type
* @param   kwh_cnt     Select @ref impulse_num
* @param   kvarh_cnt   Select @ref impulse_num
* @param   en_res      Select @ref energy_res
*         
* @return  Returns one of the following error code @ref fcn_state
*
* @remarks This mandatory function must be called in the initialization section anyway or 
*          after changing some parameters during the program execution.
*
************************************************************************************************/
extern long METERLIBFFT1PH_InitParam(tMETERLIBFFT1PH_DATA *p, unsigned long samples, unsigned long sensor, unsigned long kwh_cnt, unsigned long kvarh_cnt, unsigned long en_res);

/********************************************************************************************//*!
*
* @brief    1-Phase Main Buffers Initialization Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT1PH_DATA)
* @param   u_re        Pointer to the voltage input time domain buffer united with the frequency output 
*                      buffer (real part) in Q0.23 data format, buffer length matches with the number of input (AFE) samples
* @param   i_re        Pointer to the current input time domain buffer united with the frequency output 
*                      buffer (real part) in Q0.23 data format, buffer length matches with the number of input (AFE) samples
* @param   u_im        Pointer to the voltage frequency output buffer (imaginary part) in Q0.23 data format, buffer length 
*                      matches with the number of @ref fft_samples set by @ref METERLIBFFT1PH_InitParam function
* @param   i_im        Pointer to the current frequency output buffer (imaginary part) in Q0.23 data format, buffer length 
*                      matches with the number of @ref fft_samples set by @ref METERLIBFFT1PH_InitParam function
* @param   shift       Pointer to the input U-I phase shift buffer [0.001°], the maximal buffer length 
*                      matches with number of FFT harmonics (half of @ref fft_samples). If the S/W    
*                      phase shift correction is not used, this pointer must be assigned to NULL. 
*         
* @remarks This mandatory function must be called in the initialization section only. This
*          function initializes input time domain buffers, output frequency domain buffers, and
*          U-I phase shifts correction buffer. The first buffers position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT1PH_InitMainBuff(tMETERLIBFFT1PH_DATA *p, frac24 *u_re, frac24 *i_re, frac24 *u_im, frac24 *i_im, long *shift);

/********************************************************************************************//*!
*
* @brief    1-Phase Auxiliary Buffers Initialization Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT1PH_DATA)
* @param   mag_u       Pointer to the harmonic magnitudes voltage buffer in Q0.23 data format
* @param   mag_i       Pointer to the harmonic magnitudes current buffer in Q0.23 data format
* @param   ph_u        Pointer to the harmonic phase shift voltage buffer in 0.001°, e.g. 45000=45.000°
* @param   ph_i        Pointer to the harmonic phase shift current buffer in 0.001°, e.g. 45000=45.000°
*         
* @remarks This function should (may) be called in the initialization section. It is used  
*          for auxiliary buffers initialization only, i.e. when auxiliary computing (magnitudes, 
*          phases) is required. This calculation is done by @ref METERLIBFFT1PH_GetMagnitudes
*          and/or @ref METERLIBFFT1PH_GetPhases functions. The first buffers position matches 
*          with zero harmonic, etc. When auxiliary computing is not required, this function
*          doesn't have to be used. The length of all buffers is optional, but their maximal length 
*          cannot exceed the number of FFT harmonics (half of @ref fft_samples).
*
************************************************************************************************/
extern void METERLIBFFT1PH_InitAuxBuff(tMETERLIBFFT1PH_DATA *p, frac24 *mag_u, frac24 *mag_i, long *ph_u, long *ph_i);

/********************************************************************************************//*!
*
* @brief    1-Phase Set Calibration Coefficients Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT1PH_DATA)
* @param   u_max       Maximum peak phase voltage [V] valid for the AFE full scale range (23-bit)
* @param   i_max       Maximum peak phase current [A] valid for the AFE full scale range (23-bit)
* @param   i_offs      Pointer to the current offset correction value (Q0.23 data format). If the     
*                      offset correction is not used, this pointer should be assigned to NULL 
*                      (non-true Irms computing in this case). 
* @param   p_offs      Active power offset correction value [W]
* @param   q_offs      Reactive power offset correction value [VAr]
*         
* @return  Returns one of the following error code @ref fcn_state. When FFT_ERROR, one of the 
*          calibration coefficients is to big - overflow may occur later. The right coeff. values 
*          should be in this range: (u_max*i_max)<(2^31/10000).
*
* @remarks This function sets all calibration coefficients. It must be called in the initialization 
*          section anyway. It may be called during the AFE calibration processing too.
*
* @note Voltage offset is ignored. This simplification can be used due to zero DC offset in the mains.
************************************************************************************************/
extern long METERLIBFFT1PH_SetCalibCoeff(tMETERLIBFFT1PH_DATA *p, double u_max, double i_max, frac24 *i_offs, double p_offs, double q_offs);

/********************************************************************************************//*!
*
* @brief    1-Phase Interpolation Function.
*
* @param   p              Pointer to the metering library data structure (tMETERLIBFFT1PH_DATA)
* @param   u_ord          Voltage interpolation order - select @ref interp_order
* @param   i_ord          Current interpolation order - select @ref interp_order
* @param   samples_inp    Number of input samples (> or < than @ref fft_samples) 
*
* @return  Returns one of the following error code @ref fcn_state for undersampling use case. 
*          When FFT_ERROR, undersampling is required for big FFT samples value. 
*          When FFT_OK, undersampling ratio is good.
*         
* @remarks This function must be called before the main (FFT) calculation processing, when samples_inp 
*          doesn't match with the @ref fft_samples initialized by @ref METERLIBFFT1PH_InitParam 
*          function (asynchronous mode). When samples_inp matches with the @ref fft_samples, no 
*          interpolation is needed (synchronous mode). The mandatory parameter initialization 
*          must be done before calling this function. When u_ord or i_ord doesn't match with 
*          @ref interp_order, the function does nothing. This function interpolates raw ADC data 
*          located in both time domain buffers (see u_re and i_re buffer pointers initialized by 
*          @ref METERLIBFFT1PH_InitMainBuff function).
*
************************************************************************************************/
extern long METERLIBFFT1PH_Interpolation(tMETERLIBFFT1PH_DATA *p, unsigned long u_ord, unsigned long i_ord, unsigned long samples_inp);

/********************************************************************************************//*!
*
* @brief    1-Phase FFT Calculation and Signal Conditioning Processing Function.
*
* @param   p     Pointer to the metering library data structure (tMETERLIBFFT1PH_DATA)
*
* @remarks The mandatory parameter initialization must be done before calling this function.
*          The interpolation processing must (may) be done before calling this function. 
*          The energy calculation processing should be done after calling this function.
*
* @note  It performs not only the main FFT calculation, but the whole signal conditioning
*        processing with scaling and averaging of all non-billing values with their saving to the
*        internal structure. Both input time domain buffers addressed by u_re and i_re pointers 
*        will be rewrited by the FFT real part values, while imaginary parts of the FFT result 
*        are saved to separate buffers set by u_im and i_im pointers. Pointers to all these 
*        buffers must be initialized by @ref METERLIBFFT1PH_InitMainBuff function. The first FFT 
*        buffers position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT1PH_CalcMain(tMETERLIBFFT1PH_DATA *p);

/********************************************************************************************//*!
*
* @brief    1-Phase Harmonic Magnitudes Calculation Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT1PH_DATA)
* @param   magn_fft    Number of required magnitudes in the range <1...half of the input samples>
*
* @remarks The mandatory and auxiliary parameter initialization must be done before calling this
*          function. This function may be called after the main (FFT) calculation processing.
*
* @note    FFT U and I magnitudes are available at two buffers addressed by mag_u and 
*          mag_i pointers (initialized by @ref METERLIBFFT1PH_InitAuxBuff function). The first 
*          buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT1PH_GetMagnitudes(tMETERLIBFFT1PH_DATA *p, unsigned long magn_fft);

/********************************************************************************************//*!
*
* @brief    1-Phase Harmonic Phase Shifts Calculation Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT1PH_DATA)
* @param   ph_fft      Number of required phases in the range <1...half of the input samples>
*
* @remarks The mandatory and auxiliary parameter initialization must be done before calling this
*          function. This function may be called after the main (FFT) calculation processing.
*
* @note    Phase shift of each individual harmonics are available at two buffers addressed
*          by ph_u and ph_i pointers (initialized by @ref METERLIBFFT1PH_InitAuxBuff function). 
*          The first buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT1PH_GetPhases(tMETERLIBFFT1PH_DATA *p, unsigned long ph_fft);

/********************************************************************************************//*!
*
* @brief    1-Phase Active Energy Calculation Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT1PH_DATA), 
*                      where instantaneous active power is saved. 
* @param   wh_i        Pointer to the LCD counter representing import active energy 
*                      with resolution given by @ref energy_res
* @param   wh_e        Pointer to the LCD counter representing export active energy 
*                      with resolution given by @ref energy_res
* @param   frequency   Line frequency [mHz], e.g. 50000=50.000Hz
*         
* @return  When positive, the function returns active energy LED flashing frequency [mHz] for the 
*          current line period (only one LED flashing per one period). This method for low-jitter 
*          pulse output generation using software and timer is being patented. 
*          When negative, no output pulse generation is needed in the current period.
*
* @remarks The mandatory parameter initialization must be done before calling this function.
*          This function should be called after the main (FFT) calculation processing. The output
*          energy resolution depends on en_res parameter seetings by the @ref METERLIBFFT1PH_InitParam 
*          function.
*
************************************************************************************************/
extern long METERLIBFFT1PH_CalcWattHours(tMETERLIBFFT1PH_DATA *p, unsigned long *wh_i, unsigned long *wh_e, unsigned long frequency);

/********************************************************************************************//*!
*
* @brief    1-Phase Reactive Energy Calculation Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT1PH_DATA), 
*                      where instantaneous reactive power is saved. 
* @param   varh_i      Pointer to the LCD counter representing import reactive energy 
*                      with resolution given by @ref energy_res
* @param   varh_e      Pointer to the LCD counter representing export reactive energy 
*                      with resolution given by @ref energy_res
* @param   frequency   Line frequency [mHz], e.g. 50000=50.000Hz
*         
* @return  When positive, the function returns reactive energy LED flashing frequency [mHz] for the 
*          current line period (only one LED flashing per one period). This method for low-jitter 
*          pulse output generation using software and timer is being patented. 
*          When negative, no output pulse generation is needed in the current period.
*
* @remarks The mandatory parameter initialization must be done before calling this function.
*          This function should be called after the main (FFT) calculation processing. The output
*          energy resolution depends on en_res parameter seetings by the @ref METERLIBFFT1PH_InitParam 
*          function.
*
************************************************************************************************/
extern long METERLIBFFT1PH_CalcVarHours(tMETERLIBFFT1PH_DATA *p, unsigned long *varh_i, unsigned long *varh_e, unsigned long frequency);

/********************************************************************************************//*!
*
* @brief    1-Phase Non-Billing (U,I,P,Q,S,pf,THD) Averaged Variables Reading Function.
*
* @param   p         Pointer to the metering library data structure (tMETERLIBFFT1PH_DATA), where 
*                    all averaged non-billing metering values are saved in integer forms.
* @param   urms      Pointer to the averaged RMS value of the line voltage [V]
* @param   irms      Pointer to the averaged RMS value of the line current [A]
* @param   w         Pointer to the averaged active power [W]
* @param   var       Pointer to the averaged reactive power [VAR]
* @param   va        Pointer to the averaged apparent power [VA]
* @param   pf        Pointer to the averaged power factor [-]
* @param   thdu      Pointer to the averaged THDu [%]  
* @param   thdi      Pointer to the averaged THDi [%]  
*         
* @remarks Calling frequency of this function should be in the range <0.004Hz...line frequency>.
*          In case of lower calling frequency, internal counters may overflow. In this case, the 
*          first dummy reading is neccessary for clearing all internal counters. In case of
*          higher calling frequency, all output values will be zero. All output values are scaled
*          to engineering units in double precision form.
*
************************************************************************************************/
extern void METERLIBFFT1PH_GetAvrgValues(tMETERLIBFFT1PH_DATA *p, double *urms, double *irms, double *w, double *var, double *va, double *pf, double *thdu, double *thdi);

/********************************************************************************************//*!
*
* @brief    1-Phase Non-Billing (U,I,P,Q,S,pf,THD) Instantaneous Variables Reading Function.
*
* @param   p         Pointer to the metering library data structure (tMETERLIBFFT1PH_DATA), where 
*                    all instantaneous non-billing metering values are saved in integer forms.
* @param   urms      Pointer to the instantaneous RMS value of the line voltage [V]
* @param   irms      Pointer to the instantaneous RMS value of the line current [A]
* @param   w         Pointer to the instantaneous active power [W]
* @param   var       Pointer to the instantaneous reactive power [VAR]
* @param   va        Pointer to the instantaneous apparent power [VA]
* @param   pf        Pointer to the instantaneous power factor [-]
* @param   thdu      Pointer to the instantaneous THDu [%]  
* @param   thdi      Pointer to the instantaneous THDi [%]  
*         
* @remarks This function could be called anytime, better after the main (FFT) calculation processing.
*          All output values are scaled to engineering units in double precision form.
*
************************************************************************************************/
extern void METERLIBFFT1PH_GetInstValues(tMETERLIBFFT1PH_DATA *p, double *urms, double *irms, double *w, double *var, double *va, double *pf, double *thdu, double *thdi);

/************************************************************************************************
 ************************************************************************************************
 *                                        2-PH METER API
 ************************************************************************************************
************************************************************************************************/

/********************************************************************************************//*!
*
* @brief    2-Phase Parameters Initialization Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA)
* @param   samples     Select @ref fft_samples
* @param   sensor      Select @ref sensor_type
* @param   kwh_cnt     Select @ref impulse_num
* @param   kvarh_cnt   Select @ref impulse_num
* @param   en_res      Select @ref energy_res
*         
* @return  Returns one of the following error code @ref fcn_state
*
* @remarks This mandatory function must be called in the initialization section anyway or 
*          after changing some parameters during the program execution.
*
************************************************************************************************/
extern long METERLIBFFT2PH_InitParam(tMETERLIBFFT2PH_DATA *p, unsigned long samples, unsigned long sensor, unsigned long kwh_cnt, unsigned long kvarh_cnt, unsigned long en_res);

/********************************************************************************************//*!
*
* @brief    2-Phase Main Buffers Initialization Function for the Phase 1.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA)
* @param   u_re        Pointer to the voltage input time domain buffer1 united with the frequency output 
*                      buffer1 (real part) in Q0.23 data format, buffer length matches with the number of input (AFE) samples
* @param   i_re        Pointer to the current input time domain buffer1 united with the frequency output 
*                      buffer1 (real part) in Q0.23 data format, buffer length matches with the number of input (AFE) samples
* @param   u_im        Pointer to the voltage frequency output buffer1 (imaginary part) in Q0.23 data format, buffer length 
*                      matches with the number of @ref fft_samples set by @ref METERLIBFFT2PH_InitParam function
* @param   i_im        Pointer to the current frequency output buffer1 (imaginary part) in Q0.23 data format, buffer length 
*                      matches with the number of @ref fft_samples set by @ref METERLIBFFT2PH_InitParam function
* @param   shift       Pointer to the input U-I phase shift buffer1 [0.001°], the maximal buffer length 
*                      matches with number of FFT harmonics (half of @ref fft_samples). If the S/W    
*                      phase shift correction is not used, this pointer must be assigned to NULL. 
*         
* @remarks This mandatory function must be called in the initialization section only. This
*          function initializes input time domain buffers, output frequency domain buffers, and
*          U-I phase shifts correction buffer. The first buffers position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT2PH_InitMainBuffPh1(tMETERLIBFFT2PH_DATA *p, frac24 *u_re, frac24 *i_re, frac24 *u_im, frac24 *i_im, long *shift);

/********************************************************************************************//*!
*
* @brief    2-Phase Main Buffers Initialization Function for the Phase 2.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA)
* @param   u_re        Pointer to the voltage input time domain buffer2 united with the frequency output 
*                      buffer2 (real part) in Q0.23 data format, buffer length matches with the number of input (AFE) samples
* @param   i_re        Pointer to the current input time domain buffer2 united with the frequency output 
*                      buffer2 (real part) in Q0.23 data format, buffer length matches with the number of input (AFE) samples
* @param   u_im        Pointer to the voltage frequency output buffer2 (imaginary part) in Q0.23 data format, buffer length 
*                      matches with the number of @ref fft_samples set by @ref METERLIBFFT2PH_InitParam function
* @param   i_im        Pointer to the current frequency output buffer2 (imaginary part) in Q0.23 data format, buffer length 
*                      matches with the number of @ref fft_samples set by @ref METERLIBFFT2PH_InitParam function
* @param   shift       Pointer to the input U-I phase shift buffer2 [0.001°], the maximal buffer length 
*                      matches with number of FFT harmonics (half of @ref fft_samples). If the S/W    
*                      phase shift correction is not used, this pointer must be assigned to NULL. 
*         
* @remarks This mandatory function must be called in the initialization section only. This
*          function initializes input time domain buffers, output frequency domain buffers, and
*          U-I phase shifts correction buffer. The first buffers position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT2PH_InitMainBuffPh2(tMETERLIBFFT2PH_DATA *p, frac24 *u_re, frac24 *i_re, frac24 *u_im, frac24 *i_im, long *shift);

/********************************************************************************************//*!
*
* @brief    2-Phase Auxiliary Buffers Initialization Function for the Phase 1.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA)
* @param   mag_u       Pointer to the harmonic magnitudes voltage buffer in Q0.23 data format
* @param   mag_i       Pointer to the harmonic magnitudes current buffer in Q0.23 data format
* @param   ph_u        Pointer to the harmonic phase shift voltage buffer in 0.001°, e.g. 45000=45.000°
* @param   ph_i        Pointer to the harmonic phase shift current buffer in 0.001°, e.g. 45000=45.000°
*         
* @remarks This function should (may) be called in the initialization section. It is used  
*          for auxiliary buffers initialization only, i.e. when auxiliary computing (magnitudes, 
*          phases) is required. This calculation is done by @ref METERLIBFFT2PH_GetMagnitudesPh1
*          and/or @ref METERLIBFFT2PH_GetPhasesPh1 functions. The first buffers position matches 
*          with zero harmonic, etc. When auxiliary computing is not required, this function
*          doesn't have to be used. The length of all buffers is optional, but their maximal length 
*          cannot exceed the number of FFT harmonics (half of @ref fft_samples).
*
************************************************************************************************/
extern void METERLIBFFT2PH_InitAuxBuffPh1(tMETERLIBFFT2PH_DATA *p, frac24 *mag_u, frac24 *mag_i, long *ph_u, long *ph_i);

/********************************************************************************************//*!
*
* @brief    2-Phase Auxiliary Buffers Initialization Function for the Phase 2.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA)
* @param   mag_u       Pointer to the harmonic magnitudes voltage buffer in Q0.23 data format
* @param   mag_i       Pointer to the harmonic magnitudes current buffer in Q0.23 data format
* @param   ph_u        Pointer to the harmonic phase shift voltage buffer in 0.001°, e.g. 45000=45.000°
* @param   ph_i        Pointer to the harmonic phase shift current buffer in 0.001°, e.g. 45000=45.000°
*         
* @remarks This function should (may) be called in the initialization section. It is used  
*          for auxiliary buffers initialization only, i.e. when auxiliary computing (magnitudes, 
*          phases) is required. This calculation is done by @ref METERLIBFFT2PH_GetMagnitudesPh2
*          and/or @ref METERLIBFFT2PH_GetPhasesPh2 functions. The first buffers position matches 
*          with zero harmonic, etc. When auxiliary computing is not required, this function
*          doesn't have to be used. The length of all buffers is optional, but their maximal length 
*          cannot exceed the number of FFT harmonics (half of @ref fft_samples).
*
************************************************************************************************/
extern void METERLIBFFT2PH_InitAuxBuffPh2(tMETERLIBFFT2PH_DATA *p, frac24 *mag_u, frac24 *mag_i, long *ph_u, long *ph_i);

/********************************************************************************************//*!
*
* @brief    2-Phase Set Calibration Coefficients Function for the Phase 1.
*
* @param   p          Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA)
* @param   u_max      Maximum peak phase1 voltage [V] valid for the AFE full scale range (23-bit)
* @param   i_max      Maximum peak phase1 current [A] valid for the AFE full scale range (23-bit)
* @param   i_offs     Pointer to the current1 offset correction value (Q0.23 data format). If the     
*                     offset correction is not used, this pointer should be assigned to NULL 
*                     (non-true Irms computing in this case). 
* @param   p_offs     Active power 1 offset correction value [W]
* @param   q_offs     Reactive power 1 offset correction value [VAr]
*         
* @return  Returns one of the following error code @ref fcn_state. When FFT_ERROR, one of the 
*          calibration coefficients is to big - overflow may occur later. The right coeff. values 
*          should be in this range: (u_max*i_max)<(2^31/10000).
*
* @remarks This function sets all calibration coefficients for the phase 1. It must be called in 
*          the initialization section anyway. It may be called during the AFE calibration processing too.
*
* @note Voltage offset is ignored. This simplification can be used due to zero DC offset in the mains.
*
************************************************************************************************/
extern long METERLIBFFT2PH_SetCalibCoeffPh1(tMETERLIBFFT2PH_DATA *p, double u_max, double i_max, frac24 *i_offs, double p_offs, double q_offs);

/********************************************************************************************//*!
*
* @brief    2-Phase Set Calibration Coefficients Function for the Phase 2.
*
* @param   p          Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA)
* @param   u_max      Maximum peak phase2 voltage [V] valid for the AFE full scale range (23-bit)
* @param   i_max      Maximum peak phase2 current [A] valid for the AFE full scale range (23-bit)
* @param   i_offs     Pointer to the current2 offset correction value (Q0.23 data format). If the     
*                     offset correction is not used, this pointer should be assigned to NULL 
*                     (non-true Irms computing in this case). 
* @param   p_offs     Active power 2 offset correction value [W]
* @param   q_offs     Reactive power 2 offset correction value [VAr]
*         
* @return  Returns one of the following error code @ref fcn_state. When FFT_ERROR, one of the 
*          calibration coefficients is to big - overflow may occur later. The right coeff. values 
*          should be in this range: (u_max*i_max)<(2^31/10000).
*
* @remarks This function sets all calibration coefficients for the phase 2. It must be called in 
*          the initialization section anyway. It may be called during the AFE calibration processing too.
*
* @note Voltage offset is ignored. This simplification can be used due to zero DC offset in the mains.
*
************************************************************************************************/
extern long METERLIBFFT2PH_SetCalibCoeffPh2(tMETERLIBFFT2PH_DATA *p, double u_max, double i_max, frac24 *i_offs, double p_offs, double q_offs);

/********************************************************************************************//*!
*
* @brief    2-Phase Interpolation Function.
*
* @param   p              Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA)
* @param   u_ord          Voltage (both phases) interpolation order - select @ref interp_order
* @param   i_ord          Current (both phases) interpolation order - select @ref interp_order
* @param   samples_inp    Number of input samples (> or < than @ref fft_samples) 
*
* @return  Returns one of the following error code @ref fcn_state for undersampling use case. 
*          When FFT_ERROR, undersampling is required for big FFT samples value. 
*          When FFT_OK, undersampling ratio is good.
*         
* @remarks This function must be called before the main (FFT) calculation processing, when samples_inp 
*          doesn't match with the @ref fft_samples initialized by @ref METERLIBFFT2PH_InitParam 
*          function (asynchronous mode). When samples_inp matches with the @ref fft_samples, no 
*          interpolation is needed (synchronous mode). The mandatory parameter initialization 
*          must be done before calling this function. When u_ord or i_ord doesn't match with 
*          @ref interp_order, the function does nothing. This function interpolates raw ADC data 
*          located in all time domain buffers (see u_re and i_re buffer pointers initialized by 
*          @ref METERLIBFFT2PH_InitMainBuffPh1 and  @ref METERLIBFFT2PH_InitMainBuffPh2 functions).
*
************************************************************************************************/
extern long METERLIBFFT2PH_Interpolation(tMETERLIBFFT2PH_DATA *p, unsigned long u_ord, unsigned long i_ord, unsigned long samples_inp);

/********************************************************************************************//*!
*
* @brief    2-Phase FFT Calculation and Signal Conditioning Processing Function.
*
* @param   p     Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA)
*
* @remarks The mandatory parameter initialization must be done before calling this function.
*          The interpolation processing must (may) be done before calling this function. 
*          The energy calculation processing should be done after calling this function.
*
* @note  It performs not only the main FFT calculation, but the whole signal conditioning
*        processing with scaling and averaging of all non-billing values with their saving to the
*        internal structure. All input time domain buffers addressed by u_re, i_re pointers 
*        will be rewrited by the FFT real part values, while imaginary parts of the 
*        FFT result are saved to separate buffers set by u_im, i_im pointers. 
*        Pointers to all these buffers must be initialized by @ref METERLIBFFT2PH_InitMainBuffPh1 
*        and @ref METERLIBFFT2PH_InitMainBuffPh2 functions. The first FFT buffers position matches 
*        with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT2PH_CalcMain(tMETERLIBFFT2PH_DATA *p);

/********************************************************************************************//*!
*
* @brief    2-Phase Harmonic Magnitudes Calculation Function for the Phase 1.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA)
* @param   magn_fft    Number of required magnitudes in the range <1...half of the input samples>
*
* @remarks The mandatory and auxiliary parameter initialization must be done before calling this
*          function. This function may be called after the main (FFT) calculation processing.
*
* @note    FFT U and I magnitudes are available at two buffers addressed by mag_u and 
*          mag_i pointers (initialized by @ref METERLIBFFT2PH_InitAuxBuffPh1 function). The first 
*          buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT2PH_GetMagnitudesPh1(tMETERLIBFFT2PH_DATA *p, unsigned long magn_fft);

/********************************************************************************************//*!
*
* @brief    2-Phase Harmonic Magnitudes Calculation Function for the Phase 2.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA)
* @param   magn_fft    Number of required magnitudes in the range <1...half of the input samples>
*
* @remarks The mandatory and auxiliary parameter initialization must be done before calling this
*          function. This function may be called after the main (FFT) calculation processing.
*
* @note    FFT U and I magnitudes are available at two buffers addressed by mag_u and 
*          mag_i pointers (initialized by @ref METERLIBFFT2PH_InitAuxBuffPh2 function). The first 
*          buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT2PH_GetMagnitudesPh2(tMETERLIBFFT2PH_DATA *p, unsigned long magn_fft);

/********************************************************************************************//*!
*
* @brief    2-Phase Harmonic Phase Shifts Calculation Function for the Phase 1.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA)
* @param   ph_fft      Number of required phases in the range <1...half of the input samples>
*
* @remarks The mandatory and auxiliary parameter initialization must be done before calling this
*          function. This function may be called after the main (FFT) calculation processing.
*
* @note    Phase shift of each individual harmonics are available at two buffers addressed
*          by ph_u and ph_i pointers (initialized by @ref METERLIBFFT2PH_InitAuxBuffPh1 function). 
*          The first buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT2PH_GetPhasesPh1(tMETERLIBFFT2PH_DATA *p, unsigned long ph_fft);

/********************************************************************************************//*!
*
* @brief    2-Phase Harmonic Phase Shifts Calculation Function for the Phase 2.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA)
* @param   ph_fft      Number of required phases in the range <1...half of the input samples>
*
* @remarks The mandatory and auxiliary parameter initialization must be done before calling this
*          function. This function may be called after the main (FFT) calculation processing.
*
* @note    Phase shift of each individual harmonics are available at two buffers addressed
*          by ph_u and ph_i pointers (initialized by @ref METERLIBFFT2PH_InitAuxBuffPh2 function). 
*          The first buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT2PH_GetPhasesPh2(tMETERLIBFFT2PH_DATA *p, unsigned long ph_fft);

/********************************************************************************************//*!
*
* @brief    2-Phase Active Energy Calculation Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA), 
*                      where instantaneous active power is saved. 
* @param   wh_i        Pointer to the LCD counter representing import active energy 
*                      with resolution given by @ref energy_res
* @param   wh_e        Pointer to the LCD counter representing export active energy 
*                      with resolution given by @ref energy_res
* @param   frequency   Line frequency [mHz], e.g. 50000=50.000Hz
*         
* @return  When positive, the function returns active energy LED flashing frequency [mHz] for the 
*          current line period (only one LED flashing per one period). This method for low-jitter 
*          pulse output generation using software and timer is being patented. 
*          When negative, no output pulse generation is needed in the current period.
*
* @remarks The mandatory parameter initialization must be done before calling this function.
*          This function should be called after the main (FFT) calculation processing. The output
*          energy resolution depends on en_res parameter seetings by the @ref METERLIBFFT2PH_InitParam 
*          function.
*
************************************************************************************************/
extern long METERLIBFFT2PH_CalcWattHours(tMETERLIBFFT2PH_DATA *p, unsigned long *wh_i, unsigned long *wh_e, unsigned long frequency);

/********************************************************************************************//*!
*
* @brief    2-Phase Reactive Energy Calculation Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA), 
*                      where instantaneous reactive power is saved. 
* @param   varh_i      Pointer to the LCD counter representing import reactive energy 
*                      with resolution given by @ref energy_res
* @param   varh_e      Pointer to the LCD counter representing export reactive energy 
*                      with resolution given by @ref energy_res
* @param   frequency   Line frequency [mHz], e.g. 50000=50.000Hz
*         
* @return  When positive, the function returns reactive energy LED flashing frequency [mHz] for the 
*          current line period (only one LED flashing per one period). This method for low-jitter 
*          pulse output generation using software and timer is being patented. 
*          When negative, no output pulse generation is needed in the current period.
*
* @remarks The mandatory parameter initialization must be done before calling this function.
*          This function should be called after the main (FFT) calculation processing. The output
*          energy resolution depends on en_res parameter seetings by the @ref METERLIBFFT2PH_InitParam 
*          function.
*
************************************************************************************************/
extern long METERLIBFFT2PH_CalcVarHours(tMETERLIBFFT2PH_DATA *p, unsigned long *varh_i, unsigned long *varh_e, unsigned long frequency);

/********************************************************************************************//*!
*
* @brief    2-Phase Non-Billing (U,I,P,Q,S,pf,THD) Averaged Variables Phase 1 Reading Function.
*
* @param   p        Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA), where 
*                   all averaged non-billing metering values are saved in integer forms.
* @param   urms     Pointer to the averaged phase 1 RMS value of the line voltage [V]
* @param   irms     Pointer to the averaged phase 1 RMS value of the line current [A]
* @param   w        Pointer to the averaged phase 1 active power [W]
* @param   var      Pointer to the averaged phase 1 reactive power [VAR]
* @param   va       Pointer to the averaged phase 1 apparent power [VA]
* @param   pf       Pointer to the averaged phase 1 power factor [-]
* @param   thdu     Pointer to the averaged phase 1 THDu [%]  
* @param   thdi     Pointer to the averaged phase 1 THDi [%]  
*         
* @remarks Calling frequency of this function should be in the range <0.004Hz...line frequency>.
*          In case of lower calling frequency, internal counters may overflow. In this case, the 
*          first dummy reading is neccessary for clearing all internal counters. In case of
*          higher calling frequency, all output values will be zero. All output values are scaled
*          to engineering units in double precision form.
*
************************************************************************************************/
extern void METERLIBFFT2PH_GetAvrgValuesPh1(tMETERLIBFFT2PH_DATA *p, double *urms, double *irms, double *w, double *var, double *va, double *pf, double *thdu, double *thdi);

/********************************************************************************************//*!
*
* @brief    2-Phase Non-Billing (U,I,P,Q,S,pf,THD) Averaged Variables Phase 2 Reading Function.
*
* @param   p        Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA), where 
*                   all averaged non-billing metering values are saved in integer forms.
* @param   urms     Pointer to the averaged phase 2 RMS value of the line voltage [V]
* @param   irms     Pointer to the averaged phase 2 RMS value of the line current [A]
* @param   w        Pointer to the averaged phase 2 active power [W]
* @param   var      Pointer to the averaged phase 2 reactive power [VAR]
* @param   va       Pointer to the averaged phase 2 apparent power [VA]
* @param   pf       Pointer to the averaged phase 2 power factor [-]
* @param   thdu     Pointer to the averaged phase 2 THDu [%]  
* @param   thdi     Pointer to the averaged phase 2 THDi [%]  
*         
* @remarks Calling frequency of this function should be in the range <0.004Hz...line frequency>.
*          In case of lower calling frequency, internal counters may overflow. In this case, the 
*          first dummy reading is neccessary for clearing all internal counters. In case of
*          higher calling frequency, all output values will be zero. All output values are scaled
*          to engineering units in double precision form.
*
************************************************************************************************/
extern void METERLIBFFT2PH_GetAvrgValuesPh2(tMETERLIBFFT2PH_DATA *p, double *urms, double *irms, double *w, double *var, double *va, double *pf, double *thdu, double *thdi);

/********************************************************************************************//*!
*
* @brief    2-Phase Non-Billing (U,I,P,Q,S,pf,THD) Instantaneous Variables Phase 1 Reading Function.
*
* @param   p        Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA), where 
*                   all instantaneous non-billing metering values are saved in integer forms.
* @param   urms     Pointer to the instantaneous phase 1 RMS value of the line voltage [V]
* @param   irms     Pointer to the instantaneous phase 1 RMS value of the line current [A]
* @param   w        Pointer to the instantaneous phase 1 active power [W]
* @param   var      Pointer to the instantaneous phase 1 reactive power [VAR]
* @param   va       Pointer to the instantaneous phase 1 apparent power [VA]
* @param   pf       Pointer to the instantaneous phase 1 power factor [-]
* @param   thdu     Pointer to the instantaneous phase 1 THDu [%]  
* @param   thdi     Pointer to the instantaneous phase 1 THDi [%]  
*         
* @remarks This function could be called anytime, better after the main (FFT) calculation processing.
*          All output values are scaled to engineering units in double precision form.
*
************************************************************************************************/
extern void METERLIBFFT2PH_GetInstValuesPh1(tMETERLIBFFT2PH_DATA *p, double *urms, double *irms, double *w, double *var, double *va, double *pf, double *thdu, double *thdi);

/********************************************************************************************//*!
*
* @brief    2-Phase Non-Billing (U,I,P,Q,S,pf,THD) Instantaneous Variables Phase 2 Reading Function.
*
* @param   p        Pointer to the metering library data structure (tMETERLIBFFT2PH_DATA), where 
*                   all instantaneous non-billing metering values are saved in integer forms.
* @param   urms     Pointer to the instantaneous phase 2 RMS value of the line voltage [V]
* @param   irms     Pointer to the instantaneous phase 2 RMS value of the line current [A]
* @param   w        Pointer to the instantaneous phase 2 active power [W]
* @param   var      Pointer to the instantaneous phase 2 reactive power [VAR]
* @param   va       Pointer to the instantaneous phase 2 apparent power [VA]
* @param   pf       Pointer to the instantaneous phase 2 power factor [-]
* @param   thdu     Pointer to the instantaneous phase 2 THDu [%]  
* @param   thdi     Pointer to the instantaneous phase 2 THDi [%]  
*         
* @remarks This function could be called anytime, better after the main (FFT) calculation processing.
*          All output values are scaled to engineering units in double precision form.
*
************************************************************************************************/
extern void METERLIBFFT2PH_GetInstValuesPh2(tMETERLIBFFT2PH_DATA *p, double *urms, double *irms, double *w, double *var, double *va, double *pf, double *thdu, double *thdi);

/************************************************************************************************
 ************************************************************************************************
 *                                        3-PH METER API
 ************************************************************************************************
************************************************************************************************/

/********************************************************************************************//*!
*
* @brief    3-Phase Parameters Initialization Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   samples     Select @ref fft_samples
* @param   sensor      Select @ref sensor_type
* @param   kwh_cnt     Select @ref impulse_num
* @param   kvarh_cnt   Select @ref impulse_num
* @param   en_res      Select @ref energy_res
*         
* @return  Returns one of the following error code @ref fcn_state
*
* @remarks This mandatory function must be called in the initialization section anyway or 
*          after changing some parameters during the program execution.
*
************************************************************************************************/
extern long METERLIBFFT3PH_InitParam(tMETERLIBFFT3PH_DATA *p, unsigned long samples, unsigned long sensor, unsigned long kwh_cnt, unsigned long kvarh_cnt, unsigned long en_res);

/********************************************************************************************//*!
*
* @brief    3-Phase Main Buffers Initialization Function for the Phase 1.
*
* @param   p          Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   u_re       Pointer to the voltage input time domain buffer1 united with the frequency output 
*                     buffer1 (real part) in Q0.23 data format, buffer length matches with the number of input (AFE) samples
* @param   i_re       Pointer to the current input time domain buffer1 united with the frequency output 
*                     buffer1 (real part) in Q0.23 data format, buffer length matches with the number of input (AFE) samples
* @param   u_im       Pointer to the voltage frequency output buffer1 (imaginary part) in Q0.23 data format, buffer length 
*                     matches with the number of @ref fft_samples set by @ref METERLIBFFT3PH_InitParam function
* @param   i_im       Pointer to the current frequency output buffer1 (imaginary part) in Q0.23 data format, buffer length 
*                     matches with the number of @ref fft_samples set by @ref METERLIBFFT3PH_InitParam function
* @param   shift      Pointer to the input U-I phase shift buffer1 [0.001°], the maximal buffer length 
*                     matches with number of FFT harmonics (half of @ref fft_samples). If the S/W    
*                     phase shift correction is not used, this pointer must be assigned to NULL. 
*         
* @remarks This mandatory function must be called in the initialization section only. This
*          function initializes input time domain buffers, output frequency domain buffers, and
*          U-I phase shifts correction buffer. The first buffers position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT3PH_InitMainBuffPh1(tMETERLIBFFT3PH_DATA *p, frac24 *u_re, frac24 *i_re, frac24 *u_im, frac24 *i_im, long *shift);

/********************************************************************************************//*!
*
* @brief    3-Phase Main Buffers Initialization Function for the Phase 2.
*
* @param   p          Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   u_re       Pointer to the voltage input time domain buffer2 united with the frequency output 
*                     buffer2 (real part) in Q0.23 data format, buffer length matches with the number of input (AFE) samples
* @param   i_re       Pointer to the current input time domain buffer2 united with the frequency output 
*                     buffer2 (real part) in Q0.23 data format, buffer length matches with the number of input (AFE) samples
* @param   u_im       Pointer to the voltage frequency output buffer2 (imaginary part) in Q0.23 data format, buffer length 
*                     matches with the number of @ref fft_samples set by @ref METERLIBFFT3PH_InitParam function
* @param   i_im       Pointer to the current frequency output buffer2 (imaginary part) in Q0.23 data format, buffer length 
*                     matches with the number of @ref fft_samples set by @ref METERLIBFFT3PH_InitParam function
* @param   shift      Pointer to the input U-I phase shift buffer2 [0.001°], the maximal buffer length 
*                     matches with number of FFT harmonics (half of @ref fft_samples). If the S/W    
*                     phase shift correction is not used, this pointer must be assigned to NULL. 
*         
* @remarks This mandatory function must be called in the initialization section only. This
*          function initializes input time domain buffers, output frequency domain buffers, and
*          U-I phase shifts correction buffer. The first buffers position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT3PH_InitMainBuffPh2(tMETERLIBFFT3PH_DATA *p, frac24 *u_re, frac24 *i_re, frac24 *u_im, frac24 *i_im, long *shift);

/********************************************************************************************//*!
*
* @brief    3-Phase Main Buffers Initialization Function for the Phase 3.
*
* @param   p          Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   u_re       Pointer to the voltage input time domain buffer3 united with the frequency output 
*                     buffer3 (real part) in Q0.23 data format, buffer length matches with the number of input (AFE) samples
* @param   i_re       Pointer to the current input time domain buffer3 united with the frequency output 
*                     buffer3 (real part) in Q0.23 data format, buffer length matches with the number of input (AFE) samples
* @param   u_im       Pointer to the voltage frequency output buffer3 (imaginary part) in Q0.23 data format, buffer length 
*                     matches with the number of @ref fft_samples set by @ref METERLIBFFT3PH_InitParam function
* @param   i_im       Pointer to the current frequency output buffer3 (imaginary part) in Q0.23 data format, buffer length 
*                     matches with the number of @ref fft_samples set by @ref METERLIBFFT3PH_InitParam function
* @param   shift      Pointer to the input U-I phase shift buffer3 [0.001°], the maximal buffer length 
*                     matches with number of FFT harmonics (half of @ref fft_samples). If the S/W    
*                     phase shift correction is not used, this pointer must be assigned to NULL. 
*         
* @remarks This mandatory function must be called in the initialization section only. This
*          function initializes input time domain buffers, output frequency domain buffers, and
*          U-I phase shifts correction buffer. The first buffers position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT3PH_InitMainBuffPh3(tMETERLIBFFT3PH_DATA *p, frac24 *u_re, frac24 *i_re, frac24 *u_im, frac24 *i_im, long *shift);

/********************************************************************************************//*!
*
* @brief    3-Phase Auxiliary Buffers Initialization Function for the Phase 1.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   mag_u       Pointer to the harmonic magnitudes voltage buffer in Q0.23 data format
* @param   mag_i       Pointer to the harmonic magnitudes current buffer in Q0.23 data format
* @param   ph_u        Pointer to the harmonic phase shift voltage buffer in 0.001°, e.g. 45000=45.000°
* @param   ph_i        Pointer to the harmonic phase shift current buffer in 0.001°, e.g. 45000=45.000°
*         
* @remarks This function should (may) be called in the initialization section. It is used  
*          for auxiliary buffers initialization only, i.e. when auxiliary computing (magnitudes, 
*          phases) is required. This calculation is done by @ref METERLIBFFT3PH_GetMagnitudesPh1
*          and/or @ref METERLIBFFT3PH_GetPhasesPh1 functions. The first buffers position matches 
*          with zero harmonic, etc. When auxiliary computing is not required, this function
*          doesn't have to be used. The length of all buffers is optional, but their maximal length 
*          cannot exceed the number of FFT harmonics (half of @ref fft_samples).
*
************************************************************************************************/
extern void METERLIBFFT3PH_InitAuxBuffPh1(tMETERLIBFFT3PH_DATA *p, frac24 *mag_u, frac24 *mag_i, long *ph_u, long *ph_i);

/********************************************************************************************//*!
*
* @brief    3-Phase Auxiliary Buffers Initialization Function for the Phase 2.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   mag_u       Pointer to the harmonic magnitudes voltage buffer in Q0.23 data format
* @param   mag_i       Pointer to the harmonic magnitudes current buffer in Q0.23 data format
* @param   ph_u        Pointer to the harmonic phase shift voltage buffer in 0.001°, e.g. 45000=45.000°
* @param   ph_i        Pointer to the harmonic phase shift current buffer in 0.001°, e.g. 45000=45.000°
*         
* @remarks This function should (may) be called in the initialization section. It is used  
*          for auxiliary buffers initialization only, i.e. when auxiliary computing (magnitudes, 
*          phases) is required. This calculation is done by @ref METERLIBFFT3PH_GetMagnitudesPh2
*          and/or @ref METERLIBFFT3PH_GetPhasesPh2 functions. The first buffers position matches 
*          with zero harmonic, etc. When auxiliary computing is not required, this function
*          doesn't have to be used. The length of all buffers is optional, but their maximal length 
*          cannot exceed the number of FFT harmonics (half of @ref fft_samples).
*
************************************************************************************************/
extern void METERLIBFFT3PH_InitAuxBuffPh2(tMETERLIBFFT3PH_DATA *p, frac24 *mag_u, frac24 *mag_i, long *ph_u, long *ph_i);

/********************************************************************************************//*!
*
* @brief    3-Phase Auxiliary Buffers Initialization Function for the Phase 3.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   mag_u       Pointer to the harmonic magnitudes voltage buffer in Q0.23 data format
* @param   mag_i       Pointer to the harmonic magnitudes current buffer in Q0.23 data format
* @param   ph_u        Pointer to the harmonic phase shift voltage buffer in 0.001°, e.g. 45000=45.000°
* @param   ph_i        Pointer to the harmonic phase shift current buffer in 0.001°, e.g. 45000=45.000°
*         
* @remarks This function should (may) be called in the initialization section. It is used  
*          for auxiliary buffers initialization only, i.e. when auxiliary computing (magnitudes, 
*          phases) is required. This calculation is done by @ref METERLIBFFT3PH_GetMagnitudesPh3
*          and/or @ref METERLIBFFT3PH_GetPhasesPh3 functions. The first buffers position matches 
*          with zero harmonic, etc. When auxiliary computing is not required, this function
*          doesn't have to be used. The length of all buffers is optional, but their maximal length 
*          cannot exceed the number of FFT harmonics (half of @ref fft_samples).
*
************************************************************************************************/
extern void METERLIBFFT3PH_InitAuxBuffPh3(tMETERLIBFFT3PH_DATA *p, frac24 *mag_u, frac24 *mag_i, long *ph_u, long *ph_i);

/********************************************************************************************//*!
*
* @brief    3-Phase Set Calibration Coefficients Function for the Phase 1.
*
* @param   p          Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   u_max      Maximum peak phase1 voltage [V] valid for the AFE full scale range (23-bit)
* @param   i_max      Maximum peak phase1 current [A] valid for the AFE full scale range (23-bit)
* @param   i_offs     Pointer to the current1 offset correction value (Q0.23 data format). If the     
*                     offset correction is not used, this pointer should be assigned to NULL 
*                     (non-true Irms computing in this case). 
* @param   p_offs     Active power 1 offset correction value [W]
* @param   q_offs     Reactive power 1 offset correction value [VAr]
*         
* @return  Returns one of the following error code @ref fcn_state. When FFT_ERROR, one of the 
*          calibration coefficients is to big - overflow may occur later. The right coeff. values 
*          should be in this range: (u_max*i_max)<(2^31/10000).
*
* @remarks This function sets all calibration coefficients for the phase 1. It must be called in 
*          the initialization section anyway. It may be called during the AFE calibration processing too.
*
* @note Voltage offset is ignored. This simplification can be used due to zero DC offset in the mains.
*
************************************************************************************************/
extern long METERLIBFFT3PH_SetCalibCoeffPh1(tMETERLIBFFT3PH_DATA *p, double u_max, double i_max, frac24 *i_offs, double p_offs, double q_offs);

/********************************************************************************************//*!
*
* @brief    3-Phase Set Calibration Coefficients Function for the Phase 2.
*
* @param   p          Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   u_max      Maximum peak phase2 voltage [V] valid for the AFE full scale range (23-bit)
* @param   i_max      Maximum peak phase2 current [A] valid for the AFE full scale range (23-bit)
* @param   i_offs     Pointer to the current2 offset correction value (Q0.23 data format). If the     
*                     offset correction is not used, this pointer should be assigned to NULL 
*                     (non-true Irms computing in this case). 
* @param   p_offs     Active power 2 offset correction value [W]
* @param   q_offs     Reactive power 2 offset correction value [VAr]
*         
* @return  Returns one of the following error code @ref fcn_state. When FFT_ERROR, one of the 
*          calibration coefficients is to big - overflow may occur later. The right coeff. values 
*          should be in this range: (u_max*i_max)<(2^31/10000).
*
* @remarks This function sets all calibration coefficients for the phase 2. It must be called in 
*          the initialization section anyway. It may be called during the AFE calibration processing too.
*
* @note Voltage offset is ignored. This simplification can be used due to zero DC offset in the mains.
*
************************************************************************************************/
extern long METERLIBFFT3PH_SetCalibCoeffPh2(tMETERLIBFFT3PH_DATA *p, double u_max, double i_max, frac24 *i_offs, double p_offs, double q_offs);

/********************************************************************************************//*!
*
* @brief    3-Phase Set Calibration Coefficients Function for the Phase 3.
*
* @param   p          Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   u_max      Maximum peak phase3 voltage [V] valid for the AFE full scale range (23-bit)
* @param   i_max      Maximum peak phase3 current [A] valid for the AFE full scale range (23-bit)
* @param   i_offs     Pointer to the current3 offset correction value (Q0.23 data format). If the     
*                     offset correction is not used, this pointer should be assigned to NULL 
*                     (non-true Irms computing in this case). 
* @param   p_offs     Active power 3 offset correction value [W]
* @param   q_offs     Reactive power 3 offset correction value [VAr]
*         
* @return  Returns one of the following error code @ref fcn_state. When FFT_ERROR, one of the 
*          calibration coefficients is to big - overflow may occur later. The right coeff. values 
*          should be in this range: (u_max*i_max)<(2^31/10000).
*
* @remarks This function sets all calibration coefficients for the phase 3. It must be called in 
*          the initialization section anyway. It may be called during the AFE calibration processing too.
*
* @note Voltage offset is ignored. This simplification can be used due to zero DC offset in the mains.
*
************************************************************************************************/
extern long METERLIBFFT3PH_SetCalibCoeffPh3(tMETERLIBFFT3PH_DATA *p, double u_max, double i_max, frac24 *i_offs, double p_offs, double q_offs);

/********************************************************************************************//*!
*
* @brief    3-Phase Interpolation Function.
*
* @param   p              Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   u_ord          Voltage (all phases) interpolation order - select @ref interp_order
* @param   i_ord          Current (all phases) interpolation order - select @ref interp_order
* @param   samples_inp    Number of input samples (> or < than @ref fft_samples) 
*
* @return  Returns one of the following error code @ref fcn_state for undersampling use case. 
*          When FFT_ERROR, undersampling is required for big FFT samples value. 
*          When FFT_OK, undersampling ratio is good.
*         
* @remarks This function must be called before the main (FFT) calculation processing, when samples_inp 
*          doesn't match with the @ref fft_samples initialized by @ref METERLIBFFT3PH_InitParam 
*          function (asynchronous mode). When samples_inp matches with the @ref fft_samples, no 
*          interpolation is needed (synchronous mode). The mandatory parameter initialization 
*          must be done before calling this function. When u_ord or i_ord doesn't match with 
*          @ref interp_order, the function does nothing. This function interpolates raw ADC data 
*          located in all time domain buffers (see u_re and i_re buffer pointers initialized by 
*          @ref METERLIBFFT3PH_InitMainBuffPh1, @ref METERLIBFFT3PH_InitMainBuffPh2, and 
*          @ref METERLIBFFT3PH_InitMainBuffPh3 functions).
*
************************************************************************************************/
extern long METERLIBFFT3PH_Interpolation(tMETERLIBFFT3PH_DATA *p, unsigned long u_ord, unsigned long i_ord, unsigned long samples_inp);

/********************************************************************************************//*!
*
* @brief    3-Phase FFT Calculation and Signal Conditioning Processing Function.
*
* @param   p     Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
*
* @remarks The mandatory parameter initialization must be done before calling this function.
*          The interpolation processing must (may) be done before calling this function. 
*          The energy calculation processing should be done after calling this function.
*
* @note  It performs not only the main FFT calculation, but the whole signal conditioning
*        processing with scaling and averaging of all non-billing values with their saving to the
*        internal structure. All input time domain buffers addressed by u_re and i_re pointers
*        will be rewrited by the FFT real part values, while imaginary parts of the FFT result are 
*        saved to separate buffers set by u_im and i_im pointers. Pointers to all these buffers 
*        must be initialized by @ref METERLIBFFT3PH_InitMainBuffPh1, @ref METERLIBFFT3PH_InitMainBuffPh2, 
*        and @ref METERLIBFFT3PH_InitMainBuffPh3 functions. The first FFT buffers position matches with 
*        zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT3PH_CalcMain(tMETERLIBFFT3PH_DATA *p);

/********************************************************************************************//*!
*
* @brief    3-Phase Harmonic Magnitudes Calculation Function for the Phase 1.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   magn_fft    Number of required magnitudes in the range <1...half of the input samples>
*
* @remarks The mandatory and auxiliary parameter initialization must be done before calling this
*          function. This function may be called after the main (FFT) calculation processing.
*
* @note    FFT U and I magnitudes are available at two buffers addressed by mag_u and 
*          mag_i pointers (initialized by @ref METERLIBFFT3PH_InitAuxBuffPh1 function). The first 
*          buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT3PH_GetMagnitudesPh1(tMETERLIBFFT3PH_DATA *p, unsigned long magn_fft);

/********************************************************************************************//*!
*
* @brief    3-Phase Harmonic Magnitudes Calculation Function for the Phase 2.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   magn_fft    Number of required magnitudes in the range <1...half of the input samples>
*
* @remarks The mandatory and auxiliary parameter initialization must be done before calling this
*          function. This function may be called after the main (FFT) calculation processing.
*
* @note    FFT U and I magnitudes are available at two buffers addressed by mag_u and 
*          mag_i pointers (initialized by @ref METERLIBFFT3PH_InitAuxBuffPh2 function). The first 
*          buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT3PH_GetMagnitudesPh2(tMETERLIBFFT3PH_DATA *p, unsigned long magn_fft);

/********************************************************************************************//*!
*
* @brief    3-Phase Harmonic Magnitudes Calculation Function for the Phase 3.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   magn_fft    Number of required magnitudes in the range <1...half of the input samples>
*
* @remarks The mandatory and auxiliary parameter initialization must be done before calling this
*          function. This function may be called after the main (FFT) calculation processing.
*
* @note    FFT U and I magnitudes are available at two buffers addressed by mag_u and 
*          mag_i pointers (initialized by @ref METERLIBFFT3PH_InitAuxBuffPh3 function). The first 
*          buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT3PH_GetMagnitudesPh3(tMETERLIBFFT3PH_DATA *p, unsigned long magn_fft);

/********************************************************************************************//*!
*
* @brief    3-Phase Harmonic Phase Shifts Calculation Function for the Phase 1.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   ph_fft      Number of required phases in the range <1...half of the input samples>
*
* @remarks The mandatory and auxiliary parameter initialization must be done before calling this
*          function. This function may be called after the main (FFT) calculation processing.
*
* @note    Phase shift of each individual harmonics are available at two buffers addressed
*          by ph_u and ph_i pointers (initialized by @ref METERLIBFFT3PH_InitAuxBuffPh1 function). 
*          The first buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT3PH_GetPhasesPh1(tMETERLIBFFT3PH_DATA *p, unsigned long ph_fft);

/********************************************************************************************//*!
*
* @brief    3-Phase Harmonic Phase Shifts Calculation Function for the Phase 2.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   ph_fft      Number of required phases in the range <1...half of the input samples>
*
* @remarks The mandatory and auxiliary parameter initialization must be done before calling this
*          function. This function may be called after the main (FFT) calculation processing.
*
* @note    Phase shift of each individual harmonics are available at two buffers addressed
*          by ph_u and ph_i pointers (initialized by @ref METERLIBFFT3PH_InitAuxBuffPh2 function). 
*          The first buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT3PH_GetPhasesPh2(tMETERLIBFFT3PH_DATA *p, unsigned long ph_fft);

/********************************************************************************************//*!
*
* @brief    3-Phase Harmonic Phase Shifts Calculation Function for the Phase 3.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   ph_fft      Number of required phases in the range <1...half of the input samples>
*
* @remarks The mandatory and auxiliary parameter initialization must be done before calling this
*          function. This function may be called after the main (FFT) calculation processing.
*
* @note    Phase shift of each individual harmonics are available at two buffers addressed
*          by ph_u and ph_i pointers (initialized by @ref METERLIBFFT3PH_InitAuxBuffPh3 function). 
*          The first buffer position matches with zero harmonic, etc.
*
************************************************************************************************/
extern void METERLIBFFT3PH_GetPhasesPh3(tMETERLIBFFT3PH_DATA *p, unsigned long ph_fft);

/********************************************************************************************//*!
*
* @brief    3-Phase Angle and Rotation Calculation Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA)
* @param   u12_ph      Pointer to the phase2 to the phase1 angle [°] 
* @param   u13_ph      Pointer to the phase3 to the phase1 angle [°] 
* @param   u23_ph      Pointer to the phase3 to the phase2 angle [°] 
*
* @return  One of these states @ref sense_rotation 
*          ROT_FORWARD - clockwise rotation (1-2-3, 2-3-1, or 3-1-2)
*          ROT_REVERSE - counter-clockwise rotation (2-1-3, 1-3-2, or 3-2-1)
*          ROT_UNKNOWN - cannot be recognized (due to phase lossing)
*
* @remarks The mandatory parameter initialization must be done before calling this
*          function. This function may be called after the main (FFT) calculation processing.
*
* @note    The function computes the 1st harmonic phase shifts between each individual
*          phases (1-2, 1-3, and 2-3) and gets the phase rotation (forward, reverse).
*
************************************************************************************************/
extern long METERLIBFFT3PH_GetRotation(tMETERLIBFFT3PH_DATA *p, double *u12_ph, double *u13_ph, double *u23_ph);

/********************************************************************************************//*!
*
* @brief    3-Phase Active Energy Calculation Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA), 
*                      where instantaneous active power is saved. 
* @param   wh_i        Pointer to the LCD counter representing import active energy 
*                      with resolution given by @ref energy_res
* @param   wh_e        Pointer to the LCD counter representing export active energy 
*                      with resolution given by @ref energy_res
* @param   frequency   Line frequency [mHz], e.g. 50000=50.000Hz
*         
* @return  When positive, the function returns active energy LED flashing frequency [mHz] for the 
*          current line period (only one LED flashing per one period). This method for low-jitter 
*          pulse output generation using software and timer is being patented. 
*          When negative, no output pulse generation is needed in the current period.
*
* @remarks The mandatory parameter initialization must be done before calling this function.
*          This function should be called after the main (FFT) calculation processing. The output
*          energy resolution depends on en_res parameter seetings by the @ref METERLIBFFT3PH_InitParam 
*          function.
*
************************************************************************************************/
extern long METERLIBFFT3PH_CalcWattHours(tMETERLIBFFT3PH_DATA *p, unsigned long *wh_i, unsigned long *wh_e, unsigned long frequency);

/********************************************************************************************//*!
*
* @brief    3-Phase Reactive Energy Calculation Function.
*
* @param   p           Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA), 
*                      where instantaneous reactive power is saved. 
* @param   varh_i      Pointer to the LCD counter representing import reactive energy 
*                      with resolution given by @ref energy_res
* @param   varh_e      Pointer to the LCD counter representing export reactive energy 
*                      with resolution given by @ref energy_res
* @param   frequency   Line frequency [mHz], e.g. 50000=50.000Hz
*         
* @return  When positive, the function returns reactive energy LED flashing frequency [mHz] for the 
*          current line period (only one LED flashing per one period). This method for low-jitter 
*          pulse output generation using software and timer is being patented. 
*          When negative, no output pulse generation is needed in the current period.
*
* @remarks The mandatory parameter initialization must be done before calling this function.
*          This function should be called after the main (FFT) calculation processing. The output
*          energy resolution depends on en_res parameter seetings by the @ref METERLIBFFT3PH_InitParam 
*          function.
*
************************************************************************************************/
extern long METERLIBFFT3PH_CalcVarHours(tMETERLIBFFT3PH_DATA *p, unsigned long *varh_i, unsigned long *varh_e, unsigned long frequency);

/********************************************************************************************//*!
*
* @brief    3-Phase Non-Billing (U,I,P,Q,S,pf,THD) Averaged Variables Phase 1 Reading Function.
*
* @param   p        Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA), where 
*                   all averaged non-billing metering values are saved in integer forms.
* @param   urms     Pointer to the averaged phase 1 RMS value of the line voltage [V]
* @param   irms     Pointer to the averaged phase 1 RMS value of the line current [A]
* @param   w        Pointer to the averaged phase 1 active power [W]
* @param   var      Pointer to the averaged phase 1 reactive power [VAR]
* @param   va       Pointer to the averaged phase 1 apparent power [VA]
* @param   pf       Pointer to the averaged phase 1 power factor [-]
* @param   thdu     Pointer to the averaged phase 1 THDu [%]  
* @param   thdi     Pointer to the averaged phase 1 THDi [%]  
*         
* @remarks Calling frequency of this function should be in the range <0.004Hz...line frequency>.
*          In case of lower calling frequency, internal counters may overflow. In this case, the 
*          first dummy reading is neccessary for clearing all internal counters. In case of
*          higher calling frequency, all output values will be zero. All output values are scaled
*          to engineering units in double precision form.
*
************************************************************************************************/
extern void METERLIBFFT3PH_GetAvrgValuesPh1(tMETERLIBFFT3PH_DATA *p, double *urms, double *irms, double *w, double *var, double *va, double *pf, double *thdu, double *thdi);

/********************************************************************************************//*!
*
* @brief    3-Phase Non-Billing (U,I,P,Q,S,pf,THD) Averaged Variables Phase 2 Reading Function.
*
* @param   p        Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA), where 
*                   all averaged non-billing metering values are saved in integer forms.
* @param   urms     Pointer to the averaged phase 2 RMS value of the line voltage [V]
* @param   irms     Pointer to the averaged phase 2 RMS value of the line current [A]
* @param   w        Pointer to the averaged phase 2 active power [W]
* @param   var      Pointer to the averaged phase 2 reactive power [VAR]
* @param   va       Pointer to the averaged phase 2 apparent power [VA]
* @param   pf       Pointer to the averaged phase 2 power factor [-]
* @param   thdu     Pointer to the averaged phase 2 THDu [%]  
* @param   thdi     Pointer to the averaged phase 2 THDi [%]  
*         
* @remarks Calling frequency of this function should be in the range <0.004Hz...line frequency>.
*          In case of lower calling frequency, internal counters may overflow. In this case, the 
*          first dummy reading is neccessary for clearing all internal counters. In case of
*          higher calling frequency, all output values will be zero. All output values are scaled
*          to engineering units in double precision form.
*
************************************************************************************************/
extern void METERLIBFFT3PH_GetAvrgValuesPh2(tMETERLIBFFT3PH_DATA *p, double *urms, double *irms, double *w, double *var, double *va, double *pf, double *thdu, double *thdi);

/********************************************************************************************//*!
*
* @brief    3-Phase Non-Billing (U,I,P,Q,S,pf,THD) Averaged Variables Phase 3 Reading Function.
*
* @param   p        Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA), where 
*                   all averaged non-billing metering values are saved in integer forms.
* @param   urms     Pointer to the averaged phase 3 RMS value of the line voltage [V]
* @param   irms     Pointer to the averaged phase 3 RMS value of the line current [A]
* @param   w        Pointer to the averaged phase 3 active power [W]
* @param   var      Pointer to the averaged phase 3 reactive power [VAR]
* @param   va       Pointer to the averaged phase 3 apparent power [VA]
* @param   pf       Pointer to the averaged phase 3 power factor [-]
* @param   thdu     Pointer to the averaged phase 3 THDu [%]  
* @param   thdi     Pointer to the averaged phase 3 THDi [%]  
*         
* @remarks Calling frequency of this function should be in the range <0.004Hz...line frequency>.
*          In case of lower calling frequency, internal counters may overflow. In this case, the 
*          first dummy reading is neccessary for clearing all internal counters. In case of
*          higher calling frequency, all output values will be zero. All output values are scaled
*          to engineering units in double precision form.
*
************************************************************************************************/
extern void METERLIBFFT3PH_GetAvrgValuesPh3(tMETERLIBFFT3PH_DATA *p, double *urms, double *irms, double *w, double *var, double *va, double *pf, double *thdu, double *thdi);

/********************************************************************************************//*!
*
* @brief    3-Phase Non-Billing (U,I,P,Q,S,pf,THD) Instantaneous Variables Phase 1 Reading Function.
*
* @param   p        Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA), where 
*                   all instantaneous non-billing metering values are saved in integer forms.
* @param   urms     Pointer to the instantaneous phase 1 RMS value of the line voltage [V]
* @param   irms     Pointer to the instantaneous phase 1 RMS value of the line current [A]
* @param   w        Pointer to the instantaneous phase 1 active power [W]
* @param   var      Pointer to the instantaneous phase 1 reactive power [VAR]
* @param   va       Pointer to the instantaneous phase 1 apparent power [VA]
* @param   pf       Pointer to the instantaneous phase 1 power factor [-]
* @param   thdu     Pointer to the instantaneous phase 1 THDu [%]  
* @param   thdi     Pointer to the instantaneous phase 1 THDi [%]  
*         
* @remarks This function could be called anytime, better after the main (FFT) calculation processing.
*          All output values are scaled to engineering units in double precision form.
*
************************************************************************************************/
extern void METERLIBFFT3PH_GetInstValuesPh1(tMETERLIBFFT3PH_DATA *p, double *urms, double *irms, double *w, double *var, double *va, double *pf, double *thdu, double *thdi);

/********************************************************************************************//*!
*
* @brief    3-Phase Non-Billing (U,I,P,Q,S,pf,THD) Instantaneous Variables Phase 2 Reading Function.
*
* @param   p        Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA), where 
*                   all instantaneous non-billing metering values are saved in integer forms.
* @param   urms     Pointer to the instantaneous phase 2 RMS value of the line voltage [V]
* @param   irms     Pointer to the instantaneous phase 2 RMS value of the line current [A]
* @param   w        Pointer to the instantaneous phase 2 active power [W]
* @param   var      Pointer to the instantaneous phase 2 reactive power [VAR]
* @param   va       Pointer to the instantaneous phase 2 apparent power [VA]
* @param   pf       Pointer to the instantaneous phase 2 power factor [-]
* @param   thdu     Pointer to the instantaneous phase 2 THDu [%]  
* @param   thdi     Pointer to the instantaneous phase 2 THDi [%]  
*         
* @remarks This function could be called anytime, better after the main (FFT) calculation processing.
*          All output values are scaled to engineering units in double precision form.
*
************************************************************************************************/
extern void METERLIBFFT3PH_GetInstValuesPh2(tMETERLIBFFT3PH_DATA *p, double *urms, double *irms, double *w, double *var, double *va, double *pf, double *thdu, double *thdi);

/********************************************************************************************//*!
*
* @brief    3-Phase Non-Billing (U,I,P,Q,S,pf,THD) Instantaneous Variables Phase 3 Reading Function.
*
* @param   p        Pointer to the metering library data structure (tMETERLIBFFT3PH_DATA), where 
*                   all instantaneous non-billing metering values are saved in integer forms.
* @param   urms     Pointer to the instantaneous phase 3 RMS value of the line voltage [V]
* @param   irms     Pointer to the instantaneous phase 3 RMS value of the line current [A]
* @param   w        Pointer to the instantaneous phase 3 active power [W]
* @param   var      Pointer to the instantaneous phase 3 reactive power [VAR]
* @param   va       Pointer to the instantaneous phase 3 apparent power [VA]
* @param   pf       Pointer to the instantaneous phase 3 power factor [-]
* @param   thdu     Pointer to the instantaneous phase 3 THDu [%]  
* @param   thdi     Pointer to the instantaneous phase 3 THDi [%]  
*         
* @remarks This function could be called anytime, better after the main (FFT) calculation processing.
*          All output values are scaled to engineering units in double precision form.
*
************************************************************************************************/
extern void METERLIBFFT3PH_GetInstValuesPh3(tMETERLIBFFT3PH_DATA *p, double *urms, double *irms, double *w, double *var, double *va, double *pf, double *thdu, double *thdi);

/********************************************************************************************//*!
* @example meterlibfft1ph_test.c
* This example demonstrates use of the FFT metering library in the typical single-phase power 
* meter application with proportional current sensor and with the S/W phase shift correction.
* AFE conversion mode (single/continuous) may be selected by the 'AFE_MODE' constant. Number of 
* the FFT samples is set to 32, number of output harmonics is 16 then.
************************************************************************************************/

/********************************************************************************************//*!
* @example meterlibfft2ph_test.c
* This example demonstrates use of the FFT metering library in the typical two-phase power 
* meter application with derivative current sensor without the S/W phase shift correction.
* AFE conversion mode (single/continuous) may be selected by the 'AFE_MODE' constant. Number of 
* the FFT samples is set to 64, number of output harmonics is 32 then.
************************************************************************************************/

/********************************************************************************************//*!
* @example meterlibfft3ph_test.c
* This example demonstrates use of the FFT metering library in the typical three-phase power 
* meter application with proportional current sensor and with the S/W phase shift correction.
* AFE conversion mode (single/continuous) may be selected by the 'AFE_MODE' constant. Number of 
* the FFT samples is set to 64, number of output harmonics is 32 then. This example allows to 
* measure the sense of rotation of the 3-ph mains.
************************************************************************************************/

#endif /* __METERLIBFFT_H__ */