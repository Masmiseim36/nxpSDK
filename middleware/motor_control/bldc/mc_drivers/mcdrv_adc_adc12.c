/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
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
    s_statusPass = TRUE;

    /* pointer to array with the channel numbers */
    this->pui16AdcArray = init->ui16AdcArray;

    /* default DC bus current offset */
    this->ui16OffsetDcCurr = 0x3fff;

    /* check if there are one pair of assigned channels */
    if ((init->ui16AdcArray[MCDRV_ADC0_BEMFA] == MCDRV_CHAN_OFF) &&
        (init->ui16AdcArray[MCDRV_ADC1_BEMFA] == MCDRV_CHAN_OFF))
    {
        /* check if channel for phase A BEMF is assigned */
        s_statusPass = FALSE;
    }
    else if ((init->ui16AdcArray[MCDRV_ADC0_BEMFB] == MCDRV_CHAN_OFF) &&
             (init->ui16AdcArray[MCDRV_ADC1_BEMFB] == MCDRV_CHAN_OFF))
    {
        /* check if channel for phase B BEMF is assigned */
        s_statusPass = FALSE;
    }
    else if ((init->ui16AdcArray[MCDRV_ADC0_BEMFC] == MCDRV_CHAN_OFF) &&
             (init->ui16AdcArray[MCDRV_ADC1_BEMFC] == MCDRV_CHAN_OFF))
    {
        /* check if channel for phase C BEMF is assigned */
        s_statusPass = FALSE;
    }
    else if ((init->ui16AdcArray[MCDRV_ADC0_UDCB] == MCDRV_CHAN_OFF) &&
             (init->ui16AdcArray[MCDRV_ADC1_UDCB] == MCDRV_CHAN_OFF))
    {
        /* check if channel for DC-bus voltage is assigned */
        s_statusPass = FALSE;
    }
    else if ((init->ui16AdcArray[MCDRV_ADC0_IDCB] == MCDRV_CHAN_OFF) &&
             (init->ui16AdcArray[MCDRV_ADC1_IDCB] == MCDRV_CHAN_OFF))
    {
        /* check if channel for DC-bus current is assigned */
        s_statusPass = FALSE;
    }
    else
    {
        /* ADC module result register assignment */
        /* 0 -> result register 0 for ADC0 or ADC1 */
        /* 1 -> result register 1 for ADC0 or ADC1 */
        this->ui16RsltNumBemf = 0;
        this->ui16RsltNumAux = 1;
        this->ui16RsltNumIdcb = 8;
        this->ui16RsltNumUdcb = 9;
        this->pui32AdcBaseAdd = init->pui32AdcBase;
        /* BEMF phase A measurement */
        if (init->ui16AdcArray[MCDRV_ADC0_BEMFA] == MCDRV_CHAN_OFF)
        {
            this->bldcAdc1SectorCfg[2] = init->ui16AdcArray[MCDRV_ADC1_BEMFA] + 8;
            this->bldcAdc1SectorCfg[5] = init->ui16AdcArray[MCDRV_ADC1_BEMFA] + 8;
        }
        else
        {
            this->bldcAdc0SectorCfg[2] = init->ui16AdcArray[MCDRV_ADC0_BEMFA];
            this->bldcAdc0SectorCfg[5] = init->ui16AdcArray[MCDRV_ADC0_BEMFA];
        }

        /* BEMF phase B measurement */
        if (init->ui16AdcArray[MCDRV_ADC0_BEMFB] == MCDRV_CHAN_OFF)
        {
            this->bldcAdc1SectorCfg[1] = init->ui16AdcArray[MCDRV_ADC1_BEMFB] + 8;
            this->bldcAdc1SectorCfg[4] = init->ui16AdcArray[MCDRV_ADC1_BEMFB] + 8;
        }
        else
        {
            this->bldcAdc0SectorCfg[1] = init->ui16AdcArray[MCDRV_ADC0_BEMFB];
            this->bldcAdc0SectorCfg[4] = init->ui16AdcArray[MCDRV_ADC0_BEMFB];
        }

        /* BEMF phase C measurement */
        if (init->ui16AdcArray[MCDRV_ADC0_BEMFC] == MCDRV_CHAN_OFF)
        {
            this->bldcAdc1SectorCfg[0] = init->ui16AdcArray[MCDRV_ADC1_BEMFC] + 8;
            this->bldcAdc1SectorCfg[3] = init->ui16AdcArray[MCDRV_ADC1_BEMFC] + 8;
            this->pui32AdcBaseAdd->CLIST3 = ((this->pui32AdcBaseAdd->CLIST3 & ~(uint16_t)(ADC_CLIST3_SAMPLE8_MASK)) |
                                             ADC_CLIST3_SAMPLE8(init->ui16AdcArray[MCDRV_ADC1_BEMFC] + 8));
            this->ui16RsltNumBemf = 8;
        }
        else
        {
            this->bldcAdc0SectorCfg[0] = init->ui16AdcArray[MCDRV_ADC0_BEMFC];
            this->bldcAdc0SectorCfg[3] = init->ui16AdcArray[MCDRV_ADC0_BEMFC];
            this->pui32AdcBaseAdd->CLIST1 = ((this->pui32AdcBaseAdd->CLIST1 & ~(uint16_t)(ADC_CLIST1_SAMPLE0_MASK)) |
                                             ADC_CLIST1_SAMPLE0(init->ui16AdcArray[MCDRV_ADC0_BEMFC]));
            this->ui16RsltNumBemf = 0;
        }

        /* DC-bus current measurement */
        if (init->ui16AdcArray[MCDRV_ADC0_IDCB] == MCDRV_CHAN_OFF)
        {
            this->pui32AdcBaseAdd->CLIST3 = ((this->pui32AdcBaseAdd->CLIST3 & ~(uint16_t)(ADC_CLIST3_SAMPLE8_MASK)) |
                                             ADC_CLIST3_SAMPLE8(init->ui16AdcArray[MCDRV_ADC1_IDCB] + 8));
            this->ui16RsltNumIdcb = 8;
        }
        else
        {
            this->pui32AdcBaseAdd->CLIST1 = ((this->pui32AdcBaseAdd->CLIST1 & ~(uint16_t)(ADC_CLIST1_SAMPLE2_MASK)) |
                                             ADC_CLIST1_SAMPLE2(init->ui16AdcArray[MCDRV_ADC0_IDCB]));
            this->ui16RsltNumIdcb = 2;
        }

        /* auxiliary measurement */
        if (init->ui16AdcArray[MCDRV_ADC0_AUX] == MCDRV_CHAN_OFF)
        {
            this->pui32AdcBaseAdd->CLIST3 = ((this->pui32AdcBaseAdd->CLIST3 & ~(uint16_t)(ADC_CLIST3_SAMPLE9_MASK)) |
                                             ADC_CLIST3_SAMPLE9(init->ui16AdcArray[MCDRV_ADC1_AUX] + 8));
            this->ui16RsltNumAux = 9;
        }
        else
        {
            this->pui32AdcBaseAdd->CLIST1 = ((this->pui32AdcBaseAdd->CLIST1 & ~(uint16_t)(ADC_CLIST1_SAMPLE1_MASK)) |
                                             ADC_CLIST1_SAMPLE1(init->ui16AdcArray[MCDRV_ADC0_AUX]));
            this->ui16RsltNumAux = 1;
        }

        /* DC-bus voltage measurement */
        if (init->ui16AdcArray[MCDRV_ADC0_UDCB] == MCDRV_CHAN_OFF)
        {
            this->pui32AdcBaseAdd->CLIST3 = ((this->pui32AdcBaseAdd->CLIST3 & ~(uint16_t)(ADC_CLIST3_SAMPLE9_MASK)) |
                                             ADC_CLIST3_SAMPLE9(init->ui16AdcArray[MCDRV_ADC1_UDCB] + 8));
            this->ui16RsltNumUdcb = 9;
        }
        else
        {
            this->pui32AdcBaseAdd->CLIST1 = ((this->pui32AdcBaseAdd->CLIST1 & ~(uint16_t)(ADC_CLIST1_SAMPLE1_MASK)) |
                                             ADC_CLIST1_SAMPLE1(init->ui16AdcArray[MCDRV_ADC0_UDCB]));
            this->ui16RsltNumUdcb = 1;
        }
    }

    return (s_statusPass);
}

