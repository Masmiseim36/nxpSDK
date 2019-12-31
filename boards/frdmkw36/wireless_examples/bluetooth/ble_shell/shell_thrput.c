/*! *********************************************************************************
 * \addtogroup SHELL THRPUT
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* This file is the source file for the THRPUT Shell module
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
 *************************************************************************************
 * Include
 *************************************************************************************
 ************************************************************************************/
/* Framework / Drivers */
#include "TimersManager.h"
#include "FunctionLib.h"
#include "fsl_os_abstraction.h"
#include "shell.h"
#include "Panic.h"
#include "MemManager.h"
#include "board.h"

/* BLE Host Stack */
#include "ble_shell.h"
#include "gatt_interface.h"
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gatt_db_app_interface.h"
#include "gatt_db_dynamic.h"
#include "gap_interface.h"
#include "l2ca_interface.h"

#include "shell_gattdb.h"
#include "ApplMain.h"
#include "shell_thrput.h"
#include "dynamic_gatt_database.h"

#include <string.h>
#include <stdlib.h>

#if defined(MULTICORE_APPLICATION_CORE) && (MULTICORE_APPLICATION_CORE)
    #define UUID128(name, ...) extern uint8_t name[16];
    #include "gatt_uuid128.h"
    #undef UUID128
    #include "erpc_host.h"
#else
    #define UUID128(name, ...) static uint8_t name[16] = { __VA_ARGS__ };
    #include "gatt_uuid128.h"
    #undef UUID128
#endif

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/
#define mShellThrBufferSizeMax_c           (244U)

#define mShellThrTxInterval_c              (0U) /* ms */

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
/* Command structure type definition */
typedef struct thrCmds_tag
{
    char*       name;
    int8_t      (*cmd)(uint8_t argc, char * argv[]);
}thrCmds_t;

/* GAP roles type definition */
typedef enum thrGapRoles_tag
{
    thrNone = 0,
    thrCentral,
    thrPeripheral,
} thrGapRoles_t;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static void ShellThr_StartThroughputTest(thrGapRoles_t role, deviceId_t peerId);
static void ShellThr_PrintReport(appCallbackParam_t pParam);
static void ShellThr_CheckResults(void* p);
static bool_t ShellThr_CheckScanEvent(gapScannedDevice_t* pData);
static bool_t ShellThr_MatchDataInAdvElementList(gapAdStructure_t *pElement, void *pData, uint8_t iDataLen);
#if mShellThrTxInterval_c
static void ShellThr_TxTimerCallback(void *p);
#endif
static void ShellThr_SendData(void *p);

/* LE Callbacks */
void ShellThr_AdvertisingCallback(gapAdvertisingEvent_t* pAdvertisingEvent);
void ShellThr_ConnectionCallback(deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent);
void ShellThr_ScanningCallback (gapScanningEvent_t* pScanningEvent);

/* Shell API Functions */
static int8_t ShellThr_SetParams(uint8_t argc, char * argv[], deviceId_t peerId);
static int8_t ShellThr_Start(uint8_t argc, char * argv[]);
static int8_t ShellThr_Stop(uint8_t argc, char * argv[]);
static bool_t ShellThr_IsTestInProgress(void);

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/* Throughput test shell commands */
static const thrCmds_t mThrShellCmds[] =
{
    {"start",         ShellThr_Start},
    {"stop",          ShellThr_Stop},
};

/* GAP role */
static thrGapRoles_t mThrShellRole = thrNone;

/* Adverising status */
static bool_t mAdvOn = FALSE;

/* Scanning status */
static bool_t mScanningOn = FALSE;

/* GAP connection params */
static gapConnectionParameters_t mConnectionParams;

/* pair device to connect to was found */
static bool_t   mFoundDeviceToConnect = FALSE;

/* stores the last received notifications count */
static uint16_t mLastNotifRecv[gAppMaxConnections_c];

/* Scanning and Advertising Data */
static const uint8_t adData0[1] =  { (uint8_t)(gLeGeneralDiscoverableMode_c | gBrEdrNotSupported_c) };
static const gapAdStructure_t advScanStruct[3] =
{
    {
        .length = NumberOfElements(adData0) + 1,
        .adType = gAdFlags_c,
        .aData = (uint8_t *)adData0
    },
    {
        .length = NumberOfElements(uuid_service_throughput) + 1,
        .adType = gAdComplete128bitServiceList_c,
        .aData = (uint8_t *)uuid_service_throughput
    },
    {
        .adType = gAdShortenedLocalName_c,
        .length = 8,
        .aData = (uint8_t*)"THR_PER"
    }
};

