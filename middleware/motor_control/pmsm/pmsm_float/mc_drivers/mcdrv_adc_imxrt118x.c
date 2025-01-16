/*
 * Copyright 2013 - 2015, Freescale Semiconductor, Inc.
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

#include "mcdrv_adc_imxrt118x.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Reads and calculates 3 phase samples based on SVM sector.

          To allow measuring analog inputs higher than the selected voltage 
          reference (1.8V), the ADC input is scaled by HW (Scale factor 1/2). 
          Therefore we need to add an extra scale to reach the right measured data. 

          Reference voltage high:                 VREFH = 1.8V
          Voltage reference low:                  VREFL = 0V
          Scale factor:                           FACTOR = 1/2
          Max positive input voltage on ADC pin:  VIN_MAX = (VREFH-VREFL)/FACTOR = (1.8V*2) = 3.6V
          Max input voltage on HW application:    VIN_HW = 3.3V
          Extra scale:                            SCALE = VIN_HW/VIN_MAX = 3.3V/3.6V= 11/12

          ADC result than will be: (ADC_RSLT << 1) / (SCALE) = (ADC_RSLT << 1)*12/11


          If the reference voltage on the board does not precise value (1.8V), 
          we need to compensate difference between the reference voltage VREFH 
          and the real value of reference voltage VREFHreal, for the correct 
          ADC result. This step is needed for highly precise motor control drives.
          
          Reference voltage high                  VREFH
          Real reference voltage high             VREFHreal
          Conpensation                            VREF_COMPENSATION = VREFHreal/VREFH
          ADC result then will be:                ((ADC_RSLT << 1)/(SCALE)) * (1/VREF_COMPENSATION)
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_CurrAndVoltDcBusGet(mcdrv_adc_t *this)
{
    GMCLIB_3COOR_T_F16 sIABCtemp;

    /* ADC1 */
    /* FIFO_0 - conversion 1 - I_A */
    LPADC_GetConvResult(ADC1, &this->s_ADC_ResultStructure, 0U);
    this->ui16AdcCurrA = ((int16_t)((this->s_ADC_ResultStructure.convValue)*12/11));
    /* FIFO_1 - conversion 1 - I_B */
    LPADC_GetConvResult(ADC1, &this->s_ADC_ResultStructure, 1U);
    this->ui16AdcCurrB = ((int16_t)((this->s_ADC_ResultStructure.convValue)*12/11));
    /* FIFO_0 - conversion 2 */
    LPADC_GetConvResult(ADC1, &this->s_ADC_ResultStructure, 0U);  
    /* FIFO_1 - conversion 2 - UDCBus */
    LPADC_GetConvResult(ADC1, &this->s_ADC_ResultStructure, 1U);
    this->ui16AdcDCBVolt = ((int16_t)((this->s_ADC_ResultStructure.convValue)*12/11));
    *this->pf16UDcBus    = (frac16_t)(this->ui16AdcDCBVolt);

    /* ADC2 */
    /* FIFO_0 - conversion 1 - I_C */
    LPADC_GetConvResult(ADC2, &this->s_ADC_ResultStructure, 0U);
    this->ui16AdcCurrC = ((int16_t)((this->s_ADC_ResultStructure.convValue)*12/11));
    /* FIFO_1 - conversion 2 */
    LPADC_GetConvResult(ADC2, &this->s_ADC_ResultStructure, 1U);

    switch (*this->pui16SVMSector)
    {
        case 2:
        case 3:
            /* direct sensing of phase A and C, calculation of B */
            /* Read ADC result FIFO0 */
            sIABCtemp.f16A = (((frac16_t)(this->ui16AdcCurrA)) - this->sCurrSec23.ui16OffsetPhaA) << 1U;
            sIABCtemp.f16C = (((frac16_t)(this->ui16AdcCurrC)) - this->sCurrSec23.ui16OffsetPhaC) << 1U;
            sIABCtemp.f16B = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16C));

            break;

        case 4:
        case 5:
            /* direct sensing of phase A and B, calculation of C */
            sIABCtemp.f16A = (((frac16_t)(this->ui16AdcCurrA)) - this->sCurrSec45.ui16OffsetPhaA) << 1U;
            sIABCtemp.f16B = (((frac16_t)(this->ui16AdcCurrB)) - this->sCurrSec45.ui16OffsetPhaB) << 1U;
            sIABCtemp.f16C = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16B));

            break;

        case 1:
        case 6:
        default:
            /* direct sensing of phase B and C, calculation of A */
            sIABCtemp.f16B = (((frac16_t)(this->ui16AdcCurrB)) - this->sCurrSec16.ui16OffsetPhaB) << 1U;
            sIABCtemp.f16C = (((frac16_t)(this->ui16AdcCurrC)) - this->sCurrSec16.ui16OffsetPhaC) << 1U;
            sIABCtemp.f16A = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16B, sIABCtemp.f16C));

            break;
    }

    /* pass measured phase currents to the main module structure */
    this->psIABC->f16A = sIABCtemp.f16A;
    this->psIABC->f16B = sIABCtemp.f16B;
    this->psIABC->f16C = sIABCtemp.f16C;

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
    this->sCurrSec16.ui16OffsetPhaB = 0U;
    this->sCurrSec16.ui16OffsetPhaC = 0U;
    this->sCurrSec23.ui16OffsetPhaA = 0U;
    this->sCurrSec23.ui16OffsetPhaC = 0U;
    this->sCurrSec45.ui16OffsetPhaA = 0U;
    this->sCurrSec45.ui16OffsetPhaB = 0U;

    this->sCurrSec16.ui16CalibPhaB = 0U;
    this->sCurrSec16.ui16CalibPhaC = 0U;
    this->sCurrSec23.ui16CalibPhaA = 0U;
    this->sCurrSec23.ui16CalibPhaC = 0U;
    this->sCurrSec45.ui16CalibPhaA = 0U;
    this->sCurrSec45.ui16CalibPhaB = 0U;

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

            /* sensing of offset IA, IC*/

            this->sCurrSec23.ui16CalibPhaA =
                GDFLIB_FilterMA_F16((frac16_t)(this->ui16AdcCurrA), &this->sCurrSec23.ui16FiltPhaA);
            this->sCurrSec23.ui16CalibPhaC =
                GDFLIB_FilterMA_F16((frac16_t)(this->ui16AdcCurrC), &this->sCurrSec23.ui16FiltPhaC);
            break;

        case 4:
        case 5:
            /* sensing of offset IA, IB*/
            this->sCurrSec45.ui16CalibPhaA =
                GDFLIB_FilterMA_F16((frac16_t)(this->ui16AdcCurrA), &this->sCurrSec45.ui16FiltPhaA);
            this->sCurrSec45.ui16CalibPhaB =
                GDFLIB_FilterMA_F16((frac16_t)(this->ui16AdcCurrB), &this->sCurrSec45.ui16FiltPhaB);

            break;

        case 1:
        case 6:

        default:
            /* sensing of offset IB, IC*/
            this->sCurrSec16.ui16CalibPhaB =
                GDFLIB_FilterMA_F16((frac16_t)(this->ui16AdcCurrB), &this->sCurrSec16.ui16FiltPhaB);
            this->sCurrSec16.ui16CalibPhaC =
                GDFLIB_FilterMA_F16((frac16_t)(this->ui16AdcCurrC), &this->sCurrSec16.ui16FiltPhaC);

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
