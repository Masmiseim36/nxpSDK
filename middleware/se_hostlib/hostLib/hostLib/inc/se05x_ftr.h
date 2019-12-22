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

#ifndef SE05X_FTR_H
#define SE05X_FTR_H

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if SSS_HAVE_SE05X

#include <Applet_SE050_Ver.h>

/** @def SE05X_FTR_8BIT_CURVE_ID
 *
 * Curve IDs are 8bit wide. Else, the follow same 32 bit
 * namespace.
 */

#if APPLET_SE050_VER_MAJOR_MINOR > 10002u
#define SE05X_FTR_8BIT_CURVE_ID (1)
#define SE05X_FTR_32BIT_CURVE_ID (0)
#else
#define SE05X_FTR_8BIT_CURVE_ID (0)
#define SE05X_FTR_32BIT_CURVE_ID (1)
#endif /* APPLET_SE050_VER_MAJOR_MINOR > 10002u */

#if APPLET_SE050_VER_MAJOR_MINOR > 10002u
#define SE05X_FTR_USE_INVERT_MGMTIDEXISTS (0)
#else
#define SE05X_FTR_USE_INVERT_MGMTIDEXISTS (1)
#endif /* APPLET_SE050_VER_MAJOR_MINOR > 10002u */

#endif /* SSS_HAVE_SE05X */

#endif /* SE05X_FTR_H */