static gapAdvertisingData_t gThrAdvertisingData =
{
    NumberOfElements(advScanStruct),
    (void *)advScanStruct
};

static gapScanResponseData_t gThrScanRspData =
{
    0,
    NULL
};

/* Default Advertising Parameters */
static gapAdvertisingParameters_t mAdvParams =
{
    /* minInterval */         1600 /* 1 s */,
    /* maxInterval */         1600 /* 1 s */,
    /* advertisingType */     gAdvConnectableUndirected_c,
    /* addressType */         gBleAddrTypePublic_c,
    /* directedAddressType */ gBleAddrTypePublic_c,
    /* directedAddress */     {0, 0, 0, 0, 0, 0},
    /* channelMap */          (gapAdvertisingChannelMapFlags_t) (gAdvChanMapFlag37_c | gAdvChanMapFlag38_c | gAdvChanMapFlag39_c),
    /* filterPolicy */        gProcessAll_c
};

/* Default Connection Request Parameters */
static gapConnectionRequestParameters_t mConnReqParams =
{
    .scanInterval = gGapScanIntervalDefault_d,
    .scanWindow = gGapScanWindowDefault_d,
    .filterPolicy = (uint8_t)gUseDeviceAddress_c,
    .ownAddressType = gBleAddrTypePublic_c,
    .connIntervalMin = gGapDefaultMaxConnectionInterval_d,
    .connIntervalMax = gGapDefaultMaxConnectionInterval_d,
    .connLatency = gGapConnLatencyMin_d,
    .supervisionTimeout = gGapConnSuperTimeoutMax_d,
    .connEventLengthMin = gGapConnEventLengthMin_d,
    .connEventLengthMax = gGapConnEventLengthMax_d,
    .initiatingPHYs = gLePhy1MFlag_c
};

static gapScanningParameters_t mAppScanParams =
{
    /* type */              gScanTypePassive_c,
    /* interval */          gGapScanIntervalDefault_d,
    /* window */            gGapScanWindowDefault_d,
    /* ownAddressType */    gBleAddrTypePublic_c,
    /* filterPolicy */      (uint8_t)gScanAll_c,
    /* scanning PHYs */     (uint8_t)gLePhy1MFlag_c
};

static uint32_t mTxPktCnt[gAppMaxConnections_c];

/* Throughput test statistics */
static thrStatistics_t gThrStatistics[gAppMaxConnections_c];

/* Throughput test start is delayed until updating MTU */
static bool_t mThrWaitMtuExchange = FALSE;

/* Timer used by the throughput test */
static tmrTimerID_t gThroughputTestTimerId = gTmrInvalidTimerID_c;

/* Pointer to throughput data */
static uint8_t* pDummyTestData = NULL;
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
 * \brief        Throughput test command
 *
 * \param[in]    argc    shell argument count
 *
 * \param[in]    argv    shell argument value
 *
 * \return       -
 ********************************************************************************** */
int8_t ShellThrput_Command(uint8_t argc, char * argv[])
{
    uint8_t i;
    int8_t status = CMD_RET_USAGE;

    /* Must have at least one argument */
    if (argc > 1U)
    {
        for (i=0; i<NumberOfElements(mThrShellCmds); i++)
        {
            if(0 == strcmp((char*)argv[1], mThrShellCmds[i].name) )
            {
                /* Handle the Throughput command in the appropriate callback */
                status = mThrShellCmds[i].cmd(argc-2U, (char **)(&argv[2]));
                break;
            }
        }
    }

    return status;
}

/*! *********************************************************************************
 * \brief        Handles BLE generic callback.
 *
 * \param[in]    pGenericEvent    Pointer to gapGenericEvent_t.
 ********************************************************************************** */
void ShellThr_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
    switch(pGenericEvent->eventType)
    {
    case gExtAdvertisingParametersSetupComplete_c:
    case gAdvertisingParametersSetupComplete_c:
        {
            /* Parameters were successfully set. Go on setting advertising data */
            if (gBleSuccess_c != Gap_SetAdvertisingData(&gThrAdvertisingData, &gThrScanRspData))
            {
                shell_write("\r\n-->  GAP Event: Failed setting advertising data!");
            }
        }
        break;

    case gExtAdvertisingDataSetupComplete_c:
    case gAdvertisingDataSetupComplete_c:
        {
            /* Parameters were successfully set. Go on setting advertising data */
            if (gBleSuccess_c != App_StartAdvertising(ShellThr_AdvertisingCallback, ShellThr_ConnectionCallback))
            {
                shell_write("\r\n-->  GAP Event: Failed starting advertising!");
            }
        }
        break;

    case gTxEntryAvailable_c:
#if (mShellThrTxInterval_c == 0)
        {
            deviceId_t peerId = pGenericEvent->eventData.deviceId;
            (void)App_PostCallbackMessage(ShellThr_SendData, (uint8_t*)((uint32_t)peerId));
        }
#endif
        break;

    default:
        ; /* Other Generic Event */
        break;
    }
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
 * \brief        Configure the throughput test variables
 *
 * \param[in]    argc    shell argument count
 *
 * \param[in]    argv    shell argument value
 *
 * \param[in]    peerId  Connected peer ID for which the throughput parameters are updated
 *
 * \return       -
 ********************************************************************************** */
