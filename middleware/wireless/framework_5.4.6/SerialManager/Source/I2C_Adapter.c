/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


#include "fsl_device_registers.h"
#include "fsl_os_abstraction.h"
#include "fsl_common.h"
#include "board.h"

#if FSL_FEATURE_SOC_I2C_COUNT
#include "fsl_i2c.h"
#elif FSL_FEATURE_SOC_LPI2C_COUNT
#include "fsl_lpi2c.h"
#endif

#include "I2C_Adapter.h"
#include "pin_mux.h"
#include "FunctionLib.h"

#define mI2cMaster_StartReadFlag_c   1u
#define mI2cMaster_WriteStopFlag_c   2u
#define mI2cMaster_ReceiveByteFlag_c 8u
#define mI2cSlave_WriteFlag_c        4u
#define mI2c_DummyData_c             0xFF

#ifdef CPU_QN908X
static i2c_master_config_t mMasterConfig;
static i2c_slave_config_t mSlaveConfig;
static i2c_master_transfer_t mMasterTransfer = {0};
static i2c_master_handle_t mMasterHandle = {0};
static i2c_slave_handle_t mSlaveHandle = {0};
#endif

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
#if FSL_FEATURE_SOC_I2C_COUNT
static I2C_Type * mI2cBase[] = I2C_BASE_PTRS;
static IRQn_Type mI2cIrqs[] = I2C_IRQS;

I2C_Type maI2cRegs[FSL_FEATURE_SOC_I2C_COUNT]; 

static i2cState_t * mI2cStates[FSL_FEATURE_SOC_I2C_COUNT];

#ifndef CPU_QN908X
static void I2C_ISR(void);
#endif /* #ifndef CPU_QN908X */

#ifdef CPU_QN908X
#define BOARD_GetI2cClock(__inst) BOARD_GetSpiClock(__inst)
static void i2c_slave_transfer_callback(I2C_Type *base, volatile i2c_slave_transfer_t *transfer, void *userData);
static void i2c_master_transfer_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t completionStatus, void *userData);
volatile bool g_slaveCompleted = false;
#endif /* CPU_QN908X */
#elif FSL_FEATURE_SOC_LPI2C_COUNT
static LPI2C_Type * mI2cBase[] = LPI2C_BASE_PTRS;
static IRQn_Type mI2cIrqs[] = LPI2C_IRQS;

static i2cState_t * mI2cStates[FSL_FEATURE_SOC_LPI2C_COUNT];

#define I2C_MASTER_GLOBAL_INTERRUPT_MASK     kLPI2C_MasterRxReadyFlag | \
                                             kLPI2C_MasterTxReadyFlag

#define I2C_SLAVE_GLOBAL_INTERRUPT_MASK      kLPI2C_SlaveRxReadyFlag | \
                                             kLPI2C_SlaveAddressValidFlag

#define I2C_SLAVE_GLOBAL_STATUS_FLAG_MASK    kLPI2C_SlaveStopDetectFlag | \
                                             kLPI2C_SlaveRepeatedStartDetectFlag | \
                                             kLPI2C_SlaveBitErrFlag | \
                                             kLPI2C_SlaveFifoErrFlag

#define I2C_MASTER_GLOBAL_STATUS_FLAG_MASK   kLPI2C_MasterTxReadyFlag | \
                                             kLPI2C_MasterRxReadyFlag | \
                                             kLPI2C_MasterEndOfPacketFlag | \
                                             kLPI2C_MasterStopDetectFlag | \
                                             kLPI2C_MasterNackDetectFlag | \
                                             kLPI2C_MasterArbitrationLostFlag | \
                                             kLPI2C_MasterFifoErrFlag | \
                                             kLPI2C_MasterPinLowTimeoutFlag | \
                                             kLPI2C_MasterDataMatchFlag | \
                                             kLPI2C_MasterBusyFlag | \
                                             kLPI2C_MasterBusBusyFlag
#endif /* #if FSL_FEATURE_SOC_I2C_COUNT */

