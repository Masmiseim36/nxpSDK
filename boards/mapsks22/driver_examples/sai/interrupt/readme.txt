Overview
========
The sai_interrupt example shows how to use sai fucntional API to implement interrupt playback:

In this example, one sai instance playbacks the audio data stored in flash/SRAM using interrupt.

Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- MAPS-KS22F256 board & MAPS-DOCK board
- Personal Computer
- Headphone

Board settings
==============
The sai_interrupt example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 related jumpers connected

The flexio i2s example needs to connect to WM8960 codec, in MAPS-KS22F256 board(Yellow board), the jumper settings
should be:
- JP5 : 2-3
- JP6 : 2-3
- JP7 : 2-3
- JP8 : 2-3
- JP13 Connected
- JP14 Connected

On MAPS-DOCK board:
- JP1 all on

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the USB port (CN14) on MAPS-DOCK board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
When the demo runs successfully, you can hear the tone and the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~
SAI example started!
SAI example finished!
 ~~~~~~~~~~~~~~~~~~~

Notice: Flexio can not have Mater clock connected to WM8960 codec in MAPS board sue to hardware limit.
As WM8960 need  a master clock to work,this example uses SAI master clock to enable WM8960.
As the clock from flexio is not accurate and have some mismatch with SAI master clock,
so the output voice may have some noise.
Customization options
=====================

