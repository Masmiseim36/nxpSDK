Overview
========
The ADC12 continuous EDMA demo application demonstrates the usage of the ADC and EDMA peripheral while in a continuous mode. The
ADC12 is first set to continuous mode. In continuous convert configuration, only the initial rising-edge to launch continuous conversions is
observed, and until conversion is aborted, the ADC12 continues to do conversions on the same SCn register that initiated the conversion. 
EDMA request will be asserted during an ADC12 conversion complete event noted when any of the SC1n[COCO] flags is asserted. EDMA will transfer
ADC12 results to memory and if users press any key, demo will average ADC12 results stored in memory and print average on the terminal.  

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
This example use channel 12(PTC14) as ADC12's analog input channel. In order to observe the dynamical
measurement result, user should adjust the potentiometer to get different conversion result.

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
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADC12 CONTINUOUS EDMA DEMO
ADC calibration is done!
ADC Full Range: XXXX
Press any key to get user channel's ADC value ...
ADC value: 4092
ADC value: 4092
ADC value: 4092
ADC value: 4092
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

