/*! *********************************************************************************
 * \addtogroup Bluetooth Shell Application
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
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

#if (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4))
#if (defined(gRFCalibration_d) && (gRFCalibration_d > 0))
#include "board.h"
#include "nxp2p4_xcvr.h"
#include "nxp_xcvr_gfsk_bt_0p5_h_0p5_config.h"
#include "nxp_xcvr_coding_config.h"
#include "Flash_Adapter.h"
#include "controller_interface.h"
#endif
#endif /* (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4)) */

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

#if (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4))
#if (defined(gRFCalibration_d) && (gRFCalibration_d > 0))
/*! Default trimming value for 32MHz crystal8 */
#define DEFAULT_TRIM_VALUE      0x4B
#endif
#endif /* (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4)) */
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

#if (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4))
#if (defined(gRFCalibration_d) && (gRFCalibration_d > 0))
static const char mpCalibrationHelp[]  = "\r\n"
           "calibration enable <0/1 - disable/enable> [frequency in Hz - default 2402000000 Hz]\r\n"
           "calibration getxtaltrim <regRead - 0/1> \r\n"
           "calibration setxtaltrim <trimvalue> <savehwparamas - 0/1> (accepted trim values: 0-127) \r\n"
           "calibration getrssiadj <regRead - 0/1> \r\n"
           "calibration setrssiadj <trimvalue> <savehwparamas - 0/1> (trimValue - signed 8-bit value, represents 1/4 dBm step) \r\n";
#endif /* gRFCalibration_d */
#endif /* (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4)) */

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

#if (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4))
#if (defined(gRFCalibration_d) && (gRFCalibration_d > 0))
static cmd_tbl_t mCalibrationCmd =
{
    .name = "calibration",
    .maxargs = 4,
    .repeatable = 0,
    .cmd = ShellCalibration_Command,
    .usage = (char*)mpCalibrationHelp,
    .help = "Enalbe/Disable RF calibration. Set/Get frequency trim and RSSI adjustment"
};

/* Command structure type definition */
typedef struct calibrationCmds_tag
{
    char*       name;
    int8_t      (*cmd)(uint8_t argc, char * argv[]);
} thrCmds_t;

static int8_t ShellCalibration_EnableRFCalibration(uint8_t argc, char * argv[]);
static int8_t ShellCalibration_GetXtal32MHzTrim(uint8_t argc, char * argv[]);
static int8_t ShellCalibration_SetXtal32MHzTrim(uint8_t argc, char * argv[]);
static int8_t ShellCalibration_GetRssiAdjustment(uint8_t argc, char * argv[]);
static int8_t ShellCalibration_SetRssiAdjustment(uint8_t argc, char * argv[]);

/* Calibration test shell commands */
static const thrCmds_t mCalibrationShellCmds[] =
{
    {"enable",         ShellCalibration_EnableRFCalibration},
    {"getxtaltrim",    ShellCalibration_GetXtal32MHzTrim},
    {"setxtaltrim",    ShellCalibration_SetXtal32MHzTrim},
    {"getrssiadj",     ShellCalibration_GetRssiAdjustment},
    {"setrssiadj",     ShellCalibration_SetRssiAdjustment},
};
#endif /* gRFCalibration_d */
#endif /* (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4)) */
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

#if (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4))
#if (defined(gRFCalibration_d) && (gRFCalibration_d > 0))
static bool_t mCalibrationEnabled = FALSE;
static uint8_t gXtalTrimValue = 0U;
static int8_t  gRssiValue = 0;
#endif /* gRFCalibration_d */
#endif /* (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4)) */

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

#if (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4))
#if (defined(gRFCalibration_d) && (gRFCalibration_d > 0))
    (void)shell_register_function(&mCalibrationCmd);
#endif /* gRFCalibration_d */
#endif /* (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4)) */

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
    /* Adding GAP and GATT services in the database */
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

