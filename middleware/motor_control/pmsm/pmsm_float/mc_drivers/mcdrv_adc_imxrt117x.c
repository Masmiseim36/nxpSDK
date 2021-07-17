/*
 * Copyright 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_adc_imxrt117x.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

lpadc_conv_command_config_t lpadcCommandConfig1;
lpadc_conv_trigger_config_t lpadcTriggerConfig1;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Reads and calculates 3 phase samples based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_Curr3Ph2ShGet(mcdrv_adc_t *this)
{
    GMCLIB_3COOR_T_F16 sIABCtemp;

    switch (*this->pui16SVMSector)
    {
        case 2:
        case 3:
            /* direct sensing of phase A and C, calculation of B */
            sIABCtemp.f16A = MLIB_ShLSat_F16(((frac16_t)((int16_t)(ADC_ETC_GetADCConversionValue(ADC_ETC, 0U, 0U)*5/2) << 2U) - this->sCurrSec23.ui16OffsetPhaA), 1);
            sIABCtemp.f16C = MLIB_ShLSat_F16(((frac16_t)((int16_t)(ADC_ETC_GetADCConversionValue(ADC_ETC, 4U, 0U)*5/2) << 2U) - this->sCurrSec23.ui16OffsetPhaC), 1);
            sIABCtemp.f16B = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16C));
            break;
        case 4:
        case 5:
            /* direct sensing of phase A and B, calculation of C */
            sIABCtemp.f16A = MLIB_ShLSat_F16(((frac16_t)((int16_t)(ADC_ETC_GetADCConversionValue(ADC_ETC, 0U, 0U)*5/2) << 2U) - this->sCurrSec45.ui16OffsetPhaA), 1);
            sIABCtemp.f16B = MLIB_ShLSat_F16(((frac16_t)((int16_t)(ADC_ETC_GetADCConversionValue(ADC_ETC, 4U, 0U)*5/2) << 2U) - this->sCurrSec45.ui16OffsetPhaB), 1);
            sIABCtemp.f16C = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16B));
            break;
        case 1:
        case 6:
        default:
            /* direct sensing of phase B and C, calculation of A */
            sIABCtemp.f16B = MLIB_ShLSat_F16(((frac16_t)((int16_t)(ADC_ETC_GetADCConversionValue(ADC_ETC, 0U, 0U)*5/2) << 2U) - this->sCurrSec16.ui16OffsetPhaB), 1);
            sIABCtemp.f16C = MLIB_ShLSat_F16(((frac16_t)((int16_t)(ADC_ETC_GetADCConversionValue(ADC_ETC, 4U, 0U)*5/2) << 2U) - this->sCurrSec16.ui16OffsetPhaC), 1);
            sIABCtemp.f16A = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16B, sIABCtemp.f16C));
            break;
    }
    
    /* pass measured phase currents to the main module structure */
    this->psIABC->f16A = sIABCtemp.f16A;
    this->psIABC->f16B = sIABCtemp.f16B;
    this->psIABC->f16C = sIABCtemp.f16C;

}

