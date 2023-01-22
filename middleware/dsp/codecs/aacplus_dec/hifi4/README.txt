#
# Copyright (c) 2006-2022 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
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
Tensilica HiFi AAC-LC, aacPlus (HE-AAC) and aacPlus v2 Decoder
======================================================================

======================================================================
Revision History
======================================================================

Version 3.16 API 1.16: October 11, 2022

+ GA release.
+ Built using RI-2022.9 toolchain with xt-clang compiler.
+ Programmer's Guide reviewed and updated.
+ AACplus v2 code optimized for HiFi1 DSP.

----------------------------------------------------------------------

Version 3.15 API 1.16: April 29, 2022

+ GA release.
+ Built using RI-2021.8 toolchain with xt-clang compiler.
+ Programmer's Guide reviewed and updated.
+ AAC stereo code optimized for HiFi1 DSP.

----------------------------------------------------------------------

Version 3.14_Beta API 1.16: February 25, 2022

+ Beta release.
+ Codesize tuned and code optimised for HiFi1.
+ PG doc is not updated.

----------------------------------------------------------------------

Version 3.13_Beta API 1.16: January 10, 2022

+ Beta release using RI.8 tools and xt-clang compiler.
+ Code is optimized for HiFi 1 DSP.
+ PG doc is not updated.

----------------------------------------------------------------------

Version 3.12_Alpha API 1.16: October 12, 2021

+ Alpha release using RI.7 tools and xt-clang compiler.
+ Code is optimized for HiFi 1 DSP.
+ PG doc is not updated.

----------------------------------------------------------------------

Version 3.11 API 1.16: October 20, 2020

+ GA release
+ Programmer's Guide updated

----------------------------------------------------------------------

Version 3.10 API 1.16: August 6, 2020

+ GA release (aac_dec only).
+ Updated copyright notice for header files.

----------------------------------------------------------------------

Version 3.9p1 API 1.16: June 29, 2020

+ Patch release - AAC decoder on HiFi2
+ Built with RD.5 tools
+ Programmer's Guide not updated

[J2560] Added a new non-fatal error type
    xa_trnspt_err_next_sync_not_found.
    When sync word for next frame is not detected,
    decoder discards current frame and returns this error.

----------------------------------------------------------------------

Version 3.9 API 1.12: May 20, 2020

+ Built with xt-xcc compiler with RI.4 tools (HiFi 3z only).
+ Verified performance and functionality with xt-clang compiler.

----------------------------------------------------------------------

Version 3.8p2 API 1.16: May 6, 2020

+ Patch release - AAC decoder on HiFi4 only
+ Built with RI.2 tools
+ Programmer's Guide not updated

[J2438] Added a set-configuration parameter 
    XA_AACDEC_CONFIG_PARAM_RESET_STATE_ON_SYNC_LOSS_ERROR to control
    reset of internal decoder states upon sync loss. By default, this
    parameter is set to 1 (i.e. internal decoder states are reset upon
    sync loss error).

----------------------------------------------------------------------

Version 3.8p1 API 1.16: November 25, 2019

+ Patch release - AAC decoder on HiFi2 LE7 call0
+ Patch release - AAC decoder on HiFi Mini LE7
+ Built with RG5 tools

[J2287] TNS scale related code back-ported from Hifi3 to fix noise.

----------------------------------------------------------------------

Version 3.8 API 1.16: October 18, 2019

+ GA release
+ HiFi 5 only with RI.2 tools
+ Programmer's Guide not updated

----------------------------------------------------------------------

Version 3.7p3 API 1.16: September 5, 2019

+ Patch release - AAC decoder on HiFi3z core only
+ Built with RG5 tools

[J2299] Fix for issues reported by static code analysis tool.

----------------------------------------------------------------------

Version 3.7p2 API 1.16: August 27, 2019

+ Patch release - AAC decoder on HiFi3z core only
+ Built with RG7 tools

[J2299] Fix for issues reported by static code analysis tool.

----------------------------------------------------------------------
Version 3.7p1 API 1.16: July 11, 2019

+ Patch release - AAC decoder on HiFi2 LE7 call0 only
+ Built with RG5 tools

[J2287] TNS scale related code back-ported from Hifi3 to fix noise.

----------------------------------------------------------------------
Version 3.7 API 1.16: April 23, 2018

+ Added support for MPEG4 Amendment 4 (runtime configurable)
+ Further HiFi3 optimizations in AAC-LC stereo path
+ Updated output channel routing feature
+ Added configuration parameter to export PCE information

----------------------------------------------------------------------
Version 3.6 API 1.16: August 14, 2017

+ GA release

----------------------------------------------------------------------
Version 3.5p4 API 1.16: July 27, 2017

+ Patch release - AAC Plus V2 decoder on HiFi4 LE7 only. PIC version
+ Built with RD.5 tools

