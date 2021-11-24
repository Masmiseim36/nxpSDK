/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sln_gc0308.h"
#include "fsl_video_common.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define GC0308_DelayMs(ms) VIDEO_DelayMs(ms)
#define GC0308_Write(handle, reg, size, value)                                                   \
    VIDEO_I2C_WriteReg(GC0308_I2C_ADDR, kVIDEO_RegAddr8Bit, reg, (video_reg_width_t)size, value, \
                       ((gc0308_resource_t *)(handle->resource))->i2cSendFunc)
#define GC0308_Read(handle, reg, size, value)                                                   \
    VIDEO_I2C_ReadReg(GC0308_I2C_ADDR, kVIDEO_RegAddr8Bit, reg, (video_reg_width_t)size, value, \
                      ((gc0308_resource_t *)(handle->resource))->i2cReceiveFunc)

typedef struct _gc0308_reg
{
    uint16_t reg;
    uint8_t size;
    uint32_t value;
} gc0308_reg_t;

typedef enum _gc0308_testmode
{
    ktest_image1, /*!< test image type 1. */
    ktest_image2, /*!< test image type 2. */
} gc0308_testmode_t;

typedef enum _gc0308_white_balance
{
    kwhite_balance_auto,        /*!< GC0308 white balance settings auto. */
    kwhite_balance_tungsten,    /*!< GC0308 white balance settings tungsten. */
    kwhite_balance_daylight,    /*!< GC0308 white balance settings daylight. */
    kwhite_balance_fluorescent, /*!< GC0308 white balance settings fluorescent. */
    kwhite_balance_cloud,       /*!< GC0308 white balance settings cloud. */
} gc0308_white_balance_t;

typedef enum _gc0308_cam_effect
{
    kcam_effect_enc_normal,     /*!< GC0308 camera effect normal. */
    kcam_effect_enc_sepia,      /*!< GC0308 camera effect sepia. */
    kcam_effect_enc_colorinv,   /*!< GC0308 camera effect color inversion. */
    kcam_effect_enc_sepiagreen, /*!< GC0308 camera effect sepia green. */
    kcam_effect_enc_grayscale,  /*!< GC0308 camera effect grayscale. */
} gc0308_cam_effect_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
status_t GC0308_Init(camera_device_handle_t *handle, const camera_config_t *config);
status_t GC0308_Deinit(camera_device_handle_t *handle);
status_t GC0308_Start(camera_device_handle_t *handle);
status_t GC0308_Stop(camera_device_handle_t *handle);
status_t GC0308_Control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg);
status_t GC0308_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig);

/*******************************************************************************
 * Variables
 ******************************************************************************/
