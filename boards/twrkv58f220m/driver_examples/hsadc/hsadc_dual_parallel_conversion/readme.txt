Overview
========

The hsadc_dual_parallel_conversion example shows how to use the both converter parallel at the same time with HSADC driver.

In this example, the HSADC is configured as "kHSADC_DualConverterWorkAsTriggeredParallel" (default) work mode. User should indicate 4 channel pairs to provide the differential voltage signal or 4 channels to provide the single ended voltage signal as the inputs for HSADC sample slots. If user select channel 6 and 7 of converter A or converter B to sample for specific sample slot, the sub mux channel number of channel 6 and channel 7 also should be configured. The sample slots would be assembled into the two conversion sequence for each converter. 

In "kHSADC_DualConverterWorkAsTriggeredParallel" mode, the converter A converts sample 0 and sample 1 (sample 0-7 are available) and the converter B converts sample 8 and sample 9 (sample 8-15 are available). As the "simultaneous mode" is enabled by default, the converter A's control logic can operate both the two converters to start and stop together.

When running the project, typing any key into debug console would trigger the conversion. And then, the conversion result data of the four sample slots (Sample 0, 1, 8, 9) would be displayed in the terminal. 


Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KV58F220M board
- Personal Computer

Board settings
==============
This example samples following board pins' voltage signal:
- HSADC0A_CH2 and HSADC0A_CH3 in differential mode
- HSADC0A_CH6D in single ended mode
- HSADC0A_CH6C(PTE2) in signal ended mode
- HSADC0A_CH7F(PTE1) in single ended mode

HSADC0A_CH6D is connected to potentiometer on the board. So this pin's voltage can be adjusted conveniently. 
Other pins above need to be connected to external dynamical voltage generator or different voltage signal on the board
to oberseve the dynamical conversion result.  

Prepare the Demo
================
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, following information can be seen on the OpenSDA terminal:
Press any key to trigger the conversion ...
Sample 0    Sample 1    Sample 8    Sample 9
Result 0    Result 1    Result 2    Result 3
Customization options
=====================

