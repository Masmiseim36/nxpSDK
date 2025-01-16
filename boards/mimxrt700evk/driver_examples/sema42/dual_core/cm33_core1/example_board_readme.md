Hardware requirements
===================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.

Prepare the demo
===============
1.  Connect a USB cable between the PC host and the MCU-LINK USB port on the board.
2.  Open a serial terminal on PC for MCU-LINK serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build core1 project first, then build core0 project.
4.  Download the core0 program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the MCU-LINK terminal like below
and Red LED will turn on.

~~~~~~~~~~~~~~~~~~~~~
Sema42 example!

Press any key to unlock semaphore and Core 1 will turn off the LED
~~~~~~~~~~~~~~~~~~~~~

When press any key in the terminal, the LED will be turned off and the terminal tells

~~~~~~~~~~~~~~~~~~~~~
Now the LED should be turned off

Sema42 example succeed!
~~~~~~~~~~~~~~~~~~~~~