/*! *********************************************************************************
*************************************************************************************
* External definitions
*************************************************************************************
********************************************************************************** */
#if FSL_FEATURE_SOC_I2C_COUNT
#ifdef CPU_QN908X
extern uint32_t I2C_GetInstance(I2C_Type *base);
#endif
#endif

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
#if FSL_FEATURE_SOC_LPI2C_COUNT
static void LPI2C_ISR(void);
#endif

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
uint32_t I2C_Initialize(uint32_t instance, i2cState_t *pState)
{
    uint32_t status = gI2cSuccess_c;

#if FSL_FEATURE_SOC_I2C_COUNT
#ifndef CPU_QN908X
    if( instance >= FSL_FEATURE_SOC_I2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
        return status;
    }
    
    BOARD_InitI2C();
    mI2cStates[instance] = pState;
    pState->rxCbParam = 0;
    pState->txCbParam = 0;
    pState->txCb = NULL;
    pState->rxCb = NULL;
    pState->pRxData = NULL;
    pState->pRxData = NULL;
    pState->rxSize = 0;
    pState->txSize = 0;

    if(pState->master)
    {
        i2c_master_config_t config;
        I2C_MasterGetDefaultConfig(&config);
        I2C_MasterInit(mI2cBase[instance], &config, BOARD_GetI2cClock(instance));
    }
    else
    {
        i2c_slave_config_t config;
        I2C_SlaveGetDefaultConfig(&config);
        config.addressingMode = kI2C_Address7bit;
        config.slaveAddress = pState->address;
        I2C_SlaveInit(mI2cBase[instance], &config, BOARD_GetI2cClock(instance));
        I2C_EnableInterrupts(mI2cBase[instance], kI2C_GlobalInterruptEnable);
    }

    OSA_InstallIntHandler(mI2cIrqs[instance], I2C_ISR);
    NVIC_SetPriority(mI2cIrqs[instance], gI2C_IsrPrio_c >> (8 - __NVIC_PRIO_BITS));
    NVIC_EnableIRQ(mI2cIrqs[instance]);
    /* Save current I2C configuration */
    I2C_SaveRestoreConfig(instance, TRUE);
#else /* CPU_QN908X */

    if( instance >= FSL_FEATURE_SOC_I2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
        return status;
    }

    BOARD_InitPMod_SPI_I2C();
    mI2cStates[instance] = pState;

    if(pState->master)
    {
        pState->rxCbParam = 0;
        pState->txCbParam = 0;
        pState->txCb = NULL;
        pState->rxCb = NULL;
        pState->pRxData = NULL;
        pState->pRxData = NULL;
        pState->rxSize = 0;
        pState->txSize = 0;

        I2C_MasterGetDefaultConfig(&mMasterConfig);
        I2C_MasterInit(mI2cBase[instance], &mMasterConfig, BOARD_GetI2cClock(instance));
        NVIC_SetPriority(mI2cIrqs[instance], gI2C_IsrPrio_c >> (8 - __NVIC_PRIO_BITS));
        /* Create the I2C handle for the non-blocking transfer */
        I2C_MasterTransferCreateHandle(mI2cBase[instance], &mMasterHandle, i2c_master_transfer_callback, NULL);
    }
    else
    {
        I2C_SlaveGetDefaultConfig(&mSlaveConfig);
        mSlaveConfig.address0.address = pState->address;
        I2C_SlaveInit(mI2cBase[instance], &mSlaveConfig, BOARD_GetI2cClock(instance));
        /* Create the I2C handle for the non-blocking transfer */
        I2C_SlaveTransferCreateHandle(mI2cBase[instance], &mSlaveHandle, i2c_slave_transfer_callback, NULL);
    }    
#endif /* CPU_QN908X */
#elif FSL_FEATURE_SOC_LPI2C_COUNT
    if( instance >= FSL_FEATURE_SOC_LPI2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
        return status;
    }
    
    BOARD_InitI2C();
    mI2cStates[instance] = pState;
    pState->rxCbParam = 0;
    pState->txCbParam = 0;
    pState->txCb = NULL;
    pState->rxCb = NULL;
    pState->pRxData = NULL;
    pState->pRxData = NULL;
    pState->rxSize = 0;
    pState->txSize = 0;

    if(pState->master)
    {
        lpi2c_master_config_t config;
        LPI2C_MasterGetDefaultConfig(&config);
        LPI2C_MasterInit(mI2cBase[instance], &config, BOARD_GetI2cClock(instance));
    }
    else
    {
        lpi2c_slave_config_t config;
        LPI2C_SlaveGetDefaultConfig(&config);
        config.addressMatchMode = kLPI2C_MatchAddress0;
        config.address0 = pState->address;
        LPI2C_SlaveInit(mI2cBase[instance], &config, BOARD_GetI2cClock(instance));
        LPI2C_SlaveEnableInterrupts(mI2cBase[instance], I2C_SLAVE_GLOBAL_INTERRUPT_MASK);
    }

    OSA_InstallIntHandler(mI2cIrqs[instance], LPI2C_ISR);
    NVIC_SetPriority(mI2cIrqs[instance], gI2C_IsrPrio_c >> (8 - __NVIC_PRIO_BITS));
    NVIC_EnableIRQ(mI2cIrqs[instance]);
#else
    status = gI2cInitFail_c;
#endif /* FSL_FEATURE_SOC_LPI2C_COUNT*/
    return status;
}

