/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017, 2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */

#include "SerialManager.h"
#include "Panic.h"
#include "MemManager.h"
#include "Messaging.h"
#include "FunctionLib.h"
#include "GPIO_Adapter.h"
#include "gpio_pins.h"
#include "board.h"

#include "fsl_device_registers.h"
#include "fsl_os_abstraction.h"
#include "fsl_common.h"
#include <string.h>

#if gNvStorageIncluded_d
#include "NVM_Interface.h"
#endif

#if gSerialMgr_DisallowMcuSleep_d
#include "PWR_Interface.h"
#endif

#if (gSerialMgrUseUart_c)
#include "UART_Serial_Adapter.h"
#endif

#if (gSerialMgrUseIIC_c)
#include "I2C_Serial_Adapter.h"
#endif

#if (gSerialMgrUseSPI_c)
#include "SPI_Serial_Adapter.h"
#endif

#if (gSerialMgrUseUSB_c)
#include "VirtualComInterface.h"
#endif

#if (gSerialMgrUseUSB_VNIC_c)
#include "VirtualNicInterface.h"
#endif

#if gSerialMgrUseFSCIHdr_c
#include "FsciInterface.h"
#include "FsciCommunication.h"
#endif

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#ifndef gSMGR_UseOsSemForSynchronization_c
#define gSMGR_UseOsSemForSynchronization_c  (USE_RTOS)
#endif

#define mSerial_IncIdx_d(idx, max) if( ++(idx) >= (max) ) { (idx) = 0; }

#define mSerial_DecIdx_d(idx, max) if( (idx) > 0 ) { (idx)--; } else  { (idx) = (max) - 1; }

#define gSMRxBufSize_c (gSerialMgrRxBufSize_c + 1UL)

#define mSMGR_DapIsrPrio_c    (0x80U)

#if gSerialMgrUseFSCIHdr_c
#define mSMGR_FSCIHdrLen_c  sizeof(clientPacketHdr_t)
#endif

#if (!defined(GCOV_DO_COVERAGE) || (GCOV_DO_COVERAGE == 0))
#define SERIAL_STATIC static
#define SERIAL_PUBLIC
#else
#define SERIAL_STATIC __WEAK
#define SERIAL_PUBLIC __WEAK
#endif
/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
/*
 * Set the size of the Rx buffer indexes
 */
#if gSMRxBufSize_c < 255
typedef uint8_t bufIndex_t;
#else
typedef uint16_t bufIndex_t;
#endif

/*
 * Defines events recognized by the SerialManager's Task
 * Message used to enque async tx data
 */
typedef struct SerialManagetMsg_tag{
    pSerialCallBack_t txCallback;
    void             *pTxParam;
    const uint8_t    *pData;
    uint16_t          dataSize;
}SerialMsg_t;

/*
 * Defines the serial interface structure
 */
typedef struct serial_tag{
    serialInterfaceType_t  serialType;
    uint8_t                serialChannel;
    /* Rx parameters */
    volatile bufIndex_t    rxIn;
    volatile bufIndex_t    rxOut;
    pSerialCallBack_t      rxCallback;
    void                  *pRxParam;
    uint8_t                rxBuffer[gSMRxBufSize_c];
    /* Tx parameters */
    SerialMsg_t            txQueue[gSerialMgrTxQueueSize_c];
#if gSMGR_UseOsSemForSynchronization_c
    osaSemaphoreId_t       txSyncSemId;
#if gSerialMgr_BlockSenderOnQueueFull_c
    osaSemaphoreId_t       txQueueSemId;
    uint8_t                txBlockedTasks;
#endif
#endif
#if gSerialMgrUseFSCIHdr_c
    fsciLen_t              rxFsciIn;
    fsciLen_t              rxFsciLen;
    uint8_t                rxFsciPkt;
#endif
    volatile uint8_t       txIn;
    volatile uint8_t       txOut;
    volatile uint8_t       txCurrent;
    volatile uint8_t       txNo;
    volatile uint8_t       events;
    volatile uint8_t       state;
}serial_t;

/*
 * SMGR task event flags
 */
typedef enum{
    gSMGR_Rx_c     = (1<<0),
    gSMGR_TxDone_c = (1<<1),
    gSMGR_TxNew_c  = (1<<2)
}serialEventType_t;

/*
 * Common driver data structure union
 */
typedef union smgrDrvData_tag
{
#if (gSerialMgrUseUart_c)
  uartState_t uartState;
#endif /* #if (gSerialMgrUseUart_c) */
#if (gSerialMgrUseIIC_c)
  i2cState_t i2cState;
#endif
#if (gSerialMgrUseSPI_c)
  spiState_t spiState;
#endif
  void *pDrvData;
}smgrDrvData_t;
#endif /* #if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0) */

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
/*
 * SMGR internal functions
 */
void SerialManagerTask(osaTaskParam_t argument);
void  SerialManager_RxNotify(uint32_t i);
void  SerialManager_TxNotify(uint32_t i);
#if gSMGR_UseOsSemForSynchronization_c
static void  Serial_SyncTxCallback(void *pSer);
#endif
static void  Serial_TxQueueMaintenance(serial_t *pSer);
static serialStatus_t Serial_WriteInternal (uint8_t InterfaceId);

#if (gSerialMgrUseSPI_c)
static void Serial_SpiSendDummyByte(uint32_t i);
#endif

/*
 * UART, LPUART and LPSCI specific functions
 */
#if (gSerialMgrUseUart_c)
static void Serial_UartRxCb(uartState_t* state);
static void Serial_UartTxCb(uartState_t* state);
#endif

/*
 * SPI specific functions
 */
#if (gSerialMgrUseSPI_c)
SERIAL_STATIC void SpiMasterDapISR(void);
SERIAL_STATIC void SpiCallback(uint32_t flags, spiState_t* pState);
#endif

/*
 * IIC specific functions
 */
#if (gSerialMgrUseIIC_c)
static void I2cMasterDapISR(void);
static void Serial_I2cRxCb(i2cState_t* state);
static void Serial_I2cTxCb(i2cState_t* state);
#endif

#endif /* #if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0) */

#if defined(FWK_SMALL_RAM_CONFIG)
void FwkInit(void);
#endif

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))

/*
 * RTOS objects definition
 */
#if defined(FWK_SMALL_RAM_CONFIG)
extern osaEventId_t  gFwkCommonEventId;
extern osaTaskId_t gFwkCommonTaskId;
#define gSerialManagerTaskId gFwkCommonTaskId
#define mSMTaskEventId gFwkCommonEventId

#else

static OSA_TASK_DEFINE( SerialManagerTask, gSerialTaskPriority_c, 1, gSerialTaskStackSize_c, FALSE );
static osaTaskId_t gSerialManagerTaskId;
static osaEventId_t        mSMTaskEventId;
#endif /* defined(FWK_SMALL_RAM_CONFIG) */

/*
 * SMGR internal data
 */
static serial_t      mSerials[gSerialManagerMaxInterfaces_c];
static smgrDrvData_t mDrvData[gSerialManagerMaxInterfaces_c];

/*
 * Default configuration for IIC driver
 */
#if (gSerialMgrUseIIC_c)
gpioInputPinConfig_t mI2cMasterDapCfg = {
    .gpioPort = gI2cMasterDap_Port_d,
    .gpioPin = gI2cMasterDap_Pin_d,
#if gSerialMgrSlaveDapTxLogicOne_c
    .pullSelect = pinPull_Down_c,
    .interruptSelect = pinInt_LogicOne_c,
#else
    .pullSelect = pinPull_Up_c,
    .interruptSelect = pinInt_LogicZero_c,
#endif
};

gpioOutputPinConfig_t mI2cSlaveDapCfg = {
    .gpioPort = gI2cSlaveDap_Port_d,
    .gpioPin = gI2cSlaveDap_Pin_d,
#if gSerialMgrSlaveDapTxLogicOne_c
    .outputLogic = 0,
#else
    .outputLogic = 1,
#endif
    .slewRate = pinSlewRate_Fast_c,
    .driveStrength = pinDriveStrength_Low_c
};
#endif /* #if (gSerialMgrUseIIC_c) */

/*
 * Default configuration for SPI driver
 */
#if (gSerialMgrUseSPI_c)
static spiBusConfig_t gSpiConfig = {
    .bitsPerSec = 1000000U,
    .master = FALSE,
    .clkActiveHigh = TRUE,
    .clkPhaseFirstEdge = TRUE,
    .MsbFirst = TRUE
};

static gpioInputPinConfig_t mSpiMasterDapCfg = {
    .gpioPort = gSpiMasterDap_Port_d,
    .gpioPin = gSpiMasterDap_Pin_d,
#if gSerialMgrSlaveDapTxLogicOne_c
    .pullSelect = pinPull_Down_c,
    .interruptSelect = pinInt_LogicOne_c,
#else
    .pullSelect = pinPull_Up_c,
    .interruptSelect = pinInt_LogicZero_c,
#endif
};


static gpioOutputPinConfig_t mSpiSlaveDapCfg = {
    .gpioPort = gSpiSlaveDap_Port_d,
    .gpioPin = gSpiSlaveDap_Pin_d,
#if gSerialMgrSlaveDapTxLogicOne_c
    .outputLogic = FALSE,
#else
    .outputLogic = TRUE,
#endif
    .slewRate = pinSlewRate_Fast_c,
    .driveStrength = pinDriveStrength_Low_c
};

