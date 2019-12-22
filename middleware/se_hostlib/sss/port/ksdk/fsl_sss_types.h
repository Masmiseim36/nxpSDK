/*
 * Copyright 2018,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSL_SSS_TYPES_H
#define FSL_SSS_TYPES_H

#include <fsl_common.h>

/*! @brief Compile time sizeof() check */
#define SSCP_BUILD_ASSURE(condition, msg) \
    extern int msg[1 - 2 * (!(condition))] __attribute__((unused))

#ifndef FALSE
#define FALSE false
#endif

#ifndef TRUE
#define TRUE true
#endif

/* snprintf definition for MSVisualC */
#ifndef SNPRINTF
#define SNPRINTF snprintf
#endif /*SNPRINTF */

#endif /* FSL_SSS_TYPES_H */
