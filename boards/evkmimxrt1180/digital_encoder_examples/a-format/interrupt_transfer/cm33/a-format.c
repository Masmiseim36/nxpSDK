/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "a-format.h"

/*******************************************************************************
 * Definitions
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

static encoder_a_format_t *enc_g;
static a_format_abs_multi_single_t *abs_data_g;
static a_format_abs_single_t *single_data_g;
static a_format_abs_multi_t *multiData_g;
static a_format_status_t *statusData_g;
static float *temp_g;
static uint32_t *id_g;

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

    for (j = 0; j < crc->message_len; j++) {
        remainder ^= (crc->inputBitSwap ? Swap_Byte(crc->message[j]) : crc->message[j]);

        for (i = 0; i < 8; i++) {
            if (remainder & 0x80)
                remainder = (remainder << 1) ^ poly;
            else
                remainder <<= 1;
        }
    }

    return crc->outputBitSwap ? Swap_Byte(remainder) : (remainder >> (8 - crc->type));
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

status_t A_Format_Get_ID_IRQ(encoder_a_format_t *enc, uint8_t enc_addr, uint32_t *id)
{
    if (A_Format_Get_ID_CMD(enc_addr) == kStatus_FLEXIO_A_FORMAT_OutOfIDRange)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    flexio_a_format_transfer_t xfer = {
        .rxData   = (uint16_t *)res2,
        .dataSize = HALFWORD_NUM(a_format_res2_t)
    };

    enc_g = enc;
    id_g  = id;
    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_A_Format_TransferReceiveNonBlocking(enc->controller, ((FLEXIO_A_FORMAT_Type *)enc->controller)->hanlde,
                                               &xfer, NULL);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);

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

status_t A_Format_Readout_Encoder_status_IRQ(encoder_a_format_t *enc, uint8_t enc_addr,
                                             a_format_status_t *statusData)
{
    if (A_Format_Readout_Encoder_status_CMD(enc_addr) == kStatus_FLEXIO_A_FORMAT_OutOfIDRange)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    flexio_a_format_transfer_t xfer = {
        .rxData   = (uint16_t *)res2,
        .dataSize = HALFWORD_NUM(a_format_res2_t) * nEncoder
    };

    enc_g        = enc;
    statusData_g = statusData;
    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_A_Format_TransferReceiveNonBlocking(enc->controller, ((FLEXIO_A_FORMAT_Type *)enc->controller)->hanlde,
                                               &xfer, NULL);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);

    return kStatus_Success;
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

status_t A_Format_Get_Temperature_IRQ(encoder_a_format_t *enc, uint8_t enc_addr, float *temp)
{
    if (A_Format_Get_Temperature_CMD(enc_addr) == kStatus_FLEXIO_A_FORMAT_OutOfIDRange)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    flexio_a_format_transfer_t xfer = {
        .rxData   = (uint16_t *)res2,
        .dataSize = HALFWORD_NUM(a_format_res2_t)
    };

    enc_g  = enc;
    temp_g = temp;
    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_A_Format_TransferReceiveNonBlocking(enc->controller, ((FLEXIO_A_FORMAT_Type *)enc->controller)->hanlde,
                                               &xfer, NULL);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);

    return kStatus_Success;
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

status_t A_Format_ABS_Readout_Multi_IRQ(encoder_a_format_t *enc, uint8_t enc_addr,
                                        a_format_abs_multi_t *multiData)
{
    if (A_Format_ABS_Readout_Multi_CMD(enc_addr) == kStatus_FLEXIO_A_FORMAT_OutOfIDRange)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    flexio_a_format_transfer_t xfer = {
        .rxData   = (uint16_t *)res2,
        .dataSize = HALFWORD_NUM(a_format_res2_t) * nEncoder
    };

    enc_g       = enc;
    multiData_g = multiData;
    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_A_Format_TransferReceiveNonBlocking(enc->controller, ((FLEXIO_A_FORMAT_Type *)enc->controller)->hanlde,
                                               &xfer, NULL);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);

    return kStatus_Success;
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

status_t A_Format_ABS_Readout_Single_IRQ(encoder_a_format_t *enc, uint8_t enc_addr,
                                         a_format_abs_single_t *single_data)
{
    if (A_Format_ABS_Readout_Single_CMD(enc_addr) == kStatus_FLEXIO_A_FORMAT_OutOfIDRange)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    flexio_a_format_transfer_t xfer = {
        .rxData   = (uint16_t *)res2,
        .dataSize = HALFWORD_NUM(a_format_res2_t) * nEncoder
    };

    enc_g         = enc;
    single_data_g = single_data;
    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_A_Format_TransferReceiveNonBlocking(enc->controller, ((FLEXIO_A_FORMAT_Type *)enc->controller)->hanlde,
                                               &xfer, NULL);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);

    return kStatus_Success;
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

status_t A_Format_ABS_Readout_Multi_Single_IRQ(encoder_a_format_t *enc, uint8_t enc_addr,
                                               a_format_abs_multi_single_t *abs_data)
{
    if (A_Format_ABS_Readout_Multi_Single_CMD(enc_addr) == kStatus_FLEXIO_A_FORMAT_OutOfIDRange)
    {
        return kStatus_FLEXIO_A_FORMAT_OutOfIDRange;
    }

    flexio_a_format_transfer_t xfer = {
        .rxData   = (uint16_t *)res3,
        .dataSize = HALFWORD_NUM(a_format_res3_t) * nEncoder
    };

    enc_g      = enc;
    abs_data_g = abs_data;
    FLEXIO_A_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_A_Format_TransferReceiveNonBlocking(enc->controller, ((FLEXIO_A_FORMAT_Type *)enc->controller)->hanlde,
                                               &xfer, NULL);
    FLEXIO_A_Format_WriteBlocking(enc->controller, &cdf, 1);

    return kStatus_Success;
}

status_t A_Format_CMD_Parse(void)
{
    switch (cmd)
    {
    case A_FORMAT_REQ_IT_ABS_FULL_40BIT:
    case A_FORMAT_REQ_MT_ABS_FULL_40BIT:
        return A_Format_ABS_Readout_Multi_Single_Parse(enc_g, res3, abs_data_g);

    case A_FORMAT_REQ_IT_ABS_LOWER_24BIT:
    case A_FORMAT_REQ_MT_ABS_LOWER_24BIT:
        return A_Format_ABS_Readout_Single_Parse(enc_g, res2, single_data_g);

    case A_FORMAT_REQ_IT_ABS_UPPER_24BIT:
    case A_FORMAT_REQ_MT_ABS_UPPER_24BIT:
        return A_Format_ABS_Readout_Multi_Parse(enc_g, res2, multiData_g);

    case A_FORMAT_REQ_IT_ENCODER_STAT:
    case A_FORMAT_REQ_MT_ENCODER_STAT:
        return A_Format_Readout_Encoder_status_Parse(enc_g, res2, statusData_g);

    case A_FORMAT_REQ_IT_TEMPERATURE_10BIT:
        return A_Format_Get_Temperature_Parse(enc_g, res2, temp_g);

    case A_FORMAT_REQ_IT_ID_CODE_READ1:
        return A_Format_Get_ID_Parse(enc_g, res2, id_g);

    default:
        return kStatus_Fail;
    }
}
