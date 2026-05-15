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
- FRDM-LVPMSM-FA SW90[1-4] = 1001
- FRDM-LVPMSM-FA SW30[1-4] = 0100
- FRDM-LVPMSM-FA J70-1 -> VCC pin of encoder
- FRDM-LVPMSM-FA J70-6 -> GND pin of encoder
- FRDM-LVPMSM-FA J70-2 -> CLK+ pin of encoder
- FRDM-LVPMSM-FA J70-7 -> CLK- pin of encoder
- FRDM-LVPMSM-FA J70-3 -> Data+ pin of encoder
- FRDM-LVPMSM-FA J70-8 -> Data- pin of encoder

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
Encoder BiSS example
FlexIO Root Clock is 80 MHz
The initial frame format:
head=4; mt=12; st=16; erWr=2; crc=6; stop=1; cdm=4; data=30; total=41; whole=45
CDM Value = FFFFFFFFFC02AAAA
[OK] Shifer1 = 0xB7C46BA0; Shifter0 = 0x00001C50
head=8; mt=12; st=16; erWr=2; crc=6; stop=1; cdm=4; data=30; total=45; whole=49

******************** Running the loop test ********************
    ====
Shifer1 = 0x7C46BA20; Shifter0 = 0x0001C50B
  CRC     = 0x11; CRC_Cal = 0x11
  head    = 0xE2
  CRCdata = 0x216F88D7
  ST      = 0x0000E235
  MT      = 0x085B
    ====
Shifer1 = 0x7C46BA20; Shifter0 = 0x0001C50B
  CRC     = 0x11; CRC_Cal = 0x11
  head    = 0xE2
  CRCdata = 0x216F88D7
  ST      = 0x0000E235
  MT      = 0x085B
~~~~~~~~~~~~~~~~~~~~~
