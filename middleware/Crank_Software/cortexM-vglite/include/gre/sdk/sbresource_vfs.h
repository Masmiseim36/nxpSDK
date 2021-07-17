/*
 * Copyright 2014, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */
#ifndef _SBRESOURCE_VFS_H
#define _SBRESOURCE_VFS_H


#include <gre/gre.h>
#include <gre/sdk/image_atlas_structures.h>
#include <gre/sdk/render_common.h>
#include <stdarg.h>

/**
 * Resource lookup entry.  Originally this was only used for image content,
 * but now it is used for any exported file resource including fonts and scripts.
 **/
#define SB_RESOURCE_STRUCT_DEFINED 1
struct _sb_resources {
	const char *name;			///< The project relative name of the resource
	const void *img_data;		///< A pointer to the resource data
	int 		img_len;		///< The length of the resource data
};

typedef struct _sb_resources sbvfs_resource_t;  ///<  Structure that defines a resource for use with the sbvfs library

///<  This define allows for backwards compatibility with older header files. 
#define _sb_image_resources _sb_resources

typedef struct _sb_image_blocks {
	int num_entries;			///< The number of entries in the image block
  	char **keys;				///< A variable list of char pointers that contains the names of the images in the block 
  	void **entry_info; 			///< A variable list of entry info pointers that describe the location of an image in the block
	int width;					///< The overall width of the image block
	int height; 				///< The overall height of the image block
	void *data_address; 		///< The starting address of the image block
	int *data_size;				///< The size of the image block 
} sb_image_block_t;				///<  Structure that defines a pre allocated image block for use with the sbvfs library

typedef struct {
	int16_t advance;		///< Advance in pixels
	int16_t left;			///< Bearing X (freetype doc)
	int16_t top;			///< Bearing Y (freetype doc)
} sb_glyph_details_t;

typedef struct {
	int32_t char_code;		///< The character code of the glyph 
	int32_t index; 			///< The index of the glyph in the glyph atlas
} sb_char_code_mappings_t; 

typedef struct {
	int32_t num_mappings;					///< The number of character code mappings in the map
	const sb_char_code_mappings_t *map;		///< An array of character code mappings
} sb_char_code_map_t; 
typedef struct {
  	int32_t size;				///< The point size of the font
  	int32_t first_char;			///< First unicode character represented
	int32_t last_char;			///< Last unicode character represented
	int16_t	flags;				///< Type of font
	int16_t line_height;		///< Line height in pixels
	int16_t max_w;				///< Max character width in pixels
	int16_t max_h;				///< Max character height in pixels
	int16_t	ascender;			///< Font ascender
	int16_t descender;			///< Font descender
	gr_float_t x_scale;			///< Horizontal distance scale
	gr_float_t y_scale;			///< Vertical distance scale
} sb_font_details_t; 

typedef struct {
	sb_font_details_t 	*font_details; 		///< The details about this font that are needed to render glyphs in this font
	sb_glyph_details_t	*glyph_details;		///< The advanced data that is needed to draw specific glyphs
	sb_char_code_map_t	*char_code_map;		///< The character code mappings used for calculating glyph index for a fast lookup
  	atlas_block_t 		block_id;			///< The id of the block that contains the glyph data
	atlas_pool_t 		pool_id; 			///< the id of the pool that contains the block
} sb_font_t;

/**
 * This structure is used to describe exported bitmap fonts.
 */
#define SB_FONT_RESOURCE_STRUCT_DEFINED 1
struct _sb_font_resources {
	const char 						*name;				///< The name of the font resource
	const sb_font_details_t 		font_details;		///< The font details for the font
	const sb_glyph_details_t 		*glyph_details;		///< The individual glyph deatils for the glyphs exported for the font
	const sb_char_code_map_t		char_code_map;		///< The character code mapping for the font 
	const uint8_t 					*glyph_data;		///< A pointer the to memory that contains the rendered glyph data
	const gr_render_size_t 			glyph_data_size;	///< The width and height of the glyph data 
	const gr_atlas_entry_info_t     **entry_info;		///< The atlas entry information for the glyphs in the glyph map
	const uint32_t     				num_entries;		///< The number of glyph entries
};

typedef struct _sb_font_resources sb_font_resource_t;	///< The definition of a font resource structure


/**
 * Implemented by clients to customize resource lookups through
 * multiple different resource structures.
 *
 * @param count Return value indicating how many resource roots are specified, must be 0 or 1
 * @return An array of pointers to resource lookup tables or NULL for no resources
 */
sbvfs_resource_t ** sbvfs_get_resource_roots(int *count);

/**
 * Look up a resource based on its project relative path.
 * @param name The project relative name (ie "images/logo.png")
 * @return The matching resource or NULL if no resource found
 */
