/* Copyright 2018,2020 NXP
* SPDX-License-Identifier: Apache-2.0
*/

#include "azureDemo.h"

#if AX_EMBEDDED
#include "app_boot.h"
#include "FreeRTOS.h"
#include "azureDemo.h"
#include "ksdk_mbedtls.h"
#include "task.h"

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
#include "azure_iot_config.h"
#include "mbedtls/platform.h"
#include "sm_demo_utils.h"
#include <fsl_sss_sscp.h>
#include <fsl_sss_api.h>
#include "nxLog_App.h"

/*******************************************************************
* MACROS
*******************************************************************/

#define AZURE_TASK_PRIORITY (tskIDLE_PRIORITY)
#define AZURE_TASK_STACK_SIZE 9000

#define LOGGING_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)
#define LOGGING_TASK_STACK_SIZE (250)
#define LOGGING_QUEUE_LENGTH    (16)

extern const unsigned char privKey[];
extern const size_t lenPrivKey;

void azure_task_lwip(void *);
void BOARD_InitNetwork(const unsigned char ax_uid[18]);

#include <nxLog_App.h>
#include <ex_sss.h>
#include <ex_sss_boot.h>

static ex_sss_boot_ctx_t gex_sss_demo_boot_ctx;
ex_sss_boot_ctx_t *pex_sss_demo_boot_ctx = &gex_sss_demo_boot_ctx;

static ex_sss_cloud_ctx_t gex_sss_demo_tls_ctx;
ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx = &gex_sss_demo_tls_ctx;
;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_demo_boot_ctx)
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0
#define EX_SSS_BOOT_DO_ERASE 0

/* ************************************************************************** */
/* Include "main()" with the platform specific startup code for Plug & Trust  */
/* MW examples which will call ex_sss_entry()                                 */
/* ************************************************************************** */
#include <ex_sss_main_inc.h>

/*******************************************************************
* FUNCTIONS
*******************************************************************/

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    pex_sss_demo_tls_ctx->client_keyPair_index = AZURE_IOT_KEY_INDEX_SM;
    pex_sss_demo_tls_ctx->client_cert_index = AZURE_IOT_CLIENT_CERT_INDEX_SM;

    azure_task_lwip((void *)pCtx);

    for (;;)
        ;
}
#endif
