Overview
========

The pdb_delay_interrupt example show how to use the PDB as a general programmable interrupt timer.

The PDB is triggered by software, and other external triggers are generated from PDB in this project,
so that user can see just a general counter is working with interrupt.

Toolchain supported
===================
- IAR embedded Workbench  8.50.9
- Keil MDK  5.33
- GCC ARM Embedded  9.3.1
- MCUXpresso  11.3.0

Hardware requirements
=====================
- Mini/Micro USB cable
- FRDM-K66F board
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

PDB Delay Interrupt Example.

Type any key into terminal to trigger the PDB counter ...
PDB Delay Interrupt Counter: 1

Type any key into terminal to trigger the PDB counter ...
PDB Delay Interrupt Counter: 2

Type any key into terminal to trigger the PDB counter ...
PDB Delay Interrupt Counter: 3