const camera_device_operations_t gc0308_ops = {
    .init     = GC0308_Init,
    .deinit   = GC0308_Deinit,
    .start    = GC0308_Start,
    .stop     = GC0308_Stop,
    .control  = GC0308_Control,
    .init_ext = GC0308_InitExt,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Reset GC0308 camera.
 * This function pulls the camera reset line during 50ms.
 * @param handle Handle to the camera pointer.
 */
static status_t GC0308_Reset(camera_device_handle_t *handle)
{
    /* Hard reset */
    // The function that pullResetPin points to does nothing. No need to wait for 60ms.
    //    ((gc0308_resource_t *)(handle->resource))->pullResetPin(false);
    //     GC0308_DelayMs(50);
    //    ((gc0308_resource_t *)(handle->resource))->pullResetPin(true);
    //     GC0308_DelayMs(10);

    return kStatus_Success;
}

/*!
 * @brief Enable / Disable GC0308 power-down.
 * @param handle Handle to the camera pointer.
 * @param powerdown 1 to activate the power-down mode, 0 to activate the normal mode
 */
static status_t GC0308_PowerDown(camera_device_handle_t *handle, bool powerdown)
{
    ((gc0308_resource_t *)(handle->resource))->pullPowerDownPin(powerdown);

    return kStatus_Success;
}

/*!
 * @brief Select GC0308 Gamma Curve.
 * @param handle Handle to the camera pointer.
 * @param GammaLvl The gamma level between 1 to 5.
 */
static void GC0308_GammaSelect(camera_device_handle_t *handle, uint32_t GammaLvl)
{
    switch (GammaLvl)
    {
        case 1: // smallest gamma curve
            GC0308_Write(handle, 0x9F, 1U, 0x0B);
            GC0308_Write(handle, 0xA0, 1U, 0x16);
            GC0308_Write(handle, 0xA1, 1U, 0x29);
            GC0308_Write(handle, 0xA2, 1U, 0x3C);
            GC0308_Write(handle, 0xA3, 1U, 0x4F);
            GC0308_Write(handle, 0xA4, 1U, 0x5F);
            GC0308_Write(handle, 0xA5, 1U, 0x6F);
            GC0308_Write(handle, 0xA6, 1U, 0x8A);
            GC0308_Write(handle, 0xA7, 1U, 0x9F);
            GC0308_Write(handle, 0xA8, 1U, 0xB4);
            GC0308_Write(handle, 0xA9, 1U, 0xC6);
            GC0308_Write(handle, 0xAA, 1U, 0xD3);
            GC0308_Write(handle, 0xAB, 1U, 0xDD);
            GC0308_Write(handle, 0xAC, 1U, 0xE5);
            GC0308_Write(handle, 0xAD, 1U, 0xF1);
            GC0308_Write(handle, 0xAE, 1U, 0xFA);
            GC0308_Write(handle, 0xAF, 1U, 0xFF);
            break;
        case 2:
            GC0308_Write(handle, 0x9F, 1U, 0x0E);
            GC0308_Write(handle, 0xA0, 1U, 0x1C);
            GC0308_Write(handle, 0xA1, 1U, 0x34);
            GC0308_Write(handle, 0xA2, 1U, 0x48);
            GC0308_Write(handle, 0xA3, 1U, 0x5A);
            GC0308_Write(handle, 0xA4, 1U, 0x6B);
            GC0308_Write(handle, 0xA5, 1U, 0x7B);
            GC0308_Write(handle, 0xA6, 1U, 0x95);
            GC0308_Write(handle, 0xA7, 1U, 0xAB);
            GC0308_Write(handle, 0xA8, 1U, 0xBF);
            GC0308_Write(handle, 0xA9, 1U, 0xCE);
            GC0308_Write(handle, 0xAA, 1U, 0xD9);
            GC0308_Write(handle, 0xAB, 1U, 0xE4);
            GC0308_Write(handle, 0xAC, 1U, 0xEC);
            GC0308_Write(handle, 0xAD, 1U, 0xF7);
            GC0308_Write(handle, 0xAE, 1U, 0xFD);
            GC0308_Write(handle, 0xAF, 1U, 0xFF);
            break;
        case 3:
            GC0308_Write(handle, 0x9F, 1U, 0x10);
            GC0308_Write(handle, 0xA0, 1U, 0x20);
            GC0308_Write(handle, 0xA1, 1U, 0x38);
            GC0308_Write(handle, 0xA2, 1U, 0x4E);
            GC0308_Write(handle, 0xA3, 1U, 0x63);
            GC0308_Write(handle, 0xA4, 1U, 0x76);
            GC0308_Write(handle, 0xA5, 1U, 0x87);
            GC0308_Write(handle, 0xA6, 1U, 0xA2);
            GC0308_Write(handle, 0xA7, 1U, 0xB8);
            GC0308_Write(handle, 0xA8, 1U, 0xCA);
            GC0308_Write(handle, 0xA9, 1U, 0xD8);
            GC0308_Write(handle, 0xAA, 1U, 0xE3);
            GC0308_Write(handle, 0xAB, 1U, 0xEB);
            GC0308_Write(handle, 0xAC, 1U, 0xF0);
            GC0308_Write(handle, 0xAD, 1U, 0xF8);
            GC0308_Write(handle, 0xAE, 1U, 0xFD);
            GC0308_Write(handle, 0xAF, 1U, 0xFF);
            break;
        case 4:
            GC0308_Write(handle, 0x9F, 1U, 0x14);
            GC0308_Write(handle, 0xA0, 1U, 0x28);
            GC0308_Write(handle, 0xA1, 1U, 0x44);
            GC0308_Write(handle, 0xA2, 1U, 0x5D);
            GC0308_Write(handle, 0xA3, 1U, 0x72);
            GC0308_Write(handle, 0xA4, 1U, 0x86);
            GC0308_Write(handle, 0xA5, 1U, 0x95);
            GC0308_Write(handle, 0xA6, 1U, 0xB1);
            GC0308_Write(handle, 0xA7, 1U, 0xC6);
            GC0308_Write(handle, 0xA8, 1U, 0xD5);
            GC0308_Write(handle, 0xA9, 1U, 0xE1);
            GC0308_Write(handle, 0xAA, 1U, 0xEA);
            GC0308_Write(handle, 0xAB, 1U, 0xF1);
            GC0308_Write(handle, 0xAC, 1U, 0xF5);
            GC0308_Write(handle, 0xAD, 1U, 0xFB);
            GC0308_Write(handle, 0xAE, 1U, 0xFE);
            GC0308_Write(handle, 0xAF, 1U, 0xFF);
            break;
        case 5: // largest gamma curve
            GC0308_Write(handle, 0x9F, 1U, 0x15);
            GC0308_Write(handle, 0xA0, 1U, 0x2A);
            GC0308_Write(handle, 0xA1, 1U, 0x4A);
            GC0308_Write(handle, 0xA2, 1U, 0x67);
            GC0308_Write(handle, 0xA3, 1U, 0x79);
            GC0308_Write(handle, 0xA4, 1U, 0x8C);
            GC0308_Write(handle, 0xA5, 1U, 0x9A);
            GC0308_Write(handle, 0xA6, 1U, 0xB3);
            GC0308_Write(handle, 0xA7, 1U, 0xC5);
            GC0308_Write(handle, 0xA8, 1U, 0xD5);
            GC0308_Write(handle, 0xA9, 1U, 0xDF);
            GC0308_Write(handle, 0xAA, 1U, 0xE8);
            GC0308_Write(handle, 0xAB, 1U, 0xEE);
            GC0308_Write(handle, 0xAC, 1U, 0xF3);
            GC0308_Write(handle, 0xAD, 1U, 0xFA);
            GC0308_Write(handle, 0xAE, 1U, 0xFD);
            GC0308_Write(handle, 0xAF, 1U, 0xFF);
            break;
        default:
            break;
    }
}

/*!
 * @brief Applies the initial settings to GC0308 sensor.
 * Note that GC0308 initial registers should not be changed here.
 * @param handle Handle to the camera pointer.
 * @param config Pointer to the user-defined configuration structure.
 */
static void GC0308_SensorInitialSetting(camera_device_handle_t *handle, const camera_config_t *config)
{
    uint32_t vb = 0, hb = 0;
    uint32_t inputClockFreq_Hz = ((gc0308_resource_t *)(handle->resource))->inputClockFreq_Hz;

#define VB_MIN                  488
#define HB_MIN                  694
#define VB_HIGH_HB_HIGH(vb, hb) ((((vb - VB_MIN) >> 8) << 4) | ((hb - HB_MIN) >> 8))
#define VB_LOW(vb)              ((vb - VB_MIN) & 0xFF)
#define HB_LOW(hb)              ((hb - HB_MIN) & 0xFF)

    GC0308_Write(handle, 0xfe, 1U, 0x80);
    GC0308_Write(handle, 0xfe, 1U, 0x00); /* set page0 */
    GC0308_Write(handle, 0xd2, 1U, 0x10); /* close AEC */
    GC0308_Write(handle, 0x22, 1U, 0x55); /* close AWB */

    GC0308_Write(handle, 0x5a, 1U, 0x56);
    GC0308_Write(handle, 0x5b, 1U, 0x40);
    GC0308_Write(handle, 0x5c, 1U, 0x4a);

    GC0308_Write(handle, 0x22, 1U, 0x57); /* Open AWB */

    // fps: inputClockFreq_Hz/2/(V+Vblank)/(H+Hblank)
    // 0xf[3:0] and 0x1[7:0]  indicate Hblank or dummy pixel time
    // 0xf[7:4] and 0x2[7:0]  indicate Vblank or dummy pixel time
    if (24000000 == inputClockFreq_Hz)
    {
        switch (config->framePerSec)
        {
            case 30:
                vb = 500;
                hb = 800;
                // fps = 24M/2/(694+106)/(488+12)=30
                // GC0308_Write(handle, 0x01, 1U, 0x6a); //694 + 106 = 800
                // GC0308_Write(handle, 0x02, 1U, 0x0c); //488 +  12 = 500
                // GC0308_Write(handle, 0x0f, 1U, 0x00);
                break;
            case 20:
                vb = 750;
                hb = 800;
                // fps = 24M/2/(694+306)/(488+112)=20
                // GC0308_Write(handle, 0x01, 1U, 0x32); // 694 + 306 = 1000
                // GC0308_Write(handle, 0x02, 1U, 0x70); // 488 + 112 = 600
                // GC0308_Write(handle, 0x0f, 1U, 0x01);
                break;
            case 15:
                vb = 1000;
                hb = 800;
                // fps = 24M/2/(694+106)/(488+512)=15
                // GC0308_Write(handle, 0x01, 1U, 0x6a); // 694 + 106 = 800
                // GC0308_Write(handle, 0x02, 1U, 0x00); // 488 + 0x200 = 1000
                // GC0308_Write(handle, 0x0f, 1U, 0x20);
                break;
            case 10:
                vb = 800;
                hb = 1500;
                // GC0308_Write(handle, 0x01, 1U, 0x6a); //694 + 106 = 800
                // GC0308_Write(handle, 0x02, 1U, 0xF4); //488 + 1012 = 1500
                // GC0308_Write(handle, 0x0f, 1U, (1500-694)&0xF00);
                break;
            case 5:
                vb = 1200;
                hb = 2000;
                // GC0308_Write(handle, 0x01, 1U, VB_LOW(2000)); // 694 + 0x51a = 2000
                // GC0308_Write(handle, 0x02, 1U, HB_LOW(1200)); // 488 + 0x2c8 = 1200
                // GC0308_Write(handle, 0x0f, 1U, VB_HIGH_HB_HIGH(1200,2000));
                break;
            default:
                vb = 500;
                hb = 800;
                // fps = 24M/2/(694+106)/(488+12)=30
                // GC0308_Write(handle, 0x01, 1U, 0x6a); //694 + 106 = 800
                // GC0308_Write(handle, 0x02, 1U, 0x0c); //488 +  12 = 500
                // GC0308_Write(handle, 0x0f, 1U, 0x00);
                break;
        }
    }
    else if (12000000 == inputClockFreq_Hz)
    {
        switch (config->framePerSec)
        {
            case 15:
                vb = 500;
                hb = 800;
                // fps = 12M/2/(694+106)/(488+12)=15
                // GC0308_Write(handle, 0x01, 1U, 0x6a); //694 + 106 = 800
                // GC0308_Write(handle, 0x02, 1U, 0x0c); //488 +  12 = 500
                // GC0308_Write(handle, 0x0f, 1U, 0x00);
                break;
            case 10:
                vb = 750;
                hb = 800;
                // fps = 12M/2/(694+306)/(488+112)=10 */
                // GC0308_Write(handle, 0x01, 1U, 0x32); // 694 + 306 = 1000
                // GC0308_Write(handle, 0x02, 1U, 0x70); // 488 + 112 = 600
                // GC0308_Write(handle, 0x0f, 1U, 0x01);
                break;
            default:
                vb = 500;
                hb = 800;
                // fps = 12M/2/(694+106)/(488+12)=15
                // GC0308_Write(handle, 0x01, 1U, 0x6a); //694 + 106 = 800
                // GC0308_Write(handle, 0x02, 1U, 0x0c); //488 +  12 = 500
                // GC0308_Write(handle, 0x0f, 1U, 0x00);
                break;
        }
    }

    GC0308_Write(handle, 0x01, 1U, HB_LOW(hb));
    GC0308_Write(handle, 0x02, 1U, VB_LOW(vb));
    GC0308_Write(handle, 0x0f, 1U, VB_HIGH_HB_HIGH(vb, hb));

    GC0308_Write(handle, 0x03, 1U, 0x01); // 300
    GC0308_Write(handle, 0x04, 1U, 0x2c);

    GC0308_Write(handle, 0xe2, 1U, 0x00); /* anti-flicker step [11:8] */
    GC0308_Write(handle, 0xe3, 1U, 0x64); /* anti-flicker step [7:0] */

    /*  original camera exp setting for reference.
        GC0308_Write(handle, 0xe4, 1U, 0x02);    exp level 0  16.67fps
        GC0308_Write(handle, 0xe5, 1U, 0xbc);
        GC0308_Write(handle, 0xe6, 1U, 0x03);    exp level 1  12.5fps
        GC0308_Write(handle, 0xe7, 1U, 0xe8);
        GC0308_Write(handle, 0xe8, 1U, 0x04);    exp level 2  8.33fps
        GC0308_Write(handle, 0xe9, 1U, 0xb0);
        GC0308_Write(handle, 0xea, 1U, 0x09);    exp level 3  4.00fps
        GC0308_Write(handle, 0xeb, 1U, 0xc4);
    */

    GC0308_Write(handle, 0xe4, 1U, 0x02); /* exp level 0, 600*/
    GC0308_Write(handle, 0xe5, 1U, 0x58);
    GC0308_Write(handle, 0xe6, 1U, 0x03); /* exp level 1, 900*/
    GC0308_Write(handle, 0xe7, 1U, 0x84);
    GC0308_Write(handle, 0xe8, 1U, 0x07); /* exp level 2, 1800*/
    GC0308_Write(handle, 0xe9, 1U, 0x08);
    GC0308_Write(handle, 0xea, 1U, 0x0a); /* exp level 3, 2700*/
    GC0308_Write(handle, 0xeb, 1U, 0x8c);

    GC0308_Write(handle, 0x05, 1U, 0x00);
    GC0308_Write(handle, 0x06, 1U, 0x00);
    GC0308_Write(handle, 0x07, 1U, 0x00);
    GC0308_Write(handle, 0x08, 1U, 0x00);
    GC0308_Write(handle, 0x09, 1U, 0x01);
    GC0308_Write(handle, 0x0a, 1U, 0xe8);
    GC0308_Write(handle, 0x0b, 1U, 0x02);
    GC0308_Write(handle, 0x0c, 1U, 0x88);
    GC0308_Write(handle, 0x0d, 1U, 0x02);
    GC0308_Write(handle, 0x0e, 1U, 0x02);
    GC0308_Write(handle, 0x10, 1U, 0x22);
    GC0308_Write(handle, 0x11, 1U, 0xfd);
    GC0308_Write(handle, 0x12, 1U, 0x2a);
    GC0308_Write(handle, 0x13, 1U, 0x00);
#if (DUAL_CAMERA)
    GC0308_Write(handle, 0x14, 1U, 0x12);
#else
    GC0308_Write(handle, 0x14, 1U, 0x11);
#endif
    GC0308_Write(handle, 0x15, 1U, 0x0a);
    GC0308_Write(handle, 0x16, 1U, 0x05);
    GC0308_Write(handle, 0x17, 1U, 0x01);
    GC0308_Write(handle, 0x18, 1U, 0x44);
    GC0308_Write(handle, 0x19, 1U, 0x44);
    GC0308_Write(handle, 0x1a, 1U, 0x1e);
    GC0308_Write(handle, 0x1b, 1U, 0x00);
    GC0308_Write(handle, 0x1c, 1U, 0xc1);
    GC0308_Write(handle, 0x1d, 1U, 0x08);
    GC0308_Write(handle, 0x1e, 1U, 0x60);
    GC0308_Write(handle, 0x1f, 1U, 0x16);
    GC0308_Write(handle, 0x20, 1U, 0xff);
    GC0308_Write(handle, 0x21, 1U, 0xf8);
    GC0308_Write(handle, 0x22, 1U, 0x57);

    switch (config->pixelFormat)
    {
        case kVIDEO_PixelFormatRGB565:
            GC0308_Write(handle, 0x24, 1U, 0xa6);
            break;
        case kVIDEO_PixelFormatYUYV: /* YCbYCr */
            GC0308_Write(handle, 0x24, 1U, 0xa2);
            break;
        case kVIDEO_PixelFormatYVYU: /* YCrYCb */
            GC0308_Write(handle, 0x24, 1U, 0xa3);
            break;
        case kVIDEO_PixelFormatUYVY: /* CbYCrY */
            GC0308_Write(handle, 0x24, 1U, 0xa0);
            break;
        case kVIDEO_PixelFormatVYUY: /* CrYCbY */
            GC0308_Write(handle, 0x24, 1U, 0xa1);
            break;
        default: /* RGB565 */
            GC0308_Write(handle, 0x24, 1U, 0xa6);
            break;
    }

    GC0308_Write(handle, 0x25, 1U, 0x0f);

    /* output sync_mode */
    switch (config->controlFlags)
    {
        case (kCAMERA_DataLatchOnRisingEdge | kCAMERA_HrefActiveHigh | kCAMERA_VsyncActiveLow):
            GC0308_Write(handle, 0x26, 1U, 0x02);
            break;
        case (kCAMERA_DataLatchOnRisingEdge | kCAMERA_HrefActiveHigh | kCAMERA_VsyncActiveHigh):
            GC0308_Write(handle, 0x26, 1U, 0x03);
            break;
        case (kCAMERA_DataLatchOnRisingEdge | kCAMERA_HrefActiveLow | kCAMERA_VsyncActiveLow):
            GC0308_Write(handle, 0x26, 1U, 0x00);
            break;
        case (kCAMERA_DataLatchOnRisingEdge | kCAMERA_HrefActiveLow | kCAMERA_VsyncActiveHigh):
            GC0308_Write(handle, 0x26, 1U, 0x01);
            break;
        default:
            GC0308_Write(handle, 0x26, 1U, 0x03);
            break;
    }

    GC0308_Write(handle, 0x2f, 1U, 0x01);
    GC0308_Write(handle, 0x30, 1U, 0xf7);
    GC0308_Write(handle, 0x31, 1U, 0x50);
    GC0308_Write(handle, 0x32, 1U, 0x00);
    GC0308_Write(handle, 0x39, 1U, 0x04);
    GC0308_Write(handle, 0x3a, 1U, 0x18);
    GC0308_Write(handle, 0x3b, 1U, 0x20);
    GC0308_Write(handle, 0x3c, 1U, 0x00);
    GC0308_Write(handle, 0x3d, 1U, 0x00);
    GC0308_Write(handle, 0x3e, 1U, 0x00);
    GC0308_Write(handle, 0x3f, 1U, 0x00);
    GC0308_Write(handle, 0x50, 1U, 0x10);
    GC0308_Write(handle, 0x53, 1U, 0x82);
    GC0308_Write(handle, 0x54, 1U, 0x80);
    GC0308_Write(handle, 0x55, 1U, 0x80);
    GC0308_Write(handle, 0x56, 1U, 0x82);
    GC0308_Write(handle, 0x57, 1U, 0x78);
    GC0308_Write(handle, 0x58, 1U, 0x78);
    GC0308_Write(handle, 0x59, 1U, 0x86);

    GC0308_Write(handle, 0x8b, 1U, 0x40);
    GC0308_Write(handle, 0x8c, 1U, 0x40);
    GC0308_Write(handle, 0x8d, 1U, 0x40);
    GC0308_Write(handle, 0x8e, 1U, 0x2e);
    GC0308_Write(handle, 0x8f, 1U, 0x2e);
    GC0308_Write(handle, 0x90, 1U, 0x2e);
    GC0308_Write(handle, 0x91, 1U, 0x3c);
    GC0308_Write(handle, 0x92, 1U, 0x50);
    GC0308_Write(handle, 0x5d, 1U, 0x12);
    GC0308_Write(handle, 0x5e, 1U, 0x1a);
    GC0308_Write(handle, 0x5f, 1U, 0x24);
    GC0308_Write(handle, 0x60, 1U, 0x07);
    GC0308_Write(handle, 0x61, 1U, 0x15);
    GC0308_Write(handle, 0x62, 1U, 0x08);
    GC0308_Write(handle, 0x64, 1U, 0x03);
    GC0308_Write(handle, 0x66, 1U, 0xe8);
    GC0308_Write(handle, 0x67, 1U, 0x86);
    GC0308_Write(handle, 0x68, 1U, 0xa2);
    GC0308_Write(handle, 0x69, 1U, 0x18);
    GC0308_Write(handle, 0x6a, 1U, 0x0f);
    GC0308_Write(handle, 0x6b, 1U, 0x00);
    GC0308_Write(handle, 0x6c, 1U, 0x5f);
    GC0308_Write(handle, 0x6d, 1U, 0x8f);
    GC0308_Write(handle, 0x6e, 1U, 0x55);
    GC0308_Write(handle, 0x6f, 1U, 0x38);
    GC0308_Write(handle, 0x70, 1U, 0x15);
    GC0308_Write(handle, 0x71, 1U, 0x33);
    GC0308_Write(handle, 0x72, 1U, 0xdc);
    GC0308_Write(handle, 0x73, 1U, 0x80);
    GC0308_Write(handle, 0x74, 1U, 0x02);
    GC0308_Write(handle, 0x75, 1U, 0x3f);
    GC0308_Write(handle, 0x76, 1U, 0x02);
    GC0308_Write(handle, 0x77, 1U, 0x38);
    GC0308_Write(handle, 0x78, 1U, 0x88);
    GC0308_Write(handle, 0x79, 1U, 0x81);
    GC0308_Write(handle, 0x7a, 1U, 0x81);
    GC0308_Write(handle, 0x7b, 1U, 0x22);
    GC0308_Write(handle, 0x7c, 1U, 0xff);

    GC0308_Write(handle, 0x93, 1U, 0x48);
    GC0308_Write(handle, 0x94, 1U, 0x00);
    GC0308_Write(handle, 0x95, 1U, 0x05);
    GC0308_Write(handle, 0x96, 1U, 0xe8);
    GC0308_Write(handle, 0x97, 1U, 0x40);
    GC0308_Write(handle, 0x98, 1U, 0xf0);

    GC0308_Write(handle, 0xb1, 1U, 0x38);
    GC0308_Write(handle, 0xb2, 1U, 0x38);
    GC0308_Write(handle, 0xbd, 1U, 0x38);
    GC0308_Write(handle, 0xbe, 1U, 0x36);
    GC0308_Write(handle, 0xd0, 1U, 0xc9);
    GC0308_Write(handle, 0xd1, 1U, 0x10);

    GC0308_Write(handle, 0xd3, 1U, 0x80);
    GC0308_Write(handle, 0xd5, 1U, 0xf2);
    GC0308_Write(handle, 0xd6, 1U, 0x16);
    GC0308_Write(handle, 0xdb, 1U, 0x92);
    GC0308_Write(handle, 0xdc, 1U, 0xa5);
    GC0308_Write(handle, 0xdf, 1U, 0x23);
    GC0308_Write(handle, 0xd9, 1U, 0x00);
    GC0308_Write(handle, 0xda, 1U, 0x00);
    GC0308_Write(handle, 0xe0, 1U, 0x09);
    /*bit [4,5] decide which exp level is activated: 0-3*/
    // GC0308_Write(handle, 0xec, 1U, 0x20);
    GC0308_Write(handle, 0xec, 1U, 0x00);

    GC0308_Write(handle, 0xed, 1U, 0x04);
    GC0308_Write(handle, 0xee, 1U, 0x40);
    GC0308_Write(handle, 0xef, 1U, 0x40);
    GC0308_Write(handle, 0x80, 1U, 0x03);
    GC0308_Write(handle, 0x80, 1U, 0x03);
    GC0308_Write(handle, 0x9F, 1U, 0x10);
    GC0308_Write(handle, 0xA0, 1U, 0x20);
    GC0308_Write(handle, 0xA1, 1U, 0x38);
    GC0308_Write(handle, 0xA2, 1U, 0x4E);
    GC0308_Write(handle, 0xA3, 1U, 0x63);
    GC0308_Write(handle, 0xA4, 1U, 0x76);
    GC0308_Write(handle, 0xA5, 1U, 0x87);
    GC0308_Write(handle, 0xA6, 1U, 0xA2);
    GC0308_Write(handle, 0xA7, 1U, 0xB8);
    GC0308_Write(handle, 0xA8, 1U, 0xCA);
    GC0308_Write(handle, 0xA9, 1U, 0xD8);
    GC0308_Write(handle, 0xAA, 1U, 0xE3);
    GC0308_Write(handle, 0xAB, 1U, 0xEB);
    GC0308_Write(handle, 0xAC, 1U, 0xF0);
    GC0308_Write(handle, 0xAD, 1U, 0xF8);
    GC0308_Write(handle, 0xAE, 1U, 0xFD);
    GC0308_Write(handle, 0xAF, 1U, 0xFF);
    GC0308_Write(handle, 0xc0, 1U, 0x00);
    GC0308_Write(handle, 0xc1, 1U, 0x10);
    GC0308_Write(handle, 0xc2, 1U, 0x1C);
    GC0308_Write(handle, 0xc3, 1U, 0x30);
    GC0308_Write(handle, 0xc4, 1U, 0x43);
    GC0308_Write(handle, 0xc5, 1U, 0x54);
    GC0308_Write(handle, 0xc6, 1U, 0x65);
    GC0308_Write(handle, 0xc7, 1U, 0x75);
    GC0308_Write(handle, 0xc8, 1U, 0x93);
    GC0308_Write(handle, 0xc9, 1U, 0xB0);
    GC0308_Write(handle, 0xca, 1U, 0xCB);
    GC0308_Write(handle, 0xcb, 1U, 0xE6);
    GC0308_Write(handle, 0xcc, 1U, 0xFF);
    GC0308_Write(handle, 0xf0, 1U, 0x02);
    GC0308_Write(handle, 0xf1, 1U, 0x01);
    GC0308_Write(handle, 0xf2, 1U, 0x01);
    GC0308_Write(handle, 0xf3, 1U, 0x30);
    GC0308_Write(handle, 0xf9, 1U, 0x9f);
    GC0308_Write(handle, 0xfa, 1U, 0x78);

    GC0308_Write(handle, 0xfe, 1U, 0x01); /* set page1 */

    GC0308_Write(handle, 0x00, 1U, 0xf5);
    GC0308_Write(handle, 0x02, 1U, 0x1a);
    GC0308_Write(handle, 0x0a, 1U, 0xa0);
    GC0308_Write(handle, 0x0b, 1U, 0x60);
    GC0308_Write(handle, 0x0c, 1U, 0x08);
    GC0308_Write(handle, 0x0e, 1U, 0x4c);
    GC0308_Write(handle, 0x0f, 1U, 0x39);
    GC0308_Write(handle, 0x11, 1U, 0x3f);
    GC0308_Write(handle, 0x12, 1U, 0x72);
    GC0308_Write(handle, 0x13, 1U, 0x13);
    GC0308_Write(handle, 0x14, 1U, 0x42);
    GC0308_Write(handle, 0x15, 1U, 0x43);
    GC0308_Write(handle, 0x16, 1U, 0xc2);
    GC0308_Write(handle, 0x17, 1U, 0xa8);
    GC0308_Write(handle, 0x18, 1U, 0x18);
    GC0308_Write(handle, 0x19, 1U, 0x40);
    GC0308_Write(handle, 0x1a, 1U, 0xd0);
    GC0308_Write(handle, 0x1b, 1U, 0xf5);
    GC0308_Write(handle, 0x70, 1U, 0x40);
    GC0308_Write(handle, 0x71, 1U, 0x58);
    GC0308_Write(handle, 0x72, 1U, 0x30);
    GC0308_Write(handle, 0x73, 1U, 0x48);
    GC0308_Write(handle, 0x74, 1U, 0x20);
    GC0308_Write(handle, 0x75, 1U, 0x60);
    GC0308_Write(handle, 0x77, 1U, 0x20);
    GC0308_Write(handle, 0x78, 1U, 0x32);
    GC0308_Write(handle, 0x30, 1U, 0x03);
    GC0308_Write(handle, 0x31, 1U, 0x40);
    GC0308_Write(handle, 0x32, 1U, 0xe0);
    GC0308_Write(handle, 0x33, 1U, 0xe0);
    GC0308_Write(handle, 0x34, 1U, 0xe0);
    GC0308_Write(handle, 0x35, 1U, 0xb0);
    GC0308_Write(handle, 0x36, 1U, 0xc0);
    GC0308_Write(handle, 0x37, 1U, 0xc0);
    GC0308_Write(handle, 0x38, 1U, 0x04);
    GC0308_Write(handle, 0x39, 1U, 0x09);
    GC0308_Write(handle, 0x3a, 1U, 0x12);
    GC0308_Write(handle, 0x3b, 1U, 0x1C);
    GC0308_Write(handle, 0x3c, 1U, 0x28);
    GC0308_Write(handle, 0x3d, 1U, 0x31);
    GC0308_Write(handle, 0x3e, 1U, 0x44);
    GC0308_Write(handle, 0x3f, 1U, 0x57);
    GC0308_Write(handle, 0x40, 1U, 0x6C);
    GC0308_Write(handle, 0x41, 1U, 0x81);
    GC0308_Write(handle, 0x42, 1U, 0x94);
    GC0308_Write(handle, 0x43, 1U, 0xA7);
    GC0308_Write(handle, 0x44, 1U, 0xB8);
    GC0308_Write(handle, 0x45, 1U, 0xD6);
    GC0308_Write(handle, 0x46, 1U, 0xEE);
    GC0308_Write(handle, 0x47, 1U, 0x0d);

    GC0308_Write(handle, 0xfe, 1U, 0x00); /* set page0 */

    // GC0308_Write(handle, 0xd2, 1U, 0x90);
}

/*!
 * @brief Applies the customizable settings to GC0308 sensor.
 *  This function is provided to modify the necessary initialization Registers.
 *  Do not modify the registers in GC0308_SensorInitialSetting directly.
 * @param handle Handle to the camera pointer.
 */
static void GC0308_WriteMoreRegisters(camera_device_handle_t *handle)
{
    GC0308_Write(handle, 0xfe, 1U, 0x00); /* set page0 */

    GC0308_Write(handle, 0x10, 1U, 0x26);
    GC0308_Write(handle, 0x11, 1U, 0x0d);
    GC0308_Write(handle, 0x1a, 1U, 0x2a);

    GC0308_Write(handle, 0x1c, 1U, 0x49);
    GC0308_Write(handle, 0x1d, 1U, 0x9a);
    GC0308_Write(handle, 0x1e, 1U, 0x61);

    GC0308_Write(handle, 0x3a, 1U, 0x20);

    GC0308_Write(handle, 0x50, 1U, 0x14);
    GC0308_Write(handle, 0x53, 1U, 0x80);
    GC0308_Write(handle, 0x56, 1U, 0x80);

    GC0308_Write(handle, 0x8b, 1U, 0x20); /* LSC */
    GC0308_Write(handle, 0x8c, 1U, 0x20);
    GC0308_Write(handle, 0x8d, 1U, 0x20);
    GC0308_Write(handle, 0x8e, 1U, 0x14);
    GC0308_Write(handle, 0x8f, 1U, 0x10);
    GC0308_Write(handle, 0x90, 1U, 0x14);

    GC0308_Write(handle, 0x94, 1U, 0x02);
    GC0308_Write(handle, 0x95, 1U, 0x07);
    GC0308_Write(handle, 0x96, 1U, 0xe0);

    GC0308_Write(handle, 0xb1, 1U, 0x38);
    GC0308_Write(handle, 0xb2, 1U, 0x38);
    GC0308_Write(handle, 0xb3, 1U, 0x3a);
    GC0308_Write(handle, 0xb6, 1U, 0xe0);

    GC0308_Write(handle, 0xd0, 1U, 0xcb); /* AECT */
    GC0308_Write(handle, 0xd3, 1U, 0x48);

    GC0308_Write(handle, 0xf2, 1U, 0x00);
    GC0308_Write(handle, 0xf7, 1U, 0x12);
    GC0308_Write(handle, 0xf8, 1U, 0x0a);

    GC0308_Write(handle, 0xfe, 1U, 0x01); /* set page1 */

    GC0308_Write(handle, 0x02, 1U, 0x20);
    GC0308_Write(handle, 0x04, 1U, 0x10);
    GC0308_Write(handle, 0x05, 1U, 0x08);
    GC0308_Write(handle, 0x06, 1U, 0x20);
    GC0308_Write(handle, 0x08, 1U, 0x0a);

    GC0308_Write(handle, 0x0e, 1U, 0x44);
    GC0308_Write(handle, 0x0f, 1U, 0x32);
    GC0308_Write(handle, 0x10, 1U, 0x41);
    GC0308_Write(handle, 0x11, 1U, 0x37);
    GC0308_Write(handle, 0x12, 1U, 0x22);
    GC0308_Write(handle, 0x13, 1U, 0x19);
    GC0308_Write(handle, 0x14, 1U, 0x44);
    GC0308_Write(handle, 0x15, 1U, 0x44);

    GC0308_Write(handle, 0x1c, 1U, 0x70);
    GC0308_Write(handle, 0x1d, 1U, 0x58);
    GC0308_Write(handle, 0x1e, 1U, 0x50);

    GC0308_Write(handle, 0x19, 1U, 0x50);
    GC0308_Write(handle, 0x1a, 1U, 0xd8);

    GC0308_Write(handle, 0x32, 1U, 0x10);

    GC0308_Write(handle, 0x35, 1U, 0x00);
    GC0308_Write(handle, 0x36, 1U, 0x80);
    GC0308_Write(handle, 0x37, 1U, 0x00);
    /* -----------Update the registers end--------- */
    GC0308_Write(handle, 0xfe, 1U, 0x00); /* set page0 */

    /* Customer can adjust GAMMA, MIRROR & UPSIDEDOWN, etc... here! */
    GC0308_GammaSelect(handle, 2); // option to adjust the gamma level
    //  GC0308_set_testmode(handle, ktest_image1, false);               // option to output a test pattern
    //  GC0308_set_orientation(handle, true, false);                    // option to flip the image (horizontal and / or
    //  vertical flip) GC0308_set_white_balance(handle, kwhite_balance_auto);          // option to tune the white
    //  balance GC0308_set_brightness(handle, -1);                              // option to adjust the brightness
    //  GC0308_set_effect(handle, kcam_effect_enc_grayscale);           // option to choose camera effect
}

/*!
 * @brief Crop the image frame.
 *  Crop the image width and height, define the origin of the cropped image
 * @param handle Handle to the camera pointer.
 * @param window_width width of the output frame.
 * @param window_height height of the output frame.
 * @param origin_x origin (x axis) of the output frame.
 * @param origin_y origin (y axis) of the output frame.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static status_t GC0308_CropImage(
    camera_device_handle_t *handle, uint16_t window_width, uint16_t window_height, uint16_t origin_x, uint16_t origin_y)
{
    status_t status = kStatus_Success;

    if ((window_height > 480U) || (window_width > 640U))
    {
        return kStatus_InvalidArgument;
    }
    if (((origin_y + window_height) > 480U) || ((origin_x + window_width) > 640U))
    {
        return kStatus_InvalidArgument;
    }

    status = GC0308_Write(handle, 0xfe, 1U, 0x00); /* set page0 */
    if (kStatus_Success != status)
    {
        return status;
    }

    status = GC0308_Write(handle, 0x46, 1U, 0x80 | ((origin_y & 0x0300) >> 4) | ((origin_x & 0x0700) >> 8));
    if (kStatus_Success != status)
    {
        return status;
    }

    status = GC0308_Write(handle, 0x47, 1U, origin_y & 0x0FF);
    if (kStatus_Success != status)
    {
        return status;
    }

    status = GC0308_Write(handle, 0x48, 1U, origin_x & 0x0FF);
    if (kStatus_Success != status)
    {
        return status;
    }

    status = GC0308_Write(handle, 0x49, 1U, (window_height & 0x0100) >> 8);
    if (kStatus_Success != status)
    {
        return status;
    }

    status = GC0308_Write(handle, 0x4a, 1U, window_height & 0x00FF);
    if (kStatus_Success != status)
    {
        return status;
    }

    status = GC0308_Write(handle, 0x4b, 1U, (window_width & 0x0100) >> 8);
    if (kStatus_Success != status)
    {
        return status;
    }

    status = GC0308_Write(handle, 0x4c, 1U, window_width & 0x00FF);
    return status;
}

