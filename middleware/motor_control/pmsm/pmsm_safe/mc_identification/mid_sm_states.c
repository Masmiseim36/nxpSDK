/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "mid_sm_states.h"
#include "mid_auxiliary.h"
#include "rs_measure.h"
#include "pwrstg_characteristic.h"
#include "ls_measure.h"
#include "ke_measure.h"
#include "pp_measure.h"
#include "mech_measure.h"
#include "hall_calib.h"

MISRAC_DISABLE /* MISRA check enabled (IAR IDE only). */

#include "mlib.h"
#include "freemaster_tsa_pmsm.h"

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Bemf params structure. */
typedef struct _mid_bemf_params_t
{
    float_t fltI_gain;
    float_t fltU_gain;
    float_t fltE_gain;
    float_t fltWI_gain;
    float_t fltBemfDQ_Kps;
    float_t fltBemfDQ_Kis;
}mid_bemf_params_t;

/* Tracking observer params structure. */
typedef struct _mid_to_params_t
{
    float_t fltTO_Kpz;
    float_t fltTO_Kiz;
    float_t fltTO_Theta;
}mid_to_params_t;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* (user) Motor parameters identification state machine functions */
static void MID_StateStart(void);
static void MID_StatePwrStgCharact(void);
static void MID_StateRs(void);
static void MID_StateLd(void);
static void MID_StateLq(void);
static void MID_StatePp(void);
static void MID_StateKe(void);
static void MID_StateMech(void);
static void MID_StateHall(void);
static void MID_StateStop(void);

/* (user) Motor parameters identification state-transition functions */
static void MID_TransStart2PwrStgCharact(void);
static void MID_TransStart2Rs(void);
static void MID_TransStart2Ld(void);
static void MID_TransStart2Lq(void);
static void MID_TransStart2Ke(void);
static void MID_TransStart2Pp(void);
static void MID_TransStart2Mech(void);
static void MID_TransStart2Hall(void);
static void MID_TransAll2Stop(void);
static void MID_TransStop2Start(void);

/* Bemf and Tracking observer calculation function. */
static void MID_CalcBemfTrackingObsrv(mid_bemf_params_t *sBemfParams, mid_to_params_t *sTOParams);

/* MID TSA table */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
FMSTR_TSA_TABLE_BEGIN(TSA_MID)
    /* Measurement control from application */
    FMSTR_TSA_RW_VAR(eUserMIDMeasType,       FMSTR_TSA_UINT16)
    FMSTR_TSA_RO_VAR(ui32UserMIDStartResult, FMSTR_TSA_UINT32)

    /* The measurement configuration setup. */
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltRsIdMeas,           FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltLsIdAmplitudeReq,   FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltLsUdIncrement,      FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltLsFreqStart,        FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltLsFreqDecrement,    FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltLsFreqMin,          FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltKeIdReqOpenLoop,    FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltKeFreqElReq,        FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltPpIdReqOpenLoop,    FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltPpFreqElReq,        FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltPwrStgRs,           FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltPwrStgIdCalib,      FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechKt,             FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechIqStartup,      FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechMergingCoeff,   FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechIqAccelerate,   FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechIqDecelerate,   FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechSpeedThrsAccel, FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechSpeedThrsDecel, FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechSpeedThrsInteg, FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.ui32ScheduledMeas,     FMSTR_TSA_UINT32)

    /* MID Status */
    FMSTR_TSA_RW_VAR(sUserMIDStatus.eMIDState,        FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR(sUserMIDStatus.ui32FinishedMeas, FMSTR_TSA_UINT32)
    FMSTR_TSA_RW_VAR(sUserMIDStatus.ui16FaultMID,     FMSTR_TSA_UINT32)
    FMSTR_TSA_RW_VAR(sUserMIDStatus.ui16WarnMID,      FMSTR_TSA_UINT32)
    FMSTR_TSA_RW_VAR(sUserMIDStatus.fltSpeedAngScale, FMSTR_TSA_FLOAT)

    /* MID known motor parameters. */
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsKnown.fltRs,  FMSTR_TSA_FLOAT)

    /* MID measured and/or known motor parameters. */
    FMSTR_TSA_RO_VAR(sUserMIDMotorParamsMeas.fltRs,  FMSTR_TSA_FLOAT)

FMSTR_TSA_TABLE_END()
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* User Measurement Type selection */
mid_meas_type_t eUserMIDMeasType;

/* MID measurement status. */
mid_status_t   sUserMIDStatus;

/* User MID start result */
uint32_t       ui32UserMIDStartResult = 0UL;

/* MID measurement parameters */
mid_config_t   sUserMIDMeasConfig = MID_DEFAULT_MEAS_CONFIG;

/* User params for setting motor params */
mid_motor_params_user_t sUserMIDMotorParamsKnown = {0.0F, 0.0F, 0.0F, 0.0F,
                                                    0.0F, 0.0F, 1UL};
mid_motor_params_user_t sUserMIDMotorParamsMeas  = {0.0F, 0.0F, 0.0F, 0.0F,
                                                    0.0F, 0.0F, 1UL};

/* Global structure for all measurements */
mid_struct_t g_sMID;

