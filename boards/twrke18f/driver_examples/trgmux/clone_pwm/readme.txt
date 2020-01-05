Overview
========
The TRGMUX project is a simple demonstration program of the SDK TRGMUX driver. It generates
a 10KHz PWM by LPIT0 CH0, and clone by TRGMUX to output this PWM to eight TRGMUX_OUTx pins.

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
- An oscilloscope
- Personal Computer

Board settings
==============
The example requires connection between the trgmux pin and oscilloscope.
Connect each of the trgmux output pin with the anode of oscilloscope:
J20-6(TWR-KE18F board) -> Anode of oscilloscope.
Or J20-5(TWR-KE18F board) -> Anode of oscilloscope.
Or J20-3(TWR-KE18F board) -> Anode of oscilloscope.
Or J20-4(TWR-KE18F board) -> Anode of oscilloscope.
Or J20-8(TWR-KE18F board) -> Anode of oscilloscope.
Or J20-7(TWR-KE18F board) -> Anode of oscilloscope.
Or PTA0 (TWR-KE18F board) -> Anode of oscilloscope.
Or PTA1 (TWR-KE18F board) -> Anode of oscilloscope.
GND  (TWR-KE18F board) -> Cathode of oscilloscope.

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
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
When the example runs successfully, you can see the 10KHz PWM on each of the 8 pins.
Customization options
=====================

