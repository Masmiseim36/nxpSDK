/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "ipc_s_tests.h"
#include "psa/client.h"
#include "psa_manifest/sid.h"
#include "test_framework_helpers.h"

/* List of tests */
static void tfm_ipc_test_1001(struct test_result_t *ret);

static struct test_t ipc_veneers_tests[] = {
    {&tfm_ipc_test_1001, "TFM_S_IPC_TEST_1001",
     "Accessing stateless service from secure partition", {TEST_PASSED}},
};

void register_testsuite_s_ipc_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(ipc_veneers_tests) / sizeof(ipc_veneers_tests[0]));

    set_testsuite("IPC secure interface test (TFM_S_IPC_TEST_1XXX)",
                  ipc_veneers_tests, list_size, p_test_suite);
}

/**
 * \brief Accessing a stateless service
 *
 * \note Accessing stateless service from a secure partition.
 */
static void tfm_ipc_test_1001(struct test_result_t *ret)
{
    uint32_t data = 0xFFFFABCD;
    psa_status_t status;
    psa_invec in_vec[] = { {&data, sizeof(uint32_t)} };

    /*
     * Creating or closing a connection to stateless service from secure
     * partition will cause panic. Test calling a stateless service only.
     * The test should succeed.
     */
    status = psa_call(IPC_SERVICE_TEST_STATELESS_ROT_HANDLE, PSA_IPC_CALL,
                      in_vec, 1, NULL, 0);
    if (status < 0) {
        TEST_FAIL("Calling a stateless service test fail.\r\n");
        return;
    }

    ret->val = TEST_PASSED;
}
