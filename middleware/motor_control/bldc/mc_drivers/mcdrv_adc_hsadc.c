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
 * @brief Initializes ADC driver to measure 3 currents, DC-bus voltage and
 *        BEMF voltage for BLDC sensorless algorithm
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_HSAdcInit(mcdrv_hsadc_t *this, mcdrv_hsadc_init_t *init)
{
    s_statusPass = TRUE;

    /* pointer to array with the channel numbers */
    this->pui16AdcArray = init->ui16AdcArray;

    /* default DC-bus current offset */
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
        this->ui16RsltNumBemf = 2;
        this->ui16RsltNumAux = 3;
        this->ui16RsltNumIdcb = 0;
        this->ui16RsltNumUdcb = 1;
        this->pui32Adc0Base = init->pui32Adc0Base;
        this->pui32Adc1Base = init->pui32Adc1Base;

        /*BEMF A*/
        this->bldcAdc0SectorCfg[2] = init->ui16AdcArray[MCDRV_ADC0_BEMFA];
        this->bldcAdc0SectorCfg[5] = init->ui16AdcArray[MCDRV_ADC0_BEMFA];
        this->pui32BemfAAdcBase = init->pui32Adc0Base;

        /*BEMF B*/

        this->bldcAdc0SectorCfg[1] = init->ui16AdcArray[MCDRV_ADC0_BEMFB];
        this->bldcAdc0SectorCfg[4] = init->ui16AdcArray[MCDRV_ADC0_BEMFB];
        this->pui32BemfBAdcBase = init->pui32Adc0Base;

        /*BEMF C*/

        this->bldcAdc0SectorCfg[0] = init->ui16AdcArray[MCDRV_ADC1_BEMFC];
        this->bldcAdc0SectorCfg[3] = init->ui16AdcArray[MCDRV_ADC1_BEMFC];
        this->pui32BemfCAdcBase = init->pui32Adc1Base;
        this->pui32BemfCAdcBase->CLIST3 = ((this->pui32BemfCAdcBase->CLIST3 & ~(uint16_t)(HSADC_CLIST3_SAMPLE8_MASK)) |
                                           HSADC_CLIST3_SAMPLE8(init->ui16AdcArray[MCDRV_ADC1_BEMFC] + 8));
        this->ui16RsltNumBemfC = 8;

        /*IDCB */

        this->pui32IdcbAdcBase = init->pui32Adc0Base;
        this->pui32IdcbAdcBase->CLIST1 = ((this->pui32IdcbAdcBase->CLIST1 & ~(uint16_t)(HSADC_CLIST1_SAMPLE0_MASK)) |
                                          HSADC_CLIST1_SAMPLE0(init->ui16AdcArray[MCDRV_ADC0_IDCB]));
        this->ui16RsltNumIdcb = 0;
        this->pui32IdcbAdcBase->MUX67_SEL = HSADC_MUX67_SEL_CH7_SELA(0);

        /* auxiliary measurement */
        this->pui32AuxAdcBase = init->pui32Adc0Base;
        this->pui32AuxAdcBase->CLIST1 = ((this->pui32AuxAdcBase->CLIST1 & ~(uint16_t)(HSADC_CLIST1_SAMPLE3_MASK)) |
                                         HSADC_CLIST1_SAMPLE3(init->ui16AdcArray[MCDRV_ADC0_AUX]));

        this->ui16RsltNumAux = 3;

        /* DC-bus voltage measurement */
        this->pui32UdcbAdcBase = init->pui32Adc0Base;
        this->pui32UdcbAdcBase->CLIST1 = ((this->pui32UdcbAdcBase->CLIST1 & ~(uint16_t)(HSADC_CLIST1_SAMPLE1_MASK)) |
                                          HSADC_CLIST1_SAMPLE1(init->ui16AdcArray[MCDRV_ADC0_UDCB]));

        this->ui16RsltNumUdcb = 1;
        this->pui32UdcbAdcBase->MUX67_SEL = HSADC_MUX67_SEL_CH6_SELA(0);
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
bool_t MCDRV_AssignBemfChannel(mcdrv_hsadc_t *this)
{
    s_statusPass = TRUE;

    switch (this->ui16Sector)
    {
        /* BEMF phase C sensing */
        case 0:
        case 3:
            this->pui32BemfCAdcBase->CLIST3 = ((this->pui32BemfCAdcBase->CLIST3 & ~(uint16_t)(HSADC_CLIST3_SAMPLE8_MASK)) |
                                               HSADC_CLIST3_SAMPLE8(this->pui16AdcArray[MCDRV_ADC1_BEMFC] + 8));

            break;

        /* BEMF phase B sensing */
        case 1:
        case 4:
            this->pui32BemfBAdcBase->CLIST1 = ((this->pui32BemfBAdcBase->CLIST1 & ~(uint16_t)(HSADC_CLIST1_SAMPLE2_MASK)) |
                                               HSADC_CLIST1_SAMPLE2(this->pui16AdcArray[MCDRV_ADC0_BEMFB]));

            break;

        /* BEMF phase A sensing */
        case 2:
        case 5:
            this->pui32BemfAAdcBase->CLIST1 = ((this->pui32BemfAAdcBase->CLIST1 & ~(uint16_t)(HSADC_CLIST1_SAMPLE2_MASK)) |
                                               HSADC_CLIST1_SAMPLE2(this->pui16AdcArray[MCDRV_ADC0_BEMFA]));

            break;

        default:
            break;
    }

    return (s_statusPass);
}

/*!
 * @brief Function reads ADC result register containing actual BEMF voltage
 *
 * Result register value is shifted three times to the right and stored
 * to BEMF voltage pointer
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_BemfVoltageGet(mcdrv_hsadc_t *this)
{
    s_statusPass = TRUE;

    switch (this->ui16Sector)
    {
        /* BEMF phase C sensing */
        case 0:
        case 3:
            *this->pf16BemfVoltage = (frac16_t)(this->pui32Adc1Base->RSLT[this->ui16RsltNumBemfC]);
            break;

        /* BEMF phase B sensing */
        case 1:
        case 4:
            *this->pf16BemfVoltage = (frac16_t)(this->pui32Adc0Base->RSLT[this->ui16RsltNumBemf]);
            break;

        /* BEMF phase A sensing */
        case 2:
        case 5:
            *this->pf16BemfVoltage = (frac16_t)(this->pui32Adc0Base->RSLT[this->ui16RsltNumBemf]);
            break;

        default:
            break;
    }

    return (s_statusPass);
}

