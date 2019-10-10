Overview
========
The flexio_lpi2c_interrupt example shows how to use flexio i2c master  driver in interrupt way:

In this example, a flexio simulated i2c master connect to a LPI2C slave

Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- GCC ARM Embedded  8.2.1

Hardware requirements
=====================
- Mini USB cable
- FRDM-K32L3A6 board
- Personal Computer

Board settings
==============
To make lpi2c example work(M4), connections needed to be as follows:
   FLEXIO_I2C        connected to  LPI2C0
SCL     PTE13(J1-1)    -->        PTC10(J2-20)
SDA     PTE14(J1-3)     -->       PTC9(J2-18)
To make lpi2c example work(M0), connections needed to be as follows:
   FLEXIO_I2C        connected to  LPI2C3
SCL     PTE13(J1-1)    -->        PTB15(J3-15)
SDA     PTE14(J1-3)     -->       PTB16(J3-13)
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
You can see the similar message shows following in the terminal if the example runs successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FlexIO I2C interrupt - LPI2C interrupt
Master will send data :
0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07
0x08  0x09  0x0A  0x0B  0x0C

Slave received data :
0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07
0x08  0x09  0x0A  0x0B  0x0C
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