#if (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4))
#if (defined(gRFCalibration_d) && (gRFCalibration_d > 0))
    hardwareParameters_t hwParams = {0};
    /* get default RSSI and xtal trim values from hardware params structure */
    (void)NV_ReadHWParameters(&hwParams);
    /* Set XTAL trim value */
    if ((uint8_t)hwParams.xtalTrim != 0xFFU)
    {
        (void)XCVR_SetXtalTrim((uint8_t)hwParams.xtalTrim);
    }
    /* Set RSSI Adjustment Value */
    (void)XCVR_SetRssiAdjustment(hwParams.rssiAdjustment);
#endif
#endif
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

#if (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4))
#if (defined(gRFCalibration_d) && (gRFCalibration_d > 0))
/*! *********************************************************************************
 * \brief        RF Calibration command
 *
 * \param[in]    argc    shell argument count
 *
 * \param[in]    argv    shell argument value
 *
 * \return       status of the called callback
 ********************************************************************************** */
int8_t ShellCalibration_Command(uint8_t argc, char * argv[])
{
    uint8_t i;
    int8_t status = CMD_RET_USAGE;

    /* Must have at least one argument */
    if (argc > 1U)
    {
        for (i = 0; i < NumberOfElements(mCalibrationShellCmds); i++)
        {
            if (0 == strcmp((char*)argv[1], mCalibrationShellCmds[i].name) )
            {
                /* Handle the Calibration command in the appropriate callback */
                status = mCalibrationShellCmds[i].cmd(argc - 2U, (char **)(&argv[2]));
                break;
            }
        }
    }

    return status;
}

/*! *********************************************************************************
 * \brief        Handles the calibration enable command.
 *
 * \param[in]    argc     argument count
 *
 * \param[in]    argv     argument value
 *
 * \return       CMD_RET_FAILURE - wrong argument count
                 CMD_RET_USAGE - inappropriate argument value
                 CMD_RET_SUCCESS
 ********************************************************************************** */
static int8_t ShellCalibration_EnableRFCalibration(uint8_t argc, char * argv[])
{
    bool_t enable;
    int8_t status = CMD_RET_SUCCESS;
    uint32_t frequency = 2402000000UL;
    uint32_t enable_val;

    const xcvr_config_t *xcvrConfig = &xcvr_gfsk_bt_0p5_h_0p5_1mbps_full_config;
    const xcvr_coding_config_t *rbmeConfig = &xcvr_ble_coded_s8_config;
    xcvrStatus_t xvr_status;

    if ((gXtalTrimValue == 0U) && (gRssiValue == 0))
    {
        hardwareParameters_t hwParams = {0};
        /* get default RSSI value from hardware params structure */
        (void)NV_ReadHWParameters(&hwParams);
        gRssiValue = hwParams.rssiAdjustment;
        /* default 32MHz Xtal trim */
        gXtalTrimValue = DEFAULT_TRIM_VALUE;
    }

    if ((argc > 0U) && (argc <= 2U))
    {
        enable_val = (uint32_t)BleApp_atoi(argv[0]);
        if (enable_val == 1U)
        {
            enable = TRUE;
        }
        else if (enable_val == 0U)
        {
            enable = FALSE;
        }
        else
        {
            status = CMD_RET_USAGE;
        }
    }
    else
    {
        shell_write("Invalid argument count!\r\n");
        status = CMD_RET_FAILURE;
    }

    if (status == CMD_RET_SUCCESS)
    {
        if (enable)
        {
            /* get frequency value for calibration enablement - if it is provided */
            if (argc == 2U)
            {
                frequency = (uint32_t)BleApp_atoi(argv[1]);
            }

            /* Generate a Continuous Unmodulated Signal when calibration is enabled */
            xvr_status = XCVR_DftTxCW(frequency);
            if (xvr_status == gXcvrSuccess_c)
            {
                mCalibrationEnabled = TRUE;
                shell_write("Calibration enabled!\r\n");

            }
            else
            {
                if (xvr_status == gXcvrInvalidParameters_c)
                {
                    shell_write("Invalid frequency value!\r\n");
                }
                else
                {
                    shell_write("Calibration enablement failed!\r\n");
                }
                status = CMD_RET_FAILURE;
            }
        }
        else
        {
            /* Turn OFF the transmitter */
            XCVR_ForceTxWd();
            /* Initialize the radio for BLE */
            (void)XCVR_Init(&xcvrConfig, &rbmeConfig);
            /* Set XTAL trim value */
            (void)XCVR_SetXtalTrim(gXtalTrimValue);
            /* Set RSSI Adjustment Value */
            (void)XCVR_SetRssiAdjustment(gRssiValue);
            mCalibrationEnabled = FALSE;
            shell_write("Calibration disabled!\r\n");
        }
    }
    return status;
}

