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
#include "mlib_types.h"
#include "gmclib.h"
#include "fsl_device_registers.h"
#include "fsl_adc_etc.h"
#include "fsl_lpadc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct _pha_bc
{
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaB;           /* phase B offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaC;           /* phase C offset filter */
    uint16_t ui16ChanNumPhaB;                      /* phase B channel number */
    uint16_t ui16ChanNumPhaC;                      /* phase C channel number */
    lpadc_sample_channel_mode_t ui16ChanSidePhaB;  /* phase B channel side */
    lpadc_sample_channel_mode_t ui16ChanSidePhaC;  /* phase C channel side */
    uint16_t ui16CalibPhaB;                        /* phase B offset calibration */
    uint16_t ui16CalibPhaC;                        /* phase C offset calibration */
    uint16_t ui16OffsetPhaB;                       /* phase B offset result */
    uint16_t ui16OffsetPhaC;                       /* phase C offset result */
    uint16_t *pui16HCPhaB;                         /* pointer to phase B HC register */
    uint16_t *pui16HCPhaC;                         /* pointer to phase C HC register */
} pha_bc_t;                                        
                                                   
typedef struct _pha_ac                             
{                                                  
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaA;           /* phase A offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaC;           /* phase C offset filter */
    uint16_t ui16ChanNumPhaA;                      /* phase A channel number */
    uint16_t ui16ChanNumPhaC;                      /* phase C channel number */
    lpadc_sample_channel_mode_t ui16ChanSidePhaA;  /* phase A channel side */
    lpadc_sample_channel_mode_t ui16ChanSidePhaC;  /* phase C channel side */
    uint16_t ui16CalibPhaA;                        /* phase A offset calibration */
    uint16_t ui16CalibPhaC;                        /* phase C offset calibration */
    uint16_t ui16OffsetPhaA;                       /* phase A offset result */
    uint16_t ui16OffsetPhaC;                       /* phase C offset result */
    uint16_t *pui16HCPhaA;                         /* pointer to phase A HC register */
    uint16_t *pui16HCPhaC;                         /* pointer to phase C HC register */
} pha_ac_t;                                        
                                                   
typedef struct _pha_ab                             
{                                                  
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaA;           /* phase A offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaB;           /* phase B offset filter */
    uint16_t ui16ChanNumPhaA;                      /* phase A channel number */
    uint16_t ui16ChanNumPhaB;                      /* phase B channel number */
    lpadc_sample_channel_mode_t ui16ChanSidePhaA;  /* phase A channel side */
    lpadc_sample_channel_mode_t ui16ChanSidePhaB;  /* phase B channel side */
    uint16_t ui16CalibPhaA;                        /* phase A offset calibration */
    uint16_t ui16CalibPhaB;                        /* phase B offset calibration */
    uint16_t ui16OffsetPhaA;                       /* phase A offset result */
    uint16_t ui16OffsetPhaB;                       /* phase B offset result */
    uint16_t *pui16HCPhaA;                         /* pointer to phase A HC register */
    uint16_t *pui16HCPhaB;                         /* pointer to phase B HC register */
} pha_ab_t;

typedef struct _mcdrv_adcetc
{
    GMCLIB_3COOR_T_F16 *psIABC; /* pointer to the 3-phase currents */
    pha_bc_t sCurrSec16;        /* ADC setting for SVM sectors 1&6 */
    pha_ac_t sCurrSec23;        /* ADC setting for SVM sectors 2&3 */
    pha_ab_t sCurrSec45;        /* ADC setting for SVM sectors 4&5 */

    ADC_Type *pui32UdcbAdcBase; /* pointer to ADC where Udcb channel is assigned */
    ADC_Type *pui32AuxAdcBase;  /* pointer to ADC where auxiliary channel is assigned */
    uint16_t *pui16SVMSector;   /* pointer to the SVM sector */
    frac16_t *pui16AuxChan;     /* pointer to auxiliary ADC channel number */
    frac16_t *pf16UDcBus;       /* pointer to DC Bus voltage variable */

    uint16_t ui16ChanNumVDcb; /* DCB voltage channel number */
    lpadc_sample_channel_mode_t ui16ChanSideVDcb; /* DCB voltage channel side */
    uint16_t *pui16HCVDcb;    /* pointer to VDcb HC register */

    uint16_t ui16ChanNumAux; /* Auxiliary channel number */
    lpadc_sample_channel_mode_t ui16ChanSideAux; /* Auxiliary channel side */
    uint16_t *pui16HCAux;    /* pointer to auxiliary signal HC register */

    uint16_t ui16OffsetFiltWindow; /* ADC Offset filter window */
} mcdrv_adcetc_t;

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
RAM_FUNC_LIB
void MCDRV_Curr3Ph2ShGet(mcdrv_adcetc_t *this);

/*!
 * @brief Set new channel assignment for next sampling based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_Curr3Ph2ShChanAssign(mcdrv_adcetc_t *this);

/*!
 * @brief Initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_Curr3Ph2ShCalibInit(mcdrv_adcetc_t *this);

/*!
 * @brief Function reads current samples and filter them based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_Curr3Ph2ShCalib(mcdrv_adcetc_t *this);

/*!
 * @brief Function passes measured offset values to main structure
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_Curr3Ph2ShCalibSet(mcdrv_adcetc_t *this);

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

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_ADCETC_IMXRT11xx_H_ */
