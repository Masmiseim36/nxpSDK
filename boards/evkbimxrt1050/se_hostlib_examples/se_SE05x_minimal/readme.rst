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

.. _ex-se05x-minimal:

=======================================================================
 SE05X Minimal example
=======================================================================

This project gets available memory from secure element. This is a good 
starting point to work with SE05X at low level.


Prerequisites
=====================
- Micro USB cable
- Kinetis FRDM-K64F/imx-RT1050 board
- Personal Computer
- SE05x Arduino shield


Building the Demo
=======================================================================

- Build Plug and Trust middleware stack. (Refer :ref:`building`)
- Project: ``se05x_minimal``


Running the Example
=======================================================================

If you have built a binary, flash the binary on to the board and reset 
the board. 

If you have built an *exe* to be run from Windows using VCOM, run as::
	
	se05x_minimal.exe <PORT NAME>

Where **<PORT NAME>** is the VCOM COM port.


Console output
=======================================================================

If everything is setup correctly the output would be as follows ::

   		App:INFO :mem=32767
		App:INFO :ex_sss Finished



