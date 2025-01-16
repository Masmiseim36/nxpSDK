Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-MIMXRT1050 board
- Personal Computer

Board settings
============
UART hardware flow control on one board.
The UART will send data to itself(loopback), and hardware flow control function will be 
enabled on this example. So, please make sure the correct connection for example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Pin Name    Board Location    connect to     Pin Name    Board Location
LPUART_TX   J22   Pin 2       ----------     LPUART_RX   J22   pin 1
LPUART_CTS  J23   Pin 3       ----------     LPUART_RTS  J23   Pin 4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This is LPUART hardware flow control example on one board.
This example will send data to itself and will use hardware flow control to avoid the overflow.
Please make sure you make the correct line connection. Basically, the connection is:
      LPUART_TX    --     LPUART_RX
      LPUART_RTS   --     LPUART_CTS
Data matched! Transfer successfully.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
