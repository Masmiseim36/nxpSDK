Overview
========

The pdb_dac32_trigger example shows how to use the PDB to generate a DAC trigger.

Based on the basic counter, to use the DAC trigger, just to enable the DAC trigger's "milestone" and set the user-defined value for it.
The DAC's "milestone" is called as "interval". Multiple DAC trigger intervals can be included into one PDB counter's cycle.
DAC trigger's counter would reset after the trigger is created and start counting again to the interval value.

In this example, the DAC32 is configured with hardware buffer enabled in normal work mode. Once it gets the trigger from the PDB,
the buffer read pointer increases.


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
No special is needed.

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
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

PDB DAC32 trigger Example.

DAC32 Buffer Information
      Buffer index max  : 15
      Buffer watermark  : 14
Type any key into terminal to trigger the DAC buffer through PDB ...

DAC32 Buffer Index  0: ReadPointerTopPositionEvent
DAC32 Buffer Index  1:
DAC32 Buffer Index  2:
DAC32 Buffer Index  3:
DAC32 Buffer Index  4:
DAC32 Buffer Index  5:
DAC32 Buffer Index  6:
DAC32 Buffer Index  7:
DAC32 Buffer Index  8:
DAC32 Buffer Index  9:
DAC32 Buffer Index 10:
DAC32 Buffer Index 11:
DAC32 Buffer Index 12:
DAC32 Buffer Index 13:
DAC32 Buffer Index 14: WatermarkEvent
DAC32 Buffer Index 15: ReadPointerBottomPositionEvent
Customization options
=====================

