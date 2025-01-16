/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/* FatFs includes. */
#include "ff.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NUM_WRITERS 3 /* Number of Writer Tasks */
#define TASK_PRIO   (configMAX_PRIORITIES - 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void read_task(void *pvParameters);
static void write_task(void *pvParameters);

/*******************************************************************************
 * Globals
 ******************************************************************************/
FATFS FatFs;  /* Work area (file system object) for logical drive */
FIL file;     /* File object */

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{

    BOARD_InitHardware();

    PRINTF("FatFs RAM disk FreeRTOS example.\r\n\r\n");

    if (xTaskCreate(read_task, "READ_TASK", configMINIMAL_STACK_SIZE + 200, NULL, TASK_PRIO, NULL) !=
            pdPASS)
        {
            PRINTF("READ_TASK creation failed!.\r\n");
            vTaskSuspend(NULL);
        }
        else
        {
            PRINTF("READ_TASK created.\r\n");
        }

    vTaskStartScheduler();
    for (;;)
        ;

    return 0;
}

/*!
 * @brief Task read_task.
 */
static void read_task(void *pvParameters)
{
    FRESULT result;
    char line[82];
    BYTE work[FF_MAX_SS];
    uint32_t i;
    uint32_t counter = 0;

    result = f_mount(&FatFs, "", 0);
    if (result != FR_OK)
    {
        PRINTF("READ_TASK: Volume mount failed.\r\n");
    }

    PRINTF("READ_TASK: Create an FAT volume on the RAM disk.\r\n");
    result = f_mkfs("", 0, work, sizeof work);
    if (result != FR_OK)
    {
        PRINTF("READ_TASK: File system registration failed.\r\n");
    }

    PRINTF("READ_TASK: Creating text file.\r\n");
    result = f_open(&file, "message.txt", FA_CREATE_NEW | FA_WRITE);
    if (result != FR_OK)
    {
        PRINTF("READ_TASK: File opening failed.\r\n");
    }
    else
    {
        PRINTF("READ_TASK: File created.\r\n");
        result = f_close(&file);
        if (result != FR_OK)
        {
            PRINTF("File closing failed.\r\n");
        }
    }

    for (i = 0; i < NUM_WRITERS; i++)
    {
        if (xTaskCreate(write_task, "WRITE_TASK", configMINIMAL_STACK_SIZE + 200, (void *)i, TASK_PRIO, NULL) !=
            pdPASS)
        {
            PRINTF("READ_TASK: WRITE_TASK %d creation failed!.\r\n", i);
            vTaskSuspend(NULL);
        }
        else
        {
            PRINTF("READ_TASK: WRITE_TASK %d created.\r\n", i);
        }
    }

    while (1) /* Stop program after several reads */
    {
        PRINTF("READ_TASK: Opening file for read.\r\n");
        result = f_open(&file, "message.txt", FA_READ);
        if (result != FR_OK)
        {
            PRINTF("READ_TASK: File opening failed.\r\n");
        }
        else
        {
            PRINTF("READ_TASK: File opened.\r\n");
            /* Read all lines and display it */
            while (f_gets(line, sizeof line, &file))
            {
                PRINTF(line);
                counter++;
            }
            PRINTF("\r\n");
            result = f_close(&file);
            if (result != FR_OK)
            {
                PRINTF("READ_TASK: File closing failed.\r\n");
            }
            else
            {
                PRINTF("READ_TASK: Closing file.\r\n");
            }
            PRINTF("\r\n");
        }
        if (counter >= 2)
        {
            vTaskEndScheduler();
        }
        taskYIELD();
    }
}

/*!
 * @brief Task write_task.
 */
static void write_task(void *pvParameters)
{
    FRESULT result;
    UINT  bw = 0;
    char text[][6] = {"AAAAA", "BBBBB", "CCCCC"};

    while (1)
    {

        PRINTF("WRITE_TASK: %d: Opening for write.\r\n", pvParameters);
        result = f_open(&file, "message.txt", FA_OPEN_APPEND | FA_WRITE);
        if (result != FR_OK)
        {
            PRINTF("WRITE_TASK: %d: File opening failed.\r\n", pvParameters);
        }
        else
        {
            PRINTF("WRITE_TASK: %d: File opened.\r\n", pvParameters);
            PRINTF("WRITE_TASK %d: Writing to text file.\r\n", pvParameters);
            PRINTF("%s\r\n", text[(uint32_t)pvParameters]);
            result = f_write(&file, text[(uint32_t)pvParameters], strlen(text[(uint32_t)pvParameters]), &bw);
            if (result != FR_OK)
            {
                PRINTF("WRITE_TASK %d: Writing failed.\r\n", pvParameters);
            }
            else
            {
                PRINTF("WRITE_TASK %d: Writing finished.\r\n", pvParameters);
            }
            PRINTF("WRITE_TASK %d: Closing file.\r\n", pvParameters);
            result = f_close(&file);
            if (result != FR_OK)
            {
                PRINTF("File closing failed.\r\n");
            }
        }


        PRINTF("\r\n");
        taskYIELD();
    }
}
