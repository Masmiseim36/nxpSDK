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
#define ENC_ADDR           0x03

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
encoder_a_format_t encoder;
status_t status;
static bool cmdFlag = false;
flexio_a_format_handle_t g_aformatHandle;
uint8_t cmd_g = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
void FLEXIO_A_Format_UserCallback(FLEXIO_A_FORMAT_Type *base,
       	                          flexio_a_format_handle_t *handle,
                                  status_t status, void *userData)
{
    if (kStatus_FLEXIO_A_FORMAT_TxIdle == status)
    {
    }

    if (kStatus_FLEXIO_A_FORMAT_RxIdle == status)
    {
        cmdFlag = true;
        if (A_Format_CMD_Parse() != kStatus_Success)
        {
            cmd_g = 0xFF;
        }
        else
        {
            cmd_g = 0;
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize A-format encoder */
    flexio_a_format_config_t devConfig;
    FLEXIO_A_FORMAT_Type encDev;
    a_format_abs_multi_single_t enc_abs = {0}, abs_save = {0};

    BOARD_InitHardware();

    PRINTF("Encoder A-format example (PWM trigger)\r\n");

    /* 
     * Config->enableA_Format   = true;
     * Config->enableInDoze     = false;
     * Config->enableInDebug    = true;
     * Config->enableFastAccess = false;
     * Config->baudRate_bps     = kFLEXIO_A_FORMAT_2_5MHZ;
     * Config->userMode         = kFLEXIO_A_FORMAT_USERMODE_ONESHOT;
     */
    FLEXIO_A_Format_GetDefaultConfig(&devConfig);
    devConfig.userMode = kFLEXIO_A_FORMAT_USERMODE_SYNC;

    encDev.flexioBase                          = BOARD_FLEXIO_BASE;
    encDev.TxPinIndex                          = FLEXIO_A_FORMAT_TX_PIN; 
    encDev.RxPinIndex                          = FLEXIO_A_FORMAT_RX_PIN;
    encDev.DRPinIndex                          = FLEXIO_A_FORMAT_DR_PIN;
    encDev.shifterIndex[0]                     = A_FORMAT_TX_SHIFTER_INDEX;
    encDev.shifterIndex[1]                     = A_FORMAT_RX_SHIFTER_INDEX;
    encDev.timerIndex[A_FORMAT_TIMER_TX_INDEX] = A_FORMAT_TX_TIMER_INDEX;
    encDev.timerIndex[A_FORMAT_TIMER_RX_INDEX] = A_FORMAT_RX_TIMER_INDEX;
    encDev.timerIndex[A_FORMAT_TIMER_DR_INDEX] = A_FORMAT_DR_TIMER_INDEX;
    encDev.triggerIn                           = 0;

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

    FLEXIO_A_Format_TransferCreateHandle(&encDev, &g_aformatHandle, FLEXIO_A_Format_UserCallback, NULL);
    PWM_Trigger_Init(BOARD_PWM_INSTANCE);

    PRINTF("\r\n******************** Running the loop test ********************\r\n");
    A_Format_ABS_Readout_Multi_Single_IRQ(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), &enc_abs);
    while (1)
    {
        if (cmdFlag)
        {
            if (abs(enc_abs.singleTurn - abs_save.singleTurn) > 500)
            {
                PRINTF("Multi-turn data: %d, single-turn data: %d\r\n", enc_abs.multiTurn, enc_abs.singleTurn);
                abs_save = enc_abs;
            }
            cmdFlag = false;
            A_Format_ABS_Readout_Multi_Single_IRQ(&encoder, ENCODER_ADDRESS_IT(ENC_ADDR), &enc_abs);
        }
    }
}
