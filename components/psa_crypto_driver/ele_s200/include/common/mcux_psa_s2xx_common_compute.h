/*
 * Copyright 2025 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MCUX_PSA_S2XX_COMMON_COMPUTE_H
#define MCUX_PSA_S2XX_COMMON_COMPUTE_H

/** \file mcux_psa_s2xx_common_compute.h
 *
 * This file contains the implementation of the entry points for the
 * driver-specific functions for crypto operations
 *
 */

#include "psa/crypto.h"
#include "mcux_psa_s2xx_common_init.h"

#ifdef __cplusplus
extern "C" {
#endif

/*  AEAD  */

psa_status_t ele_s2xx_common_aead(const uint8_t *nonce, size_t nonce_length,
                                  const uint8_t *additional_data, size_t additional_data_length,
                                  const uint8_t *input, size_t input_size,
                                  uint8_t *output,
                                  uint8_t *tag, size_t *tag_length,
                                  sss_mode_t mode, sss_sscp_object_t *sssKey, sss_algorithm_t ele_alg);

/*  MAC  */

psa_status_t ele_s2xx_common_mac(const uint8_t *input, size_t input_length,
                                 uint8_t *mac, size_t mac_size, size_t *mac_length,
                                 sss_sscp_object_t *sssKey, sss_algorithm_t ele_alg);

/*  CIPHER  */

psa_status_t ele_s2xx_common_cipher(sss_sscp_object_t *sssKey,
                                    const uint8_t *iv, size_t iv_length,
                                    const uint8_t *input, uint8_t *output,
                                    size_t input_length,
                                    sss_algorithm_t ele_algo, sss_mode_t mode);

/*  ASYMMETRIC  */

psa_status_t ele_s2xx_common_sign_digest(uint8_t *digest, size_t digest_len,
                                         uint8_t *signature, size_t *signature_len,
                                         sss_sscp_object_t *sssKey, sss_algorithm_t ele_alg);

psa_status_t ele_s2xx_common_verify_digest(uint8_t *digest, size_t digest_len,
                                           uint8_t *signature, size_t signature_len,
                                           sss_sscp_object_t *sssKey, sss_algorithm_t ele_alg);

/*  KEY AGREEMENT  */

psa_status_t ele_s2xx_common_key_agreement(sss_sscp_object_t *sssKey,
                                           sss_sscp_object_t *sssKey_peer,
                                           sss_sscp_object_t *sssKey_shared,
                                           uint8_t *shared_secret,
                                           size_t shared_secret_size,
                                           size_t *shared_secret_length);

#ifdef __cplusplus
}
#endif
#endif /* MCUX_PSA_S2XX_COMMON_COMPUTE_H */
