/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
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
#define MID_SM_CTRL_NONE 0x00000000
#define MID_SM_CTRL_START_ACK 0x00000002
#define MID_SM_CTRL_START_DONE ((MID_SM_CTRL_START_ACK) << 16)
#define MID_SM_CTRL_PWR_STG_CHARACT_ACK 0x00000004
#define MID_SM_CTRL_PWR_STG_CHARACT_DONE ((MID_SM_CTRL_PWR_STG_CHARACT_ACK) << 16)
#define MID_SM_CTRL_RS_ACK 0x00000008
#define MID_SM_CTRL_RS_DONE ((MID_SM_CTRL_RS_ACK) << 16)
#define MID_SM_CTRL_LD_ACK 0x00000010
#define MID_SM_CTRL_LD_DONE ((MID_SM_CTRL_LD_ACK) << 16)
#define MID_SM_CTRL_LQ_ACK 0x00000020
#define MID_SM_CTRL_LQ_DONE ((MID_SM_CTRL_LQ_ACK) << 16)
#define MID_SM_CTRL_PP_ACK 0x00000040
#define MID_SM_CTRL_PP_DONE ((MID_SM_CTRL_PP_ACK) << 16)
#define MID_SM_CTRL_KE_ACK 0x00000080
#define MID_SM_CTRL_KE_DONE ((MID_SM_CTRL_KE_ACK) << 16)
#define MID_SM_CTRL_MECH_ACK 0x00000100
#define MID_SM_CTRL_MECH_DONE ((MID_SM_CTRL_MECH_ACK) << 16)
#define MID_SM_CTRL_HALL_ACK 0x00000200
#define MID_SM_CTRL_HALL_DONE ((MID_SM_CTRL_HALL_ACK) << 16)
#define MID_SM_CTRL_STOP_ACK 0x00000400
#define MID_SM_CTRL_STOP_DONE ((MID_SM_CTRL_STOP_ACK) << 16)

/*! @brief States of machine enumeration */
typedef enum
{
    kMID_Start         = 0,
    kMID_PwrStgCharact = 1,
    kMID_Rs            = 2,
    kMID_Ld            = 3,
    kMID_Lq            = 4,
    kMID_Pp            = 5,
    kMID_Ke            = 6,
    kMID_Mech          = 7,
    kMID_Hall          = 8,
    kMID_Stop          = 9,
} mid_sm_app_state_t;

typedef unsigned long mid_sm_app_ctrl;
typedef unsigned long mid_sm_app_fault;

/*! @brief Pointer to function */
typedef void (*mid_pfcn_void_void)(void);

/*! @brief User state machine functions structure */
typedef struct
{
    mid_pfcn_void_void MID_Start;
    mid_pfcn_void_void MID_PwrStgCharact;
    mid_pfcn_void_void MID_Rs;
    mid_pfcn_void_void MID_Ld;
    mid_pfcn_void_void MID_Lq;
    mid_pfcn_void_void MID_Pp;
    mid_pfcn_void_void MID_Ke;
    mid_pfcn_void_void MID_Mech;
    mid_pfcn_void_void MID_Hall;
    mid_pfcn_void_void MID_Stop;
} mid_sm_app_state_fcn_t;

/*! @brief User state-transition functions structure */
typedef struct
{
    mid_pfcn_void_void MID_Start2PwrStgCharact;
    mid_pfcn_void_void MID_Start2Rs;
    mid_pfcn_void_void MID_Start2Pp;
    mid_pfcn_void_void MID_Start2Mech;
    mid_pfcn_void_void MID_Start2Hall;
    mid_pfcn_void_void MID_PwrStgCharact2Stop;
    mid_pfcn_void_void MID_Rs2Ld;
    mid_pfcn_void_void MID_Ld2Lq;
    mid_pfcn_void_void MID_Lq2Ke;
    mid_pfcn_void_void MID_Ke2Stop;
    mid_pfcn_void_void MID_Pp2Stop;
    mid_pfcn_void_void MID_Mech2Stop;
    mid_pfcn_void_void MID_All2Stop;
} mid_sm_app_trans_fcn_t;

/*! @brief State machine control structure */
typedef struct
{
    mid_sm_app_state_fcn_t const *psState; /* State functions */
    mid_sm_app_trans_fcn_t const *psTrans; /* Transition functions */
    mid_sm_app_ctrl uiCtrl;                /* Control flags */
    mid_sm_app_state_t eState;             /* State */
} mid_sm_app_ctrl_t;

/*! @brief Pointer to function with a pointer to state machine control structure */
typedef void (*mid_pfcn_void_pms)(mid_sm_app_ctrl_t *sAppCtrl);

/* external variables */
extern mid_struct_t g_sMID;

/*! @brief State machine function table */
extern const mid_pfcn_void_pms g_MID_SM_STATE_TABLE[10];

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief MID State machine function
 *
 * @param sAppCtrl     Pointer to application state machine
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
