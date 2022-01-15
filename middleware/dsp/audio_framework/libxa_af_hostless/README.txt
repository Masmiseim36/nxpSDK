#
# Copyright (c) 2015-2021 Cadence Design Systems, Inc.
# 
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
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

Version 2.6p2 API 2.0 : September 8, 2021

+ Patch Release
+ Built with RI.2 tools
+ Programmers Guide not updated

+ [J3023] Avoid scheduling of a component after a fatal error during 
  initialization or execution which can lead to a potential hang 
  during the component's next processing. An additional state is 
  introduced in the base-class to record such fatal error which is 
  checked to prevent any further scheduling of the component.

----------------------------------------------------------------------

Version 2.6p1 API 2.0 : June 16, 2021

+ Patch Release
+ Built with RI.2 tools
+ Programmers Guide not updated

+ [J2973] Reverted optimizations of xf_sync_enqueue/dequeue using 
  dummy locks for non-preemptive test cases as it causes atomic access
  issues. xf_sync_enqueue/dequeue are now initialized to use interrupt
  disable/restore type of locked access.

----------------------------------------------------------------------

Version 2.6 API 2.0 : March 31, 2021

+ GA Release
+ Built with RI.2 tools

+ [J2877] Fixed potential memory overwrite bug by returning UWORD32
  instead of UWORD64 during get-config for capturer and renderer
  component plugins.

+ [J2806] Fixed the rollback logic to avoid memory corruption when
  event channel creation fails.

+ [J2804] Fixed the rollback logic during port routing to prevent
  propagation of already freed buffers.

+ [J2799] Reduced maximum number of event buffers per channel from
  1024 to 16.

+ [J2746] Renderer plugin state changed to IDLE when execution is
  complete. 
   
----------------------------------------------------------------------

Version 2.5_Beta API 2.0 : January 08, 2021

+ Beta release 
+ Built with RI.2 tools
+ Programmers Guide not updated

+ [J2714] Schedule-tree bug fixes for deleting a node which is not
  present on the schedule-tree and for adding a node which is already
  present on the schedule-tree

+ [J2662] Compulsory flush message propagation when triggered due to
  natural EOS in execution, even if the port is IDLE.

+ [J2624] In Opus encoder plugin, corrected bytes consumed calculation
  for multi-channel inputs.

+ [J2617] Corrected the calling order of functions in all the classes,
  xf_output_port_flush_done and xf_output_port_unroute_done to enable
  correct state update (uses XOR operation to toggle states).

+ [J2611] Corrected duplicate bit-masks for two of the component 
  states in mimo-proc class (XA_OUT_TRACK_FLAG_FLUSHING_DONE and 
  XA_MIMO_FLAG_PROBE_SETUP)

+ [J2601] Added -mcoproc compiler option to all source file 
  compilation of library that are not associated with ISR.

+ [J2600] -Wextra warnings: Removed warnings other than 
  'unused variable' for library

+ [J2592] Added check for maximum alignment value in xf_mem_alloc() 

+ Robustness Fixes:
  - Core: 
  1. Introduced component lookup after dequeue of message from worker 
  queue, and reply with appropriate response if the lookup fails.
  When component lookup fails, prevent internally generated UNREGISTER 
  message by responding  with an internal response_failure message.
  2. Components are assigned the highest-worker-thread priority when 
  created until the set-config-parameter stage where the priority is
  reverted to that of either the background-thread or the configured 
  priority(XAF_COMP_CONFIG_PARAM_PRIORITY)
  3. Disabled code related to unused queue "cd->response".

  - IO:
  1. Added state check XF_INPUT_FLAG_PURGING for xf_input_port_ready
  and XF_OUTPUT_FLAG_FLUSHING for xf_output_port_ready.
  2. Disabled premature input-port state change from XF_INPUT_FLAG_EOS
  to XF_INPUT_FLAG_DONE in xf_input_port_put.

  - Base-class:
  1. Updated base-cancel mechanism based on the schedule-tree fixes.
  When preemption is enabled, base_cancel attempts to remove the node
  from schedule-tree, failing which the node is enqueued
  into base_cancel queue, which is dropped from processing when a 
  matching node is dequeued from the worker->queue.
  2. Corrected the variable in scratch size assertion/check to that 
  of the worker_thread_scratch_size in base_postinit.

  - Audio-codec class: 
  1. Flush: if there is a flush on input port, and the flush completes
  immediately, then update the base->state to COMPLETED if the output
  port is unrouted.
  2. Probe_pause: probe_stop should complete flushing the probe port 
  without checking if base->state is COMPLETED.
  3. Fill_this_buffer: check xf_output_port_ready before scheduling 
  as the port may be in unrouting or flushing state.

  - Mimo-proc class: 
  1. Terminate: Remove the assert of output port number check
  2. Unroute: on unroute, update output port state to unrouted and 
  indicate disconnect to plugin early.
  3. Fill_this_buffer: check if the port is unrouting before changing 
  the output port state to routed and indicate connect to plugin.
  4. Cleanup: avoid setting the state unroute_done which is redundant 
  at this point.
  5. Route: check if the ports are ready before scheduling
  6. unroute_done() to be called after the flag
  XA_OUT_TRACK_FLAG_FLUSHING_DONE is set, to make sure all flags are
  cleared after unroute completes.
  7. Fix for handling EMPTY_THIS_BUFFER when in terminate: avoid
  calculating output index of a port for any buffer arriving on
  input port.
  8. Output port flag XA_OUT_TRACK_FLAG_FLUSHING_DONE is cleared when
  output is produced.

  - All classes:
  1. Removed assert check for COMPLETED state in fill_this_buffer().
  2. Delayed response to UNREGISTER command from application until
  the component completes flushing and freeing of memory.
  3. Handling of lookup-failure messages to prevent self-destruction 
  of the component.

  - API: 
  1. Fixed a range check error in xaf_adev_set_priorities().

  - Plugins:
  1. Capturer: Avoid assert on NULL output buffer(output buffer is 
  optional and plugin needs to take appropriate action if NULL).

