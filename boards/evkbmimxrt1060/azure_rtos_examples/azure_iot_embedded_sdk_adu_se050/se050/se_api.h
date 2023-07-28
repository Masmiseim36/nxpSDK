/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SE_API_H_
#define SE_API_H_

#ifdef __cplusplus
extern   "C" {
#endif

#include "nx_api.h"

UINT se_setup(VOID);
UINT se_open(VOID);
VOID se_close(VOID);

UINT se_device_get_certificate(UCHAR *buf, UINT *size);

UINT se_device_ecdsa_sign(UCHAR *hash, UINT hash_length,
                          UCHAR *signature, UINT *signature_length);

UINT se_device_ecdsa_verify(UCHAR *public_key, UINT public_key_length,
                            UCHAR *signature, UINT signature_length);

#ifdef __cplusplus
}
#endif

#endif /* SE_API_H_ */
