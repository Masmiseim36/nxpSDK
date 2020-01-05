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
 * @file fxls8471_demo.c
 * @brief The fxls8471_demo.c file implements the ISSDK FXLS8471 sensor
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
#include "Driver_SPI.h"
#include "Driver_USART.h"
#include "fsl_spi_cmsis.h"
#include "fsl_lpsci_cmsis.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "gpio_driver.h"
#include "host_io_uart.h"
#include "systick_utils.h"
#include "fxls8471q_drv.h"
#include "auto_detection_service.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
#define FXLS8471_STREAM_DATA_SIZE 10
#define UART_RX_RING_BUFFER_SIZE 64

/*! @brief Unique Name for this application which should match the target GUI pkg name. */
#define APPLICATION_NAME "FXLS8471Q"
/*! @brief Version to distinguish between instances the same application based on target Shield and updates. */
#define APPLICATION_VERSION "1.0"

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! Prepare the register write list to configure FXLS8471Q in non-FIFO mode. */
const registerwritelist_t cFxls8471q_Config_Isr[] = {
    /*! Configure CTRL_REG1 register to put FXLS8471Q to 12.5Hz sampling rate. */
    {FXLS8471Q_CTRL_REG1, FXLS8471Q_CTRL_REG1_DR_12DOT5HZ, FXLS8471Q_CTRL_REG1_DR_MASK},
    /*! Configure settings for interrupt notification. */
    /*! Active High, Push-Pull */
    {FXLS8471Q_CTRL_REG3, FXLS8471Q_CTRL_REG3_IPOL_HIGH | FXLS8471Q_CTRL_REG3_PP_OD_PUSHPULL,
     FXLS8471Q_CTRL_REG3_IPOL_MASK | FXLS8471Q_CTRL_REG3_PP_OD_MASK},
    {FXLS8471Q_CTRL_REG4, FXLS8471Q_CTRL_REG4_INT_EN_DRDY_ENABLED,
     FXLS8471Q_CTRL_REG4_INT_EN_DRDY_MASK}, /*! Data Ready Event. */
    {FXLS8471Q_CTRL_REG5, FXLS8471Q_CTRL_REG5_INT_CFG_DRDY_INT1, FXLS8471Q_CTRL_REG5_INT_CFG_DRDY_MASK}, /*! INT1 Pin */
    __END_WRITE_DATA__};

/*! Prepare the register read list to read the raw accel data from the FXLS8471Q. */
const registerreadlist_t cFxls8471q_Output_Values[] = {
    {.readFrom = FXLS8471Q_OUT_X_MSB, .numBytes = FXLS8471Q_ACCEL_DATA_SIZE}, __END_READ_DATA__};

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
char boardString[ADS_MAX_STRING_LENGTH] = {0}, shieldString[ADS_MAX_STRING_LENGTH] = {0},
     embAppName[ADS_MAX_STRING_LENGTH] = {0};
