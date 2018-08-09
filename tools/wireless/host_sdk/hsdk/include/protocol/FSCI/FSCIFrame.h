/*
 * \file FsciFrame.h
 * This is the header file for the FsciFrame module.
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

#ifndef __FSCI_FRAME__
#define __FSCI_FRAME__

/************************************************************************************
 ************************************************************************************
 * Include
 ************************************************************************************
 ***********************************************************************************/
#include <stdint.h>
#include <time.h>

#include "Framer.h"
#include "utils.h"

#ifdef _WINDLL
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*! *********************************************************************************
 ************************************************************************************
 * Public type definitions
 ************************************************************************************
 ********************************************************************************* */
/**
 * @brief A structure for the FSCI protocol.
 */
typedef struct {
    uint8_t sync;       /**< The sync byte field. */
    uint8_t opGroup;    /**< The operation group of the command or event. */
    uint8_t opCode;     /**< The operation code of the command or event. */
    uint32_t length;    /**< The length of the payload of the FSCIFrame. */
    uint8_t *data;      /**< The payload of the FSCIFrame. */
    /*! The checksum value of the frame. A XOR CRC between the bytes in length, data,
     * opCode, opGroup. This value is modified with the virtualInterface value, adding
     * a second byte to the CRC field in case the virtualInterface is not 0
     */
    uint32_t crc;
    /*! Timestamp of the FSCIFrame. It is the recorded timestamp of the RX of the SYNC
     * byte of the current FSCI if it is a received frame. It is the creation time of a
     * TX frame.
     */
    time_t timeStamp;
    uint32_t index;             /**< The index in the global sequence of received packets of the packet containing the SYNC byte. */
    endianness endian;          /**< The endianness of the frame. */
    uint8_t virtualInterface;   /**< The virtual interface on which the FSCIFrame is going to operate. */
} FSCIFrame;

/*! *********************************************************************************
 ************************************************************************************
 * Public memory declarations
 ************************************************************************************
 ********************************************************************************* */

/*! *********************************************************************************
 ************************************************************************************
 * Public macros
 ************************************************************************************
 ********************************************************************************* */
/* The start byte of a FSCI frame. */
#define FSCI_SYNC_BYTE 0x02

/* Command field sizes. */
#define FSCI_SYNC_SIZE 1
#define FSCI_OGF_SIZE 1
#define FSCI_OCF_SIZE 1
#define MAX_CRC_SIZE 1
#define EXPECTED_CRC_SIZE 1

/*! *********************************************************************************
 ************************************************************************************
 * Public prototypes
 ************************************************************************************
 ********************************************************************************* */
DLLEXPORT FSCIFrame *CreateFSCIFrameAdHoc(uint8_t opGroup, uint8_t opCode, uint8_t *data, uint32_t length, uint8_t virtualId, uint8_t lengthFieldSize, endianness endian);
DLLEXPORT FSCIFrame *CreateFSCIFrame(Framer *framer, uint8_t opGroup, uint8_t opCode, uint8_t *data, uint32_t length, uint8_t virtualId);
DLLEXPORT FSCIFrame *CreateRawFSCIFrameAdHoc(uint8_t sync, uint8_t opGroup, uint8_t opCode, uint8_t *data, uint32_t length, uint32_t crc, uint8_t virtualId, endianness endian);
DLLEXPORT FSCIFrame *CreateRawFSCIFrame(Framer *framer, uint8_t sync, uint8_t opGroup, uint8_t opCode, uint8_t *data, uint32_t length, uint32_t crc, uint8_t virtualId);
DLLEXPORT void DestroyFSCIFrame(FSCIFrame *);
DLLEXPORT void PrintFSCIFrame(Framer *, FSCIFrame *);

#ifdef __cplusplus
}
#endif

#endif
