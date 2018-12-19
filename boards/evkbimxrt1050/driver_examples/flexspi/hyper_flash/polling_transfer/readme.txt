Overview
========
The flexspi_hyper_flash_polling_transfer example shows how to use flexspi driver with polling:

In this example, flexspi will send data and operate the external Hyper flash connected with FLEXSPI. Some simple flash command will
be executed, such as Read ID, Erase Sector and Program Buffer.
Example will first configures hyper flash to enter ASO mode, read ID-CFI parameters, then exit ASO mode.
Second, the example erase a sector in flash, check if the erase is successful and program the same sector, then read back
the sector data. At last check if the data read back is correct.
Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer

Board settings
==============

Prepare the Demo
================
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
================
When the example runs successfully, you can see the similar information from the terminal as below.

For flexspi_nor targets, the result is:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXSPI hyperflash example started!
Found the HyperFlash by CFI
Erasing Serial NOR over FlexSPI...
Erase data - successfully.
Program data - successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~


For ram/sdram targets, the result is:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXSPI hyperflash example started!
Found the HyperFlash by CFI
Erasing whole chip over FlexSPI...
Erase finished !
Erasing Serial NOR over FlexSPI...
Erase data - successfully.
Program data - successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