/*!
 * @brief Function set new channel assignment for next BEMF voltage sensing
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AssignBemfChannel(mcdrv_adc12_t *this)
{
    s_statusPass = TRUE;

    switch (this->ui16Sector)
    {
        /* BEMF phase C sensing */
        case 0:
        case 3:
            if ((this->pui16AdcArray[MCDRV_ADC1_BEMFC]) == MCDRV_CHAN_OFF)
            {
                this->pui32AdcBaseAdd->CLIST1 = ((this->pui32AdcBaseAdd->CLIST1 & ~(uint16_t)(ADC_CLIST1_SAMPLE0_MASK)) |
                                                 ADC_CLIST1_SAMPLE0(this->pui16AdcArray[MCDRV_ADC0_BEMFC]));
                this->ui16RsltNumBemf = 0;
            }
            else
            {
                this->pui32AdcBaseAdd->CLIST3 = ((this->pui32AdcBaseAdd->CLIST3 & ~(uint16_t)(ADC_CLIST3_SAMPLE8_MASK)) |
                                                 ADC_CLIST3_SAMPLE8(this->pui16AdcArray[MCDRV_ADC1_BEMFC] + 8));
                this->ui16RsltNumBemf = 8;
            }
            break;

        /* BEMF phase B sensing */
        case 1:
        case 4:
            if ((this->pui16AdcArray[MCDRV_ADC1_BEMFB]) == MCDRV_CHAN_OFF)
            {
                this->pui32AdcBaseAdd->CLIST1 = ((this->pui32AdcBaseAdd->CLIST1 & ~(uint16_t)(ADC_CLIST1_SAMPLE0_MASK)) |
                                                 ADC_CLIST1_SAMPLE0(this->pui16AdcArray[MCDRV_ADC0_BEMFB]));
                this->ui16RsltNumBemf = 0;
            }
            else
            {
                this->pui32AdcBaseAdd->CLIST3 = ((this->pui32AdcBaseAdd->CLIST3 & ~(uint16_t)(ADC_CLIST3_SAMPLE8_MASK)) |
                                                 ADC_CLIST3_SAMPLE8(this->pui16AdcArray[MCDRV_ADC1_BEMFB] + 8));
                this->ui16RsltNumBemf = 8;
            }
            break;

        /* BEMF phase A sensing */
        case 2:
        case 5:
            if ((this->pui16AdcArray[MCDRV_ADC1_BEMFA]) == MCDRV_CHAN_OFF)
            {
                this->pui32AdcBaseAdd->CLIST1 = ((this->pui32AdcBaseAdd->CLIST1 & ~(uint16_t)(ADC_CLIST1_SAMPLE0_MASK)) |
                                                 ADC_CLIST1_SAMPLE0(this->pui16AdcArray[MCDRV_ADC0_BEMFA]));
                this->ui16RsltNumBemf = 0;
            }
            else
            {
                this->pui32AdcBaseAdd->CLIST3 = ((this->pui32AdcBaseAdd->CLIST3 & ~(uint16_t)(ADC_CLIST3_SAMPLE8_MASK)) |
                                                 ADC_CLIST3_SAMPLE8(this->pui16AdcArray[MCDRV_ADC1_BEMFA] + 8));
                this->ui16RsltNumBemf = 8;
            }
            break;

        default:
            break;
    }

    return (s_statusPass);
}

