/* Copyright 2023-2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nxp_iot_agent.h"
#include "nxp_iot_agent_status.h"
#include "iot_agent_claimcode_import.h"

#if defined(NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM) && (NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM == 1)
#include "nxp_iot_agent_macros.h"
#include <nxp_iot_agent_macros_psa.h>
#include "nxp_iot_agent_utils.h"

#include "psa/crypto.h"
#include "psa_crypto_its.h"

#define FLASH_ADDRESS           (0x08000000U)
#define CLAIM_CODE_INFO_ADDRESS (FLASH_ADDRESS + 0x004a0000U)

// An indicator to be able to make a fast and easy decision whether there is
// a claimcode blob at a particular address in Flash
static const uint8_t claimcode_blob_indicator[4] = {'E', '2', 'G', 'C'};
typedef struct _claimcode_info
{
    uint8_t indicator[4];
    uint32_t size;
    uint8_t blob[];
} claimcode_info_t;

iot_agent_status_t iot_agent_claimcode_import(void)
{
    iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    psa_status_t psa_status         = PSA_SUCCESS;

    claimcode_info_t *cc_info = (claimcode_info_t *)CLAIM_CODE_INFO_ADDRESS;

    // check if there is a valid calimcode to import
    if (memcmp(cc_info->indicator, &claimcode_blob_indicator, sizeof(claimcode_blob_indicator)) != 0)
    {
        IOT_AGENT_INFO("No claimcode for importing was found.");
        return IOT_AGENT_SUCCESS;
    }

    // Check if object exists.
    struct psa_storage_info_t storage_info = {0};
    psa_status                             = psa_its_get_info(CLAIMCODE_OBJ_ID, &storage_info);
    if (psa_status == PSA_SUCCESS)
    {
        IOT_AGENT_INFO("Claimcode with object id 0x%08x is already imported, skipping...", CLAIMCODE_OBJ_ID);
        return IOT_AGENT_SUCCESS;
    }

    psa_status = psa_its_set(CLAIMCODE_OBJ_ID, cc_info->size, cc_info->blob, PSA_STORAGE_FLAG_NONE);
    PSA_SUCCESS_OR_EXIT_MSG("import of claimcode failed: 0x%08x", psa_status);
    IOT_AGENT_INFO("Claimcode imported into psa its with object id 0x%08x", CLAIMCODE_OBJ_ID);

exit:
    return agent_status;
}

#endif //#if defined(NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM) && (NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM == 1)
