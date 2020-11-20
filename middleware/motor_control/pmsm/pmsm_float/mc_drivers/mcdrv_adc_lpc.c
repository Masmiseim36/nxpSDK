/*
 * Copyright 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_adc_lpc.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static bool_t s_statusPass;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Reads and calculates 3 phase samples based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrAndVoltDcBusGet(mcdrv_adc_t *this)
{
    GMCLIB_3COOR_T_F16 sIABCtemp;

    s_statusPass = TRUE;

    // A and B are measured simultaneous
    /* P19_4   - FIFO_0-1 - I_A */
    LPADC_GetConvResult(ADC0, &this->s_ADC_ResultStructure, 0U);
    this->ui16AdcCurrA = (this->s_ADC_ResultStructure.convValue << 1U);

    /* P18-11  - FIFO_0-2 - I_C */
    LPADC_GetConvResult(ADC0, &this->s_ADC_ResultStructure, 0U);
    this->ui16AdcCurrC = (this->s_ADC_ResultStructure.convValue << 1U);

    /* P17-19  - FIFO_0-3 - UDCBus */
    LPADC_GetConvResult(ADC0, &this->s_ADC_ResultStructure, 0U);
    this->ui16AdcDCBVolt = (this->s_ADC_ResultStructure.convValue);
    *this->pf16UDcBus    = (frac16_t)(this->ui16AdcDCBVolt);

    /* P19-2   - FIFO_1-1 - I_B */
    LPADC_GetConvResult(ADC0, &this->s_ADC_ResultStructure, 1U);
    this->ui16AdcCurrB = (this->s_ADC_ResultStructure.convValue << 1U);

    switch (*this->pui16SVMSector)
    {
        case 2:
        case 3:
            /* direct sensing of phase A and C, calculation of B */
            /* Read ADC result FIFO0 */
            sIABCtemp.f16A = (((frac16_t)(this->ui16AdcCurrA)) - this->sCurrSec23.ui16OffsetPhaA);
            sIABCtemp.f16C = (((frac16_t)(this->ui16AdcCurrC)) - this->sCurrSec23.ui16OffsetPhaC);
            sIABCtemp.f16B = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16C));

            break;

        case 4:
        case 5:
            /* direct sensing of phase A and B, calculation of C */
            sIABCtemp.f16A = (((frac16_t)(this->ui16AdcCurrA)) - this->sCurrSec45.ui16OffsetPhaA);
            sIABCtemp.f16B = (((frac16_t)(this->ui16AdcCurrB)) - this->sCurrSec45.ui16OffsetPhaB);
            sIABCtemp.f16C = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16B));

            break;

        case 1:
        case 6:
        default:
            /* direct sensing of phase B and C, calculation of A */
            sIABCtemp.f16B = (((frac16_t)(this->ui16AdcCurrB)) - this->sCurrSec16.ui16OffsetPhaB);
            sIABCtemp.f16C = (((frac16_t)(this->ui16AdcCurrC)) - this->sCurrSec16.ui16OffsetPhaC);
            sIABCtemp.f16A = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16B, sIABCtemp.f16C));

            break;
    }

    /* pass measured phase currents to the main module structure */
    this->psIABC->f16A = MLIB_Neg_F16(sIABCtemp.f16A);
    this->psIABC->f16B = MLIB_Neg_F16(sIABCtemp.f16B);
    this->psIABC->f16C = MLIB_Neg_F16(sIABCtemp.f16C);

    return (s_statusPass);
}

/*!
 * @brief Initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalibInit(mcdrv_adc_t *this)
{
    s_statusPass = TRUE;

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

    return (s_statusPass);
}

/*!
 * @brief Function reads current samples and filter them based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalib(mcdrv_adc_t *this)
{
    s_statusPass = TRUE;

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

    return (s_statusPass);
}

/*!
 * @brief Function passes measured offset values to main structure
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalibSet(mcdrv_adc_t *this)
{
    s_statusPass = TRUE;

    /* pass calibration data for sector 1 and 6 */
    this->sCurrSec16.ui16OffsetPhaB = this->sCurrSec16.ui16CalibPhaB;
    this->sCurrSec16.ui16OffsetPhaC = this->sCurrSec16.ui16CalibPhaC;

    /* pass calibration data for sector 2 and 3 */
    this->sCurrSec23.ui16OffsetPhaA = this->sCurrSec23.ui16CalibPhaA;
    this->sCurrSec23.ui16OffsetPhaC = this->sCurrSec23.ui16CalibPhaC;

    /* pass calibration data for sector 4 and 5 */
    this->sCurrSec45.ui16OffsetPhaA = this->sCurrSec45.ui16CalibPhaA;
    this->sCurrSec45.ui16OffsetPhaB = this->sCurrSec45.ui16CalibPhaB;

    return (s_statusPass);
}
