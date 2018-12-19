Overview
========

The FTM / PDB demo application demonstrates how to use the FTM external trigger to start the ADC conversion using the
PDB. FTM0 is configured as a complementary combined mode, and each channel output frequency is 16 KHz. The complementary
channel dead time is 1 µs. The PDB pre-trigger works in back-to-back mode. The ADC0 and ADC1 work in single-end mode.
The ADC0 uses channel 1 and channel 5, and ADC1 uses channel 1 and channel 7.
Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KV11Z
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
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================

1. The ADC PDB demo application will print the following message to the terminal:
   "Run PDB trig ADC with FlexTimer demo." and "Input any character to start demo."

2. Input a character to the serial console to start the ADC PDB demo. The demo will then display 256 lines of
   information for the ADC conversion result.

3. Press any key to restart the demo.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

