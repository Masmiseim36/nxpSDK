/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       app_switch_state_machine.c
\brief      ZLO Controller & Switch Demo: Remote Key Press Behaviour (Implementation)
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "dbg.h"
#include "app_events.h"
#include "app_buttons.h"
#include "app_zlo_switch_node.h"
#include "ZTimer.h"
#include "app_switch_state_machine.h"
#include "App_DimmerSwitch.h"
#include "app_main.h"
#include "SerialManager.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_SWITCH_STATE
    #define TRACE_SWITCH_STATE   TRUE
#else
    #define TRACE_SWITCH_STATE   FALSE
#endif
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vManageIndividualControlMode(te_SwitchState *peSwitchState,
                                          te_TransitionCode eTransitionCode );

PRIVATE void vManageLightControlState(te_SwitchState *peSwitchState,
                                          te_TransitionCode eTransitionCode );

PRIVATE void vManageCommissioningMode(te_SwitchState *peSwitchState,
                                          te_TransitionCode eTransitionCode );

PRIVATE void vSwitchStateMachine(te_TransitionCode eTransitionCode );

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

extern uint16 u16GroupId;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE te_SwitchState eSwitchState  = LIGHT_CONTROL_MODE;
PRIVATE te_TransitionCode eTransitionButtonCode;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: eGetDelayedButtonCode
 *
 * DESCRIPTION:
 * Gets the delayed command that is stored
 *
 * RETURNS:
 * stored transition code
 *
 ****************************************************************************/
te_TransitionCode eGetDelayedButtonCode(void)
{
    return eTransitionButtonCode;
}

/****************************************************************************
 *
 * NAME: vStopStartModeChangeTimer
 *
 * DESCRIPTION:
 * Stops and starts the commissioning timer with required ticks
 *
 * PARAMETERS:
 * u32Ticks     Ticks for timer start
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vStopStartModeChangeTimer( uint32 u32Ticks )
{
    ZTIMER_eStop(u8TimerChangeMode);
    #ifdef SLEEP_ENABLE
        if( FALSE == bWaitingToSleep())
        {
            ZTIMER_eStart(u8TimerChangeMode, u32Ticks);
        }
    #else
        ZTIMER_eStart(u8TimerChangeMode, u32Ticks);
    #endif
}

/****************************************************************************
 *
 * NAME: vDQButtonPress
 *
 * DESCRIPTION:
 * Destroys delayed command that is stored
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vDQButtonPress(void)
{
    eTransitionButtonCode = NUMBER_OF_TRANSITION_CODE;
    ZTIMER_eStop(u8TimerButtonDelay);
}

/****************************************************************************
 *
 * NAME: vQButtonPress
 *
 * DESCRIPTION:
 * Stores delayed command that is stored
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vQButtonPress( te_TransitionCode eTransitionCode)
{
    eTransitionButtonCode = eTransitionCode;
    /*Set up the button delay timer*/
    ZTIMER_eStart(u8TimerButtonDelay, ZTIMER_TIME_MSEC(BUTTON_DELAY_TIME_IN_MS));
}

