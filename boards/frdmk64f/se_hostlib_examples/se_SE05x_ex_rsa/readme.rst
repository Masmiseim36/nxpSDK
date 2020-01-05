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

.. _ex-sss-rsa:

=======================================================================
 RSA Example
=======================================================================

This project demonstrates RSA sign and verify operations using SSS APIs.

Prerequisites
=======================================================================

- Build Plug and Trust middleware stack. (Refer :ref:`building`)


About the Example
=======================================================================
This example does a RSA signing and verify operation.

It uses the following APIs and data types:
  - :cpp:func:`sss_asymmetric_context_init()`
  - :cpp:enumerator:`kAlgorithm_SSS_RSASSA_PKCS1_PSS_MGF1_SHA256` from :cpp:type:`sss_algorithm_t`
  - :cpp:enumerator:`kMode_SSS_Sign` from :cpp:type:`sss_mode_t`
  - :cpp:enumerator:`kSSS_CipherType_RSA` from :cpp:enumerator: `sss_cipher_type_t`
  - :cpp:func:`sss_asymmetric_sign_digest()`
  - :cpp:enumerator:`kMode_SSS_Verify` from :cpp:type:`sss_mode_t`
  - :cpp:func:`sss_asymmetric_verify_digest()`

.. note::
    This example tries to delete key first. Deletion would be successful, if the key already exists. 
    Otherwise it would return an error message which is perfectly alright and the example could be successfully executed. 


Console output
=======================================================================

#) If everything is setup correctly the output would be as follows ::

       App:INFO :Do Signing
       App:INFO :digest (Len=32)
          00 01 02 03   04 05 06 07   08 09 0A 0B   0C 0D 0E 0F
          10 11 12 13   14 15 16 17   18 19 1A 1B   1C 1D 1E 1F
       App:INFO :Signing successful !!!
       App:INFO :signature (Len=256)
          35 AD 29 BF   AD 96 1E F1   F1 43 06 A6   68 E9 57 2A
          CE 13 F9 55   6E FA 63 BE   AD 6A 15 C6   51 D0 59 BE
          8D 8A 78 70   C6 BF 07 09   24 91 03 91   0A 25 82 EB
          CE 21 90 72   9A 75 1B E6   B5 9E 20 7D   26 5E 0B 0E
          9D E3 D5 F1   58 FA 92 85   AE 61 38 B5   04 CF 0B 25
          D6 FF 32 99   82 D8 12 1A   94 40 8E AB   8C 77 C7 CF
          85 F7 A7 B7   9D 46 9C 63   03 41 E1 E9   09 B1 0B 51
          24 20 17 83   0D 02 4B AA   C9 39 79 66   1C D0 8B 7D
          E8 DD 94 EB   BD 3F 25 89   32 2A 64 42   53 BE 77 4A
          4A 3E 03 0A   9E 0B BC 44   2D E4 DB F7   6D 8C 76 0A
          02 19 00 7B   00 05 C3 EC   F8 8D F2 67   C2 45 F2 C8
          5F 9C EA 7D   BB 9E 9F 1B   81 7C 37 D0   B2 6D 13 DB
          A6 56 64 34   F5 81 1A 4C   BE CF 95 04   E0 83 29 1E
          79 97 41 C7   30 BC A7 1A   D5 07 11 5D   69 33 54 69
          F0 41 0D 18   64 D5 6A 9D   97 80 FD 50   AB 8B 3A 37
          16 58 67 49   7C D4 41 2D   A4 A0 4B 4D   7D FD 04 40
       App:INFO :Do Verification
       App:INFO :Verification successful !!!
       App:INFO :ex_sss_RSA Example Success !!!...



