#
# Copyright (c) 2022 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
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
Tensilica HiFi MP3 5.1-Channel Decoder:
MPEG-1/2/2.5 Layers I, II and III 2-Channel Decoding
MPEG-2 Layer II 5.1-Channel Decoding

Tensilica HiFi MP3 Decoder:
MPEG-1/2/2.5 Layers I, II and III 2-Channel Decoding

Tensilica HiFi MP2 Decoder:
MPEG-1/2 Layers I and II 2-Channel Decoding
======================================================================

======================================================================
Revision History
======================================================================
Version 3.26 API 1.17: July 22, 2022

+ GA release
+ Built using RI.8 tools.
+ Programmer's Guide updated.

----------------------------------------------------------------------

Version 3.25.beta API 1.17: April 07, 2022

+ Beta release of MP3 Decoder using RI.8 tools for HiFi DSPs.
+ Performances optimized for HiFi 1.
+ Improved code size.
+ Programmer's Guide updated.

----------------------------------------------------------------------

Version 3.24.beta API 1.16: March 30, 2022

+ Beta release of MP3 Decoder using RI.7 tools for HiFi 5 DSP.
+ Code size and Performances optimized for HiFi 5.
+ PG doc is not updated.

----------------------------------------------------------------------

Version 3.23.beta API 1.16: February 11, 2022

+ Beta release of MP3 Decoder using RI.8 tools for HiFi 1 DSP.
+ Optimized for HiFi 1.
+ PG doc is not updated.

----------------------------------------------------------------------

Version 3.23.alpha API 1.16: September 3, 2021

+ Alpha release of MP3 Decoder using RI.7 tools and Optimized for HiFi 1 DSP.
+ PG doc is not updated.

----------------------------------------------------------------------

Version 3.22 API 1.16: November 20, 2020

+ GA release
+ Built with RI.5 tools for HiFi 3z LE7 only.

----------------------------------------------------------------------

Version 3.21 API 1.16: October 22, 2020

+ GA release
+ Programmer's Guide updated

----------------------------------------------------------------------

Version 3.20 API 1.16: July 14, 2020

+ GA release
+ Updated copyright notice for header files.

----------------------------------------------------------------------

Version 3.19 API 1.16: October 22, 2019

+ GA release
+ HiFi 5 only with RI.2 tools
+ Programmer's Guide not updated
----------------------------------------------------------------------

Version 3.18p3 API 1.16: September 04, 2019
+ Source code Patch release for HiFi3z
+ Built with RG.5 tools
[J2299]: Fix for issues observed with static code analysis tool.
----------------------------------------------------------------------

Version 3.18p2 API 1.16: March 20, 2019
+ Patch release for HiFi2
+ Built with RG.5 tools

[J2215] MP3 Decoder: Decoded stream has unexpected jumps.
----------------------------------------------------------------------

Version 3.18p1 API 1.16: March 20, 2019
+ Patch release
+ Built with RG.5 tools

[J2215] MP3 Decoder: Decoded stream has unexpected jumps.
----------------------------------------------------------------------
Version 3.18 API 1.16: December 4, 2018

+ GA release.
+ Programmer's Guide updated.

----------------------------------------------------------------------
Version 3.17p4 API 1.16: September 14, 2018
+ Patch release - HiFi3 LE5, source code release
+ Built with RG.5 tools

[J2119] mp3mch_dec: Error in valid frame checks.
----------------------------------------------------------------------
Version 3.17p3 API 1.16: November 27, 2017
+ Patch release - HiFi4 LE7, pic variant only
+ Built with RG.5 tools

[J1852] Added configuration option to relax standard validity checks
    on input bitstream.
----------------------------------------------------------------------

Version 3.17p2 API 1.16: September 28, 2017
+ Patch release - HiFi3 LE5 only
+ Built with RG.5 tools
+ Same as 3.17p1 release : built using "code size optimized" codebase.

[J1803] Fix for the stream-decode issue.
----------------------------------------------------------------------

Version 3.17p1 API 1.16: September 22, 2017
+ Patch release - HiFi3 LE5 only
+ Built with RG.5 tools

[J1803] Fix for the stream-decode issue .
----------------------------------------------------------------------

Version 3.17 API 1.16: September 1, 2017

+ GA release.

----------------------------------------------------------------------
Version 3.16 API 1.16: June 5, 2017

+ Initial release for HiFi3Z.

Section 1.4.3 MP3 Decoder Memory
Text 26.3, Data 14.8

Section 1.4.4 MP3 Decoder Timings
44.1, 2, 128,  3.1,
44.1, 2, 320,  3.8,
48,   2, 128,  3.2,
48,   2, 320,  4.1,

----------------------------------------------------------------------

Version 3.15 API 1.16: March 3, 2017

+   GA release for previous version 3.14.3.

----------------------------------------------------------------------

Version 3.14.3 API 1.16: February 21, 2017

+   Built on HiFi2 with RD.5 tools, slight degradation of up to 5% in 
    MCPS may be observed w.r.t. version 3.14.2(built on HiFi2 with
    RF.2 tools).

----------------------------------------------------------------------

Version 3.14.2 API 1.16: February 21, 2017

[J1583] Added a sanity check for limitting bound(joint stereo bound) 
    to sblimit(subband limit) if it is exceeding sblimit at layer2
    decoding module.

----------------------------------------------------------------------

Version 3.14.1 API 1.16: February 15, 2017

[J1583] Fixed an issue related to AE_LB instruction on HiFi2 core
    with zero as input value. This issue does not exist for HiFi3 or
    HiFi4 cores.

