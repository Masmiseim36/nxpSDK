/* Copyright 2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
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

static ex_sss_boot_ctx_t gex_sss_boot_ctx;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_boot_ctx)
#define EX_SSS_BOOT_DO_ERASE 0
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

#include <ex_sss_main_inc.h>

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t status = kStatus_SSS_Success;

    sss_se05x_session_t *pSession = (sss_se05x_session_t *)&pCtx->session;

    uint16_t mem = 0;
    smStatus_t sm_status;
    sm_status = Se05x_API_GetFreeMemory(
        &pSession->s_ctx, kSE05x_MemoryType_TRANSIENT_DESELECT, &mem);
    if (sm_status != SM_OK) {
        LOG_E("Se05x_API_GetFreeMemory Failed");
        status = kStatus_SSS_Fail;
    }
    LOG_U16_I(mem);

    return status;
}
