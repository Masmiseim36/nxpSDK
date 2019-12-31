/*! *********************************************************************************
 * \addtogroup Bluetooth Shell Application
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the Bluetooth Shell Application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
 *************************************************************************************
 * Include
 *************************************************************************************
 ************************************************************************************/
/* Framework / Drivers */
#include "EmbeddedTypes.h"
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
#include "shell_thrput.h"

#include "ble_conn_manager.h"
#include "ApplMain.h"
#include "ble_shell.h"

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
#include "erpc_host.h"
#include "dynamic_gatt_database.h"
#endif

/************************************************************************************
*************************************************************************************
* Extern functions
*************************************************************************************
************************************************************************************/
extern void ResetMCU(void);

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
static uint8_t mSupressEvents = 0;

#define mShellThrBufferCountDefault_c      (1000U)
#define mShellThrBufferSizeDefault_c       (gAttMaxNotifIndDataSize_d(gAttMaxMtu_c))
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
static int8_t ShellReset_Command(uint8_t argc, char * argv[]);

/************************************************************************************
 *************************************************************************************
 * Private memory declarations
 *************************************************************************************
 ************************************************************************************/

static const char mpGapHelp[] = "\r\n"
           "gap address [hexvalue] -peer [hexValue] -type \r\n"
           "gap devicename [devicename]\r\n"
           "gap advcfg [-interval intervalInMs]  [-type type]\r\n"
           "gap advstart\r\n"
           "gap advstop\r\n"
           "gap advdata [-erase] [type payload]\r\n"
           "gap scanstart [filter]\r\n"
           "gap scanstop\r\n"
           "gap scancfg [-type type] [-interval intervalInMs] [-window windowInMs] [-phy scanningPhy] [-duration durationInMs] [-period periodInMs]\r\n"
           "gap scandata [-erase] [type payload]\r\n"
           "gap connectcfg [-interval intervalInMs] [-latency latency] [-timeout timeout]\r\n"
           "gap connect scannedDeviceId\r\n"
           "gap disconnect <peerID>\r\n"
           "gap connupdate <peerID> mininterval maxinterval latency timeout\r\n"
           "gap paircfg [-usebonding usebonding] [-seclevel seclevel] [-keyflags flags]\r\n"
           "gap pair <peerID>\r\n"
           "gap enterpin <peerID> pin\r\n"
           "gap bonds [-erase] [-remove deviceIndex]\r\n"
           "gap phy <peerID> [-rx rxPhy] [-tx txPhy] [-o phyOptions]\r\n"
           "gap txpower [adv/conn] [powerLevel]\r\n"
           "gap rssimonitor [peerID]/[peerAddress] [-c]\r\n"
#if defined(BLE_SHELL_AE_SUPPORT) && (BLE_SHELL_AE_SUPPORT)
           "gap extadvstart\r\n"
           "gap extadvstop\r\n"
           "gap extadvcfg [-min minIntervalInMs] [-max maxIntervalInMs] [-type advProperties] [-phy1 primaryPHY] [-phy2 secondaryPHY] [-tx advTxPower] [-sn enableScanNotification]\r\n"
           "gap extadvdata [-erase] [type payload]\r\n"
           "gap extscandata [-erase] [type payload]\r\n"
           "gap periodicstart\r\n"
           "gap periodicstop\r\n"
           "gap periodiccfg [-mininterval minIntervalInMs] [-maxinterval maxIntervalInMs] [-txpower advTxPower]\r\n"
           "gap periodicdata [type] [payload]\r\n"
           "gap periodicsync [-peer peerAddr] [-type peerAddrType]\r\n"
           "gap periodicsyncstop\r\n"
#endif /* BLE_SHELL_AE_SUPPORT */
;

static const char mpGattHelp[] = "\r\n"
           "gatt discover <peerID> [-all] [-service serviceUuid16InHex] \r\n"
           "gatt read <peerID> handle\r\n"
           "gatt write <peerID> handle valueInHex\r\n"
           "gatt writecmd <peerID> handle valueInHex\r\n"
           "gatt notify <peerID> handle\r\n"
           "gatt indicate <peerID> handle\r\n";

static const char mpGattDbHelp[] = "\r\n"
           "gattdb read handle\r\n"
           "gattdb write handle valueInHex\r\n"
           "gattdb addservice serviceUuid16InHex\r\n"
           "gattdb erase\r\n";

