Overview
========
This project is a demonstration program that uses the KSDK software for encryption/decryption sample
data using AES-CBC, AES-CCM, and XCBC-MAC algorithm.


Toolchain supported
===================
- IAR embedded Workbench  8.32.4
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KW38 board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.
Prepare the Demo
================

Running the demo
================
When the demo runs successfully, from the terminal you can see:

............................. LTC (AES)  DRIVER  EXAMPLE .............................

Testing input string:
          Once upon a midnight dreary,
           while I pondered weak and weary,
          Over many a quaint and curious volume of forgotten lore,
          While I nodded,
           nearly napping,
           suddenly there came a tapping,
          As of some one gently rapping,
           rapping at my chamber doorIts some visitor,
           I muttered,
           tapping at my chamber doorOnly this,
           and nothing more.

----------------------------------- AES-CBC method --------------------------------------
AES CBC Encryption of 320 bytes.
AES CBC encryption finished.

AES CBC Decryption of 320 bytes.
AES CBC decryption finished.
Decrypted string :
          Once upon a midnight dreary,
           while I pondered weak and weary,
          Over many a quaint and curious volume of forgotten lore,
          While I nodded,
           nearly napping,
           suddenly there came a tapping,
          As of some one gently rapping,
           rapping at my chamber doorIts some visitor,
           I muttered,
           tapping at my chamber doorOnly this,
           and nothing more.

----------------------------------- AES-CCM method --------------------------------------
AES CCM Encryption of  320 bytes.
    using iv length  : 12 bytes
    using aad length : 20 bytes
    using key length : 16 bytes
    using tag length : 8 bytes
AES CCM encryption finished.

AES CCM decryption of 320 bytes.
AES CCM decryption finished.
Decrypted string:
          Once upon a midnight dreary,
           while I pondered weak and weary,
          Over many a quaint and curious volume of forgotten lore,
          While I nodded,
           nearly napping,
           suddenly there came a tapping,
          As of some one gently rapping,
           rapping at my chamber doorIts some visitor,
           I muttered,
           tapping at my chamber doorOnly this,
           and nothing more.

----------------------------------- AES-XCBC-MAC --------------------------------------
AES XCBC-MAC Computing hash of 320 bytes
Computed hash:
9e c 5a 5a 10 bb 96 67 9f 98 3 29 94 f8 6f 9f

.............. THE  END  OF  THE  LTC (AES)  DRIVER  EXAMPLE ................................

Customization options
=====================

