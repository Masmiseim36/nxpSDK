/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __PSA_MANIFEST_TFM_IPC_CLIENT_TEST_H__
#define __PSA_MANIFEST_TFM_IPC_CLIENT_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

#define TFM_SP_IPC_CLIENT_TEST_MODEL_IPC                        1
#define TFM_SP_IPC_CLIENT_TEST_MODEL_SFN                        0


#define IPC_CLIENT_TEST_BASIC_SIGNAL                            (1U << 4)
#define IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM_SIGNAL               (1U << 5)
#define IPC_CLIENT_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SIGNAL     (1U << 6)
#define IPC_CLIENT_TEST_APP_ACCESS_PSA_MEM_SIGNAL               (1U << 7)
#define IPC_CLIENT_TEST_MEM_CHECK_SIGNAL                        (1U << 8)
#define IPC_CLIENT_TEST_RETRIEVE_APP_MEM_SIGNAL                 (1U << 9)


#ifdef __cplusplus
}
#endif

#endif /* __PSA_MANIFEST_TFM_IPC_CLIENT_TEST_H__ */
