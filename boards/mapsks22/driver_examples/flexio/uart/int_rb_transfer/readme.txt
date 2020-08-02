Overview
========
The flexio_uart_interrupt_ring_buffer example shows how to use flexio uart driver in interrupt way with
RX ring buffer enabled:

In this example, a flexio simulated uart connect to PC through USB-Serial, the board will
send back all characters that PC send to the board.

Note: The example echo every 8 characters, so input 8 characters every time.

Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- MAPS-KS22F256 board & MAPS-DOCK board
- Personal Computer
- USB-to-Serial converter

Board settings
==============
The flexio_uart_interrupt_ring_buffer example is configured to use FLEXIO D0 and D1 with PTC12 and PTC13 pins.
PTC12: FLEXIO_UART_TX_PIN, CN9D-84
PTC13: FLEXIO_UART_RX_PIN, CN9D-85

To observe the execution log on serial port, PTC12 and PTC13 should connect to the USB-to-Serial connector plug on PC,
open the related COM port on PC to monitor the execution log.
PTC12(Pin-84)  <--> USB-to-Serial(RX end)
PTC13(Pin-85)  <--> USB-to-Serial(TX end)
MAPS-Board_GND <--> USB-to-Serial(GND end)

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
   the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~
FLEXIO UART RX ring buffer example
Send back received data
Echo every 8 bytes
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

