Overview
========
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


SDK version
===========
- Version: 2.15.000

Toolchain supported
===================
- GCC ARM Embedded  12.3.1
- IAR embedded Workbench  9.50.1
- MCUXpresso  11.9.0

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- SD Card
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Insert the SD card in the SD slot of the board.
2.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, which takes a couple of minutes, the terminal displays similar information like the following:

Card inserted.

Create directory......
Directory exists.
File System initialized for Persistent Storage

ECC keys
224 bit PERSISTENT ECC_KEY_PAIR(SECP_R1) with sign/verify algo ECDSA(SHA224): PASSED
521 bit ECC_KEY_PAIR(SECP_R1) with sign/verify algo ECDSA(SHA512): PASSED
256 ECC_KEY_PAIR(BRAINPOOL_R1) with sign/verify algo ECDSA(SHA256): PASSED
384 bit ECC_KEY_PAIR(BRAINPOOL_R1) with sign/verify algo ECDSA(SHA384): PASSED

AES keys 
128 bit PERSISTENT AES key with encrypt/decrypt algo ECB_NO_PADDING: PASSED
196 bit AES key with encrypt/decrypt algo CBC_NO_PADDING: FAILED
256 bit AES key with encrypt/decrypt algo CTR: PASSED
128 bit AES key with encrypt/decrypt algo CCM: PASSED
256 bit AES key with encrypt/decrypt algo GCM: PASSED
192 bit AES key with sign/verify algo CMAC : PASSED

MAC keys 
256 bit PERSISTENT HMAC key with sign/verify algo HMAC(SHA-256): PASSED

RSA keys 
2048 bit RSA key with sign/verify algo RSA_PKCS1V15(SHA256) : PASSED
2048 bit RSA key with sign/verify algo RSA_PSS(SHA512) : PASSED
