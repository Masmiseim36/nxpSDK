/*
 * The Clear BSD License
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_dialog7212.h"

/*******************************************************************************
 * Definitations
 ******************************************************************************/

typedef struct _da7212_register_value
{
    uint8_t addr;
    uint8_t value;
} da7212_register_value_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const da7212_register_value_t kInputRegisterSequence[kDA7212_Input_MAX][17] = {
    /* DA7212_Input_AUX */
    {
        {DIALOG7212_MIXIN_L_SELECT, 0x01},
        {DIALOG7212_MIXIN_R_SELECT, 0x01},
        {DIALOG7212_CP_CTRL, 0xFD},
        {DIALOG7212_AUX_L_CTRL, 0xB4},
        {DIALOG7212_AUX_R_CTRL, 0xB0},
        {DIALOG7212_MIC_1_CTRL, 0x04},
        {DIALOG7212_MIC_2_CTRL, 0x04},
        {DIALOG7212_MIXIN_L_CTRL, 0x88},
        {DIALOG7212_MIXIN_R_CTRL, 0x88},
        {DIALOG7212_ADC_L_CTRL, 0xA0},
        {DIALOG7212_GAIN_RAMP_CTRL, 0x02},
        {DIALOG7212_PC_COUNT, 0x02},
        {DIALOG7212_CP_DELAY, 0x95},
    },
    /* DA7212_Input_MIC1_Dig */
    {
        {DIALOG7212_MICBIAS_CTRL, 0xA9},
        {DIALOG7212_CP_CTRL, 0xF1},
        {DIALOG7212_MIXIN_L_SELECT, 0x80},
        {DIALOG7212_MIXIN_R_SELECT, 0x80},
        {DIALOG7212_SYSTEM_MODES_INPUT, 0xFE},
        {DIALOG7212_SYSTEM_MODES_OUTPUT, 0xF7},
        {DIALOG7212_MIC_CONFIG, 0x07},
        {DIALOG7212_MIC_2_GAIN, 0x04},
        {DIALOG7212_MIC_2_CTRL, 0x84},
        {DIALOG7212_MIC_1_GAIN, 0x01},
        {DIALOG7212_MIC_1_CTRL, 0x80},
        {DIALOG7212_ADC_FILTERS1, 0x08},
    },
    /* DA7212_Input_MIC1_An */
    {
        {DIALOG7212_MIXIN_L_SELECT, 0x02},
        {DIALOG7212_MIXIN_R_SELECT, 0x04},
        {DIALOG7212_MIC_1_GAIN, 0x03},
        {DIALOG7212_CP_CTRL, 0xFD},
        {DIALOG7212_MIXOUT_L_SELECT, 0x08},
        {DIALOG7212_MIXOUT_R_SELECT, 0x08},
        {DIALOG7212_AUX_R_CTRL, 0x40},
        {DIALOG7212_MICBIAS_CTRL, 0x19},
        {DIALOG7212_MIC_1_CTRL, 0x84},
        {DIALOG7212_MIC_2_CTRL, 0x04},
        {DIALOG7212_MIXIN_L_CTRL, 0x88},
        {DIALOG7212_MIXIN_R_CTRL, 0x88},
        {DIALOG7212_ADC_L_CTRL, 0xA0},
        {DIALOG7212_GAIN_RAMP_CTRL, 0x02},
        {DIALOG7212_PC_COUNT, 0x02},
        {DIALOG7212_CP_DELAY, 0x95},
    },
    /* DA7212_Input_MIC2 */
    {
        {DIALOG7212_MIXIN_L_SELECT, 0x04},
        {DIALOG7212_MIXIN_R_SELECT, 0x02},
        {DIALOG7212_MIC_2_GAIN, 0x04},
        {DIALOG7212_CP_CTRL, 0xFD},
        {DIALOG7212_AUX_R_CTRL, 0x40},
        {DIALOG7212_MICBIAS_CTRL, 0x91},
        {DIALOG7212_MIC_1_CTRL, 0x08},
        {DIALOG7212_MIC_2_CTRL, 0x84},
        {DIALOG7212_MIXIN_L_CTRL, 0x88},
        {DIALOG7212_MIXIN_R_CTRL, 0x88},
        {DIALOG7212_ADC_L_CTRL, 0xA0},
        {DIALOG7212_GAIN_RAMP_CTRL, 0x02},
        {DIALOG7212_PC_COUNT, 0x02},
        {DIALOG7212_CP_DELAY, 0x95},
    }};

