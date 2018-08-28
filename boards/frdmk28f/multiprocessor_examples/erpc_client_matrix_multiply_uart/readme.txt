Overview
========
This example demonstrates usage of eRPC between PC and board using UART transport layer.
Board acts like a client and the PC as server. When client starts, it generates two random
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
To run PC side as a server with a board connected as a client to COM3 execute:
"run_uart.py --server --port COM3"

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
2.  Download the erpc_client_matrix_multiply_uart example to the target board.
3.  Start server on PC first.
4.  Then start execution of client example loaded in board to begin running the demo.

For detailed instructions, see the appropriate board User's Guide.

Running the demo
================
When the demo runs successfully, the log below shows the output of the Client eRPC Matrix Multiply demo:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Matrix #1
0021 0033 0037 0037 0009
0023 0045 0043 0000 0032
0038 0044 0008 0015 0036
0018 0018 0038 0044 0016
0022 0023 0000 0038 0007

Matrix #2
0011 0023 0027 0045 0011
0007 0019 0023 0024 0006
0032 0026 0049 0043 0016
0022 0048 0036 0034 0041
0027 0020 0032 0031 0011

eRPC request is sent to the server

Result matrix
2703 4028 4759 4865 2637
2808 3142 4787 4956 1563
2284 3358 4122 4736 1821
2940 4176 4858 4868 2894
1428 2907 2715 3051 2015

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

