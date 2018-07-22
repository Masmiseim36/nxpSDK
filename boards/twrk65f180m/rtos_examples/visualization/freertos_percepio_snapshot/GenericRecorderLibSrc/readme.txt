"Snapshot" Trace Recorder for Tracealyzer / FreeRTOS+Trace
----------------------------------------------------------
Version 3.0.9

Copyright (c) 2016, Percepio AB.
All rights reserved.

www.percepio.com

Tracealyzer is a visual PC tool for debugging, profiling and general analysis of RTOS-based embedded software at runtime and provides
25+ interconnected views of the execution of tasks, ISRs and kernel API calls. The FreeRTOS version is also known as FreeRTOS+Trace.
The tool is available in a free version and also in a more powerful commercial version. 

This folder contains the "snapshot" trace recorder library that collects trace data for Tracealyzer/FreeRTOS+Trace. This by hooking 
in to FreeRTOS and recording relevant events in a RAM buffer, that can optionally work as a ring-buffer. You can also store your own events
for logging and plotting of various events and data (see vTracePrintF in trcUser.h).

For integation and usage instructions, see http://percepio.com/docs/FreeRTOS/manual/Recorder.html#Trace_Recorder_Library_Snapshot_Recorder

Note: Continuous trace streaming is also supported. To use trace streaming, please use the latest version of the recorder library included
with the Tracealyzer / FreeRTOS+Trace application. As of version 3.0.9, streaming requires a different recorder library (the streaming recorder)
with slightly different API. The next release will provide a single recorder library with both "snapshot" and "streaming" modes.

In case you have any questions, do not hesitate to contact support@percepio.com

Files included
--------------
- trcConfig.h             		- The recorder's configuration file, set your recorder configuration here!
- trcUser.c/.h            		- The main API towards the application (trcUser.h in the only include necessary).
- trcKernel.c/.h          		- Internal routines for storing kernel events.
- trcBase.c/.h            		- Internal routines for manipulating the data structures and calculating timestamps.
- trcHardwarePort.c/.h    		- The hardware interface, especially for timestamping.
- trcKernelPort.h  			- Defines macros used by the kernel.
- trcKernelPortFreeRTOS.c  		- FreeRTOS-specific helper functions.
- trcKernelHooks.h        		- Generic trace macros (OS independent), used by trcKernelPort.h
- trcTypes.h              		- Type definitions used.

Percepio AB
Köpmangatan 1A
72215 Västerås
Sweden

www.percepio.com