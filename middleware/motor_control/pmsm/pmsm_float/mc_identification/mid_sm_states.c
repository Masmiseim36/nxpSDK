/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mid_sm_states.h"
#include "mlib.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* (user) Motor parameters identification state machine functions */
static void MID_StateStart(void);
static void MID_StatePwrStgCharact(void);
static void MID_StateRs(void);
static void MID_StateLd(void);
static void MID_StateLq(void);
static void MID_StatePp(void);
static void MID_StateKe(void);
static void MID_StateMech(void);
static void MID_StateHall(void);
static void MID_StateStop(void);

/* (user) Motor parameters identification state-transition functions */
static void MID_TransStart2PwrStgCharact(void);
static void MID_TransStart2Rs(void);
static void MID_TransStart2Pp(void);
static void MID_TransStart2Mech(void);
static void MID_TransStart2Hall(void);
static void MID_TransPwrStgCharact2Stop(void);
static void MID_TransRs2Ld(void);
static void MID_TransLd2Lq(void);
static void MID_TransLq2Ke(void);
static void MID_TransKe2Stop(void);
static void MID_TransMech2Stop(void);
static void MID_TransPp2Stop(void);
static void MID_TransAll2Stop(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
mid_struct_t g_sMID; /* Global structure for all measurements */

/* State machine functions field */
static const mid_sm_app_state_fcn_t msSTATE = {
    MID_StateStart, MID_StatePwrStgCharact, MID_StateRs,  MID_StateLd, MID_StateLq, MID_StatePp,
    MID_StateKe,    MID_StateMech,          MID_StateHall, MID_StateStop};

/* State-transition functions field */
/*
 * note: order of the transition function should be the same as in definition
 * of the mid_sm_app_trans_fcn_t type
 */
static const mid_sm_app_trans_fcn_t msTRANS = {
    MID_TransStart2PwrStgCharact,
    MID_TransStart2Rs,
    MID_TransStart2Pp,
    MID_TransStart2Mech,
    MID_TransStart2Hall,
    MID_TransPwrStgCharact2Stop,
    MID_TransRs2Ld,
    MID_TransLd2Lq,
    MID_TransLq2Ke,
    MID_TransKe2Stop,
    MID_TransPp2Stop,
    MID_TransMech2Stop,
    MID_TransAll2Stop,
};

/* State machine structure definition and initialization */
mid_sm_app_ctrl_t g_sMIDCtrl = {
    /* g_sMIDCtrl.psState, User state functions  */
    &msSTATE,

    /* g_sMIDCtrl.psTrans, User state-transition functions */
    &msTRANS,

    /* g_sMIDCtrl.uiCtrl, Default no control command */
    MID_SM_CTRL_NONE,

    /* g_sMIDCtrl.eState, Default state after reset */
    kMID_Start};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief MID START state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateStart(void)
{
    /* Type the code to do when in the START state */
    /* Init pointer to Id required */
    g_sMID.sMIDAlignment.pfltIdReq = g_sMID.sIO.pfltIdReq;

    /* MID alignment */
    MID_alignment(&g_sMID.sMIDAlignment);

    /* When MID_alignment is done */
    if (g_sMID.sMIDAlignment.ui16Active == FALSE)
    {
        /* set _DONE to switch to following transition state */
        g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_START_DONE;
    }
}

/*!
 * @brief MID PWR_STG_CHARACT state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StatePwrStgCharact(void)
{
    /* Type the code to do when in the PWR_STG_CHARACT state */
    MID_GetTransferCharacteristic(&g_sMID.sMIDPwrStgChar);

    /* If characteristic for the last current was measured (f16TransferCharIndex == number of ) */
    if (g_sMID.sMIDPwrStgChar.ui16Active == FALSE)
    {
        /* set _DONE to switch to following transition state */
        g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_PWR_STG_CHARACT_DONE;
    }
}

/*!
 * @brief MID RS state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateRs(void)
{
    /* Type the code to do when in the RS state */
    /* call Rs measurement routine */
    MID_getRs(&g_sMID.sMIDRs);

    /* when the R_s measurement is finished, set the RS_DONE flag */
    if (g_sMID.sMIDRs.ui16Active == FALSE)
    {
        /* Set dq required currents and voltages to zero */
        *(g_sMID.sIO.pfltUdReq) = 0.0;
        *(g_sMID.sIO.pfltIdReq) = 0.0;
        *(g_sMID.sIO.pfltUqReq) = 0.0;
        *(g_sMID.sIO.pfltIqReq) = 0.0;

        /* set _DONE to switch to following transition state */
        g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_RS_DONE;
    }
}

