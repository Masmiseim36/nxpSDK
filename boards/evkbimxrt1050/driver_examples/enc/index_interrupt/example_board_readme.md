Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer
- An encoder with PHASE A/B signals, optionally with INDEX signal.

Board settings
============
1. For pin connection between board and encoder,
      J22.5 -> kXBARA1_InputIomuxXbarIn21 -> kXBARA1_OutputEnc1PhaseAInput -> ENC_PHA
      J22.6 -> kXBARA1_InputIomuxXbarIn22 -> kXBARA1_OutputEnc1PhaseBInput -> ENC_PHB
      J22.3 -> kXBARA1_InputIomuxXbarIn23 -> kXBARA1_OutputEnc1Index -> ENC_INDEX


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

