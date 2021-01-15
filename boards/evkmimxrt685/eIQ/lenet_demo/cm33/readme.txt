Overview
========

The lenet_demo demo application demonstrates starting DSP core with DSP image. It demonstrates how the LeNet neural network can be accelerated using the DSP.


Toolchain supported
===================
- IAR embedded Workbench  8.50.9
- MCUXpresso  11.3.0
- Keil MDK  5.33
- GCC ARM Embedded  9.3.1

Hardware requirements
=====================
- Mini/micro USB cable
- JTAG/SWD
- evkmimxrt685 board
- Personal Computer

Prepare the Demo
================
0.  Before running the demo, a LeNet network demo should be trained.
The folder scripts contains:
- a script (export_lenet.py) which will train a LeNet model and extract the needed data for the demo
- a file (requirements.txt) which contains the requirements for the above python script

Install the requirements:
pip install -r requirements.txt

Run the script:
python export_lenet.py
This will pollute the working directory with these files:
data
    t10k-images-idx3-ubyte.gz
    t10k-labels-idx1-ubyte.gz
    train-images-idx3-ubyte.gz
    train-labels-idx1-ubyte.gz
model
    checkpoint
    lenet.data-00000-of-00001
    lenet.index
    lenet.meta
test_bin
    io
        input100.bin
        input1.bin
        output.bin
    weights
        conv1_b.bin
        conv1_W.bin
        conv2_b.bin
        conv2_W.bin
        fc1_b.bin
        fc1_W.bin
        fc2_b.bin
        fc2_W.bin
test_h
    conv1_b.h
    conv1_W.h
    conv2_b.h
    conv2_W.h
    fc1_b.h
    fc1_W.h
    fc2_b.h
    fc2_W.h
    input100.h
    input1.h
    output.h
The files from test_h are the ones needed to be used within the project. The project already have these files,
but the arrays are initialized with zeros. Replace the files with the same name from the project.
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program for CM33 core to the target board.
4.  Launch the debugger in your IDE to begin running the demo.
5.  If building debug configuration, start the xt-ocd daemon and download the program for DSP core to the target board.
6.  If building debug configuration, launch the Xtensa IDE or xt-gdb debugger to
begin running the demo.

NOTE: DSP image can only be debugged using J-Link debugger.  See
'Getting Started with Xplorer for EVK-MIMXRT685.pdf' for more information.

Running the demo CM33
=====================
When the demo runs successfully, the terminal will display the following:

Started LeNet demo and benchmark (100 iterations)
DSP Image copied to SRAM
NN started
GOOD:   99
BAD:    1
ACC:    0.990000
Avg Inference cycles: 578781 time: 2.314 ms
Throughput: 432.2 fps
