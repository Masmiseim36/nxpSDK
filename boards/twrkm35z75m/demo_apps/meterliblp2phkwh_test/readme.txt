Overview
========
The Kinetis-M metering library is used in Kinetis-M series chips and mainly used in metering applications for accurate
computation of the active energy, reactive energy, active power, reactive power, apparent power, RMS voltage, and RMS
current.
Normally this metering library is used for three-phase four-wire, two-phase two-wire or single-phase one-wire applications.
This application will guide to use the library in non-standard single-phase multi-channel metering applications. Furthermore,
the implementation described in this document can also be extended to other single-phase non-standard meters.
The Kinetis-M microcontrollers, e.g. KM34Z128 and KM35Z75M, can offer a high-performance analog front-end (24-bit AFE)
combined with an embedded Programmable Gain Amplifier(PGA). Besides high-performance analog peripherals like
auxiliary 16-bit SAR ADC, these devices integrate memory, input-output ports, digital blocks, and a variety of communication
options.
Moreover, on KM35Z75M, the ARM Cortex-M0+ core, with MMAU (Memory Mapped Arithmetic Unit) support for 32-bit and
64-bit math, enables fast execution of metering algorithms.
There are two metering libraries provided with Kinetis-M chips, Filter-Based library and FFT library. This application will use
Filter-Based library.
The Filter-Based Metering Algorithm can be easily integrated into an electronic power meter application. The algorithm
requires only instantaneous voltage and current samples to be provided at constant sampling intervals. These instantaneous
voltage and current samples are usually measured by an AFE with the help of a resistor divider, in the case of a phase voltage
measurement, and a shunt resistor, current transformer or a Rogowski coil in the case of a phase current measurement. All
current measurement sensors introduce a phase shift into current measurement. Therefore, it is necessary to align the phases
of the instantaneous voltage and current samples prior to their use by the Filter-Based Metering Algorithm using either a
digital filter or with the aid of delayed sampling.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso 11.1.0

Hardware requirements
=====================
- Micro USB cable
- TWR-KM35Z75M board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Download the program to the target board.
3.  Continue with help of the AN4255.pdf, AN4265.pdf, AN4847.pdf for more details about the  metering  library usage.

Filter-Based Metering Algorithms Configuration Tool:
================
Filter-Based Metering Algorithms Configuration Tool enables to calculates the metering parameters and generates metering configuration. The tool can be downloaded from: https://cache.nxp.com/secured/assets/documents/en/application-note-software/AN4265SW.zip?__gda__=1624456565_67a51c25a902c782827fad361d35eb20&fileExt=.zip 

Example list:
================
meterlib1ph_test.c 
This example demonstrates use of the metering library in typical one-phase power meter application.

meterlib1phkwh_test.c 
This example demonstrates  use  of  the  metering  library  in  active  energy  one-phase  power meter application.

meterlib1phrc_test.c 
This example demonstrates use of the metering library in typical one-phase power meter application with Rogowski coil sensor.

meterlib2ph_test.c 
This example demonstrates use of the metering library in typical two-phase power meter application.

meterlib2phkwh_test.c 
This example demonstrates  use  of  the  metering  library  in  active  energy  two-phase  power  meter application.

meterlib2phrc_test.c 
This example demonstrates use of the metering library in typical two-phase power meter application with Rogowski coil sensor.

meterlib3ph_test.c 
This example demonstrates use of the metering library in typical polyphase power meter application.

meterlib3phkwh_test.c 
This  example demonstrates  use  of  the  metering  library  in  active  energy  polyphase  power  meter application.

meterlib3phrc_test.c 
This example demonstrates use of the metering library in typical polyphase power meter application with Rogowski coil sensor.

meterlibFFT1ph_test.c
This  example demonstrates  use  of  the  FFT  metering  library  in  the  typical  single-phase  power  meter application
with  proportional  current sensor and  with  the  S/W  phase  shift  correction.  AFE  conversion mode (single/continuous) may
be selected by the 'AFE_MODE' constant. Number of the FFT samples is set to 32, number of output harmonics is 16 then. 

meterlibFFT2ph_test.c
This  example demonstrates  use  of  the  FFT  metering  library  in  the  typical  two-phase  power  meter application with
derivative current sensor without the S/W phase shift correction. AFE conversion mode (single/continuous) may be selected by
the 'AFE_MODE' constant. Number of the FFT samples is set to 64, number of output harmonics is 32 then. 

meterlibFFT3ph_test.c
This  example demonstrates  use  of  the  FFT  metering  library  in  the  typical  three-phase  power  meter application with
proportional  current  sensor  and  with  the  S/W  phase  shift  correction.  AFE  conversion mode (single/continuous) may be
selected by the 'AFE_MODE' constant. Number of the FFT samples is set to 64, number of output harmonics is 32 then. 
This example allows to measure the sense of rotation of the 3-ph mains. 

meterliblp1ph_test.c 
This  example  demonstrates  use  of  the  low  power  metering  library  in  typical  one-phase  power  meter application.

meterliblp1phkwh_test.c 
This example demonstrates use of the low power metering library in active energy one-phase power meter application.

meterliblp2ph_test.c 
This example demonstrates use of the low power metering library in typical two-phase power meter application.

meterliblp2phkwh_test.c 
This example demonstrates use of the low power metering library in active energy two-phase power meter application.

meterliblp3ph_test.c 
This example demonstrates use of the low power metering library in typical polyphase power meter application.

meterliblp3phkwh_test.c 
This example demonstrates use of the low power metering library in active energy polyphase power meter application. 
