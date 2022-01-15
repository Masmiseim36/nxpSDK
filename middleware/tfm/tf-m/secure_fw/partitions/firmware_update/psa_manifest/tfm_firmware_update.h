/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __PSA_MANIFEST_TFM_FIRMWARE_UPDATE_H__
#define __PSA_MANIFEST_TFM_FIRMWARE_UPDATE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define TFM_SP_FWU_MODEL_IPC                                    1
#define TFM_SP_FWU_MODEL_SFN                                    0


#define TFM_FWU_WRITE_SIGNAL                                    (1U << 4)
#define TFM_FWU_INSTALL_SIGNAL                                  (1U << 5)
#define TFM_FWU_ABORT_SIGNAL                                    (1U << 6)
#define TFM_FWU_QUERY_SIGNAL                                    (1U << 7)
#define TFM_FWU_REQUEST_REBOOT_SIGNAL                           (1U << 8)
#define TFM_FWU_ACCEPT_SIGNAL                                   (1U << 9)


#ifdef __cplusplus
}
#endif

#endif /* __PSA_MANIFEST_TFM_FIRMWARE_UPDATE_H__ */
