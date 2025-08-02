/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "a-format.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
static bool is_MultiTrans;
static uint8_t cmd, cmdErr, crc, nEncoder;
static uint16_t crc_data, cdf;
static a_format_crc_para_t crc3_para = {
    .message       = (uint8_t *)(&crc_data),
    .type          = A_FORMAT_CRC3,
    .message_len   = 2,
    .polynomial    = A_FORMAT_CRC_POLY_COMMAND_DATA,
    .inputBitSwap  = true,
    .outputBitSwap = true
};
static a_format_crc_para_t crc8_para = {
    .type          = A_FORMAT_CRC8,
    .polynomial    = A_FORMAT_CRC_POLY_ENCODER_DATA,
    .inputBitSwap  = true,
    .outputBitSwap = true
};
static a_format_res2_t res2[8];
static a_format_res3_t res3[8];

/*******************************************************************************
 * Codes
 ******************************************************************************/
static uint8_t Swap_Byte(uint8_t data)
{
    data = (data << 4) | (data >> 4);
    data = ((data << 2) & 0xCC) | ((data >> 2) & 0x33);
    data = ((data << 1) & 0xAA) | ((data >> 1) & 0x55);
    return data;
}

static uint8_t CRC_Calc(a_format_crc_para_t *crc)
{
    uint8_t remainder = 0;
    uint8_t i = 0, j = 0;
    uint8_t poly = crc->polynomial << (8 - crc->type);

    for (j = 0; j < crc->message_len; j++)
    {
        remainder ^= (crc->inputBitSwap ? Swap_Byte(crc->message[j]) : crc->message[j]);

        for (i = 0; i < 8; i++)
        {
            if (remainder & 0x80)
            {
                remainder = (remainder << 1) ^ poly;
            }
            else
            {
                remainder <<= 1;
            }
        }
    }

    return crc->outputBitSwap ? Swap_Byte(remainder) : (remainder >> (8 - crc->type));
}

void A_Format_PrintfES(logFunc logES, uint8_t es)
{
    if (es == kFLEXIO_A_FORMAT_ES_NoErr)
    {
        logES("No error in the ES field\r\n");
    }
    else
    {
        for (uint8_t i = 0; i < 6; i++)
        {
            switch (es & (0x1 << i))
            {
            case kFLEXIO_A_FORMAT_ES_Busy_MemBusy:
                logES("Encoder or memory is busy!\r\n");
                break;
            case kFLEXIO_A_FORMAT_ES_Batt:
                logES("The battery is error!\r\n");
                break;
            case kFLEXIO_A_FORMAT_ES_OvSpd_MemErr_OvTemp_OvFlow:
                logES("Over speed or memory error or emperature warning or Over Flow!\r\n");
                break;
            case kFLEXIO_A_FORMAT_ES_STErr_PSErr_MTErr_INCErr:
                logES("ST error or PS error or MT error or incremental signal Error\r\n");
                break;
            case kFLEXIO_A_FORMAT_ES_FrameErr:
                logES("Encoder frame is error!\r\n");
                break;
            case kFLEXIO_A_FORMAT_ES_Anyone:
                logES("One or more of all errors!\r\n");
                break;
            default:
                break;
            }
        }
    }
}

status_t A_Format_Set_Encoder_Address_1to1(encoder_a_format_t *enc, uint8_t enc_addr)
{
    status_t status = kStatus_Success;
    a_format_res2_t res;

    cmd = A_FORMAT_REQ_IT_SET_ENCODER_ADDR1;
    cdf = A_FORMAT_PACK_CDF((enc_addr & 0x7), A_FORMAT_REQ_IT_SET_ENCODER_ADDR1, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);

    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);

    crc8_para.message_len = 6;
    crc8_para.message     = (uint8_t const *)&res;

    for (uint8_t i = 0; i < 8; i++)
    {
        FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
        FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)&res, HALFWORD_NUM(a_format_res2_t));

        if ((A_FORMAT_GET_CMD_CODE_IF(res.IF) != cmd) || (CRC_Calc(&crc8_para) != 0))
        {
            return kStatus_FLEXIO_A_FORMAT_FrameErr;
        }
    }
    return status;
}

