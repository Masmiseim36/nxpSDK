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

.. _ex-sss-hkdf:

=======================================================================
 HKDF Example
=======================================================================

This project demonstrates an HMAC Key derivation operation based on 
info and salt using SSS APIs.

Prerequisites
=======================================================================

- Build Plug and Trust middleware stack. (Refer :ref:`building`)


About the Example
=======================================================================
This example does a HMAC Key derivation operation based on the info and salt.


It uses the following APIs and data types:
  - :cpp:func:`sss_derive_key_context_init()`
  - :cpp:enumerator:`kAlgorithm_SSS_HMAC_SHA256` from :cpp:type:`sss_algorithm_t`
  - :cpp:enumerator:`kMode_SSS_ComputeSharedSecret` from :cpp:type:`sss_mode_t`
  - :cpp:enumerator:`kSSS_CipherType_HMAC` from :cpp:type:`sss_cipher_type_t`
  - :cpp:func:`sss_derive_key_go()`


Console output
=======================================================================


#) If everything is setup correctly the output would be as follows ::

       App:INFO :Do Key Derivation
       App:INFO :salt (Len=32)
           AA 1A 2A E3   B2 76 15 4D   67 F9 D8 4C   B9 35 54 56
           BB 1B 2B 03   04 05 06 07   08 09 0A 0B   0C 0D 0E 0F
       App:INFO :info (Len=192)
           00 01 02 03   04 05 06 07   08 09 0A 0B   0C 0D 0E 0F
           10 11 12 13   14 15 16 17   18 19 1A 1B   1C 1D 1E 1F
           20 21 22 23   24 25 26 27   28 29 2A 2B   2C 2D 2E 2F
           30 31 32 33   34 35 36 37   38 39 3A 3B   3C 3D 3E 3F
           40 41 42 43   44 45 46 47   48 49 4A 4B   4C 4D 4E 4F
           50 51 52 53   54 55 56 57   58 59 5A 5B   5C 5D 5E 5F
           60 61 62 63   64 65 66 67   68 69 6A 6B   6C 6D 6E 6F
           70 71 72 73   74 75 76 77   78 79 7A 7B   7C 7D 7E 7F
           80 81 82 83   84 85 86 87   88 89 8A 8B   8C 8D 8E 8F
           90 91 92 93   94 95 96 97   98 99 9A 9B   9C 9D 9E 9F
           A0 A1 A2 A3   A4 A5 A6 A7   A8 A9 AA AB   AC AD AE AF
           B0 B1 B2 B3   B4 B5 B6 B7   B8 B9 BA BB   BC BD BE BF
       App:INFO : Key Derivation successful !!!
       App:INFO :hkdfOutput (Len=128)
           C0 BE 2B D5   37 54 6C B5   A7 CA 37 27   9A BC 7F 83
           15 9F 87 17   75 D6 83 10   BF 09 49 02   50 C6 BC D8
           FB E5 2E F8   67 EC F8 4B   3D 98 5E 8E   63 0B D1 8F
           37 02 88 15   CA 14 50 D7   A1 79 B7 43   F6 D3 0A B8
           FB 74 51 06   BD C3 87 B4   B0 E9 1F EA   B6 9A 9D 56
           10 46 ED 3F   71 CE 50 25   D1 3F D5 83   C6 20 4A BA
           48 A5 AA DC   74 81 42 5B   72 62 F8 84   6F A7 2A BB
           BB 4D 42 52   10 CA 54 9E   AF 38 E8 D7   42 0A 28 6B
       App:INFO :ex_sss_hkdf Example Success !!!...



