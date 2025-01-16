/*
 * Copyright 2013 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
 */

#ifndef _MCDRV_ADCETC_IMXRT11xx_H_
#define _MCDRV_ADCETC_IMXRT11xx_H_

#include "gdflib.h"
#include "fsl_adc_etc.h"
#include "fsl_lpadc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* init sensors/actuators pointers */
#define M1_SET_PTR_U_DC_BUS(par1) (g_sM1AdcSensor.pf16UDcBus = &(par1))
#define M1_SET_PTR_I_DC_BUS(par1) (g_sM1AdcSensor.pf16IDcBus = &(par1))
#define M1_SET_PTR_BEMF_VOLT(par1) (g_sM1AdcSensor.pf16BemfVoltage = &(par1))
#define M1_SET_PTR_AUX_CHAN(par1) (g_sM1AdcSensor.pui16AuxChan = &(par1))

/* BLDC table for control ADC phase assigment for measurement */
typedef struct _bldc_adc_table
{    
    uint32_t ui16BemfChannelNumber[6];                          /* ADC channel numbers for BEMF measurement */
    lpadc_sample_channel_mode_t ui16BemfChannelMode[6];         /* ADC channel modes/sides for BEMF measurement*/
    
} bldc_adc_table_t; 

typedef struct _mcdrv_adcetc
{
    GDFLIB_FILTER_MA_T_A32 ui16FiltDcCurr; /* Dc-bus current offset filter */

    uint16_t ui16OffsetFiltWindow;         /* ADC offset filter window */
    uint16_t ui16OffsetDcCurr;             /* Dc-bus current offset */
    uint16_t ui16CalibDcCurr;              /* Dc-bus current offset calibration */
    frac16_t *pf16BemfVoltage;             /* pointer to actual BEMF voltage     */
    frac16_t *pf16IDcBus;                  /* pointer to actual IDC bus current     */
    frac16_t *pf16UDcBus;                  /* pointer to actual UDC bus voltage     */
    uint16_t *pui16AuxChan;                /* pointer to actual auxiliary variable   */
    uint16_t ui16Sector;                   /* commutation sector */
    bldc_adc_table_t bldcAdcTable;         /* BLDC table for control ADC phase assigment for measurement */
    
} mcdrv_adcetc_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Function reads and passes DCB voltage sample
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_VoltDcBusGet(mcdrv_adcetc_t *this);

/*!
 * @brief Function reads and passes auxiliary sample
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_AuxValGet(mcdrv_adcetc_t *this);

/*!
 * @brief Function reads ADC result register containing actual DC-bus current sample
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_CurrDcBusGet(mcdrv_adcetc_t *this);

/*!
 * @brief Function set new channel assignment for next BEMF voltage sensing
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_AssignBemfChannel(mcdrv_adcetc_t *this);

/*!
 * @brief Function reads ADC result register containing actual BEMF voltage
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_BemfVoltageGet(mcdrv_adcetc_t *this);

/*!
 * @brief Function initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_CurrOffsetCalibInit(mcdrv_adcetc_t *this);

/*!
 * @brief Function reads current samples and filter them
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_CurrOffsetCalib(mcdrv_adcetc_t *this);

/*!
 * @brief Function passes measured offset values to main structure
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_CurrOffsetCalibSet(mcdrv_adcetc_t *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_ADCETC_IMXRT11xx_H_ */
