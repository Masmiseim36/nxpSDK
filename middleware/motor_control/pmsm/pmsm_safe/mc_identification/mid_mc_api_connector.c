/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "mid_mc_api_connector.h"

MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

#include "freemaster_tsa_pmsm.h"

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief MID API connector structure. */
typedef struct _mid_api_connector
{
    AMCLIB_BEMF_OBSRV_DQ_T_FLT  sBemfObsrv;         /* MID Bemf Observer structure. */
    AMCLIB_TRACK_OBSRV_T_FLT    sTo;                /* MID Tracking Observer structure. */
    GDFLIB_FILTER_IIR1_T_FLT    sSpdElEstFilt;      /* MID Speed Filter structure. */
    GMCLIB_2COOR_SINCOS_T_F16   sPosElSC;           /* MID Sin/Cos of the actual position. */
    GMCLIB_2COOR_DQ_T_FLT       sUDQReq;            /* Actual UDQ required. */
    GMCLIB_2COOR_DQ_T_FLT       sIDQfbck;           /* Actual IDQ feedback. */
    GMCLIB_2COOR_DQ_T_F16       sDQCmd;             /* The DQ quantity command. */
    mc_status_t                 sStatusMC;          /* MC status. */
    foc_ctrl_mode_cfg_t         sCfg;               /* The control mode (open/close loop, cureent/voltage ctrl) */
    float_t                     fltSpdElEstFilt;    /* Filtered Estimated Speed. */
    float_t                     fltUDCbusFilt;      /* Filtered DCbus voltage. */
    foc_app_cmd_t               eAppCmd;            /* The application command. */
    frac16_t                    f16PosElEst;        /* Actual estimated position. */
    frac16_t                    f16PosElCmd;        /* The position command. */
    frac16_t                    f16FrqElCmd;        /* The frequency command. */
    bool_t                      bUseEstPosTransform;/* Estimated/OL position swith. */
}mid_api_connector_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
static mid_api_connector_t s_sMidApiConn;

MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
FMSTR_TSA_TABLE_BEGIN(TSA_MID_API_CONN)
    /* MID Estimated Position. */
    FMSTR_TSA_RO_VAR(s_sMidApiConn.f16PosElEst,  FMSTR_TSA_FRAC16)
    /* MID Estimated Speed. */
    FMSTR_TSA_RO_VAR(s_sMidApiConn.fltSpdElEstFilt,  FMSTR_TSA_FLOAT)
FMSTR_TSA_TABLE_END()
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * Set MC application on/off
 */
void MID_MC_SetAppSwitch(bool_t bValue)
{
    if(TRUE == bValue)
    {
        s_sMidApiConn.eAppCmd = kFOC_Cmd_Run;
    }
    else
    {
        s_sMidApiConn.eAppCmd = kFOC_Cmd_Stop;
    }
}

/*!
 * Get MC main state (RUN, STOP, ...)
 */
uint16_t MID_MC_GetAppState(void)
{
    return (uint16_t)s_sMidApiConn.sStatusMC.eState;
}

/*!
 * Set External Position to FOC
 */
void MID_MC_SetExternalPosEl(frac16_t f16PosExt)
{
        s_sMidApiConn.f16PosElCmd = f16PosExt;
}

/*!
 * Get Actual External Position from FOC
 */
frac16_t MID_MC_GetExternalPosEl(void)
{
    return s_sMidApiConn.f16PosElCmd;
}

/*!
 * Get actual Estimated Position from FOC
 */
frac16_t MID_MC_GetEstimatedPosEl(void)
{
    return s_sMidApiConn.f16PosElEst;
}

/*!
 * Set required UDQ voltage
 */
void MID_MC_SetUDQReq(GMCLIB_2COOR_DQ_T_FLT sUDQReq)
{
    s_sMidApiConn.sDQCmd.f16D = MLIB_Conv_F16f(sUDQReq.fltD * (float_t)(1.0/M1_U_MAX));
    s_sMidApiConn.sDQCmd.f16Q = MLIB_Conv_F16f(sUDQReq.fltQ * (float_t)(1.0/M1_U_MAX));
}

/*!
 * Get required UDQ voltage
 */
void MID_MC_GetUDQReq(GMCLIB_2COOR_DQ_T_FLT *sUDQReq)
{
    sUDQReq->fltD = s_sMidApiConn.sUDQReq.fltD;
    sUDQReq->fltQ = s_sMidApiConn.sUDQReq.fltQ;
}

