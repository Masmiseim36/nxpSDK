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
/**   Progress Bar Management (Radial Progress Bar)                       */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_widget.h"
#include "gx_radial_progress_bar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_progress_bar_value_set                   PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the radial progress bar value.                    */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    progress_bar                          Radial Progress Bar control   */
/*                                            block                       */
/*    new_value                             New progress bar value        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_mark                 Mark the widget dirty         */
/*    _gx_widget_event_generate             Generate an event to the      */
/*                                            GUIX system                 */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*                                                                        */
/**************************************************************************/
UINT _gx_radial_progress_bar_value_set(GX_RADIAL_PROGRESS_BAR *progress_bar, GX_VALUE new_value)
{
GX_RADIAL_PROGRESS_BAR_INFO *info;

    info = &progress_bar -> gx_radial_progress_bar_info;

    if (new_value < -360)
    {
        new_value = -360;
    }
    else if (new_value > 360)
    {
        new_value = 360;
    }

    if (new_value != info -> gx_radial_progress_bar_info_current_val)
    {
        /* Set progress bar value */
        info -> gx_radial_progress_bar_info_current_val = new_value;

        /* Mark radial progress bar widget area as dirty.  */
        if (progress_bar -> gx_widget_status & GX_STATUS_VISIBLE)
        {
            _gx_system_dirty_mark((GX_WIDGET *)progress_bar);
        }

        /* notify my parent of my new value */
        _gx_widget_event_generate((GX_WIDGET *)progress_bar, GX_EVENT_PROGRESS_VALUE, new_value);
    }

    return(GX_SUCCESS);
}

