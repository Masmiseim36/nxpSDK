/*
 * \file hsdkSemaphore.c
 * This is a source file for the OS abstraction synchronization module.
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

#include "hsdkOSCommon.h"
#include "hsdkLogger.h"

#ifdef _WIN32

Semaphore HSDKCreateSemaphore(uint32_t initialValue, uint32_t maxValue)
{
    Semaphore s = CreateSemaphore(NULL, initialValue, maxValue, NULL);
    return s;
}

int HSDKAcquireExplicitlySemaphore(Semaphore s)
{
    DWORD ret = WaitForSingleObject(s, INFINITE);
    if (ret != WAIT_OBJECT_0) {
        DWORD err = GetLastError();
        switch (ret) {
            case WAIT_ABANDONED:
                logMessage(HSDK_ERROR, "[hsdkSemaphore]HSDKAcquireExplicitlySemaphore", "Wait abandoned", HSDKThreadId());
                break;
            case WAIT_TIMEOUT:
                logMessage(HSDK_ERROR, "[hsdkSemaphore]HSDKAcquireExplicitlySemaphore", "Timeout on wait", HSDKThreadId());
                break;
            case WAIT_FAILED:
                logMessage(HSDK_ERROR, "[hsdkSemaphore]HSDKAcquireExplicitlySemaphore", "WaitForSingleObject failed", HSDKThreadId());
                break;
        }
        return (int)err;
    }

    return ERROR_SUCCESS;
}

int HSDKReleaseSemaphore(Semaphore s)
{
    BOOL b = ReleaseSemaphore(s, 1, NULL);
    if (b) {
        return ERROR_SUCCESS;
    } else {
        DWORD err = GetLastError();
        logMessage(HSDK_ERROR, "[hsdkSemaphore]HSDKReleaseSemaphore", "Failed to release semaphore", HSDKThreadId());
        return (int)err;
    }
}

int HSDKDestroySemaphore(Semaphore s)
{
    BOOL b = CloseHandle(s);
    if (b) {
        return ERROR_SUCCESS;
    } else {
        DWORD err = GetLastError();
        logMessage(HSDK_ERROR, "[hsdkSemaphore]HSDKDestroySemaphore", "Failed to close semaphore", HSDKThreadId());
        return (int)err;
    }
}


#elif __linux__ || __APPLE__

#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <fcntl.h>

/* NOTE: Using eventfd as a semaphore to use select on either events, files or semaphores. */

Semaphore HSDKCreateSemaphore(uint32_t initialValue, uint32_t maxValue)
{
    return HSDKCreateEvent(initialValue);
}

int HSDKAcquireExplicitlySemaphore(Semaphore s)
{
#ifdef _WIN32
    return HSDKWaitEvent(s, 0);
#else
    return 0;
#endif
}

int HSDKReleaseSemaphore(Semaphore s)
{
    return HSDKSignalEvent(s);
}

int HSDKDestroySemaphore(Semaphore s)
{
    return HSDKDestroyEvent(s);
}

#endif
