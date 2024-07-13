Overview
========
The mbedTLS Benchmark demo application performs cryptographic algorithm benchmarking and prints results to the
terminal.


SDK version
===========
- Version: 2.16.000

Toolchain supported
===================
- GCC ARM Embedded  13.2.1
- Keil MDK  5.39.0
- IAR embedded Workbench  9.60.1
- MCUXpresso  11.10.0

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer

Board settings
==============
No special settings are required.

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
The log below shows the output of the mbedtls benchmark in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
mbedTLS version 3.3.0
Using following implementations:
  SHA: Software implementation
  AES: Software implementation
  AES GCM: Software implementation
  DES: Software implementation
  Asymmetric cryptography: Software implementation

  MD5                      :  7881.20 KB/s,   29.40 cycles/byte
  SHA-1                    :  4469.25 KB/s,   58.23 cycles/byte
  SHA-256                  :  2171.36 KB/s,  107.24 cycles/byte
  SHA-512                  :  1245.45 KB/s,  184.79 cycles/byte
  3DES                     :  740.81 KB/s,  316.41 cycles/byte
  DES                      :  1845.78 KB/s,  126.69 cycles/byte
  AES-CBC-128              :  2218.07 KB/s,  105.00 cycles/byte
  AES-CBC-192              :  1936.30 KB/s,  144.10 cycles/byte
  AES-CBC-256              :  1718.26 KB/s,  159.57 cycles/byte
  AES-XTS-128              :  1535.03 KB/s,  152.47 cycles/byte
  AES-XTS-256              :  1462.79 KB/s,  183.70 cycles/byte
  AES-GCM-128              :  1093.11 KB/s,  212.54 cycles/byte
  AES-GCM-192              :  1024.08 KB/s,  228.17 cycles/byte
  AES-GCM-256              :  954.01 KB/s,  243.81 cycles/byte
  AES-CCM-128              :  777.87 KB/s,  299.78 cycles/byte
  AES-CCM-192              :  698.62 KB/s,  341.35 cycles/byte
  AES-CCM-256              :  570.78 KB/s,  408.41 cycles/byte
  Poly1305                 :  9366.35 KB/s,   24.68 cycles/byte
  CTR_DRBG (NOPR)          :  1465.70 KB/s,  159.36 cycles/byte
  CTR_DRBG (PR)            :  1240.56 KB/s,  185.06 cycles/byte
  HMAC_DRBG SHA-1 (NOPR)   :  331.13 KB/s,  706.09 cycles/byte
  HMAC_DRBG SHA-1 (PR)     :  305.40 KB/s,  761.05 cycles/byte
  HMAC_DRBG SHA-256 (NOPR) :  220.75 KB/s,  1063.50 cycles/byte
  HMAC_DRBG SHA-256 (PR)   :  220.88 KB/s,  1063.01 cycles/byte
  RSA-1024                 :  265.67  public/s
  RSA-1024                 :    4.33 private/s
  DHE-2048                 :    0.50 handshake/s
  DH-2048                  :    0.50 handshake/s
  DHE-3072                 :    0.18 handshake/s
  DH-3072                  :    0.17 handshake/s
  ECDSA-secp521r1          :    7.00 sign/s
  ECDSA-brainpoolP512r1    :    0.67 sign/s
  ECDSA-secp384r1          :   11.33 sign/s
  ECDSA-brainpoolP384r1    :    1.67 sign/s
  ECDSA-secp256r1          :   22.33 sign/s
  ECDSA-secp256k1          :   18.00 sign/s
  ECDSA-brainpoolP256r1    :    3.67 sign/s
  ECDSA-secp224r1          :   31.00 sign/s
  ECDSA-secp224k1          :   21.00 sign/s
  ECDSA-secp192r1          :   42.00 sign/s
  ECDSA-secp192k1          :   26.33 sign/s
  ECDSA-secp521r1          :    2.33 verify/s
  ECDSA-brainpoolP512r1    :    0.33 verify/s
  ECDSA-secp384r1          :    4.00 verify/s
  ECDSA-brainpoolP384r1    :    0.67 verify/s
  ECDSA-secp256r1          :    7.67 verify/s
  ECDSA-secp256k1          :    6.00 verify/s
  ECDSA-brainpoolP256r1    :    1.33 verify/s
  ECDSA-secp224r1          :   10.33 verify/s
  ECDSA-secp224k1          :    7.00 verify/s
  ECDSA-secp192r1          :   14.33 verify/s
  ECDSA-secp192k1          :    8.67 verify/s
  ECDHE-secp521r1          :    1.67 full handshake/s
  ECDHE-brainpoolP512r1    :    0.25 full handshake/s
  ECDHE-secp384r1          :    2.67 full handshake/s
  ECDHE-brainpoolP384r1    :    0.67 full handshake/s
  ECDHE-secp256r1          :    4.33 full handshake/s
  ECDHE-secp256k1          :    3.67 full handshake/s
  ECDHE-brainpoolP256r1    :    1.00 full handshake/s
  ECDHE-secp224r1          :    6.00 full handshake/s
  ECDHE-secp224k1          :    4.33 full handshake/s
  ECDHE-secp192r1          :    8.00 full handshake/s
  ECDHE-secp192k1          :    5.00 full handshake/s
  ECDHE-x25519             :    3.67 full handshake/s
  ECDHE-x448               :    1.67 full handshake/s

