..
    Copyright 2019,2020 NXP


.. highlight:: bat

.. _ex-sss-ecc:

=======================================================================
 ECC Example
=======================================================================

This project demonstrates Elliptic Curve Cryptography sign and verify operation
using SSS APIs

Refer - :file:`simw-top/sss/ex/ecc/ex_sss_ecc.c`

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


