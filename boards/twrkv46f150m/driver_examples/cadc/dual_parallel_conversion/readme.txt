Overview
========

The cadc_dual_parallel_conversion example shows how to use the both converter parallel at the same time with CADC driver.

In this example, the CADC is configured as "kCADC_DualConverterWorkAsTriggeredParallel" (default) work mode. User should
indicate 4 channel pairs to provide the differential voltage signal (can be controlled by user) as the inputs for CADC
sample slots. These sample slots would be assembled into the two conversion sequence for each converter. 

In "kCADC_DualConverterWorkAsTriggeredParallel" mode, the converter A converts sample 0 and sample 1 (sample 0-7 are
available) and the converter B converts sample 8 and sample 9 (sample 8-15 are available). As the "simultaneous mode" is
enabled by default, the converter A's control logic can operate both the two converters, to start and stop together.

When running the project, typing any key into debug console would trigger the conversion. And then, the conversion 
result data of the four sample slots (Sample 0, 1, 8, 9) would be displayed in the terminal. 
Hardware requirements
=====================
- Mini USB cable
- TWR-KV46F150M board
- Personal Computer

Board settings
==============
To use UART on board, make sure that J505-3 connects to J505-4, J506-3 connects to J506-4

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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CADC dual parallel conversion example.
Press any key to trigger the conversion ...

Sample 0        Sample 1        Sample 8        Sample 9

23808           17080           16784           29824
23792           17360           16784           29800
........................
........................
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

User can press any key to continue the converter and get more converter data. Touching thermistor
on board to change output values.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

