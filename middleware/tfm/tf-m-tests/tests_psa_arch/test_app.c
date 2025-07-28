/*
 * Copyright (c) 2017-2023, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_app.h"
#include "tfm_log.h"
#include "os_wrapper/thread.h"

/**
 * \brief This symbol is the entry point provided by the PSA API compliance
 *        test libraries
 */
extern void val_entry(void);

/**
 * \brief Services test thread
 *
 */
__attribute__((noreturn))
void test_app(void *argument)
{
    UNUSED_VARIABLE(argument);

    val_entry();

    /* Output EOT char for test environments like FVP. */
    LOG_MSG("\x04");

    /* End of test */
    os_wrapper_thread_exit();
}
