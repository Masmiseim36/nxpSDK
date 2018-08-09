/*
 * \file RawFrame.h
 * This is the header file for the RawFrame module.
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

#ifndef __RAWFRAME_H__
#define __RAWFRAME_H__

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WINDLL
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
/**
 * @brief Simple structure for encapsulating data. Has no protocol representation.
 */
typedef struct {
    uint32_t packetIndex;   /**< The index of the global count of RawFrames created. */
    uint8_t *aRawData;      /**< An array containing the data stored in the RawFrame. */
    uint32_t cbTotalSize;   /**< The size of the payload of the RawFrame. */
    uint32_t iCrtIndex;     /**< An index into the array used in processing the data contained within the structure. */
    time_t timeStamp;       /**< Timestamp of the creation of the RawFrame. */
} RawFrame;

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
uint8_t *GetAckFrame(uint8_t lengthFieldSize);
RawFrame *CreateTxRawFrame(uint8_t *data, uint32_t size);
RawFrame *CreateRxRawFrame(uint8_t *data, uint32_t size);
RawFrame *CloneRawFrame(RawFrame *frame);
DLLEXPORT void DestroyRawFrame(RawFrame *frame);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
