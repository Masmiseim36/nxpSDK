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
#include "gx_canvas.h"
#include "gx_context.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_drawing_initiate                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function initiates drawing on the specified canvas.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    who                                   Widget control block pointer  */
/*    dirty_area                            Area to draw on               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_define          Define rectangle area.        */
/*    _gx_utility_rectangle_combine         Combine adjacent dirty areas. */
/*    _gx_context_brush_default             Define a brush                */
/*    _gx_system_error_process              Process an error              */
/*    _gx_utility_rectangle_overlap_detect  Detects two rectangles being  */
/*                                            overlap                     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    _gx_multi_line_text_view_draw                                       */
/*    _gx_single_line_text_view_draw                                      */
/*    _gx_system_canvas_refresh                                           */
/*    _gx_widget_block_move                                               */
/*    _gx_widget_children_draw                                            */
/*    _gx_window_client_scroll                                            */
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
UINT _gx_canvas_drawing_initiate(GX_CANVAS *canvas, GX_WIDGET *who, GX_RECTANGLE *dirty_area)
{
GX_WINDOW       *win;
GX_DRAW_CONTEXT *new_context;
GX_DRAW_CONTEXT *current_context = _gx_system_current_draw_context;
GX_DISPLAY      *display = canvas -> gx_canvas_display;

    if (current_context)
    {
        new_context = current_context - 1;

        /* check for exceeding our maximum stack depth */
        if (new_context < _gx_system_draw_context_stack)
        {
            _gx_system_error_process(GX_DRAW_NESTING_EXCEEDED);
            return GX_DRAW_NESTING_EXCEEDED;
        }
    }
    else
    {
        new_context = _gx_system_draw_context_stack_end - 1;
    }


    /* test to see if we have draw to this canvas since last refresh */

    if (canvas -> gx_canvas_draw_count > 0)
    {
        _gx_utility_rectangle_combine(&canvas -> gx_canvas_dirty_area, dirty_area);
    }
    else
    {
        /* initialize the dirty rectangle */
        canvas -> gx_canvas_dirty_area = *dirty_area;
    }

    /* Are we nested?  */
    if (canvas -> gx_canvas_draw_nesting > 0 && current_context)
    {
        /* Yes, clone the previous context.  */
        *new_context =  *current_context;
    }
    else
    {
        /* initialize the new context's brush */
        _gx_context_brush_default(new_context);

        /* save the canvas pointer for any function that needs it */
        new_context -> gx_draw_context_canvas = canvas;

        /* copy some canvas params to the context to speed access by drawing APIs */

        new_context -> gx_draw_context_display = canvas -> gx_canvas_display;
        new_context -> gx_draw_context_memory  = canvas -> gx_canvas_memory;
        new_context -> gx_draw_context_pitch   = canvas -> gx_canvas_x_resolution;
    }

    /* Update canvas draw count and draw nesting.  */

    canvas -> gx_canvas_draw_nesting++;
    canvas -> gx_canvas_draw_count++;

    /* let display driver know if it has notification function */
    if (display -> gx_display_driver_drawing_initiate)
    {
        display -> gx_display_driver_drawing_initiate(display, canvas);
    }

    /* Update the draw context clipping rectangle.  */
    if (who)
    {
        _gx_utility_rectangle_overlap_detect(dirty_area,
                                             &who -> gx_widget_clip, &new_context -> gx_draw_context_dirty);
#if defined(GX_BRUSH_ALPHA_SUPPORT)
        if (who -> gx_widget_style & GX_STYLE_USE_LOCAL_ALPHA)
        {
            new_context -> gx_draw_context_brush.gx_brush_alpha = who -> gx_widget_alpha;
        }
#endif
    }
    else
    {
        new_context -> gx_draw_context_dirty = *dirty_area;
    }

    /* initialize viewports to NULL */
    new_context -> gx_draw_context_view_head = GX_NULL;


    /* pick up the viewport list */

    if (canvas -> gx_canvas_status & GX_CANVAS_SIMPLE)
    {
        new_context -> gx_draw_context_simple_view.gx_view_next = NULL;
        new_context -> gx_draw_context_simple_view.gx_view_rectangle = new_context -> gx_draw_context_dirty;
        new_context -> gx_draw_context_view_head = &new_context -> gx_draw_context_simple_view;
    }
    else
    {
        if (who)
        {
            /* is the root window trying to draw */
            if (who -> gx_widget_type == GX_TYPE_ROOT_WINDOW)
            {
                win = (GX_WINDOW *)who;
                new_context -> gx_draw_context_view_head = win -> gx_window_views;
            }
            else
            {
                while (who -> gx_widget_parent)
                {
                    if (who -> gx_widget_parent -> gx_widget_type == GX_TYPE_ROOT_WINDOW)
                    {
                        if (who -> gx_widget_type >= GX_TYPE_WINDOW &&
                            !(who -> gx_widget_status & GX_STATUS_TRANSPARENT))
                        {
                            /* child windows of the root window get their own view list */
                            win = (GX_WINDOW *)who;
                        }
                        else
                        {
                            /* other widget types just inherit the view list of the root */
                            win = (GX_WINDOW *)who -> gx_widget_parent;
                        }
                        new_context -> gx_draw_context_view_head = win -> gx_window_views;
                        break;
                    }
                    who = who -> gx_widget_parent;
                }
            }

            /* the widget being drawn is no longer marked dirty */
            who -> gx_widget_status &= ~GX_STATUS_DIRTY;
        }
    }

    /* the new context becomes the current context */
    _gx_system_current_draw_context = new_context;

    if (new_context -> gx_draw_context_view_head == GX_NULL)
    {
        /* no viewports for the caller */
        return(GX_NO_VIEWS);
    }
    return(GX_SUCCESS);
}

