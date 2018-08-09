/*
 * \file utils.h
 * This is the header file for the helper functions.
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

#ifndef __UTILS__
#define __UTILS__

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WINDLL
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#ifdef __cplusplus
extern "C" {
#endif

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
/**
 * @brief Endianness types.
 */
typedef enum {
    _BIG_ENDIAN,
    _LITTLE_ENDIAN,
    _UNKNOWN_ENDIAN
} endianness;

/**
 * @brief Structure to store configuration parameters.
 */
typedef struct {
    uint8_t fsciTxAck;
    uint8_t numberOfRetries;
    int timeoutAckMs;
    uint8_t fsciRxAck;
} ConfigParams;

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
DLLEXPORT void Store16(uint16_t, uint8_t *, endianness);
DLLEXPORT void Store32(uint32_t, uint8_t *, endianness);
DLLEXPORT void Store64(uint64_t, uint8_t *, endianness);
DLLEXPORT uint16_t Read16(uint8_t *, endianness);
DLLEXPORT uint32_t Read32(uint8_t *, endianness);
DLLEXPORT uint64_t Read64(uint8_t *, endianness);
DLLEXPORT ConfigParams *ParseConfig(void);
DLLEXPORT char *UARTSystemPath(char *uartPath);

#ifdef __cplusplus
}
#endif

#endif