/* Dead-time compensation table. */
float_t g_pfltMIDUdErrorLookUp[MID_CHAR_CURRENT_POINT_NUMBERS] = MID_DT_COMP_TABLE;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*! @brief MID state machine function */
const mid_pfcn_void_pms g_MID_SM_STATE_TABLE[10U] = {MID_StateStart,
                                                     MID_StatePwrStgCharact,
                                                     MID_StateRs,
                                                     MID_StateLd,
                                                     MID_StateLq,
                                                     MID_StatePp,
                                                     MID_StateKe,
                                                     MID_StateMech,
                                                     MID_StateHall,
                                                     MID_StateStop};

/*!
 * @brief MID START state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateStart(void)
{
    uint32_t ui32CheckAux;

    /* Type the code to do when in the START state */
    /* Wait till MC.SM is ready. */
    if(kMC_StREADY < MID_MC_GetAppState())
    {
        /* MID alignment */
        MID_alignment(&g_sMID.sMIDAlignment);

        /* When MID_alignment is done and Motor Control SM proceeded to SPIN */
        if (g_sMID.sMIDAlignment.ui16Active == FALSE)
        {
            /* if kMID_Calibration go to PWR_STG_CHARACT state */
            if(g_sMID.eMeasurementType == kMID_Calibration)
            {
                MID_TransStart2PwrStgCharact();
            }

            /* if kMID_PolePairs go to PP state */
            else if(g_sMID.eMeasurementType == kMID_PolePairs)
            {
                MID_TransStart2Pp();
            }

            /* if kMID_ElectricalParams go to RS state */
            else if(g_sMID.eMeasurementType == kMID_ElectricalParams)
            {
                ui32CheckAux = g_sMID.ui32ScheduledMeas & (uint32_t)(~g_sMID.ui32FinishedMeas);

                /* If RS scheduled but not finished, go to RS */
                if(0UL < (ui32CheckAux & MID_EL_RS))
                {
                    MID_TransStart2Rs();
                }
                /* If LD scheduled but not finished, go to LD */
                else if(0UL < (ui32CheckAux & MID_EL_LD))
                {
                    MID_TransStart2Ld();
                }
                /* If LQ scheduled but not finished, go to LQ */
                else if(0UL < (ui32CheckAux & MID_EL_LQ))
                {
                    MID_TransStart2Lq();
                }
                /* If KE scheduled but not finished, go to KE */
                else if(0UL < (ui32CheckAux & MID_EL_KE))
                {
                    MID_TransStart2Ke();
                }
                /* If all measurements finished or no measurement scheduled, go to STOP */
                else
                {
                    MID_TransAll2Stop();
                }
            }

            /* if kMID_MechanicalParams go to MECH state */
            else if(g_sMID.eMeasurementType == kMID_MechanicalParams)
            {
                MID_TransStart2Mech();
            }

            /* if kMID_HallSensors go to HALL state */
            else if(g_sMID.eMeasurementType == kMID_HallSensors)
            {
                MID_TransStart2Hall();
            }

            /* if none of above eMeasurementType applies, go to STOP */
            else
            {
                MID_TransAll2Stop();
            }
        }
    }
}

/*!
 * @brief MID PWR_STG_CHARACT state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StatePwrStgCharact(void)
{
    /* Type the code to do when in the PWR_STG_CHARACT state */
    MID_GetTransferCharacteristic(&g_sMID.sMIDPwrStgChar);

    /* If characteristic for the last current was measured (f16TransferCharIndex == number of ) */
    if (g_sMID.sMIDPwrStgChar.ui16Active == FALSE)
    {
        /* Got to STOP state */
        MID_TransAll2Stop();
    }
}

/*!
 * @brief MID RS state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateRs(void)
{
    /* Type the code to do when in the RS state */
    /* call Rs measurement routine */
    MID_getRs(&g_sMID.sMIDRs, &g_sMID.sMIDPwrStgChar);

    /* when the R_s measurement is finished, set the RS_DONE flag */
    if (g_sMID.sMIDRs.ui16Active == FALSE)
    {
        /* Store measured Rs */
        g_sMID.sMotorParams.fltRs = g_sMID.sMIDRs.fltRs;

        /* Indicate finished measurement. */
        g_sMID.ui32FinishedMeas |= MID_EL_RS;

        /* Go to STOP state */
        MID_TransAll2Stop();
    }
}

/*!
 * @brief MID Ld state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateLd(void)
{
    /* Type the code to do when in the LS state */

    /* call Rs measurement routine */
    MID_getLs(&g_sMID.sMIDLs);

    /* when the L_s measurement is finished, set the LS_DONE flag */
    if (g_sMID.sMIDLs.ui16Active == FALSE)
    {
        /* Store measured Ld */
        g_sMID.sMotorParams.fltLd = g_sMID.sMIDLs.fltLs;

        /* Indicate finished measurement. */
        g_sMID.ui32FinishedMeas |= MID_EL_LD;

        /* Go to STOP state */
        MID_TransAll2Stop();
    }
}

/*!
 * @brief MID LQ state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateLq(void)
{
    /* Type the code to do when in the LS state */

    /* call Rs measurement routine */
    MID_getLs(&g_sMID.sMIDLs);

    /* when the L_s measurement is finished, set the LS_DONE flag */
    if (g_sMID.sMIDLs.ui16Active == FALSE)
    {
        /* Store measured Lq */
        g_sMID.sMotorParams.fltLq = g_sMID.sMIDLs.fltLs;

        /* Indicate finished measurement. */
        g_sMID.ui32FinishedMeas |= MID_EL_LQ;

        /* Go to STOP state */
        MID_TransAll2Stop();
    }
}