/*!
 * @brief Enable test patterns.
 *  This function allows to output test patterns supported by the GC0308.
 * @param handle Handle to the camera pointer.
 * @param mode pattern mode to be used.
 * @param enable_test_mode enable / disable the test mode.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static status_t GC0308_SetTestMode(camera_device_handle_t *handle, gc0308_testmode_t mode, bool enable_test_mode)
{
    status_t status = kStatus_Success;

    status = GC0308_Write(handle, 0xfe, 1U, 0x00); /* set page0 */
    if (kStatus_Success != status)
    {
        return status;
    }

    if (enable_test_mode == true)
    {
        switch (mode)
        {
            case ktest_image1:
                status = GC0308_Write(handle, 0x2e, 1U, 0x02); /* enable test image 1 */
                break;
            case ktest_image2:
                status = GC0308_Write(handle, 0x2e, 1U, 0x01); /* enable test image 2 */
                break;
            default:
                status = GC0308_Write(handle, 0x2e, 1U, 0x00); /* disable the test pattern */
                break;
        }
    }
    else
    {
        status = GC0308_Write(handle, 0x2e, 1U, 0x00); /* disable the test pattern */
    }
    return status;
}

/*!
 * @brief Enable XY-mirror and / or Upside-Down.
 *  This function allows swap the picture on the horizontal and, or vertical axis.
 * @param handle Handle to the camera pointer.
 * @param enable_XYmirror swap the image on the horizontal axis.
 * @param enable_UpsideDown swap the image on the vertical axis.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static status_t GC0308_SetOrientation(camera_device_handle_t *handle, bool enable_XYmirror, bool enable_UpsideDown)
{
    status_t status = kStatus_Success;
    uint8_t reg_value;

    status = GC0308_Write(handle, 0xfe, 1U, 0x00); /* set page0 */
    if (kStatus_Success != status)
    {
        return status;
    }

    status = GC0308_Read(handle, 0x14, 1u, &reg_value);
    if (kStatus_Success != status)
    {
        return status;
    }

    status = GC0308_Write(handle, 0x14, 1U, (reg_value & 0xFC) | (enable_UpsideDown << 1) | (enable_XYmirror));
    return status;
}

