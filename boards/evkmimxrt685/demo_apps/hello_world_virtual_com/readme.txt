Overview
========
The Hello World demo application provides a sanity check for the new SDK build environments and board bring up. The Hello
World demo prints the "Hello World" string to the terminal using the SDK UART drivers. The purpose of this demo is to
show how to use the UART, and to provide a simple project for debugging and further development.
Note: Please input one character at a time. If you input too many characters each time, the receiver may overflow
because the low level UART uses simple polling way for receiving. If you want to try inputting many characters each time,
just define DEBUG_CONSOLE_TRANSFER_NON_BLOCKING in your project to use the advanced debug console utility.

Toolchain supported
===================
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.1
- IAR embedded Workbench  8.40.2

Hardware requirements
=====================
- Two Micro USB cables
- EVK-MIMXRT685 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port(J5) on the target board for downloading
    the program and supply power for the board, connect another USB cable between host PC and USB port(J7)
	on the target board.
2.  Download the program to the target board.
3.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
4.  A COM port is enumerated in the Device Manager. If it prompts for CDC driver installation, please reference the virtual_com_readme.pdf which can be found at below directory:
    In Release package: <install_dir>\devices\<soc>\utilities\virtual_com_readme.pdf .to install the CDC driver.
5.  Open the COM port in a terminal tool, such as TeraTerm, SSCOM, Putty, Putty is recommanded while virtual com
is used.
6.  Then user can communicate with target boards through virtual com.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
hello world.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The debug console virtual com will echo back the received characters.
Customization options
=====================

