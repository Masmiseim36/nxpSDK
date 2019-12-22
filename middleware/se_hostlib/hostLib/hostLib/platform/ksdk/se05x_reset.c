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

#include "se05x_apis.h"
#include "ax_reset.h"
#include "se_reset_config.h"
#include <stdio.h>

#include "fsl_gpio.h"
#include "sm_timer.h"
#include "sm_types.h"
#include "smComT1oI2C.h"
#include "nxLog_smCom.h"

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if SSS_HAVE_SE05X || SSS_HAVE_LOOPBACK

void se05x_ic_reset()
{
    axReset_ResetPluseDUT();
    smComT1oI2C_ComReset();
    sm_usleep(3000);
    return;
}

#endif
