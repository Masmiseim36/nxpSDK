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

#include "mcdrv_enc_qd.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Function returns actual position and speed
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_QdEncGet(mcdrv_qd_enc_t *this)
{

   /* read number of pulses and get mechanical position */
   this->f16PosMe = (frac16_t)(MLIB_Mul_F16as(this->a32PosMeGain, (frac16_t)(this->pui32QdBase->LPOS)));

   /* tracking observer calculation */
   this->f16PosMeEst = (frac16_t)AMCLIB_TrackObsrv_A32af(this->a32PosErr, &this->sTo);

   /* calculation of error function for tracking observer */
   this->a32PosErr = (acc32_t)MLIB_Sub_F16(this->f16PosMe, this->f16PosMeEst);

   /* speed estimation by the tracking observer */
   this->fltSpdMeEst = this->sTo.fltSpeed;
   
   /* read revolution counter */
   this->f16RevCounter = (frac16_t)(this->pui32QdBase->REV);

   /* calculating position for position control */
   this->a32PosMeReal = (acc32_t)( ( ( ((int32_t)(this->f16RevCounter)) << 15) + (((uint16_t)(this->f16PosMe)) >> 1) ) ); 

   /* pass estimator speed values lower than minimal encoder speed */
   if ((MLIB_Abs_FLT(this->fltSpdMeEst) < (this->fltSpdEncMin)))
   {
       this->fltSpdMeEst = 0U;
   }

   /* store results to user-defined variables */
   *this->pf16PosElEst = (frac16_t)(this->f16PosMeEst * this->ui16Pp);
   *this->pfltSpdMeEst = (this->fltSpdMeEst);

}

/*!
 * @brief Function clears internal variables and decoder counter
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_QdEncClear(mcdrv_qd_enc_t *this)
{

   this->f16PosMe    = 0;
   this->f16PosMeEst = 0;
   this->fltSpdMeEst = 0;

   /* initilize tracking observer */
   this->sTo.f32Theta = 0;
   this->sTo.fltSpeed = 0;
   this->sTo.fltI_1   = 0;

   /* clear decoder counters */
   this->pui32QdBase->POSD = 0;
   this->pui32QdBase->REV  = 0;
   this->pui32QdBase->LPOS = 0;
   this->pui32QdBase->UPOS = 0;

}

/*!
 * @brief Function set mechanical position of quadrature encoder
 *
 * @param this     Pointer to the current object
 *        f16PosMe Mechanical position
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_QdEncSetPosMe(mcdrv_qd_enc_t *this, frac16_t f16PosMe)
{
   frac16_t f16CntMod;

   f16CntMod    = (frac16_t)(this->pui32QdBase->LMOD >> 1);

   /* set mechnical position */
   this->f16PosMe          = f16PosMe;
   this->sTo.f32Theta      = MLIB_Conv_F32s(f16PosMe);
   this->pui32QdBase->LPOS = (uint16_t)(MLIB_Mul_F16(f16PosMe, f16CntMod) + (uint16_t)f16CntMod);

}

/*!
 * @brief Function set direction of quadrature encoder
 *
 * @param this       Pointer to the current object
 *        bDirection Encoder direction
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_QdEncSetDirection(mcdrv_qd_enc_t *this)
{

   /* forward/reverse */
   if (this->bDirection)
       this->pui32QdBase->CTRL |= ENC_CTRL_REV_MASK;
   else
       this->pui32QdBase->CTRL &= ~ENC_CTRL_REV_MASK;

}

/*!
 * @brief Function set quadrature encoder pulses per one revolution
 *
 * @param this            Pointer to the current object
 *        ui16PulseNumber Encoder pulses per revolution
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_QdEncSetPulses(mcdrv_qd_enc_t *this)
{
    /* Set modulo counter to encoder number of pulses * 4 - 1 */
   this->pui32QdBase->LMOD = (this->ui16PulseNumber * 4U) - 1U;
}