/*!
 * @brief Adjust the image white balance.
 * @param handle Handle to the camera pointer.
 * @param white_balance gc0308_white_balance_t configuration.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static status_t GC0308_SetWhiteBalance(camera_device_handle_t *handle, gc0308_white_balance_t white_balance)
{
    status_t status = kStatus_Success;
    uint8_t temp_reg;

    status = GC0308_Write(handle, 0xfe, 1U, 0x00); /* set page0 */
    if (kStatus_Success != status)
    {
        return status;
    }

    status = GC0308_Read(handle, 0x14, 1u, &temp_reg);
    if (kStatus_Success != status)
    {
        return status;
    }

    switch (white_balance)
    {
        case kwhite_balance_auto:
            GC0308_Write(handle, 0x5a, 1U, 0x56); /* for AWB can adjust back */
            GC0308_Write(handle, 0x5b, 1U, 0x40);
            GC0308_Write(handle, 0x5c, 1U, 0x4a);
            GC0308_Write(handle, 0x22, 1U, temp_reg | 0x02); /* Enable AWB */
            break;
        case kwhite_balance_tungsten:
            GC0308_Write(handle, 0x22, 1U, temp_reg & ~0x02);
            GC0308_Write(handle, 0x5a, 1U, 0x40);
            GC0308_Write(handle, 0x5b, 1U, 0x54);
            GC0308_Write(handle, 0x5c, 1U, 0x70);
            break;
        case kwhite_balance_daylight:
            GC0308_Write(handle, 0x22, 1U, temp_reg & ~0x02);
            GC0308_Write(handle, 0x5a, 1U, 0x74);
            GC0308_Write(handle, 0x5b, 1U, 0x52);
            GC0308_Write(handle, 0x5c, 1U, 0x40);
            break;
        case kwhite_balance_fluorescent:
            GC0308_Write(handle, 0x22, 1U, temp_reg & ~0x02);
            GC0308_Write(handle, 0x5a, 1U, 0x40);
            GC0308_Write(handle, 0x5b, 1U, 0x42);
            GC0308_Write(handle, 0x5c, 1U, 0x50);
            break;
        case kwhite_balance_cloud:
            GC0308_Write(handle, 0x22, 1U, temp_reg & ~0x02); /* Disable AWB */
            GC0308_Write(handle, 0x5a, 1U, 0x8c);             /* WB_manual_gain */
            GC0308_Write(handle, 0x5b, 1U, 0x50);
            GC0308_Write(handle, 0x5c, 1U, 0x40);
            break;
        default:                                  /* balance auto */
            GC0308_Write(handle, 0x5a, 1U, 0x56); /* for AWB can adjust back */
            GC0308_Write(handle, 0x5b, 1U, 0x40);
            GC0308_Write(handle, 0x5c, 1U, 0x4a);
            GC0308_Write(handle, 0x22, 1U, temp_reg | 0x02); /* Enable AWB */
            break;
    }
    return kStatus_Success;
}

