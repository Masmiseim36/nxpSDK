/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the app configuration file which is pre included.
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

#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_

/*! *********************************************************************************
 * 	Drivers Configuration
 ********************************************************************************** */
 /* Defines the number of available keys for the keyboard module for this application */
#define gKBD_KeysCount_c        2
 
 /* Defines the number of available tsi electrodes for this application */
#define gKBD_TsiElectdCount_c   0

/* Specifies the number of physical LEDs on the target board */
#define gLEDsOnTargetBoardCnt_c 4

/*! *********************************************************************************
 * 	Framework Configuration
 ********************************************************************************** */
/* Defines Num of Serial Manager interfaces */
#define gSerialManagerMaxInterfaces_c   1

/* Defines Size for Timer Task*/
#define gTmrTaskStackSize_c  384

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define PoolsDetails_c \
         _block_size_  32  _number_of_blocks_    6 _eol_  \
         _block_size_  64  _number_of_blocks_    3 _eol_  \
         _block_size_ 128  _number_of_blocks_    4 _eol_  \
         _block_size_ 512  _number_of_blocks_    4 _eol_

/* Defines number of timers needed by the application */
#define gTmrApplicationTimers_c         1

/* Defines number of timers needed by the protocol stack */
#define gTmrStackTimers_c               3
             
/* Enable/Disable Low Power Timer */
#define gTMR_EnableLowPowerTimers       1

/* Enable/Disable PowerDown functionality in PwrLib */
#define cPWR_UsePowerDownMode           0

/* Enable/Disable GENFSK Link Layer DSM */
#define cPWR_GENFSK_LL_Enable           1
             
/* Default Deep Sleep Mode*/
#define cPWR_DeepSleepMode              7

/* Disable all pins when entering Low Power */           
#define APP_DISABLE_PINS_IN_LOW_POWER   1
           
/* Default deep sleep duration in ms */ 
#define cPWR_DeepSleepDurationMs        30000
             
/* Enables / Disables the DCDC platform component */
#define gDCDC_Enabled_d                 1   

/* Default DCDC Mode used by the application */           
#define APP_DCDC_MODE                   gDCDC_Mode_Buck_c

/* Default DCDC Battery Level Monitor interval */
#define APP_DCDC_VBAT_MONITOR_INTERVAL  600000  
           
/*! *********************************************************************************
 * 	RTOS Configuration
 ********************************************************************************** */
/* Defines number of OS events used */
#define osNumberOfEvents        4

/* Defines number of OS semaphores used */
#define osNumberOfSemaphores    1
             
/*! *********************************************************************************
 * 	Auto Configuration
 ********************************************************************************** */
/* Disable LEDs when enabling low power */           
#if cPWR_UsePowerDownMode || gMWS_UseCoexistence_d
#define gLEDSupported_d 0           
#endif
           
#if gMWS_UseCoexistence_d   
#undef gKBD_KeysCount_c
#define gKBD_KeysCount_c        1
#endif

#endif /* _APP_PREINCLUDE_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
