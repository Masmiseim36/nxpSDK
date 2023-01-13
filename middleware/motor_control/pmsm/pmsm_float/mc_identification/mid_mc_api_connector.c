/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "mid_mc_api_connector.h"
#include "mc_periph_init.h"

#ifdef PMSM_SNSLESS_ENC
#include "m1_sm_snsless_enc.h"
#else
#include "m1_sm_snsless.h"
#endif

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
    float_t                     fltSpdElEstFilt;    /* Filtered Estimated Speed. */
    float_t                     fltUDCbusFilt;      /* Filtered DCbus voltage. */
    frac16_t                    f16PosElEst;        /* Actual estimated position. */
    frac16_t                    f16PosElCmd;        /* The position command. */
    frac16_t                    f16FrqElCmd;        /* The frequency command. */
    bool_t                      bUseEstPosTransform;/* Estimated/OL position swith. */
}mid_api_connector_t;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
extern volatile float g_fltMIDvoltageScale;
extern volatile float g_fltMIDDCBvoltageScale;
extern volatile float g_fltMIDcurrentScale;
extern volatile float g_fltMIDspeedScale;
extern volatile float g_fltMIDspeedAngularScale;
extern volatile float g_fltMIDspeedMechanicalScale;

/* Control FOC and MID */
extern mid_pmsm_t g_sMidDrive;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * Set External Position to FOC
 */
RAM_FUNC_LIB
void MID_MC_SetExternalPosEl(frac16_t f16PosExt)
{
        g_sMidDrive.sFocPMSM.f16PosElExt = f16PosExt;
}

/*!
 * Get Actual External Position from FOC
 */
RAM_FUNC_LIB
frac16_t MID_MC_GetExternalPosEl(void)
{
    return g_sMidDrive.sFocPMSM.f16PosElExt;      
}

/*!
 * Get actual Estimated Position from FOC
 */
RAM_FUNC_LIB
frac16_t MID_MC_GetEstimatedPosEl(void)
{
    return g_sMidDrive.sFocPMSM.f16PosElEst;     
}

/*!
 * Set required UDQ voltage
 */
void MID_MC_SetUDQReq(GMCLIB_2COOR_DQ_T_FLT sUDQReq)
{
    g_sMidDrive.sFocPMSM.sUDQReq.fltD = sUDQReq.fltD;
    g_sMidDrive.sFocPMSM.sUDQReq.fltQ = sUDQReq.fltQ;
}

/*!
 * Get required UDQ voltage
 */
void MID_MC_GetUDQReq(GMCLIB_2COOR_DQ_T_FLT *sUDQReq)
{
    sUDQReq->fltD = g_sMidDrive.sFocPMSM.sUDQReq.fltD;
    sUDQReq->fltQ = g_sMidDrive.sFocPMSM.sUDQReq.fltQ;
}

/*!
 * Get observed EDQ voltage
 */
RAM_FUNC_LIB
void MID_MC_GetEDQObsrv(GMCLIB_2COOR_DQ_T_FLT *sEDQObsrv)
{
    sEDQObsrv->fltD = g_sMidDrive.sFocPMSM.sBemfObsrv.sEObsrv.fltD;
    sEDQObsrv->fltQ = g_sMidDrive.sFocPMSM.sBemfObsrv.sEObsrv.fltQ;
}

/*!
 * Set required IDQ current
 */
RAM_FUNC_LIB
void MID_MC_SetIDQReq(GMCLIB_2COOR_DQ_T_FLT sIDQReq)
{
    g_sMidDrive.sFocPMSM.sIDQReq.fltD = sIDQReq.fltD;
    g_sMidDrive.sFocPMSM.sIDQReq.fltQ = sIDQReq.fltQ;
}

/*!
 * Get actual IDQ current
 */
RAM_FUNC_LIB
void MID_MC_GetIDQfbck(GMCLIB_2COOR_DQ_T_FLT *sIDQfbck)
{
    sIDQfbck->fltD = g_sMidDrive.sFocPMSM.sIDQ.fltD;                  
    sIDQfbck->fltQ = g_sMidDrive.sFocPMSM.sIDQ.fltQ;       
}

/*!
 * Get actual DCbus voltage
 */
float_t MID_MC_GetUDCbusFilt(void)
{
    return g_sMidDrive.sFocPMSM.fltUDcBusFilt;    
}

