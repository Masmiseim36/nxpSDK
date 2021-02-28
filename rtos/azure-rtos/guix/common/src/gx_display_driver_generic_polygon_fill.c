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
/**   Canvas Management (Canvas)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_display.h"
#include "gx_canvas.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_polygon_min_max_get              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that calculates the top most and bottom    */
/*    most y coordinate of the specified polygon.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    vertex                            Array of points of the polygon,   */
/*                                        terminated by NULL entry        */
/*    num                               Number of points of polygon       */
/*    ymin                              Retrieved top most y coordinate   */
/*    ymax                              Retrieved bottom most y coordinate*/
/*    xmin                              Retrieved left most x coordinate  */
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
/*    _gx_display_driver_generic_polygon_fill                             */
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
static VOID _gx_display_driver_polygon_min_max_get(GX_POINT *vertex, INT num, INT *ymin, INT *ymax, INT *xmin)
{
GX_VALUE index = (GX_VALUE)num;
INT      y_min;
INT      y_max;
INT      x_min;

    x_min = vertex[0].gx_point_x;
    y_min = vertex[0].gx_point_y;
    y_max = vertex[0].gx_point_y;

    while (index--)
    {
        if (vertex[index].gx_point_y < y_min)
        {
            y_min = vertex[index].gx_point_y;
        }
        else if (vertex[index].gx_point_y > y_max)
        {
            y_max = vertex[index].gx_point_y;
        }

        if (vertex[index].gx_point_x < x_min)
        {
            x_min = vertex[index].gx_point_x;
        }
    }

    *ymin = y_min;
    *ymax = y_max;
    *xmin = x_min;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_polygon_new_edge_table_init      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that Initiates new edge table, which       */
/*    contain all the edges of the specified polygon.                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    vertex                            Array of points of the polygon,   */
/*                                        terminated by NULL entry        */
/*    num                               Number of points of polygon       */
/*    net                               Pointer to new edge table         */
/*    net_size                          New edge table size               */
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
/*    _gx_display_driver_generic_polygon_fill                             */
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
static VOID _gx_display_driver_polygon_new_edge_table_init(GX_DRAW_CONTEXT *context, GX_POINT *vertex, INT num, GX_POLYGON_EDGE *net, INT *net_size)
{
GX_POLYGON_EDGE edge;
INT             index;
GX_POINT        s_point;
GX_POINT        ss_point;
GX_POINT        e_point;
GX_POINT        ee_point;
GX_VALUE        net_index = 0;

    for (index = 0; index < num; index++)
    {
        s_point = vertex[index];
        e_point = vertex[(index + 1) % num];
        ss_point = vertex[(index - 1 + num) % num];
        ee_point = vertex[(index + 2) % num];

        /* Skip horizontal line. */
        if (s_point.gx_point_y != e_point.gx_point_y)
        {
            /* Add one edge to new edge table, s_point and e_point are two endpoints of the added edge. */

            edge.gx_polygon_edge_dx = GX_ABS(e_point.gx_point_x - s_point.gx_point_x);
            edge.gx_polygon_edge_dy = GX_ABS(e_point.gx_point_y - s_point.gx_point_y);

            if (context -> gx_draw_context_brush.gx_brush_style & GX_BRUSH_ALIAS)
            {
                edge.gx_polygon_edge_x_decision = 0;
            }
            else
            {
                edge.gx_polygon_edge_x_decision = edge.gx_polygon_edge_dy >> 1;
            }

            if (((e_point.gx_point_x > s_point.gx_point_x) && (e_point.gx_point_y > s_point.gx_point_y)) ||
                ((e_point.gx_point_x < s_point.gx_point_x) && (e_point.gx_point_y < s_point.gx_point_y)))
            {
                edge.gx_polygon_edge_sign = 1;
            }
            else
            {
                edge.gx_polygon_edge_sign = -1;
            }

            if (e_point.gx_point_y > s_point.gx_point_y)
            {
                /* Set xi value, which is the x coordinate value of the edge's toppest point. */
                edge.gx_polygon_edge_xi = s_point.gx_point_x;

                /* Set max y coordinate of the edge. */
                edge.gx_polygon_edge_ymax = e_point.gx_point_y;

                if (ee_point.gx_point_y >= e_point.gx_point_y)
                {
                    /* If e_point isn't the bottom most point, reset ymax to ymax - 1.
                       To avoid producing 2 intersections when scan line comes to the vertex point. */
                    edge.gx_polygon_edge_ymax -= 1;
                }

                edge.gx_polygon_edge_ymin = s_point.gx_point_y;
            }
            else
            {
                /* Set xi value. */
                edge.gx_polygon_edge_xi = e_point.gx_point_x;

                /* Set max y coordinate of the edge. */
                edge.gx_polygon_edge_ymax = s_point.gx_point_y;

                if (ss_point.gx_point_y >= s_point.gx_point_y)
                {
                    /* If e_point isn't the bottom most point, reset ymax to ymax - 1. */
                    edge.gx_polygon_edge_ymax -= 1;
                }

                edge.gx_polygon_edge_ymin = e_point.gx_point_y;
            }

            net[net_index++] = edge;
        }
    }

    *net_size = net_index;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_polygon_active_edge_insert       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that Inserts an edge from new edge table to*/
/*    active edge table.                                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    aet                               Pointer to active edge table      */
/*    net                               Pointer to new edge table         */
/*    y                                 Y coordinate of current scan line */
/*    net                               Pointer to new edge table         */
/*    net_size                          New edge table size               */
/*    aet_size                          Retured active edge table size    */
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
/*    _gx_display_driver_generic_polygon_fill                             */
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
static VOID _gx_display_driver_polygon_active_edge_insert(GX_POLYGON_EDGE *aet, GX_POLYGON_EDGE *net, INT y, INT net_size, INT *aet_size)
{
GX_VALUE ni = (GX_VALUE)(net_size - 1);
GX_VALUE ai;
INT      active_edge_table_size = *aet_size;

    for (ni = 0; ni < net_size; ni++)
    {
        if (net[ni].gx_polygon_edge_ymin == y)
        {
            /* If the toppest y coordinate value of the edge equals to the current scan line y,
               Insert the edge to active edge table, and increase aet size by 1. */

            ai = (GX_VALUE)(active_edge_table_size - 1);
            while ((ai >= 0) && (net[ni].gx_polygon_edge_xi < aet[ai].gx_polygon_edge_xi))
            {
                aet[ai + 1] = aet[ai];
                ai--;
            }
            aet[ai + 1] = net[ni];
            active_edge_table_size++;
        }
    }

    *aet_size = active_edge_table_size;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_polygon_non_active_edge_remove   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that removes an edge from active edge table*/
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    aet                               Pointer to active edge table      */
/*    y                                 Y coordinate of current scan line */
/*    aet_size                          Active edge table size            */
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
/*    _gx_display_driver_generic_polygon_fill                             */
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
static VOID _gx_display_driver_polygon_non_active_edge_remove(GX_POLYGON_EDGE *aet, INT y, INT *aet_size)
{
INT i;
INT j;

    for (i = 0; i < *aet_size; i++)
    {
        /* If the bottom most y coordinate value of the edge equals to the current scan line y,
           Remove the edge from active edge table, and decrease aet size by 1. */
        if (aet[i].gx_polygon_edge_ymax == y)
        {
            for (j = i; j < *aet_size - 1; j++)
            {
                aet[j] = aet[j + 1];
            }
            (*aet_size)--;
            i--;
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_polygon_scan_line_draw           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that filling area between pair             */
/*    intersections.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                           Drawing context                   */
/*    aet                               Pointer to active edge table      */
/*    y                                 Y coordinate of current scan line */
/*    aet_size                          Active edge table size            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_horizontal_line_draw]                            */
/*                                      Basic display driver horizontal   */
/*                                        line draw function              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_generic_polygon_fill                             */
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
static VOID _gx_display_driver_polygon_scan_line_draw(GX_DRAW_CONTEXT *context, GX_POLYGON_EDGE *aet, INT y, INT aet_size)
{
GX_VALUE      index;
INT           x1, x2;
GX_DISPLAY   *display = context -> gx_draw_context_display;
GX_RECTANGLE *clip = context -> gx_draw_context_clip;
GX_COLOR      fill_color;

    fill_color = context -> gx_draw_context_brush.gx_brush_fill_color;

    for (index = 0; index < aet_size - 1; index = (GX_VALUE)(index + (GX_VALUE)2))
    {

        /* Pick up x coordinate value of the pair intersections. */
        x1 = aet[index].gx_polygon_edge_xi;
        x2 = aet[index + 1].gx_polygon_edge_xi;

        if ((context -> gx_draw_context_brush.gx_brush_style & GX_BRUSH_ALIAS) &&
            (x1 != x2))
        {
            if (aet[index].gx_polygon_edge_x_decision &&
                aet[index].gx_polygon_edge_sign > 0)
            {
                x1 += 1;
            }

            if (aet[index + 1].gx_polygon_edge_x_decision &&
                aet[index + 1].gx_polygon_edge_sign < 0)
            {
                x2 -= 1;
            }
        }

        /* Cliping segments x1x2. */
        if (x1 < clip -> gx_rectangle_left)
        {
            x1 = clip -> gx_rectangle_left;
        }

        if (x2 > clip -> gx_rectangle_right)
        {
            x2 = clip -> gx_rectangle_right;
        }

        if (x1 <= x2)
        {
            /* Filling area between pairs of the intersections. */
            display -> gx_display_driver_horizontal_line_draw(context, x1, x2, y, 1, fill_color);
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_polygon_pixelmap_scan_line_draw  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that filling area between pair             */
/*    intersections with pixelmap.                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                           Drawing context                   */
/*    aet                               Pointer to active edge table      */
/*    aet_size                          Active edge table size            */
/*    xpos                              X coord of draw start position    */
/*    y                                 Y coordinate of current scan line */
/*    pixelmap                          Pixelmap to fill                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_horizontal_pixelmap_line_draw]                   */
/*                                      Basic display driver horizontal   */
/*                                        pixelmap line draw function     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_display_driver_generic_polygon_fill                             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed use of memcpy,      */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_polygon_pixelmap_scan_line_draw(GX_DRAW_CONTEXT *context, GX_POLYGON_EDGE *aet, INT aet_size,
                                                               INT xpos, INT y, GX_FILL_PIXELMAP_INFO *info)
{
GX_VALUE              index;
INT                   x1, x2;
GX_DISPLAY           *display = context -> gx_draw_context_display;
GX_RECTANGLE         *clip = context -> gx_draw_context_clip;
GX_FILL_PIXELMAP_INFO old_info;
GX_BOOL               is_drawn = GX_FALSE;

    /* Store the ptr info in case that this line will be drawn several times.*/
    old_info = (*info);

    for (index = 0; index < aet_size - 1; index = (GX_VALUE)(index + (GX_VALUE)2))
    {
        /* if aet_size is bigger than 2, it means there's at least two segment in this line.
           So it must be drawn this line several times and should always be started from the "correct" position. */
        (*info) = old_info;

        /* Pick up x coordinate value of the pair intersections. */
        x1 = aet[index].gx_polygon_edge_xi;
        x2 = aet[index + 1].gx_polygon_edge_xi;

        if ((context -> gx_draw_context_brush.gx_brush_style & GX_BRUSH_ALIAS) &&
            (x1 != x2))
        {
            if (aet[index].gx_polygon_edge_x_decision &&
                aet[index].gx_polygon_edge_sign > 0)
            {
                x1 += 1;
            }

            if (aet[index + 1].gx_polygon_edge_x_decision &&
                aet[index + 1].gx_polygon_edge_sign < 0)
            {
                x2 -= 1;
            }
        }

        /* Cliping segments x1x2. */
        if (x1 < clip -> gx_rectangle_left)
        {
            x1 = clip -> gx_rectangle_left;
        }

        if (x2 > clip -> gx_rectangle_right)
        {
            x2 = clip -> gx_rectangle_right;
        }

        if (x1 > x2)
        {
            continue;
        }

        /* Filling area between pairs of the intersections. */
        info -> x_offset = x1 - xpos;
        display -> gx_display_driver_horizontal_pixelmap_line_draw(context, x1, x2, y, info);
        is_drawn = GX_TRUE;
    }

    if (!is_drawn)
    {
        info -> draw = GX_FALSE;
        display -> gx_display_driver_horizontal_pixelmap_line_draw(context, 0, 0, y, info);
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_polygon_active_edge_table_update_and_restore     */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that updates x coordinate of intersection  */
/*    according to edge gradient, and sort intersections by increase x    */
/*    coordinate value.                                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    aet                               Pointer to active edge table      */
/*    aet_size                          Active edge table size            */
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
/*    _gx_display_driver_generic_polygon_fill                             */
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
static UINT _gx_display_driver_polygon_active_edge_table_update_and_restore(GX_POLYGON_EDGE *aet, INT aet_size)
{
GX_VALUE        index = (GX_VALUE)aet_size, xi, yi;
GX_POLYGON_EDGE temp;

    /* Update xi value. */
    while (index--)
    {
        if (aet[index].gx_polygon_edge_dx == 0)
        {
            continue;
        }

        /* When scan line move one step from top to bottom,
           we need to update x value of active edges with the delta of the edge,
           the following codes equals to x += delta(y). */
        aet[index].gx_polygon_edge_x_decision += aet[index].gx_polygon_edge_dx;

        while (aet[index].gx_polygon_edge_x_decision >= aet[index].gx_polygon_edge_dy)
        {
            aet[index].gx_polygon_edge_xi += aet[index].gx_polygon_edge_sign;
            aet[index].gx_polygon_edge_x_decision -= aet[index].gx_polygon_edge_dy;
        }
    }

    /* Sort with xi value in ascending order. */
    for (xi = 0; xi < aet_size - 1; xi++)
    {
        for (yi = (GX_VALUE)(xi + 1); yi < aet_size; yi++)
        {
            if (aet[xi].gx_polygon_edge_xi > aet[yi].gx_polygon_edge_xi)
            {
                temp = aet[yi];
                aet[yi] = aet[xi];
                aet[xi] = temp;
            }
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_generic_polygon_scan_line_fill   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This fills a polygon.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    num                               Number of points of polygon       */
/*    vertex                            Array of points of the polygon,   */
/*                                        terminated by NULL entry.       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                            Completion status                 */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_line_draw              Draw the specified line           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
VOID _gx_display_driver_generic_polygon_fill(GX_DRAW_CONTEXT *context, GX_POINT *vertex, INT num)
{
INT                    ymin;
INT                    ymax;
INT                    xpos;
INT                    y;
static GX_POLYGON_EDGE net[GX_POLYGON_MAX_EDGE_NUM]; /* new edge table: obtain all edges of the polygon. */
static GX_POLYGON_EDGE aet[GX_POLYGON_MAX_EDGE_NUM]; /* active edge table: obtain all edges that touched by the current scan line. */
GX_BRUSH              *brush;
INT                    aet_size = 0;
INT                    net_size = num;
GX_PIXELMAP           *pixelmap = GX_NULL;
GX_VALUE               format;
GX_FILL_PIXELMAP_INFO  info;
GX_DISPLAY            *display = context -> gx_draw_context_display;
GX_RECTANGLE          *clip = context -> gx_draw_context_clip;

    if (num <= GX_POLYGON_MAX_EDGE_NUM)
    {
        /* Get the top most and bottom most y coordinate of the specified polygon. */
        _gx_display_driver_polygon_min_max_get(vertex, num, &ymin, &ymax, &xpos);

        /* Initiate new edge table. */
        _gx_display_driver_polygon_new_edge_table_init(context, vertex, num, net, &net_size);

        brush = &context -> gx_draw_context_brush;
        if (brush -> gx_brush_style & GX_BRUSH_PIXELMAP_FILL)
        {
            /* Pick up brush pixelmap. */
            pixelmap = brush -> gx_brush_pixelmap;

            if (pixelmap == GX_NULL)
            {
                return;
            }

            if (pixelmap -> gx_pixelmap_height <= 0 ||
                pixelmap -> gx_pixelmap_width <= 0)
            {
                return;
            }

            format = context -> gx_draw_context_display -> gx_display_color_format;
            memset(&info, 0, sizeof(GX_FILL_PIXELMAP_INFO));

            /*Check the pixelmap format is supported in this driver or not.*/
            if (format >= GX_COLOR_FORMAT_24XRGB)
            {
                /*32ARGB FORMAT use 24xrgb driver for now. So this is a special case.*/
                if (pixelmap -> gx_pixelmap_format < GX_COLOR_FORMAT_24XRGB)
                {
                    /* Display driver only support its native format pixelmap. */
                    /* Nothing should be drawn if pixelmap format isn't support. */
                    return;
                }
            }
            else
            {
                if (pixelmap -> gx_pixelmap_format != format)
                {
                    /* Display driver only support its native format pixelmap. */
                    /* Nothing should be drawn if pixelmap format isn't support. */
                    return;
                }
                else if (pixelmap -> gx_pixelmap_format == GX_COLOR_FORMAT_4BIT_GRAY)
                {
                    info.mask = 0xf0;
                }
            }

            info.pixelmap = brush -> gx_brush_pixelmap;
            info.current_pixel_ptr = (GX_UBYTE *)info.pixelmap -> gx_pixelmap_data;

            if (pixelmap -> gx_pixelmap_aux_data_size)
            {
                info.current_aux_ptr = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
            }
        }

        if (ymax > clip -> gx_rectangle_bottom)
        {
            ymax = clip -> gx_rectangle_bottom;
        }

        /* Moving scan line from polygon top to bottom. */
        for (y = ymin; y <= ymax; y++)
        {
            /* This function will check if the scan line comes to the top most y coordinate of any edges in net table.
               If it is true, add it to the aet table. */
            _gx_display_driver_polygon_active_edge_insert(aet, net, y, net_size, &aet_size);

            if (y < clip -> gx_rectangle_top)
            {
                if (pixelmap)
                {
                    /* skip the undraw lines. */
                    info.draw = GX_FALSE;
                    display -> gx_display_driver_horizontal_pixelmap_line_draw(context, 0, 0, y, &info);
                }
            }
            else
            {
                if (pixelmap)
                {
                    info.draw = GX_TRUE;
                    _gx_display_driver_polygon_pixelmap_scan_line_draw(context, aet, aet_size, xpos, y, &info);
                }
                else
                {
                    /* Filling the polygon area that touched by scan line. */
                    _gx_display_driver_polygon_scan_line_draw(context, aet, y, aet_size);
                }
            }
            /* This function will check if the scan line comes to the bottom most y coordinate of any edges in aet table.
               If it is true, remove it from aet table. */
            _gx_display_driver_polygon_non_active_edge_remove(aet, y, &aet_size);

            /* Update x coordinate of intersections between polygon and scan line, and sort them by increase x coordinate value. */
            _gx_display_driver_polygon_active_edge_table_update_and_restore(aet, aet_size);
        }
    }
}