/*!
 * @brief MID PP state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StatePp(void)
{
    /* Type the code to do when in the PP state */

    /* Call Pp measurement routine */
    MID_getPp(&g_sMID.sMIDPp);

    /* Escape MID_StateKe when measurement ends */
    if(g_sMID.sMIDPp.ui16Active == FALSE)
    {
        /* Go to STOP state */
        MID_TransAll2Stop();
    }
}

/*!
 * @brief MID KE state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateKe(void)
{
    /* Type the code to do when in the KE state */
    /* Call Ke measurement routine */
    MID_getKe(&g_sMID.sMIDKe);

    /* Escape MID_StateKe when measurement ends */
    if(g_sMID.sMIDKe.ui16Active == FALSE)
    {
        /* Store measured Ke */
        g_sMID.sMotorParams.fltKe = g_sMID.sMIDKe.fltKe;

        /* Indicate finished measurement. */
        g_sMID.ui32FinishedMeas |= MID_EL_KE;

        /* Go to STOP state */
        MID_TransAll2Stop();
    }
}

/*!
 * @brief MID MECH state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateMech(void)
{
    /* Type the code to do when in the MECH state */

    /* Call Rs measurement routine */
    MID_getMech(&g_sMID.sMIDMech);

    /* Escape MID_StateKe when measurement ends */
    if(g_sMID.sMIDMech.ui16Active == FALSE)
    {
        /* Store measured J and B */
        g_sMID.sMotorParams.fltJ = g_sMID.sMIDMech.fltJ;
        g_sMID.sMotorParams.fltB = g_sMID.sMIDMech.fltB;

        /* Indicate finished measurement. */
        g_sMID.ui32FinishedMeas |= MID_EL_ME;

        /* Go to STOP state */
        MID_TransAll2Stop();
    }
}

/*!
 * @brief MID HALL state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateHall(void)
{
    /* Type the code to do when in the HALL state */

    /* Escape MID_StateKe when measurement ends */
    if(g_sMID.sMIDHall.ui16Active == FALSE)
    {
        /* Go to STOP state */
        MID_TransAll2Stop();
    }
}

/*!
 * @brief MID STOP state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateStop(void)
{
    /* Type the code to do when in the STOP state */

    /* Wait in STOP unless measurement is triggered. */
    if(g_sMID.bMIDStart == TRUE)
    {
        /* Do not go to START until the MC state machine is in STOP */
        if(MID_MC_GetAppState() == kMC_StSTOP)
        {
            /* Go to MID START (start measurement) */
            MID_TransStop2Start();
        }
        else
        {
            /* Keep turning off the MC state machine */
            MID_MC_SetAppSwitch(FALSE);
        }
    }
}

/*!
 * @brief MID START to PWR_STG_CHARACT transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransStart2PwrStgCharact(void)
{
    /* Type the code to do when going from the START to the PWR_STG_CHARACT state */

    /* enable start of characterization */
    g_sMID.sMIDPwrStgChar.ui16Active = FALSE;

    /* Enable FOC current loop */
    MID_MC_EnableCurrentLoop(TRUE);

    /* Use OL position for Park transformation. */
    MID_MC_UseEstimatedPos(FALSE);

    /* Next is PWR_STG_CHARACT state */
    g_sMID.eMIDState = kMID_PwrStgCharact;
}

/*!
 * @brief MID START to RS transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransStart2Rs(void)
{
    /* Type the code to do when going from the PWR_STG_CHARACT to the RS state */

    /* enable start of Rs measurement */
    g_sMID.sMIDRs.ui16Active = FALSE;

    /* Enable FOC current loop */
    MID_MC_EnableCurrentLoop(TRUE);

    /* Use OL position for Park transformation. */
    MID_MC_UseEstimatedPos(FALSE);

    /* Next is RS state */
    g_sMID.eMIDState = kMID_Rs;
}

/*!
 * @brief MID START to PP transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransStart2Pp(void)
{
    /* Type the code to do when going from the LS to the STOP state */
    g_sMID.sMIDPp.ui16Active = FALSE;
    g_sMID.sMIDPp.ui16PpDetermined = FALSE;

    /* Disable FOC current loop */
    MID_MC_EnableCurrentLoop(TRUE);

    /* Use OL position for Park transformation. */
    MID_MC_UseEstimatedPos(FALSE);

    /* Next is PP state */
    g_sMID.eMIDState = kMID_Pp;
}

