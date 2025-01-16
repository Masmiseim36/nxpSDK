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

#include "mcdrv_enc_eqd2.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Function returns actual position
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_QdEncGetPosition(mcdrv_eqd_enc_t *this)
{

    frac32_t f32Pos;

    /* read number of pulses and get mechanical position */
    this->ui16Dummy = this->pui32QdBase->LPOS;
    this->ui32CurrentCount = ((uint32_t)(this->pui32QdBase->UPOSH)<<16)|(this->pui32QdBase->LPOSH);

    f32Pos = ((uint64_t)this->i32Q10Cnt2PosGain * this->ui32CurrentCount)>>10; /* Q22.10 * Q32 = Q54.10, get rid of the last 10 fractional bits, keeping the last 32bits of Q54
                                                                           think of this result as a Q1.31 format, which represents -pi ~ pi */
    this->f32PosMech = f32Pos - this->f32PosMechInit + this->f32PosMechOffset;
    this->f16PosMe = MLIB_Conv_F16l(this->f32PosMech);

    /* read revolution counter */
    this->f16RevCounter = (frac16_t)(this->pui32QdBase->REV);

    /* calculating position for position control */
    this->a32PosMeReal = (acc32_t)( ( ( ((int32_t)(this->f16RevCounter)) << 15) + (((uint16_t)(this->f16PosMe)) >> 1) ) ); 

    /* store results to user-defined variables */
    *this->pf16PosElEst = (frac16_t)(this->f16PosMe * this->ui16Pp);

}


/*!
 * @brief Calculate rotor speed by QDC enhanced M/T speed measurement feature.
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_QdEncGetSpeed(mcdrv_eqd_enc_t *this)
{
    int64_t i64Numerator;

    /* Read POSDH, POSDPERH and LASTEDGEH */
    this->ui16Dummy = this->pui32QdBase->POSD;
    this->i16POSDH = this->pui32QdBase->POSDH;
    this->ui16POSDPERH = this->pui32QdBase->POSDPERH;
    this->ui16LASTEDGEH = this->pui32QdBase->LASTEDGEH;

    /* POSDH == 0? */
    if(this->i16POSDH != 0)
    {
        /* Shaft is moving during speed measurement interval */
        this->i16PosDiff = this->i16POSDH;
        this->ui16Period = this->ui16POSDPERH;
        this->ui16Period_1 = this->ui16Period;

        if(this->i16PosDiff > 0)
        {
            this->i8SpeedSign = 1;
        }
        else
        {
            this->i8SpeedSign = -1;
        }

        if(this->i8SpeedSign == this->i8SpeedSign_1)
        {
            /* Calculate speed */
            i64Numerator = ((int64_t)(this->i16PosDiff) * this->f32SpeedCalConst); /* Q16.0 * Q5.27 = Q21.27 */
            this->f32Speed = (i64Numerator / (uint32_t)(this->ui16Period))<<4; /* Q5.27 -> Q1.31 */
        }
        else
        {
            this->f32Speed = FRAC32(0.0);
        }
        this->i8SpeedSign_1 = this->i8SpeedSign;
    }
    else
    {
        /* Shaft is NOT moving during speed measurement interval */
        this->ui16Period = this->ui16LASTEDGEH;

        if((uint32_t)(this->ui16Period) > 0xF000UL)
        {
            /* Shaft hasn't been moving for a long time */
            this->f32Speed = FRAC32(0.0);
            this->i8SpeedSign_1 = this->i8SpeedSign;
        }
        else
        {
            /* Speed estimation in low speed region */
            if(this->ui16Period > this->ui16Period_1)
            {
                if(this->i8SpeedSign > 0)
                {
                    i64Numerator = ((int64_t)(1) * this->f32SpeedCalConst);
                    this->f32Speed = (i64Numerator / (uint32_t)(this->ui16Period))<<4;
                }
                else
                {
                    i64Numerator = ((int64_t)(-1) * this->f32SpeedCalConst);
                    this->f32Speed = (i64Numerator / (uint32_t)(this->ui16Period))<<4;
                }
            }
        }
    }

    this->f16SpeedFilt = GDFLIB_FilterIIR1_F16(MLIB_Conv_F16l(this->f32Speed), &this->sSpeedEncFilter);
    this->fltSpdMech = MLIB_ConvSc_FLTsf(this->f16SpeedFilt, this->fltSpeedFrac16ToAngularCoeff);
    
    this->fltSpdMeEst = this->fltSpdMech; 

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
void MCDRV_QdEncClear(mcdrv_eqd_enc_t *this)
{

    this->f16PosMe    = FRAC16(0.0);
    this->f16PosMeEst = FRAC16(0.0);
    this->f32PosMech  = FRAC32(0.0);
    this->fltSpdMeEst = 0.0F;
    this->fltSpdMech  = 0.0F;

    /* initilize tracking observer */
    this->sTo.f32Theta = FRAC32(0.0);
    this->sTo.fltSpeed = 0.0F;
    this->sTo.fltI_1   = 0.0F;
    
    this->f16SpeedFilt  = FRAC16(0.0);
    this->f32Speed      = FRAC32(0.0);
    this->i8SpeedSign   = 0;
    this->i8SpeedSign_1 = 0;
    this->ui16Period    = 0U;
    this->ui16Period_1  = 0U;
    this->i16PosDiff    = 0;

    this->sSpeedEncFilter.f16FltBfrX[0] = FRAC16(0.0);
    this->sSpeedEncFilter.f32FltBfrY[0] = FRAC32(0.0);
    
    /* clear decoder counters */
    this->pui32QdBase->POSD = 0;
    this->pui32QdBase->REV  = 0;
    this->pui32QdBase->LPOS = 0;
    this->pui32QdBase->UPOS = 0;

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
void MCDRV_QdEncSetDirection(mcdrv_eqd_enc_t *this)
{

    /* forward/reverse */
    if (this->bDirection)
        this->pui32QdBase->CTRL |= EQDC_CTRL_REV_MASK;
    else
        this->pui32QdBase->CTRL &= ~EQDC_CTRL_REV_MASK;

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
void MCDRV_QdEncSetPulses(mcdrv_eqd_enc_t *this)
{
     /* Set modulo counter to encoder number of pulses * 4 - 1 */
    this->pui32QdBase->LMOD = (this->ui16PulseNumber * 4U) - 1U;
    this->pui32QdBase->CTRL |= EQDC_CTRL_LDOK(1U);
}


/*!
 * @brief Function update QDC structure parameters
 *
 * @param this            Pointer to the current object
 *
 * @return none
 */
void MCDRV_QdEncUpdateParameters(mcdrv_eqd_enc_t *this)
{
    this->i32Q10Cnt2PosGain = ((0xffffffffU/(4*(1*this->ui16PulseNumber)))*1024); // #define M1_QDC_LINE_RECIPROCAL_4_POS_GEN
    this->f32SpeedCalConst = (frac32_t)((60.0*this->ui32QDTimerFrequency/(this->ui16Pp*(4*this->ui16PulseNumber)*g_fltM1speedScale)) * 134217728); // #define M1_SPEED_CAL_CONST
    this->fltSpeedFrac16ToAngularCoeff = (float_t)(2*PI*g_fltM1speedScale*this->ui16Pp/60.0); // #define M1_SPEED_FRAC_TO_ANGULAR_COEFF
}
