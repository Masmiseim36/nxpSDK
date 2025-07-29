# decnano_decompress

## Overview
The DECNano decompress Demo reads the RGB888 or ARGB8888 picture that is compressed in
DECNano format, decompress them, and shows them in the LCD panel. Change the DEMO_PIC_FORMAT
in input.h to test other compressed formats, all 6 DECNano compressed formats are supported.
Note this example demonstrates the LCDIF controller function, so only DEMO_PANEL_RK055AHD091,
DEMO_PANEL_RK055IQH091, DEMO_PANEL_RK055MHD091 and DEMO_PANEL_RASPI_7INCH are supported.

## Supported Boards
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/display_examples/decnano_decompress/example_board_readme.md)
