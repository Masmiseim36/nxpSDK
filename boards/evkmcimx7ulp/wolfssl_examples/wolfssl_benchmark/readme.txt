Overview
========
The wolfssl_benchmark demo application demonstrates cryptographic acceleration on supported platforms that have MMCAU,
LTC or similar accelerator peripheral. The hardware acceleration is enabled by default, and can be configured at build time in
the middleware/wolfssl/wolfssl/wolfcrypt/settings.h header file. FREESCALE_USE_MMCAU, FREESCALE_USE_LTC, FREESCALE_USE_LPC_AES
and FREESCALE_USE_LPC_SHA macros can be used to compile the wolfssl_benchmark project source code with support for
the Freescale hardware cryptographic accelerator, or when none of these macros is defined, all algorithms are implemented in software only.


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

RNG      25 kB took 0.032 seconds,    0.766 MB/s
AES enc  25 kB took 0.005 seconds,    4.875 MB/s
AES dec  25 kB took 0.006 seconds,    4.383 MB/s
AES-GCM  25 kB took 0.090 seconds,    0.273 MB/s
AES-CTR  25 kB took 0.007 seconds,    3.664 MB/s
AES-CCM  25 kB took 0.011 seconds,    2.276 MB/s
3DES     25 kB took 0.029 seconds,    0.846 MB/s

MD5      25 kB took 0.006 seconds,    4.062 MB/s
SHA      25 kB took 0.020 seconds,    1.251 MB/s
SHA-256  25 kB took 0.012 seconds,    2.109 MB/s

RSA 2048 encryption took 134.002 milliseconds, avg over 1 iterations
RSA 2048 decryption took 2212.835 milliseconds, avg over 1 iterations
DH  2048 key generation  983.215 milliseconds, avg over 1 iterations
DH  2048 key agreement   986.362 milliseconds, avg over 1 iterations

ECC  256 key generation  572.630 milliseconds, avg over 5 iterations
EC-DHE   key agreement   567.658 milliseconds, avg over 5 iterations
EC-DSA   sign   time     578.651 milliseconds, avg over 5 iterations
EC-DSA   verify time     1135.858 milliseconds, avg over 5 iterations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

