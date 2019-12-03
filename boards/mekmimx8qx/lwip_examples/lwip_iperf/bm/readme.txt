Overview
========

This is the IPerf server example to check your bandwidth using the network performance measurement IPerf application on a PC as a client.
It is currently a minimal implementation providing an IPv4 TCP server only.

Instead of the command line IPerf application, for more convenience, it is recommended to use the JPerf2 graphical tool, which can be downloaded here: https://sourceforge.net/projects/iperf/files/jperf/jperf%202.0.0/jperf-2.0.0.zip/download


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- GCC ARM Embedded  7.3.1

Hardware requirements
=====================
- Micro USB cable
- MIMX8QX MEK board
- J-Link Debug Probe
- 12V power supply
- Network cable RJ45 standard
- Personal Computer

Board settings
==============
No special setting needed.

#### Please note this application can't support running with Linux BSP! ####
This example aims to show the basic usage of the IP's function, some of the used Pads/Resources are also used by Cortex-A core.

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch SW3 to power on the board.
2.  Connect a micro USB cable between the host PC and the J11 USB port on the cpu board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Insert the Ethernet Cable into the target board's RJ45 port and connect it to your PC network adapter.
5.  Download the program to the target board.
6.  Configure the host PC IP address to 192.168.0.100.
7.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
1. When the demo starts, the log would be seen on the terminal like:
		************************************************
		IPERF Server example
		************************************************
		IPv4 Address     : 192.168.0.102
		IPv4 Subnet mask : 255.255.255.0
		IPv4 Gateway     : 192.168.0.100
		************************************************
2. Start the JPerf application, using the jperf-2.0.0/jperf.bat batch file.
	It can be downloaded here: https://sourceforge.net/projects/iperf/files/jperf/jperf%202.0.0/jperf-2.0.0.zip/download.
3. Enter the 192.168.0.102 board IPv4 address to the "server address" parameter in JPerf.
4. Press the [Run iperf!] button.
5. When the test is finished, the output log of JPerf would be seen like:
		bin/iperf.exe -c 192.168.0.102 -P 1 -i 1 -p 5001 -f k -t 10
		------------------------------------------------------------
		Client connecting to 192.168.0.102, TCP port 5001
		TCP window size: 8.00 KByte (default)
		------------------------------------------------------------
		[132] local 192.168.0.100 port 2466 connected with 192.168.0.102 port 5001
		[ ID] Interval       Transfer     Bandwidth
		[132]  0.0- 1.0 sec  5000 KBytes  40960 Kbits/sec
		[132]  1.0- 2.0 sec  3544 KBytes  29032 Kbits/sec
		[132]  2.0- 3.0 sec  3544 KBytes  29032 Kbits/sec
		[132]  3.0- 4.0 sec  3576 KBytes  29295 Kbits/sec
		[132]  4.0- 5.0 sec  3584 KBytes  29360 Kbits/sec
		[132]  5.0- 6.0 sec  3568 KBytes  29229 Kbits/sec
		[132]  6.0- 7.0 sec  3592 KBytes  29426 Kbits/sec
		[132]  7.0- 8.0 sec  3584 KBytes  29360 Kbits/sec
		[132]  8.0- 9.0 sec  3520 KBytes  28836 Kbits/sec
		[132]  9.0-10.0 sec  3584 KBytes  29360 Kbits/sec
		[132]  0.0-10.0 sec  37104 KBytes  30308 Kbits/sec
		Done.
6. Also, when the test is finished, the log would be seen on the terminal like:
		-------------------------------------------------
		 TCP_DONE_SERVER
		 Local address : 192.168.0.102  Port 5001
		 Remote address : 192.168.0.100  Port 2466
		 Bytes Transferred 37994520
		 Duration (ms) 10063
		 Bandwidth (kbitpsec) 30200
Customization options
=====================