#endif

#endif /* #if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0)) */

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief   Creates the SerialManager's task and initializes internal data structures
*
********************************************************************************** */
void Serial_InitManager( void )
{
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    static bool initialized = FALSE;

    /* Check if SMGR is already initialized */
    if( FALSE == initialized )
    {
        initialized = TRUE;

        /* Fill the structure with zeros */
        FLib_MemSet( mSerials, 0x00, sizeof(mSerials) );
#if defined(FWK_SMALL_RAM_CONFIG)
        FwkInit();
#else

        mSMTaskEventId = OSA_EventCreate( TRUE );

        assert(mSMTaskEventId);

        gSerialManagerTaskId = OSA_TaskCreate(OSA_TASK(SerialManagerTask), NULL);

        assert(gSerialManagerTaskId);
        NOT_USED(gSerialManagerTaskId);

#endif /* #if defined(FWK_SMALL_RAM_CONFIG) */
    }
#endif /* #if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0) */
}

/*! *********************************************************************************
* \brief   The main task of the Serial Manager
*
* \param[in] initialData unused
*
********************************************************************************** */
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
void SerialManagerTask(osaTaskParam_t argument)
{
    uint16_t i;
    uint8_t ev;

#if defined(FWK_SMALL_RAM_CONFIG)
    {
#else
    osaEventFlags_t  mSMTaskEventFlags = 0;

#if USE_RTOS
    while( 1 )
#endif
    {
        /* Wait for an event. The task will block here. */
#if  gSerialPollingMode_c
        (void)OSA_EventWait(mSMTaskEventId, osaEventFlagsAll_c, FALSE, gSerialPollingInterval_c ,&mSMTaskEventFlags);
#else
        (void)OSA_EventWait(mSMTaskEventId, osaEventFlagsAll_c, FALSE, osaWaitForever_c ,&mSMTaskEventFlags);
#endif /* gSerialPollingMode_c  */
#endif

        SERIAL_DBG_LOG("<--");

        for( i = 0; i < gSerialManagerMaxInterfaces_c; i++ )
        {
            OSA_InterruptDisable();
            ev = mSerials[i].events;
            mSerials[i].events = 0;
            OSA_InterruptEnable();

            if ( (bool)(ev & (uint8_t)gSMGR_Rx_c) &&
                 (NULL != mSerials[i].rxCallback) )
            {
                mSerials[i].rxCallback( mSerials[i].pRxParam );
            }

            if( (bool)(ev & (uint8_t)gSMGR_TxDone_c) )
            {
                Serial_TxQueueMaintenance(&mSerials[i]);
            }

            /* If the Serial is IDLE and there is data to tx */
            /*all judgments are made in Serial_WriteInternal()*/
                (void)Serial_WriteInternal( (uint8_t)i );

#if gSerialMgrUseSPI_c
            /* If the SPI Slave has more data to transmit, restart the transfer */
#if gSerialMgrSlaveDapTxLogicOne_c
            if((bool)(GpioReadPinInput(&mSpiMasterDapCfg)) && (mSerials[i].serialType == gSerialMgrSPIMaster_c) && (0u == mSerials[i].state))
#else
            if( (!GpioReadPinInput(&mSpiMasterDapCfg)) && (mSerials[i].serialType == gSerialMgrSPIMaster_c) && (0 == mSerials[i].state) )
#endif
            {
                if( (0u == mSerials[i].txQueue[mSerials[i].txIn].dataSize) && (NULL == mSerials[i].txQueue[mSerials[i].txIn].txCallback) )
                {
                    Serial_SpiSendDummyByte(i);
                }
            }
#endif

        }
    } /* while(TRUE) */
}
#endif

/*! *********************************************************************************
* \brief   Initialize a communication interface.
*
* \param[in] pInterfaceId   pointer to a location where the interface Id will be stored
* \param[in] interfaceType  the type of the interface: UART/SPI/IIC/USB
* \param[in] instance       the instance of the HW module (ex: if UART1 is used, this value should be 1)
*
* \return The interface number if success or gSerialManagerInvalidInterface_c if an error occurred.
*
********************************************************************************** */
serialStatus_t Serial_InitInterface( uint8_t *pInterfaceId,
                                     serialInterfaceType_t interfaceType,
                                     uint8_t instance )
{
    serialStatus_t status = gSerial_Success_c;
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    uint32_t i;
    serial_t *pSer;

    *pInterfaceId = gSerialMgrInvalidIdx_c;

    for ( i=0; i<gSerialManagerMaxInterfaces_c; i++ )
    {
        pSer = &mSerials[i];

        if ( (pSer->serialType == interfaceType) &&
            (pSer->serialChannel == instance) )
        {
            /* The Interface is already opened. */
            status = gSerial_InterfaceInUse_c;
            break;
        }

        if ( pSer->serialType == gSerialMgrNone_c )
        {
            OSA_InterruptDisable();
            pSer->serialChannel = instance;
            switch ( interfaceType )
            {
            case gSerialMgrUart_c:
#if (defined(gSerialMgrUseUart_c)&&(gSerialMgrUseUart_c > 0)) && (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
                UART_Initialize(instance, &mDrvData[i].uartState);
                mDrvData[i].uartState.pRxData = pSer->rxBuffer;
                UART_InstallRxCalback(instance, Serial_UartRxCb, i);
                UART_InstallTxCalback(instance, Serial_UartTxCb, i);
#endif
                break;

            case gSerialMgrLpuart_c:
#if (defined(gSerialMgrUseUart_c)&&(gSerialMgrUseUart_c > 0)) && (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
                (void)LPUART_Initialize(instance, &mDrvData[i].uartState);
                mDrvData[i].uartState.pRxData = pSer->rxBuffer;
                mDrvData[i].uartState.rxSize = 1;
                (void)LPUART_InstallRxCalback(instance, Serial_UartRxCb, i);
                (void)LPUART_InstallTxCalback(instance, Serial_UartTxCb, i);
#endif
                break;

            case gSerialMgrLpsci_c:
#if (defined(gSerialMgrUseUart_c)&&(gSerialMgrUseUart_c > 0)) && (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
                LPSCI_Initialize(instance, &mDrvData[i].uartState);
                mDrvData[i].uartState.pRxData = pSer->rxBuffer;
                LPSCI_InstallRxCalback(instance, Serial_UartRxCb, i);
                LPSCI_InstallTxCalback(instance, Serial_UartTxCb, i);
#endif
                break;

            case gSerialMgrUSB_c:
#if (defined(gSerialMgrUseUSB_c)&&(gSerialMgrUseUSB_c > 0))
                mDrvData[i].pDrvData = VirtualCom_Init(i);
                if (NULL == mDrvData[i].pDrvData)
                {
                    status =  gSerial_InternalError_c;
                }
#endif
                break;
            case gSerialMgrUSB_VNIC_c:
#if (defined(gSerialMgrUseUSB_VNIC_c)&&(gSerialMgrUseUSB_VNIC_c > 0))
                mDrvData[i].pDrvData = VirtualNic_Init(i);
                if (NULL == mDrvData[i].pDrvData)
                {
                    status = gSerial_InternalError_c;
                }
#endif
                break;
            case gSerialMgrIICMaster_c:
#if (defined(gSerialMgrUseIIC_c)&&(gSerialMgrUseIIC_c > 0))
                mDrvData[i].i2cState.master = TRUE;
                mDrvData[i].i2cState.address = gSerialMgrIICAddress_c;
                I2C_Initialize(instance, &mDrvData[i].i2cState);
                I2C_InstallRxCalback(instance, Serial_I2cRxCb, i);
                I2C_InstallTxCalback(instance, Serial_I2cTxCb, i);
                (void)GpioInstallIsr(I2cMasterDapISR, gGpioIsrPrioNormal_c, mSMGR_DapIsrPrio_c, &mI2cMasterDapCfg);
                GpioInputPinInit(&mI2cMasterDapCfg, 1);
#endif
                break;

            case gSerialMgrIICSlave_c:
#if (defined(gSerialMgrUseIIC_c)&&(gSerialMgrUseIIC_c > 0))

                mDrvData[i].i2cState.master = FALSE;
                mDrvData[i].i2cState.address = gSerialMgrIICAddress_c;
                I2C_Initialize(instance, &mDrvData[i].i2cState);
                mDrvData[i].i2cState.pRxData = &mSerials[i].rxBuffer[mSerials[i].rxIn];
                I2C_InstallRxCalback(instance, Serial_I2cRxCb, i);
                I2C_InstallTxCalback(instance, Serial_I2cTxCb, i);
                (void)GpioOutputPinInit(&mI2cSlaveDapCfg, 1);
#endif
                break;

            case gSerialMgrSPIMaster_c:
#if (defined(gSerialMgrUseSPI_c)&&(gSerialMgrUseSPI_c > 0))
                (void)Spi_Init(instance, &(mDrvData[i].spiState), SpiCallback, (void*)(uint32_t *)i );
                gSpiConfig.master = TRUE;
                mDrvData[i].spiState.signalRxByte = TRUE;
                mDrvData[i].spiState.pRxData = &pSer->rxBuffer[pSer->rxIn];
                (void)Spi_Configure(instance, &gSpiConfig);
                (void)GpioInstallIsr(SpiMasterDapISR, gGpioIsrPrioNormal_c, mSMGR_DapIsrPrio_c, &mSpiMasterDapCfg);
                (void)GpioInputPinInit(&mSpiMasterDapCfg, 1);
#endif
                break;

            case gSerialMgrSPISlave_c:
#if (defined(gSerialMgrUseSPI_c)&&(gSerialMgrUseSPI_c > 0))
            	(void)Spi_Init(instance, &(mDrvData[i].spiState), SpiCallback, (void*)(uint32_t *)i );
                gSpiConfig.master = FALSE;
                mDrvData[i].spiState.signalRxByte = TRUE;
                mDrvData[i].spiState.pRxData = &pSer->rxBuffer[pSer->rxIn];
                (void)Spi_Configure(instance, &gSpiConfig);
                (void)GpioOutputPinInit(&mSpiSlaveDapCfg, 1);
#endif
                break;

                case gSerialMgrUsart_c:
#if (defined(gSerialMgrUseUart_c)&&(gSerialMgrUseUart_c > 0)) && (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
                    USART_Initialize(instance, &mDrvData[i].uartState);
                    mDrvData[i].uartState.pRxData = pSer->rxBuffer;
                    USART_InstallRxCalback(instance, Serial_UartRxCb, i);
                    USART_InstallTxCalback(instance, Serial_UartTxCb, i);
#endif
                    break;

            case gSerialMgrCustom_c:
                /* Nothing to do here. The initialization is done outsinde SerialManager */
                break;

            default:
                status = gSerial_InvalidInterface_c;
                break;
            }

#if (defined(gSMGR_UseOsSemForSynchronization_c)&&(gSMGR_UseOsSemForSynchronization_c > 0))
            if( (status == gSerial_Success_c) &&
                ((pSer->txSyncSemId = OSA_SemaphoreCreate(0)) == NULL) )
            {
                status = gSerial_SemCreateError_c;
            }

#if (defined(gSerialMgr_BlockSenderOnQueueFull_c)&&(gSerialMgr_BlockSenderOnQueueFull_c > 0))
            if( (status == gSerial_Success_c) &&
                ((pSer->txQueueSemId = OSA_SemaphoreCreate( 0)) == NULL) )
            {
                status = gSerial_SemCreateError_c;
            }
#endif /* gSerialMgr_BlockSenderOnQueueFull_c */
#endif /* gSMGR_UseOsSemForSynchronization_c */

            if( status == gSerial_Success_c )
            {
                pSer->serialType = interfaceType;
                *pInterfaceId = (uint8_t)i;
            }
            OSA_InterruptEnable();
            break;
        }
    }

    /* There are no more free interfaces. */
    if( i == gSerialManagerMaxInterfaces_c )
    {
        status = gSerial_MaxInterfacesReached_c;
    }
#else
    (void)interfaceType;
    (void)instance;
    (void)pInterfaceId;
#endif
    return status;
}

/*! *********************************************************************************
* \brief   Transmit a data buffer asynchronously
*
* \param[in] InterfaceId the interface number
* \param[in] pBuf pointer to data location
* \param[in] bufLen the number of bytes to be sent
* \param[in] pSerialRxCallBack pointer to a function that will be called when
*            a new char is available
*
* \return The status of the operation
*
********************************************************************************** */
serialStatus_t Serial_AsyncWrite( uint8_t InterfaceId,
                                  const uint8_t *pBuf,
                                  uint16_t bufLen,
                                  pSerialCallBack_t cb,
                                  void *pTxParam )
{
    serialStatus_t status = gSerial_Success_c;
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    SerialMsg_t *pMsg = NULL;
    serial_t *pSer = &mSerials[InterfaceId];

#if gSerialMgr_ParamValidation_d
    if( (NULL == pBuf) || (0u == bufLen) || (InterfaceId >= gSerialManagerMaxInterfaces_c) ||
        (pSer->serialType == gSerialMgrNone_c) )
    {
        status = gSerial_InvalidParameter_c;
    }
    else
#endif
    {

#if (gSerialMgr_BlockSenderOnQueueFull_c == 0) || ((gSerialMgr_BlockSenderOnQueueFull_c) && (gSMGR_UseOsSemForSynchronization_c))
        osaTaskId_t taskHandler = OSA_TaskGetId();
#endif

#if (gSerialMgr_BlockSenderOnQueueFull_c == 0)
        if( taskHandler == gSerialManagerTaskId )
        {
            Serial_TxQueueMaintenance(pSer);
        }
#endif

        /* Check if slot is free */
        do {
            OSA_InterruptDisable();

            if( (0u == pSer->txQueue[pSer->txIn].dataSize) && (NULL == pSer->txQueue[pSer->txIn].txCallback) && (pSer->txNo < gSerialMgrTxQueueSize_c) )
            {
                pMsg = &pSer->txQueue[pSer->txIn];
                pMsg->dataSize   = bufLen;
                pMsg->pData      = pBuf;
                pMsg->txCallback = cb;
                pMsg->pTxParam   = pTxParam;
                mSerial_IncIdx_d(pSer->txIn, gSerialMgrTxQueueSize_c)
                pSer->txNo++;
            }
#if (gSerialMgr_BlockSenderOnQueueFull_c) && (gSMGR_UseOsSemForSynchronization_c)
            else
            {
                if(taskHandler != gSerialManagerTaskId)
                {
                    pSer->txBlockedTasks++;
                }
            }
#endif
            OSA_InterruptEnable();

            if( pMsg != NULL )
            {
                status = Serial_WriteInternal( InterfaceId );

                if(gSerial_Success_c != status)
                {
                break;
            }
            }
            else
            {
                status = gSerial_OutOfMemory_c;
#if gSerialMgr_BlockSenderOnQueueFull_c
#if gSMGR_UseOsSemForSynchronization_c
                if(taskHandler != gSerialManagerTaskId)
                {
                    (void)OSA_SemaphoreWait(pSer->txQueueSemId, osaWaitForever_c);
                }
                else
#endif
                {
                    Serial_TxQueueMaintenance(pSer);
                }
#else
                break;
#endif
            }
        } while( status != gSerial_Success_c );
    }
#else
    (void)InterfaceId;
    (void)pBuf;
    (void)bufLen;
    (void)cb;
    (void)pTxParam;
#endif /* gSerialManagerMaxInterfaces_c */
    return status;
}


/*! *********************************************************************************
* \brief Transmit a data buffer synchronously. The task will block until the Tx is done
*
* \param[in] pBuf pointer to data location
* \param[in] bufLen the number of bytes to be sent
* \param[in] InterfaceId the interface number
*
* \return The status of the operation
*
********************************************************************************** */
serialStatus_t Serial_SyncWrite( uint8_t InterfaceId,
                                 const uint8_t *pBuf,
                                 uint16_t bufLen )
{
    serialStatus_t status = gSerial_Success_c;
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    pSerialCallBack_t cb = NULL;
    serial_t *pSer = &mSerials[InterfaceId];

#if gSMGR_UseOsSemForSynchronization_c
    /* If the calling task is SMGR do not block on semaphore */
    if( OSA_TaskGetId() != gSerialManagerTaskId )
         cb = Serial_SyncTxCallback;
#endif

    status  = Serial_AsyncWrite(InterfaceId, pBuf, bufLen, cb, pSer);

    if( (gSerial_InvalidParameter_c != status)
#if !gSerialMgr_BlockSenderOnQueueFull_c
       && (gSerial_OutOfMemory_c != status)
#endif
       )
    {
        /* Wait until Tx finishes. The sem will be released by the SMGR task */
#if gSMGR_UseOsSemForSynchronization_c
        if( cb )
        {
            (void)OSA_SemaphoreWait(pSer->txSyncSemId, osaWaitForever_c);
        }
        else
#endif
        {
            while((bool)pSer->state) {}
        }
#if (gSerialMgrUseUart_c)
        switch (pSer->serialType)
        {
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
        case gSerialMgrUart_c:
            while(UART_IsTxActive(pSer->serialChannel)) {}
            break;
#endif
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
        case gSerialMgrLpuart_c:
            while((bool)LPUART_IsTxActive(pSer->serialChannel)) {}
            break;
#endif
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
        case gSerialMgrLpsci_c:
            while(LPSCI_IsTxActive(pSer->serialChannel)) {}
            break;
#endif
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
            case gSerialMgrUsart_c:
                while (USART_IsTxActive(pSer->serialChannel))
                {}
                break;
#endif
            default:
            	/* MISRA rule 16.4 */
                break;
        }
#endif
    }
#else
    (void)pBuf;
    (void)bufLen;
    (void)InterfaceId;
#endif /* gSerialManagerMaxInterfaces_c */
    return status;
}

/*! *********************************************************************************
* \brief   Returns a specified number of characters from the Rx buffer
*
* \param[in] InterfaceId the interface number
* \param[out] pData pointer to location where to store the characters
* \param[in] dataSize the number of characters to be read
* \param[out] bytesRead the number of characters read
*
* \return The status of the operation
*
********************************************************************************** */
serialStatus_t Serial_Read( uint8_t InterfaceId, uint8_t *pData, uint16_t dataSize, uint16_t *bytesRead )
{
    serialStatus_t status = gSerial_Success_c;

    SERIAL_DBG_LOG("<--");

#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    serial_t *pSer = &mSerials[InterfaceId];
    uint16_t i, bytes = 0;

#if gSerialMgr_ParamValidation_d
    if ( (InterfaceId >= gSerialManagerMaxInterfaces_c) || (NULL == pData) || (0u == dataSize) )
    {
        status = gSerial_InvalidParameter_c;
    }
    else
#endif
    {
        /* Copy bytes from the SMGR Rx buffer */
        status = Serial_RxBufferByteCount(InterfaceId, &bytes);

        if( bytes > 0u )
        {
            if( bytes > dataSize )
            {
                bytes = dataSize;
            }

            /* Copy data */
            for( i=0; i<bytes; i++ )
            {
                OSA_InterruptDisable();
                *pData++ = pSer->rxBuffer[pSer->rxOut++];
                if ( pSer->rxOut >= gSMRxBufSize_c )
                {
                    pSer->rxOut = 0;
                }
                OSA_InterruptEnable();
            }

        }

#if gSerialMgrUseUSB_c || gSerialMgrUseUSB_VNIC_c
        /* Aditional processing depending on interface */
        switch ( pSer->serialType )
        {
#if gSerialMgrUseUSB_c
        case gSerialMgrUSB_c:
            VirtualCom_SMReadNotify( mDrvData[InterfaceId].pDrvData );
            break;
#endif

#if gSerialMgrUseUSB_VNIC_c
        case gSerialMgrUSB_VNIC_c:
            VirtualNic_SMReadNotify( mDrvData[InterfaceId].pDrvData );
            break;
#endif

        default:
        	/*MISRA rule 16.4*/
            break;
        }
#endif

        if( bytesRead != NULL )
        {
            *bytesRead = bytes;
        }
    }
#else
    (void)InterfaceId;
    (void)pData;
    (void)dataSize;
    bytesRead = 0;
    (void)bytesRead;
#endif
    return status;
}

/*! *********************************************************************************
* \brief   Returns the number of bytes available in the RX buffer
*
* \param[in] InterfaceId the interface number
* \param[out] bytesCount the number of bytes available
*
* \return The status of the operation
*
********************************************************************************** */
serialStatus_t Serial_RxBufferByteCount( uint8_t InterfaceId, uint16_t *bytesCount )
{
    serialStatus_t status = gSerial_Success_c;
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    bufIndex_t tmp;
#if gSerialMgr_ParamValidation_d
    if ( (InterfaceId >= gSerialManagerMaxInterfaces_c) || (NULL == bytesCount) )
    {
        status = gSerial_InvalidParameter_c;
    }
    else
#endif
    {
        OSA_InterruptDisable();

        tmp = mSerials[InterfaceId].rxIn;
        if( tmp >= mSerials[InterfaceId].rxOut )
        {
            *bytesCount = ((uint16_t)tmp - (uint16_t)mSerials[InterfaceId].rxOut);
        }
        else
        {
            *bytesCount = ((uint16_t)gSMRxBufSize_c - (uint16_t)mSerials[InterfaceId].rxOut + (uint16_t)tmp);
        }

        OSA_InterruptEnable();
    }
#else
    (void)bytesCount;
    (void)InterfaceId;
#endif
    return status;
}

/*! *********************************************************************************
* \brief   Sets a pointer to a function that will be called when data is received
*
* \param[in] InterfaceId the interface number
* \param[in] pfCallBack pointer to the function to be called
* \param[in] pRxParam pointer to a parameter which will be passed to the CB function
*
* \return The status of the operation
*
********************************************************************************** */
serialStatus_t Serial_SetRxCallBack( uint8_t InterfaceId, pSerialCallBack_t cb, void *pRxParam )
{
    serialStatus_t status = gSerial_Success_c;
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
#if gSerialMgr_ParamValidation_d
    if ( InterfaceId >= gSerialManagerMaxInterfaces_c )
    {
        status = gSerial_InvalidParameter_c;
    }
    else
#endif
    {
        mSerials[InterfaceId].rxCallback = cb;
        mSerials[InterfaceId].pRxParam = pRxParam;
    }
#else
    (void)InterfaceId;
    (void)cb;
    (void)pRxParam;
#endif
    return status;
}

/*! *********************************************************************************
* \brief   Set the communication speed for an interface
*
* \param[in] baudRate communication speed
* \param[in] InterfaceId the interface number
*
* \return The status of the operation
*
********************************************************************************** */
serialStatus_t Serial_SetBaudRate( uint8_t InterfaceId, uint32_t baudRate  )
{
    serialStatus_t status = gSerial_Success_c;
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))

