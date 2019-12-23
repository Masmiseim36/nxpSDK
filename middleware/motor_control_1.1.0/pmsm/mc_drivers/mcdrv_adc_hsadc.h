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
#ifndef _MCDRV_ADC_HSADC_H_
#define _MCDRV_ADC_HSADC_H_

#include "gdflib.h"
#include "mlib_types.h"
#include "gmclib.h"
#include "fsl_device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

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
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaB; /* phase B offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaC; /* phase C offset filter */
    uint16_t ui16ChanNumPhaB;            /* phase B channel number */
    uint16_t ui16ChanNumPhaC;            /* phase C channel number */
    uint16_t ui16CalibPhaB;              /* phase B offset calibration */
    uint16_t ui16CalibPhaC;              /* phase C offset calibration */
    uint16_t ui16OffsetPhaB;             /* phase B offset result */
    uint16_t ui16OffsetPhaC;             /* phase C offset result */
    HSADC_Type *pAdcBasePhaB;            /* pointer to Phase B base address */
    HSADC_Type *pAdcBasePhaC;            /* pointer to Phase C base address */
} pha_bc_t;

typedef struct _pha_ac
{
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaA; /* phase A offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaC; /* phase C offset filter */
    uint16_t ui16ChanNumPhaA;            /* phase A channel number */
    uint16_t ui16ChanNumPhaC;            /* phase C channel number */
    uint16_t ui16CalibPhaA;              /* phase A offset calibration */
    uint16_t ui16CalibPhaC;              /* phase C offset calibration */
    uint16_t ui16OffsetPhaA;             /* phase A offset result */
    uint16_t ui16OffsetPhaC;             /* phase C offset result */
    HSADC_Type *pAdcBasePhaA;            /* pointer to phase A base address */
    HSADC_Type *pAdcBasePhaC;            /* pointer to phase C base address */
} pha_ac_t;

typedef struct _pha_ab
{
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaA; /* phase A offset filter */
    GDFLIB_FILTER_MA_T_A32 ui16FiltPhaB; /* phase B offset filter */
    uint16_t ui16ChanNumPhaA;            /* phase A channel number */
    uint16_t ui16ChanNumPhaB;            /* phase B channel number */
    uint16_t ui16CalibPhaA;              /* phase A offset calibration */
    uint16_t ui16CalibPhaB;              /* phase B offset calibration */
    uint16_t ui16OffsetPhaA;             /* phase A offset result */
    uint16_t ui16OffsetPhaB;             /* phase B offset result */
    HSADC_Type *pAdcBasePhaA;            /* pointer to phase A base address */
    HSADC_Type *pAdcBasePhaB;            /* pointer to phase B base address */
} pha_ab_t;

typedef struct _mcdrv_hsadc
{
    GMCLIB_3COOR_T_F16 *psIABC; /* pointer to the 3-phase currents */
    pha_bc_t sCurrSec16;        /* ADC setting for SVM sectors 1&6 */
    pha_ac_t sCurrSec23;        /* ADC setting for SVM sectors 2&3 */
    pha_ab_t sCurrSec45;        /* ADC setting for SVM sectors 4&5 */

    HSADC_Type *pui32UdcbAdcBase; /* pointer to ADC where Udcb channel is assigned */
    HSADC_Type *pui32AuxAdcBase;  /* pointer to ADC where auxiliary channel is assigned */
    uint16_t *pui16SVMSector;     /* pointer to the SVM sector */
    frac16_t *pui16AuxChan;       /* pointer to auxiliary ADC channel number */
    frac16_t *pf16UDcBus;         /* pointer to DC Bus voltage variable */

    uint16_t ui16OffsetFiltWindow; /* ADC Offset filter window */
} mcdrv_hsadc_t;

typedef struct _mcdrv_hsadc_init
{
    uint16_t *ui16AdcArray;    /* pointer to ADC Array of channel configuration*/
    HSADC_Type *pui32Adc0Base; /* pointer to HSADC0 module */
    HSADC_Type *pui32Adc1Base; /* pointer to HSADC1 module */
} mcdrv_hsadc_init_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Initializes ADC driver to measure 3 currents, DC-bus voltage and
 *        auxiliary quantity samples
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_InitHSADC(mcdrv_hsadc_t *this, mcdrv_hsadc_init_t *init);

/*!
 * @brief Initializes ADC driver to measure from user defined channels
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_InitHSADCOptim(mcdrv_hsadc_t *this, mcdrv_hsadc_init_t *init);

/*!
 * @brief Reads and calculates 3 phase samples based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShGet(mcdrv_hsadc_t *this);

/*!
 * @brief Set new channel assignment for next sampling based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShChanAssign(mcdrv_hsadc_t *this);

/*!
 * @brief Initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalibInit(mcdrv_hsadc_t *this);

/*!
 * @brief Function reads current samples and filter them based on SVM sector
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalib(mcdrv_hsadc_t *this);

/*!
 * @brief Function passes measured offset values to main structure
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Curr3Ph2ShCalibSet(mcdrv_hsadc_t *this);

/*!
 * @brief Function reads and passes DCB voltage sample
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_VoltDcBusGet(mcdrv_hsadc_t *this);

/*!
 * @brief Function reads and passes auxiliary sample
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AuxValGet(mcdrv_hsadc_t *this);

/*!
 * @brief Function reads and passes phase currents, DCbus voltage and auxiliary sample
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AdcGet(mcdrv_hsadc_t *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_ADC_HSADC_H_ */

