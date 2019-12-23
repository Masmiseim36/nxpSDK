Overview
========

The pdb_adc16_trigger example shows how to use the PDB to generate a ADC trigger.

Based on the basic counter, to use the ADC trigger, just to enable the ADC trigger's "milestone" and set the user-
defined value for it.
After the PDB counter is triggered to start, when the counter pass the "milestone", the ADC's Pre-Trigger would be
generated and sent to the ADC module.

In this example, the ADC16 is configured with hardware trigger and conversion complete interrupt enabled.
Once it gets the trigger from the PDB, the conversion goes, then the ISR would be executed.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/Micro USB cable
- TWR-KV10Z32 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Connect external signal in J16-9
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PDB ADC16 Pre-Trigger Example.
Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...

PDB Interrupt Counter: 1
ADC Conversion Interrupt Counter: 1
ADC Conversion Value: 3030

Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...
...............
...............
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