/*!
 * @brief MID START to MECH transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransStart2Mech(void)
{
    mid_bemf_params_t sBemfParams;
    mid_to_params_t sTOParams;
    float_t fltPpInverse = 1.0F / g_sMID.sMotorParams.fltPp;

    /* Type the code to do when going from the LS to the STOP state */
    g_sMID.sMIDMech.ui16Active    = FALSE;

    /* Calculate Bemf and Tracking parameters */
    MID_CalcBemfTrackingObsrv(&sBemfParams, &sTOParams);

    /* Update the Bemf observer parameters. */
    MID_MC_UpdateBemfObsrv(sBemfParams.fltI_gain,
                       sBemfParams.fltU_gain,
                       sBemfParams.fltE_gain,
                       sBemfParams.fltWI_gain,
                       sBemfParams.fltBemfDQ_Kps,
                       sBemfParams.fltBemfDQ_Kis);

    /* Update the Tracking observer parameters. */
    MID_MC_UpdateTrackingObsrv(sTOParams.fltTO_Kpz,
                               sTOParams.fltTO_Kiz,
                               sTOParams.fltTO_Theta);

    /* Enable FOC current loop */
    MID_MC_EnableCurrentLoop(TRUE);

    /* Use OL position for Park transformation. */
    MID_MC_UseEstimatedPos(TRUE);

    /* fltSpeedCatchUp(merging speed) = Nnom * 0.15; */
    g_sMID.sMIDMech.sStartup.fltSpeedCatchUp = 3000.0F * 0.15F;

    /* I_gain  = (MergCoeff/100) * (60/(Pp*Nmerging))/Ts */
    g_sMID.sMIDMech.sStartup.f16CoeffMerging = MLIB_Conv_F16f((MID_MECH_STARTUP_MERGE_COEFF/100.0F)*(60.0F * fltPpInverse / g_sMID.sMIDMech.sStartup.fltSpeedCatchUp)/MID_FAST_LOOP_TS/2.0F/32768.0F);

    /* Ramp RampUpInc =  RampUp * Ts/SpeedAngScale */
    g_sMID.sMIDMech.sStartup.sSpeedRampOpenLoopParams.fltRampUp = MID_MECH_STARTUP_RAMP_COEFF * MID_FAST_LOOP_TS / (60.0F * fltPpInverse / (2.0F * FLOAT_PI));

    /* Next is MECH state */
    g_sMID.eMIDState = kMID_Mech;
}

/*!
 * @brief MID START to HALL transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransStart2Hall(void)
{
    /* Type the code to do when going from the LS to the STOP state */
    g_sMID.sMIDHall.ui16Active    = FALSE;

    /* Next is HALL state */
    g_sMID.eMIDState = kMID_Hall;
}

/*!
 * @brief MID START to LD transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransStart2Ld(void)
{
    /* Type the code to do when going from the START to the LD state */
    /* enable start of Ls measurement */
    g_sMID.sMIDLs.ui16Active = FALSE;

    /* Pass value of Rs to Ls structure */
    g_sMID.sMIDLs.fltRs = g_sMID.sMIDRs.fltRs;

    /* Set the maximum measuring voltage based on actual DCbus voltage */
    g_sMID.sMIDLs.fltUdMax = MLIB_Mul_FLT(MID_LS_K_MODULATION_RATIO, MID_MC_GetUDCbusFilt());

    /* Disable FOC current loop */
    MID_MC_EnableCurrentLoop(FALSE);

    /* Use OL position for Park transformation. */
    MID_MC_UseEstimatedPos(FALSE);

    /* Next is LD state */
    g_sMID.eMIDState = kMID_Ld;
}

/*!
 * @brief MID START to LQ transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransStart2Lq(void)
{
    /* Type the code to do when going from the START to the LQ state */
    /* enable start of Ls measurement */
    g_sMID.sMIDLs.ui16Active = FALSE;

    /* Pass value of Rs to Ls structure */
    g_sMID.sMIDLs.fltRs = g_sMID.sMIDRs.fltRs;

    /* Set the maximum measuring voltage based on actual DCbus voltage */
    g_sMID.sMIDLs.fltUdMax = MLIB_Mul_FLT(MID_LS_K_MODULATION_RATIO, MID_MC_GetUDCbusFilt());

    /* Disable FOC current loop */
    MID_MC_EnableCurrentLoop(FALSE);

    /* Use OL position for Park transformation. */
    MID_MC_UseEstimatedPos(FALSE);

    /* Next is LQ state */
    g_sMID.eMIDState = kMID_Lq;
}

/*!
 * @brief MID START to KE transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransStart2Ke(void)
{
    mid_bemf_params_t sBemfParams;
    mid_to_params_t sTOParams;

    /* Type the code to do when going from the START to the KE state */
    g_sMID.sMIDKe.ui16Active = FALSE;

    /* Calculate Bemf and Tracking parameters */
    MID_CalcBemfTrackingObsrv(&sBemfParams, &sTOParams);

    /* Update the Bemf observer parameters. */
    MID_MC_UpdateBemfObsrv(sBemfParams.fltI_gain,
                       sBemfParams.fltU_gain,
                       sBemfParams.fltE_gain,
                       sBemfParams.fltWI_gain,
                       sBemfParams.fltBemfDQ_Kps,
                       sBemfParams.fltBemfDQ_Kis);

    /* Update the Tracking observer parameters. */
    MID_MC_UpdateTrackingObsrv(sTOParams.fltTO_Kpz,
                               sTOParams.fltTO_Kiz,
                               sTOParams.fltTO_Theta);

    /* Enable FOC current loop */
    MID_MC_EnableCurrentLoop(TRUE);

    /* Use OL position for Park transformation. */
    MID_MC_UseEstimatedPos(TRUE);

    /* Next is KE state */
    g_sMID.eMIDState = kMID_Ke;
}

/*!
 * @brief (general) Any state to MID STOP transition
 *
 * @param None
 *
 * @return None
 */
