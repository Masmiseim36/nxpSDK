Hardware requirements
=====================
- Micro USB cable
- MIMXRT1180-EVK  board
- J-Link Debug Probe
- 5V power supply
- Personal Computer
- RS-485 adapter board

Board settings
============
This example requires connecting between FlexIO2 pins and RS-485 adapter board which is connected to the encoder.
- FLEXIO_A_FORMAT_TX_PIN(J41-2) -> RX pin of RS-485
- FLEXIO_A_FORMAT_RX_PIN(J41-4) -> TX pin of RS-485
- FLEXIO_A_FORMAT_DR_PIN(J41-6) -> D/R pin of RS-485
- 3.3V pin(J45-8)               -> 3.3V pin of RS-485
- GND pin(J45-12)               -> GND pin of RS-485
- 5.0V pin(J45-16) -> 5.0V pin of encoder
- GND pin(J45-14)  -> GND pin of encoder
- A pin of RS-485  -> SD+ pin of encoder
- B pin of RS-485  -> SD- pin of encoder

Prepare the Demo
===============
1.  Connect 5V power supply and J-Link Debug Probe to the board, switch SW1 to power on the board.
2.  Connect a micro USB cable between the host PC and the J53 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
Encoder A-format example
Encoder ID: 5946590
Multi-turn data: 64474, single-turn data: 147617
[0.10s] Encoder ID: 0x5ABCDE
         Multi-turn data: 64474, single-turn data: 147617
         Temperature: 45.000000
[0.20s] Encoder ID: 0x5ABCDE
         Multi-turn data: 64474, single-turn data: 147617
         Temperature: 44.000000
~~~~~~~~~~~~~~~~~~~~~
