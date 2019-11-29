/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
     
/*!=============================================================================
\file       Log.h
\brief      
==============================================================================*/
     
#ifndef LOG_H_
#define LOG_H_

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include <dbg.h>
#include "SerialLink.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/** @{ Define log levels corresponding to Linux syslog */
#define LOG_EMERG    0
#define LOG_ALERT    1
#define LOG_CRIT     2
#define LOG_ERR      3
#define LOG_WARNING  4
#define LOG_NOTICE   5
#define LOG_INFO     6
#define LOG_DEBUG    7
/** @} */


/* Default log level */
#define LOG_LEVEL   LOG_INFO

/* The log macros filter out log messages that have a higher log level than that set in LOG_LEVEL */
#ifdef UART_DEBUGGING
/* When logging via UART, we don't print the level */
#define vLog_Printf(STREAM, LEVEL, FORMAT, ARGS...)\
    do {                                                            \
        if(STREAM)                                                  \
            vSL_WriteLog((FORMAT), ## ARGS);                         \
        else                                                        \
            APP_vPrintfNullImpl((STREAM), ##ARGS);                 \
    } while(0)

#else
/* When logging via Serial link to host syslog, send the log level as a char integer at the start of the message */
#define QUOTE(A) #A
#define CHAR(A) QUOTE(\x##A)
#define vLog_Printf(STREAM, LEVEL, FORMAT, ARGS...)\
    do {                                                            \
        if(STREAM && (LEVEL <= LOG_LEVEL))                                                  \
            vSL_WriteLog(CHAR(LEVEL) FORMAT, ##ARGS);                         \
        else                                                        \
            APP_vPrintfNullImpl((STREAM), ## ARGS);                 \
    } while(0)

#endif


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void vSL_LogSend(void);
PUBLIC void vSL_LogInit(void);
PUBLIC void vSL_LogFlush(void);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif /* LOG_H_ */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

