/*******************************************************************************
*
* Copyright 2004-2013 NXP Semiconductor, Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale License
* distributed with this Material.
* See the LICENSE file distributed for more details.
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
| includes
|----------------------------------------------------------------------------*/
#include "mcdrv.h"

/******************************************************************************
| select interrupt or poll-driven serial communication
|----------------------------------------------------------------------------*/
#define FMSTR_LONG_INTR        (0)                                              /* complete message processing in interrupt */
#define FMSTR_SHORT_INTR       (0)                                              /* SCI FIFO-queuing done in interrupt */
#define FMSTR_POLL_DRIVEN      (1)                                              /* no interrupt needed, polling only */

/******************************************************************************
| select communication interface (SCI or CAN)
|----------------------------------------------------------------------------*/
#define FMSTR_SCI_INTERRUPT    (47)

#define FMSTR_CAN_BASE         (0x400A4000)                                     /* MSCAN1 base on K40 */
#define FMSTR_CAN_RX_INTERRUPT (38)                                             /* HC12 only, MSCAN RX interrupt number (use PRM if not defined) */
#define FMSTR_CAN_TX_INTERRUPT (39)                                             /* HC12 only, MSCAN TX interrupt number (use PRM if not defined) */ 

#define FMSTR_SCI_TWOWIRE_ONLY (1)

#if (BOARD_FMSTR_UART_TYPE == BOARD_FMSTR_UART)
    #define FMSTR_USE_SCI          (1)                                          /* to selecet SCI communication interface */
    #define FMSTR_SCI_BASE 	   (BOARD_FMSTR_UART_PORT)                          /* SCI base for UART */
#elif (BOARD_FMSTR_UART_TYPE == BOARD_FMSTR_LPUART)
    #define FMSTR_USE_LPUART       (1)                                          /* to selecet LPUART communication interface */
    #define FMSTR_SCI_BASE 	   ((FMSTR_ADDR)(LPUART_Type *)((uint32_t)BOARD_FMSTR_UART_PORT+16))    /* SCI base for LPUART */
#else
    #define FMSTR_USE_SCI          (0)
    #define FMSTR_USE_LPUART       (0)
#endif

#define FMSTR_USE_FLEXCAN      (0)                                              /* to selecet FlexCAN communication interface */

/******************************************************************************
| input/output communication buffer size
|----------------------------------------------------------------------------*/
#define FMSTR_COMM_BUFFER_SIZE (0)                                              /* set to 0 for "automatic" */

/******************************************************************************
| receive FIFO queue size (use with FMSTR_SHORT_INTR only)
|----------------------------------------------------------------------------*/
#define FMSTR_COMM_RQUEUE_SIZE (32)                                             /* set to 0 for "default" */

/******************************************************************************
| support for Application Commands
|----------------------------------------------------------------------------*/
#define FMSTR_USE_APPCMD       (0)                                              /* enable/disable App.Commands support */
#define FMSTR_APPCMD_BUFF_SIZE (32)                                             /* App.Command data buffer size */
#define FMSTR_MAX_APPCMD_CALLS (4)                                              /* how many app.cmd callbacks? (0=disable) */

/******************************************************************************
| oscilloscope support
|----------------------------------------------------------------------------*/
#define FMSTR_USE_SCOPE        (1)                                              /* enable/disable scope support */
#define FMSTR_MAX_SCOPE_VARS   (8)                                              /* max. number of scope variables (2..8) */

/******************************************************************************
| recorder support
|----------------------------------------------------------------------------*/
#define FMSTR_USE_RECORDER     (1)                                              /* enable/disable recorder support */
#define FMSTR_MAX_REC_VARS     (8)                                              /* max. number of recorder variables (2..8) */
#define FMSTR_REC_OWNBUFF      (0)                                              /* use user-allocated rec. buffer (1=yes) */

/* built-in recorder buffer (use when FMSTR_REC_OWNBUFF is 0) */
#define FMSTR_REC_BUFF_SIZE    (2048)                                           /* built-in buffer size */

/* recorder time base, specifies how often the recorder is called in the user 
   application */ 
#define FMSTR_REC_TIMEBASE     FMSTR_REC_BASE_MICROSEC(100)                     /* 0 = "unknown" */

/******************************************************************************
| target-side address translation (TSA)
|----------------------------------------------------------------------------*/
#if (BOARD_FMSTR_USE_TSA)
	#define FMSTR_USE_TSA          (1)                                                /* enable TSA functionality */
	#define FMSTR_USE_TSA_SAFETY   (1)                                                /* enable access to TSA variables only */
	#define FMSTR_USE_TSA_INROM    (1)                                                /* TSA tables declared as const (put to ROM) */
#else
	#define FMSTR_USE_TSA          (0)                                                /* enable TSA functionality */
	#define FMSTR_USE_TSA_SAFETY   (0)                                                /* enable access to TSA variables only */
	#define FMSTR_USE_TSA_INROM    (0)                                                /* TSA tables declared as const (put to ROM) */
#endif

/******************************************************************************
| enable/disable read/write memory commands
|----------------------------------------------------------------------------*/
#define FMSTR_USE_READMEM      (1)                                              /* enable read memory commands */
#define FMSTR_USE_WRITEMEM     (1)                                              /* enable write memory commands */
#define FMSTR_USE_WRITEMEMMASK (1)                                              /* enable write memory bits commands */

/******************************************************************************
| enable/disable read/write variable commands (a bit faster than Read Mem)
|----------------------------------------------------------------------------*/
#define FMSTR_USE_READVAR      (0)                                              /* enable read variable fast commands */
#define FMSTR_USE_WRITEVAR     (0)                                              /* enable write variable fast commands */
#define FMSTR_USE_WRITEVARMASK (0)                                              /* enable write variable bits fast commands */

#define FMSTR_BUFFER_ACCESS_BY_FUNCT (1)

#endif /* _FREEMASTER_CFG_H_ */
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/
