/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mech_measure.h"

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
void MID_getMech(mid_get_mech_t *sMechMeasFcn)
{
    float_t fltJtemp            = 0;
    float_t fltTautemp          = 0;
    float_t fltInertiaConsttemp = 0;

    /* Initialization */
    if (sMechMeasFcn->ui16Active == FALSE)
    {
        sMechMeasFcn->ui16Active           = TRUE;
        sMechMeasFcn->ui16MeasNr           = MID_MECH_MEAS_NR;
        sMechMeasFcn->ui32LoopCounter      = 0;
        sMechMeasFcn->ui32AccelLoopCounter = 0;
        sMechMeasFcn->ui32DecelLoopCounter = 0;
        sMechMeasFcn->fltSpeedFilt         = 0.0F;
        sMechMeasFcn->fltTorqueFilt        = 0.0F;
        sMechMeasFcn->fltLPFCoeff          = 0.1180886217F; /* 200 Hz filter */
        sMechMeasFcn->eState               = kMID_MechInit;

        *sMechMeasFcn->pfltIdReq = 0.0F;
        *sMechMeasFcn->pfltIqReq = 0.0F;

        sMechMeasFcn->fltSpeedInteg  = 0.0F;
        sMechMeasFcn->fltTorqueInteg = 0.0F;

        /* Open loop start up */
        sMechMeasFcn->sStartup.fltSpeedMax              = M1_N_MAX;
        sMechMeasFcn->sStartup.sSpeedIntegrator.a32Gain = M1_SCALAR_INTEG_GAIN;
        GFLIB_IntegratorInit_F16(0, &sMechMeasFcn->sStartup.sSpeedIntegrator);
        sMechMeasFcn->sStartup.f16CoeffMerging   = M1_MERG_COEFF;
        sMechMeasFcn->sStartup.fltSpeedCatchUp   = sMechMeasFcn->fltSpeedThrsAccel;
        sMechMeasFcn->sStartup.fltSpeedReq       = sMechMeasFcn->fltSpeedThrsAccel;
        sMechMeasFcn->sStartup.fltCurrentStartup = sMechMeasFcn->fltIqAccelerate;
        sMechMeasFcn->sStartup.bOpenLoop         = TRUE;
        GFLIB_RampInit_FLT(0.0F, &sMechMeasFcn->sStartup.sSpeedRampOpenLoopParams);
        sMechMeasFcn->sStartup.f16PosGen            = 0.0F;
        sMechMeasFcn->sStartup.f16PosMerged         = 0.0F;
        sMechMeasFcn->sStartup.f16RatioMerging      = 0.0F;
        sMechMeasFcn->sStartup.fltSpeedRampOpenLoop = 0.0F;
    }

    /* increment loop counter and check the measurement timeout */
    if (sMechMeasFcn->ui32LoopCounter++ > sMechMeasFcn->ui32TimeMeasMax)
    {
        /* assign fault code */
        g_sMID.ui16FaultMID = MID_FAULT_MECH_TIMEOUT;
        /* end measurement */
        sMechMeasFcn->eState = kMID_MechEndOfMeas;
    }

    /* calculate actual torque */
    sMechMeasFcn->fltTorqueEst = 1.5F * sMechMeasFcn->fltPp * sMechMeasFcn->fltKt * (*sMechMeasFcn->pfltIqfbck);

    /* filter speed */
    sMechMeasFcn->fltSpeedFilt +=
        sMechMeasFcn->fltLPFCoeff * ((*sMechMeasFcn->pfltSpeedEst) - sMechMeasFcn->fltSpeedFilt);

    /* filter torque */
    sMechMeasFcn->fltTorqueFilt +=
        sMechMeasFcn->fltLPFCoeff * (sMechMeasFcn->fltTorqueEst - sMechMeasFcn->fltTorqueFilt);

    /* measurement state machine */
    switch (sMechMeasFcn->eState)
    {
        case kMID_MechInit:

            /* Apply the startup current */
            *sMechMeasFcn->pfltIqReq = sMechMeasFcn->fltIqStartup;

            /* reset loop counter */
            sMechMeasFcn->ui32LoopCounter = 0;

            /* Go to accelerate state */
            sMechMeasFcn->eState = kMID_MechStartUp;
            break;

        case kMID_MechStartUp:

            /* pass actual estimation position to OL startup structure */
            sMechMeasFcn->sStartup.f16PosEst = *sMechMeasFcn->pf16PosElEst;

            /*open loop startup */
            MIDpenLoopStartUpA1(&sMechMeasFcn->sStartup);

            /* Position and speed for FOC */
            *sMechMeasFcn->pf16PosElExt = sMechMeasFcn->sStartup.f16PosMerged;

            /* switch to close loop  */
            if (!sMechMeasFcn->sStartup.bOpenLoop)
            {
                /* produce acceleration torque */
                *sMechMeasFcn->pfltIqReq = sMechMeasFcn->fltIqAccelerate;

                /* reset loop counter */
                sMechMeasFcn->ui32LoopCounter = 0;

                /* switch to acceleration state */
                sMechMeasFcn->eState = kMID_MechAccelerate;
            }

            break;

        case kMID_MechAccelerate:

            /* Integrate torque and speed. Count acceleration time */
            if (*sMechMeasFcn->pfltSpeedEst > sMechMeasFcn->fltSpeedThrsInteg)
            {
                sMechMeasFcn->fltSpeedInteg += M1_FAST_LOOP_TS * sMechMeasFcn->fltSpeedFilt;
                sMechMeasFcn->fltTorqueInteg += M1_FAST_LOOP_TS * sMechMeasFcn->fltTorqueFilt;

                /* Acceleration time counter */
                sMechMeasFcn->ui32AccelLoopCounter++;
            }

            /* When the high speed threshold is reached */
            if (*sMechMeasFcn->pfltSpeedEst > sMechMeasFcn->fltSpeedThrsDecel)
            {
                /* produce no torque */
                *sMechMeasFcn->pfltIdReq = 0.0f;
                *sMechMeasFcn->pfltIqReq = 0.0f;

                /* reset loop counter */
                sMechMeasFcn->ui32LoopCounter = 0;

                /* end measurement */
                sMechMeasFcn->eState = kMID_MechDecelerate;
            }

            break;

        case kMID_MechDecelerate:

            /* Deceleration time counter */
            sMechMeasFcn->ui32DecelLoopCounter++;

            /* check the timer */
            if (sMechMeasFcn->ui32DecelLoopCounter < (0.05F * M1_TIME_ONESEC_COUNT))
            {
                sMechMeasFcn->fltSpeedMax = sMechMeasFcn->fltSpeedFilt;
                break;
            }

            /* check whether the speed dropped to 63pct of maximum*/
            if (sMechMeasFcn->fltSpeedFilt < (sMechMeasFcn->fltSpeedMax * 0.3678794412F))
            {
                fltTautemp =
                    ((float_t)(sMechMeasFcn->ui32DecelLoopCounter - (0.05F * M1_TIME_ONESEC_COUNT))) * M1_FAST_LOOP_TS;

                /* calculate moment of inertia  */
                fltJtemp = sMechMeasFcn->fltPp * sMechMeasFcn->fltTorqueInteg * fltTautemp /
                           (fltTautemp * (sMechMeasFcn->fltSpeedThrsDecel - sMechMeasFcn->fltSpeedThrsAccel) +
                            sMechMeasFcn->fltSpeedInteg);

                /* calculate inertia constant */
                fltInertiaConsttemp = sMechMeasFcn->fltIqAccelerate /
                                      (sMechMeasFcn->fltSpeedThrsDecel - sMechMeasFcn->fltSpeedThrsAccel) *
                                      ((float_t)sMechMeasFcn->ui32AccelLoopCounter / (float_t)M1_TIME_ONESEC_COUNT);

                /* Accumulate Tau, J and Jconst for average calculation */
                if (sMechMeasFcn->ui16MeasNr < MID_MECH_MEAS_NR)
                {
                    sMechMeasFcn->fltTauMech += fltTautemp;
                    sMechMeasFcn->fltJ += fltJtemp;
                    sMechMeasFcn->fltInertiaConst += fltInertiaConsttemp;
                }

                /* Repeat the measurement unless all measurements done */
                if (--(sMechMeasFcn->ui16MeasNr) == 0)
                {
                    /* Calculate average values */
                    sMechMeasFcn->fltTauMech      = sMechMeasFcn->fltTauMech / ((float_t)(MID_MECH_MEAS_NR - 1));
                    sMechMeasFcn->fltJ            = sMechMeasFcn->fltJ / ((float_t)(MID_MECH_MEAS_NR - 1));
                    sMechMeasFcn->fltInertiaConst = sMechMeasFcn->fltInertiaConst / ((float_t)(MID_MECH_MEAS_NR - 1));
                    sMechMeasFcn->fltB            = sMechMeasFcn->fltJ / sMechMeasFcn->fltTauMech;

                    sMechMeasFcn->eState = kMID_MechEndOfMeas;
                }
                else
                {
                    /* reset loop counter */
                    sMechMeasFcn->ui32LoopCounter = 0;

                    /* Reset values */
                    sMechMeasFcn->fltSpeedInteg        = 0.0F;
                    sMechMeasFcn->fltTorqueInteg       = 0.0F;
                    sMechMeasFcn->ui32AccelLoopCounter = 0;
                    sMechMeasFcn->ui32DecelLoopCounter = 0;

                    /* produce acceleration torque */
                    *sMechMeasFcn->pfltIqReq = sMechMeasFcn->fltIqAccelerate;

                    /* switch to acceleration state */
                    sMechMeasFcn->eState = kMID_MechAccelerate;
                }
            }
            break;

        default:
            /* end-of-measurement state */
            /* no currents required */
            *g_sMID.sIO.pfltIdReq = 0.0F;
            *g_sMID.sIO.pfltIqReq = 0.0F;

            /* When finished exit the function */
            sMechMeasFcn->ui16Active = FALSE;
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
        psStartUp->bOpenLoop = FALSE;
}
