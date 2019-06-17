/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_enc_qd.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
static bool_t s_statusPass;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Function returns actual position and speed
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_QdEncGet(mcdrv_qd_enc_t *this)
{
    s_statusPass = TRUE;
    
    /* read number of pulses and get mechanical position */
    this->f16PosMe = (frac16_t)(MLIB_Mul_F16as(this->a32PosMeGain, 
        (frac16_t)(this->pui32QdBase->LPOS)));  
       
    /* tracking observer calculation */			
    this->f16PosMeEst = (frac16_t)AMCLIB_TrackObsrv_A32af(this->a32PosErr, &this->sTo);

    /* calculation of error function for tracking observer */  
    this->a32PosErr  = (acc32_t)MLIB_Sub_F16(this->f16PosMe, this->f16PosMeEst);
    
    if(this->a32PosErr > ACC32(0.5))  this->a32PosErr -= ACC32(1.0);
    else if(this->a32PosErr < ACC32(-0.5))  this->a32PosErr += ACC32(1.0);

    /* speed estimation by the tracking observer */
    this->fltSpdMeEst = this->sTo.fltSpeed;
    /* read revolution counter */
    this->ui32RevCounter = (this->pui32QdBase->REV);
    
    /* calculating position for position control */   
    if(((this->ui32RevCounter) > 32767) & ((this->ui32RevCounter) <= 65535))
        this->a32PosMeReal = (acc32_t)((uint16_t)(this->f16PosMe) + (65535U * this->ui32RevCounter)+65535U);
    else
        this->a32PosMeReal = (acc32_t)((uint16_t)(this->f16PosMe) + (65535U * this->ui32RevCounter));
    
    /* pass estimator speed values lower than minimal encoder speed */
    if ((MLIB_Abs_FLT(this->fltSpdMeEst ) < (this->fltSpdEncMin))) 
    {
        this->fltSpdMeEst = 0U;
    }

    /* store results to user-defined variables */
    *this->pf16PosElEst = (frac16_t)(this->f16PosMeEst * this->ui16Pp );
    *this->pfltSpdMeEst = (this->fltSpdMeEst);

    return (s_statusPass);
}


/*!
 * @brief Function clears internal variables and decoder counter
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_QdEncClear(mcdrv_qd_enc_t *this)
{
    s_statusPass = TRUE;

    this->f16PosMe      = 0;
    this->f16PosMeEst   = 0;
    this->fltSpdMeEst   = 0;
    
    /* initilize tracking observer */
    this->sTo.f32Theta  = 0;
    this->sTo.fltSpeed  = 0;
    this->sTo.fltI_1    = 0;
    
    /* clear decoder counters */
    this->pui32QdBase->POSD     = 0;
    this->pui32QdBase->REV      = 0;
    this->pui32QdBase->LPOS     = 0;
    this->pui32QdBase->UPOS     = 0;
    
    return (s_statusPass);
}

/*!
 * @brief Function set mechanical position of quadrature encoder
 *
 * @param this     Pointer to the current object
 *        f16PosMe Mechanical position
 *
 * @return boot_t true on success
 */
bool_t MCDRV_QdEncSetPosMe(mcdrv_qd_enc_t *this, frac16_t f16PosMe)
{
    frac16_t f16CntMod;
  
    s_statusPass = TRUE;
    f16CntMod = (frac16_t)(this->pui32QdBase->LMOD>>1);
    
    /* set mechnical position */
    this->f16PosMe = f16PosMe;
    this->sTo.f32Theta = MLIB_Conv_F32s(f16PosMe);
    this->pui32QdBase->LPOS =  (uint16_t)(MLIB_Mul_F16(f16PosMe, f16CntMod) + 
                               (uint16_t)f16CntMod);

    return(s_statusPass);
}

/*!
 * @brief Function set direction of quadrature encoder
 *
 * @param this       Pointer to the current object
 *        bDirection Encoder direction
 *
 * @return boot_t true on success
 */
bool_t MCDRV_QdEncSetDirection(mcdrv_qd_enc_t *this)
{
    s_statusPass = TRUE;
    
    /* forward/reverse */
    if(this->bDirection)
        this->pui32QdBase->CTRL |= ENC_CTRL_REV_MASK;
    else
        this->pui32QdBase->CTRL &= ~ENC_CTRL_REV_MASK;

    return(s_statusPass);
}
