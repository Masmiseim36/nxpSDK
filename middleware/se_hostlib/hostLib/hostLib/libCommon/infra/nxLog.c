/* Copyright 2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <nxLog.h>
#include <stdarg.h>
#include <stdio.h>
#include <inttypes.h>

#include "sm_printf.h"
#if defined(_MSC_VER)
#include <windows.h>
#endif

#if defined (NONSECURE_WORLD)
#include "veneer_printf_table.h"
#endif

#define COLOR_RED "\033[0;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_RESET "\033[0m"

#define szCRLF "\r\n"
#define szLF "\n"

static void setColor(int level);
static void reSetColor(void);

#if defined(_MSC_VER)
static HANDLE sStdOutConsoleHandle = INVALID_HANDLE_VALUE;
static void msvc_setColor(int level);
static void msvc_reSetColor(void);
#define szEOL szLF
#endif

#if defined(__GNUC__)
static void ansi_setColor(int level);
static void ansi_reSetColor(void);
#if AX_EMBEDDED
#define szEOL szCRLF
#else
#define szEOL szLF
#endif
#endif /* __GNUC__ */

#ifndef szEOL
#define szEOL szCRLF
#endif

#define COMPRESSED_LOGGING_STYLE 0

#if NX_LOG_SHORT_PREFIX
static const char *szLevel[] = {"D", "I", "W", "E"};
#else
static const char *szLevel[] = {"DEBUG", "INFO ", "WARN ", "ERROR"};
#endif

#if AX_EMBEDDED
#define TAB_SEPRATOR "\t"
#else
#define TAB_SEPRATOR "   "
#endif


void nLog(const char *comp, int level, const char *format, ...)
{
#if defined (NONSECURE_WORLD)
	/* Logging for LPC55s from Non secure world */
	DbgConsole_Printf_NSE(comp);
	DbgConsole_Printf_NSE(":");
	DbgConsole_Printf_NSE(szLevel[level]);
	DbgConsole_Printf_NSE(":");
    if (format == NULL) {
        /* Nothing */
    }
    else if (format[0] == '\0') {
        /* Nothing */
    }
    else {
        char buffer[256];
        size_t size_buff = sizeof(buffer) / sizeof(buffer[0]) - 1;
        va_list vArgs;
        va_start(vArgs, format);
        vsnprintf(buffer, size_buff, format, vArgs);
        va_end(vArgs);
        DbgConsole_Printf_NSE(buffer);
    }
    DbgConsole_Printf_NSE(szEOL);
#else
    setColor(level);
    PRINTF("%10s:%s:", comp, szLevel[level]);
    if (format == NULL) {
        /* Nothing */
    }
    else if (format[0] == '\0') {
        /* Nothing */
    }
    else {
        char buffer[256];
        size_t size_buff = sizeof(buffer) / sizeof(buffer[0]) - 1;
        va_list vArgs;
        va_start(vArgs, format);
        vsnprintf(buffer, size_buff, format, vArgs);
        va_end(vArgs);
        PRINTF("%s", buffer);
    }
    reSetColor();
    PRINTF(szEOL);
#endif //defined (NONSECURE_WORLD)
}

#if 0
#if !(defined SDK_OS_FREE_RTOS)
void vLoggingPrintf(const char *pcFormat, ...) {
    char buffer[256];
    va_list vArgs;
    size_t size_buff = sizeof(buffer) / sizeof(buffer[0]) - 1;
    va_start(vArgs, pcFormat);
    setColor(NX_LEVEL_DEBUG);
    PRINTF("%10s:%s:", "AWS", szLevel[NX_LEVEL_DEBUG]);
    va_start(vArgs, pcFormat);
    vsnprintf(buffer, size_buff, pcFormat, vArgs);
    PRINTF("%s", buffer);
    va_end(vArgs);
    reSetColor();
    PRINTF(szEOL);
    va_end(vArgs);
}
#endif // SDK_OS_FREE_RTOS
#endif // 0

void nLog_au8(const char *comp, int level, const char *message, const unsigned char *array, size_t array_len)
{
    size_t i;
    setColor(level);
    PRINTF("%10s:%s:%s (Len=%" PRId32 ")", comp, szLevel[level], message, (int32_t)array_len);
    for (i = 0; i < array_len; i++) {
        if (0 == (i % 16)) {
            PRINTF(szEOL);
            if (0 == i) {
#if COMPRESSED_LOGGING_STYLE
                PRINTF("=>");
#endif
                PRINTF(TAB_SEPRATOR);
            }
            else {
                PRINTF(TAB_SEPRATOR);
            }
        }
#if !COMPRESSED_LOGGING_STYLE
        if (0 == (i % 4)) {
            PRINTF(TAB_SEPRATOR);
        }
#endif
        PRINTF("%02X ", array[i]);
    }
    reSetColor();
    PRINTF(szEOL);
}

static void setColor(int level)
{
#if defined(_MSC_VER)
    msvc_setColor(level);
#endif
#if defined(__GNUC__)
    ansi_setColor(level);
#endif
}

static void reSetColor(void)
{
#if defined(_MSC_VER)
    msvc_reSetColor();
#endif
#if defined(__GNUC__)
    ansi_reSetColor();
#endif
}

#if defined(_MSC_VER)
static void msvc_setColor(int level)
{
    WORD wAttributes = 0;
    if (sStdOutConsoleHandle == INVALID_HANDLE_VALUE) {
        sStdOutConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    switch (level) {
    case NX_LEVEL_ERROR:
        wAttributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
        break;
    case NX_LEVEL_WARN:
        wAttributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;
    case NX_LEVEL_INFO:
        wAttributes = FOREGROUND_GREEN;
        break;
    case NX_LEVEL_DEBUG:
        /* As of now put color here. All normal printfs would be in WHITE
             * Later, remove this color.
             */
        wAttributes = FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    default:
        wAttributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }
    SetConsoleTextAttribute(sStdOutConsoleHandle, wAttributes);
}

static void msvc_reSetColor()
{
    msvc_setColor(-1 /* default */);
}
#endif

#if defined(__GNUC__)
static void ansi_setColor(int level)
{
    switch (level) {
    case NX_LEVEL_ERROR:
        PRINTF(COLOR_RED);
        break;
    case NX_LEVEL_WARN:
        PRINTF(COLOR_YELLOW);
        break;
    case NX_LEVEL_INFO:
        PRINTF(COLOR_BLUE);
        break;
    case NX_LEVEL_DEBUG:
        /* As of now put color here. All normal printfs would be in WHITE
             * Later, remove this color.
             */
        PRINTF(COLOR_GREEN);
        break;
    default:
        PRINTF(COLOR_RESET);
    }
}

static void ansi_reSetColor()
{
    PRINTF(COLOR_RESET);
}
#endif

#ifdef __cplusplus
}
#endif
