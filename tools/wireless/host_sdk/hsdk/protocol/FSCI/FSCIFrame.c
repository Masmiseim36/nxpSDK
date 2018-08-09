/*
 * \file FsciFrame.c
 * This is a source file for the FSCIFrame module.
 *
 * The Clear BSD License
 * Copyright 2013-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the
 * disclaimer below) provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
 * GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/************************************************************************************
 *************************************************************************************
 * Include
 *************************************************************************************
 ************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "FSCIFrame.h"

/************************************************************************************
 *************************************************************************************
 * Private macros
 *************************************************************************************
 ************************************************************************************/

/************************************************************************************
 *************************************************************************************
 * Private prototypes
 *************************************************************************************
 ************************************************************************************/


/************************************************************************************
 *************************************************************************************
 * Private type definitions
 *************************************************************************************
 ************************************************************************************/

/************************************************************************************
 *************************************************************************************
 * Public memory declarations
 *************************************************************************************
 ************************************************************************************/

/************************************************************************************
 *************************************************************************************
 * Private memory declarations
 *************************************************************************************
 ************************************************************************************/

/************************************************************************************
 *************************************************************************************
 * Public functions
 *************************************************************************************
 ************************************************************************************/

/*! *********************************************************************************
 * \brief  Create a FSCI frame.
 *
 * \param[in] ogf               the opgroup of the frame
 * \param[in] ocf               the opcode of the frame
 * \param[in] data              the payload of the frame
 * \param[in] dataSize          the size of the payload
 * \param[in] virtualInterface  the ID of the pan the frame is intended for
 * \param[in] lengthFieldSize   the size of length field
 * \param[in] localEndian       the frame endianness
 *
 * \return NULL if allocation failed, a pointer to FSCIFrame otherwise
 ********************************************************************************** */
FSCIFrame *CreateFSCIFrameAdHoc(uint8_t ogf, uint8_t ocf, uint8_t *data, uint32_t dataSize, uint8_t virtualInterface, uint8_t lengthFieldSize, endianness localEndian)
{
    uint32_t i;
    uint8_t crc[2] = {0};
    uint8_t len[2] = {0};
    FSCIFrame *frame = (FSCIFrame *)calloc(1, sizeof(FSCIFrame));

    if (!frame) {
        return NULL;
    }

    frame->sync = FSCI_SYNC_BYTE;
    frame->opGroup = ogf;
    frame->opCode = ocf;
    frame->length = dataSize;

    if (dataSize) {
        frame->data = (uint8_t *)calloc(dataSize, sizeof(uint8_t));
        if (!frame->data) {
            free(frame);
            return NULL;
        }
        memcpy(frame->data, data, dataSize);
    } else {
        frame->data = NULL;
    }

    crc[0] = ogf;
    crc[0] ^= ocf;

    for (i = 0; i < dataSize; i++)
        crc[0] ^= data[i];

    Store16(dataSize, len, localEndian);

    for (i = 0; i < lengthFieldSize; i++) {
        crc[0] ^= len[i];
    }

    if (virtualInterface) {
        crc[1] = crc[0];
        crc[0] += virtualInterface;
        crc[1] ^= crc[0];
    }

    frame->crc = Read16(crc, localEndian);

    frame->virtualInterface = virtualInterface;
    frame->endian = localEndian;
    frame->timeStamp = time(NULL);

    return frame;
}

FSCIFrame *CreateFSCIFrame(Framer *framer, uint8_t ogf, uint8_t ocf, uint8_t *data, uint32_t dataSize, uint8_t virtualInterface)
{
    return CreateFSCIFrameAdHoc(ogf, ocf, data, dataSize, virtualInterface, framer->lengthFieldSize, framer->framerEndianness);
}

FSCIFrame *CreateRawFSCIFrameAdHoc(uint8_t sync, uint8_t opGroup, uint8_t opCode, uint8_t *data, uint32_t length, uint32_t crc, uint8_t virtualId, endianness endian)
{
    FSCIFrame *frame = (FSCIFrame *)calloc(1, sizeof(FSCIFrame));

    if (!frame) {
        return NULL;
    }

    frame->sync = sync;
    frame->opGroup = opGroup;
    frame->opCode = opCode;
    frame->length = length;

    if (length) {
        frame->data = (uint8_t *)calloc(length, sizeof(uint8_t));
        if (!frame->data) {
            free(frame);
            return NULL;
        }
        memcpy(frame->data, data, length);
    } else {
        frame->data = NULL;
    }

    frame->crc = crc;

    frame->virtualInterface = virtualId;
    frame->endian = endian;
    frame->timeStamp = time(NULL);

    return frame;
}

FSCIFrame *CreateRawFSCIFrame(Framer *framer, uint8_t sync, uint8_t opGroup, uint8_t opCode, uint8_t *data, uint32_t length, uint32_t crc, uint8_t virtualId)
{
    return CreateRawFSCIFrameAdHoc(sync, opGroup, opCode, data, length, crc, virtualId, framer->framerEndianness);
}


/*! *********************************************************************************
 * \brief  Print a FSCI frame. This function accommodates a specific format for
 * printing out the data.
 *
 * \param[in] framer    a pointer to the framer object
 * \param[in] frame     a pointer to the FSCIFrame to be printed
 *
 * \return none
 ********************************************************************************** */
void PrintFSCIFrame(Framer *framer, FSCIFrame *frame)
{
    if (frame) {
        uint32_t i;
        uint8_t len[2];
        uint8_t crc[2];
#ifdef DEBUG
        printf("Index: %d\n", frame->index);
#endif
        printf("%02X %02X %02X ", frame->sync, frame->opGroup, frame->opCode);

        Store16(frame->length, len, framer->framerEndianness);
        for (i = 0; i < framer->lengthFieldSize; i++)
            printf("%02X ", len[i]);
        for (i = 0; i < frame->length; i++)
            printf("%02X ", frame->data[i]);
        Store16(frame->crc, crc, framer->framerEndianness);
        for (i = 0; i < (uint32_t) ((frame->virtualInterface == 0) ? 1 : 2); i++)
            printf("%02X ", crc[i]);
        printf("\n\n");
    }
}

/*! *********************************************************************************
 * \brief  Deallocate memory required by a frame.
 *
 * \param[in] frame a pointer to the FSCIFrame to be printed
 *
 * \return none
 ********************************************************************************** */
void DestroyFSCIFrame(FSCIFrame *frame)
{
    if (frame) {
        if (frame->data) {
            free(frame->data);
        }
        frame->data = NULL;
        free(frame);
    }
}
