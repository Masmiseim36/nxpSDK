/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file fxls8962_demo.c
 * @brief The fxls8962_demo.c file implements the ISSDK FXLS8962 sensor
 *        demo example demonstration with interrupt mode.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_I2C.h"
#include "Driver_USART.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "gpio_driver.h"
#include "fxls8962_drv.h"
#include "host_io_uart.h"
#include "systick_utils.h"
#include "auto_detection_service.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
#define FXLS8962_DATA_SIZE 6
#define FXLS8962_STREAM_DATA_SIZE 10

/*! @brief Unique Name for this application which should match the target GUI pkg name. */
#define APPLICATION_NAME "FXLS8962AF"
/*! @brief Version to distinguish between instances the same application based on target Shield and updates. */
#define APPLICATION_VERSION "1.0"

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! @brief Register settings for Interrupt (non buffered) mode. */
const registerwritelist_t cFxls8962ConfigNormal[] = {
    /* Set Full-scale range as 2G. */
    {FXLS8962_SENS_CONFIG1, FXLS8962_SENS_CONFIG1_FSR_2G, FXLS8962_SENS_CONFIG1_FSR_MASK},
    /* Set Wake Mode ODR Rate as 6.25Hz. */
    {FXLS8962_SENS_CONFIG3, FXLS8962_SENS_CONFIG3_WAKE_ODR_6_25HZ, FXLS8962_SENS_CONFIG3_WAKE_ODR_MASK},
    /* Enable Interrupts for Data Ready Events. */
    {FXLS8962_INT_EN, FXLS8962_INT_EN_DRDY_EN_EN, FXLS8962_INT_EN_DRDY_EN_MASK},
    __END_WRITE_DATA__};

/*! @brief Address of Raw Accel Data in Normal Mode. */
const registerreadlist_t cFxls8962OutputNormal[] = {{.readFrom = FXLS8962_OUT_X_LSB, .numBytes = FXLS8962_DATA_SIZE},
                                                    __END_READ_DATA__};

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
char boardString[ADS_MAX_STRING_LENGTH] = {0}, shieldString[ADS_MAX_STRING_LENGTH] = {0},
     embAppName[ADS_MAX_STRING_LENGTH] = {0};
volatile bool bStreamingEnabled = false, bFxls8962DataReady = false, bFxls8962Ready = false;
uint8_t gStreamID; /* The auto assigned Stream ID. */
GENERIC_DRIVER_GPIO *pGpioDriver = &Driver_GPIO_KSDK;

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
/* This is the Sensor Data Ready ISR implementation.*/
void fxls8962_int_data_ready_callback(void *pUserData)
{ /*! @brief Set flag to indicate Sensor has signalled data ready. */
    bFxls8962DataReady = true;
}

