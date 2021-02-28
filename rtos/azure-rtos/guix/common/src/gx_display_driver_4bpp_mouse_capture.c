
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
/**   Dispaly Management (Dispaly)                                        */
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
/*    _gx_display_driver_4bpp_mouse_capture               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service captures memory under mouse position.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Display control block         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
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
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
#if defined(GX_MOUSE_SUPPORT)
#if !defined(GX_HARDWARE_MOUSE_SUPPORT)
VOID _gx_display_driver_4bpp_mouse_capture(GX_DISPLAY  *display)
{
INT            width;
INT            height;
INT            row;
INT            column;
GX_UBYTE      *put;
GX_UBYTE      *putrow;
GX_UBYTE      *getrow;
GX_UBYTE      *get;
GX_PIXELMAP   *map;
GX_RESOURCE_ID image_id;
GX_RECTANGLE   mouse_rect;
GX_UBYTE       getmask;
GX_UBYTE       putmask = 0xf0;
GX_UBYTE       pixel;
GX_CANVAS     *canvas;

    if (display -> gx_display_mouse.gx_mouse_cursor_info)
    {
        if (display -> gx_display_mouse.gx_mouse_capture_memory)
        {
            canvas = display -> gx_display_mouse.gx_mouse_canvas;
            image_id = display -> gx_display_mouse.gx_mouse_cursor_info -> gx_mouse_cursor_image_id;

            if (image_id && image_id < display -> gx_display_pixelmap_table_size)
            {
                map = display -> gx_display_pixelmap_table[image_id];

                mouse_rect.gx_rectangle_left = display -> gx_display_mouse.gx_mouse_position.gx_point_x;
                mouse_rect.gx_rectangle_top = display -> gx_display_mouse.gx_mouse_position.gx_point_y;
                mouse_rect.gx_rectangle_left = (GX_VALUE)(mouse_rect.gx_rectangle_left - display -> gx_display_mouse.gx_mouse_cursor_info -> gx_mouse_cursor_hotspot_x);
                mouse_rect.gx_rectangle_top = (GX_VALUE)(mouse_rect.gx_rectangle_top - display -> gx_display_mouse.gx_mouse_cursor_info -> gx_mouse_cursor_hotspot_y);

                mouse_rect.gx_rectangle_right = (GX_VALUE)(mouse_rect.gx_rectangle_left + map -> gx_pixelmap_width - 1);
                mouse_rect.gx_rectangle_bottom = (GX_VALUE)(mouse_rect.gx_rectangle_top + map -> gx_pixelmap_height - 1);

                if (mouse_rect.gx_rectangle_left < 0)
                {
                    mouse_rect.gx_rectangle_left = 0;
                }
                if (mouse_rect.gx_rectangle_top < 0)
                {
                    mouse_rect.gx_rectangle_top = 0;
                }
                if (mouse_rect.gx_rectangle_right >= canvas -> gx_canvas_x_resolution)
                {
                    mouse_rect.gx_rectangle_right = (GX_VALUE)(canvas -> gx_canvas_x_resolution - 1);
                }
                if (mouse_rect.gx_rectangle_bottom >= canvas -> gx_canvas_y_resolution)
                {
                    mouse_rect.gx_rectangle_bottom = (GX_VALUE)(canvas -> gx_canvas_y_resolution - 1);
                }
                width = mouse_rect.gx_rectangle_right - mouse_rect.gx_rectangle_left + 1;
                height = mouse_rect.gx_rectangle_bottom - mouse_rect.gx_rectangle_top + 1;
            }
            else
            {
                width = height = 0;
            }

            if (width > 0 && height > 0)
            {
                display -> gx_display_mouse.gx_mouse_rect = mouse_rect;

                getrow = (GX_UBYTE *)canvas -> gx_canvas_memory;
                getrow += ((canvas -> gx_canvas_x_resolution + 1) >> 1) * mouse_rect.gx_rectangle_top;
                getrow += mouse_rect.gx_rectangle_left >> 1;
                putrow = (GX_UBYTE *)display -> gx_display_mouse.gx_mouse_capture_memory;

                for (row = 0; row < height; row++)
                {
                    if (mouse_rect.gx_rectangle_left & 1)
                    {
                        getmask = 0x0f;
                    }
                    else
                    {
                        getmask = 0xf0;
                    }
                    get = getrow;
                    put = putrow;
                    putmask = 0xf0;

                    for (column = 0; column < width; column++)
                    {
                        pixel = (*get) & getmask;
                        if (getmask == 0x0f)
                        {
                            get++;
                            getmask = 0xf0;
                            pixel |= (GX_UBYTE)(pixel << 4);
                        }
                        else
                        {
                            getmask = 0x0f;
                            pixel |= pixel >> 4;
                        }

                        *put &= (GX_UBYTE)(~putmask);
                        *put |= pixel & putmask;
                        putmask >>= 4;
                        if (putmask == 0)
                        {
                            putmask = 0xf0;
                            put++;
                        }
                    }
                    getrow += (canvas -> gx_canvas_x_resolution + 1) >> 1;
                    putrow += (width + 1) >> 1;
                }
            }
            else
            {
                display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_top = 0;
                display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_bottom = -1;
                display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_left = 0;
                display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_right = -1;
            }
        }
    }
}
#endif
#endif

