# dsp_hello_world_usart

## Overview
The dsp_hello_world_usart demo application provides a sanity check for the SDK build
environment running both ARM and DSP cores. It also demonstrates how the ARM 
core application is used to start a DSP application.

This demo contains two applications:
- cm33/ is the ARM application for Cortex-M33 core
- dsp/ is the DSP application for HiFi4 core

The release configurations of the demo will combine both applications into one ARM
image.  With this, the ARM core will load and start the DSP application on
startup.  Pre-compiled DSP binary images are provided under dsp/binary/ directory.

The debug configurations will build two separate applications that need to be
loaded independently.  The ARM application will power and clock the DSP, so
it must be loaded prior to loading the DSP application.

## Supported Boards
- [EVK9-MIMX8ULP](../../_boards/evk9mimx8ulp/dsp_examples/hello_world_usart/example_board_readme.md)
- [EVK-MIMX8ULP](../../_boards/evkmimx8ulp/dsp_examples/hello_world_usart/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/dsp_examples/hello_world_usart/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/dsp_examples/hello_world_usart/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/dsp_examples/hello_world_usart/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/dsp_examples/hello_world_usart/example_board_readme.md)
