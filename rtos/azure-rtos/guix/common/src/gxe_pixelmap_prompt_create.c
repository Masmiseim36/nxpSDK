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
/**   Prompt Management (Prompt)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_prompt.h"
#include "gx_system.h"
#include "gx_pixelmap_prompt.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_pixelmap_prompt_create                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks errors in the pixelmap prompt create function. */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    prompt                                Prompt control block          */
/*    name                                  Name of prompt                */
/*    parent                                Parent widget control block   */
/*    text_id                               Resource string id            */
/*    left_id                               pixelmap id for left end      */
/*    fill_id                               pixelmap id for fill area     */
/*    style                                 Style of checkbox             */
/*    prompt_id                             Application-defined ID of     */
/*                                            pixelmap prompt             */
/*    size                                  Prompt size                   */
/*    pixelmap_prompt_control_block_size    Size of the pixelmap prompt   */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_pixelmap_prompt_create            The actual function           */
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
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _gxe_pixelmap_prompt_create(GX_PIXELMAP_PROMPT *prompt,
                                  GX_CONST GX_CHAR *name,
                                  GX_WIDGET *parent, GX_RESOURCE_ID text_id,
                                  GX_RESOURCE_ID fill_id, ULONG style,
                                  USHORT prompt_id,
                                  GX_CONST GX_RECTANGLE *size,
                                  UINT pixelmap_prompt_control_block_size)
{

UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if ((prompt == GX_NULL) || (size == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid widget control block size. */
    if (pixelmap_prompt_control_block_size != sizeof(GX_PIXELMAP_PROMPT))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for widget already created.  */
    if (prompt -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    status = _gx_pixelmap_prompt_create(prompt, name, parent, text_id, fill_id, style, prompt_id, size);
    return(status);
}

