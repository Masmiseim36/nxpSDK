#
# Copyright (c) 2006-2020 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
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
Tensilica HiFi Vorbis Decoder (with or without OGG container)
======================================================================

======================================================================
Revision History
======================================================================

Version 1.12 API 1.16: August 1, 2017

+ GA release

----------------------------------------------------------------------

Version 1.11p3 API 1.16: July 7, 2017

[J1662] Fixed a code crash issue occurring in case of input seek in 
    the middle of page decoding.

[J1729] Fixed two memset calls with incorrect size in the library.

[J1737] Fixed input consumption in case of Ogg pages with continued 
    packets.

- Testbench code clean-up.
- Released for HiFi3 with RF.2 Tools and REDUCE_ROM_SIZE=1. 

----------------------------------------------------------------------

Version 1.11p2 API 1.16: May 16, 2017

[J1647]
- Added XA_VORBISDEC_CONFIG_PARAM_OGG_MAX_PAGE_SIZE configuration 
  parameter to control maximum Ogg Page size (in turn input buffer 
  size) to be supported.
- Added XA_VORBISDEC_CONFIG_PARAM_RUNTIME_MEM configuration parameter 
  to control persistent and scratch memory allocation to typical and 
  worst case values respectively.

----------------------------------------------------------------------

Version 1.11p1 API 1.16: February 28, 2017

[J1557]
- This release includes a version targetd for reduced code size.
- Packscript with _REDUCE_ROM_SIZE suffix will generate the required xws.
- In the makefile, changes are protected under REDUCE_ROM_SIZE tag.
- make to be invoked with REDUCE_ROM_SIZE=1 in order to build for this version.
- Tested for HiFi3 using RF2.
- No code and PG changes.
- Codesize, table size and MCPS numbers, for RF2/AE_HiFi3_LE5, are provided in
  the Readme.boxnote accompanied with this release.

----------------------------------------------------------------------
Version 1.11 API 1.16: June 29, 2015

[J1050] Added missing BSD license file COPYING to the doc directory.

----------------------------------------------------------------------

Version 1.10 API 1.16: May 18, 2015

[J894] Reduced memory footprint for OGG stream input.

----------------------------------------------------------------------

Version 1.9 API 1.16: August 5, 2013

[1687] Improved handling of erroneous streams.

[1809] Fixed the alignment exception issue.

----------------------------------------------------------------------

Version 1.8 API 1.16: December 3, 2012

[1620, 1625-26, 1630, 1654] Fixed issues related to decoder goes into infinite
    loop due to error streams or init failure (incomplete setup packet).

[1627, 1629, 1653, 1655, 1664-65, 1667, 1687, 1700] Fixed issues related to
    decoder crash due to unaligned API object or memory info table, invalid
    input length, setting raw mode in certain sequence, step play with certain
    parameters and with certain error streams.

[1678] Added support for raw streams missing comment header.

[1679] In the init stage, the decoder will continue with the initialization
    process after parsing identification header if there is sufficient data
    in the input buffer to do so, instead of returning with "insufficient data" error. 

----------------------------------------------------------------------

Version 1.7 API 1.16: December 29, 2011

- Enhancement: Added new APIs:
    - Small input chunk feed supprt (allowing input buffer to be filled
      with a small number of bytes instead of a full frame).
    - Stream position APIs.
    - Run-time init for flushing out the states of the decoder after stream
      position change.
    - Stream change notification (return non-fatal errors when an unexpected
      identification packet or header packet is received).

----------------------------------------------------------------------
Version 1.6 API 1.12: October 21, 2011

- New feature: Decode raw Vorbis streams (without OGG container). 
    - A new API is added to select decoding of raw or ogg Vorbis stream at 
      the initiliazation.  
    - The test bench is updated to call this new API via command line.
    - The Programmer's Guide is updated accordingly

----------------------------------------------------------------------

Version 1.5 API 1.12: October 19, 2006

First customer release.

----------------------------------------------------------------------
2019-12-10:08:17:RI-2019.2:469279:566169:481317::
