/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _M1_STATE_ROUTINES_H_
#define _M1_STATE_ROUTINES_H_

#include "m1_pmsm_appconfig.h"

MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

#include "gflib.h"
#include "gmclib.h"
#include "amclib.h"

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */


/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* FOC status flags (passed to MC.SM via foc_status_t). */
#define FOC_DBG_DIS_FAULT_OVERLOAD         (3U) /* Number matches MC_FAULT_LOAD_OVER */
#define FOC_DBG_DIS_FAULT_OVERSPEED        (4U) /* Number matches MC_FAULT_SPEED_OVER */
#define FOC_DBG_DIS_FAULT_BLCKTOR          (5U) /* Number matches MC_FAULT_ROTOR_BLOCKED */
#define FOC_SPD_CTRL_SATURATED             (15U)

/* FOC status flag masks (passed to MC.SM via foc_status_t). */
#define FOC_SPD_CTRL_SATURATED_MASK        (1U << FOC_SPD_CTRL_SATURATED)
#define FOC_DBG_DIS_FAULT_OVERLOAD_MASK    (1U << FOC_DBG_DIS_FAULT_OVERLOAD)
#define FOC_DBG_DIS_FAULT_OVERSPEED_MASK   (1U << FOC_DBG_DIS_FAULT_OVERSPEED)
#define FOC_DBG_DIS_FAULT_BLCKTOR_MASK     (1U << FOC_DBG_DIS_FAULT_BLCKTOR)

/*                               MC.SM DATA TYPES                             */
/* The MC.SM state-machine states. */
typedef enum _mc_sm_state
{
    kMC_StNO_INIT = 0U,
    kMC_StFAULT   = 1U,
    kMC_StSTOP    = 2U,
    kMC_StCALIB   = 3U,
    kMC_StREADY   = 4U,
    kMC_StALIGN   = 5U,
    kMC_StLO_SPD  = 6U,
    kMC_StMI_SPD  = 7U,
    kMC_StHI_SPD  = 8U,
    kMC_StFREE    = 9U,
    kMC_StNumOfStates = 10U
} mc_sm_state_t;

/* The request from control routines (FOC) to the safe state-machine (MC.SM).
 * See table below for MC.SM behavior definition.
 *      +----------------------------------------------+
 *      ¦             ¦   The MC.SM state transition   ¦
 *      ¦-------------+--------------------------------¦
 *      ¦ MC.SM State ¦  ReqStop  ¦ ReqReady ¦  ReqRun ¦
 *      ¦-------------+-----------+----------+---------¦
 *      ¦   NO_INIT   ¦ T.INIT**  ¦ T.INIT** ¦ T.INIT**¦
 *      ¦   FAULT     ¦   T.S*    ¦   T.S*   ¦   T.S*  ¦
 *      ¦   STOP      ¦    -      ¦  T.S-C   ¦  T.S-C  ¦
 *      ¦   CALIB     ¦   T.S     ¦  T.C-R   ¦  T.C-R  ¦
 *      ¦   READY     ¦   T.S     ¦    -     ¦  T.R-A  ¦
 *      ¦   ALIGN     ¦   T.S     ¦   T.S    ¦  T.A-LS ¦
 *      ¦   LO_SPD    ¦  T.FREE   ¦  T.FREE  ¦    -    ¦
 *      ¦   MI_SPD    ¦  T.FREE   ¦  T.FREE  ¦    -    ¦
 *      ¦   HI_SPD    ¦  T.FREE   ¦  T.FREE  ¦    -    ¦
 *      ¦   FREE      ¦   T.S     ¦   T.S    ¦   T.S   ¦
 *      +----------------------------------------------+
 *      *)  No active fault must be detected
 *      **) Transition completed after the MC.SM initialization
 */
typedef enum _mc_ctrl_request
{
    kMC_ReqStop   = 0U,  /* STOP request. */
    kMC_ReqReady  = 1U,  /* READY request. */
    kMC_ReqRun    = 2U   /* START request. */
} mc_ctrl_request;