status_t A_Format_Set_ID_1to1(encoder_a_format_t *enc, uint32_t id)
{
    a_format_res2_t res;
    uint16_t mdf[3] = {
        A_FORMAT_PACK_MDF(A_FORMAT_FRAME_CODE_MDF0, id & 0x000000FF, 0),
        A_FORMAT_PACK_MDF(A_FORMAT_FRAME_CODE_MDF1, (id & 0x0000FF00) >> 8, 0),
        A_FORMAT_PACK_MDF(A_FORMAT_FRAME_CODE_MDF2, (id & 0x00FF0000) >> 16, 0)
    };

    cmd = A_FORMAT_REQ_IT_ID_CODE_WRITE2;
    cdf = A_FORMAT_PACK_CDF(0, A_FORMAT_REQ_IT_ID_CODE_WRITE2, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);
    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    for (uint8_t i = 0; i < 3; i++)
    {
        crc_data = A_FORMAT_GET_CRC_DATA_MDF(mdf[i]);
        crc = CRC_Calc(&crc3_para);
        mdf[i] = A_FORMAT_SET_CRC_CODE_MDF(mdf[i], crc);
    }

    FLEXIO_A_Format_Config_DR_length(enc->controller, 4);

    crc8_para.message_len = 6;
    crc8_para.message     = (uint8_t const *)&res;

    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
    SDK_DelayAtLeastUs(7, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    FLEXIO_A_Format_WriteBlocking(enc->controller, mdf, 1);
    SDK_DelayAtLeastUs(7, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &mdf[1], 1);
    SDK_DelayAtLeastUs(7, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &mdf[2], 1);
    FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)&res, HALFWORD_NUM(a_format_res2_t));

    if ((A_FORMAT_GET_CMD_CODE_IF(res.IF) != cmd) || (CRC_Calc(&crc8_para) != 0) ||
        (GET_ENCODER_ID(*(uint32_t *)res.DF) != id))
    {
        return kStatus_FLEXIO_A_FORMAT_FrameErr;
    }

    return kStatus_Success;
}

status_t A_Format_Get_ID_1to1(encoder_a_format_t *enc, uint32_t *id)
{
    a_format_res2_t res;

    cmd = A_FORMAT_REQ_IT_ID_CODE_READ2;
    cdf = A_FORMAT_PACK_CDF(0, A_FORMAT_REQ_IT_ID_CODE_READ2, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);
    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);

    crc8_para.message_len = 6;
    crc8_para.message     = (uint8_t const *)&res;

    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
    FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)&res, HALFWORD_NUM(a_format_res2_t));

    if ((A_FORMAT_GET_CMD_CODE_IF(res.IF) != cmd) || (CRC_Calc(&crc8_para) != 0))
    {
        return kStatus_FLEXIO_A_FORMAT_FrameErr;
    }

    *id = GET_ENCODER_ID(*(uint32_t *)res.DF);
    return kStatus_Success;
}

status_t A_Format_Set_Encoder_Address_MATCH_ID(encoder_a_format_t *enc, uint32_t id, uint8_t enc_addr)
{
    a_format_res2_t res;
    uint16_t mdf[3] = {
        A_FORMAT_PACK_MDF(A_FORMAT_FRAME_CODE_MDF0, id & 0x000000FF, 0),
        A_FORMAT_PACK_MDF(A_FORMAT_FRAME_CODE_MDF1, (id & 0x0000FF00) >> 8, 0),
        A_FORMAT_PACK_MDF(A_FORMAT_FRAME_CODE_MDF2, (id & 0x00FF0000) >> 16, 0)
    };

    cmd = A_FORMAT_REQ_IT_SET_ENCODER_ADDR2;
    cdf = A_FORMAT_PACK_CDF((enc_addr & 0x7), A_FORMAT_REQ_IT_SET_ENCODER_ADDR2, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);
    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    for (uint8_t i = 0; i < 3; i++)
    {
        crc_data = A_FORMAT_GET_CRC_DATA_MDF(mdf[i]);
        crc = CRC_Calc(&crc3_para);
        mdf[i] = A_FORMAT_SET_CRC_CODE_MDF(mdf[i], crc);
    }

    FLEXIO_A_Format_Config_DR_length(enc->controller, 4);

    crc8_para.message_len = 6;
    crc8_para.message     = (uint8_t const *)&res;

    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
    SDK_DelayAtLeastUs(7, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    FLEXIO_A_Format_WriteBlocking(enc->controller, mdf, 1);
    SDK_DelayAtLeastUs(7, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &mdf[1], 1);
    SDK_DelayAtLeastUs(7, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &mdf[2], 1);
    FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)&res, HALFWORD_NUM(a_format_res2_t));

    if ((A_FORMAT_GET_CMD_CODE_IF(res.IF) != cmd) || (CRC_Calc(&crc8_para) != 0) ||
        (GET_ENCODER_ID(*(uint32_t *)res.DF) != id))
    {
        return kStatus_FLEXIO_A_FORMAT_FrameErr;
    }

    return kStatus_Success;
}

