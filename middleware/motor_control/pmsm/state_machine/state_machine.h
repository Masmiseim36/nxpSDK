/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "stdint.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* State machine control command flags */
#define SM_CTRL_NONE 0x0
#define SM_CTRL_FAULT 0x1
#define SM_CTRL_FAULT_CLEAR 0x2
#define SM_CTRL_INIT_DONE 0x4
#define SM_CTRL_STOP 0x8
#define SM_CTRL_START 0x10
#define SM_CTRL_STOP_ACK 0x20
#define SM_CTRL_RUN_ACK 0x40

typedef uint16_t sm_app_ctrl;
typedef uint32_t sm_app_fault;

/*! @brief Pointer to function */
typedef void (*pfcn_void_void)(void);

/*! @brief Application state identification enum */
typedef enum _sm_app_state
{
    kSM_AppFault = 0,
    kSM_AppInit = 1,
    kSM_AppStop = 2,
    kSM_AppRun = 3,
} sm_app_state_t;

/*! @brief User state machine functions structure */
typedef struct _sm_app_state_fcn
{
    pfcn_void_void Fault;
    pfcn_void_void Init;
    pfcn_void_void Stop;
    pfcn_void_void Run;
} sm_app_state_fcn_t;

/*! @brief User state-transition functions structure */
typedef struct _sm_app_trans_fcn
{
    pfcn_void_void FaultStop;
    pfcn_void_void InitFault;
    pfcn_void_void InitStop;
    pfcn_void_void StopFault;
    pfcn_void_void StopRun;
    pfcn_void_void RunFault;
    pfcn_void_void RunStop;
} sm_app_trans_fcn_t;

/*! @brief State machine control structure */
typedef struct _sm_app_ctrl
{
    sm_app_state_fcn_t const *psStateFast; /* State functions */
    sm_app_state_fcn_t const *psStateSlow; /* State functions slow*/
    sm_app_trans_fcn_t const *psTrans;     /* Transition functions */
    sm_app_ctrl uiCtrl;                    /* Control flags */
    sm_app_state_t eState;                 /* State */
} sm_app_ctrl_t;

/*! @brief Pointer to function with a pointer to state machine control structure */
typedef void (*pfcn_void_psm)(sm_app_ctrl_t *sAppCtrl);

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*! @brief State machine fast function prototype */
static inline void SM_StateMachineFast(sm_app_ctrl_t *sAppCtrl);
/*! @brief State machine slow function prototype */
static inline void SM_StateMachineSlow(sm_app_ctrl_t *sAppCtrl);

/*! @brief State machine fast function table */
extern const pfcn_void_psm g_SM_STATE_TABLE_FAST[4];
/*! @brief State machine slow function table */
extern const pfcn_void_psm g_SM_STATE_TABLE_SLOW[4];

/*!
 * @brief State Machine fast application state machine function
 *
 * @param sAppCtrl     Pointer to application state machine
 *
 * @return None
 */
static inline void SM_StateMachineFast(sm_app_ctrl_t *sAppCtrl)
{
    g_SM_STATE_TABLE_FAST[sAppCtrl->eState](sAppCtrl);
}

/*!
 * @brief State Machine slow application state machine function
 *
 * @param sAppCtrl     Pointer to application state machine
 *
 * @return None
 */
static inline void SM_StateMachineSlow(sm_app_ctrl_t *sAppCtrl)
{
    g_SM_STATE_TABLE_SLOW[sAppCtrl->eState](sAppCtrl);
}

#ifdef __cplusplus
}
#endif

#endif //_STATE_MACHINE_H_

