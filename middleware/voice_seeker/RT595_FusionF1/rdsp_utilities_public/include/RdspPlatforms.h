/*
Copyright 2021 by Retune DSP
Copyright 2022 NXP

NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in accordance with the applicable license terms.  By expressly accepting such terms or by downloading, installing, activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef RDSP_PLATFORMS_H
#define RDSP_PLATFORMS_H

#include <stdint.h>
#include <stddef.h>

#ifdef NXP_RT500_CM33
#define RDSP_MATH_USES_POWERQUAD 1
#include "fsl_powerquad.h"
#endif

#if defined(FUSIONDSP)
#include <xtensa/tie/xt_fusion.h>
#endif
#if defined(HIFI3)
#include <xtensa/tie/xt_hifi3.h>
#endif
#if defined(HIFI4)
#include <xtensa/tie/xt_hifi4.h>
#endif

/* NatureDSP lib */
#if defined(HIFI3) || defined(HIFI4) || defined(FUSIONDSP)
#include "NatureDSP_Signal.h"
#include "NatureDSP_types.h"
#endif

#ifdef __ARM_NEON
#include "arm_neon.h"
#endif

#ifdef _WIN32
#define RDSP_PRAGMA_USED 
#else
#define RDSP_PRAGMA_USED __attribute__((used))
#endif

#ifdef __ARM_NEON
#define RDSP_PACKED __attribute__((__packed__))
#else
#define RDSP_PACKED
#endif

#endif // RDSP_PLATFORMS_H