static void MID_TransAll2Stop(void)
{
    /* Type the code to do when going to the STOP state */
    /* Reset active flags */
    g_sMID.sMIDAlignment.ui16Active  = FALSE;
    g_sMID.sMIDPwrStgChar.ui16Active = FALSE;
    g_sMID.sMIDRs.ui16Active         = FALSE;
    g_sMID.sMIDLs.ui16Active         = FALSE;
    g_sMID.sMIDKe.ui16Active         = FALSE;
    g_sMID.sMIDPp.ui16Active         = FALSE;
    g_sMID.sMIDMech.ui16Active       = FALSE;

    /* Clear the measurement trigger if all scheduled measurements has been finished. */
    if(((g_sMID.eMeasurementType == kMID_ElectricalParams) && (g_sMID.ui32FinishedMeas == g_sMID.ui32ScheduledMeas)) ||
        (g_sMID.eMeasurementType == kMID_PolePairs) ||
        (g_sMID.eMeasurementType == kMID_MechanicalParams) ||
        (g_sMID.eMeasurementType == kMID_Calibration))
    {
        g_sMID.bMIDStart = FALSE;
    }

    /* Turn off the MC state machine */
    MID_MC_SetAppSwitch(FALSE);

    /* Next is STOP state */
    g_sMID.eMIDState = kMID_Stop;
}

/*!
 * @brief (general) Any state to MID STOP transition
 *
 * @param None
 *
 * @return None
 */
static void MID_TransStop2Start(void)
{
    /* type the code to do when going to the START state */
    /* Enable FOC current loop */
    MID_MC_EnableCurrentLoop(TRUE);

    /* Use OL position for Park transformation. */
    MID_MC_UseEstimatedPos(FALSE);

    /* Turn the MC state machine On*/
    MID_MC_SetAppSwitch(TRUE);

    /* Next is START state */
    g_sMID.eMIDState = kMID_Start;
}

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * MID after-reset initialization function.
 */
void MID_Init_AR(void)
{
    /* Clean the internal parameters. */
    g_sMID.sMotorParams.fltPp = 1.0F;
    g_sMID.sMotorParams.fltRs = 0.0F;
    g_sMID.sMotorParams.fltLd = 0.0F;
    g_sMID.sMotorParams.fltLq = 0.0F;
    g_sMID.sMotorParams.fltKe = 0.0F;
    g_sMID.sMotorParams.fltJ  = 0.0F;
    g_sMID.sMotorParams.fltB  = 0.0F;

    /* Clear status variables. */
    g_sMID.bMIDStart    = FALSE;
    g_sMID.ui16FaultMID = 0U;
    g_sMID.ui16WarnMID  = 0U;
    g_sMID.ui32FinishedMeas = 0UL;
    g_sMID.fltSpeedAngScale = (60.0F / (2.0F * FLOAT_PI));

    /* Set the initial MID state. */
    g_sMID.eMIDState = kMID_Stop;
}

/*!
 * MID fast-loop process function.
 */
void MID_ProcessFast_FL(void)
{
    /* Read measurements from FOC motor-control module. */
    MID_MC_ReadSignals();

    /* Esecute the MID state machine. */
    g_MID_SM_STATE_TABLE[g_sMID.eMIDState]();

    /* Apply control signals results to FOC motor-control module. */
    MID_MC_ApplySignals();
}

/*!
 * MID start function.
 */
uint32_t MID_Start_BL(mid_meas_type_t eMeasurementType)
{
    uint32_t ui32RetVal = 0UL;

    /* Check validity of the selected measurement type (all necessary motor parameters set). */
    /* Check Rs non-zero (for Ld, Lq, Ke measurement). */
    if(((kMID_ElectricalParams == eMeasurementType) &&
        (0UL < (g_sMID.ui32ScheduledMeas & (MID_EL_LD | MID_EL_LQ | MID_EL_KE)))) ||
        (kMID_MechanicalParams == eMeasurementType))
    {
        /* Check whether Rs measurement is scheduled */
        if(0UL == (g_sMID.ui32ScheduledMeas & MID_EL_RS))
        {
            /* Check whether Rs is non-zero. */
            if(!(g_sMID.sMotorParams.fltRs > 0.0F))
            {
                /* Stator resistance wa not set. */
                ui32RetVal |= MID_RS_MISSING;
            }
        }
    }

    /* Check Lq non-zero (for Ke measurement). */
    if(((kMID_ElectricalParams == eMeasurementType) &&
        (0UL < (g_sMID.ui32ScheduledMeas & MID_EL_KE))) ||
        (kMID_MechanicalParams == eMeasurementType))
    {
        /* Check whether Ld measurement is scheduled */
        if(0UL == (g_sMID.ui32ScheduledMeas & MID_EL_LD))
        {
            /* Check whether Ld is non-zero. */
            if(!(g_sMID.sMotorParams.fltLd > 0.0F))
            {
                /* Ld inductance was not set. */
                ui32RetVal |= MID_LD_MISSING;
            }
        }

        /* Check whether Lq measurement is scheduled */
        if(0UL == (g_sMID.ui32ScheduledMeas & MID_EL_LQ))
        {
            /* Check whether Lq is non-zero. */
            if(!(g_sMID.sMotorParams.fltLq > 0.0F))
            {
                /* Lq inductance was not set. */
                ui32RetVal |= MID_LQ_MISSING;
            }
        }
    }

    /* Check Rs non-zero (for Ld, Lq, Ke measurement). */
    if(kMID_MechanicalParams == eMeasurementType)
    {
        /* Check whether Ke is non-zero. */
        if(!(g_sMID.sMotorParams.fltKe > 0.0F))
        {
            /* Electrical constant was nt set. */
            ui32RetVal |= MID_KE_MISSING;
        }

        /* Check whether Pp is non-zero. */
        if(!(g_sMID.sMotorParams.fltPp > 0.0F))
        {
            /* Number of pole pairs was not set. */
            ui32RetVal |= MID_PP_MISSING;
        }

    }

    /* If none of above conditions were fulfilled, all necessary parameter for
       measurement were set. */
    if(0UL == ui32RetVal)
    {
        /* Pass the measurement type */
        g_sMID.eMeasurementType = eMeasurementType;

        /* Clear the warning and fault registers. */
        g_sMID.ui16FaultMID = 0U;
        g_sMID.ui16WarnMID  = 0U;

        /* Trigger the measurement */
        g_sMID.bMIDStart = TRUE;
    }

    return ui32RetVal;
}

