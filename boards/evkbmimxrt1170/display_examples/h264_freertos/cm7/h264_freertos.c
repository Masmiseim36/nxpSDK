/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* FATFS includes. */
#include "ff.h"

#include "sdcard.h"
#include "display.h"
#include "h264_dec.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_FILE_BUF_SIZE  (32U * 1024U)
#define DEMO_FILE_BUF_COUNT (3U)

/* Structure to transfer file content between file_read_task and h264_decode_task. */
typedef struct
{
    uint8_t *data;      /* Pointer to data. */
    int32_t len;        /* Length of the data. */
    bool isEndOfFile;   /* Has reached file end. */
    bool isStartOfFile; /* Is start of file. */
} file_data_block_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void file_read_task(void *pvParameters);
static void h264_decode_task(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION(static FIL h264Fil);
SDK_ALIGN(static uint8_t s_fileBuf[DEMO_FILE_BUF_COUNT][DEMO_FILE_BUF_SIZE], FSL_FEATURE_L1DCACHE_LINESIZE_BYTE);
static QueueHandle_t s_fullFileDataBlockQueue;
static QueueHandle_t s_emptyFileDataBlockQueue;

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    s_fullFileDataBlockQueue = xQueueCreate(DEMO_FILE_BUF_COUNT, sizeof(file_data_block_t));
    if (NULL == s_fullFileDataBlockQueue)
    {
        PRINTF("File data queue creation failed!.\r\n");
        while (1)
            ;
    }

    s_emptyFileDataBlockQueue = xQueueCreate(DEMO_FILE_BUF_COUNT, sizeof(file_data_block_t));
    if (NULL == s_emptyFileDataBlockQueue)
    {
        PRINTF("File data queue creation failed!.\r\n");
        while (1)
            ;
    }

    if (xTaskCreate(file_read_task, "file_read_task", 1000, NULL, configMAX_PRIORITIES - 2, NULL) != pdPASS)
    {
        PRINTF("File read task creation failed!.\r\n");
        while (1)
            ;
    }

    if (xTaskCreate(h264_decode_task, "h264_decode_task", 8000, NULL, configMAX_PRIORITIES - 2, NULL) != pdPASS)
    {
        PRINTF("H264 decode task creation failed!.\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}

static void file_read_task(void *pvParameters)
{
    FRESULT error;
    uint16_t i = 0;
    file_data_block_t blk;
    UINT bytesRead;
    char h264FileName[10];
    bool fileStart;

    if (0 != MOUNT_SDCard())
    {
        PRINTF("SD card mount error. Demo stopped!");
        configASSERT(0);
    }

    /* Put all file data buffer to the Queue, for later use. */
    for (i = 0; i < DEMO_FILE_BUF_COUNT; i++)
    {
        blk.data = s_fileBuf[i];
        xQueueSendToBack(s_emptyFileDataBlockQueue, &blk, portMAX_DELAY);
    }

    i = 0;

    while (1)
    {
        /* format the filename */
        sprintf(h264FileName, "/%03d.264", i++);

        /* open it */
        error = f_open(&h264Fil, h264FileName, FA_READ | FA_OPEN_EXISTING);
        if (error != FR_OK)
        {
            i = 0;
            PRINTF("Could not open %s, will open 000.264 next ...\r\n", h264FileName);
            continue;
        }

        PRINTF("Start to read file %s ...\r\n", h264FileName);

        fileStart = true;

        while (1)
        {
            /* Get one empty block. */
            xQueueReceive(s_emptyFileDataBlockQueue, &blk, portMAX_DELAY);

            /* Read data into the block. */
            error = f_read(&h264Fil, blk.data, DEMO_FILE_BUF_SIZE, &bytesRead);
            if (error != FR_OK)
            {
                PRINTF("File read error\r\n");
            }

            blk.len           = bytesRead;
            blk.isEndOfFile   = (DEMO_FILE_BUF_SIZE > bytesRead);
            blk.isStartOfFile = fileStart;
            fileStart         = false;

            /* Send the full block to decode task. */
            xQueueSendToBack(s_fullFileDataBlockQueue, &blk, portMAX_DELAY);

            if (blk.isEndOfFile)
            {
                break;
            }
        }

        error = f_close(&h264Fil);
        if (error != FR_OK)
        {
            PRINTF("File close error\r\n");
        }
    }
}

static void h264_decode_task(void *pvParameters)
{
    file_data_block_t blk;

    if (0 != DEMO_InitDisplay())
    {
        PRINTF("Display initialization failed.\r\n");
        configASSERT(0);
    }

    if (0 != OpenH264_Init())
    {
        PRINTF("OpenH264 initialization failed.\r\n");
        configASSERT(0);
    }

    while (1)
    {
        /* Get one block with file data. */
        xQueueReceive(s_fullFileDataBlockQueue, &blk, portMAX_DELAY);

        OpenH264_Decode(blk.data, blk.len, blk.isStartOfFile, blk.isEndOfFile);

        /* Process done, put the block to empty block queue,
         * then file_read_task could use it.
         */
        xQueueSendToBack(s_emptyFileDataBlockQueue, &blk, portMAX_DELAY);
    }
}

/*!
 * @brief Malloc failed hook.
 */
void vApplicationMallocFailedHook(void)
{
    for (;;)
        ;
}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    /* If the buffers to be provided to the Idle task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    /* If the buffers to be provided to the Timer task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
