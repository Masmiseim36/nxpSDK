Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer

Board settings
============
Populate R469 and set reset pin as open-drain to complete the PHY hardware reset properly.

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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
NETC PSI VSI transfer from the Primary Core!

Copy Secondary core image to address: 0x303c0000, size: 22295

NETC Switch frame loopback example start.
Wait for PHY link up...
Starting Secondary core.
The secondary core application has been started.
VSI EP initialization succeeded.
 PSI sends message 0 to VSI
 PSI receives message from VSI to set PSI MAC address 54:27:8d:0:0:0.
 A frame received from VSI. The length is 1000. Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:aa:bb:cc.
 A frame received from PSI. The length is 1000. Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d: 0: 0: 0.
 PSI sends message 1 to VSI
 PSI receives message from VSI to set PSI MAC address 54:27:8d:0:0:1.
 A frame received from VSI. The length is 1000. Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:aa:bb:cc.
 A frame received from PSI. The length is 1000. Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d: 0: 0: 1.
 PSI sends message 2 to VSI
 PSI receives message from VSI to set PSI MAC address 54:27:8d:0:0:2.
 A frame received from VSI. The length is 1000. Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:aa:bb:cc.
 A frame received from PSI. The length is 1000. Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d: 0: 0: 2.

......

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~