/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
 * @brief Initializes ADC driver to measure 3 currents, DC-bus voltage and
 *        auxiliary quantity samples
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Adc12Init(mcdrv_adc12_t *this, mcdrv_adc12_init_t *init)
{
    uint16_t ui16CurrSelector = 0;

    s_statusPass = TRUE;

    /* pass ADC base from init structure */
    this->pui32AdcBase = init->pui32AdcBase;

    /* offset filter window */
    this->ui16OffsetFiltWindow = 3;

    /* check if there are one pair of assigned channels */
    if ((init->ui16AdcArray[MCDRV_ADC0_IA] != MCDRV_CHAN_OFF) && (init->ui16AdcArray[MCDRV_ADC1_IA] != MCDRV_CHAN_OFF))
    {
        /* both ADCs assigned for IA */
        ui16CurrSelector = 0;
    }
    else if ((init->ui16AdcArray[MCDRV_ADC0_IB] != MCDRV_CHAN_OFF) &&
             (init->ui16AdcArray[MCDRV_ADC1_IB] != MCDRV_CHAN_OFF))
    {
        /* both ADCs assigned for IB */
        ui16CurrSelector = 2;
    }
    else if ((init->ui16AdcArray[MCDRV_ADC0_IC] != MCDRV_CHAN_OFF) &&
             (init->ui16AdcArray[MCDRV_ADC1_IC] != MCDRV_CHAN_OFF))
    {
        /* both ADCs assigned for IC */
        ui16CurrSelector = 4;
    }
    else
    {
        /* none of current is assigned to both ADCs */
        s_statusPass = FALSE;
    }

    /* DO NOT MODIFY THIS CODE - to all MCDRV_ADC channel assigns use config
       files in board folder */
    switch (ui16CurrSelector)
    {
        case 0:
            /* phase A current on both ADCs */
            /* phase B on ADCA, phase C on ADCB */
            if ((!(init->ui16AdcArray[MCDRV_ADC0_IB] == MCDRV_CHAN_OFF)) &&
                (!(init->ui16AdcArray[MCDRV_ADC1_IC] == MCDRV_CHAN_OFF)))
            {
                this->sCurrSec16.ui16RsltNumPhaC = 8;
                this->sCurrSec16.ui16RsltNumPhaB = 0;
                this->sCurrSec16.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IB];
                this->sCurrSec16.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IC] + 8;

                this->sCurrSec23.ui16RsltNumPhaA = 0;
                this->sCurrSec23.ui16RsltNumPhaC = 8;
                this->sCurrSec23.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IA];
                this->sCurrSec23.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IC] + 8;

                this->sCurrSec45.ui16RsltNumPhaA = 8;
                this->sCurrSec45.ui16RsltNumPhaB = 0;
                this->sCurrSec45.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IB];
                this->sCurrSec45.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IA] + 8;
            }
            /* phase A on ADCB, phase C on ADCA */
            else if ((!(init->ui16AdcArray[MCDRV_ADC1_IA] == MCDRV_CHAN_OFF)) &&
                     (!(init->ui16AdcArray[MCDRV_ADC0_IC] == MCDRV_CHAN_OFF)))
            {
                this->sCurrSec16.ui16RsltNumPhaC = 0;
                this->sCurrSec16.ui16RsltNumPhaB = 8;
                this->sCurrSec16.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IC];
                this->sCurrSec16.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IB] + 8;

                this->sCurrSec23.ui16RsltNumPhaA = 8;
                this->sCurrSec23.ui16RsltNumPhaC = 0;
                this->sCurrSec23.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IC];
                this->sCurrSec23.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IA] + 8;

                this->sCurrSec45.ui16RsltNumPhaA = 0;
                this->sCurrSec45.ui16RsltNumPhaB = 8;
                this->sCurrSec45.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IA];
                this->sCurrSec45.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IB] + 8;
            }
            else
                s_statusPass = FALSE;
            break;
        case 2:
            /* phase B current on both ADCs */
            /* phase A on ADCA, phase C on ADCB */
            if ((!(init->ui16AdcArray[MCDRV_ADC0_IA] == MCDRV_CHAN_OFF)) &&
                (!(init->ui16AdcArray[MCDRV_ADC1_IC] == MCDRV_CHAN_OFF)))
            {
                this->sCurrSec16.ui16RsltNumPhaC = 8;
                this->sCurrSec16.ui16RsltNumPhaB = 0;
                this->sCurrSec16.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IB];
                this->sCurrSec16.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IC] + 8;

                this->sCurrSec23.ui16RsltNumPhaA = 0;
                this->sCurrSec23.ui16RsltNumPhaC = 8;
                this->sCurrSec23.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IA];
                this->sCurrSec23.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IC] + 8;

                this->sCurrSec45.ui16RsltNumPhaA = 0;
                this->sCurrSec45.ui16RsltNumPhaB = 8;
                this->sCurrSec45.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IA];
                this->sCurrSec45.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IB] + 8;
            }
            /* phase A  on ADCB, phase C on ADCA */
            else if ((!(init->ui16AdcArray[MCDRV_ADC1_IA] == MCDRV_CHAN_OFF)) &&
                     (!(init->ui16AdcArray[MCDRV_ADC0_IC] == MCDRV_CHAN_OFF)))
            {
                this->sCurrSec16.ui16RsltNumPhaC = 0;
                this->sCurrSec16.ui16RsltNumPhaB = 8;
                this->sCurrSec16.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IC];
                this->sCurrSec16.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IB] + 8;

                this->sCurrSec23.ui16RsltNumPhaA = 8;
                this->sCurrSec23.ui16RsltNumPhaC = 0;
                this->sCurrSec23.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IC];
                this->sCurrSec23.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IA] + 8;

                this->sCurrSec45.ui16RsltNumPhaA = 8;
                this->sCurrSec45.ui16RsltNumPhaB = 0;
                this->sCurrSec45.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IB];
                this->sCurrSec45.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IA] + 8;
            }
            else
                s_statusPass = FALSE;
            break;
        case 4:
            /* phase C current on both ADCs */
            /* phase A on ADCA, phase B on ADCB */
            if ((!(init->ui16AdcArray[MCDRV_ADC0_IA] == MCDRV_CHAN_OFF)) &&
                (!(init->ui16AdcArray[MCDRV_ADC1_IB] == MCDRV_CHAN_OFF)))
            {
                this->sCurrSec16.ui16RsltNumPhaC = 0;
                this->sCurrSec16.ui16RsltNumPhaB = 8;
                this->sCurrSec16.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IC];
                this->sCurrSec16.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IB] + 8;

                this->sCurrSec23.ui16RsltNumPhaA = 0;
                this->sCurrSec23.ui16RsltNumPhaC = 8;
                this->sCurrSec23.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IA];
                this->sCurrSec23.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IC] + 8;

                this->sCurrSec45.ui16RsltNumPhaA = 0;
                this->sCurrSec45.ui16RsltNumPhaB = 8;
                this->sCurrSec45.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IA];
                this->sCurrSec45.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IB] + 8;
            }
            /* phase A on ADCB, phase B on ADCA */
            else if ((!(init->ui16AdcArray[MCDRV_ADC1_IA] == MCDRV_CHAN_OFF)) &&
                     (!(init->ui16AdcArray[MCDRV_ADC0_IB] == MCDRV_CHAN_OFF)))
            {
                this->sCurrSec16.ui16RsltNumPhaC = 8;
                this->sCurrSec16.ui16RsltNumPhaB = 0;
                this->sCurrSec16.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IB];
                this->sCurrSec16.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IC] + 8;

                this->sCurrSec23.ui16RsltNumPhaA = 8;
                this->sCurrSec23.ui16RsltNumPhaC = 0;
                this->sCurrSec23.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IC];
                this->sCurrSec23.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IA] + 8;

                this->sCurrSec45.ui16RsltNumPhaA = 8;
                this->sCurrSec45.ui16RsltNumPhaB = 0;
                this->sCurrSec45.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IB];
                this->sCurrSec45.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IA] + 8;
            }
            else
                s_statusPass = FALSE;
            break;
        default:
            /* if there is no both MCDRV_ADC assigned, this fail occur */
            s_statusPass = FALSE;
            break;
    }

    /* check whether DC-bus voltage is on ADCA and Aux on ADCB */
    if ((init->ui16AdcArray[MCDRV_ADC0_UDCB] != MCDRV_CHAN_OFF) &&
        (init->ui16AdcArray[MCDRV_ADC1_AUX] != MCDRV_CHAN_OFF))
    {
        /* DC-bus voltage channel assigned to ADCA */
        this->pui32AdcBase->CLIST1 = ((this->pui32AdcBase->CLIST1 & ~ADC_CLIST1_SAMPLE1_MASK) |
                                      (ADC_CLIST1_SAMPLE1(init->ui16AdcArray[MCDRV_ADC0_UDCB])));
        this->ui16RsltNumDcBus = 1;

        /* Auxiliary channel assigned to ADCB */
        this->pui32AdcBase->CLIST3 = ((this->pui32AdcBase->CLIST3 & ~ADC_CLIST3_SAMPLE9_MASK) |
                                      (ADC_CLIST3_SAMPLE9(init->ui16AdcArray[MCDRV_ADC1_AUX] + 8)));
        this->ui16RsltNumAuxChan = 9;
    }

    /* check whether DC-bus voltage is on ADCB and Aux on ADCA */
    else if ((init->ui16AdcArray[MCDRV_ADC1_UDCB] != MCDRV_CHAN_OFF) &&
             (init->ui16AdcArray[MCDRV_ADC0_AUX] != MCDRV_CHAN_OFF))
    {
        /* DC-bus voltage channel assigned to ADCB */
        this->pui32AdcBase->CLIST3 = ((this->pui32AdcBase->CLIST3 & ~ADC_CLIST3_SAMPLE9_MASK) |
                                      (ADC_CLIST3_SAMPLE9(init->ui16AdcArray[MCDRV_ADC1_UDCB] + 8)));
        this->ui16RsltNumDcBus = 9;

        /* Auxiliary channel assigned to ADCA */
        this->pui32AdcBase->CLIST1 = ((this->pui32AdcBase->CLIST1 & ~ADC_CLIST1_SAMPLE1_MASK) |
                                      (ADC_CLIST1_SAMPLE1(init->ui16AdcArray[MCDRV_ADC0_AUX])));
        this->ui16RsltNumAuxChan = 1;
    }
    else
        s_statusPass = FALSE;

    return (s_statusPass);
}

