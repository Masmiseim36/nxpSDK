Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer
- Multimeter

Board settings
============
- Connect J10 pin 1-2.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================   
The log below shows in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Press any key to trigger the DAC...
DAC next output: 0
DAC next output: 100
DAC next output: 200
DAC next output: 300
DAC next output: 400
DAC next output: 500
DAC next output: 600
DAC next output: 700
DAC next output: 800
DAC next output: 900
DAC next output: 1000


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Then user can measure the DAC0 output pin(TP9) to check responding voltage = 1.8v * 1000 / 4096