/* Copyright 2018 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
*/

#include "azureDemo.h"

#if AX_EMBEDDED
#include "app_boot.h"
#include "FreeRTOS.h"
#include "azureDemo.h"
#include "ksdk_mbedtls.h"
#include "task.h"

#include "ax_api.h"
#if SSS_HAVE_ALT_A71CH
#include "ax_mbedtls.h"
#include <fsl_sscp_a71ch.h>
#endif
#if SSS_HAVE_ALT_SSS
#include "sss_mbedtls.h"
#include "ex_sss.h"
#endif

#include <fsl_sss_api.h>

#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH
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
ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx = &gex_sss_demo_tls_ctx;;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_demo_boot_ctx)
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0
#define EX_SSS_BOOT_DO_ERASE 0

#include <ex_sss_main_inc.h>

/*******************************************************************
* FUNCTIONS
*******************************************************************/

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{

    azure_task_lwip((void*)pCtx);

    for (;;)
        ;
}
#endif
