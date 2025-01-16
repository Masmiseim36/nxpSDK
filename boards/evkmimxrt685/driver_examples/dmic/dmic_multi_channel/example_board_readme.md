Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT685 boards
- Personal Computer
- 8-DMIC board
- CS42448 audio board
- Headphone

Board settings
============
EVKMIMXRT685:
1. The board enables octal flash (U19) by default. To enable external DMIC (J31), move the following resisters to 2-3 (1-2 by default)
 - R379,R380,R384,R389,R390,R391,R392.
2. Make sure R116 & R117 is weld.

CS42448 Audio board:
Install J5 2-3, J3 2-3, J4 2-3, J2, J13, J14.

Connect 8-DMIC board to J31.

To make example work, connections needed to be as follows:
CS42448 audio board             rt600 evk REVE board
J1-2                      ->          J29-pin7(GND)
J1-23                     ->          J27 pin3(BCLK)
J1-16                     ->          J27 pin2(WS)
J1-17                     ->          J27 pin1(TX)
J1-4                      ->          J28 pin8(3V3)
J1-21                     ->          J28 pin1(power enable)
J1-10                     ->          J29 pin8(5v)
J1-13                     ->          J28 pin10(scl)
J1-15                     ->          J28 pin9(sda)
J1-1                      ->          J27 pin6(MCLK)


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
4.  Connect CS42448 audio board to RT600 EVK per above description.
5.  Connect headphone to the audio board.
6.  Download the program to the target board.
7.  Launch the debugger in your IDE to begin running the demo.

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

Init codec

Start play audio data


2. During the DMIC start receive data, you can speak or play song nearby the dmic, then you can hear the multichannel audio data from the CS42448 audio board Line output.
