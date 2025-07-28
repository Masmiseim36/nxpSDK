/*
 * Copyright (c) 2017-2023, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_app.h"
#include "tfm_log.h"
#ifdef TFM_NS_REG_TEST
#include "non_secure_suites.h"
#endif
#include "os_wrapper/thread.h"

/**
 * \brief Services test thread
 *
 */
__attribute__((noreturn))
void test_app(void *argument)
{
    UNUSED_VARIABLE(argument);

#ifdef TFM_NS_REG_TEST
    ns_reg_test_start();
#endif

    /* Output EOT char for test environments like FVP. */
    LOG_MSG("\x04");

    /* End of test */
    os_wrapper_thread_exit();
}
