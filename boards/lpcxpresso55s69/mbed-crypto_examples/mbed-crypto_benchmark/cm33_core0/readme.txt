Overview
========
The Mbed-Crypto demo application performs cryptographic algorithm benchmarking and prints results to the
terminal.


Toolchain supported
===================
- IAR embedded Workbench  8.50.5
- Keil MDK  5.31
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso5500 board
- Personal Computer

Board settings
==============
Connect a USB2COM between the PC host and the board UART pins
boards           -               USB2COM
J14-Pin26                        Tx
J14-Pin28                        Rx
J14-Pin1                         GND
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

  MD5                      :       3528 KiB/s,         26 cycles/byte
  RIPEMD160                :       1295 KiB/s,         72 cycles/byte
  SHA-1                    :       1712 KiB/s,         54 cycles/byte
  SHA-256                  :        855 KiB/s,        109 cycles/byte
  SHA-512                  :        491 KiB/s,        190 cycles/byte
  ARC4                     :       5163 KiB/s,         18 cycles/byte
  3DES                     :         83 KiB/s,       1149 cycles/byte
  DES                      :        241 KiB/s,        389 cycles/byte
  3DES-CMAC                :         79 KiB/s,       1198 cycles/byte
  AES-CBC-128              :          2 KiB/s,        134 cycles/byte
  AES-CBC-192              :        605 KiB/s,        154 cycles/byte
  AES-CBC-256              :        539 KiB/s,        173 cycles/byte
  AES-XTS-128              :        610 KiB/s,        153 cycles/byte
  AES-XTS-256              :        486 KiB/s,        192 cycles/byte
  AES-GCM-128              :        344 KiB/s,        271 cycles/byte
  AES-GCM-192              :        321 KiB/s,        291 cycles/byte
  AES-GCM-256              :        301 KiB/s,        311 cycles/byte
  AES-CCM-128              :        310 KiB/s,        302 cycles/byte
  AES-CCM-192              :        274 KiB/s,        342 cycles/byte
  AES-CCM-256              :        246 KiB/s,        380 cycles/byte
  ChaCha20-Poly1305        :        513 KiB/s,        182 cycles/byte
  AES-CMAC-128             :        622 KiB/s,        150 cycles/byte
  AES-CMAC-192             :          2 KiB/s,        170 cycles/byte
  AES-CMAC-256             :        493 KiB/s,        189 cycles/byte
  AES-CMAC-PRF-128         :        619 KiB/s,        150 cycles/byte
  CAMELLIA-CBC-128         :        323 KiB/s,        290 cycles/byte
  CAMELLIA-CBC-192         :        248 KiB/s,        377 cycles/byte
  CAMELLIA-CBC-256         :        249 KiB/s,        377 cycles/byte
  ChaCha20                 :        659 KiB/s,        141 cycles/byte
  Poly1305                 :       2925 KiB/s,         31 cycles/byte
  BLOWFISH-CBC-128         :        823 KiB/s,        113 cycles/byte
  BLOWFISH-CBC-192         :        823 KiB/s,        113 cycles/byte
  BLOWFISH-CBC-256         :        823 KiB/s,        113 cycles/byte
  CTR_DRBG (NOPR)          :        533 KiB/s,        175 cycles/byte
  CTR_DRBG (PR)            :        378 KiB/s,        248 cycles/byte
  HMAC_DRBG SHA-1 (NOPR)   :        120 KiB/s,        785 cycles/byte
  HMAC_DRBG SHA-1 (PR)     :        111 KiB/s,        849 cycles/byte
  HMAC_DRBG SHA-256 (NOPR) :         89 KiB/s,       1067 cycles/byte
  HMAC_DRBG SHA-256 (PR)   :         89 KiB/s,       1067 cycles/byte
  RSA-1024                 :       0  public/s
  RSA-1024                 :       0 private/s
  DHE-2048                 :       0 handshake/s
  DH-2048                  :       0 handshake/s
  DHE-3072                 :       0 handshake/s
  DH-3072                  :       0 handshake/s
  ECDSA-secp521r1          :       2 sign/s
  ECDSA-brainpoolP512r1    :       0 sign/s
  ECDSA-secp384r1          :       4 sign/s
  ECDSA-brainpoolP384r1    :       0 sign/s
  ECDSA-secp256r1          :       5 sign/s
  ECDSA-secp256k1          :       5 sign/s
  ECDSA-brainpoolP256r1    :       0 sign/s
  ECDSA-secp224r1          :       8 sign/s
  ECDSA-secp224k1          :       0 sign/s
  ECDSA-secp192r1          :       0 sign/s
  ECDSA-secp192k1          :       0 sign/s
  ECDSA-secp521r1          :       1 verify/s
  ECDSA-brainpoolP512r1    :       0 verify/s
  ECDSA-secp384r1          :       1 verify/s
  ECDSA-brainpoolP384r1    :       0 verify/s
  ECDSA-secp256r1          :       2 verify/s
  ECDSA-secp256k1          :       2 verify/s
  ECDSA-brainpoolP256r1    :       0 verify/s
  ECDSA-secp224r1          :       3 verify/s
  ECDSA-secp224k1          :       2 verify/s
  ECDSA-secp192r1          :       3 verify/s
  ECDSA-secp192k1          :       0 verify/s
  ECDHE-secp521r1          :       0 handshake/s
  ECDHE-brainpoolP512r1    :       0 handshake/s
  ECDHE-secp384r1          :       1 handshake/s
  ECDHE-brainpoolP384r1    :       0 handshake/s
  ECDHE-secp256r1          :       2 handshake/s
  ECDHE-secp256k1          :       2 handshake/s
  ECDHE-brainpoolP256r1    :       0 handshake/s
  ECDHE-secp224r1          :       3 handshake/s
  ECDHE-secp224k1          :       2 handshake/s
  ECDHE-secp192r1          :       4 handshake/s
  ECDHE-secp192k1          :       3 handshake/s
  ECDHE-Curve25519         :       1 handshake/s
  ECDHE-Curve448           :       1 handshake/s
  ECDH-secp521r1           :       1 handshake/s
  ECDH-brainpoolP512r1     :       0 handshake/s
  ECDH-secp384r1           :       2 handshake/s
  ECDH-brainpoolP384r1     :       0 handshake/s
  ECDH-secp256r1           :       3 handshake/s
  ECDH-secp256k1           :       3 handshake/s
  ECDH-brainpoolP256r1     :       0 handshake/s
  ECDH-secp224r1           :       4 handshake/s
  ECDH-secp224k1           :       0 handshake/s
  ECDH-secp192r1           :       0 handshake/s
  ECDH-secp192k1           :       0 handshake/s
  ECDH-Curve25519          :       0 handshake/s
  ECDH-Curve448            :       2 handshake/s

[END]
