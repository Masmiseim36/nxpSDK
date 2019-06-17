/*******************************************************************************
*
* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
* 
*
*******************************************************************************/
/******************************************************************************
*
* @file      IEC60730_B_CM4_CM7.h
*
* @author    B48632
*
* @version   3.0
*
* @date      Nov-01-2018
*
* @brief     IEC60730 Class B Safety Routines main header file
*
*******************************************************************************
*
*
*
******************************************************************************/
#ifndef _IEC60730_B_CM4_CM7_H
#define _IEC60730_B_CM4_CM7_H

/******************************************************************************
* Types
******************************************************************************/
#ifndef __ASM__
  typedef unsigned long IEC60730B_RESULT;
  typedef unsigned long IEC60730_BOOL;
#endif

/******************************************************************************
* Error codes for tests
******************************************************************************/
#define IEC60730B_ST_CPU_FAIL_CODE      (0x0100)
#define IEC60730B_ST_PC_FAIL_CODE       (0x0200)
#define IEC60730B_ST_FLASH_FAIL_CODE    (0x0300)
#define IEC60730B_ST_RAM_FAIL_CODE      (0x0400)
#define IEC60730B_ST_STACK_FAIL_CODE    (0x0500)
#define IEC60730B_ST_CLK_FAIL_CODE      (0x0600)
#define IEC60730B_ST_CLKSYNC_FAIL_CODE  (0x0600)
#define IEC60730B_ST_AIO_FAIL_CODE      (0x0700)
#define IEC60730B_ST_DIO_FAIL_CODE      (0x0800)

/******************************************************************************
* Includes
******************************************************************************/

/* Macros for IDEs */
#include "asm_mac_common.h"

/* Include type definitions and safety routines */
#include "iec60730b_core.h"
#include "iec60730b_aio.h"
#include "iec60730b_clock.h"
#include "iec60730b_dio.h"
#include "iec60730b_dio_ext.h"
#include "iec60730b_wdog.h"

#endif