static int8_t ShellThr_SetParams(uint8_t argc, char * argv[], deviceId_t peerId)
{
    int8_t result = CMD_RET_SUCCESS;
    uint32_t i = 0;

    /* Search for Throughput parameters */
    /* Peripheral device configures the Tx parameters: payload size and number of packets */
    /* Central device congures the connection interval */
    while (argc > i)
    {
        /* Get the number of packets to be sent */
        if(0 == strcmp((char*)argv[i], "-c") && (mThrShellRole == thrPeripheral))
        {
            i++;
            gThroughputConfig[peerId].buffCnt = (uint16_t)BleApp_atoi(argv[i]);
            i++;
        }
        /* Get the size of packet's payload */
        else if(0 == strcmp((char*)argv[i], "-s") && (mThrShellRole == thrPeripheral))
        {
            i++;
            uint8_t newBufferSize = (uint8_t)BleApp_atoi(argv[i]);

            /* If the connection is already created and the buffer is set to a larger value than max MTU,
               trigger a new MTU Exchange */
            if ((peerId != gInvalidDeviceId_c) && (peerId < gAppMaxConnections_c))
            {
                uint16_t maxMtu = 0;

                (void)Gatt_GetMtu(peerId, &maxMtu);
                if ((newBufferSize > gAttMaxNotifIndDataSize_d(maxMtu)) &&
                    (newBufferSize <= gAttMaxNotifIndDataSize_d(gAttMaxMtu_c)))
                {
                    (void)GattClient_ExchangeMtu(peerId);
                    mThrWaitMtuExchange = TRUE;
                }
                gThroughputConfig[peerId].bTestInProgress = TRUE;

                gThroughputConfig[peerId].buffSz = newBufferSize;
                i++;
                if(gThroughputConfig[peerId].buffSz > mShellThrBufferSizeMax_c)
                {
                    gThroughputConfig[peerId].buffSz = mShellThrBufferSizeMax_c;
                }
            }
        }
        /* Get the connection interval */
        else if(0 == strcmp((char*)argv[i], "-ci") && (mThrShellRole == thrCentral))
        {
            /* User must pass both min and max connection interval */
            i++;
            mConnReqParams.connIntervalMin = (uint16_t)BleApp_atoi(argv[i]);
            if (mConnReqParams.connIntervalMin < gGapConnIntervalMin_d)
            {
                mConnReqParams.connIntervalMin = gGapConnIntervalMin_d;
            }

            i++;
            mConnReqParams.connIntervalMax = (uint16_t)BleApp_atoi(argv[i]);
            if (mConnReqParams.connIntervalMax > gGapConnIntervalMax_d)
            {
                mConnReqParams.connIntervalMax = gGapConnIntervalMax_d;
            }
            i++;
        }
        else
        {
            /* Return command usage if unknown arguments were passed */
            result = CMD_RET_USAGE;
            break;
        }
    }

    /* Print current Throughput parameters */
    if (result == CMD_RET_SUCCESS)
    {
        shell_write("\r\nCurrent config:\r\n ");
        if (mThrShellRole == thrPeripheral)
        {
            shell_writeDec(gThroughputConfig[peerId].buffCnt);
            shell_write(" buffers of ");
            shell_writeDec(gThroughputConfig[peerId].buffSz);
            shell_write(" bytes\r\n");
        }
        else
        {

            shell_write("Connection interval [min-max]: ");
            shell_writeDec(mConnReqParams.connIntervalMin);
            shell_write("-");
            shell_writeDec(mConnReqParams.connIntervalMax);
        }
        shell_write("\r\n");
    }

    return result;
}

/*! *********************************************************************************
 * \brief        Starts the throughput test
 *
 * \param[in]    argc    shell argument count
 *
 * \param[in]    argv    shell argument value
 *
 * \return       -
 ********************************************************************************** */
