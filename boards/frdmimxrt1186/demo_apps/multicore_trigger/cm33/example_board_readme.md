Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT1186 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Use Secure Provisioning Tool to combine this demo image and CM7 demo image, and download them into flash memory
4.  Press the reset button on your board to begin running the demo.

Note:
    1. This demo targets to run with CM7 standalone demos(with same/similar linkage as hello_world_cm7 demo) in this SDK out of box.
    2. This demo plays role as a trigger to boot CM7 image after POR.
    3. Refer to "Getting Started..." document for more detail on how to use Secure Provisioning Tool on this demo.

Running the demo
================
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Multicore trigger demo!
Core0 get core1 image info sucessfully!
  Core1 image reside addr = 0xxxxxxxxx
  Core1 image dest addr   = 0xxxxxxxxx
  Core1 image size        = 0xxxxxxxxx
  Core1 image boot addr   = 0xxxxxxxxx
Core0 is starting core1...

Core1 demo output log...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