/* Handler for Device Info and Streaming Control Commands. */
bool process_host_command(
    uint8_t tag, uint8_t *hostCommand, uint8_t *hostResponse, size_t *hostMsgSize, size_t respBufferSize)
{
    bool success = false;

    /* If it is Host requesting Device Info, send Board Name and Shield Name. */
    if (tag == HOST_PRO_INT_DEV_TAG)
    { /* Byte 1     : Payload - Length of APPLICATION_NAME (b)
       * Bytes=b    : Payload Application Name
       * Byte b+1   : Payload - Length of BOARD_NAME (s)
       * Bytes=s    : Payload Board Name
       * Byte b+s+2 : Payload - Length of SHIELD_NAME (v)
       * Bytes=v    : Payload Shield Name */

        size_t appNameLen = strlen(embAppName);
        size_t boardNameLen = strlen(boardString);
        size_t shieldNameLen = strlen(shieldString);

        if (respBufferSize >= boardNameLen + shieldNameLen + appNameLen + 3)
        { /* We have sufficient buffer. */
            *hostMsgSize = 0;
        }
        else
        {
            return false;
        }

        hostResponse[*hostMsgSize] = appNameLen;
        *hostMsgSize += 1;

        memcpy(hostResponse + *hostMsgSize, embAppName, appNameLen);
        *hostMsgSize += appNameLen;

        hostResponse[*hostMsgSize] = boardNameLen;
        *hostMsgSize += 1;

        memcpy(hostResponse + *hostMsgSize, boardString, boardNameLen);
        *hostMsgSize += boardNameLen;

        hostResponse[*hostMsgSize] = shieldNameLen;
        *hostMsgSize += 1;

        memcpy(hostResponse + *hostMsgSize, shieldString, shieldNameLen);
        *hostMsgSize += shieldNameLen;

        return true;
    }

    /* If it is Host sending Streaming Commands, take necessary actions. */
    if ((tag == (HOST_PRO_INT_CMD_TAG | HOST_PRO_CMD_W_CFG_TAG)) &&
        (*hostMsgSize == HOST_MSG_CMD_ACT_OFFSET - HOST_MSG_LEN_LSB_OFFSET))
    {                           /* Byte 1 : Payload - Operation Code (Start/Stop Operation Code)
                                 * Byte 2 : Payload - Stream ID (Target Stream for carrying out operation) */
        switch (hostCommand[0]) /* Execute desired operation (Start/Stop) on the requested Stream. */
        {
            case HOST_CMD_START:
                if (hostCommand[1] == gStreamID && bFxls8962Ready && bStreamingEnabled == false)
                {
                    bStreamingEnabled = true;
                    success = true;
                }
                break;
            case HOST_CMD_STOP:
                if (hostCommand[1] == gStreamID && bFxls8962Ready && bStreamingEnabled == true)
                {
                    pGpioDriver->clr_pin(&GREEN_LED);
                    bStreamingEnabled = false;
                    success = true;
                }
                break;
            default:
                break;
        }
        *hostMsgSize = 0; /* Zero payload in response. */
    }

    return success;
}

/*!
 * @brief Main function
 */