[J1735] Decoder's input memory requirement is increased by 256 bytes
    to accomodate longer PCE (with comment fields with longer strings) 

----------------------------------------------------------------------

Version 3.5p3 API 1.16: June 27, 2017

+ Patch release - AAC Plus V2 decoder on HiFi4 LE7 only. PIC version
+ Built with RD.5 tools

[J1708] Fixed compiler warnings found with LLVM compiler
[J1735] Fixed the issue in which the decoder enters into infinite 
    loop instead of fatal error for unsupported ADIF streams 
[TENX-36125] Provided work around for a LLVM compiler bug in parametric stereo
    module of aacplus_v2 / aacmchplus_v2[_loas]

----------------------------------------------------------------------

Version 3.5.2 API 1.16: April 19, 2017

[J1658] Built on HiFi2 with RD.5 tools, slight degradation of up to 5% 
    in MCPS may be observed w.r.t. ver. 3.5.1 (built with RF.2 tools).

----------------------------------------------------------------------

Version 3.5.1 API 1.16: March 17, 2017

[J1602] Fixed a bug related to decoding of 8 channel stream with 
    Program Config Element. Also, fixed an issue regarding out of 
    bound access of output buffer.
     
----------------------------------------------------------------------

Version 3.5 API 1.16: March 14, 2017

Enhancement:  Added a second library that supports LOAS/LATM for aac,
    aacplus, aacplus_v2, aacmch and  aacmchplus packages.  No changes
    for aacmchplus_v2 and aacmchplus_v2_loas packages.
     
----------------------------------------------------------------------
     
Version 3.4.2 API 1.16: February 28, 2017
[J1557]
- This release includes a version targeted for reduced code size.
- Target is ONLY for aacmchplus_v2_loas.
- Packscript with _REDUCE_ROM_SIZE suffix will generate the required xws.
- In the makefile, changes are protected under REDUCE_ROM_SIZE tag.
- make to be invoked with REDUCE_ROM_SIZE=1 in order to build for this version.
- Tested for HiFi3 using RF2.
- No code or PG changes.
- Codesize, table size and MCPS numbers, for RF2/AE_HiFi3_LE5, are provided in
  the Readme.boxnote accompanied with this release.

----------------------------------------------------------------------

Version 3.4.1 API 1.16: August 19, 2016

[J1417] Built with RD.5 tools, slight degradation up to 5% in code size 
    or MCPS may be observed w.r.t. version 3.4 (built with RF.2 tools).
     
----------------------------------------------------------------------
     
Version 3.4 API 1.16: Feb 10, 2016

[J1214] Fixed a bug related to incorrect channel index validation for
    output channel routing (aac, aacplus, aacplus_v2 only).

----------------------------------------------------------------------

Version 3.3 API 1.16: June 10, 2015

[J937] Fixed a decoder crashing issue when it is fed with ADTS stream
     with inband PCE.

----------------------------------------------------------------------

Version 3.2 API 1.16: March 20, 2014

[1961] Improved AAC-LC cache performance by rearranging codec state structure.

----------------------------------------------------------------------

Version 3.1 API 1.16: February 7, 2014

Enhancements:
    Optimized CPU load for HiFi 2, HiFi 3 and HiFi Mini (for LC libraries).

[1833] Updated usage prints in the testbench.

[1835] Added multiple sub-frame support for LOAS/LAMT streams.  The related
    code is ifdef'ed by LOAS_SUBFRAME_SUPPORT and enabled by default. 

[1913] Unsupported frame features are now classified under
    XA_AACDEC_EXECUTE_NONFATAL_UNSUPPORTED_FEATURE.  In earlier versions,
    they were grouped under XA_AACDEC_EXECUTE_NONFATAL_RAW_FRAME_PARSE_ERROR.
    The decoder still mutes the frame for unsupported features.

[1914] Corrected error text in testbench for the error 
    XA_AACDEC_EXECUTE_NONFATAL_EMPTY_INPUT_BUFFER.
    
[1925] Added support for frame-by-frame decoding. This feature is controlled
    by a new API XA_AACDEC_CONFIG_PARAM_ENABLE_FRAME_BY_FRAME_DECODE.

Bug fixes:
[1803] Fixed decoder crashing issue in case of corrupted raw frame data.

[1806] Fixed decoder crashing issue when the API structure is not set to 0.

[1821] Fixed an issue in which the decoder does not allow the application
    to set the bitstream format as unknown.

[1834] Fixed an issue that negative bitrate is reported by the testbench.

[1849] Fixed an issue found in error concealment.

[1856] Fixed an issue found in format detection.
    
[1908] Fixed an issue in testbench that implicitly disabled small input
    chunk feature during init stage.

----------------------------------------------------------------------

Version 3.0 API 1.16: April 29, 2013

