Overview
========
The mbdedTLS Benchmark demo application performs cryptographic algorithm benchmarking and prints results to the
terminal.


Toolchain supported
===================
- GCC ARM Embedded  8.2.1
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K32L3A6 board
- Personal Computer

Board settings
==============
No special settings are required.
If download M0+ core project, need to let MCU boot from M0+ core, please follow below steps:
1. Download blhost.exe from www.nxp.com/kboot.
2. Connect J8 on baord to PC using USB cable.
3. After PC recognize the USB HID device, go to blhost.exe folder, open command line.
4. Run command "blhost.exe -u -- flash-erase-all-unsecure", it will erase the flash on chip.
5. Run command "blhost.exe -u -- flash-program-once 0x84 4 ffffffbf", set FOPT3 to boot from M0+ core.

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
When the demo runs successfully, which takes a couple of minutes, the terminal displays similar information like the following:

mbedTLS version 2.12.0
fsys=120000000
Using following implementations:
  SHA: MMCAU HW accelerated
  AES: MMCAU HW accelerated
  AES GCM: MMCAU HW accelerated
  DES: MMCAU HW accelerated
  Asymmetric encryption: Software implementation


  MD5                      :  6505.98 KB/s,   17.62 cycles/byte
  SHA-1                    :  3688.66 KB/s,   31.38 cycles/byte
  SHA-256                  :  2450.96 KB/s,   47.43 cycles/byte
  SHA-512                  :  451.70 KB/s,  259.62 cycles/byte
  3DES                     :  849.94 KB/s,  137.65 cycles/byte
  DES                      :  1740.01 KB/s,   66.99 cycles/byte
  AES-CBC-128              :  2099.88 KB/s,   55.42 cycles/byte
  AES-CBC-192              :  1999.21 KB/s,   58.24 cycles/byte
  AES-CBC-256              :  1917.44 KB/s,   60.74 cycles/byte
  AES-GCM-128              :  518.11 KB/s,  226.22 cycles/byte
  AES-GCM-192              :  511.61 KB/s,  229.08 cycles/byte
  AES-GCM-256              :  505.44 KB/s,  231.92 cycles/byte
  AES-CCM-128              :  699.82 KB/s,  167.29 cycles/byte
  AES-CCM-192              :  676.79 KB/s,  173.01 cycles/byte
  AES-CCM-256              :  655.23 KB/s,  178.73 cycles/byte
  CTR_DRBG (NOPR)          :  2181.05 KB/s,   53.36 cycles/byte
  CTR_DRBG (PR)            :  1484.93 KB/s,   78.58 cycles/byte
  HMAC_DRBG SHA-1 (NOPR)   :  244.85 KB/s,  480.17 cycles/byte
  HMAC_DRBG SHA-1 (PR)     :  226.67 KB/s,  518.88 cycles/byte
  HMAC_DRBG SHA-256 (NOPR) :  268.80 KB/s,  437.21 cycles/byte
  HMAC_DRBG SHA-256 (PR)   :  235.97 KB/s,  498.34 cycles/byte
  RSA-1024                 :   94.00  public/s
  RSA-1024                 :    3.00 private/s
  DHE-2048                 :    0.18 handshake/s
  DH-2048                  :    0.18 handshake/s
  ECDSA-secp384r1          :    1.67 sign/s
  ECDSA-secp256r1          :    2.67 sign/s
  ECDSA-secp224r1          :    4.33 sign/s
  ECDSA-secp192r1          :    5.33 sign/s
  ECDSA-secp384r1          :    1.00 verify/s
  ECDSA-secp256r1          :    1.67 verify/s
  ECDSA-secp224r1          :    2.33 verify/s
  ECDSA-secp192r1          :    3.00 verify/s
  ECDHE-secp384r1          :    1.00 handshake/s
  ECDHE-secp256r1          :    1.67 handshake/s
  ECDHE-secp224r1          :    2.67 handshake/s
  ECDHE-secp192r1          :    3.33 handshake/s
  ECDH-secp384r1           :    2.00 handshake/s
  ECDH-secp256r1           :    3.00 handshake/s
  ECDH-secp224r1           :    4.67 handshake/s
  ECDH-secp192r1           :    6.00 handshake/s
Customization options
=====================

