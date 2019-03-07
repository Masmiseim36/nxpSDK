/*
 * Copyright (c) 2017-2018, NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>

#include "srtm_i2c_codec_adapter.h"
#include "srtm_heap.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Codec adapter */
typedef struct _srtm_i2c_codec_adapter
{
    struct _srtm_codec_adapter adapter;
    srtm_i2c_codec_config_t config;
    codec_handle_t *driver;
    uint32_t srate;
    uint8_t format;
} * srtm_i2c_codec_adapter_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Currently only 1 audio instance is adequate, so index is just ignored */
static srtm_status_t SRTM_I2CCodecAdapter_SetParam(srtm_codec_adapter_t adapter,
                                                   uint8_t index,
                                                   uint8_t format,
                                                   uint32_t srate)
{
    srtm_i2c_codec_adapter_t handle = (srtm_i2c_codec_adapter_t)adapter;
    SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_INFO, "%s: %d. fmt %d, srate %d\r\n", __func__, index, format, srate);

    if (format > kAUDIO_DSD32bits)
    {
        SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_ERROR, "%s: unsupported format %d!\r\n", __func__, format);
        return SRTM_Status_InvalidParameter;
    }

    if (handle->srate != srate || handle->format != format)
    {
        handle->format = format;
        if (handle->driver->op.SetEncoding != NULL)
        {
            CODEC_SetEncoding(handle->driver, format);
        }
        if (format > kAUDIO_Stereo32Bits)
        {
            format = format - 45U; /* Skip due to the format value from 3 to 47 not defined in SDK. */
        }
        /* Only set codec when configuration changes. */
        CODEC_SetFormat(handle->driver, handle->config.mclk, srate,
                        CODEC_GetMappedFormatBits((audio_format_type_t)format));
        handle->srate = srate;
    }

    return SRTM_Status_Success;
}

static srtm_status_t SRTM_I2CCodecAdapter_SetReg(srtm_codec_adapter_t adapter, uint32_t reg, uint32_t regVal)
{
    srtm_i2c_codec_adapter_t handle = (srtm_i2c_codec_adapter_t)adapter;
    status_t status;

    SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_INFO, "%s: %d, %d\r\n", __func__, reg, regVal);

    if (handle->config.writeRegMap)
    {
        status = handle->config.writeRegMap(handle->driver, reg, regVal);
    }
    else
    {
        status = CODEC_I2C_WriteReg(handle->driver->slaveAddress, handle->config.addrType, reg, handle->config.regWidth,
                                    regVal, handle->driver->I2C_SendFunc);
    }
    return status == kStatus_Success ? SRTM_Status_Success : SRTM_Status_Error;
}

static srtm_status_t SRTM_I2CCodecAdapter_GetReg(srtm_codec_adapter_t adapter, uint32_t reg, uint32_t *pRegVal)
{
    srtm_i2c_codec_adapter_t handle = (srtm_i2c_codec_adapter_t)adapter;
    status_t status;

    assert(pRegVal);

    SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_INFO, "%s: %d\r\n", __func__, reg);

    *pRegVal = 0; /* Clear high bytes. */
    if (handle->config.readRegMap)
    {
        status = handle->config.readRegMap(handle->driver, reg, pRegVal);
    }
    else
    {
        status = CODEC_I2C_ReadReg(handle->driver->slaveAddress, handle->config.addrType, reg, handle->config.regWidth,
                                   (void *)pRegVal, handle->driver->I2C_ReceiveFunc);
    }
    return status == kStatus_Success ? SRTM_Status_Success : SRTM_Status_Error;
}

srtm_codec_adapter_t SRTM_I2CCodecAdapter_Create(codec_handle_t *driver, srtm_i2c_codec_config_t *config)
{
    srtm_i2c_codec_adapter_t handle;

    assert(driver && config);

    SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_INFO, "%s\r\n", __func__);

    handle = (srtm_i2c_codec_adapter_t)SRTM_Heap_Malloc(sizeof(struct _srtm_i2c_codec_adapter));
    assert(handle);

    handle->driver = driver;
    memcpy(&handle->config, config, sizeof(struct _srtm_i2c_codec_config));
    handle->srate = 0;
    handle->format = 0;

    /* Adapter interfaces. */
    handle->adapter.setParam = SRTM_I2CCodecAdapter_SetParam;
    handle->adapter.setReg = SRTM_I2CCodecAdapter_SetReg;
    handle->adapter.getReg = SRTM_I2CCodecAdapter_GetReg;

    return &handle->adapter;
}

void SRTM_I2CCodecAdapter_Destroy(srtm_codec_adapter_t adapter)
{
    srtm_i2c_codec_adapter_t handle = (srtm_i2c_codec_adapter_t)adapter;

    assert(adapter);

    SRTM_DEBUG_MESSAGE(SRTM_DEBUG_VERBOSE_INFO, "%s\r\n", __func__);

    SRTM_Heap_Free(handle);
}
