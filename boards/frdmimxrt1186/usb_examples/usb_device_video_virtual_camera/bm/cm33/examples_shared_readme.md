This file contains the general information which can be shared by the examples on this board.
The example specific information can be found in *readme.md* or *example_board_readme.md*.

General Example Settings
========================
Hardware requirements
---------------------
- Mini/micro USB cable
- FRDM-IMXRT1186 board
- Personal Computer

Prepare the Demo
---------------
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Note on ELE Ping
=====================
RT118x ELE requires ping every 24 hours, which is mandatory, otherwise soc may reset.
Note:
1.  This is generic rule for all RT118x demos.
2.  Most of RT118x demos don't ping ELE every 24 hours, that is because those demos focus on the function demonstrate only.
    It is still MUST to ping ELE every 24 hours if demo run duration > 24 hours.
3.  Please refer to hello_world as an example.