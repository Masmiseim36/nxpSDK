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

.. _ex-sss-ecc:

=======================================================================
 ECC Example
=======================================================================

This project demonstrates Elliptic Curve Cryptography sign and verify operation 
using SSS APIs

Prerequisites
=======================================================================

- Build Plug and Trust middleware stack. (Refer :ref:`building`)


About the Example
=======================================================================
This example does a elliptic curve cryptography signing and verify operation.

It uses the following APIs and data types:
  - :cpp:func:`sss_asymmetric_context_init()`
  - :cpp:enumerator:`kAlgorithm_SSS_SHA256` from :cpp:type:`sss_algorithm_t`
  - :cpp:enumerator:`kMode_SSS_Sign` from :cpp:type:`sss_mode_t`
  - :cpp:func:`sss_asymmetric_sign_digest()`
  - :cpp:enumerator:`kMode_SSS_Verify` from :cpp:type:`sss_mode_t`
  - :cpp:func:`sss_asymmetric_verify_digest()`


Console output
=======================================================================


#) If everything is setup correctly the output would be as follows ::

    App:INFO :Running Example ex_sss_ecc.c
    App:INFO :Do Signing
    App:INFO :digest (Len=32)
        48 65 6C 6C   6F 20 57 6F   72 6C 64 00   00 00 00 00
        00 00 00 00   00 00 00 00   00 00 00 00   00 00 00 00
    App:INFO :signature (Len=70)
        30 44 02 20   0E 68 55 BE   42 2B 07 1D   63 05 4D 88
        85 30 77 0D   E0 4F DF 6F   1D 45 B6 44   68 32 A2 10
        34 B7 DB 0E   02 20 64 AB   7E 2F 56 F8   1E 39 EB 00
        C6 88 49 B7   AA 78 38 37   03 F6 69 0A   A6 97 7F D4
        2E 1C C9 7A   4C 3C
    App:INFO :Signing Successful !!!
    App:INFO :Do Verify
    App:INFO :digest (Len=32)
        48 65 6C 6C   6F 20 57 6F   72 6C 64 00   00 00 00 00
        00 00 00 00   00 00 00 00   00 00 00 00   00 00 00 00
    App:INFO :signature (Len=70)
        30 44 02 20   0E 68 55 BE   42 2B 07 1D   63 05 4D 88
        85 30 77 0D   E0 4F DF 6F   1D 45 B6 44   68 32 A2 10
        34 B7 DB 0E   02 20 64 AB   7E 2F 56 F8   1E 39 EB 00
        C6 88 49 B7   AA 78 38 37   03 F6 69 0A   A6 97 7F D4
        2E 1C C9 7A   4C 3C
    App:INFO :Verification Successful !!!
    App:INFO :ex_sss_asymmetric Test Success !!!...


