Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT1186 board
- Personal Computer

Board settings
============
1. Rework the board to change the connection of the 3 PIN 0402 JUMPER:
     - R100: Connect to SRAMC_DAT00
     - R373: Connect to SRAMC_DAT01
     - R112: Connect to SRAMC_DAT02
     - R334: Connect to SRAMC_DAT03
     - R321: Connect to SRAMC_DAT04
     - R138: Connect to SRAMC_DAT05
     - R318: Connect to SRAMC_DAT06
     - R303: Connect to SRAMC_DAT07
     - R322: Connect to SRAMC_WE
     - R328: Connect to SRAMC_OEB
     - R391: Connect to SRAMC_ADV
     - R178: Connect to SRAMC_CS0

2. Connect FRDM-IMXRT1186 board 1(SRAM controller) to FRDM-IMXRT1186 board 2(SRAM emulator) as the list:
     Pad (board 1) | SRAMC Signal (board 1) | Header (board 1) | EVK Header (board 2) | Pad (board 2) | FlexIO Pin (board 2)
    | --- | --- | --- | --- | --- | --- |
    | GPIO_EMC_B1_00 | SRAMC_D0   | J61.pin3  | J61.pin3  | GPIO_EMC_B1_00 | FLEXIO1_FLEXIO00
    | GPIO_EMC_B1_01 | SRAMC_D1   | J61.pin4  | J61.pin4  | GPIO_EMC_B1_01 | FLEXIO1_FLEXIO01
    | GPIO_EMC_B1_02 | SRAMC_D2   | J61.pin5  | J61.pin5  | GPIO_EMC_B1_02 | FLEXIO1_FLEXIO02
    | GPIO_EMC_B1_03 | SRAMC_D3   | J61.pin6 | J61.pin6  | GPIO_EMC_B1_03 | FLEXIO1_FLEXIO03
    | GPIO_EMC_B1_04 | SRAMC_D4   | J61.pin7 | J61.pin7  | GPIO_EMC_B1_04 | FLEXIO1_FLEXIO04
    | GPIO_EMC_B1_05 | SRAMC_D5   | J61.pin8 | J61.pin8  | GPIO_EMC_B1_05 | FLEXIO1_FLEXIO05
    | GPIO_EMC_B1_06 | SRAMC_D6   | J61.pin9 | J61.pin9  | GPIO_EMC_B1_06 | FLEXIO1_FLEXIO06
    | GPIO_EMC_B1_07 | SRAMC_D7   | J61.pin10 | J61.pin10 | GPIO_EMC_B1_07 | FLEXIO1_FLEXIO07
    | GPIO_EMC_B1_26 | SRAMC_WE   | J61.pin42  | J61.pin30 | GPIO_EMC_B1_16 | FLEXIO1_FLEXIO16
    | GPIO_EMC_B1_27 | SRAMC_OE_B | J61.pin44  | J61.pin31 | GPIO_EMC_B1_17 | FLEXIO1_FLEXIO17
    | GPIO_EMC_B1_28 | SRAMC_ADV  | J61.pin41  | J61.pin32 | GPIO_EMC_B1_18 | FLEXIO1_FLEXIO18
    | GPIO_EMC_B1_29 | SRAMC_CS0  | J61.pin43 | J61.pin34 | GPIO_EMC_B1_19 | FLEXIO1_FLEXIO19

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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SRAMC Example Start!
Preparing write data...
Writing 256 bytes (8-bit access) to SRAM at 0x43880000...
Write complete.
Reading 256 bytes (8-bit access) from SRAM at 0x43880000...
Read complete.
Comparing written and read data...
SRAMC De-initialized.
--- SRAM Read/Write Test Passed ---

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
