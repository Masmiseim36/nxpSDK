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
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_332rgb_native_color_get          PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    8-bit 332rgb format display driver color conversion routine         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               The associated display        */
/*    rawcolor                              Raw Color value to be         */
/*                                            converted                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    GX_COLOR                              Color value in the screen     */
/*                                            color format                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
/*                                                                        */
/**************************************************************************/
GX_COLOR _gx_display_driver_332rgb_native_color_get(GX_DISPLAY *display, GX_COLOR rawcolor)
{
GX_COLOR native;


    GX_PARAMETER_NOT_USED(display);

    native = ((rawcolor & 0xe00000) >> 16) | ((rawcolor & 0x00e000) >> 11) | ((rawcolor & 0x0000c0) >> 6);
    return native;
}