#if gSerialMgr_ParamValidation_d
    if ( (InterfaceId >= gSerialManagerMaxInterfaces_c) || (0u == baudRate) )
    {
        status = gSerial_InvalidParameter_c;
    }
    else
#endif
    {
        switch ( mSerials[InterfaceId].serialType )
        {
#if (defined(gSerialMgrUseUart_c)&&(gSerialMgrUseUart_c > 0))
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
        case gSerialMgrUart_c:
            UART_SetBaudrate(mSerials[InterfaceId].serialChannel, baudRate);
            break;
#endif
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
        case gSerialMgrLpuart_c:
            (void)LPUART_SetBaudrate(mSerials[InterfaceId].serialChannel, baudRate);
            break;
#endif
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
        case gSerialMgrLpsci_c:
            LPSCI_SetBaudrate(mSerials[InterfaceId].serialChannel, baudRate);
            break;
#endif
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
            case gSerialMgrUsart_c:
                USART_SetBaudrate(mSerials[InterfaceId].serialChannel, baudRate);
                break;
#endif
#endif /* #if (gSerialMgrUseUart_c) */

#if (defined(gSerialMgrUseIIC_c)&&(gSerialMgrUseIIC_c > 0))
        case gSerialMgrIICMaster_c:
            I2C_MasterSetSpeed(mSerials[InterfaceId].serialChannel, baudRate);
            break;
#endif

#if (defined(gSerialMgrUseSPI_c)&&(gSerialMgrUseSPI_c > 0))
        case gSerialMgrSPIMaster_c:
            gSpiConfig.bitsPerSec = baudRate;
            (void)Spi_Configure(mSerials[InterfaceId].serialChannel, &gSpiConfig);
            break;
#endif

#if (defined(gSerialMgrUseUSB_c)&&(gSerialMgrUseUSB_c > 0))
        case gSerialMgrUSB_c:
            /* Nothing to do here. */
            break;
#endif

        case gSerialMgrCustom_c:
            /* Nothing to do here. */
            break;

        default:
            status = gSerial_InvalidInterface_c;
            break;
        }
    }
