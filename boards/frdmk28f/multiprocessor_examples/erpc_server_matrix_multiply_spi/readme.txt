Overview
========
This example demonstrates usage of eRPC between two boards using SPI transport layer.
One board acts like a server and the second as client. When client starts, it generates two random
matrixes and sends them to server. Server then performs matrix multiplication and sends
result data back to client. Client then prints the result matrix.

eRPC documentation
eRPC specific files are stored in: middleware\multicore\erpc
eRPC documentation is stored in: middleware\multicore\erpc\doc
eRPC is open-source project stored on github: https://github.com/EmbeddedRPC/erpc
eRPC documentation can be also found in: http://embeddedrpc.github.io
Hardware requirements
=====================
- Mini USB cable
- FRDM-K28F board
- Another FRDM-K28F board or any other supported board to communicate with
- Personal Computer

Board settings
==============
Connect SPI signals on client board to the SPI signals on server board. Basicaly, the connection is:
   Client      |     Server
---------------------------------
   SOUT(MOSI)  --    SIN(MOSI)
   SIN(MISO)   --    SOUT(MISO)
   SCK(CLK)    --    SCK(CLK)
   PCS         --    PCS
   SLAVE_READY --    SLAVE_READY
   GND         --    GND

SPI signals for FRDM-K28F board:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI0)
Pin Name      Board Location
SIN           J27 pin 20(PTA17)
SOUT          J27 pin 19(PTA16)
SCK           J27 pin 18(PTA15)
PCS0          J27 pin 17(PTA14)
SLAVE_READY   J1 pin 8(PTC11)
GND           J27 pin 2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect all SPI signals, SLAVE_READY and GND between client and server board.
2.  Connect a USB cable between the host PC and the USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download server program to first target board.
5.  Download client program to the second target board.
6.  Start server first.
7.  Start client to begin running the demo.

For detailed instructions, see the appropriate board User's Guide.

Running the demo
================
The log below shows the output of the Client eRPC Matrix Multiply demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Matrix #1
  21   33   37   37    9
  23   45   43    0   32
  38   44    8   15   36
  18   18   38   44   16
  22   23    0   38    7

Matrix #2
  11   23   27   45   11
   7   19   23   24    6
  32   26   49   43   16
  22   48   36   34   41
  27   20   32   31   11

eRPC request is sent to the server

Result matrix
2703 4028 4759 4865 2637
2808 3142 4787 4956 1563
2284 3358 4122 4736 1821
2940 4176 4858 4868 2894
1428 2907 2715 3051 2015

Press the button to initiate the next matrix multiplication
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- MCUXpresso 10.3.0
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1

