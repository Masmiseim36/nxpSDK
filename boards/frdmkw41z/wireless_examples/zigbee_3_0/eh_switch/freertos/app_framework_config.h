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

#ifndef _APP_FRAMEWORK_CONFIG_H
#define _APP_FRAMEWORK_CONFIG_H

/*!
\file       app_framework_config.h
\brief      This file is a for framework configuration of all zigbee demo applications.
            If it is required to configure just one application use the appllication config. file.
            Ex: for zigbee router application use the specific config.h
*/

/*!=================================================================================================
       CONFIG FRAMEWORK
==================================================================================================*/

#ifndef __SEMIHOST_HARDFAULT_DISABLE
    #define __SEMIHOST_HARDFAULT_DISABLE
#endif

/*!=================================================================================================
  Panic configuration
==================================================================================================*/

/*! Use panic */
#ifndef gUsePanic_c
    #define gUsePanic_c 0
#endif

/*!=================================================================================================
  MemManager configuration
==================================================================================================*/
/*! Defines pools by block size and number of blocks. Must be alligned to 4 bytes.*/   
#ifndef PoolsDetails_c
#define PoolsDetails_c \
         _block_size_  64  _number_of_blocks_    8 _pool_id_(0) _eol_  \
         _block_size_ 180  _number_of_blocks_   20 _pool_id_(0) _eol_  \
         _block_size_ 256  _number_of_blocks_    6 _pool_id_(0) _eol_
#endif
/*!=================================================================================================
  NVM configuration
==================================================================================================*/
/*! Enable/disable NV storage module */
#ifndef gNvStorageIncluded_d
    #define gNvStorageIncluded_d             1
#endif

/*! Zigbee stack requires the NVM table to be stored in RAM memory */
#ifndef gNvTableKeptInRam_d 
    #define gNvTableKeptInRam_d              1
#endif

/*! Enable/disable interrupts when the NV FTFL controller executes a command sequence */
#ifndef gNvDisableIntCmdSeq_c
    #define gNvDisableIntCmdSeq_c            1
#endif
/*! Enable NVM FSCI monitoring */
#ifndef gNvmEnableFSCIMonitoring_c
    #define gNvmEnableFSCIMonitoring_c      (0)
#endif
/*! Indexes of the elements that needs to be merged */
#ifndef gNvRecordsCopiedBufferSize_c
    #define gNvRecordsCopiedBufferSize_c    128
#endif

/*! NVM table size */
#ifndef gNvTableEntriesCountMax_c
    #define gNvTableEntriesCountMax_c       20
#endif

/*! NVM pool id for unmirrored datasets */
#ifndef gNvmMemPoolId_c
    #define gNvmMemPoolId_c                 0
#endif

/*! Default minimum-timer-ticks-between-dataset-saves, in seconds */
#ifndef gNvMinimumTicksBetweenSaves_c
    #define gNvMinimumTicksBetweenSaves_c   2
#endif

/*!=================================================================================================
* OSA EXT configuration
==================================================================================================*/
/*! Number of semaphores */
#ifndef osNumberOfSemaphores
    #define osNumberOfSemaphores 1
#endif
/*! Number of mutexes*/
#ifndef osNumberOfMutexes
    #define osNumberOfMutexes    2
#endif
/*! Number of message queues*/
#ifndef osNumberOfMessageQs
    #define osNumberOfMessageQs  0
#endif
/*! Number of messages */
#ifndef osNumberOfMessages
    #define osNumberOfMessages   0
#endif
/*! Number of events */
#ifndef osNumberOfEvents
    #define osNumberOfEvents     5
#endif
/*! Main Task priority */
#ifndef gMainThreadPriority_c
    #define gMainThreadPriority_c  (OSA_PRIORITY_IDLE + 2)
#endif
/*! Main Task stack size */
#ifndef gMainThreadStackSize_c
    #define gMainThreadStackSize_c 1600
#endif

/*!=================================================================================================
* Uart driver
==================================================================================================*/

#ifndef UART_BAUD_RATE
    #define UART_BAUD_RATE          115200
#endif

/*!=================================================================================================
* Keyboard
==================================================================================================*/
/*! TSI electrodes count */
#ifndef gKBD_TsiElectdCount_c
    #define gKBD_TsiElectdCount_c   2
#endif

/*! Number of available keys for the keyboard module */            
#ifndef gKBD_KeysCount_c
    #define gKBD_KeysCount_c        4
#endif

/*! 3 sec - KbdVeryLongKey - used for reset to factory defaults */ 
#ifndef gKbdVeryLongKeyIterations_c
    #define gKbdVeryLongKeyIterations_c (60)
#endif
/*!=================================================================================================
* Timers manager
==================================================================================================*/

/*! Number of timers needed by the protocol stack */
#ifndef gTmrStackTimers_c
    #define gTmrStackTimers_c       20
#endif

/*!=================================================================================================
* LPM - low power
==================================================================================================*/
/*! low power mode - MCU in LLS3 mode */
#ifndef  cPWR_DeepSleepMode
   #define cPWR_DeepSleepMode        3
#endif

/*! low power modes not used - optimisation */             
#ifndef cPWR_EnableDeepSleepMode_1             
    #define cPWR_EnableDeepSleepMode_1   0
#endif
 
#ifndef cPWR_EnableDeepSleepMode_2              
    #define cPWR_EnableDeepSleepMode_2   0
#endif

#ifndef cPWR_EnableDeepSleepMode_5               
    #define cPWR_EnableDeepSleepMode_5   0
#endif
 
#ifndef cPWR_EnableDeepSleepMode_6              
    #define cPWR_EnableDeepSleepMode_6   0             
#endif

/*!=================================================================================================
* SHELL
==================================================================================================*/
#ifndef SHELL_USE_LOGO
    #define SHELL_USE_LOGO 0
#endif

#ifndef SHELL_USE_AUTO_COMPLETE
    #define SHELL_USE_AUTO_COMPLETE 0
#endif

#ifndef SHELL_USE_ALT_TASK
    #define SHELL_USE_ALT_TASK 1
#endif

#ifndef SHELL_MAX_COMMANDS
    #define SHELL_MAX_COMMANDS 18
#endif
             
#endif /* _APP_FRAMEWORK_CONFIG_H  */
/*!
** @}
*/
