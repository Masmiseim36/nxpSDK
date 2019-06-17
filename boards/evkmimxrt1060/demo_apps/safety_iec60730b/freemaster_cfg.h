/*******************************************************************************
*
* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* SPDX-License-Identifier: NXP Proprietary
*
*
****************************************************************************//*!
*
* @file     freemaster_cfg.h
*
* @brief    FreeMASTER Serial Communication Driver configuration file
*
******************************************************************************/
#ifndef _FREEMASTER_CFG_H_
#define _FREEMASTER_CFG_H_

/******************************************************************************
* Select interrupt or poll-driven serial communication
******************************************************************************/

#define FMSTR_LONG_INTR    0        // complete message processing in interrupt
#define FMSTR_SHORT_INTR   0        // SCI FIFO-queuing done in interrupt
#define FMSTR_POLL_DRIVEN  1        // no interrupt needed, polling only

/*****************************************************************************
* Select communication interface (SCI or CAN)
******************************************************************************/

#define FMSTR_SCI_TWOWIRE_ONLY (1)

#if (APPLICATION_UART_TYPE == UART_TYPE)
    #define FMSTR_USE_SCI      (1)               /* to selecet SCI communication interface */
    #define FMSTR_SCI_BASE 	   (APPLICATION_UART) /* SCI base for UART */
#elif (APPLICATION_UART_TYPE == LPUART_TYPE)
    #define FMSTR_USE_LPUART   (1)               /* to selecet LPUART communication interface */
    #define FMSTR_SCI_BASE 	   ((FMSTR_ADDR)(LPUART_Type *)((uint32_t)APPLICATION_UART+16)) /* SCI base for LPUART */
#else
    #define FMSTR_USE_SCI      (0)
    #define FMSTR_USE_LPUART   (0)
#endif

#define FMSTR_USE_FLEXCAN (0)
#define FMSTR_USE_PDBDM   (0) /* 1 for packet driven */ 

#else

#define FMSTR_SCI_BASE APLICATION_UART//0x4006A000u //  aplication uart basUART0 base

//JL comment
//#define FMSTR_SCI_TWOWIRE_ONLY   1

#define FMSTR_USE_SCI     1 //0       // To selecet SCI communication interface 
#define FMSTR_USE_FLEXCAN 0       // To selecet FlexCAN communication interface
#define FMSTR_USE_PDBDM   0 // 1 for packet driven

/******************************************************************************

* Input/output communication buffer size
******************************************************************************/

#define FMSTR_COMM_BUFFER_SIZE 0    // set to 0 for "automatic"

/******************************************************************************
* Receive FIFO queue size (use with FMSTR_SHORT_INTR only)
******************************************************************************/

//JL comment 32
#define FMSTR_COMM_RQUEUE_SIZE 0   // set to 0 for "default"

/*****************************************************************************
* Support for Application Commands 
******************************************************************************/

#define FMSTR_USE_APPCMD       0    // enable/disable App.Commands support
#define FMSTR_APPCMD_BUFF_SIZE 32   // App.Command data buffer size
#define FMSTR_MAX_APPCMD_CALLS 4    // how many app.cmd callbacks? (0=disable)

/*****************************************************************************
* Oscilloscope support
******************************************************************************/

#define FMSTR_USE_SCOPE       1     // enable/disable scope support
#define FMSTR_MAX_SCOPE_VARS  8     // max. number of scope variables (2..8)

/*****************************************************************************
* Recorder support
******************************************************************************/

#define FMSTR_USE_RECORDER    1     // enable/disable recorder support
#define FMSTR_MAX_REC_VARS    8     // max. number of recorder variables (2..8)
#define FMSTR_REC_OWNBUFF     0     // use user-allocated rec. buffer (1=yes)

// built-in recorder buffer (use when FMSTR_REC_OWNBUFF is 0)
#define FMSTR_REC_BUFF_SIZE   4096  // built-in buffer size

// recorder time base, specifies how often the recorder is called in the user app.
//#define FMSTR_REC_TIMEBASE    FMSTR_REC_BASE_MILLISEC(0) // 0 = "unknown"
#define FMSTR_REC_TIMEBASE    FMSTR_REC_BASE_MICROSEC(50) // 0 = "unknown"


/*****************************************************************************
* Target-side address translation (TSA)
******************************************************************************/

#define FMSTR_USE_TSA         0     // enable TSA functionality
//JL comment 0
#define FMSTR_USE_TSA_SAFETY  1     // enable access to TSA variables only
//JL comment 0
#define FMSTR_USE_TSA_INROM   1     // TSA tables declared as const (put to ROM)

/*****************************************************************************
* Enable/Disable read/write memory commands
******************************************************************************/

#define FMSTR_USE_READMEM      1    // enable read memory commands
#define FMSTR_USE_WRITEMEM     1    // enable write memory commands
#define FMSTR_USE_WRITEMEMMASK 1    // enable write memory bits commands

/*****************************************************************************
* Enable/Disable read/write variable commands (a bit faster than Read Mem)
******************************************************************************/

#define FMSTR_USE_READVAR      0    // enable read variable fast commands
#define FMSTR_USE_WRITEVAR     0    // enable write variable fast commands
#define FMSTR_USE_WRITEVARMASK 0    // enable write variable bits fast commands

//JL comment the whole line
//#define FMSTR_BUFFER_ACCESS_BY_FUNCT 1

#endif /* __FREEMASTER_CFG_H */

