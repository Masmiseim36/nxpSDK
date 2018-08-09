/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
