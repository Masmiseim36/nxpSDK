..
    Copyright 2019,2020 NXP


.. highlight:: bat

.. _ex-sss-hkdf:

=======================================================================
 HKDF Example
=======================================================================

This project demonstrates an HMAC Key derivation operation based on
info and salt using SSS APIs.

Refer - :file:`simw-top/sss/ex/hkdf/ex_sss_hkdf.c`

Prerequisites
=======================================================================

- Build Plug & Trust middleware stack. (Refer :ref:`building`)


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


If everything is successful, the output will be similar to:

.. literalinclude:: out_ex_hkdf.rst.txt
   :start-after: sss   :WARN :!!!Not recommended for production use.!!!


