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
Cadence HiFi DRM Decoder
======================================================================

======================================================================
Revision History
======================================================================

Version 2.14 API 1.16: July 7, 2020

+ GA release
+ Updated copyright notice for header files.

----------------------------------------------------------------------

Version 2.13p4 API 1.16: October 18, 2019

+ Patch release, HiFi2 only, built with RI.2 tools.

+ [J2349] Rebuilt previous version with LX4 HW HiFi2 core, RI.2 tools.

+ Known issue: Scratch memory reuse outside decoder may affect decoder
  behavior in some corner cases.

----------------------------------------------------------------------

Version 2.13p3 API 1.16: June 28, 2019

+ Patch release, built with RF.2 tools.

+ [J2242] Improved robustness fix provided in last (2.13p2) release.

+ Known issue: Scratch memory reuse outside decoder may affect decoder
  behavior in some corner cases.

----------------------------------------------------------------------

Version 2.13p2 API 1.16: May 21, 2019

+ Patch release, built with RF.2 tools.

+ [J2242] Robustness improvement.

+ Known issue: Scratch memory reuse outside decoder may affect decoder
  behavior in some corner cases.

----------------------------------------------------------------------

Version 2.13p1 API 1.16: August 23, 2017

+ [J1797] Robustness improvement.

+ Known issue: Scratch memory reuse outside decoder may affect decoder
  behavior in some corner cases.

----------------------------------------------------------------------
Version 2.13 API 1.16: August 16, 2017

+ GA release, same as version 2.12, built with RG.5 tools.

----------------------------------------------------------------------
Version 2.12 API 1.16: August 15, 2017

+ GA release for version 2.11_Beta, built with RF.2 tools.

----------------------------------------------------------------------
Version 2.11_Beta API 1.16: August 8, 2017

+ [J1773] [J1776] Robustness improvement.

+ [J1779] Fixed a bug in Arithmetic Decoder optimization.  

+ [J1786] Correction in Testbench help text.

+ Programmers Guide in DRAFT format, performance numbers are not updated.

+ Known issue: Scratch memory reuse outside decoder may affect decoder
  behavior in some corner cases.

----------------------------------------------------------------------
Version 2.10_Beta API 1.16: July 19, 2017

+ Fixed an issue with inconsistent error and conceal state reporting 
  for ErrorsAtTheInit.bin stream.

+ Programmers Guide in DRAFT format, performance numbers are not updated.

+ Known issue: Scratch memory reuse outside decoder may affect decoder
  behavior in some corner cases.

----------------------------------------------------------------------
Version 2.9_Beta API 1.16: July 11, 2017

+ Upgraded source code to CDK DRM Audio Decoder Library Interface
  Description, Revision 2.1.18, July 6, 2017 from Fraunhofer IIS.
  - Note, this upgrade fixed issues with inconsistent error and 
    conceal state reporting.

+ Programmers Guide in DRAFT format, performance numbers are not updated.

+ Known issue: Scratch memory reuse outside decoder may affect decoder
  behavior in some corner cases.

----------------------------------------------------------------------
Version 2.8.2 API 1.16: April 17, 2017

+ Beta-3 Release

+ Upgraded source code to CDK DRM Audio Decoder Library Interface
  Description, Revision 2.1.16, March 10, 2017 from Fraunhofer IIS
  - Note, with this upgrade lastFrameOk in case of AAC is delayed to be 
    aligned with decoded output signal(compensation for interpolation 
    concealment delay). For the first frame, lastFrameOk is initialized 
    with frameOk=1. 

+ Added new error code XA_DRM_DEC_EXECUTE_NONFATAL_NEED_DATA_SYNC_ERROR
  to convey the application about the xHEAAC sync status.

+ Programmers Guide in DRAFT format, performance numbers are not updated.

+ Known issue: Scratch memory reuse outside decoder may affect decoder
  behavior in some corner cases.

----------------------------------------------------------------------
Version 2.8.1 API 1.16: February 9, 2017

+ Beta-2 Release

+ Upgraded source code to CDK DRM Audio Decoder Library Interface
  Description, Revision 2.1.14, January 25, 2017 from Fraunhofer IIS

+ Added XA_DRM_DEC_CONFIG_PARAM_XHEAAC_HOLDOFF_MODE configurable
  parameter to control outputting silence frames from the decoder
  during xHE-AAC synchronization at decoder initialization.

+ Programmers Guide in DRAFT format, performance numbers are not updated.

+ Known issue: Scratch memory reuse outside decoder may affect decoder
  behavior in some corner cases.

----------------------------------------------------------------------
Version 2.8 API 1.16: December 8, 2016

+ Beta Release

