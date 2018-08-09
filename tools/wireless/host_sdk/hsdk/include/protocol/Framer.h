/*
 * \file Framer.h
 * This is the header file for the Framer module.
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

#ifndef __FRAMER_H__
#define __FRAMER_H__

/************************************************************************************
 ************************************************************************************
 * Include
 ************************************************************************************
 ********************************************************************************* */
#include "EventManager.h"
#include "hsdkOSCommon.h"
#include "MessageQueue.h"
#include "PhysicalDevice.h"
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
 * @brief A frame can be in the following states: valid, invalid (due to checksum),
 * junk (if not starting with the synchronization byte), incomplete, insufficient or
 * sufficient.
 */
typedef enum {
    VALID_FRAME,
    INVALID_CRC,
    JUNK_DATA,
    /** Partial frame status to indicate error or success in processing. */
    INSUFFICIENT_DATA,
    SUFFICIENT_DATA
} FrameStatus;

/**
 * @brief Structure to encompass a frame and its status.
 */
typedef struct {
    FrameStatus status; /**< The state of a the frame response. */
    void *frame;        /**< Pointer to the actual frame object. */
} FrameResponse;

/**
 * @brief The protocols supported by the library.
 */
typedef enum {
    FSCI,
    HCI,
    ASCII,
} FramerProtocol;

/**
 * @brief A structure for the framer object.
 */
typedef struct _Framer {
    /***********************************************************************
    Protocol related fields
    ************************************************************************/
    /** The number of bytes the length field has in protocols. */
    uint8_t lengthFieldSize;
    /** The number of bytes the CRC field has in protocols. */
    uint8_t crcFieldSize;
    /** The endianness of the framer, depends on the protocol. */
    endianness framerEndianness;

    /***********************************************************************
    Communication related fields.
    ************************************************************************/
    /** Pointer to the device with which the framer communicates. */
    void *physicalLayer;
    /** A pointer to the queue into which the device puts messages to be merged into
    protocol specific frames. */
    MessageQueue *queue;
    /** Pointer to the event manager. A outside module wanting to receive processed
    frames from this framer will subscribe with a callback to the EventManager. */
    EventManager *evtManager;

    /***********************************************************************
     Fields related to framer inner workings: threads, events, state.
    ************************************************************************/
    /** The thread of the framer. */
    Thread framerThread;
    /** Event to stop the thread on destruction. */
    Event stopThread;
    /** The current state the framer is in. It's a travesty to keep it an int but
    each specific implementation of a protocol state machine has a different enum. */
    int currentState;

    /***********************************************************************
     Framer function pointers
    ************************************************************************/
    /** Function pointer that returns the start state of each protocol. */
    int(*SMStartState)(void);
    /** Function pointer that returns the final state of each protocol. */
    int(*SMFinalState)(void);
    /** Continuation entry point in the state machine  evolution of the framer. A
    pointer to a specific function that handles each state. */
    FrameStatus(*StateMachineDispatch)(struct _Framer *framer, void **currentFrame, uint32_t *dataSize);
    /** Protocol specific function to create a byte array containing the data in the
    specific frame. */
    uint8_t *(*CreatePacket)(struct _Framer *, void *, uint32_t *);
} Framer;

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

/*! *********************************************************************************
 ************************************************************************************
 * Public prototypes
 ************************************************************************************
 ********************************************************************************* */
DLLEXPORT Framer *InitializeFramer(void *connDev, FramerProtocol protocol, uint8_t lengthFieldSize, uint8_t crcFieldSize, endianness endian);
DLLEXPORT int DestroyFramer(Framer *framer);
DLLEXPORT int SendFrame(Framer *framer, void *frame);
DLLEXPORT int SendBytes(Framer *framer, uint8_t *packet, uint32_t size);
DLLEXPORT void AttachToFramer(Framer *framer, void *observer, void(*Callback)(void *, void *));
DLLEXPORT void DetachFromFramer(Framer *framer, void *observer);
DLLEXPORT uint8_t *PackageFrame(Framer *framer, void *frame, uint32_t *size);
DLLEXPORT void SetLengthFieldSize(Framer *framer, uint8_t lengthFieldSize);
DLLEXPORT void SetCrcFieldSize(Framer *framer, uint8_t crcFieldSize);
DLLEXPORT void SetEndianness(Framer *framer, endianness endian);

uint8_t ReadSingleByte(MessageQueue *queue);
uint8_t *ReadMultiByte(MessageQueue *queue, uint32_t cbDemanded);
uint8_t *ReadDataUntilByte(MessageQueue *queue, uint16_t *cbSize, uint8_t startByte, uint8_t *found);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