/*!
 * @brief Adjust the brightness (actually the exposure value).
 * @param handle Handle to the camera pointer.
 * @param brightness between -2 to +2.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static status_t GC0308_SetBrightness(camera_device_handle_t *handle, int8_t brightness)
{
    status_t status = kStatus_Success;

    status = GC0308_Write(handle, 0xfe, 1U, 0x00); /* set page0 */
    if (kStatus_Success != status)
    {
        return status;
    }

    switch (brightness)
    {
        case 2:
            GC0308_Write(handle, 0xb5, 1U, 0xc0);
            GC0308_Write(handle, 0xd3, 1U, 0x38);
            break;
        case 1:
            GC0308_Write(handle, 0xb5, 1U, 0xe0);
            GC0308_Write(handle, 0xd3, 1U, 0x48);
            break;
        case 0:
            GC0308_Write(handle, 0xb5, 1U, 0x08);
            GC0308_Write(handle, 0xd3, 1U, 0x48);
            break;
        case -1:
            GC0308_Write(handle, 0xb5, 1U, 0x20);
            GC0308_Write(handle, 0xd3, 1U, 0x58);
            break;
        case -2:
            GC0308_Write(handle, 0xb5, 1U, 0x40);
            GC0308_Write(handle, 0xd3, 1U, 0x60);
            break;
        default: /* default zero */
            GC0308_Write(handle, 0xb5, 1U, 0x08);
            GC0308_Write(handle, 0xd3, 1U, 0x50);
            break;
    }

    return kStatus_Success;
}