status_t A_Format_Set_ID(encoder_a_format_t *enc, uint8_t enc_addr, uint32_t id)
{
    a_format_res2_t res;
    uint16_t mdf[3] = {
        A_FORMAT_PACK_MDF(A_FORMAT_FRAME_CODE_MDF0, id & 0x000000FF, 0),
        A_FORMAT_PACK_MDF(A_FORMAT_FRAME_CODE_MDF1, (id & 0x0000FF00) >> 8, 0),
        A_FORMAT_PACK_MDF(A_FORMAT_FRAME_CODE_MDF2, (id & 0x00FF0000) >> 16, 0)
    };

    cmd = A_FORMAT_REQ_IT_ID_CODE_WRITE1;
    cdf = A_FORMAT_PACK_CDF((enc_addr & 0x7), A_FORMAT_REQ_IT_ID_CODE_WRITE1, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);
    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    for (uint8_t i = 0; i < 3; i++)
    {
        crc_data = A_FORMAT_GET_CRC_DATA_MDF(mdf[i]);
        crc = CRC_Calc(&crc3_para);
        mdf[i] = A_FORMAT_SET_CRC_CODE_MDF(mdf[i], crc);
    }

    FLEXIO_A_Format_Config_DR_length(enc->controller, 4);

    crc8_para.message_len = 6;
    crc8_para.message     = (uint8_t const *)&res;

    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
    SDK_DelayAtLeastUs(7, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    FLEXIO_A_Format_WriteBlocking(enc->controller, mdf, 1);
    SDK_DelayAtLeastUs(7, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &mdf[1], 1);
    SDK_DelayAtLeastUs(7, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &mdf[2], 1);
    FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)&res, HALFWORD_NUM(a_format_res2_t));

    if ((A_FORMAT_GET_CMD_CODE_IF(res.IF) != cmd) || (CRC_Calc(&crc8_para) != 0) ||
        (GET_ENCODER_ID(*(uint32_t *)res.DF) != id))
    {
        return kStatus_FLEXIO_A_FORMAT_FrameErr;
    }

    return kStatus_Success;
}


status_t A_Format_Get_ID_CMD(uint8_t enc_addr)
{
    if (ENCODER_ADDRESS(enc_addr) >= A_FORMAT_ENCODER_MAX_NUM)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    nEncoder = 1;
    enc_addr &= 0x7;
    cmd = A_FORMAT_REQ_IT_ID_CODE_READ1;
    cdf = A_FORMAT_PACK_CDF(enc_addr, A_FORMAT_REQ_IT_ID_CODE_READ1, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);
    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    memset(res2, 0, sizeof(a_format_res2_t));

    return kStatus_Success;
}

status_t A_Format_Get_ID_Parse(encoder_a_format_t *enc, a_format_res2_t *res, uint32_t *id)
{
    crc8_para.message_len = 6;
    crc8_para.message     = (uint8_t const *)res;
    if ((A_FORMAT_GET_CMD_CODE_IF(res->IF) != cmd) || (CRC_Calc(&crc8_para) != 0))
    {
        return kStatus_FLEXIO_A_FORMAT_FrameErr;
    }

    *id = GET_ENCODER_ID(*(uint32_t *)res->DF);
    return kStatus_Success;
}

