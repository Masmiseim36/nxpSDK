# freertos_tickless_lpc

## Overview
This document explains the freertos_tickless example. It shows how the CPU enters the sleep mode and then
it is woken up either by expired time delay using low power timer module or by external interrupt caused by a
user defined button.

## Prepare the Demo
Connect a serial cable from the debug UART port of the board to the PC. Start Tera Term
(http://ttssh2.osdn.jp) and make a connection to the virtual serial port.

1. Start Tera Term
2. New connection -> Serial
3. Set appropriate COMx port (x is port number) in Port context menu. Number is provided by operation
   system and could be different from computer to computer. Select COM number related to virtual
   serial port. Confirm selected port by OK button.
4. Set following connection parameters in menu Setup->Serial port...
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: none
5.  Confirm selected parameters by OK button.

## Running the demo
After flashing the example to the board the console will start printing the number of tick count periodically
when the CPU is running.
To wake up the CPU by external interrupt, press the button, that is specified at the beginning of the
example (SWx where x is the number of the user defined button). When the button is pressed, the console prints
out the "CPU woken up by external interrupt" message.

Example output:

Tickless Demo example
Press SWx to wake up the CPU

Tick count :
0
5000
10000
CPU woken up by external interrupt
15000
20000
25000

## Explanation of the example
The example application prints the actual tick count number every time after the specified
delay. When the vTaskDelay() is called, the CPU enters the sleep mode for that defined period
of time.

While the CPU is in sleep mode and the user defined button is pressed, the CPU is woken up
by the external interrupt and continues to sleep after the interrupt is handled.
The period of time delay is not changed after the external interrupt occurs.

## Supported Boards
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [LPCXpresso51U68](../../_boards/lpcxpresso51u68/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [LPCXpresso54628](../../_boards/lpcxpresso54628/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [LPCXpresso54S018](../../_boards/lpcxpresso54s018/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [LPCXpresso55S06](../../_boards/lpcxpresso55s06/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [LPCXpresso55S28](../../_boards/lpcxpresso55s28/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [LPCXpresso860MAX](../../_boards/lpcxpresso860max/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/freertos_examples/freertos_tickless_lpc/example_board_readme.md)