static const char mpThrputHelp[]  = "\r\n"
           "thrput start <peerID> tx [-c packet count] [-s payload size]\r\n"
           "thrput start <peerID> rx [-ci min max]\r\n"
           "thrput stop\r\n";

/* Shell */
static cmd_tbl_t mGapCmd =
{
    .name = "gap",
    .maxargs = 11,
    .repeatable = 1,
    .cmd = ShellGap_Command,
    .usage = (char*)mpGapHelp,
    .help = "Contains commands for advertising, scanning, connecting, pairing or disconnecting."
};

static cmd_tbl_t mGattCmd =
{
    .name = "gatt",
    .maxargs = 11,
    .repeatable = 1,
    .cmd = ShellGatt_Command,
    .usage = (char*)mpGattHelp,
    .help = "Contains commands for service discovery, read, write, notify and indicate. "
            "Values in hex must be formatted as 0xXX..X"
};

static cmd_tbl_t mGattDbCmd =
{
    .name = "gattdb",
    .maxargs = 11,
    .repeatable = 1,
    .cmd = ShellGattDb_Command,
    .usage = (char*)mpGattDbHelp,
    .help = "Contains commands for adding services, reading and writing characteristics on the local database."
            "Values in hex must be formatted as 0xXX..X"
};

static cmd_tbl_t mThrputCmd =
{
    .name = "thrput",
    .maxargs = 9,
    .repeatable = 1,
    .cmd = ShellThrput_Command,
    .usage = (char*)mpThrputHelp,
    .help = "Contains commands for setting up and running throughput test"
};

static cmd_tbl_t mResetCmd =
{
    .name = "reset",
    .maxargs = 1,
    .repeatable = 1,
    .cmd = ShellReset_Command,
    .usage = NULL,
    .help = "Reset MCU"
};
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
/* Active connections bitmask */
uint8_t gActiveConnections = 0U;

/* Throughput test configuration structure */
thrConfig_t gThroughputConfig[gAppMaxConnections_c];

/* When enabled generic events are redirected to ShellThr_GenericCallback() */
bool_t gUseShellThrGenericCb = FALSE;

/************************************************************************************
 *************************************************************************************
 * Private functions prototypes
 *************************************************************************************
 ************************************************************************************/

static void BleApp_Config(void);
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
    shell_init("BLE Shell>");

    (void)shell_register_function(&mGapCmd);
    (void)shell_register_function(&mGattCmd);
    (void)shell_register_function(&mGattDbCmd);
    (void)shell_register_function(&mThrputCmd);
    (void)shell_register_function(&mResetCmd);

    TMR_TimeStampInit();

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
    /* Init eRPC host */
    init_erpc_host();
#endif

    /* Initialize throughput structures */
    for (uint32_t iCount = 0; iCount < gAppMaxConnections_c; iCount++)
    {
        /* Throughput test default configuration */
        gThroughputConfig[iCount].buffCnt = mShellThrBufferCountDefault_c;
        gThroughputConfig[iCount].buffSz =  mShellThrBufferSizeDefault_c;
    }
}

/*! *********************************************************************************
 * \brief        Handles BLE generic callback.
 *
 * \param[in]    pGenericEvent    Pointer to gapGenericEvent_t.
 ********************************************************************************** */
void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
    /* Call BLE Conn Manager */
    BleConnManager_GenericEvent(pGenericEvent);

    if (pGenericEvent->eventType == gInitializationComplete_c)
    {
        BleApp_Config();
        return;
    }

    if (mSupressEvents == 0U)
    {
        ShellGap_GenericCallback(pGenericEvent);

        if (gUseShellThrGenericCb == TRUE)
        {
            /* Redirect generic events to ShellThr_GenericCallback */
            ShellThr_GenericCallback(pGenericEvent);
        }
    }
    else
    {
        mSupressEvents--;
    }

}

/*! *********************************************************************************
 * \brief        Parses a string input interpreting its content as a hex number and
 *               writes the value at the input address.
 *
 * \param[in]    pInput         Pointer to string
 *
 * \return       uint8_t        Returns the size of the resulted uint value/array
 ********************************************************************************** */