static const da7212_register_value_t kOutputRegisterSequence[kDA7212_Output_MAX][4] = {
    /* DA7212_Output_HP */
    {
        {DIALOG7212_CP_CTRL, 0xF9},
        {DIALOG7212_LINE_CTRL, 0},
        {DIALOG7212_HP_L_CTRL, (DIALOG7212_HP_L_CTRL_AMP_EN_MASK | DIALOG7212_HP_L_CTRL_AMP_RAMP_EN_MASK |
                                DIALOG7212_HP_L_CTRL_AMP_ZC_EN_MASK | DIALOG7212_HP_L_CTRL_AMP_OE_MASK)},
        {DIALOG7212_HP_R_CTRL, (DIALOG7212_HP_R_CTRL_AMP_EN_MASK | DIALOG7212_HP_R_CTRL_AMP_RAMP_EN_MASK |
                                DIALOG7212_HP_R_CTRL_AMP_ZC_EN_MASK | DIALOG7212_HP_R_CTRL_AMP_OE_MASK)},
    },
    /* DA7212_Output_SP */
    {
        {DIALOG7212_CP_CTRL, 0x3D},
        {DIALOG7212_HP_L_CTRL, 0x40},
        {DIALOG7212_HP_R_CTRL, 0x40},
        {DIALOG7212_LINE_CTRL, 0xA8},
    }};