status_t A_Format_Get_ID(encoder_a_format_t *enc, uint8_t enc_addr, uint32_t *id)
{
    if (A_Format_Get_ID_CMD(enc_addr) == kStatus_FLEXIO_A_FORMAT_OutOfIDRange)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);

    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
    FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)res2, HALFWORD_NUM(a_format_res2_t));

    return A_Format_Get_ID_Parse(enc, res2, id);
}

status_t A_Format_Memory_Write(encoder_a_format_t *enc, uint8_t enc_addr, a_format_eeprom_t *eeprom)
{
    a_format_res2_t res;
    uint16_t mdf[3] = {
        A_FORMAT_PACK_MDF(A_FORMAT_FRAME_CODE_MDF0, eeprom->data & 0x00FF, 0),
        A_FORMAT_PACK_MDF(A_FORMAT_FRAME_CODE_MDF1, (eeprom->data & 0xFF00) >> 8, 0),
        A_FORMAT_PACK_MDF(A_FORMAT_FRAME_CODE_MDF2, eeprom->address, 0)
    };

    cmd = A_FORMAT_REQ_IT_MEMORY_WRITE;
    cdf = A_FORMAT_PACK_CDF((enc_addr & 0x7), A_FORMAT_REQ_IT_MEMORY_WRITE, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);
    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    for (uint8_t i = 0; i < 3; i++)
    {
        crc_data = A_FORMAT_GET_CRC_DATA_MDF(mdf[i]);
        crc = CRC_Calc(&crc3_para);
        mdf[i] = A_FORMAT_SET_CRC_CODE_MDF(mdf[i], crc);
    }

    FLEXIO_A_Format_Config_DR_length(enc->controller, 4);

    crc8_para.message_len = 6;
    crc8_para.message     = (uint8_t const *)&res;

    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
    SDK_DelayAtLeastUs(7, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    FLEXIO_A_Format_WriteBlocking(enc->controller, mdf, 1);
    SDK_DelayAtLeastUs(7, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &mdf[1], 1);
    SDK_DelayAtLeastUs(7, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &mdf[2], 1);
    FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)&res, HALFWORD_NUM(a_format_res2_t));

    if ((A_FORMAT_GET_CMD_CODE_IF(res.IF) != cmd) || (CRC_Calc(&crc8_para) != 0) ||
        (res.DF[0] != eeprom->data) || ((res.DF[1] & 0x00FF) != eeprom->address))
    {
        return kStatus_FLEXIO_A_FORMAT_FrameErr;
    }

    return kStatus_Success;
}

status_t A_Format_Memory_Read(encoder_a_format_t *enc, uint8_t enc_addr, a_format_eeprom_t *eeprom)
{
    a_format_res2_t res;
    uint16_t mdf = A_FORMAT_PACK_MDF(A_FORMAT_FRAME_CODE_MDF2, eeprom->address, 0);

    cmd = A_FORMAT_REQ_IT_MEMORY_READ;
    cdf = A_FORMAT_PACK_CDF((enc_addr & 0x7), A_FORMAT_REQ_IT_MEMORY_READ, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);
    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    crc_data = A_FORMAT_GET_CRC_DATA_MDF(mdf);
    crc = CRC_Calc(&crc3_para);
    mdf = A_FORMAT_SET_CRC_CODE_MDF(mdf, crc);

    FLEXIO_A_Format_Config_DR_length(enc->controller, 2);

    crc8_para.message_len = 6;
    crc8_para.message     = (uint8_t const *)&res;

    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
    SDK_DelayAtLeastUs(7, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &mdf, 1);
    FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)&res, HALFWORD_NUM(a_format_res2_t));

    if ((A_FORMAT_GET_CMD_CODE_IF(res.IF) != cmd) || (CRC_Calc(&crc8_para) != 0) ||
        ((res.DF[1] & 0x00FF) != eeprom->address))
    {
        return kStatus_FLEXIO_A_FORMAT_FrameErr;
    }

    eeprom->data = res.DF[0];
    return kStatus_Success;
}

