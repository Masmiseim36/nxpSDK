Hardware requirements
=====================
- Micro USB cable
- MIMXRT1180-EVK board
- Personal Computer

Board settings
==============
No special is needed.
Use oscilloscope to measure output the 1000Hz PWM signal pin at J44-6 pin of board,you can also
change pwm frequency and duty via terminal.

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
When the example runs successfully, you can see the PWM signal wave on oscilloscope and similar information from the terminal as below. 
~~~~~~~~~~~~~~~~~~~~~
lpit pwm demo start.

Please input pwm frequency and duty:
~~~~~~~~~~~~~~~~~~~~~