Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1010 board
- Personal Computer
- Headphone

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
4. Insert the headphones into the headphone jack on EVK-MIMXRT1010 board.
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
This following section shows how to run the demo:
Notice, on EVK-MIMXRT1010 board, the audio record source is from two microphones. The left channel is from
the headphone microphone, the right channel from the main microphone on board. It is OK for you to choose one
of them or both.
As the headphone microphone may have headphone standard differernt issue, you can just use main microphone
on board as input source.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SAI Demo started!

Please choose the option :
1. Record and playback at same time
2. Playback sine wave
3. Quit
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Known issue:
If pressing SW9 to rerun the demo frequently, it may cause booting from QSPI nor flash failed, since the SA1 RXD0 pin is muxed as boot configurations pin.
Workaround: To use SW3 instead of SW9.

Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
2. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41.
3. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
