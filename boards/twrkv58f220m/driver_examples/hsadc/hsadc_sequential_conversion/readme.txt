Overview
========

The hsadc_sequential_conversion example shows how to use a long conversion sequence (can include as many as all the 16 sample slots) with HSADC driver.

In this example, the HSADC is configured as "kHSADC_DualConverterWorkAsTriggeredSequential" work mode. User should indicate 4 channel pairs to provide the differential voltage signal or 4 channels to provide the single ended voltage signal as the inputs for HSADC sample slots. If user select channel 6 and 7 of converter A or converter B as differential analog input pair for specific sample slot, the sub mux channel number of channel 6 and channel 7 should be configured to be same. If user select channel 6 or 7 of converter A or converter B as single ended analog input for specific sample slot, the sub mux channel number of channel 6 and channel 7 can be different. The sample slots would be assembled into a long conversion sequence.

In "kHSADC_DualConverterWorkAsTriggeredSequential" mode, all the sample slots are organized as a long conversion sequence. The converter A's control logic would be used to operate this long conversion sequence. It can start and stop the sequence.

When running the project, typing any key into debug console would trigger the conversion. And then, the conversion result data of the four sample slots (Sample 0, 1, 2, 3) would be displayed in the terminal. 


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

