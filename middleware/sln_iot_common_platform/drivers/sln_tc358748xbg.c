/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sln_tc358748xbg.h"
#include "fsl_video_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TC358748_DelayMs(ms) VIDEO_DelayMs(ms)
#define TC358748_Write(reg, size, value, i2cSendFunc) \
    VIDEO_I2C_WriteReg(TC358748_I2C_SLAVE_ADDR, kVIDEO_RegAddr16Bit, reg, (video_reg_width_t)size, value, i2cSendFunc)
#define TC358748_Read(reg, size, value, i2cReceiveFunc) \
    VIDEO_I2C_ReadReg(TC358748_I2C_SLAVE_ADDR, kVIDEO_RegAddr16Bit, reg, (video_reg_width_t)size, value, i2cReceiveFunc)

typedef struct _tc358748_reg
{
    uint16_t reg_addr;
    uint16_t addr_len;
    uint16_t val;
    uint16_t val_len;
    uint32_t delay_ms;
} tc358748_reg_t;

#define TC358748_CHIP_NAME      "TC358748XBG"
#define TC358748_I2C_SLAVE_ADDR 0x0E
#define TC358748_CHIP_ID        0x4401
#define TC358748_REG_CHIP_ID    0x0000

#define TC358748_REG_SYSTEM_CONTROL      0x0002
#define TC358748_REG_CONFIG_CONTROL      0x0004
#define TC358748_REG_FIFO_CONTROL        0x0006
#define TC358748_REG_DATA_FORMAT_CONTROL 0x0008
#define TC358748_REG_MCLK_CONTROL        0x000C
#define TC358748_REG_PLL_CONTROL0        0x0016
#define TC358748_REG_PLL_CONTROL1        0x0018
#define TC358748_REG_CLK_CONTROL         0x0020
#define TC358748_REG_MIPI_PHY_TIME       0x0060

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const tc358748_reg_t tc358748_settings[] = {
    {TC358748_REG_SYSTEM_CONTROL, 2, 0x0001, 2, 1},      {TC358748_REG_SYSTEM_CONTROL, 2, 0x0000, 2, 0},
    {TC358748_REG_PLL_CONTROL0, 2, 0x105d, 2, 0},        {TC358748_REG_PLL_CONTROL1, 2, 0x0c13, 2, 0},
    {TC358748_REG_CLK_CONTROL, 2, 0x0022, 2, 0},         {TC358748_REG_MCLK_CONTROL, 2, 0x0101, 2, 0},
    {TC358748_REG_MIPI_PHY_TIME, 2, 0x800a, 2, 0},       {TC358748_REG_FIFO_CONTROL, 2, 0x0020, 2, 0},
    {TC358748_REG_DATA_FORMAT_CONTROL, 2, 0x0060, 2, 0}, // raw 12
    {TC358748_REG_CONFIG_CONTROL, 2, 0x8145, 2, 0},      // 2xlanes
};

static tc358748_resource_t *s_pTc358748Resource = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t TC358748_Init(tc358748_resource_t *resource)
{
    status_t status;
    uint16_t chip_id;
    s_pTc358748Resource = resource;
    if (s_pTc358748Resource->pullPowerEnablePin)
        s_pTc358748Resource->pullPowerEnablePin(true);

    if (s_pTc358748Resource->pullResetPin)
    {
        s_pTc358748Resource->pullResetPin(false);
        TC358748_DelayMs(10);
        s_pTc358748Resource->pullResetPin(true);
        TC358748_DelayMs(1);
    }

    /* Identify the device. */
    status = TC358748_Read(TC358748_REG_CHIP_ID, 2, &chip_id, s_pTc358748Resource->i2cReceiveFunc);

    if (kStatus_Success != status)
    {
        return status;
    }

    if (TC358748_CHIP_ID != chip_id)
    {
        return kStatus_Fail;
    }

    for (uint32_t i = 0; i < ARRAY_SIZE(tc358748_settings); i++)
    {
        status = TC358748_Write(tc358748_settings[i].reg_addr, tc358748_settings[i].val_len, tc358748_settings[i].val,
                                s_pTc358748Resource->i2cSendFunc);

        TC358748_DelayMs(tc358748_settings[i].delay_ms);
        if (kStatus_Success != status)
        {
            return status;
        }
    }

    return status;
}

status_t TC358748_SleepControl(tc358748_work_mode_t mode)
{
    status_t status;
    uint16_t value;
    if (!s_pTc358748Resource)
        return kStatus_Fail;

    if (mode == work_mode_normal)
    {
        value = 0x0000;
    }
    else
    {
        value = 0x0002;
    }
    status = TC358748_Write(TC358748_REG_SYSTEM_CONTROL, 2, value, s_pTc358748Resource->i2cSendFunc);
    return status;
}

void TC358748_DeInit(void)
{
    if (s_pTc358748Resource->pullPowerEnablePin)
    {
        s_pTc358748Resource->pullPowerEnablePin(false);
    }
    if (s_pTc358748Resource->pullResetPin)
    {
        s_pTc358748Resource->pullResetPin(false);
    }
    s_pTc358748Resource = NULL;
}