/****************************************************************************
 *
 * NAME: vSendDelayedCommands
 *
 * DESCRIPTION:
 * Sends the delayed commands in the individual control or commissioning mode
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vSendDelayedCommands(te_TransitionCode eTransitionCode)
{
    DBG_vPrintf(TRACE_SWITCH_STATE,"\r\nIn vSendDelayedCommands with TransitionCode = %d -> ",eTransitionCode);
    switch(eTransitionCode)
    {
          case ON_PRESSED:
              DBG_vPrintf(TRACE_SWITCH_STATE," E_CLD_ONOFF_CMD_ON \r\n");
              vAppOnOff(E_CLD_ONOFF_CMD_ON);
              break;

          case OFF_PRESSED:
              DBG_vPrintf(TRACE_SWITCH_STATE," E_CLD_ONOFF_CMD_OFF \r\n");
              vAppOnOff(E_CLD_ONOFF_CMD_OFF);
              break;

          case UP_PRESSED:
              DBG_vPrintf(TRACE_SWITCH_STATE," E_CLD_LEVELCONTROL_MOVE_MODE_UP \r\n");
              vAppLevelStepMove(E_CLD_LEVELCONTROL_MOVE_MODE_UP);
              break;

          case DOWN_PRESSED:
              DBG_vPrintf(TRACE_SWITCH_STATE," E_CLD_LEVELCONTROL_MOVE_MODE_DOWN \r\n");
              vAppLevelStepMove(E_CLD_LEVELCONTROL_MOVE_MODE_DOWN);
              break;

          default :
              break;
    }
    vDQButtonPress();
}

/****************************************************************************
 *
 * NAME: vManageIndividualControlMode
 *
 * DESCRIPTION:
 * Manage the Individual control modes for the lights in unicast mode
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vManageIndividualControlMode(te_SwitchState *peSwitchState, te_TransitionCode eTransitionCode )
{
      /* Calls individual function
       * Changes state
       * Addressing for individual functions
       *               Individual or group ? - well the address will be taken based on the state */
    DBG_vPrintf(TRACE_SWITCH_STATE,"\nIn vManageIndividualControlMode(Mode = %d) TransitionCode = %d -> ", *peSwitchState,eTransitionCode);
    switch(eTransitionCode)
    {
        /* Fall through for the button presses as there will be a delayed action*/
          case ON_PRESSED:

          case OFF_PRESSED:

          case UP_PRESSED:

          case DOWN_PRESSED:
              vQButtonPress(eTransitionCode);
              break;

          case UP_AND_ON_TOGETHER_PRESSED:
              vDQButtonPress();
              DBG_vPrintf(TRACE_SWITCH_STATE," vAppStoreSceneById = 1 \r\n");
              vAppStoreSceneById(SCENE_1, u16GroupId);
              *peSwitchState = LIGHT_CONTROL_MODE;
              break;

          case DOWN_AND_OFF_TOGETHER_PRESSED:
              vDQButtonPress();
              DBG_vPrintf(TRACE_SWITCH_STATE," vAppStoreSceneById = 2 \r\n");
              vAppStoreSceneById(SCENE_2, u16GroupId);
              *peSwitchState = LIGHT_CONTROL_MODE;
              break;

         case COMM_BUTTON_PRESSED:
              vAPP_ZCL_DeviceSpecific_SetIdentifyTime(0xFF);
              *peSwitchState = COMMISSIONING_MODE;
              break;

          case DOWN_RELEASED:

          case UP_RELEASED:
              vDQButtonPress();
              DBG_vPrintf(TRACE_SWITCH_STATE," vAppLevelStop \r\n");
              vAppLevelStop();
              break;

          default :
              break;
    }
    vStopStartModeChangeTimer(ZTIMER_TIME_MSEC(30000));
}

/****************************************************************************
 *
 * NAME: vManageLightControlState
 *
 * DESCRIPTION:
 * Manage the Light control state in group mode. This is power on state for the
 * state machine.
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vManageLightControlState(te_SwitchState *peSwitchState, te_TransitionCode eTransitionCode )
{
      /* Calls individual function
       * Changes state
       * Addressing for individual functions
       *               Individual or group ? - well the address will be taken based on the state */

    DBG_vPrintf(TRACE_SWITCH_STATE,"\nIn vManageLightControlState(Mode = %d) TransitionCode = %d -> ", *peSwitchState,eTransitionCode);

      switch(eTransitionCode)
      {
          case ON_PRESSED:
              DBG_vPrintf(TRACE_SWITCH_STATE," E_CLD_ONOFF_CMD_ON \r\n");
			  *peSwitchState = INDIVIDUAL_CONTROL_MODE;
              vAppOnOff(E_CLD_ONOFF_CMD_ON);
              break;

          case OFF_PRESSED:
              DBG_vPrintf(TRACE_SWITCH_STATE," E_CLD_ONOFF_CMD_OFF \r\n");
			  *peSwitchState = INDIVIDUAL_CONTROL_MODE;
              vAppOnOff(E_CLD_ONOFF_CMD_OFF);
              break;

          case UP_PRESSED:
              DBG_vPrintf(TRACE_SWITCH_STATE," E_CLD_LEVELCONTROL_MOVE_MODE_UP \r\n");
			  *peSwitchState = INDIVIDUAL_CONTROL_MODE;
              vAppLevelStepMove(E_CLD_LEVELCONTROL_MOVE_MODE_UP);
              break;

          case DOWN_PRESSED:
              DBG_vPrintf(TRACE_SWITCH_STATE," E_CLD_LEVELCONTROL_MOVE_MODE_DOWN \r\n");
			  *peSwitchState = INDIVIDUAL_CONTROL_MODE;
              vAppLevelStepMove(E_CLD_LEVELCONTROL_MOVE_MODE_DOWN);
              break;

          case UP_AND_ON_TOGETHER_PRESSED:
              DBG_vPrintf(TRACE_SWITCH_STATE," vAppRecallSceneById = 1 \r\n");
			  *peSwitchState = INDIVIDUAL_CONTROL_MODE;
              vAppRecallSceneById(SCENE_1, u16GroupId);
              break;

          case DOWN_AND_OFF_TOGETHER_PRESSED:
              DBG_vPrintf(TRACE_SWITCH_STATE," vAppRecallSceneById = 2 \r\n");
			  *peSwitchState = INDIVIDUAL_CONTROL_MODE;
              vAppRecallSceneById(SCENE_2, u16GroupId);
              break;

          case COMM_BUTTON_PRESSED:
              vAPP_ZCL_DeviceSpecific_SetIdentifyTime(0xFF);
              *peSwitchState = COMMISSIONING_MODE;
              break;

          case DOWN_RELEASED:

          case UP_RELEASED:
              DBG_vPrintf(TRACE_SWITCH_STATE," vAppLevelStop \r\n");
              vAppLevelStop();
              break;

        default:
            break;
    }
}

