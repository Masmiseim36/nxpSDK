# erpc_matrix_multiply_mu

## Overview

The Multicore eRPC Matrix Multiply project is a simple demonstration program that uses the
MCUXpresso SDK software and the Multicore SDK to show how to implement the Remote Procedure Call
between cores of the multicore system.

The primary core (eRPC client) releases the secondary core (eRPC server) from the reset and then
the erpcMatrixMultiply() eRPC call is issued to let the secondary core to perform the multiplication of
two randomly generated matrices.

The original matrices and the result matrix is printed out to the serial console by the primary core.

The matrix multiplication can be issued repeatedly when pressing a SW board button.

MU (Messaging Unit) erpc transport layer is used in this example application.

## eRPC documentation

eRPC specific files are stored in: middleware\multicore\erpc
eRPC documentation is stored in: middleware\multicore\erpc\doc
eRPC is open-source project stored on github: https://github.com/EmbeddedRPC/erpc
eRPC documentation can be also found in: http://embeddedrpc.github.io

## Building the application

This shows example how to build application for `evkbmimxrt1170` board with `cm7` core_id.
Change the `-b <board>` parameter based on board you want to build.
Change the `-Dcore_id=<core_id>` parameter based on board core you want to build.
For these parameters please see attribute `boards:` in `primary/example.yml`.

```
west build --sysbuild examples/multicore_examples/erpc_matrix_multiply_mu/primary --toolchain armgcc --config debug -b evkbmimxrt1170 -Dcore_id=cm7
```

## Supported Boards

- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/multicore_examples/erpc_matrix_multiply_mu/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/multicore_examples/erpc_matrix_multiply_mu/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/multicore_examples/erpc_matrix_multiply_mu/example_board_readme.md)
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/multicore_examples/erpc_matrix_multiply_mu/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/multicore_examples/erpc_matrix_multiply_mu/example_board_readme.md)