/*!
 * @brief Function reads ADC result register containing actual BEMF voltage
 *
 * Result register value is stored to BEMF voltage pointer
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_BemfVoltageGet(mcdrv_adc12_t *this)
{
    s_statusPass = TRUE;

    *this->pf16BemfVoltage = (frac16_t)(this->pui32AdcBaseAdd->RSLT[this->ui16RsltNumBemf]);

    return (s_statusPass);
}

/*!
 * @brief Function reads ADC result register containing actual DC-bus voltage sample
 *
 * Result register value is stored to DC-bus voltage pointer
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_VoltDcBusGet(mcdrv_adc12_t *this)
{
    s_statusPass = TRUE;
    /* read DC-bus voltage sample from defined ADCx result register */
    *this->pf16UDcBus = (frac16_t)(this->pui32AdcBaseAdd->RSLT[this->ui16RsltNumUdcb]);

    return (s_statusPass);
}

/*!
 * @brief Function reads ADC result register containing actual DC-bus current sample
 *
 * Result register value is stored to DC-bus current pointer
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrDcBusGet(mcdrv_adc12_t *this)
{
    s_statusPass = TRUE;

    *this->pf16IDcBus =
        (frac16_t)((int16_t) this->pui32AdcBaseAdd->RSLT[this->ui16RsltNumIdcb] - this->ui16OffsetDcCurr);

    return (s_statusPass);
}

/*!
 * @brief Function reads ADC result register containing actual auxiliary sample
 *
 * Result register value is shifted 1 time right and stored to
 * auxiliary pointer
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AuxValGet(mcdrv_adc12_t *this)
{
    s_statusPass = TRUE;

    /* read auxiliary channel sample from defined ADCx result register */
    *this->pui16AuxChan = (frac16_t)(MLIB_ShLSat_F16(this->pui32AdcBaseAdd->RSLT[this->ui16RsltNumAux], 1));

    return (s_statusPass);
}

/*!
 * @brief Function initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrOffsetCalibInit(mcdrv_adc12_t *this)
{
    s_statusPass = TRUE;

    /* clear offset values */
    this->ui16OffsetDcCurr = 0x3fff;
    this->ui16CalibDcCurr = 0;

    /* initialize offset filters */
    this->ui16FiltDcCurr.u16Sh = this->ui16OffsetFiltWindow;

    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->ui16FiltDcCurr);

    return (s_statusPass);
}

/*!
 * @brief Function reads current samples and filter them
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrOffsetCalib(mcdrv_adc12_t *this)
{
    s_statusPass = TRUE;

    /* sensing of DC Bus Current offset */
    this->ui16CalibDcCurr =
        GDFLIB_FilterMA_F16((frac16_t)(this->pui32AdcBaseAdd->RSLT[this->ui16RsltNumIdcb]), &this->ui16FiltDcCurr);

    return (s_statusPass);
}

/*!
 * @brief Function passes measured offset values to main structure
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrOffsetCalibSet(mcdrv_adc12_t *this)
{
    s_statusPass = TRUE;

    /* pass Calib data for DC Bus current offset */
    this->ui16OffsetDcCurr = (this->ui16CalibDcCurr);

    return (s_statusPass);
}

/*!
 * @brief Function switch between DC bus current or voltage sensing
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AssignDCBusChannel(mcdrv_adc12_t *this)
{
    s_statusPass = TRUE;

    return (s_statusPass);
}

