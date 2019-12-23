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
#include "state_machine.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void SM_StateFaultFast(sm_app_ctrl_t *psAppCtrl);
static void SM_StateInitFast(sm_app_ctrl_t *psAppCtrl);
static void SM_StateStopFast(sm_app_ctrl_t *psAppCtrl);
static void SM_StateRunFast(sm_app_ctrl_t *psAppCtrl);

static void SM_StateFaultSlow(sm_app_ctrl_t *psAppCtrl);
static void SM_StateInitSlow(sm_app_ctrl_t *psAppCtrl);
static void SM_StateStopSlow(sm_app_ctrl_t *psAppCtrl);
static void SM_StateRunSlow(sm_app_ctrl_t *psAppCtrl);

/*******************************************************************************
* Variables
******************************************************************************/

/*! @brief State machine function field - fast */
const pfcn_void_psm g_SM_STATE_TABLE_FAST[4] = {SM_StateFaultFast, SM_StateInitFast, SM_StateStopFast, SM_StateRunFast};

/*! @brief State machine function field - slow */
const pfcn_void_psm g_SM_STATE_TABLE_SLOW[4] = {SM_StateFaultSlow, SM_StateInitSlow, SM_StateStopSlow, SM_StateRunSlow};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief State machine Fault state in fast
 *
 * @param psAppCtrl     The pointer of the State machine application
 *
 * @return None
 */
static void SM_StateFaultFast(sm_app_ctrl_t *psAppCtrl)
{
    /* User Fault function */
    psAppCtrl->psStateFast->Fault();

    /* if clear fault command flag */
    if ((psAppCtrl->uiCtrl & SM_CTRL_FAULT_CLEAR) > 0)
    {
        /* Clear INIT_DONE, FAULT, FAULT_CLEAR flags */
        psAppCtrl->uiCtrl &= ~(SM_CTRL_INIT_DONE | SM_CTRL_FAULT | SM_CTRL_FAULT_CLEAR);

        /* User Fault to Init transition function */
        psAppCtrl->psTrans->FaultStop();

        /* Init state */
        psAppCtrl->eState = kSM_AppStop;
    }
}

/*!
 * @brief State machine Init state in fast
 *
 * @param psAppCtrl     The pointer of the State machine application
 *
 * @return None
 */
static void SM_StateInitFast(sm_app_ctrl_t *psAppCtrl)
{
    /* User Init function */
    psAppCtrl->psStateFast->Init();

    /* if fault flag */
    if ((psAppCtrl->uiCtrl & SM_CTRL_FAULT) > 0)
    {
        /* User Init to Fault transition function */
        psAppCtrl->psTrans->InitFault();

        /* Fault state */
        psAppCtrl->eState = kSM_AppFault;
    }
    /* if INIT_DONE flag */
    else if ((psAppCtrl->uiCtrl & SM_CTRL_INIT_DONE) > 0)
    {
        /* Clear INIT_DONE, START_STOP, OM_CHANGE, STOP_ACK, RUN_ACK flags */
        psAppCtrl->uiCtrl &= ~(SM_CTRL_INIT_DONE | SM_CTRL_STOP | SM_CTRL_START | SM_CTRL_STOP_ACK | SM_CTRL_RUN_ACK);

        /* User Init to Stop transition function */
        psAppCtrl->psTrans->InitStop();

        /* Stop state */
        psAppCtrl->eState = kSM_AppStop;
    }
}

/*!
 * @brief State machine Stop state in fast
 *
 * @param psAppCtrl     The pointer of the State machine application
 *
 * @return None
 */
static void SM_StateStopFast(sm_app_ctrl_t *psAppCtrl)
{
    /* User Stop function */
    psAppCtrl->psStateFast->Stop();

    /* if fault */
    if ((psAppCtrl->uiCtrl & SM_CTRL_FAULT) > 0)
    {
        /* User Stop to Fault transition function */
        psAppCtrl->psTrans->StopFault();

        /* Fault state */
        psAppCtrl->eState = kSM_AppFault;
    }
    else if ((psAppCtrl->uiCtrl & SM_CTRL_START) > 0)
    {
        /* User Stop to Run transition function, user must set up the SM_CTRL_RUN_ACK
        flag to allow the RUN state */
        psAppCtrl->psTrans->StopRun();

        /* Clears the START command */
        if ((psAppCtrl->uiCtrl & SM_CTRL_RUN_ACK) > 0)
        {
            /* Clears the RUN_ACK flag */
            psAppCtrl->uiCtrl &= ~(SM_CTRL_RUN_ACK | SM_CTRL_START);

            /* Run state */
            psAppCtrl->eState = kSM_AppRun;
        }
    }
}

/*!
 * @brief State machine Run state in fast
 *
 * @param psAppCtrl     The pointer of the State machine application
 *
 * @return None
 */
static void SM_StateRunFast(sm_app_ctrl_t *psAppCtrl)
{
    /* User Run function */
    psAppCtrl->psStateFast->Run();

    if ((psAppCtrl->uiCtrl & SM_CTRL_FAULT) > 0)
    {
        /* User Run to Fault transition function */
        psAppCtrl->psTrans->RunFault();

        /* Fault state */
        psAppCtrl->eState = kSM_AppFault;
    }
    else if ((psAppCtrl->uiCtrl & SM_CTRL_STOP) > 0)
    {
        /* User Run to Stop transition function, user must set up the SM_CTRL_STOP_ACK
        flag to allow the STOP state */
        psAppCtrl->psTrans->RunStop();

        if ((psAppCtrl->uiCtrl & SM_CTRL_STOP_ACK) > 0)
        {
            /* Clears the STOP_ACK flag */
            psAppCtrl->uiCtrl &= ~(SM_CTRL_STOP_ACK | SM_CTRL_STOP);

            /* Run state */
            psAppCtrl->eState = kSM_AppStop;
        }
    }
}

/*!
 * @brief State machine Fault state in slow
 *
 * @param psAppCtrl     The pointer of the State machine application
 *
 * @return None
 */
static void SM_StateFaultSlow(sm_app_ctrl_t *psAppCtrl)
{
    psAppCtrl->psStateSlow->Fault();
}

/*!
 * @brief State machine Init state in slow
 *
 * @param psAppCtrl     The pointer of the State machine application
 *
 * @return None
 */
static void SM_StateInitSlow(sm_app_ctrl_t *psAppCtrl)
{
    psAppCtrl->psStateSlow->Init();
}

/*!
 * @brief State machine Stop state in slow
 *
 * @param psAppCtrl     The pointer of the State machine application
 *
 * @return None
 */
static void SM_StateStopSlow(sm_app_ctrl_t *psAppCtrl)
{
    psAppCtrl->psStateSlow->Stop();
}

/*!
 * @brief State machine Run state in slow
 *
 * @param psAppCtrl     The pointer of the State machine application
 *
 * @return None
 */
static void SM_StateRunSlow(sm_app_ctrl_t *psAppCtrl)
{
    psAppCtrl->psStateSlow->Run();
}

