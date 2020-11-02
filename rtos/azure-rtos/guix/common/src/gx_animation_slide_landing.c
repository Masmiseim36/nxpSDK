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
/**   Animation Management (Animation)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_system.h"
#include "gx_animation.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_animation_slide_landing                         PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function shifts the sliding screens one step to target         */
/*      position.                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    animation                             Pointer to animation control  */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_detach                     Detach widget from its parent */
/*    _gx_widget_shift                      Change widget's position      */
/*    _gx_system_timer_stop                 Stop a timer for a widget     */
/*    _gx_animation_complete_event_send     Send a complete event to      */
/*                                            widget's parent             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_animation_drag_event_check                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*                                                                        */
/**************************************************************************/
UINT  _gx_animation_slide_landing(GX_ANIMATION *animation)
{
GX_ANIMATION_INFO *info = &animation -> gx_animation_info;
GX_WIDGET         *parent;
GX_WIDGET         *target_1 = GX_NULL;
GX_WIDGET         *target_2 = GX_NULL;
INT                x_shift = 0;
INT                y_shift = 0;
GX_RECTANGLE       target_size;

    parent = animation -> gx_animation_info.gx_animation_parent;

    /* Get current animation targets. */
    if (animation -> gx_animation_slide_target_index_1 >= 0)
    {
        target_1 = info -> gx_animation_slide_screen_list[animation -> gx_animation_slide_target_index_1];
    }
    else
    {
        /* Sliding animation should have 1 target at least, this should not happen. */

        /* Stop landing timer. */
        _gx_system_timer_stop(parent, GX_ANIMATION_SLIDE_TIMER);

        return GX_FAILURE;
    }

    if (animation -> gx_animation_slide_target_index_2 >= 0)
    {
        target_2 = info -> gx_animation_slide_screen_list[animation -> gx_animation_slide_target_index_2];
    }

    if (target_2)
    {
        target_size = target_2 -> gx_widget_size;
    }
    else
    {
        target_size = target_1 -> gx_widget_size;
    }

    if (info -> gx_animation_style & GX_ANIMATION_EASING_FUNC_MASK)
    {
        info -> gx_animation_steps = (GX_UBYTE)(info -> gx_animation_steps - 1);

        switch (animation -> gx_animation_slide_direction)
        {
        case GX_ANIMATION_SLIDE_LEFT:
        case GX_ANIMATION_SLIDE_RIGHT:
            _gx_utility_easing_function_calculate(info -> gx_animation_style,
                                                  info -> gx_animation_start_position.gx_point_x,
                                                  parent -> gx_widget_size.gx_rectangle_left,
                                                  animation -> gx_animation_total_steps - info -> gx_animation_steps,
                                                  animation -> gx_animation_total_steps, &x_shift);

            x_shift -= target_size.gx_rectangle_left;
            break;

        default:
            _gx_utility_easing_function_calculate(info -> gx_animation_style,
                                                  info -> gx_animation_start_position.gx_point_y,
                                                  parent -> gx_widget_size.gx_rectangle_top,
                                                  animation -> gx_animation_total_steps - info -> gx_animation_steps,
                                                  animation -> gx_animation_total_steps, &y_shift);

            y_shift -= target_size.gx_rectangle_top;
            break;
        }
    }
    else
    {
        /* Get landing shift value according to slide direction. */
        switch (animation -> gx_animation_slide_direction)
        {
        case GX_ANIMATION_SLIDE_LEFT:
            x_shift = -animation -> gx_animation_landing_speed;
            break;

        case GX_ANIMATION_SLIDE_RIGHT:
            x_shift = animation -> gx_animation_landing_speed;
            break;

        case GX_ANIMATION_SLIDE_UP:
            y_shift = -animation -> gx_animation_landing_speed;
            break;

        default:
            y_shift = animation -> gx_animation_landing_speed;
            break;
        }
    }

    if (((info -> gx_animation_style & GX_ANIMATION_EASING_FUNC_MASK) && (info -> gx_animation_steps != 0)) ||
        ((x_shift < 0) && ((GX_VALUE)(target_size.gx_rectangle_left + x_shift) > parent -> gx_widget_size.gx_rectangle_left)) ||
        ((x_shift > 0) && ((GX_VALUE)(target_size.gx_rectangle_left + x_shift) < parent -> gx_widget_size.gx_rectangle_left)) ||
        ((y_shift < 0) && ((GX_VALUE)(target_size.gx_rectangle_top + y_shift) > parent -> gx_widget_size.gx_rectangle_top)) ||
        ((y_shift > 0) && ((GX_VALUE)(target_size.gx_rectangle_top + y_shift) < parent -> gx_widget_size.gx_rectangle_top)))
    {
        /* Shift animation targets one step toward target position.  */
        _gx_widget_shift(target_1, (GX_VALUE)x_shift, (GX_VALUE)y_shift, GX_TRUE);

        if (target_2)
        {
            _gx_widget_shift(target_2, (GX_VALUE)x_shift, (GX_VALUE)y_shift, GX_TRUE);
        }
    }
    else
    {
        /* Shift animation targets to the target position. */

        /* Stop landing timer. */
        _gx_system_timer_stop(parent, GX_ANIMATION_SLIDE_TIMER);

        /* Calculate the distance from current postion to final position. */
        if (info -> gx_animation_style & GX_ANIMATION_VERTICAL)
        {
            y_shift = parent -> gx_widget_size.gx_rectangle_top - target_size.gx_rectangle_top;
        }
        else
        {
            x_shift = parent -> gx_widget_size.gx_rectangle_left - target_size.gx_rectangle_left;
        }

        if (target_2)
        {
            /* Detach the first target. */
            _gx_widget_detach(target_1);

            /* Move the second target to its final position. */
            _gx_widget_shift(target_2, (GX_VALUE)x_shift, (GX_VALUE)y_shift, GX_TRUE);
        }
        else
        {
            /* No second target, just move the first target to its final position. */
            _gx_widget_shift(target_1, (GX_VALUE)x_shift, (GX_VALUE)y_shift, GX_TRUE);
        }

        /* Reset animation information. */
        animation -> gx_animation_slide_target_index_1 = -1;
        animation -> gx_animation_slide_target_index_2 = -1;
        animation -> gx_animation_status = GX_ANIMATION_IDLE;

        /* Send animation complete event.  */
        _gx_animation_complete_event_send(animation);
    }

    /* Return completion status code. */
    return(GX_SUCCESS);
}