Enhancements:  
Revised error code reporting and improved API robustness.
    Section 3 of the Programmer's Guide is updated accordingly.

[1343] Added support for LATM format (aacmchplus_v2_loas only).

[1450] Added small input chunk support for 2-channel builds (aac, aacplus, aacplus_v2).

[1645] Added GET_CONFIG subcommand XA_AACDEC_CONFIG_PARAM_DATA_RATE support
    for all bitstream types.

[1657] Provided bit-offset API for RAW mode, removed alignment constraint
    and improved robustness in case of RAW decoding.

Bug fixes:
[1595] Fixed crashes caused by feeding multi-channel raw stream to 2-channel builds.

[1612] Fixed a compiler warning (comparing an int with an unsigned char).

[1613] Fixed the issue of decoder dropping more than one frame in case
    of header error in LOAS. (aacmchplus_v2_loas only)

[1622] Fixed the error code reporting issue with streams containing CCE.

[1652] Fixed an issue in which wrong stream position was reported,
    causing the system to lose sync.

[1666, 1724] Fixed some issues reported by certain static code analysis tool.
    Build system is updated to use cppcheck for detecting potential issues.

[1684] Fixed glitches in PCM output when DRC and PRL are enabled.

[1693] Fixed the issue of incorrect return value for consumed bytes when the
    decoder is fed a raw stream with incorrect sampling frequency.

[1698] Fixed an issue in test application for stream change detection.

----------------------------------------------------------------------

Version 2.18 API 1.16: July 19, 2012

[1440] Fixed a decoder initialization issue due to an erroneous internal
    state clearing in loas decoding.

[1449] Fixed the (one-frame long) delay when reporting the bytes consumed
    by the decoder. 

[1518] Modified the behavior of the decoder when the VLC length for the
    scale factor is out of range.  Previously this was considered as a
    frame decoding error and the frame was dropped.  Now only the current
    value is suppressed and the decoding continues.

[1532, 1561] The Programmer's Guide is updated to version 2.10 to fix
    some minor issues.

[1538] Fixed an issue in which the decoder enters into an infinite loop when
    it interprets junk/raw data as ADTS due to wrongly emulated ADTS header.

[1557] Fixed an issue in which, when the decoder was being initialized
    to handle  stream change, it was not reporting the error even if it
    encounters a bad frame header.

[1559] Fixed the number of output bytes reported, now it always reports
    zero output bytes when encountering a non-fatal error.

[1564] Code clean up: BSAC related code removed.

[1578] Added header based channel configuration support for raw decoder.
    The channel mapping is now decided by the channel configuration from
    loas/adif/adts header and also the channel mapper uses the instant
    tag information associated with syntactic elements.
    This feature is mandatory for MPEG4 AAC.

[1579] Fixed an issue in which repeated call to DO_EXECUTE without
    updating the input buffer led the decoder into a frozen state.
    This fix enhances the robustness of the decoder.

----------------------------------------------------------------------
Version 2.17 API 1.16: February 27, 2012

- Same as version 2.16, aacmchplus_v2_loas release only.

----------------------------------------------------------------------

Version 2.16 API 1.16: November 22, 2011

- Enhancement:  Added new APIs that return copyright and DRC related metadata.
    Also updated Programmer's Guide to reflect the latest memory and MHz numbers
    for multi-channel non-LOAS decoders.

----------------------------------------------------------------------

Version 2.15 API 1.16: May 10, 2011

- Enhancement:  Allow 2.1 channel (L, R, LFE) output support.

----------------------------------------------------------------------
Version 2.14 API 1.16: March 15, 2011

- Fixed "get current stream position" issue during initialization

----------------------------------------------------------------------
Version 2.13 API 1.15: January 25, 2011

- Enhancement: Allow input buffer to be filled with any number of bytes 
      instead of 2 full frames to decode. (The Programmer's Guide has not 
      been updated)

- Enhancement: improved on init sync-search & format-search  process. 
      Does not miss initialization while locking to the first loas frame 
         if part of the frame is located in the first
         input buffer and remaining part lies in the next available data in the stream.
      Does not allow LOAS frame detection during init, if ADIF or ADTS sync word 
         is present at the start of the input buffer.
      If false ADTS and LOAS syncwords are interspersed while feeding bit-streams for init, 
         the code detects the correct format properly. 

- Improved error concealment: remove clicks in erroneous AAC frames 

- Bug fix: Fixes to handle an erroneous stream. The stream has the first frameheader 
         (with stream config data) which is corrupt.  The decoder library keeps looking 
         for a next valid loas frame-header containing streamConfig info.

- Bug fix. Fixed reporting of errors whenever an erroneous stream has initial junk or 
         junk data is found between two good frames. 
         The header not found error shall be reported during init when sync-word of a 
         good frame is not found at an expected offset. The decoder library continues to 
         search for a next available good frame and decodes it, if the full frame data is 
         available in the input buffer. 

