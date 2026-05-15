/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLEXIO_SRAM_EMULATOR_H_
#define _FLEXIO_SRAM_EMULATOR_H_

#include "fsl_common.h"
#include "fsl_flexio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef FLEXIO_SRAM_DATA_BUFFER_LEN
#define FLEXIO_SRAM_DATA_BUFFER_LEN 256U
#endif

#ifndef FLEXIO_SRAM_DATA_BUS_WIDTH
/* The data bus width, must be 8 or 16. */
#define FLEXIO_SRAM_DATA_BUS_WIDTH 8U
#endif

/*! @brief Define FlexIO SRAM access structure typedef. */
typedef struct _flexio_sram_type
{
    FLEXIO_Type *flexio;     /*!< FlexIO base pointer. */
    uint8_t daPinIndex;      /*!< Pin select for Data & Address. */
    uint8_t wePinIndex;      /*!< Pin select for Write Enable. */
    uint8_t oePinIndex;      /*!< Pin select for Output Enable. */
    uint8_t advPinIndex;     /*!< Pin select for Address Valid. */
    uint8_t cePinIndex;      /*!< Pin select for Chip Enable. */
    uint8_t weShifterIndex;  /*!< Shifter index for Write Enable. */
    uint8_t advShifterIndex; /*!< Shifter index for Address Valid. */
    uint8_t weTimerIndex;    /*!< Timer index for Write Enable. */
    uint8_t advTimerIndex;   /*!< Timer index for Address Valid. */
    uint8_t oeLevel;         /*!< OE level */
    uint32_t *data;          /*!< Data array buffer */
} FLEXIO_SRAM_Type;

/*! @brief Define FlexIO SRAM configuration structure. */
typedef struct _flexio_sram_config
{
    bool enableFlexio;           /*!< Enable/disable FlexIO SRAM after configuration. */
    bool enableInDoze;     /*!< Enable/disable FlexIO operation in doze mode. */
    bool enableInDebug;    /*!< Enable/disable FlexIO operation in debug mode. */
    bool enableFastAccess; /*!< Enable/disable fast access to FlexIO registers,
                           fast access requires the FlexIO clock to be at least
                           twice the frequency of the bus clock. */
} flexio_sram_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

/**
* @brief Initializes the FLEXIO with the provided configuration.
*
* This function configures the FLEXIO hardware to act as an SRAM emulator.
* It enables the clock and configures shifters and timers.
*
* @param base Pointer to the FlexIO SRAM access structure.
* @param config Pointer to the FlexIO configuration structure.
*/
void FLEXIO_SRAM_Init(FLEXIO_SRAM_Type *base, flexio_sram_config_t *config);

/**
 * @brief Gets the default configuration for the FLEXIO module.
 *
 * This function initializes the provided configuration structure with default values.
 * These values can be used as a starting point for configuring the FLEXIO module
 * to act as an SRAM emulator.
 *
 * @param config Pointer to the configuration structure to be initialized.
 */
void FLEXIO_SRAM_GetDefaultConfig(flexio_sram_config_t *config);

/**
 * @brief Starts the FLEXIO SRAM emulator by enabling specific interrupts.
 *
 * This function initializes and enables interrupts related to the WE shifter and OE pin
 * of the FLEXIO SRAM emulator. The WE shifter interrupt is used to handle write operations,
 * while the OE pin interrupts (both rising and falling edges) are used to handle read operations.
 *
 * @param base Pointer to the FLEXIO SRAM access structure.
 */
void FLEXIO_SRAM_Start(FLEXIO_SRAM_Type *base);

/**
 * @brief Interrupt service routine for the FLEXIO SRAM emulator.
 *
 * This function handles interrupts related to the OE (Output Enable) pin and the WE (Write Enable) shifter.
 * When an OE pin interrupt occurs, it manages data transfer based on the OE pin state.
 * When a WE shifter interrupt occurs, it stores the received data at the appropriate address.
 *
 * @param base Pointer to the FLEXIO SRAM access structure.
 */
void FLEXIO_SRAM_ISR(FLEXIO_SRAM_Type *base);

#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif /*_FLEXIO_SRAM_EMULATOR_H_*/