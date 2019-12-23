Overview
========

The DAC / CADC demo application demonstrates the use of the DAC and CADC peripherals.
This application demonstrates how to configure the DAC and set the output on the DAC.
This demo also demonstrates how to configure the CADC and how to read CADC values.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- HVP-KV46F150M board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running
    the demo.

Running the demo
================
This example shows how to run the demo:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DAC CADC Demo!

ADC Full Range: XXXX

Please refer to Kinetis SDK Demo Applications User's Guide document,

Chapter DAC CADC demo, for pins configuration information.

Press any key to start demo.

Demo begin...


Select DAC output level:
        1. 1.0 V
        2. 1.5 V
        3. 2.0 V
        4. 2.5 V
        5. 3.0 V
-->2

ADC Value: 1861

ADC Voltage: 1.499

What next?:
        1. Test another DAC output value.
        2. Terminate demo.
-->1

Select DAC output level:
        1. 1.0 V
        2. 1.5 V
        3. 2.0 V
        4. 2.5 V
        5. 3.0 V
-->
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