/****************************************************************************
 *
 * NAME: vManageCommissioningMode
 *
 * DESCRIPTION:
 * Manage the Light commissioning in the EZ mode method where either
 *                                      use find and group or find and bind
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vManageCommissioningMode(te_SwitchState *peSwitchState, te_TransitionCode eTransitionCode )
{
      /* Calls individual function
       * Changes state
       * Addressing for individual functions
       * Individual or group ? - well the address will be taken based on the state */
    DBG_vPrintf(TRACE_SWITCH_STATE,"\r\nIn vManageCommissioningMode(Mode = %d): TransitionCode = %d -> ", *peSwitchState,eTransitionCode);
    switch(eTransitionCode)
    {
        case COMM_AND_ON_TOGETHER_PRESSED:
            DBG_vPrintf(TRACE_SWITCH_STATE," Network Steering \r\n");
            sBDB.sAttrib.u32bdbPrimaryChannelSet = BDB_PRIMARY_CHANNEL_SET;
            sBDB.sAttrib.u32bdbSecondaryChannelSet = BDB_SECONDARY_CHANNEL_SET;
            BDB_eNsStartNwkSteering();
            break;

        case COMM_AND_OFF_TOGETHER_PRESSED:
            DBG_vPrintf(TRACE_SWITCH_STATE," vEZ_StartFindAndBindGroup \r\n");
            sBDB.sAttrib.u16bdbCommissioningGroupID = 0xFFFF;
            BDB_eFbTriggerAsInitiator(app_u8GetDeviceEndpoint());
            break;

        case COMM_AND_UP_TOGETHER_PRESSED:
            DBG_vPrintf(TRACE_SWITCH_STATE," vEZ_StartFindAndBindGroup \r\n");
            sBDB.sAttrib.u16bdbCommissioningGroupID = u16GroupId;
            BDB_eFbTriggerAsInitiator(app_u8GetDeviceEndpoint());
            break;

        case COMM_AND_DOWN_TOGETHER_PRESSED:
            /*App would change the channel*/
            DBG_vPrintf(TRACE_SWITCH_STATE," vSetTheNextLightIdentify \r\n");
            vSelectLight();
            break;

        case COMM_BUTTON_RELEASED:
            vAPP_ZCL_DeviceSpecific_IdentifyOff();
            BDB_vFbExitAsInitiator();
            *peSwitchState = INDIVIDUAL_CONTROL_MODE;
            vStopStartModeChangeTimer(ZTIMER_TIME_MSEC(30000));
            break;

        default :
            break;
    }
}
/****************************************************************************
 *
 * NAME: vSwitchStateMachine
 *
 * DESCRIPTION:
 * The control state machine called form the button handler function.
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vSwitchStateMachine(te_TransitionCode eTransitionCode )
{
    switch (eSwitchState)
    {
        case  LIGHT_CONTROL_MODE:
            vManageLightControlState(&eSwitchState, eTransitionCode);
            break;

        case INDIVIDUAL_CONTROL_MODE:
            vManageIndividualControlMode(&eSwitchState, eTransitionCode);
            break;

        case COMMISSIONING_MODE:
            vManageCommissioningMode(&eSwitchState, eTransitionCode);
            break;

        default:
            break;
    }
}
/****************************************************************************
 *
 * NAME: vApp_ProcessKeyCombination
 *
 * DESCRIPTION:
 * Interprets the button press and calls the state machine.
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vApp_ProcessKeyCombination(APP_tsEvent sButton)
{
    te_TransitionCode eTransitionCode = NUMBER_OF_TRANSITION_CODE;
	eSwitchState = LIGHT_CONTROL_MODE;
	
    DBG_vPrintf(TRACE_SWITCH_STATE, "\nButton Event = %d\r\n",sButton.eType);
    switch(sButton.eType)
    {
        case APP_E_EVENT_BUTTON_DOWN:
            DBG_vPrintf(TRACE_SWITCH_STATE, "\nButton DN Number= %d\r\n",sButton.uEvent.sButton.u8Button);
            DBG_vPrintf(TRACE_SWITCH_STATE, "\nDIO State    = %08x\r\n",sButton.uEvent.sButton.u32DIOState);

            eTransitionCode = sButton.uEvent.sButton.u8Button;

            if(0 == (sButton.uEvent.sButton.u32DIOState & (ON)))
            {
                    eTransitionCode = ON_PRESSED; //UP_AND_ON_TOGETHER_PRESSED;
            }

            if(0 == (sButton.uEvent.sButton.u32DIOState & (OFF)))
            {
                    eTransitionCode = OFF_PRESSED; //UP_AND_OFF_TOGETHER_PRESSED;
            }

            if(0 == (sButton.uEvent.sButton.u32DIOState & (UP)))
            {
                eTransitionCode = UP_PRESSED; //DOWN_AND_OFF_TOGETHER_PRESSED;
            }


            if(0 == (sButton.uEvent.sButton.u32DIOState & (DOWN)))
            {
                eTransitionCode = DOWN_PRESSED; //DOWN_AND_ON_TOGETHER_PRESSED;
            }
#if 0			
            if(0 == (sButton.uEvent.sButton.u32DIOState & (COMM | ON)))
            {
                eTransitionCode = COMM_AND_ON_TOGETHER_PRESSED;
            }
#endif
            if(0 == (sButton.uEvent.sButton.u32DIOState & (COMM /*| OFF*/)))
            {
                eTransitionCode = COMM_AND_OFF_TOGETHER_PRESSED;
                eSwitchState = COMMISSIONING_MODE;				
            }
