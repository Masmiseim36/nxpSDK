/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_shell.h"
#include "fsl_semc.h"
#include "fsl_nand_flash.h"
#include "fsl_semc_nand_flash.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_SEMC                        SEMC
#define EXAMPLE_SEMC_START_ADDRESS          (0x80000000U)
#define EXAMPLE_SEMC_CLK_FREQ               CLOCK_GetFreq(kCLOCK_SemcClk)
#define EXAMPLE_SEMC_NAND_AXI_START_ADDRESS (0x9E000000U)
#define EXAMPLE_SEMC_NAND_IPG_START_ADDRESS (0x00000000U)
#define EXAMPLE_FLASH_PAGE_SIZE             (4096)
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
// semc_nand_timing_config_t timingConfig;

void delayUs(uint32_t delay_us)
{
    uint32_t s_tickPerMicrosecond = CLOCK_GetFreq(kCLOCK_CpuClk) / 1000000U;

    // Make sure this value is greater than 0
    if (!s_tickPerMicrosecond)
    {
        s_tickPerMicrosecond = 1;
    }
    delay_us = delay_us * s_tickPerMicrosecond;
    while (delay_us)
    {
        __NOP();
        delay_us--;
    }
}

semc_nand_config_t semcNandConfig = {
    .cePinMux          = kSEMC_MUXCSX0,                       /*!< The CE# pin mux setting. */
    .axiAddress        = EXAMPLE_SEMC_NAND_AXI_START_ADDRESS, /*!< The base address. */
    .axiMemsize_kbytes = 2 * 1024 * 1024,                     /*!< The memory size is 8*1024*2*1024*1024 = 16Gb. */
    .ipgAddress        = EXAMPLE_SEMC_NAND_IPG_START_ADDRESS, /*!< The base address. */
    .ipgMemsize_kbytes = 2 * 1024 * 1024,                     /*!< The memory size is 8*1024*2*1024*1024 = 16Gb. */
    .rdyactivePolarity = kSEMC_RdyActiveLow,                  /*!< Wait ready polarity. */
    .arrayAddrOption   = kSEMC_NandAddrOption_5byte_CA2RA3,
    .edoModeEnabled    = false,                 /*!< Address mode. */
    .columnAddrBitNum  = kSEMC_NandColum_12bit, /*!< 12bit + 1bit to access the spare area. */
    .burstLen          = kSEMC_Nand_BurstLen64, /*!< Burst length. */
    .portSize          = kSEMC_PortSize8Bit,    /*!< Port size. */
    .timingConfig      = NULL,
};

semc_mem_nand_config_t semcMemConfig = {
    .semcNandConfig   = &semcNandConfig,
    .delayUS          = delayUs,
    .onfiVersion      = kNandOnfiVersion_1p0,
    .readyCheckOption = kNandReadyCheckOption_SR,
    .eccCheckType     = kNandEccCheckType_DeviceECC,
};

nand_config_t nandConfig = {
    .memControlConfig = (void *)&semcMemConfig,
    .driverBaseAddr   = (void *)EXAMPLE_SEMC,
};

void BOARD_InitMem(void)
{
    semc_config_t config;

    /* Initializes the MAC configure structure to zero. */
    memset(&config, 0, sizeof(semc_config_t));

    /*
       Get default configuration:
       config->queueWeight.queueaEnable = true;
       config->queueWeight.queuebEnable = true;
    */
    SEMC_GetDefaultConfig(&config);
    /* Disable queue B weight, which is for AXI bus access to SDRAM slave. */
    config.queueWeight.queuebEnable = false;
    /* Initialize SEMC. */
    SEMC_Init(SEMC, &config);
    /* Set SEMC clk source for NAND flash memory controller usage. */
    semcMemConfig.clkSrc_Hz = CLOCK_GetRootClockFreq(kCLOCK_Root_Semc);
}

