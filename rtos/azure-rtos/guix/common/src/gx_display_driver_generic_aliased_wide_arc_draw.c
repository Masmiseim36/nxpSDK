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
#include "gx_canvas.h"

#if defined(GX_ARC_DRAWING_SUPPORT)

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_simple_aliased_wide_arc_draw             */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Innner help function that draw an aliased wide arc between [90, 180]*/
/*     or [270, 540].                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xcenter                               curx-coord of center of circle*/
/*    ycenter                               cury-coord of center of circle*/
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
/*    _gx_display_driver_arc_clipping_get   Get an arc clipping.          */
/*    _gx_utility_rectangle_point_detect    Detect whether a pixel is     */
/*                                            inside rectangle            */
/*    _gx_utility_circle_point_get          Get point coord on a circle   */
/*    [gx_display_driver_horizontal_line_draw]                            */
/*                                          Basic display driver          */
/*                                            horizontal line draw routine*/
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_generic_aliased_wide_arc_draw                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_generic_simple_aliased_wide_arc_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle)
{
/* The function draw a wide arc between 90 and 270 or beween 270 and 450.*/
GX_DISPLAY   *display;
GX_BRUSH     *brush;
GX_RECTANGLE *clip;
GX_RECTANGLE  arc_clip[4];
GX_POINT      point;
GX_POINT      inner_start;
GX_POINT      inner_end;
GX_POINT      outer_start;
GX_POINT      outer_end;
INT           sign[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
INT          *pLineEnds;
INT           ymin;
INT           ymax;
INT           xstart;
INT           xend;
INT           ystart;
INT           yend;
INT           curx;
INT           cury;
INT           nextx;
INT           nexty;
INT           dx;
INT           dy;
INT           Index;
INT           Index1;
INT           loop;
INT           height;
INT           xsign;
INT           ysign;
INT           error;
INT           decision;
INT           brush_width;
GX_UBYTE      alpha1;
GX_UBYTE      alpha2;
VOID          (*blend_func)(GX_DRAW_CONTEXT *, INT, INT, GX_COLOR, GX_UBYTE);
GX_BOOL       right = GX_TRUE;
INT           test;
INT           cur_shift;
INT           next_shift;

    if ((start_angle >= 90) &&
        (start_angle <= 270) &&
        (end_angle <= 270))
    {
        right = GX_FALSE;
    }

    display = context -> gx_draw_context_display;
    brush = &context -> gx_draw_context_brush;
    brush_width = brush -> gx_brush_width;
    blend_func = display -> gx_display_driver_pixel_blend;

    if (blend_func == GX_NULL)
    {
        return;
    }

    clip = context -> gx_draw_context_clip;
    pLineEnds = _gx_system_scratchpad;

    if (r <= (UINT)((brush_width - 1) >> 1))
    {
        return;
    }

    /* Calculate the ridius of the inner circle.  */
    r = (UINT)(r - (UINT)((brush_width - 1) >> 1));

    /* Get end points. */
    _gx_utility_circle_point_get(xcenter, ycenter, r, start_angle, &inner_start);
    _gx_utility_circle_point_get(xcenter, ycenter, r, end_angle, &inner_end);
    _gx_utility_circle_point_get(xcenter, ycenter, (UINT)(r + (UINT)brush_width - 1), start_angle, &outer_start);
    _gx_utility_circle_point_get(xcenter, ycenter, (UINT)(r + (UINT)brush_width - 1), end_angle, &outer_end);

    ymin = ycenter - (INT)r - brush_width + 1;
    ymax = ycenter + (INT)r + brush_width - 1;

    if (((start_angle < 90) && (end_angle < 90)) ||
        ((start_angle > 90) && (end_angle < 450)))
    {
        if (outer_start.gx_point_y < outer_end.gx_point_y)
        {
            ymin = outer_start.gx_point_y;
        }
        else
        {
            ymin = outer_end.gx_point_y;
        }

        if (inner_start.gx_point_y < ymin)
        {
            ymin = inner_start.gx_point_y;
        }

        if (inner_end.gx_point_y < ymin)
        {
            ymin = inner_end.gx_point_y;
        }
    }

    if (clip -> gx_rectangle_top > ymin)
    {
        ymin = clip -> gx_rectangle_top;
    }

    /* Calculate maximum y line. */
    if (((start_angle < 270) && (end_angle < 270)) || (start_angle > 270))
    {
        if (outer_start.gx_point_y > outer_end.gx_point_y)
        {
            ymax = outer_start.gx_point_y;
        }
        else
        {
            ymax = outer_end.gx_point_y;
        }

        if (inner_start.gx_point_y > ymax)
        {
            ymax = inner_start.gx_point_y;
        }

        if (inner_end.gx_point_y > ymax)
        {
            ymax = inner_end.gx_point_y;
        }
    }

    if (clip -> gx_rectangle_bottom < ymax)
    {
        ymax = clip -> gx_rectangle_bottom;
    }

    height = ymax - ymin + 1;

    /* default the point array to being off the screen on both sides: */

    for (loop = 0; loop < height * 2; loop += 2)
    {
        pLineEnds[loop] = 2000;
        pLineEnds[loop + 1] = 0;
    }

    /* Get point array of inner arc and outer arc. */
    for (Index1 = 0; Index1 < 2; Index1++)
    {
        if (Index1 == 1)
        {
            r += (UINT)(brush_width - 1);
        }

        _gx_display_driver_arc_clipping_get(xcenter, ycenter, r, start_angle, end_angle,
                                            &arc_clip[0], &arc_clip[1], &arc_clip[2], &arc_clip[3]);

        curx = 0;
        cury = (INT)r;
        error = 0;

        while (curx < cury)
        {
            alpha1 = (GX_UBYTE)(255 - error);
            alpha2 = (GX_UBYTE)error;

            for (loop = 0; loop < 4; loop++)
            {
                point.gx_point_x = (GX_VALUE)(curx * sign[loop][0] + xcenter);
                point.gx_point_y = (GX_VALUE)(cury * sign[loop][1] + ycenter);

                if ((_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                     _gx_utility_rectangle_point_detect(&arc_clip[1], point)) &&
                    _gx_utility_rectangle_point_detect(clip, point))
                {
                    /* Draw point(curx, cury).  */
                    blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, alpha1);
                }

                point.gx_point_y = (GX_VALUE)((cury - 1) * sign[loop][1] + ycenter);

                if ((_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                     _gx_utility_rectangle_point_detect(&arc_clip[1], point)) &&
                    _gx_utility_rectangle_point_detect(clip, point))
                {
                    blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, (GX_UBYTE)alpha2);
                }

                point.gx_point_x = (GX_VALUE)(cury * sign[loop][0] + xcenter);
                point.gx_point_y = (GX_VALUE)(curx * sign[loop][1] + ycenter);

                if ((_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                     _gx_utility_rectangle_point_detect(&arc_clip[1], point)) &&
                    _gx_utility_rectangle_point_detect(clip, point))
                {
                    /* Draw point(cury, curx).  */
                    blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color,  alpha1);
                }

                if ((point.gx_point_y >= ymin) && (point.gx_point_y <= ymax))
                {
                    if (_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                        _gx_utility_rectangle_point_detect(&arc_clip[1], point))
                    {
                        Index = (point.gx_point_y - ymin) << 1;

                        if (right)
                        {
                            if (Index1 == 0)
                            {
                                pLineEnds[Index] = cury * sign[loop][0] + xcenter;
                            }
                            else
                            {
                                pLineEnds[Index + 1] = (cury - 1) * sign[loop][0] + xcenter;
                            }
                        }
                        else
                        {
                            if (Index1 == 0)
                            {
                                pLineEnds[Index + 1] = cury * sign[loop][0] + xcenter;
                            }
                            else
                            {
                                pLineEnds[Index] = (cury - 1) * sign[loop][0] + xcenter;
                            }
                        }
                    }
                }

                point.gx_point_x = (GX_VALUE)((cury - 1) * sign[loop][0] + xcenter);

                if ((_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                     _gx_utility_rectangle_point_detect(&arc_clip[1], point)) &&
                    _gx_utility_rectangle_point_detect(clip, point))
                {
                    /* Draw point(cury - 1, curx).  */
                    blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color, (GX_UBYTE)alpha2);
                }

                if ((point.gx_point_y >= ymin) && (point.gx_point_y <= ymax))
                {
                    if (_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                        _gx_utility_rectangle_point_detect(&arc_clip[1], point))
                    {
                        Index = (point.gx_point_y - ymin) << 1;

                        if (right)
                        {
                            if (Index1 == 0)
                            {
                                pLineEnds[Index] = cury * sign[loop][0] + xcenter;
                            }
                            else
                            {
                                pLineEnds[Index + 1] = (cury - 1) * sign[loop][0] + xcenter;
                            }
                        }
                        else
                        {
                            if (Index1 == 0)
                            {
                                pLineEnds[Index + 1] = cury * sign[loop][0] + xcenter;
                            }
                            else
                            {
                                pLineEnds[Index] = (cury - 1) * sign[loop][0] + xcenter;
                            }
                        }
                    }
                }
            }

            /* Calculate the distance between mathmatical point to drawing poing,
               which is used to blending pixel.  */
            curx++;
            nexty = (INT)(r * r) - curx * curx;
            error = (cury << 8) - (INT)(_gx_utility_math_sqrt((UINT)(nexty << 10)) << 3);

            while (error >= 255)
            {
                error -= 255;
                cury--;

                for (loop = 0; loop < 4; loop++)
                {
                    point.gx_point_x = (GX_VALUE)(curx * sign[loop][0] + xcenter);
                    point.gx_point_y = (GX_VALUE)(cury * sign[loop][1] + ycenter);

                    if ((point.gx_point_y >= ymin) && (point.gx_point_y <= ymax))
                    {
                        if (_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                            _gx_utility_rectangle_point_detect(&arc_clip[1], point))
                        {
                            Index = (point.gx_point_y - ymin) << 1;

                            if (right)
                            {
                                if (Index1 == 0)
                                {
                                    pLineEnds[Index] = curx * sign[loop][0] + xcenter;
                                }
                                else
                                {
                                    pLineEnds[Index + 1] = (curx - 1) * sign[loop][0] + xcenter;
                                }
                            }
                            else
                            {
                                if (Index1 == 0)
                                {
                                    pLineEnds[Index + 1] = curx * sign[loop][0] + xcenter;
                                }
                                else
                                {
                                    pLineEnds[Index] = (curx - 1) * sign[loop][0] + xcenter;
                                }
                            }
                        }
                    }
                }
            }
        }

        alpha1 = (GX_UBYTE)(255 - error);

        for (loop = 0; loop < 4; loop++)
        {

            point.gx_point_x = (GX_VALUE)(curx * sign[loop][0] + xcenter);
            point.gx_point_y = (GX_VALUE)(cury * sign[loop][1] + ycenter);

            if ((_gx_utility_rectangle_point_detect(&arc_clip[0], point) ||
                 _gx_utility_rectangle_point_detect(&arc_clip[1], point)) &&
                _gx_utility_rectangle_point_detect(clip, point))
            {
                blend_func(context, point.gx_point_x, point.gx_point_y, brush -> gx_brush_line_color,  alpha1);
            }
        }
    }

    /* Fill in the point array by using Breshenhams line for
       2 lines of the arc end.
     */

    for (loop = 0; loop < 2; loop++)
    {
        if (loop == 0)
        {
            xstart = inner_start.gx_point_x;
            ystart = inner_start.gx_point_y;
            xend = outer_start.gx_point_x;
            yend = outer_start.gx_point_y;
        }
        else
        {
            xstart = inner_end.gx_point_x;
            ystart = inner_end.gx_point_y;
            xend = outer_end.gx_point_x;
            yend = outer_end.gx_point_y;
        }

        dx = GX_ABS(xend - xstart);
        dy = GX_ABS(yend - ystart);

        /* Horizontal Line. */
        if (ystart == yend)
        {
            continue;
        }

        /* Vertical Line. */
        if (xstart == xend)
        {
            if (ystart > yend)
            {
                GX_SWAP_VALS(xstart, xend);
                GX_SWAP_VALS(ystart, yend);
            }

            for (cury = ystart; cury <= yend; cury++)
            {
                if ((cury >= ymin) && (cury <= ymax))
                {
                    Index = (cury - ymin) << 1;
                    if (xstart <= pLineEnds[Index])
                    {
                        pLineEnds[Index] = xstart;
                    }

                    if (xstart > pLineEnds[Index + 1])
                    {
                        pLineEnds[Index + 1] = xstart;
                    }
                }
            }
            continue;
        }

        /* Simple Line. */
        if (((dx >= dy && (xstart > xend)) ||
             ((dy > dx) && ystart > yend)))
        {
            GX_SWAP_VALS(xend, xstart);
            GX_SWAP_VALS(yend, ystart);
        }

        xsign = (xend - xstart) / dx;
        ysign = (yend - ystart) / dy;

        cur_shift = 0;
        next_shift = 0;

        if (dx >= dy)
        {
            if ((right && loop == 1) ||
                (!right && loop == 0))
            {
                if (ysign > 0)
                {
                    cur_shift = 1;
                }
                else
                {
                    next_shift = 1;
                }
            }
            else
            {
                if (ysign > 0)
                {
                    next_shift = -1;
                }
                else
                {
                    cur_shift = -1;
                }
            }

            for (curx = xstart, cury = ystart, nextx = xend, nexty = yend,
                 decision = 0; curx <= nextx; curx++, nextx--,
                 decision += dy)
            {
                if (decision >= dx)
                {
                    decision -= dx;
                    cury += ysign;
                    nexty -= ysign;
                }

                test = cury + cur_shift;

                if ((test >= ymin) && (test <= ymax))
                {
                    Index = (test - ymin) << 1;

                    if (curx < pLineEnds[Index])
                    {
                        pLineEnds[Index] = curx;
                    }

                    if (curx > pLineEnds[Index + 1])
                    {
                        pLineEnds[Index + 1] = curx;
                    }
                }

                test = nexty + next_shift;

                if ((test >= ymin) && (test <= ymax))
                {
                    Index1 = (test - ymin) << 1;

                    if (nextx < pLineEnds[Index1])
                    {
                        pLineEnds[Index1] = nextx;
                    }

                    if (nextx > pLineEnds[Index1 + 1])
                    {
                        pLineEnds[Index1 + 1] = nextx;
                    }
                }
            }
        }
        else
        {
            if ((right && loop == 1) ||
                (!right && loop == 0))
            {
                next_shift = -xsign;
            }
            else
            {
                cur_shift = xsign;
            }

            for (curx = xstart, cury = ystart, nextx = xend, nexty = yend,
                 decision = 0; cury <= nexty; cury++, nexty--,
                 decision += dx)
            {
                if (decision >= dy)
                {
                    decision -= dy;
                    curx += xsign;
                    nextx -= xsign;
                }

                if ((cury >= ymin) && (cury <= ymax))
                {

                    Index = (cury - ymin) << 1;
                    test = curx + cur_shift;

                    if (test < pLineEnds[Index])
                    {
                        pLineEnds[Index] = test;
                    }

                    if (test > pLineEnds[Index + 1])
                    {
                        pLineEnds[Index + 1] = test;
                    }
                }

                if ((nexty >= ymin) && (nexty <= ymax))
                {
                    Index1 = (nexty - ymin) << 1;
                    test = nextx + next_shift;

                    if (test < pLineEnds[Index1])
                    {
                        pLineEnds[Index1] = test;
                    }

                    if (test > pLineEnds[Index1 + 1])
                    {
                        pLineEnds[Index1 + 1] = test;
                    }
                }
            }
        }
    }

    /* Filling the outline area with horizontal line. */

    Index = 0;
    for (cury = ymin; cury <= ymax; cury++)
    {
        if (pLineEnds[Index] <= pLineEnds[Index + 1])
        {
            if (pLineEnds[Index] < clip -> gx_rectangle_left)
            {
                pLineEnds[Index] = clip -> gx_rectangle_left;
            }

            if (pLineEnds[Index + 1] > clip -> gx_rectangle_right)
            {
                pLineEnds[Index + 1] = clip -> gx_rectangle_right;
            }

            display -> gx_display_driver_horizontal_line_draw(context, pLineEnds[Index], pLineEnds[Index + 1], cury, 1,
                                                              brush -> gx_brush_line_color);
        }
        Index += 2;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_aliased_wide_arc_draw    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Display driver function to draw wide anti-aliased circular arc.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                             Drawing context                 */
/*    xcenter                             curx-coord of center of circle  */
/*    ycenter                             cury-coord of center of circle  */
/*    r                                   Radius of circle                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_generic_simple_wide_arc_draw]                    */
/*                                          Real display driver wide arc  */
/*                                            draw function               */
/*    _gx_utility_circle_point_get          Get point coord on a circle   */
/*    _gx_canvas_circle_draw                Draw circle into a canvas     */
/*    [gx_display_driver_anti_aliased_line_draw]                          */
/*                                          Basic driver-level aliased    */
/*                                            line draw function          */
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
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_generic_aliased_wide_arc_draw(GX_DRAW_CONTEXT *context, INT xcenter, INT ycenter, UINT r, INT start_angle, INT end_angle)
{
GX_BRUSH   *brush;
INT         brush_width;
GX_POINT    startp;
GX_POINT    endp;
GX_DISPLAY *display;
GX_COLOR    old_fill;
UINT        old_style;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE old_alpha;

    old_alpha = context -> gx_draw_context_brush.gx_brush_alpha;
    context -> gx_draw_context_brush.gx_brush_alpha = GX_ALPHA_VALUE_OPAQUE;
#endif

    brush = &context -> gx_draw_context_brush;
    brush_width = brush -> gx_brush_width;
    display = context -> gx_draw_context_display;

    if (start_angle < 90)
    {
        if (end_angle <= 90)
        {
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, start_angle, end_angle);
        }
        else if (end_angle <= 270)
        {
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, start_angle, 90);
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, 90, end_angle);
        }
        else
        {
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, start_angle, 90);
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, 90, 270);
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, 270, end_angle);
        }
    }
    else if (start_angle < 270)
    {
        if (end_angle <= 270)
        {
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, start_angle, end_angle);
        }
        else if (end_angle <= 450)
        {
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, start_angle, 270);
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, 270, end_angle);
        }
        else
        {
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, start_angle, 270);
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, 270, 450);
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, 90, end_angle - 360);
        }
    }
    else
    {
        if (end_angle <= 450)
        {
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, start_angle, end_angle);
        }
        else if (end_angle <= 630)
        {
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, start_angle, 450);
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, 90, end_angle - 360);
        }
        else
        {
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, start_angle, 450);
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, 90, 270);
            _gx_display_driver_generic_simple_aliased_wide_arc_draw(context, xcenter, ycenter, r, 270, end_angle - 360);
        }
    }

    old_fill = brush -> gx_brush_fill_color;
    old_style = brush -> gx_brush_style;

    brush -> gx_brush_width = 1;
    brush -> gx_brush_fill_color = brush -> gx_brush_line_color;
    brush -> gx_brush_style |= GX_BRUSH_SOLID_FILL;

    r = (UINT)(r - (UINT)((brush_width - 1) >> 1));
    _gx_utility_circle_point_get(xcenter, ycenter, r, start_angle, &startp);
    _gx_utility_circle_point_get(xcenter, ycenter, r + (UINT)brush_width - 1, start_angle, &endp);

    if (brush -> gx_brush_style & GX_BRUSH_ROUND)
    {
        brush -> gx_brush_style &= (ULONG)(~GX_BRUSH_PIXELMAP_FILL);

        _gx_display_driver_generic_aliased_filled_circle_draw(context,
                                                              GX_FIXED_VAL_MAKE(startp.gx_point_x + endp.gx_point_x) >> 1,
                                                              GX_FIXED_VAL_MAKE(startp.gx_point_y + endp.gx_point_y) >> 1,
                                                              GX_FIXED_VAL_MAKE(brush_width) >> 1);
    }
    else
    {
        display -> gx_display_driver_anti_aliased_line_draw(context,
                                                            startp.gx_point_x,
                                                            startp.gx_point_y,
                                                            endp.gx_point_x,
                                                            endp.gx_point_y);
    }

    _gx_utility_circle_point_get(xcenter, ycenter, r, end_angle, &startp);
    _gx_utility_circle_point_get(xcenter, ycenter, r + (UINT)brush_width - 1, end_angle, &endp);

    if (brush -> gx_brush_style & GX_BRUSH_ROUND)
    {
        brush -> gx_brush_style &= (ULONG)(~GX_BRUSH_PIXELMAP_FILL);

        _gx_display_driver_generic_aliased_filled_circle_draw(context,
                                                              GX_FIXED_VAL_MAKE(startp.gx_point_x + endp.gx_point_x) >> 1,
                                                              GX_FIXED_VAL_MAKE(startp.gx_point_y + endp.gx_point_y) >> 1,
                                                              GX_FIXED_VAL_MAKE(brush_width) >> 1);
    }
    else
    {
        display -> gx_display_driver_anti_aliased_line_draw(context,
                                                            startp.gx_point_x,
                                                            startp.gx_point_y,
                                                            endp.gx_point_x,
                                                            endp.gx_point_y);
    }

    brush -> gx_brush_width = (GX_VALUE)brush_width;
    brush -> gx_brush_fill_color = old_fill;
    brush -> gx_brush_style = old_style;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
    context -> gx_draw_context_brush.gx_brush_alpha = old_alpha;
#endif
}
#endif

