/*
 * Copyright (c) 2017-2021 Arm Limited. All rights reserved.
 * Copyright (c) 2022 NXP. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef CRYPTO_HW_ACCELERATOR

#include "fsl_device_registers.h"
#include "crypto_hw.h"
#include "tfm_plat_defs.h"

#include <string.h>
#include "tfm_plat_otp.h"
#include "mbedtls/hkdf.h"
/*
 * \brief  This function performs key derivation
 *
 * \param[in]  label         Label for KDF
 * \param[in]  label_size    Size of the label
 * \param[in]  context       Context for KDF
 * \param[in]  context_size  Size of the context
 * \param[out] key           Buffer to output the derived key material
 * \param[in]  key_size      Requested size of the derived key material and
 *                           minimum size of the key buffer
 *
 * \return 0 on success, non-zero otherwise
 */
int crypto_hw_accelerator_huk_derive_key(const uint8_t *label,
                                         size_t label_size,
                                         const uint8_t *context,
                                         size_t context_size,
                                         uint8_t *key,
                                         size_t key_size)
{
    uint8_t huk_buf[32];
    enum tfm_plat_err_t err;
    int mbedtls_err;

    if (key == NULL) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    if (label == NULL && label_size != 0) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    if (context == NULL && context_size != 0) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    err = tfm_plat_otp_read(PLAT_OTP_ID_HUK, sizeof(huk_buf), huk_buf);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        goto out;
    }

    mbedtls_err = mbedtls_hkdf(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
                               label, label_size, huk_buf, sizeof(huk_buf),
                               context, context_size, key, key_size);
    if (mbedtls_err) {
        err = TFM_PLAT_ERR_SYSTEM_ERR;
        goto out;
    }

out:
    memset(huk_buf, 0, sizeof(huk_buf));

    return err;
}

#endif /* CRYPTO_HW_ACCELERATOR */
