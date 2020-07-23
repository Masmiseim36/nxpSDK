/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "m1_state_routines.h"

MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

#include "freemaster_tsa_pmsm.h"

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* The FOC action data type. */
typedef enum _foc_action
{
    /* No action. */
    kFOC_Action_NoAction   = 0U,
    /* Request MC.SM to go to STOP. */
    kFOC_Action_ToStop     = 1U,
    /* Request MC.SM to go to  READY.*/
    kFOC_Action_ToReady    = 2U,
    /* Request MC.SM to go to RUN and apply closed-loop speed-control FOC. */
    kFOC_Action_ToSpeedCL  = 3U,
    /* Request MC.SM to go to RUN and apply closed-loop current-control FOC. */
    kFOC_Action_ToCurrCL   = 4U,
    /* Request MC.SM to go to RUN and apply open-loop scalar control. */
    kFOC_Action_ToScalarOL = 5U,
    /* Request MC.SM to go to RUN and apply open-loop current control. */
    kFOC_Action_ToCurrOL   = 6U,
    /* Request MC.SM to go to RUN and apply open-loop voltage control. */
    kFOC_Action_ToVoltOL   = 7U,
    /* Request MC.SM to go to RUN and apply MID direct control. */
    kFOC_Action_ToMID      = 8U
} foc_action_t;

/* Pointer to the fast loop control function data type. */
typedef GMCLIB_2COOR_ALBE_T_F16 (*pfcn_ualbe_meas)(void);


/*! @brief PMSM FOC control structure. */
typedef struct _mc_foc_pmsm
{
    struct
    {
        /* Voltage control variables. */
        struct
        {
            /* Actual required voltage vector in rotor coordinates. */
            GMCLIB_2COOR_DQ_T_F16     sUDQReq;
            /* Actual required voltage vector in stator coordinates. */
            GMCLIB_2COOR_ALBE_T_F16   sUAlBeReq;
            /* Maximum allowable duty cycle. */
            frac16_t  f16DutyCycleLimit;
        } sVolt;

        /* Current control variables. */
        struct
        {
            /* Direct axis current PI controller parameters. */
            GFLIB_CTRL_PI_P_AW_T_A32  sIdPiPar;
            /* Quadrature axis current PI controller parameters. */
            GFLIB_CTRL_PI_P_AW_T_A32  sIqPiPar;
            /* Measured current vector in rotor coordinates. */
            GMCLIB_2COOR_DQ_T_F16     sIDQ;
            /* Required current vector in rotor coordinates. */
            GMCLIB_2COOR_DQ_T_F16     sIDQReq;
            /* Current error vector in rotor coordinates. */
            GMCLIB_2COOR_DQ_T_F16     sIDQErr;
            /* D-axis current PI controller manual integration stop flag. */
            bool_t    bIdPiStopInteg;
            /* Q-axis current PI controller manual integration stop flag. */
            bool_t    bIqPiStopInteg;
        } sCurr;

        /* Speed control variables. */
        struct
        {
            /* Measured speed filter. */
            GDFLIB_FILTER_IIR1_T_F32 sSpdElFiltPar;
            /* Speed PI controller parameters. */
            GFLIB_CTRL_PI_P_AW_T_A32 sSpdElPiPar;
            /* Speed ramp parameters. */
            GFLIB_RAMP_T_F32         sSpdElRampPar;
            frac16_t  f16SpdElFilt;      /* Filtered speed. */
            frac16_t  f16SpdElErr;       /* Speed error. */
            frac16_t  f16SpdElRamp;      /* Ramped required speed. */
            bool_t    bSpdElPiStopInteg; /* PI controller saturation flag. */
        } sSpd;

        /* Open-loop control variables. */
        struct
        {
            /* Frequency to position integrator parameters. */
            GFLIB_INTEGRATOR_T_A32 sFrqElIntPar;
            /* Parameters of frequency ramp. */
            GFLIB_RAMP_T_F32       sFrqElRampPar;
            acc32_t   a32VHzGain;     /* V/Hz factor gain for scalar control. */
            frac16_t  f16FrqElRamp;   /* Ramped frequency command. */
            frac16_t  f16PosEl;       /* Generated electrical position. */
        } sOL;

        /* The actually used position sine and cosine. */
        GMCLIB_2COOR_SINCOS_T_F16   sPosElSC;
    } sCtrl;

    /* The command structure */
    struct
    {
        /* The open-loop control mode commands. */
        struct
        {
            /* The commands are used in ALIGN, LO_SPD, MI_SPD and HI_SPD MC.SM
               states. */
            /* Required stator voltage for kFOC_CtrlMode_VoltOL control mode. */
            GMCLIB_2COOR_DQ_T_F16 sUDQReq;
            /* Required stator current for kFOC_CtrlMode_CurrOL control mode. */
            GMCLIB_2COOR_DQ_T_F16 sIDQReq;
            /* Required frequency for kFOC_CtrlMode_VoltOL, kFOC_CtrlMode_CurrOL
               and kFOC_CtrlMode_ScalarOL control modes. */
            frac16_t f16FrqElCmd;
            /* Required position shift for kFOC_CtrlMode_VoltOL,
               kFOC_CtrlMode_CurrOL, and kFOC_CtrlMode_ScalarOL control modes. */
            frac16_t f16PosElCmd;
            /* The open-loop control mode configuration. */
            foc_ctrl_mode_cfg_t sCfg;
        } sOL;

        /* The current FOC control mode command (valid for kFOC_CtrlMode_CurrCL
           control mode). */
        struct
        {
            /* The stator current used in LO_SPD, MI_SPD and HI_SPD MC.SM
               states. */
            /*! @note The alignment voltage is shared with sSpeedFOC. */
            GMCLIB_2COOR_DQ_T_F16 sIDQReq;
        } sCurrFOC;

        /* The speed FOC control mode commands. */
        struct
        {
            struct
            {
               /* Required stator voltage used in ALIGN MC.SM state. */
               GMCLIB_2COOR_DQ_T_F16 sUDQReq;
            } sAlign;

            struct
            {
               /* Required stator current used in LO_SPD and MI_SPD MC.SM
                  states. */
               GMCLIB_2COOR_DQ_T_F16 sIDQReq;
            } sLoMiSpd;

            struct
            {
               frac16_t  f16SpeedCmd;  /* The HI_SPD state required speed. */
            } sHiSpd;
        } sSpdFOC;

        /* The intended direction of rotor rotation. */
        frac16_t        f16Direction;
        foc_ctrl_mode_t eCtrlMode;    /* Currently active control mode. */
        foc_action_t    eAction;      /* Control action. */
    } sCmd;

    /* Configured fast-loop control functions. */
    struct
    {
       pfcn_ualbe_meas  pfcnAlignFL;  /* Alignment state control function. */
       pfcn_ualbe_meas  pfcnLoSpdFL;  /* Low-speed state control function. */
       pfcn_ualbe_meas  pfcnMiSpdFL;  /* Mid-speed state control function. */
       pfcn_ualbe_meas  pfcnHiSpdFL;  /* High-speed state control function. */
    } sCtrlFcn;

    /* Quantity scales. */
    struct
    {
        float fltVoltage;          /* Stator voltage scale. [V] */
        float fltDCBVoltage;       /* DC-bus voltage scale. [V] */
        float fltCurrent;          /* Stator current scale. [A] */
        float fltFrequency;        /* Frequency scale. [Hz] */
        float fltSpeedMe;          /* Mechanical rotor speed scale. [rpm] */
        float fltBemf;             /* BEMF voltage scale. [V] */
        float fltFastSamplPeriod;  /* Fast control loop period. [s] */
        float fltSlowSamplPeriod;  /* Slow control loop period. [s]*/
    } sScale;

    /* The FOC status to be passed to MC.SM every SL. */
    foc_status_t sStatusFOC;

    /* The MC.SM status passed to FOC every SL. */
    mc_status_t sStatusMC;

    /* The MC.SM measurement passed to FOC every FL. */
    mc_foc_input_meas_t sMeas;
} foc_pmsm_t;


/*******************************************************************************
 * Variables
 ******************************************************************************/
