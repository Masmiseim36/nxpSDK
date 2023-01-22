#
# Copyright (c) 2014-2022 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
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
Cadence Tensilica HiFi Opus Codec
======================================================================

======================================================================
Revision History
======================================================================

Version 1.19 API 1.0: June 10, 2022

+ GA release
+ Tools: RI.8 tools and xt-clang compiler.
+ Code further optimized for HiFi1 and HiFi5.
+ PG doc is updated.

---------------------------------------------------------------------

Version 1.18_Beta API 1.0: January 12, 2022

+ Beta release using RI.8 tools and xt-clang compiler.
+ Code is optimized for HiFi 1 DSP.
+ PG doc is not updated.

---------------------------------------------------------------------

Version 1.17p1 API 1.0: September 28, 2021

+ TENA-3037, avoided use of global variable in lib.
+ Tools : RI.7 with xt-clang compiler

----------------------------------------------------------------------
Version 1.16_Alpha API 1.0: September 3, 2021

+ Alpha release using RI.7 tools and optimized code for HiFi1 DSP.
+ PG doc is not updated.

----------------------------------------------------------------------
Version 1.15 API 1.0: March 5, 2021

+ GA release.
+ Now released in single package that contains 3 libraries (codec,
    decoder and encoder).

----------------------------------------------------------------------
Version 1.14 API 1.0: February 10, 2021

+ GA release based on OPUS reference C code version 1.3.1.
+ Now released in 3 packages: codec, decoder and encoder.
+ Programmer's Guide updated
+ Tools : RI.5 with xt-clang compiler

----------------------------------------------------------------------
Version 1.13_Beta API 1.0: December 23, 2020
+ Beta release with RI.5 tools and xt-clang compiler.
+ Opus reference Code upgraded to version 1.3.1 
+ Programmer's Guide added in draft format.

---------------------------------------------------------------------
Version 1.12 API 1.0: November 5, 2020

+ GA release.
+ Programmer's Guide updated

----------------------------------------------------------------------

Version 1.11 API 1.0: August 11, 2020

+ GA release.
+ Updated copyright notice for header files.

----------------------------------------------------------------------

Version 1.10 API 1.0: May 26, 2020

+ Built with xt-xcc compiler with RI.4 tools (HiFi 3z only).
+ Verified performance and functionality with xt-clang compiler.

----------------------------------------------------------------------

Version 1.9 API 1.0: November 15, 2019

+ GA release
+ HiFi 5 only with RI.2 tools
+ Programmer's Guide not updated

----------------------------------------------------------------------

Version 1.8p1 API 1.0: August 28, 2019

+ Patch release - Opus codec on HiFi3z core only
+ Built with RG5 tools

[J2299] Fix for issues reported by static code analysis tool.

----------------------------------------------------------------------

Version 1.8 API 1.0: May 25, 2018

- GA release.
- Programmer's Guide updated.

----------------------------------------------------------------------

Version 1.7_Beta API 1.0: Mar 29, 2018

- Beta Release - HiFi3, HiFi4 only
- Built with RG.5 Tools
- PG in draft format

- Added multichannel decode support for Opus Decoder.
[J1972] Fixed a precision issue in silk_noise_shape_quantizer_del_dec() 
        in HiFi2 optimization 
[J2014] Fixed a precision issue in kf_bfly3() in HiFi2 optimization

----------------------------------------------------------------------

Version 1.6 API 1.0: Jan 11, 2018

- GA release.
- PG updated, Fusion F1 performance data included.

----------------------------------------------------------------------

Version 1.5p1 API 1.0: Dec 21, 2017

- Patch Release - HiFi3 LE5/LE7 only
- Built with RG.5 Tools

[J1893] Fixed OPUS_MOVE macro implementation.
[J1900] Fixed input argument type for MULT16_16SU optimized function.

----------------------------------------------------------------------

Version 1.5 API 1.0: Dec 1, 2017

- Updated implementation to Reference source code v1.2.1.
- Optimized for HiFi2 and HiFi3.

----------------------------------------------------------------------

Version 1.4 API 1.0: May 16, 2017

- Updated implementation to Reference source code v1.1.3.
- Added Ogg container support for Opus Decoder.

----------------------------------------------------------------------

Version 1.2 API 1.0: August 15, 2016

[J1383] Fixed an issue of inconsistent MCPS numbers in PG.  Added a new
    command line option -force_mode to forcibly set encoding mode.

- Enhanced robustness by checking configuration parameter combinations.

----------------------------------------------------------------------

Version 1.1 API 1.0: July 22, 2015

[J1057] Fixed an encoder bug that causes crash.

----------------------------------------------------------------------

Version 1.0.p1 API 1.0: March 27, 2015

- Bug fix: TENA-978 : Encoder failure fixed. (crashing, freezing, corrupt encoded stream).
- Bug fix: TENA-985 : Handle made 8-byte aligned in decoder and encoder as explained in PG-doc.

----------------------------------------------------------------------

Version 1.0 API 1.0: June 13, 2014

Initial customer release.

----------------------------------------------------------------------


2022-06-08:05:14:RI-2021.8:746801:745268:746182::
