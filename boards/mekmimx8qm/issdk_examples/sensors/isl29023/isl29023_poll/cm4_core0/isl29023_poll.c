/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file isl29023_poll.c
 * @brief The isl29023_poll.c file implements the ISSDK ISL29023 sensor driver
 *        example demonstration in poll mode.
 */

/*  SDK Includes */
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_irqsteer.h"

/* CMSIS Includes */
#include "Driver_I2C.h"

/* ISSDK Includes */
#include "issdk_hal.h"
#include "isl29023_drv.h"

/*******************************************************************************
 * Macro Definitions
 ******************************************************************************/
/*! @def    ISL29023_DATA_SIZE
 *  @brief  The size of the ISL29023 sensor data.
 */
#define ISL29023_DATA_SIZE   (2)         /* 2 bytes of information */

/*! @def    ISL29023_INT_HT_DATA & ISL29023_INT_LT_DATA
 *  @brief  ISL29023 Sensor High & Low treshold values. When the current light value 
 *          is higher than HI-IRQ treshold value, or lower than LO-IRQ treshold value,
 *          the interrupt is generated. This means the new value is ready to sending out.
 *          The natural daylight value is between 20000-28000. 
 *  @see    For more info: https://www.intersil.com/content/dam/intersil/documents/isl2/isl29023.pdf
 */
#define ISL29023_INT_HT_DATA (11000)     /* HI-IRQ treshold value (relative to 16-bit res.)*/
#define ISL29023_INT_LT_DATA (5000)      /* LO-IRQ treshold value (relative to 16-bit res.)*/

/*******************************************************************************
 * Constants
 ******************************************************************************/
/*! Prepare the register write list to configure ISL29023 in 16-bits ALS continuous mode. */
const registerwritelist_t ISL29023_Config_Poll[] = {
    /*! Configure CMD-I register to 1th integration cycles, and ALS cont. mode. */
    {ISL29023_CMD_I, ISL29023_CMD_I_PRST_1 | ISL29023_CMD_I_OP_ALS_CONT,
                     ISL29023_CMD_I_PRST_MASK | ISL29023_CMD_I_OP_MASK},
    /*! Configure CMD-II register to Range #1, and 16-bits ADC resolution. */
    {ISL29023_CMD_II, ISL29023_CMD_II_RANGE_1 | ISL29023_CMD_II_RES_16,
                      ISL29023_CMD_II_RANGE_MASK | ISL29023_CMD_II_RES_MASK},
    /*! Configure Low interrupt treshold registers. */
    {ISL29023_INT_LT_LSB, (ISL29023_INT_LT_DATA & 0x00FF), 0xFF},
    {ISL29023_INT_LT_MSB, ((ISL29023_INT_LT_DATA & 0xFF00)>>8), 0xFF},
    /*! Configure High interrupt treshold registers. */
    {ISL29023_INT_HT_LSB, (ISL29023_INT_HT_DATA & 0x00FF), 0xFF},
    {ISL29023_INT_HT_MSB, ((ISL29023_INT_HT_DATA & 0xFF00)>>8), 0xFF},
    __END_WRITE_DATA__};

/*! Prepare the register read list to read ISL29023 IRQ flag bit. */
const registerreadlist_t ISL29023_Status[] = {{.readFrom = ISL29023_CMD_I, .numBytes = 1}, __END_READ_DATA__};

/*! Prepare the register read list to read the raw ADC data from the ISL29023. */
const registerreadlist_t ISL29023_Output_Values[] = {
    {.readFrom = ISL29023_DATA_LSB, .numBytes = ISL29023_DATA_SIZE}, __END_READ_DATA__};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    int32_t status;
    uint8_t irqStatus;
    uint8_t data[ISL29023_DATA_SIZE];
    isl29023_lightdata_t rawData;
    sc_ipc_t ipc;
    uint32_t freq;

    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER;
    isl29023_i2c_sensorhandle_t ISL29023drv;

    /*! Initialize the MCU hardware. */
    ipc = BOARD_InitRpc();
    BOARD_InitMemory();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    PRINTF("\r\n ------------------------------------------------------------------\r\n");
    PRINTF("\r\n ISSDK ISL29023 sensor driver example demonstration with POLL mode\r\n");
    PRINTF("\r\n ------------------------------------------------------------------\r\n");

    /* Power on LPI2C. */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_I2C_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on LPI2C\r\n");
    }

    /* Set LPI2C clock */
    freq = CLOCK_SetIpFreq(kCLOCK_DMA_Lpi2c0, SC_24MHZ);
    if (freq == 0)
    {
        PRINTF("Error: Failed to set LPI2C frequency\r\n");
    }

    /* Power on IRQSTEER. */
#if (MIMX8QM_CM4_CORE0)
    if (sc_pm_set_resource_power_mode(ipc, SC_R_IRQSTR_M4_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTEER M4_0\r\n");
    }
#elif (MIMX8QM_CM4_CORE1)
    if (sc_pm_set_resource_power_mode(ipc, SC_R_IRQSTR_M4_1, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTEER M4_1\r\n");
    }
#endif

    /* Enable interrupt in irqsteer */
    IRQSTEER_Init(IRQSTEER);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA_I2C0_INT_IRQn);

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

    /*! Initialize the ISL29023 sensor driver. */
    status = ISL29023_I2C_Initialize(&ISL29023drv, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, ISL29023_I2C_ADDRESS,
                                      ISL29023_I2C_TEST_VALUE);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n Sensor Initialization Failed\r\n");
        return -1;
    }
    PRINTF("\r\n Successfully Initiliazed ISL29023 Sensor\r\n");
    PRINTF("\r\n Device Address is 0x%x \r\n", ISL29023_I2C_ADDRESS);

    /*! Configure the ISL29023 sensor driver. */
    status = ISL29023_I2C_Configure(&ISL29023drv, ISL29023_Config_Poll);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n ISL29023 Sensor Configuration Failed, Err = %d\r\n", status);
        return -1;
    }
    PRINTF("\r\n Successfully Applied ISL29023 Sensor Configuration\r\n");

    PRINTF("\r\n Reading ISL29023 ADC data\r\n");

    for (;;) /* Forever loop */
    {
        /*! Wait for clear IRQ flag bit. */
        status = ISL29023_I2C_ReadData(&ISL29023drv, ISL29023_Status, &irqStatus);
        if (0 == (irqStatus & ISL29023_CMD_I_FLAG_MASK))
        {
            continue;
        }

        /*! Read the raw sensor data from the ISL29023. */
        status = ISL29023_I2C_ReadData(&ISL29023drv, ISL29023_Output_Values, data);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\n Read Failed. \r\n");
            return -1;
        }

        /*! Convert the raw sensor data to signed 16-bit container for display to the debug port. */
        rawData.light = ((uint16_t)data[1] << 8) | data[0];

        /* NOTE: PRINTF is relatively expensive in terms of CPU time, specially when used with-in execution loop. */
        PRINTF("Light value: %d\r\n", rawData.light);
        ASK_USER_TO_RESUME(20); /* Ask for user input after processing 20 samples. */
    }
}
