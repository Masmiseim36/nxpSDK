Overview
========
The flexio_lpi2c_interrupt example shows how to use flexio i2c master  driver in interrupt way:

In this example, a flexio simulated i2c master connect to a LPI2C slave

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
- USB-to-Serial converter

Board settings
==============
The flexio_i2c_interrupt example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 related jumpers connected

The flexio_i2c_interrupt example is configured to use FLEXIO D2 and D3 with PTC10 and PTC11 pins to simulate i2c master
and communicate with the on board LPI2C0 slave.
PTC10: FLEXIO_I2C_SCL_PIN, CN9D-82
PTC11: FLEXIO_I2C_SDA_PIN, CN9D-83

To make lpi2c example work, connections on MAPS-KS22F256 board need to be setup as follows:
- PTB0(CN9C-53) --> PTC10(CN9D-82);
- PTB1(CN9C-54) --> PTC11(CN9D-83);
- JP13 Connected
- JP14 Connected

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
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
FlexIO I2C interrupt - LPI2C interrupt

Master will send data :

0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07

0x08  0x09  0x0A  0x0B  0x0C

Slave received data :

0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07

0x08  0x09  0x0A  0x0B  0x0C
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