/*!
 * MID stop function
 */
void MID_Stop_BL(void)
{
    /* Clear the start trigger to avoid MID restart. */
    g_sMID.bMIDStart = FALSE;

    /* Check whether the pole-pair measurement is ongoing. */
    if(TRUE == g_sMID.sMIDPp.ui16Active)
    {
        /* Stop the Pp assistant */
        g_sMID.sMIDPp.ui16PpDetermined = TRUE;
    }

    /* Go to STOP state immediately */
    MID_TransAll2Stop();
}

/*!
 * Return the MID status.
 */
bool_t MID_GetStatus_BL(mid_status_t *psMIDStatus)
{
    /* Copy the statuses. */
    psMIDStatus->eMIDState        = g_sMID.eMIDState;
    psMIDStatus->ui32FinishedMeas = g_sMID.ui32FinishedMeas;
    psMIDStatus->ui16FaultMID     = g_sMID.ui16FaultMID;
    psMIDStatus->ui16WarnMID      = g_sMID.ui16WarnMID;
    psMIDStatus->fltSpeedAngScale = g_sMID.fltSpeedAngScale;

    /* Return the MID start status variable. */
    return g_sMID.bMIDStart;
}

/*!
 * Function sets known motor parameters before the measurement.
 */
void MID_SetKnownMotorParams_BL(mid_motor_params_user_t *psMotorParams)
{
    /* Copy the parameters into the internal structure. */
    /* If Pp non-zero. */
    if(psMotorParams->ui32Pp > 0UL)
    {
        g_sMID.sMotorParams.fltPp = (float_t)psMotorParams->ui32Pp;
    }
    /* If Rs non-zero. */
    if(psMotorParams->fltRs > 0.0F)
    {
        g_sMID.sMotorParams.fltRs = psMotorParams->fltRs;
    }
    /* If Ld non-zero. */
    if(psMotorParams->fltLd > 0.0F)
    {
        g_sMID.sMotorParams.fltLd = psMotorParams->fltLd;
    }
    /* If Lq non-zero. */
    if(psMotorParams->fltLq > 0.0F)
    {
        g_sMID.sMotorParams.fltLq = psMotorParams->fltLq;
    }
    /* If Ke non-zero. */
    if(psMotorParams->fltKe > 0.0F)
    {
        g_sMID.sMotorParams.fltKe = psMotorParams->fltKe;
    }
    /* If J non-zero. */
    if(psMotorParams->fltJ > 0.0F)
    {
        g_sMID.sMotorParams.fltJ  = psMotorParams->fltJ;
    }
    /* If B non-zero. */
    if(psMotorParams->fltB > 0.0F)
    {
        g_sMID.sMotorParams.fltB  = psMotorParams->fltB;
    }
}

/*!
 * Function gets motor parameters (both measured and known).
 */
void MID_GetMotorParams_BL(mid_motor_params_user_t *psMotorParams)
{
    /* Copy the parameters into the internal structure. */
    psMotorParams->ui32Pp = (uint32_t)g_sMID.sMotorParams.fltPp;
    psMotorParams->fltRs  = g_sMID.sMotorParams.fltRs;
    psMotorParams->fltLd  = g_sMID.sMotorParams.fltLd;
    psMotorParams->fltLq  = g_sMID.sMotorParams.fltLq;
    psMotorParams->fltKe  = g_sMID.sMotorParams.fltKe;
    psMotorParams->fltJ   = g_sMID.sMotorParams.fltJ;
    psMotorParams->fltB   = g_sMID.sMotorParams.fltB;
}

/*!
 * Function configures the upcoming measurement.
 */