uint32_t I2C_SaveRestoreConfig(uint32_t instance, bool_t save)
{
    uint32_t status = gI2cInitFail_c;
#if FSL_FEATURE_SOC_I2C_COUNT
    if (instance >= FSL_FEATURE_SOC_I2C_COUNT)
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        uint8_t * pSrc;
        uint8_t * pDst;
        uint32_t len = I2C_GetDataRegAddr((I2C_Type*)0);
        
        if (save)
        {
            pDst = (uint8_t*)&maI2cRegs[instance];
            pSrc = (uint8_t*)mI2cBase[instance];
        }
        else
        {
            pSrc = (uint8_t*)&maI2cRegs[instance];
            pDst = (uint8_t*)mI2cBase[instance];
        }
        
        FLib_MemCpy(pDst, pSrc, len);
        /* Skip over the I2C Data register */
        len += sizeof(maI2cRegs->D);
        pDst += len;
        pSrc += len;
        len = sizeof(I2C_Type) - len;
        
        FLib_MemCpy(pDst, pSrc, len);
        status = gI2cSuccess_c;
    }
#endif
    return status;
}

#if FSL_FEATURE_SOC_I2C_COUNT
#if defined(CPU_QN908X)
static void i2c_slave_transfer_callback(I2C_Type *base, volatile i2c_slave_transfer_t *transfer, void *userData)
{
  i2cState_t * pState;
  uint32_t instance;

  /* Get instance */
  instance = I2C_GetInstance(base);

  pState = mI2cStates[instance];

  switch (transfer->event)
  {
  case kI2C_SlaveReceiveEvent:
     transfer->rxData = pState->pRxData;
     transfer->rxSize = pState->rxSize;
    break;

  case kI2C_SlaveTransmitEvent:
     transfer->txData = pState->pTxData;
     transfer->txSize = pState->txSize;
    break;

    /* The master has sent a stop transition on the bus */
  case kI2C_SlaveCompletionEvent:
    /* Insert user defined code here */
    g_slaveCompleted = TRUE;
  default:
    break;
  }
}

static void i2c_master_transfer_callback(I2C_Type *base,
                                               i2c_master_handle_t *handle,
                                               status_t completionStatus,
                                               void *userData)
{
  /* Insert user defined code here */
}
#endif /* CPU_QN908X */
#endif