uint8_t BleApp_ParseHexValue(char* pInput)
{
    uint8_t i, length = (uint8_t)strlen(pInput);
    uint32_t value;
    uint8_t result = 0U;

    /* If the hex misses a 0, return error. Process single character */
    if ((length == 1U) || (length % 2U) == 0U)
    {
        if(0 == strncmp(pInput, "0x", 2))
        {
            length -= 2U;

            /* Save as little endian hex value */
            value = BleApp_AsciiToHex(pInput + 2, FLib_StrLen(pInput+2));

            FLib_MemCpy(pInput, &value, sizeof(uint32_t));

            result = length/2U;
        }
        else if (length > 1U)
        {
            char octet[2];

            /* Save as big endian hex */
            for(i=0U;i < length / 2U; i++)
            {
                FLib_MemCpy(octet, &pInput[i*2U], 2U);

                pInput[i] = (char)BleApp_AsciiToHex(octet, 2U);
            }
            result = length/2U;
        }
        else
        {
            /* Convert single character from ASCII to hex */
            pInput[0] = (char)BleApp_AsciiToHex(pInput, length);
            result = length;
        }
    }

    return result;

}
/**!************************************************************************************************
 * \brief  Converts a string into an integer.
 *
 * \param [in]    pStr       pointer to string
 *
 * \retval     int32_t       integer converted from string.
 * ************************************************************************************************/
int32_t BleApp_atoi
(
    char *pStr
)
{
    int32_t res = 0;
    bool_t bIsNegative = FALSE;

    if (*pStr == '-')
    {
        bIsNegative = TRUE;
        pStr++;
    }

    while ((*pStr != '\0') && (*pStr != ' ') && (*pStr >= '0') && (*pStr <= '9'))
    {
        res = res * 10 + *pStr - '0';
        pStr++;
    }

    if (bIsNegative)
    {
        res = -res;
    }

    return res;
}
/*!*************************************************************************************************
 *  \brief  Converts a string into hex.
 *
 *  \param  [in]    pString     pointer to string
 *  \param  [in]    strLen      string length
 *
 * \return uint32_t value in hex
 **************************************************************************************************/
uint32_t BleApp_AsciiToHex
(
    char *pString,
    uint32_t strLen
)
{
    uint32_t length = strLen;
    uint32_t retValue = 0U;
    int32_t hexDig = 0;
    bool_t validChar;

    /* Loop until reaching the end of the string or the given length */
    while ((length != 0U) && (pString != NULL))
    {
        hexDig = 0;
        validChar = FALSE;

        /* digit 0-9 */
        if (*pString >= '0' && *pString <= '9')
        {
            hexDig = *pString - '0';
            validChar = TRUE;
        }

        /* character 'a' - 'f' */
        if (*pString >= 'a' && *pString <= 'f')
        {
            hexDig = *pString - 'a' + 10;
            validChar = TRUE;
        }

        /* character 'A' - 'B' */
        if (*pString >= 'A' && *pString <= 'F')
        {
            hexDig = *pString - 'A' + 10;
            validChar = TRUE;
        }

        /* a hex digit is 4 bits */
        if (validChar == TRUE)
        {
            retValue = (uint32_t)((retValue << 4U) ^ (uint32_t)hexDig);
        }

        /* Increment position */
        pString++;
        length--;
    }

    return retValue;
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
#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
    if (GattDbDynamic_CreateDatabase() != gBleSuccess_c)
    {
        panic(0,0,0,0);
        return;
    }
#else
    /* Adding GAP and GATT serices in the database */
    (void)ShellGattDb_Init();
#endif /* MULTICORE_APPLICATION_CORE */

    /* Configure GAP */
#if defined(BLE_SHELL_AE_SUPPORT) && (BLE_SHELL_AE_SUPPORT)
    (void)Gap_SetExtAdvertisingParameters(&gExtAdvParams);
    mSupressEvents += 1U;
#endif /* BLE_SHELL_AE_SUPPORT */
    (void)Gap_SetAdvertisingParameters(&gAdvParams);
    (void)Gap_ReadPublicDeviceAddress();
    (void)Gap_SetDefaultPairingParameters(&gPairingParameters);
    mSupressEvents += 2U;

    /* Register for callbacks*/
    (void)App_RegisterGattServerCallback(ShellGatt_ServerCallback);
    (void)App_RegisterGattClientProcedureCallback(ShellGatt_ClientCallback);
    (void)App_RegisterGattClientIndicationCallback(ShellGatt_IndicationCallback);
    (void)App_RegisterGattClientNotificationCallback(ShellGatt_NotificationCallback);

}

/*! *********************************************************************************
 * \brief        Reset MCU.
 *
 ********************************************************************************** */
static int8_t ShellReset_Command(uint8_t argc, char * argv[])
{
    ResetMCU();

    return CMD_RET_SUCCESS;
}
/*! *********************************************************************************
 * @}
 ********************************************************************************** */
