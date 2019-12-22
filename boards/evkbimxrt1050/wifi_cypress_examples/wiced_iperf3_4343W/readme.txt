Overview
========
The qca_iperf example provides basic commands to measure performance using qca_wifi stack.
Results will be shown on your iperf server.


Known Issues
If the iperf3 server shows you that you have 0-bit/s bandwidth, it may be caused due to the version you are using.
This client works with Iperf v3.6. In case of any more issues, see https://github.com/esnet/iperf/labels/bug. 


'R' mode might not work correctly for QCA WiFi.



Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- 2x Micro USB cable
- evkbimxrt1050 board
- Personal Computer
- Murata 1DX or 1LV M.2 module
- Murata uSD M.2 Adapter
- 1 plug to receptable header cable

Board settings
==============
It is recommended to provide power directly to Murata uSD M.2 Adapter board using secondary Micro USB cable.
Jumper J1 on the adapter board has to be set up accordingly to utilize external power.

The following pins between the evkbimxrt1050 board and Murata uSD M.2 Adapter are connected using the plug to receptable cables:
- evkbimxrt1050 board's connector J22, pin 3 to Murata uSD M.2 Adapter's connector J9, pin 3
Prepare the Demo
================

1.  Configure your dedicated device to create a WiFi network with
    SSID "nxp" and password "NXP0123456789"

    Install Iperf3 on your machine:
      1. Enter command "sudo apt install iperf3"

    Start the server by running "iperf3 -s"
2.  Connect a micro USB cable between the PC host and the SDA port
3.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.


Running the demo
================
When the demo runs successfully, the log would be seen on the CMSIS DAP terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Enter IP addr of a server in format '192.168.1.2'
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Enter ip address of the iperf3 server.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Using IP xxx.xxx.xxx.xxx
Menu:
Press 's' to start client Tx mode
Press 'r' to start client Rx mode
Press 'S' to start client Tx UDP mode
Press 'R' to start client Rx UDP mode
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Press 's' to start Iperf3 client in sending mode. (TCP)
- Press 'r' to start Iperf3 client in recieving mode. (TCP)
- Press 'S' to start Iperf3 client in sending mode. (UDP)
- Press 'R' to start Iperf3 client in recieving mode. (UDP)

Customization options
=====================