#endif
    return status;
}

/*! *********************************************************************************
* \brief   Prints a string to the serial interface
*
* \param[in] InterfaceId the interface number
* \param[in] pString pointer to the string to be printed
* \param[in] allowToBlock specify if the task will wait for the tx to finish or not.
*
* \return The status of the operation
*
********************************************************************************** */
serialStatus_t Serial_Print( uint8_t InterfaceId, const char* pString, serialBlock_t allowToBlock )
{
    serialStatus_t status = gSerial_Success_c;
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    if ( (bool)allowToBlock )
    {
        status = Serial_SyncWrite( InterfaceId, (const uint8_t *)pString, (uint16_t)strlen(pString) );
    }
    else
    {
        status = Serial_AsyncWrite( InterfaceId, (const uint8_t *)pString, (uint16_t)strlen(pString), NULL, NULL );
    }
#else
    (void)pString;
    (void)allowToBlock;
    (void)InterfaceId;
#endif
    return status;
}

/*! *********************************************************************************
* \brief   Prints an number in hexadecimal format to the serial interface
*
* \param[in] InterfaceId the interface number
* \param[in] hex pointer to the number to be printed
* \param[in] len the number of bytes of the number
* \param[in] flags specify display options: comma, space, new line
*
* \return The status of the operation
*
* \remarks The task will waituntil the tx has finished
*
********************************************************************************** */
serialStatus_t Serial_PrintHex( uint8_t InterfaceId,
                                uint8_t *hex,
                                uint8_t len,
                                uint8_t flags )
{
    serialStatus_t status = gSerial_Success_c;
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    uint8_t i=0;
    uint8_t hexString[6]; /* 2 bytes  - hexadecimal display
    1 byte   - separator ( comma)
    1 byte   - separator ( space)
    2 bytes  - new line (\n\r)  */

    if ( !(bool)(flags & (uint8_t)gPrtHexBigEndian_c) )
    {
        hex = hex + (len-1u);
    }

    while ( (bool)(len) && ( gSerial_Success_c == status ) )
    {
        /* start preparing the print of a new byte */
        i=0;
        hexString[i++] = HexToAscii( (uint8_t)(*hex)>>4u );
        hexString[i++] = HexToAscii( (uint8_t)(*hex) );

        if ( (flags & (uint8_t)gPrtHexCommas_c) != 0u )
        {
            hexString[i++] = (uint8_t)',';
        }
        if ( (flags & (uint8_t)gPrtHexSpaces_c) != 0u )
        {
            hexString[i++] = (uint8_t)' ';
        }
        hex = hex + ((bool)(flags & (uint8_t)gPrtHexBigEndian_c) ? 1 : -1);
        len--;

        if ( (len == 0u) && (bool)(flags & (uint8_t)gPrtHexNewLine_c) )
        {
            hexString[i++] = (uint8_t)'\n';
            hexString[i++] = (uint8_t)'\r';
        }

        /* transmit formatted byte */
        status = Serial_SyncWrite( InterfaceId, hexString, (uint8_t)i) ;
    }
#else
    /* Avoid compiler warning */
    (void)hex;
    (void)len;
    (void)InterfaceId;
    (void)flags;
#endif
    return status;
}

