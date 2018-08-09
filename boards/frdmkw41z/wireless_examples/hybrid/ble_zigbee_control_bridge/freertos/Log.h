/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/** @{ Define log levels corresponding to Linux syslog */
#define LOG_EMERG    0
#define LOG_ALERT    1
#define LOG_CRIT     2
#define LOG_ERR	     3
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
#define vLog_Printf(STREAM, LEVEL, FORMAT, ARGS...)  DBG_vPrintf((STREAM && (LEVEL <= LOG_LEVEL)), FORMAT, ##ARGS)

#else
/* When logging via Serial link to host syslog, send the log level as a char integer at the start of the message */
#define QUOTE(A) #A
#define CHAR(A) QUOTE(\x##A)
#define vLog_Printf(STREAM, LEVEL, FORMAT, ARGS...)  DBG_vPrintf((STREAM && (LEVEL <= LOG_LEVEL)), CHAR(LEVEL) FORMAT, ##ARGS)

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

