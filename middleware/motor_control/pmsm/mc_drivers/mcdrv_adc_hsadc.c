/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_adc_hsadc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
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
bool_t MCDRV_Curr3Ph2ShGet(mcdrv_hsadc_t *this)
{
    GMCLIB_3COOR_T_F16 sIABCtemp;

    s_statusPass = TRUE;

    switch (*this->pui16SVMSector)
    {
        case 2:
        case 3:
            /* direct sensing of phase A and C, calculation of B */
            sIABCtemp.f16A = MLIB_ShLSat_F16(
                ((frac16_t)((int16_t) this->sCurrSec23.pui32AdcBasePhaA->RSLT[0]) - this->sCurrSec23.ui16OffsetPhaA), 1);
            sIABCtemp.f16C = MLIB_ShLSat_F16(
                ((frac16_t)((int16_t) this->sCurrSec23.pui32AdcBasePhaC->RSLT[0]) - this->sCurrSec23.ui16OffsetPhaC), 1);
            sIABCtemp.f16B = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16C));
            break;

        case 4:
        case 5:
            /* direct sensing of phase A and B, calculation of C */
            sIABCtemp.f16A = MLIB_ShLSat_F16(
                ((frac16_t)((int16_t) this->sCurrSec45.pui32AdcBasePhaA->RSLT[0]) - this->sCurrSec45.ui16OffsetPhaA), 1);
            sIABCtemp.f16B = MLIB_ShLSat_F16(
                ((frac16_t)((int16_t) this->sCurrSec45.pui32AdcBasePhaB->RSLT[0]) - this->sCurrSec45.ui16OffsetPhaB), 1);
            sIABCtemp.f16C = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16B));
            break;

        case 1:
        case 6:
        default:
            /* direct sensing of phase B and C, calculation of A */
            sIABCtemp.f16B = MLIB_ShLSat_F16(
                ((frac16_t)((int16_t) this->sCurrSec16.pui32AdcBasePhaB->RSLT[0]) - this->sCurrSec16.ui16OffsetPhaB), 1);
            sIABCtemp.f16C = MLIB_ShLSat_F16(
                ((frac16_t)((int16_t) this->sCurrSec16.pui32AdcBasePhaC->RSLT[0]) - this->sCurrSec16.ui16OffsetPhaC), 1);
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
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShChanAssignInit(mcdrv_hsadc_t *this)
{
    s_statusPass = TRUE;
   
    /* sector 1&6, direct sensing of phases B and C */
    /* phase current B on ADC1 and phase C on ADC0 */
    this->sCurrSec16.ui16AdcSmplPhaB = HSADC_CLIST1_SAMPLE0(this->sCurrSec16.ui16ChanNumPhaB);
    this->sCurrSec16.ui16AdcSmplPhaC = HSADC_CLIST1_SAMPLE0(this->sCurrSec16.ui16ChanNumPhaC);
    
    /* sector 2&3, direct sensing of phases A and C */
    /* phase current A on ADC1 and phase C on ADC0 */
    this->sCurrSec23.ui16AdcSmplPhaA = HSADC_CLIST1_SAMPLE0(this->sCurrSec23.ui16ChanNumPhaA);
    this->sCurrSec23.ui16AdcSmplPhaC = HSADC_CLIST1_SAMPLE0(this->sCurrSec23.ui16ChanNumPhaC);
    
    /* sector 4&5, direct sensing of phases A and B */
    /* phase current A on ADC0 and phase B on ADC1 */
    this->sCurrSec45.ui16AdcSmplPhaA = HSADC_CLIST1_SAMPLE0(this->sCurrSec45.ui16ChanNumPhaA);
    this->sCurrSec45.ui16AdcSmplPhaB = HSADC_CLIST1_SAMPLE0(this->sCurrSec45.ui16ChanNumPhaB);
   
    /* Vdcb sampling - select CLIST1 (sample number 1) */
    this->ui16AdcSmplVDcb = HSADC_CLIST1_SAMPLE1(this->ui16ChanNumVDcb);
    this->pui32AdcBaseVDcb->CLIST1 = 
        ((this->pui32AdcBaseVDcb->CLIST1 & ~(uint16_t)HSADC_CLIST1_SAMPLE1_MASK) |
          this->ui16AdcSmplVDcb);

    /* default channel assignment for default SVM sector = 2 */
    /* direct sensing of phases A and C */
    this->sCurrSec23.pui32AdcBasePhaA->CLIST1 =
        ((this->sCurrSec23.pui32AdcBasePhaA->CLIST1 & ~(uint16_t)HSADC_CLIST1_SAMPLE0_MASK) |
          this->sCurrSec23.ui16AdcSmplPhaA);
    this->sCurrSec23.pui32AdcBasePhaC->CLIST1 =
        ((this->sCurrSec23.pui32AdcBasePhaC->CLIST1 & ~(uint16_t)HSADC_CLIST1_SAMPLE0_MASK) |
          this->sCurrSec23.ui16AdcSmplPhaC);
    
    return (s_statusPass);
}

