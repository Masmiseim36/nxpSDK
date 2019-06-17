Overview
========

This is the IPerf example to check your bandwidth using the network performance measurement IPerf application on a PC as a client or a server.
It is currently a minimal implementation providing an IPv4 TCP only.

Instead of the command line IPerf application, for more convenience, it is recommended to use the JPerf2 graphical tool, which can be downloaded here: https://sourceforge.net/projects/iperf/files/jperf/jperf%202.0.0/jperf-2.0.0.zip/download

To experiment with the receive throughput, try to increase the value of TCP_WND in the file lwipopts.h and make sure
that the PBUF_POOL_SIZE is larger than (TCP_WND / TCP_MSS). Increase the PBUF_POOL_SIZE if necessary.
For RTOS applications, DEFAULT_THREAD_PRIO and TCPIP_THREAD_PRIO values can have effect on maximum throughput as well.


Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- GCC ARM Embedded  8.2.1
- MCUXpresso  11.0.0

Hardware requirements
=====================
- Mini/micro USB cable
- Network cable RJ45 standard
- EVK-MIMXRT1060 board
- Personal Computer

Board settings
==============
No special settings are required.

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
		Initializing PHY...
        
        ************************************************
         IPERF example
        ************************************************
         IPv4 Address     : 192.168.0.102
         IPv4 Subnet mask : 255.255.255.0
         IPv4 Gateway     : 192.168.0.100
        ************************************************
        Please select one of the following modes to run IPERF with:

            1: server mode (RX only test)
            2: client mode (TX only test)
            3: client dual mode (TX and RX in parallel)
            4: client tradeoff mode (TX and RX sequentially)

        Enter mode number:
2. Start the JPerf application, using the jperf-2.0.0/jperf.bat batch file.
	It can be downloaded here: https://sourceforge.net/projects/iperf/files/jperf/jperf%202.0.0/jperf-2.0.0.zip/download.
3. To run lwIP IPERF in client mode, select "Server" radio button in JPerf and press the [Run iperf!] button.
4. To run lwIP IPERF in server mode, select "Client radio button and enter the 192.168.0.102 board IPv4 address
    to the "Server address" parameter in JPerf.
5. Enter the desired mode number into the terminal.
6. If server mode has been selected in the terminal (and client mode in JPerf), press the [Run iperf!] button now.
7. When the test is finished, the output log of JPerf would be seen like below,
	where occurrences of the symbol "N" would be replaced by actual measured values.
    The log will vary depending on the selected mode:
        bin/iperf.exe -s -P 0 -i 1 -p 5001 -f k
        ------------------------------------------------------------
        Server listening on TCP port 5001
        TCP window size: 64.0 KByte (default)
        ------------------------------------------------------------
        OpenSCManager failed - Access is denied. (0x5)
        [404] local 192.168.0.100 port 5001 connected with 192.168.0.102 port 49153
        ------------------------------------------------------------
        Client connecting to 192.168.0.102, TCP port 5001
        TCP window size: 64.0 KByte (default)
        ------------------------------------------------------------
        [452] local 192.168.0.100 port 58998 connected with 192.168.0.102 port 5001
        [ ID] Interval       Transfer     Bandwidth
        [404]  0.0- 1.0 sec  N    KBytes  N     Kbits/sec
        [452]  0.0- 1.0 sec  N    KBytes  N     Kbits/sec
        [404]  1.0- 2.0 sec  N    KBytes  N     Kbits/sec
        [452]  1.0- 2.0 sec  N    KBytes  N     Kbits/sec
        [404]  2.0- 3.0 sec  N    KBytes  N     Kbits/sec
        [452]  2.0- 3.0 sec  N    KBytes  N     Kbits/sec
        [404]  3.0- 4.0 sec  N    KBytes  N     Kbits/sec
        [452]  3.0- 4.0 sec  N    KBytes  N     Kbits/sec
        [404]  4.0- 5.0 sec  N    KBytes  N     Kbits/sec
        [452]  4.0- 5.0 sec  N    KBytes  N     Kbits/sec
        [404]  5.0- 6.0 sec  N    KBytes  N     Kbits/sec
        [452]  5.0- 6.0 sec  N    KBytes  N     Kbits/sec
        [404]  6.0- 7.0 sec  N    KBytes  N     Kbits/sec
        [452]  6.0- 7.0 sec  N    KBytes  N     Kbits/sec
        [404]  7.0- 8.0 sec  N    KBytes  N     Kbits/sec
        [452]  7.0- 8.0 sec  N    KBytes  N     Kbits/sec
        [404]  8.0- 9.0 sec  N    KBytes  N     Kbits/sec
        [452]  8.0- 9.0 sec  N    KBytes  N     Kbits/sec
        [404]  9.0-10.0 sec  N    KBytes  N     Kbits/sec
        [404]  0.0-10.0 sec  N    KBytes  N     Kbits/sec
        [ ID] Interval       Transfer     Bandwidth
        [452]  9.0-10.0 sec  N    KBytes  N     Kbits/sec
        [452]  0.0-10.0 sec  N    KBytes  N     Kbits/sec
        Done.

8. Also, when the test is finished, the log would be seen on the terminal like below,
	where occurrences of the symbol "N" would be replaced by actual measured values.
    The log will vary depending on the selected mode:
        Enter mode number: 3
        -------------------------------------------------
         TCP_DONE_CLIENT (TX)
         Local address : 192.168.0.102  Port 49153
         Remote address : 192.168.0.100  Port 5001
         Bytes Transferred N
         Duration (ms) N
         Bandwidth (kbitpsec) N
        -------------------------------------------------
         TCP_DONE_SERVER (RX)
         Local address : 192.168.0.102  Port 5001
         Remote address : 192.168.0.100  Port 58998
         Bytes Transferred N
         Duration (ms) N
         Bandwidth (kbitpsec) N
Customization options
=====================

