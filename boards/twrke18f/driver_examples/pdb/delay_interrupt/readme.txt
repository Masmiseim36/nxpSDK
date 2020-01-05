Overview
========

The pdb_delay_interrupt example show how to use the PDB as a general programmable interrupt timer.

The PDB is triggered by software, and other external triggers are generated from PDB in this project,
so that user can see just a general counter is working with interrupt.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- TWR-KE18F board
- Personal Computer

Board settings
==============
No need special setting

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
PDB Delay Interrupt Example.

Type any key into terminal to trigger the PDB counter ...
PDB Delay Interrupt Counter: 1

Type any key into terminal to trigger the PDB counter ...
PDB Delay Interrupt Counter: 2

Type any key into terminal to trigger the PDB counter ...
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

