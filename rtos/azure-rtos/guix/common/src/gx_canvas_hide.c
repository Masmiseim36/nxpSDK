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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_hide                                     PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function makes a canvas invisible                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_canvas_dirty_mark                 Set the canvas dirty flag     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application                                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*                                                                        */
/**************************************************************************/
UINT  _gx_canvas_hide(GX_CANVAS *canvas)
{
VOID (*hide_function)(INT layer);

    /* change canvas status flag. */
    canvas -> gx_canvas_status &= ~(UINT)(GX_CANVAS_VISIBLE);

    if (canvas -> gx_canvas_hardware_layer >= 0)
    {
        hide_function = canvas -> gx_canvas_display -> gx_display_layer_services -> gx_display_layer_hide;

        if (hide_function)
        {
            hide_function(canvas -> gx_canvas_hardware_layer);
            return(GX_SUCCESS);
        }
    }

    /* mark the canvas dirty so that it get refreshed */
    _gx_canvas_dirty_mark(canvas, GX_NULL);

    /* Return successful status.  */
    return(GX_SUCCESS);
}

