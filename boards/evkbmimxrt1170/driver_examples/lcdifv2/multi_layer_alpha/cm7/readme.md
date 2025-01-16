# lcdifv2_multi_layer_alpha

## Overview
This example demonstrates how to use the LCDIF v2 API to get the global alpha
based on desired blended alpha, for the multi-layer blend case.

In this example, 7 layers are enabled. Layer 0 is full screen blue color background,
layer 1-7 are alpha layers, they are all green with size (PANEL_WITDH / 2) x (PANEL_HEIGHT / 4).
The posision of each layer is:

+-----------------+-------------------+
|                 |                   |
|                 |                   |
|    Layer 1      |    Layer 2        |
|                 |                   |
|                 |                   |
+-----------------+-------------------+
|                 |                   |
|                 |                   |
|    Layer 3      |    Layer 4        |
|                 |                   |
|                 |                   |
+-----------------+-------------------+
|                 |                   |
|                 |                   |
|    Layer 5      |    Layer 6        |
|                 |                   |
|                 |                   |
+-----------------+-------------------+
|                 |                   |
|                 |                   |
|    Layer 7      |                   |
|                 |                   |
|                 |                   |
+-----------------+-------------------+

The alpha layers share the same frame buffer, and the desired blended alpha are all 31.
LCDIFv2 API is called to calculate the global alpha for each layer to get the desired
blended alpha.

Because the blended alpha for each alpha layer is the same, so all alpha layers result are the same,
the panel looks like:

+-------------------------------------+
|                                     |
|                                     |
|                                     |
|                                     |
|                                     |
|       Layer 1 ~ 7, all cyan         |
|                                     |
|                                     |
|                                     |
|                                     |
|                                     |
|                                     |
|                                     |
|                                     |
|                                     |
|                                     |
|                                     |
|                 +-------------------+
|                 |                   |
|                 |    Layer 0        |
|                 |                   |
|                 |    blue           |
|                 |                   |
+-----------------+-------------------+

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/lcdifv2/lcdifv2_examples_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/lcdifv2/lcdifv2_examples_readme.md)
