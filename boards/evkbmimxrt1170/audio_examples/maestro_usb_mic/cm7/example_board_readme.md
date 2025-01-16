Hardware requirements
=====================
- 2x Micro USB cable
- JTAG/SWD debugger
- EVKB-MIMXRT1170 board
- Personal Computer

Board settings
==============
1. Set the hardware jumpers (Tower system/base module) to default settings.

Prepare the Demo
================
1. Connect the first micro USB cable between the PC host and the debug USB port on the board.
2. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Connect the second micro USB cable between the PC host and the USB port on the board.
5. Either press the reset button on your board or launch the debugger in your IDE to begin
   running the demo.

