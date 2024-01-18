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
#if defined(ELS)

#include "mcux_els.h"
#include "fsl_debug_console.h"

#include "psa/crypto.h"

int crypto_hw_accelerator_huk_derive_key(const uint8_t *label,
                                         size_t label_size,
                                         const uint8_t *context,
                                         size_t context_size,
                                         uint8_t *key,
                                         size_t key_size)
{
    enum tfm_plat_err_t             err = 0;
    mcuxClCss_KeyIndex_t            keyIdx = 0;
    mcuxClCss_KeyProp_t             KeyProp;
    uint8_t                         key_buf[32]; /* 256-bit key. */
    uint8_t                         label_buf[32] = {0}; /* It have to be padded to the 32 bytes. */   

    if ((key == NULL) || (key_size > 32)) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    if ((label == NULL && label_size != 0) || (label_size > sizeof(label_buf))) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

    if (context == NULL && context_size != 0) {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }
    
    /* Get free CSS key slot */
    for(keyIdx = 0; keyIdx < MCUXCLCSS_KEY_SLOTS; keyIdx++){
        /* Get CSS keystore slot properties */
        MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(result, token, mcuxClCss_GetKeyProperties(keyIdx, &KeyProp));
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_GetKeyProperties) != token) || (MCUXCLCSS_STATUS_OK != result)){
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }

        if(KeyProp.bits.kactv == 0){
            break; /* Found free key slot */
        }
    }; 
    
    if(keyIdx < MCUXCLCSS_KEY_SLOTS) /* Free key slot found */
    {
        /* Derive a key using the NIST SP 800-108 CMAC-based Extract-and-Expand Key Derivation Function.*/
        const mcuxClCss_KeyProp_t targetKeyProperties = { .bits.ksize = MCUXCLCSS_KEYPROPERTY_KEY_SIZE_256, 
                                                          .bits.uhkdf = 1, }; /* Must be a 256-bit key with HKDF property bit set to 1. */
        const uint8_t DerivationData[MCUXCLCSS_CKDF_DERIVATIONDATA_SIZE] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC};
        MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(resultCkdf, tokenCkdf, mcuxClCss_Ckdf_Sp800108_Async(
                                     0x0, /* NXP_DIE_MK_SK = DUK : Key index used for derivation */
                                     keyIdx, /* Free slot - Key bank number of the derived key */
                                     targetKeyProperties,
                                     DerivationData));
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_Ckdf_Sp800108_Async) != tokenCkdf) || (MCUXCLCSS_STATUS_OK_WAIT != resultCkdf)){
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(resultWaitCkdf, tokenWaitCkdf, mcuxClCss_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR));
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_WaitForOperation) != tokenWaitCkdf) || (MCUXCLCSS_STATUS_OK != resultWaitCkdf)){
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }
        
        memcpy(label_buf, label, label_size); /* Copy key_size bytes to the key buffer. */
        
        /* Derives a key using the HKDF (HMAC-based key derivation function) according to SP800-56C one-step approach with Sha2-256. */
        MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(resultHkdf, tokenHkdf, mcuxClCss_Hkdf_Sp80056c_Async(
                                     keyIdx,    /* Key index used for derivation. */
                                     key_buf,   /* Memory area to store the derived key. Will be a 256-bit key. */
                                     label_buf,     /* The derivation data */
                                     sizeof(label_buf)));
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_Hkdf_Sp80056c_Async) != tokenHkdf) || (MCUXCLCSS_STATUS_OK_WAIT != resultHkdf)){
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(resultWaitHkdf, tokenWaitHkdf, mcuxClCss_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR));
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_WaitForOperation) != tokenWaitHkdf) || (MCUXCLCSS_STATUS_OK != resultWaitHkdf)){
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }

        memcpy(key, key_buf, key_size); /* Copy key_size bytes to the key buffer. */

        /* Delete the used key slot */
        MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(resultDel, tokenDel, mcuxClCss_KeyDelete_Async(keyIdx));
        if ((tokenDel != MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_KeyDelete_Async)) || (resultDel != MCUXCLCSS_STATUS_OK_WAIT))
        {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(resultWaitDel, tokenWaitDel, mcuxClCss_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR));
        if ((tokenWaitDel != MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_WaitForOperation)) || (resultWaitDel != MCUXCLCSS_STATUS_OK))
        {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }
    }
    else {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    return err;
}

#else /* SW implementation */

#include <string.h>
#include "tfm_plat_otp.h"
#include "mbedtls/hkdf.h"

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

#endif /* ELS */

#endif /* CRYPTO_HW_ACCELERATOR */
