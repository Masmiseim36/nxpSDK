/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "mcdrv_adc_adc16.h"
#include "mcdrv_adc_adc16_frdm_ke16.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
* Variables
******************************************************************************/

static bool_t s_statusPass;
static volatile bool_t s_bkey = TRUE;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Initializes ADC driver to measure DC-bus current, DC-bus voltage
 *        and BEMF voltage for BLDC sensorless algorithm
 *
 *  Board specific file for the tower kv11
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Adc16Init_frdm_ke16(mcdrv_adc16_t *this, mcdrv_adc16_init_t *init)
{
    s_statusPass = TRUE;

    /* offset filter window */
    this->ui16OffsetFiltWindow = 3;

    /* pointer to array with the channel numbers */
    this->pui16AdcArray = init->ui16AdcArray;

    /* default DC-bus current offset */
    this->ui16OffsetDcCurr = 0x3fff;

    /* ADC module result register assignment */
    /* 0 -> result register 0 for ADC0 */
    /* 1 -> result register 1 for ADC0 */
    /* 2 -> result register 2 for ADC0 */
    this->ui16IndexBemf = 0;
    this->ui16IndexUdcb = 1;
    this->ui16IndexIdcb = 2;

    /* BEMF phase A measurement */
    this->pui32BemfAAdcBase = init->pui32Adc0Base;
    this->bldcAdc0SectorCfg[2] = init->ui16AdcArray[MCDRV_ADC0_BEMFA];
    this->bldcAdcSelCfg[2] = init->pui32Adc0Base;
    this->bldcAdc0SectorCfg[5] = init->ui16AdcArray[MCDRV_ADC0_BEMFA];
    this->bldcAdcSelCfg[5] = init->pui32Adc0Base;

    /* BEMF phase B measurement */
    this->pui32BemfBAdcBase = init->pui32Adc0Base;
    this->bldcAdc0SectorCfg[1] = init->ui16AdcArray[MCDRV_ADC0_BEMFB];
    this->bldcAdcSelCfg[1] = init->pui32Adc0Base;
    this->bldcAdc0SectorCfg[4] = init->ui16AdcArray[MCDRV_ADC0_BEMFB];
    this->bldcAdcSelCfg[4] = init->pui32Adc0Base;

    /* BEMF phase C measurement */
    this->pui32BemfCAdcBase = init->pui32Adc0Base;
    this->bldcAdc0SectorCfg[0] = init->ui16AdcArray[MCDRV_ADC0_BEMFC];
    this->bldcAdcSelCfg[0] = init->pui32Adc0Base;
    this->bldcAdc0SectorCfg[3] = init->ui16AdcArray[MCDRV_ADC0_BEMFC];
    this->bldcAdcSelCfg[3] = init->pui32Adc0Base;
    
    /* Set ADC_SC1_ADCH bitfield for BEMFC */
    this->pui32BemfCAdcBase->SC1[this->ui16IndexBemf] =
        (this->pui32BemfCAdcBase->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
        (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC0_BEMFC]));

    /* DC-bus current measurement */
    this->pui32IdcbAdcBase = init->pui32Adc0Base;
    /* Set ADC_SC1_ADCH bitfield */
    this->pui32IdcbAdcBase->SC1[this->ui16IndexIdcb] =
        (this->pui32IdcbAdcBase->SC1[this->ui16IndexIdcb] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
        (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC0_IDCB]));

    /* DC-bus voltage measurement */
    this->pui32UdcbAdcBase = init->pui32Adc0Base;
    /* Set ADC_SC1_ADCH bitfield */
    this->pui32UdcbAdcBase->SC1[this->ui16IndexUdcb] =
        (this->pui32UdcbAdcBase->SC1[this->ui16IndexUdcb] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
        (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC0_UDCB]));

    return (s_statusPass);
}

