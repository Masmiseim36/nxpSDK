Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT1186 board
- Personal Computer
- Oscilloscope

Board settings
==============
Use oscilloscope to measure the output 48KHz PWM signal pin at J1-6 pin.

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
When the example runs successfully, you can see the similar information from the terminal as below.
~~~~~~~~~~~~
FLEXIO_PWM demo start.
~~~~~~~~~~~~
