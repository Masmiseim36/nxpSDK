Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1020 board
- Personal Computer
- An encoder with PHASE A/B signals.

Board settings
============
Remove R59 

For pin connection between board and encoder
      J17.2 -> kXBARA1_InputIomuxXbarInout12 -> kXBARA1_OutputEnc1PhaseAInput -> ENC_PHA
      J17.1 -> kXBARA1_InputIomuxXbarInout13 -> kXBARA1_OutputEnc1PhaseBInput -> ENC_PHB
      U1F-46 -> kXBARA1_InputIomuxXbarInout16 -> kXBARA1_OutputEnc1Index -> ENC_INDEX


Prepare the Demo
===============
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Connect the wires between encoder and the MCU board. See to the code for pin mux setting in function "BOARD_InitPins()".
3.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
${ANCHOR}


Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
2. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
3. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