/************************************************************************************/
uint32_t I2C_InstallRxCalback(uint32_t instance, i2cCallback_t cb, uint32_t cbParam)
{
    uint32_t status = gI2cSuccess_c;

#if FSL_FEATURE_SOC_I2C_COUNT
    if( instance >= FSL_FEATURE_SOC_I2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        mI2cStates[instance]->rxCb = cb;
        mI2cStates[instance]->rxCbParam = cbParam;
    }
#elif FSL_FEATURE_SOC_LPI2C_COUNT
    if( instance >= FSL_FEATURE_SOC_LPI2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        mI2cStates[instance]->rxCb = cb;
        mI2cStates[instance]->rxCbParam = cbParam;
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t I2C_InstallTxCalback(uint32_t instance, i2cCallback_t cb, uint32_t cbParam)
{
    uint32_t status = gI2cSuccess_c;
#if FSL_FEATURE_SOC_I2C_COUNT
    if( instance >= FSL_FEATURE_SOC_I2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        mI2cStates[instance]->txCb = cb;
        mI2cStates[instance]->txCbParam = cbParam;
    }
#elif FSL_FEATURE_SOC_LPI2C_COUNT
    if( instance >= FSL_FEATURE_SOC_LPI2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        mI2cStates[instance]->txCb = cb;
        mI2cStates[instance]->txCbParam = cbParam;
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t I2C_MasterSetSpeed(uint32_t instance, uint32_t speed)
{
    uint32_t status = gI2cSuccess_c;
#if FSL_FEATURE_SOC_I2C_COUNT
    if( instance >= FSL_FEATURE_SOC_I2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        I2C_MasterSetBaudRate(mI2cBase[instance], speed, BOARD_GetI2cClock(instance));
        /* Save current I2C configuration */
        I2C_SaveRestoreConfig(instance, TRUE);
    }
#elif FSL_FEATURE_SOC_LPI2C_COUNT
    if( instance >= FSL_FEATURE_SOC_LPI2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        LPI2C_MasterSetBaudRate(mI2cBase[instance], BOARD_GetI2cClock(instance), speed);
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t I2C_MasterSend(uint32_t instance, uint8_t *pData, uint32_t size)
{
    uint32_t status = gI2cSuccess_c;
#if FSL_FEATURE_SOC_I2C_COUNT
#ifndef CPU_QN908X
    if( instance >= FSL_FEATURE_SOC_I2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        I2C_Type * base = mI2cBase[instance];
        i2cState_t * pState = mI2cStates[instance];

        OSA_InterruptDisable();
        if( (pState->rxSize) || (pState->txSize) )
        {
            OSA_InterruptEnable();
            status = gI2cBusy_c;
        }
        else
        {
            pState->pTxData = pData;
            pState->txSize = size;
            OSA_InterruptEnable();

            pState->flags &= ~mI2cMaster_StartReadFlag_c;
            I2C_MasterStart(base, pState->address, kI2C_Write);
            I2C_EnableInterrupts(base, kI2C_GlobalInterruptEnable);
        }
    }
#else /* CPU_QN908X */
    i2cState_t* pState = mI2cStates[instance];
    /* Setup the master transfer */
    mMasterTransfer.slaveAddress = pState->address;
    mMasterTransfer.direction = kI2C_Write;
    mMasterTransfer.subaddress = 0;
    mMasterTransfer.subaddressSize = 0;
    mMasterTransfer.data = pData;
    mMasterTransfer.dataSize = size;
    mMasterTransfer.flags = kI2C_TransferDefaultFlag;
    /* Send master non-blocking data to slave */
    status = I2C_MasterTransferNonBlocking(mI2cBase[instance], &mMasterHandle, &mMasterTransfer);
#endif /* #ifndef CPU_QN908X */
#elif FSL_FEATURE_SOC_LPI2C_COUNT
    if( instance >= FSL_FEATURE_SOC_LPI2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        LPI2C_Type * base = mI2cBase[instance];
        i2cState_t * pState = mI2cStates[instance];

        OSA_InterruptDisable();
        if( (pState->rxSize) || (pState->txSize) )
        {
            OSA_InterruptEnable();
            status = gI2cBusy_c;
        }
        else
        {
            pState->pTxData = pData;
            pState->txSize = size;
            OSA_InterruptEnable();

            pState->flags &= ~mI2cMaster_StartReadFlag_c;
            LPI2C_MasterStart(base, pState->address, kLPI2C_Write);
            LPI2C_MasterEnableInterrupts(mI2cBase[instance], I2C_MASTER_GLOBAL_INTERRUPT_MASK);
        }
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t I2C_MasterReceive(uint32_t instance, uint8_t *pData, uint32_t size)
{
    uint32_t status = gI2cSuccess_c;

#if FSL_FEATURE_SOC_I2C_COUNT
#ifndef CPU_QN908X
    if( instance >= FSL_FEATURE_SOC_I2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        I2C_Type * base = mI2cBase[instance];
        i2cState_t * pState = mI2cStates[instance];

        OSA_InterruptDisable();
        if( (pState->rxSize) || (pState->txSize) )
        {
            OSA_InterruptEnable();
            status = gI2cBusy_c;
        }
        else
        {
            pState->pRxData = pData;
            pState->rxSize = size;
            OSA_InterruptEnable();

            pState->flags |= mI2cMaster_StartReadFlag_c;
            I2C_MasterStart(base, pState->address, kI2C_Read);
            I2C_EnableInterrupts(base, kI2C_GlobalInterruptEnable);
        }
    }
#else /* CPU_QN908X */
    i2cState_t* pState = mI2cStates[instance];
    /* Setup the master transfer */
    mMasterTransfer.slaveAddress = pState->address;
    mMasterTransfer.direction = kI2C_Read;
    mMasterTransfer.subaddress = 0;
    mMasterTransfer.subaddressSize = 0;
    mMasterTransfer.data = pData;
    mMasterTransfer.dataSize = size;
    mMasterTransfer.flags = kI2C_TransferDefaultFlag;
    /* Send master non-blocking data to slave */
    status = I2C_MasterTransferNonBlocking(mI2cBase[instance], &mMasterHandle, &mMasterTransfer);
#endif /* #ifndef CPU_QN908X */
#elif FSL_FEATURE_SOC_LPI2C_COUNT
    if( instance >= FSL_FEATURE_SOC_LPI2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        LPI2C_Type * base = mI2cBase[instance];
        i2cState_t * pState = mI2cStates[instance];
        
        OSA_InterruptDisable();
        if( (pState->txSize) || (pState->rxSize) )
        {
            OSA_InterruptEnable();
            status = gI2cBusy_c;
        }
        else
        {
            pState->pRxData = pData;
            pState->rxSize = size;
            OSA_InterruptEnable();
            
             LPI2C_MasterEnableInterrupts(base, kLPI2C_MasterRxReadyFlag);
             LPI2C_MasterStart(base, pState->address, kLPI2C_Read);
             /*Send command to read one byte*/
             base->MTDR = LPI2C_MTDR_CMD(0x1U) | LPI2C_MTDR_DATA(0x0U);
             pState->flags |= mI2cMaster_StartReadFlag_c;
        }
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t I2C_SlaveSend(uint32_t instance, uint8_t *pData, uint32_t size)
{
    uint32_t status = gI2cSuccess_c;

#if FSL_FEATURE_SOC_I2C_COUNT
#ifndef CPU_QN908X
    if( instance >= FSL_FEATURE_SOC_I2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        i2cState_t * pState = mI2cStates[instance];

        OSA_InterruptDisable();
        if( pState->txSize )
        {
            status = gI2cBusy_c;
        }
        else
        {
            pState->pTxData = pData;
            pState->txSize = size;
        }
        OSA_InterruptEnable();
    }

#else /* CPU_QN908X */
    /* Start accepting I2C transfers on the I2C slave peripheral */
    status = I2C_SlaveTransferNonBlocking(mI2cBase[instance], &mSlaveHandle,
                                         kI2C_SlaveTransmitEvent | kI2C_SlaveCompletionEvent);
#endif /* #ifndef CPU_QN908X*/
#elif FSL_FEATURE_SOC_LPI2C_COUNT
    if( instance >= FSL_FEATURE_SOC_LPI2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        i2cState_t * pState = mI2cStates[instance];    
        OSA_InterruptDisable();
        if( pState->txSize )
        {
            status = gI2cBusy_c;
        }
        else
        {
            pState->pTxData = pData;
            pState->txSize = size;
        }
        OSA_InterruptEnable();
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t I2C_SlaveReceive(uint32_t instance, uint8_t *pData, uint32_t size)
{
    uint32_t status = gI2cSuccess_c;
#if FSL_FEATURE_SOC_I2C_COUNT
#ifndef CPU_QN908X
    if( instance >= FSL_FEATURE_SOC_I2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        i2cState_t * pState = mI2cStates[instance];

        OSA_InterruptDisable();
        if( pState->rxSize )
        {
            status = gI2cBusy_c;
        }
        else
        {
            pState->pRxData = pData;
            pState->rxSize = size;
        }
        OSA_InterruptEnable();
    }
#else /* CPU_QN908X */
    /* Start accepting I2C transfers on the I2C slave peripheral */
    status = I2C_SlaveTransferNonBlocking(mI2cBase[instance], &mSlaveHandle,
                                         kI2C_SlaveReceiveEvent | kI2C_SlaveCompletionEvent);
#endif /* #ifndef CPU_QN908X */
#elif FSL_FEATURE_SOC_LPI2C_COUNT
    if( instance >= FSL_FEATURE_SOC_LPI2C_COUNT )
    {
        status = gI2cInvalidParameter_c;
    }
    else
    {
        i2cState_t * pState = mI2cStates[instance];

        OSA_InterruptDisable();
        if( pState->rxSize )
        {
            status = gI2cBusy_c;
        }
        else
        {
            pState->pRxData = pData;
            pState->rxSize = size;
        }

        OSA_InterruptEnable();
    }
#endif
    return status;
}

/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */
#if FSL_FEATURE_SOC_I2C_COUNT
#ifndef CPU_QN908X
static void I2C_ISR(void)
{
    uint32_t irq = __get_IPSR() - 16ul;
    uint32_t instance;
    I2C_Type * base;
    i2cState_t * pState;
    uint8_t status;
    uint8_t data;

    /* Get instance */
    for( instance=0ul; instance<FSL_FEATURE_SOC_I2C_COUNT; instance++ )
    {
        if( irq == (uint32_t)mI2cIrqs[instance] )
        {
            base = mI2cBase[instance];
            pState = mI2cStates[instance];
            status = I2C_SlaveGetStatusFlags(base);
            base->S = kI2C_IntPendingFlag;

            if( pState->master )
            {
                if( pState->flags & mI2cMaster_StartReadFlag_c )
                {   /*start read */
                    pState->flags &= ~mI2cMaster_StartReadFlag_c;
                    /* Change direction to read direction and automatically send ACK. */
                    base->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);
                    /* Send NAK at the last byte. */
                    if( pState->rxSize == 1 )
                    {
                        base->C1 |= I2C_C1_TXAK_MASK;
                    }
                    /* Read dummy to free the bus. */
                    data = base->D;
                }
                else
                {
                    if( pState->flags & mI2cMaster_WriteStopFlag_c )
                    {
                        /* end of write as master - need to send WRITE STOP */
                        pState->flags &= ~mI2cMaster_WriteStopFlag_c;
                        I2C_DisableInterrupts(base, kI2C_GlobalInterruptEnable);
                        I2C_MasterStop(base);
                        if( pState->txCb )
                        {
                            pState->txCb(pState);
                        }
                    }
                    else if( pState->txSize )
                    {
                        /* writing in progress as master */
                        pState->txSize--;
                        base->D = *pState->pTxData++;

                        if( 0 == pState->txSize )
                        {
                            pState->flags |= mI2cMaster_WriteStopFlag_c;
                        }
                    }

                    /* receiving in progress as master */
                    if( pState->rxSize )
                    {
                        /* Send STOP after receiving the last byte. */
                        if( pState->rxSize == 1 )
                        {
                            I2C_MasterStop(base);
                        }

                        pState->rxSize--;
                        data = base->D;
                        *pState->pRxData++ = data;

                        if( pState->rxCb )
                        {
                            pState->rxCb(pState);
                        }

                        /* Send NAK after the last byte. */
                        if( pState->rxSize == 1 )
                        {
                            base->C1 |= I2C_C1_TXAK_MASK;
                        }

                        if( 0 == pState->rxSize )
                        {
                            I2C_DisableInterrupts(base, kI2C_GlobalInterruptEnable);
                        }
                    }
                }
            }
            else /* I2C Slave */
            {
                if( status & kI2C_AddressMatchFlag )
                {
                    /* Slave transmit, master reading from slave. */
                    if( status & kI2C_TransferDirectionFlag )
                    {
                        /* Change direction to send data. */
                        base->C1 |= I2C_C1_TX_MASK;
                        pState->flags |= mI2cSlave_WriteFlag_c;
                    }
                    else
                    {
                        /* Slave receive, master writing to slave. */
                        base->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);
                        /* Read dummy to free the bus. */
                        data = base->D;
                        pState->flags &= ~mI2cSlave_WriteFlag_c;
                        return;
                    }
                }

                /* master receive->slave send transfer is ongoing. */
                if( pState->flags & mI2cSlave_WriteFlag_c )
                {
                    if( (0 == pState->txSize) || (status & kI2C_ReceiveNakFlag) )
                    {
                        pState->flags &= ~mI2cSlave_WriteFlag_c;
                        /* Change to RX mode when send out all data in tx buffer. */
                        base->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);
                        /* Read dummy to release bus. */
                        data = base->D;
                        if( (0 == pState->txSize) && (pState->txCb) )
                        {
                            pState->txCb(pState);
                        }
                    }
                    else if( pState->txSize )
                    {
                        pState->txSize--;
                        base->D = *pState->pTxData++;
                    }
                }
                else
                    /* slave receive->master send transfer is ongoing. */
                {
                    data = base->D;

                    if( pState->rxSize )
                    {
                        pState->rxSize--;
                    }

                    if( pState->pRxData )
                    {
                        *pState->pRxData++ = data;
                    }

                    if( (0 == pState->rxSize) && (pState->rxCb) )
                    {
                        pState->rxCb(pState);
                    }
                }
            }
            break;
        } /* if( irq == (uint32_t)mI2cIrqs[instance] ) */
    } /* for(...) */
}
#endif /* #ifndef CPU_QN908X */
#elif FSL_FEATURE_SOC_LPI2C_COUNT
static void LPI2C_ISR(void)
{
    uint32_t irq = __get_IPSR() - 16ul;
    uint32_t instance;
    LPI2C_Type * base;
    i2cState_t * pState;

    uint32_t status = 0;
    uint8_t data;
    
    /* Get instance */
    for( instance=0ul; instance<FSL_FEATURE_SOC_LPI2C_COUNT; instance++ )
    {
        if( irq == (uint32_t)mI2cIrqs[instance] )
        {
            base = mI2cBase[instance];
            pState = mI2cStates[instance];

            if( pState->master )
            {
              status = LPI2C_MasterGetStatusFlags(base);
              
              if(pState->flags & mI2cMaster_StartReadFlag_c)
              {
                  pState->rxSize--;
                  data = base->MRDR;
                  *pState->pRxData++ = data;
                  
                  /* Send STOP after receiving the last byte. */
                  if( pState->rxSize == 0 )
                  {
                      LPI2C_MasterStop(base);
                      LPI2C_MasterClearStatusFlags(base, I2C_MASTER_GLOBAL_STATUS_FLAG_MASK);
                      LPI2C_MasterDisableInterrupts(base, kLPI2C_MasterRxReadyFlag);
                  }

                  if( pState->rxCb )
                  {
                      pState->rxCb(pState);
                  }
              }
              
              if( pState->flags & mI2cMaster_WriteStopFlag_c )
              {
                  pState->flags &= ~mI2cMaster_WriteStopFlag_c;
                  LPI2C_MasterStop(base);
                  LPI2C_MasterDisableInterrupts(base, I2C_MASTER_GLOBAL_INTERRUPT_MASK); 
                  if( pState->txCb )
                  {
                      pState->txCb(pState);
                  }
              }
              else if( pState->txSize )
              {
                  //master write operation in progress
                  pState->txSize--;
                  
                  base->MTDR = *pState->pTxData++;

                  if( 0 == pState->txSize )
                  {
                      pState->flags |= mI2cMaster_WriteStopFlag_c;
                  }
              }
            }  
            else /* I2C Slave */
            {                 
                status = LPI2C_SlaveGetStatusFlags(base);
                /* Slave transmit, master reading from slave. */
                if( status & kLPI2C_SlaveTxReadyFlag )
                {
                    if (status & kLPI2C_SlaveAddressValidFlag)
                    {
                        pState->flags |= mI2cSlave_WriteFlag_c;
                    }
                }
                else
                {
                    pState->flags &= ~mI2cSlave_WriteFlag_c;
                }
              
                /* master receive->slave send transfer is ongoing. */
                if( pState->flags & mI2cSlave_WriteFlag_c )
                {
                    /* no more data to send to master */      
                    if(1 == pState->txSize)
                    {
                        pState->txSize--;
                        base->STDR = *pState->pTxData++;
                        /*clear address match valid flag to prevent the interrupt from occuring repetitively */
                        pState->flags &= ~mI2cSlave_WriteFlag_c;
                        LPI2C_SlaveClearStatusFlags(base, I2C_SLAVE_GLOBAL_STATUS_FLAG_MASK);
                        base->SASR;
                      
                        if(pState->txCb )
                        {
                            pState->txCb(pState);
                        }
                    }
                    else if( pState->txSize )
                    {
                        pState->txSize--;
                        base->STDR = *pState->pTxData++;
                        base->SASR;
                        pState->flags &= ~mI2cSlave_WriteFlag_c;
                        LPI2C_SlaveClearStatusFlags(base, I2C_SLAVE_GLOBAL_STATUS_FLAG_MASK);
                    }
                }
                else
                {
                    /* slave receive->master send transfer is ongoing. */
                    data = base->SRDR;
                  
                    if( pState->rxSize )
                    {
                        pState->rxSize--;
                    }

                    if( pState->pRxData )
                    {
                        *pState->pRxData++ = data;
                    }

                    if( (0 == pState->rxSize) && (pState->rxCb) )
                    {
                        base->SASR;
                        pState->rxCb(pState);
                    }
                }
            }
          break;
      } /* if( irq == (uint32_t)mI2cIrqs[instance] ) */
  } /* for(...) */
}
#endif