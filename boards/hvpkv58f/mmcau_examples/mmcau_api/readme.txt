Overview
========
Memory-Mapped Cryptographic Acceleration Unit (MMCAU)

This project is a demonstration program that uses the KSDK software for encryption/decryption sample
data using AES-CBC, DES3-CBC and Hash algorithms MD5, SHA1 and SHA256.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- HVP-KV58F board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the mmcau_api example in the terminal window:


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
AES CBC encryption finished. Speed 2.627085 MB/s.

AES CBC Decryption of 320 bytes.
AES CBC decryption finished. Speed 2.691825 MB/s.
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
DES3 CBC encryption finished. Speed 1.505638 MB/s.

DES3 CBC decryption of 320 bytes.
DES3 CBC decryption finished. Speed 1.567230 MB/s.
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

Computed SHA1 at speed 4.264763 MB/s:
2fd4e1c67a2d28fced849ee1bb76e7391b93eb12

Computed SHA256 at speed 1.950824 MB/s:
d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592

Computed MD5 at speed 14.393575 MB/s:
9e107d9d372bb6826bd81d3542a419d6

.............. THE  END  OF  THE  MMCAU  DRIVER  EXAMPLE ................................
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

