#
# Copyright (c) 2019-2022 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
# These coded instructions, statements, and computer programs are the
# copyrighted works and confidential proprietary information of
# Cadence Design Systems Inc.  They may be adapted and modified by bona fide
# purchasers for internal use, but neither the original nor any adapted
# or modified version may be disclosed or distributed to third parties
# in any manner, medium, or form, in whole or in part, without the prior
# written consent of Cadence Design Systems Inc.  This software and its
# derivatives are to be executed solely on products incorporating a Cadence
# Design Systems processor.
#
======================================================================
Cadence Tensilica HiFi LC3 Codec
======================================================================

======================================================================
Revision History
======================================================================
----------------------------------------------------------------------

Version 1.12 API 1.1: May 25, 2022

+ Code upgraded to latest LC3 source code
+ Tools : RI.8 with xt-clang compiler
+ Programmer's Guide updated 

----------------------------------------------------------------------

Version 1.11_Beta API 1.1: March 4, 2022

+ Code upgraded to latest LC3 source code
+ Tools : RI.8 with xt-clang compiler
+ No PG update

----------------------------------------------------------------------

Version 1.10_Beta API 1.1: December 21, 2021

+ TENA-3086, Fixed memory overflow issues observed in encoder with -fsanitize option
+ Tools : RI.8 with xt-clang compiler

----------------------------------------------------------------------

Version 1.9p1 API 1.1: September 28, 2021

+ TENA-3037, avoided use of global variable in lib.
+ TENA-3037, renamed basop_memmove function.
+ Tools : RI.7 with xt-clang compiler

Version 1.8 API 1.1: August 12, 2021

+ GA release based on LC3 specification 1.0.
+ Programmer's Guide updated
+ Code updated to use HiFi 1 specific optimizations
+ Tools : RI.7 with xt-clang compiler

----------------------------------------------------------------------

Version 1.7_Beta API 1.1: June 25, 2021

+ Beta release with RI.7 tools.
+ Code optimized for HiFi1 DSP. Minor changes done in code for other HiFi DSPs.
+ Programmer's Guide updated in draft format.

----------------------------------------------------------------------

Version 1.6 API 1.1: November 19, 2020

+ GA release based on LC3 specification 1.0.
+ Programmer's Guide updated
+ RI.5 tools with xt-clang compiler

----------------------------------------------------------------------

Version 1.5_Beta API 1.1: September 30, 2020

+ Beta release.
+ Code upgraded to latest LC3 source code and LC3 specifications version 1.0.
+ Further optimization to leverage STL2019 BASOPs
+ Programmer's Guide updated in draft format.

----------------------------------------------------------------------

Version 1.4 API 1.1: May 26, 2020

+ GA release.
+ Added support for separate encoder and decoder libraries.
+ Programmer's Guide updated.

----------------------------------------------------------------------

Version 1.3 API 1.1: May 5, 2020

+ GA release.
+ Updated to LC3 specification version 0.9.
+ Programmer's Guide updated.

----------------------------------------------------------------------

Version 1.2_Beta API 1.1: March 27, 2020

+ Beta release for HiFi 3, HiFiMIni and Fusion F1 AVS cores.
+ Supports latest LC3 specification version 0.9 which enables use of 7.5 ms frame size.
+ Programmer's Guide updated in draft format

----------------------------------------------------------------------

Version 1.1 API 1.1: December 10, 2019

+ GA release for HiFi 3.
+ Programmer's Guide updated.

----------------------------------------------------------------------
Version 1.0 API 1.1: November 21, 2019

Initial customer release.

----------------------------------------------------------------------


2022-05-26:04:38:RI-2021.7:742943:726678:744055::