sbvfs_resource_t * sbvfs_resource_lookup(const char *name);

/**
 * Iterate through all of the resource directories.
 * @param last A pointer used as the last resource returned, or NULL for the first resource
 * @return A pointer to the resource following 'last' or NULL if no next resource.
 */
sbvfs_resource_t * sbvfs_resource_iterate(sbvfs_resource_t **last);

/**
 * Implemented by clients to customize precomputed image block lookups through
 * multiple different image block structures.
 *
 * @param count Return value indicating how many image blocks are specified, must be 0 or 1
 * @return An array of pointers to image blocks or NULL for no image blocks
 */
sb_image_block_t ** sbvfs_get_image_blocks(int *count);

/**
 * Look up an image based on its project relative path in the precomputed image blocks
 * @param name The project relative name of the image (ie "images/logo.png")
 * @return The precomputed image block that contains the image, otherwise NULL
 */
sb_image_block_t * sbvfs_image_block_lookup(const char *name);

/**
 * Implemented by clients to customize C callback function lookups.
 *
 * @return A pointer to an variable length array of C callback list structures.  The array ends with an entry of NULL, NULL. 
 */
sb_ccallback_t * sbvfs_get_ccallback_list();

/**
 * SIMULATED FILE IO API
 */

#define SBVFS_SEEK_SET 0
#define SBVFS_SEEK_CUR 1
#define SBVFS_SEEK_END 2

/**
 * Opaque data type for SBVFS file IO interactions.
 */
typedef struct _sbvfs_file SBFILE;

/**
 * Matching functionality to OpenGroup/ANSI fopen() 
 * - Read-only mode is the only supported mode
 * See: http://pubs.opengroup.org/onlinepubs/9699919799/functions/fopen.html
 */
SBFILE * sbvfs_fopen(const char *filename);

/**
 * Matching functionality to OpenGroup/ANSI fclose() 
 * See: http://pubs.opengroup.org/onlinepubs/9699919799/functions/fclose.html
 */
int sbvfs_fclose(SBFILE *stream);

/**
 * Matching functionality to OpenGroup/ANSI fread() 
 * See: http://pubs.opengroup.org/onlinepubs/9699919799/functions/fread.html
 */
int sbvfs_fread(void *ptr, int size, int nitems, SBFILE *stream);

/**
 * Matching functionality to OpenGroup/ANSI fwrite() 
 * See: http://pubs.opengroup.org/onlinepubs/9699919799/functions/fwrite.html
 */
int sbvfs_fwrite(const void *ptr, int size, int nitems, SBFILE *stream);

/**
 * Matching functionality to OpenGroup/ANSI fseek() 
 * Uses SBVFS_SEEK_* values for whence
 * See: http://pubs.opengroup.org/onlinepubs/9699919799/functions/fseek.html
 */
int sbvfs_fseek(SBFILE *stream, int offset, int whence);

/**
 * Matching functionality to OpenGroup/ANSI ftell() 
 * See: http://pubs.opengroup.org/onlinepubs/9699919799/functions/ftell.html
 */
int sbvfs_ftell(SBFILE *stream);

/**
 * Matching functionality to OpenGroup/ANSI fgets() 
 * See: http://pubs.opengroup.org/onlinepubs/9699919799/functions/fgets.html
 */
char * sbvfs_fgets(char *s, int nbytes, SBFILE *stream);

/**
 * Matching functionality to OpenGroup/ANSI fgetc() 
 * See: http://pubs.opengroup.org/onlinepubs/9699919799/functions/fgetc.html
 */
int sbvfs_fgetc(SBFILE *stream);

/**
 * Semi-matching functionality to OpenGroup/ANSI ungetc() 
 * See: http://pubs.opengroup.org/onlinepubs/9699919799/functions/ungetc.html
 *      or C99 N1256 7.19.7.11
 *
 * Unlike the standard ungetc which allows insertion of any character
 * (even if it wasn't previously at the position in the file), this
 * only moves the position of the stream back if the characters match.
 */
int sbvfs_ungetc(int c, SBFILE *stream);

/**
 * Matching functionality to OpenGroup/ANSI fscanf() 
 * See: http://pubs.opengroup.org/onlinepubs/9699919799/functions/fscanf.html
 */
int sbvfs_fscanf(SBFILE * stream, const char * format, ... );

/**
 * Matching functionality to OpenGroup/ANSI vfscanf() 
 * See: http://pubs.opengroup.org/onlinepubs/9699919799/functions/vfscanf.html
 */
int sbvfs_vfscanf(SBFILE * stream, const char * format, va_list args);

/**
 * Matching functionality to OpenGroup/ANSI feof() 
 * See: http://pubs.opengroup.org/onlinepubs/9699919799/functions/feof.html
 */
int sbvfs_feof(SBFILE *stream);

#endif