static int8_t ShellThr_Start(uint8_t argc, char * argv[])
{
    bleResult_t result;
    uint16_t    handle;
    int8_t      status = CMD_RET_SUCCESS;
    uint8_t     aInitialValue[] = {0x00};
    deviceId_t  peerId = gInvalidDeviceId_c;
    uint8_t     i = 0U;

    /* If device is connected, the first argument must be the peer ID */
    if (gActiveConnections != 0U)
    {
        peerId = (deviceId_t)BleApp_atoi(argv[i++]);
    }

    if (0 == strcmp((char*)argv[i], "rx"))
    {
        mThrShellRole = thrCentral;
    }
    else if (0 == strcmp((char*)argv[i], "tx")) /* GAP Peripheral */
    {
        mThrShellRole = thrPeripheral;
    }
    else
    {
        status = CMD_RET_USAGE;
    }

    i++;
    if (status == CMD_RET_SUCCESS)
    {
        if ((argc - i) > 0U)
        {
            if (peerId == gInvalidDeviceId_c)
            {
                /* User must specify the connection for which the throughput config parameters are updated */
                shell_write("Invalid peer ID!\r\n");
                status = CMD_RET_FAILURE;
            }
            else
            {
                status = ShellThr_SetParams(argc - i,  (char **)(&argv[i]), peerId);
            }
        }
    }

    if (status == CMD_RET_SUCCESS)
    {
        if ((peerId != gInvalidDeviceId_c) && (IS_CONNECTED(peerId) != 0U))
        {
            if (mThrWaitMtuExchange == FALSE)
            {
                /* Allready in a connection */
                ShellThr_StartThroughputTest(mThrShellRole, peerId);
            }
        }
        else
        {
            if (mThrShellRole == thrCentral)
            {
                /* Start scanning if not in a connection */
                (void)App_StartScanning(&mAppScanParams, ShellThr_ScanningCallback, gGapDuplicateFilteringEnable_c, gGapScanContinuously_d, gGapScanPeriodicDisabled_d);
            }
            else
            {
                /* Add service */
                result = GattDbDynamic_AddPrimaryServiceDeclaration(0, gBleUuidType128_c,
                                                                    (void*)uuid_service_throughput,
                                                                    &handle);
                if (result == gBleSuccess_c)
                {
                    /* Add characteristic */
                    result = GattDbDynamic_AddCharDescriptor(gBleUuidType128_c,
                                                            (void*)uuid_throughput_stream,
                                                            (uint16_t)sizeof(aInitialValue),
                                                            aInitialValue,
                                                            (gattAttributePermissionsBitFields_t)gPermissionFlagReadable_c,
                                                            &handle);
                }

                if (result == gBleSuccess_c)
                {
                    /* Handle gAdvertisingParametersSetupComplete_c and gAdvertisingDataSetupComplete_c */
                    gUseShellThrGenericCb = TRUE;

                    /* Set advertising parameters*/
                    result = Gap_SetAdvertisingParameters(&mAdvParams);
                }
                if (result != gBleSuccess_c)
                {
                    status = CMD_RET_FAILURE;
                }
            }
        }
    }

    return status;
}

/*! *********************************************************************************
 * \brief        Stops the throughput test
 *
 * \param[in]    argc    shell argument count
 *
 * \param[in]    argv    shell argument value
 *
 * \return       -
 ********************************************************************************** */
static int8_t ShellThr_Stop(uint8_t argc, char * argv[])
{
    tmrErrCode_t tmrStatus;
    deviceId_t peerId = gInvalidDeviceId_c;

    if (argc > 0U)
    {
        peerId = (deviceId_t)BleApp_atoi(argv[0]);
    }

    if ((peerId < gAppMaxConnections_c) && (gThroughputConfig[peerId].bTestInProgress))
    {
        /* Stop Advertising if needed */
        if (mAdvOn)
        {
            (void)Gap_StopAdvertising();
        }

        /* Stop Scanning if needed */
        if (mScanningOn)
        {
            (void)Gap_StopScanning();
        }

        /* Disconnect */
        (void)Gap_Disconnect(peerId);

        /* Stop timer if all throughput instances are stopped */
        if (ShellThr_IsTestInProgress() == FALSE)
        {
            tmrStatus = TMR_StopTimer(gThroughputTestTimerId);
            if (tmrStatus != gTmrSuccess_c)
            {
                shell_write("ERROR: Invalid timer ID\r\n");
            }

            /* Free data */
            (void)MEM_BufferFree(pDummyTestData);
            pDummyTestData = NULL;
            }

        shell_write("Throughput test stopped.\r\n");
        gThroughputConfig[peerId].bTestInProgress = FALSE;
        gUseShellThrGenericCb = FALSE;
    }

    return CMD_RET_SUCCESS;
}

