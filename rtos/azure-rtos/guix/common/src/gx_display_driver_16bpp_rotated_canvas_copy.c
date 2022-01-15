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
#include "gx_utility.h"



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_16bpp_rotated_canvas_copy        PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Generic 16bpp rotated canvas copy function.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*   canvas                                 The canvas to copy from       */
/*   composite                              The canvas to copy to         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_shift           Move the rectangle            */
/*    _gx_utility_rectangle_overlap_detect  Detect two rectangles being   */
/*                                            overlap to each other       */
/*    memcpy                                Move canvas data              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_16bpp_rotated_canvas_copy(GX_CANVAS *canvas, GX_CANVAS *composite)
{
GX_RECTANGLE dirty;
GX_RECTANGLE overlap;
USHORT      *read;
USHORT      *write;
INT          width;
INT          row;

    dirty.gx_rectangle_left = dirty.gx_rectangle_top = 0;
    dirty.gx_rectangle_right = (GX_VALUE)(canvas -> gx_canvas_x_resolution - (GX_VALUE)1);
    dirty.gx_rectangle_bottom = (GX_VALUE)(canvas -> gx_canvas_y_resolution - (GX_VALUE)1);

    _gx_utility_rectangle_shift(&dirty, canvas -> gx_canvas_display_offset_x, canvas -> gx_canvas_display_offset_y);

    if (_gx_utility_rectangle_overlap_detect(&dirty, &composite -> gx_canvas_dirty_area, &overlap))
    {
        width = overlap.gx_rectangle_bottom - overlap.gx_rectangle_top + 1;

        read = (USHORT *)canvas -> gx_canvas_memory;
        write = (USHORT *)composite -> gx_canvas_memory;

        if (canvas -> gx_canvas_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
        {
            /* index into starting row */
            read += (dirty.gx_rectangle_right - overlap.gx_rectangle_right) * canvas -> gx_canvas_y_resolution;

            /* index into pixel */
            read += overlap.gx_rectangle_top - dirty.gx_rectangle_top;

            /* calculate the write pointer */
            write += (composite -> gx_canvas_x_resolution - overlap.gx_rectangle_right - 1) * composite -> gx_canvas_y_resolution;
            write += overlap.gx_rectangle_top;
        }
        else
        {
            /* index into starting row */
            read += (overlap.gx_rectangle_left - dirty.gx_rectangle_left) * canvas -> gx_canvas_y_resolution;

            /* index into pixel */
            read += dirty.gx_rectangle_bottom - overlap.gx_rectangle_bottom;

            /* calculate the write pointer */
            write += overlap.gx_rectangle_left * composite -> gx_canvas_y_resolution;
            write += (composite -> gx_canvas_y_resolution - overlap.gx_rectangle_bottom - 1);
        }

        for (row = overlap.gx_rectangle_left; row <= overlap.gx_rectangle_right; row++)
        {
            memcpy(write, read, (size_t)(width * 2)); /* Use case of memcpy is verified. */

            write += composite -> gx_canvas_y_resolution;
            read += canvas -> gx_canvas_y_resolution;
        }
    }
}