----------------------------------------------------------------------

Version 3.14 API 1.16: December 4, 2015

Fixed issues with sign extending 24 bit numbers to 32 bit in HiFi 4.

----------------------------------------------------------------------

Version 3.13 API 1.16: March 5, 2014.

Enhancement: The decoder is optimized to reduce CPU load for
    HiFi Mini and HiFi 2 packages.

[1922] Fixed an issue related to decoder not incrementing the byte
    count when processing certain error streams.

----------------------------------------------------------------------

Version 3.12 API 1.16: April 18, 2013

Enhancement: The decoder is optimized for HiFi 3 to reduce CPU load
  (for HiFi 3 package only).

[1781] Fixed the issue related to extension stream parsing when the
    main audio is mono (mp3mch only).

[1782] Fixed the CRC issue caused by incorrect parsing of
    dyn_cross_mode (mp3mch only).

----------------------------------------------------------------------

Version 3.11 API 1.16: July 18, 2012

- Updated html documentation in xws files.
- Clean up of files in test_ref directory.

----------------------------------------------------------------------

Version 3.10 API 1.16: February 29, 2012

- Enhancement:  Added a feature to indicate a possible stream parameter
    change in the number of channels, sampling frequency and layer as
    error message.  Application can re-confirm this error message and
    may decide to do a re-initialization for decoding the remaining stream. 
- First release in xws (Xplorer workspace) file form.

----------------------------------------------------------------------

Version 3.9 API 1.16: December 7, 2011

- Enhancement:  Added new APIs that return copyright and multi-channel
    extension header related metadata.

----------------------------------------------------------------------

Version 3.8 API 1.16: July 26, 2011

- Enhancement:  Added a new API that can enable VLC rewind ( a method
  of error recovery).

----------------------------------------------------------------------
Version 3.7 API 1.16: July 5, 2011

- Bug fix: took out a debug message.
  Note: This bug only existed in the 5.1-Channel Decoder (mp3mch).
  There are no changes to the 2-channel mp3 and mp2 decoders.

- Bumped API version number to 1.16 to reflect the input buffer 
  handling enhancement made in Version 3.6 
  (No code changes).

----------------------------------------------------------------------
Version 3.6 API 1.15: December 20, 2010

- Enhancement: allow input buffer to be filled with any number of bytes 
      instead of 2 full frames to decode.

- Bug fix: bad PCM outputs caused by corrupt huffman codes in bit-stream.
- Bug fix: when multi-channel decoding is enabled, bad decoding of 
           layer2 bitstream when both 1st and 2nd frames are corrupted 
      
----------------------------------------------------------------------

Version 3.5 API 1.15: September 7, 2010

Bug fix: multi-channel mp3 decoder only (mp3mch_dec) - failure in decoding
mp3 file when the first frame is corrupted.

----------------------------------------------------------------------

Version 3.4 API 1.15: April 9, 2010

Sync-lost error reporting is now implemented.
----------------------------------------------------------------------

Version 3.3 API 1.15: January 27, 2010

Fixed CRC error checking for ScFCRC.
----------------------------------------------------------------------

Version 3.2 API 1.15: November 30, 2009

Add DAB specific scale factor CRC check.

Add a new error code to return meaningful error when invalid bitrate and
mode combination is detected.

Fix error code returned when layer change is detected.

Fix bit rate range for MPEG-1 Layer II.

----------------------------------------------------------------------

Version 3.1 API 1.15: December 12, 2008

Add a CHMODE_INFO parameter to present the channel mode and mode
extension frame header fields to the application.

Fix minor performance degradation introduced in Version 3.0.

Support C-stub library builds.

----------------------------------------------------------------------

Version 3.0 API 1.15: June 20, 2008

Implement support for MPEG-2 Layer II 5.1-channel decoding.

----------------------------------------------------------------------

Version 2.2 API 1.15: April 8, 2008

Implement a separate MP2 Decoder package providing support only for
MPEG-1/2 Layers I and II 2-channel decoding with lower memory
requirements.

Add stream-position API commands to allow input/output data mapping.

Build the object code library with Xtensa Tools version 7.1.1.

Show the current playback time in the progress output.

----------------------------------------------------------------------

Version 2.1 API 1.14: September 6, 2007

Fix a signal scaling issue causing minor inaudible distortions in the
output for some intensity stereo streams.

Fix an overflow issue causing minor inaudible distortions in the
output for some Layer I and Layer II streams.

Update the stream-dependent configuration parameters (data rate,
number of channels) for each processed frame.

Sample testbench improvements: optimize input buffer movement;
optimize file output for little-endian target processors; prevent
display of unprintable ID3 tag contents.

----------------------------------------------------------------------

Version 2.0 API 1.14: May 16, 2007

Major performance, memory requirements and accuracy improvements.

Add CRC check for Layers I and II.

Add a runtime-init API command for smooth trick-play support. Modify
the sample testbench to demonstrate step-play using the new command.

Fix a bug in the intensity stereo module causing distorted output.

Fix a bug in the sample testbench causing inconsistent WAV header
generation when the output PCM sample size is set to 24 bits.

Fix a bug in the sample testbench causing a crash when processing
certain ID3 tags.

Build with -mlongcalls.

Keep literals as a separate section in the object code library.

Clean up all compile time warnings from the library and the sample
testbench.

----------------------------------------------------------------------

Version 1.5 API 1.12: October 4, 2006

Initial customer release.

----------------------------------------------------------------------
2022-07-22:09:12:RI-2021.8:754469:745268:754457::
