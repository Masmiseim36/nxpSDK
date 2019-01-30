/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MCDRV_ADC_ADC12_H_
#define _MCDRV_ADC_ADC12_H_

#include "gdflib.h"
#include "mlib_types.h"
#include "gmclib.h"
#include "fsl_device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MCDRV_ADC12 (2)

/* macros for ADC module assignment, correspond to first Samples & results (0&8)
 * ADC0 - samples 0-7
 * ADC1 - samples 8-15
 */
#define ADC0 (0)
#define ADC1 (8)

#define MCDRV_CHAN_OFF (0x1F)

/* init sensors/actuators pointers */
#define M1_SET_PTR_U_DC_BUS(par1) (g_sM1AdcSensor.pf16UDcBus = &(par1))
#define M1_SET_PTR_I_ABC(par1) (g_sM1AdcSensor.psIABC = &(par1))
#define M1_SET_PTR_SECTOR(par1) (g_sM1AdcSensor.pui16SVMSector = &(par1))
#define M1_SET_PTR_AUX_CHAN(par1) (g_sM1AdcSensor.pui16AuxChan = &(par1))

typedef struct _pha_bc
{
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaB; /* phase B offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaC; /* phase C offset filter */
    uint16_t ui16ChanNumPhaB;            /* phase B channel number */
    uint16_t ui16ChanNumPhaC;            /* phase C channel number */
    uint16_t ui16AdcSmplPhaB;            /* phase B ADC sample number */
    uint16_t ui16AdcSmplPhaC;            /* phase C ADC sample number */        
    uint16_t ui16AdcNumPhaB;             /* phase B ADC module number */
    uint16_t ui16AdcNumPhaC;             /* phase C ADC module number */
    uint16_t ui16CalibPhaB;              /* phase B offset calibration */
    uint16_t ui16CalibPhaC;              /* phase C offset calibration */
    uint16_t ui16OffsetPhaB;             /* phase B offset result */
    uint16_t ui16OffsetPhaC;             /* phase C offset result */
    uint16_t *pui16ClistPhaB;            /* pointer to phase B clist register */
    uint16_t *pui16ClistPhaC;            /* pointer to phase C clist register */
} pha_bc_t;

typedef struct _pha_ac
{
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaA; /* phase A offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaC; /* phase C offset filter */
    uint16_t ui16ChanNumPhaA;            /* phase A channel number */
    uint16_t ui16ChanNumPhaC;            /* phase C channel number */
    uint16_t ui16AdcSmplPhaA;            /* phase A ADC sample number */
    uint16_t ui16AdcSmplPhaC;            /* phase C ADC sample number */        
    uint16_t ui16AdcNumPhaA;             /* phase A ADC module number */
    uint16_t ui16AdcNumPhaC;             /* phase C ADC module number */
    uint16_t ui16CalibPhaA;              /* phase A offset calibration */
    uint16_t ui16CalibPhaC;              /* phase C offset calibration */
    uint16_t ui16OffsetPhaA;             /* phase A offset result */
    uint16_t ui16OffsetPhaC;             /* phase C offset result */
    uint16_t *pui16ClistPhaA;            /* pointer to phase A clist register */
    uint16_t *pui16ClistPhaC;            /* pointer to phase C clist register */
} pha_ac_t;

typedef struct _pha_ab
{
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaA; /* phase A offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaB; /* phase B offset filter */
    uint16_t ui16ChanNumPhaA;            /* phase A channel number */
    uint16_t ui16ChanNumPhaB;            /* phase B channel number */
    uint16_t ui16AdcSmplPhaA;            /* phase A ADC sample number */
    uint16_t ui16AdcSmplPhaB;            /* phase B ADC sample number */        
    uint16_t ui16AdcNumPhaA;             /* phase A ADC module number */
    uint16_t ui16AdcNumPhaB;             /* phase B ADC module number */
    uint16_t ui16CalibPhaA;              /* phase A offset calibration */
    uint16_t ui16CalibPhaB;              /* phase B offset calibration */
    uint16_t ui16OffsetPhaA;             /* phase A offset result */
    uint16_t ui16OffsetPhaB;             /* phase B offset result */
    uint16_t *pui16ClistPhaA;            /* pointer to phase A clist register */
    uint16_t *pui16ClistPhaB;            /* pointer to phase B clist register */
} pha_ab_t;

typedef struct _mcdrv_adc12
{
    GMCLIB_3COOR_T_F16 *psIABC; /* pointer to the 3-phase currents */
    pha_bc_t sCurrSec16;        /* ADC setting for SVM sectors 1&6 */
    pha_ac_t sCurrSec23;        /* ADC setting for SVM sectors 2&3 */
    pha_ab_t sCurrSec45;        /* ADC setting for SVM sectors 4&5 */

    ADC_Type *pui32AdcBase;     /* pointer to ADC all quantities are measured */
    uint16_t *pui16SVMSector;   /* pointer to the SVM sector */
    frac16_t *pui16AuxChan;     /* pointer to auxiliary ADC channel number */
    frac16_t *pf16UDcBus;       /* pointer to DC Bus voltage variable */

    uint16_t ui16ChanNumVDcb;   /* DCB voltage channel number */
    uint16_t ui16AdcNumVDcb;    /* DCB voltage ADC module number */
    uint16_t ui16AdcSmplVDcb;   /* DCB voltage ADC sample number */
    
    uint16_t ui16ChanNumAux;    /* Auxiliary channel number */
    uint16_t ui16AdcNumAux;     /* Auxiliary ADC module number */
    uint16_t ui16AdcSmplAux;   /* Auxiliary ADC sample number */
    
    uint16_t ui16OffsetFiltWindow; /* ADC Offset filter window */
} mcdrv_adc12_t;

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
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShGet(mcdrv_adc12_t *this);

/*!
 * @brief Set initial channel assignment for phase currents & DCB voltage
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShChanAssignInit(mcdrv_adc12_t *this);

/*!
 * @brief Set new channel assignment for next sampling based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShChanAssign(mcdrv_adc12_t *this);

/*!
 * @brief Initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalibInit(mcdrv_adc12_t *this);

/*!
 * @brief Function reads current samples and filter them based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalib(mcdrv_adc12_t *this);

/*!
 * @brief Function passes measured offset values to main structure
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalibSet(mcdrv_adc12_t *this);

/*!
 * @brief Function reads and passes DCB voltage sample
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_VoltDcBusGet(mcdrv_adc12_t *this);

/*!
 * @brief Function reads and passes auxiliary sample
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AuxValGet(mcdrv_adc12_t *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_ADC_ADC12_H_ */

