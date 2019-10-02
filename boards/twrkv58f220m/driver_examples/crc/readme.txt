Overview
========
The CRC Example project is a demonstration program that uses the KSDK software to generate checksums
for an ASCII string. Several CRC protocols are implemented using the CRC driver API.


Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- GCC ARM Embedded  8.2.1
- MCUXpresso  11.0.0

Hardware requirements
=====================
- J-Link ARM
- TWR-KV58F220M board
- Mini/micro USB cable
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.
Running the demo
================
Example output on terminal:

CRC Peripheral Driver Example

Test string: 123456789
CRC-16 CCIT FALSE: 0x29b1
CRC-16 MAXIM: 0x44c2
CRC-16 KERMIT: 0x2189
CRC-32: 0xcbf43926
CRC-32 POSIX: 0x765e7680
Customization options
=====================

