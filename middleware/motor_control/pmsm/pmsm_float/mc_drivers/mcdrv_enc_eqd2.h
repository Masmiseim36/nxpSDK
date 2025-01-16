/*
* Copyright 2016, Freescale Semiconductor, Inc.
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
 */
#ifndef _MCDRV_ENC_EQD2_H_
#define _MCDRV_ENC_EQD2_H_

#include "mlib.h"
#include "mlib_types.h"
#include "fsl_device_registers.h"
#include "amclib.h"
#include "pmsm_control.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MCDRV_QD (1)
   
#define PI 3.1415926
#define EXPONENT_1(x)	(x)
#define EXPONENT_3(x)	(x*x*x)
#define EXPONENT_5(x)	(x*x*x*x*x)
#define EXPONENT_7(x)	(x*x*x*x*x*x*x)
#define EXPONENT_9(x)	(x*x*x*x*x*x*x*x*x)
#define EXPONENT_11(x)	(x*x*x*x*x*x*x*x*x*x*x)

#define EXPONENT_0(x)   (1)
#define EXPONENT_2(x)	(x*x)
#define EXPONENT_4(x)	(x*x*x*x)
#define EXPONENT_6(x)	(x*x*x*x*x*x)
#define EXPONENT_8(x)	(x*x*x*x*x*x*x*x)
#define EXPONENT_10(x)	(x*x*x*x*x*x*x*x*x*x)
#define EXPONENT_12(x)	(x*x*x*x*x*x*x*x*x*x*x*x)
#define EXPONENT_13(x)	(x*x*x*x*x*x*x*x*x*x*x*x*x)
#define EXPONENT_14(x)	(x*x*x*x*x*x*x*x*x*x*x*x*x*x)
#define EXPONENT_15(x)	(x*x*x*x*x*x*x*x*x*x*x*x*x*x*x)
#define EXPONENT(x,y)  EXPONENT_intermediate(x,y)
#define EXPONENT_intermediate(x,y)  EXPONENT_##y(x)  // x^y

#define TAN(x) 		                        (x+EXPONENT_3(x)/3.0F+2*EXPONENT_5(x)/15.0F+17*EXPONENT_7(x)/315.0F+62*EXPONENT_9(x)/2835.0F+1382*EXPONENT_11(x)/155925.0F)

#define WARP(x,y)	                        (2*TAN(2*PI*x/(2*y))) 

#define M1_QDC_SPEED_FILTER_CUTOFF_FREQ 	100.0 	  /* [Hz], cutoff frequency of IIR1 low pass filter for calculated raw speed out of QDC HW feature */
#define M1_QDC_SPEED_FILTER_IIR_B0		WARP(M1_QDC_SPEED_FILTER_CUTOFF_FREQ, M1_SLOW_LOOP_FREQ)/(WARP(M1_QDC_SPEED_FILTER_CUTOFF_FREQ, M1_SLOW_LOOP_FREQ) + 2.0F)
#define M1_QDC_SPEED_FILTER_IIR_B1	        WARP(M1_QDC_SPEED_FILTER_CUTOFF_FREQ, M1_SLOW_LOOP_FREQ)/(WARP(M1_QDC_SPEED_FILTER_CUTOFF_FREQ, M1_SLOW_LOOP_FREQ) + 2.0F)
#define M1_QDC_SPEED_FILTER_IIR_A1		(1.0F - M1_QDC_SPEED_FILTER_IIR_B0 - M1_QDC_SPEED_FILTER_IIR_B1)
#define M1_QDC_SPEED_FILTER_IIR_B0_FRAC		FRAC32(M1_QDC_SPEED_FILTER_IIR_B0/2)
#define M1_QDC_SPEED_FILTER_IIR_B1_FRAC		FRAC32(M1_QDC_SPEED_FILTER_IIR_B1/2)
#define M1_QDC_SPEED_FILTER_IIR_A1_FRAC		FRAC32(M1_QDC_SPEED_FILTER_IIR_A1/2)

