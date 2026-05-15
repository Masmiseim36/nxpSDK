/*
* Copyright 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024-2025 NXP
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

#define NOT_USED(x) ((void)(x))
#define MCDRV_QD (1)

typedef struct _mcdrv_eqd_enc_t
{
    EQDC_Type *pui32QdBase;       /* pointer to QD module base address*/
    float_t *pfltSpdMeEst;        /* pointer to measured mechanical speed  */
    frac16_t *pf16PosElEst;       /* pointer to measured electrical position */
    acc32_t *pa32PosMeReal;       /* pointer to real position (revolution counter + mechanical position) */

    float_t fltSpdMeEst;          /* estimated speed calculated using tracking observer */
    frac16_t f16PosMe;            /* mechanical position calculated using encoder edges */
    uint16_t ui16Pp;              /* number of motor pole pairs */
    bool_t bDirection;            /* encoder direction */
    frac16_t f16RevCounter;       /* revolution counter measured by periphery */
    uint16_t ui16PulseNumber;     /* quadrature pulses per one revolution */
    
    int16_t i16POSDH;
    uint16_t ui16POSDPERH;
    uint16_t ui16LASTEDGEH;

    int16_t  i16PosDiff;           /* position counter differences between 2 consecutive speed calculations */
    uint16_t ui16Period;           /* time duration corresponding to the position counter differences */
    uint16_t ui16Period_1;         /* last time duration */
    
    int8_t   i8SpeedSign;          /* speed sign in this step */
    int8_t   i8SpeedSign_1;        /* speed sign in last step */
    frac32_t f32Speed;             /* calculated speed in frac32 */
    float_t  fltSpdMech;	   /* mechanical speed in float */
    
    uint32_t ui32CurrentCount;     /* Current QDC counter value */
    int32_t  i32Q10Cnt2PosGain;    /* A gain to convert QDC counter value to scaled position value -1~1. This gain is Q22.10 format */
    
    frac32_t f32PosMech;           /* Rotor real mechanical position */
    frac32_t f32PosMechInit;       /* The mechanical rotor position corresponding to the initial QDC counter value */
    frac32_t f32PosMechOffset;     /* Rotor real mechanical position at the initial position */
   
    frac32_t f32SpeedCalConst;
    float_t  fltSpeedFracToAngularCoeff;
    
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
