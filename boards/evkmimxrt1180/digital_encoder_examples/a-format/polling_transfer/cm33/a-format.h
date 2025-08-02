/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _A_FORMAT_H_
#define _A_FORMAT_H_

#include "fsl_flexio_a-format.h"

/*! @brief CRC type of A-format. */
typedef enum _a_format_crc_types
{
    A_FORMAT_CRC3  = 3U, /*!< CRC code has 3 bits. */
    A_FORMAT_CRC8  = 8U, /*!< CRC code has 8 bits. */
} a_format_crc_type_t;

typedef struct _a_format_crc_para_
{
   const uint8_t       *message;
   a_format_crc_type_t type;
   uint8_t             message_len;
   uint8_t             polynomial;
   bool                inputBitSwap;
   bool                outputBitSwap;
} a_format_crc_para_t;

/*! @brief A-format encoder structure. */
typedef struct _encoder_a_format
{
    uint8_t singleTurnRevolution; /*!< The number of bits for single turn revolution. */
    uint8_t multiTunrRevolution; /*!< The number of bits for multiple turn revolution. */
    uint32_t single_turn_sign_mask;
    uint32_t single_turn_sign_extend_mask;
    uint32_t multi_turn_sign_mask;
    uint32_t multi_turn_sign_extend_mask;
    void *controller;
} encoder_a_format_t;

/*! @brief A-format encoder ABS data structure. */
typedef struct _a_format_abs_multi_single
{
    uint32_t singleTurn;
    uint16_t multiTurn;
    uint8_t es;
    uint8_t encID;
} a_format_abs_multi_single_t;

typedef struct _a_format_abs_single
{
    uint32_t singleTurn;
    uint8_t es;
    uint8_t encID;
} a_format_abs_single_t;

typedef struct _a_format_abs_multi
{
    uint16_t multiTurn;
    uint8_t es;
    uint8_t encID;
} a_format_abs_multi_t;

typedef struct _a_format_status
{
    uint16_t status;
    uint8_t es;
    uint8_t encID;
} a_format_status_t;

/*! @brief A-format encoder single turn with status structure. */
typedef struct _a_format_single_stat
{
    uint32_t singleTurn;
    uint16_t ALM;
    uint8_t es;
    uint8_t encID;
} a_format_single_stat_t;

/*! @brief A-format encoder single turn with temperature structure. */
typedef struct _a_format_single_temp
{
    uint32_t singleTurn;
    float temperature;
    uint8_t es;
    uint8_t encID;
} a_format_single_temp_t;

/*! @brief A-format encoder EEPROM data structure. */
typedef struct _a_format_eeprom
{
    uint8_t address;
    uint16_t data;
} a_format_eeprom_t;

/*! @brief A-format encoder response data structure. */
typedef struct _a_format_res3
{
    uint16_t IF;
    uint16_t DF[3];
} a_format_res3_t;

typedef struct _a_format_res2
{
    uint16_t IF;
    uint16_t DF[2];
} a_format_res2_t;

typedef int (*logFunc)(const char *fmt_s, ...);

void A_Format_PrintfES(logFunc logES, uint8_t es);
/* Getting the single-turn data and the multi-turn data */
status_t A_Format_ABS_Readout_Multi_Single(encoder_a_format_t *enc, uint8_t enc_addr,
                                           a_format_abs_multi_single_t *abs_data);
/* Getting the single-turn data */
status_t A_Format_ABS_Readout_Single(encoder_a_format_t *enc, uint8_t enc_addr,
                                     a_format_abs_single_t *singleData);
/* Getting the multi-turn data */
status_t A_Format_ABS_Readout_Multi(encoder_a_format_t *enc, uint8_t enc_addr,
                                    a_format_abs_multi_t *multiData);
/* Reading the status of the encoder */
status_t A_Format_Readout_Encoder_status(encoder_a_format_t *enc, uint8_t enc_addr,
                                         a_format_status_t *statusData);
/* Setting the encoder address */
status_t A_Format_Set_Encoder_Address_1to1(encoder_a_format_t *enc, uint8_t enc_addr);
status_t A_Format_Memory_Read(encoder_a_format_t *enc, uint8_t enc_addr, a_format_eeprom_t *eeprom);
status_t A_Format_Memory_Write(encoder_a_format_t *enc, uint8_t enc_addr, a_format_eeprom_t *eeprom);
/* Getting the temperature */
status_t A_Format_Get_Temperature(encoder_a_format_t *enc, uint8_t enc_addr, float *temp);
/* The encoder ID is get by matching the encoder address */
status_t A_Format_Get_ID(encoder_a_format_t *enc, uint8_t enc_addr, uint32_t *id);
status_t A_Format_Get_ID_1to1(encoder_a_format_t *enc, uint32_t *id);
status_t A_Format_Set_ID(encoder_a_format_t *enc, uint8_t enc_addr, uint32_t id);
status_t A_Format_Set_ID_1to1(encoder_a_format_t *enc, uint32_t id);
status_t A_Format_Set_Encoder_Address_MATCH_ID(encoder_a_format_t *enc, uint32_t id, uint8_t enc_addr);
status_t A_Format_ABS_Readout_Single_with_status(encoder_a_format_t *enc, uint8_t enc_addr, a_format_single_stat_t *singleStat);
status_t A_Format_ABS_Readout_Single_with_temperature(encoder_a_format_t *enc, uint8_t enc_addr, a_format_single_temp_t *singleTemp);

#endif
