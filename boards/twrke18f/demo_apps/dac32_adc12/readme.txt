Overview
========
The dac32_adc12 example shows a simple case of using DAC32 and ADC12. The DAC32 module would output analog signal, then ADC12 module would read this value of this signal and output the voltage value.

User needs to make sure that DAC's output and ADC's input are stable.
Notice:In few cases of this demo,DAC's input and ADC's output might not match,since the voltage signal
is unstable.User has to wait the DAC a few while to make the result as expected in application if necessary.

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
- Primary Elevator

Board settings
==============
The example is requires dac pin connect to adc pin
- A32-Elevator, B33-Elevator connected
- Jumper J16: 2-3 connected, others keep default

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal with these settings:
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
When the demo runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DAC32 ADC12 Demo!
ADC Full Range: XXXX
Press any key to start demo ...

Demo begins...
Select DAC32 output level:
	 1. 1.0 V
	 2. 1.5 V
	 3. 2.0 V
	 4. 2.5 V
	 5. 3.0 V
-->5
ADC Value: 3719
ADC Voltage: 2.996
What next?:
	 1. Test another DAC output value.
	 2. Terminate demo.
-->
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

It is normal if there exists little error between DAC32's output and ADC12's input.

User needs to make sure that DAC's output and ADC's input are stable.
Notice:In few cases of this demo,DAC's input and ADC's output might not match,since the voltage signal
is unstable.User has to wait the DAC a few while to make the result as expected in application if necessary.
Customization options
=====================

