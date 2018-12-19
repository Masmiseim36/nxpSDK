/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "vcom2i2c.h"

#include <board.h>

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && FSL_FEATURE_SOC_LPI2C_COUNT > 0
#   include "fsl_lpi2c.h" // A71CH I2C
#endif

#if defined(FSL_FEATURE_SOC_I2C_COUNT) && (FSL_FEATURE_SOC_I2C_COUNT > 0)
#   include "fsl_i2c.h" // A71CH I2C
#endif

//+ A71CH I2C
#include <stdbool.h>
#include <sm_types.h>
#include "smComSCI2C.h"
#include "global_platf.h"
#include "sm_apdu.h"
#include "scp.h"
#include "sm_timer.h"
//- A71CH I2C

//+ A71CH I2C
static uint8_t s_FrameParseInProgress = 0;
static uint16_t curr_index = 0;
//- A71CH I2C

//+ A71CH I2C
static uint8_t g_sendresp_vcom = 0;
static uint8_t g_senddata_i2c = 0;

U32 selectResponseDataLen = 0;
U8 selectResponseData[256 + 10];

uint8_t targetBuffer[512] = { 0 };
uint16_t targetBufferLen = 0;
static uint32_t nExpectedPayload = 0; // A71CH I2C

static uint8_t s_RecvBuff[256 + 10]; //+ A71CH I2C

void state_vcom_read_write(
    usb_cdc_vcom_struct_t * p_cdcVcom, volatile uint32_t * p_recvSize, const uint8_t s_currRecvBuf[DATA_BUFF_SIZE])
{
    usb_status_t error = kStatus_USB_Error;
    uint16_t statusValue = 0;
    U16 sw = SW_OK;
    if ((1 == p_cdcVcom->attach) && (1 == p_cdcVcom->startTransactions))
    {
        /* User Code */
        if ((0 != (*p_recvSize)) && (0xFFFFFFFFU != (*p_recvSize)))
        {
            if (s_FrameParseInProgress == 0)
            {
                nExpectedPayload = (s_currRecvBuf[2] << 8) + s_currRecvBuf[3];
            }

            if (nExpectedPayload > ((*p_recvSize) + curr_index - 4))
            {
                s_FrameParseInProgress = 1;
                memcpy(&s_RecvBuff[curr_index], s_currRecvBuf, (*p_recvSize));
                curr_index += (*p_recvSize);
            }
            else
            {
                s_FrameParseInProgress = 0;
                memcpy(&s_RecvBuff[curr_index], s_currRecvBuf, (*p_recvSize));
                curr_index += (*p_recvSize);
            }

            if ((curr_index == (nExpectedPayload + 4)) && (s_FrameParseInProgress == 0))
            {
                curr_index = 0;
                switch (s_RecvBuff[0])
                {
                case WAIT_FOR_CARD:
                {
                    U8 Atr[64];
                    U16 AtrLen = sizeof(Atr);

                    AtrLen = sizeof(Atr);
                    sw = smComSCI2C_Open(ESTABLISH_SCI2C, 0x00, Atr, &AtrLen);
                    if (sw == SW_OK)
                    {
                        targetBufferLen = sizeof(targetBuffer);
                        statusValue = vcomPackageApduResponse(0x00, 0x00, Atr, AtrLen, targetBuffer, &targetBufferLen);
                        if (statusValue == RJCT_OK)
                        {
                            memcpy(selectResponseData, targetBuffer, targetBufferLen);
                            selectResponseDataLen = targetBufferLen;
                            g_sendresp_vcom = 1;
                        }
                    }
                }
                    break;
                case APDU_DATA:
                    g_senddata_i2c = 1;
                    break;

                default:
#if DEBUG
                    printf("Error\r\n");
#endif
                    break;
                }
            }
            *p_recvSize = 0;
        }

        if (g_sendresp_vcom)
        {
            g_sendresp_vcom = 0;

            error = USB_DeviceCdcAcmSend(p_cdcVcom->cdcAcmHandle, USB_CDC_VCOM_BULK_IN_ENDPOINT, selectResponseData,
                selectResponseDataLen);

            if (error != kStatus_USB_Success)
            {
                /* Failure to send Data Handling code here */
            }
        }
        else if (s_FrameParseInProgress == 1)
        {
            error = USB_DeviceCdcAcmSend(p_cdcVcom->cdcAcmHandle, USB_CDC_VCOM_BULK_IN_ENDPOINT, NULL, 0);

            if (error != kStatus_USB_Success)
            {
                /* Failure to send Data Handling code here */
            }
        }

        state_vcom_usbLowPower();
    }
}

void state_vcom_2_i2c()
{
    U16 sw;
    if (g_senddata_i2c)
    {
        g_senddata_i2c = 0;
        memset(selectResponseData, 0, sizeof(selectResponseData));
        selectResponseDataLen = 0;
        sw = SM_SendAPDUVcom(&s_RecvBuff[4], nExpectedPayload, selectResponseData, (uint16_t *) &selectResponseDataLen);
        if (sw == SW_OK)
        {
            targetBufferLen = sizeof(targetBuffer);
            sw = vcomPackageApduResponse(APDU_DATA, 0x00, selectResponseData, selectResponseDataLen, targetBuffer,
                &targetBufferLen);
            memcpy(selectResponseData, targetBuffer, targetBufferLen);
            selectResponseDataLen = targetBufferLen;
            g_sendresp_vcom = 1;
        }
    }
}

//+ A71CH I2C
U16 vcomPackageApduResponse(
    U8 messageType, U8 nodeAddress, U8* payload, U16 payloadLen, U8 *targetBuf, U16 *targetBufLen)
{
    if (*targetBufLen < (4 + payloadLen))
    {
        printf("Target buffer provided too small.\r\n");
        return RJCT_ARG_FAIL;
    }

    targetBuf[0] = messageType;
    targetBuf[1] = nodeAddress;
    targetBuf[2] = (payloadLen >> 8) & 0x00FF;
    targetBuf[3] = payloadLen & 0x00FF;
    memcpy(&targetBuf[4], payload, payloadLen);
    *targetBufLen = 4 + payloadLen;
    return RJCT_OK;
}

U16 SM_SendAPDUVcom(
    U8 *cmd, U16 cmdLen, U8 *resp, U16 *respLen)
{
    U32 status = 0;
    U32 respLenLocal = *respLen;

    status = smCom_TransceiveRaw(cmd, cmdLen, resp, &respLenLocal);
    *respLen = (U16) respLenLocal;

    return (U16) status;
}
//- A71CH I2C
