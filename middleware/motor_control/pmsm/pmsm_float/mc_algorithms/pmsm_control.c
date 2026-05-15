/*
* Copyright 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024-2025 NXP
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

#include "pmsm_control.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
RAM_FUNC_LIB
void MCS_PMSMOpenloop(mcs_openloop_t *psOpenloop){

	psOpenloop->f16PosElExt = MLIB_Add_F16(
		psOpenloop->f16Theta,
		GFLIB_Integrator_F16(
			MLIB_ConvSc_F16ff(
				psOpenloop->fltFreqReq,
				psOpenloop->fltFreqMax
			),
			&psOpenloop->sFreqIntegrator
		)
	);

}

/*!
 * @brief PMSM field oriented current control.
 *
 * This function is used to compute PMSM field oriented current control.
 *
 * @param psFocPMSM     The pointer of the PMSM FOC structure
 *
 * @return None
 */
RAM_FUNC_LIB  
void MCS_PMSMFocCtrl(mcs_pmsm_foc_t *psFocPMSM)
{
    /* pass electrical position from outside function if enabled else estimated
     * electrical position is selected */
    if (psFocPMSM->bPosExtOn)
    {
        psFocPMSM->f16PosEl = psFocPMSM->f16PosElExt;
    }
    else
    {
        psFocPMSM->f16PosEl = psFocPMSM->f16PosElEst;
    }

    /* Position angle of the last PWM update */
    psFocPMSM->sAnglePosEl.fltSin = GFLIB_Sin_FLTa((acc32_t)psFocPMSM->f16PosElEst);
    psFocPMSM->sAnglePosEl.fltCos = GFLIB_Cos_FLTa((acc32_t)psFocPMSM->f16PosElEst);

    /* 3-phase to 2-phase transformation to stationary ref. frame */
    GMCLIB_Clark_FLT(&psFocPMSM->sIABC, &psFocPMSM->sIAlBe);

    /* 2-phase to 2-phase transformation to rotary ref. frame */
    GMCLIB_Park_FLT(&psFocPMSM->sIAlBe, &psFocPMSM->sAnglePosEl, &psFocPMSM->sIDQ);
    GMCLIB_Park_FLT(&psFocPMSM->sUAlBeReq, &psFocPMSM->sAnglePosEl, &psFocPMSM->sUDQEst);

    /* BEMF observer in DQ system */
    psFocPMSM->acc32BemfErr = AMCLIB_PMSMBemfObsrvDQ_A32fff(&psFocPMSM->sIDQ, &psFocPMSM->sUDQEst,
                                                            psFocPMSM->fltSpeedElEst, &psFocPMSM->sBemfObsrv);

    psFocPMSM->f16PosElEst = (frac16_t)AMCLIB_TrackObsrv_A32af(psFocPMSM->acc32BemfErr, &psFocPMSM->sTo);

    /* Speed estimation filter  */
    psFocPMSM->fltSpeedElEst = GDFLIB_FilterIIR1_FLT(psFocPMSM->sTo.fltSpeed, &psFocPMSM->sSpeedElEstFilt);

    /* for open loop control enabled parallel running of observer and FOC
     * open loop electrical position passed to rest of FOC */
    if (psFocPMSM->bOpenLoop || psFocPMSM->bPosExtOn)
    {
        psFocPMSM->sAnglePosEl.fltSin = GFLIB_Sin_FLTa((acc32_t)psFocPMSM->f16PosEl);
        psFocPMSM->sAnglePosEl.fltCos = GFLIB_Cos_FLTa((acc32_t)psFocPMSM->f16PosEl);
        GMCLIB_Park_FLT(&psFocPMSM->sIAlBe, &psFocPMSM->sAnglePosEl, &psFocPMSM->sIDQ);
    }

    /* perform current control loop if enabled */
    if (psFocPMSM->bCurrentLoopOn)
    {
        /* D current error calculation */
        psFocPMSM->sIDQError.fltD = MLIB_Sub_FLT(psFocPMSM->sIDQReq.fltD, psFocPMSM->sIDQ.fltD);
        
#if defined(Q_CURRENT_ZC_FILTER)     
        /* Zero cancellation filter */
        psFocPMSM->sIDQReqFilt.fltQ = GDFLIB_FilterIIR1_FLT(psFocPMSM->sIDQReq.fltQ, &psFocPMSM->sIqReqZCFilter);
        
        /* Q current error calculation */
        psFocPMSM->sIDQError.fltQ = MLIB_Sub_FLT(psFocPMSM->sIDQReqFilt.fltQ, psFocPMSM->sIDQ.fltQ);
#else
        psFocPMSM->sIDQError.fltQ = MLIB_Sub_FLT(psFocPMSM->sIDQReq.fltQ, psFocPMSM->sIDQ.fltQ);
#endif

        /*** D - controller limitation calculation ***/
        psFocPMSM->sIdPiParams.fltLowerLim = MLIB_MulNeg_FLT(psFocPMSM->fltDutyCycleLimit, psFocPMSM->fltUDcBusFilt);
        psFocPMSM->sIdPiParams.fltUpperLim = MLIB_Mul_FLT(psFocPMSM->fltDutyCycleLimit, psFocPMSM->fltUDcBusFilt);

        /* D current PI controller */
        psFocPMSM->sUDQReq.fltD =
            GFLIB_CtrlPIpAW_FLT(psFocPMSM->sIDQError.fltD, &psFocPMSM->bIdPiStopInteg, &psFocPMSM->sIdPiParams);

        /*** Q - controller limitation calculation ***/
        psFocPMSM->sIqPiParams.fltUpperLim =
            GFLIB_Sqrt_FLT(psFocPMSM->sIdPiParams.fltUpperLim * psFocPMSM->sIdPiParams.fltUpperLim -
                           psFocPMSM->sUDQReq.fltD * psFocPMSM->sUDQReq.fltD);
        psFocPMSM->sIqPiParams.fltLowerLim = MLIB_Neg_FLT(psFocPMSM->sIqPiParams.fltUpperLim);

        /* Q current PI controller */
        psFocPMSM->sUDQReq.fltQ =
            GFLIB_CtrlPIpAW_FLT(psFocPMSM->sIDQError.fltQ, &psFocPMSM->bIqPiStopInteg, &psFocPMSM->sIqPiParams);
    }

    /* 2-phase to 2-phase transformation to stationary ref. frame */
    GMCLIB_ParkInv_FLT(&psFocPMSM->sUDQReq, &psFocPMSM->sAnglePosEl, &psFocPMSM->sUAlBeReq);

    /* DCBus ripple elimination */
    GMCLIB_ElimDcBusRipFOC_F16ff(psFocPMSM->fltUDcBusFilt, &psFocPMSM->sUAlBeReq, &psFocPMSM->sUAlBeCompFrac);

    /* space vector modulation */
    psFocPMSM->ui16SectorSVM = GMCLIB_SvmStd_F16(&psFocPMSM->sUAlBeCompFrac, &psFocPMSM->sDutyABC);
}