volatile bool bStreamingEnabled = false, bFxls8471DataReady = false, bFxls8471Ready = false;
uint8_t gStreamID;                                       /* The auto assigned Stream ID. */
GENERIC_DRIVER_GPIO *pGpioDriver = &Driver_GPIO_KSDK;

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
/* This is the Sensor Data Ready ISR implementation.*/
void fxls8471_int_data_ready_callback(void *pUserData)
{ /*! @brief Set flag to indicate Sensor has signalled data ready. */
    bFxls8471DataReady = true;
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
                if (hostCommand[1] == gStreamID && bFxls8471Ready && bStreamingEnabled == false)
                {
                    bStreamingEnabled = true;
                    success = true;
                }
                break;
            case HOST_CMD_STOP:
                if (hostCommand[1] == gStreamID && bFxls8471Ready && bStreamingEnabled == true)
                {
                    pGpioDriver->clr_pin(&RED_LED);
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
    uint8_t data[FXLS8471Q_ACCEL_DATA_SIZE], streamingPacket[STREAMING_HEADER_LEN + FXLS8471_STREAM_DATA_SIZE];

    fxls8471q_spi_sensorhandle_t fxls8471Driver;
    fxls8471q_acceldata_t rawData = {.timestamp = 0};

    ARM_DRIVER_SPI *pSPIdriver = &SPI_S_DRIVER;
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

    /*! Initialize FXLS8471 pin used by FRDM board */
    pGpioDriver->pin_init(&FXLS8471_INT1, GPIO_DIRECTION_IN, NULL, &fxls8471_int_data_ready_callback, NULL);

    /*! Initialize RGB LED pin used by FRDM board */
    pGpioDriver->pin_init(&RED_LED, GPIO_DIRECTION_OUT, NULL, NULL, NULL);

    /*! Initialize the SPI driver. */
    status = pSPIdriver->Initialize(SPI_S_SIGNAL_EVENT);
    if (ARM_DRIVER_OK != status)
    {
        return -1;
    }

    /*! Set the SPI Power mode. */
    status = pSPIdriver->PowerControl(ARM_POWER_FULL);
    if (ARM_DRIVER_OK != status)
    {
        return -1;
    }

    /*! Set the SPI Slave speed. */
    status = pSPIdriver->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA0, SPI_S_BAUDRATE);
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

    /*! Initialize FXLS8471 sensor driver. */
    status = FXLS8471Q_SPI_Initialize(&fxls8471Driver, &SPI_S_DRIVER, SPI_S_DEVICE_INDEX, &FXLS8471_SPI_CS,
                                     FXLS8471Q_WHO_AM_I_WHOAMI_VALUE);
    if (SENSOR_ERROR_NONE == status)
    {
        /*!  Set the task to be executed while waiting for SPI transactions to complete. */
        FXLS8471Q_SPI_SetIdleTask(&fxls8471Driver, (registeridlefunction_t)SMC_SetPowerModeWait, SMC);

        /*! Configure the FXLS8471 sensor. */
        status = FXLS8471Q_SPI_Configure(&fxls8471Driver, cFxls8471q_Config_Isr);
        if (SENSOR_ERROR_NONE == status)
        {
            bFxls8471Ready = true;
        }
    }

    /*! Initialize streaming and assign a Stream ID. */
    gStreamID =
        Host_IO_Init(pUartDriver, (void *)fxls8471Driver.pCommDrv, &fxls8471Driver.deviceInfo, &fxls8471Driver.slaveParams, FXLS8471Q_I2C_ADDRESS_SA0_0_SA1_0);
    /* Confirm if a valid Stream ID has been allocated for this stream. */
    if (0 == gStreamID)
    {
        bFxls8471Ready = false;
    }
    else
    {
        /*! Populate streaming header. */
        Host_IO_Add_ISO_Header(gStreamID, streamingPacket, FXLS8471_STREAM_DATA_SIZE);
        BOARD_SystickStart(&systick);
        pGpioDriver->clr_pin(&RED_LED);
    }

    for (;;) /* Forever loop */
    {        /* Call UART Non-Blocking Receive. */
        Host_IO_Receive(process_host_command, HOST_FORMAT_HDLC);

        /* Process packets only if streaming has been enabled by Host and ISR is available.
         * In ISR Mode we do not need to check Data Ready Register.
         * The receipt of interrupt will indicate data is ready. */
        if (false == bStreamingEnabled || false == bFxls8471DataReady)
        {
            SMC_SetPowerModeWait(SMC); /* Power save, wait if nothing to do. */
            continue;
        }
        else
        { /*! Clear the data ready flag, it will be set again by the ISR. */
            bFxls8471DataReady = false;
            pGpioDriver->toggle_pin(&RED_LED);
        }

        /*! Read new raw sensor data from the FXLS8471. */
        status = FXLS8471Q_SPI_ReadData(&fxls8471Driver, cFxls8471q_Output_Values, data);
        if (ARM_DRIVER_OK != status)
        { /* Loop, if sample read failed. */
            continue;
        }

        /* Update timestamp from Systick framework. */
        rawData.timestamp += BOARD_SystickElapsedTime_us(&systick);

        /*! Convert the raw sensor data to signed 16-bit container for display to the debug port. */
        rawData.accel[0] = ((int16_t)data[0] << 8) | (int16_t)data[1];
        rawData.accel[0] /= 4;
        rawData.accel[1] = ((int16_t)data[2] << 8) | (int16_t)data[3];
        rawData.accel[1] /= 4;
        rawData.accel[2] = ((int16_t)data[4] << 8) | (int16_t)data[5];
        rawData.accel[2] /= 4;

        /* Copy Raw samples to Streaming Buffer. */
        memcpy(streamingPacket + STREAMING_HEADER_LEN, &rawData, FXLS8471_STREAM_DATA_SIZE);
        /* Send streaming packet to Host. */
        Host_IO_Send(streamingPacket, sizeof(streamingPacket), HOST_FORMAT_HDLC);
    }
}
