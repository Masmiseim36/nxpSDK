/*
 * \file hsdkThread.c
 * This is a source file for the OS abstraction threading module.
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

Thread HSDKCreateThread(void *(*startRoutine) (void *), void *arg)
{
    Thread t = CreateThread(NULL,
                            0,
                            (LPTHREAD_START_ROUTINE)startRoutine,
                            arg,
                            0,
                            NULL);

    if (t == NULL) {
        logMessage(HSDK_ERROR, "[hsdkThread]HSDKCreateThread", "Thread creation failed", HSDKThreadId());
    }

    return t;
}

int HSDKDestroyThread(Thread t)
{
    DWORD ret = WaitForSingleObject(t, INFINITE);
    if (ret != WAIT_OBJECT_0) {
        DWORD err = GetLastError();
        switch (ret) {
            case WAIT_ABANDONED:
                logMessage(HSDK_ERROR, "[hsdkThread]HSDKDestroyThread", "Wait abandoned", HSDKThreadId());
                break;
            case WAIT_TIMEOUT:
                logMessage(HSDK_ERROR, "[hsdkThread]HSDKDestroyThread", "Timeout on wait", HSDKThreadId());
                break;
            case WAIT_FAILED:
                logMessage(HSDK_ERROR, "[hsdkThread]HSDKDestroyThread", "WaitForSingleObject failed", HSDKThreadId());
                break;
        }
        return (int)err;
    }

    BOOL b = CloseHandle(t);
    if (b) {
        return ERROR_SUCCESS;
    } else {
        DWORD err = GetLastError();
        logMessage(HSDK_ERROR, "[hsdkThread]HSDKDestroyThread", "Failed to close thread", HSDKThreadId());
        return (int)err;
    }
}

int HSDKThreadId()
{
    return GetCurrentThreadId();
}

#elif __linux__ || __APPLE__

Thread HSDKCreateThread(void *(*startRoutine) (void *), void *arg)
{
    int ret;
    Thread thread;

    ret = pthread_create(&thread, NULL, startRoutine, arg);

    if (!ret) {
        return thread;
    } else {
        return 0;
    }
}

int HSDKDestroyThread(Thread thread)
{
    return pthread_join(thread, NULL);
}

int HSDKThreadId()
{
    return (int)pthread_self();
}
#endif
