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

#ifndef APP_LIGHT_NODE_H_
#define APP_LIGHT_NODE_H_

 /*!
\file       app_zlo_light_node.h
\brief      ZLO Demo: Light Node NWK Startup - Interface
*/
     
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#ifdef CLD_OTA
#include "OTA.h"
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* set to flase to fix the channel chosen by light at network start
 * set to TRUE for release builds
 */

/* Set true to produce debug output to show the received network key
 * set FALSE for the release build
 */

typedef enum
{
    E_STARTUP,
    E_NFN_START,
    E_RUNNING,
} teNODE_STATES;

typedef struct
{
    uint8 u8OnOffStartAttrib;
    uint8   u8OnOff;
#ifdef CLD_LEVEL_CONTROL
    uint8 u8LevelStartattrib;
    uint8 u8LevelOptionsAttrib;
    uint8 u8Level;
#endif
#ifdef CLD_COLOUR_CONTROL
    uint8 u8ColourOptionsAttrib;
#ifdef CLD_COLOURCONTROL_ATTR_STARTUP_COLOUR_TEMPERATURE_MIRED
    uint16 u16ColourStartAttrib;
    uint16 u16Mired;
#endif
#endif
}tsStartUpAttrib;

typedef struct {
    enum {
        FACTORY_NEW = 0,
        NOT_FACTORY_NEW = 0xff
    }eState;
    teNODE_STATES eNodeState;
    uint16 u16MyAddr;
#ifdef CLD_OTA
    bool bValid;
    uint64 u64IeeeAddrOfServer;
    uint16 u16NwkAddrOfServer;
    uint8 u8OTAserverEP;
#endif
}tsZllState;

extern tsZllState sZllState;




/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void APP_vInitialiseNode(void);
PUBLIC void APP_vFactoryResetRecords(void);
PUBLIC void APP_taskLight(void);
PUBLIC void APP_vOobcSetRunning(void);

#ifdef CLD_OTA
PUBLIC teNODE_STATES eGetNodeState(void);
PUBLIC tsOTA_PersistedData sGetOTACallBackPersistdata(void);
#endif


#ifdef COLORLIGHT
#define NFC_NWK_LIGHT             NFC_NWK_NSC_DEVICE_LAMP_RGB
#else
#ifdef DIMMABLELIGHT
#define NFC_NWK_LIGHT             NFC_NWK_NSC_DEVICE_LAMP_DIMMABLE
#else
#ifdef COLORTEMPERATURELIGHT
#define NFC_NWK_LIGHT             NFC_NWK_NSC_DEVICE_LAMP_TUNABLE_WHITE
#else
#warning "Light Type Not Defined"
#endif
#endif
#endif


/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_LIGHT_NODE_H_*/
