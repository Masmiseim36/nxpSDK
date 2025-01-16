Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
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
3.  Compile the CM7 then the CM33 project to get the binary trdc_cm33.bin.
4.  Follow the instruction of SPSDK usage in SDK, to download and run the multicore image combined with edgelock FW.

Running the demo
================
The log below shows the output of the trdc multicore demo in the terminal window, if the CM33 uses TRDC2 and CM7 uses TRDC1 by default:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TRDC example starts on primary core
In primary core example we use 1 domain for secure access only.
Set the selected MBC block to non-secure for domain that is secure access only
Core0 MBC violent access at address: 0x47400000
The MBC selected block is accessiable for secure master now
Set the selected MRC region to non-secure for domain that is secure access only
Core0 MRC violent access at address: 0x 4100000
The MRC selected region is accessiable for secure master now
TRDC example succeeds on primary core
Copy Secondary core image to address: 0x303c0000, size: 16208
Starting Secondary core.
The secondary core application has been started.

In secondary core demo we use 2 domains, one is for secure access only, and the other is for non-secure access only.
Secondary core secure access demo starts.
Set the selected MBC block to non-secure for domain that is secure access only
Core1 MBC violent access at address: 0x42000000
The MBC selected block is accessiable for secure master now
Set the selected MRC region to non-secure for domain that is secure access only
Core1 MRC violent access at address: 0x60000000
The MRC selected region is accessiable for secure master now
Secondary core non-secure access demo starts.
Set the selected MBC block to non-secure for domain that is secure access only
Set the selected MRC region to non-secure for domain that is secure access only
Core1 MBC violent access at address: 0x42000000
The MBC selected block is accessiable for non-secure master now
Core1 MRC violent access at address: 0x60000000
The MRC selected region is accessiable for non-secure master now
TRDC example succeeds on secondary core

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Note:
The "Copy Secondary core image to address..." log message is not displayed on the terminal window when MCUXpresso IDE is used.
In case of MCUXpresso IDE the secondary core image is copied to the target memory during startup automatically.
