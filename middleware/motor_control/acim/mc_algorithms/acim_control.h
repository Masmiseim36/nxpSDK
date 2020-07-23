/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ACIM_CONTROL_H_
#define _ACIM_CONTROL_H_

#include "char_pwrstg.h"
#include "gflib_FP.h"
#include "gmclib_FP.h"
#include "gdflib_FP.h"
#include "amclib_FP.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief mcs speed structure */
typedef struct _mcs_speed_flux_a1
{
    GFLIB_CTRL_BETA_IP_P_AW_T_FLT sPIParSpdMe; /* Speed controller structure */
    AMCLIB_CTRL_FLUX_WKNG_T_FLT sFluxWkng;     /* Flux weakening controller structure */
    GFLIB_FLEXSRAMP_T_FLT sRampParSpdMe;       /* Speed S-ramp structure */
    float_t fltSRampDuration;   /* Default S-ramp duration (timing of S-ramp is by default not used - small number) */
    float_t fltSpdMeFilt;       /* Filtered mechanical speed value [rpm] */
    float_t fltSpdMeRamp;       /* Ramped mechanical speed [rpm] */
    float_t fltSpdMeReq;        /* Required mechanical speed [rpm] */
    float_t fltSpdMeErr;        /* Speed error [rpm] */
    float_t fltIdStart;         /* Excitation current at speed FOC startup [A] */
    float_t fltIdStartMinPct;   /* D-axis current threshold to go spin state at speed FOC startup [%] */
    float_t fltIdReqMin;        /* D-axis minimal required current */
    float_t fltIdReqMax;        /* Q-axis minimal required current */
    float_t fltIdMTPA;          /* MTPA d-axis current */
    float_t fltIdMTPAFiltCoef;  /* D-axis current filter */
    float_t fltSpdMeReqMax;     /* Maximal mechanical required speed [rpm] */
    float_t fltSpdMeReqMin;     /* Minimal mechanical required speed [rpm] */
    bool_t bFlagPISpdMeStopInt; /* Speed PI controller saturation flag */
    bool_t bFlagPIFWStopInt;    /* Flux controller saturation flag */
    bool_t bStartupDone;        /* Startup done flag */
} mcs_speed_flux_a1_t;

/* Scalar structure */
typedef struct _mcs_scalar_a1
{
    GFLIB_RAMP_T_FLT sRampParSpdMe; /* Scalar speed ramp structure */
    float_t fltSpdMeReq;            /* Speed command [rpm] */
    float_t fltSpdMeRamp;           /* Speed command ramped [rpm] */
    float_t fltPosEl;               /* Scalar position [rad]*/
    float_t fltGainSpdMe2PosEl;     /* Scalar integration constant */
    float_t fltGainRpm2Volt;        /* V/rpm constant (mechanical speed is considered) [V/rpm] */
    float_t fltUMin;                /* Minimal voltage stator voltage [V] */
    float_t fltGainHz2PosEl;        /* Gain ratio for certain frequency generation */
} mcs_scalar_a1_t;

