Overview
========

This is the IPerf server example to check your bandwidth using the network performance measurement IPerf application on a PC as a client.
It is currently a minimal implementation providing an IPv4 TCP server only.

Instead of the command line IPerf application, for more convenience, it is recommended to use the JPerf2 graphical tool, which can be downloaded here: https://sourceforge.net/projects/iperf/files/jperf/jperf%202.0.0/jperf-2.0.0.zip/download

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA(or USB to Serial) USB port on the target board.
2.  Open a serial terminal on PC for OpenSDA serial(or USB to Serial) device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert the Ethernet Cable into the target board's RJ45 port and connect it to your PC network adapter.
4.  Configure the host PC IP address to 192.168.0.100.
5.  Open a web browser.
6.  Download the program to the target board.
7.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

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
5. When the test is finished, the output log of JPerf would be seen like below,
	where occurrences of the symbol "N" would be replaced by actual measured values:
		bin/iperf.exe -c 192.168.0.102 -P 1 -i 1 -p 5001 -f k -t 10
		------------------------------------------------------------
		Client connecting to 192.168.0.102, TCP port 5001
		TCP window size: 8.00 KByte (default)
		------------------------------------------------------------
		[132] local 192.168.0.100 port 2466 connected with 192.168.0.102 port 5001
		[ ID] Interval       Transfer     Bandwidth
		[132]  0.0- 1.0 sec  N    KBytes  N     Kbits/sec
		[132]  1.0- 2.0 sec  N    KBytes  N     Kbits/sec
		[132]  2.0- 3.0 sec  N    KBytes  N     Kbits/sec
		[132]  3.0- 4.0 sec  N    KBytes  N     Kbits/sec
		[132]  4.0- 5.0 sec  N    KBytes  N     Kbits/sec
		[132]  5.0- 6.0 sec  N    KBytes  N     Kbits/sec
		[132]  6.0- 7.0 sec  N    KBytes  N     Kbits/sec
		[132]  7.0- 8.0 sec  N    KBytes  N     Kbits/sec
		[132]  8.0- 9.0 sec  N    KBytes  N     Kbits/sec
		[132]  9.0-10.0 sec  N    KBytes  N     Kbits/sec
		[132]  0.0-10.0 sec  N    KBytes  N     Kbits/sec
		Done.
6. Also, when the test is finished, the log would be seen on the terminal like below,
	where occurrences of the symbol "N" would be replaced by actual measured values:
		-------------------------------------------------
		 TCP_DONE_SERVER
		 Local address : 192.168.0.102  Port 5001
		 Remote address : 192.168.0.100  Port 2466
		 Bytes Transferred N
		 Duration (ms) N
		 Bandwidth (kbitpsec) N

Hardware requirements
=====================
- Mini/micro USB cable
- Network cable RJ45 standard
- EVKB-IMXRT1050 board
- Personal Computer

Board settings
==============
No special settings are required.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

