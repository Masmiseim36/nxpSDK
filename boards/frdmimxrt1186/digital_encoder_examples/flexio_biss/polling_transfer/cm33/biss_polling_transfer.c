/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_flexio_biss.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CRC6_POLY_BISSC 0x43U
#define CRC6_MASK       0x3FU   /*!< 6-bit mask */
#define CRC6_MSB        0x20U   /*!< Most significant bit of 6-bit CRC */

#define BISS_FRAME_STP(data, frame)   (((data) >> (frame).cdmBits) & BISS_FRAME_MASK((frame).stopBits))
#define BISS_FRAME_CRC(data, frame)   (((data) >> ((frame).cdmBits + (frame).stopBits)) & BISS_FRAME_MASK((frame).crcBits))
#define BISS_FRAME_W_E(data, frame)   (((data) >> ((frame).cdmBits + (frame).stopBits + (frame).crcBits)) \
                                       & BISS_FRAME_MASK((frame).erWrBits))
#define BISS_FRAME_ST(data, frame)    (((data) >> ((frame).cdmBits + (frame).stopBits + (frame).crcBits + (frame).erWrBits)) \
                                       & BISS_FRAME_MASK((frame).stBits))
#define BISS_FRAME_MT(data, frame)    (((data) >> ((frame).cdmBits + (frame).stopBits + (frame).crcBits + (frame).erWrBits + (frame).stBits)) \
                                       & BISS_FRAME_MASK((frame).mtBits))
#define BISS_FRAME_HD(data, frame)    (((data) >> ((frame).cdmBits + (frame).stopBits + (frame).crcBits + (frame).dataBits)) \
                                       & BISS_FRAME_MASK((frame).headBits))
#define BISS_FRAME_DATA(data, frame)  (((data) >> ((frame).cdmBits + (frame).stopBits + (frame).crcBits)) \
                                       & BISS_FRAME_MASK((frame).dataBits))

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
} encoder_biss_t;

/*! @brief BiSS encoder ABS data structure. */
typedef struct _biss_abs_multi_single
{
    uint32_t singleTurn;
    uint16_t multiTurn;
} biss_multi_single_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexio_biss_frame_t frame_g;

/*******************************************************************************
 * Code
 ******************************************************************************/
uint8_t CRC6_Calculate(uint64_t data, uint8_t dataLength)
{
    if ((dataLength == 0U) || (dataLength > 64U))
    {
        return 0U;
    }

    uint8_t crc = 0x00 & CRC6_MASK;
    uint64_t mask = 1ULL << (dataLength - 1U);

    /* Process each bit */
    for (uint8_t i = 0U; i < dataLength; i++)
    {
        uint8_t bit = (data & mask) ? 1U : 0U;
        mask >>= 1U;


        /* XOR the bit with MSB of CRC */
        uint8_t feedback = ((crc & CRC6_MSB) != 0U) ? 1U : 0U;
        feedback ^= bit;

        /* Shift CRC left */
        crc = (crc << 1U) & CRC6_MASK;

        /* Apply polynomial if feedback is 1 */
        if (feedback != 0U)
        {
            crc ^= CRC6_POLY_BISSC;
        }
    }

    /* Apply final XOR */
    crc ^= CRC6_MASK;

    return crc & CRC6_MASK;
}

status_t BISS_Data_Parser(uint64_t *data, biss_multi_single_t *abs_data)
{
    uint64_t temp = *data;
    uint8_t crc  = BISS_FRAME_CRC(temp, frame_g);
    uint8_t head = BISS_FRAME_HD(temp, frame_g);
    uint64_t crc_data = BISS_FRAME_DATA(temp, frame_g);

    abs_data->singleTurn = BISS_FRAME_ST(temp, frame_g);
    abs_data->multiTurn  = BISS_FRAME_MT(temp, frame_g);

    PRINTF("  CRC     = 0x%02X; CRC_Cal = 0x%02X\r\n", crc, CRC6_Calculate(crc_data, frame_g.dataBits));
    PRINTF("  head    = 0x%02X\r\n", head);
    PRINTF("  CRCdata = 0x%08X\r\n", (uint32_t)crc_data);
    PRINTF("  ST      = 0x%08X\r\n", abs_data->singleTurn);
    PRINTF("  MT      = 0x%04X\r\n", abs_data->multiTurn);

    return 0;
}

void BISS_PrintfFrame(void)
{
    PRINTF("head=%d; mt=%d; st=%d; erWr=%d; crc=%d; stop=%d; cdm=%d; data=%d; total=%d; whole=%d\r\n",
           frame_g.headBits, frame_g.mtBits, frame_g.stBits, frame_g.erWrBits, frame_g.crcBits, frame_g.stopBits,
           frame_g.cdmBits, frame_g.dataBits, frame_g.totalBits, frame_g.wholeBits);
}

