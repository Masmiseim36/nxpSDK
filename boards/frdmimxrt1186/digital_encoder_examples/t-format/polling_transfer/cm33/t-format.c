/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "t-format.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* The default when a main power supply is turned on is page 0 */
static uint8_t eeprom_page = 0;
static uint8_t cf;
static t_format_res_all_info_t resAll_g;
static t_format_res_id_t resId_g;

/* Description of Status Flag */
static char *T_FORMAT_SF_EA0_String   = "One revolution data is deviated by any malfunction or defect at main power-on";
static char *T_FORMAT_SF_EA1_String   = "Logic-OR of Over-heat, Multi-turn error, Battery error and Battery alarm";
static char *T_FORMAT_SF_CA0_String   = "Parity error in Request frame occurs";
static char *T_FORMAT_SF_CA1_String   = "Delimiter error in Request frame occurs";
static char *T_FORMAT_NO_ERROR_String = "No error occurs";

/*******************************************************************************
 * Codes
 ******************************************************************************/
char *T_Format_GetStatusFlag(status_t status)
{
    switch (status)
    {
    case kStatus_FLEXIO_T_FORMAT_EncErr0_CountingErr:
        return T_FORMAT_SF_EA0_String;
    case kStatus_FLEXIO_T_FORMAT_EncErr1_LogicOR:
        return T_FORMAT_SF_EA1_String;
    case kStatus_FLEXIO_T_FORMAT_ComAlr0_ParityErr:
        return T_FORMAT_SF_CA0_String;
    case kStatus_FLEXIO_T_FORMAT_ComAlr1_DelimiterErr:
        return T_FORMAT_SF_CA1_String;
    default:
        return T_FORMAT_NO_ERROR_String;
    }
}

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

status_t T_Format_Get_Encoder_ID_Parse(encoder_t_format_t *enc, t_format_res_id_t *res, uint8_t *encID)
{
    if (CRC_Calc((uint8_t *)res, T_FORMAT_ENCODER_ID_BYTE) != 0)
    {
        return kStatus_FLEXIO_T_FORMAT_FrameErr;
    }

    *encID = res->ENCID;

    return T_Format_Check_SF(res->SF);
}

status_t T_Format_Get_Encoder_ID(encoder_t_format_t *enc, uint8_t *encID)
{
    cf = T_FORMAT_CF_GET_ENCID;

    FLEXIO_T_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_T_Format_WriteBlocking(enc->controller, &cf, 1);
    FLEXIO_T_Format_ReadBlocking(enc->controller, (uint8_t *)&resId_g, T_FORMAT_ENCODER_ID_BYTE);

    return T_Format_Get_Encoder_ID_Parse(enc, &resId_g, encID);
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

status_t T_Format_Readout_ABS_ABM(encoder_t_format_t *enc, t_format_all_info_t *all_info)
{
    cf = T_FORMAT_CF_GET_ALL;

    FLEXIO_T_Format_Config_DR_length(enc->controller, 1);
    FLEXIO_T_Format_WriteBlocking(enc->controller, &cf, 1);
    FLEXIO_T_Format_ReadBlocking(enc->controller, (uint8_t *)&resAll_g, T_FORMAT_ALL_INFO_BYTE);

    return T_Format_Readout_ABS_ABM_Parse(enc, &resAll_g, all_info);
}

status_t T_Format_Memory_Set_Page(encoder_t_format_t *enc, uint8_t page)
{
    t_format_res_eeprom_t res;
    t_format_req_eeprom_write_t req = {
        .cf  = T_FORMAT_CF_EEPROM_WRITE,
        .adf = 127,
        .edf = page
    };

    req.crc = CRC_Calc((uint8_t *)&req, 3);
    FLEXIO_T_Format_Config_DR_length(enc->controller, 4);
    FLEXIO_T_Format_WriteBlocking(enc->controller, (uint8_t *)&req, 4);
    FLEXIO_T_Format_ReadBlocking(enc->controller, (uint8_t *)&res, T_FORMAT_EEPROM_BYTE);

    if (CRC_Calc((uint8_t *)&res, T_FORMAT_EEPROM_BYTE) != 0)
    {
        return kStatus_FLEXIO_T_FORMAT_FrameErr;
    }

    if (res.ADF & T_FORMAT_SF_MASK_BUSY_STATUS)
    {
        /* EEPROM is in busy status */
        return kStatus_Busy;
    }

    eeprom_page = page;
    return kStatus_Success;
}

status_t T_Format_Memory_Read(encoder_t_format_t *enc, t_format_access_eeprom_t *eeprom)
{
    t_format_res_eeprom_t res;
    t_format_req_eeprom_read_t req = {
        .cf  = T_FORMAT_CF_EEPROM_READOUT,
        .adf = eeprom->address & T_FORMAT_ADF_MASK_ADDRESS
    };
    status_t status = kStatus_Success;

    if (eeprom->page != eeprom_page)
    {
        status = T_Format_Memory_Set_Page(enc, eeprom->page);
        if (status != kStatus_Success)
        {
            return status;
        }
        /* After the page is changed, it is not possible to access EEPROM between 18ms. */
        SDK_DelayAtLeastUs(18000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }

    req.crc = CRC_Calc((uint8_t *)&req, 2);
    FLEXIO_T_Format_Config_DR_length(enc->controller, 3);
    FLEXIO_T_Format_WriteBlocking(enc->controller, (uint8_t *)&req, 3);
    FLEXIO_T_Format_ReadBlocking(enc->controller, (uint8_t *)&res, T_FORMAT_EEPROM_BYTE);

    if (CRC_Calc((uint8_t *)&res, T_FORMAT_EEPROM_BYTE) != 0)
    {
        return kStatus_FLEXIO_T_FORMAT_FrameErr;
    }

    if (res.ADF & T_FORMAT_SF_MASK_BUSY_STATUS)
    {
        /* EEPROM is in busy status */
        return kStatus_Busy;
    }

    eeprom->data = res.EDF;

    return kStatus_Success;
}

status_t T_Format_Get_Temperature(encoder_t_format_t *enc, int8_t *temperature)
{
    t_format_access_eeprom_t eeprom = {
        .page = 7,
        .address = 5
    };
    status_t status = kStatus_Success;

    status = T_Format_Memory_Read(enc, &eeprom);
    if (status == kStatus_Success)
    {
        *temperature = (int8_t)eeprom.data;
    }
    return status;
}