typedef struct _mcdrv_eqd_enc_t
{
    AMCLIB_TRACK_OBSRV_T_FLT sTo; /* tracking observer structure */
    EQDC_Type *pui32QdBase;       /* pointer to QD module base address*/
    float_t *pfltSpdMeEst;        /* pointer to measured mechanical speed  */
    frac16_t *pf16PosElEst;       /* pointer to measured electrical position */
    acc32_t a32PosErr;            /* position error to tracking observer  */
    acc32_t a32PosMeGain;         /* encoder pulses to mechanical position scale gain */
    float_t fltSpdMeEst;          /* estimated speed calculated using tracking observer */
    frac16_t f16PosMe;            /* mechanical position calculated using encoder edges */
    frac16_t f16PosMeEst;         /* estimated position calculated using tracking observer */
    uint16_t ui16Pp;              /* number of motor pole pairs */
    bool_t bDirection;            /* encoder direction */
    bool_t bToSwitch;             /* TO switch */
    float_t fltSpdEncMin;         /* encoder minimal speed resolution */
    frac16_t f16PosErr;           /* poisition error to tracking observer  */
    frac16_t f16PosMeGain;        /* encoder pulses to mechanical position scale gain */
    int16_t i16PosMeGainSh;       /* encoder pulses to mechanical position scale shift */
    acc32_t a32PosMeReal;         /* real position (revolution counter + mechanical position) */
    frac16_t f16RevCounter;       /* revolution counter measured by periphery */
    uint16_t ui16PulseNumber;     /* quadrature pulses per one revolution */
    
    uint16_t ui16Dummy;
    int16_t i16POSDH;
    uint16_t ui16POSDPERH;
    uint16_t ui16LASTEDGEH;

    int16_t  i16PosDiff;           /* position counter differences between 2 consecutive speed calculations */
    uint16_t ui16Period;           /* time duration corresponding to the position counter differences */
    uint16_t ui16Period_1;         /* last time duration */
    
    int8_t   i8SpeedSign;          /* speed sign in this step */
    int8_t   i8SpeedSign_1;        /* speed sign in last step */
    frac32_t f32Speed;             /* calculated speed in frac32 */
    frac16_t f16SpeedFilt;         /* calculated filtered speed */
    float_t  fltSpdMech;	   /* mechanical speed in float */
    
    uint32_t ui32CurrentCount;     /* Current QDC counter value */
    int32_t  i32Q10Cnt2PosGain;    /* A gain to convert QDC counter value to scaled position value -1~1. This gain is Q22.10 format */
    
    frac32_t f32PosMech;           /* Rotor real mechanical position */
    frac32_t f32PosMechInit;       /* The mechanical rotor position corresponding to the initial QDC counter value */
    frac32_t f32PosMechOffset;     /* Rotor real mechanical position at the initial position */
   
    GDFLIB_FILTER_IIR1_T_F32  sSpeedEncFilter;
    frac32_t f32SpeedCalConst;
    float_t  fltSpeedFrac16ToAngularCoeff;
    
    uint32_t ui32QDTimerFrequency;
    
    
} mcdrv_eqd_enc_t;

extern volatile float g_fltM1speedScale;


/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Function returns actual position
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_QdEncGetPosition(mcdrv_eqd_enc_t *this);

/*!
 * @brief Function returns actual speed
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_QdEncGetSpeed(mcdrv_eqd_enc_t *this);

/*!
 * @brief Function clears internal variables and decoder counter
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_QdEncClear(mcdrv_eqd_enc_t *this);

/*!
 * @brief Function set direction of quadrature encoder
 *
 * @param this       Pointer to the current object
 *        bDirection Encoder direction
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_QdEncSetDirection(mcdrv_eqd_enc_t *this);

/*!
 * @brief Function set quadrature encoder pulses per one revolution
 *
 * @param this            Pointer to the current object
 *        ui16PulseNumber Encoder pulses per revolution
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_QdEncSetPulses(mcdrv_eqd_enc_t *this);

/*!
 * @brief Function update QDC structure parameters
 *
 * @param this            Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_QdEncUpdateParameters(mcdrv_eqd_enc_t *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_ENC_EQD2_H_ */
