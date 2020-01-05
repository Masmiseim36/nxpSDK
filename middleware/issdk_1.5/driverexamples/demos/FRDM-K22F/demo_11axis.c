/*
 * Copyright (c) 2017, Freescale Semiconductor, Inc.
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
 * @file demo_11axis.c
 * @brief The demo_11axis.c file implements the ISSDK FXLS8962, MAG3110, FXAS21002 and MPL3115 sensors
 *        12-Axis demo example demonstration for FRDM-K22F.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "board.h"
#include "pin_mux.h"
#include "fsl_uart.h"
#include "clock_config.h"

//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_I2C.h"
#include "Driver_USART.h"
#include "fsl_i2c_cmsis.h"
#include "fsl_uart_cmsis.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "gpio_driver.h"
#include "mag3110_drv.h"
#include "mpl3115_drv.h"
#include "fxls8962_drv.h"
#include "fxas21002_drv.h"
#include "host_io_uart.h"
#include "systick_utils.h"
#include "auto_detection_service.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
#define MPL3115_PADDING_SIZE (1)
#define MPL3115_PRESSURE_DATA_SIZE (3)
#define MPL3115_TEMPERATURE_DATA_SIZE (2)

#define STREAMING_PKT_TIMESTAMP_LEN (4)
#define FXLS8962_DATA_SIZE (6)
#define MAG3110_DATA_SIZE (6)
#define FXAS21002_DATA_SIZE (FXAS21002_GYRO_DATA_SIZE)
#define MPL3115_DATA_SIZE (MPL3115_PRESSURE_DATA_SIZE + MPL3115_TEMPERATURE_DATA_SIZE)

#define UART_RX_RING_BUFFER_SIZE (64)
#define STREAMING_PAYLOAD_LEN \
    (STREAMING_PKT_TIMESTAMP_LEN + FXLS8962_DATA_SIZE + MAG3110_DATA_SIZE + FXAS21002_DATA_SIZE + MPL3115_DATA_SIZE + MPL3115_PADDING_SIZE)

/*! @brief Unique Name for this application which should match the target GUI pkg name. */
#define APPLICATION_NAME "12_AXIS_DEMO"
/*! @brief Version to distinguish between instances the same application based on target Shield and updates. */
#define APPLICATION_VERSION "1.0"

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! @brief Register settings for Interrupt (non buffered) mode. */
const registerwritelist_t cFxls8962ConfigINT[] = {
    /* Set Full-scale range as 2G. */
    {FXLS8962_SENS_CONFIG1, FXLS8962_SENS_CONFIG1_FSR_2G, FXLS8962_SENS_CONFIG1_FSR_MASK},
    /* Set Wake Mode ODR Rate as 25Hz. */
    {FXLS8962_SENS_CONFIG3, FXLS8962_SENS_CONFIG3_WAKE_ODR_25HZ, FXLS8962_SENS_CONFIG3_WAKE_ODR_MASK},
    /* Enable Interrupts for Data Ready Events. */
    {FXLS8962_INT_EN, FXLS8962_INT_EN_DRDY_EN_EN, FXLS8962_INT_EN_DRDY_EN_MASK},
    __END_WRITE_DATA__};

/*! @brief Address of Raw Accel Data in Normal Mode. */
const registerreadlist_t cFxls8962OutputNormal[] = {{.readFrom = FXLS8962_OUT_X_LSB, .numBytes = FXLS8962_DATA_SIZE},
                                                    __END_READ_DATA__};

/*! @brief Register settings for Normal (non buffered) mode. */
const registerwritelist_t cMag3110ConfigNormal[] = {
    /* Set Ouput Rate @40HZ (ODR = 1 and OSR = 16). */
    {MAG3110_CTRL_REG1, MAG3110_CTRL_REG1_DR_ODR_1 | MAG3110_CTRL_REG1_OS_OSR_16,
     MAG3110_CTRL_REG1_DR_MASK | MAG3110_CTRL_REG1_OS_MASK},
    /* Set Auto Magnetic Sensor Reset. */
    {MAG3110_CTRL_REG2, MAG3110_CTRL_REG2_AUTO_MSRT_EN_EN, MAG3110_CTRL_REG2_AUTO_MSRT_EN_MASK},
    __END_WRITE_DATA__};

/*! @brief Address and size of Raw Pressure+Temperature Data in Normal Mode. */
const registerreadlist_t cMag3110OutputNormal[] = {{.readFrom = MAG3110_OUT_X_MSB, .numBytes = MAG3110_DATA_SIZE},
                                                   __END_READ_DATA__};