/*!
 * Get actual Estimated Electrical Speed
 */
RAM_FUNC_LIB
float_t MID_MC_GetSpeedElEst(void)
{
    return g_sMidDrive.sFocPMSM.sTo.fltSpeed;   
}

/*!
 * Enable FOC Current Loop On/Off
 */
RAM_FUNC_LIB
void MID_MC_EnableCurrentLoop(bool_t bValue)
{
    g_sMidDrive.sFocPMSM.bCurrentLoopOn = bValue;
}

/*!
 * Use Estimated position for Parks transformation
 */
RAM_FUNC_LIB
void MID_MC_UseEstimatedPos(bool_t bValue)
{
    g_sMidDrive.sFocPMSM.bOpenLoop = TRUE;
    g_sMidDrive.sFocPMSM.bPosExtOn = !bValue;
}

/*!
 * Update Bemf Observer
 */
RAM_FUNC_LIB
void MID_MC_UpdateBemfObsrv(float_t fltI_gain,
                        float_t fltU_gain,
                        float_t fltE_gain,
                        float_t fltWI_gain,
                        float_t fltBemfDQ_Kps,
                        float_t fltBemfDQ_Kis)
{
    /* Update Bemf Observer parameters. */
    g_sMidDrive.sFocPMSM.sBemfObsrv.fltIGain = fltI_gain;
    g_sMidDrive.sFocPMSM.sBemfObsrv.fltUGain = fltU_gain;
    g_sMidDrive.sFocPMSM.sBemfObsrv.fltEGain = fltE_gain;
    g_sMidDrive.sFocPMSM.sBemfObsrv.fltWIGain = fltWI_gain;
    g_sMidDrive.sFocPMSM.sBemfObsrv.sCtrl.fltPGain = fltBemfDQ_Kps;
    g_sMidDrive.sFocPMSM.sBemfObsrv.sCtrl.fltIGain = fltBemfDQ_Kis;

    /* Reset observer. */
    AMCLIB_PMSMBemfObsrvDQInit_A32fff(&g_sMidDrive.sFocPMSM.sBemfObsrv);
}

/*!
 * Update Tracking Observer
 */
RAM_FUNC_LIB
void MID_MC_UpdateTrackingObsrv(float_t fltTO_Kpz,
                                float_t fltTO_Kiz,
                                float_t fltTO_Theta)
{
    /* Update Tracking Observer parameters. */
    g_sMidDrive.sFocPMSM.sTo.fltPGain = fltTO_Kpz;
    g_sMidDrive.sFocPMSM.sTo.fltIGain = fltTO_Kiz;
    g_sMidDrive.sFocPMSM.sTo.fltThGain = fltTO_Theta;

    /* Reset observer. */
    AMCLIB_TrackObsrvInit_A32af(ACC32(0.0) ,&g_sMidDrive.sFocPMSM.sTo);     
    g_sMidDrive.sFocPMSM.f16PosElEst = FRAC16(0.0);                   
    g_sMidDrive.sFocPMSM.fltSpeedElEst = 0.0F;                             

    /* Initialize the speed filter. */
    g_sMidDrive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltB0 = MID_SPEED_FILTER_IIR_B0;
    g_sMidDrive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltB1 = MID_SPEED_FILTER_IIR_B1;
    g_sMidDrive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltA1 = MID_SPEED_FILTER_IIR_A1;
    GDFLIB_FilterIIR1Init_FLT(&g_sMidDrive.sFocPMSM.sSpeedElEstFilt);
}

/*!
 * Function reads voltages, currents, etc. for MID. Called before MID state
 * machine. User defined.
 */
