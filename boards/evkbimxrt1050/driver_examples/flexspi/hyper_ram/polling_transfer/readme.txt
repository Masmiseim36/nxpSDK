Overview
========
The flexspi_hyper_ram_polling_transfer example shows how to use flexspi driver.

In this example, flexspi will send data and operate the external Hyper RAM connected with FLEXSPI.
Example will first check if the data read/write is correct, then implement the performance test.

Copy the whole fold of “hyper_ram” example into:
	SDK_2.3.1_EVKB-IMXRT1050\boards\evkbimxrt1050\driver_examples\flexspi\

Toolchain supported
===================
- IAR embedded Workbench 8.22.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- HyperFlash chip should be replaced with the HyperRAM device
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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
coreclk: 600000000; ahbclk: 150000000; hyperbusclk: 332307684
FlexSPI HyperRAM example started!
Vendor ID: 0x810c
IP Command Read/Write data succeed at all address range !
AHB Command Read/Write data succeed at all address range !
##HyperRAM AHB write perf##t1: 599989; t2: 599815; diff: 174; ns: 290, datasize: 32 byte; perf: 110MB/s; g_ms: 0
##HyperRAM AHB read perf###t1: 599985; t2: 599683; diff: 302; ns: 503, datasize: 32 byte; perf: 63MB/s; g_ms: 0
0x60000000: 0x5a5a5a5a 0x5a5a5a5a 0x5a5a5a5a 0x5a5a5a5a
0x60000004: 0x5a5a5a5a 0x5a5a5a5a 0x5a5a5a5a 0x5a5a5a5a
0x60000008: 0x5a5a5a5a 0x5a5a5a5a 0x5a5a5a5a 0x5a5a5a5a
0x6000000c: 0x5a5a5a5a 0x5a5a5a5a 0x5a5a5a5a 0x5a5a5a5a

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

