Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- MIMXRT1060-EVKC board
- Personal Computer
- Speaker with 3.5 mm stereo jack
- SD card

Board settings
==============
- Make sure resistors R368/R347/R349/R365/R363 are removed to be able to use SD-Card.
- Make sure J99 is installed.
- Please insert the SDCARD into the card slot in order to record to a VoiceSeeker output.

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the debug USB port on the board
2. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Connect the speaker into the line out jack on MIMXRT1060-EVKC board (J34).
5. Either press the reset button on your board or launch the debugger in your IDE to begin
   running the demo.

