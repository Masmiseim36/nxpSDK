/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

#ifndef __PSA_MANIFEST_TFM_IPC_SERVICE_TEST_H__
#define __PSA_MANIFEST_TFM_IPC_SERVICE_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

#define TFM_SP_IPC_SERVICE_TEST_MODEL_IPC                       1
#define TFM_SP_IPC_SERVICE_TEST_MODEL_SFN                       0

#define IPC_SERVICE_TEST_BASIC_SIGNAL                           (0x10)
#define IPC_SERVICE_TEST_PSA_ACCESS_APP_MEM_SIGNAL              (0x20)
#define IPC_SERVICE_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SIGNAL    (0x40)
#define IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM_SIGNAL              (0x80)
#define IPC_SERVICE_TEST_CLIENT_PROGRAMMER_ERROR_SIGNAL         (0x100)
#define IPC_SERVICE_TEST_STATELESS_ROT_SIGNAL                   (0x200)
#define IPC_SERVICE_TEST_MMIOVEC_SIGNAL                         (0x400)


#ifdef __cplusplus
}
#endif

#endif /* __PSA_MANIFEST_TFM_IPC_SERVICE_TEST_H__ */
