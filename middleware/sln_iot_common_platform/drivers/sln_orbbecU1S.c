/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sln_orbbecU1S.h"
#include "fsl_video_common.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"
#if ORBBECU1S_RESET_IN_BOOTLOADER
#include "fsl_pit.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OBU1S_DelayMs(ms) VIDEO_DelayMs(ms)
#define OBU1S_Write(handle, reg, size, value)                                                    \
    VIDEO_I2C_WriteReg(OBU1S_I2C_ADDR, kVIDEO_RegAddr16Bit, reg, (video_reg_width_t)size, value, \
                       ((obU1S_resource_t *)(handle->resource))->i2cSendFunc)
#define OBU1S_Read(handle, reg, size, value)                                                    \
    VIDEO_I2C_ReadReg(OBU1S_I2C_ADDR, kVIDEO_RegAddr16Bit, reg, (video_reg_width_t)size, value, \
                      ((obU1S_resource_t *)(handle->resource))->i2cReceiveFunc)

typedef struct _obU1S_reg
{
    uint16_t reg;
    uint8_t size;
    uint32_t value;
} obU1S_reg_t;

typedef enum _obU1S_stream_type
{
    obU1S_stream_off,
    obU1S_stream_depth_only,
    obU1S_stream_ir_only,
    obU1S_stream_ir_depth_interlace,
} obU1S_stream_type_t;

#define OBU1S_I2C_ADDR      0x10
#define OBU1S_CHIP_ID       0x6010
#define OBU1S_REG_CHIP_ID   0x0000
#define OBU1S_REG_STREAM    0x0018
#define OBU1S_REG_DEPTH_RES 0x0033
#define OBU1S_REG_IR_RES    0x005b

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
status_t OBU1S_Init(camera_device_handle_t *handle, const camera_config_t *config);
status_t OBU1S_Deinit(camera_device_handle_t *handle);
status_t OBU1S_Start(camera_device_handle_t *handle);
status_t OBU1S_Stop(camera_device_handle_t *handle);
status_t OBU1S_Control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg);
status_t OBU1S_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig);

/*******************************************************************************
 * Variables
 ******************************************************************************/
const camera_device_operations_t obU1S_ops = {
    .init     = OBU1S_Init,
    .deinit   = OBU1S_Deinit,
    .start    = OBU1S_Start,
    .stop     = OBU1S_Stop,
    .control  = OBU1S_Control,
    .init_ext = OBU1S_InitExt,
};

static obU1S_stream_type_t s_SteamType = obU1S_stream_ir_depth_interlace;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Reset OBU1S camera.
 *
 * Reset camera.
 *
 * @param handle Handle to the camera pointer.
 */
static status_t OBU1S_Reset(camera_device_handle_t *handle)
{
#if ORBBECU1S_RESET_IN_BOOTLOADER
    uint32_t pit_count = PIT_GetCurrentTimerCount(PIT1, kPIT_Chnl_1);
    if ((0xFFFFFFFF - pit_count) < (260 * 1000))
    {
        uint32_t remain_delay_ms = (260 * 1000 - (0xFFFFFFFF - pit_count)) / 1000;
        OBU1S_DelayMs(remain_delay_ms);
    }

    PIT_StopTimer(PIT1, kPIT_Chnl_0);
    PIT_StopTimer(PIT1, kPIT_Chnl_1);
#else
    if (((obU1S_resource_t *)(handle->resource))->pullTestModePin)
        ((obU1S_resource_t *)(handle->resource))->pullTestModePin(false);
    if (((obU1S_resource_t *)(handle->resource))->pullLdpIntPin)
        ((obU1S_resource_t *)(handle->resource))->pullLdpIntPin(false);
    /* Pwr Enable */
    if (((obU1S_resource_t *)(handle->resource))->pullPowerEnablePin)
        ((obU1S_resource_t *)(handle->resource))->pullPowerEnablePin(true);
    OBU1S_DelayMs(1);
    if (((obU1S_resource_t *)(handle->resource))->pullLdpIntPin)
        ((obU1S_resource_t *)(handle->resource))->pullLdpIntPin(true);
    /* Hard reset */
    if (((obU1S_resource_t *)(handle->resource))->pullResetPin)
    {
        ((obU1S_resource_t *)(handle->resource))->pullResetPin(false);
        OBU1S_DelayMs(1);
        ((obU1S_resource_t *)(handle->resource))->pullResetPin(true);
        OBU1S_DelayMs(5);
        ((obU1S_resource_t *)(handle->resource))->pullResetPin(false);
        OBU1S_DelayMs(260);
    }
#endif
    return kStatus_Success;
}