/*!
 * @brief select and enable a picture effect.
 * @param handle Handle to the camera pointer.
 * @param white_balance gc0308_cam_effect_t configuration.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static status_t GC0308_SetEffect(camera_device_handle_t *handle, gc0308_cam_effect_t effect)
{
    status_t status = kStatus_Success;

    status = GC0308_Write(handle, 0xfe, 1U, 0x00); /* set page0 */
    if (kStatus_Success != status)
    {
        return status;
    }

    switch (effect)
    {
        case kcam_effect_enc_normal:
            GC0308_Write(handle, 0x23, 1U, 0x00);
            GC0308_Write(handle, 0x2d, 1U, 0x0a);
            GC0308_Write(handle, 0x20, 1U, 0xff);
            GC0308_Write(handle, 0xd2, 1U, 0x90);
            GC0308_Write(handle, 0x73, 1U, 0x00);
            GC0308_Write(handle, 0x77, 1U, 0x38);
            GC0308_Write(handle, 0xb3, 1U, 0x3a);
            GC0308_Write(handle, 0xb4, 1U, 0x80);
            GC0308_Write(handle, 0xba, 1U, 0x00);
            GC0308_Write(handle, 0xbb, 1U, 0x00);
            break;
        case kcam_effect_enc_sepia:
            GC0308_Write(handle, 0x23, 1U, 0x02);
            GC0308_Write(handle, 0x2d, 1U, 0x0a);
            GC0308_Write(handle, 0x20, 1U, 0xff);
            GC0308_Write(handle, 0xd2, 1U, 0x90);
            GC0308_Write(handle, 0x73, 1U, 0x00);
            GC0308_Write(handle, 0xb3, 1U, 0x40);
            GC0308_Write(handle, 0xb4, 1U, 0x80);
            GC0308_Write(handle, 0xba, 1U, 0xd0);
            GC0308_Write(handle, 0xbb, 1U, 0x28);
            break;
        case kcam_effect_enc_colorinv:
            GC0308_Write(handle, 0x23, 1U, 0x01);
            GC0308_Write(handle, 0x2d, 1U, 0x0a);
            GC0308_Write(handle, 0x20, 1U, 0xff);
            GC0308_Write(handle, 0xd2, 1U, 0x90);
            GC0308_Write(handle, 0x73, 1U, 0x00);
            GC0308_Write(handle, 0xb3, 1U, 0x40);
            GC0308_Write(handle, 0xb4, 1U, 0x80);
            GC0308_Write(handle, 0xba, 1U, 0x00);
            GC0308_Write(handle, 0xbb, 1U, 0x00);
            break;
        case kcam_effect_enc_sepiagreen:
            GC0308_Write(handle, 0x23, 1U, 0x02);
            GC0308_Write(handle, 0x2d, 1U, 0x0a);
            GC0308_Write(handle, 0x20, 1U, 0xff);
            GC0308_Write(handle, 0xd2, 1U, 0x90);
            GC0308_Write(handle, 0x77, 1U, 0x88);
            GC0308_Write(handle, 0xb3, 1U, 0x40);
            GC0308_Write(handle, 0xb4, 1U, 0x80);
            GC0308_Write(handle, 0xba, 1U, 0xc0);
            GC0308_Write(handle, 0xbb, 1U, 0xc0);
            break;
        case kcam_effect_enc_grayscale:
            GC0308_Write(handle, 0x23, 1U, 0x02);
            GC0308_Write(handle, 0x2d, 1U, 0x0a);
            GC0308_Write(handle, 0x20, 1U, 0xff);
            GC0308_Write(handle, 0xd2, 1U, 0x90);
            GC0308_Write(handle, 0x73, 1U, 0x00);
            GC0308_Write(handle, 0xb3, 1U, 0x40);
            GC0308_Write(handle, 0xb4, 1U, 0x80);
            GC0308_Write(handle, 0xba, 1U, 0x00);
            GC0308_Write(handle, 0xbb, 1U, 0x00);
            break;
        default:
            GC0308_Write(handle, 0x23, 1U, 0x00);
            GC0308_Write(handle, 0x2d, 1U, 0x0a);
            GC0308_Write(handle, 0x20, 1U, 0xff);
            GC0308_Write(handle, 0xd2, 1U, 0x90);
            GC0308_Write(handle, 0x73, 1U, 0x00);
            GC0308_Write(handle, 0x77, 1U, 0x54);
            GC0308_Write(handle, 0xb3, 1U, 0x40);
            GC0308_Write(handle, 0xb4, 1U, 0x80);
            GC0308_Write(handle, 0xba, 1U, 0x00);
            GC0308_Write(handle, 0xbb, 1U, 0x00);
            break;
    }
    return kStatus_Success;
}

