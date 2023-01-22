/*
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Logging includes. */
#include "logging_levels.h"

/* Standard includes. */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "fsl_debug_console.h"

#ifndef configLOGGING_MAX_MESSAGE_LENGTH
#error configLOGGING_MAX_MESSAGE_LENGTH must be defined in FreeRTOSConfig.h to use this logging file.  configLOGGING_MAX_MESSAGE_LENGTH sets the size of the buffer into which formatted text is written, so also sets the maximum log message length.
#endif

#ifndef configLOGGING_INCLUDE_TIME_AND_TASK_NAME
#error configLOGGING_INCLUDE_TIME_AND_TASK_NAME must be defined in FreeRTOSConfig.h to use this logging file.  Set configLOGGING_INCLUDE_TIME_AND_TASK_NAME to 1 to prepend a time stamp, message number and the name of the calling task to each logged message.  Otherwise set to 0.
#endif

/*
 * Wrapper functions for vsnprintf and snprintf to return the actual number of
 * characters written.
 *
 * From the documentation, the retrun value of vsnprintf/snprintf is:
 * 1. In case of success i.e. when the complete string is successfully written
 *    to the buffer, the return value is the number of characters written to the
 *    buffer not counting the terminating null character.
 * 2. In case when the buffer is not large enough to hold the complete string,
 *    the return value is the number of characters that would have been written
 *    if the buffer was large enough.
 * 3. In case of encoding error, a negative number is returned.
 *
 * These wrapper functions instead return the actual number of characters
 * written in all cases:
 * 1. In case of success i.e. when the complete string is successfully written
 *    to the buffer, these wrappers return the same value as from
 *    vsnprintf/snprintf.
 * 2. In case when the buffer is not large enough to hold the complete string,
 *    these wrapper functions return the number of actual characters written
 *    (i.e. n - 1) as opposed to the number of characters that would have been
 *    written if the buffer was large enough.
 * 3. In case of encoding error, these wrapper functions return 0 to indicate
 *    that nothing was written as opposed to negative value from
 *    vsnprintf/snprintf.
 */
static int vsnprintf_safe( char * s,
                           size_t n,
                           const char * format,
                           va_list arg );
static int snprintf_safe( char * s,
                          size_t n,
                          const char * format,
                          ... );
/*-----------------------------------------------------------*/

static int vsnprintf_safe( char * s,
                           size_t n,
                           const char * format,
                           va_list arg )
{
    int ret;

    ret = vsnprintf( s, n, format, arg );

    /* Check if the string was truncated and if so, update the return value
     * to reflect the number of characters actually written. */
    if( ret >= n )
    {
        /* Do not include the terminating NULL character to keep the behaviour
         * same as the standard. */
        ret = n - 1;
    }
    else if( ret < 0 )
    {
        /* Encoding error - Return 0 to indicate that nothing was written to the
         * buffer. */
        ret = 0;
    }
    else
    {
        /* Complete string was written to the buffer. */
    }

    return ret;
}

/*-----------------------------------------------------------*/

static int snprintf_safe( char * s,
                          size_t n,
                          const char * format,
                          ... )
{
    int ret;
    va_list args;

    va_start( args, format );
    ret = vsnprintf_safe( s, n, format, args );
    va_end( args );

    return ret;
}

/*-----------------------------------------------------------*/

