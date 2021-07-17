/*
 * Copyright 2007, Crank Software Inc. All Rights Reserved.
 * 
 * For more information email info@cranksoftware.com.
 */

/**
* \file pixel_formats.h
* Defines a common set of pixel defintions common to display, render and memory configurations
*/

#ifndef GR_PIXEL_FORMATS_H
#define GR_PIXEL_FORMATS_H

#if defined(__cplusplus)
extern "C"{
#endif

#define GR_RENDER_FMT_A(fmt)		(((fmt) >> 20) & 0xf)
#define GR_RENDER_FMT_R(fmt)		(((fmt) >> 16) & 0xf)
#define GR_RENDER_FMT_G(fmt)		(((fmt) >> 12) & 0xf)
#define GR_RENDER_FMT_B(fmt)		(((fmt) >> 8)  & 0xf)
#define GR_RENDER_FMT_FTYPE(fmt) 	(((fmt) >> 24) & 0xf)
#define GR_RENDER_FMT_BPP(fmt)		((fmt) & 0xff)
#define GR_RENDER_FMT_BYTESPP(fmt)	((GR_RENDER_FMT_BPP(fmt)+7)/8)

/**
 * Enumerated public pixel format types
 */
enum {
	GR_RENDER_FTYPE_PAL = 1,
	GR_RENDER_FTYPE_MONO,
	GR_RENDER_FTYPE_ALPHA,
	GR_RENDER_FTYPE_BGR,
	GR_RENDER_FTYPE_RGB,
	GR_RENDER_FTYPE_ABGR,
	GR_RENDER_FTYPE_ARGB,
	GR_RENDER_FTYPE_BGRA,
	GR_RENDER_FTYPE_RGBA,
	GR_RENDER_FTYPE_YUV422,
	GR_RENDER_FTYPE_PVRTC_2BPPV1,
	GR_RENDER_FTYPE_PVRTC_4BPPV1,
	GR_RENDER_FTYPE_RGBA8_ETC2_EAC
};

/**
 * Macro to generate pixel format constants
 */
#define GR_RENDER_FMT(order, a, r, g, b, bpp) (order<<24 | a<<20 | r<<16 | g<<12 | b<<8 | bpp)

/**
 * Public render pixel format constants
 */
typedef enum {
	GR_RENDER_FMT_NULL 				= GR_RENDER_FMT(0, 0, 0, 0, 0, 0),
	GR_RENDER_FMT_MONO8				= GR_RENDER_FMT(GR_RENDER_FTYPE_MONO, 0, 1, 1, 1, 8),
	GR_RENDER_FMT_ALPHA 			= GR_RENDER_FMT(GR_RENDER_FTYPE_ALPHA, 8, 0, 0, 0, 8),
	GR_RENDER_FMT_PAL8 				= GR_RENDER_FMT(GR_RENDER_FTYPE_PAL, 8, 8, 8, 8, 8),
	GR_RENDER_FMT_RGB565 			= GR_RENDER_FMT(GR_RENDER_FTYPE_RGB, 0, 5, 6, 5, 16),
	GR_RENDER_FMT_BGR565 			= GR_RENDER_FMT(GR_RENDER_FTYPE_BGR, 0, 5, 6, 5, 16),
	GR_RENDER_FMT_ARGB1555 			= GR_RENDER_FMT(GR_RENDER_FTYPE_ARGB, 1, 5, 5, 5, 16),
	GR_RENDER_FMT_BGRA1555 			= GR_RENDER_FMT(GR_RENDER_FTYPE_ABGR, 1, 5, 5, 5, 16),
	GR_RENDER_FMT_RGB888 			= GR_RENDER_FMT(GR_RENDER_FTYPE_RGB, 0, 8, 8, 8, 24),
	GR_RENDER_FMT_BGR888 			= GR_RENDER_FMT(GR_RENDER_FTYPE_BGR, 0, 8, 8, 8, 24),
	GR_RENDER_FMT_ARGB8888 			= GR_RENDER_FMT(GR_RENDER_FTYPE_ARGB, 8, 8, 8, 8, 32),
	GR_RENDER_FMT_BGRA8888 			= GR_RENDER_FMT(GR_RENDER_FTYPE_BGRA, 8, 8, 8, 8, 32),
	GR_RENDER_FMT_ABGR8888 			= GR_RENDER_FMT(GR_RENDER_FTYPE_ABGR, 8, 8, 8, 8, 32),
	GR_RENDER_FMT_YUV422 			= GR_RENDER_FMT(GR_RENDER_FTYPE_YUV422, 0, 5, 6, 5, 16),
	GR_RENDER_FMT_PVRTC_2BPPV1 		= GR_RENDER_FMT(GR_RENDER_FTYPE_PVRTC_2BPPV1, 0, 5, 6, 5, 16),
	GR_RENDER_FMT_PVRTC_4BPPV1 		= GR_RENDER_FMT(GR_RENDER_FTYPE_PVRTC_4BPPV1, 8, 8, 8, 8, 32),
	GR_RENDER_FMT_RGBA8_ETC2_EAC 	= GR_RENDER_FMT(GR_RENDER_FTYPE_RGBA8_ETC2_EAC, 8, 8, 8, 8, 32)
} gr_render_format_t;

#if defined(__cplusplus)
}
#endif

#endif /* GR_PIXEL_FORMATS_H */