void BOARD_ReconfigMPU(void)
{
    /* Disable code bus cache */
    if (LMEM_PCCCR_ENCACHE_MASK == (LMEM_PCCCR_ENCACHE_MASK & LMEM->PCCCR))
    {
        /* Enable the processor code bus to push all modified lines. */
        LMEM->PCCCR |= LMEM_PCCCR_PUSHW0_MASK | LMEM_PCCCR_PUSHW1_MASK | LMEM_PCCCR_GO_MASK;
        /* Wait until the cache command completes. */
        while ((LMEM->PCCCR & LMEM_PCCCR_GO_MASK) != 0U)
        {
        }
        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        LMEM->PCCCR &= ~(LMEM_PCCCR_PUSHW0_MASK | LMEM_PCCCR_PUSHW1_MASK);
        /* Now disable the cache. */
        LMEM->PCCCR &= ~LMEM_PCCCR_ENCACHE_MASK;
    }

    /* Disable system bus cache */
    if (LMEM_PSCCR_ENCACHE_MASK == (LMEM_PSCCR_ENCACHE_MASK & LMEM->PSCCR))
    {
        /* Enable the processor system bus to push all modified lines. */
        LMEM->PSCCR |= LMEM_PSCCR_PUSHW0_MASK | LMEM_PSCCR_PUSHW1_MASK | LMEM_PSCCR_GO_MASK;
        /* Wait until the cache command completes. */
        while ((LMEM->PSCCR & LMEM_PSCCR_GO_MASK) != 0U)
        {
        }
        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        LMEM->PSCCR &= ~(LMEM_PSCCR_PUSHW0_MASK | LMEM_PSCCR_PUSHW1_MASK);
        /* Now disable the cache. */
        LMEM->PSCCR &= ~LMEM_PSCCR_ENCACHE_MASK;
    }

    /* Disable MPU */
    ARM_MPU_Disable();

    /* Region 9 setting: Memory with Normal type, not shareable, outer/inner write through. */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enables the processor system bus to invalidate all lines in both ways.
    and Initiate the processor system bus cache command. */
    LMEM->PSCCR |= LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK | LMEM_PSCCR_GO_MASK;
    /* Wait until the cache command completes */
    while ((LMEM->PSCCR & LMEM_PSCCR_GO_MASK) != 0U)
    {
    }
    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    LMEM->PSCCR &= ~(LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK);
    /* Now enable the system bus cache. */
    LMEM->PSCCR |= LMEM_PSCCR_ENCACHE_MASK;

    /* Enables the processor code bus to invalidate all lines in both ways.
    and Initiate the processor code bus code cache command. */
    LMEM->PCCCR |= LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK | LMEM_PCCCR_GO_MASK;
    /* Wait until the cache command completes. */
    while ((LMEM->PCCCR & LMEM_PCCCR_GO_MASK) != 0U)
    {
    }
    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    LMEM->PCCCR &= ~(LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK);
    /* Now enable the code bus cache. */
    LMEM->PCCCR |= LMEM_PCCCR_ENCACHE_MASK;
}

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
            SHELL_Printf("\r\nWrite file successfully, speed %.1fKB/S. \r\n", wr_speed);
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
            SHELL_Printf("\r\nWrite file successfully, speed %.1fKB/S. \r\n", wr_speed);
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
            SHELL_Printf("\r\nRead file successfully, speed %.1fKB/S. \r\n", re_speed);
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
    clock_root_config_t config = {0};
    config.mux                 = 7; /* SYS_PLL3_PFD0: 664.62MHz. */
    config.div                 = 4;

    BOARD_ConfigMPU();
    BOARD_ReconfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    /* Set semc clock to 166 MHz(rootclock/div: 664.62 / 4 = 166.1MHz).
       Note: SEMC max frequecny is 200MHz, its source selects from
        000 - OSC_RC_48M_DIV2(24MHz)
        001 - OSC_24M(24MHz)
        010 - OSC_RC_400M(400MHz)
        011 - OSC_RC_16M(16MHz)
        100 - SYS_PLL1_DIV5(200MHz)
        101 - SYS_PLL2(528MHz)
        110 - SYS_PLL2_PFD2(594MHz)
        111 - SYS_PLL3_PFD0(664.62MHz)
     */
    CLOCK_SetRootClock(kCLOCK_Root_Semc, &config);
    BOARD_InitMem();
    BOARD_InitDebugConsole();
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