/* The measurements passed from MC.SM to the FOC. */
typedef struct _mc_foc_input_meas
{
    GMCLIB_2COOR_ALBE_T_F16  sIAlBe;       /* Measured stator currents. */
    frac16_t                 f16U_DCBFilt; /* DC-bus voltage. */
    frac16_t                 f16SpdEl;     /* Electrical rotor speed. */
    frac16_t                 f16PosEl;     /* Electrical rotor position. */
    frac16_t                 f16AUX;       /* Measured auxiliary quantity. */
} mc_foc_input_meas_t;

/* The MC.SM status passed to the FOC. */
typedef struct _mc_status
{
    mc_sm_state_t eState;        /* The MC.SM state. */
    uint16_t ui16FaultCaptured;  /* The captured fault code. */
    uint16_t ui16FaultPending;   /* The pending fault code. */
} mc_status_t;


/*                           FOC PUBLIC DATA TYPES                            */
/* The FOC application command. */
typedef enum _foc_app_cmd
{
    kFOC_Cmd_Stop  = 0U,  /* STOP command. */
    kFOC_Cmd_Ready = 1U,  /* READY command. */
    kFOC_Cmd_Run   = 2U   /* START command. */
} foc_app_cmd_t;

/* The FOC control modes. */
typedef enum _foc_ctrl_mode
{
    kFOC_CtrlMode_SpeedCL  = 0U,  /* Closed-loop speed vector control mode. */
    kFOC_CtrlMode_CurrCL   = 1U,  /* Closed-loop current vector control mode. */
    kFOC_CtrlMode_ScalarOL = 2U,  /* Open-loop V/Hz scalar voltage control mode. */
    kFOC_CtrlMode_CurrOL   = 3U,  /* Open-loop current control mode. */
    kFOC_CtrlMode_VoltOL   = 4U,  /* Open-loop voltage control mode. */
    kFOC_CtrlMode_MID      = 5U   /* The MID control mode. */
} foc_ctrl_mode_t;

/* The FOC module status and request to be passed to MC.SM. */
typedef struct _foc_status
{
    mc_ctrl_request eRequest;   /* MC.SM request. */
    uint16_t        ui16Flags;  /* Mixed flags. */
} foc_status_t;

/* The MID control mode configuration. */
typedef struct _foc_ctrl_mode_cfg
{
    uint32_t bCurrCtrl  : 1; /* The current control loop active when TRUE. */
    uint32_t bCloseLoop : 1; /* The MC.SM position is used when TRUE. */
} foc_ctrl_mode_cfg_t;


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
/*                         APPLICATION INTERFACE                              */
/*!
 * @brief   Function returns the MC.SM state machine status.
 *
 * @details The MC.SM status (actual state and fault codes) is read every slow
 *          control loop (SL) execution and this function can be used to
 *          retrieve the last know state.
 *
 * @param   void
 *
 * @return  The motor-control MC.SM state-machine status.
 */
mc_status_t FOC_GetMCStatus_SL(void);

/*!
 * @brief   Function returns measurements acquired by the motor-control MC.SM
 *          state-machine.
 *
 * @details The MC.SM provides the measurement every fast-control loop (FL)
 *          execution. The following information is provided:
 *          - Measured stator currents in stator axis.
 *          - DC-bus voltage.
 *          - Electrical rotor speed (estimated or measured).
 *          - Electrical rotor position (estimated or measured).
 *          - Measured auxiliary quantity.
 *
 * @param   void
 *
 * @return  The measurements acquired by the motor-control MC.SM state-machine.
 */
mc_foc_input_meas_t FOC_GetMeasMC_FL(void);

/*!
 * @brief   The function returns the actual filtered measured DC-bus voltage.
 *
 * @param   void
 *
 * @return  The actual filtered measured DC-bus voltage.
 */
frac16_t FOC_GetMeasUdcb_FL(void);

/*!
 * @brief   The function returns the actual required AlBe stator voltages.
 *
 * @param   void
 *
 * @return  The actual required stator voltages in the stator reference frame.
 */
GMCLIB_2COOR_ALBE_T_F16 FOC_GetMeasUAlBe_FL(void);

/*!
 * @brief   The function returns the actual required DQ stator voltages.
 *
 * @details The reference frame position is dependent on currently used control
 *          mode.
 *
 * @param   void
 *
 * @return  The actual required stator voltages in the currently used reference
 *          frame.
 */