RAM_FUNC_LIB
void MID_MC_ReadSignals(void)
{
    frac16_t f16PosElPark;

    /* get all adc samples - DC-bus voltage, current, bemf and aux sample */
    M1_MCDRV_ADC_GET(&g_sM1AdcSensor); 
    
    /* Convert phase currents from fractional measured values to float */
    g_sMidDrive.sFocPMSM.sIABC.fltA = MLIB_ConvSc_FLTsf(g_sMidDrive.sFocPMSM.sIABCFrac.f16A, g_fltMIDcurrentScale);
    g_sMidDrive.sFocPMSM.sIABC.fltB = MLIB_ConvSc_FLTsf(g_sMidDrive.sFocPMSM.sIABCFrac.f16B, g_fltMIDcurrentScale);
    g_sMidDrive.sFocPMSM.sIABC.fltC = MLIB_ConvSc_FLTsf(g_sMidDrive.sFocPMSM.sIABCFrac.f16C, g_fltMIDcurrentScale);
    
    /* 3-phase to 2-phase transformation to stationary ref. frame */
    GMCLIB_Clark_FLT(&g_sMidDrive.sFocPMSM.sIABC, &g_sMidDrive.sFocPMSM.sIAlBe);
    
    /* Convert voltages from fractional measured values to float */
    g_sMidDrive.sFocPMSM.fltUDcBus = MLIB_ConvSc_FLTsf(g_sMidDrive.sFocPMSM.f16UDcBus, g_fltMIDDCBvoltageScale);                    
    /* Sampled DC-Bus voltage filter */
    g_sMidDrive.sFocPMSM.fltUDcBusFilt = GDFLIB_FilterIIR1_FLT(g_sMidDrive.sFocPMSM.fltUDcBus, &g_sMidDrive.sFocPMSM.sUDcBusFilter);
    
    /* Decide which position will be used for Park transformation. */
    if(g_sMidDrive.sFocPMSM.bPosExtOn)  
    {
        f16PosElPark = g_sMidDrive.sFocPMSM.f16PosElExt; 
    }
    else
    {
        f16PosElPark = g_sMidDrive.sFocPMSM.f16PosElEst;     
    }
     
    /* Position angle of the last PWM update */
    g_sMidDrive.sFocPMSM.sAnglePosEl.fltSin = GFLIB_Sin_FLTa((acc32_t)f16PosElPark);      
    g_sMidDrive.sFocPMSM.sAnglePosEl.fltCos = GFLIB_Cos_FLTa((acc32_t)f16PosElPark);
    
    /* 2-phase to 2-phase transformation to rotary ref. frame */
    GMCLIB_Park_FLT(&g_sMidDrive.sFocPMSM.sIAlBe, &g_sMidDrive.sFocPMSM.sAnglePosEl, &g_sMidDrive.sFocPMSM.sIDQ);
    //GMCLIB_Park_FLT(&g_sMidDrive.sFocPMSM.sUAlBeReq, &g_sMidDrive.sFocPMSM.sAnglePosEl, &g_sMidDrive.sFocPMSM.sUDQEst);
    GMCLIB_Park_FLT(&g_sMidDrive.sFocPMSM.sUAlBeReq, &g_sMidDrive.sFocPMSM.sAnglePosEl, &g_sMidDrive.sFocPMSM.sUDQReq);

    /* BEMF observer in DQ system */
    g_sMidDrive.sFocPMSM.acc32BemfErr = AMCLIB_PMSMBemfObsrvDQ_A32fff(&g_sMidDrive.sFocPMSM.sIDQ, &g_sMidDrive.sFocPMSM.sUDQReq,      
                                                            g_sMidDrive.sFocPMSM.fltSpeedElEst, &g_sMidDrive.sFocPMSM.sBemfObsrv);

    g_sMidDrive.sFocPMSM.f16PosElEst = (frac16_t)AMCLIB_TrackObsrv_A32af(g_sMidDrive.sFocPMSM.acc32BemfErr, &g_sMidDrive.sFocPMSM.sTo);

    /* Speed estimation filter  */
    g_sMidDrive.sFocPMSM.fltSpeedElEst = GDFLIB_FilterIIR1_FLT(g_sMidDrive.sFocPMSM.sTo.fltSpeed, &g_sMidDrive.sFocPMSM.sSpeedElEstFilt);

    /* for open loop control enabled parallel running of observer and FOC
     * open loop electrical position passed to rest of FOC */
    if (g_sMidDrive.sFocPMSM.bOpenLoop || g_sMidDrive.sFocPMSM.bPosExtOn)
    {
        g_sMidDrive.sFocPMSM.sAnglePosEl.fltSin = GFLIB_Sin_FLTa((acc32_t)g_sMidDrive.sFocPMSM.f16PosElExt);
        g_sMidDrive.sFocPMSM.sAnglePosEl.fltCos = GFLIB_Cos_FLTa((acc32_t)g_sMidDrive.sFocPMSM.f16PosElExt);
        GMCLIB_Park_FLT(&g_sMidDrive.sFocPMSM.sIAlBe, &g_sMidDrive.sFocPMSM.sAnglePosEl, &g_sMidDrive.sFocPMSM.sIDQ);
    }
}