/*! Prepare the register write list to configure FXAS21002 in non-FIFO mode. */
const registerwritelist_t fxas21002_Config_Normal[] = {
    /*! Configure CTRL_REG1 register to put FXAS21002 to 25Hz sampling rate. */
    {FXAS21002_CTRL_REG1, FXAS21002_CTRL_REG1_DR_25HZ, FXAS21002_CTRL_REG1_DR_MASK},
    __END_WRITE_DATA__};

/*! Prepare the register read list to read the raw gyro data from the FXAS21002. */
const registerreadlist_t fxas21002_Output_Values[] = {
    {.readFrom = FXAS21002_OUT_X_MSB, .numBytes = FXAS21002_DATA_SIZE}, __END_READ_DATA__};

/*! @brief Register settings for Triggring One-Shot Sampling. */
const registerwritelist_t cMpl3115SetOST[] = {
    /* Set the One ShoT Bit. */
    {MPL3115_CTRL_REG1, MPL3115_CTRL_REG1_OST_SET , MPL3115_CTRL_REG1_OST_MASK},
    __END_WRITE_DATA__};

/*! @brief Address and size of Raw Pressure+Temperature Data in Normal Mode. */
const registerreadlist_t mpl3115_Output_Values[] = {{.readFrom = MPL3115_OUT_P_MSB, .numBytes = MPL3115_DATA_SIZE},
                                                    __END_READ_DATA__};

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
char boardString[ADS_MAX_STRING_LENGTH] = {0}, shieldString[ADS_MAX_STRING_LENGTH] = {0},
     embAppName[ADS_MAX_STRING_LENGTH] = {0};