GMCLIB_2COOR_DQ_T_F16 FOC_GetMeasUDQ_FL(void);

/*!
 * @brief   The function returns the actual measured AlBe stator currents.
 *
 * @param   void
 *
 * @return  The actual required stator currents in the stator reference frame.
 */
GMCLIB_2COOR_ALBE_T_F16 FOC_GetMeasIAlBe_FL(void);

/*!
 * @brief   The function returns the actual measured DQ stator currents.
 *
 * @details The reference frame position is dependent on currently used control
 *          mode.
 *
 * @param   void
 *
 * @return  The actual measured stator currents in the currently used reference
 *          frame.
 */
GMCLIB_2COOR_DQ_T_F16 FOC_GetMeasIDQ_FL(void);


/*!
 * @brief   Function checks the application and speed commands and sets the
 *          speed closed-loop control mode if possible.
 *
 * @details The function checks validity of the speed command and based on the
 *          application command it performs following actions:
 *          kMC_ReqStop  - The MC.SM is requested to enter the STOP state.
 *          kMC_ReqReady - The MC.SM is requested to enter the READY state.
 *          kMC_ReqRun   - If the speed command is:
 *                         Invalid(below minimum) - The MC.SM enters READY
 *                                                  state.
 *                         Valid - The speed command is internally updated.
 *                                 The motor startup is necessary when:
 *                                 - The speed FOC is not already running.
 *                                 - The spin direction has changed.
 *                                 - Other than speed FOC control mode is being
 *                                   used.
 *
 * @note    This function should not be interrupted by other FOC_SetCtrl..._BL()
 *          FOC API function.
 *
 * @param   eAppCmd - Application command (STOP, READY and RUN).
            f16SpeedCmd - Required speed command fractional value with N_MAX
 *                       scale. This value is checked against the minimal
 *                       M1_MERG_SPEED_TRH and maximal value M1_N_NOM specified
 *                       in m1_pmsm_apconfig.h.
 *
 * @retval  TRUE  - The speed command value is valid.
 * @retval  FALSE - The speed command value is NOT valid.
 */
bool_t FOC_SetCtrlSpeedCL_BL(foc_app_cmd_t eAppCmd, frac16_t f16SpeedCmd);

/*!
 * @brief   Function checks the application and current commands and sets the
 *          current closed-loop control mode if possible.
 *
 * @details The function checks validity of the current command and based on the
 *          application command it performs following actions:
 *          kMC_ReqStop  - The MC.SM is requested to enter the STOP state.
 *          kMC_ReqReady - The MC.SM is requested to enter the READY state.
 *          kMC_ReqRun   - If the current command is:
 *                         Invalid(zero) - The MC.SM enters READY state.
 *                         Valid - The current command is internally updated.
 *                                 The motor startup is necessary when:
 *                                 - The current FOC is not already running.
 *                                 - Other than current FOC control mode is
 *                                   being used.
 *
 * @note    This function should not be interrupted by other FOC_SetCtrl..._BL()
 *          FOC API function.
 *
 * @param   eAppCmd - Application command (STOP, READY and RUN).
            sIDQcmd - Required current command fractional value with I_MAX
 *                    scale. The resulting vector size is checked against the
 *                    maximal M1_SPEED_LOOP_HIGH_LIMIT value in
 *                    m1_pmsm_apconfig.h.
 *
 * @retval  TRUE  - The current command value is valid.
 * @retval  FALSE - The current command value is NOT valid.
 */
bool_t FOC_SetCtrlCurrCL_BL(foc_app_cmd_t eAppCmd,
                            GMCLIB_2COOR_DQ_T_F16 sIDQcmd);