/*!
 * @brief MID Ld state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateLd(void)
{
    /* Type the code to do when in the LS state */

    /* call Rs measurement routine */
    MID_getLs(&g_sMID.sMIDLs);

    /* when the L_s measurement is finished, set the LS_DONE flag */
    if (g_sMID.sMIDLs.ui16Active == FALSE)
    {
        /* Set dq required currents and voltages to zero */
        *(g_sMID.sIO.pfltUdReq) = 0.0;
        *(g_sMID.sIO.pfltIdReq) = 0.0;
        *(g_sMID.sIO.pfltUqReq) = 0.0;
        *(g_sMID.sIO.pfltIqReq) = 0.0;

        /* Store measured Ls to Ld */
        g_sMID.sMIDLs.fltLd = g_sMID.sMIDLs.fltLs;

        /* set _DONE to switch to following transition state */
        g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_LD_DONE;
    }
}

/*!
 * @brief MID LQ state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateLq(void)
{
    /* Type the code to do when in the LS state */

    /* call Rs measurement routine */
    MID_getLs(&g_sMID.sMIDLs);

    /* when the L_s measurement is finished, set the LS_DONE flag */
    if (g_sMID.sMIDLs.ui16Active == FALSE)
    {
        /* Set dq required currents and voltages to zero */
        *(g_sMID.sIO.pfltUdReq) = 0.0;
        *(g_sMID.sIO.pfltIdReq) = 0.0;
        *(g_sMID.sIO.pfltUqReq) = 0.0;
        *(g_sMID.sIO.pfltIqReq) = 0.0;

        /* Store measured Ls to Lq */
        g_sMID.sMIDLs.fltLq = g_sMID.sMIDLs.fltLs;

        /* set _DONE to switch to following transition state */
        g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_LQ_DONE;
    }
}

/*!
 * @brief MID PP state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StatePp(void)
{
    /* Type the code to do when in the PP state */

    /* Call Pp measurement routine */
    MID_getPp(&g_sMID.sMIDPp);

    /* Escape MID_StateKe when measurement ends */
    if (g_sMID.sMIDPp.ui16Active == FALSE)
    {
        /* Set dq required currents and voltages to zero */
        *(g_sMID.sIO.pfltUdReq) = 0.0;
        *(g_sMID.sIO.pfltIdReq) = 0.0;
        *(g_sMID.sIO.pfltUqReq) = 0.0;
        *(g_sMID.sIO.pfltIqReq) = 0.0;

        /* set _DONE to switch to following transition state */
        g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_PP_DONE;
    }
}

/*!
 * @brief MID KE state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateKe(void)
{
    /* Type the code to do when in the KE state */

    /* When MCAT calculates controllers and Bemf observer */
    if (g_sMID.sMIDKe.ui16MCATObsrvDone == TRUE)
    {
        /* Call Rs measurement routine */
        MID_getKe(&g_sMID.sMIDKe);

        /* Escape MID_StateKe when measurement ends */
        if (g_sMID.sMIDKe.ui16Active == FALSE)
        {
            /* Set dq required currents and voltages to zero */
            *(g_sMID.sIO.pfltUdReq) = 0.0;
            *(g_sMID.sIO.pfltIdReq) = 0.0;
            *(g_sMID.sIO.pfltUqReq) = 0.0;
            *(g_sMID.sIO.pfltIqReq) = 0.0;

            /* set _DONE to switch to following transition state */
            g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_KE_DONE;
        }
    }
}

/*!
 * @brief MID MECH state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateMech(void)
{
    /* Type the code to do when in the MECH state */

    /* Call Rs measurement routine */
    MID_getMech(&g_sMID.sMIDMech);

    /* Escape MID_StateKe when measurement ends */
    if (g_sMID.sMIDMech.ui16Active == FALSE)
    {
        /* Set dq required currents and voltages to zero */
        *(g_sMID.sIO.pfltUdReq) = 0.0;
        *(g_sMID.sIO.pfltIdReq) = 0.0;
        *(g_sMID.sIO.pfltUqReq) = 0.0;
        *(g_sMID.sIO.pfltIqReq) = 0.0;

        /* set _DONE to switch to following transition state */
        g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_MECH_DONE;
    }
}

