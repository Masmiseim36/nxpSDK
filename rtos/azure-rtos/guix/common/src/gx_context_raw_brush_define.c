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
/**   Context Management (Context)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_context.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_context_raw_brush_define                        PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Set the brush of the current dispaly context.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    line_color                            Color of line                 */
/*    fill_color                            Color of fill                 */
/*    style                                 Style of brush                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_brush_define                      Define the brush              */
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
UINT _gx_context_raw_brush_define(GX_COLOR line_color, GX_COLOR fill_color, UINT style)
{
GX_DISPLAY      *display;
GX_BRUSH        *brush;
GX_DRAW_CONTEXT *context = _gx_system_current_draw_context;

    display = context -> gx_draw_context_display;

    /* Set the brush.  */
    brush = &context -> gx_draw_context_brush;
    _gx_brush_define(brush,
                     display -> gx_display_driver_native_color_get(display, line_color),
                     display -> gx_display_driver_native_color_get(display, fill_color), style);

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