/*!
 * @brief   Function checks the application and frequency commands and sets the
 *          scalar open-loop control mode if possible.
 *
 * @details The function checks validity of the frequency command and based on
 *          the application command it performs following actions:
 *          kMC_ReqStop  - The MC.SM is requested to enter the STOP state.
 *          kMC_ReqReady - The MC.SM is requested to enter the READY state.
 *          kMC_ReqRun   - If the frequency command is:
 *                         Invalid(zero) - The MC.SM enters READY state.
 *                         Valid - The frequency command is internally updated.
 *                                 The motor startup is necessary when:
 *                                 - The scalar control is not already running.
 *                                 - Other than scalar control mode is
 *                                   being used.
 *
 * @note    This function should not be interrupted by other FOC_SetCtrl..._BL()
 *          FOC API function.
 *
 * @param   eAppCmd - Application command (STOP, READY and RUN).
 *          f16FrqElCmd - Required frequency command fractional value with
 *                        M1_FREQ_MAX scale.
 *
 * @retval  TRUE  - The frequency command value is valid.
 * @retval  FALSE - The frequency command value is NOT valid.
 */
bool_t FOC_SetCtrlScalarOL_BL(foc_app_cmd_t eAppCmd, frac16_t f16FrqElCmd);


/*!
 * @brief   Function checks the application and voltage commands and sets the
 *          voltage open-loop control mode if possible.
 *
 * @details The function checks validity of the voltage command and based on the
 *          application command it performs following actions:
 *          kMC_ReqStop  - The MC.SM is requested to enter the STOP state.
 *          kMC_ReqReady - The MC.SM is requested to enter the READY state.
 *          kMC_ReqRun   - If the voltage or its speed command is:
 *                         Invalid(zero) - The MC.SM enters READY state.
 *                         Valid - The voltage command is internally updated.
 *                                 The motor state-machine goes through STOP
 *                                 state when other than open-loop voltage
 *                                 control mode is active.
 *
 * @note    This function should not be interrupted by other FOC_SetCtrl..._BL()
 *          FOC API function.
 *
 * @param   eAppCmd - Application command (STOP, READY and RUN).
 *          sUDQCmd - Required voltage command fractional value with U_MAX
 *                    scale.
 *          f16FrqElCmd - Required vector rotation frequency command fractional
 *                        value with M1_FREQ_MAX scale.
 *          f16PosElCmd - Required vector position shift command fractional value.
 *
 * @retval  TRUE  - The voltage command value is non-zero (valid - goes to RUN).
 * @retval  FALSE - The voltage command value is zero (invalid - remains in
 *                  READY).
 */
bool_t FOC_SetCtrlVoltOL_BL(foc_app_cmd_t eAppCmd, GMCLIB_2COOR_DQ_T_F16 sUDQCmd,
                            frac16_t f16FrqElCmd, frac16_t f16PosElCmd);


/*!
 * @brief   Function checks the application and current commands and sets the
 *          current open-loop control mode if possible.
 *
 * @details The function checks validity of the current command and based on the
 *          application command it performs following actions:
 *          kMC_ReqStop  - The MC.SM is requested to enter the STOP state.
 *          kMC_ReqReady - The MC.SM is requested to enter the READY state.
 *          kMC_ReqRun   - If the current or its speed command is:
 *                         Invalid(zero) - The MC.SM enters READY state.
 *                         Valid - The current command is internally updated.
 *                                 The motor state-machine goes through STOP
 *                                 state when other than open-loop current
 *                                 control mode is active.
 *
 * @note    This function should not be interrupted by other FOC_SetCtrl..._BL()
 *          FOC API function.
 *
 * @param   eAppCmd - Application command (STOP, READY and RUN).
 *          sIDQCmd - Required current command fractional value with I_MAX
 *                    scale.
 *          f16FrqElCmd - Required vector rotation frequency command fractional
 *                        value with M1_FREQ_MAX scale.
 *          f16PosElCmd - Required vector position shift command fractional
 *                        value.
 *
 * @retval  TRUE  - The current command value is non-zero (valid - goes to RUN).
 * @retval  FALSE - The current command value is zero (invalid - remains in
 *                  READY).
 */
bool_t FOC_SetCtrlCurrOL_BL(foc_app_cmd_t eAppCmd, GMCLIB_2COOR_DQ_T_F16 sIDQCmd,
                            frac16_t f16FrqElCmd, frac16_t f16PosElCmd);


