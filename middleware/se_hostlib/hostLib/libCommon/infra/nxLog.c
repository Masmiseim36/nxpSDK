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

#include <nxLog.h>
#include <stdio.h>
#include <stdarg.h>


#if NX_LOG_SHORT_PREFIX
static const char * szLevel[] = {
    "D",
    "I",
    "W",
    "E" };
#else
static const char * szLevel[] = {
    "DEBUG",
    "INFO ",
    "WARN ",
    "ERROR" };
#endif

void nLog(const char * comp, int level,
    const char * format, ... ) {
    va_list   vArgs;
    printf("%s:%s:", comp, szLevel[level]);
    va_start(vArgs, format);
    vprintf(format,vArgs);
    va_end(vArgs);
    printf("\r\n");
}

void nLog_au8(const char * comp, int level,
    const char * message,
    unsigned char * array,
    size_t array_len) {
    size_t i;
    printf("%s:%s:%s (Len=%d)", comp,
        szLevel[level], message, array_len);
    for (i = 0; i < array_len; i++) {
        if ( 0 == (i % 16 )) {
            printf("\r\n");
        }
        if ( 0 == (i % 4 )) {
            printf("\t");
        }
        printf("%02X ", array[i]);
    }
    printf("\r\n");
}
