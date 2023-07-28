Overview
========
The mbedTLS Benchmark demo application performs cryptographic algorithm benchmarking and prints results to the
terminal.


Toolchain supported
===================
- Keil MDK  5.38.1
- IAR embedded Workbench  9.40.1
- GCC ARM Embedded  12.2
- MCUXpresso  11.8.0

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

mbedTLS version 3.1.0
fsys=150000000
Using following implementations:
  SHA: Software implementation
  AES: Software implementation
  AES GCM: Software implementation
  DES: Software implementation
  Asymmetric cryptography: Software implementation

  MD5                      :  2258.14 KB/s,   64.59 cycles/byte
  SHA-1                    :  1360.04 KB/s,  107.47 cycles/byte
  SHA-256                  :  520.66 KB/s,  281.57 cycles/byte
  SHA-512                  :  371.10 KB/s,  395.51 cycles/byte
  3DES                     :   96.24 KB/s,  1537.60 cycles/byte
  DES                      :  268.51 KB/s,  547.24 cycles/byte
  AES-CBC-128              :  648.28 KB/s,  225.95 cycles/byte
  AES-CBC-192              :  565.01 KB/s,  259.36 cycles/byte
  AES-CBC-256              :  498.42 KB/s,  294.12 cycles/byte
  AES-XTS-128              :  549.83 KB/s,  266.57 cycles/byte
  AES-XTS-256              :  436.78 KB/s,  335.82 cycles/byte
  AES-GCM-128              :  286.04 KB/s,  513.60 cycles/byte
  AES-GCM-192              :  268.72 KB/s,  546.82 cycles/byte
  AES-GCM-256              :  252.49 KB/s,  582.15 cycles/byte
  AES-CCM-128              :  276.64 KB/s,  531.11 cycles/byte
  AES-CCM-192              :  246.00 KB/s,  597.58 cycles/byte
  AES-CCM-256              :  220.11 KB/s,  668.20 cycles/byte
  Poly1305                 :  2493.92 KB/s,   58.43 cycles/byte
  CTR_DRBG (NOPR)          :  485.39 KB/s,  302.09 cycles/byte
  CTR_DRBG (PR)            :  359.21 KB/s,  408.59 cycles/byte
  HMAC_DRBG SHA-1 (NOPR)   :   97.81 KB/s,  1512.79 cycles/byte
  HMAC_DRBG SHA-1 (PR)     :   90.86 KB/s,  1629.72 cycles/byte
  HMAC_DRBG SHA-256 (NOPR) :   56.45 KB/s,  2640.82 cycles/byte
  HMAC_DRBG SHA-256 (PR)   :   56.45 KB/s,  2640.82 cycles/byte
  RSA-1024                 :   54.33  public/s
  RSA-1024                 :    2.33 private/s
  DHE-2048                 :    0.13 handshake/s
  DH-2048                  :    0.13 handshake/s
  DHE-3072                 :    0.04 handshake/s
  DH-3072                  :    0.04 handshake/s
  ECDSA-secp521r1          :    1.67 sign/s
  ECDSA-brainpoolP512r1    :    0.25 sign/s
  ECDSA-secp384r1          :    3.33 sign/s
  ECDSA-brainpoolP384r1    :    0.50 sign/s
  ECDSA-secp256r1          :    6.33 sign/s
  ECDSA-secp256k1          :    5.67 sign/s
  ECDSA-brainpoolP256r1    :    1.33 sign/s
  ECDSA-secp224r1          :    9.33 sign/s
  ECDSA-secp224k1          :    7.33 sign/s
  ECDSA-secp192r1          :   12.00 sign/s
  ECDSA-secp192k1          :    9.00 sign/s
  ECDSA-secp521r1          :    0.75 verify/s
  ECDSA-brainpoolP512r1    :    0.12 verify/s
  ECDSA-secp384r1          :    1.33 verify/s
  ECDSA-brainpoolP384r1    :    0.25 verify/s
  ECDSA-secp256r1          :    2.33 verify/s
  ECDSA-secp256k1          :    2.00 verify/s
  ECDSA-brainpoolP256r1    :    0.50 verify/s
  ECDSA-secp224r1          :    3.33 verify/s
  ECDSA-secp224k1          :    2.67 verify/s
  ECDSA-secp192r1          :    4.33 verify/s
  ECDSA-secp192k1          :    3.33 verify/s
  ECDHE-secp521r1          :    0.67 full handshake/s
  ECDHE-brainpoolP512r1    :    0.10 full handshake/s
  ECDHE-secp384r1          :    1.00 full handshake/s
  ECDHE-brainpoolP384r1    :    0.18 full handshake/s
  ECDHE-secp256r1          :    1.67 full handshake/s
  ECDHE-secp256k1          :    1.33 full handshake/s
  ECDHE-brainpoolP256r1    :    0.40 full handshake/s
  ECDHE-secp224r1          :    2.00 full handshake/s
  ECDHE-secp224k1          :    1.67 full handshake/s
  ECDHE-secp192r1          :    2.67 full handshake/s
  ECDHE-secp192k1          :    2.00 full handshake/s
  ECDHE-x25519             :    1.33 full handshake/s
  ECDHE-x448               :    0.60 full handshake/s