status_t A_Format_Readout_Encoder_status_CMD(uint8_t enc_addr)
{
    if (ENCODER_ADDRESS(enc_addr) >= A_FORMAT_ENCODER_MAX_NUM)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    is_MultiTrans = ENCODER_ADDRESS_IS_MT(enc_addr);
    enc_addr &= 0x7;

    cmd = is_MultiTrans ? A_FORMAT_REQ_MT_ENCODER_STAT : A_FORMAT_REQ_IT_ENCODER_STAT;
    cdf = A_FORMAT_PACK_CDF(enc_addr, cmd, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);
    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    nEncoder = is_MultiTrans ? (enc_addr + 1) : 1;
    memset(res2, 0, sizeof(a_format_res2_t) * nEncoder);

    return kStatus_Success;
}

status_t A_Format_Readout_Encoder_status_Parse(encoder_a_format_t *enc, a_format_res2_t *res,
                                               a_format_status_t *statusData)
{
    cmdErr = 0;

    crc8_para.message_len = 6;
    for (uint8_t i = 0; i < nEncoder; i++)
    {
        crc8_para.message = (uint8_t const *)&res[i];
        if ((A_FORMAT_GET_CMD_CODE_IF(res[i].IF) != cmd) || (CRC_Calc(&crc8_para) != 0))
        {
            cmdErr++;
            statusData[i].es = kFLEXIO_A_FORMAT_ES_FrameErr;
            continue;
        }

        statusData[i].encID = A_FORMAT_GET_ENC_ADDR_IF(res[i].IF);
        statusData[i].es    = A_FORMAT_GET_ENC_STAT_IF(res[i].IF);
        if (statusData[i].es != kFLEXIO_A_FORMAT_ES_NoErr)
        {
            cmdErr++;
        }

        statusData[i].status = res[i].DF[0];
    }

    return cmdErr ? kStatus_Fail : kStatus_Success;
}

status_t A_Format_Readout_Encoder_status(encoder_a_format_t *enc, uint8_t enc_addr,
                                         a_format_status_t *statusData)
{
    if (A_Format_Readout_Encoder_status_CMD(enc_addr) == kStatus_FLEXIO_A_FORMAT_OutOfIDRange)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
    FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)res2,
                                 HALFWORD_NUM(a_format_res2_t) * nEncoder);

    return A_Format_Readout_Encoder_status_Parse(enc, res2, statusData);
}

status_t A_Format_Get_Temperature_CMD(uint8_t enc_addr)
{
    if (ENCODER_ADDRESS(enc_addr) >= A_FORMAT_ENCODER_MAX_NUM)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    nEncoder = 1;
    enc_addr &= 0x7;
    cmd = A_FORMAT_REQ_IT_TEMPERATURE_10BIT;
    cdf = A_FORMAT_PACK_CDF(enc_addr, A_FORMAT_REQ_IT_TEMPERATURE_10BIT, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);
    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    memset(res2, 0, sizeof(a_format_res2_t));

    return kStatus_Success;
}

status_t A_Format_Get_Temperature_Parse(encoder_a_format_t *enc, a_format_res2_t *res, float *temp)
{
    crc8_para.message_len = 6;
    crc8_para.message     = (uint8_t const *)res;
    if ((A_FORMAT_GET_CMD_CODE_IF(res->IF) != cmd) || (CRC_Calc(&crc8_para) != 0))
    {
        return kStatus_FLEXIO_A_FORMAT_FrameErr;
    }

    *temp = GET_TEMPERATURE_VALUE(res->DF[0]);
    return kStatus_Success;
}

status_t A_Format_Get_Temperature(encoder_a_format_t *enc, uint8_t enc_addr, float *temp)
{
    if (A_Format_Get_Temperature_CMD(enc_addr) == kStatus_FLEXIO_A_FORMAT_OutOfIDRange)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }
    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);

    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
    FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)res2, HALFWORD_NUM(a_format_res2_t));

    return A_Format_Get_Temperature_Parse(enc, res2, temp);
}

