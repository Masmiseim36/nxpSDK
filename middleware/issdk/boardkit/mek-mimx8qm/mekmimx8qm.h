/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file mekmimx8qm.h
 * @brief The mekmimx8qm.h file defines GPIO pin mappings and on-board sensors information for mekmimx8qm board
*/

#ifndef MEKMIMX8QM_H_
#define MEKMIMX8QM_H_

#include "pin_mux.h"
#include "RTE_Device.h"
#include "gpio_driver.h"
#include "main/imx8qm_pads.h"
#include "svc/pad/pad_api.h"

/* I2C_BB: Pin mapping and driver information for I2C0 routed on base board */
#define I2C_BB_SCL_PIN      SC_P_HDMI_TX0_TS_SCL
#define I2C_BB_SDA_PIN      SC_P_HDMI_TX0_TS_SDA
#define I2C_BB_DRIVER       Driver_I2C2
#define I2C_BB_DEVICE_INDEX I2C2_INDEX
#define I2C_BB_SIGNAL_EVENT I2C2_SignalEvent_t

/* UART: Driver information for default UART to communicate with HOST PC. */
#define HOST_S_DRIVER       Driver_USART0
#define HOST_S_SIGNAL_EVENT HOST_SignalEvent_t

/* On-Board FXOS8700 Sensor Information */
#define FXOS8700_BB_I2C_ADDR 0x1E
//#define FXOS8700_BB_INT1 PRS_INT1
//#define FXOS8700_BB_INT2 PRS_INT2

/* On-Board FXAS21002 Sensor Information */
#define FXAS21002_BB_I2C_ADDR 0x20
//#define FXAS21002_BB_INT1 GY_INT1
//#define FXAS21002_BB_INT2 GY_INT2

/* On-Board MPL3115 Sensor Information */
#define MPL3115_BB_I2C_ADDR 0x60
//#define MPL3115_BB_INT1 PSA_INT1
//#define MPL3115_BB_INT2 PSA_INT2

/* @brief  Ask use input to resume after specified samples have been processed. */
#define ASK_USER_TO_RESUME(x)                                                          \
    static volatile bool askResume = true;                                             \
    static uint16_t samplesToProcess = x - 1;                                          \
    if (askResume && !samplesToProcess--)                                              \
    {                                                                                  \
        PRINTF("\r\n Specified samples processed, press any key to continue... \r\n"); \
        GETCHAR();                                                                     \
        askResume = false;                                                             \
    }

/* Compatability mapping for mek-mimx8qm i2c/spi definitions */
#define I2C2          LPI2C2
#define I2C_Type      LPI2C_Type
#define I2C_BASE_PTRS LPI2C_BASE_PTRS
#define SPI1          LPSPI1
#define SPI_Type      LPSPI_Type
#define SPI_BASE_PTRS LPSPI_BASE_PTRS

/* On-Board Sensor I2C address mapping for generic sensor examples */
#define FXAS21002_I2C_ADDR FXAS21002_BB_I2C_ADDR
#define FXOS8700_I2C_ADDR  FXOS8700_BB_I2C_ADDR
#define MPL3115_I2C_ADDR   MPL3115_BB_I2C_ADDR

/* @brief dummy arguement to Power Mode Wait Wrapper. */
#define SMC NULL

/* @brief Kinetis style Wrapper API for Power Mode Wait (Wait for Interrupt). */
status_t SMC_SetPowerModeWait(void *);
/* @brief Kinetis style Wrapper API for Power Mode VLPR (Wait for Interrupt). */
status_t SMC_SetPowerModeVlpr(void *);

#endif /* MEKMIMX8QM_H_ */
