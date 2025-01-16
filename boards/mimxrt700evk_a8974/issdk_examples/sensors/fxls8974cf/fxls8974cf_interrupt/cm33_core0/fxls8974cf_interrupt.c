/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file fxls8974cf_interrupt.c
 * @brief The fxls8974cf_interrupt.c file implements the ISSDK FXLS8974CF sensor
 *        driver example demonstration with interrupt mode.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "gpio_driver.h"
#include "fxls8974_drv.h"
#include "systick_utils.h"

//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_I2C.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
#define FXLS8974_DATA_SIZE 6
#define FXLS8974_INT1_GPIO     GPIO2
#define FXLS8974_INT1_PIN      14U
#define FXLS8974_INT1_IRQ      GPIO20_IRQn
#define FXLS8974_INT1_ISR      GPIO20_IRQHandler
#define BOARD_LED_GPIO         BOARD_LED_GREEN_GPIO
#define BOARD_LED_GPIO_PIN     BOARD_LED_GREEN_GPIO_PIN

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! @brief Register settings for Interrupt (non buffered) mode. */
const registerwritelist_t cFxls8974ConfigNormal[] = {
    /* Set Full-scale range as 2G. */
    {FXLS8974_SENS_CONFIG1, FXLS8974_SENS_CONFIG1_FSR_2G, FXLS8974_SENS_CONFIG1_FSR_MASK},
    /* Set Wake Mode ODR Rate as 6.25Hz. */
    {FXLS8974_SENS_CONFIG3, FXLS8974_SENS_CONFIG3_WAKE_ODR_6_25HZ, FXLS8974_SENS_CONFIG3_WAKE_ODR_MASK},
    /* Enable Interrupts for Data Ready Events. */
    {FXLS8974_INT_EN, FXLS8974_INT_EN_DRDY_EN_EN, FXLS8974_INT_EN_DRDY_EN_MASK},
    {FXLS8974_INT_PIN_SEL, FXLS8974_INT_PIN_SEL_DRDY_INT2_DIS, FXLS8974_INT_PIN_SEL_DRDY_INT2_MASK},
    __END_WRITE_DATA__};

/*! @brief Address of Raw Accel Data in Normal Mode. */
const registerreadlist_t cFxls8974OutputNormal[] = {{.readFrom = FXLS8974_OUT_X_LSB, .numBytes = FXLS8974_DATA_SIZE},
                                                    __END_READ_DATA__};

const registerreadlist_t cFxls8974IntEn[] = {{.readFrom = FXLS8974_INT_STATUS, .numBytes = 1},
                                                    __END_READ_DATA__};

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
volatile bool gFxls8974DataReady = false;

ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER;
fxls8974_i2c_sensorhandle_t fxls8974Driver;
GENERIC_DRIVER_GPIO *pGpioDriver = &Driver_GPIO_KSDK;
/* Define the init structure for the interrupt pin */
gpio_pin_config_t int1_config = {
    kGPIO_DigitalInput,
    0,
};
//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
/*! -----------------------------------------------------------------------
 *  @brief       This is the Sensor Data Ready ISR implementation.
 *  @details     This function sets the flag which indicates SDCD motion wake up event occurred.
 *  @return      void  There is no return value.
 *  -----------------------------------------------------------------------*/
void FXLS8974_INT1_ISR(void)
{
    /* Clear external interrupt flag. */
    GPIO_GpioClearInterruptFlags(FXLS8974_INT1_GPIO, 1U << FXLS8974_INT1_PIN);
    /* Change state of button. */
    gFxls8974DataReady = true;
    SDK_ISR_EXIT_BARRIER;
}

/*! -----------------------------------------------------------------------
 *  @brief       Initialize FXLS8974CF Interrupt Pin and Enable IRQ
 *  @details     This function initializs FXLS8974CF interrupt pin to wake up sensor
 *  @return      void  There is no return value.
 *  -----------------------------------------------------------------------*/