static const da7212_register_value_t kInitRegisterSequence[DA7212_INIT_SIZE] = {
    {
        DIALOG7212_DIG_ROUTING_DAI, 0x10,
    },
    {
        DIALOG7212_SR, DIALOG7212_SR_16KHZ,
    },
    {
        DIALOG7212_REFERENCES, DIALOG7212_REFERENCES_BIAS_EN_MASK,
    },
    {
        DIALOG7212_PLL_FRAC_TOP, CLEAR_REGISTER,
    },
    {
        DIALOG7212_PLL_FRAC_BOT, CLEAR_REGISTER,
    },
    {
        DIALOG7212_PLL_INTEGER, 0x20,
    },
    {
        DIALOG7212_PLL_CTRL, (DIALOG7212_PLL_INDIV_10_20MHZ | DIALOG7212_PLL_EN_MASK),
    },
    {
        DIALOG7212_DAI_CLK_MODE, (DIALOG7212_DAI_BCLKS_PER_WCLK_BCLK64),
    },
    {
        DIALOG7212_DAI_CTRL, (DIALOG7212_DAI_EN_MASK | DIALOG7212_DAI_OE_MASK | DIALOG7212_DAI_WORD_LENGTH_16B |
                              DIALOG7212_DAI_FORMAT_I2S_MODE),
    },
    {
        DIALOG7212_DIG_ROUTING_DAC, (DIALOG7212_DIG_ROUTING_DAC_R_RSC_DAC_R | DIALOG7212_DIG_ROUTING_DAC_L_RSC_DAC_L),
    },
    {
        DIALOG7212_CP_CTRL, (DIALOG7212_CP_CTRL_EN_MASK | DIALOG7212_CP_CTRL_SMALL_SWIT_CH_FREQ_EN_MASK |
                             DIALOG7212_CP_CTRL_MCHANGE_OUTPUT | DIALOG7212_CP_CTRL_MOD_CPVDD_1 |
                             DIALOG7212_CP_CTRL_ANALOG_VLL_LV_BOOSTS_CP),
    },
    {
        DIALOG7212_MIXOUT_L_SELECT, (DIALOG7212_MIXOUT_L_SELECT_DAC_L_MASK),
    },
    {
        DIALOG7212_MIXOUT_R_SELECT, (DIALOG7212_MIXOUT_R_SELECT_DAC_R_MASK),
    },
    {
        DIALOG7212_DAC_L_CTRL, (DIALOG7212_DAC_L_CTRL_ADC_EN_MASK | DIALOG7212_DAC_L_CTRL_ADC_RAMP_EN_MASK),
    },
    {
        DIALOG7212_DAC_R_CTRL, (DIALOG7212_DAC_R_CTRL_ADC_EN_MASK | DIALOG7212_DAC_R_CTRL_ADC_RAMP_EN_MASK),
    },
    {
        DIALOG7212_HP_L_CTRL, (DIALOG7212_HP_L_CTRL_AMP_EN_MASK | DIALOG7212_HP_L_CTRL_AMP_RAMP_EN_MASK |
                               DIALOG7212_HP_L_CTRL_AMP_ZC_EN_MASK | DIALOG7212_HP_L_CTRL_AMP_OE_MASK),
    },
    {
        DIALOG7212_HP_R_CTRL, (DIALOG7212_HP_R_CTRL_AMP_EN_MASK | DIALOG7212_HP_R_CTRL_AMP_RAMP_EN_MASK |
                               DIALOG7212_HP_R_CTRL_AMP_ZC_EN_MASK | DIALOG7212_HP_R_CTRL_AMP_OE_MASK),
    },
    {
        DIALOG7212_MIXOUT_L_CTRL,
        (DIALOG7212_MIXOUT_L_CTRL_AMP_EN_MASK | DIALOG7212_MIXOUT_L_CTRL_AMP_SOFT_MIX_EN_MASK |
         DIALOG7212_MIXOUT_L_CTRL_AMP_MIX_EN_MASK),
    },
    {
        DIALOG7212_MIXOUT_R_CTRL,
        (DIALOG7212_MIXOUT_R_CTRL_AMP_EN_MASK | DIALOG7212_MIXOUT_R_CTRL_AMP_SOFT_MIX_EN_MASK |
         DIALOG7212_MIXOUT_R_CTRL_AMP_MIX_EN_MASK),
    },
    {
        DIALOG7212_CP_VOL_THRESHOLD1, (DIALOG7212_CP_VOL_THRESHOLD1_VDD2(0x32)),
    },
    {
        DIALOG7212_SYSTEM_STATUS, CLEAR_REGISTER,
    },
    {
        DIALOG7212_DAC_L_GAIN, kDA7212_DACGainM6DB,
    },
    {
        DIALOG7212_DAC_R_GAIN, kDA7212_DACGainM6DB,
    },
    {
        DIALOG7212_MIXIN_L_SELECT, DIALOG7212_MIXIN_L_SELECT_AUX_L_SEL_MASK,
    },
    {
        DIALOG7212_MIXIN_R_SELECT, DIALOG7212_MIXIN_R_SELECT_AUX_R_SEL_MASK,
    },
    {
        DIALOG7212_MIXIN_L_GAIN, DIALOG7212_MIXIN_L_AMP_GAIN(0x03),
    },
    {
        DIALOG7212_MIXIN_R_GAIN, DIALOG7212_MIXIN_R_AMP_GAIN(0x03),
    },
    {
        DIALOG7212_ADC_L_GAIN, DIALOG7212_ADC_L_DIGITAL_GAIN(0x6F),
    },
    {
        DIALOG7212_ADC_R_GAIN, DIALOG7212_ADC_R_DIGITAL_GAIN(0x6F),
    },
    {
        DIALOG7212_AUX_L_CTRL, DIALOG7212_AUX_L_CTRL_AMP_EN_MASK | DIALOG7212_AUX_L_CTRL_AMP_RAMP_EN_MASK |
                                   DIALOG7212_AUX_L_CTRL_AMP_ZC_EN_MASK,
    },
    {
        DIALOG7212_AUX_R_CTRL, DIALOG7212_AUX_R_CTRL_AMP_EN_MASK | DIALOG7212_AUX_R_CTRL_AMP_RAMP_EN_MASK |
                                   DIALOG7212_AUX_R_CTRL_AMP_ZC_EN_MASK,
    },
    {
        DIALOG7212_MIXIN_L_CTRL, DIALOG7212_MIXIN_L_CTRL_AMP_EN_MASK | DIALOG7212_MIXIN_L_CTRL_AMP_MIX_EN_MASK,
    },
    {
        DIALOG7212_MIXIN_R_CTRL, DIALOG7212_MIXIN_R_CTRL_AMP_EN_MASK | DIALOG7212_MIXIN_R_CTRL_AMP_MIX_EN_MASK,
    },
    {
        DIALOG7212_ADC_L_CTRL, DIALOG7212_ADC_L_CTRL_ADC_EN_MASK | DIALOG7212_ADC_L_CTRL_ADC_RAMP_EN_MASK,
    },
    {
        DIALOG7212_ADC_R_CTRL, DIALOG7212_ADC_R_CTRL_ADC_EN_MASK | DIALOG7212_ADC_R_CTRL_ADC_RAMP_EN_MASK,
    },
};

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t DA7212_WriteRegister(codec_handle_t *handle, uint8_t u8Register, uint8_t u8RegisterData)
{
    status_t status = kStatus_Success;

    status = CODEC_I2C_WriteReg(handle->slaveAddress, kCODEC_RegAddr8Bit, u8Register, kCODEC_RegWidth8Bit,
                                u8RegisterData, handle->I2C_SendFunc);

    return status;
}

