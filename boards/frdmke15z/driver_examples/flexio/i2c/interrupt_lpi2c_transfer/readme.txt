Overview
========
The flexio_lpi2c_interrupt example shows how to use flexio i2c master  driver in interrupt way:

In this example, a flexio simulated i2c master connect to a LPI2C slave

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- FRDM-KE15Z board
- Personal Computer

Board settings
==============
The flexio_i2c_interrupt example is requires connecting the FLEXIO pins with the I2C pins
The connection should be set as following:
   FLEXIO_I2C        connected to  LPI2C0
SCL     PTE11(J2-15)     -->       PTA3(J4-12)
SDA     PTE10(J2-17)     -->       PTA2(J4-10)

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

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

