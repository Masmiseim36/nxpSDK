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
/**   Slider Management (Slider)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_button.h"
#include "gx_slider.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_slider_event_process                            PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service processes a slider event.                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Slider widget control block   */
/*    event                                 Pointer to event process      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_slider_value_calculate            Calculate the slider value    */
/*    _gx_widget_event_process              Default widget event process  */
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
UINT  _gx_slider_event_process(GX_SLIDER *slider, GX_EVENT *event_ptr)
{
UINT status;
INT  pos;

    /* Default status to success.  */
    status =  GX_SUCCESS;

    /* Process relative to the type of event.  */
    switch (event_ptr -> gx_event_type)
    {
    case GX_EVENT_PEN_DOWN:

        /* Set style for pen down.  */
        slider -> gx_widget_style |=  GX_STYLE_BUTTON_PUSHED;

        if (slider -> gx_widget_style & GX_STYLE_SLIDER_VERTICAL)
        {
            pos = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_y;
        }
        else
        {
            pos = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x;
        }

        _gx_slider_value_calculate(slider, &slider -> gx_slider_info, pos);
        break;

    case GX_EVENT_PEN_UP:

        /* Set style for pen down.  */
        slider -> gx_widget_style &=  ~GX_STYLE_BUTTON_PUSHED;
        break;

    case GX_EVENT_PEN_DRAG:
        /* move my needle */
        if (slider -> gx_widget_style &  GX_STYLE_BUTTON_PUSHED)
        {
            if (slider -> gx_widget_style & GX_STYLE_SLIDER_VERTICAL)
            {
                pos = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_y;
            }
            else
            {
                pos = event_ptr -> gx_event_payload.gx_event_pointdata.gx_point_x;
            }
            _gx_slider_value_calculate(slider, &slider -> gx_slider_info, pos);
        }
        break;

    default:

        /* Call the widget default processing.  */
        status =  _gx_widget_event_process((GX_WIDGET *)slider, event_ptr);
    }

    /* Return completion status.  */
    return(status);
}