/*! *********************************************************************************
* \brief   Prints an unsigned integer to the serial interface
*
* \param[in] InterfaceId the interface number
* \param[in] nr the number to be printed
*
* \return The status of the operation
*
* \remarks The task will waituntil the tx has finished
*
********************************************************************************** */
serialStatus_t Serial_PrintDec( uint8_t InterfaceId, uint32_t nr )
{
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    uint8_t decString[12];
    uint8_t i = sizeof(decString)-1u;

    if ( nr == 0u )
    {
        decString[i] = (uint8_t)'0';
    }
    else
    {
        while ( (bool)nr )
        {
            decString[i] = ((uint8_t)'0' + (uint8_t)(nr % 10u));
            nr = nr / 10u;
            i--;
        }
        i++;
    }

    /* transmit formatted byte */
    return Serial_SyncWrite( InterfaceId, (uint8_t*)&decString[i], (uint16_t)(sizeof(decString)-i) );
#else
    (void)nr;
    (void)InterfaceId;
    return gSerial_Success_c;
#endif
}


/*! *********************************************************************************
* \brief   Configures the enabled hardware modules of the given interface type as a wakeup source from STOP mode
*          It does not configure the interface as wakeup source from LLS or VLLS
           Lowpower module requires updates to set the interface as wakeup source from LLS or VLLS
*
* \param[in] interface type of the modules to configure
*
* \return  gSerial_Success_c if there is at least one module to configure
*          gSerial_InvalidInterface_c otherwise
* \pre
*
* \post
*
* \remarks
*
********************************************************************************** */

serialStatus_t Serial_EnableLowPowerWakeup( serialInterfaceType_t interfaceType )
{
    serialStatus_t status = gSerial_Success_c;
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    uint32_t i;

    for(i=0; i<gSerialManagerMaxInterfaces_c; i++)
    {
        if( mSerials[i].serialType == interfaceType )
        {
            switch(interfaceType)
            {
#if (gSerialMgrUseUart_c)
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
            case gSerialMgrUart_c:
                UART_EnableLowPowerWakeup(mSerials[i].serialChannel);
                break;
#endif
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
            case gSerialMgrLpuart_c:
                (void)LPUART_EnableLowPowerWakeup(mSerials[i].serialChannel);
                break;
#endif
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
            case gSerialMgrLpsci_c:
                LPSCI_EnableLowPowerWakeup(mSerials[i].serialChannel);
                break;
#endif
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
                case gSerialMgrUsart_c:
                    USART_EnableLowPowerWakeup(mSerials[i].serialChannel);
                    break;
#endif
#endif /* #if (gSerialMgrUseUart_c) */
            default:
                status = gSerial_InvalidInterface_c;
                break;
            } /* switch(...) */
        } /* if(...) */
    }/* for(...) */
#endif /* #if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0)) */
    return status;
}

/*! *********************************************************************************
* \brief   Configures the enabled hardware modules of the given interface type as modules without wakeup capabilities
*
* \param[in] interface type of the modules to configure
*
* \return  gSerial_Success_c if there is at least one module to configure
*          gSerial_InvalidInterface_c otherwise
* \pre
*
* \post
*
* \remarks
*
********************************************************************************** */
serialStatus_t Serial_DisableLowPowerWakeup( serialInterfaceType_t interfaceType )
{
    serialStatus_t status = gSerial_Success_c;
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    uint32_t i;

    for(i=0; i<gSerialManagerMaxInterfaces_c; i++)
    {
        if( mSerials[i].serialType == interfaceType )
        {
            switch(interfaceType)
            {
#if (gSerialMgrUseUart_c)
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
            case gSerialMgrUart_c:
                UART_DisableLowPowerWakeup(mSerials[i].serialChannel);
                break;
#endif
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
            case gSerialMgrLpuart_c:
                (void)LPUART_DisableLowPowerWakeup(mSerials[i].serialChannel);
                break;
#endif
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
            case gSerialMgrLpsci_c:
                LPSCI_DisableLowPowerWakeup(mSerials[i].serialChannel);
                break;
#endif
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
            case gSerialMgrUsart_c:
                USART_DisableLowPowerWakeup(mSerials[i].serialChannel);
                break;
#endif
#endif /* #if (gSerialMgrUseUart_c) */
            default:
                status = gSerial_InvalidInterface_c;
                break;
            }
        }
    }
#endif /* #if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0)) */
    return status;
}

/*! *********************************************************************************
* \brief   Decides whether a enabled hardware module of the given interface type woke up the CPU from STOP mode.
*
* \param[in] interface type of the modules to be evaluated as wakeup source.
*
* \return  TRUE if a module of the given interface type was the wakeup source
*          FALSE otherwise
* \pre
*
* \post
*
* \remarks
*
********************************************************************************** */
bool_t Serial_IsWakeUpSource( serialInterfaceType_t interfaceType)
{
    bool_t status = FALSE;

    SERIAL_DBG_LOG("<--");

#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    uint32_t i;

    for(i=0; i<gSerialManagerMaxInterfaces_c; i++)
    {
        if( mSerials[i].serialType == interfaceType )
        {
            switch(interfaceType)
            {
#if (gSerialMgrUseUart_c)
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
            case gSerialMgrUart_c:
                if( (bool)UART_IsWakeupSource(mSerials[i].serialChannel) )
                {
                    status = TRUE;
                }
                break;
#endif
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
            case gSerialMgrLpuart_c:
                status = (bool)LPUART_IsWakeupSource(mSerials[i].serialChannel);
                break;
#endif
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
            case gSerialMgrLpsci_c:
                if( LPSCI_IsWakeupSource(mSerials[i].serialChannel) )
                {
                    status = TRUE;
                }
                break;
#endif
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
            case gSerialMgrUsart_c:
                if (USART_IsWakeupSource(mSerials[i].serialChannel))
                {
                    status = TRUE;
                }
                break;
#endif
#endif /* #if (gSerialMgrUseUart_c) */
            default:
            	/*MISRA rule 16.4*/
                break;
            }
        }
    }/* for(...) */
#else
    (void)interfaceType;
#endif
    return status;
}


