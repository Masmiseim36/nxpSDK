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
Tensilica HiFi DAB+ Decoder
======================================================================

======================================================================
Revision History
======================================================================

Version 2.14 API 1.15: August 27, 2020

+ GA release
+ Updated copyright notice for header files.
+ Programmer's Guide updated

----------------------------------------------------------------------

Version 2.13 API 1.15: December 4, 2018

+ GA release
+ Programmer's Guide updated

----------------------------------------------------------------------
Version 2.12 API 1.15: July 13, 2015

[J238] Added a new API for MPEG Surround flag.
[J987] Added APIs to control concealment behavior (adjusting the lengths
   of fade-out, mute release and fade-in periods).

----------------------------------------------------------------------

Version 2.11 API 1.15: November 9, 2010
- Added API to indicate presence of DRC data 
  (based on ETSI TS 102 563 v1.1.1, section 5.4.2)
- Updated Programmer's Guide to include the new API

----------------------------------------------------------------------
Version 2.10 API 1.15: June 16, 2010
Fix PAD data extraction issues

----------------------------------------------------------------------
Version 2.6 API 1.15: March 19, 2010

Insert zeros when bad superframe is detected.
Fixed noise issue at the beginning of switching to a different bitrate.
Improved error reporting mechanism.

----------------------------------------------------------------------

Version 2.5 API 1.15: March 11, 2010

Fix noise and overflow problems in some erronous streams.
Add DAB specific error concealment.(This only applies only to DAB+ formats).

----------------------------------------------------------------------
Version 2.3 API 1.15: November 3, 2008

Initial customer release.

----------------------------------------------------------------------
2022-09-20:05:39:RI-2019.2:616665:608059:616658::
