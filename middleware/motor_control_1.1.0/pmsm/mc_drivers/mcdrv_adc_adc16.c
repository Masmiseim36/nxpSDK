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
 * @brief Initializes ADC driver to measure 3 currents, DC-bus voltage and
 *        auxiliary quantity samples
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Adc16Init(mcdrv_adc16_t *this, mcdrv_adc16_init_t *init)
{
    s_statusPass = TRUE;
    uint16_t ui16CurrSelector = 0;

    /* offset filter window */
    this->ui16OffsetFiltWindow = 3;

    /* Check if there are one pair of assigned channels */
    if ((init->ui16AdcArray[MCDRV_ADC0_IA] != MCDRV_CHAN_OFF) && (init->ui16AdcArray[MCDRV_ADC1_IA] != MCDRV_CHAN_OFF))
        ui16CurrSelector = 0; /* Both ADCs assigned for IA */
    else if ((init->ui16AdcArray[MCDRV_ADC0_IB] != MCDRV_CHAN_OFF) &&
             (init->ui16AdcArray[MCDRV_ADC1_IB] != MCDRV_CHAN_OFF))
        ui16CurrSelector = 2; /* Both ADCs assigned for IB */
    else if ((init->ui16AdcArray[MCDRV_ADC0_IC] != MCDRV_CHAN_OFF) &&
             (init->ui16AdcArray[MCDRV_ADC1_IC] != MCDRV_CHAN_OFF))
        ui16CurrSelector = 4; /* Both ADCs assigned for IC */
    else
        s_statusPass = FALSE; /* None of current is assigned to both ADCs */

    /* DO NOT MODIFY THIS CODE - to all MCDRV_ADC channel assigns use config files in board folder */
    switch (ui16CurrSelector)
    {
        case 0:
            /*  Phase A current on both ADCs */
            /* Phase B - ADC 0, Phase C - ADC1 */
            if ((!(init->ui16AdcArray[MCDRV_ADC0_IB] == MCDRV_CHAN_OFF)) &&
                (!(init->ui16AdcArray[MCDRV_ADC1_IC] == MCDRV_CHAN_OFF)))
            {
                this->sCurrSec16.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC1_IC];
                this->sCurrSec16.pAdcBasePhaC = init->pui32Adc1Base;
                this->sCurrSec16.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC0_IB];
                this->sCurrSec16.pAdcBasePhaB = init->pui32Adc0Base;

                this->sCurrSec23.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC0_IA];
                this->sCurrSec23.pAdcBasePhaA = init->pui32Adc0Base;
                this->sCurrSec23.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC1_IC];
                this->sCurrSec23.pAdcBasePhaC = init->pui32Adc1Base;

                this->sCurrSec45.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC1_IA];
                this->sCurrSec45.pAdcBasePhaA = init->pui32Adc1Base;
                this->sCurrSec45.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC0_IB];
                this->sCurrSec45.pAdcBasePhaB = init->pui32Adc0Base;
            }

            /* Phase A - MCDRV_ADC 1, Phase C - MCDRV_ADC0 */
            if ((!(init->ui16AdcArray[MCDRV_ADC1_IA] == MCDRV_CHAN_OFF)) &&
                (!(init->ui16AdcArray[MCDRV_ADC0_IC] == MCDRV_CHAN_OFF)))
            {
                this->sCurrSec16.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC0_IC];
                this->sCurrSec16.pAdcBasePhaC = init->pui32Adc0Base;
                this->sCurrSec16.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC1_IB];
                this->sCurrSec16.pAdcBasePhaB = init->pui32Adc1Base;

                this->sCurrSec23.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC1_IA];
                this->sCurrSec23.pAdcBasePhaA = init->pui32Adc1Base;
                this->sCurrSec23.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC0_IC];
                this->sCurrSec23.pAdcBasePhaC = init->pui32Adc0Base;

                this->sCurrSec45.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC0_IA];
                this->sCurrSec45.pAdcBasePhaA = init->pui32Adc0Base;
                this->sCurrSec45.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC1_IB];
                this->sCurrSec45.pAdcBasePhaB = init->pui32Adc1Base;
            }
            break;
        case 2:
            /*  Phase B current on both MCDRV_ADCs */
            /* Phase A - MCDRV_ADC 0, Phase C - MCDRV_ADC1 */
            if ((!(init->ui16AdcArray[MCDRV_ADC0_IA] == MCDRV_CHAN_OFF)) &&
                (!(init->ui16AdcArray[MCDRV_ADC1_IC] == MCDRV_CHAN_OFF)))
            {
                this->sCurrSec16.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC1_IC];
                this->sCurrSec16.pAdcBasePhaC = init->pui32Adc1Base;
                this->sCurrSec16.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC0_IB];
                this->sCurrSec16.pAdcBasePhaB = init->pui32Adc0Base;

                this->sCurrSec23.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC0_IA];
                this->sCurrSec23.pAdcBasePhaA = init->pui32Adc0Base;
                this->sCurrSec23.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC1_IC];
                this->sCurrSec23.pAdcBasePhaC = init->pui32Adc1Base;

                this->sCurrSec45.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC0_IA];
                this->sCurrSec45.pAdcBasePhaA = init->pui32Adc0Base;
                this->sCurrSec45.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC1_IB];
                this->sCurrSec45.pAdcBasePhaB = init->pui32Adc1Base;
            }
            /* Phase A - MCDRV_ADC 1, Phase C - MCDRV_ADC0 */
            if ((!(init->ui16AdcArray[MCDRV_ADC1_IA] == MCDRV_CHAN_OFF)) &&
                (!(init->ui16AdcArray[MCDRV_ADC0_IC] == MCDRV_CHAN_OFF)))
            {
                this->sCurrSec16.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC0_IC];
                this->sCurrSec16.pAdcBasePhaC = init->pui32Adc0Base;
                this->sCurrSec16.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC1_IB];
                this->sCurrSec16.pAdcBasePhaB = init->pui32Adc1Base;

                this->sCurrSec23.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC1_IA];
                this->sCurrSec23.pAdcBasePhaA = init->pui32Adc1Base;
                this->sCurrSec23.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC0_IC];
                this->sCurrSec23.pAdcBasePhaC = init->pui32Adc0Base;

                this->sCurrSec45.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC1_IA];
                this->sCurrSec45.pAdcBasePhaA = init->pui32Adc1Base;
                this->sCurrSec45.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC0_IB];
                this->sCurrSec45.pAdcBasePhaB = init->pui32Adc0Base;
            }
            break;
        case 4:
            /*  Phase C current on both MCDRV_ADCs */
            /* Phase A - MCDRV_ADC 0, Phase B - MCDRV_ADC1 */
            if ((!(init->ui16AdcArray[MCDRV_ADC0_IA] == MCDRV_CHAN_OFF)) &&
                (!(init->ui16AdcArray[MCDRV_ADC1_IB] == MCDRV_CHAN_OFF)))
            {
                this->sCurrSec16.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC0_IC];
                this->sCurrSec16.pAdcBasePhaC = init->pui32Adc0Base;
                this->sCurrSec16.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC1_IB];
                this->sCurrSec16.pAdcBasePhaB = init->pui32Adc1Base;

                this->sCurrSec23.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC0_IA];
                this->sCurrSec23.pAdcBasePhaA = init->pui32Adc0Base;
                this->sCurrSec23.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC1_IC];
                this->sCurrSec23.pAdcBasePhaC = init->pui32Adc1Base;

                this->sCurrSec45.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC0_IA];
                this->sCurrSec45.pAdcBasePhaA = init->pui32Adc0Base;
                this->sCurrSec45.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC1_IB];
                this->sCurrSec45.pAdcBasePhaB = init->pui32Adc1Base;
            }
            /* Phase A - MCDRV_ADC 1, Phase B - MCDRV_ADC0 */
            if ((!(init->ui16AdcArray[MCDRV_ADC1_IA] == MCDRV_CHAN_OFF)) &&
                (!(init->ui16AdcArray[MCDRV_ADC0_IB] == MCDRV_CHAN_OFF)))
            {
                this->sCurrSec16.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC1_IC];
                this->sCurrSec16.pAdcBasePhaC = init->pui32Adc1Base;
                this->sCurrSec16.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC0_IB];
                this->sCurrSec16.pAdcBasePhaB = init->pui32Adc0Base;

                this->sCurrSec23.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC1_IA];
                this->sCurrSec23.pAdcBasePhaA = init->pui32Adc1Base;
                this->sCurrSec23.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC0_IC];
                this->sCurrSec23.pAdcBasePhaC = init->pui32Adc0Base;

                this->sCurrSec45.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC1_IA];
                this->sCurrSec45.pAdcBasePhaA = init->pui32Adc1Base;
                this->sCurrSec45.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC0_IB];
                this->sCurrSec45.pAdcBasePhaB = init->pui32Adc0Base;
            }
            break;
        default:
            s_statusPass = FALSE; /* If there is no both MCDRV_ADC assigned, this fail occur */
            break;
    }

    /* Check whether Udcb is on ADC0 and Aux on ADC1 */
    if ((init->ui16AdcArray[MCDRV_ADC0_UDCB] != MCDRV_CHAN_OFF) &&
        (init->ui16AdcArray[MCDRV_ADC1_AUX] != MCDRV_CHAN_OFF))
    {
        /* Udcb channel assigned to ADC0 */
        this->pui32UdcbAdcBase = init->pui32Adc0Base;
        this->pui32UdcbAdcBase->SC1[1] = ((this->pui32UdcbAdcBase->SC1[1] & ~ADC_SC1_ADCH_MASK) |
                                          (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC0_UDCB])));
        /* Auxiliary channel assigned to ADC1 */
        this->pui32AuxAdcBase = init->pui32Adc1Base;
        this->pui32AuxAdcBase->SC1[1] =
            ((this->pui32AuxAdcBase->SC1[1] & ~ADC_SC1_ADCH_MASK) | (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC1_AUX])));
    }
    /* Check whether Udcb is on ADC1 and Aux on ADC0 */
    else if ((init->ui16AdcArray[MCDRV_ADC1_UDCB] != MCDRV_CHAN_OFF) &&
             (init->ui16AdcArray[MCDRV_ADC0_AUX] != MCDRV_CHAN_OFF))
    {
        /* Udcb channel assigned to ADC1 */
        this->pui32UdcbAdcBase = init->pui32Adc1Base;
        this->pui32UdcbAdcBase->SC1[1] = ((this->pui32UdcbAdcBase->SC1[1] & ~ADC_SC1_ADCH_MASK) |
                                          (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC1_UDCB])));
        /* Auxiliary channel assigned to ADC0 */
        this->pui32AuxAdcBase = init->pui32Adc0Base;
        this->pui32AuxAdcBase->SC1[1] =
            ((this->pui32AuxAdcBase->SC1[1] & ~ADC_SC1_ADCH_MASK) | (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC0_AUX])));
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
bool_t MCDRV_Adc16InitOptim(mcdrv_adc16_t *this, mcdrv_adc16_init_t *init)
{
    s_statusPass = TRUE;

    /* offset filter window */
    this->ui16OffsetFiltWindow = 3;

    /* Phase current measurement */
    /* Sector 1,6 */
    /* TWR-KV31 + TWR-LV3PH: Ic = ADC_RA(ADC0_CH0), Ib = ADC_RA(ADC1_CH0) */
    this->sCurrSec16.ui16ChanNumPhaC = 0;
    this->sCurrSec16.pAdcBasePhaC = init->pui32Adc0Base;
    this->sCurrSec16.ui16ChanNumPhaB = 0;
    this->sCurrSec16.pAdcBasePhaB = init->pui32Adc1Base;

    /* Sector 2,3 */
    /* TWR-KV31 + TWR-LV3PH: Ic = ADC_RA(ADC0_CH0), Ia = ADC_RA(ADC1_CH8) */
    this->sCurrSec23.ui16ChanNumPhaA = 8;
    this->sCurrSec23.pAdcBasePhaA = init->pui32Adc1Base;
    this->sCurrSec23.ui16ChanNumPhaC = 0;
    this->sCurrSec23.pAdcBasePhaC = init->pui32Adc0Base;

    /* Sector 4,5 */
    /* TWR-KV31 + TWR-LV3PH: Ib = ADC_RA(ADC0_CH3), Ia = ADC_RA(ADC1_CH8) */
    this->sCurrSec45.ui16ChanNumPhaA = 8;
    this->sCurrSec45.pAdcBasePhaA = init->pui32Adc1Base;
    this->sCurrSec45.ui16ChanNumPhaB = 3;
    this->sCurrSec45.pAdcBasePhaB = init->pui32Adc0Base;

    /* DCbus and Aux channel measurement */
    /* TWR-KV31 + TWR-LV3PH: DCbus = ADC_RB(ADC0_CH13), Aux = ADC_RB(ADC1_CH10) */
    this->pui32UdcbAdcBase = init->pui32Adc0Base;
    this->pui32UdcbAdcBase->SC1[1] = (this->pui32UdcbAdcBase->SC1[1] & ~ADC_SC1_ADCH_MASK) | 13;
    this->pui32AuxAdcBase = init->pui32Adc1Base;
    this->pui32AuxAdcBase->SC1[1] = (this->pui32AuxAdcBase->SC1[1] & ~ADC_SC1_ADCH_MASK) | 10;

    return (s_statusPass);
}

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
                (frac16_t)(MLIB_ShLSat_F16((this->sCurrSec23.pAdcBasePhaA->R[0] - this->sCurrSec23.ui16OffsetPhaA), 4));
            sIABCtemp.f16C =
                (frac16_t)(MLIB_ShLSat_F16((this->sCurrSec23.pAdcBasePhaC->R[0] - this->sCurrSec23.ui16OffsetPhaC), 4));
            sIABCtemp.f16B = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16C));
            break;

        case 4:
        case 5:
            // direct sensing of A, B, calculation of C
            sIABCtemp.f16A =
                (frac16_t)(MLIB_ShLSat_F16((this->sCurrSec45.pAdcBasePhaA->R[0] - this->sCurrSec45.ui16OffsetPhaA), 4));
            sIABCtemp.f16B =
                (frac16_t)(MLIB_ShLSat_F16((this->sCurrSec45.pAdcBasePhaB->R[0] - this->sCurrSec45.ui16OffsetPhaB), 4));
            sIABCtemp.f16C = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, sIABCtemp.f16B));
            break;

        case 1:
        case 6:
        default:
            // direct sensing of B, C, calculation of A
            sIABCtemp.f16B =
                (frac16_t)(MLIB_ShLSat_F16((this->sCurrSec16.pAdcBasePhaB->R[0] - this->sCurrSec16.ui16OffsetPhaB), 4));
            sIABCtemp.f16C =
                (frac16_t)(MLIB_ShLSat_F16((this->sCurrSec16.pAdcBasePhaC->R[0] - this->sCurrSec16.ui16OffsetPhaC), 4));
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
            this->sCurrSec23.pAdcBasePhaA->SC1[0] =
                ((this->sCurrSec23.pAdcBasePhaA->SC1[0] & ~ADC_SC1_ADCH_MASK) | this->sCurrSec23.ui16ChanNumPhaA);
            this->sCurrSec23.pAdcBasePhaC->SC1[0] =
                ((this->sCurrSec23.pAdcBasePhaC->SC1[0] & ~ADC_SC1_ADCH_MASK) | this->sCurrSec23.ui16ChanNumPhaC);
            break;
        case 4:
        case 5: /*  currents Ia, Ib will be measured */
            /* update ADC_SC1n registers based on actual SVM sector */
            this->sCurrSec45.pAdcBasePhaA->SC1[0] =
                ((this->sCurrSec45.pAdcBasePhaA->SC1[0] & ~ADC_SC1_ADCH_MASK) | this->sCurrSec45.ui16ChanNumPhaA);
            this->sCurrSec45.pAdcBasePhaB->SC1[0] =
                ((this->sCurrSec45.pAdcBasePhaB->SC1[0] & ~ADC_SC1_ADCH_MASK) | this->sCurrSec45.ui16ChanNumPhaB);
            break;
        case 1:
        case 6: /* currents Ib, Ic will be measured */
        default:
            /* update ADC_SC1n registers based on actual SVM sector */
            this->sCurrSec16.pAdcBasePhaB->SC1[0] =
                ((this->sCurrSec16.pAdcBasePhaB->SC1[0] & ~ADC_SC1_ADCH_MASK) | this->sCurrSec16.ui16ChanNumPhaB);
            this->sCurrSec16.pAdcBasePhaC->SC1[0] =
                ((this->sCurrSec16.pAdcBasePhaC->SC1[0] & ~ADC_SC1_ADCH_MASK) | this->sCurrSec16.ui16ChanNumPhaC);
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
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec23.pAdcBasePhaA->R[0]), &this->sCurrSec23.ui16FiltPhaA);

            this->sCurrSec23.ui16CalibPhaC =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec23.pAdcBasePhaC->R[0]), &this->sCurrSec23.ui16FiltPhaC);

            break;

        case 4:
        case 5:
            /* sensing of offset IA -> MCDRV_ADC1_A and IB -> MCDRV_ADC0_A */
            this->sCurrSec45.ui16CalibPhaA =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec45.pAdcBasePhaA->R[0]), &this->sCurrSec45.ui16FiltPhaA);

            this->sCurrSec45.ui16CalibPhaB =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec45.pAdcBasePhaB->R[0]), &this->sCurrSec45.ui16FiltPhaB);
            break;

        case 1:
        case 6:
        default:
            /* sensing of offset IB -> ADC_A and IC -> ADC_B */
            this->sCurrSec16.ui16CalibPhaB =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec16.pAdcBasePhaB->R[0]), &this->sCurrSec16.ui16FiltPhaB);

            this->sCurrSec16.ui16CalibPhaC =
                GDFLIB_FilterMA_F16((frac16_t)(this->sCurrSec16.pAdcBasePhaC->R[0]), &this->sCurrSec16.ui16FiltPhaC);
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

