/*
 * The Clear BSD License
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _EMBEDDED_RPC__ERPC_COMMON_H_
#define _EMBEDDED_RPC__ERPC_COMMON_H_

#include <stdint.h>

/*!
 * @addtogroup infra
 * @{
 */

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/*! @brief eRPC status return codes. */
enum _erpc_status
{
    //! No error occurred.
    kErpcStatus_Success = 0,

    //! Generic failure.
    kErpcStatus_Fail = 1,

    //! Argument is an invalid value.
    kErpcStatus_InvalidArgument = 4,

    //! Operated timed out.
    kErpcStatus_Timeout = 5,

    //! Message header contains an unknown version.
    kErpcStatus_InvalidMessageVersion = 6,

    //! Expected a reply message but got another message type.
    kErpcStatus_ExpectedReply,

    //! Message is corrupted.
    kErpcStatus_CrcCheckFailed,

    //! Attempt to read or write past the end of a buffer.
    kErpcStatus_BufferOverrun,

    //! Could not find host with given name.
    kErpcStatus_UnknownName,

    //! Failed to connect to host.
    kErpcStatus_ConnectionFailure,

    //! Connected closed by peer.
    kErpcStatus_ConnectionClosed,

    //! Memory allocation error.
    kErpcStatus_MemoryError,

    //! Server is stopped.
    kErpcStatus_ServerIsDown,

    //! Transport layer initialization failed.
    kErpcStatus_InitFailed,

    //! Failed to receive data.
    kErpcStatus_ReceiveFailed,

    //! Failed to send data.
    kErpcStatus_SendFailed,

    //! Sending/Receiving callback function which is not defined in IDL.
    kErpcStatus_UnknownCallback,

    //! Calling eRPC function from another eRPC function. For more information see erpc_common.h.
    kErpcStatus_NestedCallFailure,

    //! When sending address from bigger architecture to smaller.
    kErpcStatus_BadAddressScale
};

/*! @brief Type used for all status and error return values. */
typedef int32_t erpc_status_t;

/*! @} */

#endif /* _EMBEDDED_RPC__ERPC_COMMON_H_ */
