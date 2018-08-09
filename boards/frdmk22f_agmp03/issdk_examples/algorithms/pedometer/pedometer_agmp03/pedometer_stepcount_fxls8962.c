/*
 * The Clear BSD License
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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
 * @file pedometer_stepcount_fxls8962.c
 * @brief The pedometer_stepcount_fxls8962.c file implements the ISSDK FXLS8962 sensor
 *        driver example demonstration for Motion Activated Pedometer.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_I2C.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "pedometer.h"
#include "gpio_driver.h"
#include "fxls8962_drv.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
#define SDCD_LTHS_LSB 0xA0    /* Lower Threshold LSB value. */
#define SDCD_LTHS_MSB 0x0F    /* Lower Threshold MSB value. */
#define SDCD_UTHS_LSB 0x40    /* Upper Threshold LSB value. */
#define SDCD_UTHS_MSB 0x00    /* Upper Threshold MSB value. */
#define ASLP_COUNT_LSB 0xFA   /* Auto Sleep after 5s @50Hz Wake ODR */
#define ASLP_COUNT_MSB 0x00   /* Auto Sleep after 5s @50Hz Wake ODR */
#define FXLS8962_DATA_SIZE 6  /* 2 byte X,Y,Z Axis Data each. */

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! @brief FXLS8962 Motion based Pedometer Register Write List. */
const registerwritelist_t cFxls8962ConfigInitialize[] = {
    /* Enable SDCD Events */
    {FXLS8962_SDCD_CONFIG1, FXLS8962_SDCD_CONFIG1_X_OT_EN_EN | FXLS8962_SDCD_CONFIG1_Y_OT_EN_EN |
                            FXLS8962_SDCD_CONFIG1_Z_OT_EN_EN, 0},
    /* Enable SDCD Block */
    {FXLS8962_SDCD_CONFIG2, FXLS8962_SDCD_CONFIG2_SDCD_EN_EN | FXLS8962_SDCD_CONFIG2_REF_UPDM_SDCD_REF |
                            FXLS8962_SDCD_CONFIG2_OT_DBCTM_CLEARED | FXLS8962_SDCD_CONFIG2_WT_DBCTM_CLEARED, 0},
    /* Set WAKE and Sleep ODRs */
    {FXLS8962_SENS_CONFIG3, FXLS8962_SENS_CONFIG3_WAKE_ODR_50HZ | FXLS8962_SENS_CONFIG3_SLEEP_ODR_0_781HZ,
                            FXLS8962_SENS_CONFIG3_WAKE_ODR_MASK | FXLS8962_SENS_CONFIG3_SLEEP_ODR_MASK},
    /* Wake Sleep SDCD OT Event Enabled */
    {FXLS8962_SENS_CONFIG4, FXLS8962_SENS_CONFIG4_WK_SDCD_OT_EN | FXLS8962_SENS_CONFIG4_INT_POL_ACT_LOW,
                            FXLS8962_SENS_CONFIG4_WK_SDCD_OT_MASK | FXLS8962_SENS_CONFIG4_INT_POL_MASK},
    /* ASLP and Data INT enabled */
    {FXLS8962_INT_EN, FXLS8962_INT_EN_ASLP_EN_EN | FXLS8962_INT_EN_DRDY_EN_EN, 0},
    /* Auto Sleep LSB value */
    {FXLS8962_ASLP_COUNT_LSB, ASLP_COUNT_LSB, 0},
    /* Auto Sleep MSB value */
    {FXLS8962_ASLP_COUNT_MSB, ASLP_COUNT_MSB, 0},
    /* Set SDCD Lower Threshold LSB value. */
    {FXLS8962_SDCD_LTHS_LSB, SDCD_LTHS_LSB, 0},
    /* Set SDCD Lower Threshold MSB value. */
    {FXLS8962_SDCD_LTHS_MSB, SDCD_LTHS_MSB, 0},
    /* Set SDCD Upper Threshold LSB value. */
    {FXLS8962_SDCD_UTHS_LSB, SDCD_UTHS_LSB, 0},
    /* Set SDCD Upper Threshold MSB value. */
    {FXLS8962_SDCD_UTHS_MSB, SDCD_UTHS_MSB, 0},
    __END_WRITE_DATA__};

/*! @brief FXLS8962 Motion Detect Mode Register Write List. */
const registerwritelist_t cFxls8962ConfigMotionDetect[] = {
    /* Only SDCD INT Enabled */
    {FXLS8962_INT_EN, FXLS8962_INT_EN_SDCD_OT_EN_EN, 0},
    __END_WRITE_DATA__};

/*! @brief FXLS8962 DRDY and ASLP Detect Mode Register Write List. */
const registerwritelist_t cFxls8962ConfigDataReady[] = {
    /* Data and ASLP INT Enabled */
    {FXLS8962_INT_EN, FXLS8962_INT_EN_ASLP_EN_EN | FXLS8962_INT_EN_DRDY_EN_EN, 0},
    __END_WRITE_DATA__};

