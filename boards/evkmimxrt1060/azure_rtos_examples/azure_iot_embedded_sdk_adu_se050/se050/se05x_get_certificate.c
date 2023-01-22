/*
 * Copyright 2019-2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ex_sss.h"
#include "ex_sss_boot.h"
#include "fsl_sss_se05x_apis.h"

#include "nxLog_App.h"
#include "nxEnsure.h"

sss_status_t se05x_get_certificate(ex_sss_boot_ctx_t *pCtx,
                                   uint32_t certId,
                                   uint8_t *certificate_data,
                                   size_t *certLength)
{
    sss_status_t status = kStatus_SSS_Fail;
	size_t keyBitLen  = *certLength * 8;
    sss_object_t obj;

    status = sss_key_object_init(&obj, &pCtx->ks);
	ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

	status = sss_key_object_get_handle(&obj, certId);
	ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

	status = sss_key_store_get_key(&pCtx->ks, &obj,
                                   certificate_data, certLength,
                                   &keyBitLen);
	ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

	LOG_MAU8_D("\r\nSE05x:Get Certificate ",certificate_data, *certLength);

exit:
	return status;
}
