/*
 * Copyright 2013 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2022, 2024 NXP
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

#include "mcdrv_adcetc_imxrt11xx.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

lpadc_conv_command_config_t lpadcCommandConfig1;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Function set new channel assignment for next BEMF voltage sensing
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_AssignBemfChannel(mcdrv_adcetc_t *this)
{
  
  /* Assign channel number and side of BEMF for ADC*/
  lpadcCommandConfig1.channelNumber = this->bldcAdcTable.ui16BemfChannelNumber[this->ui16Sector];
  lpadcCommandConfig1.sampleChannelMode = this->bldcAdcTable.ui16BemfChannelMode[this->ui16Sector];
  LPADC_SetConvCommandConfig(LPADC1, 1U, &lpadcCommandConfig1); 
  
}

/*!
 * @brief Function reads ADC result register containing actual BEMF voltage
 *
 * Result register value stored to BEMF voltage pointer
 * 
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_BemfVoltageGet(mcdrv_adcetc_t *this)
{
  
  /* read Phase BEMF channel sample from defined ADC1 result register */
  *this->pf16BemfVoltage = (frac16_t)(MLIB_ShLSat_F16((frac16_t)((ADC_ETC_TRIGn_RESULT_1_0_DATA0_MASK & (ADC_ETC->TRIG[0U].TRIGn_RESULT_1_0))*64/55), 3U));
  
}

/*!
 * @brief Function reads ADC result register containing actual DC-bus current sample
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_CurrDcBusGet(mcdrv_adcetc_t *this)
{
  
  /* read DC-bus current channel sample from defined ADC2 result register */
  *this->pf16IDcBus = (MLIB_ShLSat_F16((frac16_t)((uint16_t)((ADC_ETC_TRIGn_RESULT_1_0_DATA0_MASK & (ADC_ETC->TRIG[4U].TRIGn_RESULT_1_0))*64/55)), 3U) - this->ui16OffsetDcCurr);
  
}

/*!
 * @brief Function reads and passes DCB voltage sample
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_VoltDcBusGet(mcdrv_adcetc_t *this)
{
  
    /* read DC-bus voltage sample from defined ADC1 result register */
    *this->pf16UDcBus = (frac16_t)(((ADC_ETC->TRIG[0U].TRIGn_RESULT_1_0) >> 13U) *64/55);
    
}

/*!
 * @brief Function reads and passes auxiliary sample
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_AuxValGet(mcdrv_adcetc_t *this)
{
  
    /* read Auxiliary channel sample from defined ADC2 result register */
    *this->pui16AuxChan = ((ADC_ETC->TRIG[4U].TRIGn_RESULT_1_0) >> ADC_ETC_TRIGn_RESULT_1_0_DATA1_SHIFT);
    
}

/*!
 * @brief Function initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_CurrOffsetCalibInit(mcdrv_adcetc_t *this)
{   
  
    /* clear offset values */
    this->ui16OffsetDcCurr = 0U;
    this->ui16CalibDcCurr  = 0U;

    /* initialize offset filters */
    this->ui16FiltDcCurr.u16Sh = this->ui16OffsetFiltWindow;

    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->ui16FiltDcCurr);
    
}

/*!
 * @brief Function reads current samples and filter them
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_CurrOffsetCalib(mcdrv_adcetc_t *this)
{
  
    /* sensing of DC Bus Current offset */
    this->ui16CalibDcCurr = GDFLIB_FilterMA_F16((MLIB_ShLSat_F16((frac16_t)((uint16_t)((ADC_ETC_TRIGn_RESULT_1_0_DATA0_MASK & (ADC_ETC->TRIG[4U].TRIGn_RESULT_1_0))*64/55)), 3U)), &this->ui16FiltDcCurr);
    
}

/*!
 * @brief Function passes measured offset values to main structure
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_CurrOffsetCalibSet(mcdrv_adcetc_t *this)
{
  
    /* pass Calib data for DC Bus current offset */
    this->ui16OffsetDcCurr = (this->ui16CalibDcCurr);
    
}