+ Event communication features:
  1. Event communication to application using event channels
  2. Fatal and non-fatal error reporting to application (as event)
  using event channel per component
  3. Event communication from one component to another
  4. Self-scheduling support for components from plugins

+ API changes:
  1. xaf_adev_open(pVOID *pp_adev, WORD32 audio_frmwk_buf_size, 
  WORD32 audio_comp_buf_size, xaf_mem_malloc_fxn_t mem_malloc, 
  xaf_mem_free_fxn_t mem_free)
  -> xaf_adev_open(pVOID *pp_adev, xaf_adev_config_t *pconfig)
  The old API is retained as xaf_adev_open_deprecated()
  New xaf_adev_open() supports configurable scratch memory size (per 
  worker thread), dsp-thread-priority and proxy-thread-priority, 
  callback function pointer for events
  Older xaf_adev_open API is available as xaf_adev_open_deprecated 

  2. xaf_comp_create(pVOID adev_ptr, pVOID *pp_comp, xf_id_t comp_id,
  UWORD32 ninbuf, UWORD32 noutbuf, pVOID pp_inbuf[], 
  xaf_comp_type comp_type)
  -> xaf_comp_create(pVOID p_adev, pVOID *pp_comp, 
  xaf_comp_config_t *pconfig)
  The old API is retained as xaf_comp_create_deprecated()
  New xaf_comp_create() supports configurable enabling of fatal-error
  reporting(as an event)
  Older xaf_comp_create API is available as xaf_comp_create_deprecated

  3. Added new APIs for event communication:
     xaf_create_event_channel
     xaf_delete_event_channel

  4. Added new APIs to initialize default configuration parameters 
     xaf_adev_config_default_init
     xaf_comp_config_default_init

+ App Interface Layer changes:
  1. Added a thread-safe mechanism for component chain access
  2. Optimized comp_state to only necessary states RESET and CREATE
  3. Delayed the component state update in comp_create until component 
  is created

+ DSP Interface Layer changes:
  1. Added BUG asserts for possible hang-case detection in
  xf_mm_alloc/xf_mm_free, enabled with DEBUG=1 build option.

+ Plugin Changes:
  1. Added XA_<component>_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES 
  configuration parameter for consistency of frame_size setting 
  in samples-per-channel.
  Components updated: PCM_GAIN, MIXER, OPUS_ENC, CAPTURER, RENDERER
  The older counterparts XA_<component>_CONFIG_PARAM_FRAME_SIZE,
  will be removed in future.
  2. Updated plugins xa-pcm-gain.c, xa-mimo-mix.c to demonstrate
  event communication between components.

+ Testbench Changes
  1. Testbench changes to demonstrate event communication between
  components: xaf-mimo-mix-test.c
  2. Testbench changes to demonstrate error reporting to application:
  xaf-playback-usecase-test.c
  3. Corrected  help text in gain-renderer testbench.

