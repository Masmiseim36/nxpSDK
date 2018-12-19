Overview
========
The wolfssl_selftest demo application demonstrates cryptographic acceleration on supported platforms that have either
an MMCAU or LTC peripheral. The hardware acceleration is enabled by default and can be configured at build time in
the middleware/wolfssl/wolfssl/wolfcrypt/settings.h header file. FREESCALE_USE_MMCAU and FREESCALE_USE_LTC macros can
be used to compile the wolfssl_benchmark project source code with support for the Freescale hardware cryptographic
accelerator, or when none of these macros is defined, all algorithms are implemented in software only.

Prepare the Demo
================
Connect a serial cable from the debug UART port of the board to the PC. Start TeraTerm
(http://ttssh2.osdn.jp) and make a connection to the virtual serial port.

1. Start TeraTerm.

2. New connection -> Serial.

3. Set the appropriate COMx port (where x is port number) in the port context menu. The number is provided by operation
   system and could be different from computer to computer. Select the COM number related to the virtual
   serial port. Confirm the selected port by clicking the "OK" button.

4. Set the following connection parameters in menu Setup -> Serial port.
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: none

5.  Confirm the selected parameters by clicking the "OK" button.

Running the demo
================
The demo prints the system clock frequency and actual hardware acceleration configuration to the terminal,
 then executes cryptographic algorithms. The results are printed to the terminal.
Actual run times may vary significantly depending on the device, its available hardware acceleration modules,
system clock frequency and the compiler used. For software implementations of asymmetric encryption on low performance
MCUs, such as ARM Cortex M0+ @ 40 MHz, the complete tests can take up to 20 minutes.
The middleware/wolfssl/wolfssl/wolfcrypt/settings.h header file can be used to enable or disable algorithms of interest at compile time.


Example terminal output:

fsys=120000000
Using following implementations:
  SHA: MMCAU HW accelerated
  AES: LTC HW accelerated
  AES GCM: LTC HW accelerated
  DES: LTC HW accelerated
  Asymmetric encryption: LTC HW accelerated

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
Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1020 board
- Personal Computer

Board settings
==============
No special settings are required.

Known issue
The example occasionally fails on EVK-MIMXRT1020 board with "ecc_test_curve_size 64 failed!: -1023" error.

Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

