/**
* @file gcpJwtTask_lwip.c
* @author NXP Semiconductors
* @version 1.0
* @par License
*
* Copyright 2017,2018,2020 NXP
* SPDX-License-Identifier: Apache-2.0
*
* @par Description
* gcp jwt task for lwip. From PC Windows testing
*/

#include "gcpJWTDemo.h"
#include "gcp_iot_config.h"
#include "gcp_client_credential_keys.h"
#include "ledHandler.h"

#if SSS_HAVE_MBEDTLS_ALT_A71CH
#include "ax_api.h"
#include "gcpJWTDemo.h"

#include "mbedtls/platform.h"
#include "sm_printf.h"
#endif

#include <stdio.h>
#include <a71ch_api.h>
#include "nxLog_App.h"

#if SSS_HAVE_MBEDTLS_ALT_SSS
#include "ax_api.h"
#include "sss_mbedtls.h"
#include "ex_sss.h"
#include "sm_printf.h"
#endif

#if SSS_HAVE_MBEDTLS_ALT_A71CH
#include "ax_mbedtls.h"
#include "sm_printf.h"
#endif

#if SSS_HAVE_MBEDTLS_ALT_SSS
#include <fsl_sss_api.h>
#include <fsl_sss_sscp.h>
sss_sscp_session_t *sscp_session;
sscp_context_t sscp;
#endif

#include <ex_sss_boot.h>
#include <nxEnsure.h>

extern ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx;
void gcp_jwt_task_lwip(void *ctx)
{
    ex_sss_boot_ctx_t *pCtx = (ex_sss_boot_ctx_t *)ctx;
    mbedtls_pk_context pk;
    int ret;

#if (SSS_HAVE_MBEDTLS_ALT)

    sss_status_t status;
    LOG_I("GCP JWT NXP Secure Element example\n\n");

    /*keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM Not used variable to avoid warning set it to NULL*/
    keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = NULL;

    /* For creating the GCP token */
    mbedtls_pk_init(&pk);

    status = sss_key_object_init(&(pex_sss_demo_tls_ctx->obj), &(pCtx->ks));
    if (status != kStatus_SSS_Success) {
        printf(" sss_key_object_init for keyPair Failed...\n");
        return;
    }

    status = sss_key_object_get_handle(&(pex_sss_demo_tls_ctx->obj), SSS_KEYPAIR_INDEX_CLIENT_PRIVATE);
    if (status != kStatus_SSS_Success) {
        printf(" sss_key_object_get_handle  for keyPair Failed...\n");
        return;
    }

    ret = sss_mbedtls_associate_keypair(&pk, &(pex_sss_demo_tls_ctx->obj));

    if (ret != 0) {
        PRINTF(" failed\n  ! ax_mbedtls_associate_keypair returned %d\n\n", ret);
        goto exit;
    }

#endif //SSS_HAVE_MBEDTLS_ALT

    gcpPubSub((void *)&pk);

exit:
    while (1) {
#if AX_EMBEDDED
        __disable_irq();
        __WFI(); /* Never exit */
#else
        LOG_I("HALT!");
        getchar();
#endif
    }
}
