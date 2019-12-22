..
    Copyright 2019 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.


.. highlight:: bat

.. _ex-se05x-info:

=======================================================================
 SE05X Get Info example
=======================================================================

This project can be used to get SE05X platform information.


Prerequisites
=====================
- Micro USB cable
- Kinetis FRDM-K64F/imx-RT1050 board
- Personal Computer
- SE05x Arduino shield


Building the Demo
=======================================================================

- Build Plug and Trust middleware stack. (Refer :ref:`building`)
- Project: ``se05x_Get_Info``


Running the Example
=======================================================================

If you have built a binary, flash the binary on to the board and reset 
the board. 

If you have built an *exe* to be run from Windows using VCOM, run as::
	
	se05x_Get_Info.exe <PORT NAME>

Where **<PORT NAME>** is the VCOM COM port.


Console output
=======================================================================

If everything is setup correctly the output would be as follows ::

       App:WARN :#####################################################
       App:INFO :uid (Len=18)
      04 00 50 01    55 55 55 55    55 55 55 04    FF FF FF FF
      FF FA
       App:WARN :#####################################################
       App:INFO :Applet Major = 3
       App:INFO :Applet Minor = 1
       App:INFO :Applet patch = 0
       App:INFO :AppletConfig = 3FFF
       App:INFO :With    ECDAA
       App:INFO :With    ECDSA_ECDH_ECDHE
       App:INFO :With    EDDSA
       App:INFO :With    DH_MONT
       App:INFO :With    HMAC
       App:INFO :With    RSA_PLAIN
       App:INFO :With    RSA_CRT
       App:INFO :With    AES
       App:INFO :With    DES
       App:INFO :With    PBKDF
       App:INFO :With    TLS
       App:INFO :With    MIFARE
       App:INFO :With    I2CM
       App:INFO :SecureBox = 010B
       App:WARN :#####################################################
       App:INFO :Tag value - proprietary data 0xFE = 0xFE
       App:INFO :Length of following data 0x45 = 0x45
       App:INFO :Tag card identification data (Len=2)
      DF 28
       App:INFO :Length of card identification data 0x46 = 0x42
       App:INFO :Tag configuration ID 0x01 = 0x01
       App:INFO :Length configuration ID 0x0C = 0x0C
       App:INFO :Configuration ID (Len=12)
      00 01 A2 00    E9 9B 58 64    DD 3C F0 FC
       App:INFO :OEF ID (Len=2)
      A2 00
       App:INFO :Tag patch ID 0x02 = 0x02
       App:INFO :Length patch ID 0x08 = 0x08
       App:INFO :Patch ID (Len=8)
      00 00 00 00    00 00 00 00
       App:INFO :Tag platform build ID1 0x03 = 0x03
       App:INFO :Length platform build ID 0x18 = 0x18
       App:INFO :Platform build ID (Len=24)
      4A 33 52 33    35 31 30 32    31 45 45 45    30 34 30 30
      30 30 30 30    30 30 30 30
       App:INFO :JCOP Platform ID = J3R351021EEE0400
       App:INFO :Tag FIPS mode 0x05 = 0x05
       App:INFO :Length FIPS mode 0x01 = 0x01
       App:INFO :FIPS mode var = 0x00
       App:INFO :Tag pre-perso state 0x07 = 0x07
       App:INFO :Length pre-perso state 0x01 = 0x01
       App:INFO :Bit mask of pre-perso state var = 0x01
       App:INFO :Tag ROM ID 0x08 = 0x08
       App:INFO :Length ROM ID 0x08 = 0x08
       App:INFO :ROM ID (Len=8)
      52 4F 4D 5F    5F 5F 49 44
       App:INFO :Status Word (SW) (Len=2)
      90 00
       App:INFO :ex_sss Finished



