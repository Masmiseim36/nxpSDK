/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "display_support.h"
#include "fsl_debug_console.h"
#include "codec_api.h"
#include "display.h"
#include "FreeRTOS.h"
#include "task.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef DEMO_DECODE_BUF_SIZE
#define DEMO_DECODE_BUF_SIZE (128 * 1024)
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Search NALU in input data, return the NALU start index, if not found returns -1. */
static int search_nalu(const uint8_t *data, int32_t len);

extern "C" int OpenH264_Init(void);

extern "C" int OpenH264_Decode(const uint8_t *data, int len, bool isStartOfFile, bool isEndOfFile);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static ISVCDecoder *s_pDecoder = NULL;

/* Decode buffer placed in DTCM for better performance. */
#if (defined(__ICCARM__))
static uint8_t s_decodeBuf[DEMO_DECODE_BUF_SIZE] @"decode_buf";
#elif (defined(__CC_ARM))
__attribute__((section("decode_buf"), zero_init)) static uint8_t s_decodeBuf[DEMO_DECODE_BUF_SIZE];
#elif (defined(__ARMCC_VERSION))
__attribute__((section("decode_buf"))) static uint8_t s_decodeBuf[DEMO_DECODE_BUF_SIZE];
#elif (defined(__GNUC__))
__attribute__((section("decode_buf,\"aw\",%nobits @"))) static uint8_t s_decodeBuf[DEMO_DECODE_BUF_SIZE];
#else
#error Toolchain not supported.
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
static int search_nalu(const uint8_t *data, int32_t len)
{
    int i;

    /* parse NALU 00 00 00 01 or 00 00 01 */
    for (i = 1; i < len - 4; i++)
    {
        if ((data[i] == 0 && data[i + 1] == 0 && data[i + 2] == 0 && data[i + 3] == 1) ||
            (data[i] == 0 && data[i + 1] == 0 && data[i + 2] == 1))
        {
            return i;
        }
    }

    if (data[i] == 0 && data[i + 1] == 0 && data[i + 2] == 1)
    {
        return i;
    }

    return -1;
}

int OpenH264_Init(void)
{
    int result               = WelsCreateDecoder(&s_pDecoder);
    SDecodingParam sDecParam = {0};

    if (result == 0)
    {
        sDecParam.uiTargetDqLayer             = 255;
        sDecParam.eEcActiveIdc                = ERROR_CON_FRAME_COPY_CROSS_IDR;
        sDecParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_DEFAULT;
        result                                = s_pDecoder->Initialize(&sDecParam);
    }

    return (result == 0) ? 0 : -1;
}