/*! *********************************************************************************
 * \brief        BLE Tx timer callback
 *
 * \param[in]    p  ignored
 *
 * \return       -
 ********************************************************************************** */
#if mShellThrTxInterval_c
static void ShellThr_TxTimerCallback(void *p)
{
    (void)App_PostCallbackMessage(ShellThr_SendData, p);
}
#endif

/*! *********************************************************************************
 * \brief        GAP peripheral send data function. Called from timer callback.
 *
 * \param[in]    p  ignored
 *
 * \return       -
 ********************************************************************************** */
static void ShellThr_SendData(void *p)
{
    bleResult_t result;
    union
    {
        deviceId_t     peerId;
        void*          p;
    } inParam;

    inParam.p = p;

    if((pDummyTestData != NULL) && (gThroughputConfig[inParam.peerId].bTestInProgress))
    {
        /* If all the packets were sent, stop timer and free buffer */
        if(mTxPktCnt[inParam.peerId] >= gThroughputConfig[inParam.peerId].buffCnt)
        {
            gThroughputConfig[inParam.peerId].bTestInProgress = FALSE;
            mTxPktCnt[inParam.peerId] = 0U;

            if (ShellThr_IsTestInProgress() == FALSE)
            {
            /* stop timer */
#if mShellThrTxInterval_c

                (void)TMR_StopTimer(gThroughputTestTimerId);

#endif
                (void)MEM_BufferFree(pDummyTestData);
                pDummyTestData = NULL;
            }

            /* Inform user that test is finished */
            shell_write("\r\nThroughput test with peer ");
            shell_writeDec(inParam.peerId);
            shell_write(" has finished.\r\n");
        }
        else
        {
            uint32_t temp = mTxPktCnt[inParam.peerId];
            /* There are still packets to send */
            FLib_MemCpy(pDummyTestData, &temp, sizeof(temp));

            /* Send data in a GATT notification message */
            result = GattServer_SendInstantValueNotification(inParam.peerId, value_throughput_stream, gThroughputConfig[inParam.peerId].buffSz, pDummyTestData);

            if(gBleSuccess_c == result)
            {
                /* If the message was sent with success, increment packet counter */
                mTxPktCnt[inParam.peerId]++;
            }
#if (mShellThrTxInterval_c == 0)
            else if (result == gBleOverflow_c)
            {
                /* Tx Overflow. Stop Tx and restart when gTxEntryAvailable_c event is received.
                   Do not increment the dropped packet count. */
            }
#endif
            else
            {
                /* Otherwise, increment dropped packets */
                gThrStatistics[inParam.peerId].droppedPackets++;
            }

#if (mShellThrTxInterval_c == 0)
            if (result != gBleOverflow_c)
            {
                (void)App_PostCallbackMessage(ShellThr_SendData, p);
            }
#endif
        }
    }
}

/*! *********************************************************************************
 * \brief        Handles BLE Advertising callback from host stack.
 *
 * \param[in]    pAdvertisingEvent    Pointer to gapAdvertisingEvent_t.
 ********************************************************************************** */
void ShellThr_AdvertisingCallback
(
gapAdvertisingEvent_t* pAdvertisingEvent
)
{
    /* Handles advertising events */
    shell_write("\r\n-->  GAP Event: Advertising ");

    switch (pAdvertisingEvent->eventType)
    {
    case gAdvertisingStateChanged_c:
        {
            /* Update advertising state */
            mAdvOn = !mAdvOn;

            if (mAdvOn)
            {
                shell_write("started.\r\n");
            }
            else
            {
                shell_write("stopped.\r\n");
            }
            break;
        }

    case gAdvertisingCommandFailed_c:
        {
            /* An error occured */
            shell_write("state could not be changed.\r\n");
            break;
        }

    default:
        {
            ; /* Other Advertising Event */
        }
        break;
    }
}

/*! *********************************************************************************
 * \brief        Handles BLE Connection callback from host stack.
 *
 * \param[in]    peerDeviceId        Peer device ID.
 * \param[in]    pConnectionEvent    Pointer to gapConnectionEvent_t.
 ********************************************************************************** */
void ShellThr_ConnectionCallback
(
deviceId_t peerDeviceId,
gapConnectionEvent_t* pConnectionEvent
)
{
    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
            SET_NEW_CONN(peerDeviceId);
            shell_write("\r\n-->  GAP Event: Connected to peer ");
            shell_writeDec(peerDeviceId);
            shell_cmd_finished();

            /* Save connection parameters */
            FLib_MemCpy(&mConnectionParams,
                        &pConnectionEvent->eventData.connectedEvent.connParameters,
                        sizeof(gapConnectionParameters_t));

            if (mThrShellRole == thrPeripheral)
            {
                gThroughputConfig[peerDeviceId].bTestInProgress = TRUE;
                (void)GattClient_ExchangeMtu(peerDeviceId);
            }
        }
        break;

        case gConnEvtDisconnected_c:
        {
            CLEAR_CONN(peerDeviceId);
            shell_write("\r\n-->  GAP Event: Disconnected\r\n");
        }
        break;

        case gConnEvtLeDataLengthChanged_c:
        ShellThr_StartThroughputTest(mThrShellRole, peerDeviceId);
        break;

        default:
        {
            ; /* Other Connection Event */
        }
        break;
    }
}