/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************* */
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
/*! *********************************************************************************
* \brief Transmit a data buffer to the specified interface.
*
* \param[in] InterfaceId the interface number
*
* \return The status of the operation
*
********************************************************************************** */
static serialStatus_t Serial_WriteInternal( uint8_t InterfaceId )
{
    serialStatus_t status = gSerial_Success_c;
    serial_t *pSer = &mSerials[InterfaceId];
    uint8_t *buf = NULL;
    uint16_t idx;

    OSA_InterruptDisable();
    if( 0U != pSer->state )
    {
        /* The interface is busy transmitting!
         * The current data will be transmitted after the previous transmissions end.
         */
        OSA_InterruptEnable();
    }
    else
    {
        pSer->state = 1;
#if gSerialMgr_DisallowMcuSleep_d
        PWR_DisallowDeviceToSleep();
#endif

        idx = pSer->txCurrent;
        if( 0u == pSer->txQueue[idx].dataSize )
        {
#if gSerialMgr_DisallowMcuSleep_d
            PWR_AllowDeviceToSleep();
#endif
            pSer->state = 0;
            OSA_InterruptEnable();
        }
        else
        {
            union
            {
                uint8_t const *constBuf;
                uint8_t *buf;
            }serTxQueueBuf;
            serTxQueueBuf.constBuf = pSer->txQueue[idx].pData;
            buf = serTxQueueBuf.buf;

            switch ( mSerials[InterfaceId].serialType )
            {
#if (gSerialMgrUseUart_c)
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
            case gSerialMgrUart_c:
                status = (serialStatus_t)UART_SendData( pSer->serialChannel, buf, pSer->txQueue[idx].dataSize );
                break;
#endif
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
            case gSerialMgrLpuart_c:
                status = (serialStatus_t)LPUART_SendData(pSer->serialChannel, buf, pSer->txQueue[idx].dataSize);
                break;
#endif
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
            case gSerialMgrLpsci_c:
                status = (serialStatus_t)LPSCI_SendData( pSer->serialChannel, buf, pSer->txQueue[idx].dataSize );
                break;
#endif
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
            case gSerialMgrUsart_c:
                status = (serialStatus_t)USART_SendData( pSer->serialChannel, buf, pSer->txQueue[idx].dataSize );
                break;
#endif
#endif /* #if (gSerialMgrUseUart_c) */

#if gSerialMgrUseUSB_c
            case gSerialMgrUSB_c:
                status = (serialStatus_t)VirtualCom_Write(mDrvData[InterfaceId].pDrvData,
                                     buf,
                                     pSer->txQueue[idx].dataSize);
                break;
#endif
#if gSerialMgrUseUSB_VNIC_c
            case gSerialMgrUSB_VNIC_c:
                status = (serialStatus_t)VirtualNic_Write(mDrvData[InterfaceId].pDrvData,
                                     buf,
                                     pSer->txQueue[idx].dataSize);
                break;
#endif

#if gSerialMgrUseIIC_c
            case gSerialMgrIICMaster_c:
                status = (serialStatus_t)I2C_MasterSend( pSer->serialChannel,
                                                       buf,
                                                       pSer->txQueue[idx].dataSize );
                break;

            case gSerialMgrIICSlave_c:
                /* Notify IIC Master that we have data to send */
                status = (serialStatus_t)I2C_SlaveSend(pSer->serialChannel,
                                                     buf,
                                                     pSer->txQueue[idx].dataSize);
                break;
#endif

#if gSerialMgrUseSPI_c
            case gSerialMgrSPISlave_c:
                /* Notify SPI Master that we have data to send */
                status = (serialStatus_t)Spi_AsyncTransferSlave(pSer->serialChannel,
                                           buf,
                                           &pSer->rxBuffer[pSer->rxIn],
                                           pSer->txQueue[idx].dataSize);
                break;

            case gSerialMgrSPIMaster_c:
                status = (serialStatus_t)Spi_AsyncTransfer(pSer->serialChannel,
                                            buf,
                                            &pSer->rxBuffer[pSer->rxIn],
                                            pSer->txQueue[idx].dataSize);
                break;
#endif

#if gSerialMgrUseCustomInterface_c
            case gSerialMgrCustom_c:
                /* The Serial_CustomSendData() function must be implemented by the custom protocol. */
                status = (serialStatus_t)Serial_CustomSendData(pSer->txQueue[idx].pData, pSer->txQueue[idx].dataSize);
                break;
#endif

            default:
                status = gSerial_InternalError_c;
                break;
            }

            OSA_InterruptEnable();

            if(gSerial_Success_c == status)
            {
#if gSerialMgrUseIIC_c
#if gSerialMgrSlaveDapTxLogicOne_c
                GpioSetPinOutput(&mI2cSlaveDapCfg);
#else
                GpioClearPinOutput(&mI2cSlaveDapCfg);
#endif
#endif /*gSerialMgrUseIIC_c*/

#if gSerialMgrUseSPI_c
                if(gSerialMgrSPISlave_c == mSerials[InterfaceId].serialType)
                {
#if gSerialMgrSlaveDapTxLogicOne_c
                    GpioSetPinOutput(&mSpiSlaveDapCfg);
#else
                    GpioClearPinOutput(&mSpiSlaveDapCfg);
#endif
#if gNvStorageIncluded_d
                    NvSetCriticalSection();
#endif
                }
                else
#endif /*gSerialMgrUseSPI_c*/
                {
                    /*MISRA rule 15.7*/
                }
            }
            else
            {
#if gSerialMgr_DisallowMcuSleep_d
                PWR_AllowDeviceToSleep();
#endif
        //        pSer->txQueue[idx].dataSize = 0;
        //        pSer->txQueue[idx].txCallback = NULL;
        //        mSerial_IncIdx_d(pSer->txCurrent, gSerialMgrTxQueueSize_c)
                pSer->state = 0;

                SERIAL_DBG_LOG("<--");
                (void)OSA_EventSet(mSMTaskEventId, (uint32_t)gSMGR_TxNew_c);
            }
        }
    }
    return status;
}

/*! *********************************************************************************
* \brief Inform the Serial Manager task that new data is available
*
* \param[in] pData The id interface
*
* \return none
*
* \remarks Called from usb task
*
********************************************************************************** */
#if gSerialMgrUseUSB_c
void SerialManager_VirtualComRxNotify(uint8_t* pData, uint16_t dataSize, uint8_t interface)
{

  while(dataSize)
  {
    OSA_InterruptDisable();
    mSerials[interface].rxBuffer[mSerials[interface].rxIn] = *pData++;
    mSerial_IncIdx_d(mSerials[interface].rxIn, gSMRxBufSize_c);
    if(mSerials[interface].rxIn == mSerials[interface].rxOut)
    {
      mSerial_IncIdx_d(mSerials[interface].rxOut, gSMRxBufSize_c);
    }
    OSA_InterruptEnable();
    dataSize--;
  }

   mSerials[interface].events |= gSMGR_Rx_c;

   SERIAL_DBG_LOG("<--");
   (void)OSA_EventSet(mSMTaskEventId, gSMGR_Rx_c);

}
#endif

/*! *********************************************************************************
* \brief Inform the Serial Manager task that new data is available
*
* \param[in] pData The id interface
*
* \return none
*
* \remarks Called from usb task
*
********************************************************************************** */
#if gSerialMgrUseUSB_VNIC_c
uint16_t SerialManager_VirtualNicRxNotify(uint8_t* pData, uint16_t dataSize, uint8_t interface)
{
  bufIndex_t inIndex;
  uint16_t charReceived = 0;
  inIndex = mSerials[interface].rxIn;
  mSerial_IncIdx_d(inIndex, gSMRxBufSize_c);
  while(dataSize && (inIndex != mSerials[interface].rxOut))
  {
    //OSA_InterruptDisable();
    mSerials[interface].rxBuffer[mSerials[interface].rxIn] = *pData++;
    mSerials[interface].rxIn = inIndex;
    charReceived++;
    mSerial_IncIdx_d(inIndex, gSMRxBufSize_c);
    //OSA_InterruptEnable();
    dataSize--;
  }
  if(charReceived)
  {
    mSerials[interface].events |= gSMGR_Rx_c;
    (void)OSA_EventSet(mSMTaskEventId, gSMGR_Rx_c);
  }
  return charReceived;
}
#endif

/*! *********************************************************************************
* \brief Inform the Serial Manager task that new data is available
*
* \param[in] pData The id interface
*
* \return none
*
* \remarks Called from ISR
*
********************************************************************************** */
void SerialManager_RxNotify( uint32_t i )
{
    serial_t *pSer = &mSerials[i];
    bufIndex_t tmp;
#if gSerialMgrUseSPI_c
#if gSerialMgrUseFSCIHdr_c
    uint8_t rxByte = pSer->rxBuffer[pSer->rxIn];
    uint8_t slaveDapRxEnd = 0;
#endif
#endif

    SERIAL_DBG_LOG("<--");

    mSerial_IncIdx_d(pSer->rxIn, gSMRxBufSize_c)
    tmp = pSer->rxIn;
    if(tmp == pSer->rxOut)
    {
        mSerial_IncIdx_d(pSer->rxOut, gSMRxBufSize_c)
    }

    switch( pSer->serialType )
    {
        /* Uart driver is in continuous Rx. No need to restart reception. */
#if gSerialMgrUseSPI_c
    case gSerialMgrSPISlave_c:
        /* No need to restart RX since SPI is in continuous RX mode */
        break;
    case gSerialMgrSPIMaster_c:
#if gSerialMgrUseFSCIHdr_c
        if( (0 == pSer->rxFsciPkt) && (gFSCI_StartMarker_c == rxByte) )
        {
            pSer->rxFsciPkt = 1;
        }

        if( pSer->rxFsciPkt )
        {
            pSer->rxFsciIn++;

#if gFsciLenHas2Bytes_c
            if( (mSMGR_FSCIHdrLen_c - 1) == pSer->rxFsciIn )
            {
                pSer->rxFsciLen = rxByte + 1; /* CRC */
            }
            else if( mSMGR_FSCIHdrLen_c == pSer->rxFsciIn )
            {
                pSer->rxFsciLen += rxByte << 8;
            }
#else
            if( mSMGR_FSCIHdrLen_c == pSer->rxFsciIn )
            {
                pSer->rxFsciLen = rxByte + 1; /* CRC */

            }
#endif
            if( pSer->rxFsciLen == (pSer->rxFsciIn - mSMGR_FSCIHdrLen_c) )
            {
                pSer->rxFsciPkt = 0;
                pSer->rxFsciIn = 0;
                pSer->rxFsciLen = 0;

#if gSerialMgrSlaveDapTxLogicOne_c
                slaveDapRxEnd = GpioReadPinInput(&mSpiMasterDapCfg);
#else
                slaveDapRxEnd = !GpioReadPinInput(&mSpiMasterDapCfg);
#endif
            }
        }

        /* If more bytes need to be received */
        if( (pSer->rxFsciPkt || slaveDapRxEnd) && !pSer->state )
        {
#if gSMGR_UseOsSemForSynchronization_c
            if( (0 == pSer->txQueue[pSer->txIn].dataSize) && (NULL == pSer->txQueue[pSer->txIn].txCallback) )
#endif
            {
                Serial_SpiSendDummyByte(i);
            }
        }
#else /* gSerialMgrUseFSCIHdr_c */
        /* If the SPI Slave has more data to transmit, restart the transfer */
#if gSerialMgrSlaveDapTxLogicOne_c
        if( (bool)(GpioReadPinInput(&mSpiMasterDapCfg)) && (0u == pSer->state) )
#else
        if( (!GpioReadPinInput(&mSpiMasterDapCfg)) && (0 == pSer->state) )
#endif
        {
#if gSMGR_UseOsSemForSynchronization_c
            if( (0 == pSer->txQueue[pSer->txIn].dataSize) && (NULL == pSer->txQueue[pSer->txIn].txCallback) )
#endif
            {
                Serial_SpiSendDummyByte(i);
            }
        }
#endif /*gSerialMgrUseFSCIHdr_c*/
        break;
#endif /*gSerialMgrUseSPI_c*/

#if gSerialMgrUseIIC_c
    case gSerialMgrIICSlave_c:
        break;

    case gSerialMgrIICMaster_c:
#if gSerialMgrSlaveDapTxLogicOne_c
        if( GpioReadPinInput(&mI2cMasterDapCfg) )
#else
        if( !GpioReadPinInput(&mI2cMasterDapCfg) )
#endif
        {
            I2C_MasterReceive( pSer->serialChannel, &mSerials[i].rxBuffer[mSerials[i].rxIn], 1 );
        }
        break;
#endif
    default:
    	/*MISRA rule 16.4*/
        break;
    }

#if gSerialPollingMode_c
    pSer->events |= gSMGR_Rx_c;
#else
    /* Signal SMGR task if not already done */
    if( !(bool)(pSer->events) )
    {
        pSer->events |= (uint8_t)gSMGR_Rx_c;
        (void)OSA_EventSet(mSMTaskEventId, (uint32_t)gSMGR_Rx_c);
    }
    else
    {
        pSer->events |= (uint8_t)gSMGR_Rx_c;
    }
#endif
}

