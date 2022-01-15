
/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Display Management (Display)                                        */
/**                                                                       */
/**************************************************************************/


#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_8bit_palette_rotated_setup       PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    8-bit palettte color format display driver rotated setup routine.   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               The display control block     */
/*    aux_data                              Driver-defined auxiliary data */
/*    toggle_function                       Driver-defined screen toggle  */
/*                                            function                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  02-02-2021     Kenneth Maxwell          Initial Version 6.1.4         */
/*                                                                        */
/**************************************************************************/
VOID     _gx_display_driver_8bit_palette_rotated_setup(GX_DISPLAY *display, VOID *aux_data,
                                                       VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                               GX_RECTANGLE *dirty_area))
{
    _gx_display_driver_8bit_palette_setup(display, aux_data, toggle_function);

#if defined(GX_MOUSE_SUPPORT)
    display -> gx_display_mouse_capture = GX_NULL;
    display -> gx_display_mouse_restore = GX_NULL;
    display -> gx_display_mouse_capture = GX_NULL;
    display -> gx_display_mouse_restore = GX_NULL;
    display -> gx_display_mouse_draw = GX_NULL;
    display -> gx_display_driver_drawing_initiate = GX_NULL;
    display -> gx_display_driver_drawing_complete = GX_NULL;
    display -> gx_display_mouse_position_set = GX_NULL;
    display -> gx_display_mouse_enable = GX_NULL;
#endif

    display -> gx_display_rotation_angle                       = GX_SCREEN_ROTATION_CW;

    display -> gx_display_driver_canvas_copy                   = _gx_display_driver_8bpp_rotated_canvas_copy;
    display -> gx_display_driver_simple_line_draw              = _gx_display_driver_8bpp_rotated_simple_line_draw;
    display -> gx_display_driver_horizontal_line_draw          = _gx_display_driver_8bpp_rotated_horizontal_line_draw;
    display -> gx_display_driver_vertical_line_draw            = _gx_display_driver_8bpp_rotated_vertical_line_draw;
    display -> gx_display_driver_horizontal_pattern_line_draw  = _gx_display_driver_8bpp_rotated_horizontal_pattern_line_draw;
    display -> gx_display_driver_horizontal_pixelmap_line_draw = _gx_display_driver_8bpp_rotated_horizontal_pixelmap_line_draw;
    display -> gx_display_driver_vertical_pattern_line_draw    = _gx_display_driver_8bpp_rotated_vertical_pattern_line_draw;
    display -> gx_display_driver_pixel_write                   = _gx_display_driver_8bpp_rotated_pixel_write;
    display -> gx_display_driver_block_move                    = _gx_display_driver_8bpp_rotated_block_move;

    display -> gx_display_driver_pixelmap_draw                 = _gx_display_driver_8bpp_rotated_pixelmap_draw;
    display -> gx_display_driver_pixelmap_rotate               = _gx_display_driver_8bpp_rotated_pixelmap_rotate;
    display -> gx_display_driver_polygon_fill                  = _gx_display_driver_generic_rotated_polygon_fill;

#if defined(GX_ARC_DRAWING_SUPPORT)
    display -> gx_display_driver_circle_fill                   = _gx_display_driver_generic_rotated_circle_fill;
    display -> gx_display_driver_pie_fill                      = _gx_display_driver_generic_rotated_pie_fill;
    display -> gx_display_driver_arc_fill                      = _gx_display_driver_generic_rotated_arc_fill;
    display -> gx_display_driver_ellipse_fill                  = _gx_display_driver_generic_rotated_ellipse_fill;
#endif

#if (GX_PALETTE_MODE_AA_TEXT_COLORS == 8)
    display -> gx_display_driver_4bit_glyph_draw               = _gx_display_driver_8bpp_rotated_glyph_3bit_draw;
#else
    display -> gx_display_driver_4bit_glyph_draw               = _gx_display_driver_8bpp_rotated_glyph_4bit_draw;
#endif
    display -> gx_display_driver_1bit_glyph_draw               = _gx_display_driver_8bpp_rotated_glyph_1bit_draw;
}

