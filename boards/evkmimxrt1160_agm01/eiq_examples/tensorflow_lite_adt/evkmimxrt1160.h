/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file evkmimxrt1160.h
 * @brief The evkmimxrt1160.h file defines GPIO pin mappings for evkmimxrt1160 board
 */

#ifndef EVKBIMX1170_H_
#define EVKBIMX1170_H_

#include "pin_mux.h"
#include "board.h"
//#include "fsl_msmc.h"
#include "RTE_Device.h"
#include "gpio_driver.h"
#include "MIMXRT1166_cm7.h"

// I2C_S1: Pin mapping and driver information for default I2C brought to shield
#define I2C_S1_SCL_PIN      IOMUXC_GPIO_LPSR_05_LPI2C5_SCL
#define I2C_S1_SDA_PIN      IOMUXC_GPIO_LPSR_04_LPI2C5_SDA
#define I2C_S1_DRIVER       Driver_I2C5
#define I2C_S1_DEVICE_INDEX I2C5_INDEX
#define I2C_S1_SIGNAL_EVENT I2C5_SignalEvent_t

// I2C_S2: Pin mapping and driver information for alternate I2C bus on shield
#define I2C_S2_SCL_PIN      IOMUXC_GPIO_AD_08_LPI2C1_SCL
#define I2C_S2_SDA_PIN      IOMUXC_GPIO_AD_09_LPI2C1_SDA
#define I2C_S2_DRIVER       Driver_I2C1
#define I2C_S2_DEVICE_INDEX I2C1_INDEX
#define I2C_S2_SIGNAL_EVENT I2C1_SignalEvent_t

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

/* Compatability definitions for evkmimxrt11170 */
#define I2C1          LPI2C1
#define I2C_Type      LPI2C_Type
#define I2C_BASE_PTRS LPI2C_BASE_PTRS
#define SPI1          LPSPI1
#define SPI_Type      LPSPI_Type
#define SPI_BASE_PTRS LPSPI_BASE_PTRS

/* @brief dummy arguement to Power Mode Wait Wrapper. */
#define SMC NULL

/* @brief Kinetis style Wrapper API for Power Mode Wait (Wait for Interrupt). */
status_t SMC_SetPowerModeWait(void *);
/* @brief Kinetis style Wrapper API for Power Mode VLPR (Wait for Interrupt). */
status_t SMC_SetPowerModeVlpr(void *);

#endif /* EVKBIMX1170_H_ */
