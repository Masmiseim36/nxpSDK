# hello_world_hybrid

## Overview
The Hello World hybrid demo application provides a sanity check for the new SDK build environments and board bring up. The Hello
World demo prints the "hello world." "Run in flash." strings to the terminal using the SDK UART drivers. The purpose of this demo is
to show how to use the UART, and to provide a simple project for debugging and further development. 
Note: Please input one character at a time. If you input too many characters each time, the receiver may overflow
because the low level UART uses simple polling way for receiving. If you want to try inputting many characters each time,
just define DEBUG_CONSOLE_TRANSFER_NON_BLOCKING in your project to use the advanced debug console utility.

## Running the demo
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
hello world.
Run in flash.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/demo_apps/hello_world_hybrid/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/demo_apps/hello_world_hybrid/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/demo_apps/hello_world_hybrid/example_board_readme.md)
