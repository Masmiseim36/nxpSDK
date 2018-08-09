/*
 * \file RawFrame.c
 * This is a source file for the RawFrame module.
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
#include <stdlib.h>
#include <string.h>
#include "RawFrame.h"

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
RawFrame *CreateRawFrame(uint8_t *data, uint32_t size);

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
uint32_t RxIndex = 0;
uint32_t TxIndex = 0;

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief    Returns a FSCI-ACK frame.
*
* \param[in] lengthFieldSize    the size of the length field in bytes
*
* \return   a pointer to a byte array containing the data
********************************************************************************** */
uint8_t *GetAckFrame(uint8_t lengthFieldSize)
{
    static const uint8_t fsciAck1[6] =  { 0x02, 0xA4, 0xFD, 0x01, 0x00, 0x58 };
    static const uint8_t fsciAck2[7] =  { 0x02, 0xA4, 0xFD, 0x01, 0x00, 0x00, 0x58 };

    return (lengthFieldSize == 1) ? (uint8_t *)fsciAck1 : (uint8_t *)fsciAck2;
}

/*! *********************************************************************************
* \brief    Creates a received RawFrame. It increments rx counter
*
* \param[in,out] data
* \param[in,out] size
*
* \return   NULL on allocation failure, a pointer to a RawFrame object containing the
*           data
********************************************************************************** */
RawFrame *CreateRxRawFrame(uint8_t *data, uint32_t size)
{
    RawFrame *frame = CreateRawFrame(data, size);
    frame->packetIndex = RxIndex++;

    return frame;
}

/*! *********************************************************************************
* \brief    Creates a received RawFrame. It increments tx counter
*
* \param[in,out] data
* \param[in,out] size
*
* \return   NULL on allocation failure, a pointer to a RawFrame object containing the
*           data
********************************************************************************** */
RawFrame *CreateTxRawFrame(uint8_t *data, uint32_t size)
{
    RawFrame *frame = CreateRawFrame(data, size);
    frame->packetIndex = TxIndex++;
    return frame;
}


/*! *********************************************************************************
* \brief    Free the memory allocated for a RawFrame object.
*
* \param[in,out] frame
*
* \return   none
********************************************************************************** */
void DestroyRawFrame(RawFrame *frame)
{
    if (frame != NULL) {
        if (frame->aRawData != NULL) {
            free(frame->aRawData);
        }

        frame->aRawData = NULL;
        free(frame);
    }
}

RawFrame *CloneRawFrame(RawFrame *frame)
{
    RawFrame *newFrame = (RawFrame *)calloc(1, sizeof(RawFrame));

    if (!newFrame) {
        return NULL;
    }

    newFrame->timeStamp = frame->timeStamp;
    newFrame->aRawData = (uint8_t *)calloc(frame->cbTotalSize, sizeof(uint8_t));

    if (!newFrame->aRawData) {
        free(newFrame);
        return NULL;
    }

    memcpy(newFrame->aRawData, frame->aRawData, frame->cbTotalSize);

    newFrame->cbTotalSize = frame->cbTotalSize;
    newFrame->iCrtIndex = frame->iCrtIndex;
    newFrame->packetIndex = frame->packetIndex;

    return newFrame;
}


/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief    Creates a RawFrame. A RawFrame represents bytes received from the device
*           layer without any logical ordering into a format.
*
* \param[in,out] data
* \param[in,out] size
*
* \return   NULL on allocation failure, a pointer to a RawFrame object containing the
*           data
********************************************************************************** */
RawFrame *CreateRawFrame(uint8_t *data, uint32_t size)
{
    RawFrame *frame = (RawFrame *)calloc(1, sizeof(RawFrame));

    if (!frame) {
        return NULL;
    }

    frame->timeStamp = time(NULL);
    frame->aRawData = (uint8_t *)calloc(size, sizeof(uint8_t));

    if (!frame->aRawData) {
        free(frame);
        return NULL;
    }

    memcpy(frame->aRawData, data, size);

    frame->cbTotalSize = size;
    frame->iCrtIndex = 0;

    return frame;
}
