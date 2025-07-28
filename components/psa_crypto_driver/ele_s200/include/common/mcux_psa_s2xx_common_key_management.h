/*
 * Copyright 2025 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MCUX_PSA_S2XX_COMMON_KEY_MANAGEMENT_H
#define MCUX_PSA_S2XX_COMMON_KEY_MANAGEMENT_H

/** \file mcux_psa_s2xx_common_key_management.h
 *
 * This file contains the declaration of the entry points associated to
 * common S2XX key management procedures.
 *
 */

#include "psa/crypto.h"

#include "fsl_common.h"

#include "fsl_sss_mgmt.h"
#include "fsl_sss_sscp.h"
#include "fsl_sscp_mu.h"

#include "mcux_psa_s2xx_common_init.h"

#ifdef __cplusplus
extern "C" {
#endif

psa_status_t ele_s2xx_import_key(const psa_key_attributes_t *attributes,
                                 const uint8_t *blob, size_t blob_size,
                                 sss_sscp_object_t *sssKey);

psa_status_t ele_s2xx_validate_blob_attributes(const psa_key_attributes_t *attributes,
                                               const uint8_t *psa_import_blob,
                                               size_t psa_import_blob_size);

psa_status_t ele_s2xx_set_key(sss_sscp_object_t *sssKey,
                              uint32_t key_id,
                              const uint8_t *key_buffer,
                              size_t key_buffer_size,
                              sss_key_part_t key_part,
                              sss_cipher_type_t cipher_type,
                              sss_sscp_key_property_t key_properties,
                              size_t allocation_size,
                              size_t key_bitlen);

psa_status_t ele_s2xx_delete_key(sss_sscp_object_t *sssKey);

#ifdef __cplusplus
}
#endif

#endif /* MCUX_PSA_S2XX_COMMON_KEY_MANAGEMENT_H */
