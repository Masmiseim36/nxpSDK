/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mech_measure.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MID_MECH_MEAS_NR_DIV_COEFF_MULT (1.0F / (float_t)(MID_MECH_MEAS_NR - 1U))

/*******************************************************************************
 * Code
 ******************************************************************************/

static void MIDpenLoopStartUpA1(mid_ol_startup_t *psStartUp);

/*!
 * @brief Mechanical params measurement routine
 *
 * @param *sMechMeasFcn   input structure of type #mid_get_mech_t for passing
 *                      all necessary parameters.
 *
 * @return None
 */
void MID_getMech(mid_get_mech_t* sMechMeasFcn)
{
    GMCLIB_2COOR_DQ_T_FLT sIDQReq;
    GMCLIB_2COOR_DQ_T_FLT sIDQfbck;
    float_t fltSpeedElEst;
    float_t fltJtemp            = 0.0F;
    float_t fltTautemp          = 0.0F;
    float_t fltInertiaConsttemp = 0.0F;

    /* Initialization */
    if(sMechMeasFcn->bActive == FALSE)
    {
        sMechMeasFcn->bActive            = TRUE;
        sMechMeasFcn->ui16MeasNr            = MID_MECH_MEAS_NR;
        sMechMeasFcn->ui32LoopCounter       = 0U;
        sMechMeasFcn->ui32AccelLoopCounter  = 0U;
        sMechMeasFcn->ui32DecelLoopCounter  = 0U;
        sMechMeasFcn->fltSpeedFilt          = 0.0F;
        sMechMeasFcn->fltTorqueFilt         = 0.0F;
        sMechMeasFcn->fltLPFCoeff           = MID_MECH_LP_FILT; /* 200 Hz filter */
        sMechMeasFcn->eState                = kMID_MechInit;

        sIDQReq.fltD = 0.0F;
        sIDQReq.fltQ = 0.0F;
        MID_MC_SetIDQReq(sIDQReq);

        sMechMeasFcn->fltSpeedInteg         = 0.0F;
        sMechMeasFcn->fltTorqueInteg        = 0.0F;

        /* Open loop start up */
        sMechMeasFcn->sStartup.fltSpeedMax                       = sMechMeasFcn->fltSpeedThrsDecel * 1.1F; /* 10% margin to Decelerate threshold. */
        sMechMeasFcn->sStartup.sSpeedIntegrator.a32Gain          = MLIB_Conv_A32f(sMechMeasFcn->sStartup.fltSpeedMax * ((float_t)(MID_FAST_LOOP_TS / FLOAT_PI)));
        GFLIB_IntegratorInit_F16(0, &sMechMeasFcn->sStartup.sSpeedIntegrator);
        sMechMeasFcn->sStartup.fltSpeedCatchUp                   = sMechMeasFcn->fltSpeedThrsAccel;
        sMechMeasFcn->sStartup.fltSpeedReq                       = sMechMeasFcn->fltSpeedThrsDecel;
        sMechMeasFcn->sStartup.fltCurrentStartup                 = sMechMeasFcn->fltIqStartup;
        sMechMeasFcn->sStartup.f16CoeffMerging                   = MLIB_Conv_F16f((sMechMeasFcn->fltMechMergingCoeff * 0.01F) * sMechMeasFcn->sStartup.fltSpeedCatchUp * ((float_t)(1.0 / (MID_FAST_LOOP_TS * 2.0 * 32768.0))));
        sMechMeasFcn->sStartup.bOpenLoop                         = TRUE;
        GFLIB_RampInit_FLT(0.0F, &sMechMeasFcn->sStartup.sSpeedRampOpenLoopParams);
        sMechMeasFcn->sStartup.f16PosGen                         = 0;
        sMechMeasFcn->sStartup.f16PosMerged                      = 0;
        sMechMeasFcn->sStartup.f16RatioMerging                   = 0;
        sMechMeasFcn->sStartup.fltSpeedRampOpenLoop              = 0.0F;
        sMechMeasFcn->ui32TimeMeasMax                            = 10U * MID_FAST_LOOP_FREQ;
    }

    /* increment loop counter and check the measurement timeout */
    if(sMechMeasFcn->ui32LoopCounter++ > sMechMeasFcn->ui32TimeMeasMax)
    {
        /* assign fault code */
        g_sMID.ui16FaultMID = MID_FAULT_MECH_TIMEOUT;
        /* end measurement */
        sMechMeasFcn->eState = kMID_MechEndOfMeas;
    }

    /* Read IDQfbck */
    MID_MC_GetIDQfbck(&sIDQfbck);

    /* calculate actual torque */
    sMechMeasFcn->fltTorqueEst = 1.5F * sMechMeasFcn->fltPp * sMechMeasFcn->fltKt * sIDQfbck.fltQ;

    /* Get estimated electrical angular speed [rad/s]. */
    fltSpeedElEst = MID_MC_GetSpeedElEst();

    /* filter speed */
    sMechMeasFcn->fltSpeedFilt += sMechMeasFcn->fltLPFCoeff *
        (fltSpeedElEst - sMechMeasFcn->fltSpeedFilt);

    /* filter torque */
    sMechMeasFcn->fltTorqueFilt += sMechMeasFcn->fltLPFCoeff *
        (sMechMeasFcn->fltTorqueEst - sMechMeasFcn->fltTorqueFilt);

    /* measurement state machine */
    switch(sMechMeasFcn->eState)
    {
    case kMID_MechInit:

        /* Apply the startup current */
        sIDQReq.fltD = 0.0F;
        sIDQReq.fltQ = sMechMeasFcn->fltIqStartup;
        MID_MC_SetIDQReq(sIDQReq);

        /* reset loop counter */
        sMechMeasFcn->ui32LoopCounter = 0U;

        /* Go to accelerate state */
        sMechMeasFcn->eState = kMID_MechStartUp;
        break;

    case kMID_MechStartUp:

        /* pass actual estimation position to OL startup structure */
        sMechMeasFcn->sStartup.f16PosEst = MID_MC_GetEstimatedPosEl();

        /*open loop startup */
        MIDpenLoopStartUpA1(&sMechMeasFcn->sStartup);

        /* Position and speed for FOC */
        MID_MC_SetExternalPosEl(sMechMeasFcn->sStartup.f16PosMerged);

        /* switch to close loop  */
        if (!sMechMeasFcn->sStartup.bOpenLoop)
        {
            /* produce acceleration torque */
            sIDQReq.fltD = 0.0F;
            sIDQReq.fltQ = sMechMeasFcn->fltIqAccelerate;
            MID_MC_SetIDQReq(sIDQReq);

            /* reset loop counter */
            sMechMeasFcn->ui32LoopCounter = 0U;

            /* switch to acceleration state */
            sMechMeasFcn->eState = kMID_MechAccelerate;
        }

        break;

    case kMID_MechAccelerate:

        /* Integrate torque and speed. Count acceleration time */
        if(fltSpeedElEst > sMechMeasFcn->fltSpeedThrsInteg)
        {
            sMechMeasFcn->fltSpeedInteg  += MID_FAST_LOOP_TS * sMechMeasFcn->fltSpeedFilt;
            sMechMeasFcn->fltTorqueInteg += MID_FAST_LOOP_TS * sMechMeasFcn->fltTorqueFilt;

            /* Acceleration time counter */
            sMechMeasFcn->ui32AccelLoopCounter++;
        }

        /* When the high speed threshold is reached */
        if(fltSpeedElEst > sMechMeasFcn->fltSpeedThrsDecel)
        {
            /* produce no torque */
            sIDQReq.fltD = 0.0F;
            sIDQReq.fltQ = 0.0F;
            MID_MC_SetIDQReq(sIDQReq);

            /* reset loop counter */
            sMechMeasFcn->ui32LoopCounter = 0U;

            /* end measurement */
            sMechMeasFcn->eState = kMID_MechDecelerate;
        }

        /* Position and speed for FOC */
        MID_MC_SetExternalPosEl(MID_MC_GetEstimatedPosEl());

        break;

    case kMID_MechDecelerate:

        /* Deceleration time counter */
        sMechMeasFcn->ui32DecelLoopCounter++;

        /* Position and speed for FOC */
        MID_MC_SetExternalPosEl(MID_MC_GetEstimatedPosEl());

         /* check the timer */
        if(sMechMeasFcn->ui32DecelLoopCounter < (50U * MID_FAST_LOOP_FREQ / 1000U))
        {
            sMechMeasFcn->fltSpeedMax = sMechMeasFcn->fltSpeedFilt;
            break;
        }

        /* check whether the speed dropped by 63pct of maximum*/
        if(sMechMeasFcn->fltSpeedFilt < (sMechMeasFcn->fltSpeedMax * 0.3678794412F))
        {
            fltTautemp = ((float_t)((float_t)sMechMeasFcn->ui32DecelLoopCounter -
                                    (0.05F * (float_t)MID_FAST_LOOP_FREQ))) *
                                    MID_FAST_LOOP_TS;

            /* calculate moment of inertia  */
            fltJtemp = sMechMeasFcn->fltPp * sMechMeasFcn->fltTorqueInteg *
                fltTautemp /(fltTautemp * (sMechMeasFcn->fltSpeedThrsDecel -
                sMechMeasFcn->fltSpeedThrsAccel) + sMechMeasFcn->fltSpeedInteg);

            /* calculate inertia constant */
            fltInertiaConsttemp = sMechMeasFcn->fltIqAccelerate / (sMechMeasFcn->fltSpeedThrsDecel - sMechMeasFcn->fltSpeedThrsAccel) *
                ((float_t)sMechMeasFcn->ui32AccelLoopCounter * MID_FAST_LOOP_TS);

            /* Accumulate Tau, J and Jconst for average calculation */
            if(sMechMeasFcn->ui16MeasNr < MID_MECH_MEAS_NR)
            {
                sMechMeasFcn->fltTauMech      += fltTautemp;
                sMechMeasFcn->fltJ            += fltJtemp;
                sMechMeasFcn->fltInertiaConst += fltInertiaConsttemp;
            }

            /* Repeat the measurement unless all measurements done */
            if(--(sMechMeasFcn->ui16MeasNr) == 0U)
            {
                /* Calculate average values */
                sMechMeasFcn->fltTauMech      = sMechMeasFcn->fltTauMech * MID_MECH_MEAS_NR_DIV_COEFF_MULT;
                sMechMeasFcn->fltJ            = sMechMeasFcn->fltJ * MID_MECH_MEAS_NR_DIV_COEFF_MULT;
                sMechMeasFcn->fltInertiaConst = sMechMeasFcn->fltInertiaConst * MID_MECH_MEAS_NR_DIV_COEFF_MULT;
                sMechMeasFcn->fltB            = sMechMeasFcn->fltJ / sMechMeasFcn->fltTauMech;

                sMechMeasFcn->eState = kMID_MechEndOfMeas;
            }
            else
            {
                /* reset loop counter */
                sMechMeasFcn->ui32LoopCounter = 0U;

                /* Reset values */
                sMechMeasFcn->fltSpeedInteg         = 0.0F;
                sMechMeasFcn->fltTorqueInteg        = 0.0F;
                sMechMeasFcn->ui32AccelLoopCounter  = 0U;
                sMechMeasFcn->ui32DecelLoopCounter  = 0U;

                /* produce acceleration torque */
                sIDQReq.fltD = 0.0F;
                sIDQReq.fltQ = sMechMeasFcn->fltIqAccelerate;
                MID_MC_SetIDQReq(sIDQReq);

                /* switch to acceleration state */
                sMechMeasFcn->eState = kMID_MechAccelerate;
            }
        }

        break;

    default:
        /* end-of-measurement state */
        /* no currents required */
        sIDQReq.fltD = 0.0F;
        sIDQReq.fltQ = 0.0F;
        MID_MC_SetIDQReq(sIDQReq);

        /* When finished exit the function */
        sMechMeasFcn->bActive = FALSE;
        break;
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
static void MIDpenLoopStartUpA1(mid_ol_startup_t *psStartUp)
{
    /* Open loop startup speed ramp */
    psStartUp->fltSpeedRampOpenLoop = GFLIB_Ramp_FLT(psStartUp->fltSpeedReq,
                                                     &psStartUp->sSpeedRampOpenLoopParams);

    /* generation of open loop position from the required speed */
    psStartUp->f16PosGen = GFLIB_Integrator_F16(MLIB_ConvSc_F16ff(psStartUp->fltSpeedRampOpenLoop,psStartUp->fltSpeedMax),
                                                &psStartUp->sSpeedIntegrator);

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
    if (psStartUp->f16RatioMerging == FRAC16(1.0F))
    {
        psStartUp->bOpenLoop = FALSE;
    }
}

