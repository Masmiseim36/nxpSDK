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
#include "gx_icon.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_icon_create                                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the icon create function call.   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    icon                                  Pointer to icon control block */
/*    name                                  Logical name of icon widget   */
/*    parent                                Pointer to the parent widget  */
/*    pixelmap_id                           Resource ID of pixelmap       */
/*    style                                 Style of icon                 */
/*    icon_id                               Application-definedID of icon */
/*    x                                     Starting x-coordinate position*/
/*    y                                     Starting y-coordinate position*/
/*    button_control_block_size             Size of the button control    */
/*                                            block                       */
/*                                                                        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_icon_create                       Actual icon create function   */
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
UINT  _gxe_icon_create(GX_ICON *icon, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                       GX_RESOURCE_ID pixelmap_id, ULONG style, USHORT icon_id,
                       GX_VALUE x, GX_VALUE y, UINT icon_control_block_size)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if (icon == GX_NULL)
    {
        return(GX_PTR_ERROR);
    }

    if (icon_control_block_size != sizeof(GX_ICON))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for widget already created.  */
    if (icon -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    /* Call the actual icon create function.  */
    status = _gx_icon_create(icon, name, parent, pixelmap_id, style, icon_id, x, y);

    /* Return completion status.  */
    return status;
}

