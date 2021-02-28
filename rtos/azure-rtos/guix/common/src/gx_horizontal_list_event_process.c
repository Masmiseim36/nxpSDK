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
/**   Horizontal List (List)                                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_system.h"
#include "gx_scrollbar.h"

#define GX_FLICK_TIMER 1001

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_horizontal_list_event_process                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service processes an event for the horizontal list.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    list                                  horizontal list widget control*/
/*                                            block                       */
/*    event_ptr                             Pointer to event to process   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_horizontal_list_selected_set      Set the list entry at the     */
/*                                            current list index          */
/*    _gx_widget_find                       Retrieve the height of the    */
/*                                            widget                      */
/*    _gx_horizontal_list_children_position                               */
/*                                          Position the children for the */
/*                                            horizontal list             */
/*    _gx_first_client_child_get            Get the first client child    */
/*    _gx_horizontal_list_scroll            Move up or down the scrollbar */
/*    _gx_horizontal_list_slide_back_check  Check the sliding back of     */
/*                                          the scrollbar                 */
/*    _gx_window_scrollbar_find             Assign a background wallpaper */
/*                                          to a GX_WINDOW object         */
/*    _gx_scrollbar_reset                   Calculate new scrollbar value */
/*    _gx_system_timer_start                Allocate a free timer and     */
/*                                          activates it                  */
/*    _gx_system_top_widget_find            Find top widget under pen     */
/*    _gx_system_timer_stop                 Stop an active GUIX timer     */
/*    _gx_window_event_process              Process events for the        */
/*                                          specified window              */
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
UINT  _gx_horizontal_list_event_process(GX_HORIZONTAL_LIST *list, GX_EVENT *event_ptr)
{
UINT          status = GX_SUCCESS;
GX_WIDGET    *widget = (GX_WIDGET *)list;
GX_SCROLLBAR *pScroll;
INT           snap_dist;
INT           timer_id;
GX_WIDGET    *child;
INT           list_width;
INT           widget_width;
INT           new_pen_index;
GX_WIDGET   **stackptr;
GX_EVENT      input_release_event;

    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_SHOW:

        /* show the children before attempting to position them, because child
           widgets often do not know their size until shown
         */

        status = _gx_window_event_process((GX_WINDOW *)list, event_ptr);

        if (!list -> gx_horizontal_list_child_count)
        {
            _gx_horizontal_list_children_position(list);
        }

        _gx_window_scrollbar_find((GX_WINDOW *)list, GX_TYPE_HORIZONTAL_SCROLL, &pScroll);

        if (pScroll)
        {
            _gx_scrollbar_reset(pScroll, GX_NULL);
        }
        break;

    case GX_EVENT_FOCUS_NEXT:
        if (list -> gx_horizontal_list_selected < list -> gx_horizontal_list_total_columns - 1)
        {
            _gx_horizontal_list_selected_set(list, list -> gx_horizontal_list_selected + 1);
        }
        break;

    case GX_EVENT_FOCUS_PREVIOUS:
        if (list -> gx_horizontal_list_selected > 0)
        {
            _gx_horizontal_list_selected_set(list, list -> gx_horizontal_list_selected - 1);
        }
        break;

    case GX_EVENT_HORIZONTAL_SCROLL:
        _gx_horizontal_list_scroll(list, event_ptr -> gx_event_payload.gx_event_intdata[1] - event_ptr -> gx_event_payload.gx_event_intdata[0]);
        return 0;

    case GX_EVENT_PEN_DOWN:
        _gx_system_input_capture(widget);
        list -> gx_window_move_start = event_ptr -> gx_event_payload.gx_event_pointdata;

        /* determine which child widget has been selected for future list select event */
        child = _gx_system_top_widget_find((GX_WIDGET *)list, event_ptr -> gx_event_payload.gx_event_pointdata, GX_STATUS_SELECTABLE);

        while (child && child -> gx_widget_parent != (GX_WIDGET *)list)
        {
            child = child -> gx_widget_parent;
        }

        if (child)
        {
            list -> gx_horizontal_list_pen_index = list -> gx_horizontal_list_top_index + _gx_widget_client_index_get(widget, child);
            if (list -> gx_horizontal_list_pen_index >= list -> gx_horizontal_list_total_columns)
            {
                list -> gx_horizontal_list_pen_index -= list -> gx_horizontal_list_total_columns;
            }
        }
        break;

    case GX_EVENT_PEN_UP:
        if (list -> gx_widget_status & GX_STATUS_OWNS_INPUT)
        {
            _gx_system_input_release(widget);

            list_width = list -> gx_horizontal_list_child_count * list -> gx_horizontal_list_child_width;
            widget_width = list -> gx_window_client.gx_rectangle_right - list -> gx_window_client.gx_rectangle_left + 1;

            if (list_width > widget_width)
            {
                _gx_horizontal_list_slide_back_check(list);
            }

            if (list -> gx_horizontal_list_pen_index >= 0 && list -> gx_horizontal_list_snap_back_distance == 0)
            {
                /* test to see if pen-up is over same child widget as pen-down */
                child = _gx_system_top_widget_find((GX_WIDGET *)list, event_ptr -> gx_event_payload.gx_event_pointdata, GX_STATUS_SELECTABLE);
                while (child && child -> gx_widget_parent != (GX_WIDGET *)list)
                {
                    child = child -> gx_widget_parent;
                }

                if (child)
                {
                    new_pen_index = list -> gx_horizontal_list_top_index + _gx_widget_client_index_get(widget, child);
                    if (new_pen_index >= list -> gx_horizontal_list_total_columns)
                    {
                        new_pen_index -= list -> gx_horizontal_list_total_columns;
                    }
                    if (new_pen_index == list -> gx_horizontal_list_pen_index)
                    {
                        _gx_horizontal_list_selected_set(list, list -> gx_horizontal_list_pen_index);
                    }
                }
            }
        }
        else
        {
            _gx_widget_event_to_parent(widget, event_ptr);
        }
        break;

    case GX_EVENT_PEN_DRAG:
        if ((widget -> gx_widget_status & GX_STATUS_OWNS_INPUT) &&
            (event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x - list -> gx_window_move_start.gx_point_x) != 0)
        {
            list_width = list -> gx_horizontal_list_child_count * list -> gx_horizontal_list_child_width;
            widget_width = list -> gx_window_client.gx_rectangle_right - list -> gx_window_client.gx_rectangle_left + 1;

            if (list_width > widget_width)
            {
                /* Start sliding, remove other widgets from input capture stack.  */
                stackptr = _gx_system_input_capture_stack;
                memset(&input_release_event, 0, sizeof(GX_EVENT));
                input_release_event.gx_event_type = GX_EVENT_INPUT_RELEASE;

                while (*stackptr)
                {
                    if (*stackptr != widget)
                    {
                        input_release_event.gx_event_target = *stackptr;
                        _gx_system_event_send(&input_release_event);
                    }
                    stackptr++;
                }

                _gx_horizontal_list_scroll(list,
                                           event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x -
                                           list -> gx_window_move_start.gx_point_x);
                _gx_window_scrollbar_find((GX_WINDOW *)list, GX_TYPE_HORIZONTAL_SCROLL, &pScroll);

                if (pScroll)
                {
                    _gx_scrollbar_reset(pScroll, GX_NULL);
                }
                list -> gx_window_move_start = event_ptr -> gx_event_payload.gx_event_pointdata;
                list -> gx_horizontal_list_pen_index = -1;
            }
        }
        else
        {
            _gx_widget_event_to_parent(widget, event_ptr);
        }
        break;

    case GX_EVENT_HORIZONTAL_FLICK:
        list_width = list -> gx_horizontal_list_child_count * list -> gx_horizontal_list_child_width;
        widget_width = list -> gx_window_client.gx_rectangle_right - list -> gx_window_client.gx_rectangle_left + 1;

        if (list_width > widget_width)
        {
            list -> gx_horizontal_list_snap_back_distance = (GX_VALUE)(GX_FIXED_VAL_TO_INT(event_ptr -> gx_event_payload.gx_event_intdata[0]) * 8);
            _gx_system_timer_start((GX_WIDGET *)list, GX_FLICK_TIMER, 1, 1);
        }
        list -> gx_horizontal_list_pen_index = -1;
        break;

    case GX_EVENT_TIMER:
        timer_id = (INT)(event_ptr -> gx_event_payload.gx_event_timer_id);

        if (timer_id == GX_FLICK_TIMER || timer_id == GX_SNAP_TIMER)
        {
            if (GX_ABS(list -> gx_horizontal_list_snap_back_distance) < GX_ABS(list -> gx_horizontal_list_child_width) / 3)
            {
                _gx_system_timer_stop(widget, event_ptr -> gx_event_payload.gx_event_timer_id);
                _gx_horizontal_list_scroll(list, list -> gx_horizontal_list_snap_back_distance);

                if (event_ptr -> gx_event_payload.gx_event_timer_id == GX_FLICK_TIMER)
                {
                    _gx_horizontal_list_slide_back_check(list);
                }
            }
            else
            {
                snap_dist = (list -> gx_horizontal_list_snap_back_distance) / 5;
                list -> gx_horizontal_list_snap_back_distance = (GX_VALUE)(list -> gx_horizontal_list_snap_back_distance - snap_dist);
                _gx_horizontal_list_scroll(list, snap_dist);
            }
            _gx_window_scrollbar_find((GX_WINDOW *)list, GX_TYPE_HORIZONTAL_SCROLL, &pScroll);

            if (pScroll)
            {
                _gx_scrollbar_reset(pScroll, GX_NULL);
            }
        }
        else
        {
            status = _gx_window_event_process((GX_WINDOW *)list, event_ptr);
        }
        break;

    default:
        status = _gx_window_event_process((GX_WINDOW *)list, event_ptr);
        break;
    }
    return status;
}