/*!
 * @brief Optimized PMSM field oriented current control.
 *
 * This function is used to compute PMSM field oriented current control.
 *
 * @param psFocPMSM     The pointer of the PMSM FOC structure
 *
 * @return None
 */
RAM_FUNC_LIB  
void MCS_PMSMFocCtrl_Optim(mcs_pmsm_foc_t *psFocPMSM)
{
    /* 3-phase to 2-phase transformation to stationary ref. frame */
    GMCLIB_Clark_FLT(&psFocPMSM->sIABC, &psFocPMSM->sIAlBe);

    /* 2-phase to 2-phase transformation to rotary ref. frame */
    GFLIB_SinCos_FLTa((acc32_t)psFocPMSM->f16PosElExt, &psFocPMSM->sAnglePosEl);
    
    GMCLIB_Park_FLT(&psFocPMSM->sIAlBe, &psFocPMSM->sAnglePosEl, &psFocPMSM->sIDQ);

    /* perform current control loop if enabled */
    if (psFocPMSM->bCurrentLoopOn)
    {
        /* D current error calculation */
        psFocPMSM->sIDQError.fltD = MLIB_Sub_FLT(psFocPMSM->sIDQReq.fltD, psFocPMSM->sIDQ.fltD);
        
#if defined(Q_CURRENT_ZC_FILTER)      
        /* Zero cancellation filter */
        psFocPMSM->sIDQReqFilt.fltQ = GDFLIB_FilterIIR1_FLT(psFocPMSM->sIDQReq.fltQ, &psFocPMSM->sIqReqZCFilter);
        
        /* Q current error calculation */
        psFocPMSM->sIDQError.fltQ = MLIB_Sub_FLT(psFocPMSM->sIDQReqFilt.fltQ, psFocPMSM->sIDQ.fltQ);
#else
        psFocPMSM->sIDQError.fltQ = MLIB_Sub_FLT(psFocPMSM->sIDQReq.fltQ, psFocPMSM->sIDQ.fltQ);
#endif
        
        /* D current PI controller */
        psFocPMSM->sUDQReq.fltD =
            GFLIB_CtrlPIpAW_FLT(psFocPMSM->sIDQError.fltD, &psFocPMSM->bIdPiStopInteg, &psFocPMSM->sIdPiParams);
        
        /* Q current PI controller */
        psFocPMSM->sUDQReq.fltQ =
            GFLIB_CtrlPIpAW_FLT(psFocPMSM->sIDQError.fltQ, &psFocPMSM->bIqPiStopInteg, &psFocPMSM->sIqPiParams);
    }

    /* 2-phase to 2-phase transformation to stationary ref. frame */
    GMCLIB_ParkInv_FLT(&psFocPMSM->sUDQReq, &psFocPMSM->sAnglePosEl, &psFocPMSM->sUAlBeReq);

    /* DCBus ripple elimination */
    GMCLIB_ElimDcBusRipFOC_F16ff(psFocPMSM->fltUDcBusFilt, &psFocPMSM->sUAlBeReq, &psFocPMSM->sUAlBeCompFrac);

    /* space vector modulation */
    psFocPMSM->ui16SectorSVM = GMCLIB_SvmStd_F16(&psFocPMSM->sUAlBeCompFrac, &psFocPMSM->sDutyABC);
}

