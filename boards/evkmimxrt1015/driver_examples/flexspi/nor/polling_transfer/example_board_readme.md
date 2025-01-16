Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1015 board
- Personal Computer

Board settings
============

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs successfully, you can see the similar information from the terminal as below.

For flexspi_nor targets, the result is:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXSPI example started!
Vendor ID: 0x1f
Erasing Serial NOR over FlexSPI...
Erase data - successfully.
Program data - successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~


For ram targets, the result is:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXSPI example started!
Vendor ID: 0x1f
Erasing whole chip over FlexSPI...
Erase finished !
Erasing Serial NOR over FlexSPI...
Erase data - successfully.
Program data - successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
