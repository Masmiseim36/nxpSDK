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

#ifndef _MCDRV_ADC_LPC_H_
#define _MCDRV_ADC_LPC_H_

#include "fsl_lpadc.h"

#include "gdflib.h"
#include "mlib_types.h"
#include "gmclib.h"
#include "fsl_device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MCDRV_ADC (3)

/* init sensors/actuators pointers */
#define M1_SET_PTR_U_DC_BUS(par1) (g_sM1AdcSensor.pf16UDcBus = &(par1))
#define M1_SET_PTR_I_ABC(par1) (g_sM1AdcSensor.psIABC = &(par1))
#define M1_SET_PTR_SECTOR(par1) (g_sM1AdcSensor.pui16SVMSector = &(par1))
#define M1_SET_PTR_AUX_CHAN(par1) (g_sM1AdcSensor.pui16AuxChan = &(par1))

typedef struct _pha_ab
{
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaA; /* phase A offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaB; /* phase B offset filter */
    uint16_t ui16CalibPhaA;              /* phase A offset calibration */
    uint16_t ui16CalibPhaB;              /* phase B offset calibration */
    uint16_t ui16OffsetPhaA;             /* phase A offset result */
    uint16_t ui16OffsetPhaB;             /* phase B offset result */

} pha_ab_t;

typedef struct _pha_bc
{
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaB; /* phase B offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaC; /* phase B offset filter */
    uint16_t ui16CalibPhaB;              /* phase B offset calibration */
    uint16_t ui16CalibPhaC;              /* phase B offset calibration */
    uint16_t ui16OffsetPhaB;             /* phase B offset result */
    uint16_t ui16OffsetPhaC;             /* phase B offset result */

} pha_bc_t;

typedef struct _pha_ac
{
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaA; /* phase A offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaC; /* phase B offset filter */

    uint16_t ui16CalibPhaA;  /* phase A offset calibration */
    uint16_t ui16CalibPhaC;  /* phase B offset calibration */
    uint16_t ui16OffsetPhaA; /* phase A offset result */
    uint16_t ui16OffsetPhaC; /* phase B offset result */

} pha_ac_t;

typedef struct _mcdrv_adc
{
    GMCLIB_3COOR_T_F16 *psIABC; /* pointer to the 3-phase currents */
    pha_bc_t sCurrSec16;        /* ADC setting for SVM sectors 1&6 */
    pha_ac_t sCurrSec23;        /* ADC setting for SVM sectors 2&3 */
    pha_ab_t sCurrSec45;        /* ADC setting for SVM sectors 4&5 */

    uint16_t *pui16SVMSector; /* pointer to the SVM sector */
    frac16_t *pui16AuxChan;   /* pointer to auxiliary ADC channel number */
    frac16_t *pf16UDcBus;     /* pointer to DC Bus voltage variable */

    uint16_t ui16OffsetFiltWindow; /* ADC Offset filter window */

    uint16_t ui16AdcCurrA;
    uint16_t ui16AdcCurrB;
    uint16_t ui16AdcCurrC;
    uint16_t ui16AdcDCBVolt;

    lpadc_conv_result_t s_ADC_ResultStructure;
} mcdrv_adc_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Reads and calculates 3 phase samples based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_CurrAndVoltDcBusGet(mcdrv_adc_t *this);

/*!
 * @brief Initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_Curr3Ph2ShCalibInit(mcdrv_adc_t *this);

/*!
 * @brief Function reads current samples and filter them based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_Curr3Ph2ShCalib(mcdrv_adc_t *this);

/*!
 * @brief Function passes measured offset values to main structure
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_Curr3Ph2ShCalibSet(mcdrv_adc_t *this);

/*!
 * @brief Function reads and passes DCB voltage sample
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_VoltDcBusGet(mcdrv_adc_t *this);

/*!
 * @brief Function reads and passes auxiliary sample
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
void MCDRV_AuxValGet(mcdrv_adc_t *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_ADC_LPC_H_ */