status_t DA7212_ReadRegister(codec_handle_t *handle, uint8_t u8Register, uint8_t *pu8RegisterData)
{
    status_t status = kStatus_Success;

    status = CODEC_I2C_ReadReg(handle->slaveAddress, kCODEC_RegAddr8Bit, u8Register, kCODEC_RegWidth8Bit,
                               pu8RegisterData, handle->I2C_ReceiveFunc);

    return status;
}

status_t DA7212_Init(codec_handle_t *handle, void *codecConfig)
{
    uint32_t i = 0;
    da7212_config_t *config = (da7212_config_t *)codecConfig;

    handle->slaveAddress = DA7212_ADDRESS;

    /* If no config structure, use default settings */
    for (i = 0; i < DA7212_INIT_SIZE; i++)
    {
        DA7212_WriteRegister(handle, kInitRegisterSequence[i].addr, kInitRegisterSequence[i].value);
    }

    if (config != NULL)
    {
        /* Set to be master or slave */
        DA7212_WriteRegister(handle, DIALOG7212_DAI_CLK_MODE,
                             (DIALOG7212_DAI_BCLKS_PER_WCLK_BCLK64 | (config->isMaster << 7U)));

        /* Set the audio protocol */
        DA7212_WriteRegister(handle, DIALOG7212_DAI_CTRL, (DIALOG7212_DAI_EN_MASK | config->protocol));

        /* Set DA7212 functionality */
        if (config->dacSource == kDA7212_DACSourceADC)
        {
            DA7212_WriteRegister(handle, DIALOG7212_DIG_ROUTING_DAC, 0x10);
        }
        else
        {
            DA7212_WriteRegister(handle, DIALOG7212_DIG_ROUTING_DAC, 0x32);
        }
    }

    return kStatus_Success;
}

status_t DA7212_ConfigAudioFormat(codec_handle_t *handle,
                                  uint32_t masterClock_Hz,
                                  uint32_t sampleRate_Hz,
                                  uint32_t dataBits)
{
    uint32_t sysClock_Hz = 0;
    uint8_t indiv = 0, inputDiv = 0, regVal = 0;
    uint64_t PllValue = 0;
    uint32_t PllFractional;
    uint8_t PllInteger;
    uint8_t PllFracTop;
    uint8_t PllFracBottom;

    switch (sampleRate_Hz)
    {
        case 8000:
            DA7212_WriteRegister(handle, DIALOG7212_SR, DIALOG7212_SR_8KHZ);
            break;
        case 11025:
            DA7212_WriteRegister(handle, DIALOG7212_SR, DIALOG7212_SR_11_025KHZ);
            break;
        case 12000:
            DA7212_WriteRegister(handle, DIALOG7212_SR, DIALOG7212_SR_12KHZ);
            break;
        case 16000:
            DA7212_WriteRegister(handle, DIALOG7212_SR, DIALOG7212_SR_16KHZ);
            break;
        case 22050:
            DA7212_WriteRegister(handle, DIALOG7212_SR, DIALOG7212_SR_22KHZ);
            break;
        case 24000:
            DA7212_WriteRegister(handle, DIALOG7212_SR, DIALOG7212_SR_24KHZ);
            break;
        case 32000:
            DA7212_WriteRegister(handle, DIALOG7212_SR, DIALOG7212_SR_32KHZ);
            break;
        case 44100:
            DA7212_WriteRegister(handle, DIALOG7212_SR, DIALOG7212_SR_44_1KHZ);
            break;
        case 48000:
            DA7212_WriteRegister(handle, DIALOG7212_SR, DIALOG7212_SR_48KHZ);
            break;
        case 88200:
            DA7212_WriteRegister(handle, DIALOG7212_SR, DIALOG7212_SR_88_2KHZ);
            break;
        case 96000:
            DA7212_WriteRegister(handle, DIALOG7212_SR, DIALOG7212_SR_96KHZ);
            break;
        default:
            break;
    }

    /* Set data bits of word */
    DA7212_ReadRegister(handle, DIALOG7212_DAI_CTRL, &regVal);
    regVal &= ~DIALOG7212_DAI_WORD_LENGTH_MASK;
    switch (dataBits)
    {
        case 16:
            regVal |= DIALOG7212_DAI_WORD_LENGTH_16B;
            break;
        case 20:
            regVal |= DIALOG7212_DAI_WORD_LENGTH_20B;
            break;
        case 24:
            regVal |= DIALOG7212_DAI_WORD_LENGTH_24B;
            break;
        case 32:
            regVal |= DIALOG7212_DAI_WORD_LENGTH_32B;
            break;
        default:
            break;
    }
    DA7212_WriteRegister(handle, DIALOG7212_DAI_CTRL, regVal);

    /* Set PLL clock settings */
    if ((sampleRate_Hz == 8000) || (sampleRate_Hz == 16000) || (sampleRate_Hz == 24000) || (sampleRate_Hz == 32000) ||
        (sampleRate_Hz == 48000) || (sampleRate_Hz == 96000))
    {
        sysClock_Hz = 12288000;
    }
    else
    {
        sysClock_Hz = 11289600;
    }

    /* Compute the PLL_INDIV and DIV value for sysClock */
    if ((masterClock_Hz > 2000000) && (masterClock_Hz <= 10000000))
    {
        indiv = DIALOG7212_PLL_INDIV_2_10MHZ;
        inputDiv = 2;
    }
    else if ((masterClock_Hz > 10000000) && (masterClock_Hz <= 20000000))
    {
        indiv = DIALOG7212_PLL_INDIV_10_20MHZ;
        inputDiv = 4;
    }
    else if ((masterClock_Hz > 20000000) && (masterClock_Hz <= 40000000))
    {
        indiv = DIALOG7212_PLL_INDIV_20_40MHZ;
        inputDiv = 8;
    }
    else
    {
        indiv = DIALOG7212_PLL_INDIV_40_80MHZ;
        inputDiv = 16;
    }

    /* PLL feedback divider is a Q13 value */
    PllValue = (uint64_t)(((uint64_t)((((uint64_t)sysClock_Hz * 8) * inputDiv) << 13)) / (masterClock_Hz));

    /* extract integer and fractional */
    PllInteger = PllValue >> 13;
    PllFractional = (PllValue - (PllInteger << 13));
    PllFracTop = (PllFractional >> 8);
    PllFracBottom = (PllFractional & 0xFF);

    DA7212_WriteRegister(handle, DIALOG7212_PLL_FRAC_TOP, PllFracTop);

    DA7212_WriteRegister(handle, DIALOG7212_PLL_FRAC_BOT, PllFracBottom);

    DA7212_WriteRegister(handle, DIALOG7212_PLL_INTEGER, PllInteger);

    regVal = DIALOG7212_PLL_EN_MASK | indiv;

    DA7212_WriteRegister(handle, DIALOG7212_PLL_CTRL, regVal);

    return kStatus_Success;
}

