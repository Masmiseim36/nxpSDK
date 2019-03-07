/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
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

#include <stdio.h>
#include <string.h>
#include "fsl_card.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"
#include "ff.h"
#include "diskio.h"

#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* buffer size (in byte) for read/write operations */
#define BUFFER_SIZE (100U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static FATFS g_fileSystem; /* File system object */
static FIL g_fileObject;   /* File object */

SDK_ALIGN(uint8_t g_bufferWrite[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CAHCE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CAHCE, HOST_DMA_BUFFER_ADDR_ALIGN));
SDK_ALIGN(uint8_t g_bufferRead[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CAHCE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CAHCE, HOST_DMA_BUFFER_ADDR_ALIGN));

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    FRESULT error;
    DIR directory; /* Directory object */
    FILINFO fileInformation;
    UINT bytesWritten;
    UINT bytesRead;
    const TCHAR driverNumberBuffer[3U] = {MMCDISK + '0', ':', '/'};
    volatile bool failedFlag = false;
    char ch = '0';
    BYTE work[_MAX_SS];
    FRESULT result;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    SystemInitIrqTable();

    PRINTF("\r\nFATFS example to demonstrate how to use FATFS with MMC card.\r\n");

    /* Mount volume work area based on card. */
    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
    {
        PRINTF("Mount volume failed.\r\n");
        return -1;
    }

#if (_FS_RPATH >= 2)
    error = f_chdrive((char const *)&driverNumberBuffer[0]);
    if (error)
    {
        PRINTF("Change drive failed.\r\n");
        return -1;
    }
#endif

#if _USE_MKFS
    PRINTF("\r\nMake file system......The time may be long if the card capacity is big.\r\n");
    result = f_mkfs(driverNumberBuffer, FM_ANY, 0U, work, sizeof work);
    if (result)
    {
        PRINTF("Make file system failed.\r\n");
        return -1;
    }
#endif /* _USE_MKFS */

    PRINTF("\r\nCreate directory......\r\n");
    error = f_mkdir(_T("/dir_1"));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("Directory exists.\r\n");
        }
        else
        {
            PRINTF("Make directory failed.\r\n");
            return -1;
        }
    }

    PRINTF("\r\nCreate a file in that directory......\r\n");
    error = f_open(&g_fileObject, _T("/dir_1/f_1.dat"), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("File exists.\r\n");
        }
        else
        {
            PRINTF("Open file failed.\r\n");
            return -1;
        }
    }

    PRINTF("\r\nList the file in that directory......\r\n");
    if (f_opendir(&directory, "/dir_1"))
    {
        PRINTF("Open directory failed.\r\n");
        return -1;
    }

    for (;;)
    {
        error = f_readdir(&directory, &fileInformation);

        /* To the end. */
        if ((error != FR_OK) || (fileInformation.fname[0U] == 0U))
        {
            break;
        }

        if (fileInformation.fattrib & AM_DIR)
        {
            PRINTF("Directory file : %s.\r\n", fileInformation.fname);
        }
        else
        {
            PRINTF("General file : %s.\r\n", fileInformation.fname);
        }
    }

    memset(g_bufferWrite, 'a', sizeof(g_bufferWrite));
    g_bufferWrite[BUFFER_SIZE - 2U] = '\r';
    g_bufferWrite[BUFFER_SIZE - 1U] = '\n';

    PRINTF("\r\n Write/read file until encounters error......\r\n");
    while (true)
    {
        if (failedFlag || (ch == 'q'))
        {
            break;
        }

        PRINTF("\r\nWrite to above created file.\r\n");
        error = f_write(&g_fileObject, g_bufferWrite, sizeof(g_bufferWrite), &bytesWritten);
        if ((error) || (bytesWritten != sizeof(g_bufferWrite)))
        {
            PRINTF("Write file failed. \r\n");
            failedFlag = true;
            continue;
        }

        /* Moves the file pointer */
        if (f_lseek(&g_fileObject, 0U))
        {
            PRINTF("Set file pointer position failed.\r\n");
            failedFlag = true;
            continue;
        }

        PRINTF("Read from above created file.\r\n");
        memset(g_bufferRead, 0U, sizeof(g_bufferRead));
        error = f_read(&g_fileObject, g_bufferRead, sizeof(g_bufferRead), &bytesRead);
        if ((error) || (bytesRead != sizeof(g_bufferRead)))
        {
            PRINTF("Read file failed.\r\n");
            failedFlag = true;
            continue;
        }

        PRINTF("Compare the read/write content.\r\n");
        if (memcmp(g_bufferWrite, g_bufferRead, sizeof(g_bufferWrite)))
        {
            PRINTF("Compare read/write content isn't consistent.\r\n");
            failedFlag = true;
            continue;
        }
        PRINTF("The read/write content is consistent.\r\n");

        PRINTF("\r\nInput 'q' to quit read/write.\r\nInput other char to read/write file again.\r\n");
        ch = GETCHAR();
        PUTCHAR(ch);
    }
    PRINTF("\r\nThe example will not read/write file again.\r\n");

    if (f_close(&g_fileObject))
    {
        PRINTF("\r\nClose file failed.\r\n");
        return -1;
    }

    while (true)
    {
    }
}