static void prvLoggingPrintfCommon( uint8_t usLoggingLevel,
                                    const char * pcFile,
                                    size_t fileLineNo,
                                    const char * pcFormat,
                                    va_list args )
{
    size_t xLength = 0;
    char * pcPrintString = NULL;

    configASSERT( usLoggingLevel <= LOG_DEBUG );
    configASSERT( pcFormat != NULL );
    configASSERT( configLOGGING_MAX_MESSAGE_LENGTH > 0 );

    /* Allocate a buffer to hold the log message. */
    pcPrintString = pvPortMalloc( configLOGGING_MAX_MESSAGE_LENGTH );

    if( pcPrintString != NULL )
    {
        const char * pcLevelString = NULL;
        size_t ulFormatLen = 0UL;

        /* Add metadata of task name and tick time for a new log message. */
        if( strcmp( pcFormat, "\n" ) != 0 )
        {
            /* Add metadata of task name and tick count if config is enabled. */
#if ( configLOGGING_INCLUDE_TIME_AND_TASK_NAME == 1 )
            {
                const char * pcTaskName;
                const char * pcNoTask = "None";
                static BaseType_t xMessageNumber = 0;

                /* Add a time stamp and the name of the calling task to the
                 * start of the log. */
                if( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
                {
                    pcTaskName = pcTaskGetName( NULL );
                }
                else
                {
                    pcTaskName = pcNoTask;
                }

                xLength += snprintf_safe( pcPrintString, configLOGGING_MAX_MESSAGE_LENGTH, "%lu %lu [%s] ",
                                          ( unsigned long ) xMessageNumber++,
                                          ( unsigned long ) xTaskGetTickCount(),
                                          pcTaskName );
            }
#endif /* if ( configLOGGING_INCLUDE_TIME_AND_TASK_NAME == 1 ) */
        }

        /* Choose the string for the log level metadata for the log message. */
        switch( usLoggingLevel )
        {
            case LOG_ERROR:
                pcLevelString = "ERROR";
                break;

            case LOG_WARN:
                pcLevelString = "WARN";
                break;

            case LOG_INFO:
                pcLevelString = "INFO";
                break;

            case LOG_DEBUG:
                pcLevelString = "DEBUG";
        }

        /* Add the chosen log level information as prefix for the message. */
        if( ( pcLevelString != NULL ) && ( xLength < configLOGGING_MAX_MESSAGE_LENGTH ) )
        {
            xLength += snprintf_safe( pcPrintString + xLength, configLOGGING_MAX_MESSAGE_LENGTH - xLength, "[%s] ", pcLevelString );
        }

        /* If provided, add the source file and line number metadata in the message. */
        if( ( pcFile != NULL ) && ( xLength < configLOGGING_MAX_MESSAGE_LENGTH ) )
        {
            /* If a file path is provided, extract only the file name from the string
             * by looking for '/' or '\' directory seperator. */
            const char * pcFileName = NULL;

            /* Check if file path contains "\" as the directory separator. */
            if( strrchr( pcFile, '\\' ) != NULL )
            {
                pcFileName = strrchr( pcFile, '\\' ) + 1;
            }
            /* Check if file path contains "/" as the directory separator. */
            else if( strrchr( pcFile, '/' ) != NULL )
            {
                pcFileName = strrchr( pcFile, '/' ) + 1;
            }
            else
            {
                /* File path contains only file name. */
                pcFileName = pcFile;
            }

            xLength += snprintf_safe( pcPrintString + xLength, configLOGGING_MAX_MESSAGE_LENGTH - xLength, "[%s:%d] ", pcFileName, fileLineNo );
            configASSERT( xLength > 0 );
        }

        if( xLength < configLOGGING_MAX_MESSAGE_LENGTH )
        {
            xLength += vsnprintf_safe( pcPrintString + xLength, configLOGGING_MAX_MESSAGE_LENGTH - xLength, pcFormat, args );
        }

        /* Add newline characters if the message does not end with them.*/
        ulFormatLen = strlen( pcFormat );

        if( ( ulFormatLen >= 2 ) &&
            ( strncmp( pcFormat + ulFormatLen, "\r\n", 2 ) != 0 ) &&
            ( xLength < configLOGGING_MAX_MESSAGE_LENGTH ) )
        {
            xLength += snprintf_safe( pcPrintString + xLength, configLOGGING_MAX_MESSAGE_LENGTH - xLength, "%s", "\r\n" );
        }

        /* The standard says that snprintf writes the terminating NULL
         * character. Just re-write it in case some buggy implementation does
         * not. */
        configASSERT( xLength < configLOGGING_MAX_MESSAGE_LENGTH );
        pcPrintString[ xLength ] = '\0';

        /* Only send the buffer to the logging task if it is
         * not empty. */
        if( xLength > 0 )
        {
            PRINTF(pcPrintString);
        }

        /* The buffer was  sent, so it must be freed. */
        vPortFree( ( void * ) pcPrintString );
    }
}

/*-----------------------------------------------------------*/

void vLoggingPrintfError( const char * pcFormat,
                          ... )
{
    va_list args;

    va_start( args, pcFormat );
    prvLoggingPrintfCommon( LOG_ERROR, NULL, 0, pcFormat, args );

    va_end( args );
}

/*-----------------------------------------------------------*/

void vLoggingPrintfWarn( const char * pcFormat,
                         ... )
{
    va_list args;

    va_start( args, pcFormat );
    prvLoggingPrintfCommon( LOG_WARN, NULL, 0, pcFormat, args );

    va_end( args );
}

/*-----------------------------------------------------------*/

void vLoggingPrintfInfo( const char * pcFormat,
                         ... )
{
    va_list args;

    va_start( args, pcFormat );
    prvLoggingPrintfCommon( LOG_INFO, NULL, 0, pcFormat, args );

    va_end( args );
}

/*-----------------------------------------------------------*/

void vLoggingPrintfDebug( const char * pcFormat,
                          ... )
{
    va_list args;

    va_start( args, pcFormat );
    prvLoggingPrintfCommon( LOG_DEBUG, NULL, 0, pcFormat, args );

    va_end( args );
}

/*-----------------------------------------------------------*/

void vLoggingPrintfWithFileAndLine( const char * pcFile,
                                    size_t fileLineNo,
                                    const char * pcFormat,
                                    ... )
{
    configASSERT( pcFile != NULL );

    va_list args;

    va_start( args, pcFormat );
    prvLoggingPrintfCommon( LOG_NONE, pcFile, fileLineNo, pcFormat, args );

    va_end( args );
}

/*-----------------------------------------------------------*/

/*!
 * \brief Formats a string to be printed and sends it
 * to the print queue.
 *
 * Appends the message number, time (in ticks), and task
 * that called vLoggingPrintf to the beginning of each
 * print statement.
 *
 */
void vLoggingPrintf( const char * pcFormat,
                     ... )
{
    va_list args;

    va_start( args, pcFormat );
    prvLoggingPrintfCommon( LOG_NONE, NULL, 0, pcFormat, args );

    va_end( args );
}

/*-----------------------------------------------------------*/

void vLoggingPrint( const char * pcMessage )
{
    char * pcPrintString = NULL;
    size_t xLength = 0;

    xLength = strlen( pcMessage ) + 1;
    pcPrintString = pvPortMalloc( xLength );

    if( pcPrintString != NULL )
    {
        strncpy( pcPrintString, pcMessage, xLength );

        /* Send the string to the logging task for IO. */
        PRINTF(pcPrintString);

        /* The buffer was sent so must be freed. */
        vPortFree( ( void * ) pcPrintString );
    }
}

/*-----------------------------------------------------------*/