status_t BISS_VerifyFrame(encoder_biss_t *enc)
{
    status_t status;
    uint32_t dataBuffer[BISS_FRAME_BUFFER_NUM];
    bool isOk = false;
    uint8_t count = 0;
    uint8_t len[8] = {0};
    uint64_t cdmValue = FLEXIO_BISS_CalClockValue(&frame_g, false);

    PRINTF("The initial frame format:\r\n");
    BISS_PrintfFrame();

    PRINTF("CDM Value = %llX\r\n", cdmValue);
verify:
    count++;
    for (int i = 0; i < 8; i++)
    {
        FLEXIO_BISS_GenClock(enc->controller, cdmValue);
        status = FLEXIO_BISS_ReadBlocking(enc->controller, dataBuffer, BISS_FRAME_BUFFER_NUM);
        if (status == kStatus_Success)
        {
            len[i] = FLEXIO_BISS_CalFrameHeadLen((uint64_t *)dataBuffer);
            if (i > 3)
            {
                isOk = (len[i] == len[i - 1] ? true : false);
	    }
        }
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }

    if (isOk)
    {
        PRINTF("[OK] Shifer1 = 0x%08X; Shifter0 = 0x%08X\r\n", dataBuffer[0], dataBuffer[1]);
        if (len[7] != frame_g.headBits)
        {
            FLEXIO_BISS_UpdateFieldBit(&frame_g, kFLEXIO_BISS_FIELD_HD, len[7]);
            FLEXIO_BISS_UpdateClockTimer(enc->controller);
        }
        BISS_PrintfFrame();
    }
    else
    {
        if (count >= 10)
        {
            PRINTF("[Fail] The validation failed\r\n");
            return -1;
	}
        goto verify;
    }

    return status;
}

status_t BISS_Readout_Multi_Single(encoder_biss_t *enc, biss_multi_single_t *abs_data)
{
    status_t status;
    static uint32_t dataBuffer[BISS_FRAME_BUFFER_NUM];

    FLEXIO_BISS_GenClock(enc->controller, FLEXIO_BISS_CalClockValue(&frame_g, false));
    status = FLEXIO_BISS_ReadBlocking(enc->controller, dataBuffer, BISS_FRAME_BUFFER_NUM);
    if (status == kStatus_Success)
    {
        PRINTF("[OK] ");
    }
    PRINTF("Shifer1 = 0x%08X; Shifter0 = 0x%08X\r\n", dataBuffer[0], dataBuffer[1]);

    BISS_Data_Parser((uint64_t *)dataBuffer, abs_data);
    return status;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize BiSS encoder */
    uint32_t clock;
    flexio_biss_config_t devConfig;
    FLEXIO_BISS_Type encDev = {
        .flexioBase       = BOARD_FLEXIO_BASE,
        .frame            = &frame_g,
        .dataPin          = FLEXIO_BISS_DATA_PIN,
        .clockPin         = FLEXIO_BISS_CLOCK_PIN,
        .shifterBaseIndex = FLEXIO_BISS_BASE_SHIFTER,
        .shifterCount     = BISS_SHIFTER_NUM,
        .timerBaseIndex   = FLEXIO_BISS_BASE_TIMER,
        .timerCount       = BISS_TIMER_NUM
    };
    encoder_biss_t encoder = {
        .controller = &encDev
    };
    biss_multi_single_t abs_data;
    status_t status;

    BOARD_InitHardware();
    clock = FLEXIO_CLOCK_FREQUENCY / 1000000;

    PRINTF("%s %s\r\n", __DATE__, __TIME__);
    PRINTF("Encoder BiSS example\r\n");
    PRINTF("FlexIO Root Clock is %d MHz\r\n", clock);

    /*
     * Config->enableBiSS       = true;
     * Config->enableInDoze     = false;
     * Config->enableInDebug    = true;
     * Config->enableFastAccess = false;
     * Config->baudRate_bps     = 10000000;
     * Config->userMode         = kFLEXIO_BISS_USERMODE_ONESHOT;
     */
    FLEXIO_BISS_MasterGetDefaultConfig(&devConfig);

    status = FLEXIO_BISS_MasterInit(&encDev, &devConfig, FLEXIO_CLOCK_FREQUENCY);
    if (status != kStatus_Success)
    {
        return -1;
    }

    /* It must be after FLEXIO_BISS_MasterInit */
    encoder.singleTurnRevolution  = frame_g.stBits;
    encoder.multiTunrRevolution   = frame_g.mtBits;
    encoder.single_turn_sign_mask = BISS_FRAME_MASK(frame_g.stBits);
    encoder.multi_turn_sign_mask  = BISS_FRAME_MASK(frame_g.mtBits);

    BISS_VerifyFrame(&encoder);

    PRINTF("\r\n******************** Running the loop test ********************\r\n");
    while (1)
    {
        PRINTF("    ====    \r\n");
        /* Delay at least 500 ms. */
        SDK_DelayAtLeastUs(500000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        BISS_Readout_Multi_Single(&encoder, &abs_data);
    }
}