/* ACIM FOC structure */
typedef struct _mcs_acim_foc_a1
{
    AMCLIB_ACIM_ROT_FLUX_OBSRV_T_FLT sRFO;   /* Rotor Flux Observer structure */
    AMCLIB_ACIM_SPEED_MRAS_T_FLT sSpdObs;    /* MRAS speed observer structure */
    GFLIB_CTRL_PI_P_AW_T_FLT sPIParId;       /* D-axis current controller struct */
    GFLIB_CTRL_PI_P_AW_T_FLT sPIParIq;       /* Q-axis current controller struct */
    GDFLIB_FILTER_IIR1_T_FLT sFiltParUDcBus; /* DC-bus voltage filter */
    GMCLIB_3COOR_T_F16 sIABCFrac;            /* Measured 3-phase current (FRAC16) */
    GMCLIB_3COOR_T_FLT sIABC;                /* Measured 3-phase current [A] */
    GMCLIB_2COOR_ALBE_T_FLT sIAlBe;          /* Alpha/Beta current [A] */
    GMCLIB_2COOR_DQ_T_FLT sIDQ;              /* DQ current in rotor flux coordinates [A] */
    GMCLIB_2COOR_DQ_T_FLT sIDQReq;           /* Required DQ current in rotor flux coordinates [A] */
    GMCLIB_2COOR_DQ_T_FLT sIDQErr;           /* DQ current error in rotor flux coordinates [A] */
    GMCLIB_3COOR_T_F16 sDutyABC;             /* Applied duty cycles ABC (FRAC16) */
    GMCLIB_2COOR_ALBE_T_FLT sUAlBe;          /* Alpha/Beta stator voltage */
    GMCLIB_2COOR_ALBE_T_F16 sUAlBeCmpFrac;   /* Alpha/Beta stator voltage with DC-bus ripple elim (FRAC16)*/
    GMCLIB_2COOR_ALBE_T_FLT sUAlBeDTComp;    /* Alpha/Beta stator voltage  */
    GMCLIB_2COOR_DQ_T_FLT sUDQReq;           /* Required DQ voltage in rotor flux coordinates [V] */
    GMCLIB_2COOR_DQ_T_FLT sUDQReqScalar;     /* Required DQ voltage in scalar control */
    GMCLIB_2COOR_SINCOS_T_FLT sSCFOC;        /* Sine & cosine of rotor flux angle [-] */
    float_t fltDutyLim;                      /* Max. allowable duty cycle [-] */
    float_t fltUDcBus;                       /* DC-bus voltage [V] */
    frac16_t f16UDcBus;                      /* DC-bus voltage (FRAC16) */
    float_t fltUDcBusFilt;                   /* Filtered DC-bus voltage [V] */
    uint16_t ui16SectorSVM;                  /* SVM sector (sector) */
    bool_t bFlagPIIdStopInt;                 /* D-axis PI controller stop integrate flag */
    bool_t bFlagPIIqStopInt;                 /* Q-axis PI controller stop integrate flag */
    bool_t bFlagSpdStart;                    /* Motor startup flag */
    bool_t bFlagDTComp;                      /* Enable/disable dead-time compensation flag */
} mcs_acim_foc_a1_t;

/* dead-time compensation voltage table */
extern const float_t pfltUDtComp[DTCOMP_TABLE_SIZE];

/* dead-time compensation look-up table */
extern GFLIB_LUT1D_T_FLT sLUTUDtComp;

#ifdef __cplusplus
extern "C" {
#endif
/*!
 * @name Motor control ACIM  functions
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief ACIM Scalar Open Loop Control
 *
 * This function is used to ACIM Open Loop scalar mode
 *
 * @param psFocACIM     The pointer of the ACIM FOC parameters structure
 * @param psScalarACIM  The pointer of the ACIM scalar parameters structure
 *
 * @return None
 */

void MCS_ACIMOpenLoopScalarCtrlA1(mcs_acim_foc_a1_t *psFocACIM, mcs_scalar_a1_t *psScalarACIM);
/*!
 * @brief ACIM open loop (scalar control).
 *
 * This function is used for motor parameter identification purpose.
 * Required speed is set in Hz and stator voltage is set directly.
 *
 * @param psFocACIM     The pointer of the ACIM FOC parameters structure
 * @param psScalarACIM  The pointer of the ACIM scalar parameters structure
 *
 * @return None
 */
void MCS_ACIMOpenLoopScalarCtrlA2(mcs_acim_foc_a1_t *psFocACIM, mcs_scalar_a1_t *psScalarACIM);

/*!
 * @brief ACIM field oriented voltage control
 *
 * This function is used to perform ACIM field oriented voltage control with
 * preset sin and cos of rotor flux position.
 *
 * @param psFocACIM     The pointer of the ACIM FOC parameters structure
 *
 * @return None
 */
void MCS_ACIMFocCtrlVoltageA1(mcs_acim_foc_a1_t *psFocACIM);

/*!
 * @brief ACIM field oriented voltage control
 *
 * This function is used to perform ACIM field oriented voltage control
 *
 * @param psFocACIM     The pointer of the ACIM FOC parameters structure
 *
 * @return None
 */
void MCS_ACIMFocCtrlVoltageA2(mcs_acim_foc_a1_t *psFocACIM);

/*!
 * @brief ACIM field oriented current control
 *
 * This function is used to perform ACIM field oriented current control
 *
 * @param psFocACIM     The pointer of the ACIM FOC parameters structure
 *
 * @return None
 */
void MCS_ACIMFocCtrlCurrentA1(mcs_acim_foc_a1_t *psFocACIM);

/*!
 * @brief ACIM FOC with speed and flux control
 *
 * This function is used to perform flux and speed control of ACIM
 *
 * @param psFocACIM     The pointer of the ACIM FOC parameters structure
 *
 * @return None
 */
void MCS_ACIMSpeedFluxCtrlA1(mcs_acim_foc_a1_t *psFocACIM, mcs_speed_flux_a1_t *psSpdFlux);

#ifdef __cplusplus
}
#endif

#endif /* _ACIM_CONTROL_H_ */
