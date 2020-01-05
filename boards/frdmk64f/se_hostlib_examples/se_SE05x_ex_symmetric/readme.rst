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

.. _ex-sss-symmetric:

=======================================================================
 Symmetric AES Example
=======================================================================

This project demonstrates symmetric cryptography - AES encryption and decryption operations.

Prerequisites
=======================================================================

- Build Plug and Trust middleware stack. (Refer :ref:`building`)


About the Example
=======================================================================
This example does a symmetric cryptography AES encryption and decryption operation.

It uses the following APIs and data types:
  - :cpp:func:`sss_symmetric_context_init()`
  - :cpp:enumerator:`kAlgorithm_SSS_AES_CBC` from :cpp:type:`sss_algorithm_t`
  - :cpp:enumerator:`kSSS_CipherType_AES` from :cpp:type:`sss_cipher_type_t`
  - :cpp:enumerator:`kMode_SSS_Encrypt` from :cpp:type:`sss_mode_t`
  - :cpp:func:`sss_cipher_one_go()`
  - :cpp:enumerator:`kMode_SSS_Decrypt` from :cpp:type:`sss_mode_t`


Console output
=======================================================================

#) If everything is setup correctly the output would be as follows ::

     App:INFO :Do Encryption
     App:INFO :iv (Len=16)
        00 00 00 00  00 00 00 00   00 00 00 00   00 00 00 00
     App:INFO :srcData (Len=16)
        48 45 4C 4C  4F 48 45 4C   4C 4F 48 45   4C 4C 4F 31
     App:INFO :Encryption successful !!!
     App:INFO :encrypted data (Len=16)
        32 A6 04 88  C5 B3 FF 40   50 AF 56 A5   68 AE D1 05
     App:INFO :Do Decryption
     App:INFO :iv (Len=16)
        00 00 00 00  00 00 00 00   00 00 00 00   00 00 00 00
     App:INFO :Encrypted data (Len=16)
        32 A6 04 88  C5 B3 FF 40   50 AF 56 A5   68 AE D1 05
     App:INFO :Decryption successful !!!
     App:INFO :decrypted data (Len=16)
        48 45 4C 4C  4F 48 45 4C   4C 4F 48 45   4C 4C 4F 31
     App:INFO :ex_sss_symmetric Example Success !!!...



