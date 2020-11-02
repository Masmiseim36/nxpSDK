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
/**   Binres Loader Management (Binres Loader)                            */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_binres_loader.h"
#include "gx_system.h"
#include "gx_utility.h"

extern GX_CONST GX_FONT _gx_system_font_mono;
extern GX_CONST GX_FONT _gx_system_font_4bpp;
extern GX_CONST GX_FONT _gx_system_font_8bpp;

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_theme_header_load                        PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads theme header from a binary data buffer.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary read control block     */
/*    header                                Returned theme header         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_theme_header_load(GX_BINRES_DATA_INFO *info, GX_THEME_HEADER *header)
{
    memcpy((GX_BYTE *)&header -> gx_theme_header_magic_number, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_theme_header_index, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_theme_header_color_count, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_theme_header_palette_count, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_theme_header_font_count, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_theme_header_pixelmap_count, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_theme_header_vscroll_appearance.gx_scroll_width, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_VALUE));
    info -> gx_binres_read_offset += sizeof(GX_VALUE);

    memcpy((GX_BYTE *)&header -> gx_theme_header_vscroll_appearance.gx_scroll_thumb_width, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_VALUE));
    info -> gx_binres_read_offset += sizeof(GX_VALUE);

    memcpy((GX_BYTE *)&header -> gx_theme_header_vscroll_appearance.gx_scroll_thumb_travel_min, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_VALUE));
    info -> gx_binres_read_offset += sizeof(GX_VALUE);

    memcpy((GX_BYTE *)&header -> gx_theme_header_vscroll_appearance.gx_scroll_thumb_travel_max, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_VALUE));
    info -> gx_binres_read_offset += sizeof(GX_VALUE);

    memcpy((GX_BYTE *)&header -> gx_theme_header_vscroll_appearance.gx_scroll_thumb_border_style, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_theme_header_vscroll_appearance.gx_scroll_fill_pixelmap, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_RESOURCE_ID));
    info -> gx_binres_read_offset += sizeof(GX_RESOURCE_ID);

    memcpy((GX_BYTE *)&header -> gx_theme_header_vscroll_appearance.gx_scroll_thumb_pixelmap, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_RESOURCE_ID));
    info -> gx_binres_read_offset += sizeof(GX_RESOURCE_ID);

    memcpy((GX_BYTE *)&header -> gx_theme_header_vscroll_appearance.gx_scroll_up_pixelmap, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_RESOURCE_ID));
    info -> gx_binres_read_offset += sizeof(GX_RESOURCE_ID);

    memcpy((GX_BYTE *)&header -> gx_theme_header_vscroll_appearance.gx_scroll_down_pixelmap, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_RESOURCE_ID));
    info -> gx_binres_read_offset += sizeof(GX_RESOURCE_ID);

    memcpy((GX_BYTE *)&header -> gx_theme_header_vscroll_appearance.gx_scroll_thumb_color, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_RESOURCE_ID));
    info -> gx_binres_read_offset += sizeof(GX_RESOURCE_ID);

    memcpy((GX_BYTE *)&header -> gx_theme_header_vscroll_appearance.gx_scroll_thumb_border_color, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_RESOURCE_ID));
    info -> gx_binres_read_offset += sizeof(GX_RESOURCE_ID);

    memcpy((GX_BYTE *)&header -> gx_theme_header_vscroll_appearance.gx_scroll_button_color, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_RESOURCE_ID));
    info -> gx_binres_read_offset += sizeof(GX_RESOURCE_ID);

    memcpy((GX_BYTE *)&header -> gx_theme_header_hscroll_appearance.gx_scroll_width, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_VALUE));
    info -> gx_binres_read_offset += sizeof(GX_VALUE);

    memcpy((GX_BYTE *)&header -> gx_theme_header_hscroll_appearance.gx_scroll_thumb_width, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_VALUE));
    info -> gx_binres_read_offset += sizeof(GX_VALUE);

    memcpy((GX_BYTE *)&header -> gx_theme_header_hscroll_appearance.gx_scroll_thumb_travel_min, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_VALUE));
    info -> gx_binres_read_offset += sizeof(GX_VALUE);

    memcpy((GX_BYTE *)&header -> gx_theme_header_hscroll_appearance.gx_scroll_thumb_travel_max, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_VALUE));
    info -> gx_binres_read_offset += sizeof(GX_VALUE);

    memcpy((GX_BYTE *)&header -> gx_theme_header_hscroll_appearance.gx_scroll_thumb_border_style, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_theme_header_hscroll_appearance.gx_scroll_fill_pixelmap, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_RESOURCE_ID));
    info -> gx_binres_read_offset += sizeof(GX_RESOURCE_ID);

    memcpy((GX_BYTE *)&header -> gx_theme_header_hscroll_appearance.gx_scroll_thumb_pixelmap, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_RESOURCE_ID));
    info -> gx_binres_read_offset += sizeof(GX_RESOURCE_ID);

    memcpy((GX_BYTE *)&header -> gx_theme_header_hscroll_appearance.gx_scroll_up_pixelmap, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_RESOURCE_ID));
    info -> gx_binres_read_offset += sizeof(GX_RESOURCE_ID);

    memcpy((GX_BYTE *)&header -> gx_theme_header_hscroll_appearance.gx_scroll_down_pixelmap, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_RESOURCE_ID));
    info -> gx_binres_read_offset += sizeof(GX_RESOURCE_ID);

    memcpy((GX_BYTE *)&header -> gx_theme_header_hscroll_appearance.gx_scroll_thumb_color, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_RESOURCE_ID));
    info -> gx_binres_read_offset += sizeof(GX_RESOURCE_ID);

    memcpy((GX_BYTE *)&header -> gx_theme_header_hscroll_appearance.gx_scroll_thumb_border_color, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_RESOURCE_ID));
    info -> gx_binres_read_offset += sizeof(GX_RESOURCE_ID);

    memcpy((GX_BYTE *)&header -> gx_theme_header_hscroll_appearance.gx_scroll_button_color, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_RESOURCE_ID));
    info -> gx_binres_read_offset += sizeof(GX_RESOURCE_ID);

    memcpy((GX_BYTE *)&header -> gx_theme_header_vscroll_style, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    memcpy((GX_BYTE *)&header -> gx_theme_header_hscroll_style, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    memcpy((GX_BYTE *)&header -> gx_theme_header_color_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    memcpy((GX_BYTE *)&header -> gx_theme_header_palette_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    memcpy((GX_BYTE *)&header -> gx_theme_header_font_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    memcpy((GX_BYTE *)&header -> gx_theme_header_pixelmap_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    memcpy((GX_BYTE *)&header -> gx_theme_header_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_color_header_load                        PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads color header from a binary data buffer.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary read control block     */