status_t A_Format_ABS_Readout_Multi_CMD(uint8_t enc_addr)
{
    if (ENCODER_ADDRESS(enc_addr) >= A_FORMAT_ENCODER_MAX_NUM)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    is_MultiTrans = ENCODER_ADDRESS_IS_MT(enc_addr);
    enc_addr &= 0x7;

    cmd = is_MultiTrans ? A_FORMAT_REQ_MT_ABS_UPPER_24BIT : A_FORMAT_REQ_IT_ABS_UPPER_24BIT;
    cdf = A_FORMAT_PACK_CDF(enc_addr, cmd, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);
    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    nEncoder = is_MultiTrans ? (enc_addr + 1) : 1;
    memset(res2, 0, sizeof(a_format_res2_t) * nEncoder);

    return kStatus_Success;
}

status_t A_Format_ABS_Readout_Multi_Parse(encoder_a_format_t *enc, a_format_res2_t *res,
                                          a_format_abs_multi_t *multiData)
{
    cmdErr = 0;

    crc8_para.message_len = 6;
    for (uint8_t i = 0; i < nEncoder; i++)
    {
        crc8_para.message = (uint8_t const *)&res[i];
        if ((A_FORMAT_GET_CMD_CODE_IF(res[i].IF) != cmd) || (CRC_Calc(&crc8_para) != 0))
        {
            cmdErr++;
            multiData[i].es = kFLEXIO_A_FORMAT_ES_FrameErr;
            continue;
        }

        multiData[i].encID = A_FORMAT_GET_ENC_ADDR_IF(res[i].IF);
        multiData[i].es    = A_FORMAT_GET_ENC_STAT_IF(res[i].IF);
        if (multiData[i].es != kFLEXIO_A_FORMAT_ES_NoErr)
        {
            cmdErr++;
        }

        multiData[i].multiTurn = (uint16_t)((*(uint32_t *)res[i].DF >> (enc->singleTurnRevolution - 16)) & enc->multi_turn_sign_mask);
    }

    return cmdErr ? kStatus_Fail : kStatus_Success;
}

status_t A_Format_ABS_Readout_Multi(encoder_a_format_t *enc, uint8_t enc_addr, a_format_abs_multi_t *multiData)
{
    if (A_Format_ABS_Readout_Multi_CMD(enc_addr) == kStatus_FLEXIO_A_FORMAT_OutOfIDRange)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
    FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)res2,
                                 HALFWORD_NUM(a_format_res2_t) * nEncoder);

    return A_Format_ABS_Readout_Multi_Parse(enc, res2, multiData);
}

status_t A_Format_ABS_Readout_Single_CMD(uint8_t enc_addr)
{
    if (ENCODER_ADDRESS(enc_addr) >= A_FORMAT_ENCODER_MAX_NUM)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    is_MultiTrans = ENCODER_ADDRESS_IS_MT(enc_addr);
    enc_addr &= 0x7;

    cmd = is_MultiTrans ? A_FORMAT_REQ_MT_ABS_LOWER_24BIT : A_FORMAT_REQ_IT_ABS_LOWER_24BIT;
    cdf = A_FORMAT_PACK_CDF(enc_addr, cmd, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);
    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    nEncoder = is_MultiTrans ? (enc_addr + 1) : 1;
    memset(res2, 0, sizeof(a_format_res2_t) * nEncoder);

    return kStatus_Success;
}

status_t A_Format_ABS_Readout_Single_Parse(encoder_a_format_t *enc, a_format_res2_t *res,
                                           a_format_abs_single_t *single_data)
{
    cmdErr = 0;

    crc8_para.message_len = 6;
    for (uint8_t i = 0; i < nEncoder; i++)
    {
        crc8_para.message = (uint8_t const *)&res[i];
        if ((A_FORMAT_GET_CMD_CODE_IF(res[i].IF) != cmd) || (CRC_Calc(&crc8_para) != 0))
        {
            cmdErr++;
            single_data[i].es = kFLEXIO_A_FORMAT_ES_FrameErr;
            continue;
        }

        single_data[i].encID = A_FORMAT_GET_ENC_ADDR_IF(res[i].IF);
        single_data[i].es    = A_FORMAT_GET_ENC_STAT_IF(res[i].IF);
        if (single_data[i].es != kFLEXIO_A_FORMAT_ES_NoErr)
        {
            cmdErr++;
        }

        single_data[i].singleTurn = *(uint32_t *)res[i].DF & enc->single_turn_sign_mask;
    }

    return cmdErr ? kStatus_Fail : kStatus_Success;
}

