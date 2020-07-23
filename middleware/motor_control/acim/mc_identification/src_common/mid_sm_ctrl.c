/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mid_sm_ctrl.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* application state machine functions */
static void MID_SM_StateStart(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StateStop(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StatePwrStgCharact(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StateRs(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StateNoLoad(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StateBlocked(mid_sm_app_ctrl_t *psAppCtrl);
static void MID_SM_StateMech(mid_sm_app_ctrl_t *psAppCtrl);

/*******************************************************************************
 * Variables
 ******************************************************************************/
mid_struct_a1_t g_sMID;

/* state machine functions field */
const mid_pfcn_void_pms g_MID_SM_STATE_TABLE[8] = {MID_SM_StateStart, MID_SM_StateStop,   MID_SM_StatePwrStgCharact,
                                                   MID_SM_StateRs,    MID_SM_StateNoLoad, MID_SM_StateBlocked,
                                                   MID_SM_StateMech};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief start state
 *
 * @param *psAppCtrl - MID application control structure
 *
 * @return None
 */
static void MID_SM_StateStart(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user START function */
    psAppCtrl->psState->MID_Start();

    if (g_sMID.ui16FaultMID)
        return;

    /* if START_DONE flag is set */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_START_DONE) > 0)
    {
        /* clear state's _ACK & _DONE SM control flags */
        psAppCtrl->uiCtrl &= ~MID_SM_CTRL_START_DONE;

        /* if ui16Calibration go to PWR_STG_CHARACT state */
        if (g_sMID.bCalib)
        {
            psAppCtrl->psTrans->MID_Start2PwrStgCharact();
            psAppCtrl->eState = kMID_PwrStgCharact;
        }
        /* if ui16ElectricalParameters go to RS state */
        else
        {
            psAppCtrl->psTrans->MID_Start2Rs();
            psAppCtrl->eState = kMID_Rs;
        }
    }
}

/*!
 * @brief stator resistance measurement
 *
 * @param *psAppCtrl - MID application control structure
 *
 * @return None
 */
static void MID_SM_StateRs(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user Rs function */
    psAppCtrl->psState->MID_Rs();

    if (g_sMID.ui16FaultMID)
        return;

    /* if RS_DONE flag is set */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_RS_DONE) > 0)
    {
        /* run transition function */
        psAppCtrl->psTrans->MID_Rs2NoLoad();

        if ((psAppCtrl->uiCtrl & MID_SM_CTRL_NOLOAD_ACK) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl->uiCtrl &= ~(MID_SM_CTRL_RS_DONE | MID_SM_CTRL_NOLOAD_ACK);

            /* next go to Ls state */
            psAppCtrl->eState = kMID_NoLoad;
        }
    }
}

/*!
 * @brief no-load measurement
 *
 * @param *psAppCtrl - MID application control structure
 *
 * @return None
 */
static void MID_SM_StateNoLoad(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user no-load measurement function */
    psAppCtrl->psState->MID_NoLoad();

    if (g_sMID.ui16FaultMID)
        return;

    /* if NOLOAD_DONE flag is set */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_NOLOAD_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl->psTrans->MID_NoLoad2Blocked();

        if ((psAppCtrl->uiCtrl & MID_SM_CTRL_BLOCKED_ACK) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl->uiCtrl &= ~(MID_SM_CTRL_NOLOAD_DONE | MID_SM_CTRL_BLOCKED_ACK);

            /* next go to NoLoad state */
            psAppCtrl->eState = kMID_Blocked;
        }
    }
}

/*!
 * @brief blocked rotor measurement
 *
 * @param *psAppCtrl - MID application control structure
 *
 * @return None
 */
static void MID_SM_StateBlocked(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user blocked-rotor measurement function */
    psAppCtrl->psState->MID_Blocked();

    if (g_sMID.ui16FaultMID)
        return;

    /* if BLOCKED_DONE flag is set */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_BLOCKED_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl->psTrans->MID_Blocked2Mech();

        if ((psAppCtrl->uiCtrl & MID_SM_CTRL_MECH_ACK) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl->uiCtrl &= ~(MID_SM_CTRL_BLOCKED_DONE | MID_SM_CTRL_MECH_ACK);

            /* next go to blocked rotor measurement state */
            psAppCtrl->eState = kMID_Mech;
        }
    }
}

/*!
 * @brief mechanical parameters measurement
 *
 * @param *psAppCtrl - MID application control structure
 *
 * @return None
 */
static void MID_SM_StateMech(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user MECH function */
    psAppCtrl->psState->MID_Mech();

    if (g_sMID.ui16FaultMID)
        return;

    /* if MECH_DONE flag is set or STOP flag received */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_MECH_DONE) > 0)
    {
        /* run transition function */
        psAppCtrl->psTrans->MID_Mech2Stop();

        if ((psAppCtrl->uiCtrl & MID_SM_CTRL_STOP_ACK) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl->uiCtrl &= ~(MID_SM_CTRL_MECH_DONE | MID_SM_CTRL_STOP_ACK);

            /* next go to STOP state */
            psAppCtrl->eState = kMID_Stop;
        }
    }
}

/*!
 * @brief power-stage characterisation
 *
 * @param *psAppCtrl - MID application control structure
 *
 * @return None
 */
static void MID_SM_StatePwrStgCharact(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user PwrStgCharact function */
    psAppCtrl->psState->MID_PwrStgCharact();

    if (g_sMID.ui16FaultMID)
        return;

    /* if PWR_STG_CHARACT_DONE flag is set */
    if ((psAppCtrl->uiCtrl & MID_SM_CTRL_PWR_STG_CHARACT_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl->psTrans->MID_PwrStgCharact2Stop();

        if ((psAppCtrl->uiCtrl & MID_SM_CTRL_STOP_ACK) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl->uiCtrl &= ~(MID_SM_CTRL_PWR_STG_CHARACT_DONE | MID_SM_CTRL_STOP_ACK);

            /* next go to STOP state */
            psAppCtrl->eState = kMID_Stop;
        }
    }
}

/*!
 * @brief stop state
 *
 * @param *psAppCtrl - MID application control structure
 *
 * @return None
 */
static void MID_SM_StateStop(mid_sm_app_ctrl_t *psAppCtrl)
{
    /* call user STOP function */
    psAppCtrl->psState->MID_Stop();
}
