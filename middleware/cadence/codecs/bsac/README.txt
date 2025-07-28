#
# Copyright (c) 2008-2022 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
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
Tensilica HiFi BSAC Decoder
======================================================================

======================================================================
Revision History
======================================================================

Version 2.8 API 1.15: July 9, 2020

+ GA release.
+ Updated copyright notice for header files.

----------------------------------------------------------------------

Version 2.7 API 1.15: September 1, 2017 

+ GA release.

----------------------------------------------------------------------

Version 2.6p1 API 1.15: August 2, 2017 

+ Patch release - HiFi4 LE7 PIC version
+ Built with RF.2 tools

[J1726] Now the INPUT_OVER can be signaled as soon as the input file ends
[J1771] Fixed a computation mistake in the layer length calculation,
    that result in incorret decoding of few streams. 
[J1708] Fixed compiler warnings found with LLVM compiler

----------------------------------------------------------------------

Version 2.6 API 1.15: August 6, 2015

[J478]  Fixed an issue of distortion in the decoded output due to some
    stale data, even after RUNTIME_INIT API is called.
[J480, 491, 983, 998, 1007] Improved the robustness of the decoder library.
[J482] Enhanced the error reporting and handling logic at API level.
[J477, 479, 618, 1016-1018, 1026-1027] Imrpoved the robustness of the testbench.

----------------------------------------------------------------------

Version 2.5 API 1.15: April 25, 2012

[1410, 1481]: Added robustness handling for certain erroneous streams.

----------------------------------------------------------------------

Version 2.4 API 1.15: November 5, 2009

Bug fix: Handling of corrupted stream

Build the object code library with Xtensa Tools version 7.1.2.1

----------------------------------------------------------------------

Version 2.1 API 1.15: May 14, 2008

Initial customer release.

Build the object code library with Xtensa Tools version 7.1.1.

----------------------------------------------------------------------
2022-09-19:08:23:RI-2019.2:605342:604569:424455::
