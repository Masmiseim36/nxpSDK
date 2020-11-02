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
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_display.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_circle_draw              PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver to draw circle.                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xcenter                               x-coord of center of circle   */
/*    ycenter                               y-coord of center of circle   */
/*    r                                     Radius of circle              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_pixel_blend]       Basic display driver pixel    */
/*                                            blend function              */
/*    _gx_utility_rectangle_point_detect    Detect whether a pixel is     */
/*                                            inside rectangle            */
/*    [gx_display_driver_pixel_write]       Basic display driver pixel    */
/*                                            write function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*                                                                        */
/**************************************************************************/
#if defined(GX_ARC_DRAWING_SUPPORT)

VOID _gx_display_driver_generic_circle_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r)
{
    /* The circle draw function is implemented from midpoint circle algorithm. */
INT           x;
INT           y;
INT           d;
GX_POINT      point;
INT           sign[4][2] = { {1, 1}, {-1, 1}, {1, -1}, {-1, -1} };
INT           index;

GX_RECTANGLE *clip = context -> gx_draw_context_clip;
GX_DISPLAY   *display = context -> gx_draw_context_display;
GX_BRUSH     *brush = &context -> gx_draw_context_brush;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE     brush_alpha = brush -> gx_brush_alpha;

    if (display -> gx_display_driver_pixel_blend == GX_NULL)
    {
        /* Pixel blend function is null means alpha isn't supported in this driver.
           So set alpha value to 0xff to make it draw the original color in case GX_BRUSH_ALPHA_SUPPORT is defined. */
        brush_alpha = 0xff;
    }
    else 
    {
        if (brush_alpha == 0)
        {
            /* Nothing to draw here. */
            return;
        }
    }
#endif

    x = 0;
    y = (INT)r;
    d = 5 - (INT)(4 * r);
#if defined (GX_BRUSH_ALPHA_SUPPORT)
    if (brush_alpha != 0xff)
    {
        while (x <= y)
        {
            for (index = 0; index < 4; index++)
            {
                point.gx_point_x = (GX_VALUE)(x * sign[index][0] + xcenter);
                point.gx_point_y = (GX_VALUE)(y * sign[index][1] + ycenter);

                if (_gx_utility_rectangle_point_detect(clip, point))
                {
                    display -> gx_display_driver_pixel_blend(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, brush_alpha);
                }

                point.gx_point_x = (GX_VALUE)(y * sign[index][0] + xcenter);
                point.gx_point_y = (GX_VALUE)(x * sign[index][1] + ycenter);

                if (_gx_utility_rectangle_point_detect(clip, point))
                {
                    display -> gx_display_driver_pixel_blend(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, brush_alpha);
                }
            }

            if (d < 0)
            {
                d += 8 * x + 12;
            }
            else
            {
                d += 8 * (x - y) + 20;
                y--;
            }
            x++;
        }
    }
    else
    {
#endif
        while (x <= y)
        {
            for (index = 0; index < 4; index++)
            {
                point.gx_point_x = (GX_VALUE)(x * sign[index][0] + xcenter);
                point.gx_point_y = (GX_VALUE)(y * sign[index][1] + ycenter);

                if (_gx_utility_rectangle_point_detect(clip, point))
                {
                    display -> gx_display_driver_pixel_write(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color);
                }

                point.gx_point_x = (GX_VALUE)(y * sign[index][0] + xcenter);
                point.gx_point_y = (GX_VALUE)(x * sign[index][1] + ycenter);

                if (_gx_utility_rectangle_point_detect(clip, point))
                {
                    display -> gx_display_driver_pixel_write(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color);
                }
            }

            if (d < 0)
            {
                d += 8 * x + 12;
            }
            else
            {
                d += 8 * (x - y) + 20;
                y--;
            }
            x++;
        }
#if defined (GX_BRUSH_ALPHA_SUPPORT)
    }
#endif
}

#endif

