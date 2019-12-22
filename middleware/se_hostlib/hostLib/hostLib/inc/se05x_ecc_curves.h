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

#ifndef SE05X_ECC_CURVES_H_INC
#define SE05X_ECC_CURVES_H_INC

#include "se05x_tlv.h"

#define PROCESS_ECC_CURVE(NAME) \
    smStatus_t Se05x_API_CreateCurve_##NAME(Se05xSession_t *pSession, Se05xPolicy_t *pPolicy, uint32_t obj_id)

#include <se05x_ecc_curves_inc.h>

#undef PROCESS_ECC_CURVE

#endif
