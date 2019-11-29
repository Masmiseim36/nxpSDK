/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "jendefs.h"

#include "string.h"
#include <dbg.h>
#include "EH_Button.h"
#include "EH_IEEE_Features.h"
#include "EH_IEEE_Commands.h"
#include "app_buttons.h"
#include "ZTimer.h"
#include "app_main.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/** Time in msec to wait to enable button interrupts when a button interrupt is detected */
#define BUTTON_STABLE_DURATION                  (15)

#define NEXT_MOVE_CMD_DURATION                  (500) /* 500m sec */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/****************************************************************************/
/***        Exported Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/* flag indicating whether button press detected, updated from ISR */
volatile bool bButtonPressed = 0;

/* The button which is pressed/released , updated from ISR */
volatile uint32 u32Button = 0;

/****************************************************************************/
/***        Local Function Prototypes                                               ***/
/****************************************************************************/
 void vEH_ButtonCallback(uint32 u32DeviceId, uint32 u32ItemBitmap);
 void vEH_TickTimerCallback(uint32 u32DeviceId, uint32 u32ItemBitmap);
 void ZTIMER_vAhiCallback ( uint32 u32Device, uint32 u32ItemBitmap);
 
/****************************************************************************/
/***        Public Functions                                              ***/
/****************************************************************************/


/****************************************************************************
 * NAME: vHandleButtonPress
 *
 * DESCRIPTION:
 * handles button press event on Dk4 and initiates appropriate commands
 *
 * RETURNS:
 * Return whether device can go to sleep
 *
 ****************************************************************************/
#ifdef DK4
void vHandleButtonPress(te_TransitionCode  eCode )
{
    static bool bIsCommMode = 0;
    vLoadPersistentData();

    switch(eCode)
    {

    case COMM_BUTTON_PRESSED:
        /* if already in operational state, switch to commissioning state */
        if(sGPDPersistentData.b8CommissioiningStatusBits & COMM_COMP_SET_BIT)
        {
            sGPDPersistentData.b8CommissioiningStatusBitsPrevious =
                    sGPDPersistentData.b8CommissioiningStatusBits;
#ifdef GPD_SUPPORT_PERSISTENT_DATA
            bAHI_WriteEEPROMBlock(EEPROM_START_SECTOR,
                    sizeof(tGPD_PersistantData),
                    (uint8 *)&sGPDPersistentData);
            DBG_vPrintf(DBG_DEVICE_SWITCH, "\n  reverted= %d, previous = %d",
                sGPDPersistentData.b8CommissioiningStatusBits ,
                sGPDPersistentData.b8CommissioiningStatusBitsPrevious
                );
#endif
            bIsCommMode = TRUE;
        }
        break;

    case ON_PRESSED:
        /* Switch 1: mapping
         * 1. Commissioning Commands
         * 2. On Or  Toggle Command
         * 3. De commissioning command
         * 4. Factory Reset
         * */
        if(bIsCommMode)
        {
            sGPDPersistentData.b8CommissioiningStatusBits = CHANNEL_SET_BIT;
            DBG_vPrintf(DBG_DEVICE_SWITCH, "\n Commissioning \n");
            bIsCommMode = FALSE;
        }
        DBG_vPrintf(DBG_DEVICE_SWITCH, "\n ON_PRESSED HandleGPDStateMachine \n");
        HandleGPDStateMachine(0);
        //tbd short press
        break;

    case OFF_PRESSED:
        DBG_vPrintf(DBG_DEVICE_SWITCH, "\n APP_E_BUTTONS_BUTTON_SW2 \n");
        /* Button 2 mapping :
         * Off command , Decommissioning command if Dio8 pressed */
        if(bIsCommMode)
        {
            if(sGPDPersistentData.b8CommissioiningStatusBits & COMM_COMP_SET_BIT)
            {
                vSendDecommissioingCmd();

                bIsCommMode = FALSE;
            }
        }
        else
        {
            HandleGPDStateMachine(E_GPD_OFF_CMD);
        }
        break;

    case UP_PRESSED:
        DBG_vPrintf(DBG_DEVICE_SWITCH, "\n APP_E_BUTTONS_BUTTON_SW3 \n");
        /* Button 3 mapping :
         * Move up with on/off( button press) ,
         * Level control stop (button release ) */

        HandleGPDStateMachine(E_GPD_MOVE_UP_CMD);

        break;

    case DOWN_PRESSED:
        HandleGPDStateMachine(E_GPD_MOVE_DOWN_CMD);
        break;

    case DOWN_RELEASED:
    case UP_RELEASED:
        HandleGPDStateMachine(E_GPD_LEVEL_CONTROL_STOP);
        break;

    case UP_AND_ON_TOGETHER_PRESSED:
        /* This is used in current commissioning failed case.
         * Revert state */
        DBG_vPrintf(DBG_DEVICE_SWITCH, "\n UP_AND_ON_TOGETHER_PRESSED = %d, previous = %d",
                sGPDPersistentData.b8CommissioiningStatusBits ,
                sGPDPersistentData.b8CommissioiningStatusBitsPrevious
                );
        if((sGPDPersistentData.b8CommissioiningStatusBits == CHANNEL_SET_BIT) &&
        (sGPDPersistentData.b8CommissioiningStatusBitsPrevious & COMM_COMP_SET_BIT))
        {
            sGPDPersistentData.b8CommissioiningStatusBits =
                    sGPDPersistentData.b8CommissioiningStatusBitsPrevious;
            sGPDPersistentData.b8CommissioiningStatusBitsPrevious = 0;
#ifdef GPD_SUPPORT_PERSISTENT_DATA
            bAHI_WriteEEPROMBlock(EEPROM_START_SECTOR,
                    sizeof(tGPD_PersistantData),
                    (uint8 *)&sGPDPersistentData);
            DBG_vPrintf(DBG_DEVICE_SWITCH, "\n  reverted= %d, previous = %d",
                sGPDPersistentData.b8CommissioiningStatusBits ,
                sGPDPersistentData.b8CommissioiningStatusBitsPrevious
                );
#endif
        }
        break;

    default:
        //DBG_vPrintf(DBG_DEVICE_SWITCH, "\n  Button Handler default= 0x%x", eCode);
        break;
    }

}
#endif
/****************************************************************************
 * NAME: bButtonInitialize
 *
 * DESCRIPTION:
 * Button Initialization function, enables interrupts only on DK4
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
bool bButtonInitialize(void)
{
  return FALSE;
}

/****************************************************************************/
/***        Local Functions                                              ***/
/****************************************************************************/
/****************************************************************************
 * NAME: vEH_ButtonCallback
 *
 * DESCRIPTION:
 * Button Interrupt CallBack
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
void vEH_TickTimerCallback(uint32 u32DeviceId, uint32 u32ItemBitmap)
{
    ZTIMER_vAhiCallback ( u32DeviceId, u32ItemBitmap);
}

void vEH_ButtonCallback(uint32 u32DeviceId, uint32 u32ItemBitmap)
{

}


