/* Copyright 2019 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <ex_sss_boot.h>
#include <fsl_sss_se05x_apis.h>
#include <nxLog_App.h>
#include <se05x_APDU.h>
#include <se05x_const.h>
#include <se05x_ecc_curves.h>
#include <se05x_ecc_curves_values.h>
#include <se05x_tlv.h>

#ifdef __cplusplus
}
#endif

#ifndef SIMW_DEMO_ENABLE__DEMO_SE05X_MINIMAL
#include "UWBIOT_APP_BUILD.h"
#endif

#if defined(SIMW_DEMO_ENABLE__DEMO_SE05X_MINIMAL)

static ex_sss_boot_ctx_t gex_sss_boot_ctx;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_boot_ctx)
#define EX_SSS_BOOT_DO_ERASE 0
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

/* ************************************************************************** */
/* Include "main()" with the platform specific startup code for Plug & Trust  */
/* MW examples which will call ex_sss_entry()                                 */
/* ************************************************************************** */
#include <ex_sss_main_inc.h>

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t status = kStatus_SSS_Success;

    sss_se05x_session_t *pSession = (sss_se05x_session_t *)&pCtx->session;

    uint16_t mem = 0;
    smStatus_t sm_status;
    sm_status = Se05x_API_GetFreeMemory(&pSession->s_ctx, kSE05x_MemoryType_PERSISTENT, &mem);
    if (sm_status != SM_OK) {
        LOG_E("Se05x_API_GetFreeMemory Failed");
        status = kStatus_SSS_Fail;
    }
    LOG_U16_I(mem);

    if (mem >= 0x7FFF) {
        LOG_W("If 32768 bytes or more bytes are available, 32767 bytes free is reported.");
    }

    if (kStatus_SSS_Success == status) {
        LOG_I("se05x_Minimal Example Success !!!...");
    }
    else {
        LOG_E("se05x_Minimal Example Failed !!!...");
    }
    return status;
}

#endif // SIMW_DEMO_ENABLE__DEMO_SE05X_MINIMAL
