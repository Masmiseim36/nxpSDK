Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
============

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J1) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the terminal will display similar information like the following (Software only for now):

  CALLOC(0,1): passed (distinct non-null)
  CALLOC(1,0): passed (distinct non-null)
  CALLOC(1): passed
  CALLOC(1 again): passed

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

  SHA-224 test #1: passed
  SHA-224 test #2: passed
  SHA-224 test #3: passed

  SHA-256 test #1: passed
  SHA-256 test #2: passed
  SHA-256 test #3: passed

  SHA-384 test #1: passed
  SHA-384 test #2: passed
  SHA-384 test #3: passed

  SHA-512 test #1: passed
  SHA-512 test #2: passed
  SHA-512 test #3: passed

  SHA3-224 test 0 passed
  SHA3-256 test 0 passed
  SHA3-384 test 0 passed
  SHA3-512 test 0 passed
  SHA3-224 test 1 passed
  SHA3-256 test 1 passed
  SHA3-384 test 1 passed
  SHA3-512 test 1 passed
  SHA3-224 long KAT test passed
  SHA3-256 long KAT test passed
  SHA3-384 long KAT test passed
  SHA3-512 long KAT test passed

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

  AES note: built-in implementation.
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

  AES-XTS-128 (dec): passed
  AES-XTS-128 (enc): passed
  AES-XTS-128 (dec): passed
  AES-XTS-128 (enc): passed
  AES-XTS-128 (dec): passed
  AES-XTS-128 (enc): passed

  GCM note: built-in implementation.
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

  AES 128 CMAC subkey #1: passed
  AES 128 CMAC subkey #2: passed
  AES 128 CMAC subkey #3: passed
  AES 128 CMAC subkey #4: passed
  AES 128 CMAC #1: passed
  AES 128 CMAC #2: passed
  AES 128 CMAC #3: passed
  AES 128 CMAC #4: passed
  AES 192 CMAC subkey #1: passed
  AES 192 CMAC subkey #2: passed
  AES 192 CMAC subkey #3: passed
  AES 192 CMAC subkey #4: passed
  AES 192 CMAC #1: passed
  AES 192 CMAC #2: passed
  AES 192 CMAC #3: passed
  AES 192 CMAC #4: passed
  AES 256 CMAC subkey #1: passed
  AES 256 CMAC subkey #2: passed
  AES 256 CMAC subkey #3: passed
  AES 256 CMAC subkey #4: passed
  AES 256 CMAC #1: passed
  AES 256 CMAC #2: passed
  AES 256 CMAC #3: passed
  AES 256 CMAC #4: passed
  3DES 2 key CMAC subkey #1: passed
  3DES 2 key CMAC subkey #2: passed
  3DES 2 key CMAC subkey #3: passed
  3DES 2 key CMAC subkey #4: passed
  3DES 2 key CMAC #1: passed
  3DES 2 key CMAC #2: passed
  3DES 2 key CMAC #3: passed
  3DES 2 key CMAC #4: passed
  3DES 3 key CMAC subkey #1: passed
  3DES 3 key CMAC subkey #2: passed
  3DES 3 key CMAC subkey #3: passed
  3DES 3 key CMAC subkey #4: passed
  3DES 3 key CMAC #1: passed
  3DES 3 key CMAC #2: passed
  3DES 3 key CMAC #3: passed
  3DES 3 key CMAC #4: passed
  AES CMAC 128 PRF #0: passed
  AES CMAC 128 PRF #1: passed
  AES CMAC 128 PRF #2: passed

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

  CTR_DRBG (PR = TRUE) : passed
  CTR_DRBG (PR = FALSE): passed

  HMAC_DRBG (PR = True) : passed
  HMAC_DRBG (PR = False) : passed

  ECP SW test #1 (constant op_count, base point G): passed
  ECP SW test #2 (constant op_count, other point): passed
  ECP Montgomery test (constant op_count): passed

  DHM parameter load: passed

  ENTROPY test: passed

  PBKDF2 (SHA1) #0: passed
  PBKDF2 (SHA1) #1: passed
  PBKDF2 (SHA1) #2: passed
  PBKDF2 (SHA1) #3: passed
  PBKDF2 (SHA1) #4: passed
  PBKDF2 (SHA1) #5: passed

  Executed 23 test suites

  [ All tests PASS ]
