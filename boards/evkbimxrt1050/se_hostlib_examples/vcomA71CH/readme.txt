Overview
========

This project allows the Kinetis FRDM-K64F/FRDM-K82F boards to be used as a
bridge between the PC and the secure module and enables the execution of the
config tool and other utilities from the PC

As a system how the integrated systme looks like.  This enables to A71CH
configuration tool from PC or enable porting/development of mbed TLS
with such use cases.

For more information see the See "A71CH examples -> Hostlib API based examples"
in A71CH Host Lib Doxygen Guide.

"A71CH Host Lib Doxygen Guide" is installed to user PC when "A71CH Host Software
Package (Windows Installer)" is installed from https://www.nxp.com/products/:A71CH


    +--------------------+          +---------------------+
    |       PC           |          |   Kinetis (USB)     |
    |--------------------|          |---------------------|
    |                    |          |Test App with USB    |
    | - A71CH Config     |          | VCOM-CDC Interface  |
    |    tool            |          |                     |
    |                    |          |                     |
    |         OR         |          |  +---------------+  |
    |                    |          |  |      APDU     |  |
    |                    |          |  +---------------+  |
    | - mbed TLS test    |   VCOM   |  +---------------+  |
    |    applications    +----------+  |  scmComSCI2C  |  |
    |                    |          |  +---------------+  |
    |                    |          |  +---------------+  |
    |                    |          |  |     sci2c     |  |
    |                    |          |  +---------------+  |
    |                    |          |  +---------------+  |
    |                    |          |  |  i2c_kinetis  |  |
    |                    |          |  +---------------+  |
    |                    |          |  +---------------+  |
    |                    |          |  |     i2c       |  |
    |                    |          |  +---------------+  |
    +--------------------+          +---------+-----------+
                                              |
                                              | I2C Interface
                                              |
                                    +---------+------------+
                                    |                      |
                                    |     A 7 1 C H        |
                                    |                      |
                                    +----------------------+


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer

Board settings
==============
No special settings are required.
Prepare the Demo
================
1. Build the demo
2. Connect a USB cable between the PC host and the OpenSDA USB port on the
   target board.
3. Connect a USB cable between the PC host and the VCOM USB port on the target
   board.
4. Download the program to the target board.
5. Either press the reset button on your board or launch the debugger in your
   IDE to begin running the demo.

Running the demo
================
The config tool or other utilities can now be executed from the PC and the
functionalities of the secure module are invoked via the VCOM bridge.

End of project
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
