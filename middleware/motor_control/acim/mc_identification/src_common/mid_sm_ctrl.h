/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MID_SM_CTRL_H_
#define _MID_SM_CTRL_H_

#include "mlib_types.h"
#include "mid_def.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* state machine control command flags */
/* maximum eight of _ACK/_DONE flag pairs  */
#define MID_SM_CTRL_NONE 0x00000000
#define MID_SM_CTRL_START_ACK 0x00000001
#define MID_SM_CTRL_START_DONE ((MID_SM_CTRL_START_ACK)*0x10000)
#define MID_SM_CTRL_STOP_ACK 0x00000002
#define MID_SM_CTRL_STOP_DONE ((MID_SM_CTRL_STOP_ACK)*0x10000)
#define MID_SM_CTRL_PWR_STG_CHARACT_ACK 0x00000004
#define MID_SM_CTRL_PWR_STG_CHARACT_DONE ((MID_SM_CTRL_PWR_STG_CHARACT_ACK)*0x10000)
#define MID_SM_CTRL_RS_ACK 0x00000008
#define MID_SM_CTRL_RS_DONE ((MID_SM_CTRL_RS_ACK)*0x10000)
#define MID_SM_CTRL_NOLOAD_ACK 0x00000010
#define MID_SM_CTRL_NOLOAD_DONE ((MID_SM_CTRL_NOLOAD_ACK)*0x10000)
#define MID_SM_CTRL_BLOCKED_ACK 0x00000020
#define MID_SM_CTRL_BLOCKED_DONE ((MID_SM_CTRL_BLOCKED_ACK)*0x10000)
#define MID_SM_CTRL_MECH_ACK 0x00000040
#define MID_SM_CTRL_MECH_DONE ((MID_SM_CTRL_MECH_ACK)*0x10000)

/* application state identification enum */
typedef enum
{
    kMID_Start         = 0,
    kMID_Stop          = 1,
    kMID_PwrStgCharact = 2,
    kMID_Rs            = 3,
    kMID_NoLoad        = 4,
    kMID_Blocked       = 5,
    kMID_Mech          = 6
} mid_sm_app_state_t;

typedef unsigned long mid_sm_app_ctrl;
typedef unsigned long mid_sm_app_fault;

/* pointer to function */
typedef void (*mid_pfcn_void_void)(void);

/* user state machine functions structure */
typedef struct
{
    mid_pfcn_void_void MID_Start;
    mid_pfcn_void_void MID_Stop;
    mid_pfcn_void_void MID_PwrStgCharact;
    mid_pfcn_void_void MID_Rs;
    mid_pfcn_void_void MID_NoLoad;
    mid_pfcn_void_void MID_Blocked;
    mid_pfcn_void_void MID_Mech;
} mid_sm_app_state_fcn_t;

/* user state-transition functions structure*/
typedef struct
{
    mid_pfcn_void_void MID_Start2PwrStgCharact;
    mid_pfcn_void_void MID_Start2Rs;
    mid_pfcn_void_void MID_PwrStgCharact2Stop;
    mid_pfcn_void_void MID_Rs2NoLoad;
    mid_pfcn_void_void MID_NoLoad2Blocked;
    mid_pfcn_void_void MID_Blocked2Mech;
    mid_pfcn_void_void MID_Mech2Stop;
    mid_pfcn_void_void MID_All2Stop;
} mid_sm_app_trans_fcn_t;

/* state machine control structure */
typedef struct
{
    mid_sm_app_state_fcn_t const *psState; /* state functions */
    mid_sm_app_trans_fcn_t const *psTrans; /* transition functions */
    mid_sm_app_ctrl uiCtrl;                /* control flags */
    mid_sm_app_state_t eState;             /* state */
} mid_sm_app_ctrl_t;

extern mid_struct_a1_t g_sMID;

/* pointer to function with a pointer to state machine control structure */
typedef void (*mid_pfcn_void_pms)(mid_sm_app_ctrl_t *sAppCtrl);

/* state machine function table */
extern const mid_pfcn_void_pms g_MID_SM_STATE_TABLE[8];

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief MID State machine function
 *
 * @param sAppCtrl Pointer to application state machine
 *
 * @return None
 */
static inline void MID_SM_StateMachine(mid_sm_app_ctrl_t *sAppCtrl)
{
    g_MID_SM_STATE_TABLE[sAppCtrl->eState](sAppCtrl);

    /* check fail state */
    if (g_sMID.ui16FaultMID != 0)
    {
        /* run transition function */
        sAppCtrl->psTrans->MID_All2Stop();

        if ((sAppCtrl->uiCtrl & MID_SM_CTRL_STOP_ACK) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            sAppCtrl->uiCtrl &= ~(MID_SM_CTRL_STOP_ACK);

            /* next go to STOP state */
            sAppCtrl->eState = kMID_Stop;
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif /* _MID_SM_CTRL_H_ */
