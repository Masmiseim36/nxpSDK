Overview
========

This is the IPerf example to check your bandwidth using the network performance measurement IPerf application on a PC as a client or a server.
It is currently a minimal implementation providing an IPv4 TCP only.

Instead of the command line IPerf application, for more convenience, it is recommended to use the JPerf2 graphical tool, which can be downloaded here: https://sourceforge.net/projects/iperf/files/jperf/jperf%202.0.0/jperf-2.0.0.zip/download
The example supports IPerf version 2.0.5

Running the demo
================
1. Adjust the IPERF_SERVER_ADDRESS definition to the IP address where the JPerf PC application will be accessible and rebuild it.
2. When the demo starts, the log would be seen on the terminal like:
Initializing WiFi Connection...
WLAN MAC Address : DC:EF:CA: 7:5E:84
WLAN Firmware    : wl0: Feb 12 2018 04:08:14 version 7.79.2 (r683798 CY) FWID 01-27b63357
WLAN CLM         : API: 12.2 Data: 9.10.39 Compiler: 1.29.4 ClmImport: 1.36.3 Creation: 2018-02-12 04:00:50
Successfully Initialized WiFi Connection
Scanning available networks...
...
Joining : nxp
Successfully joined : nxp
Getting IP address from DHCP server
 IPv4 Address got from DHCP  : 192.168.2.100

************************************************
 IPERF example
************************************************
Please select one of the following modes to run IPERF with:

    1: server mode (RX only test)
    2: client mode (TX only test)
    3: client dual mode (TX and RX in parallel)
    4: client tradeoff mode (TX and RX sequentially)

Enter mode number:

3. Start the JPerf application, using the jperf-2.0.0/jperf.bat batch file.
	It can be downloaded here: https://sourceforge.net/projects/iperf/files/jperf/jperf%202.0.0/jperf-2.0.0.zip/download.
4. To run lwIP IPERF in client mode, select "Server" radio button in JPerf and press the [Run iperf!] button.
5. To run lwIP IPERF in server mode, select "Client radio button and enter the IP address assigned to the board by DHCP
    as IPv4 address to the "server address" parameter in JPerf.
6. Enter the desired mode number into the terminal.
7. If server mode has been selected in the terminal (and client mode in JPerf), press the [Run iperf!] button now.
8. When the test is finished, the output log of JPerf would be seen like below,
	where occurrences of the symbol "N" would be replaced by actual measured values.
    The log will vary depending on the selected mode:
		bin/iperf.exe -s -P 0 -i 1 -p 5001 -f k
        ------------------------------------------------------------
        Server listening on TCP port 5001
        TCP window size: 85.3 KByte (default)
        ------------------------------------------------------------
        [  4] local 192.168.2.101 port 5001 connected with 192.168.2.100 port 49153
        [ ID] Interval       Transfer     Bandwidth
        [  4]  0.0- 1.0 sec   N   KBytes  N    Kbits/sec
        [  4]  1.0- 2.0 sec   N   KBytes  N    Kbits/sec
        [  4]  2.0- 3.0 sec   N   KBytes  N    Kbits/sec
        [  4]  3.0- 4.0 sec   N   KBytes  N    Kbits/sec
        [  4]  4.0- 5.0 sec   N   KBytes  N    Kbits/sec
        [  4]  5.0- 6.0 sec   N   KBytes  N    Kbits/sec
        [  4]  6.0- 7.0 sec   N   KBytes  N    Kbits/sec
        [  4]  7.0- 8.0 sec   N   KBytes  N    Kbits/sec
        [  4]  8.0- 9.0 sec   N   KBytes  N    Kbits/sec
        [  4]  9.0-10.0 sec   N   KBytes  N    Kbits/sec
        [  4]  0.0-10.0 sec   N   KBytes  N    Kbits/sec
        ------------------------------------------------------------
        Client connecting to 192.168.2.100, TCP port 5001
        TCP window size: 85.0 KByte (default)
        ------------------------------------------------------------
        [  4] local 192.168.2.101 port 40954 connected with 192.168.2.100 port 5001
        [  4]  0.0- 1.0 sec   N   KBytes  N    Kbits/sec
        [  4]  1.0- 2.0 sec   N   KBytes  N    Kbits/sec
        [  4]  2.0- 3.0 sec   N   KBytes  N    Kbits/sec
        [  4]  3.0- 4.0 sec   N   KBytes  N    Kbits/sec
        [  4]  4.0- 5.0 sec   N   KBytes  N    Kbits/sec
        [  4]  5.0- 6.0 sec   N   KBytes  N    Kbits/sec
        [  4]  6.0- 7.0 sec   N   KBytes  N    Kbits/sec
        [  4]  7.0- 8.0 sec   N   KBytes  N    Kbits/sec
        [  4]  8.0- 9.0 sec   N   KBytes  N    Kbits/sec
        [  4]  9.0-10.0 sec   N   KBytes  N    Kbits/sec
        [  4]  0.0-10.8 sec   N   KBytes  N    Kbits/sec
9. Also, when the test is finished, the log would be seen on the terminal like below,
	where occurrences of the symbol "N" would be replaced by actual measured values.
    The log will vary depending on the selected mode:
        Enter mode number: 4
        Minimum ever free heap size: 51152
        -------------------------------------------------
         TCP_DONE_CLIENT (TX)
         Local address : 192.168.2.100  Port 49153
         Remote address : 192.168.2.101  Port 5001
         Bytes Transferred N
         Duration (ms) N
         Bandwidth (kbitpsec) N
        -------------------------------------------------
         TCP_DONE_SERVER (RX)
         Local address : 192.168.2.100  Port 5001
         Remote address : 192.168.2.101  Port 40954
         Bytes Transferred N
         Duration (ms) N
         Bandwidth (kbitpsec) N
Hardware requirements
=====================
- Micro USB cable
- evkbimxrt1050 board
- Personal Computer
- Murata 1DX M.2 module
- Murata uSD M.2 Adapter

Board settings
==============
No special settings are required.


Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.1

