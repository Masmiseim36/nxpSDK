/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file mimxrt700evk.h
 * @brief The mimxrt700evk.h file defines GPIO pin mappings for mimxrt700evk board
 */

#ifndef MIMXRT700EVK_H_
#define MIMXRT700EVK_H_

#include "pin_mux.h"
#include "RTE_Device.h"
#include "gpio_driver.h"

extern gpioHandleiMXSDK_t RED_LED;
extern gpioHandleiMXSDK_t GREEN_LED;
extern gpioHandleiMXSDK_t BLUE_LED;

// I2C_S2: Pin mapping and driver information for alternate I2C bus on shield
#define I2C_S2_DRIVER       Driver_I2C8
#define I2C_S2_DEVICE_INDEX I2C8_INDEX
#define I2C_S2_SIGNAL_EVENT I2C8_SignalEvent_t

// SPIS: Pin mapping and driver information default SPI brought to shield
#define SPI_S_DRIVER       Driver_SPI14
#define SPI_S_BAUDRATE     500000U ///< Transfer baudrate - 500k
#define SPI_S_DEVICE_INDEX SPI14_INDEX
#define SPI_S_SIGNAL_EVENT SPI14_SignalEvent_t

// UART: Driver information for default UART to communicate with HOST PC.
#define HOST_S_DRIVER       Driver_USART1
#define HOST_S_SIGNAL_EVENT HOST_SignalEvent_t

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

/* Compatability definitions for evkmimxrt1040 */
#define I2C8          LPI2C8
#define I2C_Type      LPI2C_Type
#define I2C_BASE_PTRS LPI2C_BASE_PTRS
#define SPI14          LPSPI14
#define SPI_Type      LPSPI_Type
#define SPI_BASE_PTRS LPSPI_BASE_PTRS

/* @brief dummy arguement to Power Mode Wait Wrapper. */
#define SMC NULL

#endif /* MIMXRT700EVK_H_ */
