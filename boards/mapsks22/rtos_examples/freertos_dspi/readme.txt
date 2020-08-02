Overview
========
The freertos_dspi example shows how to use DSPI driver in FreeRTOS:

In this example , one dspi instance used as DSPI master with blocking and another dspi instance used as DSPI slave .

1. DSPI master sends/receives data using task blocking calls to/from DSPI slave. (DSPI Slave uses interrupt to receive/
send the data)


Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- J-Link ARM
- Mini USB cable
- MAPS-KS22F256 board & MAPS-DOCK board
- Personal Computer

Board settings
==============
The dspi_interrupt example is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial 
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 related jumpers connected

To observe the example result, please connect the pins of CN2 and CN13 on MAPS-KS22F256 board as follows:

        SPI0 (master)              SPI1 (slave)
  PCS0: PTC4 (Pin-76, CN2:29) <--> PTD4 (Pin-97  CN13:1)
  SCK:  PTC5 (Pin-77, CN2:27) <--> PTD5 (Pin-98  CN13:3)
  MOSI: PTD2 (Pin-95, CN2:28) <--> PTD7 (Pin-100 CN13:29)
  MISO: PTD3 (Pin-96, CN2:30) <--> PTD6 (Pin-99  CN13:30)

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the USB port (CN14) on MAPS-DOCK board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo. For detailed instructions, see Getting Started with Software Development Kit for 
   KS22F25612 (document SDKKS22F256GSUG).

Running the demo
================


Customization options
=====================