void DA7212_ChangeInput(codec_handle_t *handle, da7212_Input_t DA7212_Input)
{
    uint32_t i = 0;
    uint32_t seqSize = sizeof(kInputRegisterSequence[DA7212_Input]) / sizeof(da7212_register_value_t);

    for (i = 0; i < seqSize; i++)
    {
        DA7212_WriteRegister(handle, kInputRegisterSequence[DA7212_Input][i].addr,
                             kInputRegisterSequence[DA7212_Input][i].value);
    }
}

void DA7212_ChangeOutput(codec_handle_t *handle, da7212_Output_t DA7212_Output)
{
    uint32_t i = 0;
    uint32_t seqSize = sizeof(kOutputRegisterSequence[DA7212_Output]) / sizeof(da7212_register_value_t);

    for (i = 0; i < seqSize; i++)
    {
        DA7212_WriteRegister(handle, kOutputRegisterSequence[DA7212_Output][i].addr,
                             kOutputRegisterSequence[DA7212_Output][i].value);
    }
}

void DA7212_ChangeHPVolume(codec_handle_t *handle, da7212_volume_t volume)
{
    DA7212_WriteRegister(handle, DIALOG7212_DAC_L_GAIN, volume);
    DA7212_WriteRegister(handle, DIALOG7212_DAC_R_GAIN, volume);
}

void DA7212_Mute(codec_handle_t *handle, bool isMuted)
{
    uint8_t val = 0;

    if (isMuted)
    {
        val = DA7212_DAC_MUTE_ENABLED;
    }
    else
    {
        val = DA7212_DAC_MUTE_DISABLED;
    }

    DA7212_WriteRegister(handle, DIALOG7212_DAC_L_CTRL, val);
    DA7212_WriteRegister(handle, DIALOG7212_DAC_R_CTRL, val);
}

status_t DA7212_Deinit(codec_handle_t *handle)
{
    return kStatus_Success;
}
