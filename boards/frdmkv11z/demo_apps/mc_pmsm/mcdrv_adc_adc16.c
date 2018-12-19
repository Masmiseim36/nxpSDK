/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_adc_adc16.h"

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
 * @brief Function reads and calculates 3 phase samples based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShGet(mcdrv_adc16_t *this)
{
    GMCLIB_3COOR_T_F16 sIABCtemp;
    s_statusPass = TRUE;

    switch (*this->pui16SVMSector)
    {
        case 2:
        case 3:
            // direct sensing of A, C, calculation of B
            sIABCtemp.f16A =
                (frac16_t)(MLIB_ShLSat_F16((this->sCurrSec23.pui32AdcBasePhaA->R[0] - this->sCurrSec23.ui16OffsetPhaA), 4));
            sIABCtemp.f16C =
                (frac16_t)(MLIB_ShLSat_F16((this->sCurrSec23.pui32AdcBasePhaC->R[0] - this->sCurrSec23.ui16OffsetPhaC), 4));
            sIABCtemp.f16B = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16C));
            break;

        case 4:
        case 5:
            // direct sensing of A, B, calculation of C
            sIABCtemp.f16A =
                (frac16_t)(MLIB_ShLSat_F16((this->sCurrSec45.pui32AdcBasePhaA->R[0] - this->sCurrSec45.ui16OffsetPhaA), 4));
            sIABCtemp.f16B =
                (frac16_t)(MLIB_ShLSat_F16((this->sCurrSec45.pui32AdcBasePhaB->R[0] - this->sCurrSec45.ui16OffsetPhaB), 4));
            sIABCtemp.f16C = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16B));
            break;

        case 1:
        case 6:
        default:
            // direct sensing of B, C, calculation of A
            sIABCtemp.f16B =
                (frac16_t)(MLIB_ShLSat_F16((this->sCurrSec16.pui32AdcBasePhaB->R[0] - this->sCurrSec16.ui16OffsetPhaB), 4));
            sIABCtemp.f16C =
                (frac16_t)(MLIB_ShLSat_F16((this->sCurrSec16.pui32AdcBasePhaC->R[0] - this->sCurrSec16.ui16OffsetPhaC), 4));
            sIABCtemp.f16A = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16B, sIABCtemp.f16C));
            break;
    }

    /* pass measured phase currents to the main module structure */
    this->psIABC->f16A = sIABCtemp.f16A;
    this->psIABC->f16B = sIABCtemp.f16B;
    this->psIABC->f16C = sIABCtemp.f16C;

    return (s_statusPass);
}

/*!
 * @brief Set initial channel assignment for phase currents & DCB voltage
 *
 * @param this   Pointer to the current object
 *
 * @return bool_t true on success
 */
bool_t MCDRV_Curr3Ph2ShChanAssignInit(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    /* update ADC_SC1n registers with initial values */
    this->sCurrSec16.pui32AdcBasePhaB->SC1[0] =
        ((this->sCurrSec16.pui32AdcBasePhaB->SC1[0] & ~(uint16_t)ADC_SC1_ADCH_MASK) | 
          this->sCurrSec16.ui16ChanNumPhaB);
    this->sCurrSec16.pui32AdcBasePhaC->SC1[0] =
        ((this->sCurrSec16.pui32AdcBasePhaC->SC1[0] & ~(uint16_t)ADC_SC1_ADCH_MASK) | 
           this->sCurrSec16.ui16ChanNumPhaC);

    /* Vdcb sampling - select SC1[1] (sample number 1) */
    this->pui32UdcbAdcBase->SC1[1] = 
        ((this->pui32UdcbAdcBase->SC1[1] & ~(uint16_t)ADC_SC1_ADCH_MASK) | 
          this->ui16ChanNumVDcb);
    
    /* Aux sampling - select SC1[1] (sample number 1) */
    this->pui32AuxAdcBase->SC1[1] = 
        ((this->pui32AuxAdcBase->SC1[1] & ~(uint16_t)ADC_SC1_ADCH_MASK) | 
          this->ui16ChanNumAux);

    return (s_statusPass);
}


