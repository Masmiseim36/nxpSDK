/**
 * @file gcpJWTDemo_Main_lwip.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 *
 * Copyright 2017,2018,2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 *
 * @par Description
 * Demo to connect to GCP (Google Cloud Platform) over internet on embedded platform
 */

#include "gcpJWTDemo.h"

#if AX_EMBEDDED
#include "app_boot.h"
#include "board.h"
#include "FreeRTOS.h"
#include "gcpJWTDemo.h"
#include "ksdk_mbedtls.h"
#include "task.h"
#include "iot_logging_task.h"

#include "ax_api.h"
#if SSS_HAVE_MBEDTLS_ALT_A71CH
#include "ax_mbedtls.h"
#include <fsl_sscp_a71ch.h>
#endif
#if SSS_HAVE_MBEDTLS_ALT_SSS
#include "sss_mbedtls.h"
#include "ex_sss.h"
#endif

#include <fsl_sss_api.h>

#if SSS_HAVE_APPLET_A71CH || SSS_HAVE_APPLET_A71CH_SIM
#endif
#include "gcp_iot_config.h"
#include "mbedtls/platform.h"
#include "sm_demo_utils.h"
#include "ledHandler.h"
#include "nxLog_App.h"

#include <fsl_sss_sscp.h>
#include <fsl_sss_api.h>

/*******************************************************************
* MACROS
*******************************************************************/
#define GCP_TASK_PRIORITY (tskIDLE_PRIORITY)
#define GCP_TASK_STACK_SIZE 11000

#define LOGGING_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)
#define LOGGING_TASK_STACK_SIZE (250)
#define LOGGING_QUEUE_LENGTH    (16)

extern const unsigned char privKey[];
extern const size_t lenPrivKey;
#if USE_SERGER_RTT
extern void nInit_segger_Log(void);
#endif

void gcp_jwt_task(void *);

#include <nxLog_App.h>
#include <ex_sss.h>
#include <ex_sss_boot.h>

static ex_sss_boot_ctx_t gex_sss_demo_boot_ctx;
ex_sss_boot_ctx_t *pex_sss_demo_boot_ctx = &gex_sss_demo_boot_ctx;

static ex_sss_cloud_ctx_t gex_sss_demo_tls_ctx;
ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx = &gex_sss_demo_tls_ctx;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_demo_boot_ctx)
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0
#define EX_SSS_BOOT_DO_ERASE 0

#include <ex_sss_main_inc.h>

/*******************************************************************
* FUNCTIONS
*******************************************************************/

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    pex_sss_demo_tls_ctx->client_keyPair_index = SSS_KEYPAIR_INDEX_CLIENT_PRIVATE;
    pex_sss_demo_tls_ctx->client_cert_index = SSS_CERTIFICATE_INDEX;

    gcp_jwt_task((void *)pCtx);

    /* Should not reach this statement */
    for (;;)
        ;
}

void gcp_jwt_task(void *ctx)
{
    sss_status_t status;
    mbedtls_pk_context pk;
    int ret = 0;
    // U8 AxUID[A71CH_MODULE_UNIQUE_ID_LEN];
    ex_sss_boot_ctx_t *pCtx = (ex_sss_boot_ctx_t *)ctx;

    LOG_I("GCP JWT NXP Secure Element example");

    mbedtls_pk_init(&pk);

#if SSS_HAVE_SSS
    status = sss_key_object_init(&pex_sss_demo_tls_ctx->obj, &pCtx->ks);
    if (status != kStatus_SSS_Success) {
        LOG_E(" sss_key_object_init for keyPair Failed...\n");
        return;
    }

    status = sss_key_object_get_handle(&pex_sss_demo_tls_ctx->obj, SSS_KEYPAIR_INDEX_CLIENT_PRIVATE);
    if (status != kStatus_SSS_Success) {
        LOG_E(" sss_key_object_get_handle  for keyPair Failed...\n");
        return;
    }
#if SSS_HAVE_MBEDTLS_ALT_SSS
    ret = sss_mbedtls_associate_keypair(&pk, &pex_sss_demo_tls_ctx->obj);
#elif SSS_HAVE_MBEDTLS_ALT_A71CH
    ret = ax_mbedtls_associate_keypair(0, &pk);
#endif
    if (ret != 0) {
        LOG_E(" failed\n  ! ax_mbedtls_associate_keypair returned %d\n\n", ret);
        goto exit;
    }
#endif /*SSS_HAVE_SSS*/

    LED_RED_OFF();
    LED_GREEN_OFF();
    LED_BLUE_ON();

    LED_BLUE_OFF();
    gcpPubSub((void *)&pk);

exit:
    while (1) {
        __disable_irq();
        __WFI(); /* Never exit */
    }
}

#endif //#if AX_EMBEDDED
