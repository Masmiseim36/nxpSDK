Overview
========
The OSTIMER project is a simple demonstration program of the SDK OSTIMER driver. It sets the OSTIMER as
the wakeup source from deep-sleep mode. After wakeup from deep-sleep mode, OS timer will set match value 
to trigger the interrupt while the timer count tick reach the match value about every 2 seconds.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.1

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT685 board
- Personal Computer

Board settings
==============

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.
The log below shows the output of the ostimer example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Board will enter power deep sleep mode, and then wakeup by OS timer after about 5 seconds.
After Board wakeup, the OS timer will trigger the match interrupt about every 2 seconds.
Board wakeup from deep sleep mode.

OS timer match value reached
OS timer match value reached
OS timer match value reached

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Running the demo
================

Customization options
=====================

