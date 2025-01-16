Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
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
FLEXSPI hyperflash example started!
Found the HyperFlash by CFI
loop nummber: 0
loop nummber: 1
loop nummber: 2
loop nummber: 3
loop nummber: 4
loop nummber: 5
FLEXSPI hyperflash example successfully. 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For ram/sdram targets, the result is:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXSPI hyperflash example started!
Found the HyperFlash by CFI
Erasing whole chip over FlexSPI...
Erase finished !
loop nummber: 0
loop nummber: 1
loop nummber: 2
loop nummber: 3
loop nummber: 4
loop nummber: 5
FLEXSPI hyperflash example successfully. 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
