/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "dbg.h"
#include "app_zcl_cfg.h"
#include "app_pdum_cfg.h"
#include "app_common.h"
#include "zlo_controller_node.h"
#include "app_menu_handler.h"
#include "app_general_commands.h"
#include "app_level_commands.h"


#ifdef DEBUG_APP
    #define TRACE_APP   TRUE
#else
    #define TRACE_APP   FALSE
#endif
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/



/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/



/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/



/****************************************************************************
 *
 * NAME: vAppLevelMove
 *
 * DESCRIPTION: sends a move level command to a light or lights
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppLevelMove(teCLD_LevelControl_MoveMode eMode, uint8 u8Rate, bool_t bWithOnOff)
{
    tsCLD_LevelControl_MoveCommandPayload sPayload;
    uint8 u8Seq;
    tsZCL_Address sAddress;

    vSetAddress(&sAddress, FALSE);

    sPayload.u8Rate = u8Rate;
    sPayload.u8MoveMode = eMode;

    eCLD_LevelControlCommandMoveCommandSend(sDeviceTable.asDeviceRecords[0].u8Endpoint,
                                            sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u8Endpoint,
                                            &sAddress,
                                            &u8Seq,
                                            bWithOnOff, /* with on off */
                                            &sPayload);
}


/****************************************************************************
 *
 * NAME: vAppLevelStop
 *
 * DESCRIPTION: sends a move level stop command to a light or lights
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppLevelStop(bool_t bWithOnOff)
{
    uint8 u8Seq;
    tsZCL_Address sAddress;
    teZCL_Status eStatus;

    tsCLD_LevelControl_StopCommandPayload  sPayload;

    sPayload.u8OptionsMask = 0;
    sPayload.u8OptionsOverride = 0;


    vSetAddress(&sAddress, FALSE);
    eStatus = eCLD_LevelControlCommandStopCommandSend(
                        sDeviceTable.asDeviceRecords[0].u8Endpoint,
                        sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u8Endpoint,
                        &sAddress,
                        &u8Seq,
                        bWithOnOff,
                        &sPayload);
#if TRACE_APP
    if ( eStatus != E_ZCL_SUCCESS)
    {
        DBG_vPrintf(TRACE_APP, "\neCLD_LevelControlCommandStopCommandSend  eStatus = %d", eStatus);
    }
#else
    (void)eStatus;    
#endif

}

/****************************************************************************
 *
 * NAME: vAppLevelStepMove
 *
 * DESCRIPTION: sends a move step level command to a light or lights
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppLevelStepMove(teCLD_LevelControl_MoveMode eMode, uint8 u8StepSize, uint16 u16TransitionTime, bool_t bWithOnOff)
{
    tsCLD_LevelControl_StepCommandPayload sPayload;
    uint8 u8Seq;
    tsZCL_Address sAddress;

    vSetAddress(&sAddress, FALSE);

    sPayload.u16TransitionTime = u16TransitionTime;
    sPayload.u8StepMode = eMode;
    sPayload.u8StepSize = u8StepSize;
    eCLD_LevelControlCommandStepCommandSend(
            sDeviceTable.asDeviceRecords[0].u8Endpoint,
                        sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u8Endpoint,
                        &sAddress,
                        &u8Seq,
                        bWithOnOff,
                        &sPayload);
}



/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