/*!
 * @brief Set new channel assignment for next sampling based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShChanAssign(mcdrv_hsadc_t *this)
{
    s_statusPass = TRUE;

    switch (*this->pui16SVMSector)
    {
        case 2:
        case 3:
            /* direct sensing of phases A and C */
            this->sCurrSec23.pui32AdcBasePhaA->CLIST1 =
                ((this->sCurrSec23.pui32AdcBasePhaA->CLIST1 & ~(uint16_t)HSADC_CLIST1_SAMPLE0_MASK) |
                 this->sCurrSec23.ui16AdcSmplPhaA);
            this->sCurrSec23.pui32AdcBasePhaC->CLIST1 =
                ((this->sCurrSec23.pui32AdcBasePhaC->CLIST1 & ~(uint16_t)HSADC_CLIST1_SAMPLE0_MASK) |
                 this->sCurrSec23.ui16AdcSmplPhaC);
            break;

        case 4:
        case 5:
            /* direct sensing of phases A and B  */
            this->sCurrSec45.pui32AdcBasePhaA->CLIST1 =
                ((this->sCurrSec45.pui32AdcBasePhaA->CLIST1 & ~(uint16_t)HSADC_CLIST1_SAMPLE0_MASK) |
                 this->sCurrSec45.ui16AdcSmplPhaA);
            this->sCurrSec45.pui32AdcBasePhaB->CLIST1 =
                ((this->sCurrSec45.pui32AdcBasePhaB->CLIST1 & ~(uint16_t)HSADC_CLIST1_SAMPLE0_MASK) |
                 this->sCurrSec45.ui16AdcSmplPhaB);
            break;

        case 1:
        case 6:
        default:
            /* direct sensing of phases B and C */
            this->sCurrSec16.pui32AdcBasePhaB->CLIST1 =
                ((this->sCurrSec16.pui32AdcBasePhaB->CLIST1 & ~(uint16_t)HSADC_CLIST1_SAMPLE0_MASK) |
                 this->sCurrSec16.ui16AdcSmplPhaB);
            this->sCurrSec16.pui32AdcBasePhaC->CLIST1 =
                ((this->sCurrSec16.pui32AdcBasePhaC->CLIST1 & ~(uint16_t)HSADC_CLIST1_SAMPLE0_MASK) |
                 this->sCurrSec16.ui16AdcSmplPhaC);
            break;
    }

    return (s_statusPass);
}

/*!
 * @brief Initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalibInit(mcdrv_hsadc_t *this)
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
bool_t MCDRV_Curr3Ph2ShCalib(mcdrv_hsadc_t *this)
{
    s_statusPass = TRUE;

    switch (*this->pui16SVMSector)
    {
        case 2:
        case 3:
            /* sensing of offset IA -> HSADCA and IC -> HSADCC */
            this->sCurrSec23.ui16CalibPhaA =
                GDFLIB_FilterMA_F16((frac16_t) this->sCurrSec23.pui32AdcBasePhaA->RSLT[0], &this->sCurrSec23.ui16FiltPhaA);
            this->sCurrSec23.ui16CalibPhaC =
                GDFLIB_FilterMA_F16((frac16_t) this->sCurrSec23.pui32AdcBasePhaC->RSLT[0], &this->sCurrSec23.ui16FiltPhaC);
            break;

        case 4:
        case 5:
            /* sensing of offset IA -> HSADCA and IB -> HSADCC */
            this->sCurrSec45.ui16CalibPhaA =
                GDFLIB_FilterMA_F16((frac16_t) this->sCurrSec45.pui32AdcBasePhaA->RSLT[0], &this->sCurrSec45.ui16FiltPhaA);
            this->sCurrSec45.ui16CalibPhaB =
                GDFLIB_FilterMA_F16((frac16_t) this->sCurrSec45.pui32AdcBasePhaB->RSLT[0], &this->sCurrSec45.ui16FiltPhaB);
            break;

        case 1:
        case 6:
        default:
            /* sensing of offset IB -> HSADCA and IC -> HSADCC */
            this->sCurrSec16.ui16CalibPhaB =
                GDFLIB_FilterMA_F16((frac16_t) this->sCurrSec16.pui32AdcBasePhaB->RSLT[0], &this->sCurrSec16.ui16FiltPhaB);
            this->sCurrSec16.ui16CalibPhaC =
                GDFLIB_FilterMA_F16((frac16_t) this->sCurrSec16.pui32AdcBasePhaC->RSLT[0], &this->sCurrSec16.ui16FiltPhaC);
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
bool_t MCDRV_Curr3Ph2ShCalibSet(mcdrv_hsadc_t *this)
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
bool_t MCDRV_VoltDcBusGet(mcdrv_hsadc_t *this)
{
    s_statusPass = TRUE;

    /* read DC-bus voltage sample from defined ADCx result register */
    *this->pf16UDcBus = (frac16_t)(this->pui32AdcBaseVDcb->RSLT[1]);

    return (s_statusPass);
}

/*!
 * @brief Function reads and passes auxiliary sample
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AuxValGet(mcdrv_hsadc_t *this)
{
    s_statusPass = TRUE;

    /* read Auxiliary channel sample from defined ADCx result register */
    *this->pui16AuxChan = (frac16_t)(this->pui32AdcBaseAux->RSLT[1]);

    return (s_statusPass);
}