mid_config_result_t MID_SetMeasConfig_BL(mid_config_t *psMeasConfig)
{
    mid_config_result_t eRetVal = kMID_ConfigSuccessful;

    /* Scheduled electrical parameters. */
    g_sMID.ui32ScheduledMeas = psMeasConfig->ui32ScheduledMeas;

    /* Clear finished measurement flag. */
    g_sMID.ui32FinishedMeas = 0UL;

    /* Init MID state machine to STOP. */
    MID_TransAll2Stop();

    /* Check range of the pole-pair parameter. */
    if((g_sMID.sMotorParams.fltPp > 20.0F) ||
       (g_sMID.sMotorParams.fltPp < 1.0F))
    {
        /* Report pole-pair parameter out of range. */
        eRetVal = kMID_PpParamOutOfRange;

        /* Update the speed scale to default value. */
        g_sMID.fltSpeedAngScale = (60.0F / (2.0F * FLOAT_PI));
    }
    else
    {
        /* Set the pole-pair parameter in mechanical measurement configuration. */
        g_sMID.sMIDMech.fltPp = g_sMID.sMotorParams.fltPp;

        /* Update the speed scale (mechanical rpm -> electrical rad/s) */
        g_sMID.fltSpeedAngScale = (60.0F / (g_sMID.sMotorParams.fltPp * 2.0F * FLOAT_PI));
    }

    /* Align configuration. */
    /* Check range of the measurement configuration. */
    if((psMeasConfig->fltAlignId > MID_I_MEAS_MAX) ||
       (psMeasConfig->fltAlignId < 0.0F))
    {
        /* Report Alignment configuration out of range. */
        eRetVal = kMID_AlignCfgOutOfRange;
    }
    else
    {
        /* Set the Align measurement configuration. */
        g_sMID.sMIDAlignment.fltCurrentAlign = psMeasConfig->fltAlignId;
    }

    /* Rs measurement configuration. */
    /* Check range of the measurement configuration. */
    if((psMeasConfig->fltRsIdMeas > MID_I_MEAS_MAX) ||
       (psMeasConfig->fltRsIdMeas < 0.0F))
    {
        /* Report Rs parameter measurement configuration out of range. */
        eRetVal = kMID_RsMeasCfgOutOfRange;
    }
    else
    {
        /* Set the Rs measurement configuration. */
        g_sMID.sMIDRs.fltIdMeas = psMeasConfig->fltRsIdMeas;
    }

    /* Ls measurement configuration. */
    /* Check range of the measurement configuration. */
    if((psMeasConfig->fltLsIdAmplitudeReq > MID_I_MEAS_MAX)   ||
       (psMeasConfig->fltLsIdAmplitudeReq < 0.0F)             ||
       (psMeasConfig->fltLsUdIncrement < 0.0F)                ||
       (psMeasConfig->fltLsFreqStart > MID_FREQ_MEAS_MAX)     ||
       (psMeasConfig->fltLsFreqStart < 0.0F)                  ||
       (psMeasConfig->fltLsFreqDecrement > MID_FREQ_MEAS_MAX) ||
       (psMeasConfig->fltLsFreqDecrement < 0.0F)              ||
       (psMeasConfig->fltLsFreqMin > MID_FREQ_MEAS_MAX)       ||
       (psMeasConfig->fltLsFreqMin < 0.0F))
    {
        /* Report Ls parameters measurement configuration out of range. */
        eRetVal = kMID_LsMeasCfgOutOfRange;
    }
    else
    {
        /* Set the Ls measurement configuration. */
        g_sMID.sMIDLs.fltIdAmplitudeReq = psMeasConfig->fltLsIdAmplitudeReq;
        g_sMID.sMIDLs.fltUdIncrement    = psMeasConfig->fltLsUdIncrement;
        g_sMID.sMIDLs.fltFreqStart      = psMeasConfig->fltLsFreqStart;
        g_sMID.sMIDLs.fltFreqDecrement  = psMeasConfig->fltLsFreqDecrement;
        g_sMID.sMIDLs.fltFreqMin        = psMeasConfig->fltLsFreqMin;
    }

    /* Ke measurement configuration. */
    /* Check range of the measurement configuration. */
    if((psMeasConfig->fltKeIdReqOpenLoop > MID_I_MEAS_MAX) ||
       (psMeasConfig->fltKeIdReqOpenLoop < 0.0F)           ||
       (psMeasConfig->fltKeFreqElReq > MID_FREQ_MEAS_MAX)  ||
       (psMeasConfig->fltKeFreqElReq < 0.0F))
    {
        /* Report Ke parameter measurement configuration out of range. */
        eRetVal = kMID_KeMeasCfgOutOfRange;
    }
    else
    {
        /* Set the Ke measurement configuration. */
        g_sMID.sMIDKe.fltIdReqOpenLoop = psMeasConfig->fltKeIdReqOpenLoop;
        g_sMID.sMIDKe.fltFreqElReq     = psMeasConfig->fltKeFreqElReq;
    }

    /* Pp measurement configuration. */
    /* Check range of the measurement configuration. */
    if((psMeasConfig->fltPpIdReqOpenLoop > MID_I_MEAS_MAX) ||
       (psMeasConfig->fltPpIdReqOpenLoop < 0.0F)           ||
       (psMeasConfig->fltPpFreqElReq > MID_FREQ_MEAS_MAX)  ||
       (psMeasConfig->fltPpFreqElReq < 0.0F))
    {
        /* Report Pp parameter measurement configuration out of range. */
        eRetVal = kMID_PpMeasCfgOutOfRange;
    }
    else
    {
        /* Set the Pp measurement configuration. */
        g_sMID.sMIDPp.fltIdReqOpenLoop = psMeasConfig->fltPpIdReqOpenLoop;
        g_sMID.sMIDPp.fltFreqElReq     = psMeasConfig->fltPpFreqElReq;
    }

    /* PwrStg measurement configuration. */
    /* Check range of the measurement configuration. */
    if((psMeasConfig->fltPwrStgIdCalib > MID_I_MEAS_MAX) ||
       (psMeasConfig->fltPwrStgIdCalib < 0.0F)           ||
       (psMeasConfig->fltPwrStgRs > 100.0F)              ||
       (psMeasConfig->fltPwrStgRs < 0.5F))
    {
        /* Report PwrStg characteristic measurement configuration out of range. */
        eRetVal = kMID_PwrStgMeasCfgOutOfRange;
    }
    else
    {
        /* Set the PwrStg measurement configuration. */
        g_sMID.sMIDPwrStgChar.fltRs      = psMeasConfig->fltPwrStgRs;
        g_sMID.sMIDPwrStgChar.fltIdCalib = psMeasConfig->fltPwrStgIdCalib;
    }

    /* Mech measurement configuration. */
    /* Check range of the measurement configuration. */
    if((psMeasConfig->fltMechKt < 0.0F)                       ||
       (psMeasConfig->fltMechIqStartup > MID_I_MEAS_MAX)      ||
       (psMeasConfig->fltMechIqStartup < 0.0F)                ||
       (psMeasConfig->fltMechMergingCoeff < 10.0F)            ||
       (psMeasConfig->fltMechMergingCoeff > 200.0F)           ||
       (psMeasConfig->fltMechIqAccelerate > MID_I_MEAS_MAX)   ||
       (psMeasConfig->fltMechIqAccelerate < 0.0F)             ||
       (psMeasConfig->fltMechIqDecelerate > MID_I_MEAS_MAX)   ||
       (psMeasConfig->fltMechIqDecelerate < 0.0F)             ||
       (psMeasConfig->fltMechSpeedThrsAccel > MID_N_MEAS_MAX) ||
       (psMeasConfig->fltMechSpeedThrsAccel < MID_N_MEAS_MIN) ||
       (psMeasConfig->fltMechSpeedThrsDecel > MID_N_MEAS_MAX) ||
       (psMeasConfig->fltMechSpeedThrsDecel < MID_N_MEAS_MIN) ||
       (psMeasConfig->fltMechSpeedThrsInteg > MID_N_MEAS_MAX) ||
       (psMeasConfig->fltMechSpeedThrsInteg < MID_N_MEAS_MIN))
    {
        /* Report Mech parameters out of range. */
        eRetVal = kMID_MechMeasCfgOutOfRange;
    }
    else
    {
        /* Set the Mech measurement configuration. */
        g_sMID.sMIDMech.fltKt               = psMeasConfig->fltMechKt;
        g_sMID.sMIDMech.fltIqStartup        = psMeasConfig->fltMechIqStartup;
        g_sMID.sMIDMech.fltMechMergingCoeff = psMeasConfig->fltMechMergingCoeff;
        g_sMID.sMIDMech.fltIqAccelerate     = psMeasConfig->fltMechIqAccelerate;
        g_sMID.sMIDMech.fltIqDecelerate     = psMeasConfig->fltMechIqDecelerate;
        g_sMID.sMIDMech.fltSpeedThrsAccel   = psMeasConfig->fltMechSpeedThrsAccel;
        g_sMID.sMIDMech.fltSpeedThrsDecel   = psMeasConfig->fltMechSpeedThrsDecel;
        g_sMID.sMIDMech.fltSpeedThrsInteg   = psMeasConfig->fltMechSpeedThrsInteg;
    }

    return eRetVal;
}