(The above enhancements, error concealment and recovery mechanisms are valid only for 
 the stream in ADTS and LOAS transport stream formats)


----------------------------------------------------------------------
Version 2.12 API 1.15: November 30, 2010
- Added ability to handle channel mode change in mid-stream. 
  (Note: this feature only applies to LOAS and ADTS formats)

- (LOAS build only) Fixed decoder failure when the first frame of the 
  LOAS input file has erroneous header. 

----------------------------------------------------------------------
Version 2.11 API 1.15: October 12, 2010
- Added Dynamic Range Control(DRC) feature as described in 
ISO/IEC 14496-3 document. (LOAS build only) 

- Added feature to parse DVB downmix data (in ETSI TS 101.154 document) 
and make it available to test-wrapper for aac-loas build. (LOAS build only)

----------------------------------------------------------------------
Version 2.9.2 API 1.15: September 17, 2010
- Fixed CRC checking: 
  1. Add CRC check in LEF channel. 
  2. CRC check now complies to the ISO/IEC and ARIB standards 

- Set output bytes to zero in case of a partial frame is given at end of
a stream.

----------------------------------------------------------------------
Version 2.9.1 API 1.15: June 16, 2010
Fixed 8 channel decode

----------------------------------------------------------------------
Version 2.9 API 1.15: June 1, 2010

Fixed bug : "External BS format Configuration Error" reported while
running valid elementary streams

----------------------------------------------------------------------
Version 2.8 API 1.15: May 10, 2010

Fixes bug in API when calling XA_AACDEC_CONFIG_PARAM_SBR_TYPE.  Now the
correct type is returned.

----------------------------------------------------------------------
Version 2.7 API 1.15: April 19, 2010

Add support to retrieve downmix metadata 
Add support to retrieve PCE status.

----------------------------------------------------------------------

Version 2.6 API 1.15: March 19, 2010

Add optional LOAS support.
Add "sync loss" error reporting.
Add error concealment for ADTS and LOAS formats.

Error concealment/Detection mechanisms are identical to the updated
reference DolbyPulse decoder code received from dolby as a part of 
"ms10ddt" package.

----------------------------------------------------------------------

Version 2.2 API 1.15: June 19, 2008

Robustness improvements for erroneous streams: prevent an infinite
loop when critical header parameters change; skip over invalid ADTS
headers during initialization.

Add a non-fatal error code (ADTS_HEADER_ERROR) to indicate that a
frame with a valid ADTS header specifying different critical stream
parameters (layer, profile, frequency index, channel configuration)
has been skipped. If the application expects a change in the critical
parameters, it can monitor for this error and, once it occurs a
certain number of times in a row, reinitialize the library.

----------------------------------------------------------------------

Version 2.1 API 1.15: May 14, 2008

Fix a bug in processing ADTS frames with payload size close to the
maximum causing the decoder to fail to initialize or drop the frames.

Improve robustness for erroneous ADTS streams.

Reduce persistent memory requirements of the parametric-stereo
(aacPlus v2) libraries by 2 KB for stereo-only builds and by up to 16
KB for multi-channel builds.

Add stream-position API commands to allow input/output data mapping.

Build the object code library with Xtensa Tools version 7.1.1.

Show the input stream position of the generated data in the progress
output.

----------------------------------------------------------------------

Version 2.0 API 1.14: December 19, 2007

Bring the implementation up to Coding Technologies' version 4.0.3
Fixed Point Firmware Reference code: support for multi-channel
streams; support for explicit SBR signaling; various bug fixes and
robustness improvements.

Reduce code size and data memory requirements; improve precision and
performance.

Add a runtime-init API command for smooth trick-play support. Modify
the sample testbench to demonstrate step-play using the new command.

Support 16- and 24-bit PCM output.

Show the current playback time in the progress output.

----------------------------------------------------------------------

Version 1.9 API 1.12: June 29, 2007

Reduce code size by eliminating unused routines.

Clean up the build process.

Keep literals as a separate section in the object code library.

Use a C sample testbench and clean up all compile time warnings from
it.

----------------------------------------------------------------------

Version 1.8 API 1.12: January 10, 2007

Incorporate changes that bring the implementation up to Coding
Technologies' version 3.2.3 Fixed Point Firmware Reference code:
better handling of faulty SBR headers prevents possible infinite loop.

Eliminate unnecessary calls to floating-point emulation library
routines.

Build the library with -mlongcalls.

----------------------------------------------------------------------

Version 1.7 API 1.12: December 11, 2006

Fix library naming.

----------------------------------------------------------------------

Version 1.6 API 1.12: October 11, 2006

Initial customer release.

----------------------------------------------------------------------
2022-10-10:04:15:RI-2022.9:769353:768985:769322::
