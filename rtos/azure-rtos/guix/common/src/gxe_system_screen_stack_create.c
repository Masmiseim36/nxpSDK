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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_system_screen_stack_create                      ORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks errors in the system screen stack create       */
/*    function.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    memory                                Memory address at which to    */
/*                                            create system screen stack. */
/*    size                                  Memory size in bytes.         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_screen_stack_create        The actual screen stack       */
/*                                            create routine              */
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
UINT  _gxe_system_screen_stack_create(GX_WIDGET **memory, INT size, UINT control_block_size)
{
UINT status;

    /* Check for invalid input pointers.  */
    if ((memory == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid value.  */
    if (size <= 0)
    {
        return(GX_INVALID_VALUE);
    }

    /* Check for invalid control block size. */
    if (control_block_size != sizeof(GX_SCREEN_STACK_CONTROL))
    {
        return(GX_INVALID_SIZE);
    }

    /* Call actual screen stack create function.  */
    status = _gx_system_screen_stack_create(memory, size);

    /* Return the error status from screen stack create.  */
    return(status);
}

