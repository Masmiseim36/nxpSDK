/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
static void MID_StateStop(void);

/* (user) Motor parameters identification state-transition functions */
static void MID_TransStart2PwrStgCharact(void);
static void MID_TransStart2Rs(void);
static void MID_TransStart2Pp(void);
static void MID_TransPwrStgCharact2Stop(void);
static void MID_TransRs2Ld(void);
static void MID_TransLd2Lq(void);
static void MID_TransLq2Ke(void);
static void MID_TransKe2Stop(void);
static void MID_TransPp2Stop(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
mid_get_rs_a1_t sMIDRs;                /* Input structure for MID_getRs()*/
mid_get_char_a1_t sMIDPwrStgChar;      /* Input structure for MID_getTransChar() */
mid_get_ls_a1_t sMIDLs;                /* Input structure for MID_getLs() */
mid_get_ke_a1_t sMIDKe;                /* Input structure for MID_getKe() */
mid_get_pp_a1_t sMIDPp;                /* Input structure for MID_getPp() */
mid_align_a1_t sMIDAlignment;          /* Input structure for MID_alignment() */
uint16_t ui16FaultMID;                 /* MID fault number, 0x0000 = no fault */
uint16_t ui16EnableMeasurement;        /* Enables measurement in superior machine */
frac16_t *pf16Ud;                      /* Pointer to required voltage Ud */
frac16_t *pf16Id;                      /* Pointer to phase current Id */
frac16_t *pf16Uq;                      /* Pointer to required voltage Uq */
frac16_t *pf16Iq;                      /* Pointer to phase current Iq */
volatile float fltMIDresistanceScale;  /* FreeMaster resistance scale */
volatile float fltMIDfrequencyScale;   /* FreeMaster frequency scale */
volatile float fltMIDimpedanceScale;   /* FreeMaster impedance scale */
volatile float fltMIDinductanceDScale; /* FreeMaster D-axis inductance scale */
volatile float fltMIDinductanceQScale; /* FreeMaster Q-axis inductance scale */
volatile float fltMIDbemfConstScale;   /* FreeMaster Bemf constant scale */

/* State machine functions field */
static const mid_sm_app_state_fcn_t msSTATE = {
    MID_StateStart, MID_StatePwrStgCharact, MID_StateRs, MID_StateLd, MID_StateLq, MID_StatePp,
    MID_StateKe,    MID_StateStop};

/* State-transition functions field */
/*
 * note: order of the transition function should be the same as in definition
 * of the mid_sm_app_trans_fcn_t type
 */
static const mid_sm_app_trans_fcn_t msTRANS = {
    MID_TransStart2PwrStgCharact,
    MID_TransStart2Rs,
    MID_TransStart2Pp,
    MID_TransPwrStgCharact2Stop,
    MID_TransRs2Ld,
    MID_TransLd2Lq,
    MID_TransLq2Ke,
    MID_TransKe2Stop,
    MID_TransPp2Stop,
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
    /* MID alignment */
    MID_alignment(&sMIDAlignment);

    /* When MID_alignment is done */
    if (sMIDAlignment.ui16Active == 0U)
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
    MID_GetTransferCharacteristic(&sMIDPwrStgChar);

    /* If characteristic for the last current was measured (f16TransferCharIndex == number of ) */
    if (sMIDPwrStgChar.ui16Active == 0U)
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
    MID_getRs(&sMIDRs);

    /* when the R_s measurement is finished, set the RS_DONE flag */
    if (sMIDRs.ui16Active == 0U)
    {
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
    MID_getLs(&sMIDLs);

    /* when the L_s measurement is finished, set the LS_DONE flag */
    if (sMIDLs.ui16Active == 0U)
    {
        /* Store measured Ls to Ld */
        sMIDLs.f16Ld         = sMIDLs.f16Ls;
        sMIDLs.i16ShiftZdMax = sMIDLs.i16ShiftZsMax;
        sMIDLs.i16ShiftLdMax = sMIDLs.i16ShiftLsMax;

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
    MID_getLs(&sMIDLs);

    /* when the L_s measurement is finished, set the LS_DONE flag */
    if (sMIDLs.ui16Active == 0U)
    {
        /* Store measured Ls to Lq */
        sMIDLs.f16Lq         = sMIDLs.f16Ls;
        sMIDLs.i16ShiftZqMax = sMIDLs.i16ShiftZsMax;
        sMIDLs.i16ShiftLqMax = sMIDLs.i16ShiftLsMax;

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
    MID_getPp(&sMIDPp);

    /* Escape MID_StateKe when measurement ends */
    if (sMIDPp.ui16Active == 0U)
    {
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
    /* Type the code to do when in the LS state */

    /* When MCAT calculates controllers and Bemf observer */
    if (sMIDKe.ui16MCATObsrvDone == 1U)
    {
        /* Call Rs measurement routine */
        MID_getKe(&sMIDKe);
        /* Escape MID_StateKe when measurement ends */
        if (sMIDKe.ui16Active == 0U)
        {
            g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_KE_DONE;
        }
    }
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
    sMIDPwrStgChar.ui16Active = 0U;

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
    sMIDRs.ui16Active        = 0U;
    sMIDRs.pf16UdErrorLookUp = &(f16TransferCharError[0]);

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
    sMIDPp.ui16Active       = 0U;
    sMIDPp.ui16PpDetermined = 0U;

    /* Acknowledge that the system can proceed into the STOP state */
    g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_PP_ACK;
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
    sMIDLs.pf16UdReq  = pf16Ud;
    sMIDLs.pf16Idfbck = pf16Id;
    *pf16Uq           = FRAC16(0.0);
    *pf16Iq           = FRAC16(0.0);
    sMIDLs.f16Ls      = FRAC16(0.0);

    /* enable start of Ls measurement */
    sMIDLs.ui16Active = 0U;

    /* Pass value of Rs to Ls strucutre */
    sMIDLs.f16Rs         = sMIDRs.f16Rs;
    sMIDLs.i16ShiftRsMax = sMIDRs.i16ShiftRsMax;

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
    sMIDLs.pf16UdReq  = pf16Uq;
    sMIDLs.pf16Idfbck = pf16Iq;
    *pf16Ud           = FRAC16(0.0);
    *pf16Id           = FRAC16(0.0);
    sMIDLs.f16Ls      = FRAC16(0.0);

    /* enable start of Ls measurement */
    sMIDLs.ui16Active = 0U;

    /* Pass value of Rs to Ls strucutre */
    sMIDLs.f16Rs         = sMIDRs.f16Rs;
    sMIDLs.i16ShiftRsMax = sMIDRs.i16ShiftRsMax;

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

    /* Reset Ud voltage during calibration because it wasn't reset in Ls meas */
    *pf16Ud = FRAC16(0.0);
    *pf16Uq = FRAC16(0.0);

    sMIDKe.ui16MCATObsrvDone = 0U;
    sMIDKe.ui16Active        = 0U;

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