/*! *********************************************************************************
 * \brief        Handles BLE Scanning callback from host stack.
 *
 * \param[in]    pScanningEvent    Pointer to gapScanningEvent_t.
 ********************************************************************************** */
void ShellThr_ScanningCallback (gapScanningEvent_t* pScanningEvent)
{
    /* Handle scanning events */
    switch (pScanningEvent->eventType)
    {
    case gDeviceScanned_c:
        {
            /* Search for specific device to connect */
            if( FALSE == mFoundDeviceToConnect )
            {
                mFoundDeviceToConnect = ShellThr_CheckScanEvent(&pScanningEvent->eventData.scannedDevice);
                if (mFoundDeviceToConnect)
                {
                    mConnReqParams.peerAddressType = pScanningEvent->eventData.scannedDevice.addressType;

                    FLib_MemCpy(mConnReqParams.peerAddress,
                                pScanningEvent->eventData.scannedDevice.aAddress,
                                sizeof(bleDeviceAddress_t));

                    /* When found, stop scanning */
                    (void)Gap_StopScanning();
                }
            }
        }
        break;

    case gScanStateChanged_c:
        {
            /* Update scanning state */
            mScanningOn = !mScanningOn;

            shell_write("\r\n->  GAP Event: Scan ");
            if (mScanningOn)
            {
                mFoundDeviceToConnect = FALSE;
                shell_write("started.\r\n");
            }
            else
            {
                shell_write("stopped.\r\n");

                if (mFoundDeviceToConnect)
                {
                    /* If scanning was stopped because the device was found, initiate connection */
                    (void)App_Connect(&mConnReqParams, ShellThr_ConnectionCallback);
                }
            }
            break;
        }

    case gScanCommandFailed_c:
        {
            /* An error occured */
            shell_write("\r\n-->  GAP Event: Scan state could not be changed.");
            break;
        }
    default:
        {
            ; /* Other Scanning Event */
        }
        break;
    }
}

/*! *********************************************************************************
 * \brief        Checks scan event. Search for Temperature Custom Service.
 *
 * \param[in]    pData       Pointer to scanned device information structure
 *
 * \return       TRUE if temperature custom service is found, FALSE otherwise
 ********************************************************************************** */
static bool_t ShellThr_CheckScanEvent(gapScannedDevice_t* pData)
{
    uint8_t index = 0;
    uint8_t name[10];
    uint8_t nameLength = 0;
    bool_t foundMatch = FALSE;

    while (index < pData->dataLength)
    {
        gapAdStructure_t adElement;

        adElement.length = pData->data[index];
        adElement.adType = (gapAdType_t)pData->data[index + 1U];
        adElement.aData = &pData->data[index + 2U];

        /* Search for Temperature Custom Service */
        if ((adElement.adType == gAdIncomplete128bitServiceList_c) ||
            (adElement.adType == gAdComplete128bitServiceList_c))
        {
            foundMatch = ShellThr_MatchDataInAdvElementList(&adElement, &uuid_service_throughput, 16);
        }

        if ((adElement.adType == gAdShortenedLocalName_c) ||
            (adElement.adType == gAdCompleteLocalName_c))
        {
            nameLength = MIN(adElement.length, 10U);
            FLib_MemCpy(name, adElement.aData, nameLength);
        }

        /* Move on to the next AD element type */
        index += adElement.length + sizeof(uint8_t);
    }

    if (foundMatch)
    {
        /* UI */
        shell_write("\r\nFound device: \r\n");
        shell_writeN((char*)name, (uint16_t)nameLength-1U);
        SHELL_NEWLINE();
        shell_writeHex(pData->aAddress, 6);
    }
    return foundMatch;
}

/*! *********************************************************************************
 * \brief        Search if a particular data is present in a specified AD
 *               structure list
 *
 * \param[in]    pElement    Pointer to AD structure list
 *
 * \param[in]    pData       Pointer to data to be searched.
 *
 * \param[in]    iDataLen    AD data lenght
 *
 * \return       TRUE if data is found, FALSE otherwise
 ********************************************************************************** */
