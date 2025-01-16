Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1160-EVK board
- Personal Computer
- An encoder with PHASE A/B signals, optionally with INDEX signal.

Board settings
============
1. For pin connection between board and encoder,
      J9.4 -> kXBARA1_InputIomuxXbarIn38 -> kXBARA1_OutputEnc1PhaseAInput -> ENC_PHA
      J9.2 -> kXBARA1_InputIomuxXbarIn39 -> kXBARA1_OutputEnc1PhaseBInput -> ENC_PHB
      J9.6 -> kXBARA1_InputIomuxXbarIn40 -> kXBARA1_OutputEnc1Index -> ENC_INDEX


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

