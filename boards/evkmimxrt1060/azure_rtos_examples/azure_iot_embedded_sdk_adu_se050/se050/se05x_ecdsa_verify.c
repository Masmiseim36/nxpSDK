/*
 * Copyright 2019-2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ex_sss.h"
#include "ex_sss_boot.h"
#include "fsl_sss_se05x_apis.h"

#include "nxLog_App.h"
#include "nxEnsure.h"

sss_status_t se05x_ecdsa_verify(ex_sss_boot_ctx_t *pCtx,
                                uint32_t privateKeyId,
							    uint8_t *inputDigest, size_t inputDigestLength,
							    uint8_t *signature, size_t signatureLen)
{
	sss_asymmetric_t ctx_asymm  = {0};
	sss_status_t status = kStatus_SSS_Fail;
    sss_object_t obj;

    status = sss_key_object_init(&obj, &pCtx->ks);
	ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

	status = sss_key_object_get_handle(&obj, privateKeyId);
	ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

    status = sss_asymmetric_context_init(&ctx_asymm, &pCtx->session, &obj,
                                         kAlgorithm_SSS_SHA256, kMode_SSS_Verify);
    ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

    status = sss_asymmetric_verify_digest(&ctx_asymm,
                                          inputDigest, inputDigestLength,
                                          signature, signatureLen);
    ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

exit:
	sss_asymmetric_context_free(&ctx_asymm);
	return status;
}