static bool_t ShellThr_MatchDataInAdvElementList(gapAdStructure_t *pElement,
                                                 void *pData,
                                                 uint8_t iDataLen)
{
    uint8_t i;
    bool_t status = FALSE;

    /* Check if a data subset is present in an advertising structure */
    for (i = 0; i < (pElement->length - 1U); i += iDataLen)
    {
        if (FLib_MemCmp(pData, &pElement->aData[i], iDataLen))
        {
            /* Data found */
            status = TRUE;
            break;
        }
    }
    return status;
}

/*! *********************************************************************************
 * \brief        Starts throughput test
 *
 * \param[in]    role    GAP role of the device
 ********************************************************************************** */
static void ShellThr_StartThroughputTest(thrGapRoles_t role, deviceId_t peerId)
{
    /* Allocate throughput test timer */
    if(gTmrInvalidTimerID_c == gThroughputTestTimerId)
    {
        gThroughputTestTimerId = TMR_AllocateTimer();
    }

    if (role == thrCentral) /* GAP Central */
    {
        gThrStatistics[peerId].notifRcv = 0;
        gThrStatistics[peerId].firstPacketTs = 0;
        gThrStatistics[peerId].lastPacketTs = 0;

        if(gTmrInvalidTimerID_c != gThroughputTestTimerId)
        {
            uint16_t timeout = 3U * (mConnReqParams.connIntervalMax + mConnReqParams.connIntervalMax/4U);
            (void)TMR_StartIntervalTimer(gThroughputTestTimerId, (uint32_t)timeout, ShellThr_CheckResults, (uint8_t*)((uint32_t)peerId));
            shell_write("Throughput test started.\r\nReceiving packets...\r\n");
            gThroughputConfig[peerId].bTestInProgress = TRUE;
        }
        else
        {
            shell_write("ERROR: Invalid timer ID\r\n");
        }
    }
    else /* GAP Peripheral */
    {
        /* If the initial payload size is higher than the default MTU, exchange MTU */
        uint16_t maxMtu = 0;
        (void)Gatt_GetMtu(peerId, &maxMtu);
        if (maxMtu < gThroughputConfig[peerId].buffSz)
        {
            (void)GattClient_ExchangeMtu(peerId);
            mThrWaitMtuExchange = TRUE;
        }

        mTxPktCnt[peerId] = 0;

        if(NULL == pDummyTestData)
        {
            /* Allocate throughput test buffer */
            if((pDummyTestData = MEM_BufferAlloc(mShellThrBufferSizeMax_c)) == NULL)
            {
                shell_write("ERROR: Cannot allocate buffer.\r\n");
            }
        }

#if mShellThrTxInterval_c
        if (gTmrInvalidTimerID_c != gThroughputTestTimerId)
        {
            (void)TMR_StartIntervalTimer(gThroughputTestTimerId, mShellThrTxInterval_c, ShellThr_TxTimerCallback, (void*)peerId);
            shell_write("Throughput test started.\r\nSending packets...\r\n");
        }
        else
        {
            shell_write("ERROR: Invalid timer ID\r\n");
        }
#else
        if (mThrWaitMtuExchange == FALSE)
        {
            (void)App_PostCallbackMessage(ShellThr_SendData, (uint8_t*)((uint32_t)peerId));
            shell_write("Throughput test started.\r\nSending packets...\r\n");
        }
#endif
          gThroughputConfig[peerId].bTestInProgress = TRUE;

        /* Used to handle the gTxEntryAvailable_c event */
        gUseShellThrGenericCb = TRUE;
    }
}

/*! *********************************************************************************
* \brief  Checks if the throughput test has ended.
*
* \param[in] p           Ignored. Can be NULL.
*
* \return  -
*
* \remarks Called from TMR (callback) context
*
********************************************************************************** */
static void ShellThr_CheckResults(void* p)
{
    for (uint8_t iCount = 0; iCount < gAppMaxConnections_c; iCount++)
    {
        if (gThroughputConfig[iCount].bTestInProgress)
        {
            if(mLastNotifRecv[iCount] == gThrStatistics[iCount].notifRcv)
            {
                /* stop timer */
                if (ShellThr_IsTestInProgress() == FALSE)
                {
                    tmrErrCode_t tmrStatus = TMR_StopTimer(gThroughputTestTimerId);
                    if (tmrStatus != gTmrSuccess_c)
                    {
                        shell_write("ERROR: Invalid timer ID\r\n");
                    }
                }
                /* post message to application task */
                (void)App_PostCallbackMessage(ShellThr_PrintReport, (uint8_t*)((uint32_t)iCount));
            }
            else
            {
                mLastNotifRecv[iCount] = (uint16_t)gThrStatistics[iCount].notifRcv;
            }
        }
    }
}

