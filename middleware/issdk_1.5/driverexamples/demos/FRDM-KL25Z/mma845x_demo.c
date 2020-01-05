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
 * @file mma845x_demo.c
 * @brief The mma845x_demo.c file implements the ISSDK MMA845x sensor
 *        demo example demonstration with interrupt mode for FRDM-KL25Z.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "board.h"
#include "pin_mux.h"
#include "fsl_lpsci.h"
#include "clock_config.h"

//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_I2C.h"
#include "Driver_USART.h"
#include "fsl_i2c_cmsis.h"
#include "fsl_lpsci_cmsis.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "gpio_driver.h"
#include "mma845x_drv.h"
#include "host_io_uart.h"
#include "systick_utils.h"
#include "auto_detection_service.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
#define MMA845x_STREAM_DATA_SIZE 10 /* 6 byte Data */
#define UART_RX_RING_BUFFER_SIZE 64

/*! @brief Unique Name for this application which should match the target GUI pkg name. */
#define APPLICATION_NAME "MMA8451Q"
/*! @brief Version to distinguish between instances the same application based on target Shield and updates. */
#define APPLICATION_VERSION "1.0"
/*! @brief The String for the Onboard Sensor Name. */
#define SHIELD_NAME "ON-BOARD-MMA8451Q"

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! Prepare the register write list to configure MMA845x in non-FIFO and ISR mode. */
const registerwritelist_t mma845x_Config_Isr[] = {
    /*! Configure the MMA845x CTRL_REG1 to set mode to STANDBY and odr to 12.5Hz. */
    {MMA845x_CTRL_REG1, MMA845x_CTRL_REG1_MODE_STANDBY | MMA845x_CTRL_REG1_DR_12DOT5HZ, 0},
    /*! Configure the MMA845x to disable FIFO */
    {MMA845x_F_SETUP, MMA845x_F_SETUP_F_MODE_FIFODISABLED, 0},
    /*! Configure the MMA845x CTRL_REG2 to set the Oversampling mode to High Resolution. */
    {MMA845x_CTRL_REG2, MMA845x_CTRL_REG2_SMODS_HIGHRES, 0},
    /*! Configure the MMA845x CTRL_REG3 to set the Interrupt polarity to ACTIVE high. */
    {MMA845x_CTRL_REG3, MMA845x_CTRL_REG3_IPOL_HIGH, 0},
    /*! Configure the MMA845x CTRL_REG4 to enable the data ready interrupt. */
    {MMA845x_CTRL_REG4, MMA845x_CTRL_REG4_INT_EN_DRDY_ENABLED, 0},
    /*! Configure the MMA845x CTRL_REG5 to route Interrupt to INT1 pin. */
    {MMA845x_CTRL_REG5, MMA845x_CTRL_REG5_INT_CFG_DRDY_INT1, 0},
    __END_WRITE_DATA__};

/*! Prepare the register read list to read the raw Accel data from MMA845x. */
const registerreadlist_t mma845x_Output_Values[] = {
    {.readFrom = MMA845x_OUT_X_MSB, .numBytes = MMA845x_ACCEL_DATA_SIZE}, __END_READ_DATA__};

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
char boardString[ADS_MAX_STRING_LENGTH] = {0}, shieldString[ADS_MAX_STRING_LENGTH] = {0},
     embAppName[ADS_MAX_STRING_LENGTH] = {0};
