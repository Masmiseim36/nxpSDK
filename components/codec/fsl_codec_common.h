/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_CODEC_COMMON_H_
#define _FSL_CODEC_COMMON_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @name Driver version */
/*@{*/
/*! @brief CLOCK driver version 2.0.1. */
#define FSL_CODEC_DRIVER_VERSION (MAKE_VERSION(2, 0, 1))
/*@}*/

/*! @brief Define I2C access function. */
typedef status_t (*codec_i2c_send_func_t)(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
typedef status_t (*codec_i2c_receive_func_t)(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
/*! @brief codec common delay function pointer */
typedef void (*codec_delay)(uint32_t ms);

/*! @brief CODEC device register address type. */
typedef enum _codec_reg_addr
{
    kCODEC_RegAddr8Bit  = 1U, /*!< 8-bit register address.  */
    kCODEC_RegAddr16Bit = 2U, /*!< 16-bit register address. */
} codec_reg_addr_t;

/*! @brief CODEC device register width. */
typedef enum _codec_reg_width
{
    kCODEC_RegWidth8Bit  = 1U, /*!< 8-bit register width.  */
    kCODEC_RegWidth16Bit = 2U, /*!< 16-bit register width. */
    kCODEC_RegWidth32Bit = 4U, /*!< 32-bit register width. */
} codec_reg_width_t;

/*! @brief AUDIO format definition. */
typedef enum _codec_protocol
{
    kCODEC_BusI2S            = 0x0, /*!< I2S type */
    kCODEC_BusLeftJustified  = 0x1, /*!< Left justified mode */
    kCODEC_BusRightJustified = 0x2, /*!< Right justified mode */
    kCODEC_BusPCMA           = 0x3, /*!< PCM A mode */
    kCODEC_BusPCMB           = 0x4  /*!< PCM B mode */
} codec_protocol_t;

typedef struct _audio_format
{
    uint8_t format;
    uint8_t bitwidth;
} audio_format_t;

typedef enum
{
    kAUDIO_Stereo16Bits = 0,
    kAUDIO_Stereo24Bits = 1,
    kAUDIO_Stereo32Bits = 2,
    kAUDIO_DSD8bits     = 48,
    kAUDIO_DSD16bits    = 49,
    kAUDIO_DSD32bits    = 50,
} audio_format_type_t;

/*!@brief Define the format numbers used in the SDK. */
#define FORMAT_NUM_USED (6)

extern audio_format_t saiFormatMap[FORMAT_NUM_USED];

/*! @brief codec type for some special processing. */
typedef enum _codec_type
{
    kCODEC_TypeOthers = 0x0,
    kCODEC_TypeWM8960 = 0x1
} codec_type_t;

typedef struct codec_handle codec_handle_t;

/*! @brief Codec common operation */
typedef struct codec_operation
{
    status_t (*Init)(codec_handle_t *handle, void *config); /* Codec init operation */
    status_t (*SetFormat)(codec_handle_t *handle,
                          uint32_t mclk,
                          uint32_t sampleRate,
                          uint32_t bitWidth); /*!< Codec format setting */
    status_t (*Deinit)(codec_handle_t *handle);
    status_t (*SetEncoding)(codec_handle_t *handle, uint8_t format);
} codec_operation_t;

/*! @brief Initialize structure of the codec */
typedef struct codec_config
{
    codec_i2c_send_func_t I2C_SendFunc;       /*!< Pointer to the user-defined I2C Send Data function. */
    codec_i2c_receive_func_t I2C_ReceiveFunc; /*!< Pointer to the user-defined I2C Receive Data function. */
    codec_delay delayMs;                      /*!< ms delay function pointer */

    void *codecConfig;    /*!< Codec specific configuration */
    codec_operation_t op; /*!< codec common operation function pointer */
} codec_config_t;

/*! @brief Codec configure definition. */
struct codec_handle
{
    codec_i2c_send_func_t I2C_SendFunc;       /*!< Pointer to the user-defined I2C Send Data function. */
    codec_i2c_receive_func_t I2C_ReceiveFunc; /*!< Pointer to the user-defined I2C Receive Data function. */
    codec_delay delayMs;                      /*!< ms delay function pointer */

    uint8_t slaveAddress; /*!< slave address */
    void *codecPriv;      /*!< codec private pointer */
    codec_operation_t op; /*!< codec common operation function pointer */
};

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif
/*!
 * @brief Get the audio bit width.
 *
 * @param format which provides the format info.
 * @return Returns the audio bit width.
 */
uint32_t CODEC_GetMappedFormatBits(audio_format_type_t format);
/*!
 * @brief Write value to the register.
 *
 * @param i2cAddr I2C address.
 * @param addrType Register address type.
 * @param reg The register to write.
 * @param regWidth The width of the register.
 * @param value The value to write.
 * @param i2cSendFunc The actual I2C send function.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t CODEC_I2C_WriteReg(uint8_t i2cAddr,
                            codec_reg_addr_t addrType,
                            uint32_t reg,
                            codec_reg_width_t regWidth,
                            uint32_t value,
                            codec_i2c_send_func_t i2cSendFunc);

/*!
 * @brief Read the register value.
 *
 * @param i2cAddr I2C address.
 * @param addrType Register address type.
 * @param reg The register to read.
 * @param regWidth The width of the register.
 * @param value The value read out.
 * @param i2cReceiveFunc The actual I2C receive function.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t CODEC_I2C_ReadReg(uint8_t i2cAddr,
                           codec_reg_addr_t addrType,
                           uint32_t reg,
                           codec_reg_width_t regWidth,
                           void *value,
                           codec_i2c_receive_func_t i2cReceiveFunc);

/*!
 * @brief Modify the register value.
 *
 * This function modifies some bit fields of a register.
 * reg[clrMask] = value & clrMask
 *
 * @param i2cAddr I2C address.
 * @param addrType Register address type.
 * @param reg The register to modify.
 * @param regWidth The width of the register.
 * @param clrMask The mask value to clear.
 * @param value The value to set.
 * @param i2cReceiveFunc The actual I2C receive function.
 * @param i2cSendFunc The actual I2C send function.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t CODEC_I2C_ModifyReg(uint8_t i2cAddr,
                             codec_reg_addr_t addrType,
                             uint32_t reg,
                             codec_reg_width_t regWidth,
                             uint32_t clrMask,
                             uint32_t value,
                             codec_i2c_receive_func_t i2cReceiveFunc,
                             codec_i2c_send_func_t i2cSendFunc);

status_t CODEC_Init(codec_handle_t *handle, codec_config_t *config);

static inline status_t CODEC_SetEncoding(codec_handle_t *handle, uint8_t format)
{
    return handle->op.SetEncoding(handle, format);
}

static inline status_t CODEC_SetFormat(codec_handle_t *handle, uint32_t mclk, uint32_t sampleRate, uint32_t bitWidth)
{
    return handle->op.SetFormat(handle, mclk, sampleRate, bitWidth);
}

static inline status_t CODEC_Deinit(codec_handle_t *handle)
{
    return handle->op.Deinit(handle);
}

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_CODEC_COMMON_H_ */
