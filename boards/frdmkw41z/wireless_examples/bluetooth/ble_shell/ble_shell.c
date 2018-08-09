/*! *********************************************************************************
 * \addtogroup Bluetooth Shell Application
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This file is the source file for the Bluetooth Shell Application
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

/************************************************************************************
 *************************************************************************************
 * Include
 *************************************************************************************
 ************************************************************************************/
/* Framework / Drivers */
#include "RNG_Interface.h"
#include "Keyboard.h"
#include "LED.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "fsl_os_abstraction.h"
#include "shell.h"
#include "Panic.h"
#include "MemManager.h"
#include "board.h"

/* BLE Host Stack */
#include "gatt_interface.h"
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gatt_database.h"
#include "gap_interface.h"
#include "gatt_db_app_interface.h"

/* Shell APIs*/
#include "shell_gap.h"
#include "shell_gatt.h"
#include "shell_gattdb.h"

#include "ble_conn_manager.h"
#include "ApplMain.h"
#include "ble_shell.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
static uint8_t mSupressEvents = 0;
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
 *************************************************************************************
 * Private memory declarations
 *************************************************************************************
 ************************************************************************************/

const char mpGapHelp[] = "\r\n"
           "gap advcfg [-interval intervalInMs]\r\n"
           "gap advstart\r\n"
           "gap advstop\r\n"
           "gap advcfg [-interval intervalInMs] [-type type]\r\n"
           "gap advdata -erase -add [type] [payload]\r\n"
           "gap scanstart\r\n"
           "gap scanstop\r\n"
           "gap scancfg [-type type] [-interval intervalInMs] [-window windowInMs]\r\n"
           "gap scandata [-erase] [-add type payload]\r\n"
           "gap connectcfg [-interval intervalInMs] [-latency latency] [-timeout timeout]\r\n"
           "gap connect scannedDeviceId\r\n"
           "gap disconnect\r\n"
           "gap connupdate mininterval maxinterval latency timeout\r\n"
           "gap paircfg [-usebonding usebonding] [-seclevel seclevel] [-keyflags flags]\r\n"
           "gap pair\r\n"
           "gap enterpin pin\r\n"
           "gap bonds [-erase] [-remove deviceIndex]\r\n";

const char mpGattHelp[] = "\r\n"
           "gatt discover [-all] [-service serviceUuid16InHex] \r\n"
           "gatt read handle\r\n"
           "gatt write handle valueInHex\r\n"
           "gatt writecmd handle valueInHex\r\n"
           "gatt notify handle valueInHex\r\n"
           "gatt indicate handle valueInHex\r\n";

const char mpGattDbHelp[] = "\r\n"
           "gattdb read handle\r\n"
           "gattdb write handle valueInHex\r\n"
           "gattdb addservice serviceUuid16InHex\r\n"
           "gattdb erase\r\n";

/* Shell */
const cmd_tbl_t mGapCmd =
{
    .name = "gap",
    .maxargs = 11,
    .repeatable = 1,
    .cmd = ShellGap_Command,
    .usage = (char*)mpGapHelp,
    .help = "Contains commands for advertising, scanning, connecting, pairing or disconnecting."
};

const cmd_tbl_t mGattCmd =
{
    .name = "gatt",
    .maxargs = 11,
    .repeatable = 1,
    .cmd = ShellGatt_Command,
    .usage = (char*)mpGattHelp,
    .help = "Contains commands for service discovery, read, write, notify and indicate. "
            "Values in hex must be formatted as 0xXX..X"
};

const cmd_tbl_t mGattDbCmd =
{
    .name = "gattdb",
    .maxargs = 11,
    .repeatable = 1,
    .cmd = ShellGattDb_Command,
    .usage = (char*)mpGattDbHelp,
    .help = "Contains commands for adding services, reading and writing characteristics on the local database."
            "Values in hex must be formatted as 0xXX..X"
};

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
extern gapAdvertisingParameters_t   gAdvParams;
extern gapPairingParameters_t       gPairingParameters;
/************************************************************************************
 *************************************************************************************
 * Private functions prototypes
 *************************************************************************************
 ************************************************************************************/

static void BleApp_Config ();
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief    Initializes application specific functionality before the BLE stack init.
*
********************************************************************************** */
void BleApp_Init(void)
{
    /* UI */
    shell_init("Kinetis BLE Shell>");
    shell_register_function((cmd_tbl_t *)&mGapCmd);
    shell_register_function((cmd_tbl_t *)&mGattCmd);
    shell_register_function((cmd_tbl_t *)&mGattDbCmd);
}

/*! *********************************************************************************
 * \brief        Handles BLE generic callback.
 *
 * \param[in]    pGenericEvent    Pointer to gapGenericEvent_t.
 ********************************************************************************** */
void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
    BleConnManager_GenericEvent(pGenericEvent);
    
    if (pGenericEvent->eventType == gInitializationComplete_c)
    {
        BleApp_Config();
        return;
    }
    
    if (!mSupressEvents)
    {
        ShellGap_GenericCallback(pGenericEvent);
    }
    else
    {
        mSupressEvents--;
    }

}

/************************************************************************************
 *************************************************************************************
 * Private functions
 *************************************************************************************
 ************************************************************************************/

/*! *********************************************************************************
 * \brief        Configures BLE Stack after initialization. Usually used for
 *               configuring advertising, scanning, white list, services, et al.
 *
 ********************************************************************************** */
static void BleApp_Config (void)
{
    /* Configure GAP */
    Gap_SetAdvertisingParameters(&gAdvParams);
    Gap_ReadPublicDeviceAddress();
    Gap_SetDefaultPairingParameters(&gPairingParameters);
    mSupressEvents += 2;

    /* Register for callbacks*/
    App_RegisterGattServerCallback(ShellGatt_ServerCallback);
    App_RegisterGattClientProcedureCallback(ShellGatt_ClientCallback);
    App_RegisterGattClientIndicationCallback(ShellGatt_IndicationCallback);
    App_RegisterGattClientNotificationCallback(ShellGatt_NotificationCallback);
    
    /* Adding GAP and GATT serices in the database */
    ShellGattDb_Init();

}
/*! *********************************************************************************
 * @}
 ********************************************************************************** */