/*!
 * @brief Initializes ADC driver to measure from user defined channels
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Adc12InitOptim(mcdrv_adc12_t *this, mcdrv_adc12_init_t *init)
{
    s_statusPass = TRUE;

    /* pass ADC base from init structure */
    this->pui32AdcBase = init->pui32AdcBase;

    /* offset filter window */
    this->ui16OffsetFiltWindow = 3;

    /* Phase current measurement */
    /* Sector 1,6 */
    /* TWR-KV46 + TWR-LV3PH: Ic = SAMPLE0(ADCA_CH1), Ib = SAMPLE8(ADCB_CH3) */
    this->sCurrSec16.ui16Sample0 = 1;
    this->sCurrSec16.ui16Sample8 = 3 + 8;
    /* Select SAMPLE regs for Ib and Ic */
    this->sCurrSec16.ui16RsltNumPhaC = 0;
    this->sCurrSec16.ui16RsltNumPhaB = 8;

    /* Sector 2,3 */
    /* TWR-KV46 + TWR-LV3PH: Ic = SAMPLE0(ADCA_CH1), Ia = SAMPLE8(ADCB_CH1) */
    this->sCurrSec23.ui16Sample0 = 1;
    this->sCurrSec23.ui16Sample8 = 1 + 8;
    /* Select SAMPLE regs for Ia and Ic */
    this->sCurrSec23.ui16RsltNumPhaA = 8;
    this->sCurrSec23.ui16RsltNumPhaC = 0;

    /* Sector 4,5 */
    /* TWR-KV46 + TWR-LV3PH: Ia = SAMPLE0(ADCA_CH3), Ib = SAMPLE8(ADCB_CH3) */
    this->sCurrSec45.ui16Sample0 = 3;
    this->sCurrSec45.ui16Sample8 = 3 + 8;
    /* Select SAMPLE regs for Ia and Ib */
    this->sCurrSec45.ui16RsltNumPhaA = 0;
    this->sCurrSec45.ui16RsltNumPhaB = 8;

    /* DCbus and Aux channel measurement */
    /* TWR-KV46 + TWR-LV3PH: Aux = SAMPLE1(ADCA_CH30), DCbus = SAMPLE9(ADCB_CH0) */
    this->pui32AdcBase->CLIST1 = ((this->pui32AdcBase->CLIST1 & ~ADC_CLIST1_SAMPLE1_MASK) | (ADC_CLIST1_SAMPLE1(7)));
    this->pui32AdcBase->CLIST3 =
        ((this->pui32AdcBase->CLIST3 & ~ADC_CLIST3_SAMPLE9_MASK) | (ADC_CLIST3_SAMPLE9(0 + 8)));
    /* Select SAMPLE regs for DCbus and Aux channel */
    this->ui16RsltNumDcBus = 9;
    this->ui16RsltNumAuxChan = 1;

    return (s_statusPass);
}

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
                MLIB_ShLSat_F16(((frac16_t)((int16_t) this->pui32AdcBase->RSLT[this->sCurrSec23.ui16RsltNumPhaA]) -
                                 this->sCurrSec23.ui16OffsetPhaA),
                                1);
            sIABCtemp.f16C =
                MLIB_ShLSat_F16(((frac16_t)((int16_t) this->pui32AdcBase->RSLT[this->sCurrSec23.ui16RsltNumPhaC]) -
                                 this->sCurrSec23.ui16OffsetPhaC),
                                1);
            sIABCtemp.f16B = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16C));
            break;

        case 4:
        case 5:
            /* direct sensing of phase A and B, calculation of C */
            sIABCtemp.f16A =
                MLIB_ShLSat_F16(((frac16_t)((int16_t) this->pui32AdcBase->RSLT[this->sCurrSec45.ui16RsltNumPhaA]) -
                                 this->sCurrSec45.ui16OffsetPhaA),
                                1);
            sIABCtemp.f16B =
                MLIB_ShLSat_F16(((frac16_t)((int16_t) this->pui32AdcBase->RSLT[this->sCurrSec45.ui16RsltNumPhaB]) -
                                 this->sCurrSec45.ui16OffsetPhaB),
                                1);
            sIABCtemp.f16C = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16B));
            break;

        case 1:
        case 6:
        default:
            /* direct sensing of phase B and C, calculation of A */
            sIABCtemp.f16B =
                MLIB_ShLSat_F16(((frac16_t)((int16_t) this->pui32AdcBase->RSLT[this->sCurrSec16.ui16RsltNumPhaB]) -
                                 this->sCurrSec16.ui16OffsetPhaB),
                                1);
            sIABCtemp.f16C =
                MLIB_ShLSat_F16(((frac16_t)((int16_t) this->pui32AdcBase->RSLT[this->sCurrSec16.ui16RsltNumPhaC]) -
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
        case 2:
        case 3:
            /* direct sensing of phases A and C */
            this->pui32AdcBase->CLIST1 = ((this->pui32AdcBase->CLIST1 & ~ADC_CLIST1_SAMPLE0_MASK) |
                                          (ADC_CLIST1_SAMPLE0(this->sCurrSec23.ui16Sample0)));
            this->pui32AdcBase->CLIST3 = ((this->pui32AdcBase->CLIST3 & ~ADC_CLIST3_SAMPLE8_MASK) |
                                          (ADC_CLIST3_SAMPLE8(this->sCurrSec23.ui16Sample8)));
            break;

        case 4:
        case 5:
            /* direct sensing of phases A and B  */
            this->pui32AdcBase->CLIST1 = ((this->pui32AdcBase->CLIST1 & ~ADC_CLIST1_SAMPLE0_MASK) |
                                          (ADC_CLIST1_SAMPLE0(this->sCurrSec45.ui16Sample0)));
            this->pui32AdcBase->CLIST3 = ((this->pui32AdcBase->CLIST3 & ~ADC_CLIST3_SAMPLE8_MASK) |
                                          (ADC_CLIST3_SAMPLE8(this->sCurrSec45.ui16Sample8)));
            break;

        case 1:
        case 6:
        default:
            /* direct sensing of phases B and C */
            this->pui32AdcBase->CLIST1 = ((this->pui32AdcBase->CLIST1 & ~ADC_CLIST1_SAMPLE0_MASK) |
                                          (ADC_CLIST1_SAMPLE0(this->sCurrSec16.ui16Sample0)));
            this->pui32AdcBase->CLIST3 = ((this->pui32AdcBase->CLIST3 & ~ADC_CLIST3_SAMPLE8_MASK) |
                                          (ADC_CLIST3_SAMPLE8(this->sCurrSec16.ui16Sample8)));
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
                (frac16_t) this->pui32AdcBase->RSLT[this->sCurrSec23.ui16RsltNumPhaA], &this->sCurrSec23.ui16FiltPhaA);
            this->sCurrSec23.ui16CalibPhaC = GDFLIB_FilterMA_F16(
                (frac16_t) this->pui32AdcBase->RSLT[this->sCurrSec23.ui16RsltNumPhaC], &this->sCurrSec23.ui16FiltPhaC);
            break;

        case 4:
        case 5:
            /* direct sensing of measurement offsets on phases A and B */
            this->sCurrSec45.ui16CalibPhaA = GDFLIB_FilterMA_F16(
                (frac16_t) this->pui32AdcBase->RSLT[this->sCurrSec45.ui16RsltNumPhaA], &this->sCurrSec45.ui16FiltPhaA);
            this->sCurrSec45.ui16CalibPhaB = GDFLIB_FilterMA_F16(
                (frac16_t) this->pui32AdcBase->RSLT[this->sCurrSec45.ui16RsltNumPhaB], &this->sCurrSec45.ui16FiltPhaB);
            break;

        case 1:
        case 6:
        default:
            /* direct sensing of measurement offsets on phases B and C */
            this->sCurrSec16.ui16CalibPhaB = GDFLIB_FilterMA_F16(
                (frac16_t) this->pui32AdcBase->RSLT[this->sCurrSec16.ui16RsltNumPhaB], &this->sCurrSec16.ui16FiltPhaB);
            this->sCurrSec16.ui16CalibPhaC = GDFLIB_FilterMA_F16(
                (frac16_t) this->pui32AdcBase->RSLT[this->sCurrSec16.ui16RsltNumPhaC], &this->sCurrSec16.ui16FiltPhaC);
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
    *this->pf16UDcBus = (frac16_t)(this->pui32AdcBase->RSLT[this->ui16RsltNumDcBus]);

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
    *this->pui16AuxChan = (frac16_t)(this->pui32AdcBase->RSLT[this->ui16RsltNumAuxChan]);

    return (s_statusPass);
}