/*!
 * Get observed EDQ voltage
 */
void MID_MC_GetEDQObsrv(GMCLIB_2COOR_DQ_T_FLT *sEDQObsrv)
{
    sEDQObsrv->fltD = s_sMidApiConn.sBemfObsrv.sEObsrv.fltD;
    sEDQObsrv->fltQ = s_sMidApiConn.sBemfObsrv.sEObsrv.fltQ;
}

/*!
 * Set required IDQ current
 */
void MID_MC_SetIDQReq(GMCLIB_2COOR_DQ_T_FLT sIDQReq)
{
    s_sMidApiConn.sDQCmd.f16D = MLIB_Conv_F16f(sIDQReq.fltD * (float_t)(1.0/M1_I_MAX));
    s_sMidApiConn.sDQCmd.f16Q = MLIB_Conv_F16f(sIDQReq.fltQ * (float_t)(1.0/M1_I_MAX));
}

/*!
 * Get actual IDQ current
 */
void MID_MC_GetIDQfbck(GMCLIB_2COOR_DQ_T_FLT *sIDQfbck)
{
    sIDQfbck->fltD = s_sMidApiConn.sIDQfbck.fltD;
    sIDQfbck->fltQ = s_sMidApiConn.sIDQfbck.fltQ;
}

/*!
 * Get actual DCbus voltage
 */
float_t MID_MC_GetUDCbusFilt(void)
{
    return s_sMidApiConn.fltUDCbusFilt;
}

/*!
 * Get actual Estimated Electrical Speed
 */
float_t MID_MC_GetSpeedElEst(void)
{
    return s_sMidApiConn.sTo.fltSpeed;
}

/*!
 * Enable FOC Current Loop On/Off
 */
void MID_MC_EnableCurrentLoop(bool_t bValue)
{
    s_sMidApiConn.sCfg.bCurrCtrl = bValue;
}

/*!
 * Use Estimated position for Parks transformation
 */
void MID_MC_UseEstimatedPos(bool_t bValue)
{
    s_sMidApiConn.bUseEstPosTransform = bValue;
    s_sMidApiConn.sCfg.bCloseLoop = FALSE;
}

/*!
 * Update Bemf Observer
 */
void MID_MC_UpdateBemfObsrv(float_t fltI_gain,
                        float_t fltU_gain,
                        float_t fltE_gain,
                        float_t fltWI_gain,
                        float_t fltBemfDQ_Kps,
                        float_t fltBemfDQ_Kis)
{
    /* Update Bemf Observer parameters. */
    s_sMidApiConn.sBemfObsrv.fltIGain = fltI_gain;
    s_sMidApiConn.sBemfObsrv.fltUGain = fltU_gain;
    s_sMidApiConn.sBemfObsrv.fltEGain = fltE_gain;
    s_sMidApiConn.sBemfObsrv.fltWIGain = fltWI_gain;
    s_sMidApiConn.sBemfObsrv.sCtrl.fltPGain = fltBemfDQ_Kps;
    s_sMidApiConn.sBemfObsrv.sCtrl.fltIGain = fltBemfDQ_Kis;

    /* Reset observer. */
    AMCLIB_PMSMBemfObsrvDQInit_A32fff(&s_sMidApiConn.sBemfObsrv);
}

/*!
 * Update Tracking Observer
 */
void MID_MC_UpdateTrackingObsrv(float_t fltTO_Kpz,
                                float_t fltTO_Kiz,
                                float_t fltTO_Theta)
{
    /* Update Tracking Observer parameters. */
    s_sMidApiConn.sTo.fltPGain = fltTO_Kpz;
    s_sMidApiConn.sTo.fltIGain = fltTO_Kiz;
    s_sMidApiConn.sTo.fltThGain = fltTO_Theta;

    /* Reset observer. */
    AMCLIB_TrackObsrvInit_A32af(ACC32(0.0) ,&s_sMidApiConn.sTo);
    s_sMidApiConn.f16PosElEst = FRAC16(0.0);
    s_sMidApiConn.fltSpdElEstFilt = 0.0F;

    /* Initialize the speed filter. */
    s_sMidApiConn.sSpdElEstFilt.sFltCoeff.fltB0 = MID_SPEED_FILTER_IIR_B0;
    s_sMidApiConn.sSpdElEstFilt.sFltCoeff.fltB1 = MID_SPEED_FILTER_IIR_B1;
    s_sMidApiConn.sSpdElEstFilt.sFltCoeff.fltA1 = MID_SPEED_FILTER_IIR_A1;
    GDFLIB_FilterIIR1Init_FLT(&s_sMidApiConn.sSpdElEstFilt);
}

