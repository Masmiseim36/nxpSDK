/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************************************************************/ /*!
                                                                               *
                                                                               * @file   freemaster_cfg.h
                                                                               *
                                                                               * @brief  FreeMASTER Serial Communication
                                                                               *Driver configuration file
                                                                               *
                                                                               *******************************************************************************/

#ifndef _FREEMASTER_CFG_H_
#define _FREEMASTER_CFG_H_

/******************************************************************************
 * Select interrupt or poll-driven serial communication
 ******************************************************************************/
#define FMSTR_PLATFORM_CORTEX_M 1 /* Cortex-M platform (see freemaster.h for list of all supported platforms) */

//! Enable/Disable FreeMASTER functionalities
#define FMSTR_DISABLE 0 /* To disable all FreeMASTER functionalities */

#define FMSTR_LONG_INTR 0   /* complete message processing in interrupt */
#define FMSTR_SHORT_INTR 0  /* SCI FIFO-queuing done in interrupt */
#define FMSTR_POLL_DRIVEN 1 /* no interrupt needed, polling only */

/*****************************************************************************
 * Select communication interface (SCI, CAN, USB CDC or Packet Driven BDM)
 ******************************************************************************/

#define FMSTR_TRANSPORT FMSTR_SERIAL              /* Use serial transport layer */
#define FMSTR_SERIAL_DRV FMSTR_SERIAL_MCUX_LPUART /*Use serial driver for LPUART */

#define FMSTR_FLEXCAN_TXMB 0
#define FMSTR_FLEXCAN_RXMB 1

/******************************************************************************
 * Input/output communication buffer size
 ******************************************************************************/

#define FMSTR_COMM_BUFFER_SIZE 0 /* set to 0 for "automatic" */

/******************************************************************************
 * Receive FIFO queue size (use with FMSTR_SHORT_INTR only)
 ******************************************************************************/

#define FMSTR_COMM_RQUEUE_SIZE 32 /* set to 0 for "default" */

/*****************************************************************************
 * Support for Application Commands
 ******************************************************************************/

#define FMSTR_USE_APPCMD 1        /* enable/disable App.Commands support */
#define FMSTR_APPCMD_BUFF_SIZE 32 /* App.Command data buffer size */
#define FMSTR_MAX_APPCMD_CALLS 4  /* how many app.cmd callbacks? (0=disable) */

/*****************************************************************************
 * Oscilloscope support
 ******************************************************************************/

#define FMSTR_USE_SCOPE 1      /* enable/disable scope support */
#define FMSTR_MAX_SCOPE_VARS 8 /* max. number of scope variables (2..8) */

/*****************************************************************************
 * Recorder support
 ******************************************************************************/

#define FMSTR_USE_RECORDER 0 /* enable/disable recorder support */
#define FMSTR_MAX_REC_VARS 8 /* max. number of recorder variables (2..8) */
#define FMSTR_REC_OWNBUFF 0  /* use user-allocated rec. buffer (1=yes) */

/* built-in recorder buffer (use when FMSTR_REC_OWNBUFF is 0) */
#define FMSTR_REC_BUFF_SIZE 1024 /* built-in buffer size */

/* recorder time base, specifies how often the recorder is called in the user app. */
#define FMSTR_REC_TIMEBASE FMSTR_REC_BASE_MILLISEC(0) /* 0 = "unknown" */

#define FMSTR_REC_FLOAT_TRIG 0 /* enable/disable floating point triggering */

/*****************************************************************************
 * Target-side address translation (TSA)
 ******************************************************************************/

#define FMSTR_USE_TSA 1         /* enable TSA functionality */
#define FMSTR_USE_TSA_SAFETY 0  /* enable access to TSA variables only */
#define FMSTR_USE_TSA_INROM 1   /* TSA tables declared as const (put to ROM) */
#define FMSTR_USE_TSA_DYNAMIC 1 /* TSA can be defined also dynamically */
/*****************************************************************************
 * Enable/Disable read/write memory commands
 ******************************************************************************/

#define FMSTR_USE_READMEM 1      /* enable read memory commands */
#define FMSTR_USE_WRITEMEM 1     /* enable write memory commands */
#define FMSTR_USE_WRITEMEMMASK 0 /* enable write memory bits commands */

/*****************************************************************************
 * Enable/Disable read/write variable commands (a bit faster than Read Mem)
 ******************************************************************************/

#define FMSTR_USE_READVAR 1      /* enable read variable fast commands */
#define FMSTR_USE_WRITEVAR 0     /* enable write variable fast commands */
#define FMSTR_USE_WRITEVARMASK 0 /* enable write variable bits fast commands */

#endif /* _FREEMASTER_CFG_H_ */
