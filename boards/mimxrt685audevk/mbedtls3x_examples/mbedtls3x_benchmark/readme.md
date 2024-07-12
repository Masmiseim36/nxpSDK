Overview
========
The mbedTLS Benchmark demo application performs cryptographic algorithm benchmarking and prints results to the
terminal.


SDK version
===========
- Version: 2.16.000

Toolchain supported
===================
- Keil MDK  5.39.0
- IAR embedded Workbench  9.60.1
- GCC ARM Embedded  13.2.1
- MCUXpresso  11.10.0

Hardware requirements
=====================
- Micro USB cable
- MIMXRT685-AUD-EVK board
- Personal Computer

Board settings
==============
No special settings are required.
Prepare the Demo
================
Connect a serial cable from the debug UART port of the target board to the PC. Start TeraTerm (http://ttssh2.osdn.jp)
and make a connection to the virtual serial port.

1. Start TeraTerm.

2. New connection -> Serial.

3. Set the appropriate COMx port (where x is port number) in the port context menu. The number is provided by the operating
   system, and could be different from computer to computer. Select the COM number related to the virtual
   serial port. Confirm the selected port by clicking the "OK" button.

4. Set following connection parameters in menu Setup -> Serial port.
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: none

5.  Confirm selected parameters by clicking the "OK" button.

Running the demo
================
When the demo runs successfully, which takes a couple of minutes, the terminal displays similar information like the following:
mbedTLS version 3.5.0
Using following implementations:
  SHA: Software implementation
  AES: Software implementation
  AES GCM: Software implementation
  DES: Software implementation
  Asymmetric cryptography: Software implementation

  MD5                      :  1752.27 KB/s,   83.38 cycles/byte
  SHA-1                    :  769.59 KB/s,  190.32 cycles/byte
  SHA-256                  :  460.13 KB/s,  318.77 cycles/byte
  SHA-512                  :  240.20 KB/s,  612.11 cycles/byte
  3DES                     :  110.34 KB/s,  1339.36 cycles/byte
  DES                      :  254.40 KB/s,  577.75 cycles/byte
  3DES-CMAC                :  103.71 KB/s,  1425.88 cycles/byte
  AES-CBC-128              :  366.13 KB/s,  400.90 cycles/byte
  AES-CBC-192              :  313.44 KB/s,  468.55 cycles/byte
  AES-CBC-256              :  274.04 KB/s,  536.21 cycles/byte
  AES-XTS-128              :  333.94 KB/s,  439.70 cycles/byte
  AES-XTS-256              :  254.70 KB/s,  577.11 cycles/byte
  AES-GCM-128              :  240.26 KB/s,  611.94 cycles/byte
  AES-GCM-192              :  216.12 KB/s,  680.65 cycles/byte
  AES-GCM-256              :  196.31 KB/s,  749.68 cycles/byte
  AES-CCM-128              :  167.40 KB/s,  879.98 cycles/byte
  AES-CCM-192              :  144.94 KB/s,  1017.40 cycles/byte
  AES-CCM-256              :  127.81 KB/s,  1154.82 cycles/byte
  AES-CMAC-128             :  335.16 KB/s,  438.08 cycles/byte
  AES-CMAC-192             :  289.74 KB/s,  507.03 cycles/byte
  AES-CMAC-256             :  255.25 KB/s,  575.85 cycles/byte
  AES-CMAC-PRF-128         :  334.76 KB/s,  438.62 cycles/byte
  Poly1305                 :  2389.29 KB/s,   61.06 cycles/byte
  CTR_DRBG (NOPR)          :  262.83 KB/s,  559.17 cycles/byte
  CTR_DRBG (PR)            :  196.12 KB/s,  750.43 cycles/byte
  HMAC_DRBG SHA-1 (NOPR)   :   56.74 KB/s,  2627.69 cycles/byte
  HMAC_DRBG SHA-1 (PR)     :   52.79 KB/s,  2828.15 cycles/byte
  HMAC_DRBG SHA-256 (NOPR) :   49.23 KB/s,  3036.23 cycles/byte
  HMAC_DRBG SHA-256 (PR)   :   49.23 KB/s,  3036.24 cycles/byte
  RSA-1024                 :   78.67  public/s
  RSA-1024                 :    2.67 private/s
  DHE-2048                 :    0.20 handshake/s
  DH-2048                  :    0.18 handshake/s
  DHE-3072                 :    0.06 handshake/s
  DH-3072                  :    0.06 handshake/s
  ECDSA-secp521r1          :    2.33 sign/s
  ECDSA-brainpoolP512r1    :    0.22 sign/s
  ECDSA-secp384r1          :    3.67 sign/s
  ECDSA-brainpoolP384r1    :    0.50 sign/s
  ECDSA-secp256r1          :    7.00 sign/s
  ECDSA-secp256k1          :    6.33 sign/s
  ECDSA-brainpoolP256r1    :    1.00 sign/s
  ECDSA-secp224r1          :   10.00 sign/s
  ECDSA-secp224k1          :    7.33 sign/s
  ECDSA-secp192r1          :   13.00 sign/s
  ECDSA-secp192k1          :    9.00 sign/s
  ECDSA-secp521r1          :    1.00 verify/s
  ECDSA-brainpoolP512r1    :    0.11 verify/s
  ECDSA-secp384r1          :    1.67 verify/s
  ECDSA-brainpoolP384r1    :    0.22 verify/s
  ECDSA-secp256r1          :    2.67 verify/s
  ECDSA-secp256k1          :    2.33 verify/s
  ECDSA-brainpoolP256r1    :    0.50 verify/s
  ECDSA-secp224r1          :    3.67 verify/s
  ECDSA-secp224k1          :    2.67 verify/s
  ECDSA-secp192r1          :    4.67 verify/s
  ECDSA-secp192k1          :    3.33 verify/s
  ECDHE-secp521r1          :    0.60 full handshake/s
  ECDHE-brainpoolP512r1    :    0.09 full handshake/s
  ECDHE-secp384r1          :    1.00 full handshake/s
  ECDHE-brainpoolP384r1    :    0.18 full handshake/s
  ECDHE-secp256r1          :    1.67 full handshake/s
  ECDHE-secp256k1          :    1.67 full handshake/s
  ECDHE-brainpoolP256r1    :    0.40 full handshake/s
  ECDHE-secp224r1          :    2.33 full handshake/s
  ECDHE-secp224k1          :    1.67 full handshake/s
  ECDHE-secp192r1          :    3.00 full handshake/s
  ECDHE-secp192k1          :    2.00 full handshake/s
  ECDHE-x25519             :    1.33 full handshake/s
  ECDHE-x448               :    0.60 full handshake/s