/*!
 * @brief   Function sets the MID control mode if possible.
 *
 * @details Following actions are performed based on application command:
 *          kMC_ReqStop  - The MC.SM is requested to enter the STOP state.
 *          kMC_ReqReady - The MC.SM is requested to enter the READY state.
 *          kMC_ReqRun   - The quantity command is internally updated and
 *                         the RUN request is passed to MC.SM.
 *
 * @note    This function should not be interrupted by other FOC_SetCtrl..._BL()
 *          FOC API function.
 *
 * @param   eAppCmd - Application command (STOP, READY and RUN).
 *          sDQCmd - Required current OR voltage command fractional value with
 *                   I_MAX OR U_MAX scale (see ui16Cfg parameter description).
 *          f16FrqElCmd - Required vector rotation frequency command fractional
 *                        value with M1_FREQ_MAX scale.
 *          f16PosElCmd - Required vector position shift command fractional
 *                        value.
 *          sCfg - MID control mode configuration:
 *                 bCurrCtrl = FALSE - Voltage control is used.
 *                 bCurrCtrl = TRUE - Current control is used.
 *                 bCloseLoop = FALSE - The open-loop position si used
 *                                      (f16FrqElCmd and f16PosElCmd apply).
 *                 bCloseLoop = TRUE - The closed-loop position si used
 *                                     (MC.SM f16PosEl applies).
 *
 * @return  none
 */
void FOC_SetCtrlMID_FL(foc_app_cmd_t eAppCmd,
                       GMCLIB_2COOR_DQ_T_F16 sDQCmd,
                       frac16_t f16FrqElCmd,
                       frac16_t f16PosElCmd,
                       foc_ctrl_mode_cfg_t sCfg);



/*                INITIALIZATION FOC ROUTINES (called by MC.SM only)          */
/*!
 * @brief   FOC initialization routine, which is called during the MCU
 *          initialization phase by MC.SM state-machine.
 *
 * @param   void
 *
 * @return  none
 */
void  FOC_Init_AR(void);


/*                FAST LOOP FOC ROUTINES (called by MC.SM only)               */
/*!
 * @brief   FOC state variable reset routine.
 *
 * @param   void
 *
 * @return  none
 */
void FOC_ClearStates_FL(void);

/*!
 * @brief   FOC idle routine. No control action is generated.
 *
 * @param   sMeas - Measured stator currents and estimated (or measured) rotor
 *                  positions and speed.
 *
 * @return  none
 */
void FOC_RoutineIdle_FL(const mc_foc_input_meas_t sMeas);

/*!
 * @brief   FOC alignment state routine.
 *
 * @details The two-step voltage alignment is expected to be performed during
 *          this state. Executed in the fast control loop (FL) by the
 *          motor-control state-machine (MC.SM).
 *
 * @param   sMeas - Measured stator currents and estimated (or measured) rotor
 *                  positions and speed.
 *
 * @return  Stator voltage request.
 */
GMCLIB_2COOR_ALBE_T_F16  FOC_RoutineAlign_FL(const mc_foc_input_meas_t sMeas);

/*!
 * @brief   FOC low-speed state routine.
 *
 * @details The low-speed state, where the low-speed position and speed
 *          estimation or measurement algorithm is executed in the MC.SM.
 *          This routine is expected to return voltage action based on selected
 *          control mode. Executed in the fast control loop (FL) by the
 *          motor-control state-machine (MC.SM).
 *
 * @param   sMeas - Measured stator currents and estimated (or measured) rotor
 *                  positions and speed.
 *
 * @return  Requested stator voltage.
 */
GMCLIB_2COOR_ALBE_T_F16  FOC_RoutineLoSpd_FL(const mc_foc_input_meas_t sMeas);

/*!
 * @brief   FOC medium-speed state routine.
 *
 * @details The medium-speed state, where both the low- and high-speed position
 *          and speed estimation or measurement algorithms are executed. The
 *          final position and speed information provided to the FOC is obtained
 *          by merging of output of both algorithms. This routine is expected
 *          to return voltage action based on selected control mode. Executed
 *          in the fast control loop (FL) by the motor-control state-machine
 *          (MC.SM).
 *
 * @param   sMeas - Measured stator currents and estimated (or measured) rotor
 *                  positions and speed.
 *
 * @return  Requested stator voltage.
 */
