# jpegdec_context_switch

## Overview
The SD JPEG decoder example decodes a series of JPEG pictures stored on SD card
then shows them one by one on panel.
Two slots are used and each has one descriptor linked to itself. After slot 0
finishes slot 1 will be switch on in this round. After slot 1 finishes then the
next round starts, slot 0 will be switched on again.

## Supported Boards
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/driver_examples/jpegdec/jpegdec_examples_readme.md)
