..
    Copyright 2019,2020 NXP


.. highlight:: bat

.. _ex-sss-md:

=======================================================================
 Message Digest Example
=======================================================================

This project demonstrates a Message Digest / hashing operation using SSS APIs.

Refer - :file:`simw-top/sss/ex/md/ex_sss_md.c`

Prerequisites
=======================================================================

- Build Plug & Trust middleware stack. (Refer :ref:`building`)


About the Example
=======================================================================
This example does a Message Digest hashing operation.

It uses the following APIs and data types:
  - :cpp:func:`sss_digest_context_init()`
  - :cpp:enumerator:`kAlgorithm_SSS_SHA256` from :cpp:type:`sss_algorithm_t`
  - :cpp:enumerator:`kMode_SSS_Digest` from :cpp:type:`sss_mode_t`
  - :cpp:func:`sss_digest_one_go()`



Console output
=======================================================================


If everything is successful, the output will be similar to:

.. literalinclude:: out_ex_md.rst.txt
   :start-after: sss   :WARN :!!!Not recommended for production use.!!!


