Overview
========
This project is a demonstration program that uses the KSDK software for encryption/decryption some
data using DES-cbc and DES3-cbc method.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.
Running the demo
================
When the demo runs successfully, from the terminal you can see:

............................. LTC (DES)  DRIVER  EXAMPLE .............................

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

----------------------------------- DES-CBC method --------------------------------------
DES CBC Encryption of 320 bytes.
DES CBC encryption finished.

DES CBC Decryption of 320 bytes.
DES CBC decryption finished.
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

----------------------------------- DES3-CBC method --------------------------------------
DES3 CBC Encryption of 320 bytes.
DES3 CBC encryption finished.

DES3 CBC decryption of 320 bytes.
DES3 CBC decryption finished.
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

.............. THE  END  OF  THE  LTC (DES)  DRIVER  EXAMPLE ................................
Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL81Z board
- Personal Computer

Board settings
==============
No special settings are required.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

