Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT700-EVK board
- Personal computer

Board settings
==============
1. Example running at 325MHz which need PMIC power supply.
2. PMIC is required to drive 1.1V on VDD2, then JP1 and JP3 on the RT700 EVK need to be shorted for proper operation

Prepare the Demo
================
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Run the Demo
================
The log below shows the output of the demo in the terminal window (compiled with ARM GCC):

Model PTE file loaded. Size: 99168 bytes.
Model buffer loaded, has 1 methods
Running method forward
Setting up planned buffer 0, size 53760.
Method loaded.
Preparing inputs...
Input prepared.
Starting the model execution...
Model executed successfully.
----------------------------------------
     Inference time: 11422 us
----------------------------------------
1 outputs:
Output[0][0]: 0.027344
Output[0][1]: 0.019531
Output[0][2]: 0.054688
Output[0][3]: 0.355469
Output[0][4]: 0.054688
Output[0][5]: 0.242188
Output[0][6]: 0.078125
Output[0][7]: 0.054688
Output[0][8]: 0.039063
Output[0][9]: 0.078125
Program complete, exiting.
