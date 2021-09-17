..
    Copyright 2019,2020 NXP



.. highlight:: bat

.. _ex-se05x-minimal:

=======================================================================
 SE05X Minimal example
=======================================================================

This project gets available memory from secure element. This is a good
starting point to work with SE05X at low level.


Building the Demo
=======================================================================

- Build Plug & Trust middleware stack. (Refer :ref:`building`)
- Project: ``se05x_Minimal``


Running the Example
=======================================================================

If you have built a binary, flash the binary on to the board and reset
the board.

If you have built an *exe* to be run from Windows using VCOM, run as::

    se05x_Minimal.exe <PORT NAME>

Where **<PORT NAME>** is the VCOM COM port.

On Raspberry-Pi or iMX board, run as::

    ./se05x_Minimal


Console output
=======================================================================

If everything is successful, the output will be similar to:

.. literalinclude:: out_se05x_Minimal.rst.txt
   :start-after: sss   :WARN :!!!Not recommended for production use.!!!
