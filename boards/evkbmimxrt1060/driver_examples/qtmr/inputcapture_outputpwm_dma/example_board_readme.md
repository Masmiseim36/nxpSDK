Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1060-EVKB board
- Personal Computer
- Oscilloscope

Board settings
============
- Connect input signal to J33-6.
- Use oscilloscope to measure output the signal pin at J33-5.

Prepare the Demo
===============
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
When the demo runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
****Input capture dma example start.****

****Provide a signal input to the QTMR pin****

Captured Period time = 1000 us (±5%)

****Output PWM dma example.****

*********Make sure to connect an oscilloscope.*********

****A 50 duty cycle PWM wave is observed on an oscilloscope.****

Please enter a value to update the Duty cycle:
Note: The range of value is 1 to 9.
For example: If enter '5', the duty cycle will be set to 50 percent.
Value:3
The duty cycle was successfully updated!

Please enter a value to update the Duty cycle:
Note: The range of value is 1 to 9.
For example: If enter '5', the duty cycle will be set to 50 percent.
Value:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