/*!
 * @brief PMSM field oriented speed control.
 *
 * This function is used to compute PMSM field oriented speed control.
 *
 * @param psSpeed       The pointer of the PMSM speed structure
 *
 * @return None
 */
RAM_FUNC_LIB  
void MCS_PMSMFocCtrlSpeed(mcs_speed_t *psSpeed)
{
    /* Speed saturation flag given by the Q current controller saturation flag and speed controller saturation flag */
    psSpeed->bSpeedPiStopInteg = (bool_t)((psSpeed->sSpeedPiParams.bLimFlag | psSpeed->bIqPiLimFlag) &
    		(bool_t)(MLIB_Abs_FLT(psSpeed->fltSpeedCmd) >= MLIB_Abs_FLT(psSpeed->fltSpeedFilt)));

    /* Speed ramp generation - due to FAULT and FREEWHEEL working */
    psSpeed->fltSpeedRamp = GFLIB_Ramp_FLT(psSpeed->fltSpeedCmd, &psSpeed->sSpeedRampParams);

    if(psSpeed->bSpeedZCOn)
    {
        /* Speed zero cancellation filter - required speed is fltSpeedRamp */
        psSpeed->fltSpeedCmdFilt  = GDFLIB_FilterIIR1_FLT(psSpeed->fltSpeedRamp, &psSpeed->sSpeedCmdZCFilter); 
        
        /* Speed zero cancellation filter - required speed is fltSpeedCmd, can be used during step response tuning */
        //psSpeed->fltSpeedCmdFilt  = GDFLIB_FilterIIR1_FLT(psSpeed->fltSpeedCmd, &psSpeed->sSpeedCmdZCFilter);
        
        /* Speed error calculation */
        psSpeed->fltSpeedError = MLIB_Sub_FLT(psSpeed->fltSpeedCmdFilt, psSpeed->fltSpeedFilt); 
    }
    else
    {
        /* Speed error calculation */
        psSpeed->fltSpeedError = MLIB_Sub_FLT(psSpeed->fltSpeedRamp, psSpeed->fltSpeedFilt);
    }
      
    /* Desired current by the speed PI controller */
    psSpeed->fltIqReq =
        GFLIB_CtrlPIpAW_FLT(psSpeed->fltSpeedError, &psSpeed->bSpeedPiStopInteg, &psSpeed->sSpeedPiParams);
}

/*!
 * @brief PMSM field oriented position control.
 *
 * This function is used to compute PMSM field oriented position control.
 *
 * @param psSpeed       The pointer of the PMSM position structure
 *
 * @return None
 */