volatile bool bStreamingEnabled = false, bFxls8962DataReady = false, bAgmp03Ready = false;
uint8_t gStreamID;                                       /* The auto assigned Stream ID. */
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
                if (hostCommand[1] == gStreamID && bAgmp03Ready && bStreamingEnabled == false)
                {
                    bStreamingEnabled = true;
                    success = true;
                }
                break;
            case HOST_CMD_STOP:
                if (hostCommand[1] == gStreamID && bAgmp03Ready && bStreamingEnabled == true)
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
    uint8_t secondaryStreamID1, secondaryStreamID2, secondaryStreamID3, /* The auto assigned Stream ID not to be used to stream data. */
            streamingPacket[STREAMING_HEADER_LEN + STREAMING_PAYLOAD_LEN],
            data[FXLS8962_DATA_SIZE + MAG3110_DATA_SIZE + FXAS21002_DATA_SIZE + MPL3115_DATA_SIZE];

    fxls8962_i2c_sensorhandle_t  fxls8962Driver;
    mag3110_i2c_sensorhandle_t   mag3110Driver;
    fxas21002_i2c_sensorhandle_t fxas21002Driver;
    mpl3115_i2c_sensorhandle_t   mpl3115Driver;

    fxls8962_acceldata_t   rawData_fxls8962;
    mag3110_magdata_t      rawData_mag3110;
    fxas21002_gyrodata_t   rawData_fxas21002;
    mpl3115_pressuredata_t rawData_mpl3115;

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
    pGpioDriver->pin_init(&RED_LED, GPIO_DIRECTION_OUT, NULL, NULL, NULL);

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
    status = pUartDriver->Initialize(HOST_B_SIGNAL_EVENT);
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

    do
    {
        /*! Initialize FXLS8962 sensor driver. */
        status = FXLS8962_I2C_Initialize(&fxls8962Driver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, FXLS8962_I2C_ADDR,
                                         FXLS8962_WHOAMI_VALUE);
        if (SENSOR_ERROR_NONE != status)
        {
            break;
        }
        /*! Initialize MAG3110 sensor driver. */
        status = MAG3110_I2C_Initialize(&mag3110Driver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, MAG3110_I2C_ADDR,
                                        MAG3110_WHOAMI_VALUE);
        if (SENSOR_ERROR_NONE != status)
        {
            break;
        }
        /*! Initialize the FXAS21002 sensor driver. */
        status = FXAS21002_I2C_Initialize(&fxas21002Driver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, FXAS21002_I2C_ADDR,
                                          FXAS21002_WHO_AM_I_WHOAMI_PROD_VALUE);
        if (SENSOR_ERROR_NONE != status)
        {
            break;
        }
        /*! Initialize the MPL3115 sensor driver. */
        status = MPL3115_I2C_Initialize(&mpl3115Driver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, MPL3115_I2C_ADDR,
                                        MPL3115_WHOAMI_VALUE);
        if (SENSOR_ERROR_NONE != status)
        {
            break;
        }

        /*!  Set the task to be executed while waiting for SPI transactions to complete. */
        FXLS8962_I2C_SetIdleTask(&fxls8962Driver, (registeridlefunction_t)SMC_SetPowerModeWait, SMC);
        MAG3110_I2C_SetIdleTask(&mag3110Driver, (registeridlefunction_t)SMC_SetPowerModeWait, SMC);
        FXAS21002_I2C_SetIdleTask(&fxas21002Driver, (registeridlefunction_t)SMC_SetPowerModeWait, SMC);
        MPL3115_I2C_SetIdleTask(&mpl3115Driver, (registeridlefunction_t)SMC_SetPowerModeWait, SMC);

        /*! Configure the FXLS8962 sensor. */
        status = FXLS8962_I2C_Configure(&fxls8962Driver, cFxls8962ConfigINT);
        if (SENSOR_ERROR_NONE != status)
        {
            break;
        }
        /*! Configure the MAG3110 sensor driver. */
        status = MAG3110_I2C_Configure(&mag3110Driver, cMag3110ConfigNormal);
        if (SENSOR_ERROR_NONE != status)
        {
            break;
        }
        /*! Configure the FXAS21002 sensor driver. */
        status = FXAS21002_I2C_Configure(&fxas21002Driver, fxas21002_Config_Normal);
        if (SENSOR_ERROR_NONE != status)
        {
            break;
        }
        /*! In One-Shot Mode we do not need to Configure MPL3115, instead we will set OST bit directly. */

        bAgmp03Ready = true;
    } while(false);

    /*! Initialize streaming and assign a Stream ID. */
    gStreamID =
        Host_IO_Init(pUartDriver, (void *)fxls8962Driver.pCommDrv, &fxls8962Driver.deviceInfo, NULL, FXLS8962_I2C_ADDR);
    /* Confirm if a valid Stream ID has been allocated for this stream. */
    if (0 == gStreamID)
    {
        bAgmp03Ready = false;
    }
    secondaryStreamID1 = /* This is required for registering the slave address with Host I/O for Register Read/Write. */
        Host_IO_Init(pUartDriver, (void *)mag3110Driver.pCommDrv, &mag3110Driver.deviceInfo, NULL, MAG3110_I2C_ADDR);
    /* Confirm if a valid Stream ID has been allocated for this stream. */
    if (0 == secondaryStreamID1)
    {
        bAgmp03Ready = false;
    }
    secondaryStreamID2 = /* This is required for registering the slave address with Host I/O for Register Read/Write. */
        Host_IO_Init(pUartDriver, (void *)fxas21002Driver.pCommDrv, &fxas21002Driver.deviceInfo, NULL, FXAS21002_I2C_ADDR);
    /* Confirm if a valid Stream ID has been allocated for this stream. */
    if (0 == secondaryStreamID2)
    {
        bAgmp03Ready = false;
    }
    secondaryStreamID3 = /* This is required for registering the slave address with Host I/O for Register Read/Write. */
        Host_IO_Init(pUartDriver, (void *)mpl3115Driver.pCommDrv, &mpl3115Driver.deviceInfo, NULL, MPL3115_I2C_ADDR);
    /* Confirm if a valid Stream ID has been allocated for this stream. */
    if (0 == secondaryStreamID3)
    {
        bAgmp03Ready = false;
    }

    if (true == bAgmp03Ready)
    {
        *((uint32_t *)&streamingPacket[STREAMING_HEADER_LEN]) = 0; /* Initialize time stamp field. */
        BOARD_SystickStart(&systick);
        pGpioDriver->clr_pin(&RED_LED); /* Set LED to indicate application is ready. */
        /*! Populate streaming header. */
        Host_IO_Add_ISO_Header(gStreamID, streamingPacket, STREAMING_PAYLOAD_LEN);
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
            pGpioDriver->toggle_pin(&RED_LED);
        }

        /* Read timestamp from Systick framework. */
        *((uint32_t *)&streamingPacket[STREAMING_HEADER_LEN]) += BOARD_SystickElapsedTime_us(&systick);

        /*! Read new raw sensor data from the FXLS8962. */
        status = FXLS8962_I2C_ReadData(&fxls8962Driver, cFxls8962OutputNormal, data);
        if (ARM_DRIVER_OK != status)
        {
            return -1;
        }

        /*! Convert the raw sensor data to signed 16-bit container for display to the debug port. */
        rawData_fxls8962.accel[0] = ((int16_t)data[1] << 8) | data[0];
        rawData_fxls8962.accel[1] = ((int16_t)data[3] << 8) | data[2];
        rawData_fxls8962.accel[2] = ((int16_t)data[5] << 8) | data[4];

        /* Copy Raw samples to Streaming Buffer. */
        memcpy(streamingPacket + STREAMING_HEADER_LEN + STREAMING_PKT_TIMESTAMP_LEN,
               &rawData_fxls8962.accel, sizeof(rawData_fxls8962.accel));

        /*! Read raw sensor data from the MAG3110. */
        status = MAG3110_I2C_ReadData(&mag3110Driver, cMag3110OutputNormal, data + FXLS8962_DATA_SIZE);
        if (ARM_DRIVER_OK != status)
        {
            return -1;
        }

        /*! Convert the raw sensor data to signed 32-bit and 16-bit containers for display to the debug port. */
        rawData_mag3110.mag[0] = ((int16_t)data[6] << 8)  | data[7];
        rawData_mag3110.mag[1] = ((int16_t)data[8] << 8)  | data[9];
        rawData_mag3110.mag[2] = ((int16_t)data[10] << 8) | data[11];

        /* Copy Raw samples to Streaming Buffer. */
        memcpy(streamingPacket + STREAMING_HEADER_LEN + STREAMING_PKT_TIMESTAMP_LEN + FXLS8962_DATA_SIZE,
               &rawData_mag3110.mag, sizeof(rawData_mag3110.mag));

        /*! Read the raw sensor data from the FXAS21002. */
        status = FXAS21002_I2C_ReadData(&fxas21002Driver, fxas21002_Output_Values, data + FXLS8962_DATA_SIZE + MAG3110_DATA_SIZE);
        if (ARM_DRIVER_OK != status)
        {
            return -1;
        }

        /* Convert to Little Endian, Right Justified, Even Padded Signed integer counts. */
        rawData_fxas21002.gyro[0] = ((int16_t)data[12] << 8) | data[13];
        rawData_fxas21002.gyro[1] = ((int16_t)data[14] << 8) | data[15];
        rawData_fxas21002.gyro[2] = ((int16_t)data[16] << 8) | data[17];

        /* Copy the converted sample to the streaming buffer. */
        memcpy(streamingPacket + STREAMING_HEADER_LEN + STREAMING_PKT_TIMESTAMP_LEN + FXLS8962_DATA_SIZE + MAG3110_DATA_SIZE,
               &rawData_fxas21002.gyro, sizeof(rawData_fxas21002.gyro));

        /*! Read raw sensor data from the MPL3115. */
        status = MPL3115_I2C_ReadData(&mpl3115Driver, mpl3115_Output_Values, data + FXLS8962_DATA_SIZE + MAG3110_DATA_SIZE + FXAS21002_DATA_SIZE);
        if (ARM_DRIVER_OK != status)
        {
            return -1;
        }

        /* Convert to Little Endian, Right Justified, Even Padded Signed integer counts. */
        rawData_mpl3115.pressure = (uint32_t)((data[18]) << 16) | ((data[19]) << 8) | ((data[20]));
        rawData_mpl3115.pressure /= 16;
        rawData_mpl3115.temperature = (int16_t)((data[21]) << 8) | (data[22]);
        rawData_mpl3115.temperature /= 16;

        /* Copy the converted sample to the streaming buffer. */
        memcpy(streamingPacket + STREAMING_HEADER_LEN + STREAMING_PKT_TIMESTAMP_LEN + FXLS8962_DATA_SIZE + MAG3110_DATA_SIZE + FXAS21002_DATA_SIZE,
               &rawData_mpl3115.pressure, sizeof(rawData_mpl3115.pressure));
        memcpy(streamingPacket + STREAMING_HEADER_LEN + STREAMING_PKT_TIMESTAMP_LEN + FXLS8962_DATA_SIZE + MAG3110_DATA_SIZE + FXAS21002_DATA_SIZE + MPL3115_PRESSURE_DATA_SIZE + MPL3115_PADDING_SIZE,
               &rawData_mpl3115.temperature, sizeof(rawData_mpl3115.temperature));

        /* Trigger acquisition of New Sample. */
        status = Sensor_I2C_Write(mpl3115Driver.pCommDrv, &mpl3115Driver.deviceInfo, mpl3115Driver.slaveAddress,
                                  cMpl3115SetOST);

        /* Send streaming packet to Host. */
        Host_IO_Send(streamingPacket, sizeof(streamingPacket), HOST_FORMAT_HDLC);
    }
}