/*    header                                Returned color header         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_color_header_load(GX_BINRES_DATA_INFO *info, GX_COLOR_HEADER *header)
{
    memcpy((GX_BYTE *)&header -> gx_color_header_magic_number, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_color_header_color_count, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_color_header_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_palette_header_load                      PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads palette header from a binary data buffer.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary read control block     */
/*    header                                Returned palette header       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_palette_header_load(GX_BINRES_DATA_INFO *info, GX_PALETTE_HEADER *header)
{
    memcpy((GX_BYTE *)&header -> gx_palette_header_magic_number, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_palette_header_color_count, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_palette_header_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_font_header_load                         PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads font header from a binary data buffer.          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary read control block     */
/*    header                                Returned font header          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_font_header_load(GX_BINRES_DATA_INFO *info, GX_FONT_HEADER *header)
{
    memcpy((GX_BYTE *)&header -> gx_font_header_magic_number, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_font_header_index, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_font_header_page_count, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_font_header_deault, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_font_header_bits, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_font_header_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    memcpy((GX_BYTE *)&header -> gx_font_header_data_offset, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_page_header_load                         PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads page header from a binary data buffer.          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary read control block     */
/*    header                                Returned page header          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_page_header_load(GX_BINRES_DATA_INFO *info, GX_PAGE_HEADER *header)
{
ULONG read_data = 0;

    memcpy((GX_BYTE *)&header -> gx_page_header_magic_number, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_page_header_index, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_page_header_format, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_page_header_prespace, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_page_header_postspace, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_page_header_line_height, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_page_header_baseline, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    /* In the binary resource file, CHAR_CODE is always stored as 4-byte value. */
    memcpy((GX_BYTE *)&read_data, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);
    header -> gx_page_header_first_glyph = (GX_CHAR_CODE)read_data;

    /* In the binary resource file, CHAR_CODE is always stored as 4-byte value. */
    memcpy((GX_BYTE *)&read_data, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);
    header -> gx_page_header_last_glyph = (GX_CHAR_CODE)read_data;

    memcpy((GX_BYTE *)&header -> gx_page_header_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_glyph_header_load                        PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads glyph header from a binary data buffer.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary read control block     */
/*    header                                Returned glyph header         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_glyph_header_load(GX_BINRES_DATA_INFO *info, GX_GLYPH_HEADER *header)
{
    memcpy((GX_BYTE *)&header -> gx_glyph_header_magic_number, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_glyph_header_map_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_glyph_header_index, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_glyph_header_ascent, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(SHORT));
    info -> gx_binres_read_offset += sizeof(SHORT);

    memcpy((GX_BYTE *)&header -> gx_glyph_header_descent, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(SHORT));
    info -> gx_binres_read_offset += sizeof(SHORT);

    memcpy((GX_BYTE *)&header -> gx_glyph_header_advance, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_glyph_header_leading, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_BYTE));
    info -> gx_binres_read_offset += sizeof(GX_BYTE);

    memcpy((GX_BYTE *)&header -> gx_glyph_header_width, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_glyph_header_height, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_glyph_header_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    return GX_SUCCESS;
}
#endif


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_kerning_glyph_header_load                PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads glyph header from a binary data buffer.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary read control block     */
/*    header                                Returned glyph header         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
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
#ifdef GX_FONT_KERNING_SUPPORT
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_kerning_glyph_header_load(GX_BINRES_DATA_INFO *info, GX_KERNING_GLYPH_HEADER *header)
{
    _gx_binres_glyph_header_load(info, (GX_GLYPH_HEADER *)header);

    memcpy((GX_BYTE *)&header -> gx_glyph_header_kerning_table_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    return GX_SUCCESS;
}
#endif
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_pixelmap_header_load                     PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reads pixelmap header from a binary data buffer.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary read control block     */
/*    header                                Returned pixelmap header      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_pixelmap_header_load(GX_BINRES_DATA_INFO *info, GX_PIXELMAP_HEADER *header)
{
    memcpy((GX_BYTE *)&header -> gx_pixelmap_header_magic_number, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_pixelmap_header_index, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_pixelmap_header_version_major, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_pixelmap_header_version_minor, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_pixelmap_header_flags, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_pixelmap_header_format, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_UBYTE));
    info -> gx_binres_read_offset += sizeof(GX_UBYTE);

    memcpy((GX_BYTE *)&header -> gx_pixelmap_header_map_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    memcpy((GX_BYTE *)&header -> gx_pixelmap_header_aux_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    memcpy((GX_BYTE *)&header -> gx_pixelmap_header_transparent_color, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(GX_COLOR));
    info -> gx_binres_read_offset += sizeof(GX_COLOR);

    memcpy((GX_BYTE *)&header -> gx_pixelmap_header_width, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_pixelmap_header_height, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(USHORT));
    info -> gx_binres_read_offset += sizeof(USHORT);

    memcpy((GX_BYTE *)&header -> gx_pixelmap_header_data_size, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    memcpy((GX_BYTE *)&header -> gx_pixelmap_header_data_offset, info -> gx_binres_root_address + info -> gx_binres_read_offset, sizeof(ULONG));
    info -> gx_binres_read_offset += sizeof(ULONG);

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_theme_table_buffer_allocate              PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function allocates needed memory buffer for loading theme      */
/*    table.                                                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary resource control block */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_memory_allocator           Application defined memory    */
/*                                            allocation function         */
/*    _gx_binres_resource_header_load       Read resource header          */
/*    _gx_binres_theme_header_load          Read theme header             */
/*    _gx_binres_font_header_load           Read font header              */
/*    _gx_binres_page_header_load           Read font page header         */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_theme_buffer_allocate(GX_BINRES_DATA_INFO *info, INT theme_id)
{
GX_RESOURCE_HEADER res_header;
GX_THEME_HEADER    theme_header;
GX_FONT_HEADER     font_header;
GX_PAGE_HEADER     page_header;
USHORT             theme_index;
USHORT             font_index;
USHORT             page_index;
UINT               glyph_count;
UINT               read_offset = 0;
UINT               temp;

    /* Read resource header.  */
    info -> gx_binres_read_offset = 0;

    _gx_binres_resource_header_load(info, &res_header);

    if (res_header.gx_resource_header_magic_number != GX_MAGIC_NUMBER)
    {
        return GX_INVALID_FORMAT;
    }

    info -> gx_binres_buffer_size = 0;

    for (theme_index = 0; theme_index < res_header.gx_resource_header_theme_count; theme_index++)
    {
        _gx_binres_theme_header_load(info, &theme_header);

        if (theme_header.gx_theme_header_magic_number != GX_MAGIC_NUMBER)
        {
            return GX_INVALID_FORMAT;
        }

        if (theme_index == theme_id)
        {
            info -> gx_binres_buffer_size = sizeof(GX_THEME);

            /* Color table buffer size.  */
            if (theme_header.gx_theme_header_color_data_size)
            {
                info -> gx_binres_read_offset += theme_header.gx_theme_header_color_data_size;
            }

            /* Palette table buffer size.  */
            if (theme_header.gx_theme_header_palette_data_size)
            {
                info -> gx_binres_read_offset += theme_header.gx_theme_header_palette_data_size;
            }

            /* Font table buffer size.  */
            if (theme_header.gx_theme_header_font_data_size)
            {
                /* Font count is USHORT type, overflow cannot occur. */
                info -> gx_binres_buffer_size += sizeof(GX_FONT *) * theme_header.gx_theme_header_font_count;

                for (font_index = 0; font_index < theme_header.gx_theme_header_font_count; font_index++)
                {
                    _gx_binres_font_header_load(info, &font_header);

                    if (font_header.gx_font_header_magic_number != GX_MAGIC_NUMBER)
                    {
                        return GX_INVALID_FORMAT;
                    }

                    if (font_header.gx_font_header_data_offset)
                    {
                        /* The font data is not follow the header directly,
                           the data offset gives the position where the font
                           data located. */

                        /* Record current data offset. */
                        read_offset = info -> gx_binres_read_offset;

                        /* Temporarily reset data offset of access font data. */
                        info -> gx_binres_read_offset = font_header.gx_font_header_data_offset;

                        /* Load font header. */
                        _gx_binres_font_header_load(info, &font_header);
                    }

                    for (page_index = 0; page_index < font_header.gx_font_header_page_count; page_index++)
                    {
                        /* Load page header. */
                        _gx_binres_page_header_load(info, &page_header);
                        info -> gx_binres_read_offset += page_header.gx_page_header_data_size;

                        if (page_header.gx_page_header_magic_number != GX_MAGIC_NUMBER)
                        {
                            return GX_INVALID_FORMAT;
                        }
#if defined(GX_EXTENDED_UNICODE_SUPPORT)
                        if (page_header.gx_page_header_last_glyph > GX_MAX_GLYPH_CODE)
                        {
                            return GX_INVALID_FONT;
                        }
#endif

                        /* Max glyph code is 0x10f000, overflow cannot occur. */
                        glyph_count = (UINT)(page_header.gx_page_header_last_glyph - page_header.gx_page_header_first_glyph + 1);

                        /* Calculate size for loading font page. */
                        temp = sizeof(GX_FONT);

                        /* Calculate size for loading glyphs. */
                        if (page_header.gx_page_header_format & GX_FONT_FORMAT_COMPRESSED)
                        {
                            temp += sizeof(GX_COMPRESSED_GLYPH) * glyph_count;
                        }
#if defined(GX_FONT_KERNING_SUPPORT)
                        else if (page_header.gx_page_header_format & GX_FONT_FORMAT_KERNING)
                        {
                            temp += sizeof(GX_KERNING_GLYPH) * glyph_count;
                        }
#endif
                        else
                        {
                            temp += sizeof(GX_GLYPH) * glyph_count;
                        }

                        GX_UTILITY_MATH_UINT_ADD(info -> gx_binres_buffer_size, temp, info -> gx_binres_buffer_size);
                    }

                    if (read_offset)
                    {
                        /* Restore data offset. */
                        info -> gx_binres_read_offset = read_offset;
                        read_offset = 0;
                    }
                }
            }

            /* Pixelmap table buffer size.  */
            if (theme_header.gx_theme_header_pixelmap_data_size)
            {
                /* Pixelmap count is USHORT type, overflow can not occur. */
                temp = ((UINT)(theme_header.gx_theme_header_pixelmap_count + 1)) * sizeof(GX_PIXELMAP *);
                temp += sizeof(GX_PIXELMAP) * theme_header.gx_theme_header_pixelmap_count;

                GX_UTILITY_MATH_UINT_ADD(info -> gx_binres_buffer_size, temp, info -> gx_binres_buffer_size);

                info -> gx_binres_read_offset += theme_header.gx_theme_header_pixelmap_data_size;
            }

            break;
        }
        else
        {
            info -> gx_binres_read_offset += theme_header.gx_theme_header_data_size;
        }
    }

    info -> gx_binres_buffer = (GX_UBYTE *)_gx_system_memory_allocator(info -> gx_binres_buffer_size);

    if (!info -> gx_binres_buffer)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    memset(info -> gx_binres_buffer, 0, info -> gx_binres_buffer_size);
    info -> gx_binres_buffer_index = 0;

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_glyphs_address_get                       PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function finds the memory address of specified glyph data from */
/*    a binary data buffer.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary resource control block */
/*    glyph_count                           The number of glyphs that the */
/*                                            font page contains.         */
/*    returned_glyphs                       Returned glyph address        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_glyph_header_load          Read glyph data header        */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_glyphs_address_get(GX_BINRES_DATA_INFO *info, USHORT glyph_count, GX_CONST GX_GLYPH **returned_glyphs)
{
GX_GLYPH_HEADER header;
GX_GLYPH       *glyphs;
USHORT          index = 0;

    glyphs = (GX_GLYPH *)(info -> gx_binres_buffer + info -> gx_binres_buffer_index);
    info -> gx_binres_buffer_index += sizeof(GX_GLYPH) * glyph_count;

    for (index = 0; index < glyph_count; index++)
    {
        _gx_binres_glyph_header_load(info, &header);

        if (header.gx_glyph_header_magic_number != GX_MAGIC_NUMBER)
        {
            return GX_INVALID_FORMAT;
        }

        glyphs[index].gx_glyph_advance = header.gx_glyph_header_advance;
        glyphs[index].gx_glyph_ascent = header.gx_glyph_header_ascent;
        glyphs[index].gx_glyph_descent = header.gx_glyph_header_descent;
        glyphs[index].gx_glyph_height = header.gx_glyph_header_height;
        glyphs[index].gx_glyph_leading = header.gx_glyph_header_leading;
        glyphs[index].gx_glyph_width = header.gx_glyph_header_width;

        /* Read glyph data.  */
        if (header.gx_glyph_header_data_size)
        {
            glyphs[index].gx_glyph_map = (GX_UBYTE *)(info -> gx_binres_root_address + info -> gx_binres_read_offset);
            info -> gx_binres_read_offset += header.gx_glyph_header_data_size;
        }
        else
        {
            glyphs[index].gx_glyph_map = GX_NULL;
        }
    }

    *returned_glyphs = glyphs;

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_kerning_glyphs_address_get               PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the memory address of specified kerning glyph     */
/*      from a binary data buffer.                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary resource control block */
/*    glyph_count                           The number of glyphs that the */
/*                                            font page contains.         */
/*    returned_glyphs                       Returned glyph address        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_kerning_glyph_header_load                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*                                                                        */
/**************************************************************************/
#ifdef GX_FONT_KERNING_SUPPORT
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_kerning_glyphs_address_get(GX_BINRES_DATA_INFO *info, USHORT glyph_count,
                                                  GX_CONST GX_KERNING_GLYPH **returned_glyphs)
{
GX_KERNING_GLYPH_HEADER header;
GX_KERNING_GLYPH       *glyphs;
USHORT                  index = 0;

    glyphs = (GX_KERNING_GLYPH *)(info -> gx_binres_buffer + info -> gx_binres_buffer_index);
    info -> gx_binres_buffer_index += sizeof(GX_KERNING_GLYPH) * glyph_count;

    for (index = 0; index < glyph_count; index++)
    {
        _gx_binres_kerning_glyph_header_load(info, &header);

        if (header.gx_glyph_header_magic_number != GX_MAGIC_NUMBER)
        {
            return GX_INVALID_FORMAT;
        }

        glyphs[index].gx_glyph_advance = header.gx_glyph_header_advance;
        glyphs[index].gx_glyph_ascent = header.gx_glyph_header_ascent;
        glyphs[index].gx_glyph_descent = header.gx_glyph_header_descent;
        glyphs[index].gx_glyph_height = header.gx_glyph_header_height;
        glyphs[index].gx_glyph_leading = header.gx_glyph_header_leading;
        glyphs[index].gx_glyph_width = header.gx_glyph_header_width;

        /* Read glyph data.  */
        if (header.gx_glyph_header_data_size)
        {
            glyphs[index].gx_glyph_map = (GX_UBYTE *)(info -> gx_binres_root_address + info -> gx_binres_read_offset);
            info -> gx_binres_read_offset += header.gx_glyph_header_data_size;
        }
        else
        {
            glyphs[index].gx_glyph_map = GX_NULL;
        }

        /* Set glyph kerning table. */
        if (header.gx_glyph_header_kerning_table_size)
        {
            glyphs[index].gx_kerning_table = (GX_UBYTE *)(info -> gx_binres_root_address + info -> gx_binres_read_offset);
            info -> gx_binres_read_offset += header.gx_glyph_header_kerning_table_size;
        }
    }

    *returned_glyphs = glyphs;

    return GX_SUCCESS;
}
#endif
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_compressed_glyphs_address_get            PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the memory address of specified compressed glyph  */
/*    from a binary data buffer.                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary resource control block */
/*    glyph_count                           The number of glyphs that the */
/*                                            font page contains.         */
/*    returned_glyphs                       Returned glyph address        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_glyph_header_load          Read glyph data header        */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_compressed_glyphs_address_get(GX_BINRES_DATA_INFO *info, USHORT glyph_count,
                                                     GX_CONST GX_COMPRESSED_GLYPH **returned_glyphs)
{
GX_GLYPH_HEADER      header;
GX_COMPRESSED_GLYPH *glyphs;
USHORT               index = 0;

    glyphs = (GX_COMPRESSED_GLYPH *)(info -> gx_binres_buffer + info -> gx_binres_buffer_index);
    info -> gx_binres_buffer_index += sizeof(GX_COMPRESSED_GLYPH) * glyph_count;

    for (index = 0; index < glyph_count; index++)
    {
        _gx_binres_glyph_header_load(info, &header);

        if (header.gx_glyph_header_magic_number != GX_MAGIC_NUMBER)
        {
            return GX_INVALID_FORMAT;
        }

        glyphs[index].gx_glyph_advance = header.gx_glyph_header_advance;
        glyphs[index].gx_glyph_ascent = header.gx_glyph_header_ascent;
        glyphs[index].gx_glyph_descent = header.gx_glyph_header_descent;
        glyphs[index].gx_glyph_height = header.gx_glyph_header_height;
        glyphs[index].gx_glyph_leading = header.gx_glyph_header_leading;
        glyphs[index].gx_glyph_map_size = header.gx_glyph_header_map_size;
        glyphs[index].gx_glyph_width = header.gx_glyph_header_width;

        /* Read glyph data.  */
        if (header.gx_glyph_header_data_size)
        {
            glyphs[index].gx_glyph_map = (GX_UBYTE *)(info -> gx_binres_root_address + info -> gx_binres_read_offset);
            info -> gx_binres_read_offset += header.gx_glyph_header_data_size;
        }
        else
        {
            glyphs[index].gx_glyph_map = GX_NULL;
        }
    }

    *returned_glyphs = glyphs;

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_font_load                                PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service loads a font table from a binary data buffer.          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary resource control block */
/*    page_count                            The number of pages that a    */
/*                                            font contains.              */
/*    return_font                           Returned font                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_page_header_load           Read font page header         */
/*    _gx_binres_compressed_glyphs_read     Read compressed glyph data    */
/*    _gx_binres_glyphs_read                Read glyph data               */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_font_load(GX_BINRES_DATA_INFO *info, USHORT page_count, GX_FONT **return_font)
{
UINT           status = GX_SUCCESS;
GX_PAGE_HEADER header;
GX_FONT       *font;
GX_FONT       *head_page = GX_NULL;
GX_FONT       *pre_page = GX_NULL;
USHORT         index;
USHORT         glyph_count;

    for (index = 0; index < page_count; index++)
    {
        /* Read page header.  */
        _gx_binres_page_header_load(info, &header);

        font = (GX_FONT *)(info -> gx_binres_buffer + info -> gx_binres_buffer_index);
        info -> gx_binres_buffer_index += sizeof(GX_FONT);

        font -> gx_font_baseline = header.gx_page_header_baseline;
        font -> gx_font_first_glyph = header.gx_page_header_first_glyph;
        font -> gx_font_format = header.gx_page_header_format;
        font -> gx_font_last_glyph = header.gx_page_header_last_glyph;
        font -> gx_font_line_height = header.gx_page_header_line_height;
        font -> gx_font_postspace = header.gx_page_header_postspace;
        font -> gx_font_prespace = header.gx_page_header_prespace;

        /* Read glyphs data.  */
        glyph_count = (USHORT)(font -> gx_font_last_glyph - font -> gx_font_first_glyph + 1);

        if (font -> gx_font_format & GX_FONT_FORMAT_COMPRESSED)
        {
            status = _gx_binres_compressed_glyphs_address_get(info, glyph_count, &font -> gx_font_glyphs.gx_font_compressed_glyphs);
        }
#if defined(GX_FONT_KERNING_SUPPORT)
        else if (font -> gx_font_format & GX_FONT_FORMAT_KERNING)
        {
            status = _gx_binres_kerning_glyphs_address_get(info, glyph_count, &font -> gx_font_glyphs.gx_font_kerning_glyphs);
        }
#endif /* GX_FONT_KERNING_SUPPORT */
        else
        {
            status = _gx_binres_glyphs_address_get(info, glyph_count, &font -> gx_font_glyphs.gx_font_normal_glyphs);
        }

        if (status != GX_SUCCESS)
        {
            break;
        }

        font -> gx_font_next_page = NULL;

        if (!head_page)
        {
            head_page = font;
        }

        if (pre_page)
        {
            pre_page -> gx_font_next_page = font;
        }

        pre_page = font;
    }

    *return_font = head_page;

    return status;
}
#endif
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_color_table_load                         PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function loads a color table from a binary data buffer.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary resource control block */
/*    color_count                           The number of colors to read  */
/*    returned_color_table                  Returned color table          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_color_header_load          Read color data header        */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_color_table_load(GX_BINRES_DATA_INFO *info, USHORT color_count, GX_COLOR **returned_color_table)
{
GX_COLOR_HEADER header;
GX_COLOR       *color_table = GX_NULL;

    if (!color_count)
    {
        return GX_SUCCESS;
    }

    /* Read color table header.  */
    _gx_binres_color_header_load(info, &header);

    if (header.gx_color_header_magic_number != GX_MAGIC_NUMBER)
    {
        return GX_INVALID_FORMAT;
    }

    color_table = (GX_COLOR *)(info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += header.gx_color_header_data_size;

    *returned_color_table = color_table;

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_palette_table_load                       PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function loads a palette table from a binary data buffer.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary resource control block */
/*    color_count                           The number of colors to read  */
/*    returned_palette_table                Returned palette table        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_palette_header_load        Read palette data header      */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_palette_table_load(GX_BINRES_DATA_INFO *info, USHORT color_count, GX_COLOR **returned_palette_table)
{
GX_PALETTE_HEADER header;
GX_COLOR         *palette_table = GX_NULL;

    if (!color_count)
    {
        return GX_SUCCESS;
    }

    /* Read palette table header.  */
    _gx_binres_palette_header_load(info, &header);

    if (header.gx_palette_header_magic_number != GX_MAGIC_NUMBER)
    {
        return GX_INVALID_FORMAT;
    }

    palette_table = (GX_COLOR *)(info -> gx_binres_root_address + info -> gx_binres_read_offset);
    info -> gx_binres_read_offset += header.gx_palette_header_data_size;

    *returned_palette_table = palette_table;

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_font_table_load                          PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function loads a font table from resource data memory.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary resource control block */
/*    table_size                            Font table size               */
/*    returned_font_table                   Returned font table           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_font_header_load           Read font header              */
/*    _gx_binres_font_read                  Read font data                */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_font_table_load(GX_BINRES_DATA_INFO *info, USHORT table_size, GX_FONT ***returned_font_table)
{
UINT           status = GX_SUCCESS;
GX_FONT_HEADER header;
GX_FONT      **font_table = GX_NULL;
USHORT         index;
UINT           read_offset = 0;

    if (!table_size)
    {
        return status;
    }

    font_table = (GX_FONT **)(info -> gx_binres_buffer + info -> gx_binres_buffer_index);
    info -> gx_binres_buffer_index += sizeof(GX_FONT *) * table_size;

    for (index = 0; index < table_size; index++)
    {
        /* Read font header.  */
        _gx_binres_font_header_load(info, &header);

        if (header.gx_font_header_deault)
        {
            switch (header.gx_font_header_bits)
            {
            case 1:
                font_table[index] = (GX_FONT *)&_gx_system_font_mono;
                break;

            case 4:
                font_table[index] = (GX_FONT *)&_gx_system_font_4bpp;
                break;

            case 8:
                font_table[index] = (GX_FONT *)&_gx_system_font_8bpp;
                break;
            }
        }
        else
        {
            if (header.gx_font_header_data_offset)
            {
                /* The font data is not follow the header directly,
                   the data offset gives the position where the font
                   data located. */

                /* Record current data offset. */
                read_offset = info -> gx_binres_read_offset;

                /* Temporarily reset data offset to access font data. */
                info -> gx_binres_read_offset = header.gx_font_header_data_offset;

                /* Read font header. */
                _gx_binres_font_header_load(info, &header);
            }

            status = _gx_binres_font_load(info, header.gx_font_header_page_count, &font_table[index]);

            if (status != GX_SUCCESS)
            {
                break;
            }

            if (read_offset)
            {
                /* Restore data offset. */
                info -> gx_binres_read_offset = read_offset;
                read_offset = 0;
            }
        }
    }

    *returned_font_table = font_table;

    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_pixelmap_table_load                      PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function loads a pixelmap table from resource data memory.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    info                                  Binary resource control block */
/*    table_size                            Pixelmap table size           */
/*    returned_pixelmp_table                Returned pixelmap table       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_pixelmap_header_load       Read pixelmap header          */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
static UINT _gx_binres_pixelmap_table_load(GX_BINRES_DATA_INFO *info, USHORT table_size, GX_PIXELMAP ***returned_pixelmap_table)
{
UINT               status = GX_SUCCESS;
GX_PIXELMAP_HEADER header;
GX_PIXELMAP      **pixelmap_table = GX_NULL;
USHORT             index;
ULONG              size;
GX_BOOL            keep_looping;
UINT               read_offset = 0;

    if (table_size <= 1)
    {
        return GX_SUCCESS;
    }

    pixelmap_table = (GX_PIXELMAP **)(info -> gx_binres_buffer + info -> gx_binres_buffer_index);
    info -> gx_binres_buffer_index += sizeof(GX_PIXELMAP *) * table_size;

    for (index = 1; index < table_size; index++)
    {
        /* Read pixelmap header.  */
        _gx_binres_pixelmap_header_load(info, &header);

        if (header.gx_pixelmap_header_magic_number != GX_MAGIC_NUMBER)
        {
            return GX_INVALID_FORMAT;
        }

        if (header.gx_pixelmap_header_data_offset)
        {
            /* The pixelmap data is not follow the header directly,
               the data offset gives the position where the pixelmap
               data located. */

            /* Record current data offset. */
            read_offset = info -> gx_binres_read_offset;

            /* Temporarily reset data offset to access pixelmap data. */
            info -> gx_binres_read_offset = header.gx_pixelmap_header_data_offset;

            /* Read pixelmap header. */
            _gx_binres_pixelmap_header_load(info, &header);
        }

        keep_looping = GX_TRUE;

        while (keep_looping)
        {
            keep_looping = GX_FALSE;

            /* Allocate memory for pixelmap.  */
            pixelmap_table[index] = (GX_PIXELMAP *)(info -> gx_binres_buffer + info -> gx_binres_buffer_index);
            info -> gx_binres_buffer_index += sizeof(GX_PIXELMAP);

            switch (index)
            {
            case GX_PIXELMAP_RADIO_ON_ID:
            case GX_PIXELMAP_RADIO_OFF_ID:
            case GX_PIXELMAP_CHECKBOX_ON_ID:
            case GX_PIXELMAP_CHECKBOX_OFF_ID:
                if (index != header.gx_pixelmap_header_index)
                {
                    pixelmap_table[index++] = GX_NULL;
                    keep_looping = GX_TRUE;
                }
            }
        }

        pixelmap_table[index] -> gx_pixelmap_aux_data_size = header.gx_pixelmap_header_aux_data_size;
        pixelmap_table[index] -> gx_pixelmap_data_size = header.gx_pixelmap_header_map_size;
        pixelmap_table[index] -> gx_pixelmap_flags = header.gx_pixelmap_header_flags;
        pixelmap_table[index] -> gx_pixelmap_format = header.gx_pixelmap_header_format;
        pixelmap_table[index] -> gx_pixelmap_width = (GX_VALUE)header.gx_pixelmap_header_width;
        pixelmap_table[index] -> gx_pixelmap_height = (GX_VALUE)header.gx_pixelmap_header_height;
        pixelmap_table[index] -> gx_pixelmap_transparent_color = header.gx_pixelmap_header_transparent_color;
        pixelmap_table[index] -> gx_pixelmap_version_major = header.gx_pixelmap_header_version_major;
        pixelmap_table[index] -> gx_pixelmap_version_minor = header.gx_pixelmap_header_version_minor;

        /* Skip padding bytes. */
        info -> gx_binres_read_offset = (info -> gx_binres_read_offset + 3) & (~0x03UL);

        /* Read pixelmap data.  */
        size = pixelmap_table[index] -> gx_pixelmap_data_size;
        if (size)
        {
            pixelmap_table[index] -> gx_pixelmap_data = (GX_UBYTE *)(info -> gx_binres_root_address + info -> gx_binres_read_offset);
            info -> gx_binres_read_offset += size;
        }

        /* Read pixelmap aux data.  */
        size = pixelmap_table[index] -> gx_pixelmap_aux_data_size;
        if (size)
        {
            pixelmap_table[index] -> gx_pixelmap_aux_data = (GX_UBYTE *)(info -> gx_binres_root_address + info -> gx_binres_read_offset);
            info -> gx_binres_read_offset += size;
        }

        if (read_offset)
        {
            /* Restore data offset. */
            info -> gx_binres_read_offset = read_offset;
            read_offset = 0;
        }
    }

    *returned_pixelmap_table = pixelmap_table;

    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_binres_theme_load                               PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service loads a theme table from a resource data memory.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_address                          Root address of binary        */
/*                                            resource data               */
/*    theme_id                              The indentifier of the theme  */
/*    returned_theme                        Pointer to loaded theme       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_binres_theme_buffer_allocate      Allocate needed buffer for    */
/*                                            loading theme table         */
/*    _gx_binres_resource_header_load       Read resource header          */
/*    _gx_binres_theme_header_load          Read theme header             */
/*    _gx_binres_color_table_load           Load color table              */
/*    _gx_binres_palette_table_load         Load palette table            */
/*    _gx_binres_font_table_load            Load font table               */
/*    _gx_binres_pixelmap_table_load        Load pixelmap table           */
/*    _gx_system_memory_free                Memory free function          */
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
#ifdef GX_BINARY_RESOURCE_SUPPORT
UINT _gx_binres_theme_load(GX_UBYTE *root_address, INT theme_id, GX_THEME **returned_theme)
{
UINT                status;
GX_BINRES_DATA_INFO info;
GX_RESOURCE_HEADER  header;
GX_THEME_HEADER     theme_header;
GX_THEME           *theme;
INT                 index;

    memset(&info, 0, sizeof(GX_BINRES_DATA_INFO));
    memset(&theme_header, 0, sizeof(GX_THEME_HEADER));

    info.gx_binres_root_address = (GX_UBYTE *)root_address;

    /* Allocate memory that needed for theme table.  */
    status = _gx_binres_theme_buffer_allocate(&info, theme_id);

    /* Read binary resource header.  */

    if (status == GX_SUCCESS)
    {
        info.gx_binres_read_offset = 0;
        status = _gx_binres_resource_header_load(&info, &header);
    }

    /* Read theme table.  */
    if (status == GX_SUCCESS)
    {
        for (index = 0;; index++)
        {

            /* Read theme header.  */
            _gx_binres_theme_header_load(&info, &theme_header);

            if (index == theme_id)
            {
                theme = (GX_THEME *)(info.gx_binres_buffer + info.gx_binres_buffer_index);
                info.gx_binres_buffer_index += sizeof(GX_THEME);

                theme -> theme_color_table_size = theme_header.gx_theme_header_color_count;
                theme -> theme_palette_size = theme_header.gx_theme_header_palette_count;
                theme -> theme_font_table_size = theme_header.gx_theme_header_font_count;
                theme -> theme_pixelmap_table_size = (USHORT)(theme_header.gx_theme_header_pixelmap_count + 1);
                theme -> theme_vertical_scrollbar_appearance = theme_header.gx_theme_header_vscroll_appearance;
                theme -> theme_horizontal_scrollbar_appearance = theme_header.gx_theme_header_hscroll_appearance;
                theme -> theme_vertical_scroll_style = theme_header.gx_theme_header_hscroll_style;
                theme -> theme_horizontal_scroll_style = theme_header.gx_theme_header_hscroll_style;

                /* Read color table.  */
                status = _gx_binres_color_table_load(&info,
                                                     theme_header.gx_theme_header_color_count,
                                                     &theme -> theme_color_table);

                /* Read palette table.  */
                if ((status == GX_SUCCESS) && theme_header.gx_theme_header_palette_data_size)
                {
                    status = _gx_binres_palette_table_load(&info,
                                                           theme_header.gx_theme_header_palette_count,
                                                           &theme -> theme_palette);
                }

                /* Read font table.  */
                if ((status == GX_SUCCESS) && theme_header.gx_theme_header_font_data_size)
                {
                    status = _gx_binres_font_table_load(&info,
                                                        theme_header.gx_theme_header_font_count,
                                                        &theme -> theme_font_table);
                }

                /* Read pixelmap table.  */
                if ((status == GX_SUCCESS) && theme_header.gx_theme_header_pixelmap_data_size)
                {
                    status = _gx_binres_pixelmap_table_load(&info,
                                                            (USHORT)(theme_header.gx_theme_header_pixelmap_count + 1),
                                                            &theme -> theme_pixelmap_table);
                }

                if (status == GX_SUCCESS)
                {
                    *returned_theme = theme;
                }
                else
                {
                    /* Free allocated buffer if theme loading failed.  */
                    _gx_system_memory_free(info.gx_binres_buffer);

                    *returned_theme = GX_NULL;
                }

                break;
            }
            else
            {
                info.gx_binres_read_offset += theme_header.gx_theme_header_data_size;
            }
        }
    }

    return status;
}
#endif