/*!
 * Function reads voltages, currents, etc. for MID. Called before MID state
 * machine. User defined.
 */
void MID_MC_ReadSignals(void)
{
    GMCLIB_2COOR_ALBE_T_F16 sIAlBeFbck, sUAlBeReq;
    GMCLIB_2COOR_DQ_T_F16 sIDQFbck, sUDQReq;
    acc32_t a32Error;
    frac16_t f16PosElPark;

    /* Read FOC status */
    s_sMidApiConn.sStatusMC = FOC_GetMCStatus_SL();

    /* Read alpha-beta stator current and voltage. */
    sUAlBeReq = FOC_GetMeasUAlBe_FL();
    sIAlBeFbck = FOC_GetMeasIAlBe_FL();

    /* Decide which position will be used for Park transformation. */
    if(s_sMidApiConn.bUseEstPosTransform)
    {
        f16PosElPark = s_sMidApiConn.f16PosElEst;
    }
    else
    {
        f16PosElPark = s_sMidApiConn.f16PosElCmd;
    }
    /* Calculate the sine and cosine of the f16PosElPark.  */
    s_sMidApiConn.sPosElSC.f16Sin = GFLIB_Sin_F16(f16PosElPark);
    s_sMidApiConn.sPosElSC.f16Cos = GFLIB_Cos_F16(f16PosElPark);

    /* Transform stator current and voltage from alpha-beta to D-Q */
    GMCLIB_Park_F16(&sUAlBeReq, &s_sMidApiConn.sPosElSC, &sUDQReq);
    GMCLIB_Park_F16(&sIAlBeFbck, &s_sMidApiConn.sPosElSC, &sIDQFbck);

    /* Convert fractional scaled current and voltage to float. */
    s_sMidApiConn.sUDQReq.fltD = MLIB_ConvSc_FLTsf(sUDQReq.f16D, M1_U_MAX);
    s_sMidApiConn.sUDQReq.fltQ = MLIB_ConvSc_FLTsf(sUDQReq.f16Q, M1_U_MAX);
    s_sMidApiConn.sIDQfbck.fltD = MLIB_ConvSc_FLTsf(sIDQFbck.f16D, M1_I_MAX);
    s_sMidApiConn.sIDQfbck.fltQ = MLIB_ConvSc_FLTsf(sIDQFbck.f16Q, M1_I_MAX);

    /* Read DC-bus voltage. */
    s_sMidApiConn.fltUDCbusFilt = MLIB_ConvSc_FLTsf(FOC_GetMeasUdcb_FL(), M1_U_DCB_MAX);

    /* Call Bemf observer. */
    a32Error = AMCLIB_PMSMBemfObsrvDQ_A32fff(&s_sMidApiConn.sIDQfbck, &s_sMidApiConn.sUDQReq,
                                             s_sMidApiConn.fltSpdElEstFilt,
                                             &s_sMidApiConn.sBemfObsrv);

    /* Tracking observer calculation. */
    s_sMidApiConn.f16PosElEst = (frac16_t)AMCLIB_TrackObsrv_A32af(a32Error, &s_sMidApiConn.sTo);

    /* Speed estimation IIR filter.  */
    s_sMidApiConn.fltSpdElEstFilt = GDFLIB_FilterIIR1_FLT(s_sMidApiConn.sTo.fltSpeed, &s_sMidApiConn.sSpdElEstFilt);
}

/*!
 * Function applying results/output from MID. Called after MID state
 * machine. User defined.
 */
void MID_MC_ApplySignals(void)
{
    /* Apply FOC signals. */
    FOC_SetCtrlMID_FL(s_sMidApiConn.eAppCmd,
                      s_sMidApiConn.sDQCmd,
                      s_sMidApiConn.f16FrqElCmd,
                      s_sMidApiConn.f16PosElCmd,
                      s_sMidApiConn.sCfg);
}