/*!
 * @brief Function set new channel assignment for next sampling based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShChanAssign(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    switch (*this->pui16SVMSector)
    {
        case 2:
        case 3: /* currents Ia, Ic will be measured */
            /* update ADC_SC1n registers based on actual SVM sector */
            this->sCurrSec23.pui32AdcBasePhaA->SC1[0] =
                ((this->sCurrSec23.pui32AdcBasePhaA->SC1[0] & ~(uint16_t)ADC_SC1_ADCH_MASK) | this->sCurrSec23.ui16ChanNumPhaA);
            this->sCurrSec23.pui32AdcBasePhaC->SC1[0] =
                ((this->sCurrSec23.pui32AdcBasePhaC->SC1[0] & ~(uint16_t)ADC_SC1_ADCH_MASK) | this->sCurrSec23.ui16ChanNumPhaC);
            break;
        case 4:
        case 5: /*  currents Ia, Ib will be measured */
            /* update ADC_SC1n registers based on actual SVM sector */
            this->sCurrSec45.pui32AdcBasePhaA->SC1[0] =
                ((this->sCurrSec45.pui32AdcBasePhaA->SC1[0] & ~(uint16_t)ADC_SC1_ADCH_MASK) | this->sCurrSec45.ui16ChanNumPhaA);
            this->sCurrSec45.pui32AdcBasePhaB->SC1[0] =
                ((this->sCurrSec45.pui32AdcBasePhaB->SC1[0] & ~(uint16_t)ADC_SC1_ADCH_MASK) | this->sCurrSec45.ui16ChanNumPhaB);
            break;
        case 1:
        case 6: /* currents Ib, Ic will be measured */
        default:
            /* update ADC_SC1n registers based on actual SVM sector */
            this->sCurrSec16.pui32AdcBasePhaB->SC1[0] =
                ((this->sCurrSec16.pui32AdcBasePhaB->SC1[0] & ~(uint16_t)ADC_SC1_ADCH_MASK) | this->sCurrSec16.ui16ChanNumPhaB);
            this->sCurrSec16.pui32AdcBasePhaC->SC1[0] =
                ((this->sCurrSec16.pui32AdcBasePhaC->SC1[0] & ~(uint16_t)ADC_SC1_ADCH_MASK) | this->sCurrSec16.ui16ChanNumPhaC);
            break;
    }
    return (s_statusPass);
}

/*!
 * @brief Function initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalibInit(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

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

    return (s_statusPass);
}

/*!
 * @brief Function reads current samples and filter them based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalib(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    switch (*this->pui16SVMSector)
    {
        case 2:
        case 3:
            /* sensing of offset IA -> MCDRV_ADC1_A and IC -> MCDRV_ADC0_A */
            this->sCurrSec23.ui16CalibPhaA =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec23.pui32AdcBasePhaA->R[0]), &this->sCurrSec23.ui16FiltPhaA);

            this->sCurrSec23.ui16CalibPhaC =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec23.pui32AdcBasePhaC->R[0]), &this->sCurrSec23.ui16FiltPhaC);

            break;

        case 4:
        case 5:
            /* sensing of offset IA -> MCDRV_ADC1_A and IB -> MCDRV_ADC0_A */
            this->sCurrSec45.ui16CalibPhaA =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec45.pui32AdcBasePhaA->R[0]), &this->sCurrSec45.ui16FiltPhaA);

            this->sCurrSec45.ui16CalibPhaB =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec45.pui32AdcBasePhaB->R[0]), &this->sCurrSec45.ui16FiltPhaB);
            break;

        case 1:
        case 6:
        default:
            /* sensing of offset IB -> ADC_A and IC -> ADC_B */
            this->sCurrSec16.ui16CalibPhaB =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec16.pui32AdcBasePhaB->R[0]), &this->sCurrSec16.ui16FiltPhaB);

            this->sCurrSec16.ui16CalibPhaC =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec16.pui32AdcBasePhaC->R[0]), &this->sCurrSec16.ui16FiltPhaC);
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
bool_t MCDRV_Curr3Ph2ShCalibSet(mcdrv_adc16_t *this)
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

/*!
 * @brief Function reads and passes DCB voltage sample
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_VoltDcBusGet(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    /* read DC-bus voltage sample from defined ADCx result register */
    *this->pf16UDcBus = (frac16_t)(MLIB_ShLSat_F16((this->pui32UdcbAdcBase->R[1]), 3));

    return (s_statusPass);
}

/*!
 * @brief Function reads and passes auxiliary sample
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AuxValGet(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    /* read Auxiliary channel sample from defined ADCx result register */
    *this->pui16AuxChan = (frac16_t)(MLIB_ShLSat_F16((this->pui32AuxAdcBase->R[1]), 3));

    return (s_statusPass);
}

