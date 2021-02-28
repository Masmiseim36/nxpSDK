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


#include "gx_api.h"
#include "gx_display.h"

#define ALPHAVAL(_c) (GX_UBYTE)((_c) >> 24)
#define REDVAL(_c)   (GX_UBYTE)((_c) >> 16)
#define GREENVAL(_c) (GX_UBYTE)((_c) >> 8)
#define BLUEVAL(_c)  (GX_UBYTE)(_c)

#define ASSEMBLECOLOR(_a, _r, _g, _b) \
    (((_a) << 24)   |                 \
     ((_r) << 16)   |                 \
     ((_g) << 8)    |                 \
     (_b))


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_32argb_pixel_blend               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Pixel blend function for 32argb color format.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    x                                     X coordinate                  */
/*    y                                     Y coordinate                  */
/*    color                                 Color of line to write        */
/*    alpha                                 blending value 0 to 255       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
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
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_32argb_pixel_blend(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR fcolor, GX_UBYTE alpha)
{

GX_UBYTE falpha, fred, fgreen, fblue;
GX_UBYTE balpha, bred, bgreen, bblue;
GX_UBYTE inv_alpha;
ULONG    bcolor;
ULONG   *put;
INT      combined_alpha;


    falpha = ALPHAVAL(fcolor);
    combined_alpha = falpha * alpha;
    combined_alpha /= 255;

    /* Is the pixel non-transparent? */
    if (combined_alpha > 0)
    {
        /* calculate address of pixel */
        put = (ULONG *)context -> gx_draw_context_memory;
        put += context -> gx_draw_context_pitch * y;
        put += x;

        /* No need to blend if alpha value is 255. */
        if (combined_alpha == 255)
        {
            *put = (ULONG)fcolor;
            return;
        }

        /* split foreground into alpha, red, green, and blue components */

        fred = REDVAL(fcolor);
        fgreen = GREENVAL(fcolor);
        fblue = BLUEVAL(fcolor);

        /* read background color */
        bcolor = *put;

        /* split background color into red, green, and blue components */
        balpha = ALPHAVAL(bcolor);
        bred = REDVAL(bcolor);
        bgreen = GREENVAL(bcolor);
        bblue = BLUEVAL(bcolor);

        /* background alpha is inverse of foreground alpha */
        inv_alpha = (GX_UBYTE)(256 - combined_alpha);

        /* blend foreground and background, each color channel */
        falpha = (GX_UBYTE)(((balpha * inv_alpha) + (falpha * combined_alpha)) >> 8);
        fred = (GX_UBYTE)(((bred * inv_alpha) + (fred * combined_alpha)) >> 8);
        fgreen = (GX_UBYTE)(((bgreen * inv_alpha) + (fgreen * combined_alpha)) >> 8);
        fblue = (GX_UBYTE)(((bblue * inv_alpha) + (fblue * combined_alpha)) >> 8);

        /* re-assemble into 16-bit color and write it out */
        *put = (ULONG)ASSEMBLECOLOR(falpha, fred, fgreen, fblue);
    }
}