/*!
 * @brief Set initial channel assignment for phase currents & DCB voltage
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_Curr3Ph2ShChanAssignInit(mcdrv_adc_t *this)
{

}

/*!
 * @brief Set new channel assignment for next sampling based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_Curr3Ph2ShChanAssign(mcdrv_adc_t *this)
{
    
    // M1_I_A - GPIO_AD_10 - ADC1 CH2-A
    // M1_I_B - GPIO_AD_12 - ADC1 CH3-A; ADC2 CH3-A
    // M1_I_C - GPIO_AD_13 - ADC1 CH3-B; ADC2 CH3-B
    
    switch (*this->pui16SVMSector)
    {
        /* direct sensing of phases A and C */
        case 2:
        case 3:
            // M1_I_A - GPIO_AD_10 - ADC1 CH2-A
            // M1_I_C - GPIO_AD_13 - ADC2 CH3-B
          
            /* HCx update with ADC channel - phase current A */
            lpadcCommandConfig1.channelNumber = 2U;
            lpadcCommandConfig1.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;  
            LPADC_SetConvCommandConfig(LPADC1, 1U, &lpadcCommandConfig1); // CMDL[number]   
            LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig1);
            lpadcTriggerConfig1.targetCommandId = 1U; //CMDL
            lpadcTriggerConfig1.enableHardwareTrigger = true;
            LPADC_SetConvTriggerConfig(LPADC1, 0U, &lpadcTriggerConfig1); // trigger from TCTRL[number]
            
            /* HCx update with ADC channel - phase current C */
            lpadcCommandConfig1.channelNumber = 3U;
            lpadcCommandConfig1.sampleChannelMode = kLPADC_SampleChannelSingleEndSideB;  
            LPADC_SetConvCommandConfig(LPADC2, 1U, &lpadcCommandConfig1); // CMDL[number]   
            LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig1);
            lpadcTriggerConfig1.targetCommandId = 1U; //CMDL
            lpadcTriggerConfig1.enableHardwareTrigger = true;
            LPADC_SetConvTriggerConfig(LPADC2, 0U, &lpadcTriggerConfig1); // trigger from TCTRL[number]

            break;

        /* direct sensing of phases A and B  */
        case 4:
        case 5:
            // M1_I_A - GPIO_AD_10 - ADC1 CH2-A
            // M1_I_B - GPIO_AD_12 - ADC2 CH3-A
          
            /* HCx update with ADC channel - phase current A */
            lpadcCommandConfig1.channelNumber = 2U;
            lpadcCommandConfig1.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;  
            LPADC_SetConvCommandConfig(LPADC1, 1U, &lpadcCommandConfig1); // CMDL[number]   
            LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig1);
            lpadcTriggerConfig1.targetCommandId = 1U; //CMDL
            lpadcTriggerConfig1.enableHardwareTrigger = true;
            LPADC_SetConvTriggerConfig(LPADC1, 0U, &lpadcTriggerConfig1); // trigger from TCTRL[number]
            
            /* HCx update with ADC channel - phase current B */
            lpadcCommandConfig1.channelNumber = 3U;
            lpadcCommandConfig1.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;  
            LPADC_SetConvCommandConfig(LPADC2, 1U, &lpadcCommandConfig1); // CMDL[number]   
            LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig1);
            lpadcTriggerConfig1.targetCommandId = 1U; //CMDL
            lpadcTriggerConfig1.enableHardwareTrigger = true;
            LPADC_SetConvTriggerConfig(LPADC2, 0U, &lpadcTriggerConfig1); // trigger from TCTRL[number]

            break;

        /* direct sensing of phases B and C */
        case 1:
        case 6:
        default:
            // M1_I_B - GPIO_AD_12 - ADC1 CH3-A
            // M1_I_C - GPIO_AD_13 - ADC2 CH3-B
          
            /* HCx update with ADC channel - phase current B */
            lpadcCommandConfig1.channelNumber = 3U;
            lpadcCommandConfig1.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;  
            LPADC_SetConvCommandConfig(LPADC1, 1U, &lpadcCommandConfig1); // CMDL[number]   
            LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig1);
            lpadcTriggerConfig1.targetCommandId = 1U; //CMDL
            lpadcTriggerConfig1.enableHardwareTrigger = true;
            LPADC_SetConvTriggerConfig(LPADC1, 0U, &lpadcTriggerConfig1); // trigger from TCTRL[number]
            
            /* HCx update with ADC channel - phase current C */
            lpadcCommandConfig1.channelNumber = 3U;
            lpadcCommandConfig1.sampleChannelMode = kLPADC_SampleChannelSingleEndSideB;  
            LPADC_SetConvCommandConfig(LPADC2, 1U, &lpadcCommandConfig1); // CMDL[number]   
            LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig1);
            lpadcTriggerConfig1.targetCommandId = 1U; //CMDL
            lpadcTriggerConfig1.enableHardwareTrigger = true;
            LPADC_SetConvTriggerConfig(LPADC2, 0U, &lpadcTriggerConfig1); // trigger from TCTRL[number]

            break;
    }
    
}

/*!
 * @brief Initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_Curr3Ph2ShCalibInit(mcdrv_adc_t *this)
{

    /* clear offset values */
    this->sCurrSec16.ui16OffsetPhaB = 0;
    this->sCurrSec16.ui16OffsetPhaC = 0;
    this->sCurrSec23.ui16OffsetPhaA = 0;
    this->sCurrSec23.ui16OffsetPhaC = 0;
    this->sCurrSec45.ui16OffsetPhaA = 0;
    this->sCurrSec45.ui16OffsetPhaB = 0;

    this->sCurrSec16.ui16CalibPhaB = 0;
    this->sCurrSec16.ui16CalibPhaC = 0;
    this->sCurrSec23.ui16CalibPhaA = 0;
    this->sCurrSec23.ui16CalibPhaC = 0;
    this->sCurrSec45.ui16CalibPhaA = 0;
    this->sCurrSec45.ui16CalibPhaB = 0;

    /* initialize offset filters */
    this->sCurrSec16.ui16FiltPhaB.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec16.ui16FiltPhaC.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec23.ui16FiltPhaA.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec23.ui16FiltPhaC.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec45.ui16FiltPhaA.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec45.ui16FiltPhaB.u16Sh = this->ui16OffsetFiltWindow;

    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec16.ui16FiltPhaB);
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec16.ui16FiltPhaC);
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec23.ui16FiltPhaA);
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec23.ui16FiltPhaC);
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec45.ui16FiltPhaA);
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec45.ui16FiltPhaB);

}

