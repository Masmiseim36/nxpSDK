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
#include "gx_slider.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_pixelmap_slider_event_process                  PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks errors in the pixelmap slider event process    */
/*    function.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    slider                                Slider's widget control block */
/*    event_ptr                             Incoming event to process     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_event_process              Default widget event process  */
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
/*                                                                        */
/**************************************************************************/
UINT  _gxe_pixelmap_slider_event_process(GX_PIXELMAP_SLIDER *slider, GX_EVENT *event_ptr)
{

UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check error for valid pointer.  */
    if ((slider == GX_NULL) || (event_ptr == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid widget. */
    if (slider->gx_widget_type == 0)
    {
        return(GX_INVALID_WIDGET);
    }

    status = _gx_pixelmap_slider_event_process(slider, event_ptr);

    /* Return completion status.  */
    return(status);
}

