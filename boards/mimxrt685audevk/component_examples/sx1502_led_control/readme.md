# sx1502_led_control

## Overview
The sx1502_led_control Demo application demonstrates to control Leds of external DMIC board by commands.
Note: Please input one character at a time. If you input too many characters each time, the receiver may overflow
because the low level UART uses simple polling way for receiving. If you want to try inputting many characters each time,
just define DEBUG_CONSOLE_TRANSFER_NON_BLOCKING in your project to use the advanced debug console utility.
Besides, the demo does not support semihosting mode. The external_dmic_led_control demo is based on shell, debug console and serial manager. When semihosting is used, debug console and serial manager are bypassed. So the sx1502_led_control demo cannot work with semihosting.

## Supported Boards
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/component_examples/sx1502_led_control/example_board_readme.md)
