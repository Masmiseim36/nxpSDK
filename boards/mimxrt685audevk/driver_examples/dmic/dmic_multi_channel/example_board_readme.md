Hardware requirements
=====================
- Micro USB cable
- MIMXRT685-AUD-EVK boards
- Personal Computer
- 8CH-DMIC board
- Headphone

Board settings
============
1. Connect 8-DMIC board to J31.
2. JP44 2-3, JP45 2-3
3. R389 1-2, R390 1-2, R391 1-2, R392 1-2

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert 8dmic board to J31
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
Note: As the 8-DMIC board support 8 dmics, so the demo provide several macros to control the specific DMIC's functionality,
#define DEMO_ENABLE_DMIC_0 1
#define DEMO_ENABLE_DMIC_1 1
#define DEMO_ENABLE_DMIC_2 1
#define DEMO_ENABLE_DMIC_3 1

#define DEMO_ENABLE_DMIC_4 1
#define DEMO_ENABLE_DMIC_5 1
#define DEMO_ENABLE_DMIC_6 1
#define DEMO_ENABLE_DMIC_7 1
All the DMIC enabled by default, you can modify the macro to test one DMIC or several DMICS as you like. But please enable at least one DMIC for the demo, otherwise there will be a compile error generate.

1.  Launch the debugger in your IDE to begin running the demo.

The following lines are printed to the serial terminal when the demo program is executed.

DMIC multi channel example.



Initializing codec



Start play audio data


2. During the DMIC start receive data, you can speak or play song nearby the dmic, then you can hear the multichannel audio data from the J4, J50, J51, J52.