/*!
 * @brief Function set new channel assignment for next BEMF voltage sensing
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AssignBemfChannel_frdm_ke16(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    switch (this->ui16Sector)
    {
        /* BEMF phase C sensing */
        case 0:
        case 3:
            if ((this->bldcAdcSelCfg[this->ui16Sector]) == ADC0)
                /* Set ADC_SC1_ADCH bitfield */
                this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] =
                    (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] &
                     ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                    (ADC_SC1_ADCH(this->bldcAdc0SectorCfg[this->ui16Sector]));
            else
                /* Set ADC_SC1_ADCH bitfield */
                this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] =
                    (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] &
                     ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                    (ADC_SC1_ADCH(this->bldcAdc1SectorCfg[this->ui16Sector]));
            break;

        /* BEMF phase B sensing */
        case 1:
        case 4:
            if ((this->bldcAdcSelCfg[this->ui16Sector]) == ADC0)
                /* Set ADC_SC1_ADCH bitfield */
                this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] =
                    (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] &
                     ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                    (ADC_SC1_ADCH(this->bldcAdc0SectorCfg[this->ui16Sector]));
            else
                /* Set ADC_SC1_ADCH bitfield */
                this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] =
                    (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] &
                     ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                    (ADC_SC1_ADCH(this->bldcAdc1SectorCfg[this->ui16Sector]));
            break;

        /* BEMF phase A sensing */
        case 2:
        case 5:
            if ((this->bldcAdcSelCfg[this->ui16Sector]) == ADC0)
                /* Set ADC_SC1_ADCH bitfield */
                this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] =
                    (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] &
                     ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                    (ADC_SC1_ADCH(this->bldcAdc0SectorCfg[this->ui16Sector]));
            else
                /* Set ADC_SC1_ADCH bitfield */
                this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] =
                    (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] &
                     ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                    (ADC_SC1_ADCH(this->bldcAdc1SectorCfg[this->ui16Sector]));
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
bool_t MCDRV_BemfVoltageGet_frdm_ke16(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    switch (this->ui16Sector)
    {
        /* BEMF phase C sensing */
        case 0:
        case 3:
            *this->pf16BemfVoltage =
                (frac16_t)(MLIB_ShLSat_F16((this->bldcAdcSelCfg[this->ui16Sector]->R[this->ui16IndexBemf]), 3));
            break;

        /* BEMF phase B sensing */
        case 1:
        case 4:
            *this->pf16BemfVoltage =
                (frac16_t)(MLIB_ShLSat_F16((this->bldcAdcSelCfg[this->ui16Sector]->R[this->ui16IndexBemf]), 3));
            break;

        /* BEMF phase A sensing */
        case 2:
        case 5:
            *this->pf16BemfVoltage =
                (frac16_t)(MLIB_ShLSat_F16((this->bldcAdcSelCfg[this->ui16Sector]->R[this->ui16IndexBemf]), 3));
            break;

        /* default */
        default:
            *this->pf16BemfVoltage =
                (frac16_t)(MLIB_ShLSat_F16((this->bldcAdcSelCfg[this->ui16Sector]->R[this->ui16IndexBemf]), 3));
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
bool_t MCDRV_VoltDcBusGet_frdm_ke16(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    /* read DC-bus voltage sample from defined ADCx result register */
    *this->pf16UDcBus = (frac16_t)(MLIB_ShLSat_F16((this->pui32UdcbAdcBase->R[this->ui16IndexUdcb]), 3));
    
    return (s_statusPass);
}

/*!
 * @brief Function reads ADC result register containing actual DC-bus current sample
 *
 * Result register value is shifted three times to the right and stored
 * to DC-bus current pointer
 *0
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrDcBusGet_frdm_ke16(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;
    *this->pf16IDcBus =
        (frac16_t)(MLIB_ShLSat_F16((this->pui32IdcbAdcBase->R[this->ui16IndexIdcb] - this->ui16OffsetDcCurr), 3));

    return (s_statusPass);
}

/*!
 * @brief Function reads current samples and filter them
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrOffsetCalib_frdm_ke16(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    /* sensing of DC Bus Current offset */
    this->ui16CalibDcCurr =
        GDFLIB_FilterMA_F16((frac16_t) this->pui32IdcbAdcBase->R[this->ui16IndexIdcb], &this->ui16FiltDcCurr);

    return (s_statusPass);
}

/*!
 * @brief Function initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrOffsetCalibInit_frdm_ke16(mcdrv_adc16_t *this)
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
 * @brief Function passes measured offset values to main structure
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrOffsetCalibSet_frdm_ke16(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    /* pass Calib data for DC Bus current offset */
    this->ui16OffsetDcCurr = (this->ui16CalibDcCurr);

    return (s_statusPass);
}

/*!
 * @brief Function set new channel assignment for next BEMF voltage sensing
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AssignDCBusChannel(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    this->pui32IdcbAdcBase->SC1[this->ui16IndexIdcb] =
        (this->pui32IdcbAdcBase->SC1[this->ui16IndexIdcb] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
        (ADC_SC1_ADCH(this->pui16AdcArray[MCDRV_ADC0_IDCB]));

    return (s_statusPass);
}