status_t A_Format_ABS_Readout_Single(encoder_a_format_t *enc, uint8_t enc_addr, a_format_abs_single_t *singleData)
{
    if (A_Format_ABS_Readout_Single_CMD(enc_addr) == kStatus_FLEXIO_A_FORMAT_OutOfIDRange)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
    FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)res2,
                                 HALFWORD_NUM(a_format_res2_t) * nEncoder);

    return A_Format_ABS_Readout_Single_Parse(enc, res2, singleData);
}

status_t A_Format_ABS_Readout_Single_with_status(encoder_a_format_t *enc, uint8_t enc_addr, a_format_single_stat_t *singleStat)
{
    a_format_res3_t *res = res3;

    cmdErr = 0;
    is_MultiTrans = ENCODER_ADDRESS_IS_MT(enc_addr);

    cmd = is_MultiTrans ? A_FORMAT_REQ_MT_ABS_LOWER_24BIT_STAT : A_FORMAT_REQ_IT_ABS_LOWER_24BIT_STAT;
    cdf = A_FORMAT_PACK_CDF((enc_addr & 0x7), cmd, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);
    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    nEncoder = is_MultiTrans ? ((enc_addr & 0x7) + 1) : 1;
    memset(res, 0, sizeof(a_format_res3_t) * nEncoder);

    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
    FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)res,
                                 HALFWORD_NUM(a_format_res3_t) * nEncoder);

    crc8_para.message_len = 8;
    for (uint8_t i = 0; i < nEncoder; i++)
    {
        crc8_para.message = (uint8_t const *)&res[i];
        if ((A_FORMAT_GET_CMD_CODE_IF(res[i].IF) != cmd) || (CRC_Calc(&crc8_para) != 0))
       	{
            cmdErr++;
            singleStat[i].es = kFLEXIO_A_FORMAT_ES_FrameErr;
            continue;
        }

        singleStat[i].encID = A_FORMAT_GET_ENC_ADDR_IF(res[i].IF);
        singleStat[i].es    = A_FORMAT_GET_ENC_STAT_IF(res[i].IF);
        if (singleStat[i].es != kFLEXIO_A_FORMAT_ES_NoErr)
        {
            cmdErr++;
        }

        singleStat[i].singleTurn = *(uint32_t *)res[i].DF & enc->single_turn_sign_mask;
        singleStat[i].ALM = (res[i].DF[2] << 8) | (res[i].DF[1] >> 8);
    }

    return cmdErr ? kStatus_Fail : kStatus_Success;
}

status_t A_Format_ABS_Readout_Single_with_temperature(encoder_a_format_t *enc, uint8_t enc_addr, a_format_single_temp_t *singleTemp)
{
    a_format_res3_t *res = res3;
    uint16_t temp;

    cmdErr = 0;
    is_MultiTrans = ENCODER_ADDRESS_IS_MT(enc_addr) ? true : false;

    cmd = is_MultiTrans ? A_FORMAT_REQ_MT_ABS_LOWER_24BIT_TEMP : A_FORMAT_REQ_IT_ABS_LOWER_24BIT_TEMP;
    cdf = A_FORMAT_PACK_CDF((enc_addr & 0x7), cmd, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);
    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    nEncoder = is_MultiTrans ? ((enc_addr & 0x7) + 1) : 1;
    memset(res, 0, sizeof(a_format_res3_t) * nEncoder);

    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
    FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)res,
                                 HALFWORD_NUM(a_format_res3_t) * nEncoder);

    crc8_para.message_len = 8;
    for (uint8_t i = 0; i < nEncoder; i++)
    {
        crc8_para.message = (uint8_t const *)&res[i];
        if ((A_FORMAT_GET_CMD_CODE_IF(res[i].IF) != cmd) || (CRC_Calc(&crc8_para) != 0))
       	{
            cmdErr++;
            singleTemp[i].es = kFLEXIO_A_FORMAT_ES_FrameErr;
            continue;
        }

        singleTemp[i].encID = A_FORMAT_GET_ENC_ADDR_IF(res[i].IF);
        singleTemp[i].es    = A_FORMAT_GET_ENC_STAT_IF(res[i].IF);
        if (singleTemp[i].es != kFLEXIO_A_FORMAT_ES_NoErr)
        {
            cmdErr++;
        }

        singleTemp[i].singleTurn = *(uint32_t *)res[i].DF & enc->single_turn_sign_mask;
        temp = (res[i].DF[2] << 8) | (res[i].DF[1] >> 8);
        singleTemp[i].temperature = GET_TEMPERATURE_VALUE(temp);
    }

    return cmdErr ? kStatus_Fail : kStatus_Success;
}

