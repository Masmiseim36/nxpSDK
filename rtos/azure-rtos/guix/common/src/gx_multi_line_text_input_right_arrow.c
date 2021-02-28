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
/**   Multi Line Text Input Management (Multi Line Text Input)            */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_window.h"
#include "gx_scrollbar.h"
#include "gx_multi_line_text_input.h"
#include "gx_multi_line_text_view.h"
#include "gx_text_input_cursor.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_right_arrow               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function move the cursor to right by one pixel.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                            Multi line text input         */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_text_input_cursor_dirty_rectangle_get                           */
/*                                          Get cursor rectangle          */
/*    _gx_multi_line_text_input_cursor_pos_update                         */
/*                                          Calculate cursor position     */
/*                                            according to insert index   */
/*    _gx_system_dirty_partial_add          Add one dirty area to         */
/*                                            dirty list                  */
/*    _gx_system_dirty_mark                 Mark widget area dirty        */
/*    _gx_utility_utf8_string_character_get Parses utf8 string to         */
/*                                            multi-byte glyph            */
/*    _gx_utility_string_length_check       Test string length            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_input_keydown_process                           */
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
UINT _gx_multi_line_text_input_right_arrow(GX_MULTI_LINE_TEXT_INPUT *text_input)
{
GX_TEXT_INPUT_CURSOR *cursor_ptr = &text_input -> gx_multi_line_text_input_cursor_instance;
UINT                  index;
GX_RECTANGLE          cur_rect;
INT                   shift;
UINT                  glyph_len = 1;
GX_STRING            *view_text;
UINT                  start_mark = text_input -> gx_multi_line_text_input_start_mark;
UINT                  end_mark = text_input -> gx_multi_line_text_input_end_mark;
GX_POINT              end_pos;
#ifdef GX_UTF8_SUPPORT
GX_STRING             string;
#endif

    if (start_mark != end_mark)
    {
        end_pos = cursor_ptr -> gx_text_input_cursor_pos;

        if (start_mark < end_mark)
        {
            _gx_multi_line_text_input_cursor_visible(text_input);

            /* Get highlight area. */
            _gx_multi_line_text_input_highlight_rectangle_get(text_input, &cur_rect);
        }
        else
        {
            text_input -> gx_multi_line_text_input_text_insert_position = start_mark;

            shift = text_input -> gx_multi_line_text_view_text_scroll_shift;

            /* Recalculate cursor position according to cursor insert position. */
            _gx_multi_line_text_input_cursor_pos_update(text_input, GX_TRUE);

            /* Dirty mark. */
            if (shift == text_input -> gx_multi_line_text_view_text_scroll_shift)
            {
                _gx_multi_line_text_input_text_rectangle_get(text_input, cursor_ptr -> gx_text_input_cursor_pos, end_pos, &cur_rect);
            }
            else
            {
                cur_rect = text_input -> gx_widget_size;
            }
        }

        text_input -> gx_multi_line_text_input_start_mark = 0;
        text_input -> gx_multi_line_text_input_end_mark = 0;

        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cur_rect);

        /* Mark cursor area as dirty. */
        _gx_text_input_cursor_dirty_rectangle_get(&text_input -> gx_multi_line_text_input_cursor_instance, &cur_rect);
        _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cur_rect);
    }
    else
    {
        view_text = &text_input -> gx_multi_line_text_view_text;
        index = text_input -> gx_multi_line_text_input_text_insert_position;
        if (index < view_text -> gx_string_length)
        {
            if ((view_text -> gx_string_ptr[index] == GX_KEY_CARRIAGE_RETURN) ||
                (view_text -> gx_string_ptr[index] == GX_KEY_LINE_FEED))
            {
                glyph_len = text_input -> gx_multi_line_text_input_new_line_character_size;
            }
#ifdef GX_UTF8_SUPPORT
            else
            {
                /* Calculate glygh length. */
                string.gx_string_ptr = view_text -> gx_string_ptr + index;
                string.gx_string_length = view_text -> gx_string_length - index;
                _gx_utility_utf8_string_character_get(&string, GX_NULL, &glyph_len);
            }
#endif

            /* Move the cursor position in the input buffer forward by one character. */
            text_input -> gx_multi_line_text_input_text_insert_position += glyph_len;

            /* Record scroll shift before recalculate cursor position. */
            _gx_text_input_cursor_dirty_rectangle_get(&text_input -> gx_multi_line_text_input_cursor_instance, &cur_rect);
            shift = text_input -> gx_multi_line_text_view_text_scroll_shift;

            /* Recalculate cursor position according to cursor insert position. */
            _gx_multi_line_text_input_cursor_pos_update(text_input, GX_TRUE);

            /* Dirty mark. */
            if (shift == text_input -> gx_multi_line_text_view_text_scroll_shift)
            {
                _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cur_rect);
                _gx_text_input_cursor_dirty_rectangle_get(&text_input -> gx_multi_line_text_input_cursor_instance, &cur_rect);
                _gx_system_dirty_partial_add((GX_WIDGET *)text_input, &cur_rect);
            }
            else
            {
                _gx_system_dirty_mark((GX_WIDGET *)text_input);
            }
        }
    }
    return GX_SUCCESS;
}

