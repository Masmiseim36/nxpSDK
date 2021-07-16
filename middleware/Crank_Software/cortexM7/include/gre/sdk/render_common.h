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

// round n up to the nearest multiple of alignment. Alignment must be a power of 2.
#define GET_ALIGNED_VALUE(n, alignment) (n + alignment - 1) & ~(alignment - 1)

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
	unsigned				flags;
	unsigned				size;
	int						letter_spacing;
	int		 				word_spacing;
	unsigned				line_height;
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
 * 
 * @param rect The gr_render_rect_t rect to convert 
 * The rect to return, left and top can be preset with an offset
 * @param size The dimensions to convert
 */
#define gr_size_to_rect(_rect, _size) {							\
		(_rect)->right = (_rect)->left + (_size)->width - 1;	\
		(_rect)->bottom = (_rect)->top + (_size)->height - 1;	\
	}

/*
 * Get the dimensions of a rectangle
 * 
 * @param rect The gr_render_rect_t rect to convert
 * @param size The gr_render_size_t to contain the size
 */	
#define gr_rect_to_size(_rect, _size) {							\
		(_size)->width = (_rect)->right - (_rect)->left + 1;  	\
		(_size)->height = (_rect)->bottom - (_rect)->top + 1;	\
	}

/*
 * Translate a rectangle
 * 
 * @param rect 	The gr_render_rect_t rect to translate
 * @param x 	The x offset
 * @param y 	The y offset
 */
#define gr_translate_rect(_rect, _x, _y) {			\
		int _tx = (_x);								\
		int _ty = (_y);								\
		(_rect)->left += (_tx);						\
		(_rect)->right += (_tx);					\
		(_rect)->top += (_ty);						\
		(_rect)->bottom += (_ty);					\
	}

/*
 * Determine if a point lies within the bounds of a rectangle
 * 
 * @param x 	The x position to check
 * @param y 	The y position to check
 * @param rect 	The gr_render_rect_t rect
 * @returns 1 for intersection, 0 on non intersection
 */
#define gr_point_in_rect(_x, _y, _rect)				\
	(((_x) < (_rect)->left) || 						\
	 ((_y) < (_rect)->top) || 						\
	 ((_x) > (_rect)->right) || 					\
	 ((_y) > (_rect)->bottom)) ? 0 : 1

/*
 * Determine if one rectangle is fully enclosed in another rectangle
 * 
 * @param outside The larger (outside) gr_render_rect_t
 * @param inside The smaller (inside) gr_render_rect_t
 * @returns	1 if inside is fully contained within outside
 */
#define gr_rect_in_rect(_outside, _inside) 				\
	(((_inside)->left >= (_outside)->left) && 			\
	 ((_inside)->right <= (_outside)->right) &&			\
	 ((_inside)->top >= (_outside)->top) && 				\
	 ((_inside)->bottom <= (_outside)->bottom)) ? 1 : 0

/*
 * Combine the src rectangle with the destination, the destination may be modified
 * 
 * @param dst The destination rectangle
 * @param src The source rectangle
 */
DLLExport void 	gr_rect_combine(gr_render_rect_t *dst, gr_render_rect_t *src);

/*
 * Calculate if a rectangle intersects a rectangle list.  And optionally merge this rect with the list
 * 
 * @param rect 		The rectangle to check
 * @param list 		A list of existing non-self-intersecting rectangles to check against
 * @param list_size The number of rectangles in the list
 * @param merge  	If non-zero then this rectangle will be MERGED into the list if an intersection is found
 * @returns 0 on failure to intersect, otherwise the size of the rectangle list (always non-zero may get smaller than list_size on intersect)
 */
DLLExport int 	gr_rect_intersect(gr_render_rect_t *rect, gr_render_rect_t *list, unsigned list_size, unsigned merge);

/*
 * Calculate the bounding rectangle of a rectangle list
 * 
 * @param bounds The location to store the returned rectangle
 * @param list The rectangle list
 * @param list_size The number of rectangles in the list
 */
DLLExport void 	gr_rect_bounds(gr_render_rect_t *bounds, gr_render_rect_t *list, unsigned list_size);

/*
 * Rotate a rectangle
 * 
 * @param rect 		The source rectangle to rotate
 * @param bounds 	The bounding size of the new rectangle
 * @param rotation 	The degrees to rotate
 */
DLLExport void 	gr_rect_rotate_bounds(gr_render_rect_t *rect, gr_render_size_t *bounds, gr_float_t rotation);

/*
 * Rotate a rectangle with float precision
 * 
 * @param rect 		The source rectangle to rotate
 * @param bounds 	The bounding size of the new rectangle
 * @param rotation 	The degrees to rotate
 */
DLLExport void 	gr_rect_rotate_bounds_f(gr_render_rect_t *rect, float *width, float *height, gr_float_t rotation);

#endif /* GR_RENDER_COMMON_H */
