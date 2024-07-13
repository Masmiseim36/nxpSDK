Overview
========
The iperf3 example provides basic commands to measure performance of network stack.
Results will be shown on your iperf server. In the case of 'R' mode, server shows the amount of sent data,
check the terminal output in this case for the total amount of payload in bytes received over UDP,
which marks the device's throughput.

Known Issues
If the iperf3 server shows you that you have 0-bit/s bandwidth, it may be caused due to the version you are using.
This client works with Iperf v3.7. In case of any more issues, see https://github.com/esnet/iperf/labels/bug. 

Rx UDP mode ('R') has limited bandwidth by UDP_RX_BANDWIDTH macro as sending UDP packets too fast may cause
the control TCP socket to lost data and test failure.
Set the macro to a little bit higher than the device can receive or "0" for no limit.

The network throughput is limited by UART interface between board and Wi-Fi module. Default UART speed is set to 115200 Bd.


SDK version
===========
- Version: 2.16.000

Toolchain supported
===================
- IAR embedded Workbench  9.60.1
- Keil MDK  5.39.0
- GCC ARM Embedded  13.2.1
- MCUXpresso  11.10.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1020 board
- Personal Computer
- Murata CMWC1ZZABR-107-EVB
- LPCXpresso Click Shield


Board settings
==============
Connect the LPCXpresso Click Shield to the board and then connect the Murata Wi-Fi module to mikroBUS slot 2.

Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
2. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
3. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.

Prepare the Demo
================
1.  Prepare iperf3 server on your machine:
    - Install iperf3 (Enter command "sudo apt install iperf3")
    - Start the server by running "iperf3 -s"
2.  Connect a micro USB cable between the PC host and the SDA port
3.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.
5.  During the runtime will be necessary to configure Wi-Fi module. Follow instructions in the terminal for Wi-Fi module configuration or resetting to provisioning mode.
    If the module is in provisioning mode, it will create micro-AP:
        SSID: Serial2Wifi
        Passphrase: nxp12345
    Connect your PC or mobile phone to this Wi-Fi micro-AP, open address: http://192.168.10.1 in web browser, select "Provisioning" and then select WLAN for connection.
    Wi-Fi module stores the WLAN credentials and it will use them automatically for next connection.

Running the demo
================
When the demo runs successfully, the log would be seen on the CMSIS DAP terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Enter IP address of a server in format '192.168.1.2'
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Enter IP address of the iperf3 server.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Using IP xxx.xxx.xxx.xxx
Menu:
Press 's' to start client Tx mode
Press 'r' to start client Rx mode
Press 'S' to start client Tx UDP mode
Press 'R' to start client Rx UDP mode
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Press 's' to start Iperf3 client in sending mode. (TCP)
- Press 'r' to start Iperf3 client in receiving mode. (TCP)
- Press 'S' to start Iperf3 client in sending mode. (UDP)
- Press 'R' to start Iperf3 client in receiving mode. (UDP)

