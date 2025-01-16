Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
============


Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-LINK on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TRNG Peripheral Driver Example
Generate 10 random numbers: 
Random[0] = 0x201FEB53
Random[1] = 0x85A32C28
Random[2] = 0xDC66342C
Random[3] = 0xFD43ACFF
Random[4] = 0x1CD53C0
Random[5] = 0xB27EDBEB
Random[6] = 0x32AB81D5
Random[7] = 0x6AA5B70F
Random[8] = 0x81C41286
Random[9] = 0xC9620BD8

 Press any key to continue... 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
