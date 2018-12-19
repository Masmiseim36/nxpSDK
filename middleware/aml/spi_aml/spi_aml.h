/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file spi_aml.h
 *
 * SPI, DSPI and LPSPI driver intended for S32 SDK and MCUXpresso SDK.
 * This driver creates abstraction layer for SPI, DSPI and LPSPI peripherals.
 */

#ifndef SOURCE__SPI_AML_H_
#define SOURCE__SPI_AML_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../common_aml.h"
#include "../gpio_aml.h"

#if (SDK_VERSION == SDK_KSDK)
#include "fsl_device_registers.h"
#if FSL_FEATURE_SOC_SPI_COUNT
#include "fsl_spi.h"
#elif FSL_FEATURE_SOC_DSPI_COUNT
#include "fsl_dspi.h"
#endif
#elif (SDK_VERSION == SDK_S32)
#include "device_registers.h"
#include "lpspi_master_driver.h"
#include "lpspi_slave_driver.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @addtogroup macro_group
 * @{
 */
#define SPI_AML_TIMEOUT 1000U /*!< Timeout for blocking transfer. */
/*! @} */

#if (SDK_VERSION == SDK_KSDK)
#if FSL_FEATURE_SOC_SPI_COUNT
#define SPI_AML_DEV_CNT FSL_FEATURE_SOC_SPI_COUNT
typedef spi_master_config_t spi_sdk_master_config_t;
typedef spi_slave_config_t spi_sdk_slave_config_t;
/* typedef spi_slave_handle_t spi_slave_handle_t; */
/* typedef spi_slave_callback_t spi_slave_callback_t; */
#elif FSL_FEATURE_SOC_DSPI_COUNT
#define SPI_AML_DEV_CNT FSL_FEATURE_SOC_DSPI_COUNT
typedef dspi_master_config_t spi_sdk_master_config_t;
typedef dspi_slave_config_t spi_sdk_slave_config_t;
typedef dspi_slave_handle_t spi_slave_handle_t;
typedef dspi_slave_transfer_callback_t spi_slave_callback_t;
#endif
#elif (SDK_VERSION == SDK_S32)
#define SPI_AML_DEV_CNT LPSPI_INSTANCE_COUNT
typedef lpspi_master_config_t spi_sdk_master_config_t;
typedef lpspi_slave_config_t spi_sdk_slave_config_t;
typedef uint32_t spi_slave_handle_t;
typedef uint32_t spi_slave_callback_t;
#endif

/* Enum types definition. */
/*!
 * @addtogroup enum_group
 * @{
 */
/*! @brief Error codes. */
enum _spi_aml_status
{
    kStatus_AML_SPI_Busy = MAKE_STATUS(kStatusGroup_AML_SPI, 0),  /*!< SPI transfer is busy.*/
    kStatus_AML_SPI_Error = MAKE_STATUS(kStatusGroup_AML_SPI, 1)  /*!< Other SPI driver error. */
};

/*! @brief Number of bits per frame (8 or 16 bits). */
typedef enum
{
    spiBitcount8bit            = 0x0U, /*!< 8 bits per frame. */
    spiBitcount16bit           = 0x1U  /*!< 16 bits per frame. */
} spi_aml_bitcount_t;

/*! @brief Clock phase. */
typedef enum
{
    spiClockPhaseFirstEdge     = 0x0U, /*!< Data is captured on the leading edge of the SCK
                                            and changed on the following edge. */
    spiClockPhaseSecondEdge    = 0x1U  /*!< Data is changed on the leading edge of the SCK
                                            and captured on the following edge. */
} spi_aml_clock_phase_t;

/*! @brief Clock polarity. */
typedef enum
{
    spiClockPolarityActiveHigh = 0x0U, /*!< Active-high SPI clock (idles low). */
    spiClockPolarityActiveLow  = 0x1U  /*!< Active-low SPI clock (idles high). */
} spi_aml_clock_polarity_t;

/*! @brief Active level of chip select pin (PCS). */
typedef enum
{
    spiPcsActiveHigh           = 0x0U, /*!< Pcs Active High (idles low). */
    spiPcsActiveLow            = 0x1U  /*!< Pcs Active Low (idles high). */
} spi_aml_pcs_polarity_t;
/*! @} */

/* Configure struct types definition. */
/*!
 * @addtogroup struct_group
 * @{
 */
/*!
 * @brief AML SPI master parameters available for superior drivers (pointers).
 *
 * These parameters are used in superior drivers (user selectable or needed by driver).
 * Values from this structure are written to SDK specific configuration structures
 * (original values are overwritten by these values) only if the pointer is not NULL.
 */
typedef struct
{
    uint32_t baudRateHz;                  /*!< Baudrate in Hz. */
    uint16_t bitCount;                    /*!< Number of bits/frame, minimum is 8-bits. */
    spi_aml_clock_phase_t clkPhase;       /*!< Clock phase. */
    spi_aml_clock_polarity_t clkPolarity; /*!< Clock polarity. */
    bool lsbFirst;                        /*!< true = LSB first, false = MSB first. */
    spi_aml_pcs_polarity_t pcsPolarity;   /*!< Active level of chip select pin (PCS). */
    uint32_t sourceClockHz;               /*!< Peripheral source clock frequency in Hz. */
} spi_aml_master_config_t;

/*!
 * @brief AML SPI slave parameters available for superior drivers (pointers).
 *
 * These parameters are used in superior drivers (user selectable or needed by driver).
 * Values from this structure are written to SDK specific configuration structures
 * (original values are overwritten by these values) only if the pointer is not NULL.
 */
typedef struct
{
    uint16_t bitCount;                    /*!< Number of bits/frame, minimum is 8-bits. */
    spi_aml_clock_phase_t clkPhase;       /*!< Clock phase. */
    spi_aml_clock_polarity_t clkPolarity; /*!< Clock polarity. */
    bool lsbFirst;                        /*!< true = LSB first, false = MSB first. */
    spi_aml_pcs_polarity_t pcsPolarity;   /*!< Active level of chip select pin (PCS). */
} spi_aml_slave_config_t;

/*!
 * @brief SPI transfer structure.
 */
typedef struct
{
    uint8_t *txBuffer;                    /*!< Send buffer. */
    uint8_t *rxBuffer;                    /*!< Receive buffer. */
    size_t dataSize;                      /*!< Number of bytes to transfer. */
    uint32_t configFlags;                 /*!< SPI control flags, only for DSPI. */
} spi_aml_transfer_t;
/*! @} */

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @addtogroup function_group
 * @{
 */ 
/*!
 * @brief Initializes the SPI as master.
 * 
 * @param instance - Peripheral instance number.
 * @param spiSdkMasterConfig - Pointer to SDK specific SPI master configuration.
 * @param sourceClockHz - SPI source clock in Hz (not used in S32 SDK).
 */
void SPI_AML_MasterInit(aml_instance_t instance,
        const spi_sdk_master_config_t *spiSdkMasterConfig,
        uint32_t sourceClockHz);

/*!
 * @brief Initializes the SPI as slave.
 * 
 * @param instance - Peripheral instance number.
 * @param spiSdkSlaveConfig - Pointer to SDK specific SPI slave configuration.
 */
void SPI_AML_SlaveInit(aml_instance_t instance,
        const spi_sdk_slave_config_t *spiSdkSlaveConfig);

/*!
 * @brief Deinitializes the SPI master.
 *
 * @param instance - Peripheral instance number.
 */
void SPI_AML_MasterDeinit(aml_instance_t instance);

/*!
 * @brief Deinitializes the SPI slave.
 * 
 * @param instance - Peripheral instance number.
 */
void SPI_AML_SlaveDeinit(aml_instance_t instance);

/*!
 * @brief Gets and stores default SPI master configuration.
 *
 * @param spiSdkMasterConfig - Pointer where the default SPI master configuration is stored.
 */
void SPI_AML_MasterGetDefaultConfig(spi_sdk_master_config_t *spiSdkMasterConfig);

/*!
 * @brief Gets and stores default SPI slave configuration.
 *
 * @param spiSdkSlaveConfig - Pointer where the default SPI slave configuration is stored.
 */
void SPI_AML_SlaveGetDefaultConfig(spi_sdk_slave_config_t *spiSdkSlaveConfig);

/*!
 * @brief Fills SDK SPI master configuration with AML data.
 *
 * @param spiAmlMasterConfig - Pointer to AML data.
 * @param spiSdkMasterConfig - Pointer where the resulting SDK SPI master configuration is stored.
 */
void SPI_AML_MasterFillSdkConfig(const spi_aml_master_config_t *spiAmlMasterConfig,
        spi_sdk_master_config_t *spiSdkMasterConfig);

/*!
 * @brief Fills SDK SPI slave configuration with AML data.
 *
 * @param spiAmlSlaveConfig - Pointer to AML data.
 * @param spiSdkSlaveConfig - Pointer where the resulting SDK SPI slave configuration is stored.
 */
void SPI_AML_SlaveFillSdkConfig(const spi_aml_slave_config_t *spiAmlSlaveConfig,
        spi_sdk_slave_config_t *spiSdkSlaveConfig);

/*!
 * @brief Selects the device with software controlled chip-select pin (PCS).
 *
 * Not applicable for hardware controlled chip-select.
 *
 * @param portInstance - Instance number of GPIO port, e.g. PTA1 - portInstance = instanceA.
 * @param pinIndex - Index of GPIO pin used as chip-select (PCS), e.g. PTA1 - pinIndex = 1.
 * @param pcsPolarity - Active level of chip select pin (PCS).
 */
void SPI_AML_MasterSelectDevice(aml_instance_t portInstance,
        uint8_t pinIndex,
        spi_aml_pcs_polarity_t pcsPolarity);

/*!
 * @brief Unselects the device with software controlled chip-select pin (PCS).
 *
 * Not applicable for hardware controlled chip-select.
 *
 * @param portInstance - Instance number of GPIO port, e.g. PTA1 - portInstance = instanceA.
 * @param pinIndex - Index of GPIO pin used as chip-select (PCS), e.g. PTA1 - pinIndex = 1.
 * @param pcsPolarity - Active level of chip select pin (PCS).
 */
void SPI_AML_MasterUnselectDevice(aml_instance_t portInstance,
        uint8_t pinIndex,
        spi_aml_pcs_polarity_t pcsPolarity);

/*!
 * @brief Performs blocking master transfer of data. The method returns
 *        when all data are sent and received.
 * 
 * @param instance - Peripheral instance number.
 * @param masterTransfer - Pointer to transfer structure containing data and configuration.
 *
 * @return  kStatus_Success if driver starts to send/receive data successfully.
 *          kStatus_InvalidArgument if wrong parameter is passed.
 *          kStatus_AML_SPI_Busy if driver is receiving/transmitting data and not available.
 *          kStatus_AML_SPI_Error if driver is error and needs to clean error.
 */
status_t SPI_AML_MasterTransfer(aml_instance_t instance,
        spi_aml_transfer_t *masterTransfer);

/*!
 * @brief Performs slave transfer of data.
 *
 * This method uses non-blocking transfer.
 * In S32 SDK the slaveHandle and slaveCallback parameters should be NULL and 0.
 *
 * @param instance - Peripheral instance number.
 * @param slaveTransfer - Pointer to transfer structure containing data and configuration.
 * @param slaveHandle - Pointer to slave handle structure.
 * @param slaveCallback - Completion callback function.
 *
 * @return  kStatus_Success if driver starts to send/receive data successfully.
 *          kStatus_InvalidArgument if wrong parameter is passed.
 *          kStatus_AML_SPI_Busy if driver is receiving/transmitting data and not available.
 *          kStatus_AML_SPI_Error if driver is error and needs to clean error.
 */
status_t SPI_AML_SlaveTransfer(aml_instance_t instance,
        spi_aml_transfer_t *slaveTransfer,
        spi_slave_handle_t *slaveHandle,
        spi_slave_callback_t slaveCallback);
/*! @} */

#endif /* SOURCE_SPI_AML_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
