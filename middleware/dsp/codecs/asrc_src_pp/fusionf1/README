#
# Copyright (c) 2011-2020 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
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
Tensilica HiFi Aynchronous and Synchronous Sample Rate Converter (SRC)
======================================================================

======================================================================
Revision History
======================================================================

Version 1.10 API 1.12: April 13, 2020

+ Changed package name from src_pp to asrc_src_pp.
[J2183] Added conversion to 4kHz and 6kHz.

----------------------------------------------------------------------
Version 1.9 API 1.12: July 30, 2018

+ GA release, REDUCE_ROM_SIZE=0.
+ Programmer's Guide updated.

----------------------------------------------------------------------

Version 1.8 API 1.12: May 8, 2018

+ GA release - HiFi 3z only
+ .tgz release with 2 libs and 2 test apps. One for base SRC and one for 384K support.

[J1820] Requesting separate HiFi 3z libs for SRC with reduced code size.
[J1734] SRC: Request to support 384 kHz conversion (for limited scenarios.)
    - This release includes a version targeted  for reduced code size.
    - Apart from base SRC, supports conversion to 384kHz for a few rates.

----------------------------------------------------------------------

Version 1.7_Beta API 1.12:  December 8, 2017

+ Beta release - HiFi3Z LE5 only
+ Built with RG.5 tools
 
[J1820] Requesting separate HiFi 3z libs for SRC with reduced code size.
        - This release includes a version targetd for reduced code size.
        - No code changes.
        - No PG update
  
        SRC Timings
        ======================================================================
        inrate     outrate    ch:1     ch:2      ch:6     ch:8    Cubic/Linear
        ======================================================================
        8k         44.1k     1.6        2.7       8.4      10.5
        16k        44.1k     2.9        4.7      14.3      17.8
        44.1k      44.1k     0.0        0.1       0.1       0.2
        48k        44.1k     6.4       10.9      33.7      42.3    Linear
        48k        44.1k    13.5       17.3      42.6      51.4    cubic

        8k         48k       0.6        1.3       3.9       5.2
        16k        48k       0.7        1.4       4.3       5.8
        44.1k      48k       4.5        6.7      20.8      25.0    Linear
        44.1k      48k      12.2       13.5      30.5      34.9    cubic
        48k        48k       0.1        0.1       0.3       0.4
        96k        48k       2.4        4.9      14.7      20.0
        192k       48k       3.8        7.7      23.0      31.5
                           
        48k        96k       1.8        3.9      11.7      15.8    Linear
        48k        96k       1.8        3.9      11.7      15.8    cubic
        96k        96k       0.1        0.2       0.6       0.8    Linear
        96k        96k       0.1        0.2       0.6       0.8    cubic
        192k       96k       4.8        9.8      29.4      40.0    Linear
        192k       96k       4.8        9.8      29.4      40.0    cubic
                           
        48k       192k       2.6        5.7      16.9      22.8    Linear
        48k       192k       2.6        5.7      16.9      22.8    cubic
        96k       192k       3.7        7.8      23.4      31.6    Linear
        96k       192k       3.7        7.8      23.4      31.6    cubic
        192k      192k       0.3        0.4       1.2       1.6    Linear
        192k      192k       0.3        0.4       1.2       1.6    cubic
        ======================================================================

----------------------------------------------------------------------

Version 1.6_Beta API 1.12: August 18, 2017

+ Beta release - HiFi3 LE5 only
+ Built with RF.2 tools
 
[J1734] 384kHz support is now handled by a new library with a new 
    API name xa_src384k_pp (xa_src384k_pp.a);
    xa_src_pp (xa_src_pp.a) handles all other conversion ratios.
  
----------------------------------------------------------------------

Version 1.5_Beta API 1.12: July 5, 2017

[J1734] 32 kHz, 44.1 kHz, 48 kHz, 96 kHz and 192 kHz to 384 kHz support added.
        384 kHz to 48 kHz support added.
----------------------------------------------------------------------

Version 1.4 API 1.12: June 6, 2017

Initial release for HiFi3Z.

Libary Memory Usage (page 9 of PG)
SRC: Text 34.0, Data 18.6

