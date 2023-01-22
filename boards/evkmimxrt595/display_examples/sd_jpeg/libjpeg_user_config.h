/*
 * Copyright 2019,2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _LIBJPEG_USER_CONFIG_H_
#define _LIBJPEG_USER_CONFIG_H_

#include "display_support.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LIBJPEG_USER_CONFIG_PIXEL_FORMAT 1

#if ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || \
     (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || \
     (DEMO_PANEL_RK055MHD091 == DEMO_PANEL))

/* The LCDIF does not support RGB888. So use XRGB8888 here. */
#define RGB_RED       2 /* Offset of Red in an RGB scanline element */
#define RGB_GREEN     1 /* Offset of Green */
#define RGB_BLUE      0 /* Offset of Blue */
#define RGB_PIXELSIZE 4 /* JSAMPLEs per RGB scanline element */

#else

#define RGB_RED       2 /* Offset of Red in an RGB scanline element */
#define RGB_GREEN     1 /* Offset of Green */
#define RGB_BLUE      0 /* Offset of Blue */
#define RGB_PIXELSIZE 3 /* JSAMPLEs per RGB scanline element */

#endif

#endif /* _LIBJPEG_USER_CONFIG_H_ */