volatile bool bStreamingEnabled = false, bMma845xDataReady = false, bMma845xReady = false;
uint8_t gStreamID;                                       /* The auto assigned Stream ID. */
GENERIC_DRIVER_GPIO *pGpioDriver = &Driver_GPIO_KSDK;

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
/* This is the Sensor Data Ready ISR implementation.*/
void mma845x_int_data_ready_callback(void *pUserData)
{ /*! @brief Set flag to indicate Sensor has signalled data ready. */
    bMma845xDataReady = true;
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
                if (hostCommand[1] == gStreamID && bMma845xReady && bStreamingEnabled == false)
                {
                    bStreamingEnabled = true;
                    success = true;
                }
                break;
            case HOST_CMD_STOP:
                if (hostCommand[1] == gStreamID && bMma845xReady && bStreamingEnabled == true)
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
    uint8_t data[MMA845x_ACCEL_DATA_SIZE], streamingPacket[STREAMING_HEADER_LEN + MMA845x_STREAM_DATA_SIZE];

    mma845x_i2c_sensorhandle_t mma845xDriver;
    mma845x_acceldata_t rawData = {.timestamp = 0};

    ARM_DRIVER_I2C *pI2cDriver = &I2C_S_DRIVER;
    ARM_DRIVER_USART *pUartDriver = &HOST_S_DRIVER;

    /*! Initialize the MCU hardware. */
    BOARD_BootClockRUN();
    BOARD_SystickEnable();
    CLOCK_SetLpsci0Clock(1); /* Start the LPSCI Clock (01 : IRC48M). */

    /* Create the Short Application Name String for ADS. */
    sprintf(embAppName, "%s:%s", APPLICATION_NAME, APPLICATION_VERSION);

    /* Run ADS. */
    BOARD_RunADS(embAppName, boardString, shieldString, ADS_MAX_STRING_LENGTH);

    /* Create the Full Application Name String for Device Info Response. */
    sprintf(embAppName, "%s:%s:%s", SHIELD_NAME, APPLICATION_NAME, APPLICATION_VERSION);

    /*! Initialize INT1 MMA845x pin used by FRDM board */
    pGpioDriver->pin_init(&MMA845x_INT1, GPIO_DIRECTION_IN, NULL, &mma845x_int_data_ready_callback, NULL);

    /*! Initialize GREEN LED pin used by FRDM board */
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

    /*! Initialize the MMA845x sensor driver. */
    status = MMA845x_I2C_Initialize(&mma845xDriver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, MMA845x_I2C_ADDR,
                                    MMA8451_WHO_AM_I_WHOAMI_VALUE);
    if (SENSOR_ERROR_NONE == status)
    {
        /*!  Set the task to be executed while waiting for I2C transactions to complete. */
        MMA845x_I2C_SetIdleTask(&mma845xDriver, (registeridlefunction_t)SMC_SetPowerModeWait, SMC);

        /*! Configure the MMA845x sensor driver. */
        status = MMA845x_I2C_Configure(&mma845xDriver, mma845x_Config_Isr);
        if (SENSOR_ERROR_NONE == status)
        {
            bMma845xReady = true;
        }
    }

    /*! Initialize streaming and assign a Stream ID. */
    gStreamID =
        Host_IO_Init(pUartDriver, (void *)mma845xDriver.pCommDrv, &mma845xDriver.deviceInfo, NULL, MMA845x_I2C_ADDR);
    /* Confirm if a valid Stream ID has been allocated for this stream. */
    if (0 == gStreamID)
    {
        bMma845xReady = false;
    }
    else
    {
        /*! Populate streaming header. */
        Host_IO_Add_ISO_Header(gStreamID, streamingPacket, MMA845x_STREAM_DATA_SIZE);
        BOARD_SystickStart(&systick);
        pGpioDriver->clr_pin(&GREEN_LED); /* Set LED to indicate application is ready. */
    }

    for (;;) /* Forever loop */
    {        /* Call UART Non-Blocking Receive. */
        Host_IO_Receive(process_host_command, HOST_FORMAT_HDLC);

        /* Process packets only if streaming has been enabled by Host and ISR is available.
         * In ISR Mode we do not need to check Data Ready Register.
         * The receipt of interrupt will indicate data is ready. */
        if (false == bStreamingEnabled || false == bMma845xDataReady)
        {
            SMC_SetPowerModeWait(SMC); /* Power save, wait if nothing to do. */
            continue;
        }
        else
        { /*! Clear the data ready flag, it will be set again by the ISR. */
            bMma845xDataReady = false;
            pGpioDriver->toggle_pin(&GREEN_LED);
        }

        /*! Read raw sensor data from the MMA845x. */
        status = MMA845x_I2C_ReadData(&mma845xDriver, mma845x_Output_Values, data);
        if (ARM_DRIVER_OK != status)
        { /* Loop, if sample read failed. */
            continue;
        }

        /* Update timestamp from Systick framework. */
        rawData.timestamp += BOARD_SystickElapsedTime_us(&systick);

        /*! Convert the raw sensor data to signed 32-bit and 16-bit containers for display to the debug port. */
        rawData.accel[0] = ((int16_t)data[0] << 8) | data[1];
        rawData.accel[0] /= 4;
        rawData.accel[1] = ((int16_t)data[2] << 8) | data[3];
        rawData.accel[1] /= 4;
        rawData.accel[2] = ((int16_t)data[4] << 8) | data[5];
        rawData.accel[2] /= 4;

        /* Copy Raw samples to Streaming Buffer. */
        memcpy(streamingPacket + STREAMING_HEADER_LEN, &rawData, MMA845x_STREAM_DATA_SIZE);
        /* Send streaming packet to Host. */
        Host_IO_Send(streamingPacket, sizeof(streamingPacket), HOST_FORMAT_HDLC);
    }
}
