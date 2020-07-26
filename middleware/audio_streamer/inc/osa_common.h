/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef OSA_COMMON_H
#define OSA_COMMON_H

/*!
 * @file    osa_common.h
 * @brief   Primary include for libosa.
 * @details This file includes all component headers for libosa.  Any
 *          program using libosa should include only this header.
 */

/* This should be defined as one of the below supported OSes. */
/*#define FSL_OS_SELECTED <MY_OS>*/

#define FSL_OS_NONE       0
#define FSL_OS_MQX        1
#define FSL_OS_FREE_RTOS  2
#define FSL_OS_UCOS       3
#define FSL_OS_CMSIS      4
#define FSL_OS_UCOSIII    5
#define FSL_OS_LINUX      6

#ifdef __cplusplus
extern "C" {
#endif

#include "osa_types.h"

/* Include platform specific headers here.  These contain further API includes
 * for things like standard IO libraries and system headers, etc. that differ
 * between the different OSes. */
#include "osa_freertos.h"

#include "osa_memory.h"
#include "osa_thread.h"
#include "osa_mutex.h"
#include "osa_cond.h"
#include "osa_message.h"
#include "osa_time.h"
#include "osa_misc.h"
#include "osa_dir.h"
#include "osa_init.h"
#include "osa_event.h"

#include "error.h"

#ifdef __cplusplus
}
#endif

/*! Debug print macro for function entering. */
#define DBG_entering      LOG_ENTER(LOGMDL_COMMON);

/*! Debug print macro for function exiting. */
#define DBG_leaving       LOG_EXIT(LOGMDL_COMMON);

/*
** Memory type definitions for OSA sub-modules
*/
#define OSA_DMEM_TOOL_MEMTYPE   OSA_OSAMEMTYPE_BASE
#define OSA_COND_MEMTYPE        (OSA_OSAMEMTYPE_BASE + 1)
#define OSA_DIR_MEMTYPE         (OSA_OSAMEMTYPE_BASE + 2)
#define OSA_EVENT_MEMTYPE       (OSA_OSAMEMTYPE_BASE + 3)
#define OSA_MESSAGE_MEMTYPE     (OSA_OSAMEMTYPE_BASE + 4)
#define OSA_MUTEX_MEMTYPE       (OSA_OSAMEMTYPE_BASE + 5)
#define OSA_THREAD_MEMTYPE      (OSA_OSAMEMTYPE_BASE + 6)
#define OSA_SEM_MEMTYPE         (OSA_OSAMEMTYPE_BASE + 7)
#define OSA_MEMTYPE_END         OSA_SEM_MEMTYPE

/*
** Memory type definitions for dynamic memory gathering utility
*/
#define DMG_MEMTYPE_BASE    (OSA_MEMTYPE_END + 50)

#endif