/*!
 * @brief Control OBU1S stream type.
 *
 * @param handle Handle to the camera pointer.
 * @param obU1S_stream_type configuration.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static status_t OBU1S_StreamControl(camera_device_handle_t *handle, obU1S_stream_type_t type)
{
    status_t status;
    uint8_t value;
    switch (type)
    {
        case obU1S_stream_off:
            value = 0x00;
            break;
        case obU1S_stream_depth_only:
            value = 0x01;
            break;
        case obU1S_stream_ir_only:
            value = 0x05;
            break;
        case obU1S_stream_ir_depth_interlace:
        default:
            value = 0x06;
            break;
    }

    status = OBU1S_Write(handle, OBU1S_REG_STREAM, kVIDEO_RegWidth8Bit, value);
    return status;
}

/*!
 * @brief Set Resolution of OBU1S camera.
 *
 * @param handle Handle to the camera pointer.
 * @param resolution
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static status_t OBU1S_SetResolution(camera_device_handle_t *handle, uint32_t resolution)
{
    switch (resolution)
    {
        case FSL_VIDEO_RESOLUTION(1080, 1280):
            OBU1S_Write(handle, OBU1S_REG_IR_RES, kVIDEO_RegWidth8Bit, 0x16);
            OBU1S_Write(handle, 0x005d, kVIDEO_RegWidth8Bit, 0x1e); // 30fps
            OBU1S_Write(handle, 0x005c, kVIDEO_RegWidth8Bit, 0x4);  // 12bit
            OBU1S_Write(handle, 0x0055, kVIDEO_RegWidth8Bit, 0x1);  // take effect
            OBU1S_Write(handle, OBU1S_REG_DEPTH_RES, kVIDEO_RegWidth8Bit, 0x16);
            OBU1S_Write(handle, 0x0035, kVIDEO_RegWidth8Bit, 0x1e); // 30fps
            OBU1S_Write(handle, 0x0034, kVIDEO_RegWidth8Bit, 0x4);  // 12bit
            OBU1S_Write(handle, 0x002d, kVIDEO_RegWidth8Bit, 0x1);  // take effect
            break;

        case FSL_VIDEO_RESOLUTION(540, 640):
            OBU1S_Write(handle, OBU1S_REG_IR_RES, kVIDEO_RegWidth8Bit, 0x17);
            OBU1S_Write(handle, 0x005d, kVIDEO_RegWidth8Bit, 0x1e); // 30fps
            OBU1S_Write(handle, 0x005c, kVIDEO_RegWidth8Bit, 0x4);  // 12bit
            OBU1S_Write(handle, 0x0055, kVIDEO_RegWidth8Bit, 0x1);  // take effect
            OBU1S_Write(handle, OBU1S_REG_DEPTH_RES, kVIDEO_RegWidth8Bit, 0x17);
            OBU1S_Write(handle, 0x0035, kVIDEO_RegWidth8Bit, 0x1e); // 30fps
            OBU1S_Write(handle, 0x0034, kVIDEO_RegWidth8Bit, 0x4);  // 12bit
            OBU1S_Write(handle, 0x002d, kVIDEO_RegWidth8Bit, 0x1);  // take effect
            break;
        default:
            return kStatus_Fail;
    }
    return kStatus_Success;
}

/*!
 * @brief Initialize OBU1S sensor.
 *
 * Reset the sensor, disable the power-down mode and write the sensor settings via I2C.
 *
 * @param handle Handle to the camera pointer.
 * @param config pointer to camera_config_t configuration.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t OBU1S_Init(camera_device_handle_t *handle, const camera_config_t *config)
{
    status_t status;
    uint16_t chip_id;
    uint8_t value;

    OBU1S_Reset(handle);
    status = OBU1S_Read(handle, OBU1S_REG_CHIP_ID, kVIDEO_RegWidth16Bit, &chip_id);
    if (kStatus_Success != status)
    {
        return status;
    }
    if (OBU1S_CHIP_ID != chip_id)
    {
        return kStatus_Fail;
    }
    return kStatus_Success;
}

/*!
 * @brief Deinitialize OBU1S sensor.
 *
 * Reset the sensor and enable the power-down mode
 *
 * @param handle Handle to the camera pointer.
 * @return Allways returns @ref kStatus_Success.
 */
status_t OBU1S_Deinit(camera_device_handle_t *handle)
{
    if (((obU1S_resource_t *)(handle->resource))->pullPowerEnablePin)
        ((obU1S_resource_t *)(handle->resource))->pullPowerEnablePin(false);

    if (((obU1S_resource_t *)(handle->resource))->pullResetPin)
        ((obU1S_resource_t *)(handle->resource))->pullResetPin(false);

    if (((obU1S_resource_t *)(handle->resource))->pullLdpIntPin)
        ((obU1S_resource_t *)(handle->resource))->pullLdpIntPin(false);

    if (((obU1S_resource_t *)(handle->resource))->pullTestModePin)
        ((obU1S_resource_t *)(handle->resource))->pullTestModePin(false);
    return kStatus_Success;
}

/*!
 * @brief Start OBU1S sensor.
 *
 * Actually does nothing !
 *
 * @param handle Handle to the camera pointer.
 * @return Allways returns @ref kStatus_Success.
 */
status_t OBU1S_Start(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

/*!
 * @brief Stop OBU1S sensor.
 *
 * Actually does nothing !
 *
 * @param handle Handle to the camera pointer.
 * @return Allways returns @ref kStatus_Success.
 */
status_t OBU1S_Stop(camera_device_handle_t *handle)
{
    status_t status;
    status = OBU1S_StreamControl(handle, obU1S_stream_off);
    return status;
}

/*!
 * @brief Control OBU1S sensor.
 *
 * Actually does nothing !
 *
 * @param handle Handle to the camera pointer.
 * @return Allways returns @ref kStatus_Success.
 */
status_t OBU1S_Control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg)
{
    status_t status = kStatus_Success;

    if (cmd == k3DCAMERA_StreamType)
    {
        status = OBU1S_StreamControl(handle, (obU1S_stream_type_t)arg);
    }

    return status;
}

/*!
 * @brief Initialize OBU1S sensor.
 *
 * Launch OBU1S_Init function.
 *
 * @param handle Handle to the camera pointer.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t OBU1S_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig)
{
    return OBU1S_Init(handle, config);
}
