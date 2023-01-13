/*
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */
#include "FreeRTOS.h"
#include "FreeRTOS_CLI_Console.h"
#include "fsl_debug_console.h"

/**
 * @brief UART console blocking read for requested number of bytes.
 * Function blocks reading from UART console until the requested
 * number of bytes are read.
 */
static int32_t prvUARTConsoleRead( char * const pcInputBuffer,
                                   uint32_t ulInputBufferLen,
                                   uint32_t * pulBytesRead );

/**
 * @brief UART console blocking write for requested number of bytes.
 * Function returns only after the requested number of
 * bytes are written to console.
 */
static void prvUARTConsoleWrite( const char * const pcOutputBuffer,
                                 uint32_t xOutputBufferLen );

xConsoleIO_t uartConsoleIO =
{
    .getChar = DbgConsole_Getchar,
    .read    = prvUARTConsoleRead,
    .write   = prvUARTConsoleWrite
};

int32_t prvUARTConsoleRead( char * const pcInputBuffer,
                            uint32_t ulInputBufferLen,
                            uint32_t * pulBytesRead )
{
    uint32_t ulIndex = 0;
    int charRead;
    int32_t retStatus = 0;

    for( ulIndex = 0; ulIndex < ulInputBufferLen; ulIndex++ )
    {
        charRead = DbgConsole_Getchar();

        if( ( charRead >= 0 ) && ( charRead <= 255 ) )
        {
            pcInputBuffer[ ulIndex ] = ( char ) ( charRead );
        }
        else
        {
            retStatus = charRead;
            break;
        }
    }

    ( *pulBytesRead ) = ( ulIndex );

    return retStatus;
}

void prvUARTConsoleWrite( const char * const pcOutputBuffer,
                          uint32_t xOutputBufferLen )
{
    int32_t status;

    uint32_t index;

    if( xOutputBufferLen > 0 )
    {
        for( index = 0; index < xOutputBufferLen; index++ )
        {
            status = DbgConsole_Putchar( pcOutputBuffer[ index ] );

            if( status < 0 )
            {
                break;
            }
        }

        ( void ) DbgConsole_Flush();
    }
}
