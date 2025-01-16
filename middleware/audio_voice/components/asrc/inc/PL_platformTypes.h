/*
 * Copyright 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
@file
Header file for the application layer of the VIT module.
This files includes all definitions, types, structures and function prototypes
required by the calling layer. All other types, structures and functions are
private.
*/

#ifndef PLATFORMTYPES_H_
#define PLATFORMTYPES_H_

#include <stdio.h>

// #define PLATFORM_RT1060_CORTEXM7

#ifdef PLATFORM_RT1060_CORTEXM7
#include "PL_platformTypes_CortexM.h"
#elif defined PLATFORM_CORTEXM7_NOAFE
#include "PL_platformTypes_CortexM.h"
#elif defined PLATFORM_RT1160_CORTEXM7
#include "PL_platformTypes_CortexM.h"
#elif defined PLATFORM_RT1170_CORTEXM7
#include "PL_platformTypes_CortexM.h"
#elif defined PLATFORM_CORTEXM33
#include "PL_platformTypes_CortexM.h"
#elif defined PLATFORM_HIFI4
#include "PL_platformTypes_HIFI4_FUSIONF1.h"
#elif defined PLATFORM_FUSIONF1
#include "PL_platformTypes_HIFI4_FUSIONF1.h"
#elif defined PLATFORM_WINDOWS
#include "PL_platformTypes_windows.h"
#elif defined PLATFORM_LINUX
#include "PL_platformTypes_linux.h"
#elif defined PLATFORM_AARCH64
#include "PL_platformTypes_CortexA.h"
#elif defined PLATFORM_IMX8MINI_CORTEXM4
#include "PL_platformTypes_CortexM.h"
#elif defined PLATFORM_IMX8PLUS_CORTEXM7
#include "PL_platformTypes_CortexM.h"
#endif

#endif /* PLATFORMTYPES_H_ */

/* End of file */
