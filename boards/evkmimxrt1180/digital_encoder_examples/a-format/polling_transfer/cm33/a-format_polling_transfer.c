/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "a-format.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ENC_ADDR 0x03
#define ENC_ID   0x030201

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
encoder_a_format_t encoder;
status_t status;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize A-format encoder */
    flexio_a_format_config_t devConfig;
    FLEXIO_A_FORMAT_Type encDev;
    a_format_abs_multi_t multiData;
    a_format_abs_single_t singleData;
    a_format_single_stat_t singleStat;
    a_format_single_temp_t singleTemp;
    a_format_abs_multi_single_t enc_abs[4], abs_save;
    uint32_t enc_id = 0;
    uint32_t clock;
    float temp, temp_save;
    uint32_t time = 0;
    a_format_status_t statusData;
    a_format_eeprom_t eeprom = {
        .address = 0x01,
        .data = 0x5A2B
    };

    BOARD_InitHardware();
    clock = FLEXIO_CLOCK_FREQUENCY / 1000000;

    PRINTF("%s %s\r\n", __DATE__, __TIME__);
    PRINTF("Encoder A-format example\r\n");
    PRINTF("FlexIO Root Clock is %d MHz\r\n", clock);

    /* 
     * Config->enableA_Format   = true;
     * Config->enableInDoze     = false;
     * Config->enableInDebug    = true;
     * Config->enableFastAccess = false;
     * Config->baudRate_bps     = kFLEXIO_A_FORMAT_2_5MHZ;
     * Config->userMode         = kFLEXIO_A_FORMAT_USERMODE_ONESHOT;
     */
    FLEXIO_A_Format_GetDefaultConfig(&devConfig);

    encDev.flexioBase                          = BOARD_FLEXIO_BASE;
    encDev.TxPinIndex                          = FLEXIO_A_FORMAT_TX_PIN; 
    encDev.RxPinIndex                          = FLEXIO_A_FORMAT_RX_PIN;
    encDev.DRPinIndex                          = FLEXIO_A_FORMAT_DR_PIN;
    encDev.shifterIndex[0]                     = A_FORMAT_TX_SHIFTER_INDEX;
    encDev.shifterIndex[1]                     = A_FORMAT_RX_SHIFTER_INDEX;
    encDev.timerIndex[A_FORMAT_TIMER_TX_INDEX] = A_FORMAT_TX_TIMER_INDEX;
    encDev.timerIndex[A_FORMAT_TIMER_RX_INDEX] = A_FORMAT_RX_TIMER_INDEX;
    encDev.timerIndex[A_FORMAT_TIMER_DR_INDEX] = A_FORMAT_DR_TIMER_INDEX;

    encoder.controller = &encDev;
    encoder.singleTurnRevolution  = 20;
    encoder.multiTunrRevolution   = 16;
    encoder.single_turn_sign_mask = 0x000FFFFF;
    encoder.multi_turn_sign_mask  = 0x0000FFFF;

    status = FLEXIO_A_Format_Init(&encDev, &devConfig, FLEXIO_CLOCK_FREQUENCY);
    if (status != kStatus_Success)
    {
        return -1;
    }

    PRINTF("\r\n******************** 1 to 1 mode (Only one encoder connects to the board) ********************\r\n");
    PRINTF("****************\r\n* Test case  1 *\r\n****************\r\n");
    PRINTF("> Set the encoder address to %d ==> ", ENC_ADDR);
    status = A_Format_Set_Encoder_Address_1to1(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR));
    if (status == kStatus_FLEXIO_A_FORMAT_FrameErr)
    {
        PRINTF("failed\r\n");
    }
    else
    {
        PRINTF("successful\r\n");
    }

    PRINTF("****************\r\n* Test case  2 *\r\n****************\r\n");
    PRINTF("> Set the encoder ID to 0x%06X ==> ", ENC_ID);
    status = A_Format_Set_ID_1to1(&encoder, ENC_ID);
    if (status == kStatus_FLEXIO_A_FORMAT_FrameErr)
    {
        PRINTF("failed\r\n");
    }
    else
    {
        PRINTF("successful\r\n");
    }

    PRINTF("****************\r\n* Test case  3 *\r\n****************\r\n");
    PRINTF("> Get the encoder ID ==> ");
    status = A_Format_Get_ID_1to1(&encoder, &enc_id);
    if (status == kStatus_FLEXIO_A_FORMAT_FrameErr)
    {
        PRINTF("failed\r\n");
    }
    else
    {
        PRINTF("0x%06X (successful)\r\n", enc_id);
    }

    PRINTF("\r\n******************** Individual Transmission mode (The board accesses only one encoder on the bus) ********************\r\n");
    PRINTF("****************\r\n* Test case  4 *\r\n****************\r\n");
    PRINTF("> The encoder address %d is set by matching the ID 0x%06X ==> ", ENC_ADDR, ENC_ID);
    status = A_Format_Set_Encoder_Address_MATCH_ID(&encoder, ENC_ID, ENCODER_ADDRESS_IT(ENC_ADDR));
    if (status == kStatus_FLEXIO_A_FORMAT_FrameErr)
    {
        PRINTF("failed\r\n");
    }
    else
    {
        PRINTF("successful\r\n");
    }

    PRINTF("****************\r\n* Test case  5 *\r\n****************\r\n");
    PRINTF("> Set the encoder ID to 0x%06X ==> ", ENC_ID);
    status = A_Format_Set_ID(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), ENC_ID);
    if (status == kStatus_FLEXIO_A_FORMAT_FrameErr)
    {
        PRINTF("failed\r\n");
    }
    else
    {
        PRINTF("successful\r\n");
    }

    PRINTF("****************\r\n* Test case  6 *\r\n****************\r\n");
    PRINTF("> Get the encoder ID ==> ");
    status = A_Format_Get_ID(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), &enc_id);
    if (status == kStatus_FLEXIO_A_FORMAT_FrameErr)
    {
        PRINTF("failed\r\n");
    }
    else
    {
        PRINTF("0x%06X (successful)\r\n", enc_id);
    }

    PRINTF("****************\r\n* Test case  7 *\r\n****************\r\n");
    PRINTF("> Write the data 0x%04X to the address 0x%02X of EEPROM ==> ", eeprom.data, eeprom.address);
    status = A_Format_Memory_Write(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), &eeprom);
    if (status == kStatus_FLEXIO_A_FORMAT_FrameErr)
    {
        PRINTF("failed\r\n");
    }
    else
    {
        PRINTF("successful\r\n");
    }

    PRINTF("****************\r\n* Test case  8 *\r\n****************\r\n");
    PRINTF("> Read the data from the address 0x%02X of EEPROM ==> ", eeprom.address);
    memset(&eeprom.data, 0, 2);
    SDK_DelayAtLeastUs(30000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    status = A_Format_Memory_Read(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), &eeprom);
    if (status == kStatus_FLEXIO_A_FORMAT_FrameErr)
    {
        PRINTF("failed\r\n");
    }
    else
    {
        PRINTF("0x%04X (successful)\r\n", eeprom.data);
    }

    PRINTF("****************\r\n* Test case  9 *\r\n****************\r\n");
    PRINTF("> Read the status of the encoder 0x%02X ==> ", ENC_ADDR);
    A_Format_Readout_Encoder_status(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), &statusData);
    if (statusData.es == kFLEXIO_A_FORMAT_ES_FrameErr)
    {
        PRINTF("failed\r\n");
    }
    else
    {
        PRINTF("ES: 0x%02X, ALM: 0x%04X (successful)\r\n", statusData.es, statusData.status);
    }

    PRINTF("****************\r\n* Test case 10 *\r\n****************\r\n");
    PRINTF("> Get the temperature of the encoder ==> ");
    status = A_Format_Get_Temperature(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), &temp);
    if (status == kStatus_FLEXIO_A_FORMAT_FrameErr)
    {
        PRINTF("failed\r\n");
    }
    else
    {
        PRINTF("%f (successful)\r\n", temp);
    }

    PRINTF("****************\r\n* Test case 11 *\r\n****************\r\n");
    PRINTF("> Get the multi-turn data of the encoder ==> ");
    A_Format_ABS_Readout_Multi(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), &multiData);
    if (multiData.es == kFLEXIO_A_FORMAT_ES_FrameErr)
    {
        PRINTF("failed\r\n");
    }
    else
    {
        PRINTF("ES: 0x%02X, Multi-turn: %d (successful)\r\n", multiData.es, multiData.multiTurn);
    }

    PRINTF("****************\r\n* Test case 12 *\r\n****************\r\n");
    PRINTF("> Get the single-turn data of the encoder ==> ");
    A_Format_ABS_Readout_Single(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), &singleData);
    if (singleData.es == kFLEXIO_A_FORMAT_ES_FrameErr)
    {
        PRINTF("failed\r\n");
    }
    else
    {
        PRINTF("ES: 0x%02X, Single-turn: %d (successful)\r\n", singleData.es, singleData.singleTurn);
    }

    PRINTF("****************\r\n* Test case 13 *\r\n****************\r\n");
    PRINTF("> Get the single-turn data and status of the encoder ==> ");
    A_Format_ABS_Readout_Single_with_status(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), &singleStat);
    if (singleStat.es == kFLEXIO_A_FORMAT_ES_FrameErr)
    {
        PRINTF("failed\r\n");
    }
    else
    {
        PRINTF("ES: 0x%02X, Single-turn: %d, status: 0x%04X (successful)\r\n",
               singleStat.es, singleStat.singleTurn, singleStat.ALM);
    }

    PRINTF("****************\r\n* Test case 14 *\r\n****************\r\n");
    PRINTF("> Get the single-turn data and temperature of the encoder ==> ");
    A_Format_ABS_Readout_Single_with_temperature(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), &singleTemp);
    if (singleTemp.es == kFLEXIO_A_FORMAT_ES_FrameErr)
    {
        PRINTF("failed\r\n");
    }
    else
    {
        PRINTF("ES: 0x%02X, Single-turn: %d, temperature: %f (successful)\r\n",
               singleTemp.es, singleTemp.singleTurn, singleTemp.temperature);
    }

    PRINTF("****************\r\n* Test case 15 *\r\n****************\r\n");
    PRINTF("> Get the single-turn data and the multi-turn data of the encoder ==> \r\n");
    status = A_Format_ABS_Readout_Multi_Single(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), &enc_abs[0]);
    if (enc_abs[0].es != kFLEXIO_A_FORMAT_ES_FrameErr)
    {
        A_Format_PrintfES(PRINTF, enc_abs[0].es);
        PRINTF("\tMulti-turn data: %d, single-turn data: %d\r\n", enc_abs[0].multiTurn, enc_abs[0].singleTurn);
    }
    else
    {
        PRINTF("failed\r\n");
    }

    PRINTF("\r\n******************** Multiple Transmission mode (The board can access multiple encoders on the bus simultaneously) ********************\r\n");
    PRINTF("****************\r\n* Test case 16 *\r\n****************\r\n");
    PRINTF("> Get the single-turn data and the multi-turn data of the encoder ==> \r\n");
    status = A_Format_ABS_Readout_Multi_Single(&encoder, ENCODER_ADDRESS_MT(ENC_ADDR), &enc_abs[0]);
    for (uint8_t index = 0; index <= ENC_ADDR; index++)
    {
        if (enc_abs[index].es != kFLEXIO_A_FORMAT_ES_FrameErr)
        {
            A_Format_PrintfES(PRINTF, enc_abs[index].es);
            PRINTF("\t[Encoder %d] Multi-turn data: %d, single-turn data: %d\r\n",
                   enc_abs[index].encID, enc_abs[index].multiTurn, enc_abs[index].singleTurn);
        }
        else
        {
            PRINTF("\t[Encoder -] Disconnected\r\n");
        }
    }

    PRINTF("\r\n******************** Running the loop test ********************\r\n");
    abs_save  = enc_abs[0];
    temp_save = temp;
    while (1)
    {
        /* Delay at least 90 ms. */
        SDK_DelayAtLeastUs(90000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        time++;
        status = A_Format_ABS_Readout_Multi_Single(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), &enc_abs[0]);
        if (enc_abs[0].es == kFLEXIO_A_FORMAT_ES_FrameErr)
        {
            PRINTF("Encoder frame is error!\r\n");
            continue;
        }

        SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        status = A_Format_Get_Temperature(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), &temp);
        if (status == kStatus_FLEXIO_A_FORMAT_FrameErr)
        {
            continue;
        }
        if ((abs(enc_abs[0].singleTurn - abs_save.singleTurn) > 500) || (fabs(temp - temp_save) > 0.1))
        {
            PRINTF("[%.2fs] Multi-turn data: %d, single-turn data: %d\r\n\t Temperature: %f\r\n\r\n",
                   time/10.0, enc_abs[0].multiTurn, enc_abs[0].singleTurn, temp);
            abs_save = enc_abs[0];
            temp_save = temp;
        }
    }
    return 0;
}
