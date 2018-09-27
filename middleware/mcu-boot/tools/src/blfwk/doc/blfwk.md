Host Bootloader Framework {#blfwk}
=====

Introduction
-----
The Bootloader Framework is a library of C++ classes implementing communication from a host PC to bootloader firmware running on a target device. Due to the nature of the underlying bootloader command and data protocol, the Bootloader Frameworks supports all target devices without requiring any device-specific knowledge.

Applications
-----
The Bootloader Framework library is used by the Blhost command line tool and by the KinetisFlashTool GUI firmware download tool. It can be built as a library or included as source in a PC or embedded application. The current release includes tool chains to build the library for Windows OS and Mac OS X.

Peripherals
-----
Support for the following PC peripherals is included:

- UART (COM port)
- USB-HID (using custom reports)
- Bus Pal example (UART to I2C/SPI using special hardware)
