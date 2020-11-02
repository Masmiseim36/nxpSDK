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
#include "gx_system.h"
#include "gx_display.h"
#include "gx_widget.h"
#include "gx_prompt.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_prompt_text_id_set                              PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the text of a prompt widget.                      */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    prompt                                Pointer to prompt widget      */
/*                                            control block               */
/*    string_id                             Resource ID of the string     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_mark                 Mark this prompt as dirty     */
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
UINT  _gx_prompt_text_id_set(GX_PROMPT *prompt, GX_RESOURCE_ID string_id)
{
    prompt -> gx_prompt_text_id = string_id;

    /* If text id is used, clear prompt string to avoid confuse. */
    if (prompt -> gx_widget_style & GX_STYLE_TEXT_COPY)
    {
        if (prompt -> gx_prompt_string.gx_string_ptr)
        {
            _gx_system_memory_free((void *)(prompt -> gx_prompt_string.gx_string_ptr));
        }
    }

    prompt -> gx_prompt_string.gx_string_ptr = GX_NULL;
    prompt -> gx_prompt_string.gx_string_length = 0;

    if (prompt -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        _gx_system_dirty_mark((GX_WIDGET *)prompt);
    }

    return(GX_SUCCESS);
}

