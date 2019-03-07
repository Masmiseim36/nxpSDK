Overview
========
The wolfssl_selftest demo application demonstrates cryptographic acceleration on supported platforms that have either
an MMCAU or LTC peripheral. The hardware acceleration is enabled by default and can be configured at build time in
the middleware/wolfssl/wolfssl/wolfcrypt/settings.h header file. FREESCALE_USE_MMCAU and FREESCALE_USE_LTC macros can
be used to compile the wolfssl_benchmark project source code with support for the Freescale hardware cryptographic
accelerator, or when none of these macros is defined, all algorithms are implemented in software only.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- GCC ARM Embedded  7.3.1

Hardware requirements
=====================
- Micro USB cable
- MCIMX7ULP-EVK board
- J-Link Debug Probe
- 5V power supply
- Personal Computer

Board settings
==============
No special settings are required.

#### Please note this application can't support running with Linux BSP! ####

#### Please note that this application must be built with flash link file! Build with RAM link
file might fail due to shortage of RAM size. ####

Prepare the Demo
================
1.  Connect 5V power supply and J-Link Debug Probe to the board, switch SW1 to power on the board.
2.  Connect a micro USB cable between the host PC and the J6 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
fsys=115200000
Using following implementations:
  SHA: MMCAU HW accelerated
  AES: LTC HW accelerated
  AES GCM: MMCAU HW accelerated
  DES: MMCAU HW accelerated
  Asymmetric encryption: Software implementation

MD5      test passed!
MD4      test passed!
SHA      test passed!
SHA-256  test passed!
HMAC-MD5 test passed!
HMAC-SHA test passed!
HMAC-SHA256 test passed!
GMAC     test passed!
HC-128   test passed!
DES      test passed!
DES3     test passed!
AES      test passed!
AES-GCM  test passed!
AES-CCM  test passed!
RANDOM   test passed!
RSA      test passed!
DH       test passed!
DSA      test passed!
PWDBASED test passed!
ECC      test passed!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

