/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "ff.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    FATFS FatFs;
    FIL file;
    char line[82];
    FRESULT result;
    BYTE work[FF_MAX_SS];

    char buffer[] = "Text in file.";
    UINT bw   = 0;

    BOARD_InitHardware();

    PRINTF("Ramdisk example.\r\n");

    PRINTF("Format ram disk.\r\n");
    result = f_mkfs("", 0, work, sizeof work);
    if (result != FR_OK)
    {
        PRINTF("File system registration failed.\r\n");
    }

    result = f_mount(&FatFs, "", 0);
    if (result != FR_OK)
    {
        PRINTF("Volume mount failed.\r\n");
    }

    PRINTF("Creating text file.\r\n");
    result = f_open(&file, "message.txt", FA_CREATE_NEW | FA_WRITE);
    if (result != FR_OK)
    {
        PRINTF("File opening failed.\r\n");
    }
    else
    {
        PRINTF("Writing to text file.\r\n");
        result = f_write(&file, buffer, strlen(buffer), &bw);
        if (result != FR_OK)
        {
            PRINTF("File writing failed.\r\n");
        }
        result = f_close(&file);
        if (result != FR_OK)
        {
            PRINTF("File closing failed.\r\n");
        }
    }


    PRINTF("Opening file for read.\r\n");
    result = f_open(&file, "message.txt", FA_READ);
    if (result != FR_OK)
    {
        PRINTF("File opening failed.\r\n");
    }
    else
    {
        while (f_gets(line, sizeof line, &file))
        {
            PRINTF(line);
        }
        result = f_close(&file);
        if (result != FR_OK)
        {
            PRINTF("File closing failed.\r\n");
        }
    }

    return 0;
}