+ [J1392] Added following configurable concealment parameters -
    XA_DRM_DEC_CONFIG_PARAM_CONCEAL_FADEIN
    XA_DRM_DEC_CONFIG_PARAM_CONCEAL_FADEOUT
    XA_DRM_DEC_CONFIG_PARAM_CONCEAL_MUTE_RELEASE
    XA_DRM_DEC_CONFIG_PARAM_CONCEAL_COMFORT_NOISE 

+ Upgraded source code to CDK DRM Audio Decoder Library Interface 
  Description, Revision 2.1.13, December 2, 2016 from Fraunhofer IIS

----------------------------------------------------------------------
Version 2.7 API 1.16: April 11, 2016

+ [J1213] Added GET_CONFIG API XA_DRM_DEC_CONFIG_PARAM_CONCEAL_STATE
  for decoder internal concealment states.  Fixed return code where
  the decoder reported DRMDEC_OK instead of DRMDEC_NEED_DATA.
  Improved concealment behavior for xHE-AAC by implementing the logic
  to detect missing audio frames.  Now the  decoder can automatically
  fill in concealed frames to compensate for  the lost output time.
  Solved an issue in which the conceal state is incorrectly reported
  when the stream starts with erroneous frames.

+ Concealment fix regarding AAC coding + SBR.

+ Conformance fix regarding xHE-AAC coding: MPS212 with residuals + 
  parameter smoothing.

----------------------------------------------------------------------
Version 2.6 API 1.16: March 3, 2015

+ [J972] Added missing comment in API header file to indicate an unused
  error code.

+ [J974] Upgraded to Fraunhofer reference code dated February 25, 2015.

+ [J975] Decoder now reports error to the application when a frame is
  concealed due to superframe header parse errors.

----------------------------------------------------------------------
Version 2.5 API 1.16: January 16, 2015

+ [J949] Decoupled AAC core concealment from SBR parse error to improve
  perceived concealment performance for cases where only the SBR payload
  is corrupted

----------------------------------------------------------------------
Version 2.4 API 1.16: December 19, 2014

+ [J949] Fixed an issue that causes noisy decoder output due to streams
  with erroneous SBR header.

----------------------------------------------------------------------
Version 2.3 API 1.16: October 31, 2014

+ General release.
+ [2101, 2102]  Improved codec robustness.
+ Minor code clean-up.

----------------------------------------------------------------------
Version 2.2 API 1.16: Aug 13, 2014

+ Beta Release.
+ [2085] Fixed an issue in which the decoder generates silence output
     even for valid DRM streams.
+ [2032, 2096] The decoder can now handle configuration changes without
    requiring re-initialization.
+ [2098] Improved the concealment (no more artifacts) with re-sync.
    Fade-in and fade-out also works better now for xHE-AAC streams.
+ The Decoder is updated with the following changes from the reference:
    - Switch hold-off calculations now use net bits instead of gross bits
      per super frame to be in line with Fraunhofer's Encoder. This will
      reduce the hold-off time in some scenarios.
    - The decoder handles some error streams (generated by older encoder
      with internal bug in setting short_fac_flag) gracefully.
    - The headroom for energy calculation is increased to avoid overflow.
      The output is no longer bit exact with earlier versions.

----------------------------------------------------------------------
Version 2.1 API 1.16: July 2, 2014

+ Beta Release.
+ Optimized for CPU load, especially for HiFi2.
+ Decoder can now handle dynamic configuration change (stream change
  detection logic in place).
+ Improved API robustness.
+ API XA_DRM_DEC_CONFIG_PARAM_FRAME_CORRUPT is removed.
  See PG for handling super frame loss.
+ DRM decoder will only accept streams with bitrates allowed in the DRM
  standard (up to 71.96kbps for AAC(DRM30), up to 186.32kbps for AAC(DRM+)
  and up to 163.92kbps for xHE-AAC).
+ Raw SDC configuration length can be zero.  Now the application need not
  create the default configuration if SDC type9 data is not available.  
+ The sample application now also demonstrates feeding input super frame only
  when the application requires more data (pull model).
+ Programmer's Guide updated. 

Known Issues:
+ The re-sync mechanism (triggered after detecting a super frame error or
  configuration error) in xHE-AAC may create some artifacts.
+ A few error streams still report fatal errors and require the decoder to be
  initialized again.  This may cause breaks in audio output by the application.
+ Mismatch between the configuration received from the application and the raw
  SDC may trigger the concealment without reporting any error to the application.

----------------------------------------------------------------------
Version 2.0 API 1.16: April 16, 2014

+ Alpha Release.
+ Updated to DRM v4.1.1 (ETSI ES 201 980).

Known Issues in this release:
+ Configuration change is not working correctly.
+ The API for superframe corruption is not working correctly,
  please clear superframe with memset for now.
+ Error detection and concealment mechanism not fully tested.
+ Not fully optimized.
----------------------------------------------------------------------
2022-09-20:06:25:RI-2019.2:604707:604569:422110::
