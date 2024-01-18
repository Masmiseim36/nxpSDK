/**************************************************************************
MODULE:    MCOHW_CFG.H
CONTAINS:  MicroCANopen hardware configuration
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2020
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.10, ESA 20-SEP-02
           $LastChangedDate: 2020-09-03 22:04:52 +0200 (Thu, 03 Sep 2020) $
           $LastChangedRevision: 5038 $
***************************************************************************/

#ifndef _MCOHW_CFG_H
#define _MCOHW_CFG_H

#include <stdint.h>
#include <stdio.h>

// CANopen Data Types
#include "mco_types.h"

#include "fsl_debug_console.h"
#include "pin_mux.h"

#include "board.h"
/**************************************************************************
MACROS: Memory copy and compare to use
**************************************************************************/
#include <string.h>

#define MEM_CPY_FAR(d, s, l) memcpy((void *)d, (void *)s, l)
#define MEM_CPY              memcpy
#define MEM_CMP              memcmp

/**************************************************************************
DEFINES: BASIC CAN COMMUNICATION
Modify these for your application
**************************************************************************/

// Default CAN bitrate, may be used in MCOUSER_ResetCommunication() to initialize CAN
#define CAN_BITRATE     500
#define CAN_BRS_BITRATE 2000 // CAN FD data bitrate, make ==CAN_BITRATE to disable BRS

/*
 * Note: All settings below are for informational purposes only and
 *       must not be changed in projects that use the NXP SDK library
 *       version of MicroCANopen. Changes here will not affect the
 *       delivered library and may lead to applications that fail to
 *       build or perform.
 */

// Use CAN SW Filtering
#define USE_CANSWFILTER 1

// use 29-bit CAN message for LSS feedback
#define USE_29BIT_LSSFEEDBACK 0

/**************************************************************************
MACROS: RTOS SLEEP FUNCTION
**************************************************************************/

// If used in RTOS, allow other tasks to run in blocking function calls
// Only define RTOS_SLEEP when an RTOS is used!
// #define RTOS_SLEEP Sleep(3);
#define RTOS_LOCK_PI(access, area)
#define RTOS_UNLOCK_PI(access, area)

/**************************************************************************
DEFINES: CAN HARDWARE DRIVER DEFINITIONS
**************************************************************************/

// if defined a SW FIFO for CAN receive and transmit is used
#define USE_CANFIFO 0

// Tx FIFO depth (must be 0, 4, 8, 16 or 32)
#define TXFIFOSIZE 32

// Rx FIFO depth (must be 0, 4, 8, 16 or 32)
#define RXFIFOSIZE 64

// Manager Rx FIFO depth (must be 0, 4, 8, 16 or 32)
#define MGRFIFOSIZE 64

/**************************************************************************
DEFINES: BOOTLOADER SUPPORT
**************************************************************************/
// If enabled, supports the ESAcademy CANopen bootloader, copy values
// from bootloader configuration
#define REBOOT_FLAG_ADR     0x40003FFCUL
#define REBOOT_FLAG         *(volatile uint32_t *)REBOOT_FLAG_ADR
#define REBOOT_BOOTLOAD_VAL 0x21654387

/**************************************************************************
PUBLIC FUNCTIONS
**************************************************************************/

/**************************************************************************
DOES:    Stack timer tick function. Must be called once per millisecond.
**************************************************************************/
void MCOHW_Tick(void);

#endif // _MCOHW_CFG_H

/*----------------------- END OF FILE ----------------------------------*/
