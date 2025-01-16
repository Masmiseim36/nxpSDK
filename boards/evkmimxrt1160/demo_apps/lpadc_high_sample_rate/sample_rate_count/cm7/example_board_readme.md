Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1160-EVK board
- Personal Computer

Board settings
============
No special settings are required.

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
The log below shows the output of the lpadc sample rate count demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADC High Sample Rate Demo!
Current ADC clock frequency is 88004352 Hz!

Please select sample time!
        A -- 3 ADCK cycles total sample time
        B -- 5 ADCK cycles total sample time
        C -- 7 ADCK cycles total sample time
        D -- 11 ADCK cycles total sample time
        E -- 19 ADCK cycles total sample time
        F -- 35 ADCK cycles total sample time
        G -- 67 ADCK cycles total sample time
        H -- 131 ADCK cycles total sample time
a
Please press any keys to trigger ADC conversion!
Sample Rate: 4187264 SPS

Please select sample time!
        A -- 3 ADCK cycles total sample time
        B -- 5 ADCK cycles total sample time
        C -- 7 ADCK cycles total sample time
        D -- 11 ADCK cycles total sample time
        E -- 19 ADCK cycles total sample time
        F -- 35 ADCK cycles total sample time
        G -- 67 ADCK cycles total sample time
        H -- 131 ADCK cycles total sample time
h
Please press any keys to trigger ADC conversion!
Sample Rate: 590464 SPS

Please select sample time!
        A -- 3 ADCK cycles total sample time
        B -- 5 ADCK cycles total sample time
        C -- 7 ADCK cycles total sample time
        D -- 11 ADCK cycles total sample time
        E -- 19 ADCK cycles total sample time
        F -- 35 ADCK cycles total sample time
        G -- 67 ADCK cycles total sample time
        H -- 131 ADCK cycles total sample time

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
