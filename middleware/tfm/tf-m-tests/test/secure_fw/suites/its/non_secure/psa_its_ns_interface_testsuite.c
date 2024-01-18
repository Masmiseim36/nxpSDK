/*
 * Copyright (c) 2019-2023, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "its_ns_tests.h"
#include "test_framework_helpers.h"
#include "../its_tests_common.h"
#include "config_tfm.h"
#include "test_framework.h"
#include "psa/internal_trusted_storage.h"

/* List of tests */
static void tfm_its_ns_test_001(struct test_result_t *ret);
static void tfm_its_ns_test_002(struct test_result_t *ret);
static void tfm_its_ns_test_003(struct test_result_t *ret);

static struct test_t psa_its_ns_tests[] = {
    {&tfm_its_test_common_001, "TFM_NS_ITS_TEST_1001",
     "Set interface"},
    {&tfm_its_test_common_002, "TFM_NS_ITS_TEST_1002",
     "Set interface with create flags and get latest set flag"},
    {&tfm_its_test_common_003, "TFM_NS_ITS_TEST_1003",
     "Set interface with NULL data pointer"},
    {&tfm_its_test_common_004, "TFM_NS_ITS_TEST_1004",
     "Set interface with write once UID"},
    {&tfm_its_test_common_005, "TFM_NS_ITS_TEST_1005",
     "Get interface with valid data"},
    {&tfm_its_test_common_006, "TFM_NS_ITS_TEST_1006",
     "Get interface with zero data length"},
    {&tfm_its_test_common_007, "TFM_NS_ITS_TEST_1007",
     "Get interface with invalid UIDs"},
    {&tfm_its_test_common_008, "TFM_NS_ITS_TEST_1008",
     "Get interface with invalid data lengths and offsets"},
    {&tfm_its_test_common_009, "TFM_NS_ITS_TEST_1009",
     "Get interface with NULL data pointer"},
    {&tfm_its_test_common_010, "TFM_NS_ITS_TEST_1010",
     "Get info interface with write once UID"},
    {&tfm_its_test_common_011, "TFM_NS_ITS_TEST_1011",
     "Get info interface with valid UID"},
    {&tfm_its_test_common_012, "TFM_NS_ITS_TEST_1012",
     "Get info interface with invalid UIDs"},
    {&tfm_its_test_common_013, "TFM_NS_ITS_TEST_1013",
     "Remove interface with valid UID"},
    {&tfm_its_test_common_014, "TFM_NS_ITS_TEST_1014",
     "Remove interface with write once UID"},
    {&tfm_its_test_common_015, "TFM_NS_ITS_TEST_1015",
     "Remove interface with invalid UID"},
    {&tfm_its_test_common_016, "TFM_NS_ITS_TEST_1016",
     "Block compaction after remove"},
    {&tfm_its_test_common_017, "TFM_NS_ITS_TEST_1017",
     "Multiple partial gets"},
    {&tfm_its_test_common_018, "TFM_NS_ITS_TEST_1018",
     "Multiple sets to same UID from same thread"},
    {&tfm_its_test_common_019, "TFM_NS_ITS_TEST_1019",
     "Set, get and remove interface with different asset sizes"},
    {&tfm_its_test_common_020, "TFM_NS_ITS_TEST_1020",
     "Set with asset size that exceeds the maximum"},
    {&tfm_its_test_common_021, "TFM_NS_ITS_TEST_1021",
    "Get interface with data_len over 0 and NULL data length pointer"},
    {&tfm_its_ns_test_001, "TFM_NS_ITS_TEST_001",
    "Set interface with NULL data pointer and length over 0"},
    {&tfm_its_ns_test_002, "TFM_NS_ITS_TEST_002",
    "Get info interface with NULL p_info"},
    {&tfm_its_ns_test_003, "TFM_NS_ITS_TEST_003",
    "Get interface with NULL data pointer and data_size over 0"},
};

void register_testsuite_ns_psa_its_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(psa_its_ns_tests) / sizeof(psa_its_ns_tests[0]));

    set_testsuite("PSA internal trusted storage NS interface tests "
                 "(TFM_NS_ITS_TEST_1XXX)",
                  psa_its_ns_tests, list_size, p_test_suite);
}

void tfm_its_ns_test_001(struct test_result_t *ret)
{
    psa_status_t status = psa_its_set(TEST_UID_2, 1, NULL, PSA_STORAGE_FLAG_NONE);
     if (status != PSA_ERROR_PROGRAMMER_ERROR) {
        TEST_FAIL("Set with null data pointer and length over 0 "
                  "should return PSA_ERROR_PROGRAMMER_ERROR");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_its_ns_test_002(struct test_result_t *ret)
{
    const psa_storage_uid_t uid = TEST_UID_2;

    psa_status_t status = psa_its_set(uid, WRITE_DATA_SIZE, WRITE_DATA, PSA_STORAGE_FLAG_NONE);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Set should not fail");
        return;
    }

    status = psa_its_get_info(uid, NULL);
    if (status != PSA_ERROR_PROGRAMMER_ERROR) {
        TEST_FAIL("Get info with null pointer should return PSA_ERROR_PROGRAMMER_ERROR");
        return;
    }

    status = psa_its_remove(uid);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Remove should not fail with valid UID");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_its_ns_test_003(struct test_result_t *ret)
{
    const psa_storage_uid_t uid = TEST_UID_2;
    const size_t data_len = WRITE_DATA_SIZE;
    const uint8_t write_data[] = WRITE_DATA;
    size_t read_data_length;

    psa_status_t status = psa_its_set(uid, data_len, write_data, PSA_STORAGE_FLAG_NONE);
     if (status != PSA_SUCCESS) {
        TEST_FAIL("Failed to set UID with PSA_STORAGE_FLAG_NONE flag.");
        return;
    }

    status = psa_its_get(uid, 0, 1, NULL, &read_data_length);
    if (status != PSA_ERROR_PROGRAMMER_ERROR) {
        TEST_FAIL("Get with null data pointer and size greater than 0 should return "
                  "PSA_PROGRAMMER_ERROR");
        return;
    }

    status = psa_its_remove(uid);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Remove should not fail with valid UID");
        return;
    }

    ret->val = TEST_PASSED;
}
