/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mid_sm_ctrl.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void MID_SM_StateStart(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StatePwrStgCharact(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StateRs(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StateLd(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StateLq(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StatePp(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StateKe(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StateMech(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StateHall(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StateStop(mid_sm_app_ctrl_t *psAppCtrl);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief State machine functions field */
const mid_pfcn_void_pms g_MID_SM_STATE_TABLE[10] = {
    MID_SM_StateStart, MID_SM_StatePwrStgCharact, MID_SM_StateRs,   MID_SM_StateLd,  MID_SM_StateLq, MID_SM_StatePp,
    MID_SM_StateKe,    MID_SM_StateMech,          MID_SM_StateHall, MID_SM_StateStop};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Start state called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_SM_StateStart(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user Start function */
    psAppCtrl->psState->MID_Start();

    /* if START_DONE flag is set */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_START_DONE) > 0)
    {
        /* clear state's _ACK & _DONE SM control flags */
        psAppCtrl->uiCtrl &= ~(MID_SM_CTRL_START_DONE | MID_SM_CTRL_START_ACK);

        /* if kMID_Calibration go to PWR_STG_CHARACT state */
        if (g_sMID.eMeasurementType == kMID_Calibration)
        {
            psAppCtrl->psTrans->MID_Start2PwrStgCharact();
            psAppCtrl->eState = kMID_PwrStgCharact;
        }

        /* if kMID_PolePairs go to PP state */
        if (g_sMID.eMeasurementType == kMID_PolePairs)
        {
            psAppCtrl->psTrans->MID_Start2Pp();
            psAppCtrl->eState = kMID_Pp;
        }

        /* if kMID_ElectricalParams go to RS state */
        if (g_sMID.eMeasurementType == kMID_ElectricalParams)
        {
            psAppCtrl->psTrans->MID_Start2Rs();
            psAppCtrl->eState = kMID_Rs;
        }

        /* if kMID_MechanicalParams go to MECH state */
        if (g_sMID.eMeasurementType == kMID_MechanicalParams)
        {
            psAppCtrl->psTrans->MID_Start2Mech();
            psAppCtrl->eState = kMID_Mech;
        }

        /* if kMID_HallSensors go to HALL state */
        if (g_sMID.eMeasurementType == kMID_HallSensors)
        {
            psAppCtrl->psTrans->MID_Start2Hall();
            psAppCtrl->eState = kMID_Hall;
        }
    }
}

/*!
 * @brief PwrStgChar state called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_SM_StatePwrStgCharact(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user PwrStgCharact function */
    psAppCtrl->psState->MID_PwrStgCharact();

    /* if PWR_STG_CHARACT_DONE flag is set */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_PWR_STG_CHARACT_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl->psTrans->MID_All2Stop();

        if ((psAppCtrl->uiCtrl & MID_SM_CTRL_PWR_STG_CHARACT_DONE) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl->uiCtrl &= ~(MID_SM_CTRL_PWR_STG_CHARACT_DONE | MID_SM_CTRL_PWR_STG_CHARACT_ACK);

            /* next go to Stop state */
            psAppCtrl->eState = kMID_Stop;
        }
    }
}

/*!
 * @brief Rs state called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_SM_StateRs(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user Rs function */
    psAppCtrl->psState->MID_Rs();

    /* if RS_DONE flag is set */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_RS_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl->psTrans->MID_Rs2Ld();

        if ((psAppCtrl->uiCtrl & MID_SM_CTRL_RS_DONE) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl->uiCtrl &= ~(MID_SM_CTRL_RS_DONE | MID_SM_CTRL_RS_ACK);

            /* next go to Ls state */
            psAppCtrl->eState = kMID_Ld;
        }
    }
}

/*!
 * @brief Ld state called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_SM_StateLd(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user Ls function */
    psAppCtrl->psState->MID_Ld();

    /* if LD_DONE flag is set */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_LD_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl->psTrans->MID_Ld2Lq();

        if ((psAppCtrl->uiCtrl & MID_SM_CTRL_LD_DONE) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl->uiCtrl &= ~(MID_SM_CTRL_LD_DONE | MID_SM_CTRL_LD_ACK);

            /* Stop state */
            psAppCtrl->eState = kMID_Lq;
        }
    }
}

/*!
 * @brief Lq state called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_SM_StateLq(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user Ls function */
    psAppCtrl->psState->MID_Lq();

    /* if LQ_DONE flag is set */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_LQ_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl->psTrans->MID_Lq2Ke();

        if ((psAppCtrl->uiCtrl & MID_SM_CTRL_LQ_DONE) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl->uiCtrl &= ~(MID_SM_CTRL_LQ_DONE | MID_SM_CTRL_LQ_ACK);

            /* Stop state */
            psAppCtrl->eState = kMID_Ke;
        }
    }
}

/*!
 * @brief Pp state called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_SM_StatePp(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user Ls function */
    psAppCtrl->psState->MID_Pp();

    /* if PP_DONE flag is set */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_PP_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl->psTrans->MID_All2Stop();

        if ((psAppCtrl->uiCtrl & MID_SM_CTRL_PP_DONE) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl->uiCtrl &= ~(MID_SM_CTRL_PP_DONE | MID_SM_CTRL_PP_ACK);

            /* Stop state */
            psAppCtrl->eState = kMID_Stop;
        }
    }
}

/*!
 * @brief Ke state called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_SM_StateKe(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user Ls function */
    psAppCtrl->psState->MID_Ke();

    /* if KE_DONE flag is set */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_KE_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl->psTrans->MID_All2Stop();

        if ((psAppCtrl->uiCtrl & MID_SM_CTRL_KE_DONE) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl->uiCtrl &= ~(MID_SM_CTRL_KE_DONE | MID_SM_CTRL_KE_ACK);

            /* Stop state */
            psAppCtrl->eState = kMID_Stop;
        }
    }
}

/*!
 * @brief Mech state called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_SM_StateMech(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user Ls function */
    psAppCtrl->psState->MID_Mech();

    /* if MECH_DONE flag is set */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_MECH_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl->psTrans->MID_All2Stop();

        if ((psAppCtrl->uiCtrl & MID_SM_CTRL_MECH_DONE) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl->uiCtrl &= ~(MID_SM_CTRL_MECH_DONE | MID_SM_CTRL_MECH_ACK);

            /* Stop state */
            psAppCtrl->eState = kMID_Stop;
        }
    }
}

/*!
 * @brief Hall state called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_SM_StateHall(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user Ls function */
    psAppCtrl->psState->MID_Hall();

    /* if HALL_DONE flag is set */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_HALL_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl->psTrans->MID_All2Stop();

        if ((psAppCtrl->uiCtrl & MID_SM_CTRL_HALL_DONE) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl->uiCtrl &= ~(MID_SM_CTRL_HALL_DONE | MID_SM_CTRL_HALL_ACK);

            /* Stop state */
            psAppCtrl->eState = kMID_Stop;
        }
    }
}

/*!
 * @brief Stop state called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_SM_StateStop(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user Stop function */
    psAppCtrl->psState->MID_Stop();
}
