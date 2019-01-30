Overview
========

The cadc_sequential_conversion example shows how to use a long conversion sequence (can include as many as all the 16 
sample slots) with CADC driver.

In this example, the CADC is configured as "kCADC_DualConverterWorkAsTriggeredSequential" work mode. User should 
indicate 4 channel pairs to provide the differential voltage signal (can be controlled by user) as the inputs for CADC
sample slots. These sample slots would be assembled into a long conversion sequence.

In "kCADC_DualConverterWorkAsTriggeredSequential" mode, all the sample slots are organized as a long conversion 
sequence. The converter A's control logic would be used to operate this long conversion sequence. It can start and stop
the sequence.

When running the project, typing any key into debug console would trigger the conversion. And then, the conversion 
result data of the four sample slots (Sample 0, 1, 2, 3) would be displayed in the terminal. 

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

~~~~~~~~~~~~~~~~~~~~~~
CADC sequential conversion example.
Press any key to trigger the conversion ...

Sample 0        Sample 1        Sample 2        Sample 3

23920           17088           16808           29760
23896           17552           16800           29352
23920           17840           16792           29152
23912           18088           16800           29120
............
............
~~~~~~~~~~~~~~~~~~~~~

User can press any key to continue the converter and get more converter data. Touching thermistor
on board to change output values.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