/*                             STATIC FUNCTIONS                               */
/*!
 * @brief Function calculated BEMF and Tracking observer params from g_sMID.sMotorParams
 *
 * @param sBemfParams - Bemf observer parameters.
 *        sTOParams - Tracking observer parameters.
 *
 * @return MID configuration result
 */
static void MID_CalcBemfTrackingObsrv(mid_bemf_params_t *sBemfParams, mid_to_params_t *sTOParams)
{
    /* Calculate Bemf observer parameters:
        I_gain  = Ld/(Ld+Ts*Rs);
        U_gain  = Ts/(Ld+Ts*Rs);
        E_gain  = Ts/(Ld+Ts*Rs);
        WI_gain = Lq*Ts/(Ld+Ts*Rs);
        BemfDQ_Kpz = 2*BEMF_DQ_att*2*Math.PI*BEMF_DQ_f0*Ld-Rs
        BemfDQ_Kiz = Ld*Math.pow(2*Math.PI*BEMF_DQ_f0,2)*Ts
    */
    sBemfParams->fltI_gain  = g_sMID.sMotorParams.fltLd/(g_sMID.sMotorParams.fltLd+MID_FAST_LOOP_TS*g_sMID.sMotorParams.fltRs);
    sBemfParams->fltU_gain  = MID_FAST_LOOP_TS/(g_sMID.sMotorParams.fltLd+MID_FAST_LOOP_TS*g_sMID.sMotorParams.fltRs);
    sBemfParams->fltE_gain  = MID_FAST_LOOP_TS/(g_sMID.sMotorParams.fltLd+MID_FAST_LOOP_TS*g_sMID.sMotorParams.fltRs);
    sBemfParams->fltWI_gain = g_sMID.sMotorParams.fltLq*MID_FAST_LOOP_TS/(g_sMID.sMotorParams.fltLd+MID_FAST_LOOP_TS*g_sMID.sMotorParams.fltRs);
    sBemfParams->fltBemfDQ_Kps = MID_BEMF_COEFF_KPZ*g_sMID.sMotorParams.fltLd-g_sMID.sMotorParams.fltRs;
    sBemfParams->fltBemfDQ_Kis = g_sMID.sMotorParams.fltLd*MID_BEMF_COEFF_KIZ*MID_FAST_LOOP_TS;

    /* Calculate Tracking observer parameters:
        TO_Kpz = 2*TO_att*2*Math.PI*TO_f0
        TO_Kiz = Math.pow(2*Math.PI*TO_f0,2)*Ts
        TO_Theta = Ts/Math.PI
    */
    sTOParams->fltTO_Kpz = MID_TO_COEFF_KPZ;
    sTOParams->fltTO_Kiz = MID_TO_COEFF_KIZ*MID_FAST_LOOP_TS;
    sTOParams->fltTO_Theta = MID_TO_COEFF_THETA*MID_FAST_LOOP_TS;
}