int main(void)
{
    int32_t status, systick;
    uint8_t data[FXLS8962_DATA_SIZE], streamingPacket[STREAMING_HEADER_LEN + FXLS8962_STREAM_DATA_SIZE];

    fxls8962_i2c_sensorhandle_t fxls8962Driver;
    fxls8962_acceldata_t rawData = {.timestamp = 0};

    ARM_DRIVER_I2C *pI2cDriver = &I2C_S_DRIVER;
    ARM_DRIVER_USART *pUartDriver = &HOST_S_DRIVER;

    /*! Initialize the MCU hardware. */
    BOARD_BootClockRUN();
    BOARD_SystickEnable();

    /* Create the Short Application Name String for ADS. */
    sprintf(embAppName, "%s:%s", APPLICATION_NAME, APPLICATION_VERSION);

    /* Run ADS. */
    BOARD_RunADS(embAppName, boardString, shieldString, ADS_MAX_STRING_LENGTH);

    /* Create the Full Application Name String for Device Info Response. */
    sprintf(embAppName, "%s:%s:%s", SHIELD_NAME, APPLICATION_NAME, APPLICATION_VERSION);

    /*! Initialize FXLS8962 pin used by FRDM board */
    pGpioDriver->pin_init(&FXLS8962_INT1, GPIO_DIRECTION_IN, NULL, &fxls8962_int_data_ready_callback, NULL);

    /*! Initialize RGB LED pin used by FRDM board */
    pGpioDriver->pin_init(&GREEN_LED, GPIO_DIRECTION_OUT, NULL, NULL, NULL);

    /*! Initialize the I2C driver. */
    status = pI2cDriver->Initialize(I2C_S_SIGNAL_EVENT);
    if (ARM_DRIVER_OK != status)
    {
        return -1;
    }

    /*! Set the I2C Power mode. */
    status = pI2cDriver->PowerControl(ARM_POWER_FULL);
    if (ARM_DRIVER_OK != status)
    {
        return -1;
    }

    /*! Set the I2C bus speed. */
    status = pI2cDriver->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
    if (ARM_DRIVER_OK != status)
    {
        return -1;
    }

    /*! Initialize the UART driver. */
    status = pUartDriver->Initialize(HOST_S_SIGNAL_EVENT);
    if (ARM_DRIVER_OK != status)
    {
        return -1;
    }

    /*! Set the UART Power mode. */
    status = pUartDriver->PowerControl(ARM_POWER_FULL);
    if (ARM_DRIVER_OK != status)
    {
        return -1;
    }

    /*! Set UART Baud Rate. */
    status = pUartDriver->Control(ARM_USART_MODE_ASYNCHRONOUS, BOARD_DEBUG_UART_BAUDRATE);
    if (ARM_DRIVER_OK != status)
    {
        return -1;
    }

    /*! Initialize FXLS8962 sensor driver. */
    status = FXLS8962_I2C_Initialize(&fxls8962Driver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, FXLS8962_I2C_ADDR,
                                     FXLS8962_WHOAMI_VALUE);
    if (SENSOR_ERROR_NONE == status)
    {
        /*!  Set the task to be executed while waiting for I2C transactions to complete. */
        FXLS8962_I2C_SetIdleTask(&fxls8962Driver, (registeridlefunction_t)SMC_SetPowerModeWait, SMC);

        /*! Configure the FXLS8962 sensor. */
        status = FXLS8962_I2C_Configure(&fxls8962Driver, cFxls8962ConfigNormal);
        if (SENSOR_ERROR_NONE == status)
        {
            bFxls8962Ready = true;
        }
    }

    /*! Initialize streaming and assign a Stream ID. */
    gStreamID =
        Host_IO_Init(pUartDriver, (void *)fxls8962Driver.pCommDrv, &fxls8962Driver.deviceInfo, NULL, FXLS8962_I2C_ADDR);
    /* Confirm if a valid Stream ID has been allocated for this stream. */
    if (0 == gStreamID)
    {
        bFxls8962Ready = false;
    }
    else
    {
        /*! Populate streaming header. */
        Host_IO_Add_ISO_Header(gStreamID, streamingPacket, FXLS8962_STREAM_DATA_SIZE);
        BOARD_SystickStart(&systick);
        pGpioDriver->clr_pin(&GREEN_LED);
    }

    for (;;) /* Forever loop */
    {        /* Call UART Non-Blocking Receive. */
        Host_IO_Receive(process_host_command, HOST_FORMAT_HDLC);

        /* Process packets only if streaming has been enabled by Host and ISR is available.
         * In ISR Mode we do not need to check Data Ready Register.
         * The receipt of interrupt will indicate data is ready. */
        if (false == bStreamingEnabled || false == bFxls8962DataReady)
        {
            SMC_SetPowerModeWait(SMC); /* Power save, wait if nothing to do. */
            continue;
        }
        else
        { /*! Clear the data ready flag, it will be set again by the ISR. */
            bFxls8962DataReady = false;
            pGpioDriver->toggle_pin(&GREEN_LED);
        }

        /*! Read new raw sensor data from the FXLS8962. */
        status = FXLS8962_I2C_ReadData(&fxls8962Driver, cFxls8962OutputNormal, data);
        if (ARM_DRIVER_OK != status)
        { /* Loop, if sample read failed. */
            continue;
        }

        /* Update timestamp from Systick framework. */
        rawData.timestamp += BOARD_SystickElapsedTime_us(&systick);

        /*! Convert the raw sensor data to signed 16-bit container for display to the debug port. */
        rawData.accel[0] = ((int16_t)data[1] << 8) | data[0];
        rawData.accel[1] = ((int16_t)data[3] << 8) | data[2];
        rawData.accel[2] = ((int16_t)data[5] << 8) | data[4];

        /* Copy Raw samples to Streaming Buffer. */
        memcpy(streamingPacket + STREAMING_HEADER_LEN, &rawData, FXLS8962_STREAM_DATA_SIZE);
        /* Send streaming packet to Host. */
        Host_IO_Send(streamingPacket, sizeof(streamingPacket), HOST_FORMAT_HDLC);
    }
}
