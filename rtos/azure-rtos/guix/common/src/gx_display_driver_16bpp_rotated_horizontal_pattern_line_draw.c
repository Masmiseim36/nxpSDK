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
/*    _gx_display_driver_16bpp_rotated_horizontal_pattern_line_draw       */
/*                                                        PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic 16bpp color format rotated horizontal pattern line draw     */
/*    function.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xstart                                x-coord of left endpoint      */
/*    xend                                  x-coord of right endpoint     */
/*    ypos                                  y-coord of line top           */
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
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_16bpp_rotated_horizontal_pattern_line_draw(GX_DRAW_CONTEXT *context, INT xstart, INT xend, INT ypos)
{
INT     column;
USHORT *put;
USHORT *rowstart;
ULONG   pattern;
ULONG   mask;
USHORT  on_color;
USHORT  off_color;
INT     len = xend - xstart + 1;

    /* Pick up start address of canvas memory.  */
    rowstart = (USHORT *)context -> gx_draw_context_memory;

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        /* Calculate start of row address.  */
        rowstart += (context -> gx_draw_context_canvas -> gx_canvas_x_resolution - xstart - 1) * context -> gx_draw_context_pitch;

        /* Calculate pixel address.  */
        rowstart += ypos;
    }
    else
    {
        /* Calculate start of row address.  */
        rowstart += xend * context -> gx_draw_context_pitch;

        /* Calculate pixel address.  */
        rowstart += (context -> gx_draw_context_canvas -> gx_canvas_y_resolution - ypos - 1);
    }

    /* Draw 1-pixel hi lines to fill width.  */

    /* Pick up the requested pattern and mask.  */
    pattern = context -> gx_draw_context_brush.gx_brush_line_pattern;
    mask = context -> gx_draw_context_brush.gx_brush_pattern_mask;
    on_color = (USHORT)context -> gx_draw_context_brush.gx_brush_line_color;
    off_color = (USHORT)context -> gx_draw_context_brush.gx_brush_fill_color;

    put = rowstart;

    /* Draw line from bottom to top.  */
    for (column = 0; column < len; column++)
    {
        if (pattern & mask)
        {
            *put = on_color;
        }
        else
        {
            *put = off_color;
        }

        put -= context -> gx_draw_context_pitch;
        mask >>= 1;
        if (!mask)
        {

            /* Set most significant bit to repeat the pattern draw.  */
            mask = 0x80000000;
        }
    }

    /* Save current masks value back to brush.  */
    context -> gx_draw_context_brush.gx_brush_pattern_mask = mask;
}