/*!
 * Function applying results/output from MID. Called after MID state
 * machine. User defined.
 */
RAM_FUNC_LIB
void MID_MC_ApplySignals(void)
{  
    /* perform current control loop if enabled */
    if (g_sMidDrive.sFocPMSM.bCurrentLoopOn)
    {
        /* D current error calculation */
        g_sMidDrive.sFocPMSM.sIDQError.fltD = MLIB_Sub_FLT(g_sMidDrive.sFocPMSM.sIDQReq.fltD, g_sMidDrive.sFocPMSM.sIDQ.fltD);

        /* Q current error calculation */
        g_sMidDrive.sFocPMSM.sIDQError.fltQ = MLIB_Sub_FLT(g_sMidDrive.sFocPMSM.sIDQReq.fltQ, g_sMidDrive.sFocPMSM.sIDQ.fltQ);

        /*** D - controller limitation calculation ***/
        g_sMidDrive.sFocPMSM.sIdPiParams.fltLowerLim = MLIB_MulNeg_FLT(g_sMidDrive.sFocPMSM.fltDutyCycleLimit, g_sMidDrive.sFocPMSM.fltUDcBusFilt);
        g_sMidDrive.sFocPMSM.sIdPiParams.fltUpperLim = MLIB_Mul_FLT(g_sMidDrive.sFocPMSM.fltDutyCycleLimit, g_sMidDrive.sFocPMSM.fltUDcBusFilt);

        /* D current PI controller */
        g_sMidDrive.sFocPMSM.sUDQReq.fltD =
            GFLIB_CtrlPIpAW_FLT(g_sMidDrive.sFocPMSM.sIDQError.fltD, &g_sMidDrive.sFocPMSM.bIdPiStopInteg, &g_sMidDrive.sFocPMSM.sIdPiParams);

        /*** Q - controller limitation calculation ***/
        g_sMidDrive.sFocPMSM.sIqPiParams.fltUpperLim =
            GFLIB_Sqrt_FLT(g_sMidDrive.sFocPMSM.sIdPiParams.fltUpperLim * g_sMidDrive.sFocPMSM.sIdPiParams.fltUpperLim -
                           g_sMidDrive.sFocPMSM.sUDQReq.fltD * g_sMidDrive.sFocPMSM.sUDQReq.fltD);
        g_sMidDrive.sFocPMSM.sIqPiParams.fltLowerLim = MLIB_Neg_FLT(g_sMidDrive.sFocPMSM.sIqPiParams.fltUpperLim);

        /* Q current PI controller */
        g_sMidDrive.sFocPMSM.sUDQReq.fltQ =
            GFLIB_CtrlPIpAW_FLT(g_sMidDrive.sFocPMSM.sIDQError.fltQ, &g_sMidDrive.sFocPMSM.bIqPiStopInteg, &g_sMidDrive.sFocPMSM.sIqPiParams);
    }

    /* 2-phase to 2-phase transformation to stationary ref. frame */
    GMCLIB_ParkInv_FLT(&g_sMidDrive.sFocPMSM.sUDQReq, &g_sMidDrive.sFocPMSM.sAnglePosEl, &g_sMidDrive.sFocPMSM.sUAlBeReq);

    /* DCBus ripple elimination */
    GMCLIB_ElimDcBusRipFOC_F16ff(g_sMidDrive.sFocPMSM.fltUDcBusFilt, &g_sMidDrive.sFocPMSM.sUAlBeReq, &g_sMidDrive.sFocPMSM.sUAlBeCompFrac);

    /* space vector modulation */
    g_sMidDrive.sFocPMSM.ui16SectorSVM = GMCLIB_SvmStd_F16(&g_sMidDrive.sFocPMSM.sUAlBeCompFrac, &g_sMidDrive.sFocPMSM.sDutyABC);  
    
    /* PWM peripheral update */
    M1_MCDRV_PWM3PH_SET(&g_sM1Pwm3ph);   
    
    /* set current sensor for  sampling */
    M1_MCDRV_CURR_3PH_CHAN_ASSIGN(&g_sM1AdcSensor);     
}
