/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file evkmimxrt595.h
 * @brief The evkmimxrt595.h file defines GPIO pin mappings for evkmimxrt595 board
 */

#ifndef EVKMIMX595_H_
#define EVKMIMX595_H_

#include "pin_mux.h"
//#include "fsl_msmc.h"
#include "RTE_Device.h"
#include "gpio_driver.h"
#include "fsl_power.h"

// I2C Pin Handles
extern gpioHandleiMXSDK_t D15;
extern gpioHandleiMXSDK_t D14;

// I2C2 Handle
extern gpioHandleiMXSDK_t A5;
extern gpioHandleiMXSDK_t A4;

// SPI5 Handle
extern gpioHandleiMXSDK_t D13;
extern gpioHandleiMXSDK_t D11;
extern gpioHandleiMXSDK_t D12;

// USART0 Handle
extern gpioHandleiMXSDK_t D0;
extern gpioHandleiMXSDK_t D1;

// I2C_S1: Pin mapping and driver information for default I2C brought to shield
#define I2C_S1_SCL_PIN      A5
#define I2C_S1_SDA_PIN      A4
#define I2C_S1_DRIVER       Driver_I2C11
#define I2C_S1_DEVICE_INDEX I2C11_INDEX
#define I2C_S1_SIGNAL_EVENT I2C11_SignalEvent_t

// I2C_S2: Pin mapping and driver information for alternate I2C bus on shield
#define I2C_S2_SCL_PIN      D15
#define I2C_S2_SDA_PIN      D14
#define I2C_S2_DRIVER       Driver_I2C11
#define I2C_S2_DEVICE_INDEX I2C11_INDEX
#define I2C_S2_SIGNAL_EVENT I2C11_SignalEvent_t

// SPIS: Pin mapping and driver information default SPI brought to shield
#define SPI_S_SCLK         D13
#define SPI_S_MISO         D12
#define SPI_S_MOSI         D11
#define SPI_S_DRIVER       Driver_SPI1
#define SPI_S_BAUDRATE     500000U ///< Transfer baudrate - 500k
#define SPI_S_DEVICE_INDEX SPI1_INDEX
#define SPI_S_SIGNAL_EVENT SPI1_SignalEvent_t

// UART: Driver information for default UART to communicate with HOST PC.
#define HOST_S_DRIVER       Driver_USART1
#define HOST_S_SIGNAL_EVENT HOST_SignalEvent_t

// On-Board FXOS8700 Sensor Information
#define FXOS8700_BB_I2C_ADDR 0x1E
#define FXOS8700_BB_INT1     INT1
#define FXOS8700_BB_INT2     INT2

/* @brief  Ask use input to resume after specified samples have been processed. */
#define ASK_USER_TO_RESUME(x)                                                          \
    static volatile bool askResume   = true;                                           \
    static uint16_t samplesToProcess = x - 1;                                          \
    if (askResume && !samplesToProcess--)                                              \
    {                                                                                  \
        PRINTF("\r\n Specified samples processed, press any key to continue... \r\n"); \
        GETCHAR();                                                                     \
        askResume = false;                                                             \
    }

/* @brief dummy arguement to Power Mode Wait Wrapper. */
#define SMC NULL

/* @brief Kinetis style Wrapper API for Power Mode Wait (Wait for Interrupt). */
status_t SMC_SetPowerModeWait(void *);
/* @brief Kinetis style Wrapper API for Power Mode VLPR (Wait for Interrupt). */
status_t SMC_SetPowerModeVlpr(void *);

#endif /* EVKMIMX595_H_ */
