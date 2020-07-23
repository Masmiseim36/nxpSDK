/*
 * Copyright 2019 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_


/*! *********************************************************************************
 *     Board Configuration
 ********************************************************************************** */
 /* Defines the number of required keys for the keyboard module */
#define gKeyBoardSupported_d    0

#define gKBD_KeysCount_c        0

/* Specifies the number of required LEDs for the LED module */
#define gLEDsOnTargetBoardCnt_c 2

#define  gLEDBlipEnabled_d     0

/*! *********************************************************************************
 * 	App Configuration
 ********************************************************************************** */

/*! *********************************************************************************
 *     Framework Configuration
 ********************************************************************************** */
/* Defines Num of Serial Manager interfaces */
#define gSerialManagerMaxInterfaces_c   1

/* Defines Size for Timer Task*/
#define gTmrTaskStackSize_c  384

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define AppPoolsDetails_c \
         _block_size_  32  _number_of_blocks_    6  _pool_id_(0) _eol_  \
         _block_size_  64  _number_of_blocks_    3  _pool_id_(0) _eol_  \
         _block_size_ 128  _number_of_blocks_   10  _pool_id_(0) _eol_  \
         _block_size_ 512  _number_of_blocks_   40  _pool_id_(0) _eol_

/* Defines number of timers needed by the application */
#define gTmrApplicationTimers_c         0

/* Set this define TRUE if the PIT frequency is an integer number of MHZ */
#define gTMR_PIT_FreqMultipleOfMHZ_d    0

           /*
 * TimerManager Configuration
 */
#define FSL_OSA_BM_TIMER_CONFIG FSL_OSA_BM_TIMER_SYSTICK
#define gTimestamp_Enabled_d                    0
#define gTMR_PIT_Timestamp_Enabled_d 0
#define gTMR_EnableLowPowerTimers_d             1
#define gTMR_EnableMinutesSecondsTimers_d       0

/*! *********************************************************************************
 *     RTOS Configuration
 ********************************************************************************** */
/* Defines number of OS events used */
#define osNumberOfEvents        6

/*! *********************************************************************************
 *     BLE Stack Configuration
 ********************************************************************************** */

#define gUseHciTransportUpward_d    1

/*! *********************************************************************************
 *     BLE LL Configuration
 ***********************************************************************************/
/*  ble_ll_config.h file lists the parameters with their default values. User can override
 *    the parameter here by defining the parameter to a user defined value. */
/* enable periodic advertiser list */
#define gAppExtAdvEnable_d                   1
#define gLlScanPeriodicAdvertiserListSize_c (8U)
/* disable autonomous feature exchange */
#define gL1AutonomousFeatureExchange_d 0

/*
 * Specific configuration of LL pools by block size and number of blocks for this application.
 * Optimized using the MEM_OPTIMIZE_BUFFER_POOL feature in MemManager,
 * we find that the most optimized combination for LL buffers.
 *
 * If LlPoolsDetails_c is not defined, default LL buffer configuration in app_preinclude_common.h
 * will be applied.
 */

/* Include common configuration file and board configuration file */
#include "app_preinclude_common.h"
#endif /* _APP_PREINCLUDE_H_ */