GMCLIB_2COOR_ALBE_T_F16  FOC_RoutineMiSpd_FL(const mc_foc_input_meas_t sMeas);

/*!
 * @brief   FOC high-speed state routine.
 *
 * @details The high-speed state, where the low-speed position and speed
 *          estimation or measurement algorithm is executed. This routine is
 *          expected to return voltage action based on selected control mode.
 *          Executed in the fast control loop (FL) by the motor-control
 *          state-machine (MC.SM).
 *
 * @param   sMeas - Measured stator currents and estimated (or measured) rotor
 *                  positions and speed.
 *
 * @return  Requested stator voltage.
 */
GMCLIB_2COOR_ALBE_T_F16  FOC_RoutineHiSpd_FL(const mc_foc_input_meas_t sMeas);


/*                Slow LOOP FOC ROUTINES (called by MC.SM only)               */
/*!
 * @brief   FOC idle routine. No control action is generated.
 *
 * @param   sMCStatus - The motor-control state-machine status.
 *
 * @return  Control request from FOC based on application command.
 */
foc_status_t  FOC_RoutineIdle_SL(const mc_status_t sMCStatus);

/*!
 * @brief   FOC alignment state routine.
 *
 * @details Executed in the slow control loop (SL) by the motor-control
 *          state-machine (MC.SM). The function is expected to perform
 *          slow-control loop update MC.SM request based on the application
 *          command.
 *
 * @param   sMCStatus - The motor-control state-machine status.
 *
 * @return  Control request from FOC based on application command.
 */
foc_status_t  FOC_RoutineAlign_SL(const mc_status_t sMCStatus);

/*!
 * @brief   FOC low-speed state routine.
 *
 * @details Executed in the slow control loop (SL) by the motor-control
 *          state-machine (MC.SM). The function is expected to perform
 *          slow-control loop update MC.SM request based on the application
 *          command.
 *
 * @param   sMCStatus - The motor-control state-machine status.
 *
 * @return  Control request from FOC based on application command.
 */
foc_status_t  FOC_RoutineLoSpd_SL(const mc_status_t sMCStatus);

/*!
 * @brief   FOC mid-speed state routine.
 *
 * @details Executed in the slow control loop (SL) by the motor-control
 *          state-machine (MC.SM). The function is expected to perform
 *          slow-control loop update MC.SM request based on the application
 *          command.
 *
 * @param   sMCStatus - The motor-control state-machine status.
 *
 * @return  Control request from FOC based on application command.
 */
foc_status_t  FOC_RoutineMiSpd_SL(const mc_status_t sMCStatus);

/*!
 * @brief   FOC high-speed state routine.
 *
 * @details Executed in the slow control loop (SL) by the motor-control
 *          state-machine (MC.SM). The function is expected to perform
 *          slow-control loop update MC.SM request based on the application
 *          command.
 *
 * @param   sMCStatus - The motor-control state-machine status.
 *
 * @return  Control request from FOC based on application command.
 */
foc_status_t  FOC_RoutineHiSpd_SL(const mc_status_t sMCStatus);



/*              CONTROL ROUTINE TRANSITIONS (called by MC.SM only)            */
/*!
 * @brief   Transition to the idle FOC routine.
 *
 * @param   void
 *
 * @return  none
 */
void  FOC_TransToIdle_FL(void);

/*!
 * @brief   Transition to the align FOC routine.
 *
 * @param   void
 *
 * @return  none
 */
void  FOC_TransToAlign_FL(void);

/*!
 * @brief   Transition to the low-speed FOC routine.
 *
 * @param   void
 *
 * @retval  FRAC16(1.0)  - Positive startup direction is required.
 * @retval  FRAC16(-1.0) - Negative startup direction is required.
 */
frac16_t  FOC_TransToLoSpd_FL(void);

/*!
 * @brief   Transition to the medium-speed FOC routine.
 *
 * @param   void
 *
 * @return  none
 */
void  FOC_TransToMiSpd_FL(void);

/*!
 * @brief   Transition to the high-speed FOC routine.
 *
 * @param   void
 *
 * @return  none
 */
void  FOC_TransToHiSpd_FL(void);




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _M1_STATE_ROUTINES_H_ */