/*!
 * @brief Initialize GC0308 registers.
 * Write GC0308 default settings and customizable settings via I2C.
 * @param handle Handle to the camera pointer.
 * @param config pointer to camera_config_t configuration.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static status_t GC0308_RegInit(camera_device_handle_t *handle, const camera_config_t *config)
{
    status_t status = kStatus_Success;

    uint16_t width  = FSL_VIDEO_EXTRACT_WIDTH(config->resolution);
    uint16_t height = FSL_VIDEO_EXTRACT_HEIGHT(config->resolution);
    GC0308_SensorInitialSetting(handle, config);
    GC0308_WriteMoreRegisters(handle);

    switch (config->resolution)
    {
        case kVIDEO_ResolutionVGA:
            /* GC0308 has a native resolution of 640x480. This is already handled in GC0308_SensorInitialSetting
             * function */
            break;
        default:
            status = GC0308_CropImage(handle, width, height, (uint16_t)((GC0308_SENSOR_WIDTH - width) / 2),
                                      (uint16_t)((GC0308_SENSOR_HEIGHT - height) / 2));
            break;
    }
    return status;
}

/*!
 * @brief send multiple registers via I2C bus.
 * @param handle Handle to the camera pointer.
 * @param regs registers table.
 * @param num number of registers to be sent
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static status_t GC0308_MultiWrite(camera_device_handle_t *handle, const gc0308_reg_t regs[], uint32_t num)
{
    status_t status = kStatus_Success;

    for (uint32_t i = 0; i < num; i++)
    {
        status = GC0308_Write(handle, regs[i].reg, regs[i].size, regs[i].value);

        if (kStatus_Success != status)
        {
            break;
        }
    }
    return status;
}

/*!
 * @brief Initialize GC0308 sensor.
 * Reset the sensor, disable the power-down mode and write the sensor settings via I2C.
 * @param handle Handle to the camera pointer.
 * @param config pointer to camera_config_t configuration.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t GC0308_Init(camera_device_handle_t *handle, const camera_config_t *config)
{
    status_t status;
    uint8_t chip_id;

    if ((kCAMERA_InterfaceGatedClock != config->interface) && (kCAMERA_InterfaceNonGatedClock != config->interface))
    {
        return kStatus_InvalidArgument;
    }

    /* support 640x480 (VGA), 320x240 (QVGA), 160x120 (QQVGA), 480x272 (iMXRT1062 EVK configuration) resolutions */
    if ((kVIDEO_ResolutionVGA != config->resolution) && (kVIDEO_ResolutionQVGA != config->resolution) &&
        (kVIDEO_ResolutionQQVGA != config->resolution) &&
        (FSL_VIDEO_RESOLUTION(GC0308_SENSOR_WIDTH, GC0308_SENSOR_HEIGHT) != config->resolution))
    {
        return kStatus_InvalidArgument;
    }

    /* support 15 fps. */
    if (15 != config->framePerSec && 20 != config->framePerSec && 30 != config->framePerSec &&
        10 != config->framePerSec && 5 != config->framePerSec)
    {
        return kStatus_InvalidArgument;
    }

    /* support RGB565 and YUV422 */
    if ((kVIDEO_PixelFormatRGB565 != config->pixelFormat) && (kVIDEO_PixelFormatYUYV != config->pixelFormat) &&
        (kVIDEO_PixelFormatYVYU != config->pixelFormat) && (kVIDEO_PixelFormatUYVY != config->pixelFormat) &&
        (kVIDEO_PixelFormatVYUY != config->pixelFormat))
    {
        return kStatus_InvalidArgument;
    }

    /* The input clock (EXTCLK) must be 24MHz or 12MHz. */
    if ((24000000 != ((gc0308_resource_t *)(handle->resource))->inputClockFreq_Hz) &&
        (12000000 != ((gc0308_resource_t *)(handle->resource))->inputClockFreq_Hz))
    {
        return kStatus_InvalidArgument;
    }

    /* Polarity check */
    if ((config->controlFlags != (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge | kCAMERA_VsyncActiveLow)) &&
        (config->controlFlags != (kCAMERA_HrefActiveLow | kCAMERA_DataLatchOnRisingEdge | kCAMERA_VsyncActiveLow)) &&
        (config->controlFlags != (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge | kCAMERA_VsyncActiveHigh)) &&
        (config->controlFlags != (kCAMERA_HrefActiveLow | kCAMERA_DataLatchOnRisingEdge | kCAMERA_VsyncActiveHigh)))
    {
        return kStatus_InvalidArgument;
    }

    GC0308_Reset(handle);

    /* it is mandatory to deactivate the power-down, otherwise the sensor does not answer on the I2C bus */
    GC0308_PowerDown(handle, false);

    /* Identify the device. */
    status = GC0308_Read(handle, GC0308_REG_CHIP_ID, 1u, &chip_id);
    if (kStatus_Success != status)
    {
        return status;
    }
    if (GC0308_CHIP_ID != chip_id)
    {
        return kStatus_Fail;
    }

    status = GC0308_RegInit(handle, config);
    if (kStatus_Success != status)
    {
        return status;
    }

    return kStatus_Success;
}

