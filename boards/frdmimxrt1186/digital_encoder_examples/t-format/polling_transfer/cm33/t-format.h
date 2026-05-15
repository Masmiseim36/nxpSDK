/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _T_FORMAT_H_
#define _T_FORMAT_H_

#include "fsl_flexio_t-format.h"

/*! @brief T-format encoder structure. */
typedef struct _encoder_t_format
{
    uint8_t singleTurnRevolution; /*!< The number of bits for single turn revolution. */
    uint8_t multiTunrRevolution; /*!< The number of bits for multiple turn revolution. */
    uint32_t single_turn_sign_mask;
    uint32_t single_turn_sign_extend_mask;
    uint32_t multi_turn_sign_mask;
    uint32_t multi_turn_sign_extend_mask;
    void *controller;
} encoder_t_format_t;

/*! @brief All the information of the T-format encoder. */
typedef struct _t_format_all_info
{
    uint32_t singleTurn;
    uint32_t multiTurn;
    uint8_t encID;
    uint8_t ALMC;
} t_format_all_info_t;

/*! @brief T-format encoder EEPROM data structure. */
typedef struct _t_format_access_eeprom
{
    uint8_t page;
    uint8_t address;
    uint8_t data;
} t_format_access_eeprom_t;

/*! @brief T-format encoder request data structure. */
typedef struct _t_format_req_eeprom_read
{
    uint8_t cf;
    uint8_t adf;
    uint8_t crc;
} t_format_req_eeprom_read_t;

typedef struct _t_format_req_eeprom_write
{
    uint8_t cf;
    uint8_t adf;
    uint8_t edf;
    uint8_t crc;
} t_format_req_eeprom_write_t;

typedef struct _t_format_res_id
{
    uint8_t CF;
    uint8_t SF;
    uint8_t ENCID;
    uint8_t CRC8;
} t_format_res_id_t;

typedef struct _t_format_res_all_info
{
    uint8_t CF;
    uint8_t SF;
    uint8_t ABS[3];
    uint8_t ENCID;
    uint8_t ABM[3];
    uint8_t ALMC;
    uint8_t CRC8;
} t_format_res_all_info_t;

typedef struct _t_format_res_eeprom
{
    uint8_t CF;
    uint8_t ADF;
    uint8_t EDF;
    uint8_t CRC8;
} t_format_res_eeprom_t;

char *T_Format_GetStatusFlag(status_t status);
status_t T_Format_Readout_ABS_ABM(encoder_t_format_t *enc, t_format_all_info_t *all_info);
status_t T_Format_Get_Encoder_ID(encoder_t_format_t *enc, uint8_t *encID);
status_t T_Format_Get_Temperature(encoder_t_format_t *enc, int8_t *temperature);

#endif