int OpenH264_Decode(const uint8_t *data, int len, bool isStartOfFile, bool isEndOfFile)
{
    SBufferInfo sDstBufInfo = {0};

    int32_t copiedLen = 0;
    int32_t sliceSize = 0;
    uint8_t *dst[3];
    int32_t num_of_frames_in_buffer = 0;
    int32_t leftDataLen;

    static int32_t decodeBufStart = 0;
    static int32_t decodeBufEnd   = 0;

    /* Used for frame rate calculation. */
    static uint16_t frame                  = 0;
    static uint32_t frameRateCalcStartTick = 0;
    static uint32_t frameRateCalcEndTick   = 0;

    if (isStartOfFile)
    {
        decodeBufStart         = 0;
        decodeBufEnd           = 0;
        frame                  = 0;
        frameRateCalcStartTick = xTaskGetTickCount();
    }

    leftDataLen = len;

    while (leftDataLen > 0)
    {
        copiedLen = MIN(leftDataLen, ((int32_t)sizeof(s_decodeBuf) - decodeBufEnd));

        /* Copy the input data to the end of decode buffer. */
        memcpy(&s_decodeBuf[decodeBufEnd], data, copiedLen);

        decodeBufEnd += copiedLen;
        data += copiedLen;
        leftDataLen -= copiedLen;

        while (decodeBufStart < decodeBufEnd)
        {
            sliceSize = search_nalu(&s_decodeBuf[decodeBufStart], decodeBufEnd - decodeBufStart);

            /* Could not find NALU. */
            if (sliceSize < 0)
            {
                /* This is the file end part, pass them all to H264 decoder. */
                if (isEndOfFile && (0 == leftDataLen))
                {
                    PRINTF("Decode reached end of file\r\n");
                    sliceSize = decodeBufEnd - decodeBufStart;
                }
                else
                {
                    /* After searching the full buffer, no slice found, then drop the data in buffer. */
                    if ((decodeBufStart == 0) && (decodeBufEnd == sizeof(s_decodeBuf)))
                    {
                        PRINTF("Decode buffer should be enlarged, change the macro DEMO_DECODE_BUF_SIZE\r\n");

                        /* Drop the decode buffer, fill using left input data. */
                        decodeBufEnd   = 0;
                        decodeBufStart = 0;
                    }

                    /* Have processed all slice in the buffer. */
                    break;
                }
            }
            /* Slice size too small, skip it. */
            else if (sliceSize < 4)
            {
                decodeBufStart += sliceSize;
                continue;
            }

            /* Found NALU, decode. */
            if (s_pDecoder->DecodeFrameNoDelay(&s_decodeBuf[decodeBufStart], sliceSize, dst, &sDstBufInfo) == 0)
            {
                if (sDstBufInfo.iBufferStatus == 1)
                {
                    frame++;

                    if (frame >= 60)
                    {
                        frameRateCalcEndTick = xTaskGetTickCount();

                        PRINTF("Frame Update Rate: %dHz\r\n",
                               (frame * configTICK_RATE_HZ) / (frameRateCalcEndTick - frameRateCalcStartTick));

                        frameRateCalcStartTick = frameRateCalcEndTick;
                        frame                  = 0;
                    }

                    DEMO_DisplayFrame(
                        sDstBufInfo.UsrData.sSystemBuffer.iWidth, sDstBufInfo.UsrData.sSystemBuffer.iHeight,
                        sDstBufInfo.pDst[0], sDstBufInfo.pDst[1], sDstBufInfo.pDst[2],
                        sDstBufInfo.UsrData.sSystemBuffer.iStride[0], sDstBufInfo.UsrData.sSystemBuffer.iStride[1]);
                }
            }
            else
            {
                PRINTF("decode error\r\n");
            }

            decodeBufStart += sliceSize;
        }

        /* Move the left data in decode buffer to the start of decode buffer,
         * left input data will be appended to the end.
         */
        memcpy(s_decodeBuf, &s_decodeBuf[decodeBufStart], decodeBufEnd - decodeBufStart);
        decodeBufEnd -= decodeBufStart;
        decodeBufStart = 0;
    }

    if (isEndOfFile)
    {
        s_pDecoder->GetOption(DECODER_OPTION_NUM_OF_FRAMES_REMAINING_IN_BUFFER, &num_of_frames_in_buffer);

        for (int32_t i = 0; i < num_of_frames_in_buffer; i++)
        {
            dst[0] = NULL;
            dst[1] = NULL;
            dst[2] = NULL;

            s_pDecoder->FlushFrame(dst, &sDstBufInfo);

            if (sDstBufInfo.iBufferStatus == 1)
            {
                DEMO_DisplayFrame(sDstBufInfo.UsrData.sSystemBuffer.iWidth, sDstBufInfo.UsrData.sSystemBuffer.iHeight,
                                  sDstBufInfo.pDst[0], sDstBufInfo.pDst[1], sDstBufInfo.pDst[2],
                                  sDstBufInfo.UsrData.sSystemBuffer.iStride[0],
                                  sDstBufInfo.UsrData.sSystemBuffer.iStride[1]);
            }
        }
    }

    return 0;
}
