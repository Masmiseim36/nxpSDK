/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MCDRV_ADC_ADC16_H_
#define _MCDRV_ADC_ADC16_H_

#include "fsl_device_registers.h"
#include "gdflib.h"
#include "gmclib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MCDRV_ADC16 (1)

#define MCDRV_CHAN_OFF (0x1F)
#define MCDRV_ADC0_IA (0)
#define MCDRV_ADC1_IA (1)
#define MCDRV_ADC0_IB (2)
#define MCDRV_ADC1_IB (3)
#define MCDRV_ADC0_IC (4)
#define MCDRV_ADC1_IC (5)
#define MCDRV_ADC0_UDCB (6)
#define MCDRV_ADC1_UDCB (7)
#define MCDRV_ADC0_AUX (8)
#define MCDRV_ADC1_AUX (9)

/* init sensors/actuators pointers */
#define M1_SET_PTR_U_DC_BUS(par1) (g_sM1AdcSensor.pf16UDcBus = &(par1))
#define M1_SET_PTR_I_ABC(par1) (g_sM1AdcSensor.psIABC = &(par1))
#define M1_SET_PTR_SECTOR(par1) (g_sM1AdcSensor.pui16SVMSector = &(par1))
#define M1_SET_PTR_AUX_CHAN(par1) (g_sM1AdcSensor.pui16AuxChan = &(par1))

typedef struct _pha_bc
{
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaB; /* Phase B offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaC; /* Phase C offset filter */
    uint16_t ui16ChanNumPhaB;            /* Phase B channel number */
    uint16_t ui16ChanNumPhaC;            /* Phase C channel number */
    uint16_t ui16CalibPhaB;              /* Phase B offset calibration */
    uint16_t ui16CalibPhaC;              /* Phase C offset calibration */
    uint16_t ui16OffsetPhaB;             /* Phase B offset result */
    uint16_t ui16OffsetPhaC;             /* Phase C offset result */
    ADC_Type *pui32AdcBasePhaB;          /* Pointer to Phase B base address */
    ADC_Type *pui32AdcBasePhaC;          /* Pointer to Phase C base address */
} pha_bc_t;

typedef struct _pha_ac
{
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaA; /* Phase A offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaC; /* Phase C offset filter */
    uint16_t ui16ChanNumPhaA;            /* Phase A channel number */
    uint16_t ui16ChanNumPhaC;            /* Phase C channel number */
    uint16_t ui16CalibPhaA;              /* Phase A offset calibration */
    uint16_t ui16CalibPhaC;              /* Phase C offset calibration */
    uint16_t ui16OffsetPhaA;             /* Phase A offset result */
    uint16_t ui16OffsetPhaC;             /* Phase C offset result */
    ADC_Type *pui32AdcBasePhaA;          /* Pointer to phase A base address */
    ADC_Type *pui32AdcBasePhaC;          /* Pointer to phase C base address */
} pha_ac_t;

typedef struct _pha_ab
{
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaA; /* Phase A offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaB; /* Phase B offset filter */
    uint16_t ui16ChanNumPhaA;            /* Phase A channel number */
    uint16_t ui16ChanNumPhaB;            /* Phase B channel number */
    uint16_t ui16CalibPhaA;              /* Phase A offset calibration */
    uint16_t ui16CalibPhaB;              /* Phase B offset calibration */
    uint16_t ui16OffsetPhaA;             /* Phase A offset result */
    uint16_t ui16OffsetPhaB;             /* Phase B offset result */
    ADC_Type *pui32AdcBasePhaA;          /* Pointer to phase A base address */
    ADC_Type *pui32AdcBasePhaB;          /* Pointer to phase B base address */
} pha_ab_t;

typedef struct _mcdrv_adc16
{
    GMCLIB_3COOR_T_F16 *psIABC; /* Pointer to the 3-phase currents */
    pha_bc_t sCurrSec16;        /* ADC setting for SVM sectors 1&6 */
    pha_ac_t sCurrSec23;        /* ADC setting for SVM sectors 2&3 */
    pha_ab_t sCurrSec45;        /* ADC setting for SVM sectors 4&5 */

    ADC_Type *pui32UdcbAdcBase; /* Pointer to ADC where Udcb channel is assigned */
    uint16_t ui16ChanNumVDcb;   /* DCB voltage channel number */
    ADC_Type *pui32AuxAdcBase;  /* Pointer to ADC where auxiliary channel is assigned */
    uint16_t ui16ChanNumAux;    /* Auxiliary channel number */
    uint16_t *pui16SVMSector;   /* Pointer to the SVM sector */
    frac16_t *pui16AuxChan;     /* Pointer to auxiliary ADC channel number */
    frac16_t *pf16UDcBus;       /* Pointer to DC Bus voltage variable */

    uint16_t ui16OffsetFiltWindow; /* ADC Offset filter window */
} mcdrv_adc16_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Function reads and calculates 3 phase samples based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShGet(mcdrv_adc16_t *this);

/*!
 * @brief Set initial channel assignment for phase currents & DCB voltage
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShChanAssignInit(mcdrv_adc16_t *this);

/*!
 * @brief Function set new channel assignment for next sampling based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShChanAssign(mcdrv_adc16_t *this);

/*!
 * @brief Function initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalibInit(mcdrv_adc16_t *this);

/*!
 * @brief Function reads current samples and filter them based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalib(mcdrv_adc16_t *this);

/*!
 * @brief Function passes measured offset values to main structure
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalibSet(mcdrv_adc16_t *this);

/*!
 * @brief Function reads and passes DCB voltage sample
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_VoltDcBusGet(mcdrv_adc16_t *this);

/*!
 * @brief Function reads and passes auxiliary sample
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AuxValGet(mcdrv_adc16_t *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_ADC_ADC16_H_ */

