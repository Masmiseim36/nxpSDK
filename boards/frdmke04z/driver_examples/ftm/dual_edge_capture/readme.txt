Overview
========
The FTM project is a demonstration program of the SDK FTM driver's dual-edge capture feature.
This feature is available only on certain SoC's.
The example sets up a FTM channel-pair for dual-edge capture. Once the input signal is received,
this example will print the capture values and period of the input signal on the terminal window.

Toolchain supported
===================
- IAR embedded Workbench  9.10.2
- Keil MDK  5.34

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KE04Z board
- Personal Computer

Board settings
==============
connect input signal to J1-8

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FTM dual-edge capture example
Once the input signal is received the input capture values are printed
The input signal's pulse width is calculated from the capture values & printed

Capture value C(n)V=3

Capture value C(n+1)V=5a8

Input signals pulse width=24 us
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note:
Due to the influence of the size of the MKE04Z8xxxx flash memory, the pulse width accuracy of this example is 1us.
