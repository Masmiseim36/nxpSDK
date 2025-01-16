# mpi_loader_extram_hello_world_loader

## Overview
The extram loader application provides an example of a small loader running in SoC internal memory,
which is able to initialize the external RAM controller and load the other application to external
memory and kick off it.
The loader itself is built to run in internal memory, and the application to be loaded is built to
run in external RAM. They are combined with elftosb tool. On system reset, ROM loads them as a single
image into SoC internal memory, and starts the loader. Then the loader will load the secondary
application and kick off it to run in external memory.
The elftosb and blhost tool can be downloaded from nxp.com.

## Supported Boards
- [EVK-MIMXRT595](../../../../_boards/evkmimxrt595/component_examples/mpi_loader/extram_loader/example_board_readme.md)
- [EVK-MIMXRT685](../../../../_boards/evkmimxrt685/component_examples/mpi_loader/extram_loader/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../../_boards/mimxrt685audevk/component_examples/mpi_loader/extram_loader/example_board_readme.md)
