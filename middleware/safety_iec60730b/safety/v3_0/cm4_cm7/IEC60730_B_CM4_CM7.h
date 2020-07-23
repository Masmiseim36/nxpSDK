/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2011 Freescale Semiconductor, Inc.
* (c) Copyright 2001-2004 Motorola, Inc.
* ALL RIGHTS RESERVED.
* =================================================================== *
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY              *
* EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE *
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR  *
* PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL FREESCALE OR             *
* ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    *
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT        *
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;        *
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)            *
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, *
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       *
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED *
* OF THE POSSIBILITY OF SUCH DAMAGE.                                  *
* =================================================================== *
*
***************************************************************************//*!
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

/* Includes of each safety routine */
  #include "register/IEC60730_B_CM4_CM7_reg.h"
  #include "flash/IEC60730_B_CM4_CM7_flash.h"
  #include "ram/IEC60730_B_CM4_CM7_ram.h"
  #include "stack/IEC60730_B_CM4_CM7_stack.h"
  #include "dio/IEC60730_B_CM4_CM7_dio.h"
  #include "dio/IEC60730_B_CM4_CM7_dio_ext.h"
  #include "aio/IEC60730_B_CM4_CM7_aio.h"
  #include "clock/IEC60730_B_CM4_CM7_clock.h"
  #include "programCounter/IEC60730_B_CM4_CM7_pc.h"
  #include "watchdog/IEC60730_B_CM4_CM7_wdg.h"



#endif
