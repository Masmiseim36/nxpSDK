Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
============
This example project uses the ACMP to compare with the voltage signal from external input(PIO10_8) J3-28 or J7-9. 
Please keep the external input voltage signal stable to avoid floating voltage.
The ACMP positive input voltage is from the internal DAC output, in this example, the internal DAC reference voltage is from
the internal 1.8v PAD and the DAC output voltage is half of the reference. 

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the MCU-LINK USB port on the target board. 
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
When the demo runs successfully, the following information can be seen on the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The example compares analog input to the reference DAC output(CMP positive port).
The terminal will print CMP's output value when press any key.
Please press any key to get CMP's output value.
The analog input is LOWER than DAC output
The analog input is HIGHER than DAC output
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