SRC Timings (page 12 of PG)
  1.1,   1.8,   5.9,   7.2,
  1.9,   3.1,  10.0,  12.2,
  0.0,   0.1,   0.1,   0.2,
  4.1,   6.8,  22.1,  27.0,
  8.7,  11.7,  27.4,  31.8,
  0.5,   1.1,   3.3,   4.4,
  0.6,   1.3,   4.0,   5.3,
  2.7,   3.8,  13.4,  15.1,
  7.7,   9.2,  19.2,  20.4,
  0.1,   0.1,   0.3,   0.4,
  1.7,   3.3,  10.0,  13.3,
  2.7,   5.2,  15.4,  20.6,
  1.6,   3.2,   9.5,  12.6,
  1.6,   3.2,   9.5,  12.6,
  0.1,   0.2,   0.6,   0.8,
  0.1,   0.2,   0.6,   0.8,
  3.4,   6.7,  20.0,  26.6,
  3.4,   6.7,  20.0,  26.6,
  2.1,   4.4,  13.2,  17.6,
  2.1,   4.4,  13.2,  17.6,
  3.1,   6.3,  18.9,  25.2,
  3.1,   6.3,  18.9,  25.2,
  0.2,   0.4,   1.2,   1.6,
  0.2,   0.4,   1.2,   1.6,

----------------------------------------------------------------------
Version 1.3p1 API 1.12: February 20, 2017

- This release includes a version targetd for reduced code size.
- Packscript with _REDUCE_ROM_SIZE suffix will generate the required xws.
- In the makefile, changes are protected under REDUCE_ROM_SIZE tag.
- make to be invoked with REDUCE_ROM_SIZE=1 in order to build for this version.
- Tested for HiFi3 using RF2.
- No code changes.
- Codesize, table size and MCPS numbers, for RF2/AE_HiFi3_LE5, are provided in
  the Readme.boxnote accompanied with this release.
----------------------------------------------------------------------
Version 1.3 API 1.12: November 5, 2014

- HiFi 3 release only.
- Supports the following new features compared to previous versions.
   + Flexible and customizable architecture.
   + New kernels and various quality filter tables added.
   + Supports both 16 and 24 bit PCM input data.
   + Supports up to 24 channels.
   + Supports all 15x15 possible conversion ratios.
   + Supports ASRC mode with drift range of -0.04 to 0.04 (with 1 PPM step).
   + Optional supports cubic interpolation for polyphase filers.

[2171-2174, 2176-2180, 2183, 2208] Improved code robustness.

----------------------------------------------------------------------

Version 1.2 API 1.12: March 25, 2014

[1964] Added support for 16-bit PCM I/O data.

----------------------------------------------------------------------

Version 1.1 API 1.12: June 28, 2013

[1705] Fixed persistent memory allocation issue when using custom-designed
    filter coefficients.
[1707] Fixed a missing variable initialization issue.
[1712] The input and output PCM data alignment is changed from right justified
    to left justified.  24-bit PCM data occupies 24 MSBs of a 32-bit word.
 
- Added support for custom-designed SRC module.
- Major clean up of API 
   + Cleaned up API control logic
   + Enhanced API robustness
   + Cleaned up error code to be more relevant to the error. Specifically, 
        Added the following new error codes
            XA_SRC_PP_EXECUTE_NON_FATAL_INVALID_CONFIG_SEQ
            XA_SRC_PP_EXECUTE_NON_FATAL_INVALID_API_SEQ
            XA_SRC_PP_EXECUTE_FATAL_ERR_EXECUTE
        Removed the following error code
            XA_SRC_PP_CONFIG_FATAL_INVALID_NUM_STAGES
        Return a fatal error for unsupported sample conversion ratios
- Major clean up of SRC control logic, code size is reduced.
- Cleaned up algo/utilities directory.
- Default channel count is changed from 1 to 2.
- Polyphase filter/fractional converter is optimized for performance.
- Updated testbench for calling "reset" SRC function at a user specified frame_count.
- Updated Programmer's Guide.

----------------------------------------------------------------------

Version 1.0 API 1.12: February 3, 2012

- Tested for THD/BW for conversions between 8, 16, 44.1, 48 kHz and
  any supported sample rate.
- Interleaved Stereo output provision added (active by default).
- Updated Programmer's guide.
  
----------------------------------------------------------------------

Version 0.8: November 25, 2011

- Added 128 <-> 48, 48 -> 64, 44.1 -> 64 and 128 <-> 44.1 kHz conversion
- Improved polyphase filter, anti-aliasing filters and cutoff by 1/3 filters
- Default non-halfband filter
  
----------------------------------------------------------------------

Version 0.7: September 30, 2011

- Added 88.2 -> 48, 64 -> 48 and 64 -> 44.1 kHz conversion
- Changes in Downsample by 6 and Downsample by 12 control code to 
  take care of any input chunk size which is a multiple of 4
  
----------------------------------------------------------------------

Version 0.6: September 21, 2011

- Supports integer conversion ratios upto 24
- Conversion from all standard sample rates to 8 kHz is supported now
- Bugfix in 44.1 kHz to 8kHz anti-aliasing

----------------------------------------------------------------------

Version 0.5: August 17, 2011

- Preliminary Release

----------------------------------------------------------------------
2020-04-13:12:56:RI-2019.2:588588:587929:552342::
