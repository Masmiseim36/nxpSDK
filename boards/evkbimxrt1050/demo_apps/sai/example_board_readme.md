Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
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
4. Insert the headphones into the headphone jack on EVKB-IMXRT1050 board (J12).
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
Note: This demo uses both headphone mic and board main mic(P1) as input source. The headphone mic provides left
channel data, and main mic (P1) provides right channel data. If users found there is noise while do record operation,
most probably it is the headphone standard issue (OMTP and CTIA standard difference). should use the OMTP
standard headphone. You can disable the left channel
by using an headphone without microphone feature.
This following section shows how to run the demo:
Please note that the log print out may be different according to the different card,
If the card doesn't been formatted ,the case will making new file system and below log will be printed,
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SAI Demo started!
Please insert a card into board.

Card inserted.

Make file system......The time may be long if the card capacity is big.

Create directory......
Please choose the option :
1. Record and playback at same time
2. Playback sine wave
3. Record to SDcard, after record playback it
4. Quit
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If the card is formatted already, the case will mount the file system only, the case will print below log,
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SAI Demo started!
Please insert a card into board.

Card inserted.

Mount volume Successfully.

Create directory......
Please choose the option :
1. Record and playback at same time
2. Playback sine wave
3. Record to SDcard, after record playback it
4. Quit
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~