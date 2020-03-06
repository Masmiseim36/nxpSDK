Overview
========
The dsp_hello_world demo application provides the simplest example of building a
DSP application using the Xtensa toolchain. This demo is designed to be run
through the Xtensa simulator for a specific NXP hardware target.

See the 'hello_world_usart' DSP demo for a version that is executed on hardware.


Toolchain supported
===================
- Xtensa Xplorer  7.0.9
- Xtensa C Compiler  12.0.8

Hardware requirements
=====================
- Personal Computer

Board settings
==============

Prepare the Demo
================
1. Build the application using the 'sim' target for simluator with Xtensa IDE or
xcc project.
2. Launch the application using Xtensa simulator from Xtena IDE or with 'xt-run
sim_debug/dsp_hello_world_hifi4.elf' on the command-line.

Running the demo DSP
====================
When the demo runs successfully, the terminal will display the following:

    Hello World running on core nxp_rt600_RI2019_newlib

Customization options
=====================

