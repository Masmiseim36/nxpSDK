/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_adc_adc12.h"

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
bool_t MCDRV_Curr3Ph2ShGet(mcdrv_adc12_t *this)
{
    GMCLIB_3COOR_T_F16 sIABCtemp;

    s_statusPass = TRUE;

    switch (*this->pui16SVMSector)
    {
        case 2:
        case 3:
            /* direct sensing of phase A and C, calculation of B */
            sIABCtemp.f16A =
                MLIB_ShLSat_F16(((frac16_t)((int16_t) this->pui32AdcBase->RSLT[this->sCurrSec23.ui16AdcNumPhaA]) -
                                 this->sCurrSec23.ui16OffsetPhaA),
                                1);
            sIABCtemp.f16C =
                MLIB_ShLSat_F16(((frac16_t)((int16_t) this->pui32AdcBase->RSLT[this->sCurrSec23.ui16AdcNumPhaC]) -
                                 this->sCurrSec23.ui16OffsetPhaC),
                                1);
            sIABCtemp.f16B = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16C));
            break;

        case 4:
        case 5:
            /* direct sensing of phase A and B, calculation of C */
            sIABCtemp.f16A =
                MLIB_ShLSat_F16(((frac16_t)((int16_t) this->pui32AdcBase->RSLT[this->sCurrSec45.ui16AdcNumPhaA]) -
                                 this->sCurrSec45.ui16OffsetPhaA),
                                1);
            sIABCtemp.f16B =
                MLIB_ShLSat_F16(((frac16_t)((int16_t) this->pui32AdcBase->RSLT[this->sCurrSec45.ui16AdcNumPhaB]) -
                                 this->sCurrSec45.ui16OffsetPhaB),
                                1);
            sIABCtemp.f16C = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16B));
            break;

        case 1:
        case 6:
        default:
            /* direct sensing of phase B and C, calculation of A */
            sIABCtemp.f16B =
                MLIB_ShLSat_F16(((frac16_t)((int16_t) this->pui32AdcBase->RSLT[this->sCurrSec16.ui16AdcNumPhaB]) -
                                 this->sCurrSec16.ui16OffsetPhaB),
                                1);
            sIABCtemp.f16C =
                MLIB_ShLSat_F16(((frac16_t)((int16_t) this->pui32AdcBase->RSLT[this->sCurrSec16.ui16AdcNumPhaC]) -
                                 this->sCurrSec16.ui16OffsetPhaC),
                                1);
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
bool_t MCDRV_Curr3Ph2ShChanAssignInit(mcdrv_adc12_t *this)
{
    s_statusPass = TRUE;
   
    /* set pointers to required CLIST registers for samples 0&8
     * ADC0 -> CLIST1, ADC1 -> CLIST3
     */
    /* sector 1&6, direct sensing of phases B and C */
    /* phase current B on ADC0 and phase C on ADC1 */
    if((this->sCurrSec16.ui16AdcNumPhaB == ADC0)&&(this->sCurrSec16.ui16AdcNumPhaC == ADC1))
    {
        this->sCurrSec16.pui16ClistPhaB = (uint16_t *)&this->pui32AdcBase->CLIST1;
        this->sCurrSec16.pui16ClistPhaC = (uint16_t *)&this->pui32AdcBase->CLIST3;
        this->sCurrSec16.ui16AdcSmplPhaB = ADC_CLIST1_SAMPLE0(this->sCurrSec16.ui16ChanNumPhaB);
        this->sCurrSec16.ui16AdcSmplPhaC = ADC_CLIST3_SAMPLE8(this->sCurrSec16.ui16ChanNumPhaC + 8);
        
    }
    /* phase current B on ADC1 and phase C on ADC0 */
    else
    {
        this->sCurrSec16.pui16ClistPhaB = (uint16_t *)&this->pui32AdcBase->CLIST3;
        this->sCurrSec16.pui16ClistPhaC = (uint16_t *)&this->pui32AdcBase->CLIST1;
        this->sCurrSec16.ui16AdcSmplPhaC = ADC_CLIST1_SAMPLE0(this->sCurrSec16.ui16ChanNumPhaC);
        this->sCurrSec16.ui16AdcSmplPhaB = ADC_CLIST3_SAMPLE8(this->sCurrSec16.ui16ChanNumPhaB + 8);
    }
    
    /* sector 2&3, direct sensing of phases A and C */
    /* phase current A on ADC0 and phase C on ADC1 */
    if((this->sCurrSec23.ui16AdcNumPhaA == ADC0)&&(this->sCurrSec23.ui16AdcNumPhaC == ADC1))
    {
        this->sCurrSec23.pui16ClistPhaA = (uint16_t *)&this->pui32AdcBase->CLIST1;
        this->sCurrSec23.pui16ClistPhaC = (uint16_t *)&this->pui32AdcBase->CLIST3;
        this->sCurrSec23.ui16AdcSmplPhaA = ADC_CLIST1_SAMPLE0(this->sCurrSec23.ui16ChanNumPhaA);
        this->sCurrSec23.ui16AdcSmplPhaC = ADC_CLIST3_SAMPLE8(this->sCurrSec23.ui16ChanNumPhaC + 8);
    }
    /* phase current A on ADC1 and phase C on ADC0 */
    else
    {
        this->sCurrSec23.pui16ClistPhaA = (uint16_t *)&this->pui32AdcBase->CLIST3;
        this->sCurrSec23.pui16ClistPhaC = (uint16_t *)&this->pui32AdcBase->CLIST1;
        this->sCurrSec23.ui16AdcSmplPhaC = ADC_CLIST1_SAMPLE0(this->sCurrSec23.ui16ChanNumPhaC);
        this->sCurrSec23.ui16AdcSmplPhaA = ADC_CLIST3_SAMPLE8(this->sCurrSec23.ui16ChanNumPhaA + 8);
    }
    
    /* sector 4&5, direct sensing of phases A and B */
    /* phase current A on ADC0 and phase B on ADC1 */
    if((this->sCurrSec45.ui16AdcNumPhaA == ADC0)&&(this->sCurrSec45.ui16AdcNumPhaB == ADC1))
    {
        this->sCurrSec45.pui16ClistPhaA = (uint16_t *)&this->pui32AdcBase->CLIST1;
        this->sCurrSec45.pui16ClistPhaB = (uint16_t *)&this->pui32AdcBase->CLIST3;
        this->sCurrSec45.ui16AdcSmplPhaA = ADC_CLIST1_SAMPLE0(this->sCurrSec45.ui16ChanNumPhaA);
        this->sCurrSec45.ui16AdcSmplPhaB = ADC_CLIST3_SAMPLE8(this->sCurrSec45.ui16ChanNumPhaB + 8);
    }
    /* phase current A on ADC1 and phase B on ADC0 */
    else
    {
        this->sCurrSec45.pui16ClistPhaA = (uint16_t *)&this->pui32AdcBase->CLIST3;
        this->sCurrSec45.pui16ClistPhaB = (uint16_t *)&this->pui32AdcBase->CLIST1;
        this->sCurrSec45.ui16AdcSmplPhaB = ADC_CLIST1_SAMPLE0(this->sCurrSec45.ui16ChanNumPhaB);
        this->sCurrSec45.ui16AdcSmplPhaA = ADC_CLIST3_SAMPLE8(this->sCurrSec45.ui16ChanNumPhaA + 8);
    }
   
    /* Vdcb sampling - select CLIST1 (sample number 1) or CLIST3 (sample number 9) */
    if(this->ui16AdcNumVDcb == ADC0)
    {    
        this->pui32AdcBase->CLIST1 = ((this->pui32AdcBase->CLIST1 & ~(uint16_t)ADC_CLIST1_SAMPLE1_MASK) | 
                                       ADC_CLIST1_SAMPLE1(this->ui16ChanNumVDcb));
        this->ui16AdcSmplVDcb = 1;
    }
    else
    {    
        this->pui32AdcBase->CLIST3 = ((this->pui32AdcBase->CLIST3 & ~(uint16_t)ADC_CLIST3_SAMPLE9_MASK) | 
                                      ADC_CLIST3_SAMPLE9(this->ui16ChanNumVDcb + 8));
        this->ui16AdcSmplVDcb = 9;
    }    

    /* default channel assignment for default SVM sector = 2 */
    /* direct sensing of phases A and C */
    *this->sCurrSec23.pui16ClistPhaA = 
            ((*this->sCurrSec23.pui16ClistPhaA & ~(uint16_t)0xFU) |
               this->sCurrSec23.ui16AdcSmplPhaA);
        
        *this->sCurrSec23.pui16ClistPhaC =
                ((*this->sCurrSec23.pui16ClistPhaC & ~(uint16_t)0xFU) |
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
bool_t MCDRV_Curr3Ph2ShChanAssign(mcdrv_adc12_t *this)
{
    s_statusPass = TRUE;

    switch (*this->pui16SVMSector)
    {
        /* direct sensing of phases A and C */
        case 2:
        case 3:
            /* CLISTx update with ADC channel - phase current A */
            *this->sCurrSec23.pui16ClistPhaA =
                ((*this->sCurrSec23.pui16ClistPhaA & ~(uint16_t)0xFU) |
                   this->sCurrSec23.ui16AdcSmplPhaA);
            /* CLISTx update with ADC channel - phase current C */
            *this->sCurrSec23.pui16ClistPhaC =
                ((*this->sCurrSec23.pui16ClistPhaC & ~(uint16_t)0xFU) |
                   this->sCurrSec23.ui16AdcSmplPhaC);           
            break;

        /* direct sensing of phases A and B */
        case 4:
        case 5:
            /* CLISTx update with ADC channel - phase current A */
            *this->sCurrSec45.pui16ClistPhaA =
                ((*this->sCurrSec45.pui16ClistPhaA & ~(uint16_t)0xFU) |
                   this->sCurrSec45.ui16AdcSmplPhaA);
            /* CLISTx update with ADC channel - phase current B */
            *this->sCurrSec45.pui16ClistPhaB =
                ((*this->sCurrSec45.pui16ClistPhaB & ~(uint16_t)0xFU) |
                   this->sCurrSec45.ui16AdcSmplPhaB);
            break;
        
        /* direct sensing of phases B and C */
        case 1:
        case 6:
        default:
            /* CLISTx update with ADC channel - phase current B */
            *this->sCurrSec16.pui16ClistPhaB =
                ((*this->sCurrSec16.pui16ClistPhaB & ~(uint16_t)0xFU) |
                   this->sCurrSec16.ui16AdcSmplPhaB);
            /* CLISTx update with ADC channel - phase current C */
            *this->sCurrSec16.pui16ClistPhaC =
                ((*this->sCurrSec16.pui16ClistPhaC & ~(uint16_t)0xFU) |
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
bool_t MCDRV_Curr3Ph2ShCalibInit(mcdrv_adc12_t *this)
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
bool_t MCDRV_Curr3Ph2ShCalib(mcdrv_adc12_t *this)
{
    s_statusPass = TRUE;

    switch (*this->pui16SVMSector)
    {
        case 2:
        case 3:
            /* direct sensing of measurement offsets on phases A and C */
            this->sCurrSec23.ui16CalibPhaA = GDFLIB_FilterMA_F16(
                (frac16_t) this->pui32AdcBase->RSLT[this->sCurrSec23.ui16AdcNumPhaA], 
                          &this->sCurrSec23.ui16FiltPhaA);
            this->sCurrSec23.ui16CalibPhaC = GDFLIB_FilterMA_F16(
                (frac16_t) this->pui32AdcBase->RSLT[this->sCurrSec23.ui16AdcNumPhaC], 
                           &this->sCurrSec23.ui16FiltPhaC);
            break;

        case 4:
        case 5:
            /* direct sensing of measurement offsets on phases A and B */
            this->sCurrSec45.ui16CalibPhaA = GDFLIB_FilterMA_F16(
                (frac16_t) this->pui32AdcBase->RSLT[this->sCurrSec45.ui16AdcNumPhaA], 
                          &this->sCurrSec45.ui16FiltPhaA);
            this->sCurrSec45.ui16CalibPhaB = GDFLIB_FilterMA_F16(
                (frac16_t) this->pui32AdcBase->RSLT[this->sCurrSec45.ui16AdcNumPhaB], 
                          &this->sCurrSec45.ui16FiltPhaB);
            break;

        case 1:
        case 6:
        default:
            /* direct sensing of measurement offsets on phases B and C */
            this->sCurrSec16.ui16CalibPhaB = GDFLIB_FilterMA_F16(
                (frac16_t) this->pui32AdcBase->RSLT[this->sCurrSec16.ui16AdcNumPhaB], 
                          &this->sCurrSec16.ui16FiltPhaB);
            this->sCurrSec16.ui16CalibPhaC = GDFLIB_FilterMA_F16(
                (frac16_t) this->pui32AdcBase->RSLT[this->sCurrSec16.ui16AdcNumPhaC], 
                          &this->sCurrSec16.ui16FiltPhaC);
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
bool_t MCDRV_Curr3Ph2ShCalibSet(mcdrv_adc12_t *this)
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
bool_t MCDRV_VoltDcBusGet(mcdrv_adc12_t *this)
{
    s_statusPass = TRUE;

    /* read DC-bus voltage sample from defined ADCx result register */
    *this->pf16UDcBus = (frac16_t)(this->pui32AdcBase->RSLT[this->ui16AdcSmplVDcb]);

    return (s_statusPass);
}

/*!
 * @brief Function reads and passes auxiliary sample
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AuxValGet(mcdrv_adc12_t *this)
{
    s_statusPass = TRUE;

    /* read Auxiliary channel sample from defined ADCx result register */
    *this->pui16AuxChan = (frac16_t)(this->pui32AdcBase->RSLT[this->ui16AdcNumAux]);

    return (s_statusPass);
}