status_t A_Format_ABS_Readout_Multi_Single_Parse(encoder_a_format_t *enc, a_format_res3_t *res,
                                                 a_format_abs_multi_single_t *abs_data)
{
    cmdErr = 0;

    crc8_para.message_len = 8;
    for (uint8_t i = 0; i < nEncoder; i++)
    {
        crc8_para.message = (uint8_t const *)&res[i];
        if ((A_FORMAT_GET_CMD_CODE_IF(res[i].IF) != cmd) || (CRC_Calc(&crc8_para) != 0))
        {
            cmdErr++;
            abs_data[i].es = kFLEXIO_A_FORMAT_ES_FrameErr;
            continue;
        }

        abs_data[i].encID = A_FORMAT_GET_ENC_ADDR_IF(res[i].IF);
        abs_data[i].es    = A_FORMAT_GET_ENC_STAT_IF(res[i].IF);
        if (abs_data[i].es != kFLEXIO_A_FORMAT_ES_NoErr)
        {
            cmdErr++;
        }

        abs_data[i].singleTurn = *(uint32_t *)res[i].DF & enc->single_turn_sign_mask;
        abs_data[i].multiTurn  = (uint16_t)((*(uint32_t *)&(res[i].DF[1]) >> (enc->singleTurnRevolution - 16)) & enc->multi_turn_sign_mask);
    }

    return cmdErr ? kStatus_Fail : kStatus_Success;
}

status_t A_Format_ABS_Readout_Multi_Single_CMD(uint8_t enc_addr)
{
    if (ENCODER_ADDRESS(enc_addr) >= A_FORMAT_ENCODER_MAX_NUM)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    is_MultiTrans = ENCODER_ADDRESS_IS_MT(enc_addr);
    enc_addr &= 0x7;

    cmd = is_MultiTrans ? A_FORMAT_REQ_MT_ABS_FULL_40BIT : A_FORMAT_REQ_IT_ABS_FULL_40BIT;
    cdf = A_FORMAT_PACK_CDF(enc_addr, cmd, 0);
    crc_data = A_FORMAT_GET_CRC_DATA_CDF(cdf);
    crc = CRC_Calc(&crc3_para);
    cdf = A_FORMAT_SET_CRC_CODE_CDF(cdf, crc);

    nEncoder = is_MultiTrans ? (enc_addr + 1) : 1;
    memset(res3, 0, sizeof(a_format_res3_t) * nEncoder);

    return kStatus_Success;
}

status_t A_Format_ABS_Readout_Multi_Single(encoder_a_format_t *enc, uint8_t enc_addr,
                                           a_format_abs_multi_single_t *abs_data)
{
    if (A_Format_ABS_Readout_Multi_Single_CMD(enc_addr) == kStatus_FLEXIO_A_FORMAT_OutOfIDRange)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);
    FLEXIO_A_Format_ReadBlocking(enc->controller, (uint16_t *)res3,
                                 HALFWORD_NUM(a_format_res3_t) * nEncoder);

    return A_Format_ABS_Readout_Multi_Single_Parse(enc, res3, abs_data);
}
