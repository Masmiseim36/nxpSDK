..
    Copyright 2019,2020 NXP

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

- Build Plug & Trust middleware stack. (Refer :ref:`building`)


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


If everything is successful, the output will be similar to:

.. literalinclude:: out_ex_ecc.rst.txt
   :start-after: sss   :WARN :!!!Not recommended for production use.!!!


