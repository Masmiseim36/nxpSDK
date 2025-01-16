/*
 * Copyright 2021 by Retune DSP
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef RDSP_TYPES_H_
#define RDSP_TYPES_H_

#include "RdspPlatforms.h"

#ifdef __cplusplus
extern "C" {
#endif

//	rdsp types				RDSP_USES_IEEE754_CODE
//-------------------------------------------------
// rdsp_float				float
// rdsp_dB:					float
// rdsp_floatx2:			float[2]
// rdsp_complex:			float[2]
// rdsp_double:				double
//-------------------------------------------------


#ifdef HIFI4

typedef xtfloat rdsp_float;
typedef xtfloat rdsp_dB;
typedef xtfloatx2 rdsp_floatx2;
typedef xtfloatx2 rdsp_complex;
typedef xtfloatx2 rdsp_double;

#elif defined(FUSIONDSP)

typedef xtfloat rdsp_float;
typedef xtfloat rdsp_dB;
typedef xtfloatx2 rdsp_floatx2;
typedef xtfloatx2 rdsp_complex;
typedef xtfloatx2 rdsp_double;

#else

typedef float rdsp_float;
typedef float rdsp_dB;
typedef float rdsp_floatx2[2];
typedef float rdsp_complex[2];
typedef double rdsp_double;

#endif

typedef rdsp_float rdsp_xyz_t[3];

#ifdef __cplusplus
}
#endif

#endif // RDSP_TYPES_H_
