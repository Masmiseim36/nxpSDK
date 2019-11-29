/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef __I2C_ADAPTER_H__
#define __I2C_ADAPTER_H__

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#ifndef gI2C_IsrPrio_c
#define gI2C_IsrPrio_c (0x40)
#endif


/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef struct i2cState_tag i2cState_t;

typedef void (*i2cCallback_t)(i2cState_t* pState);

struct i2cState_tag {
    i2cCallback_t txCb;
    i2cCallback_t rxCb;
    uint32_t txCbParam;
    uint32_t rxCbParam;
    uint8_t *pTxData;
    uint8_t *pRxData;
    volatile uint16_t txSize;
    volatile uint16_t rxSize;
    volatile uint8_t  flags;
    uint8_t  address;
    uint8_t  master;
};

enum i2cStatus_tag {
    gI2cSuccess_c,
    gI2cInvalidParameter_c,
    gI2cBusy_c
};


/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
uint32_t I2C_Initialize(uint32_t instance, i2cState_t *pState);
uint32_t I2C_InstallRxCalback(uint32_t instance, i2cCallback_t cb, uint32_t cbParam);
uint32_t I2C_InstallTxCalback(uint32_t instance, i2cCallback_t cb, uint32_t cbParam);

uint32_t I2C_MasterSetSpeed(uint32_t instance, uint32_t speed);
uint32_t I2C_MasterSend(uint32_t instance, uint8_t *pData, uint32_t size);
uint32_t I2C_MasterReceive(uint32_t instance, uint8_t *pData, uint32_t size);

uint32_t I2C_SlaveSend(uint32_t instance, uint8_t *pData, uint32_t size);
uint32_t I2C_SlaveReceive(uint32_t instance, uint8_t *pData, uint32_t size);

#endif /* __I2C_ADAPTER_H__ */