/*!
 * @brief Function reads current samples and filter them based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_Curr3Ph2ShCalib(mcdrv_adc_t *this)
{

    switch (*this->pui16SVMSector)
    {
        case 2:
        case 3:
            /* sensing of offset IA -> ADCA and IC -> ADCC */
            this->sCurrSec23.ui16CalibPhaA = GDFLIB_FilterMA_F16((frac16_t)((ADC_ETC_GetADCConversionValue(ADC_ETC, 0U, 0U)*5/2) << 2), &this->sCurrSec23.ui16FiltPhaA);
            this->sCurrSec23.ui16CalibPhaC = GDFLIB_FilterMA_F16((frac16_t)((ADC_ETC_GetADCConversionValue(ADC_ETC, 4U, 0U)*5/2) << 2), &this->sCurrSec23.ui16FiltPhaC);
            break;
        case 4:
        case 5:
            /* sensing of offset IA -> ADCA and IB -> ADCC */
            this->sCurrSec45.ui16CalibPhaA = GDFLIB_FilterMA_F16((frac16_t)((ADC_ETC_GetADCConversionValue(ADC_ETC, 0U, 0U)*5/2) << 2), &this->sCurrSec45.ui16FiltPhaA);
            this->sCurrSec45.ui16CalibPhaB = GDFLIB_FilterMA_F16((frac16_t)((ADC_ETC_GetADCConversionValue(ADC_ETC, 4U, 0U)*5/2) << 2), &this->sCurrSec45.ui16FiltPhaB);
            break;
        case 1:
        case 6:
        default:
            /* sensing of offset IB -> ADCA and IC -> ADCC */
            this->sCurrSec16.ui16CalibPhaB = GDFLIB_FilterMA_F16((frac16_t)((ADC_ETC_GetADCConversionValue(ADC_ETC, 0U, 0U)*5/2) << 2), &this->sCurrSec16.ui16FiltPhaB);
            this->sCurrSec16.ui16CalibPhaC = GDFLIB_FilterMA_F16((frac16_t)((ADC_ETC_GetADCConversionValue(ADC_ETC, 4U, 0U)*5/2) << 2), &this->sCurrSec16.ui16FiltPhaC);
            break;
    }

}

/*!
 * @brief Function passes measured offset values to main structure
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_Curr3Ph2ShCalibSet(mcdrv_adc_t *this)
{

    /* pass calibration data for sector 1 and 6 */
    this->sCurrSec16.ui16OffsetPhaB = this->sCurrSec16.ui16CalibPhaB;
    this->sCurrSec16.ui16OffsetPhaC = this->sCurrSec16.ui16CalibPhaC;

    /* pass calibration data for sector 2 and 3 */
    this->sCurrSec23.ui16OffsetPhaA = this->sCurrSec23.ui16CalibPhaA;
    this->sCurrSec23.ui16OffsetPhaC = this->sCurrSec23.ui16CalibPhaC;

    /* pass calibration data for sector 4 and 5 */
    this->sCurrSec45.ui16OffsetPhaA = this->sCurrSec45.ui16CalibPhaA;
    this->sCurrSec45.ui16OffsetPhaB = this->sCurrSec45.ui16CalibPhaB;

}

/*!
 * @brief Function reads and passes DCB voltage sample
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_VoltDcBusGet(mcdrv_adc_t *this)
{

    /* read DC-bus voltage sample from defined ADCx result register */
    *this->pf16UDcBus = (frac16_t)((ADC_ETC_GetADCConversionValue(ADC_ETC, 0U, 1U)*5/2) << 2); //ADC_ETC trigger0 (ADC1) chain1 

}

/*!
 * @brief Function reads and passes auxiliary sample
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_AuxValGet(mcdrv_adc_t *this)
{

    /* read Auxiliary channel sample from defined ADCx result register */
    *this->pui16AuxChan = ADC_ETC_GetADCConversionValue(ADC_ETC, 4U, 1U); //ADC_ETC trigger4 (ADC2) chain1 
    
}
