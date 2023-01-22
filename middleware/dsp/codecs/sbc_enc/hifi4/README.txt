#
# Copyright (c) 2009-2020 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
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
Tensilica HiFi SBC Encoder.
======================================================================

======================================================================
Revision History
======================================================================

Version 1.7 API 1.12: October 14, 2020

+ GA release.
+ Programmer's Guide updated.

----------------------------------------------------------------------

Version 1.6 API 1.12: July 21, 2020

+ GA release.
+ Updated copyright notice for header files.

----------------------------------------------------------------------

Version 1.5 API 1.12: September 4, 2017

+ GA release.

-------------------------------------------------------------------------
Version 1.4 API 1.12: February 7, 2014

*  Encoder library configuration parameter API changed to accept 
   bitpool value instead of bitrate value. Library internally 
   generates target bitrate. See Table 3.6 and Section 3.3 of 
   Programmer's Guide for details of new API and details of
   Bitpool to/from Bitrate conversion. 

*  [1937] [1938] [1939] [1940] Fixed bugs related to out-of-sequence 
   API calls. Library returns appropriate error message for 
   out-of-sequence API calls.
-------------------------------------------------------------------------
Version 1.3 API 1.12: July 24, 2013

[1832]  Fixed the bug related to syncword corruption.
-------------------------------------------------------------------------
Version 1.2 API 1.12:  October 16, 2012

* Input buffer alignment requirement changed to 4 bytes.
* Output buffer alignment requirement changed to 1 byte.
-------------------------------------------------------------------------
Version 1.1 API 1.12: July 8, 2009

Initial customer release.   
-------------------------------------------------------------------------
2020-10-14:09:52:RI-2019.2:628375:621908:628356::
