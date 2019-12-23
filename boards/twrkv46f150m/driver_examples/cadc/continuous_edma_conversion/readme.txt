Overview
========
The CADC continuous EDMA demo application demonstrates the usage of the ADC and EDMA peripheral while in a Loop Sequential mode. The
CADC is first set to Loop Sequential mode. In Loop Sequential convert configuration, only the initial rising-edge to launch continuous conversions is
observed, and until conversion is aborted, the CADC continues to do conversions on the same CLIST register that initiated the conversion. 
EDMA request will be asserted during an CADC conversion READY event noted when any of the ADC_RDY[RDYn] flags is asserted. EDMA will transfer
CADC results to memory and if users press any key, demo will average CADC results stored in memory and print average on the terminal.  

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini USB cable
- TWR-KV46F150M board
- Personal Computer

Board settings
==============
- To use UART on board, make sure that J505-3 connects to J505-4, J506-3 connects to J506-4.
- Connect an external DC voltage source to ADCA_CH6D (J503-10)

Prepare the Demo
================
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
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
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~
CADC continuous edma conversion.
Press any key to get user channel's ADC value ...
ADC value: 1479
ADC value: 1477
ADC value: 1471
ADC value: 1479
ADC value: 1480
ADC value: 1480

Customization options
=====================

