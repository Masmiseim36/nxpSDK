Overview
========
The mbedTLS PSA Crypto example program performs PSA API testing and prints results to the
terminal.


SDK version
===========
- Version: 2.15.000

Toolchain supported
===================
- Keil MDK  5.38.1
- IAR embedded Workbench  9.40.1
- GCC ARM Embedded  12.2
- MCUXpresso  11.8.0

Hardware requirements
=====================
- Micro USB cable
- MIMXRT685-AUD-EVK board
- Personal Computer

Board settings
==============
No special settings are required.
Prepare the Demo
================
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

Running the demo
================
When the demo runs successfully, the terminal will display similar information like the following:

cipher encrypt/decrypt AES CBC no padding: success!
cipher encrypt/decrypt AES CBC PKCS7 multipart: success!
cipher encrypt/decrypt AES CTR multipart: success!