/*!
 * @brief Deinitialize GC0308 sensor.
 * Reset the sensor and enable the power-down mode
 * @param handle Handle to the camera pointer.
 * @return Allways returns @ref kStatus_Success.
 */
status_t GC0308_Deinit(camera_device_handle_t *handle)
{
    GC0308_Reset(handle);
    GC0308_PowerDown(handle, true);

    return kStatus_Success;
}

/*!
 * @brief Start GC0308 sensor.
 * Actually does nothing !
 * @param handle Handle to the camera pointer.
 * @return Allways returns @ref kStatus_Success.
 */
status_t GC0308_Start(camera_device_handle_t *handle)
{
    GC0308_PowerDown(handle, false);
    return kStatus_Success;
}

/*!
 * @brief Stop GC0308 sensor.
 * Actually does nothing !
 * @param handle Handle to the camera pointer.
 * @return Allways returns @ref kStatus_Success.
 */
status_t GC0308_Stop(camera_device_handle_t *handle)
{
    GC0308_PowerDown(handle, true);
    return kStatus_Success;
}

/*!
 * @brief Control GC0308 sensor.
 * Actually does nothing !
 * @param handle Handle to the camera pointer.
 * @return Allways returns @ref kStatus_Success.
 */
status_t GC0308_Control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg)
{
    if (cmd == kCAMERA_DeviceMonoMode)
    {
        if (arg == 1)
            GC0308_SetEffect(handle, kcam_effect_enc_grayscale);
        else
            GC0308_SetEffect(handle, kcam_effect_enc_normal);
        return kStatus_Success;
    }
    else if (cmd == kCAMERA_DeviceExposureMode)
    {
        uint8_t reg_value;
        GC0308_Write(handle, 0xfe, 1U, 0x00); /* set page0 */
        GC0308_Write(handle, 0xec, 1U, (arg << 4));

        switch (arg)
        {
            case CAMERA_EXPOSURE_MODE_AUTO_LEVEL0:
            {
                GC0308_Write(handle, 0xee, 1U, 0x80); // post gain limit
                GC0308_Write(handle, 0xef, 1U, 0x40); // pre gain limit
            }
            break;
            case CAMERA_EXPOSURE_MODE_AUTO_LEVEL1:
            {
                GC0308_Write(handle, 0xee, 1U, 0x80); // post gain limit
                GC0308_Write(handle, 0xef, 1U, 0x80); // pre gain limit
            }
            break;
            case CAMERA_EXPOSURE_MODE_AUTO_LEVEL2:
            {
                GC0308_Write(handle, 0xee, 1U, 0xa0); // post gain limit
                GC0308_Write(handle, 0xef, 1U, 0x80); // pre gain limit
            }
            break;
            case CAMERA_EXPOSURE_MODE_AUTO_LEVEL3:
            {
                GC0308_Write(handle, 0xee, 1U, 0xa0); // post gain limit
                GC0308_Write(handle, 0xef, 1U, 0xa0); // pre gain limit
            }
            break;
            default:
                break;
        }
        return kStatus_Success;
    }
    else
    {
        return kStatus_InvalidArgument;
    }
}

/*!
 * @brief Initialize GC0308 sensor.
 * Launch GC0308_Init function.
 * @param handle Handle to the camera pointer.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t GC0308_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig)
{
    return GC0308_Init(handle, config);
}
