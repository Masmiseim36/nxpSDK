/* Copyright 2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 * */

#include "aws_demo.h"
#include "sm_types.h"

#if AX_EMBEDDED
#include "app_boot.h"
#include "app.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ksdk_mbedtls.h"
#include "ax_api.h"
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
ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx = &gex_sss_demo_tls_ctx;;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_demo_boot_ctx)
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0
#define EX_SSS_BOOT_DO_ERASE 0

#include <ex_sss_main_inc.h>

/*******************************************************************************
 * Code
 ******************************************************************************/

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    awsPubSub_task((void*)pCtx);

    /* Should not reach this statement */
    for (;;)
        ;
}

#endif //#if AX_EMBEDDED
