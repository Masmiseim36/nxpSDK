/*
 * Copyright (c) 2013 Freescale Semiconductor, Inc.
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if !defined(__FSL_RTOS_ABSTRACTION_H__)
#define __FSL_RTOS_ABSTRACTION_H__

#include <stdint.h>
#include <stdbool.h>
#include "fsl_common.h"
#include "fsl_os_abstraction.h"

//! @addtogroup rtos_irq_sync
//! @{

////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////
/*! @brief Type for an semaphore */
typedef struct Semaphore
{
    uint32_t time_start;       /*!< The time to start timeout                        */
    uint32_t timeout;          /*!< Timeout to wait in milliseconds                  */
    volatile bool isWaiting;   /*!< Is any task waiting for a timeout on this object */
    volatile uint8_t semCount; /*!< The count value of the object                    */

} semaphore_t;

//! @brief Type for an interrupt synchronization object.
typedef volatile int32_t sync_object_t;

//! @brief Type for an interrupt lock object.
typedef volatile uint32_t lock_object_t;

enum sync_timeouts
{
    //! @brief Constant to pass for the sync_wait() timeout in order to wait indefinitely.
    kSyncWaitForever = 0xffffffffU
};

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif


//! @name Interrupt handler synchronization
//@{

/*!
 * @brief Initialize a synchronization object to a given state.
 *
 * @param obj The sync object to initialize.
 * @param state The initial state of the object. Pass true to make the sync object start
 *      out locked, or false to make it unlocked.
 */
void sync_init(sync_object_t *obj, bool state);

/*!
 * @brief Wait for a synchronization object to be signalled.
 *
 * @param obj The synchronization object.
 * @param timeout The maximum number of milliseconds to wait for the object to be signalled.
 *      Pass the #kSyncWaitForever constant to wait indefinitely for someone to signal the object.
 *      If 0 is passed for this timeout, then the function will return immediately if the object
 *      is locked.
 *
 * @retval true The object was signalled.
 * @retval false A timeout occurred.
 */
bool sync_wait(sync_object_t *obj, uint32_t timeout);

/*!
 * @brief Signal for someone waiting on the syncronization object to wake up.
 *
 * @param obj The synchronization object to signal.
 */
void sync_signal(sync_object_t *obj);

/*!
 * @brief Reset the synchronization object
 *
 * @param obj The synchronization object to signal.
 */
void sync_reset(sync_object_t *obj);

//@}

//! @addtogroup irq_lock
//! @{

//! @name Interrupt handler lock
//@{

//! @brief Initialize the lock object
void lock_init(void);

//! @brief Disable global irq and store previous state.
void lock_acquire(void);

//! @brief Restore previous state.
void lock_release(void);

//@}

//! @}

#if defined(__cplusplus)
}
#endif

#endif // __FSL_RTOS_ABSTRACTION_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
