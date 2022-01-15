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
- EVK-MIMXRT1060 board
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
 wolfSSL version 4.8.1
------------------------------------------------------------------------------
wolfCrypt Benchmark (block bytes 1024, min 1.0 sec each)
RNG                100 KB took 1.166 seconds,   85.763 KB/s
AES-128-CBC-enc     67 MB took 1.000 seconds,   67.310 MB/s
AES-128-CBC-dec     67 MB took 1.000 seconds,   67.261 MB/s
AES-192-CBC-enc      8 MB took 1.000 seconds,    7.861 MB/s
AES-192-CBC-dec      8 MB took 1.000 seconds,    7.935 MB/s
AES-256-CBC-enc      7 MB took 1.001 seconds,    7.122 MB/s
AES-256-CBC-dec      7 MB took 1.000 seconds,    7.129 MB/s
AES-128-GCM-enc      3 MB took 1.001 seconds,    3.171 MB/s
AES-128-GCM-dec      3 MB took 1.001 seconds,    3.171 MB/s
AES-192-GCM-enc      2 MB took 1.005 seconds,    2.332 MB/s
AES-192-GCM-dec      2 MB took 1.005 seconds,    2.332 MB/s
AES-256-GCM-enc      2 MB took 1.010 seconds,    2.224 MB/s
AES-256-GCM-dec      2 MB took 1.010 seconds,    2.224 MB/s
GMAC Default         4 MB took 1.000 seconds,    4.082 MB/s
AES-128-ECB-enc     14 MB took 1.000 seconds,   14.049 MB/s
AES-128-ECB-dec     14 MB took 1.000 seconds,   14.052 MB/s
AES-192-ECB-enc      8 MB took 1.000 seconds,    8.198 MB/s
AES-192-ECB-dec      8 MB took 1.000 seconds,    8.272 MB/s
AES-256-ECB-enc      7 MB took 1.000 seconds,    7.426 MB/s
AES-256-ECB-dec      7 MB took 1.000 seconds,    7.349 MB/s
CHACHA              21 MB took 1.000 seconds,   21.411 MB/s
MD5                 66 MB took 1.000 seconds,   66.016 MB/s
SHA                 92 MB took 1.000 seconds,   91.528 MB/s
SHA-256            109 MB took 1.000 seconds,  108.862 MB/s
SHA-512              8 MB took 1.002 seconds,    7.797 MB/s
SHA3-224             6 MB took 1.000 seconds,    6.372 MB/s
SHA3-256             6 MB took 1.002 seconds,    6.018 MB/s
SHA3-384             5 MB took 1.001 seconds,    4.634 MB/s
SHA3-512             3 MB took 1.002 seconds,    3.241 MB/s
HMAC-MD5            51 MB took 1.000 seconds,   50.781 MB/s
HMAC-SHA            87 MB took 1.000 seconds,   87.451 MB/s
HMAC-SHA256        103 MB took 1.000 seconds,  103.052 MB/s
HMAC-SHA512          7 MB took 1.002 seconds,    7.456 MB/s
PBKDF2               5 KB took 1.004 seconds,    5.291 KB/s
RSA     2048 public        222 ops took 1.006 sec, avg 4.532 ms, 220.676 ops/sec
RSA     2048 private         4 ops took 1.327 sec, avg 331.750 ms, 3.014 ops/sec
ECC   [      SECP256R1]   256 key gen       637 ops took 1.000 sec, avg 1.570 ms, 637.000 ops/sec
ECDHE [      SECP256R1]   256 agree         286 ops took 1.002 sec, avg 3.503 ms, 285.429 ops/sec
ECDSA [      SECP256R1]   256 sign          414 ops took 1.003 sec, avg 2.423 ms, 412.762 ops/sec
ECDSA [      SECP256R1]   256 verify        196 ops took 1.000 sec, avg 5.102 ms, 196.000 ops/sec
ED     25519 key gen        21 ops took 1.012 sec, avg 48.190 ms, 20.751 ops/sec
ED     25519 sign           22 ops took 1.077 sec, avg 48.955 ms, 20.427 ops/sec
ED     25519 verify         10 ops took 1.013 sec, avg 101.300 ms, 9.872 ops/sec
Benchmark complete
