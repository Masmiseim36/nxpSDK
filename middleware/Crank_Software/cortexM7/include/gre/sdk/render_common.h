/*
 * Copyright 2007, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

/**
* \file render_common.h
*	This file describes the structures and routines which are common amongst all renderers
*/

#ifndef GR_RENDER_COMMON_H
#define GR_RENDER_COMMON_H

typedef uint32_t	gr_render_color_t;

typedef struct _gr_render_point {
	int16_t x;
	int16_t y;
} gr_render_point_t;

typedef struct _gr_render_float_point {
	float x;
	float y;
} gr_render_float_point_t;

typedef struct _gr_render_rect {
	int16_t left;
	int16_t top;
	int16_t right;
	int16_t bottom;
} gr_render_rect_t;

typedef struct _gr_render_rect_f {
	float left;
	float top;
	float right;
	float bottom;
} gr_render_rect_f_t;

typedef struct _gr_render_size {
	int16_t width;
	int16_t height;
} gr_render_size_t;


// This needs to go here because the text style is using it
typedef struct _gr_render_font gr_render_font_t;

/* text rendering flags */
/* types */
typedef enum {
	GR_RENDER_TEXT_BITMAP,      //ANTIALIAS1 
	GR_RENDER_TEXT_ANTIALIAS,   //ANTIALIAS8 
	GR_RENDER_TEXT_OUTLINE, 
	GR_RENDER_TEXT_ANTIALIAS2, 
	GR_RENDER_TEXT_ANTIALIAS4, 	
} gr_render_text_type_t;

typedef struct {
	gr_render_font_t		*font;
	gr_render_text_type_t	type;
	int						flags;
	int						size;
	int 					letter_spacing;
	int						word_spacing;
	int						line_height;
	gr_render_color_t 		text_color;
	gr_render_color_t 		outline_color;
} gr_text_style_t;

typedef struct _gr_render_font_info {
	int 				line_height;
	gr_render_size_t 	max_glyph;
	int 				delta;
	int 				ascender;
} gr_render_font_info_t;


/*
 * Rectangle manipulation prototypes
 */

/*
 * Convert a dimension to a rectangle
 * @param rect The rect to return, left and top can be preset with an offset
 * @param size The dimensions to convert
 */
DLLExport void	gr_size_to_rect(gr_render_rect_t *rect, gr_render_size_t *size);

/*
 * Get the dimensions of a rectangle
 * @param rect The rect to convert
 * @param size A location to store the size
 */
DLLExport void	gr_rect_to_size(gr_render_rect_t *rect, gr_render_size_t *size);

/*
 * Translate a rectangle
 * @param rect 	The rect to translate
 * @param x 	The x offset
 * @param y 	The y offset
 */
DLLExport void	gr_translate_rect(gr_render_rect_t *rect, int x, int y);

/*
 * Does the given point intersect the rectangle
 * @param x 	The x position to check
 * @param y 	The y position to check
 * @param rect 	The rectangle to check
 * @returns 1 for intersection, 0 on non intersection
 */
DLLExport int 	gr_point_in_rect(int x, int y, gr_render_rect_t *rect);

/*
 * Combine the src rectangle with the destination, the destination may be modified
 * @param dst The destination rectangle
 * @param src The source rectangle
 */
DLLExport void 	gr_rect_combine(gr_render_rect_t *dst, gr_render_rect_t *src);

/*
 * Calculate if a rectangle intersects a rectangle list.  And optionally merge this rect with the list
 * @param rect 		The rectangle to check
 * @param list 		The list of rectangle to check against
 * @param list_size The number of rectangles in the list
 * @param merge  	If non-zero then this rectangle will be MERGED/added into the list. End result is not an intersection
 * @returns 1 if the rectangle intersects the list, 0 on failure
 */
DLLExport int 	gr_rect_intersect(gr_render_rect_t *rect, gr_render_rect_t *list, unsigned list_size, unsigned merge);

/*
 * Calculate the bounding rectangle of a rectangle list
 * @param bounds The location to store the returned rectangle
 * @param list The rectangle list
 * @param list_size The number of rectangles in the list
 */
DLLExport void 	gr_rect_bounds(gr_render_rect_t *bounds, gr_render_rect_t *list, unsigned list_size);

/*
 * Rotate a rectangle
 * @param rect 		The source rectangle to rotate
 * @param bounds 	The bounding size of the new rectangle
 * @param rotation 	The degrees to rotate
 */
DLLExport void 	gr_rect_rotate_bounds(gr_render_rect_t *rect, gr_render_size_t *bounds, gr_float_t rotation);

/*
 * Rotate a rectangle with float precision
 * @param rect 		The source rectangle to rotate
 * @param bounds 	The bounding size of the new rectangle
 * @param rotation 	The degrees to rotate
 */
DLLExport void 	gr_rect_rotate_bounds_f(gr_render_rect_t *rect, float *width, float *height, gr_float_t rotation);

/*
 * Determines if a rectangle is fully contained in another rectangle
 * @param rect 		The 'larger' rectangle
 * @param check 	The rectangle to check for containment
 * @returns	1 if check is fully contained inside rect
 */
DLLExport int gr_rect_in_rect(gr_render_rect_t *rect, gr_render_rect_t *check);

#endif /* GR_RENDER_COMMON_H */