static foc_pmsm_t   s_sM1FOC; /* The main FOC data structure. */


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*                         APPLICATION INTERFACE                              */
/*!
 * @brief   The FOC variables TSA table.
 *
 * @details See freemaster_tsa.h for more details about TSA table construction.
 *
 * @param   TSA table identificator.
 *
 * @return  FMSTR_TSA_ENTRY* - The TSA table entry data type.
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
FMSTR_TSA_TABLE_BEGIN(TSA_FOC)
    /*               Read-write variables (parameters) below.                 */
    /* The scalar control loop parameters. */
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sOL.a32VHzGain,   FMSTR_TSA_FRAC_Q(16, 15))
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sOL.sFrqElIntPar.a32Gain, FMSTR_TSA_FRAC_Q(16, 15))
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sOL.sFrqElRampPar.f32RampUp,   FMSTR_TSA_FRAC32)
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sOL.sFrqElRampPar.f32RampDown, FMSTR_TSA_FRAC32)

    /* The voltage control loop parameters. */
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sVolt.f16DutyCycleLimit, FMSTR_TSA_FRAC16)

    /* The current control loop parameters. */
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sCurr.sIdPiPar.a32PGain, FMSTR_TSA_FRAC_Q(16, 15))
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sCurr.sIdPiPar.a32IGain, FMSTR_TSA_FRAC_Q(16, 15))
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sCurr.sIqPiPar.a32PGain, FMSTR_TSA_FRAC_Q(16, 15))
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sCurr.sIqPiPar.a32IGain, FMSTR_TSA_FRAC_Q(16, 15))

    /* The speed control loop parameters. */
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sSpd.sSpdElFiltPar.sFltCoeff.f32B0, FMSTR_TSA_FRAC32)
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.f16UpperLim, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.f16LowerLim, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.a32PGain, FMSTR_TSA_FRAC_Q(16, 15))
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.a32IGain, FMSTR_TSA_FRAC_Q(16, 15))
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sSpd.sSpdElRampPar.f32RampUp,   FMSTR_TSA_FRAC32)
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCtrl.sSpd.sSpdElRampPar.f32RampDown, FMSTR_TSA_FRAC32)

    /* Command parameters. */
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCmd.sSpdFOC.sAlign.sUDQReq.f16D,  FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR(s_sM1FOC.sCmd.sSpdFOC.sLoMiSpd.sIDQReq.f16D, FMSTR_TSA_FRAC16)


    /* Quantity scales. */
    FMSTR_TSA_RW_VAR(s_sM1FOC.sScale.fltVoltage,    FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(s_sM1FOC.sScale.fltDCBVoltage, FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(s_sM1FOC.sScale.fltCurrent,    FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(s_sM1FOC.sScale.fltFrequency,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(s_sM1FOC.sScale.fltSpeedMe,    FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(s_sM1FOC.sScale.fltBemf,       FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(s_sM1FOC.sScale.fltFastSamplPeriod,FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(s_sM1FOC.sScale.fltSlowSamplPeriod,FMSTR_TSA_FLOAT)

    /*          Read-only variables(measurements, states, flags) below.       */
    /* The scalar control loop state variables. */
    FMSTR_TSA_RO_VAR(s_sM1FOC.sCtrl.sOL.f16FrqElRamp, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR(s_sM1FOC.sCtrl.sOL.f16PosEl,     FMSTR_TSA_FRAC16)

    /* The voltage control loop state variables. */
    FMSTR_TSA_RO_VAR(s_sM1FOC.sCtrl.sVolt.sUDQReq.f16D, FMSTR_TSA_FRAC16)

    /* The current control loop state variables. */
    FMSTR_TSA_RO_VAR(s_sM1FOC.sCtrl.sCurr.sIDQ.f16D, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR(s_sM1FOC.sCtrl.sCurr.sIDQReq.f16D, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR(s_sM1FOC.sCtrl.sCurr.sIdPiPar.bLimFlag, FMSTR_TSA_SINT16)
    FMSTR_TSA_RO_VAR(s_sM1FOC.sCtrl.sCurr.sIqPiPar.bLimFlag, FMSTR_TSA_SINT16)
    FMSTR_TSA_RO_VAR(s_sM1FOC.sCtrl.sCurr.bIdPiStopInteg, FMSTR_TSA_SINT16)
    FMSTR_TSA_RO_VAR(s_sM1FOC.sCtrl.sCurr.bIqPiStopInteg, FMSTR_TSA_SINT16)

    /* The speed control loop state variables. */
    FMSTR_TSA_RO_VAR(s_sM1FOC.sCtrl.sSpd.f16SpdElFilt, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR(s_sM1FOC.sCtrl.sSpd.f16SpdElRamp, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RO_VAR(s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.bLimFlag, FMSTR_TSA_SINT16)
    FMSTR_TSA_RO_VAR(s_sM1FOC.sCtrl.sSpd.bSpdElPiStopInteg, FMSTR_TSA_SINT16)

    /* The state-machine states. */
    FMSTR_TSA_RO_VAR(s_sM1FOC.sStatusFOC.ui16Flags, FMSTR_TSA_UINT16)
    FMSTR_TSA_RO_VAR(s_sM1FOC.sCmd.eAction, FMSTR_TSA_UINT16)

FMSTR_TSA_TABLE_END()
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*                         APPLICATION INTERFACE                               */
/*!
 * @brief   Function decides the FOC action based on the application command and
 *          quantity command validity.
 *
 * @details The function checks validity of the quantity command and based on the
 *          application command it performs following actions:
 *          kMC_ReqStop  - The STOP request is issued to the MC.SM. Function
 *                         returns FALSE
 *          kMC_ReqReady - The READY request is issued to the MC.SM (if not
 *                         already issued). Function returns FALSE.
 *          kMC_ReqRun   - If the quantity command is:
 *                         Invalid - The READY request is issued to the MC.SM.
 *                                   Function returns FALSE.
 *                         Valid - Function returns FALSE. The caller function
 *                                 should then decide whether FOC action should
 *                                 be issued.
 *
 * @param   eAppCmd - Application command (STOP, READY and RUN).
 *          bQtyCmdValid - Quantity command validity flag
 *                         (TRUE = valid, FALSE = invalid).
 *
 * @retval  TRUE  - The RUN request will be sent to MC.SM. The caller function
 *                  should then decide whether to issue FOC action.
 * @retval  FALSE - Other than RUN request was sent to the MC.SM because of
 *                  STOP/READY application command or invalid quantity command.
 */
static bool_t FOC_CheckAppCmd_BL(foc_app_cmd_t eAppCmd, bool_t bQtyCmdValid);

/*                           CONTROL FUNCTIONS                                */
/*!
 * @brief   Function executes the open-loop stator current, voltage or
 *          scalar-voltage control.
 *
 * @details The open-loop control function. The stator voltage is controlled
 *          based on the control mode as follows:
 *          kFOC_CtrlMode_VoltOL - The open-loop voltage control. The stator
 *                                 voltage is set based on parameters passed to
 *                                 FOC_SetCtrlVoltOL(). The currents are not
 *                                 limited in this control mode.
 *          kFOC_CtrlMode_ScalarOL - The scalar voltage control. The stator
 *                                   voltage is set based on parameters passed to
 *                                   FOC_SetCtrlScalarOL(). The currents are not
 *                                   limited in this control mode.
 *          kFOC_CtrlMode_CurrOL - The open-loop current control. The stator
 *                                 current is controlled based on parameters
 *                                 passed to FOC_SetCtrlCurrOL().
 *          Following steps are performed:
 *          - The required electrical frequency is ramped.
 *          - The electrical position is generated by integration of the ramped
 *            required electrical frequency.
 *          - Sine and cosine of rotor angle provided by MC.SM is calculated.
 *          - The DQ-axis currents are updated.
 *          - Following step differs based on the control mode:
 *             -kFOC_CtrlMode_VoltOL - Zero current control error is set. The
 *                                     required DQ-axis voltages are set into
 *                                     DQ-axis current PI controller
 *                                     accumulators.
 *             -kFOC_CtrlMode_CurrOL - The current control error is calculated.
 *             -kFOC_CtrlMode_ScalarOL - Zero current control error is set. The
 *                                       required Q-axis voltage is calculated
 *                                       based on V/Hz gain and ramped frequency.
 *                                       The resulting DQ-axis voltages are set
 *                                       into DQ-axis current PI controller
 *                                       accumulators.
 *          - Control error is calculated.
 *          - D-axis voltage limits is calculated based on the duty-cycle limit
 *            and the actual DC-bus voltage.
 *          - D-axis voltage required value is calculated by executing PI
 *            controller algorithm.
 *          - Q-axis voltage limit is calculated based on the required D-axis
 *            voltage.
 *          - Q-axis voltage required value is calculated by executing PI
 *            controller algorithm.
 *          - Required DQ-axis voltages are transformed into alpha-beta stator
 *            voltages using inverse Park's transformation.
 *          - Required stator voltage control action is returned.
 *
 * @param   void
 *
 * @return  Required stator voltages in stator reference frame.
 */
static GMCLIB_2COOR_ALBE_T_F16 FOC_CtrlOpenLoop_FL(void);

/*!
 * @brief   Function executes the stator-voltage control.
 *
 * @details The closed-loop current control. Following steps are performed:
 *          -Sine and cosine of rotor angle provided by MC.SM is calculated.
 *          -The DQ-axis currents are updated.
 *          -Required DQ-axis voltages are transformed into alpha-beta stator
 *           voltages using inverse Park's transformation.
 *          -Required stator voltage control action is returned.
 *
 * @param   void
 *
 * @return  Required stator voltages in stator reference frame.
 */
static GMCLIB_2COOR_ALBE_T_F16 FOC_CtrlVolt_FL(void);

/*!
 * @brief   Function executes the closed-loop stator-current control.
 *
 * @details The closed-loop current control. Following steps are performed:
 *          -Sine and cosine of rotor angle provided by MC.SM is calculated.
 *          -The DQ-axis currents are updated.
 *          -Control error is calculated.
 *          -D-axis voltage limits is calculated based on the duty-cycle limit
 *           and the actual DC-bus voltage.
 *          -D-axis voltage required value is calculated by PI controller.
 *          -Q-axis voltage limit is calculated based on the required D-axis
 *           voltage.
 *          -Q-axis voltage required value is calculated by PI controller.
 *          -Required DQ-axis voltages are transformed into alpha-beta stator
 *           voltages using inverse Park's transformation.
 *          -Required stator voltage control action is returned.
 *
 * @param   void
 *
 * @return  Required stator voltages in stator reference frame.
 */
static GMCLIB_2COOR_ALBE_T_F16 FOC_CtrlCurr_FL(void);

/*!
 * @brief   Function executes the closed-loop speed control.
 *
 * @details The closed-loop speed control. Following steps are performed:
 *          -The required speed ramp is updated.
 *          -Speed PI controller saturation flag is updated based.
 *          -Speed control error is calculated.
 *          -The required Q-axis current is calculated by PI controller.
 *
 * @param   void
 *
 * @return  Required quadrature axis current.
 */
static frac16_t FOC_CtrlSpeed_SL(void);


/*******************************************************************************
 * Code
 ******************************************************************************/
/*                         APPLICATION INTERFACE                              */
/*!
 * Function returns the MC.SM state machine status.
 */
mc_status_t FOC_GetMCStatus_SL(void)
{
    /* Return the last MC.SM status. */
    return s_sM1FOC.sStatusMC;
}

/*!
 * Function returns the MC.SM measurements.
 */
mc_foc_input_meas_t FOC_GetMeasMC_FL(void)
{
    /* Return the last MC.SM measurements. */
    return s_sM1FOC.sMeas;
}

/*!
 * The function returns the actual measured DC-bus voltage.
 */
frac16_t FOC_GetMeasUdcb_FL(void)
{
    /* Return the last measured U_DCB value. */
    return s_sM1FOC.sMeas.f16U_DCBFilt;
}

/*!
 * The function returns the actual required AlBe stator voltages.
 */
GMCLIB_2COOR_ALBE_T_F16 FOC_GetMeasUAlBe_FL(void)
{
    /* Return the last required voltage value. */
    return s_sM1FOC.sCtrl.sVolt.sUAlBeReq;
}

/*!
 * The function returns the actual required DQ stator voltages.
 */
GMCLIB_2COOR_DQ_T_F16 FOC_GetMeasUDQ_FL(void)
{
    /* Return the last required voltage value. */
    return s_sM1FOC.sCtrl.sVolt.sUDQReq;
}

/*!
 * The function returns the actual measured AlBe stator currents.
 */
GMCLIB_2COOR_ALBE_T_F16 FOC_GetMeasIAlBe_FL(void)
{
    /* Return the last measured current value. */
    return s_sM1FOC.sMeas.sIAlBe;
}

/*!
 * The function returns the actual measured DQ stator currents.
 */
GMCLIB_2COOR_DQ_T_F16 FOC_GetMeasIDQ_FL(void)
{
    /* Return the last measured current value. */
    return s_sM1FOC.sCtrl.sCurr.sIDQ;
}

/*!
 * Function decides the FOC action based on the application command and
 * quantity command validity.
 */
static bool_t FOC_CheckAppCmd_BL(foc_app_cmd_t eAppCmd, bool_t bQtyCmdValid)
{
    bool_t bRun = FALSE; /* The RUN command active flag value. */

    /* Check the application command.  */
    switch(eAppCmd)
    {
    /* Check whether a RUN application command was issued. */
    case kMC_ReqRun:

        /* Check whether the quantity command is valid. */
        if(FALSE == bQtyCmdValid)
        {
           /* The quantity command is NOT valid. Issue READY action. */
           s_sM1FOC.sCmd.eAction = kFOC_Action_ToReady;
        }
        else
        {
            /* Valid quantity command in RUN state. The caller function should
               issue new control mode action if necessary. */
            bRun = TRUE;
        }
        break;

    /* Check whether a READY application command was issued. */
    case kMC_ReqReady:
        /* Check whether the MC.SM READY was NOT requested from MC.SM. */
        if(kMC_ReqReady != s_sM1FOC.sStatusFOC.eRequest)
        {
            /* Issue READY action. */
            s_sM1FOC.sCmd.eAction = kFOC_Action_ToReady;
        }
        break;

    /* Check whether a STOP application command was issued. */
    default:
        /* Issue STOP action. */
        s_sM1FOC.sCmd.eAction = kFOC_Action_ToStop;
        break;
    }

    /* Return the RUN command active flag value. */
    return bRun;
}
/*!
 * Function checks the application and speed commands and sets the
 * speed closed-loop control mode if possible.
 */
bool_t FOC_SetCtrlSpeedCL_BL(foc_app_cmd_t eAppCmd, frac16_t f16SpeedCmd)
{
    frac16_t  f16SpdReqTmp; /* Variable for checked speed command. */
    frac16_t  f16Dir;       /* Polarity of the speed command. */
    bool_t    bQtyCmdValid; /* Quantity command validity flag. */

    /* Get the absolute value and direction of the speed command. */
    f16SpdReqTmp = MLIB_AbsSat_F16(f16SpeedCmd);
    f16Dir = MLIB_Sign_F16(f16SpeedCmd);

    /* Check whether the speed command is below minimal value. */
    if(f16SpdReqTmp < M1_MERG_SPEED_TRH)
    {
        /* Invalid speed command. */
        bQtyCmdValid = FALSE;

        /* Clear the speed command. */
        f16SpeedCmd = 0;
    }
    else
    {
        /* Check whether the speed command is above the speed limit. */
        if(f16SpdReqTmp > M1_N_NOM)
        {
            /* Limit the speed command to get valid value. */
            f16SpeedCmd = MLIB_MulSat_F16(M1_N_NOM, f16Dir);
        }

        /* The command is valid. */
        bQtyCmdValid = TRUE;
    }

    /* Check the application and quantity command. */
    if(TRUE == FOC_CheckAppCmd_BL(eAppCmd, bQtyCmdValid))
    {
        /* Multiply the direction of command with the actual requried speed. The direction sign */
        f16Dir = MLIB_MulSat_F16(f16Dir,
             MLIB_Sign_F16(s_sM1FOC.sCmd.sSpdFOC.sHiSpd.f16SpeedCmd));

        /* Both the the application and quantity command are valid.
           Check for following conditions to determine whether action is needed:
           - The MC.SM RUN was NOT requested.
           - The speed command direction is opposite to the actual speed command.
           - The control mode is NOT speed FOC. */
        if((kMC_ReqRun != s_sM1FOC.sStatusFOC.eRequest) || (0 > f16Dir) ||
           (kFOC_CtrlMode_SpeedCL != s_sM1FOC.sCmd.eCtrlMode))
        {
            /* Issue closed-loop speed control action. */
            s_sM1FOC.sCmd.eAction = kFOC_Action_ToSpeedCL;
        }
    }

    /* Set value of checked speed command to the FOC. */
    /*! @note To avoid timing issues, make sure, that the FOC action is
              asserted prior to the quantity command. */
    s_sM1FOC.sCmd.sSpdFOC.sHiSpd.f16SpeedCmd = f16SpeedCmd;

    /* Return the command validity flag. */
    return bQtyCmdValid;
}

/*!
 * Function checks the application and current commands and sets the
 * current closed-loop control mode if possible.
 */
bool_t FOC_SetCtrlCurrCL_BL(foc_app_cmd_t eAppCmd, GMCLIB_2COOR_DQ_T_F16 sIDQcmd)
{
    frac16_t  f16IqLim;     /* Q-axis current limit. */
    bool_t    bQtyCmdValid; /* Quantity validity flag. */

    /* Check whether the current command is valid (non-zero). */
    if(0U != ((uint16_t)sIDQcmd.f16Q | (uint16_t)sIDQcmd.f16D))
    {
        /* Limit the direct-axis current. */
        sIDQcmd.f16D = GFLIB_Limit_F16(sIDQcmd.f16D,
            MLIB_NegSat_F16(s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.f16UpperLim),
                            s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.f16UpperLim);

        /* Calculate the Q-axis current limit. */
        f16IqLim = GFLIB_Sqrt_F16(MLIB_Sub_F16(
            MLIB_Mul_F16(s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.f16UpperLim,
                         s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.f16UpperLim),
            MLIB_Mul_F16(sIDQcmd.f16D, sIDQcmd.f16D)));

        /* Limit the Q-axis current. */
        sIDQcmd.f16Q = GFLIB_Limit_F16(sIDQcmd.f16Q, MLIB_Neg_F16(f16IqLim), f16IqLim);

        /* The command is valid. */
        bQtyCmdValid = TRUE;
    }
    else
    {
        /* The command is not valid. */
        bQtyCmdValid = FALSE;

        /* Reset the required values. */
        sIDQcmd.f16D = 0;
        sIDQcmd.f16Q = 0;
    }

    /* Check the application and quantity command. */
    if(TRUE == FOC_CheckAppCmd_BL(eAppCmd, bQtyCmdValid))
    {
        /* Both the the application and quantity commands are valid.
           Check for following conditions to determine whether action is needed:
           - The MC.SM RUN was NOT requested.
           - The control mode is NOT current FOC. */
        if((kMC_ReqRun != s_sM1FOC.sStatusFOC.eRequest) ||
           (kFOC_CtrlMode_CurrCL != s_sM1FOC.sCmd.eCtrlMode))
        {
            /* Issue closed-loop current control action. */
            s_sM1FOC.sCmd.eAction = kFOC_Action_ToCurrCL;
        }
        else
        {
            /* The current command is valid and we are running. */
            s_sM1FOC.sCtrl.sCurr.sIDQReq = s_sM1FOC.sCmd.sCurrFOC.sIDQReq;
        }
    }

    /* Set value of checked current command to the FOC. */
    /*! @note To avoid timing issues, make sure, that the FOC action is
              asserted prior to the quantity command. */
    s_sM1FOC.sCmd.sCurrFOC.sIDQReq = sIDQcmd;

    /* Return the command validity flag. */
    return bQtyCmdValid;
}


/*!
 * Function checks the application and frequency commands and sets the
 * scalar open-loop control mode if possible.
 */
bool_t FOC_SetCtrlScalarOL_BL(foc_app_cmd_t eAppCmd, frac16_t f16FrqElCmd)
{
    bool_t    bQtyCmdValid; /* Quantity validity flag. */

    /* Check whether the command is valid. */
    bQtyCmdValid = (bool_t)(int16_t)(0 != f16FrqElCmd);

    /* Check the application and quantity command. */
    if(TRUE == FOC_CheckAppCmd_BL(eAppCmd, bQtyCmdValid))
    {
        /* Both the the application and quantity command are valid.
           Check for following conditions to determine whether action is needed:
           - The MC.SM RUN was NOT requested.
           - The control mode is NOT scalar. */
        if((kMC_ReqRun != s_sM1FOC.sStatusFOC.eRequest) ||
           (kFOC_CtrlMode_ScalarOL != s_sM1FOC.sCmd.eCtrlMode))
        {
            /* Issue open-loop scalar control action. */
            s_sM1FOC.sCmd.eAction = kFOC_Action_ToScalarOL;
        }
    }

    /* Set value of checked frequency command to the FOC. */
    /*! @note To avoid timing issues, make sure, that the FOC action is
              asserted prior to the quantity command. */
    s_sM1FOC.sCmd.sOL.f16FrqElCmd = f16FrqElCmd;

    /* Return the command validity flag. */
    return bQtyCmdValid;
}

/*!
 * Function checks the application and voltage commands and sets the
 * voltage open-loop control mode if possible.
 */
bool_t FOC_SetCtrlVoltOL_BL(foc_app_cmd_t eAppCmd, GMCLIB_2COOR_DQ_T_F16 sUDQCmd,
                            frac16_t f16FrqElCmd, frac16_t f16PosElCmd)
{
    bool_t    bQtyCmdValid; /* Quantity validity flag. */

    /* Check whether the command is valid. */
    bQtyCmdValid = (bool_t)(int16_t)(0U != ((uint16_t)sUDQCmd.f16D | (uint16_t)sUDQCmd.f16Q));

    /* Check the application and quantity command. */
    if(TRUE == FOC_CheckAppCmd_BL(eAppCmd, bQtyCmdValid))
    {
        /* Both the the application and quantity command are valid.
           Check for following conditions to determine whether action is needed:
           - The MC.SM RUN was NOT requested.
           - The control mode is NOT scalar. */
        if((kMC_ReqRun != s_sM1FOC.sStatusFOC.eRequest) ||
           (kFOC_CtrlMode_VoltOL != s_sM1FOC.sCmd.eCtrlMode))
        {
            /* Issue open-loop voltage control action. */
            s_sM1FOC.sCmd.eAction = kFOC_Action_ToVoltOL;
        }
    }

    /* Set value of checked voltage, position, and frequency command. */
    /*! @note To avoid timing issues, make sure, that the FOC action is
              asserted prior to the quantity command. */
    s_sM1FOC.sCmd.sOL.sUDQReq = sUDQCmd;
    s_sM1FOC.sCmd.sOL.f16FrqElCmd = f16FrqElCmd;
    s_sM1FOC.sCmd.sOL.f16PosElCmd = f16PosElCmd;

    /* Return the command validity flag. */
    return bQtyCmdValid;
}

/*!
 * Function checks the application and current commands and sets the
 * current open-loop control mode if possible.
 */
bool_t FOC_SetCtrlCurrOL_BL(foc_app_cmd_t eAppCmd, GMCLIB_2COOR_DQ_T_F16 sIDQCmd,
                         frac16_t f16FrqElCmd, frac16_t f16PosElCmd)
{
    bool_t    bQtyCmdValid; /* Quantity validity flag. */

    /* Check whether the command is valid. */
    bQtyCmdValid = (bool_t)(int16_t)(0U != ((uint16_t)sIDQCmd.f16D | (uint16_t)sIDQCmd.f16Q));

    /* Check the application command.  */
    if(TRUE == FOC_CheckAppCmd_BL(eAppCmd, bQtyCmdValid))
    {
        /* Both the the application and quantity command are valid.
           Check for following conditions to determine whether action is needed:
           - The MC.SM RUN was NOT requested.
           - The control mode is NOT scalar. */
        if((kMC_ReqRun != s_sM1FOC.sStatusFOC.eRequest) ||
           (kFOC_CtrlMode_CurrOL != s_sM1FOC.sCmd.eCtrlMode))
        {
            /* Issue open-loop current control action. */
            s_sM1FOC.sCmd.eAction = kFOC_Action_ToCurrOL;
        }
        else
        {
            /*  Apply the required currents. */
            s_sM1FOC.sCtrl.sCurr.sIDQReq = s_sM1FOC.sCmd.sOL.sIDQReq;
        }
    }

    /* Set value of checked current, position, and frequency command. */
    /*! @note To avoid timing issues, make sure, that the FOC action is
              asserted prior to the quantity command. */
    s_sM1FOC.sCmd.sOL.sIDQReq = sIDQCmd;
    s_sM1FOC.sCmd.sOL.f16FrqElCmd = f16FrqElCmd;
    s_sM1FOC.sCmd.sOL.f16PosElCmd = f16PosElCmd;

    /* Return the command validity flag. */
    return bQtyCmdValid;
}

/*!
 * Function sets the MID control mode if possible.
 */
void FOC_SetCtrlMID_FL(foc_app_cmd_t eAppCmd,
                         GMCLIB_2COOR_DQ_T_F16 sDQCmd,
                         frac16_t f16FrqElCmd,
                         frac16_t f16PosElCmd,
                         foc_ctrl_mode_cfg_t sCfg)
{
    /* Check whether the RUN application command was set. */
    if(kFOC_Cmd_Run == eAppCmd)
    {
        /* Both the the application and quantity commands are valid.
           Check for following conditions to determine whether action is needed:
           - The MC.SM RUN was NOT requested.
           - The control mode is NOT the MID. */
        if((kMC_ReqRun != s_sM1FOC.sStatusFOC.eRequest) ||
           (kFOC_CtrlMode_MID != s_sM1FOC.sCmd.eCtrlMode))
        {
            /* Issue the MID control action. */
            s_sM1FOC.sCmd.eAction = kFOC_Action_ToMID;
        }
        else
        {
            /*  Apply the required currents. */
            s_sM1FOC.sCmd.sOL.sIDQReq = sDQCmd;
            s_sM1FOC.sCmd.sOL.sUDQReq = sDQCmd;
        }
    }
    else
    {
        /* Issue STOP action. */
        s_sM1FOC.sCmd.eAction = kFOC_Action_ToStop;
    }

    s_sM1FOC.sCmd.sOL.f16FrqElCmd = f16FrqElCmd;
    s_sM1FOC.sCmd.sOL.f16PosElCmd = f16PosElCmd;
    s_sM1FOC.sCmd.sOL.sCfg = sCfg;
}


/*                           CONTROL FUNCTIONS                                */


/*!
 * Function executes the open-loop stator current, voltage or scalar-voltage
 * control.
 */
static GMCLIB_2COOR_ALBE_T_F16 FOC_CtrlOpenLoop_FL(void)
{
    frac16_t                   f16PosEl; /* The actually used position. */
    foc_ctrl_mode_cfg_t        sCfg;     /* The control mode configuration. */

    /* Check whether the MID control mode is active. */
    if(kFOC_CtrlMode_MID == s_sM1FOC.sCmd.eCtrlMode)
    {
        /* Assign the user control configuration directly. */
        sCfg = s_sM1FOC.sCmd.sOL.sCfg;

        /* Assign the required currents. */
        s_sM1FOC.sCtrl.sCurr.sIDQReq = s_sM1FOC.sCmd.sOL.sIDQReq;
    }
    else
    {
        /* Check whether the current control mode is active. */
        sCfg.bCurrCtrl = (uint16_t)(int16_t)(kFOC_CtrlMode_MID == s_sM1FOC.sCmd.eCtrlMode);

        /* Use the open-loop only. */
        sCfg.bCloseLoop = 0U;
    }

    /* Check the open-loop control should be used. */
    if(0U == sCfg.bCloseLoop)
    {
        /* Ramp the required frequency command value. */
        s_sM1FOC.sCtrl.sOL.f16FrqElRamp = MLIB_Conv_F16l(GFLIB_Ramp_F32(
            MLIB_Conv_F32s(s_sM1FOC.sCmd.sOL.f16FrqElCmd),
            &s_sM1FOC.sCtrl.sOL.sFrqElRampPar));

        /* Integrate the required frequency into the position. */
        f16PosEl = GFLIB_Integrator_F16(s_sM1FOC.sCtrl.sOL.f16FrqElRamp,
                                       &s_sM1FOC.sCtrl.sOL.sFrqElIntPar);
    }
    else
    {
        /* Use the MC.SM electrical position. */
        f16PosEl = s_sM1FOC.sMeas.f16PosEl;
    }

    /* Add the position shift. */
    s_sM1FOC.sCtrl.sOL.f16PosEl = MLIB_Add_F16(f16PosEl, s_sM1FOC.sCmd.sOL.f16PosElCmd);

    /* Calculate the sine and cosine of the generated rotor angle.  */
    s_sM1FOC.sCtrl.sPosElSC.f16Sin = GFLIB_Sin_F16(s_sM1FOC.sCtrl.sOL.f16PosEl);
    s_sM1FOC.sCtrl.sPosElSC.f16Cos = GFLIB_Cos_F16(s_sM1FOC.sCtrl.sOL.f16PosEl);

    /* Get the DQ-axis currents. */
    GMCLIB_Park_F16(&s_sM1FOC.sMeas.sIAlBe,
                    &s_sM1FOC.sCtrl.sPosElSC,
                    &s_sM1FOC.sCtrl.sCurr.sIDQ);

    /* Check whether the current control is active. */
    if(1U == sCfg.bCurrCtrl)
    {
        /* D-current error calculation */
        s_sM1FOC.sCtrl.sCurr.sIDQErr.f16D = MLIB_SubSat_F16(
            s_sM1FOC.sCtrl.sCurr.sIDQReq.f16D, s_sM1FOC.sCtrl.sCurr.sIDQ.f16D);

        /* Q-current error calculation */
        s_sM1FOC.sCtrl.sCurr.sIDQErr.f16Q = MLIB_SubSat_F16(
            s_sM1FOC.sCtrl.sCurr.sIDQReq.f16Q, s_sM1FOC.sCtrl.sCurr.sIDQ.f16Q);
    }
    else
    {
        /* Clear the control error. */
        s_sM1FOC.sCtrl.sCurr.sIDQErr.f16D = 0;
        s_sM1FOC.sCtrl.sCurr.sIDQErr.f16Q = 0;

        /* The scalar control is active. */
        if(kFOC_CtrlMode_ScalarOL == s_sM1FOC.sCmd.eCtrlMode)
        {
            /* Set the required voltage into controller accumulator.
               This will ensure the limitation. */
            s_sM1FOC.sCtrl.sCurr.sIdPiPar.f32IAccK_1 = 0L;
            s_sM1FOC.sCtrl.sCurr.sIqPiPar.f32IAccK_1 = MLIB_Conv_F32s(
                MLIB_MulRndSat_F16as(s_sM1FOC.sCtrl.sOL.a32VHzGain,
                s_sM1FOC.sCtrl.sOL.f16FrqElRamp));
        }
        else
        {
            /* Set the required voltage into controller accumulator.
               This will ensure the stator voltage vector limitation. */
            s_sM1FOC.sCtrl.sCurr.sIdPiPar.f32IAccK_1 =
                MLIB_Conv_F32s(s_sM1FOC.sCmd.sOL.sUDQReq.f16D);
            s_sM1FOC.sCtrl.sCurr.sIqPiPar.f32IAccK_1 =
                MLIB_Conv_F32s(s_sM1FOC.sCmd.sOL.sUDQReq.f16Q);
        }
    }

    /* D-axis current controller output limitation calculation. */
    s_sM1FOC.sCtrl.sCurr.sIdPiPar.f16UpperLim = MLIB_Mul_F16(
        s_sM1FOC.sCtrl.sVolt.f16DutyCycleLimit, s_sM1FOC.sMeas.f16U_DCBFilt);
    s_sM1FOC.sCtrl.sCurr.sIdPiPar.f16LowerLim = MLIB_Neg_F16(
        s_sM1FOC.sCtrl.sCurr.sIdPiPar.f16UpperLim);

    /* D-axis current PI controller. */
    s_sM1FOC.sCtrl.sVolt.sUDQReq.f16D = GFLIB_CtrlPIpAW_F16(
        s_sM1FOC.sCtrl.sCurr.sIDQErr.f16D,
        &s_sM1FOC.sCtrl.sCurr.bIdPiStopInteg,
        &s_sM1FOC.sCtrl.sCurr.sIdPiPar);

    /* Q-axis current controller output limitation calculation. */
    s_sM1FOC.sCtrl.sCurr.sIqPiPar.f16UpperLim = GFLIB_Sqrt_F16(
        MLIB_Sub_F16(MLIB_Mul_F16(s_sM1FOC.sCtrl.sCurr.sIdPiPar.f16UpperLim,
                                  s_sM1FOC.sCtrl.sCurr.sIdPiPar.f16UpperLim),
                     MLIB_Mul_F16(s_sM1FOC.sCtrl.sVolt.sUDQReq.f16D,
                                  s_sM1FOC.sCtrl.sVolt.sUDQReq.f16D)));
    s_sM1FOC.sCtrl.sCurr.sIqPiPar.f16LowerLim = MLIB_Neg_F16(
        s_sM1FOC.sCtrl.sCurr.sIqPiPar.f16UpperLim);

    /* Q-axis current PI controller. */
    s_sM1FOC.sCtrl.sVolt.sUDQReq.f16Q = GFLIB_CtrlPIpAW_F16(
        s_sM1FOC.sCtrl.sCurr.sIDQErr.f16Q,
        &s_sM1FOC.sCtrl.sCurr.bIqPiStopInteg,
        &s_sM1FOC.sCtrl.sCurr.sIqPiPar);

    /* Transform the required stator voltage to the stator reference frame. */
    GMCLIB_ParkInv_F16(&s_sM1FOC.sCtrl.sVolt.sUDQReq,
                       &s_sM1FOC.sCtrl.sPosElSC,
                       &s_sM1FOC.sCtrl.sVolt.sUAlBeReq);

    /* Pass the required stator voltage to the MC.SM state-machine. */
    return s_sM1FOC.sCtrl.sVolt.sUAlBeReq;
}

/*!
 * Function executes the stator-voltage control.
 */
static GMCLIB_2COOR_ALBE_T_F16 FOC_CtrlVolt_FL(void)
{
    /* Calculate the sine and cosine of the actual rotor angle.  */
    s_sM1FOC.sCtrl.sPosElSC.f16Sin = GFLIB_Sin_F16(s_sM1FOC.sMeas.f16PosEl);
    s_sM1FOC.sCtrl.sPosElSC.f16Cos = GFLIB_Cos_F16(s_sM1FOC.sMeas.f16PosEl);

    /* Get the DQ-axis currents. */
    GMCLIB_Park_F16(&s_sM1FOC.sMeas.sIAlBe,
                    &s_sM1FOC.sCtrl.sPosElSC,
                    &s_sM1FOC.sCtrl.sCurr.sIDQ);

    /* Transform the required stator voltage to the stator reference frame. */
    GMCLIB_ParkInv_F16(&s_sM1FOC.sCtrl.sVolt.sUDQReq,
                       &s_sM1FOC.sCtrl.sPosElSC,
                       &s_sM1FOC.sCtrl.sVolt.sUAlBeReq);

    /* Pass the required stator voltage to the MC.SM state-machine. */
    return s_sM1FOC.sCtrl.sVolt.sUAlBeReq;
}

/*!
 * Function executes the closed-loop stator-current control.
 */
static GMCLIB_2COOR_ALBE_T_F16 FOC_CtrlCurr_FL(void)
{
    /* Calculate the sine and cosine of the actual rotor angle.  */
    s_sM1FOC.sCtrl.sPosElSC.f16Sin = GFLIB_Sin_F16(s_sM1FOC.sMeas.f16PosEl);
    s_sM1FOC.sCtrl.sPosElSC.f16Cos = GFLIB_Cos_F16(s_sM1FOC.sMeas.f16PosEl);

    /* Get the DQ-axis currents. */
    GMCLIB_Park_F16(&s_sM1FOC.sMeas.sIAlBe,
                    &s_sM1FOC.sCtrl.sPosElSC,
                    &s_sM1FOC.sCtrl.sCurr.sIDQ);

    /* D-axis current error calculation */
    s_sM1FOC.sCtrl.sCurr.sIDQErr.f16D =  MLIB_SubSat_F16(
        s_sM1FOC.sCtrl.sCurr.sIDQReq.f16D, s_sM1FOC.sCtrl.sCurr.sIDQ.f16D);

    /* Q-axis current error calculation */
    s_sM1FOC.sCtrl.sCurr.sIDQErr.f16Q = MLIB_SubSat_F16(
        s_sM1FOC.sCtrl.sCurr.sIDQReq.f16Q, s_sM1FOC.sCtrl.sCurr.sIDQ.f16Q);

    /* D-axis controller limitation calculation. */
    s_sM1FOC.sCtrl.sCurr.sIdPiPar.f16UpperLim = MLIB_Mul_F16(
        s_sM1FOC.sCtrl.sVolt.f16DutyCycleLimit, s_sM1FOC.sMeas.f16U_DCBFilt);
    s_sM1FOC.sCtrl.sCurr.sIdPiPar.f16LowerLim = MLIB_Neg_F16(
        s_sM1FOC.sCtrl.sCurr.sIdPiPar.f16UpperLim);

    /* D-axis current PI controller */
    s_sM1FOC.sCtrl.sVolt.sUDQReq.f16D = GFLIB_CtrlPIpAW_F16(
        s_sM1FOC.sCtrl.sCurr.sIDQErr.f16D,
        &s_sM1FOC.sCtrl.sCurr.bIdPiStopInteg,
        &s_sM1FOC.sCtrl.sCurr.sIdPiPar);

    /* Q-axis controller limitation calculation. */
    s_sM1FOC.sCtrl.sCurr.sIqPiPar.f16UpperLim = GFLIB_Sqrt_F16(
        MLIB_Sub_F16(MLIB_Mul_F16(s_sM1FOC.sCtrl.sCurr.sIdPiPar.f16UpperLim,
                                  s_sM1FOC.sCtrl.sCurr.sIdPiPar.f16UpperLim),
                     MLIB_Mul_F16(s_sM1FOC.sCtrl.sVolt.sUDQReq.f16D,
                                  s_sM1FOC.sCtrl.sVolt.sUDQReq.f16D)));
    s_sM1FOC.sCtrl.sCurr.sIqPiPar.f16LowerLim = MLIB_Neg_F16(
        s_sM1FOC.sCtrl.sCurr.sIqPiPar.f16UpperLim);

    /* Q-axis current PI controller */
    s_sM1FOC.sCtrl.sVolt.sUDQReq.f16Q = GFLIB_CtrlPIpAW_F16(
        s_sM1FOC.sCtrl.sCurr.sIDQErr.f16Q,
        &s_sM1FOC.sCtrl.sCurr.bIqPiStopInteg,
        &s_sM1FOC.sCtrl.sCurr.sIqPiPar);

    /* Transform the required stator voltage to the stator reference frame. */
    GMCLIB_ParkInv_F16(&s_sM1FOC.sCtrl.sVolt.sUDQReq,
                       &s_sM1FOC.sCtrl.sPosElSC,
                       &s_sM1FOC.sCtrl.sVolt.sUAlBeReq);

    /* Pass the required stator voltage to the MC.SM state-machine. */
    return s_sM1FOC.sCtrl.sVolt.sUAlBeReq;
}

/*!
 * Function executes the closed-loop speed control.
 */
static frac16_t FOC_CtrlSpeed_SL(void)
{
    /* Ramp the required electrical speed. */
    s_sM1FOC.sCtrl.sSpd.f16SpdElRamp = MLIB_Conv_F16l(GFLIB_Ramp_F32(
        MLIB_Conv_F32s(s_sM1FOC.sCmd.sSpdFOC.sHiSpd.f16SpeedCmd),
                       &s_sM1FOC.sCtrl.sSpd.sSpdElRampPar));

    /* Check whether the actual speed is below the speed command. */
    if(MLIB_AbsSat_F16(s_sM1FOC.sCtrl.sSpd.f16SpdElRamp) >=
       MLIB_AbsSat_F16(s_sM1FOC.sCtrl.sSpd.f16SpdElFilt))
    {
        /* Check whether both the speed and current PI controllers were
           saturated. If so, the speed control loop is over-loaded. */
        s_sM1FOC.sCtrl.sSpd.bSpdElPiStopInteg =
            s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.bLimFlag |
            s_sM1FOC.sCtrl.sCurr.sIqPiPar.bLimFlag;
    }
    else
    {
        /* The speed PI controller is not saturated. */
        s_sM1FOC.sCtrl.sSpd.bSpdElPiStopInteg = FALSE;
    }

    /* Calculate the speed control error. */
    s_sM1FOC.sCtrl.sSpd.f16SpdElErr = MLIB_SubSat_F16(
        s_sM1FOC.sCtrl.sSpd.f16SpdElRamp, s_sM1FOC.sCtrl.sSpd.f16SpdElFilt);

    /* Calculate the required Q-axis current. */
    return GFLIB_CtrlPIpAW_F16(s_sM1FOC.sCtrl.sSpd.f16SpdElErr,
                              &s_sM1FOC.sCtrl.sSpd.bSpdElPiStopInteg,
                              &s_sM1FOC.sCtrl.sSpd.sSpdElPiPar);
}


/*                     FAST-LOOP CONTROL ROUTINES                             */
/*!
 * FOC initialization routine, which is called during the MCU initialization
 * phase by MC.SM state-machine.
 */
void FOC_Init_AR(void)
{
    /*! The open-loop control parameters. */
    s_sM1FOC.sCtrl.sOL.a32VHzGain = M1_SCALAR_VHZ_FACTOR_GAIN;
    s_sM1FOC.sCtrl.sOL.sFrqElRampPar.f32RampUp   = M1_SCALAR_RAMP_UP;
    s_sM1FOC.sCtrl.sOL.sFrqElRampPar.f32RampDown = M1_SCALAR_RAMP_DOWN;
    s_sM1FOC.sCtrl.sOL.sFrqElIntPar.a32Gain = M1_SCALAR_INTEG_GAIN;

    /*! The voltage control parameters. */
    s_sM1FOC.sCtrl.sVolt.f16DutyCycleLimit =  M1_CLOOP_LIMIT;

    /*! The current control loop. */
    /* Direct-axis current controller parameter initialization. */
    s_sM1FOC.sCtrl.sCurr.sIdPiPar.a32PGain    = M1_D_KP_GAIN;
    s_sM1FOC.sCtrl.sCurr.sIdPiPar.a32IGain    = M1_D_KI_GAIN;

    /* Quadrature-axis current controller parameter initialization. */
    s_sM1FOC.sCtrl.sCurr.sIqPiPar.a32PGain    = M1_Q_KP_GAIN;
    s_sM1FOC.sCtrl.sCurr.sIqPiPar.a32IGain    = M1_Q_KI_GAIN;

    /*! The speed control loop. */
    /* Measured speed filter. */
    s_sM1FOC.sCtrl.sSpd.sSpdElFiltPar.sFltCoeff.f32B0 = M1_TO_SPEED_IIR_B0;
    s_sM1FOC.sCtrl.sSpd.sSpdElFiltPar.sFltCoeff.f32B1 = M1_TO_SPEED_IIR_B1;
    s_sM1FOC.sCtrl.sSpd.sSpdElFiltPar.sFltCoeff.f32A1 = M1_TO_SPEED_IIR_A1;

    /* Rotor electrical speed controller parameter initialization. */
    s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.f16UpperLim = M1_SPEED_LOOP_HIGH_LIMIT;
    s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.f16LowerLim = M1_SPEED_LOOP_LOW_LIMIT;
    s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.a32PGain    = M1_SPEED_PI_PROP_GAIN;
    s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.a32IGain    = M1_SPEED_PI_INTEG_GAIN;

    /* Required speed ramp parameter initialization. */
    s_sM1FOC.sCtrl.sSpd.sSpdElRampPar.f32RampUp   = M1_SPEED_RAMP_UP;
    s_sM1FOC.sCtrl.sSpd.sSpdElRampPar.f32RampDown = M1_SPEED_RAMP_DOWN;


    /*! Initialize commands - speed FOC. */
    s_sM1FOC.sCmd.eCtrlMode = kFOC_CtrlMode_SpeedCL;
    s_sM1FOC.sCmd.eAction = kFOC_Action_NoAction;

    /*! Closed-loop speed control mode. */
    /* Alignment voltage (voltage control only). */
    s_sM1FOC.sCmd.sSpdFOC.sAlign.sUDQReq.f16D = M1_ALIGN_VOLTAGE;
    s_sM1FOC.sCmd.sSpdFOC.sAlign.sUDQReq.f16Q = FRAC16(0.0);

    /* Low- and mid-speed required current (current control only). */
    s_sM1FOC.sCmd.sSpdFOC.sLoMiSpd.sIDQReq.f16D = FRAC16(0.0);
    s_sM1FOC.sCmd.sSpdFOC.sLoMiSpd.sIDQReq.f16Q = M1_OL_START_I;

    /* Clear the speed command and intended direction. */
    s_sM1FOC.sCmd.sSpdFOC.sHiSpd.f16SpeedCmd = 0;
    s_sM1FOC.sCmd.f16Direction = FRAC16(1.0);

    /*! Closed-loop current control mode. */
    /* The LO_SPD, MI_SPD and HI_SPD state required current. */
    s_sM1FOC.sCmd.sCurrFOC.sIDQReq.f16D = 0;
    s_sM1FOC.sCmd.sCurrFOC.sIDQReq.f16Q = 0;

    /*! Open-loop control mode. */
    /* The scalar speed command. */
    s_sM1FOC.sCmd.sOL.f16FrqElCmd = 0;
    s_sM1FOC.sCmd.sOL.f16PosElCmd = 0;
    s_sM1FOC.sCmd.sOL.sIDQReq.f16D = 0;
    s_sM1FOC.sCmd.sOL.sIDQReq.f16Q = 0;
    s_sM1FOC.sCmd.sOL.sUDQReq.f16D = 0;
    s_sM1FOC.sCmd.sOL.sUDQReq.f16Q = 0;
    s_sM1FOC.sCmd.sOL.sCfg.bCurrCtrl = 0U;
    s_sM1FOC.sCmd.sOL.sCfg.bCloseLoop = 0U;


    /*! Quantity scales. */
    s_sM1FOC.sScale.fltVoltage    = M1_U_MAX;
    s_sM1FOC.sScale.fltDCBVoltage = M1_U_DCB_MAX;
    s_sM1FOC.sScale.fltCurrent    = M1_I_MAX;
    s_sM1FOC.sScale.fltFrequency  = M1_FREQ_MAX;
    s_sM1FOC.sScale.fltSpeedMe    = M1_N_MAX;
    s_sM1FOC.sScale.fltBemf       = M1_E_MAX;
    s_sM1FOC.sScale.fltFastSamplPeriod = M1_CLOOP_SAMPLE_TIME;
    s_sM1FOC.sScale.fltSlowSamplPeriod = M1_SLOOP_SAMPLE_TIME;

    /* Set the initial request to STOP. */
    s_sM1FOC.sStatusFOC.eRequest = kMC_ReqStop;
}

/*!
 * FOC state variable reset routine.
 */
void FOC_ClearStates_FL(void)
{
    /*! The open-loop voltage and current control. */
    s_sM1FOC.sCtrl.sOL.f16FrqElRamp = 0;
    s_sM1FOC.sCtrl.sOL.f16PosEl = 0;
    s_sM1FOC.sCtrl.sOL.sFrqElIntPar.f16InValK_1 = 0;
    s_sM1FOC.sCtrl.sOL.sFrqElIntPar.f32IAccK_1 = 0L;
    s_sM1FOC.sCtrl.sOL.sFrqElRampPar.f32State  = 0L;

    /*! The voltage control. */
    s_sM1FOC.sCtrl.sVolt.sUDQReq.f16D = 0;
    s_sM1FOC.sCtrl.sVolt.sUDQReq.f16Q = 0;
    s_sM1FOC.sCtrl.sVolt.sUAlBeReq.f16Alpha = 0;
    s_sM1FOC.sCtrl.sVolt.sUAlBeReq.f16Beta  = 0;

    /*! The current control loop. */
    s_sM1FOC.sCtrl.sCurr.sIDQReq.f16D = 0;
    s_sM1FOC.sCtrl.sCurr.sIDQReq.f16Q = 0;
    s_sM1FOC.sCtrl.sCurr.sIDQErr.f16D = 0;
    s_sM1FOC.sCtrl.sCurr.sIDQErr.f16Q = 0;
    s_sM1FOC.sCtrl.sCurr.sIdPiPar.bLimFlag = FALSE;
    s_sM1FOC.sCtrl.sCurr.sIqPiPar.bLimFlag = FALSE;
    s_sM1FOC.sCtrl.sCurr.sIdPiPar.f32IAccK_1  = 0L;
    s_sM1FOC.sCtrl.sCurr.sIdPiPar.f16InErrK_1 = 0;
    s_sM1FOC.sCtrl.sCurr.sIqPiPar.f32IAccK_1  = 0L;
    s_sM1FOC.sCtrl.sCurr.sIqPiPar.f16InErrK_1 = 0;
    s_sM1FOC.sCtrl.sCurr.bIdPiStopInteg = FALSE;
    s_sM1FOC.sCtrl.sCurr.bIqPiStopInteg = FALSE;

    /*! The speed control loop. */
    s_sM1FOC.sCtrl.sSpd.sSpdElRampPar.f32State = 0L;
    s_sM1FOC.sCtrl.sSpd.f16SpdElFilt = 0;
    s_sM1FOC.sCtrl.sSpd.f16SpdElErr  = 0;
    s_sM1FOC.sCtrl.sSpd.f16SpdElRamp = 0;
    s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.f32IAccK_1 = 0L;
    s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.bLimFlag = FALSE;
    s_sM1FOC.sCtrl.sSpd.bSpdElPiStopInteg = FALSE;
    GDFLIB_FilterIIR1Init_F16(&s_sM1FOC.sCtrl.sSpd.sSpdElFiltPar);

    /* Clear the flags, which are passed to MC.SM. */
    s_sM1FOC.sStatusFOC.ui16Flags = 0U;
}

/*!
 * FOC idle routine. No control action is generated.
 */
void FOC_RoutineIdle_FL(const mc_foc_input_meas_t sMeas)
{
    /* Store the measurements. */
    s_sM1FOC.sMeas = sMeas;
}

/*!
 * FOC alignment state routine.
 */
GMCLIB_2COOR_ALBE_T_F16  FOC_RoutineAlign_FL(const mc_foc_input_meas_t sMeas)
{
    /* Store the measurements. */
    s_sM1FOC.sMeas = sMeas;

    /* Execute the selected control function. */
    return s_sM1FOC.sCtrlFcn.pfcnAlignFL();
}

/*!
 * FOC low-speed state routine.
 */
GMCLIB_2COOR_ALBE_T_F16  FOC_RoutineLoSpd_FL(const mc_foc_input_meas_t sMeas)
{
    /* Store the measurements. */
    s_sM1FOC.sMeas = sMeas;

    /* Execute the selected control function. */
    return s_sM1FOC.sCtrlFcn.pfcnLoSpdFL();
}

/*!
 * FOC medium-speed state routine.
 */
GMCLIB_2COOR_ALBE_T_F16  FOC_RoutineMiSpd_FL(const mc_foc_input_meas_t sMeas)
{
    /* Store the measurements. */
    s_sM1FOC.sMeas = sMeas;

    /* Execute the selected control function. */
    return s_sM1FOC.sCtrlFcn.pfcnMiSpdFL();
}

/*!
 * FOC high-speed state routine.
 */
GMCLIB_2COOR_ALBE_T_F16  FOC_RoutineHiSpd_FL(const mc_foc_input_meas_t sMeas)
{
    /* Store the measurements. */
    s_sM1FOC.sMeas = sMeas;

    /* Execute the selected control function. */
    return s_sM1FOC.sCtrlFcn.pfcnHiSpdFL();
}



/*                     SLOW-LOOP CONTROL ROUTINES                             */
/*!
 * FOC idle routine. No control action is generated.
 */
foc_status_t FOC_RoutineIdle_SL(const mc_status_t sMCStatus)
{
    mc_ctrl_request eNewSMRequest; /* A new MC.SM request. */

    /* Check whether new FOC action was registered. */
    if(kFOC_Action_NoAction != s_sM1FOC.sCmd.eAction)
    {
        /* Check whether STOP action was issued. */
        if(kFOC_Action_ToStop == s_sM1FOC.sCmd.eAction)
        {
            /* Request SM.MC to enter STOP state. */
            eNewSMRequest = kMC_ReqStop;
        }
        /* Check whether READY action was issued.. */
        else if(kFOC_Action_ToReady == s_sM1FOC.sCmd.eAction)
        {
            /* Request SM.MC to enter READY state. */
            eNewSMRequest = kMC_ReqReady;
        }
        /* Check whether RUN action (new control mode) was issued. */
        else
        {
            /* Configure the required control mode. */
            s_sM1FOC.sCmd.eCtrlMode = (foc_ctrl_mode_t)((uint16_t)s_sM1FOC.sCmd.eAction - 3U);

            /* Request SM.MC to enter RUN state. */
            eNewSMRequest = kMC_ReqRun;
        }

        /* Enter new MC.SM request. */
        s_sM1FOC.sStatusFOC.eRequest = eNewSMRequest;

        /* The action was registered. Clear it. */
        s_sM1FOC.sCmd.eAction = kFOC_Action_NoAction;
    }

    /* Store the last known MC.SM status. */
    s_sM1FOC.sStatusMC = sMCStatus;

    /* Return the FOC status/requests. */
    return s_sM1FOC.sStatusFOC;
}

/*!
 * FOC alignment state routine.
 */
foc_status_t  FOC_RoutineAlign_SL(const mc_status_t sMCStatus)
{
    /* Check whether new action was registered. */
    if(kFOC_Action_NoAction != s_sM1FOC.sCmd.eAction)
    {
        /* Request to go to STOP state so new action can be handled. */
        s_sM1FOC.sStatusFOC.eRequest = kMC_ReqStop;
    }

    /* Store the last known MC.SM status. */
    s_sM1FOC.sStatusMC = sMCStatus;

    /* Return the FOC status/requests. */
    return s_sM1FOC.sStatusFOC;
}

/*!
 * FOC low-speed state routine.
 */
foc_status_t  FOC_RoutineLoSpd_SL(const mc_status_t sMCStatus)
{
    /* Check whether new action was registered. */
    if(kFOC_Action_NoAction == s_sM1FOC.sCmd.eAction)
    {
        /* Filter the speed. */
        s_sM1FOC.sCtrl.sSpd.f16SpdElFilt =
            GDFLIB_FilterIIR1_F16(s_sM1FOC.sMeas.f16SpdEl,
                                 &s_sM1FOC.sCtrl.sSpd.sSpdElFiltPar);
    }
    else
    {
        /* Request to go to STOP state so new action can be handled. */
        s_sM1FOC.sStatusFOC.eRequest = kMC_ReqStop;
    }

    /* Store the last known MC.SM status. */
    s_sM1FOC.sStatusMC = sMCStatus;

    /* Return the FOC status/requests. */
    return s_sM1FOC.sStatusFOC;
}

/*!
 * FOC mid-speed state routine.
 */
foc_status_t  FOC_RoutineMiSpd_SL(const mc_status_t sMCStatus)
{
    /* Check whether new action was registered. */
    if(kFOC_Action_NoAction == s_sM1FOC.sCmd.eAction)
    {
        /* Filter the speed. */
        s_sM1FOC.sCtrl.sSpd.f16SpdElFilt =
            GDFLIB_FilterIIR1_F16(s_sM1FOC.sMeas.f16SpdEl,
                                 &s_sM1FOC.sCtrl.sSpd.sSpdElFiltPar);
    }
    else
    {
        /* Request to go to STOP state so new action can be handled. */
        s_sM1FOC.sStatusFOC.eRequest = kMC_ReqStop;
    }

    /* Store the last known MC.SM status. */
    s_sM1FOC.sStatusMC = sMCStatus;

    /* Return the FOC status/requests. */
    return s_sM1FOC.sStatusFOC;
}

/*!
 * FOC high-speed state routine.
 */
foc_status_t  FOC_RoutineHiSpd_SL(const mc_status_t sMCStatus)
{
    /* Check whether new action was registered. */
    if(kFOC_Action_NoAction == s_sM1FOC.sCmd.eAction)
    {
        /* Filter the speed. */
        s_sM1FOC.sCtrl.sSpd.f16SpdElFilt =
            GDFLIB_FilterIIR1_F16(s_sM1FOC.sMeas.f16SpdEl,
                                 &s_sM1FOC.sCtrl.sSpd.sSpdElFiltPar);

        /* Check whether we are in speed FOC control mode. */
        if(kFOC_CtrlMode_SpeedCL == s_sM1FOC.sCmd.eCtrlMode)
        {
            /* Execute the speed controller. */
            s_sM1FOC.sCtrl.sCurr.sIDQReq.f16Q = FOC_CtrlSpeed_SL();

            /* Clear the saturation flag. */
            s_sM1FOC.sStatusFOC.ui16Flags &= (uint16_t)(~FOC_SPD_CTRL_SATURATED_MASK);

            /* Set the saturation flag. */
            s_sM1FOC.sStatusFOC.ui16Flags |= (uint16_t)
                (((uint16_t)s_sM1FOC.sCtrl.sSpd.bSpdElPiStopInteg) << FOC_SPD_CTRL_SATURATED);
        }
    }
    else
    {
        /* Request to go to STOP state so new action can be handled. */
        s_sM1FOC.sStatusFOC.eRequest = kMC_ReqStop;
    }

    /* Store the last known MC.SM status. */
    s_sM1FOC.sStatusMC = sMCStatus;

    /* Return the FOC status/requests. */
    return s_sM1FOC.sStatusFOC;
}



/*                     CONTROL ROUTINE TRANSITIONS                            */
/*!
 * Transition to the idle FOC routine.
 */
void  FOC_TransToIdle_FL(void)
{
    /* Clear the internal FOC states. */
    FOC_ClearStates_FL();
}

/*!
 * Transition to the align FOC routine.
 */
void  FOC_TransToAlign_FL(void)
{
    /* Check which control mode was activated. */
    if((kFOC_CtrlMode_SpeedCL == s_sM1FOC.sCmd.eCtrlMode) ||
       (kFOC_CtrlMode_CurrCL  == s_sM1FOC.sCmd.eCtrlMode))
    {
        /* Assign fast-loop control functions for speed and current FOC. */
        s_sM1FOC.sCtrlFcn.pfcnAlignFL = FOC_CtrlVolt_FL;
        s_sM1FOC.sCtrlFcn.pfcnLoSpdFL = FOC_CtrlCurr_FL;
        s_sM1FOC.sCtrlFcn.pfcnMiSpdFL = FOC_CtrlCurr_FL;
        s_sM1FOC.sCtrlFcn.pfcnHiSpdFL = FOC_CtrlCurr_FL;

        /* Set the required stator voltage for alignment. */
        s_sM1FOC.sCtrl.sVolt.sUDQReq = s_sM1FOC.sCmd.sSpdFOC.sAlign.sUDQReq;
    }
    else
    {
        /* Assign fast loop control functions for open-loop control. */
        s_sM1FOC.sCtrlFcn.pfcnAlignFL = FOC_CtrlOpenLoop_FL;
        s_sM1FOC.sCtrlFcn.pfcnLoSpdFL = FOC_CtrlOpenLoop_FL;
        s_sM1FOC.sCtrlFcn.pfcnMiSpdFL = FOC_CtrlOpenLoop_FL;
        s_sM1FOC.sCtrlFcn.pfcnHiSpdFL = FOC_CtrlOpenLoop_FL;

#if defined(DEBUG)
        /*! @note Fault detection can be selectively disabled only in debug
                  configuration. */
        /* Disable the under-speed, over-speed, and blocked-rotor faults. */
        s_sM1FOC.sStatusFOC.ui16Flags |= (uint16_t)(FOC_DBG_DIS_FAULT_OVERLOAD_MASK  |
                                                 FOC_DBG_DIS_FAULT_OVERSPEED_MASK |
                                                 FOC_DBG_DIS_FAULT_BLCKTOR_MASK);
#endif /* defined(DEBUG)*/
    }
}

/*!
 * Transition to the low-speed FOC routine.
 */
frac16_t  FOC_TransToLoSpd_FL(void)
{
    /* Check whether the closed-loop speed control mode was selected. */
    if(kFOC_CtrlMode_SpeedCL == s_sM1FOC.sCmd.eCtrlMode)
    {
        /* Get the direction of rotation. */
        s_sM1FOC.sCmd.f16Direction =
            MLIB_Sign_F16(s_sM1FOC.sCmd.sSpdFOC.sHiSpd.f16SpeedCmd);

        /* Set the required currents. */
        s_sM1FOC.sCtrl.sCurr.sIDQReq.f16D =
            s_sM1FOC.sCmd.sSpdFOC.sLoMiSpd.sIDQReq.f16D;
        s_sM1FOC.sCtrl.sCurr.sIDQReq.f16Q =
            MLIB_MulSat_F16(s_sM1FOC.sCmd.sSpdFOC.sLoMiSpd.sIDQReq.f16Q,
                            s_sM1FOC.sCmd.f16Direction);

        /* Initialize the current PI controller accumulators to the actual
           required stator voltage values. */
        s_sM1FOC.sCtrl.sCurr.sIdPiPar.f32IAccK_1 =
            MLIB_Conv_F32s(s_sM1FOC.sCtrl.sVolt.sUDQReq.f16D);
        s_sM1FOC.sCtrl.sCurr.sIqPiPar.f32IAccK_1 =
            MLIB_Conv_F32s(s_sM1FOC.sCtrl.sVolt.sUDQReq.f16Q);
    }
    /* Check whether the closed-loop current control mode was selected. */
    else if(kFOC_CtrlMode_CurrCL == s_sM1FOC.sCmd.eCtrlMode)
    {
        /* Get the direction of intended rotation. */
        s_sM1FOC.sCmd.f16Direction =
            MLIB_Sign_F16(s_sM1FOC.sCmd.sCurrFOC.sIDQReq.f16Q);

        /* Set the required currents. */
        s_sM1FOC.sCtrl.sCurr.sIDQReq = s_sM1FOC.sCmd.sCurrFOC.sIDQReq;

        /* Initialize the current PI controller accumulators to the actual
           required stator voltage values. */
        s_sM1FOC.sCtrl.sCurr.sIdPiPar.f32IAccK_1 =
            MLIB_Conv_F32s(s_sM1FOC.sCtrl.sVolt.sUDQReq.f16D);
        s_sM1FOC.sCtrl.sCurr.sIqPiPar.f32IAccK_1 =
            MLIB_Conv_F32s(s_sM1FOC.sCtrl.sVolt.sUDQReq.f16Q);
    }
    /* The open-loop control mode is selected. */
    else
    {
        /* Set the default positive direction of intended rotation.. */
        s_sM1FOC.sCmd.f16Direction = FRAC16(1.0);
    }

    /* Return the intended startup direction. */
    return s_sM1FOC.sCmd.f16Direction;
}

/*!
 * Transition to the medium-speed FOC routine.
 */
void  FOC_TransToMiSpd_FL(void)
{

}

/*!
 * Transition to the high-speed FOC routine.
 */
void  FOC_TransToHiSpd_FL(void)
{
    /* Check whether the closed-loop speed control mode was selected. */
    if(kFOC_CtrlMode_SpeedCL == s_sM1FOC.sCmd.eCtrlMode)
    {
        /* Initialize the speed PI controller to the actual required Q-axis
           current value. */
        s_sM1FOC.sCtrl.sSpd.sSpdElPiPar.f32IAccK_1 =
            MLIB_Conv_F32s(s_sM1FOC.sCtrl.sCurr.sIDQReq.f16Q);

        /* Initialize the speed ramp to the actual speed value. */
        s_sM1FOC.sCtrl.sSpd.sSpdElRampPar.f32State =
            MLIB_Conv_F32s(s_sM1FOC.sCtrl.sSpd.f16SpdElFilt);
    }
}



