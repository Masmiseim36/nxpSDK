/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
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