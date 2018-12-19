/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_adc_adc16.h"
#include "mcdrv_adc_adc16_twr_ke18.h"

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
 * @brief Initializes ADC driver to measure DC-bus current, DC-bus voltage
 *        and BEMF voltage for BLDC sensorless algorithm
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Adc16Init_twr_ke18(mcdrv_adc16_t *this, mcdrv_adc16_init_t *init)
{
    s_statusPass = TRUE;

    /* offset filter window */
    this->ui16OffsetFiltWindow = 3;

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
        /* 0 -> result register 0 for ADC0 or ADC2 */
        /* 1 -> result register 1 for ADC0 or ADC2 */
        /* 2 -> result register 2 for ADC2*/
        this->ui16IndexBemf = 0; /* ADC0 and ADC2 */
        this->ui16IndexAux = 0;  /* ADC0 and ADC2 */
        this->ui16IndexUdcb = 1; /* ADC2          */
        this->ui16IndexIdcb = 2; /* ADC2          */

        /* BEMF phase A measurement */
        if (init->ui16AdcArray[MCDRV_ADC0_BEMFA] == MCDRV_CHAN_OFF)
        {
            this->pui32BemfAAdcBase = init->pui32Adc1Base;
            this->bldcAdc1SectorCfg[2] = init->ui16AdcArray[MCDRV_ADC1_BEMFA];
            this->bldcAdcSelCfg[2] = init->pui32Adc1Base;
            this->bldcAdc1SectorCfg[5] = init->ui16AdcArray[MCDRV_ADC1_BEMFA];
            this->bldcAdcSelCfg[5] = init->pui32Adc1Base;
            /* Set ADC_SC1_ADCH bitfield */
        }
        else
        {
            this->pui32BemfAAdcBase = init->pui32Adc0Base;
            this->bldcAdc0SectorCfg[2] = init->ui16AdcArray[MCDRV_ADC0_BEMFA];
            this->bldcAdcSelCfg[2] = init->pui32Adc0Base;
            this->bldcAdc0SectorCfg[5] = init->ui16AdcArray[MCDRV_ADC0_BEMFA];
            this->bldcAdcSelCfg[5] = init->pui32Adc0Base;
        }

        /* BEMF phase B measurement */
        if (init->ui16AdcArray[MCDRV_ADC0_BEMFB] == MCDRV_CHAN_OFF)
        {
            this->pui32BemfBAdcBase = init->pui32Adc1Base;
            this->bldcAdc1SectorCfg[1] = init->ui16AdcArray[MCDRV_ADC1_BEMFB];
            this->bldcAdcSelCfg[1] = init->pui32Adc1Base;
            this->bldcAdc1SectorCfg[4] = init->ui16AdcArray[MCDRV_ADC1_BEMFB];
            this->bldcAdcSelCfg[4] = init->pui32Adc1Base;
        }
        else
        {
            this->pui32BemfBAdcBase = init->pui32Adc0Base;
            this->bldcAdc0SectorCfg[1] = init->ui16AdcArray[MCDRV_ADC0_BEMFB];
            this->bldcAdcSelCfg[1] = init->pui32Adc0Base;
            this->bldcAdc0SectorCfg[4] = init->ui16AdcArray[MCDRV_ADC0_BEMFB];
            this->bldcAdcSelCfg[4] = init->pui32Adc0Base;
        }

        /* BEMF phase C measurement */
        if (init->ui16AdcArray[MCDRV_ADC0_BEMFC] == MCDRV_CHAN_OFF)
        {
            this->pui32BemfCAdcBase = init->pui32Adc1Base;
            this->bldcAdc1SectorCfg[0] = init->ui16AdcArray[MCDRV_ADC1_BEMFC];
            this->bldcAdcSelCfg[0] = init->pui32Adc1Base;
            this->bldcAdc1SectorCfg[3] = init->ui16AdcArray[MCDRV_ADC1_BEMFC];
            this->bldcAdcSelCfg[3] = init->pui32Adc1Base;
            /* Set ADC_SC1_ADCH bitfield */
            this->pui32BemfCAdcBase->SC1[this->ui16IndexBemf] =
                (this->pui32BemfCAdcBase->SC1[this->ui16IndexBemf] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC1_BEMFC]));
        }
        else
        {
            this->pui32BemfCAdcBase = init->pui32Adc0Base;
            this->bldcAdc0SectorCfg[0] = init->ui16AdcArray[MCDRV_ADC0_BEMFC];
            this->bldcAdcSelCfg[0] = init->pui32Adc0Base;
            this->bldcAdc0SectorCfg[3] = init->ui16AdcArray[MCDRV_ADC0_BEMFC];
            this->bldcAdcSelCfg[3] = init->pui32Adc0Base;
            /* Set ADC_SC1_ADCH bitfield */
            this->pui32BemfCAdcBase->SC1[this->ui16IndexBemf] =
                (this->pui32BemfCAdcBase->SC1[this->ui16IndexBemf] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC0_BEMFC]));
        }

        /* DC-bus current measurement */
        if (init->ui16AdcArray[MCDRV_ADC0_IDCB] == MCDRV_CHAN_OFF)
        {
            this->pui32IdcbAdcBase = init->pui32Adc1Base;
            /* Set ADC_SC1_ADCH bitfield */
            this->pui32IdcbAdcBase->SC1[this->ui16IndexIdcb] =
                (this->pui32IdcbAdcBase->SC1[this->ui16IndexIdcb] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC1_IDCB]));
        }
        else
        {
            this->pui32IdcbAdcBase = init->pui32Adc0Base;
            /* Set ADC_SC1_ADCH bitfield */
            this->pui32IdcbAdcBase->SC1[this->ui16IndexIdcb] =
                (this->pui32IdcbAdcBase->SC1[this->ui16IndexIdcb] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC0_IDCB]));
        }

        /* DC-bus voltage measurement */
        if (init->ui16AdcArray[MCDRV_ADC0_UDCB] == MCDRV_CHAN_OFF)
        {
            this->pui32UdcbAdcBase = init->pui32Adc1Base;
            /* Set ADC_SC1_ADCH bitfield */
            this->pui32UdcbAdcBase->SC1[this->ui16IndexUdcb] =
                (this->pui32UdcbAdcBase->SC1[this->ui16IndexUdcb] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC1_UDCB]));
        }
        else
        {
            this->pui32UdcbAdcBase = init->pui32Adc0Base;
            /* Set ADC_SC1_ADCH bitfield */
            this->pui32UdcbAdcBase->SC1[this->ui16IndexUdcb] =
                (this->pui32UdcbAdcBase->SC1[this->ui16IndexUdcb] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC0_UDCB]));
        }

        this->pui32AuxAdcBase = init->pui32Adc1Base;
        /* Set ADC_SC1_ADCH bitfield */
        this->pui32AuxAdcBase->SC1[this->ui16IndexAux] =
            (this->pui32AuxAdcBase->SC1[this->ui16IndexAux] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
            (ADC_SC1_ADCH(MCDRV_TEMP));

        /* trigger */
        ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
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
bool_t MCDRV_AssignBemfChannel_twr_ke18(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    switch (this->ui16Sector)
    {
        /* BEMF phase C sensing */
        case 0:
        case 3:
            if ((this->bldcAdcSelCfg[this->ui16Sector]) == ADC0)
            {
                /* Set ADC_SC1_ADCH bitfield */
                this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] =
                    (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] &
                     ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                    (ADC_SC1_ADCH(this->bldcAdc0SectorCfg[this->ui16Sector]));

                this->pui32AuxAdcBase = this->pui32UdcbAdcBase;
                /* Set ADC_SC1_ADCH bitfield */
                this->pui32AuxAdcBase->SC1[this->ui16IndexAux] =
                    (this->pui32AuxAdcBase->SC1[this->ui16IndexAux] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                    (ADC_SC1_ADCH(MCDRV_TEMP));
            }
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
            {
                /* Set ADC_SC1_ADCH bitfield */
                this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] =
                    (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] &
                     ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                    (ADC_SC1_ADCH(this->bldcAdc1SectorCfg[this->ui16Sector]));

                this->pui32AuxAdcBase = ADC0;
                /* Set ADC_SC1_ADCH bitfield */
                this->pui32AuxAdcBase->SC1[this->ui16IndexAux] =
                    (this->pui32AuxAdcBase->SC1[this->ui16IndexAux] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                    (ADC_SC1_ADCH(MCDRV_TEMP));
            }
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
            {
                /* Set ADC_SC1_ADCH bitfield */
                this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] =
                    (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] &
                     ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                    (ADC_SC1_ADCH(this->bldcAdc1SectorCfg[this->ui16Sector]));

                this->pui32AuxAdcBase = ADC0;
                /* Set ADC_SC1_ADCH bitfield */
                this->pui32AuxAdcBase->SC1[this->ui16IndexAux] =
                    (this->pui32AuxAdcBase->SC1[this->ui16IndexAux] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) |
                    (ADC_SC1_ADCH(MCDRV_TEMP));
            }
            break;

        default:
            break;
    }

    return (s_statusPass);
}