/*! *********************************************************************************
* \brief Inform the Serial Manager task that a transmission has finished
*
* \param[in] pData the Id interface
*
* \return none
*
* \remarks Called from ISR
*
********************************************************************************** */
void SerialManager_TxNotify( uint32_t i )
{
    serial_t *pSer = &mSerials[i];

    OSA_InterruptDisable();
#if (gSerialMgrUseSPI_c)
    if( 2u != pSer->state )
#endif
    {
    pSer->txQueue[pSer->txCurrent].dataSize = 0; /* Mark as transmitted */
    mSerial_IncIdx_d(pSer->txCurrent, gSerialMgrTxQueueSize_c)
    }
#if gSerialMgr_DisallowMcuSleep_d
    PWR_AllowDeviceToSleep();
#endif
    pSer->state = 0;
    OSA_InterruptEnable();

    /* Transmit next block if available */
      if( (bool)(pSer->txQueue[pSer->txCurrent].dataSize) )
    {
#if gSerialMgrUseIIC_c
       if( (pSer->serialType != gSerialMgrIICMaster_c) && (pSer->serialType != gSerialMgrIICSlave_c) )
#endif
        {
#if gNvStorageIncluded_d
            if (pSer->serialType == gSerialMgrSPISlave_c)
             {
                NvClearCriticalSection();
            }
#endif
            (void)Serial_WriteInternal((uint8_t)i);
        }
    }
    else
    {
#if (gSerialMgrUseIIC_c)
      if(gSerialMgrIICMaster_c == pSer->serialType)
      {
#if gSerialMgrSlaveDapTxLogicOne_c
          if( GpioReadPinInput(&mI2cMasterDapCfg) )
#else
          if( !GpioReadPinInput(&mI2cMasterDapCfg) )
#endif
          {
              I2C_MasterReceive( pSer->serialChannel, &mSerials[i].rxBuffer[mSerials[i].rxIn], 1 );
          }
      }
      else if(gSerialMgrIICSlave_c == pSer->serialType)
      {
#if gSerialMgrSlaveDapTxLogicOne_c
          GpioClearPinOutput(&mI2cSlaveDapCfg);
#else
          GpioSetPinOutput(&mI2cSlaveDapCfg);
#endif
      }
      else
#endif /*gSerialMgrUseIIC_c*/
#if (gSerialMgrUseSPI_c)
      if(gSerialMgrSPISlave_c == pSer->serialType)
      {
#if gSerialMgrSlaveDapTxLogicOne_c
          GpioClearPinOutput(&mSpiSlaveDapCfg);
#else
          GpioSetPinOutput(&mSpiSlaveDapCfg);
#endif

#if gNvStorageIncluded_d
          NvClearCriticalSection();
#endif
      }
      else
#endif /*gSerialMgrUseSPI_c*/
      {
          /*MISRA rule 15.7*/
      }
    }

#if gSerialPollingMode_c
    pSer->events |= gSMGR_TxDone_c;
#else
    /* Signal SMGR task if not already done */
    if( (pSer->events & (uint8_t)gSMGR_TxDone_c) == 0u)
    {
    pSer->events |= (uint8_t)gSMGR_TxDone_c;
    (void)OSA_EventSet(mSMTaskEventId, (uint32_t)gSMGR_TxDone_c);
    }
#endif
}


/*! *********************************************************************************
* \brief   This function will mark all finished TX queue entries as empty.
*          If a calback was provided, it will be run.
*
* \param[in] pSer pointer to the serial interface internal structure
*
********************************************************************************** */
static void Serial_TxQueueMaintenance(serial_t *pSer)
{
    uint32_t i;
    static volatile bool_t txMntOngoing = FALSE;
    bool_t tempTxMntOngoing;

    OSA_InterruptDisable();
    tempTxMntOngoing = txMntOngoing;
        txMntOngoing = TRUE;
        OSA_InterruptEnable();

    if( FALSE == tempTxMntOngoing )
        {
        while ((bool)(pSer->txNo) && ( pSer->txQueue[pSer->txOut].dataSize == 0u ))
            {
            i = pSer->txOut;
            OSA_InterruptDisable();
            pSer->txNo--;
            OSA_InterruptEnable();
            mSerial_IncIdx_d(pSer->txOut, gSerialMgrTxQueueSize_c)

            /* Run Calback */
            if( (bool)(pSer->txQueue[i].txCallback) )
            {
                pSer->txQueue[i].txCallback( pSer->txQueue[i].pTxParam );
                pSer->txQueue[i].txCallback = NULL;
            }

#if gSerialMgr_BlockSenderOnQueueFull_c && gSMGR_UseOsSemForSynchronization_c
            OSA_InterruptDisable();
            if( pSer->txBlockedTasks )
            {
                pSer->txBlockedTasks--;
                OSA_InterruptEnable();
                (void)OSA_SemaphorePost(pSer->txQueueSemId);
            }
            else
            {
              OSA_InterruptEnable();
            }
#endif
/*
#if defined(FWK_SMALL_RAM_CONFIG)
            if( pSer->txQueue[pSer->txOut].dataSize )
                (void)OSA_EventSet(mSMTaskEventId, gSMGR_TxDone_c);
            return;
#endif
*/
        }

        txMntOngoing = FALSE;
    }
}

/*! *********************************************************************************
* \brief   This function will unblock the task who called Serial_SyncWrite().
*
* \param[in] pSer pointer to the serial interface internal structure
*
********************************************************************************** */
#if gSMGR_UseOsSemForSynchronization_c
static void Serial_SyncTxCallback(void *pSer)
{
    (void)OSA_SemaphorePost(((serial_t *)pSer)->txSyncSemId );
}
#endif

/*! *********************************************************************************
* \brief   This function will return the interfaceId for the specified interface
*
* \param[in] type     the interface type
* \param[in] channel  the instance of the interface
*
* \return The mSerials index for the specified interface type and channel
*
********************************************************************************** */
uint32_t Serial_GetInterfaceId(serialInterfaceType_t type, uint32_t channel)
{
    uint32_t i, id = gSerialMgrInvalidIdx_c;

    for(i=0; i<gSerialManagerMaxInterfaces_c; i++)
    {
        if(mSerials[i].serialType == type)
        {
            id = i;
            if((uint32_t)0xFF != channel)
            {
                if(mSerials[i].serialChannel == channel)
                {
                    break;
                }
                id = gSerialMgrInvalidIdx_c;
            }
            else
            {
                break;
            }
        }
    }
    return id;
}

/*! *********************************************************************************
* \brief   SPI transfet complete ISR callback
*
* \param[in] instance     the instance of the SPI module
*
********************************************************************************** */
#if (gSerialMgrUseSPI_c)
SERIAL_STATIC void SpiCallback(uint32_t flags, spiState_t* pState)
{
    uint32_t    instance = (uint32_t)(uint32_t *)pState->callbackParam;
    serial_t   *pSer = &mSerials[instance];

    if( (flags & gSPI_TxEndFlag_d) != 0U )
    {
        /* SPI Tx sequence end */
        SerialManager_TxNotify(instance);
    }

    if( (flags & (gSPI_RxEndFlag_d | gSPI_ByteRxFlag_d)) != 0U )
    {
        /* SPI Rx sequence end OR new byte received */
        SerialManager_RxNotify(instance);
    }

    if( (flags & gSPI_ByteRxFlag_d) != 0U )
    {
        /* Update data pointer for next SPI Rx*/
        pState->pRxData = &pSer->rxBuffer[pSer->rxIn];
    }
}
#endif