RAM_FUNC_LIB  
void MCS_PMSMFocCtrlPosition(mcs_position_t *psPosition)
{
    /* Position error calculation */
    psPosition->fltPositionError = MLIB_Conv_FLTa((psPosition->a32PositionCmd - psPosition->a32Position));
    
    /* Convert acc32 to flt */
    psPosition->fltPositionCmd = MLIB_Conv_FLTa(psPosition->a32PositionCmd);
    psPosition->fltPosition = MLIB_Conv_FLTa(psPosition->a32Position);      
    
    /* If Feed-Forward is on */
    if(psPosition->bFeedFrwdOn)
    {  
      psPosition->fltPositionCmdDelta = MLIB_Sub_FLT(psPosition->fltPositionCmd, psPosition->fltPositionCmd_stored);
                                         
      /* Limit fltPositionCmdDelta due to very high value after derivation */
      GFLIB_Limit_FLT(psPosition->fltPositionCmdDelta, -0.5F, 0.5F); 
               
      psPosition->fltFirstDerivation = MLIB_Div_FLT((psPosition->fltPositionCmdDelta), (psPosition->fltSpeedLoopTs)); 
      
      psPosition->fltFirstDerivationDelta = MLIB_Sub_FLT((psPosition->fltFirstDerivation), (psPosition->fltFirstDerivation_stored));
      
      /* Limit fltFirstDerivationDelta due to very high value after derivation */
      GFLIB_Limit_FLT(psPosition->fltFirstDerivationDelta, -0.8F, 0.8F); 
      
      psPosition->fltSecondDerivation = MLIB_Div_FLT(psPosition->fltFirstDerivationDelta, (psPosition->fltSpeedLoopTs));
    
      psPosition->fltSpeedFeedFrwdK1 = MLIB_Mul_FLT(psPosition->fltFirstDerivation, psPosition->fltFeedFrwdK1);
      psPosition->fltSpeedFeedFrwdK2 = MLIB_Mul_FLT(psPosition->fltSecondDerivation, psPosition->fltFeedFrwdK2);
      
    }
    else
    {
      psPosition->fltSpeedFeedFrwdK1 = 0.0F;
      psPosition->fltSpeedFeedFrwdK2 = 0.0F;
    }
      
    /* Position P controller output */
    psPosition->fltSpeedReq = GFLIB_CtrlPIpAW_FLT(psPosition->fltPositionError, &psPosition->bPositionPiStopInteg, &psPosition->sPositionPiParams) + psPosition->fltSpeedFeedFrwdK1 + psPosition->fltSpeedFeedFrwdK2;
    
    /* Store actual fltPositionCmd needed for next derivation */
    psPosition->fltPositionCmd_stored = psPosition->fltPositionCmd;
    /* Store actual fltFirstDerivation needed for next derivation */
    psPosition->fltFirstDerivation_stored = psPosition->fltFirstDerivation;
    
    /* Speed saturation flag given by the Q current controller saturation flag and speed controller saturation flag */
    psPosition->bSpeedPiStopInteg = (bool_t)((psPosition->sSpeedPiParams.bLimFlag | psPosition->bIqPiLimFlag) &
    		(bool_t)(MLIB_Abs_FLT(psPosition->fltSpeedReq) >= MLIB_Abs_FLT(psPosition->fltSpeedFilt)));

    /* Speed zero cancellation filter */
    psPosition->fltSpeedReqFilt  = GDFLIB_FilterIIR1_FLT(psPosition->fltSpeedReq, &psPosition->sSpeedReqZCFilter);
    /* Speed error calculation */
    psPosition->fltSpeedError = MLIB_Sub_FLT(psPosition->fltSpeedReqFilt, psPosition->fltSpeedFilt);

    /* Desired current by the speed PI controller */
    psPosition->fltIqReq =  GFLIB_CtrlPIpAW_FLT(psPosition->fltSpeedError, &psPosition->bSpeedPiStopInteg, &psPosition->sSpeedPiParams);
}

/*!
 * @brief PMSM 2-step rotor alignment - 120deg in first step and 0deg in second.
 *
 * This function is used for alignment rotor in two steps - 120deg in first step and 0deg in second
 *
 * @param psAlignment   The pointer of the motor control alignment structure
 *
 * @return None
 */
