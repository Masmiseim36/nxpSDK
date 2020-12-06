Overview
========
This is the IPerf example to check your bandwidth using the network performance measurement IPerf application on a PC as a client or a server.
IPv4 is implemented. The UDP implementation is based on lwIP community experimental patches, therefore some issues could be experienced.
UDP sending rate is calculated from the system time, which has a resolution of 1 ms in lwIP. Therefore the actual sending rate could be
a little lower or higher due to sending a calculated (rounded) number of frames each one or more milliseconds. It can also result in higher jitter.
The desired UDP sending rate is determined by the IPERF_UDP_CLIENT_RATE definition. If you want to change the rate without the need to compile
the application with a new value of IPERF_UDP_CLIENT_RATE, you can run the application as a UDP server instead and control the sending rate
by using the tradeoff mode from the PC application and determining the rate there.
For client modes, the address of the remote end where the server PC application is running is determined by the IPERF_SERVER_ADDRESS definition.

Instead of the command line IPerf application, for more convenience, it is recommended to use the JPerf2 graphical tool, which can be downloaded here: https://sourceforge.net/projects/iperf/files/jperf/jperf%202.0.0/jperf-2.0.0.zip/download
The example supports IPerf version 2.0.5. JPerf2, downloaded from the link above, contains version 1.7.0 of iperf.exe for Windows however.
Therefore the iperf.exe version has to be updated when using MS Windows. IPerf 2.0.5b for Windows can be downloaded from the following link:
https://iperf.fr/download/windows/iperf-2.0.5b-win32.zip
The contents of the downloaded archive have to be unpacked into jperf-2.0.0/bin folder, overwriting iperf.exe.

To experiment with the receive throughput, try to increase the value of TCP_WND in the file lwipopts.h and make sure
that the PBUF_POOL_SIZE is larger than (TCP_WND / TCP_MSS). Increase the PBUF_POOL_SIZE if necessary.
For RTOS applications, DEFAULT_THREAD_PRIO and TCPIP_THREAD_PRIO values can have effect on maximum throughput as well.

The demo is able to connect as a WiFi client to your local WiFi network or act as an AP so that it is possible to connect to it with you computer or smartphone.
The connection parameteres are defined by macros WIFI_SSID, WIFI_PASSWORD and WIFI_SECURITY.
Connection from a smartphone with Android OS was tested with 'Magic iPerf' application available in the Play store: https://play.google.com/store/apps/details?id=com.nextdoordeveloper.miperf.miperf

By default the example connects to network SSID "nxp-iperf" with "NXP0123456789" key.



Toolchain supported
===================
- MCUXpresso  11.2.0
- GCC ARM Embedded  9.2.1

Hardware requirements
=====================
- USB-C Cable
- SLN-ALEXA-IOT Development Kit
- Personal Computer
- SEGGER J-Link

Board settings
==============
Ensure J27 is set to position '1' (and is not set to serial downloader mode)

Prepare the Demo
================
1. Make sure that a valid 'bootstrap' and 'bootloader' are loaded onto target board.
2. Connect a USB cable between the host PC and the USB-C port on the target board. 
3. Connect SEGGER J-Link to the 10-pin SWD connector on bottom of target board.
4. Download the program to the target board.
5. Either power cycle the board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
Customization options
=====================

