/* Copyright 2018 NXP
 * SPDX-License-Identifier: Apache-2.0
 * */

#include "aws_demo.h"
#include "sm_types.h"

#if AX_EMBEDDED
#include "app_boot.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "aws_iot_config.h"
#include "nxLog_App.h"

/*******************************************************************
* MACROS
*******************************************************************/
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define AWS_TASK_PRIORITY (tskIDLE_PRIORITY)
#define AWS_TASK_STACK_SIZE 9000

#define LOGGING_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)
#define LOGGING_TASK_STACK_SIZE (250)
#define LOGGING_QUEUE_LENGTH    (16)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern void awsPubSub_task(void *);

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

/* ************************************************************************** */
/* Include "main()" with the platform specific startup code for Plug & Trust  */
/* MW examples which will call ex_sss_entry()                                 */
/* ************************************************************************** */
#include <ex_sss_main_inc.h>

/*******************************************************************************
 * Code
 ******************************************************************************/

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    pex_sss_demo_tls_ctx->client_keyPair_index = SSS_KEYPAIR_INDEX_CLIENT_PRIVATE;
    pex_sss_demo_tls_ctx->client_cert_index = SSS_CERTIFICATE_INDEX_CLIENT;

    awsPubSub_task((void *)pCtx);

    /* Should not reach this statement */
    for (;;)
        ;
}

#endif //#if AX_EMBEDDED