/*! @brief Address of INT Status Register. */
const registerreadlist_t cFxls8962INTStatus[] = {{.readFrom = FXLS8962_INT_STATUS, .numBytes = 1}, __END_READ_DATA__};

/*! @brief Address of Data Output Registers. */
const registerreadlist_t cFxls8962Output[] = {{.readFrom = FXLS8962_OUT_X_LSB, .numBytes = FXLS8962_DATA_SIZE}, __END_READ_DATA__};

/*! @brief Pedometer Mode Name Strings. */
const char *pActivity[5] = {"Unknown ", "Rest    ", "Walking ", "Jogging ", "Running "};

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
volatile bool gFxls8962EventReady = false;

/* Pedometer configuration. These configuration are algorithm and user dependent data. */
pedometer_config_t cPedoConfig =
{
    .sleepcount_threshold = 1,
    .bits = {.config = 1},
    .keynetik =
    {
        .steplength = 0,
        .height = 175,
        .weight = 80,
        .filtersteps = 1,
        .bits =
        {
            .filtertime = 2, .male = 1,
        },
        .speedperiod = 3,
        .stepthreshold = 0,
    },
    .stepcoalesce = 1,
    .oneG = PEDO_ONEG_2G,
    .frequency = 50,
};

gpioConfigKSDK_t gpioConfigINT1 =
{
    .pinConfig = {kGPIO_DigitalInput, 0},
    .portPinConfig = {.pullSelect = kPORT_PullUp, .mux = kPORT_MuxAsGpio},
    .interruptMode = kPORT_InterruptFallingEdge,
};

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
/*! -----------------------------------------------------------------------
 *  @brief       This is the Sensor Event Ready ISR implementation.
 *  @details     This function sets the flag which indicates if a new sample(s) is available for reading.
 *  @param[in]   pUserData This is a void pointer to the instance of the user specific data structure for the ISR.
 *  @return      void  There is no return value.
 *  @constraints None
 *  @reeentrant  Yes
 *  -----------------------------------------------------------------------*/
void fxls8962_int_callback(void *pUserData)
{ /*! @brief Set flag to indicate Sensor has signaled event ready. */
    gFxls8962EventReady = true;
}

/*! -----------------------------------------------------------------------
 *  @brief       This is the The main function implementation.
 *  @details     This function invokes board initializes routines, then then brings up the sensor and
 *               finally enters an endless loop to continuously read available samples.
 *  @param[in]   void This is no input parameter.
 *  @return      void  There is no return value.
 *  @constraints None
 *  @reeentrant  No
 *  -----------------------------------------------------------------------*/
