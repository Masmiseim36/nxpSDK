/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "fsl_debug_console.h"
#include "ff.h"
#include "diskio.h"
#include "fsl_nand_disk.h"
#include "pin_mux.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "board.h"
#include "app.h"
#include "fsl_shell.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_DIR_NAME             "/dir_1"
#define DEMO_WRITE_LINE_CONTENT   "NXP Nand flash management demo"
#define DEMO_READ_WRITE_LINE_SIZE (30U)
#define SHELL_Printf              PRINTF
/*! @brief Task stack size. */
#define GC_TASK_STACK_SIZE (2048U)
/*! @brief Task stack priority. */
#define GC_TASK_PRIORITY (configMAX_PRIORITIES - 1U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static shell_status_t GetFreeCluster(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t WriteNewFile(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t WriteAppendFile(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t ReadFile(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t ListFileInDir(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t DeleteFileInDir(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t FormatFileSystem(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t createFileSystem(void);

/*!
 * @brief shell task.
 *
 * @param pvParameters Task parameter.
 */
static void shell_task(void *pvParameters);
/*!
 * @brief garbage collection task.
 *
 * @param pvParameters Task parameter.
 */
static void gc_task(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static FATFS g_fileSystem; /* File system object */
static FIL g_fileObject;   /* File object */
extern struct dhara_map map;
const TCHAR g_driverNumberBuffer[3U] = {NANDDISK + '0', ':', '/'};

SHELL_COMMAND_DEFINE(free, "\r\n\"free\":\r\n return the free cluster of current file system\r\n", GetFreeCluster, 0);

SHELL_COMMAND_DEFINE(write,
                     "\r\n\"write arg1 arg2\":\r\n Usage:\r\n    arg1: 1|2|3|4...         "
                     "   line numbers to write\r\n    arg2: ***                   file name\r\n",
                     WriteNewFile,
                     2);

SHELL_COMMAND_DEFINE(writeappend,
                     "\r\n\"writeappend arg1 arg2\":\r\n Usage:\r\n    arg1: 1|2|3|4...         "
                     "   line numbers to write\r\n    arg2: ***                   file name\r\n",
                     WriteAppendFile,
                     2);

SHELL_COMMAND_DEFINE(read,
                     "\r\n\"read arg1 arg2\":\r\n Usage:\r\n    arg1: 1|2|3|4...         "
                     "   line numbers to read\r\n    arg2: ***                   file name\r\n",
                     ReadFile,
                     2);

SHELL_COMMAND_DEFINE(list, "\r\n\"list\":\r\nreturn a list of file in dir_1\r\n", ListFileInDir, 0);

SHELL_COMMAND_DEFINE(del,
                     "\r\n\"del arg1\":\r\n Usage:\r\n    arg1: ***                   file name\r\n",
                     DeleteFileInDir,
                     1);

SHELL_COMMAND_DEFINE(format, "\r\n\"format\":\r\nformat the file system\r\n", FormatFileSystem, 0);

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

/*!@brief dhara io semaphore*/
SemaphoreHandle_t s_dharaIoSemaphore;
/*******************************************************************************
 * Code
 ******************************************************************************/
static shell_status_t FormatFileSystem(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    shell_status_t status = kStatus_SHELL_Success;

    xSemaphoreTake(s_dharaIoSemaphore, portMAX_DELAY);

    SHELL_Printf("\r\n");

    status = createFileSystem();

    if (!status)
    {
        SHELL_Printf("\r\nFormat done.\r\n");
    }

    xSemaphoreGive(s_dharaIoSemaphore);

    return status;
}

static shell_status_t GetFreeCluster(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    DWORD freeCluster = 0U;
    FATFS *fs;
    shell_status_t status = kStatus_SHELL_Success;

    xSemaphoreTake(s_dharaIoSemaphore, portMAX_DELAY);

    if (FR_OK != f_getfree((char const *)&g_driverNumberBuffer[0U], &freeCluster, &fs))
    {
        PRINTF("\r\nGet free cluster failed.\r\n");
        xSemaphoreGive(s_dharaIoSemaphore);
        return kStatus_SHELL_Error;
    }

    SHELL_Printf("\r\nFile system free cluster: %d\r\n", freeCluster);

    xSemaphoreGive(s_dharaIoSemaphore);

    return status;
}

static shell_status_t ListFileInDir(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    DIR directory; /* Directory object */
    FILINFO fileInformation;
    FRESULT error;
    shell_status_t status = kStatus_SHELL_Success;

    xSemaphoreTake(s_dharaIoSemaphore, portMAX_DELAY);

    error = f_opendir(&directory, DEMO_DIR_NAME);
    if (error)
    {
        PRINTF("\r\nOpen directory failed, error code %d.\r\n", error);
        status = kStatus_SHELL_Error;
    }
    else
    {
        for (;;)
        {
            error = f_readdir(&directory, &fileInformation);

            /* To the end. */
            if ((error != FR_OK) || (fileInformation.fname[0U] == 0U))
            {
                SHELL_Printf("\r\nNo More file in directory\r\n");
                break;
            }

            if (fileInformation.fname[0] == '.')
            {
                continue;
            }

            if (fileInformation.fattrib & AM_DIR)
            {
                SHELL_Printf("\r\nDirectory file : %s.\r\n", fileInformation.fname);
            }
            else
            {
                SHELL_Printf("\r\nFile: name -> %s  size -> %d.\r\n", fileInformation.fname, fileInformation.fsize);
            }
        }
    }

    xSemaphoreGive(s_dharaIoSemaphore);

    return status;
}

static shell_status_t DeleteFileInDir(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    char *targetFile                        = argv[1];
    char tempBuf[DEMO_READ_WRITE_LINE_SIZE] = DEMO_DIR_NAME;
    shell_status_t status                   = kStatus_SHELL_Success;

    xSemaphoreTake(s_dharaIoSemaphore, portMAX_DELAY);
    sprintf(tempBuf + strlen(DEMO_DIR_NAME), "/%s", targetFile);

    if (f_unlink(tempBuf))
    {
        SHELL_Printf("\r\nDelete file failed.\r\n");
        status = kStatus_SHELL_Error;
    }

    SHELL_Printf("\r\nDelete file successfully.\r\n");

    xSemaphoreGive(s_dharaIoSemaphore);

    return status;
}

static shell_status_t WriteNewFile(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    FRESULT error;
    char *targetFile                        = argv[2];
    int32_t writeLines                      = ((int32_t)atoi(argv[1]));
    char tempBuf[DEMO_READ_WRITE_LINE_SIZE] = DEMO_DIR_NAME;
    uint32_t i                              = 0U;
    UINT bytesWritten                       = 0U;
    uint32_t startMs                        = 0U;
    uint32_t endMs                          = 0U;
    float wr_speed;
    float totalS          = 0U;
    shell_status_t status = kStatus_SHELL_Success;

    xSemaphoreTake(s_dharaIoSemaphore, portMAX_DELAY);

    sprintf(tempBuf + strlen(DEMO_DIR_NAME), "/%s", targetFile);

    startMs = xTaskGetTickCount();
    error   = f_open(&g_fileObject, _T(tempBuf), (FA_WRITE | FA_CREATE_ALWAYS));
    if (error)
    {
        if (error != FR_EXIST)
        {
            SHELL_Printf("\r\nOpen file failed, error code %d.\r\n", error);
            status = kStatus_SHELL_Error;
        }
    }
    else
    {
        for (i = 0U; i < writeLines; i++)
        {
            error = f_write(&g_fileObject, DEMO_WRITE_LINE_CONTENT, DEMO_READ_WRITE_LINE_SIZE, &bytesWritten);
            if ((error) || (bytesWritten != DEMO_READ_WRITE_LINE_SIZE))
            {
                error = FR_DISK_ERR;
                SHELL_Printf("\r\nWrite file failed. \r\n");
                break;
            }
        }
        endMs = xTaskGetTickCount();

        if (error == FR_OK)
        {
            totalS   = ((float)(endMs - startMs)) / 1000U;
            wr_speed = ((float)(writeLines * DEMO_READ_WRITE_LINE_SIZE) / 1024U) / totalS;
            SHELL_Printf("\r\nWrite file successfully, speed %.1fKB/S. \r\n", (double)wr_speed);
        }

        if (f_close(&g_fileObject))
        {
            SHELL_Printf("\r\nClose file failed.\r\n");
            status = kStatus_SHELL_Error;
        }
    }

    xSemaphoreGive(s_dharaIoSemaphore);

    return status;
}

static shell_status_t WriteAppendFile(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    FRESULT error;
    char *targetFile                        = argv[2];
    int32_t writeLines                      = ((int32_t)atoi(argv[1]));
    char tempBuf[DEMO_READ_WRITE_LINE_SIZE] = DEMO_DIR_NAME;
    uint32_t i                              = 0U;
    UINT bytesWritten                       = 0U;
    uint32_t startMs                        = 0U;
    uint32_t endMs                          = 0U;
    float wr_speed;
    float totalS          = 0U;
    shell_status_t status = kStatus_SHELL_Success;

    xSemaphoreTake(s_dharaIoSemaphore, portMAX_DELAY);

    sprintf(tempBuf + strlen(DEMO_DIR_NAME), "/%s", targetFile);

    startMs = xTaskGetTickCount();
    error   = f_open(&g_fileObject, _T(tempBuf), (FA_WRITE | FA_OPEN_APPEND));
    if (error)
    {
        if (error != FR_EXIST)
        {
            SHELL_Printf("\r\nOpen file failed, error code %d.\r\n", error);
            status = kStatus_SHELL_Error;
        }
    }
    else
    {
        for (i = 0U; i < writeLines; i++)
        {
            error = f_write(&g_fileObject, DEMO_WRITE_LINE_CONTENT, DEMO_READ_WRITE_LINE_SIZE, &bytesWritten);
            if ((error) || (bytesWritten != DEMO_READ_WRITE_LINE_SIZE))
            {
                SHELL_Printf("\r\nWrite file failed, error code %d, target size %d, actually size %d. \r\n", error,
                             DEMO_READ_WRITE_LINE_SIZE, bytesWritten);
                error = FR_DISK_ERR;
                break;
            }
        }
        endMs = xTaskGetTickCount();

        if (error == FR_OK)
        {
            totalS   = ((float)(endMs - startMs)) / 1000U;
            wr_speed = ((float)(writeLines * DEMO_READ_WRITE_LINE_SIZE) / 1024U) / totalS;
            SHELL_Printf("\r\nWrite file successfully, speed %.1fKB/S. \r\n", (double)wr_speed);
        }

        if (f_close(&g_fileObject))
        {
            SHELL_Printf("\r\nClose file failed.\r\n");
            status = kStatus_SHELL_Error;
        }
    }

    xSemaphoreGive(s_dharaIoSemaphore);

    return status;
}

static shell_status_t ReadFile(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    FRESULT error;
    char *targetFile                        = argv[2];
    int32_t readLines                       = ((int32_t)atoi(argv[1]));
    char tempBuf[DEMO_READ_WRITE_LINE_SIZE] = DEMO_DIR_NAME;
    uint32_t i                              = 0U;
    UINT bytesRead                          = 0U;
    uint32_t startMs                        = 0U;
    uint32_t endMs                          = 0U;
    float re_speed;
    float totalS          = 0U;
    shell_status_t status = kStatus_SHELL_Success;

    xSemaphoreTake(s_dharaIoSemaphore, portMAX_DELAY);

    sprintf(tempBuf + strlen(DEMO_DIR_NAME), "/%s", targetFile);

    startMs = xTaskGetTickCount();
    error   = f_open(&g_fileObject, _T(tempBuf), (FA_READ | FA_OPEN_EXISTING));
    if (error)
    {
        SHELL_Printf("\r\nOpen file failed, error code %d.\r\n", error);
        status = kStatus_SHELL_Error;
    }
    else
    {
        for (i = 0U; i < readLines; i++)
        {
            error = f_read(&g_fileObject, tempBuf, DEMO_READ_WRITE_LINE_SIZE, &bytesRead);
            if ((error) || (bytesRead != DEMO_READ_WRITE_LINE_SIZE))
            {
                SHELL_Printf("\r\nRead file failed, error code %d, target size %d, actually size %d. \r\n", error,
                             DEMO_READ_WRITE_LINE_SIZE, bytesRead);
                error = FR_DISK_ERR;
                break;
            }
        }
        endMs = xTaskGetTickCount();

        if (error == FR_OK)
        {
            totalS   = ((float)(endMs - startMs)) / 1000U;
            re_speed = ((float)(readLines * DEMO_READ_WRITE_LINE_SIZE) / 1024U) / totalS;
            SHELL_Printf("\r\nRead file successfully, speed %.1fKB/S. \r\n", (double)re_speed);
        }

        if (f_close(&g_fileObject))
        {
            SHELL_Printf("\r\nClose file failed.\r\n");
            status = kStatus_SHELL_Error;
        }
    }

    xSemaphoreGive(s_dharaIoSemaphore);

    return status;
}

static void shell_task(void *pvParameters)
{
    /* Init SHELL */
    s_shellHandle = &s_shellHandleBuffer[0];
    SHELL_Init(s_shellHandle, g_serialHandle, "SHELL>> ");

    /* Add new command to commands list */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(free));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(write));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(writeappend));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(read));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(list));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(del));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(format));

    while (1)
    {
        SHELL_Task(s_shellHandle);
    }
}

