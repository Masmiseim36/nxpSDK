/* Copyright 2020-2022,2024-2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <iot_agent_claimcode_inject.h>
#include <nxp_iot_agent_macros.h>
#include <nxp_iot_agent_status.h>
#include <nxp_iot_agent_common.h>

#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)

#include <nxp_iot_agent_macros_sss.h>
#include <se05x_APDU.h>
#include <ex_sss_boot.h>

static uint8_t CheckIfKeyIdExists(sss_object_t* obj, uint32_t keyId, pSe05xSession_t session_ctx)
{

#if (SSS_HAVE_SE05X_VER_03_XX)
    if (sss_key_object_get_handle(obj, keyId) == (sss_status_t)kStatus_SSS_Success)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#else
    AX_UNUSED_ARG(obj);
    smStatus_t retStatus = SM_NOT_OK;
    SE05x_Result_t IdExists = kSE05x_Result_NA;

    retStatus = Se05x_API_CheckObjectExists(session_ctx, keyId, &IdExists);
    if (retStatus == SM_OK)
    {
        if (IdExists == kSE05x_Result_SUCCESS)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else {
        IOT_AGENT_ERROR("Error in Se05x_API_CheckObjectExists\n");
        return 0;
    }
#endif
}

iot_agent_status_t iot_agent_claimcode_inject(ex_sss_boot_ctx_t *pCtx, char *claimcode, size_t claimcode_len)
{
    iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    sss_status_t sss_status;
    sss_object_t obj;

    //Create Policies
    const sss_policy_u default_user = {
        .type = KPolicy_File,
        .auth_obj_id = DEFAULT_USER_ID,
        .policy = {
            .file = {
               .can_Write = 1U
            }
        }
    };
    const sss_policy_u default_user_common = {
        .type = KPolicy_Common,
        .auth_obj_id = DEFAULT_USER_ID,
        .policy = {
            .common = {
                .can_Delete = 1U
            }
        }
    };

    sss_policy_u edgelock2go_user_common = {
        .type = KPolicy_Common,
        .auth_obj_id = EDGELOCK2GO_USER_ID,
        .policy = {
            .common = {
                .can_Delete = 1U,
            }
        }
    };
    sss_policy_u edgelock2go_user = {
        .type = KPolicy_File,
        .auth_obj_id = EDGELOCK2GO_USER_ID,
        .policy = {
            .file = {
                .can_Read = 1U,
                .can_Write = 1U
            }
        }
    };

    sss_policy_t policy_for_claimcode = {
    .nPolicies = 4U,
    .policies = { &default_user_common, &default_user, &edgelock2go_user_common, &edgelock2go_user }
    };

    sss_status = sss_key_object_init(&obj, &pCtx->ks);
    SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_init failed with 0x%04x", sss_status);

    // Update policy with the demo ID only if is present on the secure element
    if (CheckIfKeyIdExists(&obj, EDGELOCK2GO_USER_ID_DEMO, &((sss_se05x_session_t *)(&pCtx->session))->s_ctx))
    {
    	//update policy with correct authentication id
    	edgelock2go_user_common.auth_obj_id = EDGELOCK2GO_USER_ID_DEMO;
    	edgelock2go_user.auth_obj_id = EDGELOCK2GO_USER_ID_DEMO;
    }

    if (CheckIfKeyIdExists(&obj, CLAIMCODE_OBJ_ID, &((sss_se05x_session_t *)(&pCtx->session))->s_ctx))
    {
        obj.keyId = CLAIMCODE_OBJ_ID;
        IOT_AGENT_INFO("INFO: Claimcode exist, deleting..!");
        sss_status = sss_key_store_erase_key(&pCtx->ks, &obj);
        SSS_SUCCESS_OR_EXIT_MSG("sss_key_store_erase_key failed with 0x%04x", sss_status);
    }
    sss_status = sss_key_object_allocate_handle(&obj,
        CLAIMCODE_OBJ_ID,
        kSSS_KeyPart_Default,
        kSSS_CipherType_Binary,
        claimcode_len,
        (uint32_t)kKeyObject_Mode_Persistent);
    SSS_SUCCESS_OR_EXIT_MSG("sss_key_object_allocate_handle failed with 0x%04x", sss_status);

    sss_status = sss_key_store_set_key(&pCtx->ks,
        &obj,
        (uint8_t*)claimcode,
        claimcode_len,
        claimcode_len * 8U,
        &policy_for_claimcode,
        0U);
    SSS_SUCCESS_OR_EXIT_MSG("sss_key_store_set_key failed with 0x%04x", sss_status);
    IOT_AGENT_INFO("Injecting new claim code successful!");
exit:
    return agent_status;
}

#endif //#if defined(NXP_IOT_AGENT_HAVE_SSS) && (NXP_IOT_AGENT_HAVE_SSS == 1)
