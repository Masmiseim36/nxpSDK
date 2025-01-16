Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1010 board
- Personal Computer
- Oscilloscope

Board settings
==============

* Probe the pwm signal using an oscilloscope
  R793,R794,R796,R798 need to be connected
 - At J60-15
 - At J60-13
 - At J60-11
 - At J60-7

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~
FlexPWM driver example
~~~~~~~~~~~~~~~~~~~~~~~

Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
2. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41.
3. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