+ MCPS reduction changes:
  1. A buffer can be submitted by a component to the destination 
  worker-queue by directly calling dispatch, thus avoiding enqueue 
  and dequeue of DSP-thread queue.
  2. base_cancel to check the presence of schedule-node in worker 
  queue to avoid spurious scheduling.
  3. Sync msgq access with disable/restore of interrupts instead of
  RTOS' mutex locks.
  4. Added port readiness check for input port in mimo class
  fill_this_buffer() to avoid excess scheduling.
  5. Added a generic lock wrapper as xf-flx-lock-if.h (flexi-lock)
  The locks are initialized as dummy by default. With pre-emption
  enabled, when worker threads are created, the scheduler, shmem and
  component-memory locks are reinitialized to RTOS-mutex-locks and
  xf_sync_enqueue/dequeue locks are reinitialized to interrupt
  disable/restore type of access.

+ Code size reduction changes:
  Following optional (not used by default) compile time switches are 
  provided:
  1. XA_DISABLE_EVENT: To disable event-communication related code 
  2. XA_DISABLE_DEPRECATED_API:  To disable deprecated API code

----------------------------------------------------------------------
Version 2.3 API 1.3 : July 24, 2020

+ GA release 
+ Built with RI.2 tools

+ [J2593] Added support for following two config params in Opus
  encoder plugin:
    - XA_OPUS_ENC_CONFIG_PARAM_SIGNAL_TYPE
    - XA_OPUS_ENC_CONFIG_PARAM_RESET_STATE

+ [J2592] Fixed XAF internal memory allocator to use alignment 
  parameter appropriately

+ [J2586, J2587] Fixed argument to xos_start_system_timer() API
  to select a timer with highest interrupt-priority <= EXCMLEVEL

+ [J2576, J2577] Fixed memory leaks by deleting the timer object in 
  capturer and renderer plugins

+ [J2567] Updated audio-codec-class to reset base state when an 
  output buffer is received after execution completion

+ Renamed pcm_mix component plugin to mimo_mix

----------------------------------------------------------------------
Version 2.2_Beta API 1.3 : May 21, 2020

+ Beta release 
+ Built with RI.2 tools
+ Programmers Guide not updated

+ [J2220] Hostless XAF is tested and supported on Fusion cores

+ [J2226, J2464] Unified Hostless XAF release packages across cores

+ [J2543] Fixed a bug by clearing output_setup flag on relaxed output 
  ports during postprocess in MIMO class

+ [J2544] Added reset EOS stream flags in Opus Encoder, PCM Gain, 
  SRC plugin files to enable re-run upon receiving new input

+ [J2545] PCM Gain plugin updated to support 8, 24, 32 bit PCM width
  and multichannel input data

+ [J2546] Fixed a bug in scratch allocation with pre-emptive 
  scheduling - allocated independent scratch for each worker thread

+ [J2553] Fixed a bug in MIMO class postprocess to avoid re-schedule
  if execution is complete

+ [J2561] Updated XF_TIMEBASE_FREQ to support 48kHz, 16ch, 32bit PCM
  configuration

+ [J2563] Fixed a memory leak bug by freeing all mutex locks in 
  xaf_adev_close API

+ [J2456] Updated runtime commands logic in testbench to allow other
  runtime commands to execute while disconnect API is in progress

----------------------------------------------------------------------
Version 2.1_Alpha API 1.3 : April 23, 2020

+ Alpha release - HiFi4 only
+ Built with RI.2 tools

+ Changed some non-standard return codes from XAF Developer APIs to
  standards XAF Error codes

+ Updated xaf_comp_get_status API to return EXEC_DONE status for
  components returning output or probe data to application

+ [2453] Enhanced mem_malloc and mem_free wrappers to avoid maximum
  number of allocations allowed constraint

+ [2472] Minimum connect buffers between components requirement is 
  reduced from 2 to 1. Note with this change, MIMO class preprocess
  allows execution if any of input port has non-zero data (earlier,
  it would check for non-zero data on all input ports) 

+ [2473] Moved TRACE control tags to xf-debug.h for better 
  configurability

+ [2475] Fixed bug in renderer disconnect API implementation

+ [2486] Increased auxiliary buffer size from 128 to 256 to allow
  32 configuration parameters in xaf_comp_set_config and 
  xaf_comp_get_config API (shared memory usage increases by 4kB)
 
+ [2491] Updated xaf_get_mem_stats API implementation to report
  peak and current memory usage. Note with this change, the API
  expects pointer to array of five integers as second argument 
  (earlier it expected pointer to array of three integers)

+ [2498] Fixed bug of redundant stack allocation during thread
  creation with FreeRTOS

+ [2513] Reduced input buffer size requirement of Opus encoder
  plugin to hold only one frame

+ [2528] Removed fatal error from capturer class in case of output 
  buffer non-availability, this should be handled by plugin

+ [2530] Updated copyright headers to MIT style for all files

+ Programmers Guide not updated

----------------------------------------------------------------------
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
2021-09-08:11:27:RI-2019.2:695074:687195:660711::
