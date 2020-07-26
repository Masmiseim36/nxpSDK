/*
 * Copyright 2007, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

/**
* \file generic_display.h
*
*  This file describes the interface used to intialize
*  and control generic displays with the 'GENERIC' SW renderer
*/

#ifndef GR_GENERIC_DISPLAY_H
#define GR_GENERIC_DISPLAY_H

#include <gre/sdk/pixel_formats.h>
#include <gre/sdk/render_common.h>

/**
 * Describes several attributes for a framebuffer.
 */
typedef struct {
	uint16_t           				width;          ///< Width of the display in pixels.
	uint16_t           				height;         ///< Height of the display in pixels.
	gr_render_format_t 				render_format;  ///< Rendering format for the framebuffer. (render_format is 4 byte aligned)
	uint32_t						flags;			///< For layer options
	uint16_t           				stride;         ///< Number of bytes per row of pixels. (0 means implied)
	uint8_t							num_buffers;	///< Number of buffers the layer has
	uint8_t							buffer_draw_index;	///< Index of buffer with most recent content
	gr_render_rect_t				dirty_rect;		///< Rect to dirty, if applicable
	void * 							buffer[2];		///< Array of addresses for the layer. Need a minimum of 1
} gr_generic_display_layer_info_t;

/**
 * Used for customizing the initialization of hardware
 * that uses a custom BSP image.
 */
typedef struct {
	uint16_t							flags;		///< Flags
	uint8_t								reserved;	///< reserved
	uint8_t								num_layers;	///< Size of the layer info structure
	gr_generic_display_layer_info_t *	layer_info;	///< Per layer information.  Need a minimum of 1
} gr_generic_display_info_t;


/**
 * Function that would be implemented in BSP code
 * that would provide any necessary configuration information
 * regarding framebuffers to be used for rendering.
 *
 * @param[out] info A pointer to a gr_generic_display_info_t instance that all
 *                  necessary configuration information will be provided to.
 *
 * @retval 0 upon success
 * @retval 1 if an error occurred while setting display information.
 */
int gr_generic_display_init(gr_generic_display_info_t *info);

/**
 * Function that would be implemented in BSP code
 * that handles any framebuffer changes.
 *
 * @param[in] info Information about a framebuffer update. May be used
 *                 to perform platform-specific behavior related to displays.
 *
 * @retval 0 upon success.
 * @retval 1 if an error occurred while handling the display update.
 */
int gr_generic_display_update(const gr_generic_display_info_t *info);

#endif /* GR_GENERIC_DISPLAY_H */
