# mbedtls_selftest

## Overview
The mbedTLS SelfTest program performs cryptographic algorithm testing and prints results to the
terminal.

## Prepare the Demo
Connect a serial cable from the debug UART port of the target board to the PC. Start TeraTerm
(http://ttssh2.osdn.jp) and make a connection to the virtual serial port.

1. Start TeraTerm.

2. New connection -> Serial.

3. Set the appropriate COMx port (where x is port number) in port context menu. The number is provided by the operating
   system, and could be different from computer to computer. Select the COM number related to the virtual
   serial port. Confirm the selected port by clicking the "OK" button.

4. Set following connection parameters in menu Setup -> Serial port.
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: none

5.  Confirm the selected parameters by clicking the "OK" button.

## Running the demo
When the demo runs successfully, the terminal will display similar information like the following:

mbedTLS version 2.26.0
fsys=250105263
Using following implementations:
  SHA: HASHCRYPT HW accelerated
  AES: HASHCRYPT HW accelerated
  AES GCM: Software implementation
  DES: Software implementation
  Asymmetric cryptography: CASPER HW accelerated ECC256/384/521 and RSA verify
  MD5 test #1: passed
  MD5 test #2: passed
  MD5 test #3: passed
  MD5 test #4: passed
  MD5 test #5: passed
  MD5 test #6: passed
  MD5 test #7: passed

  SHA-1 test #1: passed
  SHA-1 test #2: passed
  SHA-1 test #3: passed

  SHA-256 test #1: passed
  SHA-256 test #2: passed
  SHA-256 test #3: passed

  SHA-384 test #1: passed
  SHA-384 test #2: passed
  SHA-384 test #3: passed
  SHA-512 test #1: passed
  SHA-512 test #2: passed
  SHA-512 test #3: passed

  DES -ECB- 56 (dec): passed
  DES -ECB- 56 (enc): passed
  DES3-ECB-112 (dec): passed
  DES3-ECB-112 (enc): passed
  DES3-ECB-168 (dec): passed
  DES3-ECB-168 (enc): passed

  DES -CBC- 56 (dec): passed
  DES -CBC- 56 (enc): passed
  DES3-CBC-112 (dec): passed
  DES3-CBC-112 (enc): passed
  DES3-CBC-168 (dec): passed
  DES3-CBC-168 (enc): passed

  AES-ECB-128 (dec): passed
  AES-ECB-128 (enc): passed
  AES-ECB-192 (dec): passed
  AES-ECB-192 (enc): passed
  AES-ECB-256 (dec): passed
  AES-ECB-256 (enc): passed

  AES-CBC-128 (dec): passed
  AES-CBC-128 (enc): passed
  AES-CBC-192 (dec): passed
  AES-CBC-192 (enc): passed
  AES-CBC-256 (dec): passed
  AES-CBC-256 (enc): passed

  AES-CFB128-128 (dec): passed
  AES-CFB128-128 (enc): passed
  AES-CFB128-192 (dec): passed
  AES-CFB128-192 (enc): passed
  AES-CFB128-256 (dec): passed
  AES-CFB128-256 (enc): passed

  AES-OFB-128 (dec): passed
  AES-OFB-128 (enc): passed
  AES-OFB-192 (dec): passed
  AES-OFB-192 (enc): passed
  AES-OFB-256 (dec): passed
  AES-OFB-256 (enc): passed

  AES-CTR-128 (dec): passed
  AES-CTR-128 (enc): passed
  AES-CTR-128 (dec): passed
  AES-CTR-128 (enc): passed
  AES-CTR-128 (dec): passed
  AES-CTR-128 (enc): passed

  AES-GCM-128 #0 (enc): passed
  AES-GCM-128 #0 (dec): passed
  AES-GCM-128 #0 split (enc): passed
  AES-GCM-128 #0 split (dec): passed
  AES-GCM-128 #1 (enc): passed
  AES-GCM-128 #1 (dec): passed
  AES-GCM-128 #1 split (enc): passed
  AES-GCM-128 #1 split (dec): passed
  AES-GCM-128 #2 (enc): passed
  AES-GCM-128 #2 (dec): passed
  AES-GCM-128 #2 split (enc): passed
  AES-GCM-128 #2 split (dec): passed
  AES-GCM-128 #3 (enc): passed
  AES-GCM-128 #3 (dec): passed
  AES-GCM-128 #3 split (enc): passed
  AES-GCM-128 #3 split (dec): passed
  AES-GCM-128 #4 (enc): passed
  AES-GCM-128 #4 (dec): passed
  AES-GCM-128 #4 split (enc): passed
  AES-GCM-128 #4 split (dec): passed
  AES-GCM-128 #5 (enc): passed
  AES-GCM-128 #5 (dec): passed
  AES-GCM-128 #5 split (enc): passed
  AES-GCM-128 #5 split (dec): passed
  AES-GCM-192 #0 (enc): passed
  AES-GCM-192 #0 (dec): passed
  AES-GCM-192 #0 split (enc): passed
  AES-GCM-192 #0 split (dec): passed
  AES-GCM-192 #1 (enc): passed
  AES-GCM-192 #1 (dec): passed
  AES-GCM-192 #1 split (enc): passed
  AES-GCM-192 #1 split (dec): passed
  AES-GCM-192 #2 (enc): passed
  AES-GCM-192 #2 (dec): passed
  AES-GCM-192 #2 split (enc): passed
  AES-GCM-192 #2 split (dec): passed
  AES-GCM-192 #3 (enc): passed
  AES-GCM-192 #3 (dec): passed
  AES-GCM-192 #3 split (enc): passed
  AES-GCM-192 #3 split (dec): passed
  AES-GCM-192 #4 (enc): passed
  AES-GCM-192 #4 (dec): passed
  AES-GCM-192 #4 split (enc): passed
  AES-GCM-192 #4 split (dec): passed
  AES-GCM-192 #5 (enc): passed
  AES-GCM-192 #5 (dec): passed
  AES-GCM-192 #5 split (enc): passed
  AES-GCM-192 #5 split (dec): passed
  AES-GCM-256 #0 (enc): passed
  AES-GCM-256 #0 (dec): passed
  AES-GCM-256 #0 split (enc): passed
  AES-GCM-256 #0 split (dec): passed
  AES-GCM-256 #1 (enc): passed
  AES-GCM-256 #1 (dec): passed
  AES-GCM-256 #1 split (enc): passed
  AES-GCM-256 #1 split (dec): passed
  AES-GCM-256 #2 (enc): passed
  AES-GCM-256 #2 (dec): passed
  AES-GCM-256 #2 split (enc): passed
  AES-GCM-256 #2 split (dec): passed
  AES-GCM-256 #3 (enc): passed
  AES-GCM-256 #3 (dec): passed
  AES-GCM-256 #3 split (enc): passed
  AES-GCM-256 #3 split (dec): passed
  AES-GCM-256 #4 (enc): passed
  AES-GCM-256 #4 (dec): passed
  AES-GCM-256 #4 split (enc): passed
  AES-GCM-256 #4 split (dec): passed
  AES-GCM-256 #5 (enc): passed
  AES-GCM-256 #5 (dec): passed
  AES-GCM-256 #5 split (enc): passed
  AES-GCM-256 #5 split (dec): passed

  CCM-AES #1: passed
  CCM-AES #2: passed
  CCM-AES #3: passed

  Poly1305 test 0 passed
  Poly1305 test 1 passed

  Base64 encoding test: passed
  Base64 decoding test: passed

  MPI test #1 (mul_mpi): passed
  MPI test #2 (div_mpi): passed
  MPI test #3 (exp_mod): passed
  MPI test #4 (inv_mod): passed
  MPI test #5 (simple gcd): passed

  RSA key validation: passed
  PKCS#1 encryption : passed
  PKCS#1 decryption : passed
  PKCS#1 data sign  : passed
  PKCS#1 sig. verify: passed

  X.509 certificate load: passed
  X.509 signature verify: passed

  CTR_DRBG (PR = TRUE) : passed
  CTR_DRBG (PR = FALSE): passed

  HMAC_DRBG (PR = True) : passed
  HMAC_DRBG (PR = False) : passed

  ECP SW test #1 (constant op_count, base point G): passed
  ECP SW test #2 (constant op_count, other point): passed

  DHM parameter load: passed

  ENTROPY test: passed

  PBKDF2 (SHA1) #0: passed
  PBKDF2 (SHA1) #1: passed
  PBKDF2 (SHA1) #2: passed
  PBKDF2 (SHA1) #3: passed
  PBKDF2 (SHA1) #4: passed
  PBKDF2 (SHA1) #5: passed

  Executed 19 test suites

  [ All tests PASS ]

## Supported Boards
- [EVK9-MIMX8ULP](../../_boards/evk9mimx8ulp/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [EVK-MCIMX7ULP](../../_boards/evkmcimx7ulp/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [EVK-MIMX8ULP](../../_boards/evkmimx8ulp/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [EVK-MIMXRT1010](../../_boards/evkmimxrt1010/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [EVK-MIMXRT1020](../../_boards/evkmimxrt1020/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [MIMXRT1024-EVK](../../_boards/evkmimxrt1024/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [FRDM-K32L2A4S](../../_boards/frdmk32l2a4s/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [FRDM-MCXN236](../../_boards/frdmmcxn236/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [FRDM-MCXW71](../../_boards/frdmmcxw71/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [K32W148-EVK](../../_boards/k32w148evk/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [LPCXpresso54628](../../_boards/lpcxpresso54628/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [LPCXpresso54S018](../../_boards/lpcxpresso54s018/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [LPCXpresso55S06](../../_boards/lpcxpresso55s06/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [LPCXpresso55S28](../../_boards/lpcxpresso55s28/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
- [TWR-KM35Z75M](../../_boards/twrkm35z75m/mbedtls_examples/mbedtls_selftest/example_board_readme.md)
