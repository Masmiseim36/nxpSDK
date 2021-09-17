Overview
========
The WolfSSL benchmark program performs cryptographic algorithm prints results to the
terminal.


Toolchain supported
===================
- IAR embedded Workbench  9.10.2
- GCC ARM Embedded  10.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-MIMXRT1060 board
- Personal Computer

Board settings
==============
No special settings are required.
Prepare the Demo
================
Connect a serial cable from the debug UART port of the target board to the PC. Start TeraTerm
(http://ttssh2.osdn.jp) and make a connection to the virtual serial port.

1. Start TeraTerm.

2. New connection -> Serial.

3. Set the appropriate COMx port (where x is port number) in port context menu. The number is provided by the operating
   system, and could be different from computer to computer. Select the COM number related to the virtual
   serial port. Confirm the selected port by clicking the "OK" button.

4. Set following connection parameters in menu Setup -> Serial port.
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: none

5.  Confirm the selected parameters by clicking the "OK" button.

Running the demo
================
When the demo runs successfully, the terminal will display similar information like the following:

wolfCrypt Benchmark (block bytes 1024, min 1.0 sec each)
------------------------------------------------------------------------------
 wolfSSL version 4.6.0
------------------------------------------------------------------------------
wolfCrypt Benchmark (block bytes 1024, min 1.0 sec each)
RNG                100 KB took 1.167 seconds,   85.690 KB/s
AES-128-CBC-enc     66 MB took 1.001 seconds,   65.925 MB/s
AES-128-CBC-dec     66 MB took 1.000 seconds,   65.942 MB/s
AES-192-CBC-enc      5 MB took 1.004 seconds,    4.620 MB/s
AES-192-CBC-dec      5 MB took 1.001 seconds,    4.585 MB/s
AES-256-CBC-enc      4 MB took 1.005 seconds,    4.203 MB/s
AES-256-CBC-dec      4 MB took 1.002 seconds,    4.191 MB/s
AES-128-GCM-enc      2 MB took 1.010 seconds,    2.441 MB/s
AES-128-GCM-dec      2 MB took 1.003 seconds,    2.434 MB/s
AES-192-GCM-enc      2 MB took 1.012 seconds,    1.640 MB/s
AES-192-GCM-dec      2 MB took 1.013 seconds,    1.639 MB/s
AES-256-GCM-enc      2 MB took 1.000 seconds,    1.563 MB/s
AES-256-GCM-dec      2 MB took 1.015 seconds,    1.563 MB/s
AES-128-ECB-enc      8 MB took 1.000 seconds,    7.798 MB/s
AES-128-ECB-dec     12 MB took 1.000 seconds,   11.961 MB/s
AES-192-ECB-enc      4 MB took 1.001 seconds,    3.664 MB/s
AES-192-ECB-dec      4 MB took 1.000 seconds,    3.781 MB/s
AES-256-ECB-enc      3 MB took 1.000 seconds,    3.420 MB/s
AES-256-ECB-dec      4 MB took 1.000 seconds,    3.617 MB/s
CHACHA               5 MB took 1.000 seconds,    5.420 MB/s
MD5                 34 MB took 1.000 seconds,   33.594 MB/s
SHA                 87 MB took 1.000 seconds,   86.768 MB/s
SHA-256            102 MB took 1.000 seconds,  102.319 MB/s
SHA-512              1 MB took 1.000 seconds,    1.440 MB/s
SHA3-224             7 MB took 1.002 seconds,    6.749 MB/s
SHA3-256             6 MB took 1.001 seconds,    6.366 MB/s
SHA3-384             5 MB took 1.000 seconds,    4.907 MB/s
SHA3-512             3 MB took 1.003 seconds,    3.432 MB/s
HMAC-MD5            33 MB took 1.000 seconds,   33.228 MB/s
HMAC-SHA            85 MB took 1.000 seconds,   84.668 MB/s
HMAC-SHA256         99 MB took 1.000 seconds,   99.487 MB/s
HMAC-SHA512          1 MB took 1.011 seconds,    1.401 MB/s
PBKDF2               5 KB took 1.001 seconds,    4.527 KB/s
RSA     2048 public        222 ops took 1.008 sec, avg 4.541 ms, 220.238 ops/sec
RSA     2048 private         4 ops took 1.327 sec, avg 331.750 ms, 3.014 ops/sec
ECC      256 key gen       601 ops took 1.001 sec, avg 1.666 ms, 600.400 ops/sec
ECDHE    256 agree         272 ops took 1.002 sec, avg 3.684 ms, 271.457 ops/sec
ECDSA    256 sign          404 ops took 1.000 sec, avg 2.475 ms, 404.000 ops/sec
ECDSA    256 verify        194 ops took 1.000 sec, avg 5.155 ms, 194.000 ops/sec
ED     25519 key gen        14 ops took 1.001 sec, avg 71.500 ms, 13.986 ops/sec
ED     25519 sign           14 ops took 1.025 sec, avg 73.214 ms, 13.659 ops/sec
ED     25519 verify          8 ops took 1.204 sec, avg 150.500 ms, 6.645 ops/sec
Benchmark complete