/*! *********************************************************************************
 * \brief        Handles the calibration getxtaltrim command.
 *
 * \param[in]    argc     argument count
 *
 * \param[in]    argv     argument value
 *
 * \return       CMD_RET_FAILURE - calibration not enabled or wrong argument count
                 CMD_RET_USAGE - inappropriate argument value
                 CMD_RET_SUCCESS
 ********************************************************************************** */
static int8_t ShellCalibration_GetXtal32MHzTrim(uint8_t argc, char * argv[])
{
    bool_t regRead;
    uint32_t regReadVal;
    uint8_t xTalTrim;
    int8_t status = CMD_RET_SUCCESS;

    if (argc != 1U)
    {
        status = CMD_RET_FAILURE;
    }
    else
    {
        regReadVal = (uint32_t)BleApp_atoi(argv[0]);
        if (regReadVal == 1U)
        {
            regRead = TRUE;
        }
        else if (regReadVal == 0U)
        {
            regRead = FALSE;
        }
        else
        {
            shell_write("Invalid argument!\r\n");
            status = CMD_RET_USAGE;
        }

        if (status == CMD_RET_SUCCESS)
        {
            xTalTrim = BOARD_GetXtal32MhzTrim(regRead);
            shell_write("Current Xtal trim value: ");
            shell_writeDec(xTalTrim);
            shell_write("\r\n");
        }
    }

    return status;
}

/*! *********************************************************************************
 * \brief        Handles the calibration setxtaltrim command.
 *
 * \param[in]    argc     argument count
 *
 * \param[in]    argv     argument value
 *
 * \return       CMD_RET_FAILURE - calibration not enabled or wrong argument count
                 CMD_RET_USAGE - inappropriate argument value
                 CMD_RET_SUCCESS
 ********************************************************************************** */
static int8_t ShellCalibration_SetXtal32MHzTrim(uint8_t argc, char * argv[])
{
    uint32_t trimValue;
    bool_t saveToHwParams;
    uint32_t saveToHwParamsVal;
    uint8_t idx = 0U;
    int8_t status = CMD_RET_SUCCESS;

    if (mCalibrationEnabled == FALSE)
    {
        shell_write("Calibration not enabled!\r\n");
        status = CMD_RET_FAILURE;
    }
    else if (argc != 2U)
    {
        shell_write("Invalid argument count!\r\n");
        status = CMD_RET_FAILURE;
    }
    else
    {
        /* Extract parameters and call function to set frequency offset */
        trimValue = (uint32_t)BleApp_atoi(argv[idx]);
        if (trimValue > 127U)
        {
            shell_write("Invalid argument!\r\n");
            status = CMD_RET_USAGE;
        }
        else
        {
            idx++;
            saveToHwParamsVal = (uint32_t)BleApp_atoi(argv[idx]);
            if (saveToHwParamsVal == 1U)
            {
                saveToHwParams = TRUE;
            }
            else if (saveToHwParamsVal == 0U)
            {
                saveToHwParams = FALSE;
            }
            else
            {
                shell_write("Invalid argument!\r\n");
                status = CMD_RET_USAGE;
            }
        }

        if (status == CMD_RET_SUCCESS)
        {
            gXtalTrimValue = (uint8_t)trimValue;
            BOARD_SetXtal32MHzTrim((uint8_t)trimValue, saveToHwParams);
            shell_writeDec(trimValue);
            shell_write(" - new Xtal trim value set!\r\n");
        }
    }

    return status;
}