void init_fxls8974_wakeup_int(void)
{
    CLOCK_EnableClock(kCLOCK_Gpio2);
    CLOCK_EnableClock(kCLOCK_Gpio0);
    GPIO_SetPinInterruptConfig(FXLS8974_INT1_GPIO, FXLS8974_INT1_PIN, kGPIO_InterruptRisingEdge);

    EnableIRQ(FXLS8974_INT1_IRQ);
    GPIO_PinInit(FXLS8974_INT1_GPIO, FXLS8974_INT1_PIN, &int1_config);
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
    uint8_t whoami;
    uint8_t int_en;
    uint8_t data[FXLS8974_DATA_SIZE];
    fxls8974_acceldata_t rawData;

    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER; // Now using the shield.h value!!!
    fxls8974_i2c_sensorhandle_t fxls8974Driver;
    GENERIC_DRIVER_GPIO *pGpioDriver = &Driver_GPIO_KSDK;

    /*! Initialize the MCU hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_SystickEnable();
    BOARD_InitDebugConsole();

    PRINTF("\r\n ISSDK FXLS8974 sensor driver example demonstration with interrupt mode.\r\n");

    /*! Initialize FXLS8974 wakeup pin used by MCX board */
    init_fxls8974_wakeup_int();

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

    /*! Initialize FXLS8974 sensor driver. */
    status = FXLS8974_I2C_Initialize(&fxls8974Driver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, FXLS8974_I2C_ADDR,
                                     &whoami);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n Sensor Initialization Failed\r\n");
        return -1;
    }
    if ((FXLS8964_WHOAMI_VALUE == whoami) || (FXLS8967_WHOAMI_VALUE == whoami))
    {
    	PRINTF("\r\n Successfully Initialized Gemini with WHO_AM_I = 0x%X\r\n", whoami);
    }
    else if ((FXLS8974_WHOAMI_VALUE == whoami) || (FXLS8968_WHOAMI_VALUE == whoami))
    {
    	PRINTF("\r\n Successfully Initialized Timandra with WHO_AM_I = 0x%X\r\n", whoami);
    }
    else if ((FXLS8971_WHOAMI_VALUE == whoami) || (FXLS8961_WHOAMI_VALUE == whoami))
    {
    	PRINTF("\r\n Successfully Initialized Chiron with WHO_AM_I = 0x%X\r\n", whoami);
    }
    else if (FXLS8962_WHOAMI_VALUE == whoami)
    {
    	PRINTF("\r\n Successfully Initialized Newstein with WHO_AM_I = 0x%X\r\n", whoami);
    }
    else
    {
    	PRINTF("\r\n Bad WHO_AM_I = 0x%X\r\n", whoami);
    }

    /*! Configure the FXLS8974 sensor. */
    status = FXLS8974_I2C_Configure(&fxls8974Driver, cFxls8974ConfigNormal);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n FXLS8974 Sensor Configuration Failed, Err = %d\r\n", status);
        return -1;
    }
    PRINTF("\r\n Successfully Applied FXLS8974 Sensor Configuration\r\n");

    for (;;) /* Forever loop */
    {        /* In ISR Mode we do not need to check Data Ready Register.
              * The receipt of interrupt will indicate data is ready. */
#if 0 //Waiting for D2 Interrupt to occur
        if (false == gFxls8974DataReady)
        { /* Loop, if new sample is not available. */
            SMC_SetPowerModeWait(SMC);
            continue;
        }
        else
        { /*! Clear the data ready flag, it will be set again by the ISR. */
            gFxls8974DataReady = false;
            pGpioDriver->toggle_pin(&GREEN_LED);
        }
#else
        /*! Read INT Status from the FXLS8974. */
        status = FXLS8974_I2C_ReadData(&fxls8974Driver, cFxls8974IntEn, &int_en);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\n Read Failed. \r\n");
            return -1;
        }
        /* Check whether DRDY Interrupt was raised */
        if ((int_en & FXLS8974_INT_STATUS_SRC_DRDY_MASK) != 0x80)
        {
          continue;
        }

        pGpioDriver->toggle_pin(&GREEN_LED);
#endif
        /*! Read new raw sensor data from the FXLS8974. */
        status = FXLS8974_I2C_ReadData(&fxls8974Driver, cFxls8974OutputNormal, data);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\n Read Failed. \r\n");
            return -1;
        }

        /*! Process the sample and convert the raw sensor data. */
        rawData.accel[0] = ((int16_t)data[1] << 8) | data[0];
        rawData.accel[1] = ((int16_t)data[3] << 8) | data[2];
        rawData.accel[2] = ((int16_t)data[5] << 8) | data[4];

        /* NOTE: PRINTF is relatively expensive in terms of CPU time, specially when used with-in execution loop. */
        PRINTF("\r\n X=%5d Y=%5d Z=%5d\r\n", rawData.accel[0], rawData.accel[1], rawData.accel[2]);
    }
}