int main(void)
{
    int32_t status;
    bool motionDetect;
    uint16_t lastReportedSteps;
    uint8_t intStatus, data[FXLS8962_DATA_SIZE];

    pedometer_t pedometer; /* This handle holds the current configuration and status value for the pedometer.*/
    fxls8962_acceldata_t rawData;
    fxls8962_i2c_sensorhandle_t fxls8962Driver;

    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER; // Now using the shield.h value!!!
    GENERIC_DRIVER_GPIO *pGpioDriver = &Driver_GPIO_KSDK;

    /*! Initialize the MCU hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\n ISSDK FXLS8962 sensor driver example for Motion Activated Pedometer.\r\n");
    
    /*! Initialize FXLS8962 pin used by FRDM board */
    pGpioDriver->pin_init(&FXLS8962_INT1, GPIO_DIRECTION_IN, &gpioConfigINT1, &fxls8962_int_callback, NULL);

    /*! Initialize RGB LED pin used by FRDM board */
    pGpioDriver->pin_init(&GREEN_LED, GPIO_DIRECTION_OUT, NULL, NULL, NULL);

    /*! Initialize the I2C driver. */
    status = I2Cdrv->Initialize(I2C_S_SIGNAL_EVENT);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Initialization Failed\r\n");
        return -1;
    }

    /*! Set the I2C Power mode. */
    status = I2Cdrv->PowerControl(ARM_POWER_FULL);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Power Mode setting Failed\r\n");
        return -1;
    }

    /*! Set the I2C bus speed. */
    status = I2Cdrv->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Control Mode setting Failed\r\n");
        return -1;
    }

    /*! Initialize FXLS8962 sensor driver. */
    status = FXLS8962_I2C_Initialize(&fxls8962Driver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, FXLS8962_I2C_ADDR,
                                     FXLS8962_WHOAMI_VALUE);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n Sensor Initialization Failed\r\n");
        return -1;
    }

    /*!  Set the task to be executed while waiting for I2C transactions to complete. */
    FXLS8962_I2C_SetIdleTask(&fxls8962Driver, (registeridlefunction_t)SMC_SetPowerModeWait, SMC);

    /* Apply FXLS8962 Configuration for Motion Activated Pedometer. */
    status = Sensor_I2C_Write(fxls8962Driver.pCommDrv, &fxls8962Driver.deviceInfo,
                              fxls8962Driver.slaveAddress, cFxls8962ConfigInitialize);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n Write FXLS8962 Initialization Failed.\r\n");
        return -1;
    }

    /*! Initialize the pedometer*/
    pedometer_init(&pedometer);
    PRINTF("\r\n Pedometer successfully Initialized and Ready for measurements.");

    for (;;) /* Forever loop for Motion Detection */
    {
        /*! Configure the pedometer*/
        pedometer_configure(&pedometer, &cPedoConfig);

        /* Apply FXLS8962 Configuration for Motion Detection. */
        status = FXLS8962_I2C_Configure(&fxls8962Driver, cFxls8962ConfigMotionDetect);
        if (SENSOR_ERROR_NONE != status)
        {
            PRINTF("\r\n Write FXLS8962 Motion Configuration Failed!\r\n");
            return -1;
        }

        motionDetect = true;
        pGpioDriver->set_pin(&GREEN_LED);
        PRINTF("\r\n\r\n Waiting for Motion | MCU switching to Sleep Mode ...\r\n");

        for (;;) /* Loop for Motion Detection */
        {   /* In ISR Mode we do not need to check Data Ready Register.
             * The receipt of interrupt will indicate event is ready. */
            if (false == gFxls8962EventReady)
            { /* Loop, if new sample is not available. */
                SMC_SetPowerModeWait(SMC);
                continue;
            }
            else
            { /*! Clear the data ready flag, it will be set again by the ISR. */
                gFxls8962EventReady = false;
            }

            /*! Read the INT Status from the FXLS8962. */
            status = FXLS8962_I2C_ReadData(&fxls8962Driver, cFxls8962INTStatus, &intStatus);
            if (ARM_DRIVER_OK != status)
            {
                PRINTF("\r\n Read INT Status Failed!\r\n");
                return -1;
            }

            if (motionDetect)
            {
                if ((intStatus & FXLS8962_INT_STATUS_SRC_SDCD_OT_MASK) == FXLS8962_INT_STATUS_SRC_SDCD_OT_MASK)
                {
                    /*! Display that a Motion event has been detected. */
                    PRINTF(" Motion detected...\r\n");

                    /* Apply FXLS8962 Configuration for ASLP Detection with DATA. */
                    status = FXLS8962_I2C_Configure(&fxls8962Driver, cFxls8962ConfigDataReady);
                    if (SENSOR_ERROR_NONE != status)
                    {
                        PRINTF("\r\n Write FXLS8962 DRDY Configuration Failed.\r\n");
                        return -1;
                    }
                    motionDetect = false;
                    pGpioDriver->clr_pin(&GREEN_LED);
                }
            }
            else
            {
                /*! Read the Output from the FXLS8962. */
                status = FXLS8962_I2C_ReadData(&fxls8962Driver, cFxls8962Output, data);
                if (ARM_DRIVER_OK != status)
                {
                    PRINTF("\r\n ERROR : Read Data Failed!\r\n");
                    return -1;
                }

                /*! Convert the raw sensor data for feeding to pedometer algorithm. */
                rawData.accel[0] = ((int16_t)data[1] << 8) | data[0];
                rawData.accel[0] *= 16; /* Pedometer requires signed 12-bit Left justified data @1024 counts/g. */
                rawData.accel[1] = ((int16_t)data[3] << 8) | data[2];
                rawData.accel[1] *= 16; /* Pedometer requires signed 12-bit Left justified data @1024 counts/g. */
                rawData.accel[2] = ((int16_t)data[5] << 8) | data[4];
                rawData.accel[2] *= 16; /* Pedometer requires signed 12-bit Left justified data @1024 counts/g. */

                /*!  Execute the pedometer Algorithm */
                pedometer_run(&pedometer, (ped_accel_t *)&rawData.accel);

                /* Display Steps when there is a change */
                if (pedometer.status.stepcount && pedometer.status.stepcount != lastReportedSteps)
                {
                    pGpioDriver->toggle_pin(&GREEN_LED);
                    lastReportedSteps = pedometer.status.stepcount;
                    if(pedometer.status.stepcount == 1)
                    {
                        PRINTF("\r\n | Steps | Distance |  Speed  | Calories | Activity |\r\n");
                    }
                    PRINTF("\033[K | %5d |    %4dm |  %2dkmph |      %3d | %s |\r",
                           pedometer.status.stepcount, pedometer.status.distance, pedometer.status.speed/1000, pedometer.status.calories, pActivity[pedometer.status.status.bits.activity]);
                }

                if ((intStatus & FXLS8962_INT_STATUS_SRC_ASLP_MASK) == FXLS8962_INT_STATUS_SRC_ASLP_MASK)
                {
                    break;
                }
            }
        }
    }
}
