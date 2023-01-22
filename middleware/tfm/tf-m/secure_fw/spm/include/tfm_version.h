/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_VERSION_H__
#define __TFM_VERSION_H__

/*
 * Defines for TFM version.
 */
#define TFM_VERSION        1.6.0

#define VERSION_STRING     ""

#define VERSTR(x)          #x
#define VERCON(x)          VERSTR(x)

#define VERSION_FULLSTR    VERCON(TFM_VERSION)""VERSION_STRING

#endif /* __TFM_VERSION_H__ */