/*! *********************************************************************************
* \brief  Prints the throughput test report
*
* \param[in] pParam           Ignored. Can be NULL.
*
* \return  -
*
* \remarks This function is executed in the context of the Application Task.
*
********************************************************************************** */
static void ShellThr_PrintReport(appCallbackParam_t pParam)
{
    union
    {
        deviceId_t     peerId;
        void*          p;
    } inParam;

    inParam.p = pParam;

    if (gThrStatistics[inParam.peerId].notifRcv != 0U)
    {
        shell_write("\r\n************************************\r\n"
                        "***** TEST REPORT FOR PEER ID ");
        shell_writeDec(inParam.peerId);
        shell_write(" ****\r\n"
                        "************************************\r\n"
                    "\r\nPackets received: ");
        shell_writeDec(gThrStatistics[inParam.peerId].notifRcv);
        shell_write("\r\nTotal bytes: ");
        shell_writeDec(gThrStatistics[inParam.peerId].bytesRcv);
        shell_write("\r\nReceive duration: ");
        /* Convert to milliseconds */
        uint64_t rxDuration = (gThrStatistics[inParam.peerId].lastPacketTs - gThrStatistics[inParam.peerId].firstPacketTs)/1000U;
        shell_writeDec((uint32_t)rxDuration);
        shell_write(" ms\r\n"
                    "Average bitrate: ");
        /* Convert to bits */
        uint32_t average = gThrStatistics[inParam.peerId].bytesRcv*8U/((uint32_t)rxDuration);
        shell_writeDec((uint32_t)average);
        shell_write(" kbps \r\n"
                    "\r\n************************************\r\n"
                        "********** END OF REPORT ***********\r\n"
                        "************************************\r\n");

        /* Reset counter */
        gThrStatistics[inParam.peerId].notifRcv = 0U;
        mLastNotifRecv[inParam.peerId] = (uint16_t)gThrStatistics[inParam.peerId].notifRcv;

        gThroughputConfig[inParam.peerId].bTestInProgress = FALSE;
        shell_cmd_finished();
    }


    /* Wait untill all throughput instances are finished */
    if (ShellThr_IsTestInProgress() == FALSE)
    {
        gUseShellThrGenericCb = FALSE;

        /* Free timer */
        (void)TMR_FreeTimer(gThroughputTestTimerId);
        gThroughputTestTimerId = gTmrInvalidTimerID_c;
    }
}

/*! *********************************************************************************
 * \brief  Updates statistics. Restart the report timer
 *
 * \remarks This function is executed in the context of the Application Task.
 *
 ********************************************************************************** */
void ShellThr_ProcessNotification(uint32_t len, deviceId_t peerId)
{
    if ( 0U == gThrStatistics[peerId].notifRcv )
    {
        uint32_t timeout = SHELL_THRPUT_TIMEOUT_MS * (uint32_t)gActiveConnections;
        gThrStatistics[peerId].firstPacketTs = TMR_GetTimestamp();
        gThrStatistics[peerId].bytesRcv = 0;
        /* Restart the timer */
        (void)TMR_StartIntervalTimer(gThroughputTestTimerId, timeout, ShellThr_CheckResults, (uint8_t*)((uint32_t)peerId));
    }

    gThrStatistics[peerId].lastPacketTs = TMR_GetTimestamp();
    gThrStatistics[peerId].notifRcv++;
    gThrStatistics[peerId].bytesRcv += len;
}

/*! *********************************************************************************
 * \brief  Signals that the MTU Exchange procedure has been completed.
 *
 * \remarks This function is executed in the context of the Application Task.
 *
 * \param[in]   serverDeviceId  Peer device ID
 ********************************************************************************** */
void ShellThr_MtuExchangeCallback(deviceId_t serverDeviceId)
{
    (void)Gap_UpdateLeDataLength(serverDeviceId, gBleMaxTxOctets_c, gBleMaxTxTime_c);
    if (mThrWaitMtuExchange == TRUE)
    {
        mThrWaitMtuExchange = FALSE;
        ShellThr_StartThroughputTest(mThrShellRole, serverDeviceId);
    }
}

static bool_t ShellThr_IsTestInProgress(void)
{
    bool_t bTestInProgress = FALSE;

    for (uint32_t iCount = 0; iCount < gAppMaxConnections_c; iCount++)
    {
        if (gThroughputConfig[iCount].bTestInProgress)
        {
            bTestInProgress = TRUE;
            break;
        }
    }

    return bTestInProgress;
}
/* EOF */