/*!
 * @brief MID HALL state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateHall(void)
{
    /* Type the code to do when in the MECH state */

    /* set _DONE to switch to following transition state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_HALL_DONE;
}

/*!
 * @brief MID STOP state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_StateStop(void)
{
    /* Type the code to do when in the STOP state */

    /*
     * motor parameters measurement is finished, main state machine
     * can proceed with next state
     */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_STOP_DONE;
}

/*!
 * @brief MID START to PWR_STG_CHARACT transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransStart2PwrStgCharact(void)
{
    /* Type the code to do when going from the START to the PWR_STG_CHARACT state */

    /* enable start of characterisation */
    g_sMID.sMIDPwrStgChar.ui16Active = FALSE;

    /* Init pointers */
    g_sMID.sMIDPwrStgChar.pfltIdReq  = g_sMID.sIO.pfltIdReq;
    g_sMID.sMIDPwrStgChar.pfltUdReq  = g_sMID.sIO.pfltUdReq;
    g_sMID.sMIDPwrStgChar.pfltIdfbck = g_sMID.sIO.pfltId;
    g_sMID.sMIDPwrStgChar.pfltUDCbus = g_sMID.sIO.pfltUDCbus;

    /* acknowledge that the state machine can proceed into PWR_STG_CHARACT state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_PWR_STG_CHARACT_ACK;
}

/*!
 * @brief MID START to RS transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransStart2Rs(void)
{
    /* Type the code to do when going from the PWR_STG_CHARACT to the RS state */

    /* enable start of Rs measurement */
    g_sMID.sMIDRs.ui16Active = FALSE;

    /* Init pointers */
    g_sMID.sMIDRs.pfltIdReq  = g_sMID.sIO.pfltIdReq;
    g_sMID.sMIDRs.pfltUdReq  = g_sMID.sIO.pfltUdReq;
    g_sMID.sMIDRs.pfltIdfbck = g_sMID.sIO.pfltId;

    /* acknowledge that the state machine can proceed into RS state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_RS_ACK;
}

/*!
 * @brief MID START to PP transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransStart2Pp(void)
{
    /* Type the code to do when going from the LS to the STOP state */
    g_sMID.sMIDPp.ui16Active       = FALSE;
    g_sMID.sMIDPp.ui16PpDetermined = FALSE;

    /* Init pointers */
    g_sMID.sMIDPp.pf16PosEl = g_sMID.sIO.pf16PosElExt;
    g_sMID.sMIDPp.pfltIdReq = g_sMID.sIO.pfltIdReq;

    /* Acknowledge that the system can proceed into the STOP state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_PP_ACK;
}

/*!
 * @brief MID START to MECH transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransStart2Mech(void)
{
    /* Type the code to do when going from the LS to the STOP state */

    /* Init pointers */
    g_sMID.sMIDMech.pfltIdReq    = g_sMID.sIO.pfltIdReq;
    g_sMID.sMIDMech.pfltIqReq    = g_sMID.sIO.pfltIqReq;
    g_sMID.sMIDMech.pfltIdfbck   = g_sMID.sIO.pfltId;
    g_sMID.sMIDMech.pfltIqfbck   = g_sMID.sIO.pfltIq;
    g_sMID.sMIDMech.pfltSpeedEst = g_sMID.sIO.pfltSpeedEst;
    g_sMID.sMIDMech.pf16PosElEst = g_sMID.sIO.pf16PosElEst;
    g_sMID.sMIDMech.pf16PosElExt = g_sMID.sIO.pf16PosElExt;

    g_sMID.sMIDMech.ui16Active = FALSE;

    /* Acknowledge that the system can proceed into the STOP state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_MECH_ACK;
}

/*!
 * @brief MID START to HALL transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransStart2Hall(void)
{
    /* Type the code to do when going from the LS to the STOP state */

    /* Init pointers */
    g_sMID.sMIDHall.pfltSpeedReq = g_sMID.sIO.pfltSpeedReq;
    g_sMID.sMIDHall.pfltSpeedEst = g_sMID.sIO.pfltSpeedEst;

    g_sMID.sMIDHall.ui16Active = FALSE;

    /* Acknowledge that the system can proceed into the STOP state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_HALL_ACK;
}

/*!
 * @brief MID PWR_STG_CHARACT to STOP transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransPwrStgCharact2Stop(void)
{
    /* Type the code to do when going from the PWR_STG_CHARACT to the RS state */

    /* acknowledge that the state machine can proceed into RS state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_STOP_ACK;
}

/*!
 * @brief MID RS to LD transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransRs2Ld(void)
{
    /* Type the code to do when going from the RS to the LS state */

    /* Pass Ud, Id references to UdReq, Idfbck => measurement in direct axis */
    g_sMID.sMIDLs.pfltUdReq  = g_sMID.sIO.pfltUdReq;
    g_sMID.sMIDLs.pfltIdfbck = g_sMID.sIO.pfltId;

    /* enable start of Ls measurement */
    g_sMID.sMIDLs.ui16Active = FALSE;

    /* Pass value of Rs to Ls strucutre */
    g_sMID.sMIDLs.fltRs = g_sMID.sMIDRs.fltRs;

    /* acknowledge that the state machine can proceed into LS state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_LD_ACK;
}

/*!
 * @brief MID LD to LQ transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransLd2Lq(void)
{
    /* Type the code to do when going from the RS to the LS state */

    /* Pass Ud, Id references to Udfbck, Idfbck => measurement in direct axis */
    g_sMID.sMIDLs.pfltUdReq  = g_sMID.sIO.pfltUqReq;
    g_sMID.sMIDLs.pfltIdfbck = g_sMID.sIO.pfltIq;

    /* enable start of Ls measurement */
    g_sMID.sMIDLs.ui16Active = FALSE;

    /* Pass value of Rs to Ls strucutre */
    g_sMID.sMIDLs.fltRs = g_sMID.sMIDRs.fltRs;

    /* acknowledge that the state machine can proceed into LS state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_LQ_ACK;
}

/*!
 * @brief MID LQ to KE transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransLq2Ke(void)
{
    /* Type the code to do when going from the LS to the STOP state */

    /* Init pointers */
    g_sMID.sMIDKe.pf16PosEl  = g_sMID.sIO.pf16PosElExt;
    g_sMID.sMIDKe.pfltIdfbck = g_sMID.sIO.pfltId;
    g_sMID.sMIDKe.pfltIdReq  = g_sMID.sIO.pfltIdReq;
    g_sMID.sMIDKe.pfltUdReq  = g_sMID.sIO.pfltUdReq;
    g_sMID.sMIDKe.pfltEd     = g_sMID.sIO.pfltEd;
    g_sMID.sMIDKe.pfltEq     = g_sMID.sIO.pfltEq;

    g_sMID.sMIDKe.ui16MCATObsrvDone = FALSE;
    g_sMID.sMIDKe.ui16Active        = FALSE;

    /* Acknowledge that the system can proceed into the STOP state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_KE_ACK;
}

/*!
 * @brief MID KE to STOP transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransKe2Stop(void)
{
    /* Type the code to do when going from the LS to the STOP state */

    /* Acknowledge that the system can proceed into the STOP state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_STOP_ACK;
}

/*!
 * @brief MID PP to STOP transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransPp2Stop(void)
{
    /* Type the code to do when going from the LS to the STOP state */

    /* Acknowledge that the system can proceed into the STOP state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_STOP_ACK;
}

/*!
 * @brief MID MECH to STOP transition
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void MID_TransMech2Stop(void)
{
    /* Type the code to do when going from the LS to the STOP state */

    /* Acknowledge that the system can proceed into the STOP state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_STOP_ACK;
}

/*!
 * @brief (general) Any state to MID STOP transition
 *
 * @param None
 *
 * @return None
 */
static void MID_TransAll2Stop(void)
{
    /* type the code to do when going to the STOP state */

    /* Zero all output pointers */
    *g_sMID.sIO.pf16PosElExt = 0;
    *g_sMID.sIO.pfltIdReq    = 0.0F;
    *g_sMID.sIO.pfltIqReq    = 0.0F;
    *g_sMID.sIO.pfltUdReq    = 0.0F;
    *g_sMID.sIO.pfltUqReq    = 0.0F;

    /* Reset control flags */
    g_sMID.sMIDKe.ui16MCATObsrvDone = FALSE;
    g_sMID.ui16EnableMeasurement    = FALSE;

    /* Reset active flags */
    g_sMID.sMIDAlignment.ui16Active  = FALSE;
    g_sMID.sMIDPwrStgChar.ui16Active = FALSE;
    g_sMID.sMIDRs.ui16Active         = FALSE;
    g_sMID.sMIDLs.ui16Active         = FALSE;
    g_sMID.sMIDKe.ui16Active         = FALSE;
    g_sMID.sMIDPp.ui16Active         = FALSE;
    g_sMID.sMIDMech.ui16Active       = FALSE;

    /* acknowledge that the state machine can proceed into STOP state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_STOP_ACK;
}
