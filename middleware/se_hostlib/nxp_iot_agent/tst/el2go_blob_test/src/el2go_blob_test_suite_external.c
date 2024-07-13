/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "el2go_blob_test.h"
#include "el2go_blob_test_executor_psa.h"
#include "el2go_blob_test_suite_external.h"

/* BINARY */

static void el2go_blob_test_external_2000(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RAW_DATA, 1 * 8, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_DATA, 0x3D000000,
                         EXTERNAL_BIN1B_EXPORT_NONE, sizeof(EXTERNAL_BIN1B_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2001(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RAW_DATA, 256 * 8, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_DATA,
                         0x3D000001, EXTERNAL_BIN256B_EXPORT_NONE, sizeof(EXTERNAL_BIN256B_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2002(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RAW_DATA, 512 * 8, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_DATA,
                         0x3D000002, EXTERNAL_BIN512B_EXPORT_NONE, sizeof(EXTERNAL_BIN512B_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2003(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RAW_DATA, 1024 * 8, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_DATA,
                         0x3D000003, EXTERNAL_BIN1024B_EXPORT_NONE, sizeof(EXTERNAL_BIN1024B_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2004(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RAW_DATA, 2783 * 8, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_DATA,
                         0x3D000004, EXTERNAL_BIN2783B_EXPORT_NONE, sizeof(EXTERNAL_BIN2783B_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2005(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RAW_DATA, 4 * 1024 * 8, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_DATA,
                         0x3D000005, EXTERNAL_BIN4096B_EXPORT_NONE, sizeof(EXTERNAL_BIN4096B_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2006(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RAW_DATA, 8063 * 8, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_DATA,
                         0x3D000006, EXTERNAL_BIN8063B_EXPORT_NONE, sizeof(EXTERNAL_BIN8063B_EXPORT_NONE), ret);
}

/* NIST */

static void el2go_blob_test_external_2010(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 192, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000010, EXTERNAL_NISTP192_SIGVERMSG_ECDSAANYHASH,
                            sizeof(EXTERNAL_NISTP192_SIGVERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2011(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 192, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000011,
                             EXTERNAL_NISTP192_SIGVERHASH_ECDSAANYHASH,
                             sizeof(EXTERNAL_NISTP192_SIGVERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2012(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 192, PSA_ALG_ECDSA_ANY,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000012, EXTERNAL_NISTP192_SIGVERHASH_ECDSAANY,
                             sizeof(EXTERNAL_NISTP192_SIGVERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2013(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 192, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000013, EXTERNAL_NISTP192_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_NISTP192_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2014(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 192, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000014, EXTERNAL_NISTP192_EXPORT_NONE,
                         sizeof(EXTERNAL_NISTP192_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2015(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 192, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000015, EXTERNAL_NISTP192_VERMSG_ECDSAANYHASH,
                         sizeof(EXTERNAL_NISTP192_VERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2016(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 192, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000016, EXTERNAL_NISTP192_VERHASH_ECDSAANYHASH,
                          sizeof(EXTERNAL_NISTP192_VERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2017(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 192, PSA_ALG_ECDSA_ANY,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000017, EXTERNAL_NISTP192_VERHASH_ECDSAANY,
                          sizeof(EXTERNAL_NISTP192_VERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2018(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 192, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000018, EXTERNAL_NISTP192_NONE_NONE,
                         sizeof(EXTERNAL_NISTP192_NONE_NONE), ret);
}

static void el2go_blob_test_external_2020(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 224, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000020, EXTERNAL_NISTP224_SIGVERMSG_ECDSAANYHASH,
                            sizeof(EXTERNAL_NISTP224_SIGVERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2021(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 224, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000021,
                             EXTERNAL_NISTP224_SIGVERHASH_ECDSAANYHASH,
                             sizeof(EXTERNAL_NISTP224_SIGVERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2022(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 224, PSA_ALG_ECDSA_ANY,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000022, EXTERNAL_NISTP224_SIGVERHASH_ECDSAANY,
                             sizeof(EXTERNAL_NISTP224_SIGVERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2023(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 224, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000023, EXTERNAL_NISTP224_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_NISTP224_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2024(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 224, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000024, EXTERNAL_NISTP224_EXPORT_NONE,
                         sizeof(EXTERNAL_NISTP224_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2025(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 224, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000025, EXTERNAL_NISTP224_VERMSG_ECDSAANYHASH,
                         sizeof(EXTERNAL_NISTP224_VERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2026(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 224, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000026, EXTERNAL_NISTP224_VERHASH_ECDSAANYHASH,
                          sizeof(EXTERNAL_NISTP224_VERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2027(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 224, PSA_ALG_ECDSA_ANY,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000027, EXTERNAL_NISTP224_VERHASH_ECDSAANY,
                          sizeof(EXTERNAL_NISTP224_VERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2028(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 224, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000028, EXTERNAL_NISTP224_NONE_NONE,
                         sizeof(EXTERNAL_NISTP224_NONE_NONE), ret);
}

static void el2go_blob_test_external_1023(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 256, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3E000023, EXTERNAL_NISTP256_VERMSG_ECDSAANYHASH,
                         sizeof(EXTERNAL_NISTP256_VERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_1024(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 256, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3E000024, EXTERNAL_NISTP256_VERHASH_ECDSAANYHASH,
                          sizeof(EXTERNAL_NISTP256_VERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_1025(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 256, PSA_ALG_ECDSA_ANY,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3E000025, EXTERNAL_NISTP256_VERHASH_ECDSAANY,
                          sizeof(EXTERNAL_NISTP256_VERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_1026(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 256, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3E000026, EXTERNAL_NISTP256_NONE_NONE,
                         sizeof(EXTERNAL_NISTP256_NONE_NONE), ret);
}

static void el2go_blob_test_external_2030(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 384, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000030, EXTERNAL_NISTP384_SIGVERMSG_ECDSAANYHASH,
                            sizeof(EXTERNAL_NISTP384_SIGVERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2031(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 384, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000031,
                             EXTERNAL_NISTP384_SIGVERHASH_ECDSAANYHASH,
                             sizeof(EXTERNAL_NISTP384_SIGVERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2032(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 384, PSA_ALG_ECDSA_ANY,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000032, EXTERNAL_NISTP384_SIGVERHASH_ECDSAANY,
                             sizeof(EXTERNAL_NISTP384_SIGVERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2033(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 384, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000033, EXTERNAL_NISTP384_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_NISTP384_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2034(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 384, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000034, EXTERNAL_NISTP384_EXPORT_NONE,
                         sizeof(EXTERNAL_NISTP384_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2035(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 384, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000035, EXTERNAL_NISTP384_VERMSG_ECDSAANYHASH,
                         sizeof(EXTERNAL_NISTP384_VERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2036(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 384, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000036, EXTERNAL_NISTP384_VERHASH_ECDSAANYHASH,
                          sizeof(EXTERNAL_NISTP384_VERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2037(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 384, PSA_ALG_ECDSA_ANY,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000037, EXTERNAL_NISTP384_VERHASH_ECDSAANY,
                          sizeof(EXTERNAL_NISTP384_VERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2038(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 384, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000038, EXTERNAL_NISTP384_NONE_NONE,
                         sizeof(EXTERNAL_NISTP384_NONE_NONE), ret);
}

static void el2go_blob_test_external_2040(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 521, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000040, EXTERNAL_NISTP521_SIGVERMSG_ECDSAANYHASH,
                            sizeof(EXTERNAL_NISTP521_SIGVERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2041(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 521, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000041,
                             EXTERNAL_NISTP521_SIGVERHASH_ECDSAANYHASH,
                             sizeof(EXTERNAL_NISTP521_SIGVERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2042(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 521, PSA_ALG_ECDSA_ANY,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000042, EXTERNAL_NISTP521_SIGVERHASH_ECDSAANY,
                             sizeof(EXTERNAL_NISTP521_SIGVERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2043(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 521, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000043, EXTERNAL_NISTP521_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_NISTP521_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2044(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1), 521, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000044, EXTERNAL_NISTP521_EXPORT_NONE,
                         sizeof(EXTERNAL_NISTP521_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2045(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 521, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000045, EXTERNAL_NISTP521_VERMSG_ECDSAANYHASH,
                         sizeof(EXTERNAL_NISTP521_VERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2046(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 521, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000046, EXTERNAL_NISTP521_VERHASH_ECDSAANYHASH,
                          sizeof(EXTERNAL_NISTP521_VERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2047(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 521, PSA_ALG_ECDSA_ANY,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000047, EXTERNAL_NISTP521_VERHASH_ECDSAANY,
                          sizeof(EXTERNAL_NISTP521_VERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2048(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1), 521, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000048, EXTERNAL_NISTP521_NONE_NONE,
                         sizeof(EXTERNAL_NISTP521_NONE_NONE), ret);
}

/* BRAINPOOL */

static void el2go_blob_test_external_2050(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 192,
                            PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000050,
                            EXTERNAL_BRAINPOOLP192R1_SIGVERMSG_ECDSAANYHASH,
                            sizeof(EXTERNAL_BRAINPOOLP192R1_SIGVERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2051(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 192,
                             PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000051,
                             EXTERNAL_BRAINPOOLP192R1_SIGVERHASH_ECDSAANYHASH,
                             sizeof(EXTERNAL_BRAINPOOLP192R1_SIGVERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2052(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 192, PSA_ALG_ECDSA_ANY,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000052,
                             EXTERNAL_BRAINPOOLP192R1_SIGVERHASH_ECDSAANY,
                             sizeof(EXTERNAL_BRAINPOOLP192R1_SIGVERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2053(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 192, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000053, EXTERNAL_BRAINPOOLP192R1_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_BRAINPOOLP192R1_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2054(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 192, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000054, EXTERNAL_BRAINPOOLP192R1_EXPORT_NONE,
                         sizeof(EXTERNAL_BRAINPOOLP192R1_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2055(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 192,
                         PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000055,
                         EXTERNAL_BRAINPOOLP192R1_VERMSG_ECDSAANYHASH,
                         sizeof(EXTERNAL_BRAINPOOLP192R1_VERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2056(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 192,
                          PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000056,
                          EXTERNAL_BRAINPOOLP192R1_VERHASH_ECDSAANYHASH,
                          sizeof(EXTERNAL_BRAINPOOLP192R1_VERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2057(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 192, PSA_ALG_ECDSA_ANY,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000057, EXTERNAL_BRAINPOOLP192R1_VERHASH_ECDSAANY,
                          sizeof(EXTERNAL_BRAINPOOLP192R1_VERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2058(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 192, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000058, EXTERNAL_BRAINPOOLP192R1_NONE_NONE,
                         sizeof(EXTERNAL_BRAINPOOLP192R1_NONE_NONE), ret);
}

static void el2go_blob_test_external_2060(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 224,
                            PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000060,
                            EXTERNAL_BRAINPOOLP224R1_SIGVERMSG_ECDSAANYHASH,
                            sizeof(EXTERNAL_BRAINPOOLP224R1_SIGVERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2061(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 224,
                             PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000061,
                             EXTERNAL_BRAINPOOLP224R1_SIGVERHASH_ECDSAANYHASH,
                             sizeof(EXTERNAL_BRAINPOOLP224R1_SIGVERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2062(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 224, PSA_ALG_ECDSA_ANY,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000062,
                             EXTERNAL_BRAINPOOLP224R1_SIGVERHASH_ECDSAANY,
                             sizeof(EXTERNAL_BRAINPOOLP224R1_SIGVERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2063(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 224, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000063, EXTERNAL_BRAINPOOLP224R1_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_BRAINPOOLP224R1_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2064(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 224, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000064, EXTERNAL_BRAINPOOLP224R1_EXPORT_NONE,
                         sizeof(EXTERNAL_BRAINPOOLP224R1_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2065(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 224,
                         PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000065,
                         EXTERNAL_BRAINPOOLP224R1_VERMSG_ECDSAANYHASH,
                         sizeof(EXTERNAL_BRAINPOOLP224R1_VERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2066(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 224,
                          PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000066,
                          EXTERNAL_BRAINPOOLP224R1_VERHASH_ECDSAANYHASH,
                          sizeof(EXTERNAL_BRAINPOOLP224R1_VERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2067(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 224, PSA_ALG_ECDSA_ANY,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000067, EXTERNAL_BRAINPOOLP224R1_VERHASH_ECDSAANY,
                          sizeof(EXTERNAL_BRAINPOOLP224R1_VERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2068(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 224, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000068, EXTERNAL_BRAINPOOLP224R1_NONE_NONE,
                         sizeof(EXTERNAL_BRAINPOOLP224R1_NONE_NONE), ret);
}

static void el2go_blob_test_external_2070(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 256,
                            PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000070,
                            EXTERNAL_BRAINPOOLP256R1_SIGVERMSG_ECDSAANYHASH,
                            sizeof(EXTERNAL_BRAINPOOLP256R1_SIGVERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2071(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 256,
                             PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000071,
                             EXTERNAL_BRAINPOOLP256R1_SIGVERHASH_ECDSAANYHASH,
                             sizeof(EXTERNAL_BRAINPOOLP256R1_SIGVERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2072(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 256, PSA_ALG_ECDSA_ANY,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000072,
                             EXTERNAL_BRAINPOOLP256R1_SIGVERHASH_ECDSAANY,
                             sizeof(EXTERNAL_BRAINPOOLP256R1_SIGVERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2073(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 256, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000073, EXTERNAL_BRAINPOOLP256R1_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_BRAINPOOLP256R1_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2074(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 256, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000074, EXTERNAL_BRAINPOOLP256R1_EXPORT_NONE,
                         sizeof(EXTERNAL_BRAINPOOLP256R1_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2075(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 256,
                         PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000075,
                         EXTERNAL_BRAINPOOLP256R1_VERMSG_ECDSAANYHASH,
                         sizeof(EXTERNAL_BRAINPOOLP256R1_VERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2076(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 256,
                          PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000076,
                          EXTERNAL_BRAINPOOLP256R1_VERHASH_ECDSAANYHASH,
                          sizeof(EXTERNAL_BRAINPOOLP256R1_VERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2077(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 256, PSA_ALG_ECDSA_ANY,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000077, EXTERNAL_BRAINPOOLP256R1_VERHASH_ECDSAANY,
                          sizeof(EXTERNAL_BRAINPOOLP256R1_VERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2078(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 256, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000078, EXTERNAL_BRAINPOOLP256R1_NONE_NONE,
                         sizeof(EXTERNAL_BRAINPOOLP256R1_NONE_NONE), ret);
}

static void el2go_blob_test_external_2080(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 320,
                            PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000080,
                            EXTERNAL_BRAINPOOLP320R1_SIGVERMSG_ECDSAANYHASH,
                            sizeof(EXTERNAL_BRAINPOOLP320R1_SIGVERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2081(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 320,
                             PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000081,
                             EXTERNAL_BRAINPOOLP320R1_SIGVERHASH_ECDSAANYHASH,
                             sizeof(EXTERNAL_BRAINPOOLP320R1_SIGVERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2082(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 320, PSA_ALG_ECDSA_ANY,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000082,
                             EXTERNAL_BRAINPOOLP320R1_SIGVERHASH_ECDSAANY,
                             sizeof(EXTERNAL_BRAINPOOLP320R1_SIGVERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2083(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 320, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000083, EXTERNAL_BRAINPOOLP320R1_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_BRAINPOOLP320R1_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2084(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 320, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000084, EXTERNAL_BRAINPOOLP320R1_EXPORT_NONE,
                         sizeof(EXTERNAL_BRAINPOOLP320R1_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2085(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 320,
                         PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000085,
                         EXTERNAL_BRAINPOOLP320R1_VERMSG_ECDSAANYHASH,
                         sizeof(EXTERNAL_BRAINPOOLP320R1_VERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2086(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 320,
                          PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000086,
                          EXTERNAL_BRAINPOOLP320R1_VERHASH_ECDSAANYHASH,
                          sizeof(EXTERNAL_BRAINPOOLP320R1_VERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2087(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 320, PSA_ALG_ECDSA_ANY,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000087, EXTERNAL_BRAINPOOLP320R1_VERHASH_ECDSAANY,
                          sizeof(EXTERNAL_BRAINPOOLP320R1_VERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2088(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 320, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000088, EXTERNAL_BRAINPOOLP320R1_NONE_NONE,
                         sizeof(EXTERNAL_BRAINPOOLP320R1_NONE_NONE), ret);
}

static void el2go_blob_test_external_2090(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 384,
                            PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000090,
                            EXTERNAL_BRAINPOOLP384R1_SIGVERMSG_ECDSAANYHASH,
                            sizeof(EXTERNAL_BRAINPOOLP384R1_SIGVERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2091(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 384,
                             PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000091,
                             EXTERNAL_BRAINPOOLP384R1_SIGVERHASH_ECDSAANYHASH,
                             sizeof(EXTERNAL_BRAINPOOLP384R1_SIGVERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2092(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 384, PSA_ALG_ECDSA_ANY,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000092,
                             EXTERNAL_BRAINPOOLP384R1_SIGVERHASH_ECDSAANY,
                             sizeof(EXTERNAL_BRAINPOOLP384R1_SIGVERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2093(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 384, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000093, EXTERNAL_BRAINPOOLP384R1_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_BRAINPOOLP384R1_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2094(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 384, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000094, EXTERNAL_BRAINPOOLP384R1_EXPORT_NONE,
                         sizeof(EXTERNAL_BRAINPOOLP384R1_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2095(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 384,
                         PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000095,
                         EXTERNAL_BRAINPOOLP384R1_VERMSG_ECDSAANYHASH,
                         sizeof(EXTERNAL_BRAINPOOLP384R1_VERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2096(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 384,
                          PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000096,
                          EXTERNAL_BRAINPOOLP384R1_VERHASH_ECDSAANYHASH,
                          sizeof(EXTERNAL_BRAINPOOLP384R1_VERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2097(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 384, PSA_ALG_ECDSA_ANY,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000097, EXTERNAL_BRAINPOOLP384R1_VERHASH_ECDSAANY,
                          sizeof(EXTERNAL_BRAINPOOLP384R1_VERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2098(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 384, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000098, EXTERNAL_BRAINPOOLP384R1_NONE_NONE,
                         sizeof(EXTERNAL_BRAINPOOLP384R1_NONE_NONE), ret);
}

static void el2go_blob_test_external_2100(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 512,
                            PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000100,
                            EXTERNAL_BRAINPOOLP512R1_SIGVERMSG_ECDSAANYHASH,
                            sizeof(EXTERNAL_BRAINPOOLP512R1_SIGVERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2101(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 512,
                             PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000101,
                             EXTERNAL_BRAINPOOLP512R1_SIGVERHASH_ECDSAANYHASH,
                             sizeof(EXTERNAL_BRAINPOOLP512R1_SIGVERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2102(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 512, PSA_ALG_ECDSA_ANY,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000102,
                             EXTERNAL_BRAINPOOLP512R1_SIGVERHASH_ECDSAANY,
                             sizeof(EXTERNAL_BRAINPOOLP512R1_SIGVERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2103(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 512, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000103, EXTERNAL_BRAINPOOLP512R1_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_BRAINPOOLP512R1_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2104(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 512, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000104, EXTERNAL_BRAINPOOLP512R1_EXPORT_NONE,
                         sizeof(EXTERNAL_BRAINPOOLP512R1_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2105(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 512,
                         PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000105,
                         EXTERNAL_BRAINPOOLP512R1_VERMSG_ECDSAANYHASH,
                         sizeof(EXTERNAL_BRAINPOOLP512R1_VERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2106(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 512,
                          PSA_ALG_ECDSA(PSA_ALG_ANY_HASH), PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000106,
                          EXTERNAL_BRAINPOOLP512R1_VERHASH_ECDSAANYHASH,
                          sizeof(EXTERNAL_BRAINPOOLP512R1_VERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2107(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 512, PSA_ALG_ECDSA_ANY,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000107, EXTERNAL_BRAINPOOLP512R1_VERHASH_ECDSAANY,
                          sizeof(EXTERNAL_BRAINPOOLP512R1_VERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2108(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1), 512, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000108, EXTERNAL_BRAINPOOLP512R1_NONE_NONE,
                         sizeof(EXTERNAL_BRAINPOOLP512R1_NONE_NONE), ret);
}

/* KOBLITZ */

static void el2go_blob_test_external_2110(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 192, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000110,
                            EXTERNAL_KOBLITZ192_SIGVERMSG_ECDSAANYHASH,
                            sizeof(EXTERNAL_KOBLITZ192_SIGVERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2111(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 192, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000111,
                             EXTERNAL_KOBLITZ192_SIGVERHASH_ECDSAANYHASH,
                             sizeof(EXTERNAL_KOBLITZ192_SIGVERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2112(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 192, PSA_ALG_ECDSA_ANY,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000112, EXTERNAL_KOBLITZ192_SIGVERHASH_ECDSAANY,
                             sizeof(EXTERNAL_KOBLITZ192_SIGVERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2113(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 192, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000113, EXTERNAL_KOBLITZ192_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_KOBLITZ192_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2114(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 192, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000114, EXTERNAL_KOBLITZ192_EXPORT_NONE,
                         sizeof(EXTERNAL_KOBLITZ192_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2115(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_K1), 192, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000115, EXTERNAL_KOBLITZ192_VERMSG_ECDSAANYHASH,
                         sizeof(EXTERNAL_KOBLITZ192_VERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2116(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_K1), 192, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000116, EXTERNAL_KOBLITZ192_VERHASH_ECDSAANYHASH,
                          sizeof(EXTERNAL_KOBLITZ192_VERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2117(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_K1), 192, PSA_ALG_ECDSA_ANY,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000117, EXTERNAL_KOBLITZ192_VERHASH_ECDSAANY,
                          sizeof(EXTERNAL_KOBLITZ192_VERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2118(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_K1), 192, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000118, EXTERNAL_KOBLITZ192_NONE_NONE,
                         sizeof(EXTERNAL_KOBLITZ192_NONE_NONE), ret);
}

#ifndef CONFIG_RUN_VERIFIED_ONLY
static void el2go_blob_test_external_2120(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 225, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000120,
                            EXTERNAL_KOBLITZ224_SIGVERMSG_ECDSAANYHASH,
                            sizeof(EXTERNAL_KOBLITZ224_SIGVERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2121(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 225, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000121,
                             EXTERNAL_KOBLITZ224_SIGVERHASH_ECDSAANYHASH,
                             sizeof(EXTERNAL_KOBLITZ224_SIGVERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2122(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 225, PSA_ALG_ECDSA_ANY,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000122, EXTERNAL_KOBLITZ224_SIGVERHASH_ECDSAANY,
                             sizeof(EXTERNAL_KOBLITZ224_SIGVERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2123(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 225, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000123, EXTERNAL_KOBLITZ224_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_KOBLITZ224_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2124(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 225, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000124, EXTERNAL_KOBLITZ224_EXPORT_NONE,
                         sizeof(EXTERNAL_KOBLITZ224_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2125(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_K1), 225, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000125, EXTERNAL_KOBLITZ224_VERMSG_ECDSAANYHASH,
                         sizeof(EXTERNAL_KOBLITZ224_VERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2126(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_K1), 225, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000126, EXTERNAL_KOBLITZ224_VERHASH_ECDSAANYHASH,
                          sizeof(EXTERNAL_KOBLITZ224_VERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2127(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_K1), 225, PSA_ALG_ECDSA_ANY,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000127, EXTERNAL_KOBLITZ224_VERHASH_ECDSAANY,
                          sizeof(EXTERNAL_KOBLITZ224_VERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2128(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_K1), 225, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000128, EXTERNAL_KOBLITZ224_NONE_NONE,
                         sizeof(EXTERNAL_KOBLITZ224_NONE_NONE), ret);
}
#endif

static void el2go_blob_test_external_2130(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 256, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000130,
                            EXTERNAL_KOBLITZ256_SIGVERMSG_ECDSAANYHASH,
                            sizeof(EXTERNAL_KOBLITZ256_SIGVERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2131(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 256, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000131,
                             EXTERNAL_KOBLITZ256_SIGVERHASH_ECDSAANYHASH,
                             sizeof(EXTERNAL_KOBLITZ256_SIGVERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2132(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 256, PSA_ALG_ECDSA_ANY,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000132, EXTERNAL_KOBLITZ256_SIGVERHASH_ECDSAANY,
                             sizeof(EXTERNAL_KOBLITZ256_SIGVERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2133(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 256, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000133, EXTERNAL_KOBLITZ256_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_KOBLITZ256_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2134(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_K1), 256, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000134, EXTERNAL_KOBLITZ256_EXPORT_NONE,
                         sizeof(EXTERNAL_KOBLITZ256_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2135(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_K1), 256, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000135, EXTERNAL_KOBLITZ256_VERMSG_ECDSAANYHASH,
                         sizeof(EXTERNAL_KOBLITZ256_VERMSG_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2136(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_K1), 256, PSA_ALG_ECDSA(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000136, EXTERNAL_KOBLITZ256_VERHASH_ECDSAANYHASH,
                          sizeof(EXTERNAL_KOBLITZ256_VERHASH_ECDSAANYHASH), ret);
}

static void el2go_blob_test_external_2137(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_K1), 256, PSA_ALG_ECDSA_ANY,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000137, EXTERNAL_KOBLITZ256_VERHASH_ECDSAANY,
                          sizeof(EXTERNAL_KOBLITZ256_VERHASH_ECDSAANY), ret);
}

static void el2go_blob_test_external_2138(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_K1), 256, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000138, EXTERNAL_KOBLITZ256_NONE_NONE,
                         sizeof(EXTERNAL_KOBLITZ256_NONE_NONE), ret);
}

/* MONTGOMERY */

static void el2go_blob_test_external_2140(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_MONTGOMERY), 255, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000140, EXTERNAL_MONTDH25519_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_MONTDH25519_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2141(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_MONTGOMERY), 255, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000141, EXTERNAL_MONTDH25519_EXPORT_NONE,
                         sizeof(EXTERNAL_MONTDH25519_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2142(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_MONTGOMERY), 255, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000142, EXTERNAL_MONTDH25519_NONE_NONE,
                         sizeof(EXTERNAL_MONTDH25519_NONE_NONE), ret);
}

static void el2go_blob_test_external_2150(struct test_result_t *ret)
{
    psa_blob_keyexch_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_MONTGOMERY), 448, PSA_ALG_ECDH,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000150, EXTERNAL_MONTDH448_KEYEXCH_ECDH,
                          sizeof(EXTERNAL_MONTDH448_KEYEXCH_ECDH), ret);
}

static void el2go_blob_test_external_2151(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_MONTGOMERY), 448, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000151, EXTERNAL_MONTDH448_EXPORT_NONE,
                         sizeof(EXTERNAL_MONTDH448_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2152(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_MONTGOMERY), 448, PSA_ALG_NONE,
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000152, EXTERNAL_MONTDH448_NONE_NONE,
                         sizeof(EXTERNAL_MONTDH448_NONE_NONE), ret);
}

/* RSA */

static void el2go_blob_test_external_2160(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 1024, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000160, EXTERNAL_RSA1024_SIGVERMSG_PSSANYHASH,
                            sizeof(EXTERNAL_RSA1024_SIGVERMSG_PSSANYHASH), ret);
}

static void el2go_blob_test_external_2161(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 1024, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000161,
                            EXTERNAL_RSA1024_SIGVERMSG_PKCS1V15ANYHASH,
                            sizeof(EXTERNAL_RSA1024_SIGVERMSG_PKCS1V15ANYHASH), ret);
}

static void el2go_blob_test_external_2162(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 1024, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000162, EXTERNAL_RSA1024_SIGVERHASH_PSSANYHASH,
                             sizeof(EXTERNAL_RSA1024_SIGVERHASH_PSSANYHASH), ret);
}

static void el2go_blob_test_external_2163(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 1024, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000163,
                             EXTERNAL_RSA1024_SIGVERHASH_PKCS1V15ANYHASH,
                             sizeof(EXTERNAL_RSA1024_SIGVERHASH_PKCS1V15ANYHASH), ret);
}

#ifndef CONFIG_RUN_VERIFIED_ONLY
static void el2go_blob_test_external_2164(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 1024, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000164, EXTERNAL_RSA1024_SIGVERHASH_RAW,
                             sizeof(EXTERNAL_RSA1024_SIGVERHASH_RAW), ret);
}

static void el2go_blob_test_external_2165(struct test_result_t *ret)
{
    psa_blob_crypt_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 1024, PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256),
                        PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000165, EXTERNAL_RSA1024_CRYPT_OAEPSHA256,
                        sizeof(EXTERNAL_RSA1024_CRYPT_OAEPSHA256), ret);
}

static void el2go_blob_test_external_2166(struct test_result_t *ret)
{
    psa_blob_crypt_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 1024, PSA_ALG_RSA_PKCS1V15_CRYPT,
                        PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000166, EXTERNAL_RSA1024_CRYPT_PKCS1V15,
                        sizeof(EXTERNAL_RSA1024_CRYPT_PKCS1V15), ret);
}
#endif

static void el2go_blob_test_external_2167(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 1024, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY,
                         0x3D000167, EXTERNAL_RSA1024_EXPORT_NONE, sizeof(EXTERNAL_RSA1024_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2168(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 1024, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000168, EXTERNAL_RSA1024_VERMSG_PSSANYHASH,
                         sizeof(EXTERNAL_RSA1024_VERMSG_PSSANYHASH), ret);
}

static void el2go_blob_test_external_2169(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 1024, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000169, EXTERNAL_RSA1024_VERMSG_PKCS1V15ANYHASH,
                         sizeof(EXTERNAL_RSA1024_VERMSG_PKCS1V15ANYHASH), ret);
}

static void el2go_blob_test_external_216A(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 1024, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D00016A, EXTERNAL_RSA1024_VERHASH_PSSANYHASH,
                          sizeof(EXTERNAL_RSA1024_VERHASH_PSSANYHASH), ret);
}

static void el2go_blob_test_external_216B(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 1024, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D00016B, EXTERNAL_RSA1024_VERHASH_PKCS1V15ANYHASH,
                          sizeof(EXTERNAL_RSA1024_VERHASH_PKCS1V15ANYHASH), ret);
}

#ifndef CONFIG_RUN_VERIFIED_ONLY
static void el2go_blob_test_external_216C(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 1024, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D00016C, EXTERNAL_RSA1024_VERHASH_RAW,
                          sizeof(EXTERNAL_RSA1024_VERHASH_RAW), ret);
}
#endif

static void el2go_blob_test_external_216D(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 1024, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY,
                         0x3D00016D, EXTERNAL_RSA1024_NONE_NONE, sizeof(EXTERNAL_RSA1024_NONE_NONE), ret);
}

static void el2go_blob_test_external_2170(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 2048, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000170, EXTERNAL_RSA2048_SIGVERMSG_PSSANYHASH,
                            sizeof(EXTERNAL_RSA2048_SIGVERMSG_PSSANYHASH), ret);
}

static void el2go_blob_test_external_2171(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 2048, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000171,
                            EXTERNAL_RSA2048_SIGVERMSG_PKCS1V15ANYHASH,
                            sizeof(EXTERNAL_RSA2048_SIGVERMSG_PKCS1V15ANYHASH), ret);
}

static void el2go_blob_test_external_2172(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 2048, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000172, EXTERNAL_RSA2048_SIGVERHASH_PSSANYHASH,
                             sizeof(EXTERNAL_RSA2048_SIGVERHASH_PSSANYHASH), ret);
}

static void el2go_blob_test_external_2173(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 2048, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000173,
                             EXTERNAL_RSA2048_SIGVERHASH_PKCS1V15ANYHASH,
                             sizeof(EXTERNAL_RSA2048_SIGVERHASH_PKCS1V15ANYHASH), ret);
}

#ifndef CONFIG_RUN_VERIFIED_ONLY
static void el2go_blob_test_external_2174(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 2048, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000174, EXTERNAL_RSA2048_SIGVERHASH_RAW,
                             sizeof(EXTERNAL_RSA2048_SIGVERHASH_RAW), ret);
}

static void el2go_blob_test_external_2175(struct test_result_t *ret)
{
    psa_blob_crypt_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 2048, PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256),
                        PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000175, EXTERNAL_RSA2048_CRYPT_OAEPSHA256,
                        sizeof(EXTERNAL_RSA2048_CRYPT_OAEPSHA256), ret);
}

static void el2go_blob_test_external_2176(struct test_result_t *ret)
{
    psa_blob_crypt_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 2048, PSA_ALG_RSA_PKCS1V15_CRYPT,
                        PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000176, EXTERNAL_RSA2048_CRYPT_PKCS1V15,
                        sizeof(EXTERNAL_RSA2048_CRYPT_PKCS1V15), ret);
}
#endif

static void el2go_blob_test_external_2177(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 2048, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY,
                         0x3D000177, EXTERNAL_RSA2048_EXPORT_NONE, sizeof(EXTERNAL_RSA2048_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2178(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 2048, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000178, EXTERNAL_RSA2048_VERMSG_PSSANYHASH,
                         sizeof(EXTERNAL_RSA2048_VERMSG_PSSANYHASH), ret);
}

static void el2go_blob_test_external_2179(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 2048, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000179, EXTERNAL_RSA2048_VERMSG_PKCS1V15ANYHASH,
                         sizeof(EXTERNAL_RSA2048_VERMSG_PKCS1V15ANYHASH), ret);
}

static void el2go_blob_test_external_217A(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 2048, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D00017A, EXTERNAL_RSA2048_VERHASH_PSSANYHASH,
                          sizeof(EXTERNAL_RSA2048_VERHASH_PSSANYHASH), ret);
}

static void el2go_blob_test_external_217B(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 2048, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D00017B, EXTERNAL_RSA2048_VERHASH_PKCS1V15ANYHASH,
                          sizeof(EXTERNAL_RSA2048_VERHASH_PKCS1V15ANYHASH), ret);
}

#ifndef CONFIG_RUN_VERIFIED_ONLY
static void el2go_blob_test_external_217C(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 2048, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D00017C, EXTERNAL_RSA2048_VERHASH_RAW,
                          sizeof(EXTERNAL_RSA2048_VERHASH_RAW), ret);
}
#endif

static void el2go_blob_test_external_217D(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 2048, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY,
                         0x3D00017D, EXTERNAL_RSA2048_NONE_NONE, sizeof(EXTERNAL_RSA2048_NONE_NONE), ret);
}

static void el2go_blob_test_external_2180(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 3072, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000180, EXTERNAL_RSA3072_SIGVERMSG_PSSANYHASH,
                            sizeof(EXTERNAL_RSA3072_SIGVERMSG_PSSANYHASH), ret);
}

static void el2go_blob_test_external_2181(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 3072, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000181,
                            EXTERNAL_RSA3072_SIGVERMSG_PKCS1V15ANYHASH,
                            sizeof(EXTERNAL_RSA3072_SIGVERMSG_PKCS1V15ANYHASH), ret);
}

static void el2go_blob_test_external_2182(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 3072, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000182, EXTERNAL_RSA3072_SIGVERHASH_PSSANYHASH,
                             sizeof(EXTERNAL_RSA3072_SIGVERHASH_PSSANYHASH), ret);
}

static void el2go_blob_test_external_2183(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 3072, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000183,
                             EXTERNAL_RSA3072_SIGVERHASH_PKCS1V15ANYHASH,
                             sizeof(EXTERNAL_RSA3072_SIGVERHASH_PKCS1V15ANYHASH), ret);
}

#ifndef CONFIG_RUN_VERIFIED_ONLY
static void el2go_blob_test_external_2184(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 3072, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000184, EXTERNAL_RSA3072_SIGVERHASH_RAW,
                             sizeof(EXTERNAL_RSA3072_SIGVERHASH_RAW), ret);
}

static void el2go_blob_test_external_2185(struct test_result_t *ret)
{
    psa_blob_crypt_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 3072, PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256),
                        PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000185, EXTERNAL_RSA3072_CRYPT_OAEPSHA256,
                        sizeof(EXTERNAL_RSA3072_CRYPT_OAEPSHA256), ret);
}

static void el2go_blob_test_external_2186(struct test_result_t *ret)
{
    psa_blob_crypt_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 3072, PSA_ALG_RSA_PKCS1V15_CRYPT,
                        PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000186, EXTERNAL_RSA3072_CRYPT_PKCS1V15,
                        sizeof(EXTERNAL_RSA3072_CRYPT_PKCS1V15), ret);
}
#endif

static void el2go_blob_test_external_2187(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 3072, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY,
                         0x3D000187, EXTERNAL_RSA3072_EXPORT_NONE, sizeof(EXTERNAL_RSA3072_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2188(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 3072, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000188, EXTERNAL_RSA3072_VERMSG_PSSANYHASH,
                         sizeof(EXTERNAL_RSA3072_VERMSG_PSSANYHASH), ret);
}

static void el2go_blob_test_external_2189(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 3072, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000189, EXTERNAL_RSA3072_VERMSG_PKCS1V15ANYHASH,
                         sizeof(EXTERNAL_RSA3072_VERMSG_PKCS1V15ANYHASH), ret);
}

static void el2go_blob_test_external_218A(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 3072, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D00018A, EXTERNAL_RSA3072_VERHASH_PSSANYHASH,
                          sizeof(EXTERNAL_RSA3072_VERHASH_PSSANYHASH), ret);
}

static void el2go_blob_test_external_218B(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 3072, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D00018B, EXTERNAL_RSA3072_VERHASH_PKCS1V15ANYHASH,
                          sizeof(EXTERNAL_RSA3072_VERHASH_PKCS1V15ANYHASH), ret);
}

#ifndef CONFIG_RUN_VERIFIED_ONLY
static void el2go_blob_test_external_218C(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 3072, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D00018C, EXTERNAL_RSA3072_VERHASH_RAW,
                          sizeof(EXTERNAL_RSA3072_VERHASH_RAW), ret);
}
#endif

static void el2go_blob_test_external_218D(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 3072, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY,
                         0x3D00018D, EXTERNAL_RSA3072_NONE_NONE, sizeof(EXTERNAL_RSA3072_NONE_NONE), ret);
}

static void el2go_blob_test_external_2190(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 4096, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000190, EXTERNAL_RSA4096_SIGVERMSG_PSSANYHASH,
                            sizeof(EXTERNAL_RSA4096_SIGVERMSG_PSSANYHASH), ret);
}

static void el2go_blob_test_external_2191(struct test_result_t *ret)
{
    psa_blob_sigvermsg_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 4096, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                            PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000191,
                            EXTERNAL_RSA4096_SIGVERMSG_PKCS1V15ANYHASH,
                            sizeof(EXTERNAL_RSA4096_SIGVERMSG_PKCS1V15ANYHASH), ret);
}

static void el2go_blob_test_external_2192(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 4096, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000192, EXTERNAL_RSA4096_SIGVERHASH_PSSANYHASH,
                             sizeof(EXTERNAL_RSA4096_SIGVERHASH_PSSANYHASH), ret);
}

static void el2go_blob_test_external_2193(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 4096, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000193,
                             EXTERNAL_RSA4096_SIGVERHASH_PKCS1V15ANYHASH,
                             sizeof(EXTERNAL_RSA4096_SIGVERHASH_PKCS1V15ANYHASH), ret);
}

#ifndef CONFIG_RUN_VERIFIED_ONLY
static void el2go_blob_test_external_2194(struct test_result_t *ret)
{
    psa_blob_sigverhash_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 4096, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
                             PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000194, EXTERNAL_RSA4096_SIGVERHASH_RAW,
                             sizeof(EXTERNAL_RSA4096_SIGVERHASH_RAW), ret);
}

static void el2go_blob_test_external_2195(struct test_result_t *ret)
{
    psa_blob_crypt_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 4096, PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256),
                        PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000195, EXTERNAL_RSA4096_CRYPT_OAEPSHA256,
                        sizeof(EXTERNAL_RSA4096_CRYPT_OAEPSHA256), ret);
}

static void el2go_blob_test_external_2196(struct test_result_t *ret)
{
    psa_blob_crypt_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 4096, PSA_ALG_RSA_PKCS1V15_CRYPT,
                        PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000196, EXTERNAL_RSA4096_CRYPT_PKCS1V15,
                        sizeof(EXTERNAL_RSA4096_CRYPT_PKCS1V15), ret);
}
#endif

static void el2go_blob_test_external_2197(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RSA_KEY_PAIR, 4096, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY,
                         0x3D000197, EXTERNAL_RSA4096_EXPORT_NONE, sizeof(EXTERNAL_RSA4096_EXPORT_NONE), ret);
}

static void el2go_blob_test_external_2198(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 4096, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000198, EXTERNAL_RSA4096_VERMSG_PSSANYHASH,
                         sizeof(EXTERNAL_RSA4096_VERMSG_PSSANYHASH), ret);
}

static void el2go_blob_test_external_2199(struct test_result_t *ret)
{
    psa_blob_vermsg_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 4096, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                         PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D000199, EXTERNAL_RSA4096_VERMSG_PKCS1V15ANYHASH,
                         sizeof(EXTERNAL_RSA4096_VERMSG_PKCS1V15ANYHASH), ret);
}

static void el2go_blob_test_external_219A(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 4096, PSA_ALG_RSA_PSS(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D00019A, EXTERNAL_RSA4096_VERHASH_PSSANYHASH,
                          sizeof(EXTERNAL_RSA4096_VERHASH_PSSANYHASH), ret);
}

static void el2go_blob_test_external_219B(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 4096, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_ANY_HASH),
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D00019B, EXTERNAL_RSA4096_VERHASH_PKCS1V15ANYHASH,
                          sizeof(EXTERNAL_RSA4096_VERHASH_PKCS1V15ANYHASH), ret);
}

#ifndef CONFIG_RUN_VERIFIED_ONLY
static void el2go_blob_test_external_219C(struct test_result_t *ret)
{
    psa_blob_verhash_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 4096, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
                          PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY, 0x3D00019C, EXTERNAL_RSA4096_VERHASH_RAW,
                          sizeof(EXTERNAL_RSA4096_VERHASH_RAW), ret);
}
#endif

static void el2go_blob_test_external_219D(struct test_result_t *ret)
{
    psa_blob_export_test(PSA_KEY_TYPE_RSA_PUBLIC_KEY, 4096, PSA_ALG_NONE, PSA_KEY_LOCATION_S50_ENC_STORAGE_KEY,
                         0x3D00019D, EXTERNAL_RSA4096_NONE_NONE, sizeof(EXTERNAL_RSA4096_NONE_NONE), ret);
}

/* TEST SUITE */

static struct test_t blob_external_tests[] = {
    {&el2go_blob_test_external_2000, "EL2GO_BLOB_TEST_EXTERNAL_2000", "External BIN1B EXPORT NONE"},
    {&el2go_blob_test_external_2001, "EL2GO_BLOB_TEST_EXTERNAL_2001", "External BIN256B EXPORT NONE"},
    {&el2go_blob_test_external_2002, "EL2GO_BLOB_TEST_EXTERNAL_2002", "External BIN512B EXPORT NONE"},
    {&el2go_blob_test_external_2003, "EL2GO_BLOB_TEST_EXTERNAL_2003", "External BIN1024B EXPORT NONE"},
    {&el2go_blob_test_external_2004, "EL2GO_BLOB_TEST_EXTERNAL_2004", "External BIN2783B EXPORT NONE"},
    {&el2go_blob_test_external_2005, "EL2GO_BLOB_TEST_EXTERNAL_2005", "External BIN4096B EXPORT NONE"},
    {&el2go_blob_test_external_2006, "EL2GO_BLOB_TEST_EXTERNAL_2006", "External BIN8063B EXPORT NONE"},

    {&el2go_blob_test_external_2010, "EL2GO_BLOB_TEST_EXTERNAL_2010", "External NISTP192 SIGVERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2011, "EL2GO_BLOB_TEST_EXTERNAL_2011", "External NISTP192 SIGVERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2012, "EL2GO_BLOB_TEST_EXTERNAL_2012", "External NISTP192 SIGVERHASH ECDSAANY"},
    {&el2go_blob_test_external_2013, "EL2GO_BLOB_TEST_EXTERNAL_2013", "External NISTP192 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2014, "EL2GO_BLOB_TEST_EXTERNAL_2014", "External NISTP192 EXPORT NONE"},
    {&el2go_blob_test_external_2015, "EL2GO_BLOB_TEST_EXTERNAL_2015", "External NISTP192 VERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2016, "EL2GO_BLOB_TEST_EXTERNAL_2016", "External NISTP192 VERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2017, "EL2GO_BLOB_TEST_EXTERNAL_2017", "External NISTP192 VERHASH ECDSAANY"},
    {&el2go_blob_test_external_2018, "EL2GO_BLOB_TEST_EXTERNAL_2018", "External NISTP192 NONE NONE"},
    {&el2go_blob_test_external_2020, "EL2GO_BLOB_TEST_EXTERNAL_2020", "External NISTP224 SIGVERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2021, "EL2GO_BLOB_TEST_EXTERNAL_2021", "External NISTP224 SIGVERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2022, "EL2GO_BLOB_TEST_EXTERNAL_2022", "External NISTP224 SIGVERHASH ECDSAANY"},
    {&el2go_blob_test_external_2023, "EL2GO_BLOB_TEST_EXTERNAL_2023", "External NISTP224 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2024, "EL2GO_BLOB_TEST_EXTERNAL_2024", "External NISTP224 EXPORT NONE"},
    {&el2go_blob_test_external_2025, "EL2GO_BLOB_TEST_EXTERNAL_2025", "External NISTP224 VERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2026, "EL2GO_BLOB_TEST_EXTERNAL_2026", "External NISTP224 VERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2027, "EL2GO_BLOB_TEST_EXTERNAL_2027", "External NISTP224 VERHASH ECDSAANY"},
    {&el2go_blob_test_external_2028, "EL2GO_BLOB_TEST_EXTERNAL_2028", "External NISTP224 NONE NONE"},
    {&el2go_blob_test_external_1023, "EL2GO_BLOB_TEST_EXTERNAL_1023", "External NISTP256 VERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_1024, "EL2GO_BLOB_TEST_EXTERNAL_1024", "External NISTP256 VERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_1025, "EL2GO_BLOB_TEST_EXTERNAL_1025", "External NISTP256 VERHASH ECDSAANY"},
    {&el2go_blob_test_external_1026, "EL2GO_BLOB_TEST_EXTERNAL_1026", "External NISTP256 NONE NONE"},
    {&el2go_blob_test_external_2030, "EL2GO_BLOB_TEST_EXTERNAL_2030", "External NISTP384 SIGVERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2031, "EL2GO_BLOB_TEST_EXTERNAL_2031", "External NISTP384 SIGVERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2032, "EL2GO_BLOB_TEST_EXTERNAL_2032", "External NISTP384 SIGVERHASH ECDSAANY"},
    {&el2go_blob_test_external_2033, "EL2GO_BLOB_TEST_EXTERNAL_2033", "External NISTP384 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2034, "EL2GO_BLOB_TEST_EXTERNAL_2034", "External NISTP384 EXPORT NONE"},
    {&el2go_blob_test_external_2035, "EL2GO_BLOB_TEST_EXTERNAL_2035", "External NISTP384 VERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2036, "EL2GO_BLOB_TEST_EXTERNAL_2036", "External NISTP384 VERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2037, "EL2GO_BLOB_TEST_EXTERNAL_2037", "External NISTP384 VERHASH ECDSAANY"},
    {&el2go_blob_test_external_2038, "EL2GO_BLOB_TEST_EXTERNAL_2038", "External NISTP384 NONE NONE"},
    {&el2go_blob_test_external_2040, "EL2GO_BLOB_TEST_EXTERNAL_2040", "External NISTP521 SIGVERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2041, "EL2GO_BLOB_TEST_EXTERNAL_2041", "External NISTP521 SIGVERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2042, "EL2GO_BLOB_TEST_EXTERNAL_2042", "External NISTP521 SIGVERHASH ECDSAANY"},
    {&el2go_blob_test_external_2043, "EL2GO_BLOB_TEST_EXTERNAL_2043", "External NISTP521 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2044, "EL2GO_BLOB_TEST_EXTERNAL_2044", "External NISTP521 EXPORT NONE"},
    {&el2go_blob_test_external_2045, "EL2GO_BLOB_TEST_EXTERNAL_2045", "External NISTP521 VERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2046, "EL2GO_BLOB_TEST_EXTERNAL_2046", "External NISTP521 VERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2047, "EL2GO_BLOB_TEST_EXTERNAL_2047", "External NISTP521 VERHASH ECDSAANY"},
    {&el2go_blob_test_external_2048, "EL2GO_BLOB_TEST_EXTERNAL_2048", "External NISTP521 NONE NONE"},

    {&el2go_blob_test_external_2050, "EL2GO_BLOB_TEST_EXTERNAL_2050",
     "External BRAINPOOLP192R1 SIGVERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2051, "EL2GO_BLOB_TEST_EXTERNAL_2051",
     "External BRAINPOOLP192R1 SIGVERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2052, "EL2GO_BLOB_TEST_EXTERNAL_2052", "External BRAINPOOLP192R1 SIGVERHASH ECDSAANY"},
    {&el2go_blob_test_external_2053, "EL2GO_BLOB_TEST_EXTERNAL_2053", "External BRAINPOOLP192R1 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2054, "EL2GO_BLOB_TEST_EXTERNAL_2054", "External BRAINPOOLP192R1 EXPORT NONE"},
    {&el2go_blob_test_external_2055, "EL2GO_BLOB_TEST_EXTERNAL_2055", "External BRAINPOOLP192R1 VERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2056, "EL2GO_BLOB_TEST_EXTERNAL_2056", "External BRAINPOOLP192R1 VERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2057, "EL2GO_BLOB_TEST_EXTERNAL_2057", "External BRAINPOOLP192R1 VERHASH ECDSAANY"},
    {&el2go_blob_test_external_2058, "EL2GO_BLOB_TEST_EXTERNAL_2058", "External BRAINPOOLP192R1 NONE NONE"},
    {&el2go_blob_test_external_2060, "EL2GO_BLOB_TEST_EXTERNAL_2060",
     "External BRAINPOOLP224R1 SIGVERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2061, "EL2GO_BLOB_TEST_EXTERNAL_2061",
     "External BRAINPOOLP224R1 SIGVERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2062, "EL2GO_BLOB_TEST_EXTERNAL_2062", "External BRAINPOOLP224R1 SIGVERHASH ECDSAANY"},
    {&el2go_blob_test_external_2063, "EL2GO_BLOB_TEST_EXTERNAL_2063", "External BRAINPOOLP224R1 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2064, "EL2GO_BLOB_TEST_EXTERNAL_2064", "External BRAINPOOLP224R1 EXPORT NONE"},
    {&el2go_blob_test_external_2065, "EL2GO_BLOB_TEST_EXTERNAL_2065", "External BRAINPOOLP224R1 VERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2066, "EL2GO_BLOB_TEST_EXTERNAL_2066", "External BRAINPOOLP224R1 VERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2067, "EL2GO_BLOB_TEST_EXTERNAL_2067", "External BRAINPOOLP224R1 VERHASH ECDSAANY"},
    {&el2go_blob_test_external_2068, "EL2GO_BLOB_TEST_EXTERNAL_2068", "External BRAINPOOLP224R1 NONE NONE"},
    {&el2go_blob_test_external_2070, "EL2GO_BLOB_TEST_EXTERNAL_2070",
     "External BRAINPOOLP256R1 SIGVERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2071, "EL2GO_BLOB_TEST_EXTERNAL_2071",
     "External BRAINPOOLP256R1 SIGVERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2072, "EL2GO_BLOB_TEST_EXTERNAL_2072", "External BRAINPOOLP256R1 SIGVERHASH ECDSAANY"},
    {&el2go_blob_test_external_2073, "EL2GO_BLOB_TEST_EXTERNAL_2073", "External BRAINPOOLP256R1 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2074, "EL2GO_BLOB_TEST_EXTERNAL_2074", "External BRAINPOOLP256R1 EXPORT NONE"},
    {&el2go_blob_test_external_2075, "EL2GO_BLOB_TEST_EXTERNAL_2075", "External BRAINPOOLP256R1 VERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2076, "EL2GO_BLOB_TEST_EXTERNAL_2076", "External BRAINPOOLP256R1 VERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2077, "EL2GO_BLOB_TEST_EXTERNAL_2077", "External BRAINPOOLP256R1 VERHASH ECDSAANY"},
    {&el2go_blob_test_external_2078, "EL2GO_BLOB_TEST_EXTERNAL_2078", "External BRAINPOOLP256R1 NONE NONE"},
    {&el2go_blob_test_external_2080, "EL2GO_BLOB_TEST_EXTERNAL_2080",
     "External BRAINPOOLP320R1 SIGVERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2081, "EL2GO_BLOB_TEST_EXTERNAL_2081",
     "External BRAINPOOLP320R1 SIGVERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2082, "EL2GO_BLOB_TEST_EXTERNAL_2082", "External BRAINPOOLP320R1 SIGVERHASH ECDSAANY"},
    {&el2go_blob_test_external_2083, "EL2GO_BLOB_TEST_EXTERNAL_2083", "External BRAINPOOLP320R1 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2084, "EL2GO_BLOB_TEST_EXTERNAL_2084", "External BRAINPOOLP320R1 EXPORT NONE"},
    {&el2go_blob_test_external_2085, "EL2GO_BLOB_TEST_EXTERNAL_2085", "External BRAINPOOLP320R1 VERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2086, "EL2GO_BLOB_TEST_EXTERNAL_2086", "External BRAINPOOLP320R1 VERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2087, "EL2GO_BLOB_TEST_EXTERNAL_2087", "External BRAINPOOLP320R1 VERHASH ECDSAANY"},
    {&el2go_blob_test_external_2088, "EL2GO_BLOB_TEST_EXTERNAL_2088", "External BRAINPOOLP320R1 NONE NONE"},
    {&el2go_blob_test_external_2090, "EL2GO_BLOB_TEST_EXTERNAL_2090",
     "External BRAINPOOLP384R1 SIGVERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2091, "EL2GO_BLOB_TEST_EXTERNAL_2091",
     "External BRAINPOOLP384R1 SIGVERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2092, "EL2GO_BLOB_TEST_EXTERNAL_2092", "External BRAINPOOLP384R1 SIGVERHASH ECDSAANY"},
    {&el2go_blob_test_external_2093, "EL2GO_BLOB_TEST_EXTERNAL_2093", "External BRAINPOOLP384R1 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2094, "EL2GO_BLOB_TEST_EXTERNAL_2094", "External BRAINPOOLP384R1 EXPORT NONE"},
    {&el2go_blob_test_external_2095, "EL2GO_BLOB_TEST_EXTERNAL_2095", "External BRAINPOOLP384R1 VERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2096, "EL2GO_BLOB_TEST_EXTERNAL_2096", "External BRAINPOOLP384R1 VERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2097, "EL2GO_BLOB_TEST_EXTERNAL_2097", "External BRAINPOOLP384R1 VERHASH ECDSAANY"},
    {&el2go_blob_test_external_2098, "EL2GO_BLOB_TEST_EXTERNAL_2098", "External BRAINPOOLP384R1 NONE NONE"},
    {&el2go_blob_test_external_2100, "EL2GO_BLOB_TEST_EXTERNAL_2100",
     "External BRAINPOOLP512R1 SIGVERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2101, "EL2GO_BLOB_TEST_EXTERNAL_2101",
     "External BRAINPOOLP512R1 SIGVERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2102, "EL2GO_BLOB_TEST_EXTERNAL_2102", "External BRAINPOOLP512R1 SIGVERHASH ECDSAANY"},
    {&el2go_blob_test_external_2103, "EL2GO_BLOB_TEST_EXTERNAL_2103", "External BRAINPOOLP512R1 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2104, "EL2GO_BLOB_TEST_EXTERNAL_2104", "External BRAINPOOLP512R1 EXPORT NONE"},
    {&el2go_blob_test_external_2105, "EL2GO_BLOB_TEST_EXTERNAL_2105", "External BRAINPOOLP512R1 VERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2106, "EL2GO_BLOB_TEST_EXTERNAL_2106", "External BRAINPOOLP512R1 VERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2107, "EL2GO_BLOB_TEST_EXTERNAL_2107", "External BRAINPOOLP512R1 VERHASH ECDSAANY"},
    {&el2go_blob_test_external_2108, "EL2GO_BLOB_TEST_EXTERNAL_2108", "External BRAINPOOLP512R1 NONE NONE"},

    {&el2go_blob_test_external_2110, "EL2GO_BLOB_TEST_EXTERNAL_2110", "External KOBLITZ192 SIGVERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2111, "EL2GO_BLOB_TEST_EXTERNAL_2111", "External KOBLITZ192 SIGVERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2112, "EL2GO_BLOB_TEST_EXTERNAL_2112", "External KOBLITZ192 SIGVERHASH ECDSAANY"},
    {&el2go_blob_test_external_2113, "EL2GO_BLOB_TEST_EXTERNAL_2113", "External KOBLITZ192 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2114, "EL2GO_BLOB_TEST_EXTERNAL_2114", "External KOBLITZ192 EXPORT NONE"},
    {&el2go_blob_test_external_2115, "EL2GO_BLOB_TEST_EXTERNAL_2115", "External KOBLITZ192 VERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2116, "EL2GO_BLOB_TEST_EXTERNAL_2116", "External KOBLITZ192 VERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2117, "EL2GO_BLOB_TEST_EXTERNAL_2117", "External KOBLITZ192 VERHASH ECDSAANY"},
    {&el2go_blob_test_external_2118, "EL2GO_BLOB_TEST_EXTERNAL_2118", "External KOBLITZ192 NONE NONE"},
#ifndef CONFIG_RUN_VERIFIED_ONLY
    {&el2go_blob_test_external_2120, "EL2GO_BLOB_TEST_EXTERNAL_2120", "External KOBLITZ224 SIGVERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2121, "EL2GO_BLOB_TEST_EXTERNAL_2121", "External KOBLITZ224 SIGVERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2122, "EL2GO_BLOB_TEST_EXTERNAL_2122", "External KOBLITZ224 SIGVERHASH ECDSAANY"},
    {&el2go_blob_test_external_2123, "EL2GO_BLOB_TEST_EXTERNAL_2123", "External KOBLITZ224 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2124, "EL2GO_BLOB_TEST_EXTERNAL_2124", "External KOBLITZ224 EXPORT NONE"},
    {&el2go_blob_test_external_2125, "EL2GO_BLOB_TEST_EXTERNAL_2125", "External KOBLITZ224 VERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2126, "EL2GO_BLOB_TEST_EXTERNAL_2126", "External KOBLITZ224 VERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2127, "EL2GO_BLOB_TEST_EXTERNAL_2127", "External KOBLITZ224 VERHASH ECDSAANY"},
    {&el2go_blob_test_external_2128, "EL2GO_BLOB_TEST_EXTERNAL_2128", "External KOBLITZ224 NONE NONE"},
#endif
    {&el2go_blob_test_external_2130, "EL2GO_BLOB_TEST_EXTERNAL_2130", "External KOBLITZ256 SIGVERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2131, "EL2GO_BLOB_TEST_EXTERNAL_2131", "External KOBLITZ256 SIGVERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2132, "EL2GO_BLOB_TEST_EXTERNAL_2132", "External KOBLITZ256 SIGVERHASH ECDSAANY"},
    {&el2go_blob_test_external_2133, "EL2GO_BLOB_TEST_EXTERNAL_2133", "External KOBLITZ256 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2134, "EL2GO_BLOB_TEST_EXTERNAL_2134", "External KOBLITZ256 EXPORT NONE"},
    {&el2go_blob_test_external_2135, "EL2GO_BLOB_TEST_EXTERNAL_2135", "External KOBLITZ256 VERMSG ECDSAANYHASH"},
    {&el2go_blob_test_external_2136, "EL2GO_BLOB_TEST_EXTERNAL_2136", "External KOBLITZ256 VERHASH ECDSAANYHASH"},
    {&el2go_blob_test_external_2137, "EL2GO_BLOB_TEST_EXTERNAL_2137", "External KOBLITZ256 VERHASH ECDSAANY"},
    {&el2go_blob_test_external_2138, "EL2GO_BLOB_TEST_EXTERNAL_2138", "External KOBLITZ256 NONE NONE"},

    {&el2go_blob_test_external_2140, "EL2GO_BLOB_TEST_EXTERNAL_2140", "External MONTDH25519 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2141, "EL2GO_BLOB_TEST_EXTERNAL_2141", "External MONTDH25519 EXPORT NONE"},
    {&el2go_blob_test_external_2142, "EL2GO_BLOB_TEST_EXTERNAL_2142", "External MONTDH25519 NONE NONE"},
    {&el2go_blob_test_external_2150, "EL2GO_BLOB_TEST_EXTERNAL_2150", "External MONTDH448 KEYEXCH ECDH"},
    {&el2go_blob_test_external_2151, "EL2GO_BLOB_TEST_EXTERNAL_2151", "External MONTDH448 EXPORT NONE"},
    {&el2go_blob_test_external_2152, "EL2GO_BLOB_TEST_EXTERNAL_2152", "External MONTDH448 NONE NONE"},

    {&el2go_blob_test_external_2160, "EL2GO_BLOB_TEST_EXTERNAL_2160", "External RSA1024 SIGVERMSG PSSANYHASH"},
    {&el2go_blob_test_external_2161, "EL2GO_BLOB_TEST_EXTERNAL_2161", "External RSA1024 SIGVERMSG PKCS1V15ANYHASH"},
    {&el2go_blob_test_external_2162, "EL2GO_BLOB_TEST_EXTERNAL_2162", "External RSA1024 SIGVERHASH PSSANYHASH"},
    {&el2go_blob_test_external_2163, "EL2GO_BLOB_TEST_EXTERNAL_2163", "External RSA1024 SIGVERHASH PKCS1V15ANYHASH"},
#ifndef CONFIG_RUN_VERIFIED_ONLY
    {&el2go_blob_test_external_2164, "EL2GO_BLOB_TEST_EXTERNAL_2164", "External RSA1024 SIGVERHASH RAW"},
    {&el2go_blob_test_external_2165, "EL2GO_BLOB_TEST_EXTERNAL_2165", "External RSA1024 CRYPT OAEPSHA256"},
    {&el2go_blob_test_external_2166, "EL2GO_BLOB_TEST_EXTERNAL_2166", "External RSA1024 CRYPT PKCS1V15"},
#endif
    {&el2go_blob_test_external_2167, "EL2GO_BLOB_TEST_EXTERNAL_2167", "External RSA1024 EXPORT NONE"},
    {&el2go_blob_test_external_2168, "EL2GO_BLOB_TEST_EXTERNAL_2168", "External RSA1024 VERMSG PSSANYHASH"},
    {&el2go_blob_test_external_2169, "EL2GO_BLOB_TEST_EXTERNAL_2169", "External RSA1024 VERMSG PKCS1V15ANYHASH"},
    {&el2go_blob_test_external_216A, "EL2GO_BLOB_TEST_EXTERNAL_216A", "External RSA1024 VERHASH PSSANYHASH"},
    {&el2go_blob_test_external_216B, "EL2GO_BLOB_TEST_EXTERNAL_216B", "External RSA1024 VERHASH PKCS1V15ANYHASH"},
#ifndef CONFIG_RUN_VERIFIED_ONLY
    {&el2go_blob_test_external_216C, "EL2GO_BLOB_TEST_EXTERNAL_216C", "External RSA1024 VERHASH RAW"},
#endif
    {&el2go_blob_test_external_216D, "EL2GO_BLOB_TEST_EXTERNAL_216D", "External RSA1024 NONE NONE"},
    {&el2go_blob_test_external_2170, "EL2GO_BLOB_TEST_EXTERNAL_2170", "External RSA2048 SIGVERMSG PSSANYHASH"},
    {&el2go_blob_test_external_2171, "EL2GO_BLOB_TEST_EXTERNAL_2171", "External RSA2048 SIGVERMSG PKCS1V15ANYHASH"},
    {&el2go_blob_test_external_2172, "EL2GO_BLOB_TEST_EXTERNAL_2172", "External RSA2048 SIGVERHASH PSSANYHASH"},
    {&el2go_blob_test_external_2173, "EL2GO_BLOB_TEST_EXTERNAL_2173", "External RSA2048 SIGVERHASH PKCS1V15ANYHASH"},
#ifndef CONFIG_RUN_VERIFIED_ONLY
    {&el2go_blob_test_external_2174, "EL2GO_BLOB_TEST_EXTERNAL_2174", "External RSA2048 SIGVERHASH RAW"},
    {&el2go_blob_test_external_2175, "EL2GO_BLOB_TEST_EXTERNAL_2175", "External RSA2048 CRYPT OAEPSHA256"},
    {&el2go_blob_test_external_2176, "EL2GO_BLOB_TEST_EXTERNAL_2176", "External RSA2048 CRYPT PKCS1V15"},
#endif
    {&el2go_blob_test_external_2177, "EL2GO_BLOB_TEST_EXTERNAL_2177", "External RSA2048 EXPORT NONE"},
    {&el2go_blob_test_external_2178, "EL2GO_BLOB_TEST_EXTERNAL_2178", "External RSA2048 VERMSG PSSANYHASH"},
    {&el2go_blob_test_external_2179, "EL2GO_BLOB_TEST_EXTERNAL_2179", "External RSA2048 VERMSG PKCS1V15ANYHASH"},
    {&el2go_blob_test_external_217A, "EL2GO_BLOB_TEST_EXTERNAL_217A", "External RSA2048 VERHASH PSSANYHASH"},
    {&el2go_blob_test_external_217B, "EL2GO_BLOB_TEST_EXTERNAL_217B", "External RSA2048 VERHASH PKCS1V15ANYHASH"},
#ifndef CONFIG_RUN_VERIFIED_ONLY
    {&el2go_blob_test_external_217C, "EL2GO_BLOB_TEST_EXTERNAL_217C", "External RSA2048 VERHASH RAW"},
#endif
    {&el2go_blob_test_external_217D, "EL2GO_BLOB_TEST_EXTERNAL_217D", "External RSA2048 NONE NONE"},
    {&el2go_blob_test_external_2180, "EL2GO_BLOB_TEST_EXTERNAL_2180", "External RSA3072 SIGVERMSG PSSANYHASH"},
    {&el2go_blob_test_external_2181, "EL2GO_BLOB_TEST_EXTERNAL_2181", "External RSA3072 SIGVERMSG PKCS1V15ANYHASH"},
    {&el2go_blob_test_external_2182, "EL2GO_BLOB_TEST_EXTERNAL_2182", "External RSA3072 SIGVERHASH PSSANYHASH"},
    {&el2go_blob_test_external_2183, "EL2GO_BLOB_TEST_EXTERNAL_2183", "External RSA3072 SIGVERHASH PKCS1V15ANYHASH"},
#ifndef CONFIG_RUN_VERIFIED_ONLY
    {&el2go_blob_test_external_2184, "EL2GO_BLOB_TEST_EXTERNAL_2184", "External RSA3072 SIGVERHASH RAW"},
    {&el2go_blob_test_external_2185, "EL2GO_BLOB_TEST_EXTERNAL_2185", "External RSA3072 CRYPT OAEPSHA256"},
    {&el2go_blob_test_external_2186, "EL2GO_BLOB_TEST_EXTERNAL_2186", "External RSA3072 CRYPT PKCS1V15"},
#endif
    {&el2go_blob_test_external_2187, "EL2GO_BLOB_TEST_EXTERNAL_2187", "External RSA3072 EXPORT NONE"},
    {&el2go_blob_test_external_2188, "EL2GO_BLOB_TEST_EXTERNAL_2188", "External RSA3072 VERMSG PSSANYHASH"},
    {&el2go_blob_test_external_2189, "EL2GO_BLOB_TEST_EXTERNAL_2189", "External RSA3072 VERMSG PKCS1V15ANYHASH"},
    {&el2go_blob_test_external_218A, "EL2GO_BLOB_TEST_EXTERNAL_218A", "External RSA3072 VERHASH PSSANYHASH"},
    {&el2go_blob_test_external_218B, "EL2GO_BLOB_TEST_EXTERNAL_218B", "External RSA3072 VERHASH PKCS1V15ANYHASH"},
#ifndef CONFIG_RUN_VERIFIED_ONLY
    {&el2go_blob_test_external_218C, "EL2GO_BLOB_TEST_EXTERNAL_218C", "External RSA3072 VERHASH RAW"},
#endif
    {&el2go_blob_test_external_218D, "EL2GO_BLOB_TEST_EXTERNAL_218D", "External RSA3072 NONE NONE"},
    {&el2go_blob_test_external_2190, "EL2GO_BLOB_TEST_EXTERNAL_2190", "External RSA4096 SIGVERMSG PSSANYHASH"},
    {&el2go_blob_test_external_2191, "EL2GO_BLOB_TEST_EXTERNAL_2191", "External RSA4096 SIGVERMSG PKCS1V15ANYHASH"},
    {&el2go_blob_test_external_2192, "EL2GO_BLOB_TEST_EXTERNAL_2192", "External RSA4096 SIGVERHASH PSSANYHASH"},
    {&el2go_blob_test_external_2193, "EL2GO_BLOB_TEST_EXTERNAL_2193", "External RSA4096 SIGVERHASH PKCS1V15ANYHASH"},
#ifndef CONFIG_RUN_VERIFIED_ONLY
    {&el2go_blob_test_external_2194, "EL2GO_BLOB_TEST_EXTERNAL_2194", "External RSA4096 SIGVERHASH RAW"},
    {&el2go_blob_test_external_2195, "EL2GO_BLOB_TEST_EXTERNAL_2195", "External RSA4096 CRYPT OAEPSHA256"},
    {&el2go_blob_test_external_2196, "EL2GO_BLOB_TEST_EXTERNAL_2196", "External RSA4096 CRYPT PKCS1V15"},
#endif
    {&el2go_blob_test_external_2197, "EL2GO_BLOB_TEST_EXTERNAL_2197", "External RSA4096 EXPORT NONE"},
    {&el2go_blob_test_external_2198, "EL2GO_BLOB_TEST_EXTERNAL_2198", "External RSA4096 VERMSG PSSANYHASH"},
    {&el2go_blob_test_external_2199, "EL2GO_BLOB_TEST_EXTERNAL_2199", "External RSA4096 VERMSG PKCS1V15ANYHASH"},
    {&el2go_blob_test_external_219A, "EL2GO_BLOB_TEST_EXTERNAL_219A", "External RSA4096 VERHASH PSSANYHASH"},
    {&el2go_blob_test_external_219B, "EL2GO_BLOB_TEST_EXTERNAL_219B", "External RSA4096 VERHASH PKCS1V15ANYHASH"},
#ifndef CONFIG_RUN_VERIFIED_ONLY
    {&el2go_blob_test_external_219C, "EL2GO_BLOB_TEST_EXTERNAL_219C", "External RSA4096 VERHASH RAW"},
#endif
    {&el2go_blob_test_external_219D, "EL2GO_BLOB_TEST_EXTERNAL_219D", "External RSA4096 NONE NONE"}};

void testsuite_blob_external(struct test_suite_t *test_suite)
{
    test_suite->name           = "EXTERNAL (EL2GO_BLOB_TEST_EXTERNAL_2XXX)";
    test_suite->test_list      = blob_external_tests;
    test_suite->test_list_size = (sizeof(blob_external_tests) / sizeof(blob_external_tests[0]));
}
