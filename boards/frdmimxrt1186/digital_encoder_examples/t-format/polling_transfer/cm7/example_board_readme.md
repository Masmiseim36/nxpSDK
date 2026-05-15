Hardware requirements
=====================
- USB Type-C cable
- FRDM-IMXRT1186 board
- 5V power supply
- Personal Computer
- FRDM-LVPMSM-FA board

Board settings
============
This example requires connecting between FRDM-IMXRT1186 board and FRDM-LVPMSM-FA board.
- Short J300-1 and J300-2 on the FRDM-LVPMSM-FA board
- FRDM-LVPMSM-FA SW90[1-4] = 0111
- FRDM-LVPMSM-FA SW30[1-4] = 0100
- FRDM-LVPMSM-FA J70-1 -> 5.0V pin of encoder
- FRDM-LVPMSM-FA J70-6 -> GND pin of encoder
- FRDM-LVPMSM-FA J70-4 -> SD+ pin of encoder
- FRDM-LVPMSM-FA J70-9 -> SD- pin of encoder

Prepare the Demo
===============
1. Connect a USB Type-C cable between the PC host and the OpenSDA USB port on the FRDM-IMXRT1186 board.
2. Connect 5V power supply, then switch SW5 to power on the FRDM-IMXRT1186 board.
3. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4. Download the program to the target board.
5. Either re-power up your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
Encoder T-format example
Encoder ID: 5946590
Multi-turn data: 64474, single-turn data: 147617
[0.10s] Encoder ID: 0x5ABCDE
         Multi-turn data: 64474, single-turn data: 147617
         Temperature: 45.000000
[0.20s] Encoder ID: 0x5ABCDE
         Multi-turn data: 64474, single-turn data: 147617
         Temperature: 44.000000
~~~~~~~~~~~~~~~~~~~~~
