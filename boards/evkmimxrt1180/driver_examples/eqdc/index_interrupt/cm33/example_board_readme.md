Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1180 board
- Personal Computer
- An encoder with PHASE A/B signals.

Board settings
============
1. For pin connection between board and encoder,
      J44.6 -> kXBAR1_InputRESERVED22 -> kXBAR1_OutputEnc1Phasea -> EQDC_PHA
      J44.8 -> kXBAR1_InputRESERVED20 -> kXBAR1_OutputEnc1Phaseb -> EQDC_PHB
      J39.10 -> kXBAR1_InputRESERVED18 -> kXBAR1_OutputEnc1Index -> EQDC_INDEX


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
