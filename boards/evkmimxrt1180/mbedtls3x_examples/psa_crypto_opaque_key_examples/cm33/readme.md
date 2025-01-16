# psa_crypto_opaque_key_examples

## Overview

The PSA crypto opaque key example illustrates the generation and usage of following keys:
    1. ECC keys
    2. AES keys
    3. RSA keys
    4. MAC Keys

The location of these keys can be chosen in the yml file for the boards
which the example is being included for. By deafult even though the
example name says opaque, the default location in test is LOCAL_STORAGE i.e
keys in plaintext. This is chosen as default as it would work for all platforms.

However the intent is that this example can be built to test in opaque location keys. For this,
the calling yml needs to define the location of TEST KEYS i.e TEST_KEY_LOCATION.
For eg. For RT1180, we choose this location as PSA_CRYPTO_ELE_S4XX_LOCATION

The test also covers generation and test of both volatile and persistent keys.

## Prepare the Demo

Insert a SD card if FATFS is being used as storage location for persistent keys.
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

## Running the demo

When the demo runs successfully, which takes a couple of minutes, the terminal displays similar information like the following:

Card inserted.

Create directory......
Directory exists.
File System initialized for Persistent Storage
Location is 1

VOLATILE KEYS

ECC keys
224 bit ECC_KEY_PAIR(SECP_R1) with sign/verify algo ECDSA(SHA224): PASSED
521 bit ECC_KEY_PAIR(SECP_R1) with sign/verify algo ECDSA(SHA512): PASSED
256 bit ECC_KEY_PAIR(SECP_R1) with sign/verify algo ECDSA(SHA256): PASSED
256 ECC_KEY_PAIR(BRAINPOOL_R1) with sign/verify algo ECDSA(SHA256): PASSED
384 bit ECC_KEY_PAIR(BRAINPOOL_R1) with sign/verify algo ECDSA(SHA384): PASSED

AES keys
128 bit AES key with encrypt/decrypt algo ECB_NO_PADDING: PASSED
192 bit AES key with encrypt/decrypt algo CBC_NO_PADDING: PASSED
256 bit AES key with encrypt/decrypt algo CTR: PASSED
128 bit AES key with encrypt/decrypt algo CCM: PASSED
256 bit AES key with encrypt/decrypt algo GCM: PASSED
192 bit AES key with sign/verify algo CMAC : PASSED

MAC keys
256 bit PERSISTENT HMAC key with sign/verify algo HMAC(SHA-256): PASSED

RSA keys
2048 bit RSA key with sign/verify algo RSA_PKCS1V15(SHA256) : PASSED
2048 bit RSA key with sign/verify algo RSA_PSS(SHA512) : PASSED

PERSISTENT KEYS

ECC keys
224 bit ECC_KEY_PAIR(SECP_R1) with sign/verify algo ECDSA(SHA224): PASSED
521 bit ECC_KEY_PAIR(SECP_R1) with sign/verify algo ECDSA(SHA512): PASSED
256 bit ECC_KEY_PAIR(SECP_R1) with sign/verify algo ECDSA(SHA256): PASSED
256 ECC_KEY_PAIR(BRAINPOOL_R1) with sign/verify algo ECDSA(SHA256): PASSED
384 bit ECC_KEY_PAIR(BRAINPOOL_R1) with sign/verify algo ECDSA(SHA384): PASSED

AES keys
128 bit AES key with encrypt/decrypt algo ECB_NO_PADDING: PASSED
192 bit AES key with encrypt/decrypt algo CBC_NO_PADDING: PASSED
256 bit AES key with encrypt/decrypt algo CTR: PASSED
128 bit AES key with encrypt/decrypt algo CCM: PASSED
256 bit AES key with encrypt/decrypt algo GCM: PASSED
192 bit AES key with sign/verify algo CMAC : PASSED

MAC keys
256 bit PERSISTENT HMAC key with sign/verify algo HMAC(SHA-256): PASSED

RSA keys
2048 bit RSA key with sign/verify algo RSA_PKCS1V15(SHA256) : PASSED
2048 bit RSA key with sign/verify algo RSA_PSS(SHA512) : PASSED

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/mbedtls3x_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
