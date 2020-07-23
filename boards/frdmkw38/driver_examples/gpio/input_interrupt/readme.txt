Overview
========
The GPIO Example project is a demonstration program that uses the KSDK software to manipulate the general-purpose
outputs.
The example is supported by the set, clear, and toggle write-only registers for each port output data register. The 
example uses the software button to control/toggle the LED.

Toolchain supported
===================
- IAR embedded Workbench  8.32.4
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM_KW38 board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GPIO Driver example
Connect J14-13 (PTB0) to J7-2 (VCC1V8)/J7-1 (GND) to turn on/off a LED
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
If you press the SW2, the LED RED will be toggled, and "SW2 is pressed" is shown on the terminal window.
Customization options
=====================

