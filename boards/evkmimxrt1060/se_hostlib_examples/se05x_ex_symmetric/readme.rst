..
    Copyright 2019,2020 NXP


.. highlight:: bat

.. _ex-sss-symmetric:

=======================================================================
 Symmetric AES Example
=======================================================================

This project demonstrates symmetric cryptography - AES encryption and decryption operations.

Refer - :file:`simw-top/sss/ex/symmetric/ex_sss_symmetric.c`

Prerequisites
=======================================================================

- Build Plug & Trust middleware stack. (Refer :ref:`building`)


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


If everything is successful, the output will be similar to:

.. literalinclude:: out_ex_symmetric.rst.txt
   :start-after: sss   :WARN :!!!Not recommended for production use.!!!