RAM_FUNC_LIB  
void MCS_PMSMAlignment(mcs_alignment_t *psAlignment)
{
    /* first half duration time is position set to 120 degree */
    if (psAlignment->ui16TimeHalf > 0U)
    {
        psAlignment->f16PosAlign = FRAC16(120.0 / 180.0);
        psAlignment->ui16TimeHalf--;
    }
    else
    {
        psAlignment->f16PosAlign = FRAC16(0.0);
    }
}

/*!
 * @brief PMSM Open Loop Start-up
 *
 * This function is used to PMSM Open Loop Start-up
 *
 * @param psStartUp     The pointer of the PMSM open loop start up parameters structure
 *
 * @return None
 */
RAM_FUNC_LIB  
void MCS_PMSMOpenLoopStartUp(mcs_pmsm_startup_t *psStartUp)
{
    /* Open loop startup speed ramp */
    psStartUp->fltSpeedRampOpenLoop = GFLIB_Ramp_FLT(psStartUp->fltSpeedReq, &psStartUp->sSpeedRampOpenLoopParams);

    /* generation of open loop position from the required speed */
    psStartUp->f16PosGen = GFLIB_Integrator_F16(
        MLIB_ConvSc_F16ff(psStartUp->fltSpeedRampOpenLoop, psStartUp->fltSpeedMax), &psStartUp->sSpeedIntegrator);

    /* position merging starts above merging speed threshold*/
    if (MLIB_Abs_FLT(psStartUp->fltSpeedRampOpenLoop) >= psStartUp->fltSpeedCatchUp)
    {
        /* increment position merging coefficient */
        psStartUp->f16RatioMerging = MLIB_AddSat_F16(psStartUp->f16RatioMerging, psStartUp->f16CoeffMerging);

        /* merging equation */
        psStartUp->f16PosMerged = MLIB_Add_F16(
            psStartUp->f16PosGen,
            MLIB_Mul_F16(MLIB_Sub_F16(psStartUp->f16PosEst, psStartUp->f16PosGen), psStartUp->f16RatioMerging));
    }
    else
    {
        psStartUp->f16PosMerged = psStartUp->f16PosGen;
    }

    /* clear open loop flag */
    if (psStartUp->f16RatioMerging == FRAC16(1.0))
    {
        psStartUp->bOpenLoop = FALSE;
    }
}

/*!
 * @brief PMSM scalar control, voltage is set based on required speed
 *
 * This function is used for alignment rotor in two steps - 120deg in first step and 0deg in second
 *
 * @param psScalarPMSM   The pointer of the PMSM scalar control structure
 *
 * @return None
 */
RAM_FUNC_LIB  
void MCS_PMSMScalarCtrl(mcs_pmsm_scalar_ctrl_t *psScalarPMSM)
{
    /* this part of code is executed when scalar control is turned-on */
    /* frequency ramp */
    psScalarPMSM->fltFreqRamp = GFLIB_Ramp_FLT(psScalarPMSM->fltFreqCmd, &psScalarPMSM->sFreqRampParams);

    /* voltage calculation */
    psScalarPMSM->sUDQReq.fltQ = psScalarPMSM->fltVHzGain * psScalarPMSM->fltFreqRamp;
    psScalarPMSM->sUDQReq.fltD = 0.0F;

    /* voltage limitation to f16UqMin */
    if (psScalarPMSM->sUDQReq.fltQ >= 0.0F)
    {
        psScalarPMSM->sUDQReq.fltQ = GFLIB_LowerLimit_FLT(psScalarPMSM->sUDQReq.fltQ,
                                                                     psScalarPMSM->fltUqMin);
    }
    else
    {
        psScalarPMSM->sUDQReq.fltQ = GFLIB_UpperLimit_FLT(psScalarPMSM->sUDQReq.fltQ,
                                                                     MLIB_Neg_FLT(psScalarPMSM->fltUqMin));
    }

    /* stator voltage angle , used the same integrator as for the open-loop start up*/
    psScalarPMSM->f16PosElScalar = GFLIB_Integrator_F16(
        MLIB_ConvSc_F16ff(psScalarPMSM->fltFreqRamp, psScalarPMSM->fltFreqMax), &psScalarPMSM->sFreqIntegrator);
}
