Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1040-EVK board
- Personal Computer
- Headphone(OMTP standard)

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
4. Insert the headphones into the headphone jack on MIMXRT1040-EVK board (J34).
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
Note: This demo uses both headphone mic and board main mic(P1) as input source. The headphone mic provides left
channel data, and main mic (P1) provides right channel data. If users found there is noise while do record operation,
most probably it is the headphone standard issue (OMTP and CTIA standard difference). You should use the OMTP
standard headphone. You can disable the left channel by using an headphone without microphone feature.
This demo uses codec master mode(external mclk mode).

When the demo runs successfully, you can hear the tone and the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~
SAI example started!
~~~~~~~~~~~~~~~~~~~

Note:
Examples can't be downloaded again after dowmloading this case by MCUXpresso IDE. Please erase flash by serial download mode(SW7:0001b). This issue will be fixed in later release.
