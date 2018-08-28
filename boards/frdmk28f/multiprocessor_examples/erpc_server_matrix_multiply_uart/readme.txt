Overview
========
This example demonstrates usage of eRPC between PC and board using UART transport layer.
Board acts like a server and the PC as client. When client starts, it generates two random
matrixes and sends them to server. Server then performs matrix multiplication and sends
result data back to client. Result matrix is then printed on the PC side.

eRPC documentation
eRPC specific files are stored in: middleware\multicore_<version>\erpc
eRPC documentation is stored in: middleware\multicore_<version>\erpc\doc
eRPC is open-source project stored on github: https://github.com/EmbeddedRPC/erpc
eRPC documentation can be also found in: http://embeddedrpc.github.io

PC Side Setup (Python)
1. Make sure you have Python installed on your PC
2. Install eRPC module to Python by executing setup.py located in: middleware\multicore\erpc\erpc_python
3. Install serial module by executing following command in command line: "python -m pip install pyserial"

Usage of run_uart.py
usage: run_uart.py [-h] [-c] [-s] [-p PORT] [-b BD]

eRPC Matrix Multiply example

optional arguments:
  -h, --help            show this help message an
  -c, --client          Run client
  -s, --server          Run server
  -p PORT, --port PORT  Serial port
  -b BD, --bd BD        Baud rate (default value is 115200)

Either server or client has to be selected to run

Example:
To run PC side as a client with a board connected as a server to COM3 execute:
"run_uart.py --client --port COM3"

Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini USB cable
- FRDM-K28F board
- Another FRDM-K28F board or any other supported board to comunicate with
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the USB port on the target board.
2.  Download the erpc_server_matrix_multiply_uart example to the target board.
3.  Start server first.
4.  Then start client on PC to begin running the demo.

For detailed instructions, see the appropriate board User's Guide.

Running the demo
================
When the demo runs successfully, the log below shows the output of the Client eRPC Matrix Multiply demo 
in the Phyton Shell window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Matrix #1
0022 0020 0024 0031 0024
0004 0016 0036 0032 0016
0011 0038 0011 0025 0038
0049 0047 0000 0046 0047
0004 0019 0035 0011 0026

Matrix #2
0021 0019 0024 0039 0031
0044 0024 0011 0048 0013
0030 0003 0039 0038 0033
0042 0018 0002 0040 0043
0039 0048 0027 0044 0004

eRPC request is sent to the server

Result matrix
4300 2680 2394 5026 3163
3836 1912 2172 4276 2960
4765 3428 2187 5343 2425
6862 5143 3054 8075 4296
3446 2083 2394 3982 2103

Press Enter to initiate the next matrix multiplication
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

