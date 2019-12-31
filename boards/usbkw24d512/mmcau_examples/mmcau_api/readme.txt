Overview
========
Memory-Mapped Cryptographic Acceleration Unit (MMCAU)

This project is a demonstration program that uses the KSDK software for encryption/decryption sample
data using AES-CBC, DES3-CBC and Hash algorithms MD5, SHA1 and SHA256.


Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- USB-KW24D512 board
- Personal Computer

Board settings
==============
No special settings are required.
The USB port on the target board is used as termial.
Prepare the Demo
================
Connect a serial cable from the debug UART port of the board to the PC. Start Tera Term
(http://ttssh2.osdn.jp) and make a connection to the virtual serial port.

1. Start Tera Term
2. New connection -> Serial
3. Set apropriate COMx port (x is port number) in Port context menu. Number is provided by operation
   system and could be different from computer to computer. Select COM number related to virtual
   serial port. Confirm selected port by OK button.
4. Set following connection parameters in menu Setup->Serial port...
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: one
5.  Confirm selected parameters by OK button.

Running the demo
================
When the demo runs successfully, from the terminal you can see:

............................. MMCAU  DRIVER  EXAMPLE .............................

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

--------------------------------------- HASH ------------------------------------------
Computing hash of 64 bytes.
Input string:
          The quick brown fox jumps over the lazy dog
Computed SHA1:
2fd4e1c67a2d28fced849ee1bb76e7391b93eb12

Computed SHA256:
d7a8fbb37d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592

Computed MD5:
9e107d9d372bb6826bd81d3542a419d6

.............. THE  END  OF  THE  MMCAU  DRIVER  EXAMPLE ................................

Customization options
=====================

