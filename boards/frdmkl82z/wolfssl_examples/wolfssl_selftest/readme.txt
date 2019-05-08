Overview
========
The wolfssl_selftest demo application demonstrates cryptographic acceleration on supported platforms that have either
an MMCAU or LTC peripheral. The hardware acceleration is enabled by default and can be configured at build time in
the middleware/wolfssl/wolfssl/wolfcrypt/settings.h header file. FREESCALE_USE_MMCAU and FREESCALE_USE_LTC macros can
be used to compile the wolfssl_benchmark project source code with support for the Freescale hardware cryptographic
accelerator, or when none of these macros is defined, all algorithms are implemented in software only.

Prepare the Demo
================
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
================
These instructions are displayed/shown on the terminal window:
Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL82Z board
- Personal Computer

Board settings
==============
No special settings are required.


Toolchain supported
===================
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26

