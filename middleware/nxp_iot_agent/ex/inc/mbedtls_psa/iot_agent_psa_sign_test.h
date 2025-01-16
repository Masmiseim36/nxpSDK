/* 
 * Copyright 2022,2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */
#include <nxp_iot_agent.h>

#define PSA_KEY_LOCATION_S50_TEMP_STORAGE             ((psa_key_location_t)(PSA_KEY_LOCATION_VENDOR_FLAG | 0x02U))
#define PSA_EL2GO_KEK_LIFETIME PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_LIFETIME_PERSISTENT, PSA_KEY_LOCATION_S50_TEMP_STORAGE)

#define IMPORT_OBJ_ID    		0x00004000U
#define IMPORT_OBJ_ID_TMP    		0x00004001U
#define OBJ_ID    		        0x00008000U
#define PSA_KEY_LOCATION_S50_BLOB_STORAGE             ((psa_key_location_t)(PSA_KEY_LOCATION_VENDOR_FLAG | 0x01U))
#define PSA_EL2GO_BLOB_LIFETIME PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_LIFETIME_PERSISTENT, PSA_KEY_LOCATION_S50_BLOB_STORAGE)

#define PSA_KEY_LOCATION_S50_ENC_STORAGE             ((psa_key_location_t)(PSA_KEY_LOCATION_VENDOR_FLAG | 0x00U))
#define PSA_EL2GO_BLOB_ENC_LIFETIME PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(PSA_KEY_LIFETIME_PERSISTENT, PSA_KEY_LOCATION_S50_ENC_STORAGE)

/**
 * @brief Signs dummy data with imported key. Import of key blob is done by agent,
 *        after establishinkg TLS connection to backend. Key is used to sign dummy
 *        data in order to verify provisioning process. Note that verification of
 *        the signature will not be part of the agent demo application.
 * @return Success on signature creation
 */
iot_agent_status_t iot_agent_verify_psa_import(void);

/**
 * @brief Exports the certificate. Import of binary data blob is done by agent,
 *        after establishinkg TLS connection to backend.
 * @return Success on signature creation
 */
iot_agent_status_t iot_agent_verify_psa_export(void);