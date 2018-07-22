Overview
========
The SAI Demo application demonstrates digital audio playback. The demo performs audio playback from a .wav
file repeatedly, stored in on-chip flash or some other memory. The repeat time decided by MUSIC_LEN/DATA_LEN in music.h
file.The project also uses the CMSIS-DSP library to perform a Fast Fourier Transform, and to return the fundamental 
frequency of the audio.

Notice: As the audio data stored is very short, this demo playbacked the audio data repeatedly, there is a slight
periodic glitch, it is not a noise, just because of repeatdely playback, the end of audio data is not continous with
the begin data.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K22F120M board
- Personal Computer
- TWR-SGTL5000
- Elevator Tower
- Headphones

Board settings
==============
Insert all boards into the primary elevator tower.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Connect the headphones to the headphone jack on the TWR-SGTL5000 board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, you can hear the tone and the following log is visible on the OpenSDA terminal:

~~~~~~~~~~~~~~~~~~~
SAI example started!
SAI example finished!
~~~~~~~~~~~~~~~~~~~
Customization options
=====================

