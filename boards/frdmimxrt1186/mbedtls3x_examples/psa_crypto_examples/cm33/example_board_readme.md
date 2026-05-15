Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT1186 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
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
The log below shows the output of the psa crypto example demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * PSA crypto example *

cipher encrypt/decrypt AES CBC no padding:
        success!
cipher encrypt/decrypt AES CBC PKCS7 multipart:
        success!
cipher encrypt/decrypt AES CTR multipart:
        success!
cipher encrypt/decrypt AES CBC no padding one go:
        success!
cipher encrypt/decrypt AES CBC PKCS7 padding one go:
        success!
Hash a message SHA-256:
        success!

 * Example End *
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