/*!
 * @brief Function reads ADC result register containing actual DC-bus voltage sample
 *
 * Result register value is shifted three times to the right and stored
 * to DC-bus voltage pointer
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_VoltDcBusGet(mcdrv_hsadc_t *this)
{
    s_statusPass = TRUE;
    /* read DC-bus voltage sample from defined ADCx result register */
    *this->pf16UDcBus = (frac16_t)(this->pui32UdcbAdcBase->RSLT[this->ui16RsltNumUdcb]);
    return (s_statusPass);
}

/*!
 * @brief Function reads ADC result register containing actual DC-bus current sample
 *
 * Result register value is shifted three times to the right and stored
 * to DC-bus current pointer
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrDcBusGet(mcdrv_hsadc_t *this)
{
    s_statusPass = TRUE;
    *this->pf16IDcBus =
        (frac16_t)((int16_t)(this->pui32IdcbAdcBase->RSLT[this->ui16RsltNumIdcb]) - this->ui16OffsetDcCurr);

    return (s_statusPass);
}

/*!
 * @brief Function reads ADC result register containing actual auxiliary sample
 *
 * Result register value is shifted 3 times right and stored to
 * auxiliary pointer
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AuxValGet(mcdrv_hsadc_t *this)
{
    s_statusPass = TRUE;

    /* read auxiliary channel sample from defined ADCx result register */
    *this->pui16AuxChan = (frac16_t)(this->pui32AuxAdcBase->RSLT[this->ui16RsltNumAux]);

    return (s_statusPass);
}

/*!
 * @brief Function initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrOffsetCalibInit(mcdrv_hsadc_t *this)
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
bool_t MCDRV_CurrOffsetCalib(mcdrv_hsadc_t *this)
{
    s_statusPass = TRUE;

    /* sensing of DC Bus Current offset */
    this->ui16CalibDcCurr =
        GDFLIB_FilterMA_F16((frac16_t) this->pui32IdcbAdcBase->RSLT[this->ui16RsltNumIdcb], &this->ui16FiltDcCurr);

    return (s_statusPass);
}

/*!
 * @brief Function passes measured offset values to main structure
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrOffsetCalibSet(mcdrv_hsadc_t *this)
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
bool_t MCDRV_AssignDCBusChannel(mcdrv_hsadc_t *this)
{
    s_statusPass = TRUE;

    return (s_statusPass);
}

