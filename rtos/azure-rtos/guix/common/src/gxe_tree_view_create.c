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
/**   Tree View Management (Tree View)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_tree_view.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_tree_view_create                               PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in tree view create function call.  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    tree                                  Pointer to the tree view      */
/*                                            control block               */
/*    name                                  Name of the tree view         */
/*    parent                                Parent widget control block   */
/*    style                                 Style of the widget           */
/*    tree_id                               Application-defined ID of     */
/*                                          the tree view                 */
/*    size                                  Tree view size                */
/*    control_block_size                    Control block size            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_tree_view_create                  Actual tree view create       */
/*                                            function call               */
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
UINT  _gxe_tree_view_create(GX_TREE_VIEW *tree, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                            ULONG style, USHORT tree_id, GX_CONST GX_RECTANGLE *size, UINT control_block_size)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if ((!tree) || (!size))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for widget already created.  */
    if (tree -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    /* Check for invalid control block size. */
    if (control_block_size != sizeof(GX_TREE_VIEW))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for invalid parent widget. */
    if (parent && (parent -> gx_widget_type == 0))
    {
        return(GX_INVALID_WIDGET);
    }

    /* Call actual tree view create function.  */
    status = _gx_tree_view_create(tree, name, parent, style, tree_id, size);

    return(status);
}

