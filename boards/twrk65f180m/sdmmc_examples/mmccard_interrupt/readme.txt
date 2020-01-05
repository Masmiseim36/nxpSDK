Overview
========
The MMCCARD Interrupt project is a demonstration program that uses the SDK software. It reads/writes
/erases the MMC card continusly. The purpose of this example is to show how to use MMCCARD driver and
show how to use interrupt based transfer API in SDK software driver to access MMC card.

Note:
User can use MMC plus card or emmc(on board IC, but not recommand use emmc socket,due to high speed timing restriction)

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K65F180M board
- TWR-MEM board
- Personal Computer

Board settings
==============
J12 5-6 7-8 on TWR-MEM board should be shorted off for 4-bit data bus which can achieve better performance. 

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the example.

Running the demo
================

When the example runs successfully, you can see the similar information from the terminal as below:

MMCCARD interrupt example.

Card user partition size xxxx bytes

Working condition:

  Voltage: VCC - xxxx  VCCQ - xxxx

  Timing mode: xxxx

  Bus width: xxxx

  Freq : xxxx HZ

Read/write the user partition continuously until encounter error......

Write/read one data block......
Compare the read/write content......
The read/write content is consistent.
Write/read multiple data blocks......
Compare the read/write content......
The read/write content is consistent.
Erase data groups......

Input 'q' to quit read/write/erase process.                
Input other char to read/write/erase data blocks again.
Customization options
=====================

