/**
* @file gcpJwtTask_lwip.c
* @author NXP Semiconductors
* @version 1.0
* @par License
* Copyright 2017,2018 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
*
* @par Description
* gcp jwt task for lwip
*/

#include "gcpJWTDemo.h"
#include "gcp_iot_config.h"
#include "gcp_client_credential_keys.h"
#include "ledHandler.h"

#if SSS_HAVE_ALT_A71CH
#include "ax_api.h"
#include "gcpJWTDemo.h"

#include "mbedtls/platform.h"
#include "sm_printf.h"
#endif

#include <stdio.h>
#include <a71ch_api.h>
#include "nxLog_App.h"

#if SSS_HAVE_ALT_SSS
#include "ax_api.h"
#include "sss_mbedtls.h"
#include "ex_sss.h"
#include "sm_printf.h"
#endif

#if SSS_HAVE_ALT_A71CH
#include "ax_mbedtls.h"
#include "sm_printf.h"
#endif

#if AX_EMBEDDED
#include <FreeRTOS.h>
#include "board.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "ksdk_mbedtls.h"
#include "pin_mux.h"
#include "task.h"
#ifdef IMX_RT
#include "fsl_dcp.h"
#include "fsl_iomuxc.h"
#include "fsl_trng.h"
#include "pin_mux.h"
#endif

#ifdef CPU_LPC54018
#include "fsl_power.h"
#endif

#endif

#if SSS_HAVE_ALT_SSS
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
	ex_sss_boot_ctx_t *pCtx = (ex_sss_boot_ctx_t*)ctx;
    mbedtls_pk_context pk;
    int ret;

#if ((SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH) && AX_EMBEDDED)
    U8 AxUID[A71CH_MODULE_UNIQUE_ID_LEN];
#endif

#if SSS_HAVE_ALT_SSS || SSS_HAVE_ALT_A71CH

    sss_status_t status;

#if (SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH)
    uint16_t connectStatus;
    SmCommState_t commState;
    U8 Atr[64];
    U16 AtrLen = sizeof(Atr);

    U16 AxUIDLen = A71CH_MODULE_UNIQUE_ID_LEN;

    /*keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM Not used variable to avoid warning set it to NULL*/
    keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = NULL;

    LOG_I("GCP JWT NXP Secure Element example\n\n");

#if AX_EMBEDDED
    connectStatus = SM_Connect(&commState, Atr, &AtrLen);
    ENSURE_OR_GO_EXIT(connectStatus == SW_OK);
#else
    //connectStatus = SM_RjctConnect("COM15", &commState, Atr, &AtrLen);
#endif
#endif

#if (SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH)
    A71_GetUniqueID(AxUID, &AxUIDLen);
#endif

    /*keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM Not used variable to avoid warning set it to NULL*/
    keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = NULL;

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

#if AX_EMBEDDED
    LED_RED_OFF();
    LED_GREEN_OFF();
    LED_BLUE_ON();

    LED_BLUE_OFF();
#endif
#endif

    gcpPubSub((void *) &pk);

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
