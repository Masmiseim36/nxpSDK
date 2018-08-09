/*
 * \file hsdkLogger.c
 * This is a source file for the Logger module.
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

#include "hsdkLogger.h"

#include "hsdkError.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define DEFAULT_LOG "hsdk.log"

#define LINE_SIZE 256
#define TAG_SIZE 64

Logger *logger = NULL;
FILE *logFile = NULL;
#ifdef USE_LOGGER
static Lock initialLock;
static int initialLockSet = 0;
#endif

#ifndef LOG_PRIORITY
#define LOG_PRIORITY 3
#endif

const char *prios[] = { "", "HSDK_ERROR", "HSDK_WARNING", "HSDK_INFO" };


static void *LoggerThreadRoutine(void *lpParameter)
{
    char *line = NULL;
    Event eventArray[2];

    eventArray[0] = logger->stopThread;
    eventArray[1] = logger->queue->sAnnounceData;

    int loop = 1;
    while (loop) {
        int triggeredEvent;
        int ret = HSDKWaitMultipleEvents(eventArray, 2, INFINITE_WAIT, &triggeredEvent);
        if (ret != HSDK_ERROR_SUCCESS) {
            loop = 0;
            continue;
        }

        switch (triggeredEvent) {
            case 0:
                loop = 0;
                break;

            case 1:
                line = (char *)MessageQueueGet(logger->queue);
                MessageQueueDecrementSize(logger->queue, 1);
                if (line != NULL) {
                    fprintf(logFile, "%s", line);
                    fflush(logFile);
                    free(line);
                }
#if defined(__linux__) || defined(__APPLE__)
                HSDKResetEvent(logger->queue->sAnnounceData);
#endif
        }
    }

    int size = MessageQueueGetContentSize(logger->queue);
    while (size > 0) {
        size--;
        char *line = (char *)MessageQueueGet(logger->queue);
        if (line != NULL) {
            fprintf(logFile, "%s", line);
            fflush(logFile);
            free(line);
        }
    }

    return NULL;
}

void logMessage(int prio, const char *messageTag, const char *message, int threadId)
{
#ifdef USE_LOGGER
    if (prio > LOG_PRIORITY) {
        return;
    }

    if (logger != NULL) {
        char *line = (char *)calloc(LINE_SIZE, sizeof(char));
        if (line) {
            sprintf(line, "%s - [%d] %s:%s\n", prios[prio], threadId, messageTag, message);
            MessageQueuePutWithSize(logger->queue, line, 1);
            HSDKReleaseSemaphore(logger->queue->sAnnounceData);
        }
    }
#endif
}

void initLogger(char *filename)
{
#ifdef USE_LOGGER
    if (logger == NULL) {
        if (!initialLockSet) {
            initialLock = HSDKCreateLock();
            initialLockSet = 1;
        }

        HSDKAcquireLock(initialLock);
        if (logger == NULL) {
            logger = (Logger *) calloc(1, sizeof(Logger));


            logger->queue = InitializeMessageQueue(256);
            if (logger->queue == NULL) {
                HSDKReleaseLock(initialLock);
                return;
            }

            logger->referenceCount = 0;
            logger->stopThread = HSDKCreateEvent(0);
            logger->loggerThread = HSDKCreateThread(LoggerThreadRoutine, NULL);

            if (filename)
                logFile = fopen(filename, "w");
            else
                logFile = fopen(DEFAULT_LOG, "w");
        }
        logger->referenceCount++;
        HSDKReleaseLock(initialLock);
        return;
    }

    HSDKAcquireLock(initialLock);
    logger->referenceCount++;
    HSDKReleaseLock(initialLock);

#endif
}

void closeLogger()
{
#ifdef USE_LOGGER
    if (logger->referenceCount) {
        HSDKAcquireLock(initialLock);
        if (logger->referenceCount) {
            logger->referenceCount--;
        }
        HSDKReleaseLock(initialLock);
    }

    if (!logger->referenceCount) {
        HSDKAcquireLock(initialLock);
        if (logger != NULL) {
            HSDKSignalEvent(logger->stopThread);
            HSDKDestroyThread(logger->loggerThread);
            HSDKDestroyEvent(logger->stopThread);
            DestroyMessageQueue(logger->queue);

            logger->queue = NULL;
            fclose(logFile);
            free(logger);
            logger = NULL;
            logFile = NULL;
        }
        HSDKReleaseLock(initialLock);
    }
#endif
}
