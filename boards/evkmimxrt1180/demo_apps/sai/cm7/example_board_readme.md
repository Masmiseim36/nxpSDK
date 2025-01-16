Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer
- Headphone(OMTP standard)
- SD card

Board settings
============
1. Insert headphone to J101.
2. Insert SD card to card slot J15.

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
4.  Press the reset button on your board to begin running the demo.

Note
===============
The CM7 demo is supposed to work with multicore_trigger CM33 demo in this SDK.

Running the demo
================
When the demo runs successfully, you can hear the sound and the log would be seen on the OpenSDA terminal like:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SAI Demo started!

Please insert a card into board.

Card inserted.
Mount volume Successfully.

Create directory......
Directory exists.

Please choose the option :
1. Record and playback at same time
2. Playback sine wave
3. Record to SDcard, after record playback it
4. Quit
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Note: 
  1. log "Directory exists." is optional according to the content in the SD card.
  2. This demo uses both headphone mic and board main mic(P1) as input source. The headphone mic provides left
     channel data, and main mic (P1) provides right channel data. If users found there is noise while do record operation,
     most probably it is the headphone standard issue (OMTP and CTIA standard difference). It requires to use the OMTP
     standard headphone. You can disable the left channel by using an headphone without microphone feature.
