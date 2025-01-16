Hardware requirements
===================
- Mini/micro USB cable
- Network cable RJ45 standard
- MIMXRT1170-EVKB board
- Personal Computer
- Optional: another network cable and another PC for simultaneous use of both ethernet interfaces.

Board settings
============
This example can use both 1G port(J4) and 100M port(J3). No macro definition changes needed.
The 100M port(J3) uses:
  Interface name: en0
  IPv4 address: 192.168.0.102
  Subnet mask: 255.255.255.0
  Gateway: 192.168.0.100
The 1G port(J4) uses:
  Interface name: en1
  IPv4 address: 192.168.1.103
  Subnet mask: 255.255.255.0
  Gateway: 192.168.1.200
You can check and modify these in the app.h file. Use these IP addresses accordingly when running the demo.
