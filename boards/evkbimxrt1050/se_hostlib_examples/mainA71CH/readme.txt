Overview
========

This project demonstrates the usage of Secure Module functionality in
combination with host library and MBEDTLS cryptographic library.

    Walkthrough example => Demonstrates usage of secure module from a system
        integrator's perspective. Illustrates typical product use

    AES example => Demonstrates wrapped setting/retrieving of symmetric keys and
        crypto applications of stored symmetric keys

    Config example => Demonstrates storage and usage of config keys

    Debug example => Demonstrates miscellaneous functionality supported by Debug
        Mode of secure module

    EccNohc => Demonstrates ECC crypto functionality of secure module without
        relying on Host Crypto

    GPStorage => Demonstrates general purpose storage functionality and
        monotonic counter usage

    Misc => Demonstrates miscellaneous module functionality

    PSK => Demonstrates plain or ECDH enhanced pre-shared key based master key
        creation

    SCP => Demonstrates setting up an SCP03 channel between Host and secure
        module

    Sst => Demonstrate storage of symmetric keys and public keys

    Boot => Demonstrates that the HostOS does NOT need to know the SCP03 Base
        Keys to establish an SCP03 session (provided the boot loader has
        established the SCP03 session and saved the SCP03 session state)

    SstKp => Demonstrates storage of key pairs


Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer

Prepare the Demo
================
1. Build the demo
2. Connect a USB cable between the PC host and the OpenSDA USB port on the
   target board.
3. Download the program to the target board.
4. Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
    - change Setup->Terminal->New-line->Receive->AUTO
5. Either press the reset button on your board or launch the debugger in your
   IDE to begin running the demo.

Running the demo
================

The below messages are seen on the terminal followed by the test specific log
messages

selectResponseDataLen: 2
0x01:0x31:
ATR=0xB8.04.11.01.05.04.B9.02.01.01.BA.01.01.BB.0C.41.37.31.30.78.43.48.32.34.32.52.31.BC.00.
SCI2C_HostLib Version  : 0x0130
Applet Version   : 0x0131
SecureBox Version: 0x0000

(more tests are run as part of this example)


End of project
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Board settings
==============
No special settings are required.
