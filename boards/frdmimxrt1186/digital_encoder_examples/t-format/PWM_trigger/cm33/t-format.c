/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "t-format.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
static encoder_t_format_t *enc_g;
static uint8_t cf;
static t_format_res_all_info_t resAll_g;
static t_format_all_info_t *allInfo_g;

static uint8_t CRC_Calc(const uint8_t *message, uint8_t message_len)
{
    uint8_t remainder = 0;
    uint8_t i = 0, j = 0;
    uint8_t poly = T_FORMAT_CRC_POLYNOMIAL;

    for (j = 0; j < message_len; j++) {
        remainder ^= message[j];

        for (i = 0; i < 8; i++) {
            if (remainder & 0x80)
                remainder = (remainder << 1) ^ poly;
            else
                remainder <<= 1;
        }
    }

    return remainder;
}

status_t T_Format_Check_SF(uint8_t sf)
{
    uint8_t temp;

    if (sf)
    {
        temp = T_FORMAT_SF_GET_COMMUNICATION_ALARM(sf);
        if (temp)
        {
            if (temp & 0x01)
            {
                return kStatus_FLEXIO_T_FORMAT_ComAlr0_ParityErr;
            }
            else
            {
                return kStatus_FLEXIO_T_FORMAT_ComAlr1_DelimiterErr;
            }
        }
        temp = T_FORMAT_SF_GET_ENCODER_ERROR(sf);
        if (temp)
        {
            if (temp & 0x01)
            {
                return kStatus_FLEXIO_T_FORMAT_EncErr0_CountingErr;
            }
            else
            {
                return kStatus_FLEXIO_T_FORMAT_EncErr1_LogicOR;
            }
        }
    }
    return kStatus_Success;
}

status_t T_Format_Readout_ABS_ABM_IRQ(encoder_t_format_t *enc, t_format_all_info_t *all_info)
{
    flexio_t_format_transfer_t xfer = {
        .rxData   = (uint8_t *)&resAll_g,
        .dataSize = T_FORMAT_ALL_INFO_BYTE
    };

    cf        = T_FORMAT_CF_GET_ALL;
    enc_g     = enc;
    allInfo_g = all_info;
    FLEXIO_T_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_T_Format_TransferReceiveNonBlocking(enc->controller, ((FLEXIO_T_FORMAT_Type *)enc->controller)->hanlde,
                                               &xfer, NULL);
    FLEXIO_T_Format_WriteBlocking(enc->controller, &cf, 1);

    return kStatus_Success;
}

status_t T_Format_Readout_ABS_ABM_Parse(encoder_t_format_t *enc, t_format_res_all_info_t *res,
                                        t_format_all_info_t *all_info)
{
    if (CRC_Calc((uint8_t *)res, T_FORMAT_ALL_INFO_BYTE) != 0)
    {
        return kStatus_FLEXIO_T_FORMAT_FrameErr;
    }

    all_info->ALMC  = res->ALMC;
    all_info->encID = res->ENCID;
    memcpy(&all_info->singleTurn, &res->ABS, 3);
    all_info->singleTurn &= enc->single_turn_sign_mask;
    memcpy(&all_info->multiTurn, &res->ABM, 3);
    all_info->multiTurn &= enc->multi_turn_sign_mask;

    return T_Format_Check_SF(res->SF);
}

status_t T_Format_CMD_Parse(void)
{
    switch (cf)
    {
    case T_FORMAT_CF_GET_ALL:
        return T_Format_Readout_ABS_ABM_Parse(enc_g, &resAll_g, allInfo_g);

    default:
        break;
    }
    return kStatus_InvalidArgument;
}