/*! *********************************************************************************
 * \brief        Handles the calibration getrssiadj command.
 *
 * \param[in]    argc     argument count
 *
 * \param[in]    argv     argument value
 *
 * \return       CMD_RET_FAILURE - calibration not enabled or wrong argument count
                 CMD_RET_USAGE - inappropriate argument value
                 CMD_RET_SUCCESS
 ********************************************************************************** */
static int8_t ShellCalibration_GetRssiAdjustment(uint8_t argc, char * argv[])
{
    bool_t regRead;
    uint32_t regReadVal;
    int8_t rssi;
    int8_t status = CMD_RET_SUCCESS;

    if (argc != 1U)
    {
        status = CMD_RET_FAILURE;
    }
    else
    {
        /* Extract parameters and call the function to get the current RSSI adjustment value */
        regReadVal = (uint32_t)BleApp_atoi(argv[0]);
        if (regReadVal == 1U)
        {
            regRead = TRUE;
        }
        else if (regReadVal == 0U)
        {
            regRead = FALSE;
        }
        else
        {
            shell_write("Invalid argument!\r\n");
            status = CMD_RET_USAGE;
        }

        if (status == CMD_RET_SUCCESS)
        {
            rssi = BOARD_GetRssiAdjustment(regRead);
            shell_write("Current RSSI value: ");
            shell_writeSignedDec(rssi);
            shell_write("\r\n");
        }
    }

    return status;
}

/*! *********************************************************************************
 * \brief        Handles the calibration setrssiadj command.
 *
 * \param[in]    argc     argument count
 *
 * \param[in]    argv     argument value
 *
 * \return       CMD_RET_FAILURE - calibration not enabled or wrong argument count
                 CMD_RET_USAGE - inappropriate argument value
                 CMD_RET_SUCCESS
 ********************************************************************************** */
static int8_t ShellCalibration_SetRssiAdjustment(uint8_t argc, char * argv[])
{
    int32_t newRssiAdjustmentValue;
    bool_t saveToHwParams;
    uint32_t saveToHwParamsVal;
    uint8_t idx = 0U;
    int8_t status = CMD_RET_SUCCESS;

    if (mCalibrationEnabled == FALSE)
    {
        shell_write("Calibration not enabled!\r\n");
        status = CMD_RET_FAILURE;
    }
    else if (argc != 2U)
    {
        shell_write("Invalid argument count!\r\n");
        status = CMD_RET_FAILURE;
    }
    else
    {
        /* Extract parameters and call function to set the RSSI adjustment value */
        newRssiAdjustmentValue = BleApp_atoi(argv[idx]);
        idx++;
        saveToHwParamsVal = (uint32_t)BleApp_atoi(argv[idx]);
        if (saveToHwParamsVal == 1U)
        {
            saveToHwParams = TRUE;
        }
        else if (saveToHwParamsVal == 0U)
        {
            saveToHwParams = FALSE;
        }
        else
        {
            shell_write("Invalid argument!\r\n");
            status = CMD_RET_USAGE;
        }

        if (status == CMD_RET_SUCCESS)
        {
            gRssiValue = (int8_t)newRssiAdjustmentValue;
            BOARD_SetRssiAdjustment((int8_t)newRssiAdjustmentValue, saveToHwParams);
            shell_writeSignedDec((int8_t)newRssiAdjustmentValue);
            shell_write(" - new RSSI value set!\r\n");
        }
    }

    return status;
}
#endif
#endif /* (defined(CPU_MKW37A512VFT4) || defined(CPU_MKW38A512VFT4)) */
/*! *********************************************************************************
 * @}
 ********************************************************************************** */
