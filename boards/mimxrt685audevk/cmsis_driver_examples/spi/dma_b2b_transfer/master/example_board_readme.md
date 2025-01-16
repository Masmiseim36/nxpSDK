Hardware requirements
=====================
- Micro USB cable
- MIMXRT685-AUD-EVK board
- Personal Computer

Board settings
============
Make sure the JP41 2-3 is connected. 

Connect SPI master on board to SPI slave on other board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Master_board(SPI5)           Slave_board(SPI5)                          
Pin Name   Board Location     Pin Name   Board Location                     
MISO       J28 pin 5          MISO       J28 pin 5
MOSI       J28 pin 4          MOSI       J28 pin 4
SCK        J28 pin 6          SCK        J28 pin 6
PCS0       J28 pin 3          PCS0       J28 pin 3
GND        J28 pin 7          GND        J28 pin 7
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SPI CMSIS driver board to board dma example.
This example use one board as master and another as slave.
Master and slave uses DMA way. Slave should start first. 
Please make sure you make the correct line connection. Basically, the connection is: 
SPI_master -- SPI_slave   
   CLK      --    CLK  
   PCS      --    PCS 
   MOSI     --    MOSI 
   MISO     --    MISO 
   GND      --    GND 

 Master transmit:

  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
 
SPI transfer all data matched! 

 Master received:

  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