static void gc_task(void *pvParameters)
{
    dhara_error_t err;
    /* Block for 100ms. */
    const TickType_t xDelay = 100U / portTICK_PERIOD_MS;

    while (1)
    {
        xSemaphoreTake(s_dharaIoSemaphore, portMAX_DELAY);

        if (dhara_map_gc(&map, &err) < 0)
        {
            SHELL_Printf("Garbage collection failed, error code %d\r\n", err);
        }

        xSemaphoreGive(s_dharaIoSemaphore);

        vTaskDelay(xDelay);
    }
}

static shell_status_t createFileSystem(void)
{
    FRESULT error;
    BYTE work[FF_MAX_SS];

    /* Register new fs but doesn't mount it immediately. */
    error = f_mount(&g_fileSystem, g_driverNumberBuffer, 0U);
    if (error)
    {
        SHELL_Printf("\r\nMount volume failed, error %d.\r\n", error);
        return kStatus_SHELL_Error;
    }

#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&g_driverNumberBuffer[0U]);
    if (error)
    {
        SHELL_Printf("\r\nChange drive failed, error %d.\r\n", error);
        return kStatus_SHELL_Error;
    }
#endif
    SHELL_Printf("\r\nMake file system..............\r\n");
    error = f_mkfs(g_driverNumberBuffer, 0, work, FF_MAX_SS);
    if (error)
    {
        SHELL_Printf("\r\nMake file system failed, error %d.\r\n", error);
        return kStatus_SHELL_Error;
    }

    SHELL_Printf("\r\nCreate directory......\r\n");
    error = f_mkdir(_T(DEMO_DIR_NAME));
    if (error)
    {
        if (error == FR_EXIST)
        {
            SHELL_Printf("\r\nDirectory exists.\r\n");
        }
        else
        {
            SHELL_Printf("\r\nMake directory failed, error %d.\r\n", error);
            return kStatus_SHELL_Error;
        }
    }

    return kStatus_SHELL_Success;
}

/*! @brief Main function */
int main(void)
{
    BOARD_InitHardware();
    int status = 0;

    SHELL_Printf("\r\nnand flash management demo.\r\n");

    s_dharaIoSemaphore = xSemaphoreCreateMutex();
    /* create file system */
    status = createFileSystem();

    if (status < 0)
    {
        return -1;
    }

    if (pdPASS != xTaskCreate(shell_task, "shell_task", SHELL_TASK_STACK_SIZE, NULL, SHELL_TASK_PRIORITY, NULL))
    {
        SHELL_Printf("\r\nCreate task failed\r\n");
        return -1;
    }

    if (pdPASS != xTaskCreate(gc_task, "gc_task", GC_TASK_STACK_SIZE, NULL, GC_TASK_PRIORITY, NULL))
    {
        SHELL_Printf("\r\nCreate task failed\r\n");
        return -1;
    }

    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    /* Scheduler should never reach this point. */
    while (true)
    {
    }
}
