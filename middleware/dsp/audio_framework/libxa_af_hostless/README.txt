#
# Copyright (c) 2015-2020 Cadence Design Systems, Inc.
# 
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to use this Software with Cadence processor cores only and 
# not with any other processors and platforms, subject to
# the following conditions:
# 
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

======================================================================
Xtensa Audio Framework (XAF) - Hostless solution
======================================================================

======================================================================
Revision History
======================================================================

Version 2.0 API 1.3 : January 31, 2020

+ GA Release - HiFi3, HiFi4 and HiFi5 only
+ Built with RI.2 tools

----------------------------------------------------------------------
Version 1.8_3_Alpha API 1.3 : December 23, 2019

+ Patch release - HiFi4 only
+ Built with RI.2 tools

+ Added MIMO (Multi-Input, Multi-Output) data processing class.

+ Added following new XAF Developer APIs:
    XAF_ERR_CODE xaf_pause(pVOID p_comp, WORD32 port);
    XAF_ERR_CODE xaf_resume(pVOID p_comp, WORD32 port);
    XAF_ERR_CODE xaf_probe_start(pVOID p_comp);
    XAF_ERR_CODE xaf_probe_stop(pVOID p_comp);
    XAF_ERR_CODE xaf_disconnect(pVOID p_src, WORD32 src_out_port, 
                 pVOID p_dest, WORD32 dest_in_port);

+ Updated prototype for xaf_connect API:
    XAF_ERR_CODE xaf_connect(pVOID p_src, WORD32 src_out_port, 
                 pVOID p_dest, WORD32 dest_in_port, WORD32 num_buf);

+ Added support for FreeRTOS.

+ Added support for preemptive scheduling for both XOS and FreeRTOS.

+ Following updates are done for Renderer Class:
- Changed logic to schedule execution of renderer on each interrupt 
  irrespective of input port readiness
- Added XAF_START_FLAG command support for explicit initialization of 
  renderer
- Added support for additional, optional output port to generate 
  reference or feedback output

+ Following updates are done for Audio Codec Class:
- Updated Audio Codec Class to initialize all components except 
  decoders (comp_type XAF_DECODER) without input data

+ Following updates are done for Renderer Plugin:
- Added actual FIFO in the plugin to mimic hardware operation
- Added state configuration parameter (XA_RENDERER_STATE_START) to 
  start renderer hardware operation (e.g. enable interrupt)
  Note, even if this configuration parameter is not used from 
  application, renderer hardware operation starts once FIFO is full
- Added logic to zero-fill output if real data is not available
  At start, this logic would insert zero frames or prepend zeros to 
  real data, if required
  At runtime, this logic would insert zero frames or append zeros to 
  real data, if required
- Added support for additional, optional output port to generate 
  reference or feedback data
- Moved renderer execution from set_input_bytes() to do_execute() 
  function
- Added logic to set exec-done from plugin when input is over

+ Following updates are done for Capturer Plugin:
- Added state configuration parameter (XA_CAPTURER_STATE_START) to 
  start capturer hardware operation (e.g. enable interrupt)
  Note, this configuration parameter must be issued from application 
  for capturer hardware operation to start
- Moved capturer execution from set_mem_ptr() to do_execute() function

+ Updated initialization API call sequence requirement as - 
  initialization status must be queried by xaf_comp_get_status() API
  after xaf_comp_process(...XAF_START_FLAG) API call. 

+ Corrected Capturer output port number to 0 from 1.

+ Added few robustness fixes as per internal QA testing.

+ Added new example testbenches and plugins to demonstrate usage of 
  new features.

+ Programmers Guide not updated.

+ Notes
 - New feature implementation mentioned above has undergone limited 
   testing and may get updated.
 - By default, this release uses XOS as RTOS.
 - To use FreeRTOS instead of XOS, please refer to instructions in 
   'libxa_af_hostless/build/readme-freertos-bld.txt'.
 - XOS is configured with 100 MHz clock frequency with 0.1 ms timer 
   tick resolution whereas FreeRTOS is configured with 50 MHz clock
   frequency with 1 ms timer tick resolution.
 - Known constraint: MCPS computation and prints are not supported
   with FreeRTOS in this release.

----------------------------------------------------------------------
Version 1.4 API 1.1 : March 11, 2019

+ GA release.

----------------------------------------------------------------------
Version 1.3p4_Alpha API 1.1 : August 30, 2018

+ Patch release - HiFi3 and HiFi4 only
+ Built with RG.5 tools

+ [J2117] Fixed an issue in xaf_adev_close function (ensured internal 
  proxy thread is closed properly).
+ [J2118] Sample Rate Convertor plugin wrapper is updated to work 
  with Sample Rate Convertor v1.9 Library.

+ Updated Programmers Guide to indicate 
  - XAF must not be used with fast functional 'TurboSim' mode of 
    Instruction Set Simulator.
  - Sample Rate Convertor plugin wrapper is updated to work with 
    Sample Rate Convertor v1.9 Library.

----------------------------------------------------------------------
Version 1.3p3_Alpha API 1.1 : March 12, 2018

+ Patch release 
+ Built with RG.5 tools

+ [J2004] Fixed an issue in compilation of ISR code (removed -mcoproc
  compile flag for ISR code). 

----------------------------------------------------------------------
Version 1.3p2_Alpha API 1.1 : February 2, 2018

+ Patch release - HiFi4 only
+ Built with RG.5 tools

+ Fixed issues in packaging of release packages.

----------------------------------------------------------------------
Version 1.3p1_Alpha API 1.1 : February 1, 2018

+ Patch release - HiFi3, HiFi4 only
+ Built with RG.5 tools

+ [J1965] Avoided use of cache line size (core specific) while 
  calculating size of cache line aligned DSP Thread Handle struct.  

----------------------------------------------------------------------
Version 1.3_Alpha API 1.1 : December 14, 2017

+ Initial (Alpha) customer release. 

----------------------------------------------------------------------
Version 1.2 API 1.1 : November 21, 2016

+ Known issues in Release 1.1 fixed.
+ Added support for renderer and capturer components.
+ Added AAC-Dec, Renderer, Capturer, SRC components along with
   corresponding test benches.
+ Testbenches updated for Realtek ALC5677 EVM.

----------------------------------------------------------------------
Version 1.1 API 1.1 : June 2, 2016

+ Known issues in Release 1.0 fixed.
+ Added testbenches for mixer, audio encoder and speech decoder.
+ Improved robustness and minor changes in API.

----------------------------------------------------------------------
Version 1.0 API 1.0 : March 04, 2016

+ Initial release.

----------------------------------------------------------------------
Version 0.7 API 1.0 : December 17, 2015

+ Beta release.

----------------------------------------------------------------------
2020-01-30:10:50:RI-2019.2:575174:566169:575218::
