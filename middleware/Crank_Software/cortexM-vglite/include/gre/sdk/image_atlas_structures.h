/*
 * Copyright 2007, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

/**
* \file image_atlas_structures.h
*
*/

#ifndef GR_IMAGE_ATLAS_STRUCTURES_H
#define GR_IMAGE_ATLAS_STRUCTURES_H

#include <gre/sdk/pixel_formats.h>

typedef void* atlas_pool_t;			///< The opaque pool handle definition
typedef void* atlas_block_t;		///< The opaque block handle definition
typedef void* atlas_entry_t;		///< The opaque entry handle definition

typedef struct _gr_atlas_entry {
	gr_render_format_t format;		///< the render format that the data is in
	void *user_data; 				///< the user data attached to this entry
	void *data;						///< the image data
	int flags;						///< the flags applying to the entry
	int read_ops;					///< the pending read ops on entry
	char *key;						///< the entry key
	atlas_pool_t pool_handle;		///< the pool handle to which this entry belongs
	atlas_block_t block_handle;		///< the block handle to which this entry belongs
	atlas_entry_t entry_handle;		///< the entry handle
} gr_atlas_entry_t;

typedef struct _gr_atlas_entry_info {
	int16_t x;						///< the x offset into the image atlas that this atlas entry is located
	int16_t y;						///< the y offset into the image atlas that this atlas entry is located
	int16_t w;						///< the width of this atlas entry
	int16_t h;						///< the height of this atlas entry
} gr_atlas_entry_info_t; 

typedef struct _gr_atlas_entry_data {
	gr_atlas_entry_t *entry;			///< the entry to upload
	gr_atlas_entry_info_t *info_entry;	///< the entry info to upload
} gr_atlas_entry_data_t; 

#endif /* IMAGE_ATLAS_STRUCTURES_H */