/*! *********************************************************************************
* \brief   SPI Master data available pin ISR
*
********************************************************************************** */
#if (gSerialMgrUseSPI_c)
SERIAL_STATIC void SpiMasterDapISR(void)
{
    uint32_t i = Serial_GetInterfaceId(gSerialMgrSPIMaster_c,0xFF);

    assert( gSerialMgrInvalidIdx_c != i );

    if( (bool)GpioIsPinIntPending(&mSpiMasterDapCfg) )
    {
#if gSerialMgrSlaveDapTxLogicOne_c
        if( (bool)GpioReadPinInput(&mSpiMasterDapCfg) )
#else
        if( !GpioReadPinInput(&mSpiMasterDapCfg) )
#endif
        {
            /* Change IRQ logic to detect when SPI Slave has no more data to send */
#if gSerialMgrSlaveDapTxLogicOne_c
            mSpiMasterDapCfg.interruptSelect = pinInt_LogicZero_c;
#else
            mSpiMasterDapCfg.interruptSelect = pinInt_LogicOne_c;
#endif

            /* SPI Master will start a dummy transfer to receive data from SPI Slave */
                Serial_SpiSendDummyByte(i);
        }
        else
        {
            /* Change IRQ logic to detect when SPI Slave has new data to send */
#if gSerialMgrSlaveDapTxLogicOne_c
            mSpiMasterDapCfg.interruptSelect = pinInt_LogicOne_c;
#else
            mSpiMasterDapCfg.interruptSelect = pinInt_LogicZero_c;
#endif
        }

        (void)GpioInputPinInit(&mSpiMasterDapCfg, 1);
        GpioClearPinIntFlag(&mSpiMasterDapCfg);
    }
}
#endif

/*! *********************************************************************************
* \brief   I2C Rx ISR callback.
*
* \param[in] state     pointer to the I2C state structure
*
********************************************************************************** */
#if (gSerialMgrUseIIC_c)
static void Serial_I2cRxCb(i2cState_t* state)
{
    uint32_t i = state->rxCbParam;

    SerialManager_RxNotify(i);
    /* Update rxBuff because rxIn was incremented by the RxNotify function */
    state->pRxData = &mSerials[i].rxBuffer[mSerials[i].rxIn];
}

/*! *********************************************************************************
* \brief   I2C Tx ISR callback.
*
* \param[in] state     pointer to the I2C state structure
*
********************************************************************************** */
static void Serial_I2cTxCb(i2cState_t* state)
{
    SerialManager_TxNotify(state->txCbParam);
}
#endif

/*! *********************************************************************************
* \brief   I2C Master data available pin ISR
*
********************************************************************************** */
#if (gSerialMgrUseIIC_c)
static void I2cMasterDapISR(void)
{
    uint32_t i = Serial_GetInterfaceId(gSerialMgrIICMaster_c,0xFF);

    if( i == gSerialMgrInvalidIdx_c )
    {
        panic(0,0,0,0);
    }
    else if( GpioIsPinIntPending(&mI2cMasterDapCfg) )
    {
#if gSerialMgrSlaveDapTxLogicOne_c
        if( GpioReadPinInput(&mI2cMasterDapCfg) )
#else
        if( !GpioReadPinInput(&mI2cMasterDapCfg) )
#endif
        {
#if gSerialMgrSlaveDapTxLogicOne_c
            mI2cMasterDapCfg.interruptSelect = pinInt_LogicZero_c;
#else
            mI2cMasterDapCfg.interruptSelect = pinInt_LogicOne_c;
#endif

            I2C_MasterReceive(mSerials[i].serialChannel, &mSerials[i].rxBuffer[mSerials[i].rxIn], 1 );
        }
        else
        {
#if gSerialMgrSlaveDapTxLogicOne_c
            mI2cMasterDapCfg.interruptSelect = pinInt_LogicOne_c;
#else
            mI2cMasterDapCfg.interruptSelect = pinInt_LogicZero_c;
#endif
        }

        GpioInputPinInit(&mI2cMasterDapCfg, 1);
        GpioClearPinIntFlag(&mI2cMasterDapCfg);
    }
}
#endif /* #if (gSerialMgrUseIIC_c) */

#if (gSerialMgrUseUart_c)
/*! *********************************************************************************
* \brief   UART Rx ISR callback.
*
* \param[in] state     pointer to the UART state structure
*
********************************************************************************** */
static void Serial_UartRxCb(uartState_t* state)
{
    uint32_t i = state->rxCbParam;
    SerialManager_RxNotify(i);
    /* Update rxBuff because rxIn was incremented by the RxNotify function */
    state->pRxData = &mSerials[i].rxBuffer[mSerials[i].rxIn];
    state->rxSize = 1;
}

/*! *********************************************************************************
* \brief   UART Tx ISR callback.
*
* \param[in] state     pointer to the UART state structure
*
********************************************************************************** */
static void Serial_UartTxCb(uartState_t* state)
{
    SerialManager_TxNotify(state->txCbParam);
}
#endif /* #if (gSerialMgrUseUart_c) */

#if gSerialMgrUseCustomInterface_c
/*! *********************************************************************************
* \brief   This function is used for a custom interface to notify the SerialManager
*          that the data transfer has ended
*
* \param[in] InterfaceId the interface number
*
********************************************************************************** */
void Serial_CustomSendCompleted(uint32_t InterfaceId)
{
    SerialManager_TxNotify(InterfaceId);
}

/*! *********************************************************************************
* \brief   This function is used for a custom interface to notify the SerialManager
*          that the data transfer has ended
*
* \param[in] InterfaceId - The interface number
* \param[in] pRxData     - Pointer to the received bytes
* \param[in] size        - Number of bytes received
*
* \return The number of bytes that have not been stored.
*
********************************************************************************** */
uint32_t Serial_CustomReceiveData(uint8_t InterfaceId, uint8_t *pRxData, uint32_t size)
{
    serial_t *pSer = &mSerials[InterfaceId];

    while(size--)
    {
        OSA_InterruptDisable();
        pSer->rxBuffer[pSer->rxIn] = *pRxData++;
        mSerial_IncIdx_d(pSer->rxIn, gSMRxBufSize_c);
        /* Check for overflow */
        if(pSer->rxIn == pSer->rxOut)
        {
            mSerial_DecIdx_d(pSer->rxIn, gSMRxBufSize_c);
            OSA_InterruptEnable();
            size++;
            break;
        }
        OSA_InterruptEnable();
    }

    /* Signal SMGR task if not already done */
    pSer->events |= gSMGR_Rx_c;

    SERIAL_DBG_LOG("<--");
    (void)OSA_EventSet(mSMTaskEventId, gSMGR_Rx_c);

    return size;
}
#endif

#if (gSerialMgrUseSPI_c)
static void Serial_SpiSendDummyByte(uint32_t i) /*send dummy byte*/
{
    bool_t proceed = FALSE;
    OSA_InterruptDisable();
    if( mSerials[i].state == 0u )
    {
        mSerials[i].state = 2;
        proceed = TRUE;
#if gSerialMgr_DisallowMcuSleep_d
        PWR_DisallowDeviceToSleep();
#endif
    }
    OSA_InterruptEnable();
    if(proceed)  /*send dummy byte when proceed is true*/
    {
    (void)Spi_SendDummyByte(mSerials[i].serialChannel, &mSerials[i].rxBuffer[mSerials[i].rxIn]);
    }
}
#endif
#endif /* #if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0)) */

void SerialInterface_Reinit(uint8_t instance)
{
    SERIAL_DBG_LOG("<--");
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    assert (instance < gSerialManagerMaxInterfaces_c);

    switch ( mSerials[instance].serialType )
    {
#if gSerialMgrUseIIC_c
    case gSerialMgrIICMaster_c:
    case gSerialMgrIICSlave_c:
       I2C_SaveRestoreConfig(instance, FALSE);
        break;
#endif
#if gSerialMgrUseSPI_c
    case gSerialMgrSPIMaster_c:
    case gSerialMgrSPISlave_c:
        (void)Spi_Init(instance, &(mDrvData[instance].spiState), NULL, NULL);
        (void)Spi_Configure(instance, &gSpiConfig);
        break;
#endif
#if gSerialMgrUseUart_c && (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
    case gSerialMgrUsart_c:
        USART_Initialize(instance, &mDrvData[instance].uartState);
        break;
#endif
#if gSerialMgrUseUart_c && FSL_FEATURE_SOC_LPUART_COUNT
    case gSerialMgrLpuart_c:
        (void)LPUART_Initialize((uint32_t)instance, &mDrvData[instance].uartState);

        break;
#endif /* #if FSL_FEATURE_SOC_LPUART_COUNT */
#if gSerialMgrUseUart_c && (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
    case gSerialMgrLpsci_c:
        LPSCI_Initialize(instance, &mDrvData[i].uartState);
        break;
#endif
#if gSerialMgrUseUart_c && (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
    case gSerialMgrUsart_c:
        USART_Initialize(instance, &mDrvData[i].uartState);
        break;
#endif

    default:
    	/*MISRA rule 16.4*/
        break;
    }
#endif /* #if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0)) */
    SERIAL_DBG_LOG("-->");
}

void SerialInterface_SerialIsWakeupSrc(uint8_t instance)
{
    SERIAL_DBG_LOG("<--");
#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    assert (instance < gSerialManagerMaxInterfaces_c);

    switch ( mSerials[instance].serialType )
    {
    /* Only LPUART implemented for now */
#if gSerialMgrUseUart_c && FSL_FEATURE_SOC_LPUART_COUNT
    case gSerialMgrLpuart_c:
        /* Wakeup from an IO from the serial peripheral -> the first byte is lost */
        Serial_UartRxCb( &mDrvData[instance].uartState );
        break;
#endif /* #if FSL_FEATURE_SOC_LPUART_COUNT */

    default:
    	/*MISRA rule 16.4*/
        assert(0);
        break;
    }
#endif
}

