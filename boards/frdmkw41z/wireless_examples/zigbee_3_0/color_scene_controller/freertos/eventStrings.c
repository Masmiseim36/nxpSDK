/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       eventStrings.c
\brief      ZLL Application/Stack event strings - Implementation
==============================================================================*/

/****************************************************************************/
/*          Include files                                                   */
/****************************************************************************/

#include <jendefs.h>

/****************************************************************************/
/*          Macro Definitions                                               */
/****************************************************************************/


/****************************************************************************/
/***        Type Definitions                                                */
/****************************************************************************/


/****************************************************************************/
/*          Local Function Prototypes                                       */
/****************************************************************************/


/****************************************************************************/
/*          Exported Variables                                              */
/****************************************************************************/
#ifdef EVENT_STR
PUBLIC char *apcZPSEventStrings[] =
{
        "ZPS_EVENT_NONE",
        "ZPS_EVENT_APS_DATA_INDICATION",
        "ZPS_EVENT_APS_DATA_CONFIRM",
        "ZPS_EVENT_APS_DATA_ACK",
        "ZPS_EVENT_NWK_STARTED",
        "ZPS_EVENT_NWK_JOINED_AS_ROUTER",
        "ZPS_EVENT_NWK_JOINED_AS_ENDDEVICE",
        "ZPS_EVENT_NWK_FAILED_TO_START",
        "ZPS_EVENT_NWK_FAILED_TO_JOIN",
        "ZPS_EVENT_NWK_NEW_NODE_HAS_JOINED",
        "ZPS_EVENT_NWK_DISCOVERY_COMPLETE",
        "ZPS_EVENT_NWK_LEAVE_INDICATION",
        "ZPS_EVENT_NWK_LEAVE_CONFIRM",
        "ZPS_EVENT_NWK_STATUS_INDICATION",
        "ZPS_EVENT_NWK_ROUTE_DISCOVERY_CONFIRM",
        "ZPS_EVENT_NWK_POLL_CONFIRM",
        "ZPS_EVENT_NWK_ED_SCAN",
        "ZPS_EVENT_ZDO_BIND",
        "ZPS_EVENT_ZDO_UNBIND",
        "ZPS_EVENT_ZDO_LINK_KEY",
        "ZPS_EVENT_BIND_REQUEST_SERVER",
        "ZPS_EVENT_ERROR",
        "ZPS_EVENT_APS_INTERPAN_DATA_INDICATION",
        "ZPS_EVENT_APS_INTERPAN_DATA_CONFIRM",
        "ZPS_EVENT_APS_ZDP_RESPONSE"

};
#endif

#ifdef EVENT_STR
PUBLIC char *apcAPPEventStrings[] =
{
    "APP_E_EVENT_NONE",
    "APP_E_EVENT_BUTTON_UP",
    "APP_E_EVENT_BUTTON_DOWN",
    "APP_E_EVENT_PGM_BUTTON_UP",
    "APP_E_EVENT_PGM_BUTTON_DOWN",

};
#endif


/****************************************************************************/
/*          Local Variables                                                 */
/****************************************************************************/

/****************************************************************************/
/*          Exported Functions                                              */
/****************************************************************************/

/****************************************************************************/
/***        Local Functions                                                 */
/****************************************************************************/

/****************************************************************************/
/*          END OF FILE                                                     */
/****************************************************************************/