#if 0
            if(0 == (sButton.uEvent.sButton.u32DIOState & (COMM | UP)))
            {
                eTransitionCode = COMM_AND_UP_TOGETHER_PRESSED;
            }

            if(0 == (sButton.uEvent.sButton.u32DIOState & (COMM | DOWN)))
            {
                eTransitionCode = COMM_AND_DOWN_TOGETHER_PRESSED;
            }
#endif			

            DBG_vPrintf(TRACE_SWITCH_STATE, "\nTransition Code = %d\r\n",eTransitionCode);            
			
            vSwitchStateMachine(eTransitionCode);
            break;

        case APP_E_EVENT_BUTTON_UP:
            DBG_vPrintf(TRACE_SWITCH_STATE, "Button UP Number= %d\r\n",sButton.uEvent.sButton.u8Button);
            DBG_vPrintf(TRACE_SWITCH_STATE, "DIO State    = %08x\r\n",sButton.uEvent.sButton.u32DIOState);
                        

		
            switch (sButton.uEvent.sButton.u8Button)
            {
                case COMM_BUTTON_PRESSED:
                    eTransitionCode = COMM_BUTTON_RELEASED;
                    break;

                case DOWN_PRESSED:
                    eTransitionCode = DOWN_RELEASED;
                    break;

                case UP_PRESSED:
                    eTransitionCode = UP_RELEASED;
                    break;

                default :
                    break;
            }
            vSwitchStateMachine(eTransitionCode);
            break;
        default :
            break;
    }
}
/****************************************************************************
 *
 * NAME: eGetSwitchState
 *
 * DESCRIPTION:
 * Access function for the switch state.
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC te_SwitchState eGetSwitchState(void)
{
    return eSwitchState;
}

/****************************************************************************
 *
 * NAME: APP_tskDelayedButtonAction
 *
 * DESCRIPTION:
 * Activated by the button delay timer and sends out the delayed command.
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_cbTimerButtonDelay(void *pvParam)
{
    /*Get the stored transition code */
    te_TransitionCode eTransitionCode = eGetDelayedButtonCode();
    /*Destroy the stored button code */
    vDQButtonPress();
    /*Take action based on the button code*/
    if( ON_PRESSED <= eTransitionCode && DOWN_PRESSED >= eTransitionCode )
    {
        vSendDelayedCommands(eTransitionCode);
    }

}

/****************************************************************************
 *
 * NAME: APP_cbTimerChangeMode
 *
 * DESCRIPTION:
 * Activated by the button delay timer and sends out the delayed command.
 *
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_cbTimerChangeMode(void *pvParam)
{
    /*Restart the Timer from expiring */
    if(ZTIMER_eGetState(u8TimerChangeMode) == E_ZTIMER_STATE_EXPIRED)
    {
        ZTIMER_eStop(u8TimerChangeMode);
        eSwitchState = LIGHT_CONTROL_MODE;
    }
    else
    {
        vStopStartModeChangeTimer(ZTIMER_TIME_MSEC(30000));
    }
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
